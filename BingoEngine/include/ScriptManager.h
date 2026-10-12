//Dustin Gehm

#pragma once
#ifndef _SCRIPT_MANAGER_H
#define _SCRIPT_MANAGER_H

#define BINGO_SCRIPT_LANG_LUA 1
#define BINGO_SCRIPT_LANG_PY 0

#include <string>

#include "Core.h"
#include "Singleton.h"

#if BINGO_SCRIPT_LANG_LUA
//THESE FILES MUST BE INCLUDED AS C CODE!
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#endif

#if BINGO_SCRIPT_LANG_PY
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

		using Core::Manager;

		class ScriptManager : public Manager {
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

			/*class LuaState {
			public:
				LuaState(lua_State* lState) {
					state = lState;
				}

				void pushBool(bool val) {
					lua_pushboolean(state, val);
				}

				bool popBool() {
					//
				}
			private:
				lua_State* state;
			};

			typedef int (*ScriptFunc)(LuaState*);
			typedef int (*ScriptFunc)(lua_State*); */

			typedef lua_CFunction ScriptFunc;

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
					luaL_dofile(state, (scriptDir + "/" + filename).c_str());
				}

				virtual void executeStr(string code) {
					luaL_dostring(state, code.c_str());
				}

				void registerFunction(string name, ScriptFunc func) {
					lua_register(state, name.c_str(), func);
				}

				template<typename... Args>
				std::string call(string funcName, Args... args) {
					int numInputs = sizeof...(args);
					std::string result = "";

					// Get function from function name
					lua_getglobal(state, funcName.c_str());

					// push inputs onto stack
					push_var(args...);

					lua_pcall(state, numInputs, 1, 0);

					// Get string result and pop said string from stack
					result = lua_tostring(state, -1);

					lua_pop(state, 1);

					return result;
				}
			private:
				template<typename... Args>
				void push_var(const bool& first, Args... args) {
					lua_pushboolean(state, first);

					push_var(args...);
				}

				template<typename... Args>
				void push_var(const int& first, Args... args) {
					lua_pushnumber(state, first);

					push_var(args...);
				}

				template<typename... Args>
				void push_var(const double& first, Args... args) {
					lua_pushnumber(state, first);

					push_var(args...);
				}

				template<typename... Args>
				void push_var(const string& first, Args... args) {
					lua_pushlstring(state, first.data(), first.size());

					push_var(args...);
				}

				void push_var() {
					// Overload for when no arguments are left
				}

				lua_State* state;
			};

		}

#if BINGO_SCRIPT_LANG_PY
		namespace Python {

			class ScriptManager_Py : public ScriptManager, public Singleton<ScriptManager_Py> {
			public:
				ScriptManager_Py(string scriptDir)
					: ScriptManager(scriptDir) {
					interpGuard = std::make_shared<py::scoped_interpreter>();

					scope = py::module_::import("__main__").attr("__dict__");

					pyModule = py::module_::import("PyBingo");
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

				template<class... Args>
				void registerFunction(string name, void (*func)(Args... args) ) {
					pyModule.def(name.c_str(), func, args);
				}

			private:
				std::shared_ptr<py::scoped_interpreter> interpGuard;
				py::object scope;
				py::module_ pyModule;
			};

		}
#endif

	}

}

#endif