#include <StdAfx.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crtdbg.h>

#include "cbdebug.h"

_CrtMemState checkPt1;

int a_count, r_count, f_count;
FILE* logFile;

int Array[100] = {0};
int array_count;
CCBdebug::CCBdebug()
{
	logFile = fopen("debug.txt", "w");

	a_count = r_count = f_count = 0;
	array_count = 0;
}

CCBdebug::~CCBdebug()
{
}

int __cdecl CCBdebug::AllocHook(int nAllocType, void * pvData, size_t nSize, int nBlockUse, long lRequest,const unsigned char * szFileName, int nLine)
{
	if(!logFile)
		return 0;

	char *operation[] = { "", "  할당", "재할당", "  제거" };
	char *blockType[] = { "Free", "Normal", "CRT", "Ignore", "Client" };
	char path[41] = {0};
	int len = 0;
	
	if ( nBlockUse == _CRT_BLOCK ) // Ignore internal C runtime library allocations
		return( TRUE );
	
	_ASSERT( ( nAllocType > 0 ) && ( nAllocType < 4 ) );
	_ASSERT( ( nBlockUse >= 0 ) && ( nBlockUse < 5 ) );

	if(szFileName)
		len = strlen((const char*)szFileName);

	if(nAllocType == 1)
		a_count++;
	else if(nAllocType == 2)
		r_count++;
	else if(nAllocType == 3)
		f_count++;

	if(len > 40)
	{
		fprintf( logFile, 
			"[%7s] - %40s : (%04d) %10d-byte '%s' block (# %3ld)",
			operation[nAllocType], path, nLine, nSize, 
			blockType[nBlockUse], lRequest );
	}
	else
	{
		fprintf( logFile, 
			"[%7s] - %40s : (%04d) %10d-byte '%s' block (# %3ld)",
			operation[nAllocType], szFileName, nLine, nSize, 
			blockType[nBlockUse], lRequest );
	}

	for(int i=0; i<array_count; i++)
	{
		if(Array[i] == lRequest)
		{
			_CrtDbgBreak();
			break;
		}
	}
	
	if ( pvData != NULL )
		fprintf( logFile, " => at 0x%08X\n", pvData );
	else
		fprintf( logFile, "\n");
	
	return 1; // Allow the memory operation to proceed
}

int CCBdebug::ReportHook(int nRptType, char *szMsg, int *retVal )
{
	if(!logFile)
		return 0;
	
	char *RptTypes[] = { "Warning", "Error", "Assert" };
	fprintf( logFile, "%s: %s", RptTypes[nRptType], szMsg );
	
	retVal = 0;
	
	return 1; // Allow the report to be made as usual
	
}

void CCBdebug::BeginCheck()
{
    if (!logFile)
        return;
	
    memset(&checkPt1, 0, sizeof(_CrtMemState));
    
    SET_CRT_DEBUG_FIELD( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	
    char timeStr[100], dateStr[100];
    const char lineStr[] = { "----------------------------------------------------------------------------\n" };
	
	_strtime( timeStr );
    _strdate( dateStr );
	
    fprintf( logFile, "Memory Allocation Log File for Example Program, run at %s on %s.\n",    timeStr, dateStr );
	fputs( lineStr, logFile );
	
    _CrtSetAllocHook( AllocHook );
    _CrtSetReportHook( ReportHook );
	
    
    _CrtMemDumpAllObjectsSince( NULL );
    _CrtCheckMemory( );
    _CrtMemCheckpoint( &checkPt1 );
	
}

void CCBdebug::EndCheck()
{
   _CrtMemCheckpoint( &checkPt1 );
   _CrtDumpMemoryLeaks();
	
	fprintf(logFile, "\n총 할당 카운트 : %4d\n총 재할당 카우트 : %4d\n총 제거 카운트 : %4d", a_count, r_count, f_count);
	
    fclose( logFile );
}

void CCBdebug::AddBreak(int n)
{
	Array[array_count++] = n;
}