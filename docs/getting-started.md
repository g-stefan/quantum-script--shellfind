# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...) and `quantum-script--console`
must be installed to the SDK first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

Two libraries are produced:

| Project                            | Kind                                | Use it when                                         |
|------------------------------------|-------------------------------------|-----------------------------------------------------|
| `quantum-script--shellfind`        | DLL / shared library (`dll-or-lib`) | scripts run by `quantum-script`, or a host using the engine DLL |
| `quantum-script--shellfind.static` | static library, static CRT          | self-contained hosts built with `quantum-script.static` |

After `fabricare install`, `quantum-script--shellfind.dll` (Windows) /
`libquantum-script--shellfind.so` (Linux) sits in the SDK `bin` folder next
to `quantum-script.exe`, which is where
`Script.requireExtension("ShellFind")` finds it.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("ShellFind");

var files = 0;
var folders = 0;
var scan = new ShellFind();
for (scan.find("source/*"); scan.isValid(); scan.next()) {
	if (scan.isDirectory()) {
		if ((scan.name() == ".") || (scan.name() == "..")) {
			continue;
		};
		++folders;
		continue;
	};
	++files;
	Console.writeLn("source/" + scan.name() + (scan.isReadOnly() ? " (read-only)" : ""));
};
Console.writeLn(files + " files, " + folders + " folders");
```

Run it with:

```bash
quantum-script list.js
```

`Script.requireExtension("ShellFind")` looks for an external
`quantum-script--shellfind` library first (the file as named, then every
include path folder: next to the interpreter, next to the script), then for
an internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "ShellFind"`.

`Script.requireExtension("Shell")` loads `ShellFind` too, so a script that
already uses `Shell` has the `ShellFind` global without asking for it.

Relative patterns are relative to the **process current directory**
(`Shell.getcwd()`), not to the folder of the script.

## 3. fabricare build scripts

`fabricare` registers `ShellFind` as an internal extension and loads it
before running the build scripts, so `fabricare/*.js` scripts use
`ShellFind` directly, without `requireExtension`:

```javascript
// fabricare/make.js: remove every __pycache__ folder under a tree
function findDirectories(path, what) {
	var found = [];
	var scan = new ShellFind();
	for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
		if (!scan.isDirectory() || scan.isLink()) {
			continue;
		};
		if ((scan.name() == ".") || (scan.name() == "..")) {
			continue;
		};
		if (scan.name() == what) {
			found[found.length] = path + "/" + what;
			continue;
		};
		found = found.concat(findDirectories(path + "/" + scan.name(), what));
	};
	return found;
};

var list = findDirectories("temp/output/Lib", "__pycache__");
for (var k = 0; k < list.length; ++k) {
	Shell.removeDirRecursivelyForce(list[k]);
};
```

`fabricare` links `quantum-script--shellfind.static` into its own
executable, so its `ShellFind` is the one it was built with: `isLink()`
and the fixes listed in [Fixed issues](README.md#fixed-issues) reach
fabricare scripts only after `fabricare` itself is rebuilt with this
version installed. Until then, test for `isLink` first
(`Script.isFunction(ShellFind.prototype.isLink)`).

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `ShellFind` available as an
internal extension by registering it in the init callback (this is what
`test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/ShellFind.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::ShellFind::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeFile("main.js")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("ShellFind")`. With the DLL build of the engine an
external `quantum-script--shellfind.dll` found on the include path wins
over the internal one for `requireExtension`; use
`Script.requireInternalExtension("ShellFind")` to force the internal one.

A host that registers `Shell` must register `ShellFind` too: `Shell`
requires it when it is loaded.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--shellfind"
	]
}
```

`quantum-script--shellfind` brings `quantum-script` and
`quantum-script--console` with it.

## 5. Static builds

For a self-contained executable depend on the static variant and the static
CRT:

```json
{
	"name": "my-host.static",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"crt": "static",
	"dependency": [
		"quantum-script--shellfind.static"
	]
}
```

`quantum-script--shellfind.static` defines
`XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_LIBRARY` for its users (empty export
macro, no `quantumScriptExtension` entry point). There is no DLL to find,
so the host **must** register `ShellFind` as an internal extension
(section 4).

## 6. Threads

Each thread that runs scripts has its own engine and must require
`ShellFind` itself. A `ShellFind` object holds an open directory handle and
is not meant to be shared between threads: create one per thread. The
current directory that relative patterns use is **process wide**
(`Shell.chdir` in one thread changes it for all of them); give worker
threads absolute paths.
