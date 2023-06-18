//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptExec.h"
#include "Log.h"
#include "LuaState.h"
#include "lua.hpp"
#include "common/Mutex.h"
#include "common/ScopedLock.h"
#include "common/Os.h"
#include <Clipbrd.hpp>
#include <psapi.h>
#include <string>
#include <assert.h>
#include <time.h>
#include <map>
#include <deque>

#pragma link "psapi.lib"

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

/** \brief Mutex protecting access to variables */
Mutex mutexVariables;
/** \brief Named variables - shared by scripts and plugins */
std::map<AnsiString, AnsiString> variables;

/** \brief Mutex protecting access to queues */
Mutex mutexQueues;
/** \brief Named queues - shared by scripts and plugins */
std::map<AnsiString, std::deque<AnsiString> > queues;

long timediff(clock_t t1, clock_t t2) {
	long elapsed;
	elapsed = static_cast<long>(((double)t2 - t1) / CLOCKS_PER_SEC * 1000);
	return elapsed;
}

struct {
	HWND hWndFound;
	const char* windowName;
	const char* exeName;
} findWindowData =
	{ NULL, NULL, NULL };

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	if (!hWnd)
		return TRUE;		// Not a window
	if (findWindowData.windowName)
	{
		char String[512];
		if (!SendMessage(hWnd, WM_GETTEXT, sizeof(String), (LPARAM)String))
		{
			return TRUE;		// No window title (length = 0)
		}
		if (strcmp(String, findWindowData.windowName))
		{
			return TRUE;
		}
	}
	if (findWindowData.exeName)
	{
		//HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		DWORD dwProcessId;
		DWORD dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);
		(void)dwThreadId;

		#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
		HANDLE hProcess;
		if (IsWinVistaOrLater())
		{
			hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, 0, dwProcessId);
		}
		else
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, dwProcessId);
		}
		BOOL result = TRUE;
		if (hProcess)
		{
			char exeName[512] = {0};
			if (GetProcessImageFileName(hProcess, exeName, sizeof(exeName)))
			{
				if (stricmp(exeName, findWindowData.exeName) == 0)
				{
					findWindowData.hWndFound = hWnd;
					result = FALSE;
				}
			}
			CloseHandle(hProcess);
		}
		return result;
	}
	return TRUE;
}

ScriptExec::CommonCallbacks cc;

std::vector<ScriptExec::Symbol> symbols;
bool globalsSetComplete = false;

inline void AddSymbol(const char* name, const char* brief, const char* description)
{
	struct ScriptExec::Symbol s;
	s.name = name;
	s.brief = brief;
	s.description = description;
	symbols.push_back(s);
}

inline void lua_register2(Lua_State &L, lua_CFunction fn, const char* name, const char* brief, const char* description)
{
	if (!globalsSetComplete)
	{
		AddSymbol(name, brief, description);
	}
	lua_register(L, name, fn);
}

}	// namespace


void ScriptExec::SetCommonCallbacks(const CommonCallbacks &callbacks)
{
	cc = callbacks;
}

int ScriptExec::SetVariable(const char* name, const char* value)
{
	ScopedLock<Mutex> lock(mutexVariables);
	variables[name] = value;
	return 0;
}

int ScriptExec::ClearVariable(const char* name)
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

void ScriptExec::QueuePush(const char* name, const char* value)
{
	ScopedLock<Mutex> lock(mutexQueues);
	std::deque<AnsiString> &que = queues[name];
	que.push_back(value);
}

int ScriptExec::QueuePop(const char* name, AnsiString &value)
{
	std::map<AnsiString, std::deque<AnsiString> >::iterator it;
	it = queues.find(name);
	if (it != queues.end())
	{
		std::deque<AnsiString> &que = it->second;
		if (que.empty())
		{
            return -2;
        }
		value = que[0];
		que.pop_front();
		return 0;
	}
	return -1;
}

int ScriptExec::QueueClear(const char* name)
{
	std::map<AnsiString, std::deque<AnsiString> >::iterator it;
	it = queues.find(name);
	if (it != queues.end())
	{
		queues.erase(it);
		return 0;
	}
	return -1;
}

int ScriptExec::QueueGetSize(const char* name)
{
	std::map<AnsiString, std::deque<AnsiString> >::iterator it;
	it = queues.find(name);
	if (it != queues.end())
	{
		return it->second.size();
	}
	return 0;
}

void ScriptExec::BreakAllScripts(void)
{
	std::map<lua_State*, ScriptExec*>::iterator it = contexts.begin();
	while (it != contexts.end()) {
		ScriptExec* se = it->second;
		se->Break();
		++it;
	}
}

