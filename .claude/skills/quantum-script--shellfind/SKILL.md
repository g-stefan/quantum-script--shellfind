---
name: quantum-script--shellfind
description: >-
  How to use the Quantum Script ShellFind extension
  (quantum-script--shellfind), the directory iterator loaded with
  Script.requireExtension("ShellFind") (also loaded by
  Script.requireExtension("Shell"), registered as internal by fabricare):
  new ShellFind() / ShellFind(pattern), ShellFind.isShellFind(x), and the
  methods find(pattern) (returns this or undefined), next(), close(),
  isValid(), name() (entry name without the folder), isDirectory() (also .
  and ..), isFile() (= !isDirectory), isReadOnly(), isLink() (symbolic
  link, junction), all entry queries returning undefined when the search
  is not valid; the loop for (scan.find(p); scan.isValid(); scan.next()) and
  for (var name of ShellFind(p)) for names only, wildcard patterns (* ? in
  the last part only, case insensitive on Windows, case sensitive on
  Linux), walking a tree recursively without following links, finding
  folders by name, and old SDKs / fabricare builds up to 5.9.0 build 7
  (no isLink, name() crash, for ... of crash or endless). Also the C++ side:
  registerInternalExtension, initExecutive, VariableShellFind wrapping
  XYO::System::ShellFind. Use when writing or reviewing Quantum Script or
  fabricare .js code that lists directories or walks folder trees with
  ShellFind, C++ code that includes <XYO/QuantumScript.Extension/ShellFind.hpp>,
  a fabricare.json depending on "quantum-script--shellfind", or when
  working inside the quantum-script--shellfind repository.
---

# quantum-script--shellfind

Directory iterator extension of Quantum Script (see the `quantum-script`
skill for the language and its differences from JavaScript, the
`xyo-system` skill for the underlying `XYO::System::ShellFind`, the
`quantum-script--shell` skill for `Shell`, and the `fabricare` skill for
build scripts; their rules apply). Purpose: **walk the entries of one
directory that match a wildcard pattern, one at a time, with their name and
type** (directory, file, link, read-only), so scripts can list folders, filter,
stop at the first match and recurse through trees. One API for Windows and
Linux.

Full documentation: `docs/` in the quantum-script--shellfind repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--shellfind\docs` on this
machine): README (purpose, fixed issues), getting-started, **script-api**
(exact behavior, patterns, Windows / Linux differences), **recipes** (list,
filter, walk a tree, find folders by name, first match), cpp-api,
reference. Read the matching page when you need more than this summary.
When in doubt read `source/XYO/QuantumScript.Extension/ShellFind/Library.cpp`
(~200 lines) and `xyo-system` `source/XYO/System/ShellFind-OS-Windows.cpp` /
`ShellFind-OS-Linux.cpp`.

## Script API

```javascript
Script.requireExtension("ShellFind");   // or "Shell", which loads it; fabricare: already there

var scan = new ShellFind();              // empty, isValid() == false
var scan = ShellFind("dir/*.cpp");       // new optional; created + find; CHECK isValid()
ShellFind.isShellFind(x);                // Boolean

scan.find(pattern);                      // first match -> this, or undefined; closes previous search
scan.next();                             // true on next entry, false at end (and closed)
scan.close();                            // release the directory handle -> undefined
scan.isValid();                          // positioned on an entry: THE LOOP CONDITION
// entry queries: UNDEFINED (not false) when !isValid()
scan.name();                             // "main.cpp": NO FOLDER
scan.isDirectory();                      // also "." and "..", and links to directories
scan.isFile();                           // exactly !isDirectory() (links to files, dangling links)
scan.isReadOnly();                       // Windows: attribute; Linux: owner write bit missing
scan.isLink();                           // symbolic link / junction (also reported as its target)

