// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1E91D5A2_DC0D_4983_A67E_9DA7C0D8789A__INCLUDED_)
#define AFX_STDAFX_H__1E91D5A2_DC0D_4983_A67E_9DA7C0D8789A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

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

//#include <afxsock.h>		// MFC socket extensions

#pragma warning(disable: 4070)

#define MakeNull(xx)	xx=NULL;
#define SafeDelete(xx)	if(xx)delete xx; MakeNull(xx);
#define CheckSafe(xx, basexx) if(xx==NULL) xx = new basexx;

LONG WINAPI GetInferface(WPARAM wp, LPARAM lp);
void* WINAPI GetPCTrData(WPARAM wp, LPARAM lp);
LONG WINAPI SetPCTrData(WPARAM wp, LPARAM lp);
LONG WINAPI NotifyPCTrState(WPARAM wp, LPARAM lp);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1E91D5A2_DC0D_4983_A67E_9DA7C0D8789A__INCLUDED_)
