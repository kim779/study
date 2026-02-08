// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E5CAFCE7_5C9C_4119_B9DA_C38117511521__INCLUDED_)
#define AFX_STDAFX_H__E5CAFCE7_5C9C_4119_B9DA_C38117511521__INCLUDED_

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

#include <comdef.h>

#include <afxtempl.h>   //<= CList
#include "../Dr_Include/Signal_MngDef.h"

#include "./DefineFiles/color_TypeDef.h"
#include "./DefineFiles/client_TypeDef.h"
#include "./DefineFiles/message_TypeDef.h"

#include "../../Chart/COMMONTR/TRDefine.h"
#include "../../ForSite/FS_Env.h"

#include "../../DRCommon/Utils.h" //20120112 ÀÌ¹®¼ö >>

#include <MMSystem.h>

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define SAFE_DELETEW(p) if(p) { p->DestroyWindow(); delete p; p = NULL; }
void Msg(char* fmt, ...);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E5CAFCE7_5C9C_4119_B9DA_C38117511521__INCLUDED_)
