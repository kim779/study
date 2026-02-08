// TabSubIndicatorSetUpCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "chartenvsetupdlgaddin.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "BaseDialog.h"
#include "DlgMainTabIndicatorSetUp.h"
#include "GraphSetting.h"
#include "DlgSubTabIndicatorExp.h"
#include "TabSubIndicatorSetUpCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSubIndicatorSetUpCtrl

CTabSubIndicatorSetUpCtrl::CTabSubIndicatorSetUpCtrl()
{
	m_pParentDlg = NULL;
}


CTabSubIndicatorSetUpCtrl::~CTabSubIndicatorSetUpCtrl()
{
	for( int nCount = 0 ; nCount < m_nTotalTabPages ; nCount++ )
	{
		if( m_aTabPages[nCount] != NULL)
		{
			m_aTabPages[nCount]->DestroyWindow();
			delete m_aTabPages[nCount];
		}
	}
}


BEGIN_MESSAGE_MAP(CTabSubIndicatorSetUpCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CTabSubIndicatorSetUpCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSubIndicatorSetUpCtrl message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 10
// Return void	: 
// Param  NMHDR* pNMHDR : 
// Param  LRESULT* pResult : 
// Comments		: 
//-----------------------------------------------------------------------------
void CTabSubIndicatorSetUpCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nPrevTabPageIndex = m_nCurTabPage;
	m_nCurTabPage = GetCurSel();
	*pResult = 0;
	
	if(nPrevTabPageIndex == m_nCurTabPage)
		return;

	// 설명 Tab으로 전환
	if( nPrevTabPageIndex == 0 && m_nCurTabPage == 1 )
	{
		m_pParentDlg->ShowChildDialog( FALSE );
		m_aTabPages[0]->ShowWindow(SW_HIDE);
		m_aTabPages[1]->ShowWindow(SW_SHOW);
		ReadIndicatorExpFile();
		m_aTabPages[1]->SetFocus();
	}

	else if( nPrevTabPageIndex == 1 && m_nCurTabPage == 0 )
	{
		m_aTabPages[1]->ShowWindow(SW_HIDE);
		m_aTabPages[0]->ShowWindow(SW_SHOW);
		m_pParentDlg->ShowChildDialog( TRUE );
		m_pParentDlg->ShowChildDialog( TRUE );
		m_aTabPages[0]->SetFocus();
	}
}



void CTabSubIndicatorSetUpCtrl::Init( IChartOCX *p_pIChartOCX, CTabParentDlg *pParentDlg)
{
	// pParentDlg의 값을 확인한다.
	if( pParentDlg == NULL )
		return;

	m_pParentDlg = pParentDlg;

	// 현재에는 TabPage의 개수가 2개로 정해져 있다.
// 2008.01.25 by LYH >>
//	InsertItem( 0, _T("설정") );
	CString strSetting, strDescript;
	strSetting.LoadString(IDS_STRING_SETTING);
	strDescript.LoadString(IDS_STRING_DESCRIPT);
	InsertItem( 0, strSetting);
// 2008.01.25 by LYH <<

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - 동부증권과 타 증권사의 UI를 분리함.
	int nVersion = GetAddInIVersion();

	InsertItem( 1, strDescript );
	m_nTotalTabPages = 2;

	// Active되어야 되는 Tab Page의 Index를 저장한다.
	m_nCurTabPage = 0;

	// Tab Page를 생성한다.
	CreateTabPage( p_pIChartOCX);
	SetTabPagePosition();
}


void CTabSubIndicatorSetUpCtrl::CreateTabPage( IChartOCX *p_pIChartOCX)
{
	// 1. 각 하위 TabPage를 저장할 m_tabPages[]를 초기화 한다.
	m_aTabPages[0] = NULL;
	m_aTabPages[1] = NULL;
	

	// 2. Tab Dialog를 생성한다.
	m_aTabPages[0] = new CDialog( IDD_SUBTAB_PAGE_SETUP, this);
	m_aTabPages[0]->Create(IDD_SUBTAB_PAGE_SETUP, this);
	
	m_aTabPages[1] = new CDlgSubTabIndicatorExp( p_pIChartOCX);
	m_aTabPages[1]->Create(IDD_SUBTAB_PAGE_EXP, this);
}


CWnd *CTabSubIndicatorSetUpCtrl::GetSubTab( int nSubTabIdx )
{
	if( m_aTabPages[nSubTabIdx] != NULL )
		return m_aTabPages[nSubTabIdx]->GetDlgItem( IDC_SUBDLG_SETUP );

	return NULL;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 6 / 30
// Return void : 
// Comments		: Tab위에 올라갈 Dlg의 위치잡기
//-----------------------------------------------------------------------------
void CTabSubIndicatorSetUpCtrl::SetTabPagePosition()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	// 1. Tab의 영역을 얻어온다.
	GetClientRect( &tabRect );
	GetItemRect( 0, &itemRect );

	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 1;
	nYc = tabRect.bottom - nY - 1;

	for( int nCount = 0 ; nCount < m_nTotalTabPages ; nCount++ )
	{
		if( nCount == m_nCurTabPage )
		{
			SetCurSel( nCount );
			m_aTabPages[nCount]->SetWindowPos( &wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW );
		}

		else
			m_aTabPages[nCount]->SetWindowPos( &wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW );
	}
}


void CTabSubIndicatorSetUpCtrl::ReadIndicatorExpFile( void )
{
	if( m_aTabPages[1] != NULL )
		((CDlgSubTabIndicatorExp *)m_aTabPages[1])->ReadIndicatorExpFile( m_strIndicatorName );
}


CString CTabSubIndicatorSetUpCtrl::GetSubGraphName( void )
{
	return m_strIndicatorName;
}

	
void CTabSubIndicatorSetUpCtrl::SetSubGraphName( CString strIndicatorName )
{
	m_strIndicatorName = strIndicatorName;
}


BOOL CTabSubIndicatorSetUpCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentDlg != NULL )
			{
				((CDlgMainTabIndicatorSetUp *)m_pParentDlg)->OK();
			}
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentDlg != NULL )
			{
				((CDlgMainTabIndicatorSetUp *)m_pParentDlg)->Cancel();
			}
		}
	}
	
	return CTabCtrl::PreTranslateMessage(pMsg);
}