// ConditionSearch.cpp : implementation file
//

#include "stdafx.h"
#include "ConditionSearch.h"
#include "ScreenWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CConditionSearchApp theApp;
HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CConditionSearch

CConditionSearchApp::CConditionSearchApp()
{
}

CConditionSearchApp::~CConditionSearchApp()
{
}

BOOL CConditionSearchApp::InitInstance()
{
	g_hInstance = theApp.m_hInstance;

	return CWinApp::InitInstance();
}


BEGIN_MESSAGE_MAP(CConditionSearchApp, CWinApp)
	//{{AFX_MSG_MAP(CConditionSearch)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



extern "C" __declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//20120112 이문수 리소스체크기능추가 >>
	BOOL bResult	= CUtils::CheckResource();
	if(!bResult)	return NULL;
	//20120112 이문수 <<

	CScreenWnd* pWnd = new CScreenWnd(parent);

	if(pWnd->Create(NULL, "CONDITIONSEARCH", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), parent, 101) == FALSE)
		return NULL;

	return pWnd;
}