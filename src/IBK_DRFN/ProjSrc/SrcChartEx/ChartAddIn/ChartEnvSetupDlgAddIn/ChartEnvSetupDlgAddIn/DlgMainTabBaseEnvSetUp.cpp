// DlgMainTabBaseEnvSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "DlgMainTabBaseEnvSetUp.h"

#include "../Include_Addin_135914/I135914/_AddInCommand.h"	// for EAI_ENVSETUP_LHLCHC_CHANGED
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for IMetaTable
#include "../Include_Chart/OutsideCommonInterface.h"		// for CMChartInterface
#include "DlgEnvSetUpAll.h"
#include "ChartEnvSetupDlgAddInImp.h"						// for CChartEnvSetupDlgAddInImp

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabBaseEnvSetUp dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 20
// Return void	: 
// Param  IChartOCX *p_pIChartOCX : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
CDlgMainTabBaseEnvSetUp::CDlgMainTabBaseEnvSetUp( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, BOOL bPriceChart /*=FALSE*/ )
	: CBaseTabDlg(CDlgMainTabBaseEnvSetUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainTabBaseEnvSetUp)
	m_nCandleTypeOnResize = 0;
	m_bShowCurrentPrice = FALSE;
	m_bYScalePositionLeftSide = FALSE;
	m_bYScalePositionRightSide = FALSE;
	m_bShowIndicatorValue = FALSE;
	m_bShowIndicatorVariable = FALSE;
	m_bUseCrossLine = FALSE;
	m_nCurPriceSub = -1;
	m_bInquieryDataWindow = FALSE;
	m_bShowAllTitle = FALSE;
	m_bShowPriceMinMaxText = FALSE;
	m_bUseTooptipShow = TRUE;
	m_bUseTooptipShowAll = TRUE;		//[A00000441]110407 풍선도움말 보기시 전체 조회 옵션 추가
	m_bFixInterval = FALSE;
	m_bDrawBaseLine = TRUE;
	m_nMouseWheelIndex = 0;
	m_nMinMaxRatio = 0;
	//}}AFX_DATA_INIT

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	m_nInquiryDlgManualMove = 0;
	m_bShowExRightDividend = FALSE;
	m_bShowHighLowBound = FALSE;
	m_bUseRevisedPrice = TRUE;
	m_nCandleType = 0;
	m_bLH_LC_HC = FALSE;
	m_bUseGapRevision = FALSE;
	m_nLH_LC_HC = -1;
	
	m_pIChartOCX = p_pIChartOCX;

	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
		ILPCSTR szDataPath = m_pIChartOCX->GetDefaultPathString();
		m_MChartInterface.SetOutsideCommonInterface( m_pIChartOCX->GetMChartInterface(), CString( szDataPath));
	}
	
	m_bPreview = FALSE;

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	m_bPriceChart = bPriceChart;
}

CDlgMainTabBaseEnvSetUp::~CDlgMainTabBaseEnvSetUp()   // standard destructor
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
}

IMPLEMENT_DYNAMIC( CDlgMainTabBaseEnvSetUp, CBaseTabDlg)


void CDlgMainTabBaseEnvSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainTabBaseEnvSetUp)
	DDX_Control(pDX, IDC_SPIN_IDC_EDIT_UPDOWN_SPACE, m_spnUpDownSpace);
	DDX_Control(pDX, IDC_EDIT_UPDOWN_SPACE, m_edtUpDownSpace);
	DDX_Control(pDX, IDC_SPIN_IDC_EDIT_RIGHT_MARGIN, m_spnRightMargin);
	DDX_Control(pDX, IDC_EDIT_RIGHT_MARGIN, m_edtRightMargin);
	DDX_Radio(pDX, IDC_CANDLETYPE_ONSIZE1, m_nCandleTypeOnResize);
	DDX_Check(pDX, IDC_SHOW_CURRENT_PRICE, m_bShowCurrentPrice);
	DDX_Check(pDX, IDC_Y_SCALE_POSITION_LEFTSIDE, m_bYScalePositionLeftSide);
	DDX_Check(pDX, IDC_Y_SCALE_POSITION_RIGHTSIDE, m_bYScalePositionRightSide);
	DDX_Check(pDX, IDC_INDICATOR_VALUE, m_bShowIndicatorValue);
	DDX_Check(pDX, IDC_INDICATOR_VARIABLE, m_bShowIndicatorVariable);
	DDX_Check(pDX, IDC_USE_CROSS_LINE, m_bUseCrossLine);
	DDX_Check(pDX, IDC_INQUIRYDATA_WINDOW_SHOW, m_bInquieryDataWindow);
	DDX_Check(pDX, IDC_HIDE_ALL_TITLE, m_bShowAllTitle);
	DDX_Check(pDX, IDC_SHOW_MIN_MAX_PRICE, m_bShowPriceMinMaxText);
	DDX_Check(pDX, IDC_USE_TOOLTIP_SHOW, m_bUseTooptipShow);
	DDX_Check(pDX, IDC_USE_TOOLTIP_SHOW_ALL, m_bUseTooptipShowAll);		//[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
	DDX_Check(pDX, IDC_FIX_INTERVAL, m_bFixInterval);
	DDX_Check(pDX, IDC_DISPLAY_BASELINE, m_bDrawBaseLine);
	DDX_CBIndex(pDX, IDC_COMBO_MOUSEWHEEL, m_nMouseWheelIndex);
	DDX_Radio(pDX, IDC_RDO_CURPRICE, m_nMinMaxRatio);
	DDX_Control(pDX, IDC_TICK_GUBUN,		m_chkHScaleGridType_Day);
	DDX_Control(pDX, IDC_DAY_WEEK_GUBUN,	m_chkHScaleGridType_Day_Week);
	DDX_Control(pDX, IDC_DAY_MON_GUBUN,		m_chkHScaleGridType_Day_Mon);
	DDX_Control(pDX, IDC_DAY_YEAR_GUBUN,	m_chkHScaleGridType_Day_Year);
	DDX_Control(pDX, IDC_WEEK_MON_GUBUN,	m_chkHScaleGridType_Week_Mon);
	DDX_Control(pDX, IDC_WEEK_YEAR_GUBUN,	m_chkHScaleGridType_Week_Year);
	DDX_Control(pDX, IDC_MON_YEAR_GUBUN,	m_chkHScaleGridType_Mon_Year);
	//}}AFX_DATA_MAP

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL))
		DDX_Radio(pDX, IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL, m_nInquiryDlgManualMove);
	if( GetDlgItem( IDC_EDT_FUTURE_TIME))
		DDX_Control(pDX, IDC_EDT_FUTURE_TIME, m_edtFutureTime);
	if( GetDlgItem( IDC_SPIN_FUTURE_TIME))
		DDX_Control(pDX, IDC_SPIN_FUTURE_TIME, m_spnFutureTime);
	if( GetDlgItem( IDC_SHOW_EXRIGHT_DIVIDEND))
		DDX_Check(pDX, IDC_SHOW_EXRIGHT_DIVIDEND, m_bShowExRightDividend);
	if( GetDlgItem( IDC_SHOW_HIGH_LOW_BOUND))
		DDX_Check(pDX, IDC_SHOW_HIGH_LOW_BOUND, m_bShowHighLowBound);
	if( GetDlgItem( IDC_USE_REVISED_PRICE))
		DDX_Check(pDX, IDC_USE_REVISED_PRICE, m_bUseRevisedPrice);
	//>>20100120_JS.Kim_alzioyes	상한가/하한가 정보표시
	if( GetDlgItem( IDC_SHOW_LOWHIGH_MARKER))
		DDX_Check(pDX, IDC_SHOW_LOWHIGH_MARKER, m_bShowLowHighMarker);
	//<<
	if( GetDlgItem( IDC_CANDLE_ONLY))
		DDX_Radio(pDX, IDC_CANDLE_ONLY, m_nCandleType);
	if( GetDlgItem( IDC_CHK_LH_LC_HC))
		DDX_Check(pDX, IDC_CHK_LH_LC_HC, m_bLH_LC_HC);
	if( GetDlgItem( IDC_USE_GAP_REVISION))
		DDX_Check(pDX, IDC_USE_GAP_REVISION, m_bUseGapRevision);
	if( GetDlgItem( IDC_RDO_OHLC))
		DDX_Radio(pDX, IDC_RDO_OHLC, m_nLH_LC_HC);

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	if( GetDlgItem(IDC_CUR_PRICE_TYPE_COMBO) )
		DDX_Control(pDX, IDC_CUR_PRICE_TYPE_COMBO, m_curPriceTypeCombo);
	if( GetDlgItem( IDC_CUR_PRICE_TYPE_PREDAY))
		DDX_Radio(pDX, IDC_CUR_PRICE_TYPE_PREDAY, m_nPriceBongType);
}


BEGIN_MESSAGE_MAP(CDlgMainTabBaseEnvSetUp, CDialog)
	//{{AFX_MSG_MAP(CDlgMainTabBaseEnvSetUp)
	ON_BN_CLICKED(IDC_INQUIRYDATA_WINDOW_SHOW, OnInquirydataWindowShow)
	ON_BN_CLICKED(IDC_SHOW_CURRENT_PRICE, OnShowCurrentPrice)
	ON_BN_CLICKED(IDC_HIDE_ALL_TITLE, OnUseIndicatorName)
	ON_BN_CLICKED(IDC_CHK_LH_LC_HC, OnChkLhLcHc)
	ON_BN_CLICKED(IDC_RDO_LH_LC_HC, OnRdoLhLcHc)
	ON_BN_CLICKED(IDC_SHOW_MIN_MAX_PRICE, OnShowMinMaxPrice)
	//}}AFX_MSG_MAP
	// (2009/9/3 - Seung-Won, Bae) Sync UI
	ON_BN_CLICKED(IDC_RDO_NONE, OnRdoLhLcHc)
	ON_BN_CLICKED(IDC_RDO_OHLC, OnRdoLhLcHc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabBaseEnvSetUp message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void	: 
// Comments		: 수치 조회창 표시여부 ( 0 : 없음, 1 : Window )
//-----------------------------------------------------------------------------
void CDlgMainTabBaseEnvSetUp::OnInquirydataWindowShow( void ) 
{
	// 1. "마우스 원클릭 시"에서 "수치 조회창 표시"를 선택했을 경우
	UpdateData(TRUE);

	BOOL bOnNiDlg = ( ( ( CButton *)GetDlgItem( IDC_INQUIRYDATA_WINDOW_SHOW))->GetCheck() == 1);

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL))
	{
		GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL)->EnableWindow( bOnNiDlg);
		GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL2)->EnableWindow( bOnNiDlg);
	}

	UpdateData(FALSE);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 1
