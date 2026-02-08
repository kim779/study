// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>   // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxdlgs.h>

/////////////////////////////////////////////////////////////////////////////
// NewMenu includes and macros

// enable new toolbar support
#define USE_NEW_DOCK_BAR
// enable new menubar support
#define USE_NEW_MENU_BAR

// we are building this project as a DLL
#define NEW_MENU_DLL

#include "NewMenu.h"
#include "NewMenuBar.h"
#include "NewToolBar.h"
#include "NewProperty.h"
#include "NewStatusbar.h"
#include "NewDialogBar.h"
