//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptExec.h"
#include "Log.h"
#include "LuaState.h"
#include "lua.hpp"
#include "common/Mutex.h"
#include "common/ScopedLock.h"
#include <Clipbrd.hpp>
#include <string>
#include <map>
#include <assert.h>
#include <time.h>

namespace {
	std::map<lua_State*, ScriptExec*> contexts;

	ScriptExec* GetContext(lua_State* L)
	{
		std::map<lua_State*, ScriptExec*>::iterator it;
		it = contexts.find(L);
		if (it == contexts.end())
		{
			assert(!"Unregistered lua_State!");
			return NULL;
		}
		return it->second;
	}

	Mutex mutexVariables;
	std::map<AnsiString, AnsiString> variables;

	long timediff(clock_t t1, clock_t t2) {
		long elapsed;
		elapsed = static_cast<long>(((double)t2 - t1) / CLOCKS_PER_SEC * 1000);
		return elapsed;
	}

	ScriptExec::CommonCallbacks cc;	
std::set<AnsiString> globalsSet;
bool globalsSetComplete = false;
#define lua_register2(L,n,f) if (!globalsSetComplete) globalsSet.insert(n); lua_register(L,n,f)

}	// namespace


void ScriptExec::SetCommonCallbacks(const CommonCallbacks &callbacks)
{
	cc = callbacks;
}

