#if !defined(LUAHELPER_H)
#define LUAHELPER_H

#if defined(_MSC_VER)
#pragma once
#endif

#include "luajit/lua.hpp"

namespace LuaHelper {

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

inline void SetField(int index, const char* val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushstring(L, val);
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

inline void SetField(float index, const char* val, lua_State *L)
{
	lua_pushnumber(L, index);
	lua_pushstring(L, val);
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

inline void SetField(ILuaObject *pKey, ILuaObject *pVal, lua_State *L)
{
	switch(pKey->GetType())
	{
	case GLua::TYPE_STRING:
		lua_pushstring(L, pKey->GetString());
		break;
	case GLua::TYPE_NUMBER:
		lua_pushnumber(L, (float)pKey->GetFloat());
		break;
	case GLua::TYPE_BOOL:
		lua_pushboolean(L, pKey->GetBool());
		break;
	}
	switch(pVal->GetType())
	{
	case GLua::TYPE_STRING:
		lua_pushstring(L, pVal->GetString());
		break;
	case GLua::TYPE_NUMBER:
		lua_pushnumber(L, pVal->GetFloat());
		break;
	case GLua::TYPE_BOOL:
		lua_pushboolean(L, pVal->GetBool());
		break;
	}
	lua_rawset(L, -3);
}

inline ILuaObject* ConvertToGLuaTable(lua_State *pGLua, lua_State *pGLuaJIT, int nTable)
{
	modulemanager->GetLuaInterface(pGLua)->NewTable();
	ILuaObject* pTable = modulemanager->GetLuaInterface(pGLua)->GetObject();

	// TODO: Walk pGLuaJIT

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
			switch(entry.pKey->GetType())
			{
			case GLua::TYPE_STRING:
				lua_pushstring(L, entry.pKey->GetString());
				break;
			case GLua::TYPE_NUMBER:
				lua_pushnumber(L, (float)entry.pKey->GetFloat());
				break;
			case GLua::TYPE_BOOL:
				lua_pushboolean(L, entry.pKey->GetBool());
				break;
			}
			ConvertToGLuaJITTable(pGLuaJIT, pGLua, entry.pValue);
			lua_rawset(L, -3);
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