// Modifier		: 
// Comments		: CDlgMainTabBaseEnvSetUp Dialog룰 처음 띄울때 필요한 값을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CDlgMainTabBaseEnvSetUp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_pIChartOCX == NULL )
		return FALSE;

	// 1. Y축 눈금표시 ( 0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음 ) ~ based on First Vertical Scale Group.
	// 1.1 왼쪽 Y축 보이기
	// 1.2 오른쪽 Y축 보이기
	m_nOrgYScalePosition = 0;
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( pIChartManager)
	{
		IBlock *pIBlock = pIChartManager->GetBlock( 0, 0);
		if( pIBlock)
		{
			m_nOrgYScalePosition = pIBlock->GetVScaleGroupYScalePosition( 0);
			pIBlock->Release();
		}
	}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	// (2010/7/22 - Seung-Won, Bae) Check base price.
	// 3.5 현재가 등락율 표시방법
	m_bEnablePreDay = FALSE;
	IPacketManager *pIPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	IPacket* pIPacketClose = pIPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( pIPacketClose)
	{
		double dData = 0.0;
		m_bEnablePreDay = pIPacketClose->GetStandardValue( dData);
		pIPacketClose->Release();
	}

	EnableCurPrice();
	OnShowCurrentPrice();

	switch( m_nOrgYScalePosition)
	{
		case 0 :
			m_bYScalePositionLeftSide = TRUE;
			m_bYScalePositionRightSide = FALSE;
			break;
		
		case 1 :
			m_bYScalePositionLeftSide = FALSE;
			m_bYScalePositionRightSide = TRUE;
			break;
		
		case 2 :
			m_bYScalePositionLeftSide = TRUE;
			m_bYScalePositionRightSide = TRUE;
			break;
			
		case 3 :
			m_bYScalePositionLeftSide = FALSE;
			m_bYScalePositionRightSide = FALSE;
			break;
	}

	// (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
	m_nLH_LC_HC = m_nOrgLH_LC_HC = GetShowLH_LC_HC();
	// LH/LC/HC - ojtaso (20071226)
	m_bLH_LC_HC = m_bOrgLH_LC_HC = ( 1 == m_nLH_LC_HC);

	// 1.3 가격눈금위의 현재가(등락율) 보기
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	m_nOrgShowCurrentPrice = GetShowCurrentPrice();
// 	EnableCurPrice();
	
	// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
	m_nRightMargin = m_pIChartOCX->GetRightMarginWidth();
	m_nOrgRightMargin = m_nRightMargin;
	Initial_Spin( m_spnRightMargin, m_edtRightMargin, m_nRightMargin);

	// (2009/5/17 - Seung-Won, Bae) for Future Time
	m_nFutureTime = m_pIChartOCX->GetFutureTimeCount();
	m_nOrgFutureTime = m_nFutureTime;
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( m_edtFutureTime.GetSafeHwnd() && m_spnFutureTime.GetSafeHwnd())
		Initial_Spin( m_spnFutureTime, m_edtFutureTime, m_nFutureTime);

	m_nMouseWheelIndex = m_pIChartOCX->GetMouseWheelIndex();
	m_nOrgMouseWheelIndex = m_nMouseWheelIndex;

	if( pIChartManager)
	{
		m_bFixInterval = pIChartManager->GetFixedInterval();
		m_nCandleTypeOnResize = pIChartManager->GetCandleTypeOnResize();
	}
	else
	{
		m_bFixInterval = FALSE;
		m_nCandleTypeOnResize = 0;
	}

	// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
	int nUpSpace = 0, nDownSpace = 0;
	int nR = 0, nC = 0, nV = 0, nG = 0;
	ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART ), nR, nC, nV, nG, NULL);
	if( !szGraphName.IsEmpty())
	{
		pIChartManager->GetVertScaleSpace( nR, nC, nV, nUpSpace, nDownSpace);

		BOOL bSpaceSpinEnable = TRUE;
		BOOL bIsLock = FALSE;
		CScaleBaseData::VERTMINMAXTYPE eMinMaxType = CScaleBaseData::VERT_MINMAX_OF_VIEW;
		double p_dMin = 0.0, p_dMax = 0.0;
		while( !szGraphName.IsEmpty() && bSpaceSpinEnable)
		{
			IBlock *pIBlock = pIChartManager->GetBlock( nR, nC);
			if( !pIBlock) break;

			if( pIBlock->GetVScaleMinMaxSetting( nV, bIsLock, eMinMaxType, p_dMin, p_dMax))
			{
				if( bIsLock) bSpaceSpinEnable = FALSE;
				if( CScaleBaseData::VERT_MINMAX_OF_USER == eMinMaxType) nUpSpace = nDownSpace = 0;
			}

			pIBlock->Release();

			nR++;
			szGraphName = pIChartManager->FindNextIndicator( _MTEXT( C2_PRICE_CHART ), nR, nC, nV, nG, NULL);
		}
		m_edtUpDownSpace.EnableWindow( bSpaceSpinEnable);
	}
	m_nUpDownSpace = nUpSpace + nDownSpace;
	m_nOrgUpDownSpace = m_nUpDownSpace;
	Initial_Spin( m_spnUpDownSpace, m_edtUpDownSpace, m_nUpDownSpace, 90);

	// 2.1 봉간격일정
	m_bOrgFixInterval = m_bFixInterval;

	// 3. 각종정보 표시
	// 3.1 배당락/권리락 표시
	EnableShowExRightDividend();
	m_bOrgShowExRightDividend = ( m_bShowExRightDividend ) ? true : false;

	// 3.2 상하한가 Bar 보이기
	EnableHighLowBound();
	m_bOrgShowHighLowBound = ( m_bShowHighLowBound ) ? true : false;

	// 3.3 수정주가 사용
	EnableRevisedPrice();
	m_bOrgUseRevisedPrice = m_bUseRevisedPrice;

	//>>20100120_JS.Kim_alzioyes	상한가/하한가 정보표시
	// 3.4 상한가/하한가 정보표시
	EnableLowHighMarker();
	m_bOrgShowLowHighMarker = (m_bShowLowHighMarker) ? true : false;
	//<<

	// 4. 지표이름 표시
	// 4.1 지표이름 표시
	m_bShowAllTitle = m_bOrgShowAllTitle = !m_pIChartOCX->IsHideAllTitle();
	EnableUseIndicatorName(m_bShowAllTitle);

	// 4.2 지표 정보 표시
	m_pIChartOCX->GetGraphTitleOption( m_bShowIndicatorVariable, m_bShowIndicatorValue);	// 지표정보 - 지표변수 및 최종값 표시
	m_bOrgShowIndicatorVariable = ( m_bShowIndicatorVariable ) ? true : false;
	m_bOrgShowIndicatorValue = ( m_bShowIndicatorValue ) ? true : false;

	// 5. 봉작도 기준 - 허수봉설정.
	m_nCandleType = m_nOrgCandleType = ( m_pIChartOCX->GetNormalizedMinuteTimeRule() ? 1 : 0);

	// 2008.10.15 by LYH >> 갭보정
	//>> 갭보정 - vntsorl(20090217)
	//	m_bUseGapRevision = m_bOrgUseGapRevision = m_pIChartOCX->GetUseGapRevision();
	m_bUseGapRevision = m_bOrgUseGapRevision = GetGapRevision();
	//<< 갭보정 - vntsorl(20090217)
	
	// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	GetDlgItem(IDC_USE_GAP_MONTHREV)->EnableWindow(GetShowMonthGapRevision());
	( ( CButton *)GetDlgItem( IDC_USE_GAP_MONTHREV))->SetCheck(GetMonthGapRevision());

	// 6. 편리한 기능
	// 6.1 최고/최저가 표시
	m_bShowPriceMinMaxText = m_bOrgShowPriceMinMaxText = m_pIChartOCX->IsShowPriceMinMaxText();

	m_nMinMaxRatio = m_nOrgMinMaxRatio = m_pIChartOCX->GetPriceMinMaxShowType();
	GetDlgItem(IDC_RDO_CURPRICE)->EnableWindow(m_bShowPriceMinMaxText);
	GetDlgItem(IDC_RDO_STANDPRICE)->EnableWindow(m_bShowPriceMinMaxText);

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_bDrawBaseLine = m_bOrgDrawBaseLine = (BOOL)m_pIChartOCX->GetDrawBaseLine();

	// 6.2. 수치조회창 표시여부 0:확대/축소, 1:Window
	m_lOrgNumericalInquiryOption = m_pIChartOCX->GetNumericalInquiryOption();
	m_bInquieryDataWindow = ( m_lOrgNumericalInquiryOption & NUMERICALINQUIRYDLG);
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL))
	{
		// ( 2006 / 11 / 07 - Sang-Woo, Cho ) - 수치조회 - 0으로 고정
		GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL)->EnableWindow( m_bInquieryDataWindow);
		GetDlgItem( IDC_INQUIRYDATA_WINDOW_MOVETYPE_MANUAL2)->EnableWindow( m_bInquieryDataWindow);
	}

	// 수치조회창 이동형태설정 0:자동동이동	, 1:수동이동
	//m_nInquiryDlgManualMove = ( m_lOrgNumericalInquiryOption & NIDLG_MANUALMOVE ? 1 : 0);
	m_nInquiryDlgManualMove = 0;

	// 6.3 십자선 사용 여부		0:사용안함, 1:사용
	m_bUseCrossLine = ( m_lOrgNumericalInquiryOption & CROSSLINEVIEW ? 1 : 0);

	// 6.4 캔들에서의 풍선도움말 사용여부		0:사용안함, 1:사용
	m_bUseTooptipShow = ( m_lOrgNumericalInquiryOption & TOOLTIPVIEW ? 1 : 0);
	m_bUseTooptipShowAll = ( m_lOrgNumericalInquiryOption & TOOLTIPVIEW_ALL ? 1 : 0);	//[A00000441]풍선도움말 보기시 전체 조회 옵션 추가

	// 7. 봉 고정기준
	m_nOrgCandleTypeOnResize = m_nCandleTypeOnResize;

	// 9. 미리보기를 눌렀을때의 값을 저장한다.
	m_nApplyCandleType = m_nOrgCandleType;
	m_bApplyUseGapRevision = m_bOrgUseGapRevision;						// 2008.10.15 by LYH >> 갭보정
	m_nApplyRightMargin = m_nOrgRightMargin;
	m_nApplyFutureTime = m_nOrgFutureTime;			// (2009/5/17 - Seung-Won, Bae) for Future Time
	m_nApplyMouseWheelIndex = m_nOrgMouseWheelIndex;
	m_bApplyFixInterval = m_bOrgFixInterval;
	m_nApplyCandleTypeOnResize = m_nOrgCandleTypeOnResize;
	m_bApplyShowIndicatorVariable = m_bOrgShowIndicatorVariable;		// 지표정보 - 지표변수 표시
	m_bApplyShowIndicatorValue = m_bOrgShowIndicatorValue;				// 지표정보 - 지표값 표시
	m_lApplyNumericalInquiryOption = m_lOrgNumericalInquiryOption;
	m_bApplyShowHighLowBound = m_bOrgShowHighLowBound;
	m_bApplyUseRevisedPrice = m_bOrgUseRevisedPrice;					// 수정주가 사용여부
	m_bApplyShowExRightDividend = m_bOrgShowExRightDividend;
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	m_nApplyShowCurrentPrice = m_nOrgShowCurrentPrice;
	m_nApplyYScalePosition = m_nOrgYScalePosition;						// Y축 눈금표시 ( 0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음 )
	m_bApplyDrawBaseLine = m_bDrawBaseLine;								// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_nApplyUpDownSpace = m_nUpDownSpace;								// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
	m_nApplyMouseWheelIndex = m_nOrgMouseWheelIndex;

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	SetDlgItemEnable(IDC_SHOW_CURRENT_PRICE, m_bPriceChart);
	SetDlgItemEnable(IDC_CUR_PRICE_TYPE_COMBO, m_bPriceChart);
	SetDlgItemEnable(IDC_CUR_PRICE_TYPE_PREDAY, m_bPriceChart);
	SetDlgItemEnable(IDC_CUR_PRICE_TYPE_PREBONG, m_bPriceChart);
	SetDlgItemEnable(IDC_SHOW_LOWHIGH_MARKER, m_bPriceChart);

	m_bApplyShowAllTitle = m_bOrgShowAllTitle;
	m_bApplyShowPriceMinMaxText = m_bOrgShowPriceMinMaxText;
	m_nApplyMinMaxRatio = m_nOrgMinMaxRatio;
	

	BOOL bCheck = FALSE;
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::MINTICK_DAY_GUBUN );
	m_chkHScaleGridType_Day.SetCheck(bCheck);
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::DAYCHART_WEEK_GUBUN );
	m_chkHScaleGridType_Day_Week.SetCheck(bCheck);
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::DAYCHART_MON_GUBUN );
	m_chkHScaleGridType_Day_Mon.SetCheck(bCheck);
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::DAYCHART_YEAR_GUBUN );
	m_chkHScaleGridType_Day_Year.SetCheck(bCheck);
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::WEEKCHART_MON_GUBUN );
	m_chkHScaleGridType_Week_Mon.SetCheck(bCheck);
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::WEEKCHART_YEAR_GUBUN );
	m_chkHScaleGridType_Week_Year.SetCheck(bCheck);
	bCheck=m_pIChartOCX->GetFirstBlockHScaleType_TimeGubun( CScaleBaseData::MONCHART_YEAR_GUBUN );
	m_chkHScaleGridType_Mon_Year.SetCheck(bCheck);
	//END

	UpdateData( FALSE );
	Invalidate();

	if( pIChartManager) pIChartManager->Release();
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	if( pIPacketManager ) pIPacketManager->Release();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 17
// Modifier		: 
// Comments		: IDC_USE_REVISED_PRICE 는 Default가 Disable이다.
//-----------------------------------------------------------------------------
void CDlgMainTabBaseEnvSetUp::EnableRevisedPrice()
{
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( !GetDlgItem( IDC_USE_REVISED_PRICE)) return;

	// 동부증권은 사용하지 않고 타 증권사는 현재 Disable시킴.
	int nVersion = GetAddInIVersion();
	if( 419999 < nVersion && nVersion < 430000  )
	{
		GetDlgItem(IDC_USE_REVISED_PRICE)->ShowWindow(SW_HIDE);
		return;
	}
	
	if( m_MChartInterface.InitMChartInterface061115() )
	{
		GetDlgItem(IDC_USE_REVISED_PRICE)->EnableWindow( TRUE );
		m_bUseRevisedPrice = m_MChartInterface.GetRevisedPriceFlag();
	}
	// (2007/10/5 - Seung-Won, Bae) Support Map
	else m_pIChartOCX->FireAddInEvent( CChartEnvSetupDlgAddInImp::m_strAddInItemName, 1005, NULL, 0, 0);
}
// (2007/10/5 - Seung-Won, Bae) for Revised Price of Map
void CDlgMainTabBaseEnvSetUp::EnableRevisedPrice( BOOL p_bCheck)
{
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( !GetDlgItem( IDC_USE_REVISED_PRICE)) return;

	GetDlgItem(IDC_USE_REVISED_PRICE)->EnableWindow( TRUE );
	m_bUseRevisedPrice = p_bCheck;
}


