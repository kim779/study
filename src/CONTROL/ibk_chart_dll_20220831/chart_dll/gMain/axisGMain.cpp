#include "stdafx.h"
#include "axisGMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CAxisGMainApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisGMainApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAxisGMainApp::CAxisGMainApp()
{
}

CAxisGMainApp theApp;

#include "../../h/axisfire.h"
CFont* CAxisGMainApp::GetFont(CWnd *pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CAxisGMainApp::GetPen(CWnd *pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CAxisGMainApp::GetBrush(CWnd *pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}
