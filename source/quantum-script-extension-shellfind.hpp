//
// Quantum Script Extension ShellFind
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_HPP
#define QUANTUM_SCRIPT_EXTENSION_SHELLFIND_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND__EXPORT_HPP
#include "quantum-script-extension-shellfind--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_COPYRIGHT_HPP
#include "quantum-script-extension-shellfind-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_LICENSE_HPP
#include "quantum-script-extension-shellfind-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELLFIND_VERSION_HPP
#include "quantum-script-extension-shellfind-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace ShellFind {

				using namespace Quantum::Script;

				class ShellFindContext:
					public Object {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(ShellFindContext);
					public:

						Symbol symbolFunctionShellFind;
						TPointerX<Prototype> prototypeShellFind;

						QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT ShellFindContext();
				};

				QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT ShellFindContext *getContext();

				QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_SHELLFIND_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

