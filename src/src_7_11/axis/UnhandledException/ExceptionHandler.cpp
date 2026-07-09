// ExceptionHandler.cpp: implementation of the CExceptionHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExceptionHandler.h"

#include "pch.h"
#include "BugslayerUtil.h"
#include "CrashHandler.h"

#pragma comment(lib, "UnhandledException/BugslayerUtil.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LONG __stdcall CrashHandlerExceptionFilter( EXCEPTION_POINTERS* pExPtrs);

IExceptionHandler* CExceptionHandler::m_pOuter = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExceptionHandler::CExceptionHandler()
{
#ifndef _DEBUG
	SetErrorMode(SEM_NOGPFAULTERRORBOX);
#endif

	SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);

}

CExceptionHandler::~CExceptionHandler()
{

}


//*****************************************************************************
// CrashHandlerExceptionFilter()
// Purpose : [ 프로그램이 충돌했을 때, 정보를 수집하기 위해서 호출됩니다. ]
// Parameters :	
// [pExPtrs] :
// Returns :	LONG __stdcall
// Last Update : 2001-06-13
//*****************************************************************************
LONG __stdcall CrashHandlerExceptionFilter( EXCEPTION_POINTERS* pExPtrs)
{
	LONG lRet;

// 디버깅 모드 일때에는 추가 정보를 보여주지 않고
// EXCEPTION_CONTINUE_SEARCH  값을 리턴함으로써
// 프로그래머가 디버깅 할 수 있도록 합니다.
#ifdef _DEBUG
	lRet = EXCEPTION_CONTINUE_SEARCH ;
	return lRet;
#else
	lRet = EXCEPTION_EXECUTE_HANDLER;
#endif
	char strText[500] = {'\0',};

	// 왜 죽었는지 알아내도록 합니다.
//	CString strReason;
//	strReason.Format("%s", GetFaultReason(pExPtrs));
	
//	CString strTemp;
//	strTemp.Format("\n\n - Registers\n\n%s", GetRegisterString(pExPtrs));
//	strReason += strTemp;

//	CExceptionHandler::m_pOuter->ExceptionInformation(strReason);

	return lRet;
}