//sy 2005.11.25.
void CDlgMainTabBaseEnvSetUp::EnableShowExRightDividend()
{ 
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( !GetDlgItem( IDC_SHOW_EXRIGHT_DIVIDEND)) return;

	m_bShowExRightDividend = FALSE;
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( pIChartManager)
	{
		int nR = -1, nC = -1, nV = -1, nG = -1;
		ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_LOCK), nR, nC, nV, nG, NULL);
		m_bShowExRightDividend = !szGraphName.IsEmpty();
		pIChartManager->Release();
	}

	IPacketManager *pIPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	if( pIPacketManager)
	{
		// <일> 경우만 <락> 표현
		GetDlgItem( IDC_SHOW_EXRIGHT_DIVIDEND)->EnableWindow( pIPacketManager->GetBaseTimeDataType() == CScaleBaseData::HORZ_DAILY);
		pIPacketManager->Release();
	}
}


void CDlgMainTabBaseEnvSetUp::EnableUseIndicatorName( BOOL m_bShowAllTitle )
{
	GetDlgItem(IDC_INDICATOR_VARIABLE)->EnableWindow(m_bShowAllTitle);
	GetDlgItem(IDC_INDICATOR_VALUE)->EnableWindow(m_bShowAllTitle);
}


void CDlgMainTabBaseEnvSetUp::EnableHighLowBound()
{
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( !GetDlgItem( IDC_SHOW_HIGH_LOW_BOUND)) return;

	if( GetExistenceHighLowBound())
	{
		GetDlgItem( IDC_SHOW_HIGH_LOW_BOUND)->EnableWindow( TRUE );

		long lResult = 0;
		IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
		if( pIAddInToolMgr)
		{
			pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_BOUNDMARK_FLAG);
			pIAddInToolMgr->Release();
		}
		m_bShowHighLowBound = lResult;		// 상 / 하한가 표시
	}
	else
	{
		GetDlgItem( IDC_SHOW_HIGH_LOW_BOUND)->EnableWindow( FALSE );
	}
}

//>>20120227 : KHD	상한가/하한가 정보표시
void CDlgMainTabBaseEnvSetUp::EnableLowHighMarker()
{
	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( !GetDlgItem( IDC_SHOW_LOWHIGH_MARKER)) return;
	
	if( GetExistenceLowHighMarker())
	{
		GetDlgItem( IDC_SHOW_LOWHIGH_MARKER)->EnableWindow( TRUE );
		
		long lResult = 0;
		IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
		if( pIAddInToolMgr)
		{
			pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_LOWHIGHMARK_FLAG);
			pIAddInToolMgr->Release();
		}
		m_bShowLowHighMarker = lResult;		// 상 / 하한가 표시
	}
	else
	{
		GetDlgItem( IDC_SHOW_LOWHIGH_MARKER)->EnableWindow( FALSE );
	}
}
//>>

