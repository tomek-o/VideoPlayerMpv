//---------------------------------------------------------------------------
#pragma hdrstop

#include "LuaWinapi.h"
#include "ScriptExec.h"	// sharing some functions
#include "lua.hpp"

//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace
{

int l_WinapiFindWindow(lua_State* L)
{
	const char* className = lua_tostring(L, 1);
	const char* windowName = lua_tostring(L, 2);
	HWND hWnd = FindWindow(className, windowName);
	lua_pushnumber(L, reinterpret_cast<unsigned int>(hWnd));
	return 1;
}

int l_WinapiSendMessage(lua_State* L)
{
	unsigned int hWnd = static_cast<unsigned int>(lua_tointegerx(L, 1, NULL));
	unsigned int msg = static_cast<unsigned int>(lua_tointegerx(L, 2, NULL));
	unsigned int wParam = static_cast<unsigned int>(lua_tointegerx(L, 3, NULL));
	unsigned int lParam = static_cast<unsigned int>(lua_tointegerx(L, 4, NULL));
	int ret = SendMessage((HWND)hWnd, msg, wParam, lParam);
	lua_pushnumber(L, ret);
	return 1;
}

int l_WinapiGetAsyncKeyState(lua_State* L)
{
	int vKey = static_cast<unsigned int>(lua_tointegerx(L, 1, NULL));
	int ret = GetAsyncKeyState(vKey);
	lua_pushnumber(L, ret);
	return 1;
}

int l_WinapiPlaySound(lua_State* L)
{
	const char* pszSound = lua_tostring(L, 1);
	unsigned int hmod = static_cast<unsigned int>(lua_tointegerx(L, 2, NULL));
	unsigned fdwSound = static_cast<unsigned int>(lua_tointegerx(L, 3, NULL));
	int ret = PlaySound(pszSound, reinterpret_cast<HMODULE>(hmod), fdwSound);
	lua_pushnumber(L, ret);
	return 1;
}

}	// namespace


int luaopen_jsonedit_winapi (lua_State *L)
{
	static const struct luaL_Reg tsip_winapi[] = {
		{"FindWindow", l_WinapiFindWindow},
		{"SendMessage", l_WinapiSendMessage},
		{"Beep", ScriptExec::l_Beep},
		{"MessageBox", ScriptExec::l_MessageBox},
		{"GetAsyncKeyState", l_WinapiGetAsyncKeyState},
		{"PlaySound", l_WinapiPlaySound},
		{NULL, NULL}
	};
	luaL_newlib(L, tsip_winapi);
	return 1;
}