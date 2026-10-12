#pragma once

#include "pch.h"
#include "ScriptManager.h"

#include <iostream>
#include <sstream>

namespace Lua {
	int HelloFunc(lua_State* state) {
		int one = luaL_checkint(state, 1);
		int two = luaL_checkint(state, 2);
		int three = luaL_checkint(state, 3);

		std::stringstream ss;

		ss << "Hello Lua!";
		ss << " " << one << " " << two << " " << three;

		std::string result = ss.str();

		lua_pushfstring(state, result.c_str());

		return 1;
	}
}

#if BINGO_SCRIPT_LANG_PY
namespace Python {
	void HelloFunc() {
		std::cout << "Hello Python!";
	}
}
#endif

TEST(TestScriptManagerLua, TestExecute) {
	auto& scriptMan = Bingo::Script::Lua::ScriptManager_Lua::getSingleton();

	EXPECT_NO_THROW(scriptMan.execute("Hello.lua"));
}

TEST(TestScriptManagerLua, TestExecuteStr) {
	auto& scriptMan = Bingo::Script::Lua::ScriptManager_Lua::getSingleton();

	EXPECT_NO_THROW(scriptMan.executeStr("print('Evaluating lua code from string')"));
}

TEST(TestScriptManagerLua, TestRegister) {
	auto& scriptMan = Bingo::Script::Lua::ScriptManager_Lua::getSingleton();

	EXPECT_NO_THROW(scriptMan.registerFunction("HelloLua", Lua::HelloFunc));
}

TEST(TestScriptManagerLua, TestCall) {
	auto& scriptMan = Bingo::Script::Lua::ScriptManager_Lua::getSingleton();
	std::string result = "";

	scriptMan.registerFunction("HelloLua", Lua::HelloFunc);

	EXPECT_NO_THROW(result = scriptMan.call("HelloLua", 1, 2, 3));

	EXPECT_EQ(result, "Hello Lua! 1 2 3");
}

#if BINGO_SCRIPT_LANG_PY
TEST(TestScriptManagerPy, TestRegister) {
	auto& scriptMan = Bingo::Script::Python::ScriptManager_Py::getSingleton();

	EXPECT_NO_THROW(scriptMan.registerFunction("Hello", Python::HelloFunc));
}
#endif