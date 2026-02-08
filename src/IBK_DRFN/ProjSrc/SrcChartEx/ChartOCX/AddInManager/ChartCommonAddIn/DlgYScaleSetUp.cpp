// DlgYScaleSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "chartcommonaddin.h"
#include "DlgYScaleSetUp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_LANGUAGE_RES
#include "../../Include_Chart/Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgYScaleSetUp dialog


CDlgYScaleSetUp::CDlgYScaleSetUp(CYScaleSetUpAddInImp *pAddIn, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode, UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog( nIDTemplate, pParent)
{
	// (2009/9/14 - Seung-Won, Bae) for World-On
	m_eChartMode = p_eChartMode;

	//{{AFX_DATA_INIT(CDlgYScaleSetUp)
	m_nYScaleMaxMinSetupType = 0;
	m_nSelectPercent = 0;
	m_strYesterdayClosePrice = _T("");
	m_strMaxPrice = _T("");
	m_strMinPrice = _T("");
	//}}AFX_DATA_INIT
	m_strUpperBound = _T("");
	m_strLowerBound = _T("");
	
	m_pAddIn = NULL;
	if( pAddIn )
		m_pAddIn = pAddIn;

	m_nYScaleMaxMinSetupType = 0;
	m_nSelectPercent = -1;

	m_bShow = FALSE;

	// (2007/5/2 - Seung-Won, Bae) Check Market Type for Unit.
	m_eMarketType = CPacketType::STOCK;

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
}


void CDlgYScaleSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgYScaleSetUp)
	DDX_Radio(pDX, IDC_USE_SETUP_VALUE, m_nYScaleMaxMinSetupType);
	DDX_Radio(pDX, IDC_30_PERCENT, m_nSelectPercent);
	DDX_Text(pDX, IDC_YESTERDAY_CLOSE_PRICE, m_strYesterdayClosePrice);
	DDX_Text(pDX, IDC_MAX_PRICE_EDIT, m_strMaxPrice);
	DDX_Text(pDX, IDC_MIN_PRICE_EDIT, m_strMinPrice);
	//}}AFX_DATA_MAP

	// (2009/9/28 - Seung-Won, Bae) Check the control not in common.
	if( GetDlgItem( IDC_UPPER_BOUND))
		DDX_Text(pDX, IDC_UPPER_BOUND, m_strUpperBound);
	if( GetDlgItem( IDC_LOWER_BOUND))
		DDX_Text(pDX, IDC_LOWER_BOUND, m_strLowerBound);
}


