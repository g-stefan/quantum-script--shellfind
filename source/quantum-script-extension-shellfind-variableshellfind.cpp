//
// Quantum Script Extension ShellFind
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-extension-shellfind.hpp"
#include "quantum-script-extension-shellfind-variableshellfind.hpp"
#include "quantum-script-extension-shellfind-iteratorvalue.hpp"


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				using namespace XYO;

				XYO_DYNAMIC_TYPE_IMPLEMENT(VariableShellFind, "{6F5DF4E6-0BBB-469D-B990-4AC7D36B8887}");

				const char *VariableShellFind::strTypeShellFind = "ShellFind";

				VariableShellFind::VariableShellFind() {
					XYO_DYNAMIC_TYPE_PUSH(VariableShellFind);
				};

				String VariableShellFind::getVariableType() {
					return strTypeShellFind;
				};

				Variable *VariableShellFind::newVariable() {
					return (Variable *) TMemory<VariableShellFind>::newMemory();
				};

				Variable *VariableShellFind::instancePrototype() {
					return (Extension::ShellFind::getContext())->prototypeShellFind->prototype;
				};

				void VariableShellFind::activeDestructor() {
					value.close();
				};

				TPointer<Iterator> VariableShellFind::getIteratorValue() {
					IteratorValue *iterator_ = (IteratorValue *)TMemory<IteratorValue>::newMemory();
					iterator_->value_ = this;
					iterator_->sourceShellFind = &this->value;
					return iterator_;
				};

				bool VariableShellFind::toBoolean() {
					return true;
				};

				String VariableShellFind::toString() {
					return strTypeShellFind;
				};

			};
		};
	};
};