void CDlgMainTabBaseEnvSetUp::EnableCurPrice()
{
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	// 디자인설정과 동일게 맞춤 - ojtaso (20071112)
// 	if(!m_bOrgShowCurrentPrice)
// 	{
// 		m_bShowCurrentPrice = FALSE;	// 가격눈금위에 현재가표시
// 		GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_1)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_2)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_3)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_4)->EnableWindow( FALSE );
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_1)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_2)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_3)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_4)->EnableWindow( TRUE );
// 		m_bShowCurrentPrice = TRUE;
// 		m_nCurPriceSub = m_bOrgShowCurrentPrice - 1;
// 	}

	if( GetDlgItem(IDC_CUR_PRICE_TYPE_COMBO) )
	{

		UpdateData();

		while( m_curPriceTypeCombo.GetCount() )
			m_curPriceTypeCombo.DeleteString(0);

		CString tmp;
		tmp.LoadString(IDS_LOWHIGH_MARK_STR_1);
		m_curPriceTypeCombo.AddString(tmp);
		tmp.LoadString(IDS_LOWHIGH_MARK_STR_5);		// (2011/2/9 - Seung-Won, Bae) for the difference only.
		m_curPriceTypeCombo.AddString(tmp);
		tmp.LoadString(IDS_LOWHIGH_MARK_STR_2);
		m_curPriceTypeCombo.AddString(tmp);
		tmp.LoadString(IDS_LOWHIGH_MARK_STR_3);
		m_curPriceTypeCombo.AddString(tmp);

		if( m_pIChartOCX->GetChartMode() == CChartInfo::CHART_MODE::DEVELOPER ||
			m_pIChartOCX->GetChartMode() == CChartInfo::CHART_MODE::FX ){
			// Fx 차트라면 추가
			tmp.LoadString(IDS_LOWHIGH_MARK_STR_4);
			m_curPriceTypeCombo.AddString(tmp);
		}

		if( m_nCurPriceSub < 0 ){
			m_nCurPriceSub = GetShowCurrentPrice()-1;
		}
		m_nOrgShowCurrentPrice = m_nCurPriceSub + 1;

		int cur = m_nCurPriceSub;
		switch(cur){
		case 0:
		case 4:
		case 2:
		case 6:
		case 8:
			if( cur == 4 ) cur = 0;
			else if( cur == 8 ) cur = 1;	// (2011/2/9 - Seung-Won, Bae) for the difference only.
			else if( cur == 0 ) cur = 2;
			else if( cur == 2 ) cur = 3;
			else if( cur == 6 ) cur = 4;
			m_nPriceBongType=0;
			m_bShowCurrentPrice = TRUE;
			break;
		case 1:
		case 5:
		case 3:
		case 7:
		case 9:
			if( cur == 5 ) cur = 0;
			else if( cur == 9 ) cur = 1;	// (2011/2/9 - Seung-Won, Bae) for the difference only.
			else if( cur == 1 ) cur = 2;
			else if( cur == 3 ) cur = 3;
			else if( cur == 7 ) cur = 4;
			m_nPriceBongType=1;
			m_bShowCurrentPrice = TRUE;
			break;
		default:
			cur = 0;
			m_nPriceBongType=0;
			m_bShowCurrentPrice = FALSE;
			break;
		}

		m_curPriceTypeCombo.SetCurSel(cur);

		UpdateData(FALSE);
	}
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabBaseEnvSetUp::Save() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = FALSE;
	
	UpdateData(TRUE);
	
	// 1. Y축 눈금표시 ( 0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음 )
	int nYScalePosition;
	if( m_bYScalePositionLeftSide && m_bYScalePositionRightSide == FALSE )
		nYScalePosition = 0;

	else if( m_bYScalePositionLeftSide == FALSE && m_bYScalePositionRightSide)
		nYScalePosition = 1;

	else if( m_bYScalePositionLeftSide && m_bYScalePositionRightSide)
		nYScalePosition = 2;

	else if( m_bYScalePositionLeftSide == FALSE && m_bYScalePositionRightSide == FALSE )
		nYScalePosition = 3;

	if( m_nApplyYScalePosition != nYScalePosition )
	{
		SetYScalePosition( nYScalePosition );
		m_nApplyYScalePosition = nYScalePosition;
		m_nOrgYScalePosition = nYScalePosition;
	}

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_RDO_OHLC))
	{
		// (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
		int nLH_LC_HC = m_nLH_LC_HC;
		if( m_nApplyLH_LC_HC != nLH_LC_HC || m_nOrgLH_LC_HC != nLH_LC_HC)
		{
			SetShowLH_LC_HC( nLH_LC_HC );
			m_nApplyLH_LC_HC = nLH_LC_HC;
			m_nOrgLH_LC_HC = nLH_LC_HC;
		}
	}
	else
	{
		BOOL bLH_LC_HC = m_bLH_LC_HC;
		if( m_bApplyLH_LC_HC != bLH_LC_HC || m_bOrgLH_LC_HC != bLH_LC_HC)
		{
			SetShowLH_LC_HC( bLH_LC_HC ? 1 : 0);
			m_bApplyLH_LC_HC = bLH_LC_HC;
			m_bOrgLH_LC_HC = bLH_LC_HC;
		}
	}

	int nShowCurrentPrice = GetCurrentPrice();
	if(IsApplyShowCurrentPrice(nShowCurrentPrice))
	{
		SetShowCurrentPrice( nShowCurrentPrice );
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
		m_nApplyShowCurrentPrice = nShowCurrentPrice;
		m_nOrgShowCurrentPrice = nShowCurrentPrice;
	}

	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();

	// 2. 오른쪽 여백
	CString szRightMargin = "";
	m_edtRightMargin.GetWindowText(szRightMargin);
	int nRightMargin = atoi( szRightMargin );
	if( pIChartManager && m_nApplyRightMargin != nRightMargin )
	{
		// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
		m_pIChartOCX->SetRightMarginWidth( nRightMargin);
		m_nApplyRightMargin = nRightMargin;
		m_nOrgRightMargin = nRightMargin;
	}

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( m_edtFutureTime.GetSafeHwnd() && m_spnFutureTime.GetSafeHwnd())
	{
		// (2009/5/17 - Seung-Won, Bae) for Future Time
		CString szFutureTime = "";
		m_edtFutureTime.GetWindowText(szFutureTime);
		int nFutureTime = atoi( szFutureTime );
		if( pIChartManager && m_nApplyFutureTime != nFutureTime )
		{
			// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
			m_pIChartOCX->SetFutureTimeCount( nFutureTime);
			m_nApplyFutureTime = nFutureTime;
			m_nOrgFutureTime = nFutureTime;
		}
	}

	m_pIChartOCX->SetMouseWheelIndex( m_nMouseWheelIndex);
	m_nApplyMouseWheelIndex = m_nMouseWheelIndex;
	m_nOrgMouseWheelIndex = m_nMouseWheelIndex;

	// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
	CString szUpDownSpace = "";
	m_edtUpDownSpace.GetWindowText(szUpDownSpace);
	int nUpDownSpace = atoi( szUpDownSpace );
	if( m_nApplyUpDownSpace != nUpDownSpace )
	{
		SetUpDownSpace( nUpDownSpace );
		m_nApplyUpDownSpace = nUpDownSpace;
		m_nOrgUpDownSpace = nUpDownSpace;
	}

	// 2.1 봉간격일정
	if( pIChartManager && m_bApplyFixInterval != m_bFixInterval )
	{
		pIChartManager->SetFixedInterval( m_bFixInterval, TRUE );
		m_bApplyFixInterval = m_bFixInterval;
		m_bOrgFixInterval = m_bFixInterval;
	}

	// 3. 각종지표 표시
	if( m_bApplyShowExRightDividend != ( m_bShowExRightDividend ? true : false ) )
	{
		SetShowExRightDividend( ( m_bShowExRightDividend ? true : false ) );
		m_bApplyShowExRightDividend = ( m_bShowExRightDividend ? true : false );
		m_bOrgShowExRightDividend = m_bApplyShowExRightDividend;
	}

	if( GetExistenceHighLowBound())
	{
		if( m_bApplyShowHighLowBound != ( m_bShowHighLowBound ? true : false ) )
		{
			SetShowHighLowBound( ( m_bShowHighLowBound ? true : false ) );
			m_bApplyShowHighLowBound = ( m_bShowHighLowBound ? true : false );
			m_bOrgShowHighLowBound = m_bApplyShowHighLowBound;
		}
	}

	//>>20100120_JS.Kim_alzioyes	상한가/하한가 정보표시
	if( GetExistenceLowHighMarker())
	{
		if( m_bApplyShowLowHighMarker != ( m_bShowLowHighMarker ? true : false ) )
		{
			SetShowLowHighMarker( ( m_bShowLowHighMarker ? true : false ) );
			m_bApplyShowLowHighMarker = ( m_bShowLowHighMarker ? true : false );
			m_bOrgShowLowHighMarker = m_bApplyShowLowHighMarker;
		}
	}
	//<<

	if( m_MChartInterface.InitMChartInterface061115() ) m_MChartInterface.SetRevisedPriceFlag( m_bUseRevisedPrice );
	// (2007/10/5 - Seung-Won, Bae) Support Map
	else m_pIChartOCX->FireAddInEvent( CChartEnvSetupDlgAddInImp::m_strAddInItemName, 1006, NULL, 0, ( m_bUseRevisedPrice ? 1 : 0));
	m_bApplyUseRevisedPrice = m_bOrgUseRevisedPrice = m_bUseRevisedPrice;


	// 4. 지표 정보 표시
	m_pIChartOCX->SetHideAllTitle( !m_bShowAllTitle );
	m_bOrgShowAllTitle = m_bApplyShowAllTitle = m_bShowAllTitle;

	if( m_bApplyShowIndicatorVariable != ( m_bShowIndicatorVariable ? true : false ) )					// 지표정보 - 지표변수 표시
	{
		SetShowIndicatorVariable( ( m_bShowIndicatorVariable ? true : false ) );
		m_bApplyShowIndicatorVariable = ( m_bShowIndicatorVariable ? true : false );
		m_bOrgShowIndicatorVariable = m_bApplyShowIndicatorVariable;
	}
	
	if( m_bApplyShowIndicatorValue != ( m_bShowIndicatorValue ? true : false ) )						// 지표정보 - 지표값 표시
	{
		SetShowIndicatorValue( ( m_bShowIndicatorValue ? true : false ) );
		m_bApplyShowIndicatorValue = ( m_bShowIndicatorValue ? true : false );
		m_bOrgShowIndicatorValue = m_bApplyShowIndicatorValue;
	}

	BOOL bUseMonthGapRevision = ( ( ( CButton *)GetDlgItem( IDC_USE_GAP_MONTHREV))->GetCheck() == 1);

	// 5. 봉작도 기준
	// (2008/11/2 - Seung-Won, Bae) Notify to MChart with CandleType and GapRevision.
	BOOL bIsChanged = ( m_nApplyCandleType != m_nCandleType || m_bApplyUseGapRevision != m_bUseGapRevision || m_bApplyUseMonthGapRevision != bUseMonthGapRevision);

	BOOL bChangeAllGraph = FALSE;
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if (pIAddInToolMgr)
	{
		// 2008.10.15 by LYH >> 갭보정
		if( m_bApplyUseGapRevision != m_bUseGapRevision)
		{
			//<< 갭보정 설정변경 - vntsorl(20090217)
			pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_GAPREVISION_CHANGED, m_bUseGapRevision, m_nApplyCandleType != m_nCandleType);

			m_bApplyUseGapRevision = m_bUseGapRevision;
			m_bOrgUseGapRevision = m_bApplyUseGapRevision;

			bChangeAllGraph = TRUE;
		}

		//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		if( m_bApplyUseMonthGapRevision != bUseMonthGapRevision)
		{
			pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_MONTHGAPREVISION_CHANGED, bUseMonthGapRevision);
			m_bApplyUseMonthGapRevision = bUseMonthGapRevision;
		}
		//>> (Issue No : 0006572)

		pIAddInToolMgr->Release();
	}
	// 2008.10.15 by LYH <<
	if( m_nApplyCandleType != m_nCandleType)
	{
		m_pIChartOCX->SetNormalizedMinuteTimeRule( m_nCandleType);
		m_nApplyCandleType = m_nCandleType;
		m_nOrgCandleType = m_nApplyCandleType;
	}
	// (2008/11/2 - Seung-Won, Bae) Notify to MChart with CandleType and GapRevision.
	if( bIsChanged)
		m_MChartInterface.MChart_SetImaginaryCandleAndGapRevisionToOutside( m_nApplyCandleType ? true : false, m_bApplyUseGapRevision ? true : false);

	// 6. 편리한 기능
	// 6.1 최고/최저가 표시
	m_pIChartOCX->ShowPriceMinMaxText(m_bShowPriceMinMaxText);
	m_bOrgShowPriceMinMaxText = m_bApplyShowPriceMinMaxText = m_bShowPriceMinMaxText;

	m_pIChartOCX->SetPriceMinMaxShowType(m_nMinMaxRatio);
	m_nOrgMinMaxRatio = m_nApplyMinMaxRatio = m_nMinMaxRatio;

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_pIChartOCX->SetDrawBaseLine(m_bDrawBaseLine);
	m_bOrgDrawBaseLine = m_bApplyDrawBaseLine = m_bDrawBaseLine;

	// 6.2 수치조회창
	//long lNumericalInquiryOption = ( m_pIChartOCX->GetNumericalInquiryOption() & TOOLTIPVIEW);
	long lNumericalInquiryOption = ( m_pIChartOCX->GetNumericalInquiryOption());
	if( m_bInquieryDataWindow)			lNumericalInquiryOption |= NUMERICALINQUIRYDLG;
	else								lNumericalInquiryOption &= ~NUMERICALINQUIRYDLG;

	if( m_nInquiryDlgManualMove == 1)	lNumericalInquiryOption |= NIDLG_MANUALMOVE;
	else								lNumericalInquiryOption &= ~NIDLG_MANUALMOVE;

	if( m_bUseCrossLine)				lNumericalInquiryOption |= CROSSLINEVIEW;
	else								lNumericalInquiryOption &= ~CROSSLINEVIEW;

	if(m_bUseTooptipShow)				lNumericalInquiryOption |= TOOLTIPVIEW;
	else								lNumericalInquiryOption  &= ~TOOLTIPVIEW;

	//>>[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
	if(m_bUseTooptipShowAll)			lNumericalInquiryOption |= TOOLTIPVIEW_ALL;
	else								lNumericalInquiryOption  &= ~TOOLTIPVIEW_ALL;
	//<<

	if( m_lApplyNumericalInquiryOption != lNumericalInquiryOption)
	{
		m_pIChartOCX->SetNumericalInquiryOption( lNumericalInquiryOption);
		m_lApplyNumericalInquiryOption = lNumericalInquiryOption;
		m_lOrgNumericalInquiryOption = lNumericalInquiryOption;
	}

	// 7. 봉 고정기준
	if( pIChartManager && m_nApplyCandleTypeOnResize != m_nCandleTypeOnResize )
	{
		pIChartManager->SetCandleTypeOnResize(m_nCandleTypeOnResize, TRUE);
		//m_MChartInterface.MChart_SetImaginaryCandleToOutside( m_nCandleType ? true : false);
		m_nApplyCandleTypeOnResize = m_nCandleTypeOnResize;
		m_nOrgCandleTypeOnResize = m_nApplyCandleTypeOnResize;
		//>> (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거
// 		pIChartManager->Release();
	}

	//KHD : 시간구분
	SetTimeGubunGrid();

	// Update to Graph -> Because GraphDraw data manage in Graph Object for Final Version.
	if (bChangeAllGraph)
		pIChartManager->ChangeAllGraphDataFromGraphInfo( TRUE);

	if( pIChartManager) pIChartManager->Release();
	//<< (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거

	m_pIChartOCX->InvalidateControl();
}

//sy 2005.10.12.
bool CDlgMainTabBaseEnvSetUp::IsApplyShowCurrentPrice(const int nShowCurrentPrice) const
{
	// 지표설정에서 값을 바꾸는 현상이 있기 때문에 이렇게 처리한다.
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	if(m_nApplyShowCurrentPrice != nShowCurrentPrice)
		return true;

	return (m_nOrgShowCurrentPrice != nShowCurrentPrice);
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void	: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabBaseEnvSetUp::Recover() 
{
	// TODO: Add extra cleanup here
	m_bPreview = FALSE;
	
	UpdateData(TRUE);
	
	// 1. Y축 눈금표시 ( 0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음 )
	int nYScalePosition;
	if( m_bYScalePositionLeftSide && m_bYScalePositionRightSide == FALSE )
		nYScalePosition = 0;

	else if( m_bYScalePositionLeftSide == FALSE && m_bYScalePositionRightSide)
		nYScalePosition = 1;

	else if( m_bYScalePositionLeftSide && m_bYScalePositionRightSide)
		nYScalePosition = 2;

	else if( m_bYScalePositionLeftSide == FALSE && m_bYScalePositionRightSide == FALSE )
		nYScalePosition = 3;

	// Y축 눈금표시 ( 0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음 )
	if( m_nOrgYScalePosition != nYScalePosition )
	{
		m_nApplyYScalePosition = m_nOrgYScalePosition;
		SetYScalePosition( m_nOrgYScalePosition );
		switch( m_nOrgYScalePosition )
		{
			case 0 :
				m_bYScalePositionLeftSide = TRUE;
				m_bYScalePositionRightSide = FALSE;
				break;
			
			case 1 :
				m_bYScalePositionLeftSide = FALSE;
				m_bYScalePositionRightSide = TRUE;
				break;
			
			case 2 :
				m_bYScalePositionLeftSide = TRUE;
				m_bYScalePositionRightSide = TRUE;
				break;
				
			case 3 :
				m_bYScalePositionLeftSide = FALSE;
				m_bYScalePositionRightSide = FALSE;
				break;
		}
	}

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_RDO_OHLC))
	{
		// (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
		if( m_nOrgLH_LC_HC != m_nLH_LC_HC )
		{
			SetShowLH_LC_HC( m_nOrgLH_LC_HC);
			m_nApplyLH_LC_HC = m_nLH_LC_HC = m_nOrgLH_LC_HC;
		}
	}
	else
	{
		// LH/LC/HC - ojtaso (20071226)
		if( m_bOrgLH_LC_HC != m_bLH_LC_HC )
		{
			SetShowLH_LC_HC( m_bOrgLH_LC_HC ? 1 : 0);
			m_bApplyLH_LC_HC = m_bLH_LC_HC = m_bOrgLH_LC_HC;
		}
	}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	if( ( m_bOrgShowCurrentPrice ? TRUE : FALSE ) != m_bShowCurrentPrice )
