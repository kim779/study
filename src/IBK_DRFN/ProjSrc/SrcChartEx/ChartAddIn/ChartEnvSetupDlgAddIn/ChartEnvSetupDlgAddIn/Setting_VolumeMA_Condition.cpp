// Setting_VolumeMA_Condition.cpp : implementation file
//

#include "stdafx.h"
#include "chartenvsetupdlgaddin.h"
#include "Setting_VolumeMA.h"
#include "Setting_VolumeMA_Condition.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_VolumeMA_Condition dialog


CSetting_VolumeMA_Condition::CSetting_VolumeMA_Condition( CWnd* pParent /*=NULL*/)
	: CDialog(CSetting_VolumeMA_Condition::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetting_VolumeMA_Condition)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	if( pParent )
		m_pParentDlg = (CBaseDialog *)pParent;
	else
		m_pParentDlg = NULL;

	m_pIGraphInfo = NULL;

	m_nSubGraphCount = 0;

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}


CSetting_VolumeMA_Condition::~CSetting_VolumeMA_Condition()
{
	if( m_pIGraphInfo)
	{
		m_pIGraphInfo->Release();
		m_pIGraphInfo = NULL;
	}
}

void CSetting_VolumeMA_Condition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_VolumeMA_Condition)
	DDX_Control(pDX, IDC_THICKNESS9, m_cmbThicknessGraph9);
	DDX_Control(pDX, IDC_THICKNESS8, m_cmbThicknessGraph8);
	DDX_Control(pDX, IDC_THICKNESS7, m_cmbThicknessGraph7);
	DDX_Control(pDX, IDC_THICKNESS6, m_cmbThicknessGraph6);
	DDX_Control(pDX, IDC_THICKNESS5, m_cmbThicknessGraph5);
	DDX_Control(pDX, IDC_THICKNESS4, m_cmbThicknessGraph4);
	DDX_Control(pDX, IDC_THICKNESS3, m_cmbThicknessGraph3);
	DDX_Control(pDX, IDC_THICKNESS2, m_cmbThicknessGraph2);
	DDX_Control(pDX, IDC_THICKNESS1, m_cmbThicknessGraph1);
	DDX_Control(pDX, IDC_THICKNESS0, m_cmbThicknessGraph0);
	DDX_Control(pDX, IDC_SPIN_COND9, m_spnGraph9);
	DDX_Control(pDX, IDC_SPIN_COND8, m_spnGraph8);
	DDX_Control(pDX, IDC_SPIN_COND7, m_spnGraph7);
	DDX_Control(pDX, IDC_SPIN_COND6, m_spnGraph6);
	DDX_Control(pDX, IDC_SPIN_COND5, m_spnGraph5);
	DDX_Control(pDX, IDC_SPIN_COND4, m_spnGraph4);
	DDX_Control(pDX, IDC_SPIN_COND3, m_spnGraph3);
	DDX_Control(pDX, IDC_SPIN_COND2, m_spnGraph2);
	DDX_Control(pDX, IDC_SPIN_COND1, m_spnGraph1);
	DDX_Control(pDX, IDC_SPIN_COND0, m_spnGraph0);
	DDX_Control(pDX, IDC_DRAWSTYLE9, m_cmbDrawStyleGraph9);
	DDX_Control(pDX, IDC_DRAWSTYLE8, m_cmbDrawStyleGraph8);
	DDX_Control(pDX, IDC_DRAWSTYLE7, m_cmbDrawStyleGraph7);
	DDX_Control(pDX, IDC_DRAWSTYLE6, m_cmbDrawStyleGraph6);
	DDX_Control(pDX, IDC_DRAWSTYLE5, m_cmbDrawStyleGraph5);
	DDX_Control(pDX, IDC_DRAWSTYLE4, m_cmbDrawStyleGraph4);
	DDX_Control(pDX, IDC_DRAWSTYLE3, m_cmbDrawStyleGraph3);
	DDX_Control(pDX, IDC_DRAWSTYLE2, m_cmbDrawStyleGraph2);
	DDX_Control(pDX, IDC_DRAWSTYLE1, m_cmbDrawStyleGraph1);
	DDX_Control(pDX, IDC_DRAWSTYLE0, m_cmbDrawStyleGraph0);
	DDX_Control(pDX, IDC_COND_GRAPH9, m_edtGraph9);
	DDX_Control(pDX, IDC_COND_GRAPH8, m_edtGraph8);
	DDX_Control(pDX, IDC_COND_GRAPH7, m_edtGraph7);
	DDX_Control(pDX, IDC_COND_GRAPH6, m_edtGraph6);
	DDX_Control(pDX, IDC_COND_GRAPH5, m_edtGraph5);
	DDX_Control(pDX, IDC_COND_GRAPH4, m_edtGraph4);
	DDX_Control(pDX, IDC_COND_GRAPH3, m_edtGraph3);
	DDX_Control(pDX, IDC_COND_GRAPH2, m_edtGraph2);
	DDX_Control(pDX, IDC_COND_GRAPH1, m_edtGraph1);
	DDX_Control(pDX, IDC_COND_GRAPH0, m_edtGraph0);
	DDX_Control(pDX, IDC_COLOR_BUTTON9, m_btnColor9);
	DDX_Control(pDX, IDC_COLOR_BUTTON8, m_btnColor8);
	DDX_Control(pDX, IDC_COLOR_BUTTON7, m_btnColor7);
	DDX_Control(pDX, IDC_COLOR_BUTTON6, m_btnColor6);
	DDX_Control(pDX, IDC_COLOR_BUTTON5, m_btnColor5);
	DDX_Control(pDX, IDC_COLOR_BUTTON4, m_btnColor4);
	DDX_Control(pDX, IDC_COLOR_BUTTON3, m_btnColor3);
	DDX_Control(pDX, IDC_COLOR_BUTTON2, m_btnColor2);
	DDX_Control(pDX, IDC_COLOR_BUTTON1, m_btnColor1);
	DDX_Control(pDX, IDC_COLOR_BUTTON0, m_btnColor0);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD9, m_cmbCalcMethod9);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD8, m_cmbCalcMethod8);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD7, m_cmbCalcMethod7);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD6, m_cmbCalcMethod6);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD5, m_cmbCalcMethod5);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD4, m_cmbCalcMethod4);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD3, m_cmbCalcMethod3);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD2, m_cmbCalcMethod2);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD1, m_cmbCalcMethod1);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD0, m_cmbCalcMethod0);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH9, m_btnShowGraph9);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH8, m_btnShowGraph8);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH7, m_btnShowGraph7);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH6, m_btnShowGraph6);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH5, m_btnShowGraph5);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH4, m_btnShowGraph4);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH3, m_btnShowGraph3);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH2, m_btnShowGraph2);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH1, m_btnShowGraph1);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH0, m_btnShowGraph0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_VolumeMA_Condition, CDialog)
	//{{AFX_MSG_MAP(CSetting_VolumeMA_Condition)
	ON_BN_CLICKED(IDC_COLOR_BUTTON0, OnColorButton0)
	ON_BN_CLICKED(IDC_COLOR_BUTTON1, OnColorButton1)
	ON_BN_CLICKED(IDC_COLOR_BUTTON2, OnColorButton2)
	ON_BN_CLICKED(IDC_COLOR_BUTTON3, OnColorButton3)
	ON_BN_CLICKED(IDC_COLOR_BUTTON4, OnColorButton4)
	ON_BN_CLICKED(IDC_COLOR_BUTTON5, OnColorButton5)
	ON_BN_CLICKED(IDC_COLOR_BUTTON6, OnColorButton6)
	ON_BN_CLICKED(IDC_COLOR_BUTTON7, OnColorButton7)
	ON_BN_CLICKED(IDC_COLOR_BUTTON8, OnColorButton8)
	ON_BN_CLICKED(IDC_COLOR_BUTTON9, OnColorButton9)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH0, OnCheckShowGraph0)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH1, OnCheckShowGraph1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH2, OnCheckShowGraph2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH3, OnCheckShowGraph3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH4, OnCheckShowGraph4)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH5, OnCheckShowGraph5)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH6, OnCheckShowGraph6)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH7, OnCheckShowGraph7)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH8, OnCheckShowGraph8)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH9, OnCheckShowGraph9)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE0, OnSelchangeDrawstyle0)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE1, OnSelchangeDrawstyle1)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE2, OnSelchangeDrawstyle2)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE3, OnSelchangeDrawstyle3)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE4, OnSelchangeDrawstyle4)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE5, OnSelchangeDrawstyle5)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE6, OnSelchangeDrawstyle6)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE7, OnSelchangeDrawstyle7)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE8, OnSelchangeDrawstyle8)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE9, OnSelchangeDrawstyle9)
	ON_CBN_SELCHANGE(IDC_THICKNESS0, OnSelchangeThickness0)
	ON_CBN_SELCHANGE(IDC_THICKNESS1, OnSelchangeThickness1)
	ON_CBN_SELCHANGE(IDC_THICKNESS2, OnSelchangeThickness2)
	ON_CBN_SELCHANGE(IDC_THICKNESS3, OnSelchangeThickness3)
	ON_CBN_SELCHANGE(IDC_THICKNESS4, OnSelchangeThickness4)
	ON_CBN_SELCHANGE(IDC_THICKNESS5, OnSelchangeThickness5)
	ON_CBN_SELCHANGE(IDC_THICKNESS6, OnSelchangeThickness6)
	ON_CBN_SELCHANGE(IDC_THICKNESS7, OnSelchangeThickness7)
	ON_CBN_SELCHANGE(IDC_THICKNESS8, OnSelchangeThickness8)
	ON_CBN_SELCHANGE(IDC_THICKNESS9, OnSelchangeThickness9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_VolumeMA_Condition message handlers
BOOL CSetting_VolumeMA_Condition::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
// 2008.01.25 by LYH >>
	m_strMsgLineThick.LoadString(IDS_MSG_LINE_THICK);
	m_strMsgLineShape.LoadString(IDS_MSG_LINE_SHAPE);
	m_strMsgExistMin.LoadString(IDS_MSG_EXIST_MIN);
// 2008.01.25 by LYH <<

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetting_VolumeMA_Condition::Initial (IGraphInfo* p_pIGraphInfo)
{
	if( m_pIGraphInfo) m_pIGraphInfo->Release();
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return int	: 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
int CSetting_VolumeMA_Condition::GetSubGraphCount( void )
{
	return m_nSubGraphCount;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return void	: 
// Param  int nCount : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_VolumeMA_Condition::SetSubGraphCount( int nSubGraphCount )
{
	m_nSubGraphCount = nSubGraphCount;

	ShowScrollBar( SB_VERT, FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CButton*	: 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CButton* CSetting_VolumeMA_Condition::GetCheckButton(const int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CButton *)GetDlgItem( IDC_CHECK_SHOW_GRAPH0 + nSubGraphIndex );

	return (CButton *)GetDlgItem( IDC_CHECK_SHOW_GRAPH0 );
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CNumEdit*	: 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CNumEdit* CSetting_VolumeMA_Condition::GetEditCtrl(const int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CNumEdit *)GetDlgItem( IDC_COND_GRAPH0 + nSubGraphIndex );

	return (CNumEdit *)GetDlgItem( IDC_COND_GRAPH0 );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CNumSpinCtrl*	: 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CNumSpinCtrl* CSetting_VolumeMA_Condition::GetSpinCtrl(const int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CNumSpinCtrl *)GetDlgItem( IDC_SPIN_COND0 + nSubGraphIndex );

	return (CNumSpinCtrl *)GetDlgItem( IDC_SPIN_COND0 );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CBitmapItemComboBox*	: 
// Param  int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CBitmapItemComboBox* CSetting_VolumeMA_Condition::GetDrawStyleCombo(int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CBitmapItemComboBox *)GetDlgItem( IDC_DRAWSTYLE0 + nSubGraphIndex );

	return (CBitmapItemComboBox *)GetDlgItem( IDC_DRAWSTYLE0 );
}


void CSetting_VolumeMA_Condition::SetDrawStyle( int nSubGraphIndex, int nCmbIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT)
	{
		CBitmapItemComboBox *pBitmapItemComboBox = ( CBitmapItemComboBox *)GetDlgItem( IDC_DRAWSTYLE0 + nSubGraphIndex );
		if( pBitmapItemComboBox) pBitmapItemComboBox->SetCurSel( nCmbIndex);
	}
}


void CSetting_VolumeMA_Condition::SetLineThickness( int nSubGraphIndex, int nCmbIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT)
	{
		CBitmapItemComboBox *pBitmapItemComboBox = ( CBitmapItemComboBox *)GetDlgItem( IDC_THICKNESS0 + nSubGraphIndex);
		if( pBitmapItemComboBox) pBitmapItemComboBox->SetCurSel( nCmbIndex);
	}
}


CBitmapItemComboBox* CSetting_VolumeMA_Condition::GetThicknessCombo(int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CBitmapItemComboBox *)GetDlgItem( IDC_THICKNESS0 + nSubGraphIndex );

	return (CBitmapItemComboBox *)GetDlgItem( IDC_THICKNESS0 );
}


void CSetting_VolumeMA_Condition::SetLineColor( int nSubGraphIndex, int nColorIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
	{
		CColorButton *pBtnColor = (CColorButton *)GetDlgItem( IDC_COLOR_BUTTON0 + nSubGraphIndex );
		if( pBtnColor)
		{
			pBtnColor->SetColor( nColorIndex );
			pBtnColor->SetText("");
			pBtnColor->Invalidate();
		}
	}
}


CComboBox *CSetting_VolumeMA_Condition::GetCalcMethodCombo( int nSubGraphIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CComboBox *)GetDlgItem(IDC_CMB_CALCMETHOD0 + nSubGraphIndex );

	return (CComboBox *)GetDlgItem(IDC_CMB_CALCMETHOD0);
}


