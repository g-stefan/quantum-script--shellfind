# C++ API

For hosts that embed Quantum Script and for maintainers of this extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first. From plain C++ code use
`XYO::System::ShellFind` directly (see the `xyo-system` repository,
`docs/files.md`); this extension only wraps it for scripts.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/ShellFind.hpp>   // Library.hpp: initExecutive, registerInternalExtension

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::ShellFind`. Export macro:
`XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT` (empty when
`XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_LIBRARY` is defined, i.e. static
builds; set to export when building the library itself,
`XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_INTERNAL`).

Inside that namespace `ShellFind` is the namespace itself: name the system
class `XYO::System::ShellFind` in full.

## Registering the extension

```cpp
void Extension::ShellFind::registerInternalExtension(Executive *executive);
void Extension::ShellFind::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"ShellFind"` as an internal
  extension; call it from the host's init callback (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `ShellFind` in a thread. It sets the extension name,
  info (license text), version and marks it public, creates the global
  `ShellFind` function and its prototype (`newContext`), and registers the
  methods with `executive->setFunction2`. Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--shellfind.dll`.

## Script function → C++

| Script | Native function in `Library.cpp` | `XYO::System::ShellFind` | Result |
|--------|----------------------------------|--------------------------|--------|
| `ShellFind(pattern)` | `functionShellFind` | `find(pattern)` on a new value | new `VariableShellFind` (empty for `undefined` / `null`) |
| `ShellFind.isShellFind(x)` | `isShellFind` | | `TIsType<VariableShellFind>` |
| `find(pattern)` | `findFile` | `find` | `this`, or `undefined` on `false` |
| `next()` | `findNext` | `next` | Boolean |
| `close()` | `findClose` | `close` | `undefined` |
| `isValid()` | `isValid` | `operator bool` | Boolean |
| `name()` | `name` | `name` (`char *`) | new `VariableString` (a copy); `undefined` when not valid |
| `isDirectory()` | `isDirectory` | `isDirectory` | Boolean; `undefined` when not valid |
| `isFile()` | `isFile` | `isFile` | Boolean; `undefined` when not valid |
| `isReadOnly()` | `isReadOnly` | `isReadOnly` | Boolean; `undefined` when not valid |
| `isLink()` | `isLink` | `isLink` | Boolean; `undefined` when not valid |

Every method except `isShellFind` starts with
`if (!TIsType<VariableShellFind>(this_)) throw(Error("invalid parameter"));`.
The five entry queries then return `Context::getValueUndefined()` when
`!value` (not positioned on an entry). This check is required, not just
convenient: `XYO::System::ShellFind::name` is uninitialized before the
first successful search and, on Linux, points into the closed directory
stream after the end.

## VariableShellFind

```cpp
class VariableShellFind : public Variable {
	public:
		XYO::System::ShellFind value;          // the search, closed in activeDestructor

		static Variable *newVariable();        // pooled (TMemoryPoolActive)
		String getVariableType();              // "ShellFind"
		Variable *instancePrototype();         // ShellFindContext::prototypeShellFind->prototype
		TPointer<Iterator> getIteratorValue(); // IteratorValue, for (var x of scan)
		bool toBoolean();                      // always true
		String toString();                     // "ShellFind"
};
```

A host function that receives a `ShellFind` from a script checks the type
and uses `value`:

```cpp
static TPointer<Variable> countEntries(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointerX<Variable> &arg(arguments->index(0));
	if (!TIsType<Extension::ShellFind::VariableShellFind>(arg)) {
		throw(Error("invalid parameter"));
	};
	XYO::System::ShellFind &scan(((Extension::ShellFind::VariableShellFind *)arg.value())->value);
	Number count = 0;
	for (; scan; scan.next()) {
		++count;
	};
	return VariableNumber::newVariable(count);
};
```

`XYO_DYNAMIC_TYPE_IMPLEMENT(VariableShellFind, "{6F5DF4E6-0BBB-469D-B990-4AC7D36B8887}")`
gives the type its identity across DLLs.

## ShellFindContext

```cpp
class ShellFindContext : public Object {
	public:
		Symbol symbolFunctionShellFind;           // "ShellFind"
		TPointerX<Prototype> prototypeShellFind;  // prototype of the ShellFind function
};
ShellFindContext *getContext();                   // TSingleton<ShellFindContext>
```

`newContext` fills it when the extension is initialized; `deleteContext`
(registered with `setExtensionDeleteContext`) releases the prototype when
the executive ends.

## IteratorValue (for ... of)

```cpp
bool IteratorValue::next(Variable *out) {
	if (!sourceShellFind || !(*sourceShellFind)) { out = undefined; return false; };
	out = String(sourceShellFind->name);
	sourceShellFind->next();
	return true;
};
```

`getIteratorValue` points the iterator at the value's own
`XYO::System::ShellFind` (and keeps the value alive with `value_`). Each
step returns the current name and advances; the loop ends when the search
is no longer valid. In builds up to 5.9.0 build 7 the validity check was
missing: the loop never ended and an empty search crashed (see
[Fixed issues](README.md#fixed-issues)).

## Notes for maintainers

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console and ShellFind as internal and runs `test/test.01.js`,
  which only loads both extensions), `fabricare install`. `quantum-script`
  and `quantum-script--console` must be installed first.
- `ShellFind.Amalgam.cpp` includes every `.cpp`: add new source files there.
- Native functions are
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`
  and are registered in `initExecutive` with
  `executive->setFunction2("ShellFind.prototype.name()", name)`.
- `test/test.01.js` checks every method, the not-valid cases and
  `for ... of`; add a check there when changing a method.
- New methods: update `README.md`,
  `docs/script-api.md`, `docs/reference.md`, this page and the skill in
  `.claude/skills/quantum-script--shellfind/`.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header MIT for `source/`, Unlicense for
  `test/` (see `.reuse/dep5`).
