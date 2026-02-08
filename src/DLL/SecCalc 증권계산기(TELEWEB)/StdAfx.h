// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A2E68A57_AF0B_4582_B75A_0072883DD237__INCLUDED_)
#define AFX_STDAFX_H__A2E68A57_AF0B_4582_B75A_0072883DD237__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#define	WM_CHILD	WM_USER+1
	#define	TAB_SELCHANGE	1
	#define	EDIT_FOCUS	2

#include "CommaEdit.h"
#include "FreeEdit.h"
#include "ColorButton.h"

#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A2E68A57_AF0B_4582_B75A_0072883DD237__INCLUDED_)