// 	{
// 		SetShowCurrentPrice( m_bOrgShowCurrentPrice ? 1: 0);
// 		m_bShowCurrentPrice = ( m_bOrgShowCurrentPrice ? TRUE : FALSE );
// 		m_bApplyShowCurrentPrice = m_bOrgShowCurrentPrice;
// 	}
// 
// 	//sy 2005.10.12
// 	int nShowCurrentPrice = GetCurrentPrice();
// 	if( m_bOrgShowCurrentPrice != nShowCurrentPrice )
// 	{
// 		SetShowCurrentPrice( m_bOrgShowCurrentPrice );
// 	
// 		// 디자인설정과 동일게 맞춤 - ojtaso (20071112)
// 		if(!m_bOrgShowCurrentPrice)
// 		{
// 			m_bShowCurrentPrice = FALSE;	// 가격눈금위에 현재가표시
// 			GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( FALSE );
// 			GetDlgItem(IDC_CUR_PRICE_1)->EnableWindow( FALSE );
// 			GetDlgItem(IDC_CUR_PRICE_2)->EnableWindow( FALSE );
// 			GetDlgItem(IDC_CUR_PRICE_3)->EnableWindow( FALSE );
// 			GetDlgItem(IDC_CUR_PRICE_4)->EnableWindow( FALSE );
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( TRUE );
// 			GetDlgItem(IDC_CUR_PRICE_1)->EnableWindow( TRUE );
// 			GetDlgItem(IDC_CUR_PRICE_2)->EnableWindow( TRUE );
// 			GetDlgItem(IDC_CUR_PRICE_3)->EnableWindow( TRUE );
// 			GetDlgItem(IDC_CUR_PRICE_4)->EnableWindow( TRUE );
// 			m_bShowCurrentPrice = TRUE;
// 			m_nCurPriceSub = m_bOrgShowCurrentPrice - 1;
// 		}
// 
// 		m_bApplyShowCurrentPrice = m_bOrgShowCurrentPrice;
// 	}
	
	if( 0 < m_nOrgShowCurrentPrice && !m_bShowCurrentPrice
		|| 0 == m_nOrgShowCurrentPrice && m_bShowCurrentPrice)
	{
		SetShowCurrentPrice( m_nOrgShowCurrentPrice);
		m_bShowCurrentPrice = ( 0 < m_nOrgShowCurrentPrice ? TRUE : FALSE );
		m_nApplyShowCurrentPrice = m_nOrgShowCurrentPrice;
	}

	//sy 2005.10.12
	int nShowCurrentPrice = GetCurrentPrice();
	if( m_nOrgShowCurrentPrice != nShowCurrentPrice )
	{
		SetShowCurrentPrice( m_nOrgShowCurrentPrice);
		m_nApplyShowCurrentPrice = m_nOrgShowCurrentPrice;
	}

	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();

	// 2. 오른쪽 여백
	CString szRightMargin = "";
	m_edtRightMargin.GetWindowText( szRightMargin );
	int nRightMargin = atoi( szRightMargin );
	if( pIChartManager && m_nOrgRightMargin != nRightMargin )
	{
		// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
		m_pIChartOCX->SetRightMarginWidth( m_nOrgRightMargin);
		m_nRightMargin = m_nOrgRightMargin;
		m_nApplyRightMargin = m_nOrgRightMargin;
		Initial_Spin( m_spnRightMargin, m_edtRightMargin, m_nOrgRightMargin );
	}

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( m_edtFutureTime.GetSafeHwnd() && m_spnFutureTime.GetSafeHwnd())
	{
		// (2009/5/17 - Seung-Won, Bae) for Future Time
		CString szFutureTime = "";
		m_edtFutureTime.GetWindowText( szFutureTime );
		int nFutureTime = atoi( szFutureTime );
		if( pIChartManager && m_nOrgFutureTime != nFutureTime )
		{
			// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
			m_pIChartOCX->SetFutureTimeCount( m_nOrgFutureTime);
			m_nFutureTime = m_nOrgFutureTime;
			m_nApplyFutureTime = m_nOrgFutureTime;
			Initial_Spin( m_spnFutureTime, m_edtFutureTime, m_nOrgFutureTime);
		}
	}
	
	m_nMouseWheelIndex = m_nOrgMouseWheelIndex;
	m_nApplyMouseWheelIndex = m_nOrgMouseWheelIndex;

	// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
	CString szUpDownSpace = "";
	m_edtUpDownSpace.GetWindowText( szUpDownSpace );
	int nUpDownSpace = atoi( szUpDownSpace );

	if( m_nOrgUpDownSpace != nUpDownSpace )
	{
		SetUpDownSpace( m_nOrgUpDownSpace );
		m_nUpDownSpace = m_nOrgUpDownSpace;
		m_nApplyUpDownSpace = m_nOrgUpDownSpace;
		Initial_Spin( m_spnUpDownSpace, m_edtUpDownSpace, m_nOrgUpDownSpace, 90);
	}
	
	// 2.1 봉간격일정
	if( pIChartManager && m_bOrgFixInterval != m_bFixInterval )
	{
		pIChartManager->SetFixedInterval( m_bOrgFixInterval, TRUE );
		m_bFixInterval = m_bOrgFixInterval;
		m_bApplyFixInterval = m_bOrgFixInterval;
	}

	// 3. 각종 정보 표시
	if( GetExistenceHighLowBound())
	{
		if( ( m_bOrgShowHighLowBound ? TRUE : FALSE ) != m_bShowHighLowBound )
		{
			SetShowHighLowBound( m_bOrgShowHighLowBound );
			m_bShowHighLowBound = ( m_bOrgShowHighLowBound ? TRUE : FALSE );
			m_bApplyShowHighLowBound = m_bOrgShowHighLowBound;
		}
	}
	
	if( ( m_bOrgShowExRightDividend ? TRUE : FALSE ) != m_bShowExRightDividend )
	{
		SetShowExRightDividend( m_bOrgShowExRightDividend );
		m_bShowExRightDividend = ( m_bOrgShowExRightDividend ? TRUE : FALSE );
		m_bApplyShowExRightDividend = m_bOrgShowExRightDividend;
	}

	if( m_bOrgUseRevisedPrice != m_bUseRevisedPrice)
	{
		if( m_MChartInterface.InitMChartInterface061115()) m_MChartInterface.SetRevisedPriceFlag( m_bOrgUseRevisedPrice);
		// (2007/10/5 - Seung-Won, Bae) Support Map
		else m_pIChartOCX->FireAddInEvent( CChartEnvSetupDlgAddInImp::m_strAddInItemName, 1006, NULL, 0, ( m_bOrgUseRevisedPrice ? 1 : 0));
		m_bApplyUseRevisedPrice = m_bUseRevisedPrice = m_bOrgUseRevisedPrice;
	}

	//>>20100120_JS.Kim_alzioyes	상한가/하한가 정보표시
	if( GetExistenceLowHighMarker())
	{
		if( ( m_bOrgShowLowHighMarker ? TRUE : FALSE ) != m_bShowLowHighMarker )
		{
			SetShowLowHighMarker( m_bOrgShowLowHighMarker );
			m_bShowLowHighMarker = ( m_bOrgShowLowHighMarker ? TRUE : FALSE );
			m_bApplyShowLowHighMarker = m_bOrgShowLowHighMarker;
		}
	}
	//<<

	// 4. 지표 정보 표시
	if( m_bOrgShowAllTitle != m_bShowAllTitle )
	{
		if( ( m_bOrgShowIndicatorVariable ? TRUE : FALSE ) != m_bShowIndicatorVariable )					// 지표정보 - 지표변수 표시
		{
			SetShowIndicatorVariable( m_bOrgShowIndicatorVariable );
			m_bShowIndicatorVariable = ( m_bOrgShowIndicatorVariable ? TRUE : FALSE );
			m_bApplyShowIndicatorVariable = m_bOrgShowIndicatorVariable;
		}
		
		if( ( m_bOrgShowIndicatorValue ? TRUE : FALSE ) != m_bShowIndicatorValue )						// 지표정보 - 지표값 표시
		{
			SetShowIndicatorValue( m_bOrgShowIndicatorValue );
			m_bShowIndicatorValue = ( m_bOrgShowIndicatorValue ? TRUE : FALSE );
			m_bApplyShowIndicatorValue = m_bOrgShowIndicatorValue;
		}

		EnableUseIndicatorName( m_bOrgShowAllTitle );
		m_pIChartOCX->SetHideAllTitle( m_bOrgShowAllTitle );
		m_bApplyShowAllTitle = m_bShowAllTitle = m_bOrgShowAllTitle;
	}
	
	BOOL bUseMonthGapRevision = ( ( ( CButton *)GetDlgItem( IDC_USE_GAP_MONTHREV))->GetCheck() == 1);
	// (2008/11/2 - Seung-Won, Bae) Notify to MChart with CandleType and GapRevision.
	BOOL bIsChanged = ( m_nOrgCandleType != m_nCandleType || m_bOrgUseGapRevision != m_bUseGapRevision || m_bOrgUseMonthGapRevision != bUseMonthGapRevision);

	BOOL bChangeAllGraph = FALSE;
	// 2008.10.15 by LYH >> 갭보정
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		//>> 갭보정 설정변경 - vntsorl(20090217)
//		m_pIChartOCX->SetUseGapRevision( m_bOrgUseGapRevision, m_nOrgCandleType != m_nCandleType);
		if( m_bOrgUseGapRevision != m_bUseGapRevision)
		{
			pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_GAPREVISION_CHANGED, m_bOrgUseGapRevision, m_nOrgCandleType != m_nCandleType);
		//<< 갭보정 설정변경 - vntsorl(20090217)
			m_bApplyUseGapRevision = m_bUseGapRevision = m_bOrgUseGapRevision;

			bChangeAllGraph = TRUE;
		}

		//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		if( m_bOrgUseMonthGapRevision != bUseMonthGapRevision)
		{
			pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_MONTHGAPREVISION_CHANGED, m_bOrgUseMonthGapRevision);
			m_bApplyUseMonthGapRevision = m_bOrgUseMonthGapRevision;
			( ( CButton *)GetDlgItem( IDC_USE_GAP_MONTHREV))->SetCheck(m_bApplyUseMonthGapRevision);
		}
		//>> (Issue No : 0006572)

		pIAddInToolMgr->Release();
	}
	// 5. 봉작도 기준
	if( m_nOrgCandleType != m_nCandleType)
	{
		m_pIChartOCX->SetNormalizedMinuteTimeRule( m_nOrgCandleType);
		m_nApplyCandleType = m_nCandleType = m_nOrgCandleType;
	}
	// (2008/11/2 - Seung-Won, Bae) Notify to MChart with CandleType and GapRevision.
	if( bIsChanged)
		m_MChartInterface.MChart_SetImaginaryCandleAndGapRevisionToOutside( m_nOrgCandleType ? true : false, m_bOrgUseGapRevision ? true : false);
		
	// 6. 편리한 기능
	// 6.1 최고/최저가 표시
	if( m_bOrgShowPriceMinMaxText != m_bShowPriceMinMaxText )
	{
		m_pIChartOCX->ShowPriceMinMaxText(m_bOrgShowPriceMinMaxText);
		m_bApplyShowPriceMinMaxText = m_bShowPriceMinMaxText = m_bOrgShowPriceMinMaxText;
	}

	if( m_nOrgMinMaxRatio != m_nMinMaxRatio )
	{
		m_pIChartOCX->SetPriceMinMaxShowType(m_nOrgMinMaxRatio);
		m_nApplyMinMaxRatio = m_nMinMaxRatio = m_nOrgMinMaxRatio;
	}

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	if( m_bOrgDrawBaseLine != m_bDrawBaseLine )
	{
		m_pIChartOCX->SetDrawBaseLine(m_bOrgDrawBaseLine);
		m_bApplyDrawBaseLine = m_bDrawBaseLine = m_bOrgDrawBaseLine;
	}

	// 6.2 수치조회창
	long lNumericalInquiryOption = m_pIChartOCX->GetNumericalInquiryOption();
	if( m_lOrgNumericalInquiryOption != lNumericalInquiryOption)
	{
		m_pIChartOCX->SetNumericalInquiryOption( m_lOrgNumericalInquiryOption);

		m_bInquieryDataWindow = ( lNumericalInquiryOption & NUMERICALINQUIRYDLG);
		m_nInquiryDlgManualMove = ( lNumericalInquiryOption & NIDLG_MANUALMOVE ? 1 : 0);
		m_bUseCrossLine = ( lNumericalInquiryOption & CROSSLINEVIEW);
		m_bUseTooptipShow = ( lNumericalInquiryOption & TOOLTIPVIEW);
		m_bUseTooptipShowAll = ( lNumericalInquiryOption & TOOLTIPVIEW_ALL);//[A00000441]풍선도움말 보기시 전체 조회 옵션 추가

		m_lApplyNumericalInquiryOption = m_lOrgNumericalInquiryOption;
	}
	
	// 7. 봉 고정기준
	if( pIChartManager && m_nOrgCandleTypeOnResize != m_nCandleTypeOnResize )
	{
		pIChartManager->SetCandleTypeOnResize( m_nOrgCandleTypeOnResize, TRUE);
		//m_MChartInterface.MChart_SetImaginaryCandleToOutside( m_nOrgCandleType ? true : false);
		m_nApplyCandleTypeOnResize = m_nCandleTypeOnResize = m_nOrgCandleTypeOnResize;
		pIChartManager->Release();
	}

	// Update to Graph -> Because GraphDraw data manage in Graph Object for Final Version.
	if (bChangeAllGraph)
		pIChartManager->ChangeAllGraphDataFromGraphInfo( TRUE);

	UpdateData(FALSE);
	Invalidate();	
	m_pIChartOCX->InvalidateControl();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 12
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabBaseEnvSetUp::PreView() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	// 1. Y축 눈금표시 ( 0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음 )
	int nYScalePosition;
	if( m_bYScalePositionLeftSide && m_bYScalePositionRightSide == FALSE )
		nYScalePosition = 0;

	else if( m_bYScalePositionLeftSide == FALSE && m_bYScalePositionRightSide)
		nYScalePosition = 1;

	else if( m_bYScalePositionLeftSide && m_bYScalePositionRightSide)
		nYScalePosition = 2;

	else if( m_bYScalePositionLeftSide == FALSE && m_bYScalePositionRightSide == FALSE )
		nYScalePosition = 3;

	if( m_nApplyYScalePosition != nYScalePosition )
	{
		SetYScalePosition( nYScalePosition );
		m_nApplyYScalePosition = nYScalePosition;
	}

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_RDO_OHLC))
	{
		// (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
		int nLH_LC_HC = m_nLH_LC_HC;
		if( m_nApplyLH_LC_HC != nLH_LC_HC)
		{
			SetShowLH_LC_HC( nLH_LC_HC );
			m_nApplyLH_LC_HC = nLH_LC_HC;
		}
	}
	else
	{
		BOOL bLH_LC_HC = m_bLH_LC_HC;
		if( m_bApplyLH_LC_HC != bLH_LC_HC)
		{
			SetShowLH_LC_HC( bLH_LC_HC ? 1 : 0);
			m_bApplyLH_LC_HC = bLH_LC_HC;
		}
	}

	int nShowCurrentPrice = GetCurrentPrice();
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	if( m_nApplyShowCurrentPrice != nShowCurrentPrice )
	{
		SetShowCurrentPrice( nShowCurrentPrice );
		m_nApplyShowCurrentPrice = nShowCurrentPrice;
	}

	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();

	// 2. 오른쪽 여백
	CString szRightMargin = "";
	m_edtRightMargin.GetWindowText(szRightMargin);
	int nRightMargin = atoi( szRightMargin );
	if( pIChartManager && m_nApplyRightMargin != nRightMargin )
	{
		// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
		m_pIChartOCX->SetRightMarginWidth( nRightMargin);
		m_nApplyRightMargin = nRightMargin;
	}

	// (2009/9/3 - Seung-Won, Bae) Check the control not in common.
	if( m_edtFutureTime.GetSafeHwnd() && m_spnFutureTime.GetSafeHwnd())
	{
		// (2009/5/17 - Seung-Won, Bae) for Future Time
		CString szFutureTime = "";
		m_edtFutureTime.GetWindowText(szFutureTime);
		int nFutureTime = atoi( szFutureTime );
		if( pIChartManager && m_nApplyFutureTime != nFutureTime )
		{
			// (2009/5/13 - Seung-Won, Bae) Manage right margin for only non special chart.
			m_pIChartOCX->SetFutureTimeCount( nFutureTime);
			m_nApplyFutureTime = nFutureTime;
		}
	}

	if(m_nApplyMouseWheelIndex != m_nMouseWheelIndex )
	{
		m_pIChartOCX->SetFutureTimeCount( m_nMouseWheelIndex);
		m_nApplyMouseWheelIndex = m_nMouseWheelIndex;
	}

	// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
	CString szUpDownSpace = "";
	m_edtUpDownSpace.GetWindowText(szUpDownSpace);
	int nUpDownSpace = atoi( szUpDownSpace );
	if( m_nApplyUpDownSpace != nUpDownSpace )
	{
		SetUpDownSpace( nUpDownSpace );
		m_nApplyUpDownSpace = nUpDownSpace;
	}

	// 2.1 봉간격일정
	if( pIChartManager && m_bApplyFixInterval != m_bFixInterval )
	{
		pIChartManager->SetFixedInterval( m_bFixInterval, TRUE );
		m_bApplyFixInterval = m_bFixInterval;
	}

	// 3. 각종지표 표시
	if( m_bApplyShowExRightDividend != ( m_bShowExRightDividend ? true : false ) )
	{
		SetShowExRightDividend( ( m_bShowExRightDividend ? true : false ) );
		m_bApplyShowExRightDividend = ( m_bShowExRightDividend ? true : false );
	}

	if( GetExistenceHighLowBound())
	{
		if( m_bApplyShowHighLowBound != ( m_bShowHighLowBound ? true : false ) )
		{
			SetShowHighLowBound( ( m_bShowHighLowBound ? true : false ) );
			m_bApplyShowHighLowBound = ( m_bShowHighLowBound ? true : false );
		}
	}

	//>>20100120_JS.Kim_alzioyes	상한가/하한가 정보표시
	if( GetExistenceLowHighMarker())
	{
		if( m_bApplyShowLowHighMarker != ( m_bShowLowHighMarker ? true : false ) )
		{
			SetShowLowHighMarker( ( m_bShowLowHighMarker? true : false ) );
			m_bApplyShowLowHighMarker = ( m_bShowLowHighMarker ? true : false );
		}
	}
	//<<

	if( m_MChartInterface.InitMChartInterface061115()) m_MChartInterface.SetRevisedPriceFlag(m_bUseRevisedPrice);
	// (2007/10/5 - Seung-Won, Bae) Support Map
	else m_pIChartOCX->FireAddInEvent( CChartEnvSetupDlgAddInImp::m_strAddInItemName, 1006, NULL, 0, ( m_bUseRevisedPrice ? 1 : 0));
	m_bApplyUseRevisedPrice = m_bUseRevisedPrice;


	// 4. 지표 정보 표시
	m_pIChartOCX->SetHideAllTitle( !m_bShowAllTitle );
	m_bApplyShowAllTitle = m_bShowAllTitle;

	if( m_bApplyShowIndicatorVariable != ( m_bShowIndicatorVariable ? true : false ) )					// 지표정보 - 지표변수 표시
	{
		SetShowIndicatorVariable( ( m_bShowIndicatorVariable ? true : false ) );
		m_bApplyShowIndicatorVariable = ( m_bShowIndicatorVariable ? true : false );
	}
	
	if( m_bApplyShowIndicatorValue != ( m_bShowIndicatorValue ? true : false ) )						// 지표정보 - 지표값 표시
	{
		SetShowIndicatorValue( ( m_bShowIndicatorValue ? true : false ) );
		 m_bApplyShowIndicatorValue = ( m_bShowIndicatorValue ? true : false );
	}

	BOOL bUseMonthGapRevision = ( ( ( CButton *)GetDlgItem( IDC_USE_GAP_MONTHREV))->GetCheck() == 1);

	// 5. 봉작도 기준
	// (2008/11/2 - Seung-Won, Bae) Notify to MChart with CandleType and GapRevision.
	BOOL bIsChanged = ( m_nApplyCandleType != m_nCandleType || m_bApplyUseGapRevision != m_bUseGapRevision || m_bApplyUseMonthGapRevision != bUseMonthGapRevision);

	BOOL bChangeAllGraph = FALSE;
	// 2008.10.15 by LYH >> 갭보정
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		//>> 갭보정 설정변경 - vntsorl(20090217)
		if(m_bApplyUseGapRevision != m_bUseGapRevision)
		{
			pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_GAPREVISION_CHANGED, m_bUseGapRevision, m_nApplyCandleType != m_nCandleType);
			m_bApplyUseGapRevision = m_bUseGapRevision;
			bChangeAllGraph = TRUE;
		}
		//<< 갭보정 설정변경 - vntsorl(20090217)

		//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		if( m_bApplyUseMonthGapRevision != bUseMonthGapRevision)
		{
			pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_MONTHGAPREVISION_CHANGED, bUseMonthGapRevision);
			m_bApplyUseMonthGapRevision = bUseMonthGapRevision;
		}
		//>> (Issue No : 0006572)

		pIAddInToolMgr->Release();
	}

	if( m_nApplyCandleType != m_nCandleType)
	{
		m_pIChartOCX->SetNormalizedMinuteTimeRule( m_nCandleType);
		m_nApplyCandleType = m_nCandleType;
	}
	// (2008/11/2 - Seung-Won, Bae) Notify to MChart with CandleType and GapRevision.
	if( bIsChanged)
		m_MChartInterface.MChart_SetImaginaryCandleAndGapRevisionToOutside( m_nApplyCandleType ? true : false, m_bApplyUseGapRevision ? true : false);

	// 6. 편리한 기능
	// 6.1 최고/최저가 표시
	m_pIChartOCX->ShowPriceMinMaxText(m_bShowPriceMinMaxText);
	m_bApplyShowPriceMinMaxText = m_bShowPriceMinMaxText;

	m_pIChartOCX->SetPriceMinMaxShowType(m_nMinMaxRatio);
	m_nApplyMinMaxRatio = m_nMinMaxRatio;

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_pIChartOCX->SetDrawBaseLine(m_bDrawBaseLine);
	m_bApplyDrawBaseLine = m_bDrawBaseLine;

	// 6.2 수치조회창
	//long lNumericalInquiryOption = ( m_pIChartOCX->GetNumericalInquiryOption() & TOOLTIPVIEW);
	long lNumericalInquiryOption = ( m_pIChartOCX->GetNumericalInquiryOption());
	if( m_bInquieryDataWindow)			lNumericalInquiryOption |= NUMERICALINQUIRYDLG;
	else								lNumericalInquiryOption &= ~NUMERICALINQUIRYDLG;

	if( m_nInquiryDlgManualMove == 1)	lNumericalInquiryOption |= NIDLG_MANUALMOVE;
	else								lNumericalInquiryOption &= ~NIDLG_MANUALMOVE;

	if( m_bUseCrossLine)				lNumericalInquiryOption |= CROSSLINEVIEW;
	else								lNumericalInquiryOption &= ~CROSSLINEVIEW;

	if( m_bUseTooptipShow)				lNumericalInquiryOption |= TOOLTIPVIEW;
	else								lNumericalInquiryOption &= ~TOOLTIPVIEW;

	//>>[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
	if( m_bUseTooptipShowAll)			lNumericalInquiryOption |= TOOLTIPVIEW_ALL;
	else								lNumericalInquiryOption &= ~TOOLTIPVIEW_ALL;
	//<< 

	if( m_lApplyNumericalInquiryOption != lNumericalInquiryOption)
	{
		m_pIChartOCX->SetNumericalInquiryOption( lNumericalInquiryOption);
		m_lApplyNumericalInquiryOption = lNumericalInquiryOption;
	}

	// 7. 봉 고정기준
	if( pIChartManager && m_nApplyCandleTypeOnResize != m_nCandleTypeOnResize )
	{
		pIChartManager->SetCandleTypeOnResize( m_nCandleTypeOnResize, TRUE);
		//m_MChartInterface.MChart_SetImaginaryCandleToOutside( m_nCandleType ? true : false);
		m_nApplyCandleTypeOnResize = m_nCandleTypeOnResize;
		pIChartManager->Release();
	}

	// Update to Graph -> Because GraphDraw data manage in Graph Object for Final Version.
	if (bChangeAllGraph)
		pIChartManager->ChangeAllGraphDataFromGraphInfo( TRUE);

	m_bPreview = TRUE;
	m_pIChartOCX->InvalidateControl();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Return void : 
