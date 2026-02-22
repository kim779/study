// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4CC23A52_9838_422A_879F_332A81085713__INCLUDED_)
#define AFX_STDAFX_H__4CC23A52_9838_422A_879F_332A81085713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CppTooltip STL warning
#pragma warning (disable : 4786)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>

#include "../dll/axiscm/AxCommon.h"

#include "GDIHelper.h"
#include "MDebug.h"
using namespace mdebug;

#include "Helper.h"

#include "../h/axis.h"
#include "../h/axiserr.h"
#include "../h/axisgenv.h"
#include "../h/axisvar.h"
#include "../h/axisfire.h"
#include "../h/axisfm.h"
#include "../h/stopmsg.h"
#include "../h/ctinterface.h"

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
#include <comdef.h>
extern CComModule _Module;
#include <atlcom.h>

//#pragma warning(disable : 4005 ;  disable : 4838)

#include <AxStd.hpp>

#define DF_ENCUSER
#define DF_LOGKEY  "command"
#define DF_2LOGKEY  "RunVers"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

typedef BOOL(WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);

#define DF_MK_CAPTION   //childframe 타이틀에 거래소 추가
#define DF_MNG_THREAD1
#define DF_WEAKPOINT
#define DF_AUTOLOGV1
#define DF_EDGE_MAP   "IB980002"
#define DF_NEW_MNG

inline void output_DebugString(CString sdata)
{
	OutputDebugString(sdata);
}
//#define DF_NO_PRELOADHIDDEN

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4CC23A52_9838_422A_879F_332A81085713__INCLUDED_)
