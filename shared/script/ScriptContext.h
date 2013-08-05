#ifndef _RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_
#define _RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_

#include <angelscript.h>
#include "stdtypes.h"
#include "scriptstdstring/scriptstdstring.h"
#include <boost/date_time.hpp>

#include <string>

class ScriptContext
{
public:
	inline ScriptContext();
	inline ~ScriptContext();
	inline bool LoadScript(std::string fileName);
	inline bool Bind();
	template<class T> inline bool GetVariableValue(std::string name, T &value);
private:
	inline void LoadScriptFile(std::string fileName, std::string &script);
	static inline void MessageCallback(const asSMessageInfo *msg, void *param);
	static inline std::string FormatCurrentDateTime(std::string format);
private:
	asIScriptEngine *engine;
	asIScriptModule *module;
	asSMessageInfo outputMessage;
	asIScriptContext *context;
	bool binded;
};

ScriptContext::ScriptContext()
{
	this->engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	this->engine->SetMessageCallback(asFUNCTION(ScriptContext::MessageCallback), this, asCALL_CDECL);
	this->module = this->engine->GetModule(NULL, asGM_ALWAYS_CREATE);
	this->context = this->engine->CreateContext();
	RegisterStdString(this->engine);

	this->engine->RegisterGlobalFunction("string FormatCurrentDateTime(string format)", asFUNCTION(ScriptContext::FormatCurrentDateTime), asCALL_CDECL);

	this->binded = false;
}

ScriptContext::~ScriptContext()
{	
	this->context->Release();
	this->engine->Release();
}

void ScriptContext::LoadScriptFile(std::string fileName, std::string &script)
{
	script = "";
	FILE *f = fopen(fileName.c_str(), "rb");
	if(f == NULL) return;
	fseek(f, 0, SEEK_END);
	int len = ftell(f);
	fseek(f, 0, SEEK_SET);
	script.resize(len);
	fread(&script[0], len, 1, f);
	fclose(f);
}

void ScriptContext::MessageCallback(const asSMessageInfo *msg, void *param)
{
	((ScriptContext*)param)->outputMessage = *msg;
}

bool ScriptContext::LoadScript(std::string fileName)
{
	if(this->binded) return false;
	std::string script;
	LoadScriptFile(fileName, script);
	if(script.empty()) return false;
	int r = this->module->AddScriptSection(fileName.c_str(), script.c_str());
	if(r < 0) return false;
	return true;
}
bool ScriptContext::Bind()
{
    this->binded = this->module->Build() >= 0;
	return this->binded;
}

template<class T> bool ScriptContext::GetVariableValue(std::string name, T &value)
{
	int idx = this->module->GetGlobalVarIndexByDecl(name.c_str());
	void* addr = this->module->GetAddressOfGlobalVar(idx);
	if(addr == NULL) return false;
	value =  *(T*)addr;
	return true;
}

std::string ScriptContext::FormatCurrentDateTime(std::string format)
{
	using namespace boost::posix_time;
	ptime now = second_clock::local_time();
	std::locale loc(std::wcout.getloc(),
		new time_facet(format.c_str()));
	std::basic_stringstream<char> ss;
	ss.imbue(loc);
	ss << now;
	return ss.str();
}


#endif //_RISKEMULIBRARY_SCRIPT_SCRIPTCONTEXT_H_