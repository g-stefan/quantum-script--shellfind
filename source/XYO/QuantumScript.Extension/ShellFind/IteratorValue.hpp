// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_ITERATORVALUE_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_ITERATORVALUE_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_VARIABLESHELLFIND_HPP
#	include <XYO/QuantumScript.Extension/ShellFind/VariableShellFind.hpp>
#endif

namespace XYO::QuantumScript::Extension::ShellFind {

	class IteratorValue;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::ShellFind::IteratorValue> : public TMemoryPoolActive<XYO::QuantumScript::Extension::ShellFind::IteratorValue> {};
};

namespace XYO::QuantumScript::Extension::ShellFind {

	class IteratorValue : public Iterator {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(IteratorValue);

		public:
			TPointer<VariableShellFind> value_;
			XYO::System::ShellFind *sourceShellFind;

			inline IteratorValue(){};

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT bool next(Variable *out);

			inline void activeDestructor() {
				value_.deleteMemory();
			};
	};

};

#endif