// Param  CNumSpinCtrl &pSpin : 
// Param  CNumEdit &pEdit : 
// Param  const double Value : 
// Comments		: 설정부분 초기화
//-----------------------------------------------------------------------------
void CDlgMainTabBaseEnvSetUp::Initial_Spin( CNumSpinCtrl &pSpin, CNumEdit &pEdit, const int Value, const float p_dMax)
{
	CNumEdit *pNumEdit = pSpin.GetBuddy();
	
	// 1. 인자로 받은 pEdit으로 Period에 값을 설정한다.
	if( pNumEdit == NULL || *pNumEdit != pEdit )
		pSpin.SetBuddy( &pEdit ); 
	
	// 2. Spin을 초기화 한다.
	pSpin.SetRange( 0, p_dMax);
	pSpin.SetPos( ( float)Value);
	pSpin.SetDelta( 1);	
}


BOOL CDlgMainTabBaseEnvSetUp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentWnd != NULL )
				( (CDlgEnvSetUpAll *)m_pParentWnd)->OK();
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
				( (CDlgEnvSetUpAll *)m_pParentWnd)->Cancel();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgMainTabBaseEnvSetUp::OnShowCurrentPrice() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	if( m_bShowCurrentPrice)
// 	{
// 		GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_1)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_2)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_3)->EnableWindow( TRUE );
// 		GetDlgItem(IDC_CUR_PRICE_4)->EnableWindow( TRUE );
// 
// 		// 디자인설정과 동일게 맞춤 - ojtaso (20071112)
// 		m_nCurPriceSub = GetShowCurrentPrice() - 1;
// 
// 		if(m_nCurPriceSub<0) m_nCurPriceSub=0;
// 		UpdateData(FALSE);
// 	}
// 
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_1)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_2)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_3)->EnableWindow( FALSE );
// 		GetDlgItem(IDC_CUR_PRICE_4)->EnableWindow( FALSE );
// 	}

	if( GetDlgItem(IDC_STATIC_CUR_PRICE) ){

		CButton *pBtn = NULL;
		if( pBtn = (CButton *)GetDlgItem(IDC_CUR_PRICE_TYPE_PREDAY) ){
			pBtn->EnableWindow(m_bEnablePreDay && m_bShowCurrentPrice);
		}

		if( (pBtn = (CButton *)GetDlgItem(IDC_CUR_PRICE_TYPE_PREBONG)) && !m_bEnablePreDay ){
			// 전일기준이 불가능하고 전봉기준만 가능한경우
			m_nPriceBongType = 1;
		}

		if( m_bShowCurrentPrice)
		{
			GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( TRUE );
			GetDlgItem(IDC_CUR_PRICE_TYPE_COMBO)->EnableWindow( TRUE );
			pBtn->EnableWindow(TRUE);

			// 디자인설정과 동일게 맞춤 - ojtaso (20071112)
			m_nCurPriceSub = GetShowCurrentPrice() - 1;
			UpdateData(FALSE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_CUR_PRICE)->EnableWindow( FALSE );
			GetDlgItem(IDC_CUR_PRICE_TYPE_COMBO)->EnableWindow( FALSE );
			pBtn->EnableWindow(FALSE);
		}
	}
}

