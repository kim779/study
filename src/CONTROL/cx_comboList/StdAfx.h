// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <atlimage.h>
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

#include <AxStd.hpp>
#define VS1_TYPE   0
#define VS2_TYPE   1

#define ID_BTN_DROP     9899
#define ID_BTN_CLEAR    9900
#define ID_BTN_LANG     9898
#define ID_EDIT_CODE     9989

#define EDIT_MSG_FOCUS      1   // 포커스 받음
#define EDIT_MSG_KILLFOCUS  2   // 포커스 잃음
#define EDIT_MSG_HANGUL     3   // 한영 상태 변경

#define WM_POPLISTWINDOW  (WM_USER + 300)
#define WM_EDIT_MSG     (WM_USER + 500)

#define POPLIST_DBCLICKCODE 5
#define POPLIST_CLEARALL 4
#define POPLIST_CREATE 1
#define POPLIST_SHOW  2
#define POPLIST_HIDE 3

void Msg(char* fmt, ...);