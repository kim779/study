// SignalSet.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ScreenWnd.h"
#include "SignalSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSignalSetApp, CWinApp)
//{{AFX_MSG_MAP(CChartSectionSearchApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSignalSetApp::CSignalSetApp()
{
}

CSignalSetApp theApp;
HINSTANCE g_hInstance;

BOOL CSignalSetApp::InitInstance()
{
	g_hInstance = theApp.m_hInstance;
	
	return CWinApp::InitInstance();
}


extern "C" __declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//20120112 이문수 리소스체크기능추가 >>
	BOOL bResult	= CUtils::CheckResource();
	if(!bResult)	return FALSE;
	//20120112 이문수 <<

	CScreenWnd* pWnd = new CScreenWnd(parent);
	if(pWnd->Create(NULL, "SIGNALSET", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), parent, 101) == FALSE)
		return NULL;
	
	return pWnd;
}

