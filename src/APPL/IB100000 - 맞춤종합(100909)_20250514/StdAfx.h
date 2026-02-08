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
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
const double g_lratex[] = {0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5};
const double g_lratey[] = {0.9, 1, 1.05, 1.1, 1.15, 1.2, 1.25};
#include <afxtempl.h>

#pragma comment (lib, "Msimg32.lib")

#define					BACK_INDEX				64
#define					CONTENTS_BACK_INDEX		181
#define					CONTENTS_BORDER_INDEX	93
#define					TAB_BORDER_INDEX	44

#define					MAP_HEIGHT	660
#define					MAP_WIDTH	1040
#define					CONFIG_HEIGHT	24
#define TIMER_WAIT	100
#define TIMER_WAIT1	101
#define TIMER_WAIT2	101
#define JPRC	"ed_jprc"


#include <AxStd.hpp>