class ScriptImp
{
public:

static int SetVariable(const char* name, const char* value)
{
	ScopedLock<Mutex> lock(mutexVariables);
	variables[name] = value;
	return 0;
}

static int ClearVariable(const char* name)
{
	ScopedLock<Mutex> lock(mutexVariables);
	std::map<AnsiString, AnsiString>::iterator it;
	it = variables.find(name);
	if (it != variables.end())
	{
        variables.erase(it);
	}
	return 0;
}



static int LuaPrint(lua_State *L)
{
	int nArgs = lua_gettop(L);
	int i;
	lua_getglobal(L, "tostring");
	std::string ret;//this is where we will dump the output
	//make sure you start at 1 *NOT* 0
	for(i = 1; i <= nArgs; i++)
	{
		const char *s;
		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);
		if(s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to ", LUA_QL("print"));

		if(i > 1) ret.append("\t");

		ret.append(s);
		lua_pop(L, 1);
	};
	//ret.append("\n");
	GetContext(L)->onAddOutputText(ret.c_str());
	return 0;
};

static int LuaError( lua_State *L )
{
	const char* str = lua_tostring( L, -1 );
	lua_pop(L, 1);
	//send string to console
	GetContext(L)->onAddOutputText(str);
	return 0;
}

static int l_ShowMessage( lua_State* L )
{
	AnsiString msg = lua_tostring(L, -1);
	MessageBox(NULL, msg.c_str(), "Lua message", MB_ICONINFORMATION);
	lua_pushnumber( L, 0 );
	return 1;
}

/** Beep(frequency, time_ms)
	\note blocking and not interruptable
*/
static int l_Beep(lua_State *L)
{
	/*
	To refer to elements in the stack, the API uses indices. The first element in the stack (that is, the element that was pushed first) has index 1, the next one has index 2, and so on. We can also access elements using the top of the stack as our reference, using negative indices. In that case, -1 refers to the element at the top (that is, the last element pushed), -2 to the previous element, and so on. For instance, the call lua_tostring(L, -1) returns the value at the top of the stack as a string.
	*/
	int freq = static_cast<int>(lua_tointegerx(L, 1, NULL));
	int time = static_cast<int>(lua_tointegerx(L, 2, NULL));
	Beep(freq, time);
	return 0;
}

static int l_MessageBox(lua_State* L)
{
	const char* msg = lua_tostring(L, 1);
	const char* title = lua_tostring(L, 2);
	unsigned int flags = static_cast<unsigned int>(lua_tointegerx(L, 3, NULL));
	int ret = MessageBox(Application->Handle, msg, title, flags);
	lua_pushnumber(L, ret);
	return 1;
}

static int l_InputQuery(lua_State* L)
{
	const char* caption = lua_tostring(L, 1);
	const char* prompt = lua_tostring(L, 2);
	AnsiString text = lua_tostring(L, 3);
	bool ret = InputQuery(caption, prompt, text);
	lua_pushstring(L, text.c_str());
	lua_pushboolean(L, ret);
	return 2;
}

static int l_Sleep(lua_State* L)
{
	int ret = 0;
	int time = static_cast<int>(lua_tointegerx(L, -1, NULL));
	clock_t t1 = clock();
	long elapsed = 0;
	if (GetContext(L)->breakReq)
	{
		ret = -1;
	}
	else
	{
		while (elapsed < time)
		{
			if (GetContext(L)->breakReq)
			{
				ret = -1;
				break;
			}
			Application->ProcessMessages();
			Sleep(1);
			elapsed = timediff(t1, clock());
			if (GetContext(L)->breakReq)
			{
				ret = -1;
				break;
			}
		}
	}
	lua_pushnumber(L, ret);
	return 1;					// number of return values
}

static int l_GetClipboardText( lua_State* L )
{
	AnsiString text = Clipboard()->AsText;
	lua_pushstring( L, text.c_str() );
	return 1;
}

static int l_SetClipboardText( lua_State* L )
{
	const char* str = lua_tostring( L, -1 );
	if (str == NULL)
	{
		LOG("Lua error: str == NULL\n");
		return 0;
	}
	Clipboard()->AsText = str;
	return 0;
}

/** \return 0 on success */
static int l_ForceDirectories(lua_State* L)
{
	const char* str = lua_tostring(L, -1);
	if (DirectoryExists(str))
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	int ret = -1;
	if (ForceDirectories(str))
	{
		ret = 0;
	}
	lua_pushnumber(L, ret);
	return 1;
}

static int l_SetVariable(lua_State* L)
{
	const char* name = lua_tostring( L, 1 );
	if (name == NULL)
	{
		LOG("Lua error: name == NULL\n");
		return 0;
	}
	const char* value = lua_tostring( L, 2 );
	if (value == NULL)
	{
		LOG("Lua error: value == NULL\n");
		return 0;
	}
	SetVariable(name, value);
	return 0;
}

static int l_GetVariable(lua_State* L)
{
	const char* name = lua_tostring( L, 1 );
	if (name == NULL)
	{
		LOG("Lua error: name == NULL\n");
		return 0;
	}
	ScopedLock<Mutex> lock(mutexVariables);
	std::map<AnsiString, AnsiString>::iterator it;
	it = variables.find(name);
	if (it != variables.end())
	{
		// returning two variables, second one informs if variable is found
		lua_pushstring( L, it->second.c_str() );
		lua_pushinteger( L, 1 );
	}
	else
	{
        // void lua_pushnil (lua_State *L);
		lua_pushstring( L, "" );
		lua_pushinteger( L, 0 );
	}
	return 2;
}

static int l_ClearVariable(lua_State* L)
{
	const char* name = lua_tostring( L, 1 );
	if (name == NULL)
	{
		LOG("Lua error: name == NULL\n");
		return 0;
	}	
	ClearVariable(name);
	return 0;
}

static int l_ClearAllVariables(lua_State* L)
{
	variables.clear();
	return 0;
}

static int l_ShellExecute(lua_State* L)
{
	const char* verb = lua_tostring( L, 1 );
	const char* file = lua_tostring( L, 2 );
	const char* parameters = lua_tostring( L, 3 );
	const char* directory = lua_tostring( L, 4 );
	int showCmd = static_cast<int>(lua_tointeger( L, 5 ));
	int ret = reinterpret_cast<int>(ShellExecute(NULL, verb, file, parameters, directory, showCmd));
	lua_pushinteger(L, ret);
	return 1;
}

static int l_GetExecSourceType(lua_State* L)
{
	lua_pushinteger(L, GetContext(L)->srcType);
	return 1;
}

static int l_GetExecSourceId(lua_State* L)
{
	lua_pushinteger(L, GetContext(L)->srcId);
	return 1;
}

static int l_GetExeName(lua_State* L)
{
	lua_pushstring(L, Application->ExeName.c_str());
	return 1;
}

/** \brief Check if user interrupted script for clean, controlled exit
*/
static int l_CheckBreak(lua_State *L)
{
	int ret = GetContext(L)->breakReq?1:0;
	lua_pushnumber(L, ret);
	return 1;					// number of return values
}

static int l_ClearOutput(lua_State *L)
{
	GetContext(L)->onClearOutput();
	return 0;					// number of return values
}

static int l_GetCurrentFileName(lua_State *L)
{
	AnsiString filename;
	if (cc.onGetCurrentFileName)
	{
		int ret = cc.onGetCurrentFileName(filename);
		(void)ret;
    }
	lua_pushstring(L, filename.c_str());
	return 1;
}

static int l_Stop(lua_State *L)
{
	if (cc.onStop)
		cc.onStop();
	return 0;
}

static int l_Skip(lua_State *L)
{
	if (cc.onSkip)
		cc.onSkip();
	return 0;
}

static int l_Play(lua_State *L)
{
	if (cc.onPlay)
		cc.onPlay();
	return 0;
}

static int l_BringToFront(lua_State *L)
{
	Application->BringToFront();
	return 0;
}

static int l_MoveFile(lua_State *L)
{
	int ret = -1;
	const char* src = lua_tostring( L, 1 );
	if (src == NULL)
	{
		LOG("Lua error: MoveFile source == NULL\n");
		lua_pushnumber(L, ret);
		return 1;
	}
	const char* dst = lua_tostring( L, 2 );
	if (dst == NULL)
	{
		LOG("Lua error: MoveFile dst == NULL\n");
		lua_pushnumber(L, ret);
		return 1;
	}

	if (MoveFile(src, dst) != 0)
	{
    	ret = 0;
	}

	lua_pushnumber(L, ret);
	return 1;
}

};	// ScriptImp

