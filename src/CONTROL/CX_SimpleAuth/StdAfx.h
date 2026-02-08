// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1A289D24_B08C_46F4_9FE6_35E5991FEE5C__INCLUDED_)
#define AFX_STDAFX_H__1A289D24_B08C_46F4_9FE6_35E5991FEE5C__INCLUDED_

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

#define DF_TYPE_LOGIN 0
#define DF_TYPE_TRD 1
#define DF_TYPE_OTP 2
#define DF_TYPE_AUTO_OTP 3

#define DF_ST_TAGREG 1
#define DF_ST_CHKREG 2
#define DF_ST_SEARCHTRDNUM 3
#define DF_ST_VEFYLOGIN 4
#define DF_ST_VEFYTRADE 5
#define DF_ST_OTPCERT   6

#define TRKEY_OPTCERT	7
#define TRKEY_OPTSEARCH	8
#include <AxStd.hpp>
#include "../../h/axisfire.h"

#include <Iphlpapi.h>
#pragma comment (lib, "IpHlpApi.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1A289D24_B08C_46F4_9FE6_35E5991FEE5C__INCLUDED_)
