// DlgVertScaleSet.cpp : implementation file
//

#include "stdafx.h"
#include "ChartObject.h"
#include "DlgVertScaleSet.h"

#include <Math.h>										// for HUGE_VAL
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVertScaleSet dialog


// 호가단위, 기준가 사용 - ojtaso (20070917)
// 20081025 이주표 strType추가
CDlgVertScaleSet::CDlgVertScaleSet( CWnd* pParent, CVerticalScalePtrDataList* pVertScalePtrDataList, BOOL bHogaUnit)
	: CScaleSet(CDlgVertScaleSet::IDD, pParent),
	m_pVertScalePtrDataList(pVertScalePtrDataList),
	m_baseVertScaleDataList(pVertScalePtrDataList),
	m_bHogaUnit(bHogaUnit)
{
	CScaleSet::SetScalePosition(1);
	//{{AFX_DATA_INIT(CDlgVertScaleSet)
	m_nScaleUnitType = 0;
	m_bIsInvert = FALSE;
	m_bIsLog = FALSE;
	m_bMinMaxLock = FALSE;
	m_nMinMaxType = 0;
	m_dHogaNumber = 0.0;
	m_dUserNumber = 0.0;
	m_nUserHogaType = 0;
	m_nUpSpace = 0;
	m_nDownSpace = 0;
	//}}AFX_DATA_INIT

	// (2007/7/16 - Seung-Won, Bae) Do not show the INF Value in UI.
	m_dMax = 0.0;
	m_dMin = 0.0;
	m_strMin = "";	// 20081015 이주표
	m_strMax = "";	// 20081015 이주표
}


void CDlgVertScaleSet::DoDataExchange(CDataExchange* pDX)
{

// (2007/7/16 - Seung-Won, Bae) Check Invalid Value.
BOOL bException = FALSE;
TRY
{

	CScaleSet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVertScaleSet)	
	DDX_Radio(pDX, IDC_SCALEUNITTYPE, m_nScaleUnitType);
	DDX_Check(pDX, IDC_INVERT, m_bIsInvert);
	DDX_Check(pDX, IDC_LOG, m_bIsLog);
	DDX_Check(pDX, IDC_CHK_LOCK, m_bMinMaxLock);
	DDX_Radio(pDX, IDC_RDO_MINMAX_TYPE0, m_nMinMaxType);
	DDX_Text(pDX, IDC_HOGASCALE_EDIT, m_dHogaNumber);
	DDX_Text(pDX, IDC_USERSCALE_EDIT, m_dUserNumber);
	DDX_Radio(pDX, IDC_RDO_USERHOGATYPE0, m_nUserHogaType);
	DDX_Text(pDX, IDC_EDT_DSPACE, m_nDownSpace);
	DDV_MinMaxInt(pDX, m_nDownSpace, 0, 90);
	DDX_Text(pDX, IDC_EDT_USPACE, m_nUpSpace);
	DDV_MinMaxInt(pDX, m_nUpSpace, 0, 90);
	//}}AFX_DATA_MAP
}
AND_CATCH_ALL(e)
{
	// validation failed due to OOM or other resource failure
	bException = TRUE;
}
END_CATCH_ALL

	// (2007/7/16 - Seung-Won, Bae) Do not show the INF Value in UI.
/*	if( pDX->m_bSaveAndValidate || ( -HUGE_VAL != m_dMax && -HUGE_VAL != m_dMin))
	{
		DDX_Text(pDX, IDC_EDT_MAX, m_dMax);
		DDX_Text(pDX, IDC_EDT_MIN, m_dMin);
	}
*/

	// (2007/7/16 - Seung-Won, Bae) Support Up/Down Space.
	int nPreUpSpace		= m_nUpSpace;
	int nPreDownSpace	= m_nDownSpace;
	if( m_nUpSpace < 0)		m_nUpSpace = 0;
	if( 90 < m_nUpSpace)	m_nUpSpace = 90;
	if( m_nDownSpace < 0)	m_nDownSpace = 0;
	if( 90 < m_nDownSpace)	m_nDownSpace = 90;
	CheckValue( nPreUpSpace,	m_nUpSpace,		IDC_EDT_USPACE);
	CheckValue( nPreDownSpace,	m_nDownSpace,	IDC_EDT_DSPACE);
	if( 90 < m_nUpSpace + m_nDownSpace)
	{
		double dFactor = 90. / ( m_nUpSpace + m_nDownSpace);
		m_nUpSpace		= int( m_nUpSpace	* dFactor);
		m_nDownSpace	= int( m_nDownSpace	* dFactor);

		ML_SET_LANGUAGE_RES();
		CString strMsg;
		strMsg.LoadString( IDS_MARGIN_LIMIT);
		AfxMessageBox( strMsg);
		bException = TRUE;
	}
	CheckValue( nPreUpSpace,	m_nUpSpace,		IDC_EDT_USPACE);
	CheckValue( nPreDownSpace,	m_nDownSpace,	IDC_EDT_DSPACE);

	if( bException) AfxThrowUserException();
}


