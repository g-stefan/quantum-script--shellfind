// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript.Extension/ShellFind/IteratorValue.hpp>

namespace XYO::QuantumScript::Extension::ShellFind {

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
