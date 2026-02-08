// Sheet1.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "Sheet1.h"
#include "StgPage.h"
#include "SonikPage.h"
#include "MyStgPage.h"
#include "HedgePage.h"
#include "SearchPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheet1

IMPLEMENT_DYNAMIC(CSheet1, CPropertySheet)

CSheet1::CSheet1(UINT nIDCaption, CWnd* pParent, UINT iSelectPage, CWnd* pView)
	:CPropertySheet(nIDCaption, pParent, iSelectPage)
{
	m_pSonikPage = std::make_unique<CSonikPage>(pView, pParent);
	m_pStgPage = std::make_unique<CStgPage>(pView, pParent);
	m_pMyStgPage = std::make_unique <CMyStgPage>(pView, pParent);
	m_pHedgePage = std::make_unique <CHedgePage>(pView, pParent);
	AddPage(m_pSonikPage.get());
	AddPage(m_pStgPage.get());
	AddPage(m_pMyStgPage.get());
	AddPage(m_pHedgePage.get());
}

CSheet1::CSheet1(LPCTSTR pszCaption, CWnd* pParent, UINT iSelectPage, CWnd* pView)
	:CPropertySheet(pszCaption, pView, iSelectPage)
{
	m_pSonikPage = std::make_unique<CSonikPage>(pView, pParent);
	m_pStgPage = std::make_unique<CStgPage>(pView, pParent);
	m_pMyStgPage = std::make_unique <CMyStgPage>(pView, pParent);
	m_pHedgePage = std::make_unique <CHedgePage>(pView, pParent);
	AddPage(m_pSonikPage.get());
	AddPage(m_pStgPage.get());
	AddPage(m_pMyStgPage.get());
	AddPage(m_pHedgePage.get());
}

CSheet1::~CSheet1()
{
}

BEGIN_MESSAGE_MAP(CSheet1, CPropertySheet)
	//{{AFX_MSG_MAP(CSheet1)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheet1 message handlers

void CSheet1::OnDestroy() 
{
	//DELWND(m_pSonikPage);
	//DELWND(m_pStgPage);
	//DELWND(m_pMyStgPage);
	//DELWND(m_pHedgePage);

	CPropertySheet::OnDestroy();
}

BOOL CSheet1::OnEraseBkgnd(CDC* pDC) 
{	
	//return CPropertySheet::OnEraseBkgnd(pDC);
	return TRUE;
}

void CSheet1::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);
	
	if (!m_pSonikPage->GetSafeHwnd()) 
		return;

	CRect rcClient;
	GetClientRect(&rcClient);
	const int idxActive = GetActiveIndex();

	switch(idxActive)
	{
	case 0:
		if (m_pSonikPage->GetSafeHwnd())
			::SetWindowPos(m_pSonikPage->m_hWnd, HWND_TOP, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 0); 
		break;
	case 1:
		if (m_pStgPage->GetSafeHwnd())
			::SetWindowPos(m_pStgPage->m_hWnd, HWND_TOP, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 0); 
		break;
	case 2:
		if (m_pMyStgPage->GetSafeHwnd())
			::SetWindowPos(m_pMyStgPage->m_hWnd, HWND_TOP, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 0); 
		break;
	case 3:
		if (m_pHedgePage->GetSafeHwnd())
			::SetWindowPos(m_pHedgePage->m_hWnd, HWND_TOP, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 0); 
		break;
	}
}

void CSheet1::SetBkColor(CDC* pDc, int nSelTab, COLORREF clrBK)
{

}