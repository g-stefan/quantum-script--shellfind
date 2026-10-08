# Quantum Script Extension ShellFind — Documentation

`quantum-script--shellfind` is the **directory iterator of Quantum Script**.
Loaded with `Script.requireExtension("ShellFind")`, it adds a `ShellFind`
constructor to scripts: start a search with a wildcard pattern, then walk
the matching directory entries one at a time and ask for each one its
name and whether it is a directory, a file, a link or read-only. One API
for Windows and Linux.

```javascript
Script.requireExtension("ShellFind");

var scan = new ShellFind();
for (scan.find("source/*"); scan.isValid(); scan.next()) {
	if (scan.isDirectory()) {
		if ((scan.name() == ".") || (scan.name() == "..")) {
			continue;
		};
		Console.writeLn("[dir]  " + scan.name());
		continue;
	};
	Console.writeLn("[file] " + scan.name());
};
```

Every method is a thin wrapper over `XYO::System::ShellFind` from
`xyo-system` (`FindFirstFileEx` / `FindNextFile` on Windows, `opendir` /
`readdir` on Linux). Failures are **return values**, never exceptions:
`find` returns `undefined` when nothing matches, and every query about the
current entry (`name`, `isDirectory`, `isFile`, `isReadOnly`, `isLink`)
returns `undefined` when the search is not positioned on an entry. Only
calling a method on something that is not a `ShellFind` throws.

```
scripts: fabricare build scripts, quantum-script .js, ...
quantum-script--shell      (Shell, loads ShellFind automatically)
quantum-script--shellfind  <-- this extension: the ShellFind iterator
quantum-script             (Executive, Variable, Context)
xyo-system                 (XYO::System::ShellFind)
xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

`Shell.getFileList` and `Shell.getDirList` (from `quantum-script--shell`)
return plain lists of paths, one kind of entry at a time. `ShellFind` is
the iterator underneath them, for when a script needs more than a list:

| Need | What `ShellFind` gives |
|------|------------------------|
| Files **and** directories in one pass | every entry, with `isFile()` / `isDirectory()` |
| Walk a whole tree (recursive search, clean `__pycache__`, ...) | `isDirectory()` to decide where to recurse, `isLink()` to avoid following links |
| Skip or handle read-only entries | `isReadOnly()` |
| Stop at the first match ("is there any `*.dll` here?") | `find` returns `undefined` when nothing matches; no list is built |
| Very large folders | one entry at a time, no array of every name |

For a simple list of files or folders matching a pattern, `Shell.getFileList`
/ `Shell.getDirList` are shorter.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("ShellFind");` | `Script.requireExtension("Shell")` loads it too |
| An empty iterator | `var scan = new ShellFind();` | `isValid()` is `false` until a `find` succeeds |
| Search and position on the first match | `scan.find("dir/*.cpp")` | returns `scan`, or `undefined` if nothing matches |
| Create and search in one step | `var scan = ShellFind("dir/*");` | with or without `new`; check `isValid()` |
| The loop | `for (scan.find(p); scan.isValid(); scan.next()) { ... };` | gives access to every attribute |
| Names only | `for (var name of ShellFind(p)) { ... };` | ends after the last entry, nothing for no match |
| Current entry | `scan.name()` | the name only, **without** the folder; `undefined` when not valid |
| What it is | `isDirectory()`, `isFile()`, `isReadOnly()`, `isLink()` | a link is also seen as what it points to |
| Skip `.` and `..` | compare `name()` | they are returned when the pattern matches them (`"*"`) |
| Stop early | `scan.close()` | `find` on the same object also closes the previous search |
| Is this a ShellFind? | `ShellFind.isShellFind(x)` | |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, fabricare scripts, register it in a C++ host, static builds |
| [Script API](script-api.md) | Every function: arguments, exact behavior, return values, pattern rules, links, Windows / Linux differences |
| [Recipes](recipes.md) | List a folder, filter, count, walk a tree, find directories by name, first match, read-only entries |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, `VariableShellFind`, the iterator, notes for maintainers |
| [API reference](reference.md) | Every function on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`; the underlying `XYO::System::ShellFind` in the `xyo-system`
repository, `docs/files.md`; the `Shell` object in the
`quantum-script--shell` repository, `docs/`.

## Fixed issues

Builds up to **5.9.0 build 7** have these problems. They are fixed in the
sources; an SDK or a `fabricare` executable built before the fix still
behaves the old way until it is rebuilt (`fabricare` links
`quantum-script--shellfind.static` into itself, so it needs a rebuild of
`fabricare` too):

| Code | Old behavior | Fix |
|------|--------------|-----|
| `new ShellFind().name()`, or `name()` after a `find` that matched nothing | **crash**: `XYO::System::ShellFind::name` is an uninitialized `char *` until a search succeeds | `undefined` when the search is not valid |
| `name()` after the end of a search | last name (Windows), freed memory (Linux) | `undefined` |
| `isDirectory()`, `isFile()`, `isReadOnly()` when not valid | `false`, or the values of the last entry | `undefined` |
| `for (var x of ShellFind("nomatch*"))` | **crash**: the value iterator read `name` without checking the search | no iteration |
| `for (var x of ShellFind("*.txt"))` | **never ended**: kept returning the last name | ends after the last entry |
| links | no way to tell a link from a real entry | `isLink()` |

Scripts that must also run on an older SDK should use the
`for (find; isValid(); next)` loop, call `name()` and the type tests only
while `isValid()` is `true`, and check that `isLink` exists before using
it:

```javascript
var hasIsLink = Script.isFunction(ShellFind.prototype.isLink);
```

## Source map

```
source/XYO/QuantumScript.Extension/ShellFind.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/ShellFind.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/ShellFind/
    Dependency.hpp                                          <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                      initExecutive, registerInternalExtension,
                                                            the ShellFind function and its methods
    VariableShellFind[.hpp/.cpp]                            the script value: holds an XYO::System::ShellFind
    IteratorValue[.hpp/.cpp]                                for (var x of scan) support
    Context.hpp                                             ShellFindContext: symbol and prototype
    Copyright / License / Version                           library metadata
test/test.01.cpp                                            C++ host registering Console and ShellFind
                                                            as internal extensions
test/test.01.js                                             checks every method, the not-valid cases
                                                            and for ... of (regressions of the fixed issues)
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--shellfind/`](../.claude/skills/quantum-script--shellfind/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use
`ShellFind` (fabricare scripts, Quantum Script tools, other extensions).