BEGIN_MESSAGE_MAP(CDlgYScaleSetUp, CDialog)
	//{{AFX_MSG_MAP(CDlgYScaleSetUp)
	ON_BN_CLICKED(IDC_USE_SETUP_VALUE, OnUseSetupValue)
	ON_BN_CLICKED(IDC_USE_PERCENT, OnUsePercent)
	ON_BN_CLICKED(IDC_30_PERCENT, On30Percent)
	ON_BN_CLICKED(IDC_50_PERCENT, On50Percent)
	ON_BN_CLICKED(IDC_70_PERCENT, On70Percent)
	ON_BN_CLICKED(IDC_100_PERCENT, On100Percent)
	ON_EN_CHANGE(IDC_MAX_PRICE_EDIT, OnChangeMaxPriceEdit)
	ON_EN_CHANGE(IDC_MIN_PRICE_EDIT, OnChangeMinPriceEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgYScaleSetUp message handlers

BOOL CDlgYScaleSetUp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// Dialog에 포커스를 주기 위해 사용
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	// 1. "구분" 값의 변경시 Control들이 움직여야 되는 높이를 구한다.
	CRect rcStaticPercent;
	GetDlgItem(IDC_STATIC_PERCENT)->GetWindowRect(rcStaticPercent);

	CRect rcEdit;
	GetDlgItem(IDC_MAX_PRICE_EDIT)->GetWindowRect(rcEdit);

	// 2. Control들이 움직여야 되는 높이는 Percent 설정 GroupBox의 높이만큼으로 정한다.
	m_nHeightToMoving = rcEdit.top - rcStaticPercent.top - 4;

//	m_strYesterdayClosePrice.Format( m_strValueFormat, m_dYesterdayClosePrice );
	// 20081016 이주표

//@Solomon-091113-->
// Before
//	m_strYesterdayClosePrice = CDataConversion::DoubleToString(m_dYesterdayClosePrice, m_strPacketType);
//	m_strUpperBound.Format( m_strValueFormat, m_dUpperBound );
//	m_strLowerBound.Format( m_strValueFormat, m_dLowerBound );
// After
	if(m_strValueFormat.Compare("%.2f")==0)
	{
		m_strYesterdayClosePrice.Format("%d.%02d", (int)m_dYesterdayClosePrice/100, (int)m_dYesterdayClosePrice%100);
		m_strUpperBound.Format("%d.%02d", (int)m_dUpperBound/100, (int)m_dUpperBound%100);
		m_strLowerBound.Format("%d.%02d", (int)m_dLowerBound/100, (int)m_dLowerBound%100);
	}
	else
	{
		m_strYesterdayClosePrice = CDataConversion::DoubleToString(m_dYesterdayClosePrice, m_strPacketType);
		m_strUpperBound.Format( m_strValueFormat, m_dUpperBound );
		m_strLowerBound.Format( m_strValueFormat, m_dLowerBound );
	}
//@Solomon-091113<--

	if( m_nYScaleMaxMinSetupType == 0 )
		OnUseSetupValue();

	else
		OnUsePercent();

	// (2009/9/14 - Seung-Won, Bae) Show WorldOn Label.
	if( CChartInfo::WORLD_ON == m_eChartMode)
	{
		CString strAdjustedPrice;
		strAdjustedPrice.LoadString( IDS_ADJUSTED_PRICE);
		GetDlgItem( IDC_STC_PREV_PRICE)->SetWindowText( strAdjustedPrice);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgYScaleSetUp::SetValueFormat( CString strFormat )
{
	m_strValueFormat = strFormat;
}


void CDlgYScaleSetUp::SetStockPriceInfo( double dYesterpayClosePrice, double dUpperBound, double dLowerBound )
{
	m_dYesterdayClosePrice = dYesterpayClosePrice;
	m_dUpperBound = dUpperBound;
	m_dLowerBound = dLowerBound;
}


void CDlgYScaleSetUp::MovingAllControls( void )
{
	CRect rcDlgYScaleSetUp;
	GetWindowRect(rcDlgYScaleSetUp);

	CRect rcStatic;
	GetDlgItem(IDC_STATIC_MAX_MIN)->GetWindowRect(rcStatic);
	ScreenToClient(rcStatic);

	if( m_bShow == FALSE )
	{
		rcDlgYScaleSetUp.bottom = rcDlgYScaleSetUp.bottom - m_nHeightToMoving;
		rcStatic.bottom = rcStatic.bottom - m_nHeightToMoving;
	}

	else
	{
		rcDlgYScaleSetUp.bottom = rcDlgYScaleSetUp.bottom + m_nHeightToMoving;
		rcStatic.bottom = rcStatic.bottom + m_nHeightToMoving;
	}

	MoveWindow(rcDlgYScaleSetUp);

	GetDlgItem(IDC_STATIC_MAX_MIN)->MoveWindow(rcStatic);

	// 최대값 Control들의 이동
	MoveControlPos(IDC_MAX_PRICE_TITLE, m_bShow);
	MoveControlPos(IDC_MAX_PRICE_EDIT, m_bShow);
	MoveControlPos(IDC_MAX_PRICE_UNIT, m_bShow);

	// 최소값 Control들의 이동
	MoveControlPos(IDC_MIN_PRICE_TITLE, m_bShow);
	MoveControlPos(IDC_MIN_PRICE_EDIT, m_bShow);
	MoveControlPos(IDC_MIN_PRICE_UNIT, m_bShow);

	// 확인 취소버튼의 이동
	MoveControlPos(IDOK, m_bShow);
	MoveControlPos(IDCANCEL, m_bShow);

	m_bShow = !m_bShow;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 08
// Return void	: 
// Param  int nID : 
// Param  int nHeightToMoving : 
// Param  UINT nFlags : 
// Comments		: nID의 Control을 nHeightToMoving만큼 이동시킨다.
//-----------------------------------------------------------------------------
void CDlgYScaleSetUp::MoveControlPos( int nID, BOOL bSign )
{
	CRect rcTemp;
	GetDlgItem(nID)->GetWindowRect(rcTemp);
	ScreenToClient(rcTemp);

	if( bSign )
	{
		rcTemp.top = rcTemp.top + m_nHeightToMoving;
		rcTemp.bottom = rcTemp.bottom + m_nHeightToMoving;
	}

	else
	{
		rcTemp.top = rcTemp.top - m_nHeightToMoving;
		rcTemp.bottom = rcTemp.bottom - m_nHeightToMoving;
	}

	GetDlgItem(nID)->MoveWindow(rcTemp);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 08
// Modifier		: 
// Comments		: 설정값을 사용할 때에는 비율 설정 부분을 사용하지 않는다.
//-----------------------------------------------------------------------------
void CDlgYScaleSetUp::OnUseSetupValue() 
{
	// TODO: Add your control notification handler code here
		
	m_nYScaleMaxMinSetupType = 0;

	// 1. 초기 상태 그대로이면 return한다.
	if( !m_bShow  )
		MovingAllControls();

	// 4. Percent GroupBox를 숨긴다.
	GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow( SW_HIDE );

	// 5. Each Percent 들을 숨긴다.
	GetDlgItem(IDC_30_PERCENT)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_50_PERCENT)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_70_PERCENT)->ShowWindow( SW_HIDE );
	GetDlgItem(IDC_100_PERCENT)->ShowWindow( SW_HIDE );

	// (2007/5/2 - Seung-Won, Bae) Check Market Type for Unit.
	ML_SET_LANGUAGE_RES();
	CString strUnit;
	strUnit.LoadString( IDS_WON);
	if( CPacketType::STOCK != m_eMarketType && CPacketType::ELW != m_eMarketType) strUnit = "Pt";
	if( CChartInfo::WORLD_ON == m_eChartMode) strUnit = "";

	// 6. 단위 부분의 Text를 변경한다.
	GetDlgItem(IDC_MAX_PRICE_UNIT)->SetWindowText( strUnit);
	GetDlgItem(IDC_MIN_PRICE_UNIT)->SetWindowText( strUnit);

//	m_strMaxPrice.Format( m_strValueFormat, m_dYScaleMaxValue);
//	m_strMinPrice.Format( m_strValueFormat, m_dYScaleMinValue);
	// 20081016 이주표
	m_strMaxPrice = CDataConversion::DoubleToString(m_dYScaleMaxValue, m_strPacketType);
	m_strMinPrice =	CDataConversion::DoubleToString(m_dYScaleMinValue, m_strPacketType);
	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 08
// Modifier		: 
// Comments		: 비율값을 사용할 때에는 비율 설정 부분을 보이게 한다.
//-----------------------------------------------------------------------------
void CDlgYScaleSetUp::OnUsePercent() 
{
	// TODO: Add your control notification handler code here
	m_nYScaleMaxMinSetupType = 1;

	// 1. 초기 상태 그대로이면 return한다.
	if( m_bShow )
		MovingAllControls();		// 2. 위치이동이 필요한 Control들을 이동시켜준다.

	// 3. "최대 / 최소 설정" GroupBox의 크기를 원상태로 늘려준다.

	
	// 4. Percent GroupBox를 보이게 한다.
	GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow( SW_SHOW );

	// 5. Each Percent 들을 보이게 한다.
	GetDlgItem(IDC_30_PERCENT)->ShowWindow( SW_SHOW );
	GetDlgItem(IDC_50_PERCENT)->ShowWindow( SW_SHOW );
	GetDlgItem(IDC_70_PERCENT)->ShowWindow( SW_SHOW );
	GetDlgItem(IDC_100_PERCENT)->ShowWindow( SW_SHOW );

	// 6. 단위 부분의 Text를 변경한다.
	GetDlgItem(IDC_MAX_PRICE_UNIT)->SetWindowText("%");
	GetDlgItem(IDC_MIN_PRICE_UNIT)->SetWindowText("%");

	m_strMaxPrice.Format("%.f", m_dYScaleMaxPercentValue);
	m_strMinPrice.Format("%.f", m_dYScaleMinPercentValue);
	
	// 7. percent값에 따라 Redio를 선택해준다.
	if( m_strMaxPrice == "30" && m_strMinPrice == "-30" )
		m_nSelectPercent = 0;
	else if( m_strMaxPrice == "50" && m_strMinPrice == "-50" )
		m_nSelectPercent = 1;
	else if( m_strMaxPrice == "70" && m_strMinPrice == "-70" )
		m_nSelectPercent = 2;
	else if( m_strMaxPrice == "100" && m_strMinPrice == "-100" )
		m_nSelectPercent = 3;
	else
		m_nSelectPercent = -1;

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 08
// Modifier		: 
// Comments		: 현재값을 저장한다.
//-----------------------------------------------------------------------------
void CDlgYScaleSetUp::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if( m_nYScaleMaxMinSetupType == 0 )
	{
		// 20081016 이주표 
		CString strMaxPrice, strMinPrice;
		BOOL bSuccess = CDataConversion::GetDataToDec(m_strMaxPrice, m_strPacketType, strMaxPrice);
		if(!bSuccess)
			strMaxPrice = "0.0";
		bSuccess = CDataConversion::GetDataToDec(m_strMinPrice, m_strPacketType, strMinPrice);
		if(!bSuccess)
			strMinPrice = "0.0";
		m_dYScaleMaxValue = atof(strMaxPrice);
		m_dYScaleMinValue = atof(strMinPrice);
//		m_dYScaleMaxValue = atof(m_strMaxPrice);
//		m_dYScaleMinValue = atof(m_strMinPrice);
	}

	else
	{
		m_dYScaleMaxPercentValue = atof(m_strMaxPrice);
		m_dYScaleMinPercentValue = atof(m_strMinPrice);
	}

	if( m_pAddIn )
		m_pAddIn->SetYScaleAllValue( m_nYScaleMaxMinSetupType, m_dYScaleMaxValue, m_dYScaleMinValue,
									 m_dYScaleMaxPercentValue, m_dYScaleMinPercentValue );
	CDialog::OnOK();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 08
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgYScaleSetUp::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return int	: 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------	
int CDlgYScaleSetUp::GetYScaleMaxMinSetupType( void )
{
	return m_nYScaleMaxMinSetupType;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return void	: 
// Param  int nYScaleMinMaxSetupType : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgYScaleSetUp::SetYScaleMaxMinSetupType( int nYScaleMinMaxSetupType )
{
	m_nYScaleMaxMinSetupType = nYScaleMinMaxSetupType;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return double: 
// Param  void	: 
// Comments		: m_nYScaleMaxMinSetupType에 따라 해당되는 값을 return한다.
//-----------------------------------------------------------------------------
double CDlgYScaleSetUp::GetYScaleMax( void )
{
	// 최대 설정값을 return한다.
	return m_dYScaleMaxValue;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return double: 
// Param  void	: 
// Comments		: m_nYScaleMaxMinSetupType에 따라 해당되는 값을 return한다.
//-----------------------------------------------------------------------------	
double CDlgYScaleSetUp::GetYScaleMin( void )
{
	// 최소 설정값을 return한다.
	return m_dYScaleMinValue;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return BOOL	: 
// Param  double dMax : 
// Param  double dMin : 
// Comments		: 
//-----------------------------------------------------------------------------	
void CDlgYScaleSetUp::SetYScaleMaxMin( double dMax, double dMin )
{
	m_dYScaleMaxValue = dMax;
	m_dYScaleMinValue = dMin;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return BOOL	: 
// Param  double dMax : 
// Param  double dMin : 
// Comments		: 
//-----------------------------------------------------------------------------	
void CDlgYScaleSetUp::SetYScaleMaxMinPercent( double dMax, double dMin )
{
	m_dYScaleMaxPercentValue = dMax;
	m_dYScaleMinPercentValue = dMin;
}

void CDlgYScaleSetUp::On30Percent() 
{
	// TODO: Add your control notification handler code here
	m_nSelectPercent = 0;

	m_dYScaleMaxPercentValue = 30;
	m_dYScaleMinPercentValue = -30;
		
	m_strMaxPrice.Format("%.f", m_dYScaleMaxPercentValue);
	m_strMinPrice.Format("%.f", m_dYScaleMinPercentValue);

	UpdateData(FALSE);
}

void CDlgYScaleSetUp::On50Percent() 
{
	// TODO: Add your control notification handler code here
	m_nSelectPercent = 1;

	m_dYScaleMaxPercentValue = 50;
	m_dYScaleMinPercentValue = -50;
		
	m_strMaxPrice.Format("%.f", m_dYScaleMaxPercentValue);
	m_strMinPrice.Format("%.f", m_dYScaleMinPercentValue);

	UpdateData(FALSE);	
}

void CDlgYScaleSetUp::On70Percent() 
{
	// TODO: Add your control notification handler code here
	m_nSelectPercent = 2;

	m_dYScaleMaxPercentValue = 70;
	m_dYScaleMinPercentValue = -70;
		
	m_strMaxPrice.Format("%.f", m_dYScaleMaxPercentValue);
	m_strMinPrice.Format("%.f", m_dYScaleMinPercentValue);

	UpdateData(FALSE);	
}

void CDlgYScaleSetUp::On100Percent() 
{
	// TODO: Add your control notification handler code here
	m_nSelectPercent = 3;

	m_dYScaleMaxPercentValue = 100;
	m_dYScaleMinPercentValue = -100;
		
	m_strMaxPrice.Format("%.f", m_dYScaleMaxPercentValue);
	m_strMinPrice.Format("%.f", m_dYScaleMinPercentValue);

	UpdateData(FALSE);	
}

void CDlgYScaleSetUp::OnChangeMaxPriceEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	if( m_nYScaleMaxMinSetupType == 0)
		return;

	UpdateData();

	if( m_strMaxPrice == "30" && m_strMinPrice == "-30" )
		m_nSelectPercent = 0;
		
	else if( m_strMaxPrice == "50" && m_strMinPrice == "-50" )
		m_nSelectPercent = 1;

	else if( m_strMaxPrice == "70" && m_strMinPrice == "-70" )
		m_nSelectPercent = 2;

	else if( m_strMaxPrice == "100" && m_strMinPrice == "-100" )
		m_nSelectPercent = 3;
	
	else
		m_nSelectPercent = -1;

	UpdateData(FALSE);
}


void CDlgYScaleSetUp::OnChangeMinPriceEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
if( m_nYScaleMaxMinSetupType == 0)
		return;

	UpdateData();

	if( m_strMaxPrice == "30" && m_strMinPrice == "-30" )
		m_nSelectPercent = 0;
		
	else if( m_strMaxPrice == "50" && m_strMinPrice == "-50" )
		m_nSelectPercent = 1;

	else if( m_strMaxPrice == "70" && m_strMinPrice == "-70" )
		m_nSelectPercent = 2;

	else if( m_strMaxPrice == "100" && m_strMinPrice == "-100" )
		m_nSelectPercent = 3;
	
	else
		m_nSelectPercent = -1;

	UpdateData(FALSE);	
}