void CDlgVertScaleSet::CheckValue( int p_nPreSpace, int p_nNewSpace, int p_nID)
{
	if( p_nPreSpace != p_nNewSpace)
	{
		CString strData;
		strData.Format( "%d", p_nNewSpace);
		GetDlgItem( p_nID)->SetWindowText( strData);
	}
}


BEGIN_MESSAGE_MAP(CDlgVertScaleSet, CDialog)
	//{{AFX_MSG_MAP(CDlgVertScaleSet)
	ON_BN_CLICKED(IDC_APPLY_BT, OnApplyBt)
	ON_BN_CLICKED(IDC_ALLSCALEGROUP_CHECK, OnAllscalegroupCheck)
	ON_CBN_SELCHANGE(IDC_SCALEGROUP, OnSelchangeScalegroup)
	ON_BN_CLICKED(IDC_GRID_COLOR, OnGridColor)
	ON_BN_CLICKED(IDC_TEXT_COLOR, OnTextColor)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RDO_MINMAX_TYPE0, IDC_RDO_MINMAX_TYPE2, OnRdoMinmaxType)
	ON_CONTROL_RANGE( BN_CLICKED, IDC_RDO_USERHOGATYPE0, IDC_RDO_USERHOGATYPE1, OnRdoUserHogaType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVertScaleSet message handlers
/////////////////////////////////////////////////////////////////////////////
void CDlgVertScaleSet::OnCancel() 
{
	// 원래의 값으로 복원한다.
	SetScaleColor( m_baseVertScaleDataList.GetScaleGridColor(), m_baseVertScaleDataList.GetScaleTextColor());
	SetScaleType( m_baseVertScaleDataList.IsScaleLogType());
	for( int nIndex = 0; nIndex < m_baseVertScaleDataList.GetCount(); nIndex++)
		m_pVertScalePtrDataList->SetVertScale(nIndex, m_baseVertScaleDataList.GetAt(nIndex));

	CScaleSet::OnCancel();
}


///////////////////////////////////////////////////////////////////////////////
// protected ==================================================================
void CDlgVertScaleSet::InitializeData()
{
	InitializeData_ScaleGroup();
	InitializeData_ScaleType();
	InitializeData_Etc();
	UpdateData(FALSE);

	EnableWindowOnAllScaleGroup();
}

void CDlgVertScaleSet::ApplyInEtcData()
{
	SetScaleType( m_bIsLog ? true : false);
}

// ----------------------------------------------------------------------------
void CDlgVertScaleSet::SetAllScale()
{
	if(m_pVertScalePtrDataList == NULL)
		return;

	int nIndex = 0;
	for( nIndex = 0; nIndex < m_pVertScalePtrDataList->GetCount(); nIndex++)
		UpdateScale( nIndex);

	// (2007/7/13 - Seung-Won, Bae) Set Activate.
	SetActiveVertcialScale( nIndex);
}

void CDlgVertScaleSet::SetScale(const int nIndex)
{
	// (2007/7/13 - Seung-Won, Bae) Set Activate.
	UpdateScale( nIndex);
	SetActiveVertcialScale( nIndex);
}

void CDlgVertScaleSet::UpdateScale(const int nIndex)
{
	CVerticalScale* pVertScale = GetVertScale(nIndex);
	if(pVertScale == NULL)
		return;

	pVertScale->SetVertScalePosition((CScaleBaseData::VERTSCALEPOSITION) CScaleSet::GetScalePosition());
	pVertScale->SetGridType((CScaleBaseData::SCALEGRIDTYPE) CScaleSet::GetScaleGrid());
	pVertScale->SetVertScaleUnitType((CScaleBaseData::VERTSCALEUNITTYPE) m_nScaleUnitType);

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	pVertScale->SetVertScaleMinMaxLock(	m_bMinMaxLock);
	pVertScale->SetVertScaleMinMaxType(	( CScaleBaseData::VERTMINMAXTYPE)m_nMinMaxType);
	// 20081015 이주표 >>
	CString strMin, strMax;
	BOOL bSuccess;
	GetDlgItem(IDC_EDT_MIN)->GetWindowText(m_strMin);
	GetDlgItem(IDC_EDT_MAX)->GetWindowText(m_strMax);
	bSuccess = CDataConversion::GetDataToDec(m_strMin, m_pVertScalePtrDataList->GetAt( nIndex).GetVertScalePacketType(), strMin);
	if(!bSuccess)
		strMin = "0.0";
	bSuccess = CDataConversion::GetDataToDec(m_strMax, m_pVertScalePtrDataList->GetAt( nIndex).GetVertScalePacketType(), strMax);
	if(!bSuccess)
		strMax = "0.0";
	m_dMin = atof(strMin);
	m_dMax = atof(strMax);
	// 20081015 이주표 <<
	pVertScale->SetVertScaleMin(		m_dMin);
	pVertScale->SetVertScaleMax(		m_dMax);
	//2007.01.22 Scale사용자 설정,호가단위별
	pVertScale->SetVertScaleHoGaNumber(m_dHogaNumber);
	pVertScale->SetVertScaleUserNumber(m_dUserNumber);
	pVertScale->SetVertUserHogaScaleType(m_nUserHogaType);

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	pVertScale->SetSpace( m_nUpSpace, m_nDownSpace);

	// 지표 뒤집기 - ojtaso (20071023)
	pVertScale->SetVertScaleInvertType(m_bIsInvert ? true : false);
}

// (2007/7/13 - Seung-Won, Bae) Set Activate.
void CDlgVertScaleSet::SetActiveVertcialScale( const int p_nIndex)
{
	// 1. Get Activate Code
	CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = ( CScaleBaseData::VERTSCALEPOSITION)CScaleSet::GetScalePosition();
	int nActiveFlag = 0;
	switch( eVertScalePosition)
	{
		case CScaleBaseData::VERT_LEFT:		nActiveFlag = VSGA_LEFTSCALE;						break;
		case CScaleBaseData::VERT_RIGHT:	nActiveFlag = VSGA_RIGHTSCALE;						break;
		case CScaleBaseData::VERT_BOTH:		nActiveFlag = VSGA_LEFTSCALE | VSGA_RIGHTSCALE;	break;
	}
	if( nActiveFlag) nActiveFlag |= VSGA_GRIDLINE;

	// 2. Reset and Remove Flags.
	for( int nIndex = 0; nIndex < m_pVertScalePtrDataList->GetCount(); nIndex++)
	{
		CVerticalScale* pVertScale = GetVertScale( nIndex);
		if( !pVertScale) continue;

		if( p_nIndex == nIndex) pVertScale->ResetActiveFlag( nActiveFlag);
		else					pVertScale->RemoveActiveFlag( nActiveFlag);
	}
}

void CDlgVertScaleSet::SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor)
{
	m_pVertScalePtrDataList->SetScaleColor(gridColor, textColor);
}