//sy 2005.10.12
CBaseTabDlg::DLG_TYPE CDlgMainTabBaseEnvSetUp::GetDialogType() const
{
	return CBaseTabDlg::ENV_SET;
}

bool CDlgMainTabBaseEnvSetUp::GetCurrentPrice(int& nCurPrice) const
{
	nCurPrice = GetCurrentPrice();
	return true;
}

int CDlgMainTabBaseEnvSetUp::GetCurrentPrice() const
{
	if(!m_bShowCurrentPrice)
		return 0;

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	// 디자인설정과 동일게 맞춤 - ojtaso (20071112)
// 	return m_nCurPriceSub + 1;

	int cur = m_curPriceTypeCombo.GetCurSel();
	int rslt = 0;

	if( m_nPriceBongType==0 ){
	// 전일기준
		switch(cur){
		case 0: rslt = 4; break;
		case 1: rslt = 8; break;	// (2011/2/9 - Seung-Won, Bae) for the difference only.
		case 2: rslt = 0; break;
		case 3: rslt = 2; break;
		case 4: rslt = 6; break;
		default: rslt = 0; break;
		}
	}
	else {
	// 전봉기준
		switch(cur){
		case 0: rslt = 5; break;
		case 1: rslt = 9; break;	// (2011/2/9 - Seung-Won, Bae) for the difference only.
		case 2: rslt = 1; break;
		case 3: rslt = 3; break;
		case 4: rslt = 7; break;
		default: rslt = 0; break;
		}
	}

	return rslt+1;
}
//sy end

/////////////////////////////////////////////////////////////////////////////
// (2006/8/16 - Seung-Won, Bae) Info Get/Set Library Functions
/////////////////////////////////////////////////////////////////////////////

int CDlgMainTabBaseEnvSetUp::GetShowCurrentPrice( void)
{
	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return 0;

	IGraphInfo *pIPriceChart = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, _MTEXT( C2_PRICE_CHART ));
	pIGraphInfoManager->Release();
	if( !pIPriceChart) return 0;

	BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE, bShowOHLC = FALSE;
	int nCurData = 0;
	pIPriceChart->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
	pIPriceChart->Release();

	return nCurData;
}

// LH/LC/HC - ojtaso (20071226)
int CDlgMainTabBaseEnvSetUp::GetShowLH_LC_HC( void)
{
	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return 0;

	IGraphInfo *pIPriceChart = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, _MTEXT( C2_PRICE_CHART ));
	pIGraphInfoManager->Release();
	if( !pIPriceChart) return 0;

	BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE, bShowOHLC = FALSE;
	int nCurData = 0;
	pIPriceChart->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
	pIPriceChart->Release();

	if( bLowHigh && bLowCur && bHighCur) return 1;
	if( bShowOHLC) return 0;
	return 2;
}

BOOL CDlgMainTabBaseEnvSetUp::GetExistenceHighLowBound( void)
{
	long lResult = 0;
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_EXISTENCE_BOUNDMARK);
		pIAddInToolMgr->Release();
	}
	return lResult;
}

void CDlgMainTabBaseEnvSetUp::SetShowHighLowBound( BOOL p_bEnable)
{
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, p_bEnable);
		pIAddInToolMgr->Release();
	}

	m_MChartInterface.MChart_SetShowHighLowBoundToOutside( p_bEnable ? true : false );
}

//>>20100120_JS.Kim_alzioyes	상한가/하한가 정보표시
BOOL CDlgMainTabBaseEnvSetUp::GetExistenceLowHighMarker( void)
{
	long lResult = 0;
 	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
 	if( pIAddInToolMgr)
 	{
 		pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_EXISTENCE_LOWHIGHMARK);
 		pIAddInToolMgr->Release();
 	}
	return lResult;
/*
	BOOL bReturn = FALSE;
	ILPCSTR strData;
	IPacketManager *pIPacketManager = m_pIChartOCX->GetIDefaultPacketManager();
	IPacket *pPacket = pIPacketManager->GetPacket( _MTEXT( C0_UPDOWN_SIGN));
	bReturn = (pPacket!=NULL ? TRUE : FALSE);
	if( pIPacketManager ) pIPacketManager->Release();
	if( pPacket ) pPacket->Release();
	return bReturn;
*/
}

void CDlgMainTabBaseEnvSetUp::SetShowLowHighMarker( BOOL p_bEnable)
{
 	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
 	if( pIAddInToolMgr)
 	{
 		pIAddInToolMgr->OnAddInToolCommand( EAI_CUSTOM_ENABLE_LOWHIGHMARK, p_bEnable);
 		pIAddInToolMgr->Release();
 	}
 	
//KHD 	m_MChartInterface.MChart_SetShowLowHighMarkerToOutside( p_bEnable ? true : false );
}
//<<

void CDlgMainTabBaseEnvSetUp::SetShowExRightDividend(const bool bShow)
{
	BOOL bIsCurExistence = FALSE;
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( pIChartManager)
	{
		int nR = -1, nC = -1, nV = -1, nG = -1;
		ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_LOCK), nR, nC, nV, nG, NULL);
		bIsCurExistence = !szGraphName.IsEmpty();
		pIChartManager->Release();
	}

	if( bIsCurExistence && !bShow)		m_pIChartOCX->RemoveChart(		_MTEXT( C2_LOCK));
	else if( !bIsCurExistence && bShow) m_pIChartOCX->AddChartBlock(	_MTEXT( C2_LOCK));

	m_MChartInterface.MChart_SetShowExRightDividendToOutside( bShow);
}

