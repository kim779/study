// XTPIncludes.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ?004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////


//--------------
// MFC includes:
//--------------
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>
#endif // _AFX_NO_OLE_SUPPORT

#ifdef _MFC_OVERRIDES_NEW
#define _INC_MALLOC
#endif
// MFC support for docking windows
#include <afxpriv.h>		
// MFC template classes
#include <afxtempl.h>		
// MFC ListView / TreeView support
#include <afxcview.h>		

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


////addd extra
#define CXTPCmdTarget CCmdTarget

#define CXTStringHelper CString


#if _MSC_VER >= 1400
#define _XTP_DEPRECATE(_Message) __declspec(deprecated(_Message))
#elif _MSC_VER >= 1300
#define _XTP_DEPRECATE(_Message) __declspec(deprecated)
#else
#define _XTP_DEPRECATE(_Message)
#endif

#define DISABLE_COPY_OPERATOR(theClass) \
	private:\
	theClass& operator=(const theClass &) { ASSERT(FALSE); return *this; }


#define DISABLE_WNDCREATE() \
	private:\
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {\
		return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);}

#define DISABLE_WNDCREATEEX() \
	private:\
	BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,\
		int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL) {\
		return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, nIDorHMenu, lpParam);\
	}\
	BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,\
		const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL) {\
		return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);\
	}
	
#define CMDTARGET_RELEASE(x) if (x) { x->InternalRelease(); x = 0;}
//====================================>

// MFC support for shell extensions
#include <shlobj.h>			

// Version and Package of Xtreme Toolkit Pro
#include "XTPVersion.h"

//////////////////////////////////////////////////////////////////////
// NOTE: If using the Xtreme Toolkit Pro as a static library linked to an application
// that is dynamically linked with MFC, you will need to do the following:
//
// Open the XTPToolkit_Lib project workspace and select one of the 
// Win32 Dynamic build settings and build the library. Add the following lines 
// of code to your stdafx.h file:
//
//<pre>#define _XTP_STATICLINK
// #include <XTToolkitPro.h></pre>
//
// Add the following line of code to your *.rc2 file after the comment:
// "Add manually edited resources here...":
//
//<pre>#include "XTToolkitPro.rc"</pre>
//
#if !defined( _AFXDLL ) || defined( _XTP_STATICLINK )
#define _XTP_EXT_CLASS
#else
#define _XTP_EXT_CLASS	__declspec( dllimport )
#endif // !defined( _AFXDLL ) || defined( _XT_STATICLINK )

#ifndef AFX_INLINE
#define AFX_INLINE inline
#endif

#ifndef AFX_STATIC
#define AFX_STATIC static
#endif

#ifndef CBRS_GRIPPER
#define CBRS_GRIPPER                    0x00400000L
#endif


//-----------------------------------------
// Xtreme global and resource definitions:
//-----------------------------------------
#include "XTPResource.h"
#include "XTPColorManager.h"

#include "Tmschema.h"
#include "XTPWinThemeWrapper.h"
#include "XTPDrawHelpers.h"

//-----------------------------------------------
// Xtreme Pro Controls -
//-----------------------------------------------

#ifdef _XTP_INCLUDE_CONTROLS
#include "Controls/XTDefines.h"
#include "Controls/XTIncludes.h"
#endif


//-----------------------------------------------
// Xtreme Pro Command Bars -
//-----------------------------------------------

#ifdef _XTP_INCLUDE_COMMANDBARS
#include "CommandBars/XTPCommandBarsIncludes.h"
#endif

//-----------------------------------------------
// Xtreme Pro Docking Pane -
//-----------------------------------------------

#ifdef _XTP_INCLUDE_DOCKINGPANE
#include "DockingPane/XTPDockingPaneIncludes.h"
#endif

//----------------------------
// Xtreme Property Grid classes:
//----------------------------

#ifdef _XTP_INCLUDE_PROPERTYGRID
#include "PropertyGrid/XTPPropertyGridIncludes.h"
#endif

//----------------------------
// Xtreme tabmanager classes:
//----------------------------

#ifdef _XTP_INCLUDE_TABMANAGER
#include "TabManager/TabManagerIncludes.h"
#endif

