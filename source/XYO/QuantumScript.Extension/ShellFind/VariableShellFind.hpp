// Quantum Script Extension ShellFind
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_VARIABLESHELLFIND_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_VARIABLESHELLFIND_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/ShellFind/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::ShellFind {

	class VariableShellFind;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::ShellFind::VariableShellFind> : public TMemoryPoolActive<XYO::QuantumScript::Extension::ShellFind::VariableShellFind> {};
};

namespace XYO::QuantumScript::Extension::ShellFind {

	class VariableShellFind : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableShellFind);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT, VariableShellFind);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT static const char *strTypeShellFind;

		public:
			XYO::System::ShellFind value;

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT VariableShellFind();

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT void activeDestructor();

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT TPointer<Iterator> getIteratorValue();

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT String toString();
	};

};

#endif
