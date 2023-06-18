//---------------------------------------------------------------------------

#ifndef ScriptExecH
#define ScriptExecH
//---------------------------------------------------------------------------

#include "ScriptSource.h"
#include <string>
#include <set>
#include <vector>
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
		enum ScriptSource srcType,
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

	static int SetVariable(const char* name, const char* value);
	static int ClearVariable(const char* name);

	/** \brief Add value to the end of the queue; queue is created if does not exist
		\param name queue name
	*/
	static void QueuePush(const char* name, const char* value);
	/**	\brief Try to take value from the front of the queue
		\param name queue name
		\param value returned value, valid if queue exists and was not empty
		\return 0 if value was successfully taken from queue
	*/
	static int QueuePop(const char* name, AnsiString &value);
	/** \brief Delete queue
		\param name queue name
		\return 0 on success (queue existed)
	*/
	static int QueueClear(const char* name);
	/** \brief Get number of elements in queue
		\param name queue name
		\return number of elements in queue; 0 if queue does not exist
	*/
	static int QueueGetSize(const char* name);

	static int l_Beep(lua_State *L);
	static int l_MessageBox(lua_State* L);
	/** \brief Set break flag for all currently active scripts
	*/
	static void BreakAllScripts(void);
	/** \brief Try to wait while any of the script is running
	*/
	static void WaitWhileAnyRunning(unsigned int ms);

	struct Symbol
	{
		AnsiString name;
		AnsiString brief;
		AnsiString description;
		bool operator<(const struct Symbol &other) const
		{
		   return name < other.name;
		}
	};

	static const std::vector<Symbol>& GetSymbols(void);
private:
	enum ScriptSource srcType;
	int srcId;
};

#endif
