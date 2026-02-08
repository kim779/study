#include "StdAfx.h"

#include "../../Inc/ExKoscomCtrl.h"
#include "stdafx.h"
#include "Include/KoscomLoaderHelper.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExKoscomCtrl::CExKoscomCtrl(void)
{
	m_pLoader = new CLoaderKoscomCtrl();
	m_bCreated = FALSE;
}

CExKoscomCtrl::~CExKoscomCtrl(void)
{
	delete m_pLoader;
}

BOOL CExKoscomCtrl::InitKoscomCtrl(CWnd *pParentWnd, UINT nCtrlType, UINT nCtrlID, long pMasterMgr)
{
	CRect rcControl;
	pParentWnd->GetDlgItem(nCtrlID)->GetWindowRect(rcControl);
	pParentWnd->ScreenToClient(rcControl);

	pParentWnd->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	
	if(m_bCreated)
		m_pLoader->ExitKoscomCtrl();
	
	m_bCreated = m_pLoader->CreateKoscomCtrl(pParentWnd, NULL, nCtrlType, rcControl, nCtrlID, pMasterMgr);
	return m_bCreated;
}

BOOL CExKoscomCtrl::InitCodeInput(CWnd *pParentWnd, UINT nCtrlID, UINT nProType)
{
	CRect rcControl;
	pParentWnd->GetDlgItem(nCtrlID)->GetWindowRect(rcControl);
	pParentWnd->ScreenToClient(rcControl);

	pParentWnd->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);

	if(m_bCreated)
		m_pLoader->ExitKoscomCtrl();

	m_bCreated = m_pLoader->CreateCodeInput(pParentWnd, NULL, rcControl, nCtrlID, nProType);
	return m_bCreated;
}

BOOL CExKoscomCtrl::InitButton(CWnd *pParentWnd, UINT nCtrlID)
{
	CRect rcControl;
	pParentWnd->GetDlgItem(nCtrlID)->GetWindowRect(rcControl);
	pParentWnd->ScreenToClient(rcControl);

	pParentWnd->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);

	if(m_bCreated)
		m_pLoader->ExitKoscomCtrl();

	m_bCreated = m_pLoader->CreateButton(pParentWnd, NULL, rcControl);
	return m_bCreated;
}

void CExKoscomCtrl::SetDataText(LPCTSTR szText)
{
	m_pLoader->SetDataText(szText);
}


CString CExKoscomCtrl::GetDataText()
{
	return m_pLoader->GetDataText();
}

long CExKoscomCtrl::GetBase(LPCTSTR lpszKey)
{
//	return m_pLoader->GetBase(lpszKey);
	return 0L;
}

void CExKoscomCtrl::SetEnabled(BOOL bEnable)
{
	m_pLoader->SetEnabled(bEnable);
}

BOOL CExKoscomCtrl::GetEnabled()
{
	return m_pLoader->GetEnabled();
}

void CExKoscomCtrl::SetFocus()
{
	m_pLoader->SetFocus();
}

OLE_HANDLE CExKoscomCtrl::GetHWnd()
{
	return m_pLoader->GetHWnd();
}

HWND CExKoscomCtrl::GetSafeHwnd()
{
	return m_pLoader->GetSafeHwnd();
}

void CExKoscomCtrl::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(x, y, nWidth, nHeight, bRepaint);
}

void CExKoscomCtrl::MoveWindow(LPCRECT lpRect, BOOL bRepaint/* = TRUE*/)
{
	m_pLoader->MoveWindow(lpRect, bRepaint);
}

BOOL CExKoscomCtrl::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
{
	return m_pLoader->SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
}

BOOL CExKoscomCtrl::ShowWindow(int nCmdShow)
{
	return m_pLoader->ShowWindow(nCmdShow);
}

void CExKoscomCtrl::GetWindowRect(LPRECT lpRect)
{
	m_pLoader->GetWindowRect(lpRect);
}

void CExKoscomCtrl::GetClientRect(LPRECT lpRect)
{
	m_pLoader->GetClientRect(lpRect);
}

void CExKoscomCtrl::SetMarketKind(UINT nMarketKind)
{
	m_pLoader->SetMarketKind(nMarketKind);
}

UINT CExKoscomCtrl::GetMarketKind()
{
	return m_pLoader->GetMarketKind();
}
