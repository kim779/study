// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B35E0A19_7F63_406E_8776_F3EDCC0D1211__INCLUDED_)
#define AFX_STDAFX_H__B35E0A19_7F63_406E_8776_F3EDCC0D1211__INCLUDED_

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

#include <afxTempl.h>

#include "../Dr_Include/Condition_MngDef.h"

#include "./definefiles/server_typedef.h"
#include "./definefiles/client_typedef.h"
#include "./definefiles/color_typedef.h"
#include "./definefiles/Message_TypeDef.h"

#include "../../../SrcSite/include/axisfire.h"
#include "../../Chart/COMMONTR/TRDefine.h"
#include "../../ForSite/FS_Env.h"
#include "../../DRCommon/Utils.h" //20120112 ÀÌ¹®¼ö >>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B35E0A19_7F63_406E_8776_F3EDCC0D1211__INCLUDED_)
