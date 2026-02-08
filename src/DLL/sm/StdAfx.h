// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__63ECC687_418D_4DEC_85E8_0E83F1C9DCF9__INCLUDED_)
#define AFX_STDAFX_H__63ECC687_418D_4DEC_85E8_0E83F1C9DCF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CppTooltip STL warning
#pragma warning (disable : 4786)
#if defined __cplusplus
namespace shutting_up_MSVC_warning_C4786 {
    template <typename T> struct Nut {
    virtual void f() {
        new T[1];
    }
    };
    struct Nuts : Nut< Nut< Nut<int> > > {
    };
}
#endif

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

#include <afxsock.h>		// MFC socket extensions
#include "smdefine.h"

//#ifndef ULONG_PTR
//#define ULONG_PTR unsigned long *
//#endif

//#include <Gdiplus.h> //항상 넣어 둔다.
//#include <GdiplusBase.h> //항상 넣어 둔다.
//#include <GdiplusColor.h>
//#include <GdiplusPen.h>
//#include <GdiplusBrush.h>
//#include <GdiplusPath.h>
//#include <Gdiplusgraphics.h>
//
//using namespace Gdiplus;
//#pragma comment(lib,"GdiPlus.lib")

#include "../../dll/axiscm/AxCommon.h"
#include "../../axis/helper.h"
#include "../../appl/axcommon/mdebug.h"

using namespace mdebug;
#include <AxStd.hpp>

#define DF_MK_CAPTION

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__63ECC687_418D_4DEC_85E8_0E83F1C9DCF9__INCLUDED_)