for (var name of ShellFind("dir/*.cpp")) { ... };   // names only; nothing if no match; ends
```

The loop with attributes:

```javascript
var scan = new ShellFind();
for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
	var name = scan.name();
	if (scan.isDirectory()) {
		if ((name == ".") || (name == "..") || scan.isLink()) {
			continue;
		};
		// recurse: walk(path + "/" + name)
		continue;
	};
	// file: path + "/" + name
};
```

## Hard rules

1. **Entry queries return `undefined` when not valid** (never searched, no
   match, at the end, closed): `name()`, `isDirectory()`, `isFile()`,
   `isReadOnly()`, `isLink()`. `undefined` is false in a condition, but
   `scan.isDirectory() == false` is `false` for it: use `!x` or check
   `isValid()` first.
2. **`for (var x of scan)` gives names only**, from the current entry to
   the end, and advances the same search (inside the body the type tests
   already describe the next entry): use the `find / isValid / next` loop
   when attributes matter. `for (var k in scan)` throws `key not iterable`.
3. **Old builds (SDK, or a `fabricare` executable, up to 5.9.0 build 7)**:
   no `isLink()` (test `Script.isFunction(ShellFind.prototype.isLink)`),
   `name()` on a value with no current entry crashes, `for ... of` crashes
   on no match and never ends otherwise. `fabricare` links the static
   library: it gets the fixes only when rebuilt. Code that must run there:
   the `find / isValid / next` loop, `name()` only while `isValid()`.
4. **`ShellFind(pattern)` does not tell you if it matched**: the value is
   always truthy (`toBoolean` is `true`), test `isValid()`. `find` returns
   `this` or `undefined`: `Script.isUndefined(scan.find(p))` is the "no
   match" test.
5. **`name()` has no folder**: keep the folder of the pattern and join it
   (`path + "/" + scan.name()`).
6. **Skip `.` and `..`** when the pattern matches them (`"*"`), especially
   before recursing.
7. **Wildcards (`*`, `?`) only in the last part**, no recursive search:
   recurse yourself, **one `ShellFind` per level** (a local `var`); one
   object may be reused for consecutive (not nested) searches.
8. **Use `"*"` for every entry**: Windows matching is case insensitive
   (`*.txt` matches `B.TXT`) and `"*.*"` matches everything; Linux is case
   sensitive and `"*.*"` needs a dot. Windows accepts `/` and `\`, Linux
   only `/`. Order is file system order: sort (`list.sort()` returns a new
   array) when it matters. For case-insensitive filtering on both systems:
   `name.toLowerCaseASCII().matchASCII("*.cpp")`.
9. **Links are also seen as their target** (a directory link is
   `isDirectory()` and `isLink()`): skip `isLink()` directories when
   recursing, or a link to a parent loops forever. Windows: symbolic links,
   junctions, WSL links (not OneDrive / other reparse points); Linux:
   symbolic links, dangling ones too (`isFile()`).
10. **Collect first, then delete**: removing entries of a folder while it is
   being searched can skip or repeat entries. Read-only files need
   `Shell.removeFileForce` / `Shell.removeDirRecursivelyForce`.
11. **`close()` when leaving a loop early** and the object stays alive, so
    the folder handle is released (an open handle can block removing or
    renaming the folder on Windows).
12. Relative patterns use the **process current directory** (process wide,
    changed by `Shell.chdir` in any thread).
13. Only calling a method on a non-`ShellFind` throws (`Error: invalid
    parameter`); a missing folder or no match just leaves the search not
    valid. A missing argument to `find` becomes the string `"undefined"`.

## Recipes

```javascript
// sorted list of files (or folders) of one directory
function getEntries(path, wantDirectories) {
	var list = [];
	var scan = new ShellFind();
	for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
		if ((scan.name() == ".") || (scan.name() == "..")) {
			continue;
		};
		if (scan.isDirectory() != wantDirectories) {
			continue;
		};
		list[list.length] = path + "/" + scan.name();
	};
	return list.sort();
};

// any match?
function hasMatch(pattern) {
	var scan = new ShellFind();
	var found = !Script.isUndefined(scan.find(pattern));
	scan.close();
	return found;
};

