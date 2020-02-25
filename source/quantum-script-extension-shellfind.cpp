//
// Quantum Script Extension ShellFind
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-extension-shellfind.hpp"
#include "quantum-script-extension-shellfind-license.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_NO_VERSION
#include "quantum-script-extension-shellfind-version.hpp"
#endif
//
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenull.hpp"
//
#include "quantum-script-extension-shellfind-variableshellfind.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				using namespace XYO;
				using namespace Quantum::Script;

				ShellFindContext::ShellFindContext() {
					symbolFunctionShellFind = 0;
					prototypeShellFind.pointerLink(this);
				};

				ShellFindContext *getContext() {
					return TSingleton<ShellFindContext>::getValue();
				};

				static TPointer<Variable> functionShellFind(VariableFunction *function, Variable *this_, VariableArray *arguments) {
					TPointerX<Variable> &toFind(arguments->index(0));
					if(VariableUndefined::isVariableUndefined(toFind) ||
						VariableNull::isVariableNull(toFind)) {
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
					((Context::getGlobalObject())->operatorReferenceOwnProperty(shellFindContext->symbolFunctionShellFind))=defaultPrototypeFunction;
					shellFindContext->prototypeShellFind = defaultPrototypeFunction->prototype;
				};

				static TPointer<Variable> isShellFind(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-is-shell-find\n");
#endif
					return VariableBoolean::newVariable(VariableShellFind::isVariableShellFind(arguments->index(0)));
				};

				static TPointer<Variable> findFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-find\n");
#endif
					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					if(((VariableShellFind *)this_)->value.find((arguments->index(0))->toString())) {
						this_->incReferenceCount();
						return this_;
					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> findNext(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-find-next\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.next());
				};

				static TPointer<Variable> findClose(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-close\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableShellFind *) this_)->value.close();

					return Context::getValueUndefined();
				};


				static TPointer<Variable> isReadOnly(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-is-read-only\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.isReadOnly);
				};

				static TPointer<Variable> isDirectory(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-is-directory\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.isDirectory);
				};

				static TPointer<Variable> isFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-is-file\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableShellFind *)this_)->value.isFile);
				};

				static TPointer<Variable> name(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-name\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableString::newVariable(((VariableShellFind *)this_)->value.name);
				};

				static TPointer<Variable> isValid(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shellfind-is-valid\n");
#endif

					if(!VariableShellFind::isVariableShellFind(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(((VariableShellFind *)this_)->value);
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("ShellFind", initExecutive);
				};


				void initExecutive(Executive *executive, void *extensionId) {

					String info = "ShellFind\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "ShellFind");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::ShellFind::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					newContext(executive, extensionId);

					executive->setFunction2("ShellFind.isShellFind(x)", isShellFind);
					executive->setFunction2("ShellFind.prototype.find(file)",  findFile);
					executive->setFunction2("ShellFind.prototype.next()",  findNext);
					executive->setFunction2("ShellFind.prototype.close()",  findClose);
					executive->setFunction2("ShellFind.prototype.isReadOnly()",  isReadOnly);
					executive->setFunction2("ShellFind.prototype.isDirectory()",  isDirectory);
					executive->setFunction2("ShellFind.prototype.isFile()",  isFile);
					executive->setFunction2("ShellFind.prototype.name()",  name);
					executive->setFunction2("ShellFind.prototype.isValid()",  isValid);

				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::ShellFind::initExecutive(executive, extensionId);
};
#endif

