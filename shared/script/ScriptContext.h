#ifndef _RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_
#define _RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include "stdtypes.h"
#include "Utils.h"
#include <boost/date_time.hpp>
#include <string>

class ScriptContext
{
public:
	inline ScriptContext();
	inline ~ScriptContext();
	inline bool LoadScript(std::string fileName);
	inline bool Run();
	template<class T> inline bool GetVariableValue(std::string name, T &value);
	inline bool CheckExistVariables(std::vector<const char*> &vars);
    inline std::string GetLastMessage();
private:
	inline void LoadScriptFile(std::string fileName);
	static inline std::string FormatCurrentDateTime(std::string format);
private:
	lua_State *luaState;
    bool started;
    std::string lastMessage;
};

ScriptContext::ScriptContext()
{
    this->luaState = luaL_newstate();
    luaL_openlibs(this->luaState);
    this->started = false;
}

ScriptContext::~ScriptContext()
{	
	lua_close(this->luaState);
}

bool ScriptContext::LoadScript(std::string fileName)
{
	if(this->started) return false;	
    int r = luaL_loadfile(this->luaState, fileName.c_str());
    if(r != LUA_OK) this->lastMessage = lua_tostring(this->luaState, -1);
    return r == LUA_OK;
}

bool ScriptContext::Run()
{
    this->started = lua_pcall(this->luaState, 0, 0, 0) == LUA_OK;
    if(!this->started) this->lastMessage = lua_tostring(this->luaState, -1);
	return this->started;
}

template<class T> bool ScriptContext::GetVariableValue(std::string name, T &value)
{
    //value =  *(T*)addr;
	return true;
}

bool ScriptContext::CheckExistVariables(std::vector<const char*> &vars)
{
	std::vector<const char*>::iterator it = vars.begin();
	std::string msg;

	while(it != vars.end())
	{
		
	}
	this->lastMessage = msg;
	return this->lastMessage.empty();
}

std::string ScriptContext::GetLastMessage()
{
    return this->lastMessage;   
}

#endif //_RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_
