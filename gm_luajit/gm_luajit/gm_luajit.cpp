#include "gm_luajit.h"
#include "luahelper.h"

namespace GLuaJIT {

lua_State* g_pLuaJIT = NULL;

static int Run(lua_State *L)
{
	Lua()->CheckType(1, GLua::TYPE_STRING);

	std::string strCode(
		Lua()->GetString(1), 
		Lua()->StringLength(1)
		);

	lua_newtable(g_pLuaJIT);
	if( Lua()->GetStackTop() > 1 )
	{
		for(int nPos = 1, nCount = 1; nPos < Lua()->GetStackTop(); nPos++, nCount++)
		{
			int nStackPos = nPos + 1;
			switch( Lua()->GetType(nStackPos) )
			{
			case GLua::TYPE_STRING:
				{
					LuaHelper::SetField(nCount, Lua()->GetString(nStackPos), g_pLuaJIT);
				}
				break;
			case GLua::TYPE_NUMBER:
				{
					LuaHelper::SetField(nCount, Lua()->GetNumber(nStackPos), g_pLuaJIT);
				}	
				break;
			case GLua::TYPE_BOOL:
				{
					LuaHelper::SetField(nCount, Lua()->GetBool(nStackPos), g_pLuaJIT);
				}
				break;
			case GLua::TYPE_NIL:
				{
					LuaHelper::SetField(nCount, g_pLuaJIT);
				}
			default:
				{
					// Unsupported Type
					Lua()->ErrorNoHalt("LuaJIT: Passing non-supported type - %s (Ignored)", Lua()->GetTypeName(nStackPos));
				}
				break;
			}
		}
	}
	lua_setglobal(g_pLuaJIT, "_A");

	luaL_loadstring(g_pLuaJIT, strCode.c_str());

	int nCurrentState = lua_gettop(g_pLuaJIT);

	int nResult = lua_pcall(g_pLuaJIT, 0, LUA_MULTRET, 0);
	if( nResult != 0 )
	{
		const char* pszError = lua_tostring(g_pLuaJIT, -1);
		Lua()->Msg("LuaJIT: %s", pszError);
		return 0;
	}

	int nResultCount = (lua_gettop(g_pLuaJIT) - nCurrentState) + 1;
	for(int i = nResultCount; i > 0; i--)
	{
		int nStackPos = i;
		switch(lua_type(g_pLuaJIT, -nStackPos))
		{
		case LUA_TNUMBER:
			{
				Lua()->Push( (float)lua_tonumber(g_pLuaJIT, -nStackPos) );
			}
			break;
		case LUA_TSTRING:
			{
				Lua()->Push( lua_tostring(g_pLuaJIT, -nStackPos) );
			}
			break;
		case LUA_TBOOLEAN:
			{
				Lua()->Push( lua_toboolean(g_pLuaJIT, -nStackPos) != 0 );
			}
			break;
		case LUA_TNIL:
			{
				Lua()->PushNil();
			}
			break;
		default:
			{
				int nType = lua_type(g_pLuaJIT, -nStackPos);
				Lua()->ErrorNoHalt("LuaJIT: Code is resulting a non-supported type - %s  (Ignored)", LuaHelper::GetTypeName(nType));
			}
			break;
		}
	}

	lua_pop(g_pLuaJIT, nResultCount);

	return nResultCount;
}

int Startup(lua_State *L)
{
	g_pLuaJIT = lua_open();

	luaopen_io(g_pLuaJIT);
	luaopen_base(g_pLuaJIT);
	luaopen_table(g_pLuaJIT);
	luaopen_string(g_pLuaJIT);
	luaopen_math(g_pLuaJIT);
	luaopen_bit(g_pLuaJIT);
	luaopen_jit(g_pLuaJIT);
	//luaopen_ffi(L);
	luaopen_os(g_pLuaJIT);

	luaL_openlibs(g_pLuaJIT);

	Lua()->NewGlobalTable("LuaJIT");
	ILuaObject *pLuaJIT = Lua()->GetGlobal("LuaJIT");
	{
		pLuaJIT->SetMember("Run", Run);
	}
	pLuaJIT->UnReference();

	return 0;
}

int Cleanup(lua_State *L)
{
	lua_close(g_pLuaJIT);

	return 0;
}

}