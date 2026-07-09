// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5120D025_F392_4497_9065_DBF240105E78__INCLUDED_)
#define AFX_STDAFX_H__5120D025_F392_4497_9065_DBF240105E78__INCLUDED_

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

#include "Global.h"
#include "resource.h"
#include <afxtempl.h>
#include <imm.h>
#include "../../h/axisfire.h"
#include "../../h/jmCode.h"
#include "../../h/MemDC.h"
#include "myuser.h"
#include "libSort.h"
#include "btnst.h"
#include "skinlistctrl.h"
#include "sfont.h"
#include "EditX.h"
#include "tool.h"
#include "checkbox.h"
#include "myedit.h"
#include <afxcontrolbars.h>

#define _TRIMDATA(data) \
		data.TrimLeft(); \
		data.TrimRight();


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#pragma warning( disable : 4996 )

#endif // !defined(AFX_STDAFX_H__5120D025_F392_4497_9065_DBF240105E78__INCLUDED_)
