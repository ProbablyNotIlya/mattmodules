#if !defined(GM_LUAJIT_H)
#define GM_LUAJIT_H

#if defined(_MSC_VER)
#pragma once
#endif

#include <string>
#include <stack>

#include <boost/thread.hpp>
#include "GMLuaModule.h"

namespace GLuaJIT {

static boost::recursive_mutex s_Lock;

int Startup(lua_State *L);
int Cleanup(lua_State *L);

}

#endif // GM_LUAJIT_H