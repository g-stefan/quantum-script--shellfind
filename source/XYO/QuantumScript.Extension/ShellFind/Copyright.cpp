// Quantum Script Extension ShellFind
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/ShellFind/Copyright.hpp>
#include <XYO/QuantumScript.Extension/ShellFind/Copyright.rh>

namespace XYO::QuantumScript::Extension::ShellFind::Copyright {

	static const char *copyright_ = XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_COPYRIGHT;
	static const char *publisher_ = XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_PUBLISHER;
	static const char *company_ = XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_COMPANY;
	static const char *contact_ = XYO_QUANTUMSCRIPT_EXTENSION_SHELLFIND_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
