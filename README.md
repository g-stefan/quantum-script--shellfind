# Quantum Script Extension ShellFind

Quantum Script extension
- A `ShellFind` directory iterator, one API for Windows and Linux.
- Search one folder with a wildcard pattern (`dir/*.cpp`) and walk the
matching entries one at a time.
- For each entry: its name, whether it is a directory, a file, a link,
read-only.
- Lists, filters, stops at the first match, walks whole trees (recursion in
the script, without following links).
- Errors are return values, not exceptions: `undefined` when nothing
matches or there is no current entry.

```javascript
Script.requireExtension("ShellFind");

ShellFind();
ShellFind(pattern);
ShellFind.isShellFind(x);
ShellFind.prototype.find(file);
ShellFind.prototype.next();
ShellFind.prototype.close();
ShellFind.prototype.isReadOnly();
ShellFind.prototype.isDirectory();
ShellFind.prototype.isFile();
ShellFind.prototype.isLink();
ShellFind.prototype.name();
ShellFind.prototype.isValid();
```

```javascript
var scan = new ShellFind();
for (scan.find("source/*"); scan.isValid(); scan.next()) {
	if ((scan.name() == ".") || (scan.name() == "..")) {
		continue;
	};
	Console.writeLn((scan.isDirectory() ? "[dir]  " : "[file] ") + scan.name());
};
```

Built on `quantum-script` and `xyo-system` (`XYO::System::ShellFind`), part
of the XYO C++ SDK. Also loaded by `Script.requireExtension("Shell")`.

## Documentation

- [Overview](docs/README.md) - purpose, concepts, fixed issues
- [Getting started](docs/getting-started.md) - build, load from a script, fabricare scripts, register in a C++ host, static builds
- [Script API](docs/script-api.md) - every function: exact behavior, patterns, Windows / Linux differences
- [Recipes](docs/recipes.md) - list, filter, first match, walk a tree, find folders by name, read-only entries
- [C++ API](docs/cpp-api.md) - registration, `VariableShellFind`, the iterator, notes for maintainers
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--shellfind](.claude/skills/quantum-script--shellfind/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