void ScriptExec::WaitWhileAnyRunning(unsigned int ms)
{
	const int sleepPerPoll = 10;
	for (unsigned int i=0; i<ms/sleepPerPoll; i++) {
		std::map<lua_State*, ScriptExec*>::iterator it = contexts.begin();
		bool anyRunning = false;
		while (it != contexts.end()) {
			ScriptExec* se = it->second;
			if (se->isRunning()) {
				anyRunning = true;
				break;
			}
			++it;
		}
		if (anyRunning == false) {
			break;
		}
		Sleep(sleepPerPoll);
	}
}

class ScriptImp
{
public:


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

/** \note Sleep(ms) returns non-zero if break is called */
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

static int l_FindWindowByCaptionAndExeName(lua_State* L)
{
	static Mutex mutex;

	ScopedLock<Mutex> lock(mutex);
	findWindowData.hWndFound = NULL;

	findWindowData.windowName = lua_tostring(L, 1);
	const char* exeName = lua_tostring(L, 2);
	AnsiString dosExeName = "";
	if (exeName)
	{
		if (strlen(exeName) >= 2)
		{
			char targetPath[512];
			char drive[3];
			drive[0] = exeName[0];
			drive[1] = exeName[1];
			drive[2] = '\0';
			if (QueryDosDevice(drive, targetPath, sizeof(targetPath)))
			{
				dosExeName.cat_printf("%s%s", targetPath, &exeName[2]);
				findWindowData.exeName = dosExeName.c_str();
			}
		}
	}
	else
	{
		findWindowData.exeName = NULL;
	}
	if (findWindowData.windowName == NULL && findWindowData.exeName == NULL)
	{
		LOG("Lua: either window name or exe name is required\n");
		lua_pushnumber(L, 0);
		return 1;
	}

	::EnumWindows((WNDENUMPROC)EnumWindowsProc, NULL);

	lua_pushnumber(L, reinterpret_cast<unsigned int>(findWindowData.hWndFound));
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
	ScriptExec::SetVariable(name, value);
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
	ScriptExec::ClearVariable(name);
	return 0;
}

static int l_ClearAllVariables(lua_State* L)
{
	variables.clear();
	return 0;
}

static int l_QueuePush(lua_State* L)
{
	int argCnt = lua_gettop(L);
	if (argCnt < 2)
	{
		LOG("Lua error: missing argument(s) for QueuePush() - 2 arguments required, %d received\n", argCnt);
		return 0;
	}
	const char* queue_name = lua_tostring( L, 1 );
	if (queue_name == NULL)
	{
		LOG("Lua error: queue_name == NULL\n");
		return 0;
	}
	const char* value = lua_tostring( L, 2 );
	if (value == NULL)
	{
		LOG("Lua error: value == NULL\n");
		return 0;
	}
	ScriptExec::QueuePush(queue_name, value);
	return 0;
}

static int l_QueuePop(lua_State* L)
{
	const char* queue_name = lua_tostring( L, 1 );
	if (queue_name == NULL)
	{
		LOG("Lua error: queue_name == NULL\n");
		return 0;
	}
	AnsiString value;
	int ret = ScriptExec::QueuePop(queue_name, value);
	lua_pushstring(L, value.c_str());
	if (ret != 0)
	{
		lua_pushinteger(L, 0);
	}
	else
	{
		lua_pushinteger(L, 1);	// "valid"
	}
	return 2;
}

static int l_QueueClear(lua_State* L)
{
	const char* queue_name = lua_tostring( L, 1 );
	if (queue_name == NULL)
	{
		LOG("Lua error: queue_name == NULL\n");
		lua_pushinteger(L, -1);
		return 1;
	}
	int ret = ScriptExec::QueueClear(queue_name);
	lua_pushinteger(L, ret);
	return 1;
}

static int l_QueueGetSize(lua_State* L)
{
	const char* queue_name = lua_tostring( L, 1 );
	if (queue_name == NULL)
	{
		LOG("Lua error: queue_name == NULL\n");
		lua_pushinteger(L, 0);
		return 1;
	}
	int ret = ScriptExec::QueueGetSize(queue_name);
	lua_pushinteger(L, ret);
	return 1;
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
		enum ScriptSource srcType,
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

const std::vector<ScriptExec::Symbol>& ScriptExec::GetSymbols(void)
{
	return symbols;
}

void ScriptExec::Run(const char* script)
{
	breakReq = false;
	running = true;
	Lua_State L;
	luaL_openlibs(L);
	contexts[L] = this;

	// do not include internal/standard functions in help
	lua_register(L, "_ALERT", ScriptImp::LuaError);
	lua_register(L, "print", ScriptImp::LuaPrint);

	lua_register2(L, ScriptImp::l_ShowMessage, "ShowMessage", "Show simple message dialog", "Example: ShowMessage(\"text\")");
	lua_register2(L, ScriptImp::l_MessageBox, "MessageBox", "Show standard WinAPI MessageBox", "");
	lua_register2(L, ScriptImp::l_InputQuery, "InputQuery", "Display modal dialog allowing to take text input from the user", "");
	lua_register2(L, ScriptImp::l_Sleep, "Sleep", "Pause script for specified time (miliseconds)", "Function returns non-zero if it exited due to user break, zero if full delay passed. Example: Sleep(100).");
	lua_register2(L, ScriptImp::l_Beep, "Beep", "Equivalent of WinAPI Beep(frequency, time)", "Example: Beep(400, 250).");
	lua_register2(L, ScriptImp::l_CheckBreak, "CheckBreak", "Check if \"Break\" button was pressed by the user", "Allowing to interrupt scripts");
	lua_register2(L, ScriptImp::l_GetClipboardText, "GetClipboardText", "Get clipboard content as text", "");
	lua_register2(L, ScriptImp::l_SetClipboardText, "SetClipboardText", "Copy text to clipboard", "");
	lua_register2(L, ScriptImp::l_ForceDirectories, "ForceDirectories", "Make sure directory path exists, possibly creating folders recursively", "Equivalent of VCL function with same name.");
	lua_register2(L, ScriptImp::l_FindWindowByCaptionAndExeName, "FindWindowByCaptionAndExeName", "Search for window by caption and executable name", "");

	lua_register2(L, ScriptImp::l_SetVariable, "SetVariable", "Set value for variable with specified name", "Example: SetVariable(\"runcount\", count).");
	lua_register2(L, ScriptImp::l_GetVariable, "GetVariable", "Get variable value and isSet flag for variable with specified name", "Example: local count, var_isset = GetVariable(\"runcount\")");
	lua_register2(L, ScriptImp::l_ClearVariable, "ClearVariable", "Delete/unset variable with specified name", "Example: ClearVariable(\"runcount\")");
	lua_register2(L, ScriptImp::l_ClearAllVariables, "ClearAllVariables", "Delete/unset all variables", "");

	// QueuePush(queueName, stringValue)
	lua_register2(L, ScriptImp::l_QueuePush, "QueuePush", "Push string value to queue with specified name", "");
	lua_register2(L, ScriptImp::l_QueuePop, "QueuePop", "Get value from specified queue", "Example: local value, isValid = QueuePop(queueName)");
	lua_register2(L, ScriptImp::l_QueueClear, "QueueClear", "Clear specified queue", "");
	// local queue_size = QueueGetSize(queueName)
	lua_register2(L, ScriptImp::l_QueueGetSize, "QueueGetSize", "Get number of elements in specified queue", "");

	lua_register2(L, ScriptImp::l_GetExecSourceType, "GetExecSourceType", "Get type of event that triggered script execution", "See also: GetExecSourceId().");
	lua_register2(L, ScriptImp::l_GetExecSourceId, "GetExecSourceId", "Get ID of object that triggered script (depending on trigger type)", "See also: GetExecSourceType().");

	lua_register2(L, ScriptImp::l_GetExeName, "GetExeName", "Get name and full path of this executable",  "");

	lua_register2(L, ScriptImp::l_ClearOutput, "ClearOutput", "Clear log in script window", "");

	lua_register2(L, ScriptImp::l_GetCurrentFileName, "GetCurrentFileName", "Get the name of currently played file", "");
	lua_register2(L, ScriptImp::l_Stop, "Stop", "Stop playing current file", "");
	lua_register2(L, ScriptImp::l_Skip, "Skip", "Skip playing current file", "");
	lua_register2(L, ScriptImp::l_Play, "Play", "Play file", "");
	lua_register2(L, ScriptImp::l_MoveFile, "MoveFile", "Move (or rename) file from location A to B", "Usage: MoveFile(source, destination). Returns 0 on success.");

	lua_register2(L, ScriptImp::l_BringToFront, "BringToFront", "Bring application window to front", "");
	lua_register2(L, ScriptImp::l_ShellExecute, "ShellExecute", "Equivalent of WinAPI ShellExecute function", "");


	// add library
	luaL_requiref(L, "jsonedit_winapi", luaopen_jsonedit_winapi, 0);

	if (!globalsSetComplete)
	{
		// symbols from tsip_winapi
		AddSymbol("winapi.FindWindow", "WinAPI FindWindow equivalent", "");
		AddSymbol("winapi.SendMessage", "WinAPI SendMessage equivalent", "Example use: sending WM_CLOSE to other application");
		AddSymbol("winapi.Beep", "WinAPI Beep equivalent", "Same as Beep, example: Beep(frequencyHz, timeMs)");
		AddSymbol("winapi.MessageBox", "WinAPI MessageBox equivalent", "");
		AddSymbol("winapi.GetAsyncKeyState", "WinAPI GetAsyncKeyState equivalent", "Example use: modify button behavior depending on Ctrl/Alt/Shift state.");
		AddSymbol("winapi.PlaySound", "WinAPI PlaySound equivalent", "");
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


