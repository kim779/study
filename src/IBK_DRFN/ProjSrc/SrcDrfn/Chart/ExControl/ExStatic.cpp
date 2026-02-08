// ExStatic.cpp: implementation of the CExStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExStatic.h"
#include "../../Inc/ExDialog.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CExStatic, CStatic)

/////////////////////////////////////////////////////////////////////////////
// CExStatic

CExStatic::CExStatic(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderStatic(this, bUseDefSkin);
}

CExStatic::~CExStatic()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExStatic, CStatic)
	//{{AFX_MSG_MAP(CExStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExStatic message handlers
long CExStatic::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

void CExStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_pLoader->OnPaint(&dc);
}

BOOL CExStatic::SetStaticColor(COLORREF clrOutLine, COLORREF clrBackground, COLORREF clrText)
{
	return m_pLoader->SetStaticColor(clrOutLine, clrBackground, clrText);
}

BOOL CExStatic::SetSkinIndex(UINT nOutLine, UINT nBackground, UINT nText)
{
	((CExDialog*)GetParent())->SetChangeSkinNotifyFunction(m_pLoader->GetSkinNotifyFunction());
	return m_pLoader->SetStaticIndex(nOutLine, nBackground, nText);
}

BOOL CExStatic::SetSkinString(LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr)
{
	((CExDialog*)GetParent())->SetChangeSkinNotifyFunction(m_pLoader->GetSkinNotifyFunction());
	return m_pLoader->SetStaticString(lpszOutLine, lpszBackground, lpszTextClr);
}

BOOL CExStatic::SetIconImage(CSize size, HICON hIcon, UINT nIconAlign/* = STC_ICON_NORMAL*/)
{
	return m_pLoader->SetIconImage(size, hIcon, nIconAlign);
}

void CExStatic::SetText(LPCTSTR lpszText)
{
	m_pLoader->SetText(lpszText);
}

void CExStatic::PreSubclassWindow() 
{
	m_pLoader->PreSubclassWindow();
}

LRESULT CExStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return m_pLoader->WindowProc(message, wParam, lParam);
}

BOOL CExStatic::OnEraseBkgnd(CDC* pDC)
{
	return m_pLoader->OnEraseBkgnd(pDC);
}

void CExStatic::GetWindowText(CString& rString)
{
	m_pLoader->GetWindowText(rString);
}