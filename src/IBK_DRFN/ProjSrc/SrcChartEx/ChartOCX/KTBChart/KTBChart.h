#if !defined(AFX_KTBCHART_H__DBEBD6AC_DC7A_43F7_8796_9868D99F730C__INCLUDED_)
#define AFX_KTBCHART_H__DBEBD6AC_DC7A_43F7_8796_9868D99F730C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// KTBChart.h : main header file for KTBCHART.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKTBChartApp : See KTBChart.cpp for implementation.

class CKTBChartApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.23
//
//	목적	:	OCX가 설치된 경로를 구하여 관리한다.
//
//	주의	:	1. 끝에 '\'를 포함한다.
///////////////////////////////////////////////////////////////////////////////
extern CString	g_strOcxPath;

// (2004.11.26, 배승원) Real의 분차트시 허수봉개념의 전체 시간을 지원할 것인지를 Flag으로 관리 지원한다.
//		특히, 별도의 Global을 속성을 두어 HTS 기본 설정을 관리하고,
//		별도의 Property를 제공하여 Chart별 속성을 변경토록 한다.
//		기본으로 실봉을 기준으로 하며, HTS의 기본 설정 INI File을 참조하여 변경토록한다.
extern BOOL g_bNormalizedMinuteTimeRule;

// (2004.11.26, 배승원) 외부에 제공할 Interface의 Export Type을 Macro로 지정한다.
#define CHARTOCX_API extern "C" __declspec(dllexport)

#define _ICHARTCTRL_CTRL_HEADER			"KTBChartCtl.h"
#define _ICHARTCTRL_CTRL				CKTBChartCtrl

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KTBCHART_H__DBEBD6AC_DC7A_43F7_8796_9868D99F730C__INCLUDED)
