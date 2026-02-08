// Setting_Special_Type_B.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Special_Type_B.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "GraphData.h"
#include "TabMainChartEnvSetUp.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"
#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_B dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_B::CSetting_Special_Type_B( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_Special_Type_B::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Special_Type_B)
	m_strBlockChange = _T("");
	//}}AFX_DATA_INIT
	m_nSubGraphIndex = -1;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_B::~CSetting_Special_Type_B()
{
}



void CSetting_Special_Type_B::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Special_Type_B)
	DDX_Control(pDX, IDC_THICKNESS, m_cmbThickness);
	DDX_Control(pDX, IDC_BTN_COLOR, m_btnColorBtn);
	DDX_Text(pDX, IDC_EDIT_BLOCKCHANGE, m_strBlockChange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_Special_Type_B, CBaseDialog)
	//{{AFX_MSG_MAP(CSetting_Special_Type_B)
	ON_BN_CLICKED(IDC_BTN_COLOR, OnBtnColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_B message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Special_Type_B::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_B::Initial(IGraphInfo *p_pIGraphInfo)
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;

	m_nSubGraphIndex = 0;

	m_btnColorBtn.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1) );
	m_btnColorBtn.Invalidate();

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if(pICalCnd)
	{
		POSITION psnCnd = pICalCnd->GetHeadPosition();
		if( psnCnd)
		{
			double dData = 0;
			pICalCnd->GetNext( psnCnd, dData);
			m_strBlockChange.Format("%.0f",dData);
		}
	}
	pICalCnd->Release();

	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
	// for Swing Chart
	if( !strcmp( szGraphName, _MTEXT( C2_SWING_CHART )))
	{
		CString strSwing;
		strSwing.LoadString(IDS_STRING_SWING_LINE);
		GetDlgItem( IDC_STC_COLOR)->SetWindowText( strSwing);
		GetDlgItem( IDC_STC_THICKNESS)->ShowWindow( SW_SHOW);
		m_cmbThickness.ShowWindow( SW_SHOW);
		// Line Thickness 초기화.
		Initial_ThicknessComboBox(m_nSubGraphIndex, 0, m_cmbThickness);
	}
	// for Kagi Chart
	else
	{
// 2008.01.25 by LYH >>
		//GetDlgItem( IDC_STC_COLOR)->SetWindowText( "카기선");
		CString strKagi;
		strKagi.LoadString(IDS_STRING_KAGI_LINE);
		GetDlgItem( IDC_STC_COLOR)->SetWindowText( strKagi);
// 2008.01.25 by LYH <<
		GetDlgItem( IDC_STC_THICKNESS)->ShowWindow( SW_HIDE);
		m_cmbThickness.ShowWindow( SW_HIDE);

		// 조건값 사용자 지정 가능 - ojtaso (20070913)
		CRect rcItem;

		CWnd* pItem = GetDlgItem( IDC_STATIC_BLOCKCHANGE);
		pItem->GetWindowRect(rcItem);
		ScreenToClient(rcItem);

		rcItem.OffsetRect(0, -24);
		pItem->MoveWindow(rcItem);

		pItem = GetDlgItem( IDC_EDIT_BLOCKCHANGE);
		pItem->GetWindowRect(rcItem);
		ScreenToClient(rcItem);

		rcItem.OffsetRect(0, -24);
		pItem->MoveWindow(rcItem);
	}

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Special_Type_B::OnApply( IGraphInfo* p_pIGraphInfo )
//void CSetting_Special_Type_B::OnApply( IGraphInfo* p_pIGraphInfo )
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pICalCnd)
	{
		POSITION psnCnd = pICalCnd->GetHeadPosition();
		if( psnCnd)
		{
			UpdateData();
			pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
		}
	}

	if( !CBaseDialog::m_bDoCalculate)
	{
		IDoubleList *pICalCndOrg = p_pIGraphInfo->GetCommonCalculateConditionList();
		if( pICalCndOrg)
		{
			CBaseDialog::m_bDoCalculate = !DoesEqualDoubleList( pICalCndOrg, pICalCnd);
			pICalCndOrg->Release();
		}
	}
	
	pICalCnd->Release();		

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	m_pIChartOCX->ReCalculateIndicator();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_B::OnBtnColor() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColorBtn.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
		m_btnColorBtn.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
		m_btnColorBtn.Invalidate();
	}
}

BOOL CSetting_Special_Type_B::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->OK();

				else
					( (CGraphSetting *)m_pParentWnd)->OK();
			}
		}

		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->Cancel();

				else
					( (CGraphSetting *)m_pParentWnd)->Cancel();
			}
		}
	}

	return CBaseDialog::PreTranslateMessage(pMsg);
}
