// Setting_Special_Type_D.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Special_Type_D.h"

#include "GraphData.h"
#include "TabMainChartEnvSetUp.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_D dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_D::CSetting_Special_Type_D( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd )
	: CBaseDialog(CSetting_Special_Type_D::IDD, pParent, p_pIChartOCX)
{
	//{{AFX_DATA_INIT(CSetting_Special_Type_D)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSubGraphIndex = -1;
}


void CSetting_Special_Type_D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Special_Type_D)
	DDX_Control(pDX, IDC_BTN_COLOR_START,		m_btnColorStart);
	DDX_Control(pDX, IDC_BTN_COLOR_END,			m_btnColorEnd);
	DDX_Control(pDX, IDC_BTN_COLOR_TURN_UP,		m_btnColorTurnUp);
	DDX_Control(pDX, IDC_BTN_COLOR_BUY,			m_btnColorBuy);
	DDX_Control(pDX, IDC_BTN_COLOR_KEEP_BUY,	m_btnColorKeepBuy);
	DDX_Control(pDX, IDC_BTN_COLOR_HOLD_BUY,	m_btnColorHoldBuy);
	DDX_Control(pDX, IDC_BTN_COLOR_TURN_DOWN,	m_btnColorTurnDown);
	DDX_Control(pDX, IDC_BTN_COLOR_SELL,		m_btnColorSell);
	DDX_Control(pDX, IDC_BTN_COLOR_KEEP_SELL,	m_btnColorKeepSell);
	DDX_Control(pDX, IDC_BTN_COLOR_HOLD_SELL,	m_btnColorHoldSell);
	DDX_Control(pDX, IDC_THICKNESS, m_cmbThickness);
	DDX_Control(pDX, IDC_SPIN1, m_spnCond1);
	DDX_Control(pDX, IDC_EDIT1, m_edtCond1);
	//}}AFX_DATA_MAP
}