void CDlgMainTabBaseEnvSetUp::SetShowCurrentPrice(const int nShowType)
{
	// (2008/11/3 - Seung-Won, Bae) for multiple indicator.
	IPacketListManager *pIPacketListManager = m_pIChartOCX->GetIPacketListManager();
	if( !pIPacketListManager) return;
	CStringArray arRQ;
	pIPacketListManager->GetAllRQ( arRQ);
	arRQ.Add( "DEFAULT");
	pIPacketListManager->Release();

	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return;

	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager)
	{
		pIChartManager->Release();
		return;
	}

	for( int i = 0; i < arRQ.GetSize(); i++)
	{
		int nR = -1, nC = -1, nV = -1, nG = -1;
		ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, arRQ.GetAt( i));
		if( !szGraphName.IsEmpty())
		{
			IGraphInfo *pIPriceChart = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
			if( pIPriceChart)
			{
				BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE, bShowOHLC = FALSE;
				int nCurData = 0;
				pIPriceChart->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
				
				nCurData = nShowType;
				pIPriceChart->SetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
				pIPriceChart->Release();
			}
		}
	}
			
	// Update to Graph -> Because GraphDraw data manage in Graph Object for Final Version.
	pIChartManager->ChangeAllGraphDataFromGraphInfo( FALSE);

	pIChartManager->Release();
	pIGraphInfoManager->Release();
	pIChartManager->Release();

	m_MChartInterface.MChart_SetShowCurrentPriceToOutside( nShowType);
	m_pIChartOCX->InvalidateControl();
}

// LH/LC/HC - ojtaso (20071226)
void CDlgMainTabBaseEnvSetUp::SetShowLH_LC_HC(const int nLH_LC_HC)
{
	// (2008/11/3 - Seung-Won, Bae) for multiple indicator.
	IPacketListManager *pIPacketListManager = m_pIChartOCX->GetIPacketListManager();
	if( !pIPacketListManager) return;
	CStringArray arRQ;
	pIPacketListManager->GetAllRQ( arRQ);
	arRQ.Add( "DEFAULT");
	pIPacketListManager->Release();

	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return;

	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager)
	{
		pIChartManager->Release();
		return;
	}

	for( int i = 0; i < arRQ.GetSize(); i++)
	{
		int nR = -1, nC = -1, nV = -1, nG = -1;
		ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, arRQ.GetAt( i));
		if( !szGraphName.IsEmpty())
		{
			IGraphInfo *pIPriceChart = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
			if( pIPriceChart)
			{
				BOOL bLowHigh = FALSE, bLowCur = FALSE, bHighCur = FALSE, bPrevCur = FALSE, bShowOHLC = FALSE;
				int nCurData = 0;
				pIPriceChart->GetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
				bLowHigh = bLowCur = bHighCur = ( nLH_LC_HC == 1);
				bShowOHLC = ( nLH_LC_HC == 0);
				pIPriceChart->SetGraphDataDrawingData( bLowHigh, bLowCur, bHighCur, bPrevCur, nCurData, bShowOHLC);
				pIPriceChart->Release();
			}
		}
	}

	// Update to Graph -> Because GraphDraw data manage in Graph Object for Final Version.
	pIChartManager->ChangeAllGraphDataFromGraphInfo( FALSE);

	pIChartManager->Release();
	pIGraphInfoManager->Release();

	// (2008/10/26 - Seung-Won, Bae) for O/H/L/C (not LH/LC/HC)
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( EAI_ENVSETUP_LHLCHC_CHANGED);
		pIAddInToolMgr->Release();
	}

	m_pIChartOCX->InvalidateControl();
}

void CDlgMainTabBaseEnvSetUp::SetShowIndicatorVariable(const bool bShow)
{
	BOOL bWithConditionValue = FALSE, bWithLastValue = FALSE;
	m_pIChartOCX->GetGraphTitleOption( bWithConditionValue, bWithLastValue);

	bWithConditionValue = bShow;
	m_pIChartOCX->SetGraphTitleOption( bWithConditionValue, bWithLastValue);

//	m_MChartInterface.MChart_SetShowIndicatorVariableToOutside(bShow);
}

void CDlgMainTabBaseEnvSetUp::SetShowIndicatorValue(const bool bShow)
{
	BOOL bWithConditionValue = FALSE, bWithLastValue = FALSE;
	m_pIChartOCX->GetGraphTitleOption( bWithConditionValue, bWithLastValue);

	bWithLastValue = bShow;
	m_pIChartOCX->SetGraphTitleOption( bWithConditionValue, bWithLastValue);

//	m_MChartInterface.MChart_SetShowIndicatorVariableToOutside(bShow);
}

void CDlgMainTabBaseEnvSetUp::SetYScalePosition(const int nPosition)
{
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return;

	pIChartManager->SetAllVertScalePosition( CScaleBaseData::VERTSCALEPOSITION( nPosition));
	pIChartManager->Release();
}

void CDlgMainTabBaseEnvSetUp::OnUseIndicatorName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	EnableUseIndicatorName( m_bShowAllTitle );
}

// (2007/7/18 - Seung-Won, Bae) Up/Down Space.
void CDlgMainTabBaseEnvSetUp::SetUpDownSpace(const int nUpDownSpace)
{
	IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return;

	int nNewSpace = nUpDownSpace / 2;
	int nR = 0, nC = 0, nV = 0, nG = 0;
	BOOL bIsLock = FALSE;
	CScaleBaseData::VERTMINMAXTYPE eMinMaxType = CScaleBaseData::VERT_MINMAX_OF_VIEW;
	double p_dMin = 0.0, p_dMax = 0.0;
	ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART ), nR, nC, nV, nG, NULL);
	while( !szGraphName.IsEmpty())
	{
		IBlock *pIBlock = pIChartManager->GetBlock( nR, nC);
		if( !pIBlock) break;

		nV = -1;
		do
		{
			nV++;
			if( !pIBlock->GetVScaleMinMaxSetting( nV, bIsLock, eMinMaxType, p_dMin, p_dMax)) break;
			if( !bIsLock && CScaleBaseData::VERT_MINMAX_OF_USER == eMinMaxType)
				pIBlock->SetVScaleMinMaxSetting( nV, bIsLock, CScaleBaseData::VERT_MINMAX_OF_VIEW, p_dMin, p_dMax);
		}
		while( ( bIsLock ? TRUE : pIChartManager->SetVertScaleSpace( nR, nC, nV, nNewSpace, nUpDownSpace - nNewSpace)));
		pIBlock->Release();

		nR++;
		nV = 0;
		nG = -1;		// for Include first graph.
		szGraphName = pIChartManager->FindNextIndicator( _MTEXT( C2_PRICE_CHART ), nR, nC, nV, nG, NULL);
	}

	if( pIChartManager) pIChartManager->Release();
	m_pIChartOCX->InvalidateControl();
}

//>> 갭보정 - vntsorl(20090217)
BOOL CDlgMainTabBaseEnvSetUp::GetGapRevision( void)
{
	long lResult = 0;
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_GAPREVISION);
		pIAddInToolMgr->Release();
	}
	return lResult;
}
//<< 갭보정 - vntsorl(20090217)

//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
BOOL CDlgMainTabBaseEnvSetUp::GetMonthGapRevision( void)
{
	long lResult = 0;
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_CUSTOM_GET_MONTHGAPREVISION);
		pIAddInToolMgr->Release();
	}
	return lResult;
}

BOOL CDlgMainTabBaseEnvSetUp::GetShowMonthGapRevision( void)
{
	long lResult = 0;
	IAddInToolMgr *pIAddInToolMgr = m_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( lResult, EAI_ENVSETUP_SHOWMONTHGAPREVISION_CHANGED);
		pIAddInToolMgr->Release();
	}
	return lResult;
}
//>> (Issue No : 0006572)

// (2009/9/3 - Seung-Won, Bae) Sync UI
void CDlgMainTabBaseEnvSetUp::OnChkLhLcHc() 
{
	// TODO: Add your control notification handler code here
	if( !GetDlgItem( IDC_RDO_NONE)) return;
	CButton *pButton = ( CButton *)GetDlgItem( IDC_CHK_LH_LC_HC);
	( ( CButton *)GetDlgItem( IDC_RDO_LH_LC_HC))->SetCheck( pButton->GetCheck());
	( ( CButton *)GetDlgItem( IDC_RDO_OHLC))->SetCheck( FALSE);
	( ( CButton *)GetDlgItem( IDC_RDO_NONE))->SetCheck( !pButton->GetCheck());
}
void CDlgMainTabBaseEnvSetUp::OnRdoLhLcHc() 
{
	// TODO: Add your control notification handler code here
	if( !GetDlgItem( IDC_CHK_LH_LC_HC)) return;
	CButton *pButton = ( CButton *)GetDlgItem( IDC_RDO_LH_LC_HC);
	( ( CButton *)GetDlgItem( IDC_CHK_LH_LC_HC))->SetCheck( pButton->GetCheck());
}

void CDlgMainTabBaseEnvSetUp::OnShowMinMaxPrice() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_RDO_CURPRICE)->EnableWindow(m_bShowPriceMinMaxText);
	GetDlgItem(IDC_RDO_STANDPRICE)->EnableWindow(m_bShowPriceMinMaxText);
}

//KHD : 시간구분 
void CDlgMainTabBaseEnvSetUp::SetTimeGubunGrid()
{
	//KHD : 첫 번째 인자는 그리드타입을 넣어줘야 한다. 점선, 실선....이것을 세팅하는 것이다.
	//일단, Style Dlg에서 클릭 후 컬러세팅을 하면 OCX에 저장이 되므로 OCX에 세팅된 값을 직접 가져와서 여기서 직접 다시 세팅하는 방법을 간구해보자.
	//Ex 일에서 체크했다 하면 스타일의 일자 정보를 자져와서 세팅. 중간단계가 필요함
	//여기서 Pixel 정보도 넘겨야 함.
	int nGridType = 3;
	//KHD : 두번째 인자는 현재 사용되지 않기에 현재 세팅된 시간구분 Check 여부를 보낸다.
	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::MINTICK_DAY_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_TICK_GUBUN))->GetCheck(), CScaleBaseData::MINTICK_DAY_GUBUN );

	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::DAYCHART_WEEK_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_DAY_WEEK_GUBUN))->GetCheck(), CScaleBaseData::DAYCHART_WEEK_GUBUN );

	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::DAYCHART_MON_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_DAY_MON_GUBUN))->GetCheck(), CScaleBaseData::DAYCHART_MON_GUBUN );

	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::DAYCHART_YEAR_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_DAY_YEAR_GUBUN))->GetCheck(), CScaleBaseData::DAYCHART_YEAR_GUBUN );

	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::WEEKCHART_MON_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_WEEK_MON_GUBUN))->GetCheck(), CScaleBaseData::WEEKCHART_MON_GUBUN );

	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::WEEKCHART_YEAR_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_WEEK_YEAR_GUBUN))->GetCheck(), CScaleBaseData::WEEKCHART_YEAR_GUBUN );

	nGridType = ((CDlgEnvSetUpAll *)m_pParentWnd)->GetGridTypeCombo(CScaleBaseData::MONCHART_YEAR_GUBUN);
	m_pIChartOCX->SetAllHScaleGridType_TimeGubun(nGridType, ((CButton *)GetDlgItem(IDC_MON_YEAR_GUBUN))->GetCheck(), CScaleBaseData::MONCHART_YEAR_GUBUN );
}
//END
