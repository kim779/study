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

#define			WXEDIT_ENTER		90
#define			WM_WXMSG		(WM_USER + 1030)

#include "Util.h"

#include "tr_data.h"
#include "datap.h"
#include <AxStd.hpp>

//#pragma warning <disable : C2143>
#include <wil/com.h>
#include <wrl/event.h>
#include <wil/result.h>
#include <wrl.h>
#include <cpprest/json.h>
#include "WebView2EnvironmentOptions.h"
#include "WebView2.h"


#include "../../h/axisfire.h"

#define DF_NOEXCEL  //현재 excel.h 와 충돌한다.. 먼가 방법이 필요하다

#define DF_USEEDGE