CColorButton* CSetting_VolumeMA_Condition::GetColorButton(int nSubGraphIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
		return (CColorButton *)GetDlgItem(IDC_COLOR_BUTTON0 + nSubGraphIndex );

	return (CColorButton *)GetDlgItem(IDC_COLOR_BUTTON0);
}



void CSetting_VolumeMA_Condition::ShowHideSubGraph( int nSubGraphIndex, int nCmdShow )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < VOLUME_MA_CONDITION_COUNT )
	{
		CWnd *pWnd = GetDlgItem( IDC_CHECK_SHOW_GRAPH0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_COND_GRAPH0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_SPIN_COND0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_CMB_CALCMETHOD0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_COLOR_BUTTON0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_THICKNESS0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_DRAWSTYLE0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
	}
}

BOOL CSetting_VolumeMA_Condition::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentDlg )
			{
				((CSetting_VolumeMA *)m_pParentDlg)->OK();
			}
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentDlg )
			{
				((CSetting_VolumeMA *)m_pParentDlg)->Cancel();
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetting_VolumeMA_Condition::OnColorButton0() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor0.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 0, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 0, 2, color.GetColor());
		m_btnColor0.SetColor( m_pIGraphInfo->GetSubGraphColor( 0, 1));
		m_btnColor0.Invalidate();
	}
}

