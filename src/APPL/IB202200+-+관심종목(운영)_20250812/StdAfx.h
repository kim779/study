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
#include <afxmt.h>
#include <AxStd.hpp>

#define RTS_TIMER  

#define DF_NONORDMAP

#ifdef DF_NONORDMAP
struct	_Ralert {
	std::string code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	std::unique_ptr<char[]> ptr[999]{};	// data record array
};

struct	_Ralertx {
	std::string code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	std::unique_ptr<char[]> ptr[999]{};	// data record array
};
#else
struct  _Ralert {
	CString code;
	int	stat{};				// DLL_ALERT.stat
	int	size{};				// data record count
	std::unique_ptr<char[]> ptr[999]{};	// data record array
};
#define	L_RalertR	sizeof(struct _Ralert)
#endif

#define ROW_COMMENT '8'   //test mod
#define ROW_BOOKMARK '9'

#define DF_NEWTREE
#define DF_SERVERMEMO