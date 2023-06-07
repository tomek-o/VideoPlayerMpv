//---------------------------------------------------------------------------
#ifndef LuaWinapiH
#define LuaWinapiH
//---------------------------------------------------------------------------

/** \brief Separating few WinAPI equivalent functions into Lua library
*/

struct lua_State;

extern "C" int luaopen_jsonedit_winapi (lua_State *L);

#endif
