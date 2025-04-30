// Quantum Script Extension ShellFind
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript.Extension/ShellFind/VariableShellFind.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/IteratorValue.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/Context.hpp>

namespace XYO::QuantumScript::Extension::ShellFind {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableShellFind, "{6F5DF4E6-0BBB-469D-B990-4AC7D36B8887}");

	const char *VariableShellFind::strTypeShellFind = "ShellFind";

	VariableShellFind::VariableShellFind() {
		XYO_DYNAMIC_TYPE_PUSH(VariableShellFind);
	};

	String VariableShellFind::getVariableType() {
		return strTypeShellFind;
	};

	Variable *VariableShellFind::newVariable() {
		return (Variable *)TMemory<VariableShellFind>::newMemory();
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
