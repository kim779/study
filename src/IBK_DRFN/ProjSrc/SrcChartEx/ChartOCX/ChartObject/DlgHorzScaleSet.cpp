// DlgHorzScaleSet.cpp : implementation file
//

#include "stdafx.h"
#include "ChartObject.h"
#include "DlgHorzScaleSet.h"

#include "../Include_Chart/DLL_Load/XScaleManager.h"	// xScaleManager 추가 : xScaleManager - ojtaso (20070525)
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_LANGUAGE_RES
#include "../Include_Chart/Dll_Load/MainBlock.h"		// for CMainBlock
#include "../Include_Chart/MessageDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHorzScaleSet dialog

// xScaleManager 추가 : xScaleManager - ojtaso (20070525)
CDlgHorzScaleSet::CDlgHorzScaleSet(CWnd* pParent, CMainBlock *p_pMainBlock, CHorizontalScalePtrDataList* pHorzScalePtrDataList, CXScaleManager* lpxScaleManager)
	: CScaleSet(CDlgHorzScaleSet::IDD, pParent),
	m_pHorzScalePtrDataList(pHorzScalePtrDataList),
	m_baseHorzScaleDataList(pHorzScalePtrDataList),
	m_lpxScaleManager(lpxScaleManager)
{
	CScaleSet::SetAllScaleGroup(true);
	CScaleSet::SetScalePosition(3);
	//{{AFX_DATA_INIT(CDlgHorzScaleSet)
	m_strCompart_Date = _T("");
	m_strCompart_Time = _T("");
	m_nMarginCount = 0;
	m_bReverse = FALSE;
	//}}AFX_DATA_INIT
	m_nOrgMarginCount = 0;
	m_bOrgReverse = FALSE;

	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	m_pMainBlock = p_pMainBlock;
}


void CDlgHorzScaleSet::DoDataExchange(CDataExchange* pDX)
{
	CScaleSet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHorzScaleSet)
	DDX_CBString(pDX, IDC_COMPART_DATE, m_strCompart_Date);
	DDX_CBString(pDX, IDC_COMPART_TIME, m_strCompart_Time);
	DDX_Text(pDX, IDC_MARGIN, m_nMarginCount);
	DDV_MinMaxInt(pDX, m_nMarginCount, 0, 99999);
	DDX_Check(pDX, IDC_REVERSE, m_bReverse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHorzScaleSet, CDialog)
	//{{AFX_MSG_MAP(CDlgHorzScaleSet)
	ON_BN_CLICKED(IDC_APPLY_BT, OnApplyBt)
	ON_CBN_SELCHANGE(IDC_SCALEGROUP, OnSelchangeScalegroup)
	ON_BN_CLICKED(IDC_ALLSCALEGROUP_CHECK, OnAllscalegroupCheck)
	ON_BN_CLICKED(IDC_TEXT_COLOR, OnTextColor)
	ON_BN_CLICKED(IDC_GRID_COLOR, OnGridColor)
	ON_EN_CHANGE(IDC_MARGIN, OnChangeMargin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHorzScaleSet message handlers
/////////////////////////////////////////////////////////////////////////////
void CDlgHorzScaleSet::OnCancel() 
{
	// 원래의 값으로 복원한다.
	SetScaleColor(m_baseHorzScaleDataList.GetScaleGridColor(), m_baseHorzScaleDataList.GetScaleTextColor());
	for(int nIndex = 0; nIndex < m_baseHorzScaleDataList.GetCount(); nIndex++){
		m_pHorzScalePtrDataList->SetHorzScale(nIndex, m_baseHorzScaleDataList.GetAt(nIndex));
	}	

	if(m_lpxScaleManager) m_lpxScaleManager->SetReverse( m_bOrgReverse);
		
	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	CScaleSet::GetParent()->SendMessage( UM_SET_ETC, 0, m_nOrgMarginCount);

	CScaleSet::OnCancel();
}

// ----------------------------------------------------------------------------
void CDlgHorzScaleSet::OnChangeMargin() 
{
	int nOldMarginCount = m_nMarginCount;
	if(UpdateData(TRUE))
		return;

	// 정수가 아닐 경우에 이전값으로 바꾼다.
	m_nMarginCount = nOldMarginCount;
	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
// protected ==================================================================
void CDlgHorzScaleSet::InitializeData()
{
	InitializeData_ScaleGroup();
	InitializeData_Date();
	InitializeData_Time();
	InitializeData_Etc();
	UpdateData(FALSE);

	EnableWindowOnAllScaleGroup();
}

void CDlgHorzScaleSet::ApplyInEtcData()
{
	if(m_lpxScaleManager) m_lpxScaleManager->SetReverse( m_bReverse);

	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	CScaleSet::GetParent()->SendMessage( UM_SET_ETC, 0, m_nMarginCount);
}

// ----------------------------------------------------------------------------
// 가로 scale 에 값 적용.
void CDlgHorzScaleSet::SetAllScale()
{
	if(m_pHorzScalePtrDataList == NULL)
		return;

	for(int nIndex = 0; nIndex < m_pHorzScalePtrDataList->GetCount(); nIndex++){
		SetScale(nIndex);
	}
}

void CDlgHorzScaleSet::SetScale(const int nIndex)
{
	CHorizontalScale* pHorzScale = GetScale(nIndex);
	if(pHorzScale == NULL)
		return;

	if(!CScaleSet::IsAllScaleGroup())
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		if( m_pMainBlock) m_pMainBlock->SetHorzScalePosition( *pHorzScale, ( CScaleBaseData::HORZSCALEPOSITION)CScaleSet::GetScalePosition());
	}

	pHorzScale->SetGridType((CScaleBaseData::SCALEGRIDTYPE) CScaleSet::GetScaleGrid());
	pHorzScale->GetScaleCompart().SetDateCompart(m_strCompart_Date);
	pHorzScale->GetScaleCompart().SetTimeCompart(m_strCompart_Time);
}

void CDlgHorzScaleSet::SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor)
{
	m_pHorzScalePtrDataList->SetScaleColor(gridColor, textColor);
}

