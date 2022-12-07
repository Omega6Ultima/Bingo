//Dustin Gehm

#pragma once
#ifndef _SCRIPT_MANAGER_H
#define _SCRIPT_MANAGER_H

#define S_LANG_LUA 1
#define S_LANG_PY 1

#include <string>

#include "Singleton.h"

#if S_LANG_LUA
//THESE FILES MUST BE INCLUDED AS C CODE!
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#endif

#if S_LANG_PY
#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>

namespace py = pybind11;
#endif

/*
* make script state a class that encapsulates API calls
* setup diff projects 1 for lua, 1 for python
*/

namespace Bingo {

	namespace Script {

		class ScriptManager {
		public:
			ScriptManager(string scriptDirectory)
				:scriptDir(scriptDirectory) {
				//
			}

			virtual ~ScriptManager() {
				//
			}

			virtual void execute(string filename) = 0;
			virtual void executeStr(string code) = 0;
		protected:
			string scriptDir;
		};

		namespace Lua {

			//class LuaState {
			//public:
			//	LuaState(lua_State* lState) {
			//		state = lState;
			//	}

			//	void pushBool(bool val) {
			//		lua_pushboolean(state, val);
			//	}

			//	bool popBool() {
			//		//
			//	}
			//private:
			//	lua_State* state;
			//};

			//typedef int (*ScriptFunc)(LuaState*);
			typedef int (*ScriptFunc)(lua_State*);

			class ScriptManager_Lua : public ScriptManager, public Singleton<ScriptManager_Lua> {
			public:
				ScriptManager_Lua(string scriptDir)
					:ScriptManager(scriptDir) {

					state = lua_open();

					luaL_openlibs(state);
				}

				virtual ~ScriptManager_Lua() {
					lua_close(state);
				}

				virtual void execute(string filename) {
					luaL_dofile(state, (scriptDir + filename).c_str());
				}

				virtual void executeStr(string code) {
					luaL_dostring(state, code.c_str());
				}

				void registerFunction(string name, ScriptFunc func) {
					lua_register(state, name.c_str(), func);
				}

				void call(string funcName) {
					lua_getglobal(state, funcName.c_str());
					//

					//lua_call(state, narg, nresults);
				}
			private:
				lua_State* state;
			};

		}

		namespace Python {

#define PY_REG_START(pyModule, modDocStr) PYBIND11_MODULE(pyModule, m) { m.doc() = modDocStr;
#define PY_REG_END() }

#define PY_REG_FUNC_3(name, functionName, docStr) m.def(#name, &functionName, docStr);
#define PY_REG_FUNC_2(name, functionName) PY_REG_FUNC_3(name, functionName, "")
#define PY_REG_FUNC_1(name) PY_REG_FUNC_2(name, name)
#define PY_REG_FUNC_0() 

			//4 being the max number of args + 1
#define PY_REG_FUNC_X(f1, f2, f3, f4, ...) f4

#define FUNC_RECOMPOSER(args) PY_REG_FUNC_X args

#define CHOOSE_FROM_ARG_COUNT(...) FUNC_RECOMPOSER((__VA_ARGS__, PY_REG_FUNC_3, PY_REG_FUNC_2, PY_REG_FUNC_1, ))

#define NO_ARG_EXPANDER() ,,,,PY_REG_FUNC_0

#define MACRO_CHOOSER(...) CHOOSE_FROM_ARG_COUNT(NO_ARG_EXPANDER __VA_ARGS__ ())

			//(token, token, string, token)
#define PY_REG_FUNC(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

			class ScriptManager_Py : public ScriptManager, public Singleton<ScriptManager_Py> {
			public:
				ScriptManager_Py(string scriptDir)
					: ScriptManager(scriptDir) {
					interpGuard = std::make_shared<py::scoped_interpreter>();

					scope = py::module_::import("__main__").attr("__dict__");
				}

				virtual ~ScriptManager_Py() {
					//
				}

				virtual void execute(string filename) {
					py::eval_file(scriptDir + filename, scope);
				}

				virtual void executeStr(string code) {
					py::eval(code, scope);
				}
			private:
				std::shared_ptr<py::scoped_interpreter> interpGuard;
				py::object scope;
			};

		}

	}

}

#endif