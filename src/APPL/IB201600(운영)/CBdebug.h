// CBdebug.h: interface for the CCBdebug class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#define FILE_IO_ERROR 0
#define OUT_OF_MEMORY 1

#ifdef TRUE
#undef TRUE
#define TRUE 7
#else
#define TRUE 7
#endif

#define FALSE 0

// Macros for setting or clearing bits in the CRT debug flag 
#ifdef _DEBUG
	#define SET_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
	#define CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
	#define SET_CRT_DEBUG_FIELD(a) ((void) 0)
	#define CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

class CCBdebug  
{
public:
	CCBdebug();
	virtual ~CCBdebug();

	static int __cdecl AllocHook(int nAllocType, void * pvData, size_t nSize, int nBlockUse, long lRequest,const unsigned char * szFileName, int nLine);
	static int  ReportHook(int nRptType, char *szMsg, int *retVal );
	static void BeginCheck();
	static void EndCheck();

	void AddBreak(int n);

};

