#ifndef _RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_
#define _RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include "stdtypes.h"
#include "Utils.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

class ScriptContext
{
public:
	inline ScriptContext();
	inline ~ScriptContext();
	inline bool LoadScript(std::string fileName);
	inline bool Run();
	template<class T> inline bool GetVariableValue(std::string name, T &value);
    inline std::string GetLastMessage();
private:	

	template<class T> typename boost::enable_if<boost::is_float<T>, bool>::type  GetLuaStackValue(T &value);
	template<class T> typename boost::enable_if<boost::is_same<T, std::string>, bool>::type  GetLuaStackValue(T &value);
	template<class T> typename boost::enable_if<boost::is_signed<T>, bool>::type  GetLuaStackValue(T &value);
	template<class T> typename boost::enable_if<boost::is_unsigned<T>, bool>::type  GetLuaStackValue(T &value);
private:
	lua_State *luaState;
    bool started;
    std::string lastMessage;
};

ScriptContext::ScriptContext()
{
    this->luaState = luaL_newstate();
	luaL_requiref(this->luaState, "os", luaopen_os, 1);
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

template<class T> typename boost::enable_if<boost::is_unsigned<T>, bool>::type  ScriptContext::GetLuaStackValue(T &value)
{
	if(lua_isnumber(this->luaState, -1))
		value = lua_tounsigned(this->luaState, -1);
	else if(lua_isboolean(this->luaState, -1))
		value = lua_toboolean(this->luaState, -1);
	else
		return false;
	return true;
}

template<class T> typename boost::enable_if<boost::is_signed<T>, bool>::type  ScriptContext::GetLuaStackValue(T &value)
{
	if(lua_isnumber(this->luaState, -1) == 0) return false;
	value = lua_tointeger(this->luaState, -1);
	return true;
}

template<class T> typename boost::enable_if<boost::is_same<T, std::string>, bool>::type  ScriptContext::GetLuaStackValue(T &value)
{
	if(lua_isstring(this->luaState, -1) == 0) return false;
	value = lua_tostring(this->luaState, -1);
	return true;
}

template<class T> typename boost::enable_if<boost::is_float<T>, bool>::type  ScriptContext::GetLuaStackValue(T &value)
{
	if(lua_isnumber(this->luaState, -1) == 0) return false;
	value = lua_tonumber(this->luaState, -1);
	return true;
}

template<class T> bool ScriptContext::GetVariableValue(std::string name, T &value)
{
	std::vector<std::string> names;
	boost::split(names, name, boost::is_any_of("."));
	std::vector<std::string>::iterator it = names.begin();

	lua_getglobal(this->luaState, (*it).c_str());
	if(lua_isnil(this->luaState, -1)) return false;

	it++;

	if(it != names.end() && !lua_istable(this->luaState, -1))
		return false;

	while(it != names.end())
	{
		lua_getfield(this->luaState, -1, (*it).c_str());
		it++;

		if(it != names.end() && !lua_istable(this->luaState, -1))
			return false;
	}

	if(lua_isnil(this->luaState, -1)) return false;

	return GetLuaStackValue(value);	
}

std::string ScriptContext::GetLastMessage()
{
    return this->lastMessage;   
}

#endif //_RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_
