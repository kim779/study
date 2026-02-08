// DlgEnvSetUpAll.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "DlgEnvSetUpAll.h"

#include "../Include_Chart/OutsideCommonInterface.h"		// for CMChartInterface

#include "ChartEnvSetupDlgAddInImp.h"
#include "Conversion.h"
#include "DlgMainTabIndicatorSetUp.h"
#include "DlgMainTabBaseEnvSetUp.h"
#include "DlgMainTabAnalysisSetUp.h"
#include "DlgMainTabStyleSetUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define INDICATOR_TAB	0
#define BASE_ENV_TAB	1
#define ANALYSIS_TAB	2
#define	STYLE_TAB		3


/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetUpAll dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
// Return void	: 
// Param  IChartOCX *p_pIChartOCX : 
// Param  CWnd* pParent = NULL : 
// Comments		: 
//-----------------------------------------------------------------------------
CDlgEnvSetUpAll::CDlgEnvSetUpAll( IChartOCX *p_pIChartOCX, const int nType, const int nActiveTabIdx, CChartAddInBase *pParentAddIn, CWnd* pParent, BOOL p_bWin98)
				: CDialog(CDlgEnvSetUpAll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnvSetUpAll)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pIChartOCX = p_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();
	
	m_nType = nType;
	m_nActiveTabIdx = nActiveTabIdx;

	m_pParentAddIn = NULL;
	m_pParentAddIn = pParentAddIn;

	// (2007/1/19 - Seung-Won, Bae) Check OS Version. (for excepting Win98)
	m_bWin98 = p_bWin98;
}


CDlgEnvSetUpAll::~CDlgEnvSetUpAll()
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
}

void CDlgEnvSetUpAll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvSetUpAll)
	DDX_Control(pDX, IDC_MAINTAB_ALL, m_mtabChartEnvSetUpAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvSetUpAll, CDialog)
	//{{AFX_MSG_MAP(CDlgEnvSetUpAll)
	ON_BN_CLICKED(ID_BTN_PREVIEW, OnBtnPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetUpAll message handlers


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 8
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CDlgEnvSetUpAll::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	//<<20100420_JS.Kim 리소스부족 체크
	if( !m_mtabChartEnvSetUpAll.GetSafeHwnd() || !::IsWindow(m_mtabChartEnvSetUpAll.GetSafeHwnd()) )
	{
		OnCancel();
		return FALSE;
	}
	//>>

	// 1. Dialog에 원버튼으로 포커스를 주기 위해 사용
	mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
	mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );

	// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - 동부증권과 타 증권사의 UI를 분리함.
	int nVersion = GetAddInIVersion();
	if( 419999 < nVersion && nVersion < 430000  )
		m_nType = 0;
	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
//	else	
//		m_nType = 1;

	//<<20100420_JS.Kim 리소스부족 체크
	//m_mtabChartEnvSetUpAll.Init( m_pIChartOCX, this, m_nType, m_nActiveTabIdx);
	if( !m_mtabChartEnvSetUpAll.Init( m_pIChartOCX, this, m_nType, m_nActiveTabIdx) )
	{
		OnCancel();
		return FALSE;
	}
	//>>
	
#ifdef _DEBUG
	CRect rcWindowRect;
	GetWindowRect( rcWindowRect );
	TRACE( "%d, %d, %d, %d", rcWindowRect.left, rcWindowRect.top, rcWindowRect.right, rcWindowRect.bottom );
