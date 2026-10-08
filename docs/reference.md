# API reference

`Script.requireExtension("ShellFind")` (also loaded by
`Script.requireExtension("Shell")`; fabricare registers it as internal).
Details in [Script API](script-api.md).

## Global

| Function | Returns |
|----------|---------|
| `ShellFind()` / `new ShellFind()` | an empty `ShellFind`, not valid |
| `ShellFind(pattern)` / `new ShellFind(pattern)` | a `ShellFind` after `find(pattern)`; check `isValid()` |
| `ShellFind.isShellFind(x)` | `true` if `x` is a `ShellFind` |

## ShellFind.prototype

| Method | Returns | Notes |
|--------|---------|-------|
| `find(pattern)` | `this`, or `undefined` if nothing matches | closes a previous search first |
| `next()` | `true` on the next entry, `false` at the end | at the end the search is closed |
| `close()` | `undefined` | releases the directory handle |
| `isValid()` | `true` while positioned on an entry | the loop condition |
| `name()` | `String`, the entry name without the folder | |
| `isDirectory()` | Boolean | `.` and `..` too; links to directories |
| `isFile()` | Boolean | exactly `!isDirectory()` |
| `isReadOnly()` | Boolean | Windows: attribute; Linux: owner write bit missing |
| `isLink()` | Boolean | symbolic link, junction; also reported as its target |

`name()`, `isDirectory()`, `isFile()`, `isReadOnly()` and `isLink()`
return `undefined` when the search is not valid. All methods throw
`Error: invalid parameter` when `this` is not a `ShellFind`.

## The loops

```javascript
var scan = new ShellFind();
for (scan.find("dir/*"); scan.isValid(); scan.next()) {
	if ((scan.name() == ".") || (scan.name() == "..")) {
		continue;
	};
	// "dir/" + scan.name(), scan.isDirectory(), scan.isFile(), scan.isReadOnly(), scan.isLink()
};

for (var name of ShellFind("dir/*.cpp")) {
	// names only, from the current entry to the last
};
```

SDKs up to 5.9.0 build 7: no `isLink()`, `name()` crashes when not valid,
`for ... of` crashes or never ends ([Fixed issues](README.md#fixed-issues)).

## Patterns

Wildcards `*` and `?` in the last part of the path only; no recursion.
Windows: case insensitive, `/` or `\`, `"*.*"` matches everything. Linux:
case sensitive, `/` only, `"*.*"` needs a dot. Use `"*"` for every entry,
sort the results when order matters.

## Errors

| Situation | Result |
|-----------|--------|
| folder missing / unreadable, nothing matches | `find` returns `undefined`, `isValid()` is `false` |
| no current entry (not searched, no match, at the end, closed) | entry queries return `undefined` |
| method called on something that is not a `ShellFind` | throws `Error: invalid parameter` |
| `for (var k in scan)` | throws `Error: key not iterable` |
| extension not found | `Script.requireExtension` throws `Unable to open "ShellFind"` |

## C++

```cpp
#include <XYO/QuantumScript.Extension/ShellFind.hpp>

Extension::ShellFind::registerInternalExtension(executive);   // host init callback
```

`fabricare.json`: `"quantum-script--shellfind"`, or
`"quantum-script--shellfind.static"` with `"crt": "static"`. See
[C++ API](cpp-api.md).
