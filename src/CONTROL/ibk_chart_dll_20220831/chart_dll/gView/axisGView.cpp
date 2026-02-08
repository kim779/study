#include "stdafx.h"
#include "axisGView.h"
#include "grpView.h"
#include <stdarg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAxisGViewApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisGViewApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAxisGViewApp::CAxisGViewApp()
{
	m_root = m_uid = m_uname = m_tabDir = m_userDir = m_gexDir = _T("");
}

CAxisGViewApp::~CAxisGViewApp()
{
}

CAxisGViewApp theApp;

CWnd* CAxisGViewApp::Create(CWnd* pwndView, char* pcDllName)
{
	m_root =  (LPCTSTR)pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC));
	m_uname = (LPCTSTR)pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC));
	m_uid =   (LPCTSTR)pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC));
	
	m_tabDir.Format ("%s\\%s\\", m_root, TABDIR);
	m_userDir.Format("%s\\%s\\", m_root, USRDIR);
	m_gexDir.Format ("%s\\%s\\", m_root, GEXDIR);
	m_imgDir.Format ("%s\\%s\\", m_root, IMAGEDIR);

	CGrpView* pGrpView = new CGrpView(pwndView, pcDllName);
	if (!pGrpView->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
		CRect(0, 0, 1, 1), pwndView, 0))
	{
		delete pGrpView;
		pGrpView = NULL;
		return NULL;
	}

	return pGrpView;
}

CFont* CAxisGViewApp::GetFont(CWnd *pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (LPSTR)(LPCTSTR)name;
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CAxisGViewApp::GetPen(CWnd *pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CAxisGViewApp::GetBrush(CWnd *pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}

UINT CAxisGViewApp::MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT button)
{
	return AfxGetMainWnd()->MessageBox(lpszText, lpszCaption, button);
}

UINT CAxisGViewApp::MessageBoxF(LPCTSTR lpszFormat, ...)
{
	char szTemp[4096];
	
	va_list argList;
	va_start(argList, lpszFormat);
	vsprintf(szTemp, lpszFormat, argList);
	va_end(argList);

	return MessageBox(szTemp, COMPANYNAME);
}


__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pwndView, char* pcDllName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CAxisGViewApp* pApp = (CAxisGViewApp *)AfxGetApp();
	CWnd* result = pApp->Create(pwndView, pcDllName);

	return result;
}


