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
	inline std::string GetLastMessage();
private:
	inline void LoadScriptFile(std::string fileName, std::string &script);
	static inline void MessageCallback(const asSMessageInfo *msg, void *param);
	static inline std::string FormatCurrentDateTime(std::string format);
private:
	asIScriptEngine *engine;
	asIScriptModule *module;
	std::string lastMessage;
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

std::string ScriptContext::GetLastMessage()
{
	return this->lastMessage;	
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
	ScriptContext* pthis = ((ScriptContext*)param);
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";
	
	char tmp[2048];
	sprintf(tmp, "%s (%d, %d) : %s : %s", msg->section, 
		msg->row, msg->col, type, msg->message);

	pthis->lastMessage = tmp;
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
	if(idx == asERROR)
	{
		this->lastMessage = "The module was not built successfully";
		return false;
	}
	if(idx == asNO_GLOBAL_VAR)
	{
		this->lastMessage = "No matching global variable was found";
		return false;
	}
	if(idx == asINVALID_DECLARATION)
	{
		this->lastMessage = "The given declaration is invalid";
		return false;
	}
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