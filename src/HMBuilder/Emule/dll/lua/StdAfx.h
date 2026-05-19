// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__33AFC3E8_D132_4E1C_BBA1_99BE135BE54A__INCLUDED_)
#define AFX_STDAFX_H__33AFC3E8_D132_4E1C_BBA1_99BE135BE54A__INCLUDED_

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

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxconv.h>
#include <string>
#include <vector>
#include <algorithm>
//#include <boost/regex.hpp>
//#include <boost/algorithm/string.hpp>
//#include <luabind/luabind.hpp>

using namespace std;
//using namespace boost;

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
#include <luabind/luabind.hpp>
#include <luabind/class_info.hpp> 


#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)  if(p){ delete p; p = NULL;                     }
	#define SAFE_DELETEA(p) if(p){ delete[] p; p = NULL;                   }
	#define SAFE_DELETEW(p) if(p){ p->DestroyWindow(); delete p; p = NULL; }
#endif

#define CSTRING(t) CString(t.data(), t.length())
#define ATOI(t)    atoi(t.data())
#define ATOL(t)    atol(t.data())
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__33AFC3E8_D132_4E1C_BBA1_99BE135BE54A__INCLUDED_)
