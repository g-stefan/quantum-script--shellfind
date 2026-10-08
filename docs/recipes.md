# Recipes

All recipes assume:

```javascript
Script.requireExtension("Console");
Script.requireExtension("ShellFind");   // or Script.requireExtension("Shell"), which loads it
```

and one of the two loop shapes:

```javascript
for (scan.find(pattern); scan.isValid(); scan.next()) { ... };   // every attribute
for (var name of ShellFind(pattern)) { ... };                     // names only
```

## List a folder

```javascript
function listFolder(path) {
	var scan = new ShellFind();
	for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
		if ((scan.name() == ".") || (scan.name() == "..")) {
			continue;
		};
		Console.writeLn((scan.isDirectory() ? "[dir]  " : "[file] ") + path + "/" + scan.name());
	};
};

listFolder("source");
```

`name()` has no folder: join it with the folder of the pattern.

## Names only

When the attributes do not matter, `for ... of` gives the names:

```javascript
var names = [];
for (var name of ShellFind("source/*.cpp")) {
	names[names.length] = name;
};
```

It gives nothing when nothing matches. With `"*"` it also gives `.` and
`..`.

## Collect paths into an array, sorted

```javascript
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
	return list.sort();   // Quantum Script: sort returns a new array
};

var files = getEntries("source", false);
var folders = getEntries("source", true);
```

The order of a search is the order of the file system, not alphabetical
on Linux: sort when it matters.

## Filter by extension

Let the pattern filter (case insensitive on Windows, case sensitive on
Linux):

```javascript
var scan = new ShellFind();
for (scan.find("source/*.cpp"); scan.isValid(); scan.next()) {
	if (scan.isFile()) {
		Console.writeLn(scan.name());
	};
};
```

For the same result on both systems regardless of case, search `"*"` and
match yourself:

```javascript
for (scan.find("source/*"); scan.isValid(); scan.next()) {
	if (scan.isFile() && scan.name().toLowerCaseASCII().matchASCII("*.cpp")) {
		Console.writeLn(scan.name());
	};
};
```

## Test for one entry

A pattern without wildcards finds one entry; `find` returns `undefined`
when it does not exist:

```javascript
var scan = new ShellFind();
if (Script.isUndefined(scan.find("output/bin"))) {
	Console.writeLn("output/bin does not exist");
} else if (!scan.isDirectory()) {
	Console.writeLn("output/bin is a file");
};
scan.close();
```

## Stop at the first match

```javascript
function hasMatch(pattern) {
	var scan = new ShellFind();
	var found = !Script.isUndefined(scan.find(pattern));
	scan.close();
	return found;
};

if (!hasMatch("output/bin/*.dll")) {
	Console.writeLn("no DLL to copy");
};
```

`close()` releases the directory handle at once instead of when the object
is collected.

## Walk a tree

`ShellFind` searches one folder; recurse into each directory yourself,
skipping `.` and `..`, and links to directories so the walk cannot loop:

```javascript
function walk(path, onFile) {
	var scan = new ShellFind();
	for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
		var name = scan.name();
		if (scan.isDirectory()) {
			if ((name == ".") || (name == "..")) {
				continue;
			};
			if (scan.isLink()) {
				continue;   // symbolic link or junction: do not follow
			};
			walk(path + "/" + name, onFile);
			continue;
		};
		onFile(path + "/" + name);
	};
};

var total = 0;
walk("source", function(file) {
	++total;
});
Console.writeLn(total + " files");
```

- Each level keeps its own `ShellFind` (a local `var`), so recursion is
  safe; each level holds one open directory handle while its children are
  walked.
- Links to files are passed to `onFile` like files; add
  `if (scan.isLink()) { continue; };` before `onFile` to skip them too.
- To follow directory links on purpose, drop the `isLink()` test and cap
  the depth instead, since a link can point to one of its parents:

```javascript
function walk(path, onFile, depth) {
	if (depth > 32) {
		return;
	};
	// ... as above without the isLink() test, calling walk(path + "/" + name, onFile, depth + 1)
};
```

## Find directories by name

Collect every `__pycache__` (or `.git`, `node_modules`, ...) under a tree,
without entering the ones found:

```javascript
function findDirectories(path, what) {
	var found = [];
	var scan = new ShellFind();
	for (scan.find(path + "/*"); scan.isValid(); scan.next()) {
		var name = scan.name();
		if (!scan.isDirectory() || scan.isLink()) {
			continue;
		};
		if ((name == ".") || (name == "..")) {
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

Script.requireExtension("Shell");
var list = findDirectories("temp/output/Lib", "__pycache__");
for (var k = 0; k < list.length; ++k) {
	Shell.removeDirRecursivelyForce(list[k]);
};
```

Collect first, remove afterwards: removing a folder while its parent is
being searched can make the search skip or repeat entries.

## Read-only entries

```javascript
Script.requireExtension("Shell");

var scan = new ShellFind();
for (scan.find("release/*"); scan.isValid(); scan.next()) {
	if (scan.isFile() && scan.isReadOnly()) {
		Console.writeLn("read-only: " + scan.name());
		Shell.removeFileForce("release/" + scan.name());   // removeFile would fail
	};
};
```

## Count files and size of a tree

`ShellFind` has no size; combine it with `Shell.getFileSize`:

```javascript
Script.requireExtension("Shell");

var count = 0;
var size = 0;
walk("output", function(file) {   // walk from "Walk a tree"
	++count;
	size += Shell.getFileSize(file);
});
Console.writeLn(count + " files, " + size + " bytes");
```

## Reuse one object

`find` closes the previous search, so one object can serve many searches
(but not nested ones: a recursive function needs one object per level):

```javascript
var scan = new ShellFind();
var folders = ["source", "test", "docs"];
for (var k = 0; k < folders.length; ++k) {
	var n = 0;
	for (scan.find(folders[k] + "/*"); scan.isValid(); scan.next()) {
		if (scan.isFile()) {
			++n;
		};
	};
	Console.writeLn(folders[k] + ": " + n + " files");
};
```
