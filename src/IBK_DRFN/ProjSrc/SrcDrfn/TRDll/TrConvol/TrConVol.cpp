// TrConVol.cpp : implementation file
//

#include "stdafx.h"
#include "TrConVol.h"
#include "ScreenWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTrConVolApp theApp;
HINSTANCE g_hInstance;

BEGIN_MESSAGE_MAP(CTrConVolApp, CWinApp)
//{{AFX_MSG_MAP(CChartSectionSearchApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrConVol

CTrConVolApp::CTrConVolApp()
{
}

CTrConVolApp::~CTrConVolApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTrConVol message handlers

BOOL CTrConVolApp::InitInstance()
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
	if(pWnd->Create(NULL, "TRCONVOL", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), parent, 101) == FALSE)
		return NULL;
	
	return pWnd;
}
