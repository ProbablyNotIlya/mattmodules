#if !defined(LUAHELPER_H)
#define LUAHELPER_H

#if defined(_MSC_VER)
#pragma once
#endif

#include "luajit/lua.hpp"

namespace GLuaJIT {

extern lua_State *g_pGLua;

}

namespace LuaHelper {

struct UserData_t
{
	void* pData;
	int nType;
};

inline void Push(lua_State *L, ILuaObject *pObj)
{
	int nType = pObj->GetType();

	switch(nType)
	{
	case GLua::TYPE_NUMBER:
		{
			lua_pushnumber(L, (float)pObj->GetFloat());
		}
		break;
	case GLua::TYPE_STRING:
		{
			lua_pushstring(L, pObj->GetString());
		}
		break;
	case GLua::TYPE_BOOL:
		{
			lua_pushboolean(L, pObj->GetBool());
		}
		break;
	case GLua::TYPE_NIL:
		{
			lua_pushnil(L);
		}
		break;
	default:
		{
			ILuaInterface *pGLua = modulemanager->GetLuaInterface(GLuaJIT::g_pGLua);

			pGLua->Push(pObj);
			int nReference = pGLua->GetReference(-1, true);

			UserData_t *pUserData = (UserData_t*)lua_newuserdata(L, sizeof(UserData_t));
			pUserData->pData = (void*)nReference;
		}
	}
}

inline void SetField(ILuaObject *pKey, ILuaObject *pVal, lua_State *L)
{
	Push(L, pKey);
	Push(L, pVal);
	lua_rawset(L, -3);
}

inline void SetField(const char *index, int val, lua_State *L)
{
	lua_pushstring(L, index);
	lua_pushnumber(L, val);
	lua_rawset(L, -3);
}

inline void SetField(const char *index, float val, lua_State *L)
{
	lua_pushstring(L, index);
	lua_pushnumber(L, val);
	lua_rawset(L, -3);
}

inline void SetField(const char *index, bool val, lua_State *L)
{
	lua_pushstring(L, index);
	lua_pushboolean(L, val);
	lua_rawset(L, -3);
}

inline void SetField(const char *index, const char *val, lua_State *L)
{
	lua_pushstring(L, index);
	lua_pushstring(L, val);
	lua_rawset(L, -3);
}

inline void SetField(const char *index, const char *val, size_t len, lua_State *L)
{
	lua_pushstring(L, index);
	lua_pushlstring(L, val, len);
	lua_rawset(L, -3);
}

inline void SetField(const char *index, lua_State *L)
{
	lua_pushstring(L, index);
	lua_pushnil(L);
	lua_rawset(L, -3);
}

inline void SetField(int index, int val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushnumber(L, val);
	lua_rawset(L, -3);
}

inline void SetField(int index, float val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushnumber(L, val);
	lua_rawset(L, -3);
}

inline void SetField(int index, ILuaObject *pObj, lua_State *L)
{
	lua_pushnumber(L, index);
	Push(L, pObj);
	lua_rawset(L, -3);
}

inline void SetField(int index, void *val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushlightuserdata(L, val);
	lua_rawset(L, -3);
}

inline void SetField(int index, const char* val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushstring(L, val);
	lua_rawset(L, -3);
}

inline void SetField(int index, const char* val, size_t len, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushlstring(L, val, len);
	lua_rawset(L, -3);
}

inline void SetField(int index, bool val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushboolean(L, val);
	lua_rawset(L, -3);
}

inline void SetField(int index, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushnil(L);
	lua_rawset(L, -3);
}

inline void SetField(float index, int val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushnumber(L, val);
	lua_rawset(L, -3);
}

inline void SetField(float index, float val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushnumber(L, val);
	lua_rawset(L, -3);
}

inline void SetField(float index, void *val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushlightuserdata(L, val);
	lua_rawset(L, -3);
}

inline void SetField(float index, const char* val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushstring(L, val);
	lua_rawset(L, -3);
}

inline void SetField(float index, const char* val, size_t len, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushlstring(L, val, len);
	lua_rawset(L, -3);
}

inline void SetField(float index, bool val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushboolean(L, val);
	lua_rawset(L, -3);
}

inline void SetField(float index, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushnil(L);
	lua_rawset(L, -3);
}

inline ILuaObject* ConvertToGLuaTable(lua_State *pGLua, lua_State *pGLuaJIT, int nTable)
{
	ILuaInterface *pLua = modulemanager->GetLuaInterface(pGLua);

	pLua->NewTable();
	ILuaObject* pTable = pLua->GetObject(-1);
	pLua->Pop(1);

	lua_pushvalue(pGLuaJIT, nTable);
	// stack now contains: -1 => table

	lua_pushnil(pGLuaJIT);
	// stack now contains: -1 => nil; -2 => table

	while (lua_next(pGLuaJIT, -2))
	{
		lua_pushvalue(pGLuaJIT, -2);

		// Debug Purpose
#if defined(_DEBUG)
		if( lua_isstring(pGLuaJIT, -1) )
		{
			const char* pszKey = lua_tostring(pGLuaJIT, -1);
			_cprintf("Key: %s\n", pszKey);
		}
#endif

		switch(lua_type(pGLuaJIT, -2))
		{
		case LUA_TTABLE:
			{
				lua_pushvalue(pGLuaJIT, -2);
				ILuaObject* pSecondTable = ConvertToGLuaTable(pGLua, pGLuaJIT, -1);
				lua_pop(pGLuaJIT, 1);

				if( lua_isnumber(pGLuaJIT, -1) )
					pTable->SetMember((float)lua_tonumber(pGLuaJIT, -1), pSecondTable);
				else if( lua_isstring(pGLuaJIT, -1) )
					pTable->SetMember(lua_tostring(pGLuaJIT, -1), pSecondTable);
			}
			break;
		case LUA_TSTRING:
			{
				const char* pszString = lua_tostring(pGLuaJIT, -2);

				if( lua_isnumber(pGLuaJIT, -1) )
					pTable->SetMember((float)lua_tonumber(pGLuaJIT, -1), pszString);
				else if( lua_isstring(pGLuaJIT, -1) )
					pTable->SetMember(lua_tostring(pGLuaJIT, -1), pszString);
			}
			break;
		case LUA_TNUMBER:
			{
				const float flNumber = lua_tonumber(pGLuaJIT, -2);

				if( lua_isnumber(pGLuaJIT, -1) )
					pTable->SetMember((float)lua_tonumber(pGLuaJIT, -1), flNumber);
				else if( lua_isstring(pGLuaJIT, -1) )
					pTable->SetMember(lua_tostring(pGLuaJIT, -1), flNumber);
			}
			break;
		case LUA_TNIL:
			{
				if( lua_isnumber(pGLuaJIT, -1) )
					pTable->SetMember((float)lua_tonumber(pGLuaJIT, -1), (ILuaObject*)NULL);
				else if( lua_isstring(pGLuaJIT, -1) )
					pTable->SetMember(lua_tostring(pGLuaJIT, -1), (ILuaObject*)NULL);
			}
			break;
		case LUA_TBOOLEAN:
			{
				bool bState = (bool)lua_toboolean(pGLuaJIT, -2);

				if( lua_isnumber(pGLuaJIT, -1) )
					pTable->SetMember((float)lua_tonumber(pGLuaJIT, -1), bState);
				else if( lua_isstring(pGLuaJIT, -1) )
					pTable->SetMember(lua_tostring(pGLuaJIT, -1), bState);
			}
			break;
		case LUA_TLIGHTUSERDATA:
		case LUA_TUSERDATA:
			{
				UserData_t* pUserData = (UserData_t*)lua_touserdata(pGLuaJIT, -2);
				pLua->PushReference((int)pUserData->pData);

				if( lua_isnumber(pGLuaJIT, -1) )
					pTable->SetMember((float)lua_tonumber(pGLuaJIT, -1), pLua->GetObject());
				else if( lua_isstring(pGLuaJIT, -1) )
					pTable->SetMember(lua_tostring(pGLuaJIT, -1), pLua->GetObject());
			}
			break;
		}
		lua_pop(pGLuaJIT, 2);
	}

	// Restore
	lua_pop(pGLuaJIT, 1);

	return pTable;
}

inline void ConvertToGLuaJITTable(lua_State *pGLuaJIT, lua_State *pGLua, ILuaObject *pTable)
{
	lua_newtable(pGLuaJIT);

	CUtlLuaVector* pTableElements = pTable->GetMembers();
	for( int i = 0; i < pTableElements->Size(); i++ )
	{
		LuaKeyValue& entry = pTableElements->Element(i);
		if( entry.pValue->GetType() == GLua::TYPE_TABLE )
		{
			// Recursive
			Push(pGLuaJIT, entry.pKey);
			ConvertToGLuaJITTable(pGLuaJIT, pGLua, entry.pValue);
			lua_rawset(pGLuaJIT, -3);
		}
		else
		{
			SetField(entry.pKey, entry.pValue, pGLuaJIT);
		}
	}
}

inline const char* GetTypeName(int luatype)
{
	static const char* pszTypeNames[] = 
	{
		"nil", "bool", "lightuserdata", "number", "string", "table", "function",
		"userdata", "thread"
	};

	if( luatype <= 8 && luatype >= 0 )
		return pszTypeNames[luatype];

	return "none";
}

}
#endif // LUAHELPER_H