void CSetting_VolumeMA_Condition::OnColorButton1() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor1.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 1, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 1, 2, color.GetColor());
		m_btnColor1.SetColor( m_pIGraphInfo->GetSubGraphColor( 1, 1));
		m_btnColor1.Invalidate();
	}	
}

void CSetting_VolumeMA_Condition::OnColorButton2() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor2.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 2, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 2, 2, color.GetColor());
		m_btnColor2.SetColor( m_pIGraphInfo->GetSubGraphColor( 2, 1));
		m_btnColor2.Invalidate();
	}	
}

void CSetting_VolumeMA_Condition::OnColorButton3() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor3.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 3, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 3, 2, color.GetColor());
		m_btnColor3.SetColor( m_pIGraphInfo->GetSubGraphColor( 3, 1));
		m_btnColor3.Invalidate();
	}
}

void CSetting_VolumeMA_Condition::OnColorButton4() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor4.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 4, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 4, 2, color.GetColor());
		m_btnColor4.SetColor( m_pIGraphInfo->GetSubGraphColor( 4, 1));
		m_btnColor4.Invalidate();
	}	
}

void CSetting_VolumeMA_Condition::OnColorButton5() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor5.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 5, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 5, 2, color.GetColor());
		m_btnColor5.SetColor( m_pIGraphInfo->GetSubGraphColor( 5, 1));
		m_btnColor5.Invalidate();
	}	
}

