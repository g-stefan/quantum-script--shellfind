//
// Quantum Script Extension ShellFind
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_ITERATORVALUE_HPP
#define QUANTUM_SCRIPT_EXTENSION_SHELLFIND_ITERATORVALUE_HPP

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_VARIABLESHELLFIND_HPP
#include "quantum-script-extension-shellfind-variableshellfind.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				class IteratorValue;

			};
		};
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::ShellFind::IteratorValue>:
			public TMemoryPoolActive<Quantum::Script::Extension::ShellFind::IteratorValue> {};
	};
};

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				using namespace XYO;

				class IteratorValue :
					public Iterator {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(IteratorValue);
					public:

						TPointer<VariableShellFind> value_;
						XYO::ShellFind *sourceShellFind;

						inline IteratorValue() {
						};

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT bool next(TPointerX<Variable> &out);

						inline void activeDestructor() {
							value_.deleteMemory();
						};

				};

			};
		};
	};
};

#endif