// ----------------------------------------------------------------------------
// 화면 변경
void CDlgVertScaleSet::ChangeScaleWindow(const int nGroupIndex)
{
	CVerticalScale* pVertScale = GetVertScale(nGroupIndex);
	if(pVertScale == NULL)
		return;

	// scale unit type
	m_nScaleUnitType = pVertScale->GetVertScaleUnitType();

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	m_bMinMaxLock	= pVertScale->IsVertScaleMinMaxLock();
	m_nMinMaxType	= pVertScale->GetVertScaleMinMaxType();
	// 20081015 이주표 8진법 32진법 변환 >>
	m_strMin = CDataConversion::DoubleToString(pVertScale->GetVertScaleMin(), m_pVertScalePtrDataList->GetAt( nGroupIndex).GetVertScalePacketType());
	m_strMax = CDataConversion::DoubleToString(pVertScale->GetVertScaleMax(), m_pVertScalePtrDataList->GetAt( nGroupIndex).GetVertScalePacketType());	
//	m_dMin			= pVertScale->GetVertScaleMin();
//	m_dMax			= pVertScale->GetVertScaleMax();
	GetDlgItem(IDC_EDT_MIN)->SetWindowText(m_strMin);
	GetDlgItem(IDC_EDT_MAX)->SetWindowText(m_strMax);
	// 20081015 이주표 <<

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	pVertScale->GetSpace( m_nUpSpace, m_nDownSpace);

	// 지표 뒤집기 - ojtaso (20071023)
	m_bIsInvert = pVertScale->GetVertScaleInvertType();

	CScaleSet::ChangeScaleWindow(nGroupIndex, (int)pVertScale->GetVertScalePosition(), (int)pVertScale->GetGridType());

	OnRdoMinmaxType( m_nMinMaxType == 2 ? IDC_RDO_MINMAX_TYPE2 : 0);

	//2007.01.22 Scale사용자 설정,호가단위별
	m_dHogaNumber = pVertScale->GetVertScaleHoGaNumber();
	m_dUserNumber = pVertScale->GetVertScaleUserNumber();
	m_nUserHogaType = pVertScale->GetVertUserHogaScaleType();

	if(SetEnableHogaUnit(nGroupIndex))
		OnRdoUserHogaType(m_nUserHogaType == 0 ? IDC_RDO_USERHOGATYPE0 : IDC_RDO_USERHOGATYPE1);
}


