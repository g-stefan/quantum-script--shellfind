//
// Quantum Script Extension ShellFind
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_VARIABLESHELLFIND_HPP
#define QUANTUM_SCRIPT_EXTENSION_SHELLFIND_VARIABLESHELLFIND_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				class VariableShellFind;

			};
		};

	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::ShellFind::VariableShellFind>:
			public TMemoryPoolActive<Quantum::Script::Extension::ShellFind::VariableShellFind> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				using namespace XYO;

				class VariableShellFind :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableShellFind);
						XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT, VariableShellFind);
					protected:
						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT static const char *strTypeShellFind;
					public:

						XYO::ShellFind value;

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT VariableShellFind();

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT void activeDestructor();

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT static Variable *newVariable();

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT String getVariableType();

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT Variable *instancePrototype();

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT TPointer<Iterator> getIteratorValue();

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT bool toBoolean();
						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT String toString();
				};


			};
		};
	};
};

#endif