// (2007/5/3 - Seung-Won, Bae) Support 8 Range Color.
#define _COLOR_BTN_ID_START	20000
BEGIN_MESSAGE_MAP(CSetting_Special_Type_D, CDialog)
	//{{AFX_MSG_MAP(CSetting_Special_Type_D)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE( BN_CLICKED, _COLOR_BTN_ID_START, _COLOR_BTN_ID_START + 9, OnBtnColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_D message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Special_Type_D::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// (2007/5/3 - Seung-Won, Bae) Support 8 Range Color.
	m_btnColorStart.SetDlgCtrlID(		_COLOR_BTN_ID_START + 0);
	m_btnColorEnd.SetDlgCtrlID(			_COLOR_BTN_ID_START + 1);
	m_btnColorTurnUp.SetDlgCtrlID(		_COLOR_BTN_ID_START + 2);
	m_btnColorBuy.SetDlgCtrlID(			_COLOR_BTN_ID_START + 3);
	m_btnColorKeepBuy.SetDlgCtrlID(		_COLOR_BTN_ID_START + 4);
	m_btnColorHoldBuy.SetDlgCtrlID(		_COLOR_BTN_ID_START + 5);
	m_btnColorTurnDown.SetDlgCtrlID(	_COLOR_BTN_ID_START + 6);
	m_btnColorSell.SetDlgCtrlID(		_COLOR_BTN_ID_START + 7);
	m_btnColorKeepSell.SetDlgCtrlID(	_COLOR_BTN_ID_START + 8);
	m_btnColorHoldSell.SetDlgCtrlID(	_COLOR_BTN_ID_START + 9);

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
void CSetting_Special_Type_D::Initial(IGraphInfo *p_pIGraphInfo)
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

	// (2007/5/3 - Seung-Won, Bae) Support 8 Range Color.
	m_btnColorStart.SetColor(		m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	m_btnColorEnd.SetColor(			m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 2));
	m_btnColorTurnUp.SetColor(		m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	m_btnColorBuy.SetColor(			m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 4));
	m_btnColorKeepBuy.SetColor(		m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
	m_btnColorHoldBuy.SetColor(		m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 6));
	double dColor = 0;
	POSITION posColor = NULL;
	IDoubleList *pIDrawCndList = m_pIGraphInfo->GetDrawConditionList();
	posColor = ( pIDrawCndList ? pIDrawCndList->FindIndex( 0) : NULL);
	if( posColor) pIDrawCndList->GetAt( posColor, dColor);	else dColor = 0;
	m_btnColorTurnDown.SetColor( ( long)dColor);
	posColor = ( pIDrawCndList ? pIDrawCndList->FindIndex( 1) : NULL);
	if( posColor) pIDrawCndList->GetAt( posColor, dColor);	else dColor = 0;
	m_btnColorSell.SetColor( ( long)dColor);
	posColor = ( pIDrawCndList ? pIDrawCndList->FindIndex( 2) : NULL);
	if( posColor) pIDrawCndList->GetAt( posColor, dColor);	else dColor = 0;
	m_btnColorKeepSell.SetColor( ( long)dColor);
	posColor = ( pIDrawCndList ? pIDrawCndList->FindIndex( 3) : NULL);
	if( posColor) pIDrawCndList->GetAt( posColor, dColor);	else dColor = 0;
	m_btnColorHoldSell.SetColor( ( long)dColor);
	if( pIDrawCndList) pIDrawCndList->Release();

	m_btnColorStart.Invalidate();
	m_btnColorEnd.Invalidate();
	m_btnColorTurnUp.Invalidate();
	m_btnColorTurnDown.Invalidate();
	m_btnColorSell.Invalidate();
	m_btnColorBuy.Invalidate();
	m_btnColorKeepSell.Invalidate();
	m_btnColorKeepBuy.Invalidate();
	m_btnColorHoldSell.Invalidate();
	m_btnColorHoldBuy.Invalidate();

	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( !pIDblCnd) return;
	double dData = 0;
	pIDblCnd->GetHead( dData);
	pIDblCnd->Release();

	Initial_Spin( m_spnCond1, m_edtCond1, dData);
	Initial_ThicknessComboBox(m_nSubGraphIndex, 0, m_cmbThickness);

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Special_Type_D::OnApply( IGraphInfo* p_pIGraphInfo )
//void CSetting_Special_Type_D::OnApply( IGraphInfo* p_pIGraphInfo )
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL) return FALSE;
	
	UpdateData();

	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( pIDblCnd)
	{
		double dData = 0;
		POSITION psnCnd = pIDblCnd->GetHeadPosition();
		if( psnCnd)
		{
			CString szConditionData;
			m_edtCond1.GetWindowText( szConditionData);
			double dNewData = atof( szConditionData);

			pIDblCnd->GetAt( psnCnd, dData);
			if( !m_bDoCalculate ) m_bDoCalculate = ( dData != dNewData);
			pIDblCnd->SetAt( psnCnd, dNewData);
		}
		pIDblCnd->Release();
	}

	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}

BOOL CSetting_Special_Type_D::PreTranslateMessage(MSG* pMsg) 
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
	
	return CDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// (2007/5/3 - Seung-Won, Bae) Support 8 Range Color.
/////////////////////////////////////////////////////////////////////////////
void CSetting_Special_Type_D::OnBtnColor( UINT nID) 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	CColorButton *pColorButton = ( CColorButton *)GetDlgItem( nID);
	if( !pColorButton) return;

	COLORREF crColor = pColorButton->GetColor();
	CColorDialog color( crColor, CC_FULLOPEN);
	if( color.DoModal() != IDOK)  return;

	if( nID - _COLOR_BTN_ID_START < 6) m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, nID - _COLOR_BTN_ID_START + 1, color.GetColor());
	else
	{
		IDoubleList *pIDrawCndList = m_pIGraphInfo->GetDrawConditionList();
		if( pIDrawCndList)
		{
			POSITION posColor = pIDrawCndList->FindIndex( nID - _COLOR_BTN_ID_START - 6);
			if( posColor) pIDrawCndList->SetAt( posColor, color.GetColor());
			pIDrawCndList->Release();
		}
	}

	pColorButton->SetColor( color.GetColor() );
	pColorButton->Invalidate();	
}
