#pragma once

#include <windows.h>

class CLibraryHandle
{
private:
	HMODULE m_hModule;
public:
	CLibraryHandle()
	{
		m_hModule = NULL;
	}
	bool Load(const TCHAR *pchModule)
	{
		m_hModule = LoadLibrary(pchModule);
		return m_hModule != NULL;
	}
	bool LoadEx(const TCHAR *pchModule, DWORD nFlags)
	{
		m_hModule = LoadLibraryEx(pchModule, 0, nFlags);
		return m_hModule != NULL;
	}
	bool Free()
	{
		if( !m_hModule )
			return false;
		return FreeLibrary(m_hModule) ? true : false;
	}
	template<typename R, typename A1>
	R StdCall(const char *pchExport, A1 P1)
	{
		typedef R (__stdcall* fnProto)(A1);

		fnProto _p = (fnProto)GetProcAddress(m_hModule, pchExport);
		if( !_p )
			return (R)(0);

		return _p(P1);
	}
	template<typename R, typename A1, typename A2>
	R StdCall(const char *pchExport, A1 P1, A2 P2)
	{
		typedef R (__stdcall* fnProto)(A1, A2);

		fnProto _p = (fnProto)GetProcAddress(m_hModule, pchExport);
		if( !_p )
			return (R)(0);

		return _p(P1, P2);
	}
	template<typename R, typename A1, typename A2, typename A3>
	R StdCall(const char *pchExport, A1 P1, A2 P2, A3 P3)
	{
		typedef R (__stdcall* fnProto)(A1, A2, A3);

		fnProto _p = (fnProto)GetProcAddress(m_hModule, pchExport);
		if( !_p )
			return (R)(0);

		return _p(P1, P2, P3);
	}
	template<typename R, typename A1, typename A2, typename A3, typename A4>
	R StdCall(const char *pchExport, A1 P1, A2 P2, A3 P3, A4 P4)
	{
		typedef R (__stdcall* fnProto)(A1, A2, A3, A4);

		fnProto _p = (fnProto)GetProcAddress(m_hModule, pchExport);
		if( !_p )
			return (R)(0);

		return _p(P1, P2, P3, P4);
	}
	template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
	R StdCall(const char *pchExport, A1 P1, A2 P2, A3 P3, A4 P4, A5 P5)
	{
		typedef R (__stdcall* fnProto)(A1, A2, A3, A4, A5);

		fnProto _p = (fnProto)GetProcAddress(m_hModule, pchExport);
		if( !_p )
			return (R)(0);

		return _p(P1, P2, P3, P4, P5);
	}
	template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	R StdCall(const char *pchExport, A1 P1, A2 P2, A3 P3, A4 P4, A5 P5, A6 P6)
	{
		typedef R (__stdcall* fnProto)(A1, A2, A3, A4, A5, A6);

		fnProto _p = (fnProto)GetProcAddress(m_hModule, pchExport);
		if( !_p )
			return (R)(0);

		return _p(P1, P2, P3, P4, P5, P6);
	}
};