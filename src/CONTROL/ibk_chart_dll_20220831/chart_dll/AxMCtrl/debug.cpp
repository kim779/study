// debug.cpp : implementation of the Debug
//

#include "stdafx.h"
#include <stdarg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void DebugTrace(LPCTSTR debugStr)
{
	HANDLE hHeader, hQ;
	
	hHeader = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 
		sizeof(QHEADER), DEBUG_Q_HEADER);
	
	hQ = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0,
		sizeof(DEBUGINFO) * DEBUG_Q_COUNT, DEBUG_Q_BUFFER);
	
	PQHEADER pHeader = (PQHEADER)MapViewOfFile(hHeader, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	PDEBUGINFO pQ = (PDEBUGINFO)MapViewOfFile(hQ, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	
	PDEBUGINFO p = (PDEBUGINFO)((LPBYTE)pQ + pHeader->pos * sizeof(DEBUGINFO));
	
	//p->DebugFlag = 0x00;
	strcpy(p->AppName, AfxGetAppName());
	strcpy(p->DebugStr, debugStr);
	
	pHeader->pos = (pHeader->pos + 1) % DEBUG_Q_COUNT;
	pHeader->cnt++;
	
	UnmapViewOfFile(pQ);
	UnmapViewOfFile(pHeader);
	CloseHandle(hQ);
	CloseHandle(hHeader);
}

void M_DebugProc(LPCTSTR lpszFormat, ...)
{
	char szTemp[4096];
	
	va_list argList;
	va_start(argList, lpszFormat);
	vsprintf(szTemp, lpszFormat, argList);
	va_end(argList);
	
	DebugTrace(szTemp);
	
	TRACE("[%s] %s\n", CTime::GetCurrentTime().Format("%H:%M:%S"), szTemp);
}

