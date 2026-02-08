#if !defined(AFX_STDAFX_H__889EB0C9_5879_4866_B9A5_1ED05F27E66E__INCLUDED_)
#define AFX_STDAFX_H__889EB0C9_5879_4866_B9A5_1ED05F27E66E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes
#include <Afxmt.h>

#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
#include <comdef.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__889EB0C9_5879_4866_B9A5_1ED05F27E66E__INCLUDED_)

#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#include "IO.h"