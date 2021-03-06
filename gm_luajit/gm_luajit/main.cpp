#define WIN32_LEAN_AND_MEAN

#include "GMLuaModule.h"

GMOD_MODULE(Startup, Cleanup);

#include "gm_luajit.h"

int Startup(lua_State* L)
{
#if defined(_DEBUG) && defined(_WIN32)
	AllocConsole();
#endif

	GLuaJIT::Startup(L);
	return 0;
}

int Cleanup(lua_State* L)
{
	GLuaJIT::Cleanup(L);
	return 0;
}