// ----------------------------------------------------------------------------
// 화면 변경
void CDlgHorzScaleSet::ChangeScaleWindow(const int nGroupIndex)
{
	CHorizontalScale* pHorzScale = GetScale(nGroupIndex);
	if(pHorzScale == NULL)
		return;

	m_strCompart_Date = pHorzScale->GetScaleCompart().GetDateCompart();
	m_strCompart_Time = pHorzScale->GetScaleCompart().GetTimeCompart();

	CScaleSet::ChangeScaleWindow(nGroupIndex, (int) pHorzScale->GetHorzScalePosition(), (int) pHorzScale->GetGridType());
}

// 모든 scale group에 적용 여부에 따른 Enable.
void CDlgHorzScaleSet::EnableWindowOnAllScaleGroup()
{
	CScaleSet::EnableWindowOnAllScaleGroup();
	// scale position
	CScaleSet::EnableWindow(GetDlgItem(IDC_SCALEPOSITION));
	CScaleSet::EnableWindow(GetDlgItem(IDC_SCALEPOSITION1));
	CScaleSet::EnableWindow(GetDlgItem(IDC_SCALEPOSITION2));
	CScaleSet::EnableWindow(GetDlgItem(IDC_SCALEPOSITION3));
	CScaleSet::EnableWindow(GetDlgItem(IDC_SCALEPOSITION4));
}


// private ====================================================================
// data 초기화
void CDlgHorzScaleSet::InitializeData_ScaleGroup()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_SCALEGROUP);
	if(pComboBox == NULL || m_pHorzScalePtrDataList == NULL)
		return;

	for(int nIndex = 0; nIndex < m_pHorzScalePtrDataList->GetCount(); nIndex++){
		pComboBox->AddString(m_pHorzScalePtrDataList->GetAt(nIndex).GetHorzScaleTitle());
	}

	ChangeScaleWindow(m_pHorzScalePtrDataList->GetFirstIndex());
}

void CDlgHorzScaleSet::InitializeData_Date()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMPART_DATE);
	if(pComboBox == NULL)
		return;

	pComboBox->AddString(".");
	pComboBox->AddString("-");
	pComboBox->AddString(":");
	pComboBox->AddString("/");

	ML_SET_LANGUAGE_RES();
	CString strComparts;
	strComparts.LoadString( IDS_YEAR_MONTH_DAY);
	pComboBox->AddString( strComparts);
	
	InitializeData_Compart(pComboBox, m_strCompart_Date);
}

void CDlgHorzScaleSet::InitializeData_Time()
{
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COMPART_TIME);
	if(pComboBox == NULL)
		return;

	pComboBox->AddString(".");
	pComboBox->AddString("-");
	pComboBox->AddString(":");
	pComboBox->AddString("/");

	ML_SET_LANGUAGE_RES();
	CString strComparts;
	strComparts.LoadString( IDS_HOUR_MINUTE_SECOND);
	pComboBox->AddString( strComparts);

	InitializeData_Compart(pComboBox, m_strCompart_Time);
}

void CDlgHorzScaleSet::InitializeData_Compart(CComboBox* pComboBox, const CString& currentComboBoxString)
{
	if(pComboBox == NULL)
		return;

	if(pComboBox->FindStringExact(0, currentComboBoxString) == LB_ERR)
		pComboBox->AddString(currentComboBoxString);
}

void CDlgHorzScaleSet::InitializeData_Etc()
{
	// scale color
	CScaleSet::SetScaleColorInButton(m_baseHorzScaleDataList.GetScaleGridColor(), m_baseHorzScaleDataList.GetScaleTextColor());
	// margin
	if(m_pHorzScalePtrDataList == NULL)
		return;
	
	if(m_lpxScaleManager)
	{
		// (2009/5/13 - Seung-Won, Bae) for Right Margin
		m_nOrgMarginCount = m_nMarginCount = CScaleSet::GetParent()->SendMessage( UM_SET_ETC, 0, -1);

		m_bOrgReverse = m_bReverse = m_lpxScaleManager->GetReverse();
	}
	else
		m_nMarginCount = m_pHorzScalePtrDataList->GetMarginCount();

	// (2009/5/18 - Seung-Won, Bae) Disable X scale margin on special chart.
	if( CScaleSet::GetParent()->SendMessage( UM_SET_ETC, 2, 0))
		GetDlgItem( IDC_MARGIN)->EnableWindow( FALSE);
}

// ----------------------------------------------------------------------------
void CDlgHorzScaleSet::SendMessageToParent(const int nMarginCount)
{
	if(CScaleSet::GetParent() == NULL)
		return;

	CScaleSet::GetParent()->SendMessage(UM_SET_ETC, 0, nMarginCount);
}

// ----------------------------------------------------------------------------
// 해당 index의 horzScale 가져오기.
CHorizontalScale* CDlgHorzScaleSet::GetScale(const int nIndex)
{
	if(m_pHorzScalePtrDataList == NULL || nIndex < 0 || nIndex >= m_pHorzScalePtrDataList->GetCount())
		return NULL;

	return m_pHorzScalePtrDataList->GetAt(nIndex).GetHorzScale();
}
