// TabMainChartEnvSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "TabMainChartEnvSetUp.h"

#include "DlgEnvSetUpAll.h"
#include "DlgMainTabIndicatorSetUp.h"
#include "DlgMainTabBaseEnvSetUp.h"
#include "DlgMainTabAnalysisSetUp.h"
#include "DlgMainTabStyleSetUp.h"

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabMainChartEnvSetUp

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 8
// Modifier		: 
// Comments		: Construction
//-----------------------------------------------------------------------------
CTabMainChartEnvSetUp::CTabMainChartEnvSetUp()
{
	m_pIChartOCX = NULL;
	m_pDlgEnvSetUpAll = NULL;
	
	if( m_lstTabActiveOrder.GetCount() > 0 )
		m_lstTabActiveOrder.RemoveAll();
	
	m_nUIType = 0;

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	m_bPriceChart = FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 8
// Modifier		: 
// Comments		: Destruction
//-----------------------------------------------------------------------------
CTabMainChartEnvSetUp::~CTabMainChartEnvSetUp()
{
	for( int nCount = 0 ; nCount < m_nTotalTabPages ; nCount++ )
	{
		if( m_aTabPages[nCount] != NULL)
			delete m_aTabPages[nCount];
	}

	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTabMainChartEnvSetUp, CTabCtrl)
	//{{AFX_MSG_MAP(CTabMainChartEnvSetUp)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabMainChartEnvSetUp message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 8
// Return void  : 
// Param  IChartOCX *p_pIChartOCX : 
// Comments		: 인자로 받은 값으로 Child TabPage를 생성한다.
//-----------------------------------------------------------------------------
BOOL CTabMainChartEnvSetUp::Init( IChartOCX *p_pIChartOCX, CDlgEnvSetUpAll *pParentDlg, int nType, int nActiveTabIdx )
{
	if( p_pIChartOCX == NULL )
		return FALSE;

	// 1. 인자로 받은 값을 각각 Member 변수와 연결한다.
	if( m_pIChartOCX) m_pIChartOCX->Release();
	m_pIChartOCX = p_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();

	// Sang-Woo, Cho	2005 / 11 / 02	추가
	m_pDlgEnvSetUpAll = pParentDlg;

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) 
	m_nUIType = nType;

// 2008.01.25 by LYH >>
	CString strTabIndicator, strTabBase, strTabAnal, strTabStyle;
	strTabIndicator.LoadString(IDS_TAB_INDICATOR_SET);
	strTabBase.LoadString(IDS_TAB_BASE_SET);
	strTabAnal.LoadString(IDS_TAB_ANAL_SET);
	strTabStyle.LoadString(IDS_TAB_STYLE_SET);
// 2008.01.25 by LYH <<

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	// 가격차트인지 확인하는 함수
	CheckPriceChart();

	// 2. 각 TabPage의 Title을 설정한다.
	switch( m_nUIType )
	{
		case LOAD_SETUP_ALL:
		{
// 2008.01.25 by LYH >>
			//InsertItem( 0, _T("차트 / 지표") );
			//InsertItem( 1, _T("기본환경") );
			//InsertItem( 2, _T("분석도구 설정") );
			//InsertItem( 3, _T("스타일") );
			InsertItem( 0, strTabIndicator );
			InsertItem( 1, strTabBase );
			InsertItem( 2, strTabAnal );
			InsertItem( 3, strTabStyle );
// 2008.01.25 by LYH <<
			// Sub TabPage의 개수
			m_nTotalTabPages = 4;
			break;
		}

		case LOAD_OTHER_UI:
		{
// 2008.01.25 by LYH >>
			//InsertItem( 0, _T("차트 / 지표") );
			//InsertItem( 1, _T("기본환경") );
			//InsertItem( 2, _T("스타일") );
			InsertItem( 0, strTabIndicator );
			InsertItem( 1, strTabBase );
			InsertItem( 2, strTabStyle );
// 2008.01.25 by LYH <<
			// Sub TabPage의 개수
			m_nTotalTabPages = 3;
			break;
		}
		// 환경설정 탭별 선택 가능 - ojtaso (20080109)
		case LOAD_SEL_ITEM:
		{
			if(nActiveTabIdx == 0)
				InsertItem( 0, strTabIndicator );
			else if(nActiveTabIdx == 1)
				InsertItem( 0, strTabBase );
			else if(nActiveTabIdx == 2)
				InsertItem( 0, strTabAnal );
			else if(nActiveTabIdx == 3)
				InsertItem( 0, strTabStyle );
			
			m_nTotalTabPages = 1;
			break;
		}
	}

	if( m_nUIType < 0 )
		m_nCurTabPage = 0;
	else
		m_nCurTabPage = nActiveTabIdx;

	// 4. TabPage들을 생성한다.
	//<<20100420_JS.Kim 리소스부족 체크
	//CreateTabPage();
	if( CreateTabPage() == FALSE )
		return FALSE;
	//>>
	
	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	if(m_nUIType == LOAD_SEL_ITEM)
		m_nCurTabPage = 0;

	SetTabPagePosition();

	long lResult;
	m_nCurTabPage = 0;
	OnSelchange( NULL, &lResult);

	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	if(m_nUIType == LOAD_SEL_ITEM && nActiveTabIdx)
		TabPageMoveWindow(FALSE);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 8
// Return void  : 
// Param  void  : 
// Comments		: TabPage들을 생성한다.
//-----------------------------------------------------------------------------
BOOL CTabMainChartEnvSetUp::CreateTabPage( void )
{
	// 1. 각 하위 TabPage를 저장할 m_tabPages[]를 초기화 한다.
	for( int nCount = 0 ; nCount < m_nTotalTabPages ; nCount++ )
	{
		m_aTabPages[ nCount ] = NULL;
	}

	// (2009/9/1 - Seung-Won, Bae) Show UI Dialog of current chart mode.
	UINT nEnvType = IDD_MAINTAB_PAGE_BASE_ENV_HTS;
	if( m_pIChartOCX)
	{
		switch( m_pIChartOCX->GetChartMode())
		{
			case CChartInfo::DEVELOPER:	nEnvType = IDD_MAINTAB_PAGE_BASE_ENV_DEV;
										break;
			case CChartInfo::FX:		nEnvType = IDD_MAINTAB_PAGE_BASE_ENV_FX;
										break;
			case CChartInfo::WORLD_ON:	nEnvType = IDD_MAINTAB_PAGE_BASE_ENV_WORLDON;
										break;
		}
	}

	// 2. 각 TabPage들을 생성한다.
	if( m_nUIType == LOAD_SETUP_ALL )
	{
		m_aTabPages[0] = new CDlgMainTabIndicatorSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
		if( !m_aTabPages[0] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
		m_aTabPages[1] = new CDlgMainTabBaseEnvSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll, m_bPriceChart);
		if( !m_aTabPages[1] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
		m_aTabPages[2] = new CDlgMainTabAnalysisSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
		if( !m_aTabPages[2] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
		m_aTabPages[3] = new CDlgMainTabStyleSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
		if( !m_aTabPages[3] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크

		if( !m_aTabPages[0]->Create(IDD_MAINTAB_PAGE_INDICATOR, this) )
			return FALSE;	//20100420_JS.Kim 리소스부족 체크
		if( !m_aTabPages[1]->Create( nEnvType, this) )
			return FALSE;	//20100420_JS.Kim 리소스부족 체크
		if( !m_aTabPages[2]->Create(IDD_MAINTAB_PAGE_ANALYSIS, this) )
			return FALSE;	//20100420_JS.Kim 리소스부족 체크
		if( !m_aTabPages[3]->Create(IDD_MAINTAB_PAGE_STYLE, this) )
			return FALSE;	//20100420_JS.Kim 리소스부족 체크

		m_lstTabActiveOrder.AddTail(0);
		m_lstTabActiveOrder.AddTail(1);
		m_lstTabActiveOrder.AddTail(2);
		m_lstTabActiveOrder.AddTail(3);
	}
	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	else if(m_nUIType == LOAD_SEL_ITEM)
	{
		switch(m_nCurTabPage)
		{
		case 0:
			m_aTabPages[0] = new CDlgMainTabIndicatorSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
			if( !m_aTabPages[0] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
			if( !m_aTabPages[0]->Create(IDD_MAINTAB_PAGE_INDICATOR, this) )
				return FALSE;	//20100420_JS.Kim 리소스부족 체크
			break;
		case 1:
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
			m_aTabPages[0] = new CDlgMainTabBaseEnvSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll, m_bPriceChart);
			if( !m_aTabPages[0] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
			if(!m_aTabPages[0]->Create(nEnvType, this))
				return FALSE;	//20100420_JS.Kim 리소스부족 체크
			break;
		case 2:
			m_aTabPages[0] = new CDlgMainTabAnalysisSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
			if( !m_aTabPages[0] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
			if(!m_aTabPages[0]->Create(IDD_MAINTAB_PAGE_ANALYSIS, this))
				return FALSE;	//20100420_JS.Kim 리소스부족 체크
			break;
		case 3:
			m_aTabPages[0] = new CDlgMainTabStyleSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
			if( !m_aTabPages[0] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
			if(!m_aTabPages[0]->Create(IDD_MAINTAB_PAGE_STYLE, this))
				return FALSE;	//20100420_JS.Kim 리소스부족 체크
			break;
		}
		
		m_lstTabActiveOrder.AddTail(0);
		m_aTabPages[0]->ShowWindow(SW_SHOW);
	}
	else
	{
		m_aTabPages[0] = new CDlgMainTabIndicatorSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
		if( !m_aTabPages[0] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
		m_aTabPages[1] = new CDlgMainTabBaseEnvSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll, m_bPriceChart);
		if( !m_aTabPages[1] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크
		m_aTabPages[2] = new CDlgMainTabStyleSetUp( m_pIChartOCX, m_pDlgEnvSetUpAll );
		if( !m_aTabPages[2] )		return FALSE;	//20100420_JS.Kim 리소스부족 체크

		if(!m_aTabPages[0]->Create(IDD_MAINTAB_PAGE_INDICATOR, this))
			return FALSE;	//20100420_JS.Kim 리소스부족 체크
		if(!m_aTabPages[1]->Create( nEnvType, this))
			return FALSE;	//20100420_JS.Kim 리소스부족 체크
		if(!m_aTabPages[2]->Create(IDD_MAINTAB_PAGE_STYLE, this))
			return FALSE;	//20100420_JS.Kim 리소스부족 체크

		m_lstTabActiveOrder.AddTail(0);
		m_lstTabActiveOrder.AddTail(1);
		m_lstTabActiveOrder.AddTail(2);
	}
	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 6 / 30
// Return void : 
// Comments		: Tab위에 올라갈 Dlg의 위치잡기
//-----------------------------------------------------------------------------
void CTabMainChartEnvSetUp::SetTabPagePosition( void )
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
				
			// TabPage를 전환할때 전환 순서를 기억한다.
			POSITION pos = m_lstTabActiveOrder.Find( m_nCurTabPage );
			if( pos != NULL	)
				m_lstTabActiveOrder.RemoveAt(pos);
			
			m_lstTabActiveOrder.AddTail(m_nCurTabPage);
		}

		else
			m_aTabPages[nCount]->SetWindowPos( &wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 9
// Return void	: 
// Param  NMHDR* pNMHDR : 
// Param  LRESULT* pResult : 
// Comments		: MainTab에서 TabPage가 바뀔때 해당되는 TabPage로 바꾸어 준다.
//-----------------------------------------------------------------------------
//sy 2005.10.12
void CTabMainChartEnvSetUp::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nPrevTabPageIndex = m_nCurTabPage;
	m_nCurTabPage = GetCurSel();
	*pResult = 0;

	if(nPrevTabPageIndex == m_nCurTabPage)
		return;

	CBaseTabDlg* pPrevTabDlg = m_aTabPages[nPrevTabPageIndex];
	CBaseTabDlg* pCurTabDlg = m_aTabPages[m_nCurTabPage];
	if(pPrevTabDlg == NULL || pCurTabDlg == NULL)
		return;

	if( nPrevTabPageIndex == 0 && m_nCurTabPage != 0 )
		TabPageMoveWindow(FALSE);
	else if( nPrevTabPageIndex != 0 && m_nCurTabPage == 0 )
		TabPageMoveWindow(TRUE);

	// TabPage를 전환할때 전환 순서를 기억한다.
	POSITION pos = m_lstTabActiveOrder.Find( m_nCurTabPage );
	if( pos != NULL	)
		m_lstTabActiveOrder.RemoveAt(pos);

	m_lstTabActiveOrder.AddTail(m_nCurTabPage);

	// tab 전환
	pPrevTabDlg->ShowWindow(SW_HIDE);
	pCurTabDlg->ShowWindow(SW_SHOW);
	pCurTabDlg->SetFocus();

	// tab data 교환
	ChangeDataInTab(pPrevTabDlg);
}


bool CTabMainChartEnvSetUp::ChangeDataInTab(CBaseTabDlg* pSourceTabDlg)
{
	if(pSourceTabDlg == NULL)
		return false;

	bool bResult = false;

	for(int nIndex = 0; nIndex < m_nTotalTabPages; nIndex++)
	{
		CBaseTabDlg* pTabDlg = m_aTabPages[nIndex];
		if(pTabDlg == NULL)
			break;

		if(pTabDlg->ChangeTabPage(pSourceTabDlg))
			bResult = true;
	}

	return bResult;
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
// Return CDialog : 
// Param  int index : 
// Comments		: 
//-----------------------------------------------------------------------------
CDialog *CTabMainChartEnvSetUp::GetTabPages(int index)
{
	return m_aTabPages[index];
}


BOOL CTabMainChartEnvSetUp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			m_pDlgEnvSetUpAll->OK();
		}

		if( pMsg->wParam == VK_ESCAPE )
		{
			m_pDlgEnvSetUpAll->Cancel();
		}
	}	
	
	return CTabCtrl::PreTranslateMessage(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
/////////////////////////////////////////////////////////////////////////////
void CTabMainChartEnvSetUp::UpdateAnalysisColorInStyle( COLORREF p_clrAnalysis)
{
	CBaseTabDlg *pStylePage = NULL;
	if( m_nTotalTabPages == 4) pStylePage = m_aTabPages[ 3];
	else pStylePage = m_aTabPages[ 4];
	if( !pStylePage) return;
	if( pStylePage->IsKindOf( RUNTIME_CLASS( CDlgMainTabStyleSetUp)))
		( ( CDlgMainTabStyleSetUp *)pStylePage)->UpdateAnalysisColor( p_clrAnalysis);
}
void CTabMainChartEnvSetUp::UpdateAnalysisColorInAnalysisTool( COLORREF p_clrAnalysis)
{
	CBaseTabDlg *pStylePage = NULL;
	if( m_nTotalTabPages == 4) pStylePage = m_aTabPages[ 2];
	else pStylePage = m_aTabPages[ 1];
	if( !pStylePage) return;
	if( pStylePage->IsKindOf( RUNTIME_CLASS( CDlgMainTabAnalysisSetUp)))
		( ( CDlgMainTabAnalysisSetUp *)pStylePage)->UpdateAnalysisColor( p_clrAnalysis);
}

void CTabMainChartEnvSetUp::TabPageMoveWindow(BOOL	bResult)
{
	short nMove = 0;
/*	if(bResult)
		nMove+=130;
	else
		nMove-=130;
*/
//		nMove+=130;

	CRect rcDlgEndSetUtAll, rcMainTab, rcTabPage;
	this->GetWindowRect( rcMainTab );
	m_pDlgEnvSetUpAll->GetWindowRect( rcDlgEndSetUtAll );
	m_aTabPages[ m_nCurTabPage ]->GetWindowRect( rcTabPage );

	rcDlgEndSetUtAll.right = rcDlgEndSetUtAll.right + nMove;
	m_pDlgEnvSetUpAll->MoveWindow(rcDlgEndSetUtAll);

	rcMainTab.right = rcMainTab.right + nMove;
	m_pDlgEnvSetUpAll->ScreenToClient( rcMainTab);
	MoveWindow(rcMainTab);

	rcTabPage.right = rcTabPage.right + nMove;
	ScreenToClient( rcTabPage);
	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	for( int i = 0; i < m_nTotalTabPages; i++)
		m_aTabPages[ i]->MoveWindow( rcTabPage);

	CRect rcOk, rcCancel, rcPrev;
	CButton *pBtnOk = (CButton *)m_pDlgEnvSetUpAll->GetDlgItem(IDOK);
	CButton *pBtnCancel = (CButton *)m_pDlgEnvSetUpAll->GetDlgItem(IDCANCEL);
	CButton *pBtnPrev = (CButton *)m_pDlgEnvSetUpAll->GetDlgItem(ID_BTN_PREVIEW);

	pBtnOk->GetWindowRect( rcOk );
	m_pDlgEnvSetUpAll->ScreenToClient( &rcOk);
	pBtnCancel->GetWindowRect( rcCancel );
	m_pDlgEnvSetUpAll->ScreenToClient( &rcCancel);
	pBtnPrev->GetWindowRect( rcPrev );
	m_pDlgEnvSetUpAll->ScreenToClient( &rcPrev);

	rcOk.left = rcOk.left + nMove;
	rcOk.right = rcOk.right + nMove;
	rcCancel.left = rcCancel.left + nMove;
	rcCancel.right = rcCancel.right + nMove;
	rcPrev.left = rcPrev.left + nMove;
	rcPrev.right = rcPrev.right + nMove;
	pBtnOk->MoveWindow(rcOk);
	pBtnCancel->MoveWindow(rcCancel);
	pBtnPrev->MoveWindow(rcPrev);
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CTabMainChartEnvSetUp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if( !m_aTabPages[ 0]) return;
	if( !m_aTabPages[ 0]->IsKindOf( RUNTIME_CLASS( CDlgMainTabIndicatorSetUp))) return;

	( ( CDlgMainTabIndicatorSetUp *)m_aTabPages[ 0])->OnGraphDeleted( p_szGraphName, p_szIndicatorName);
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
void CTabMainChartEnvSetUp::CheckPriceChart()
{
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();

	m_bPriceChart = FALSE;

	int nUpSpace = 0, nDownSpace = 0;
	int nR = 0, nC = 0, nV = 0, nG = 0;
	ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART ), nR, nC, nV, nG, NULL);
	if( !szGraphName.IsEmpty())
	{
		m_bPriceChart = TRUE;
	}
}