// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3AC5A175_66CD_4368_A8C5_C36303FF5A8A__INCLUDED_)
#define AFX_STDAFX_H__3AC5A175_66CD_4368_A8C5_C36303FF5A8A__INCLUDED_

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
#include <GdiPlus.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <deque>
#include <map>


using namespace std;
using namespace Gdiplus;
//using namespace boost;

//#define _last
#define _GDI	1

#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)  if(p){ delete p; p = NULL;                     }
	#define SAFE_DELETEA(p) if(p){ delete[] p; p = NULL;                   }
	#define SAFE_DELETEW(p) if(p){ p->DestroyWindow(); delete p; p = NULL; }
#endif

#ifndef TRIM
	#define TRIM(a)		         { a.TrimLeft(), a.TrimRight();            }
#endif

#ifndef RECTG
	#define RECTG(rc)	Rect(rc.left, rc.top, rc.Width() - 1, rc.Height() - 1)
#endif

#ifndef RECTGA
	#define RECTGA(rc)	Rect(rc.left, rc.top, rc.Width(), rc.Height())
#endif


#ifndef RECTF
	#define RECTF(rc)	RectF((REAL)rc.left, (REAL)rc.top, (REAL)(rc.Width()), (REAL)(rc.Height()))
#endif

#ifndef _ARGB
	#define _ARGB(a,r,g,b)	((COLORREF)((BYTE)(b)|((WORD)((BYTE)(g))<<8)|(((DWORD)(BYTE)(r))<<16)|(((DWORD)(BYTE)(a))<<24)))
#endif


#ifndef GRGB 

	#define  GRGB(r) r
/*
#else
	#define GRGB(r) _ARGB(255, GetRValue(r), GetGValue(r), GetBValue(r))
#endif
	*/
#endif

CString AFXAPI FORMAT(const char* formatString, ...);
void	AFXAPI Msg(char* fmt, ...);
int Split(std::vector<std::string> &elems, const std::string &s, char delim);


#define mCLASS		0x08
#define mPRO		0x0c
#define mITEM		0x7f
#define mSCRIPT		0x0e

#define CSTRING(t) CString(t.data(), t.length())
#define ATOI(t)    atoi(t.data())
#define ATOL(t)    atol(t.data())


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3AC5A175_66CD_4368_A8C5_C36303FF5A8A__INCLUDED_)
