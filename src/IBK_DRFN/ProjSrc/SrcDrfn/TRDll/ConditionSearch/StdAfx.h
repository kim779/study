// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F867A6CD_6342_43FE_BC98_2B0598AE2F7E__INCLUDED_)
#define AFX_STDAFX_H__F867A6CD_6342_43FE_BC98_2B0598AE2F7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
#import "msxml.dll"
using namespace MSXML;

/*
#include <afxtempl.h>
#import "msxml6.dll" named_guids
using namespace MSXML2;
#define MSXML MSXML2
*/


typedef MSXML::IXMLDOMElementPtr				XMLELEMENT;
typedef MSXML::IXMLDOMDocumentPtr				DOCUMENT;
typedef MSXML::IXMLDOMNodeListPtr				NODELIST;
typedef MSXML::IXMLDOMNodePtr					NODE;
typedef MSXML::IXMLDOMNamedNodeMapPtr			ATTRIBUTE;
typedef MSXML::IXMLDOMProcessingInstructionPtr	PROCESSINGINSTRCT;
typedef MSXML::IXMLDOMParseErrorPtr				PARSEERROR;

#include "../Dr_Include/Condition_MngDef.h"

#include "./definefiles/color_typedef.h"
#include "./definefiles/Message_TypeDef.h"
#include "./definefiles/client_TypeDef.h"
#include "./definefiles/server_TypeDef.h"

#include "../../Chart/COMMONTR/TRDefine.h"
#include "../../../SrcSite/include/axisfire.h"
#include "../../DRCommon/Utils.h" //20120112 이문수 >>

static void LOGOUT(LPCTSTR szFmt, ...)
{
	//  ___________________    Log 메세지 보내기  ____________________
	HWND   hTrace;
	COPYDATASTRUCT cds;
	hTrace = ::FindWindow (_T("LogViewer"), NULL);
	if(hTrace)
	{
		CString strLog;
		va_list argptr;
		va_start(argptr, szFmt);
		strLog.FormatV(szFmt, argptr);
		va_end(argptr);
		cds.dwData = 0xFEFF;
		cds.lpData = (void*)(LPCSTR)strLog;
		cds.cbData = strLog.GetLength();
		::SendMessage(hTrace, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
	} 
}

#ifdef __LOG_DEBUG__
void DebugStr(CString s);
#endif
//#include "../../../Common/as_packet.h"	-hidden-

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F867A6CD_6342_43FE_BC98_2B0598AE2F7E__INCLUDED_)


