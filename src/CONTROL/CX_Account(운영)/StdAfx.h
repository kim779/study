// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include <AxStd.hpp>
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/ledger.h"
#include "../../h/group.h"

//#define DF_ACC_EMPHASIS  //사용안하기로 함

#define  DF_TEST_MODE1
#define DF_ACC_HASH
#define DF_SINGLETONE_FILE		// AccountHistory ini 파일 접근을 CAccHistoryStore 싱글톤으로 처리 (실험용 토글)

inline void Output_DebugString(CString slog)
{
#ifdef  DF_TEST_MODE
	OutputDebugString(slog);
#endif
}
