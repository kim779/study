#if !defined(AFX_STDAFX_H__DC3B90BF_A245_4178_8C60_EE04C4E104D1__INCLUDED_)
#define AFX_STDAFX_H__DC3B90BF_A245_4178_8C60_EE04C4E104D1__INCLUDED_

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

// (2004.10.08, 배승원) AddIn DLL에게 제공되는 Chart Object중, OCX의 Interface Wrapper Class를
//		타 Site Source와 공유하기 위하여 Macro Name을 정의한다.
#define MChartOcx	CKTBChartCtrl
// (2004.10.11, 배승원) AddIn DLL에게 제공되는 Chart Object중, OCX의 Interface Wrapper Class의
//		OCX Control Class Accessing을 타 Site와 Source를 공유하기 위하여 Macro Header Path를 정의한다.
#define MChartOcxInclude	"../KTBChart/OCX2/KTBChartCtl.h"

#pragma warning(disable: 2039)

// type casting에 관한 warning를 없앤다.
#pragma warning(disable: 4800)

// stl header에서 나는 warning를 없앤다.
#pragma warning(disable: 4786)

//#include "../Include/LoadDRDebuger.h"
#include "../../../SrcDrfn/Inc/LoadDRDebuger.h"
extern CDRDebugerLoader *g_pDRDebuger;
extern CDRDebugerLoader* gfnGetDRDebuger();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DC3B90BF_A245_4178_8C60_EE04C4E104D1__INCLUDED_)
