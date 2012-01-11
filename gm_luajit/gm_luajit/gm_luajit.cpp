#include "gm_luajit.h"
#include "luahelper.h"

namespace GLuaJIT {

lua_State *g_pLuaJIT = NULL;
lua_State *g_pGLua = NULL;

static int Print(lua_State *L)
{
	ILuaInterface *pLua = modulemanager->GetLuaInterface(g_pGLua);
	if( !pLua )
		return 0;

	ILuaObject* pPrint = pLua->GetGlobal("print");
	pPrint->Push();

	int nArgs = lua_gettop(L);
	for(int i = 0; i < nArgs; i++)
	{
		int nStackPos = i + 1;
		switch(lua_type(L, nStackPos))
		{
		case LUA_TNIL:
			{
				pLua->PushNil();
			}
			break;
		case LUA_TNUMBER:
			{
				pLua->Push((float)lua_tonumber(L, nStackPos));
			}
			break;
		case LUA_TSTRING:
			{
				pLua->PushDataString(lua_tostring(L, nStackPos), lua_strlen(L, nStackPos));
			}
			break;
		case LUA_TBOOLEAN:
			{
				pLua->Push( (bool)(lua_toboolean(L, nStackPos) != 0) );
			}
			break;
		}
	}
	pLua->Call(nArgs);

	return 0;
}

static int Run(lua_State *L)
{
	if( !L )
		return 0;

	g_pGLua = L;

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
			int nType = Lua()->GetType(nStackPos);

			switch( nType )
			{
			case GLua::TYPE_STRING:
				{
					LuaHelper::SetField(nCount, Lua()->GetString(nStackPos), Lua()->StringLength(nStackPos), g_pLuaJIT);
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
				break;
			case GLua::TYPE_TABLE:
				{
					LuaHelper::ConvertToGLuaJITTable(g_pLuaJIT, L, Lua()->GetObject(nStackPos));
				}
				break;
			case GLua::TYPE_LIGHTUSERDATA:
				{
					LuaHelper::SetField(nCount, Lua()->GetLightUserData(nStackPos), g_pLuaJIT);
				}
				break;
			case GLua::TYPE_ENTITY:
				{
					LuaHelper::UserData_t *pUserData = (LuaHelper::UserData_t*)lua_newuserdata(g_pLuaJIT, sizeof(LuaHelper::UserData_t));
					pUserData->pData = (void*)Lua()->GetReference(nStackPos);
					//lua_pop(g_pLuaJIT, 1);

					LuaHelper::SetField(nCount, (void*)pUserData, g_pLuaJIT);
				}
				break;
			default:
				{
					LuaHelper::UserData_t *pUserData = (LuaHelper::UserData_t*)lua_newuserdata(g_pLuaJIT, sizeof(LuaHelper::UserData_t));
					pUserData->pData = (void*)Lua()->GetReference(nStackPos);
					//lua_pop(g_pLuaJIT, 1);

					LuaHelper::SetField(nCount, (void*)pUserData, g_pLuaJIT);

					// Unsupported Type
					//Lua()->ErrorNoHalt("LuaJIT: Passing non-supported type - %s (Ignored)", Lua()->GetTypeName(nStackPos));
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
				Lua()->PushDataString( lua_tostring(g_pLuaJIT, -nStackPos), lua_strlen(g_pLuaJIT, -nStackPos) );
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
				Lua()->ErrorNoHalt("LuaJIT: Code is resulting a non-supported type - %ss  (Ignored)", LuaHelper::GetTypeName(nType));
			}
			break;
		}
	}

	lua_pop(g_pLuaJIT, nResultCount);

	return nResultCount;
}

// Called from LuaJIT to call GLua and result back to LuaJIT
static int GLuaCall(lua_State *L)
{
	ILuaInterface *pGLua = modulemanager->GetLuaInterface(g_pGLua);

	if( lua_gettop(L) == 0 )
		return 0;

	int nFunctionType = lua_type(L, 1);
	switch(nFunctionType)
	{
	case LUA_TLIGHTUSERDATA:
		{
			LuaHelper::UserData_t *pUserData = (LuaHelper::UserData_t*)lua_touserdata(L, 1);
			Lua()->PushReference((int)pUserData->pData);
		}
		break;
	case LUA_TSTRING:
		{
			const char* pszFunction = lua_tostring(L, 1);
			ILuaObject* pFunction = pGLua->GetGlobal(pszFunction);
			pFunction->Push();
		}
		break;
	default:
		{
			const char* pszTypeName = LuaHelper::GetTypeName(lua_type(L, 1));
			pGLua->Msg("GLuaJit Error: First parameter must be string, '%s' given", pszTypeName);
			return 0;
		}
		break;
	}

	int nArgs = lua_gettop(L) - 1;
	for(int i = 0; i < nArgs; i++)
	{
		int nStackPos = i + 2;
		switch(lua_type(L, nStackPos))
		{
		case LUA_TNIL:
			{
				pGLua->PushNil();
			}
			break;
		case LUA_TNUMBER:
			{
				pGLua->Push((float)lua_tonumber(L, nStackPos));
			}
			break;
		case LUA_TSTRING:
			{
				pGLua->PushDataString(lua_tostring(L, nStackPos), lua_strlen(L, nStackPos));
			}
			break;
		case LUA_TBOOLEAN:
			{
				pGLua->Push( (bool)(lua_toboolean(L, nStackPos) != 0) );
			}
			break;
		case LUA_TTABLE:
			{
				pGLua->Push( LuaHelper::ConvertToGLuaTable(g_pGLua, L, nStackPos) );
			}
			break;
		case LUA_TUSERDATA:
			{
				LuaHelper::UserData_t *pUserData = (LuaHelper::UserData_t*)lua_touserdata(L, nStackPos);
				pGLua->PushReference((int)pUserData->pData);
			}
			break;
		case LUA_TLIGHTUSERDATA:
			{
				LuaHelper::UserData_t *pUserData = (LuaHelper::UserData_t*)lua_touserdata(L, nStackPos);
				pGLua->PushReference((int)pUserData->pData);
			}
			break;

		}
	}

	pGLua->Call(nArgs);

	return 0;
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

	lua_register(g_pLuaJIT, "print", Print);
	lua_register(g_pLuaJIT, "GLuaCall", GLuaCall);

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