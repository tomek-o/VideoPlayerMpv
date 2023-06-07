//---------------------------------------------------------------------------

#ifndef ScriptExecH
#define ScriptExecH
//---------------------------------------------------------------------------

#include <string>
#include <set>
#include <System.hpp>

class LuaState;
struct lua_State;
extern "C" int luaopen_jsonedit_winapi (lua_State *L);


class ScriptExec
{
private:
	friend class LuaState;
	typedef void (__closure *CallbackAddOutputText)(const char* text);
	typedef void (__closure *CallbackClearOutput)(void);

	CallbackAddOutputText onAddOutputText;
	CallbackClearOutput onClearOutput;

	friend class ScriptImp;
	
	// let's put some winapi equivalents into library
	// sharing "Beep" and "MessageBox" - both in global namespace (backward compatibility) and in library
	friend int luaopen_jsonedit_winapi (lua_State *L);

	bool breakReq;
	bool running;
public:
	enum SrcType {
		SRC_TYPE_FORM_LUA = 0,
		SRC_TYPE_ON_FILE_SAVED,
        SRC_TYPE_ON_FILE_OPENED,
		SRC_TYPE_ON_HOTKEY,
		SRC_TYPE_ON_STARTUP,
		SRC_TYPE_ON_TIMER,

		SRC_TYPE_LIMITER
	};

	struct CommonCallbacks
	{
		typedef int (__closure *CallbackGetCurrentFileName)(AnsiString &filename);
		typedef void (__closure *CallbackStop)(void);
		typedef void (__closure *CallbackSkip)(void);
		typedef void (__closure *CallbackPlay)(void);

		CallbackGetCurrentFileName onGetCurrentFileName;
		CallbackStop onStop;
		CallbackSkip onSkip;
		CallbackPlay onPlay;

		CommonCallbacks(
			CallbackGetCurrentFileName onGetCurrentFileName,
			CallbackStop onStop,
			CallbackSkip onSkip,
			CallbackPlay onPlay
		):
			onGetCurrentFileName(onGetCurrentFileName),
			onStop(onStop),
			onSkip(onSkip),
			onPlay(onPlay)
		{
			assert(onGetCurrentFileName);
			assert(onStop);
			assert(onSkip),
			assert(onPlay);
		}
		CommonCallbacks(void):
			onGetCurrentFileName(NULL),
			onStop(NULL),
			onSkip(NULL),
			onPlay(NULL)
		{
		}
	};

	ScriptExec(
		enum SrcType srcType,
		int srcId,
		CallbackAddOutputText onAddOutputText,
		CallbackClearOutput onClearOutput
		);
	~ScriptExec();
	void Run(const char* script);
	void Break(void);
	bool isRunning(void) {
		return running;
	}
	static void SetCommonCallbacks(const CommonCallbacks &callbacks);

	static int l_Beep(lua_State *L);
	static int l_MessageBox(lua_State* L);

	static const std::set<AnsiString>& GetGlobals(void);
private:
	enum SrcType srcType;
	int srcId;
};

#endif
