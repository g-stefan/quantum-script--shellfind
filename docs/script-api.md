# Script API

Everything the extension defines after `Script.requireExtension("ShellFind")`:
one global function `ShellFind`, one static function on it and eight
methods on `ShellFind.prototype`.

A `ShellFind` value is a search over **one directory**: it is either
*positioned on an entry* (`isValid()` is `true`; `name()` and the type
tests describe that entry) or *not valid* (never searched, nothing matched,
past the last entry, or closed). `next()` moves to the following matching
entry and becomes not valid after the last one. While the value is not
valid, every query about the current entry returns `undefined`.

```javascript
var scan = new ShellFind();
for (scan.find("dir/*"); scan.isValid(); scan.next()) {
	// scan.name(), scan.isDirectory(), scan.isFile(), scan.isReadOnly(), scan.isLink()
};
```

Every method throws `Error: invalid parameter` when `this` is not a
`ShellFind` (for example `ShellFind.prototype.next.call({})`). Nothing else
throws: a missing folder, a pattern that matches nothing or an access
error just leave the search not valid.

## ShellFind(pattern)

```javascript
var scan = new ShellFind();            // empty, not valid
var scan = new ShellFind("dir/*.cpp"); // created and searched
var scan = ShellFind("dir/*.cpp");     // same, new is optional
```

Creates a `ShellFind` value. Without an argument (or with `undefined` /
`null`) the value is empty and not valid. With a pattern, `find(pattern)`
is called on it; the result of the search is **not** returned separately,
check `scan.isValid()`.

`typeof(ShellFind)` is `"Function"`; `typeof(scan)` and `"" + scan` are
`"ShellFind"`. A `ShellFind` value is always `true` in a condition, even
when it is not valid: test `isValid()`, not the value.

## ShellFind.isShellFind(x)

`true` if `x` is a `ShellFind` value, `false` for anything else.

## ShellFind.prototype.find(pattern)

Starts a new search and positions on the first matching entry. Returns
the object itself (`this`) on success, `undefined` when nothing matches or
the folder cannot be read.

```javascript
if (Script.isUndefined(scan.find("output/bin/*.dll"))) {
	Console.writeLn("no DLL in output/bin");
};
```

- `pattern` is converted with `toString()`: a missing argument searches for
  the name `undefined`.
- A search still open on the same object is closed first, so one object
  can be reused for many searches.
- See [Patterns](#patterns) for what the pattern may contain.

## ShellFind.prototype.next()

Moves to the next matching entry. Returns `true` if there is one, `false`
at the end; at the end the search is closed and `isValid()` becomes
`false`. Calling `next()` on a value that is not valid returns `false`.

## ShellFind.prototype.close()

Ends the search and releases the directory handle; `isValid()` becomes
`false`. Returns `undefined`. Safe to call more than once.

The handle is also released by the next `find`, at the end of the search
(`next()` returning `false`) and when the value is garbage collected.
Call `close()` when you stop a loop early (`break`, `return`) and the
object stays alive, so the folder is not kept open (on Windows an open
search handle can stop the folder from being removed or renamed).

## ShellFind.prototype.isValid()

`true` while the search is positioned on an entry, `false` before the
first successful `find`, after a `find` that matched nothing, after the
last `next()` and after `close()`. This is the loop condition.

## ShellFind.prototype.name()

The name of the current entry as a `String`: the name only, **without the
folder** of the pattern (`find("source/*.cpp")` gives `"main.cpp"`, not
`"source/main.cpp"`). The returned string is a copy and can be kept.
`undefined` when the search is not valid.

## ShellFind.prototype.isDirectory()

`true` if the current entry is a directory, including `.` and `..` and a
link (symbolic link, junction) that points to a directory. `undefined`
when the search is not valid.

## ShellFind.prototype.isFile()

Exactly `!isDirectory()` while valid: `true` for regular files, links to
files, dangling links, and on Linux also for anything that is not a
directory (devices, sockets, pipes). `undefined` when the search is not
valid.

## ShellFind.prototype.isReadOnly()

- Windows: the entry has the read-only attribute (`attrib +r`).
- Linux: the owner write bit is not set (`chmod u-w`). This is the mode,
  not whether the current user can actually write.

`undefined` when the search is not valid.

`Shell.removeFile` and `Shell.removeDirRecursively` fail on read-only
entries (on Windows `Shell.removeFile` returns `false` and the file stays);
the `...Force` variants of `Shell` clear the attribute first.

## ShellFind.prototype.isLink()

`true` if the current entry is a link, `undefined` when the search is not
valid:

- Windows: a symbolic link (to a file or a directory), a junction (mount
  point) or a WSL symbolic link. Other reparse points (OneDrive / cloud
  files, deduplication, ...) are normal entries.
- Linux: a symbolic link, also a dangling one.

A link is also reported as what it points to: a link to a directory is
`isDirectory()` and `isLink()`, a link to a file or a dangling link is
`isFile()` and `isLink()`. Check `isLink()` before recursing into a
directory when links must not be followed (see
[Recipes](recipes.md#walk-a-tree)).

## Undefined versus false

The type tests return `undefined`, not `false`, when there is no current
entry. In a condition `undefined` is false, so `if (scan.isDirectory())`
works either way; but `scan.isDirectory() == false` is not the same as
`!scan.isDirectory()` (`undefined == false` is `false`). Inside the
`for (find; isValid(); next)` loop the values are always Booleans.

## Iterating with for ... of

```javascript
for (var name of ShellFind("source/*.cpp")) {
	Console.writeLn(name);
};
```

`for (var x of scan)` gives the **names** (without the folder) from the
current entry to the last one, then ends. It continues the search the
value is positioned on, so it gives nothing for a search that matched
nothing, a value that was never searched, or one already at its end. The
loop advances the same search: after it `isValid()` is `false`.

Use the `find / isValid / next` loop when you need the attributes of each
entry; inside `for ... of` the type tests describe the entry *after* the
current name.

`for (var k in scan)` throws `Error: key not iterable`.

## Patterns

The pattern is a path whose **last part** may contain wildcards:

| Pattern | Matches |
|---------|---------|
| `"*"` | every entry of the current directory, `.` and `..` included |
| `"dir/*"` | every entry of `dir` |
| `"dir/*.cpp"` | entries of `dir` ending in `.cpp` |
| `"dir/test.??"` | `?` is exactly one character |
| `"dir/name"` | no wildcard: the single entry `name`, if it exists (a cheap existence test that also tells file from folder and link from real entry) |

- Wildcards only in the last part: `"*/x.txt"` is not a recursive search.
  To go deeper, recurse yourself (see [Recipes](recipes.md#walk-a-tree)).
- Relative patterns use the process current directory.
- `name()` is never prefixed with the folder: keep the folder of the
  pattern in a variable and join it yourself.

### Windows and Linux differences

| | Windows (`FindFirstFileEx`) | Linux (`opendir` / `readdir` + `matchASCII`) |
|---|---|---|
| Case | insensitive: `*.txt` matches `B.TXT` | sensitive |
| `"*.*"` | every entry, also names without a dot | only names with a dot (and `.`, `..`) |
| Separators | `/` and `\` | `/` only |
| Hidden / system entries | returned | dot files returned |
| Order | file system order (NTFS: mostly alphabetical) | directory order, unspecified |
| Other | with 8.3 short names enabled, `*.htm` may also match `.html` files | |
| `isReadOnly()` | read-only attribute | owner write bit missing |
| `isLink()` | symbolic link, junction, WSL symbolic link | symbolic link |

Use `"*"` for "every entry" on both systems, and sort the names yourself
when the order matters.
