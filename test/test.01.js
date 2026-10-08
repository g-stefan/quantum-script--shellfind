// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("ShellFind");


function check(condition, name) {
	if (!condition) {
		throw "Test failed: " + name;
	};
	Console.writeLn("- " + name + ": ok");
};

// run from output/test
var path = "../../test";

// not valid: every query returns undefined, no crash
var scan = new ShellFind();
check(!scan.isValid(), "new ShellFind() not valid");
check(Script.isUndefined(scan.name()), "name() not valid");
check(Script.isUndefined(scan.isDirectory()), "isDirectory() not valid");
check(Script.isUndefined(scan.isFile()), "isFile() not valid");
check(Script.isUndefined(scan.isReadOnly()), "isReadOnly() not valid");
check(Script.isUndefined(scan.isLink()), "isLink() not valid");
check(Script.isUndefined(scan.find(path + "/nothing-matches-*")), "find() no match");
check(Script.isUndefined(scan.name()), "name() after no match");

// a single entry
check(scan.find(path + "/test.01.js") == scan, "find() returns this");
check(scan.isValid(), "isValid()");
check(scan.name() == "test.01.js", "name()");
check(scan.isFile() && !scan.isDirectory(), "isFile()");
check(scan.isLink() == false, "isLink() file");
check(scan.isReadOnly() == false, "isReadOnly()");
check(!scan.next(), "next() at end");
check(!scan.isValid(), "not valid at end");
check(Script.isUndefined(scan.name()), "name() at end");
check(Script.isUndefined(scan.isFile()), "isFile() at end");

// close
scan.find(path + "/*");
scan.close();
check(!scan.isValid(), "close()");
check(Script.isUndefined(scan.name()), "name() after close");

// the loop
var count = 0;
for (scan.find(path + "/test.01.*"); scan.isValid(); scan.next()) {
	++count;
};
check(count == 2, "loop");

// for ... of ends, and does nothing for an empty search
count = 0;
for (var name of ShellFind(path + "/test.01.*")) {
	if (++count > 2) {
		break;
	};
};
check(count == 2, "for ... of");
count = 0;
for (var name of ShellFind(path + "/nothing-matches-*")) {
	++count;
};
check(count == 0, "for ... of empty");
for (var name of new ShellFind()) {
	++count;
};
check(count == 0, "for ... of not searched");