// every folder named `what` under `path`, not entering the ones found
function findDirectories(path, what) {
	var found = [];
	var scan = new ShellFind();
	for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
		var name = scan.name();
		if (!scan.isDirectory() || scan.isLink() || (name == ".") || (name == "..")) {
			continue;
		};
		if (name == what) {
			found[found.length] = path + "/" + name;
			continue;
		};
		found = found.concat(findDirectories(path + "/" + name, what));
	};
	return found;
};
```

A pattern without wildcards is a single-entry test that also tells file from
folder and link from real entry: `scan.find("output/bin")` then `scan.isDirectory()`. For plain lists
of paths `Shell.getFileList("dir/*.cpp")` / `Shell.getDirList("dir/*")` are
shorter; use `ShellFind` for mixed files and folders, attributes, early stop
or recursion.

## C++

```cpp
#include <XYO/QuantumScript.Extension/ShellFind.hpp>
using namespace XYO::QuantumScript;

// host init callback (a host registering Shell must register ShellFind too)
Extension::ShellFind::registerInternalExtension(executive);
// scripts still call Script.requireExtension("ShellFind")
```

- `fabricare.json`: depend on `"quantum-script--shellfind"` (brings
  `quantum-script` and `quantum-script--console`); static:
  `"quantum-script--shellfind.static"`, `"crt": "static"`, register it as
  internal. The static library defines
  `XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_LIBRARY` (empty export macro, no
  `quantumScriptExtension` entry point).
- A script value is `Extension::ShellFind::VariableShellFind` (check with
  `TIsType<...>`), its `value` member is the `XYO::System::ShellFind`.
  Inside `namespace XYO::QuantumScript::Extension::ShellFind` the name
  `ShellFind` is the namespace: write `XYO::System::ShellFind` in full.
- From plain C++ use `XYO::System::ShellFind` directly: `for
  (scan.find(p); scan; scan.next())`, `scan.name` is a `char *` into the
  iterator (copy it), uninitialized before the first match and dangling
  after the end on Linux: read it only while `scan` is true.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console and ShellFind as internal and runs `test/test.01.js`
  from `output/test`: checks of every method, the not-valid cases and
  `for ... of`; run `make` first), `fabricare install` (see the
  `fabricare` skill). `quantum-script` and `quantum-script--console` must
  be installed first. Windows: run from a `vcvars64.bat` environment. If
  `fabricare test` reports `'test.01' is not recognized`, the environment
  sets `NoDefaultCurrentDirectoryInExePath` (Claude Code does): run
  `output/test/test.01.exe` from `output/test` with `output/bin` first on
  `PATH`.
- Quick checks: write a `.js` file in a scratch folder and run it with
  `quantum-script`. The interpreter loads the extension found **next to
  itself first**, i.e. the installed SDK one, not `output/bin`: to try a
  fresh build without installing, copy `quantum-script.exe`,
  `quantum-script--console.dll` and the new `quantum-script--shellfind.dll`
  into one scratch folder and run that copy (Linux / WSL: the same with the
  `.so` files and `LD_LIBRARY_PATH` set to the SDK `bin`). Test links with
  `mklink /J` (junction, no admin needed) / `ln -s`.
- Layout: `ShellFind/Library.cpp` (the `ShellFind` function and every
  method, registered in `initExecutive` with
  `executive->setFunction2("ShellFind.prototype.name()", name)`; entry
  queries return `Context::getValueUndefined()` when `!value`),
  `VariableShellFind` (the value, holds `XYO::System::ShellFind value`),
  `IteratorValue` (`for ... of`, stops when `!(*sourceShellFind)`),
  `Context.hpp` (symbol and prototype singleton), `ShellFind.Amalgam.cpp`
  (includes every `.cpp`).
- Every new entry query must check `!value` before reading the
  `XYO::System::ShellFind` fields (`name` is not safe to read otherwise).
  After a change update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `docs/cpp-api.md`, `docs/README.md` and this skill,
  and add a check to `test/test.01.js`.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
