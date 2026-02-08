// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3B465467_5AEE_4D57_A7DE_A6F2AEA9840E__INCLUDED_)
#define AFX_STDAFX_H__3B465467_5AEE_4D57_A7DE_A6F2AEA9840E__INCLUDED_

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

#import "msxml.dll"

#include "../Dr_Include/Signal_PacketDef.h"
#include "../Dr_Include/Signal_MngDef.h"

#include "./definefiles/color_typedef.h"
#include "./definefiles/client_typedef.h"

#include <afxtempl.h>"
#include "../../ForSite/FS_Env.h"
#include "../../Chart/COMMONTR/TRDefine.h"

#include "../../DRCommon/Utils.h" //20120112 이문수 >>

#include "../../Chart/COMMONTR/KB_p0602.h"
#include "../../inc/PcTrMngHelper.h"

#include <MMSystem.h>

//#define	MAX_VERSION		200703	// 운영중인 Max
#define	MAX_VERSION		200709	// 신시스템버전.

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define SAFE_DELETEW(p) if(p) { p->DestroyWindow(); delete p; p = NULL; }
void Msg(char* fmt, ...);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3B465467_5AEE_4D57_A7DE_A6F2AEA9840E__INCLUDED_)