void CSetting_VolumeMA_Condition::OnColorButton6() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor6.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 6, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 6, 2, color.GetColor());
		m_btnColor6.SetColor( m_pIGraphInfo->GetSubGraphColor( 6, 1));
		m_btnColor6.Invalidate();
	}	
}

void CSetting_VolumeMA_Condition::OnColorButton7() 
{
	// TODO: Add your control notification handler code here
		COLORREF crColor = m_btnColor7.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 7, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 7, 2, color.GetColor());
		m_btnColor7.SetColor( m_pIGraphInfo->GetSubGraphColor( 7, 1));
		m_btnColor7.Invalidate();
	}
}

void CSetting_VolumeMA_Condition::OnColorButton8() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor8.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 8, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 8, 2, color.GetColor());
		m_btnColor8.SetColor( m_pIGraphInfo->GetSubGraphColor( 8, 1));
		m_btnColor8.Invalidate();
	}
}

void CSetting_VolumeMA_Condition::OnColorButton9() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor9.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 9, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 9, 2, color.GetColor());
		m_btnColor9.SetColor( m_pIGraphInfo->GetSubGraphColor( 9, 1));
		m_btnColor9.Invalidate();
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle0() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph0.GetCurSel() != 0 && m_cmbDrawStyleGraph0.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph0.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle1() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph1.GetCurSel() != 0 && m_cmbDrawStyleGraph1.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph1.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle2() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph2.GetCurSel() != 0 && m_cmbDrawStyleGraph2.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph2.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle3() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph3.GetCurSel() != 0 && m_cmbDrawStyleGraph3.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph3.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle4() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph4.GetCurSel() != 0 && m_cmbDrawStyleGraph4.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph4.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle5() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph5.GetCurSel() != 0 && m_cmbDrawStyleGraph5.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph5.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle6() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph6.GetCurSel() != 0 && m_cmbDrawStyleGraph6.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph6.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle7() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph7.GetCurSel() != 0 && m_cmbDrawStyleGraph7.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph7.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle8() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph8.GetCurSel() != 0 && m_cmbDrawStyleGraph8.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph8.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeDrawstyle9() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph9.GetCurSel() != 0 && m_cmbDrawStyleGraph9.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph9.SetCurSel(0);
		return;
	}	
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness0() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph0.GetCurSel() != 0 && m_cmbThicknessGraph0.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<

		m_cmbDrawStyleGraph0.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness1() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph1.GetCurSel() != 0 && m_cmbThicknessGraph1.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph1.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness2() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph2.GetCurSel() != 0 && m_cmbThicknessGraph2.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph2.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness3() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph3.GetCurSel() != 0 && m_cmbThicknessGraph3.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph3.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness4() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph4.GetCurSel() != 0 && m_cmbThicknessGraph4.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph4.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness5() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph5.GetCurSel() != 0 && m_cmbThicknessGraph5.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph5.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness6() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph6.GetCurSel() != 0 && m_cmbThicknessGraph6.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph6.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness7() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph7.GetCurSel() != 0 && m_cmbThicknessGraph7.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph7.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness8() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph8.GetCurSel() != 0 && m_cmbThicknessGraph8.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph8.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnSelchangeThickness9() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph9.GetCurSel() != 0 && m_cmbThicknessGraph9.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph9.SetCurSel(0);
		return;
	}
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph0() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH0 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph1() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH1 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph2() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH2 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph3() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH3 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph4() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH4 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph5() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH5 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph6() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH6 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph7() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH7 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph8() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH8 );
}


void CSetting_VolumeMA_Condition::OnCheckShowGraph9() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH9 );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  const BOOL bCheck : 
// Comments		: 모든 이평 그래프를 감추기 했을때.
//-----------------------------------------------------------------------------
// (2006/8/7 - Seung-Won, Bae) Check uncheck, and All Uncheck can't be possible.
void CSetting_VolumeMA_Condition::WhenAllSubGraphisHide( const int p_nCheckID )
{
	CButton *pButton = ( CButton *)GetDlgItem( p_nCheckID );
	if( pButton->GetCheck() ) return;

	int nCheckCount = 0;
	
	CButton *pCheckButton = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		pCheckButton = GetCheckButton( i);
		if( pCheckButton) if( pCheckButton->GetCheck()) nCheckCount++;
	}

	if( 0 < nCheckCount)
		return;

// 2008.01.25 by LYH >>
	//AfxMessageBox("최소 하나 이상의 구성설정이 존재해야 됩니다.");
	AfxMessageBox(m_strMsgExistMin);
// 2008.01.25 by LYH <<
	pButton->SetCheck(1);
	
	SendMessage( WM_COMMAND, p_nCheckID);
}