#endif

	//<<20100402_JS.Kim 솔로몬.  
	// 듀얼모니터 Second 모니터에서 차트화면을 윈도우 영역밖으로 옮긴 다음 환경설정창 띄우면 탭이 왼쪽으로 이동하는 버그 때문에 추가
	CRect rcTab;
	m_mtabChartEnvSetUpAll.GetWindowRect(&rcTab);
	ScreenToClient(&rcTab);

	if( rcTab.left < 0 || rcTab.top < 0 )
	{
		rcTab.OffsetRect(rcTab.left<0 ? 6-rcTab.left : 0, rcTab.top<0 ? 7-rcTab.top : 0);
		m_mtabChartEnvSetUpAll.SetWindowPos(NULL, rcTab.left, rcTab.top, rcTab.Width(), rcTab.Height(), SWP_SHOWWINDOW|SWP_NOZORDER);
	}
	//>>

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgEnvSetUpAll::OnOK() 
{
	int nInitChartIndex = -1;
	POSITION pos = m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetHeadPosition();

	while(pos)
	{
		switch( m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetNext(pos) )
		{
			case 0 :
			{
				CDlgMainTabIndicatorSetUp* pIndiSetUp = (CDlgMainTabIndicatorSetUp*) m_mtabChartEnvSetUpAll.GetTabPages(0);
				pIndiSetUp->Save();
				nInitChartIndex = pIndiSetUp->GetInitChartIndex();
				break;
			}
			case 1 :
			{
				((CDlgMainTabBaseEnvSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(1))->Save();
				break;
			}
			case 2 :
			{
				// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - DongBu와 다른 증권사 UI 분리
				if( m_nType == LOAD_SETUP_ALL )
					((CDlgMainTabAnalysisSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->Save();
				else
					((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->Save();
				break;
			}
			case 3 :
			{
				((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(3))->Save();
				break;
			}
		}
	}

	//sy 2006.04.20. -> 지표설정창에서 임시로 사용한 ChartCfg 삭제 및 현화면 저장.
	m_pIChartOCX->SaveChartCFG_RemoveFromIndexToEnd( nInitChartIndex);
	m_pIChartOCX->SaveChartCFG_SaveNow();

	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	COutsideCommonInterfaceFunction outsideIF;
	ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
	outsideIF.SetOutsideCommonInterface( m_pIChartOCX->GetMChartInterface(), CString( szDataPath));
	outsideIF.MChart_OnChartEnvSetupOk();

	CDialog::OnOK();
}


void CDlgEnvSetUpAll::OnBtnPreview() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetHeadPosition();

	while(pos)
	{
		switch( m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetNext(pos) )
		{
			case 0 :
			{
				((CDlgMainTabIndicatorSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(0))->PreView();
				break;
			}
			case 1 :
			{
				((CDlgMainTabBaseEnvSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(1))->PreView();
				break;
			}
			case 2 :
			{
				// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - DongBu와 다른 증권사 UI 분리
				if( m_nType == LOAD_SETUP_ALL )
					((CDlgMainTabAnalysisSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->PreView();
				else
					((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->PreView();
				break;
			}
			case 3 :
			{
				((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(3))->PreView();
				break;
			}
		}
	}
}


void CDlgEnvSetUpAll::OnCancel() 
{
	// TODO: Add extra cleanup here
	POSITION pos = m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetHeadPosition();

	while(pos)
	{
		switch( m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetNext(pos) )
		{
			case 0 :
			{
				if( ((CDlgMainTabIndicatorSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(0))->m_bPreview == TRUE )
					((CDlgMainTabIndicatorSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(0))->Recover();
				break;
			}
			case 1 :
			{
				if( ((CDlgMainTabBaseEnvSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(1))->m_bPreview == TRUE )
					((CDlgMainTabBaseEnvSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(1))->Recover();

				break;
			}
			case 2 :
			{
				// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - DongBu와 다른 증권사 UI 분리
				if( m_nType == LOAD_SETUP_ALL )
				{
					if( ((CDlgMainTabAnalysisSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->m_bPreview == TRUE )
						((CDlgMainTabAnalysisSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->Recover();
				}
				else
				{
					if( ((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->m_bPreview == TRUE )
						((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(2))->Recover();
				}
				break;
			}
			case 3 :
			{
				if( ((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(3))->m_bPreview == TRUE )
					((CDlgMainTabStyleSetUp *)m_mtabChartEnvSetUpAll.GetTabPages(3))->Recover();
				break;
			}
		}
	}

//	m_pIChartOCX->InvalidateOCX();	
	CDialog::OnCancel();
}


BOOL CDlgEnvSetUpAll::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnOK();
			return TRUE;
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			OnCancel();
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgEnvSetUpAll::BackUpIndicatorInfo( IStringList *pStringList )
{
	if( pStringList == NULL )
		return;

	if( m_pParentAddIn )
		((CChartEnvSetupDlgAddInImp *)m_pParentAddIn)->BackUpIndicatorInfo(pStringList);
}


//KHD : 일 주 년 월의 그리드보를 가져간다. 
int CDlgEnvSetUpAll::GetGridTypeCombo(int nType)
{
	POSITION pos = m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetHeadPosition();
	int _nx = 0;
	CDialog *pDlg = NULL;
	CDlgMainTabStyleSetUp *pStyleDlg = NULL;//스타일텝 정보를 가죠온다.
	while( pos )
	{
		_nx = m_mtabChartEnvSetUpAll.m_lstTabActiveOrder.GetNext( pos );
		pDlg = m_mtabChartEnvSetUpAll.GetTabPages( _nx );
		if( pDlg == NULL ) continue;

		if( pDlg->IsKindOf( RUNTIME_CLASS( CDlgMainTabStyleSetUp ) ) )
		{
			pStyleDlg = (CDlgMainTabStyleSetUp *)pDlg;
			break;
		}
	}

	if( !pStyleDlg )
		return 0;

	if( pStyleDlg->IsKindOf( RUNTIME_CLASS( CDlgMainTabStyleSetUp ) ) )
	{
		if( nType == CScaleBaseData::MINTICK_DAY_GUBUN )
			return pStyleDlg->m_cmbHScaleGridType_Day.GetCurSel();
		else if( nType == CScaleBaseData::DAYCHART_WEEK_GUBUN )
			return pStyleDlg->m_cmbHScaleGridType_Week.GetCurSel();
		else if( nType == CScaleBaseData::DAYCHART_MON_GUBUN || nType == CScaleBaseData::WEEKCHART_MON_GUBUN )
			return pStyleDlg->m_cmbHScaleGridType_Mon.GetCurSel();
		else if( nType == CScaleBaseData::DAYCHART_YEAR_GUBUN || nType == CScaleBaseData::WEEKCHART_YEAR_GUBUN || nType == CScaleBaseData::MONCHART_YEAR_GUBUN )
			return pStyleDlg->m_cmbHScaleGridType_Year.GetCurSel();

	}

	return -1;
}
