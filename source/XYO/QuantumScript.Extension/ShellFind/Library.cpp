// Quantum Script Extension ShellFind
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/ShellFind/Library.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/Copyright.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/License.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/Version.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/VariableShellFind.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/Context.hpp>

namespace XYO::QuantumScript::Extension::ShellFind {

	ShellFindContext::ShellFindContext() {
		symbolFunctionShellFind = 0;
		prototypeShellFind.pointerLink(this);
	};

	ShellFindContext *getContext() {
		return TSingleton<ShellFindContext>::getValue();
	};

	static TPointer<Variable> functionShellFind(VariableFunction *function, Variable *this_, VariableArray *arguments) {
		TPointerX<Variable> &toFind(arguments->index(0));
		if (TIsTypeExact<VariableUndefined>(toFind) ||
		    TIsType<VariableNull>(toFind)) {
			return VariableShellFind::newVariable();
		};
		TPointer<Variable> find_(VariableShellFind::newVariable());
		((VariableShellFind *)find_.value())->value.find(toFind->toString());
		return find_;
	};

	static void deleteContext() {
		ShellFindContext *shellFindContext = getContext();
		shellFindContext->prototypeShellFind.deleteMemory();
		shellFindContext->symbolFunctionShellFind = 0;
	};

	static void newContext(Executive *executive, void *extensionId) {
		VariableFunction *defaultPrototypeFunction;

		ShellFindContext *shellFindContext = getContext();
		executive->setExtensionDeleteContext(extensionId, deleteContext);

		shellFindContext->symbolFunctionShellFind = Context::getSymbol("ShellFind");
		shellFindContext->prototypeShellFind.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(nullptr, nullptr, nullptr, functionShellFind, nullptr, nullptr);
		(Context::getGlobalObject())->setPropertyBySymbol(shellFindContext->symbolFunctionShellFind, defaultPrototypeFunction);
		shellFindContext->prototypeShellFind = defaultPrototypeFunction->prototype;
	};

	static TPointer<Variable> isShellFind(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-is-shell-find\n");
#endif
		return VariableBoolean::newVariable(TIsType<VariableShellFind>(arguments->index(0)));
	};

	static TPointer<Variable> findFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-find\n");
#endif
		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		if (((VariableShellFind *)this_)->value.find((arguments->index(0))->toString())) {
			this_->incReferenceCount();
			return this_;
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> findNext(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-find-next\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.next());
	};

	static TPointer<Variable> findClose(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-close\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableShellFind *)this_)->value.close();

		return Context::getValueUndefined();
	};

	static TPointer<Variable> isReadOnly(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-is-read-only\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.isReadOnly);
	};

	static TPointer<Variable> isDirectory(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-is-directory\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.isDirectory);
	};

	static TPointer<Variable> isFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-is-file\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.isFile);
	};

	static TPointer<Variable> name(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-name\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableString::newVariable(((VariableShellFind *)this_)->value.name);
	};

	static TPointer<Variable> isValid(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shellfind-is-valid\n");
#endif

		if (!TIsType<VariableShellFind>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(((VariableShellFind *)this_)->value);
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("ShellFind", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "ShellFind\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "ShellFind");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::ShellFind::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		newContext(executive, extensionId);

		executive->setFunction2("ShellFind.isShellFind(x)", isShellFind);
		executive->setFunction2("ShellFind.prototype.find(file)", findFile);
		executive->setFunction2("ShellFind.prototype.next()", findNext);
		executive->setFunction2("ShellFind.prototype.close()", findClose);
		executive->setFunction2("ShellFind.prototype.isReadOnly()", isReadOnly);
		executive->setFunction2("ShellFind.prototype.isDirectory()", isDirectory);
		executive->setFunction2("ShellFind.prototype.isFile()", isFile);
		executive->setFunction2("ShellFind.prototype.name()", name);
		executive->setFunction2("ShellFind.prototype.isValid()", isValid);
	};

};

#ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::ShellFind::initExecutive(executive, extensionId);
};
#endif