// private ==================================================================
void CDlgVertScaleSet::InitializeData_ScaleGroup()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_SCALEGROUP);
	if(pComboBox == NULL || m_pVertScalePtrDataList == NULL)
		return;

	for(int nIndex = 0; nIndex < m_pVertScalePtrDataList->GetCount(); nIndex++){
		pComboBox->AddString(m_pVertScalePtrDataList->GetAt(nIndex).GetVertScaleTitle());
	}

	ChangeScaleWindow(m_pVertScalePtrDataList->GetFirstIndex());
}

// 호가단위, 기준가 사용 - ojtaso (20070917)
BOOL CDlgVertScaleSet::SetEnableHogaUnit(const int nGroupIndex)
{
	CVerticalScale* pVertScale = GetVertScale(nGroupIndex);
	if(pVertScale == NULL)
		return TRUE;

	CString strScalePacketName = pVertScale->GetScaleData();
	BOOL bEnable = (strScalePacketName == _MTEXT( C6_INDICATOR_DATA) || 
					strScalePacketName == _MTEXT( C0_TEXT) ||
					strScalePacketName == _MTEXT( C6_PERCENT));

	GetDlgItem(IDC_RDO_USERHOGATYPE0)->EnableWindow(bEnable);
	GetDlgItem(IDC_USERSCALE_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_USERHOGATYPE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_HOGASCALE_EDIT)->EnableWindow(bEnable);

	return bEnable;
}

void CDlgVertScaleSet::InitializeData_ScaleType()
{
	m_bIsLog = m_baseVertScaleDataList.IsScaleLogType();

	// log scale enable -> "가격차트"가 있을 경우만 log 보임!!
	if(!m_pVertScalePtrDataList->IsEnableLogType())
		CScaleSet::EnableWindow(GetDlgItem(IDC_LOG), false);
}

void CDlgVertScaleSet::InitializeData_Etc()
{
	// scale color
	CScaleSet::SetScaleColorInButton(m_baseVertScaleDataList.GetScaleGridColor(), m_baseVertScaleDataList.GetScaleTextColor());
}

void CDlgVertScaleSet::SetScaleType( const bool bIsLog)
{
	if(m_pVertScalePtrDataList == NULL)
		return;

	m_pVertScalePtrDataList->SetLogType(bIsLog);
}


// ----------------------------------------------------------------------------
// 해당 index의 vertScale 가져오기.
CVerticalScale* CDlgVertScaleSet::GetVertScale(const int nIndex)
{
	if(m_pVertScalePtrDataList == NULL || nIndex < 0 || nIndex >= m_pVertScalePtrDataList->GetCount())
		return NULL;

	return m_pVertScalePtrDataList->GetAt(nIndex).GetVertScale();
}

// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
void CDlgVertScaleSet::EnableWindowOnAllScaleGroup( void)
{
	CScaleSet::EnableWindowOnAllScaleGroup();

	if( IsAllScaleGroup() && m_nMinMaxType == 2)
	{
		( ( CButton *)GetDlgItem( IDC_RDO_MINMAX_TYPE0))->SetCheck( 1);
		( ( CButton *)GetDlgItem( IDC_RDO_MINMAX_TYPE2))->SetCheck( 0);
		OnRdoMinmaxType( 0);
	}

	EnableWindow( GetDlgItem( IDC_RDO_MINMAX_TYPE2));
}

// (2006/11/11 - Seung-Won, Bae) Min/Max Type Changed.
void CDlgVertScaleSet::OnRdoMinmaxType( UINT nID) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_EDT_MIN)->EnableWindow( nID == IDC_RDO_MINMAX_TYPE2);
	GetDlgItem( IDC_EDT_MAX)->EnableWindow( nID == IDC_RDO_MINMAX_TYPE2);

	// (2007/7/16 - Seung-Won, Bae) Support Up/Down Space.
	GetDlgItem( IDC_EDT_USPACE)->EnableWindow( nID != IDC_RDO_MINMAX_TYPE2);
	GetDlgItem( IDC_EDT_DSPACE)->EnableWindow( nID != IDC_RDO_MINMAX_TYPE2);
}
//2007.01.22 Scale사용자 설정,호가단위별
void CDlgVertScaleSet::OnRdoUserHogaType( UINT nID) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_USERSCALE_EDIT)->EnableWindow( nID == IDC_RDO_USERHOGATYPE0);
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	GetDlgItem( IDC_HOGASCALE_EDIT)->EnableWindow( nID == IDC_RDO_USERHOGATYPE1 && m_bHogaUnit);
}
