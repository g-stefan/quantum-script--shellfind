// Quantum Script Extension ProcessInteractive
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/ShellFind/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::ShellFind {

	class ShellFindContext : public Object {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(ShellFindContext);

		public:
			Symbol symbolFunctionShellFind;
			TPointerX<Prototype> prototypeShellFind;

			XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT ShellFindContext();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_EXPORT ShellFindContext *getContext();
};

#endif
