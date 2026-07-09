// File: DbgOut.h
//

#ifndef DBGOUT_H_D7274660_0C36_11d2_9253_0000C06932AE__INCLUDED_
#define DBGOUT_H_D7274660_0C36_11d2_9253_0000C06932AE__INCLUDED_

#include "Time.h"

static  LPCTSTR    g_dbgOut                = _T("DebugOutput      ?EJOR, 1998");
static  LPCTSTR    g_dbgOutwindowClassName = _T("EJOR DebugOut Window");


//#ifdef _NO_DLL

	extern  clock_t time_start , time_finish, g_time_start, time_prev;
/*
#else

	#ifdef _CSLIBRARY
		extern AFX_DATA_EXPORT clock_t time_start , time_finish, time_prev;
	#else
		AFX_DATA_IMPORT clock_t time_start , time_finish, time_prev;
	#endif

#endif
*/

/********************************************************************************************
기능 : MyAgency에게 Log를 전송.
********************************************************************************************/
inline void MyLog(LPCTSTR strLog)
{
    HWND hWnd = ::FindWindow ("MyAgency2", "MyAgency2"); 
	if(hWnd) {
		COPYDATASTRUCT tCopydata;
		
		tCopydata.dwData = 0xFEF0;
		tCopydata.cbData = ((DWORD)strlen(strLog)+1)*sizeof(char);
		tCopydata.lpData = (void *)strLog; 

		::SendMessage(hWnd, WM_COPYDATA, (WPARAM)0, (LPARAM) & tCopydata);
	}
}

