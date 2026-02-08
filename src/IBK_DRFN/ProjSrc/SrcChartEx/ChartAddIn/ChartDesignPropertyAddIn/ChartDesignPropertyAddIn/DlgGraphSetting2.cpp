// DlgGraphSetting2.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgGraphSetting2.h"

#include "LineColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting2 dialog


CDlgGraphSetting2::CDlgGraphSetting2(CGraphSetting2Data* pGraphSetting2Data, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSetting2::IDD, pParent), 
	m_pGraphSetting2Data(pGraphSetting2Data)
{
	//{{AFX_DATA_INIT(CDlgGraphSetting2)
	m_bCurData = FALSE;
	m_bHighNow = FALSE;
	m_bLowHigh = FALSE;
	m_bLowNow = FALSE;
	m_bPrevNow = FALSE;
	m_nCurDataType = -1;
	//}}AFX_DATA_INIT
}


void CDlgGraphSetting2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSetting2)
	DDX_Check(pDX, IDC_CUR_DATA, m_bCurData);
	DDX_Check(pDX, IDC_HIGH_NOW_UPDOWN, m_bHighNow);
	DDX_Check(pDX, IDC_LOW_HIGH_UPDOWN, m_bLowHigh);
	DDX_Check(pDX, IDC_LOW_NOW_UPDOWN, m_bLowNow);
	DDX_Check(pDX, IDC_PRE_NOW_CONTRAST, m_bPrevNow);
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	DDX_Radio(pDX, IDC_CUR_PRICE_1, m_nCurDataType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSetting2, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSetting2)
	ON_BN_CLICKED(IDC_CUR_DATA, OnCurData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting2 message handlers

BOOL CDlgGraphSetting2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGraphSetting2::OnCurData() 
{
	UpdateData(TRUE);
	EnableWindow_CurDataType(m_bCurData == TRUE ? true: false);
}

void CDlgGraphSetting2::OnOK() 
{
	SetGraphSetting2Data();	
	CDialog::OnOK();
}

void CDlgGraphSetting2::OnCancel() 
{
	CDialog::OnCancel();
}

// public =====================================================================
void CDlgGraphSetting2::SetGraphSetting2Data(CGraphSetting2Data* pGraphSetting2Data)
{
	m_pGraphSetting2Data = pGraphSetting2Data;
}

// private ====================================================================
bool CDlgGraphSetting2::SetDlgItemData()
{
	if(m_pGraphSetting2Data == NULL)
		return false;

	m_bHighNow = m_pGraphSetting2Data->IsHighNow();
	m_bLowHigh = m_pGraphSetting2Data->IsLowHigh();
	m_bLowNow = m_pGraphSetting2Data->IsLowNow();
	m_bPrevNow = m_pGraphSetting2Data->IsPrevNow();
	SetDlgItemData_CurDataType(m_pGraphSetting2Data->GetCurDataType());

	UpdateData(FALSE);
	return true;
}

void CDlgGraphSetting2::SetDlgItemData_CurDataType(const int nCurDataType)
{
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	if( 0 < nCurDataType && nCurDataType <= 4)
// 	{
// 		m_nCurDataType = nCurDataType - 1;
// 		m_bCurData = true;
// 		EnableWindow_CurDataType(true);
// 	}
// 	else
// 	{
// 		m_nCurDataType = 0;
// 		m_bCurData = false;
// 		EnableWindow_CurDataType(false);
// 	}

	int cur = 0;
	switch( nCurDataType - 1 ){
	case 0: cur = 2; break;
	case 1: cur = 3; break;
	case 2: cur = 4; break;
	case 3: cur = 5; break;
	case 4: cur = 0; break;
	case 5: cur = 1; break;
	case 6: cur = 6; break;
	case 7: cur = 7; break;
	default: cur = 0; break;
	}

	m_nCurDataType = cur;

	m_bCurData = ( nCurDataType ? TRUE : FALSE );

	UpdateData(FALSE);

	EnableWindow_CurDataType(m_bCurData ? true : false);
}

bool CDlgGraphSetting2::SetGraphSetting2Data()
{
	if(m_pGraphSetting2Data == NULL)
		return false;

	UpdateData(TRUE);

	m_pGraphSetting2Data->SetHighNow(m_bHighNow == TRUE ? true: false);
	m_pGraphSetting2Data->SetLowHigh(m_bLowHigh == TRUE ? true: false);
	m_pGraphSetting2Data->SetLowNow(m_bLowNow == TRUE ? true: false);
	m_pGraphSetting2Data->SetPrevNow(m_bPrevNow == TRUE ? true: false);

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	if(m_bCurData)
// 		m_pGraphSetting2Data->SetCurDataType(m_nCurDataType + 1);
// 	else
// 		m_pGraphSetting2Data->SetCurDataType(0);

	if( m_bCurData ){
		int cur = 0;
		switch( m_nCurDataType ){
		case 0: cur = 4; break;
		case 1: cur = 5; break;
		case 2: cur = 0; break;
		case 3: cur = 1; break;
		case 4: cur = 2; break;
		case 5: cur = 3; break;
		case 6: cur = 6; break;
		case 7: cur = 7; break;
		default: cur = 0; break;
		}
		m_pGraphSetting2Data->SetCurDataType(cur + 1);
	}
	else {
		m_pGraphSetting2Data->SetCurDataType(0);
	}

	return true;
}

void CDlgGraphSetting2::EnableWindow_CurDataType(const bool bEnable)
{
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	EnableWindow(IDC_CUT_DATA_PREV, bEnable);
// 	EnableWindow(IDC_CUT_DATA_VIEW, bEnable);
// 	EnableWindow(IDC_CUT_DATA_CLOSE, bEnable);
// 	EnableWindow(IDC_CUT_DATA_VIEW_ONLY, bEnable);

	EnableWindow(IDC_CUR_PRICE_1, bEnable);
	EnableWindow(IDC_CUR_PRICE_2, bEnable);
	EnableWindow(IDC_CUR_PRICE_3, bEnable);
	EnableWindow(IDC_CUR_PRICE_4, bEnable);
	EnableWindow(IDC_CUR_PRICE_5, bEnable);
	EnableWindow(IDC_CUR_PRICE_6, bEnable);
	EnableWindow(IDC_CUR_PRICE_7, bEnable);
	EnableWindow(IDC_CUR_PRICE_8, bEnable);
}

void CDlgGraphSetting2::EnableWindow(const int nID, const bool bEnable)
{
	CWnd* pWnd = (CWnd*) GetDlgItem(nID);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
}
