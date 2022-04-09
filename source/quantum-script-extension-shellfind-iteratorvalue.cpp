﻿//
// Quantum Script Library
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-shellfind.hpp"
#include "quantum-script-extension-shellfind-iteratorvalue.hpp"

#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				using namespace XYO;

				bool IteratorValue::next(Variable *out) {
					if (!sourceShellFind) {
						out->referenceSet(Context::getValueUndefined());
						return false;
					};
					out->referenceSet(VariableString::newVariable(sourceShellFind->name));
					sourceShellFind->next();
					return true;
				};

			};
		};
	};
};