inline void DbgOutA (LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassName, g_dbgOut); 
    if (hWnd)
    {  
        cd.dwData = 0;
        cd.cbData = ((DWORD)strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}


// DBGOUTA
#ifdef _DEBUG
#define DBGOUTA(p) DbgOutA(p);
#else
#define DBGOUTA ((void)0)
#endif




inline void DbgOutW (LPCWSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassName, g_dbgOut); 
    if (hWnd)
    {  
        cd.dwData = 0xFEFF;
        cd.cbData = ((DWORD)wcslen(p)+1)*sizeof(wchar_t);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

// DBGOUTW
#ifdef _DEBUG
#define DBGOUTW(p) DbgOutW(p);
#else
#define DBGOUTW ((void)0)
#endif





inline void DbgOut (LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

    _TCHAR buffer [1024*sizeof(_TCHAR)];
	wvsprintf(buffer, pFormat, args);

    #ifdef UNICODE
    DbgOutW (buffer);
    #else
    DbgOutA (buffer);
    #endif

    va_end(args);
}

// DBGOUT
#ifdef _DEBUG
#define DBGOUT ::DbgOut
#else
#define DBGOUT ((void)0)
#endif



inline DWORD DbgOutLastError (LPCTSTR pFormat, ...)
{
   if (::GetLastError() == 0) 
        return 0;
   
	va_list args;
	va_start(args, pFormat);

    _TCHAR buffer [1024*sizeof(_TCHAR)];
	wvsprintf(buffer, pFormat, args);

    LPVOID pMessage;
    DWORD  result;
    result = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             GetLastError(),
                             MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                             (LPTSTR)&pMessage,
                             0,
                             NULL);
  
    lstrcat (buffer, _T(" : "));
    lstrcat (buffer, (TCHAR*)pMessage);
    
  //  DBGOUT (buffer);
    
    if(result)
        ::LocalFree(pMessage);
   
    va_end(args);
    return result;
}

// DBGOUT_LASTERROR
#ifdef _DEBUG
#define DBGOUT_LASTERROR ::DbgOutLastError
#else
#define DBGOUT_LASTERROR ((void)0)
#endif


inline void DbgOutExA (int n, LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassName, g_dbgOut); 
    if (hWnd)
    {  
        cd.dwData = n;
        cd.cbData = ((DWORD)strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}


// DBGOUTA
#ifdef _DEBUG
#define DBGOUTEXA(n,p) DbgOutExA(n,p);
#else
#define DBGOUTEXA ((void)0)
#endif



inline void DbgOutEx (int n, LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

    _TCHAR buffer [1024*sizeof(_TCHAR)];
	if (!pFormat)
		pFormat = "Format is NULL\n";
	wvsprintf(buffer, pFormat, args);

    DbgOutExA (n, buffer);

    va_end(args);
}

// DBGOUT
#ifdef _DEBUG
#define DBGOUTEX ::DbgOutEx
#else
#define DBGOUTEX ((void)0)
#endif




#ifdef _DEBUG
#define ENT(name)	static long cnt__dbg;						\
					{											\
																\
					cnt__dbg++;									\
					DBGOUT("Entering "#name" : cnt = %ld----------------|\n", cnt__dbg);	\
					}
#else
#define ENT(name) ((void)0)
#endif

#ifdef _DEBUG
#define LEA(name)	{													\
						DBGOUT("Leaving "#name" : cnt = %ld\n", cnt__dbg);	\
					}
#else
#define LEA(name) ((void)0)
#endif

#ifdef _DEBUG
#define ENT2(n, name)	static long cnt__dbg;						\
					{											\
																\
					cnt__dbg++;									\
					DBGOUTEX(n, "Entering "#name" : cnt = %ld------------------|\n", cnt__dbg);	\
					}
#else
#define ENT2(n, name) ((void)0)
#endif

#ifdef _DEBUG
#define LEA2(n, name)	{													\
						DBGOUTEX(n, "Leaving "#name" : cnt = %ld\n", cnt__dbg);	\
					}
#else
#define LEA2(n, name) ((void)0)
#endif

#ifdef _DEBUG

	#define CSS		{ time_start = clock() ; }
	#define CSE		{ time_finish = clock() ;  }
	#define CSPI	{ TRACE( "%s(%d) : Elapsed : %2.3lf\n" , __FILE__, __LINE__, (( double )( time_finish - time_start )) / CLOCKS_PER_SEC ); time_start = time_finish; }
	#define CSP		{		\
						if ((time_finish - time_prev)/ CLOCKS_PER_SEC > 0.1)	\
							TRACE("-----------------------Delayed : %2.3lf----------------------\n", (double)(time_finish - time_prev)/ CLOCKS_PER_SEC);	\
						TRACE( "              %s(%d) : Elapsed : %2.3lf\n" , __FILE__, __LINE__, (( double )( time_finish - time_start )) / CLOCKS_PER_SEC ); time_prev = time_finish;}

	#define CSEP	CSE;CSP;
	#define CSEPI	CSE;CSPI;

	#define GSS		{g_time_start = clock();}
	#define GSI		{g_time_inter = clock();}
	#define GSP		{ TRACE("%s(%d) : [Global Time] Elapsed Time : %2.3lf\n", __FILE__, __LINE__, (( double )( g_time_inter - g_time_start )) / CLOCKS_PER_SEC );  }
	#define GSIP	GSI;GSP;

#else

	#define CSS		
	#define CSE		
	#define CSP		
	#define CSPI
	#define CSEPI

	#define CSEP	CSE;CSP;

	#define GSS		
	#define GSI		
	#define GSP		
	#define GSIP	

#endif 

#ifdef _DEBUG
	#define ST(name)	static long cnt__dbg;								\
					{														\
					cnt__dbg++;												\
					TRACE("Entering "#name" : cnt = %ld------------------|\n", cnt__dbg);		\
					}
	#define ET(name)	{													\
					TRACE("Leaving "#name" : cnt = %ld ------------------|\n", cnt__dbg);		\
					}
#else
	#define ST(name)		
	#define ET(name)
#endif

#endif//  DBGOUT_H_D7274660_0C36_11d2_9253_0000C06932AE__INCLUDED_