ScriptExec::ScriptExec(
		enum SrcType srcType,
		int srcId,
		CallbackAddOutputText onAddOutputText,
		CallbackClearOutput onClearOutput
		):
	srcType(srcType),
	srcId(srcId),
	onAddOutputText(onAddOutputText),
	onClearOutput(onClearOutput),
	breakReq(false),
	running(false)
{
	assert(onAddOutputText);
	assert(onClearOutput);
}

ScriptExec::~ScriptExec()
{
}

const std::set<AnsiString>& ScriptExec::GetGlobals(void)
{
	return globalsSet;
}

void ScriptExec::Run(const char* script)
{
	breakReq = false;
	running = true;
	Lua_State L;
	luaL_openlibs(L);
	contexts[L] = this;

	lua_register2(L, "_ALERT", ScriptImp::LuaError );
	lua_register2(L, "print", ScriptImp::LuaPrint );

	lua_register2(L, "ShowMessage", ScriptImp::l_ShowMessage);
	lua_register2(L, "MessageBox", ScriptImp::l_MessageBox);
	lua_register2(L, "InputQuery", ScriptImp::l_InputQuery);
	lua_register2(L, "Sleep", ScriptImp::l_Sleep);
	lua_register2(L, "Beep", ScriptImp::l_Beep);
	lua_register2(L, "GetClipboardText", ScriptImp::l_GetClipboardText);
	lua_register2(L, "SetClipboardText", ScriptImp::l_SetClipboardText);
	lua_register2(L, "ForceDirectories", ScriptImp::l_ForceDirectories);
	lua_register2(L, "SetVariable", ScriptImp::l_SetVariable);
	lua_register2(L, "GetVariable", ScriptImp::l_GetVariable);
	lua_register2(L, "ClearVariable", ScriptImp::l_ClearVariable);
	lua_register2(L, "ClearAllVariables", ScriptImp::l_ClearAllVariables);
	lua_register2(L, "GetExecSourceType", ScriptImp::l_GetExecSourceType);
	lua_register2(L, "GetExecSourceId", ScriptImp::l_GetExecSourceId);
	lua_register2(L, "GetExeName", ScriptImp::l_GetExeName);
	lua_register2(L, "CheckBreak", ScriptImp::l_CheckBreak);
	lua_register2(L, "ClearOutput", ScriptImp::l_ClearOutput);

	lua_register2(L, "GetCurrentFileName", ScriptImp::l_GetCurrentFileName);
	lua_register2(L, "Stop", ScriptImp::l_Stop);
	lua_register2(L, "Skip", ScriptImp::l_Skip);
	lua_register2(L, "Play", ScriptImp::l_Play);
	lua_register2(L, "MoveFile", ScriptImp::l_MoveFile);

	lua_register2(L, "BringToFront", ScriptImp::l_BringToFront);
	lua_register2(L, "ShellExecute", ScriptImp::l_ShellExecute);


	// add library
	luaL_requiref(L, "jsonedit_winapi", luaopen_jsonedit_winapi, 0);

	if (!globalsSetComplete)
	{
		globalsSet.insert("winapi.FindWindow");
		globalsSet.insert("winapi.SendMessage");
		globalsSet.insert("winapi.Beep");
		globalsSet.insert("winapi.MessageBox");
		globalsSet.insert("winapi.GetAsyncKeyState");
		globalsSet.insert("winapi.PlaySound");
	}
	globalsSetComplete = true;

	int res = luaL_dostring(L, script);
	if(res != 0)
	{
		AnsiString txt;
		txt.sprintf("Execution error:\n%s", lua_tostring(L, -1));
		MessageBox(NULL, txt.c_str(), "Lua", MB_ICONINFORMATION);
	}
	running = false;

	std::map<lua_State*, ScriptExec*>::iterator it;
	it = contexts.find(L);
	assert(it != contexts.end());
	contexts.erase(it);
}

void ScriptExec::Break(void)
{
	breakReq = true;
}

int ScriptExec::l_Beep(lua_State *L)
{
	return ScriptImp::l_Beep(L);
}

int ScriptExec::l_MessageBox(lua_State* L)
{
	return ScriptImp::l_MessageBox(L);
}


