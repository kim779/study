// ExToolTipCtrl.cpp: implementation of the CExToolTipCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExToolTipCtrl.h"
#include "Include/LoadHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExToolTipCtrl::CExToolTipCtrl()
{
	m_pLoader = new CLoaderToolTipCtrl(this);
}

CExToolTipCtrl::~CExToolTipCtrl()
{
	delete m_pLoader;
}

BOOL CExToolTipCtrl::AddTool(CWnd* pWnd, LPCTSTR lpszText/* = LPSTR_TEXTCALLBACK*/, LPCRECT lpRectTool/* = NULL*/, UINT nIDTool/* = 0*/)
{
	return m_pLoader->AddTool(pWnd, lpszText, lpRectTool, nIDTool);
}

BOOL CExToolTipCtrl::AddTool(CWnd* pWnd, UINT nIDText, LPCRECT lpRectTool/* = NULL*/, UINT nIDTool/* = 0*/)
{
	return m_pLoader->AddTool(pWnd, nIDText, lpRectTool, nIDTool);
}

void CExToolTipCtrl::Activate(BOOL bActivate)
{
	m_pLoader->Activate(bActivate);
}

void CExToolTipCtrl::UpdateTipText(LPCTSTR lpszText, CWnd* pWnd, UINT nIDTool/* = 0*/)
{
	m_pLoader->UpdateTipText(lpszText, pWnd, nIDTool);
}

void CExToolTipCtrl::UpdateTipText(UINT nIDText, CWnd* pWnd, UINT nIDTool/* = 0*/)
{
	m_pLoader->UpdateTipText(nIDText, pWnd, nIDTool);
}

void CExToolTipCtrl::RelayEvent(LPMSG lpMsg)
{
	m_pLoader->RelayEvent(lpMsg);
}

HWND CExToolTipCtrl::GetSafeHwnd()
{
	return m_pLoader->GetSafeHwnd();
}

void CExToolTipCtrl::Update()
{
	m_pLoader->Update();
}

void CExToolTipCtrl::DelTool(CWnd* pWnd, UINT nIDTool/* = 0*/)
{
	m_pLoader->DelTool(pWnd, nIDTool);
}

void CExToolTipCtrl::FillInToolInfo(TOOLINFO& ti, CWnd* pWnd, UINT nIDTool)
{
	m_pLoader->FillInToolInfo(ti, pWnd, nIDTool);
}

int CExToolTipCtrl::GetDelayTime(DWORD dwDuration)
{
	return m_pLoader->GetDelayTime(dwDuration);
}

void CExToolTipCtrl::GetMargin(LPRECT lprc)
{
	m_pLoader->GetMargin(lprc);
}

int CExToolTipCtrl::GetMaxTipWidth()
{
	return m_pLoader->GetMaxTipWidth();
}

void CExToolTipCtrl::GetText(CString& str, CWnd* pWnd, UINT nIDTool/* = 0*/)
{
	m_pLoader->GetText(str, pWnd, nIDTool);
}

COLORREF CExToolTipCtrl::GetTipBkColor()
{
	return m_pLoader->GetTipBkColor();
}

COLORREF CExToolTipCtrl::GetTipTextColor()
{
	return m_pLoader->GetTipTextColor();
}

int CExToolTipCtrl::GetToolCount()
{
	return m_pLoader->GetToolCount();
}

BOOL CExToolTipCtrl::GetToolInfo(CToolInfo& ToolInfo, CWnd* pWnd, UINT nIDTool/* = 0*/)
{
	return m_pLoader->GetToolInfo(ToolInfo, pWnd, nIDTool);
}

BOOL CExToolTipCtrl::HitTest(CWnd* pWnd, CPoint pt, LPTOOLINFO lpToolInfo)
{
	return m_pLoader->HitTest(pWnd, pt, lpToolInfo);
}

void CExToolTipCtrl::Pop()
{
	m_pLoader->Pop();
}

void CExToolTipCtrl::SetDelayTime(UINT nDelay)
{
	m_pLoader->SetDelayTime(nDelay);
}

void CExToolTipCtrl::SetMargin(LPRECT lprc)
{
	m_pLoader->SetMargin(lprc);
}

int CExToolTipCtrl::SetMaxTipWidth(int iWidth)
{
	return m_pLoader->SetMaxTipWidth(iWidth);
}

void CExToolTipCtrl::SetTipBkColor(COLORREF clr)
{
	m_pLoader->SetTipBkColor(clr);
}

void CExToolTipCtrl::SetTipTextColor(COLORREF clr)
{
	m_pLoader->SetTipTextColor(clr);
}

void CExToolTipCtrl::SetToolInfo(LPTOOLINFO lpToolInfo)
{
	m_pLoader->SetToolInfo(lpToolInfo);
}

void CExToolTipCtrl::SetToolRect(CWnd* pWnd, UINT nIDTool, LPCRECT lpRect)
{
	m_pLoader->SetToolRect(pWnd, nIDTool, lpRect);
}
