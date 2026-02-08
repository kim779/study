// Setting_Volume.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Volume.h"

#include "GraphData.h"
#include "TabMainChartEnvSetUp.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"
#include "Conversion.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Volume dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Volume::CSetting_Volume( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_Volume::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Volume)
	//}}AFX_DATA_INIT
	m_nSubGraphIndex = -1;

	//sy 2006.04.12. -> "거래량차트" title 색상 관련 옵션 유지
	// -> default : 보합색(일반색)으로 보기
	m_nTitleColorDrawCond = 1;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Volume::~CSetting_Volume()
{
}


void CSetting_Volume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Volume)
	DDX_Control(pDX, IDC_CHECK_UP, m_btnFillUp);
	DDX_Control(pDX, IDC_CHECK_DOWN, m_btnFillDown);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_FILL_DOWN, m_FillDown_Bt);
	DDX_Control(pDX, IDC_FILL, m_Fill_Bt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_Volume, CBaseDialog)
	//{{AFX_MSG_MAP(CSetting_Volume)
	ON_BN_CLICKED(IDC_UPDOWN_VOLUME, OnUpdownVolume)
	ON_BN_CLICKED(IDC_SIMPLE_VOLUME, OnSimpleVolume)
	ON_BN_CLICKED(IDC_SAME_BONG, OnSameBong)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_FILL, OnFill)
	ON_BN_CLICKED(IDC_FILL_DOWN, OnFillDown)
	ON_BN_CLICKED(IDC_CHECK_UP, OnCheckUp)
	ON_BN_CLICKED(IDC_CHECK_DOWN, OnCheckDown)
	ON_BN_CLICKED(IDC_UPDOWN_PRICE, OnUpdownPrice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Volume message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Volume::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();

	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::Initial(IGraphInfo* p_pIGraphInfo)
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
	Initial_ColorButton( m_pIGraphInfo, m_nSubGraphIndex );
	Initial_FillUp(m_nSubGraphIndex, m_btnFillUp);
	Initial_FillDown(m_nSubGraphIndex, m_btnFillDown);
	Initial_FillUp(m_nSubGraphIndex, m_Fill_Bt);
	Initial_FillDown(m_nSubGraphIndex, m_FillDown_Bt);

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Param  int m_nSubGraphIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::Initial_ColorButton( IGraphInfo *p_pIGraphInfo, int m_nSubGraphIndex )
{
	if( m_nSubGraphIndex < 0
		|| p_pIGraphInfo == NULL) return;

	CButton *pButton = (CButton *)GetDlgItem(IDC_SIMPLE_VOLUME);	
	CButton *pUpDownButton = (CButton *)GetDlgItem(IDC_UPDOWN_VOLUME);
	CButton *pSameBongButton = (CButton *)GetDlgItem(IDC_SAME_BONG);
	//sy 2005.11.02.
	CButton *pUpDownPriceButton = (CButton *)GetDlgItem(IDC_UPDOWN_PRICE);
	if(pButton == NULL || pUpDownButton == NULL || pSameBongButton == NULL || pUpDownPriceButton == NULL)
		return;
	//sy end

	// Type에 따라 화면의 아이템을 배치한다.
	//sy 2006.04.12. -> "거래량차트" title 색상 관련 옵션 유지
	int nDrawType = 1;
	IDoubleList *pIDrawCondition = m_pIGraphInfo->GetDrawConditionList();
	if( pIDrawCondition)
	{
		double dDrawType = -1;
		POSITION psn = pIDrawCondition->GetHeadPosition( );
		if( psn)
		{
			pIDrawCondition->GetNext( psn, dDrawType);
			if( 1 <= dDrawType) nDrawType = ( int)dDrawType;
		}
		dDrawType = -1;
		if( psn)
		{
			pIDrawCondition->GetNext( psn, dDrawType);
			if( 1 <= dDrawType) m_nTitleColorDrawCond = (int)dDrawType;
			else m_nTitleColorDrawCond = 1;
		}
		pIDrawCondition->Release();
	}
	//sy end

	switch(nDrawType)
	{
		case 1 :		// 일반
		{
			pButton->SetCheck(1);
			pUpDownButton->SetCheck(0);
			pSameBongButton->SetCheck(0);
			pUpDownPriceButton->SetCheck(0);

			OnSimpleVolume();

			Initial_FillUp(m_nSubGraphIndex, m_Fill_Bt);
			Initial_FillDown(m_nSubGraphIndex, m_FillDown_Bt);
			break;
		}

		case 2 :		// 상승 / 하락
		{
			pButton->SetCheck(0);
			pUpDownButton->SetCheck(1);
			pSameBongButton->SetCheck(0);
			pUpDownPriceButton->SetCheck(0);
						
			OnUpdownVolume();

			Initial_FillUp(m_nSubGraphIndex, m_Fill_Bt);
			Initial_FillDown(m_nSubGraphIndex, m_FillDown_Bt);
			
			break;
		}

		case 3 :		// 캔들과 같이
		{
			pButton->SetCheck(0);
			pUpDownButton->SetCheck(0);
			pSameBongButton->SetCheck(1);
			pUpDownPriceButton->SetCheck(0);

			OnSameBong();

			Initial_FillUp(m_nSubGraphIndex, m_btnFillUp);
			Initial_FillDown(m_nSubGraphIndex, m_btnFillDown);
			break;
		}

		case 4 :		// 전일종가대비상승/하락
		{
			pButton->SetCheck(0);
			pUpDownButton->SetCheck(0);
			pSameBongButton->SetCheck(0);
			pUpDownPriceButton->SetCheck(1);

			OnUpdownPrice();

			Initial_FillUp(m_nSubGraphIndex, m_Fill_Bt);
			Initial_FillDown(m_nSubGraphIndex, m_FillDown_Bt);
			break;
		}
	}
	
	m_btnFillUp.SetCheck( m_Fill_Bt.GetCheck() );
	m_btnFillDown.SetCheck( m_FillDown_Bt.GetCheck() );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::OnSimpleVolume() 
{
	// 1. Dialog의 Item들의 조절한다.
	GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FILL)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FILL_DOWN)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SAME_CANDLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_DOWN)->ShowWindow(SW_HIDE);	

	ML_SET_LANGUAGE_RES();
	CString strColor;
	strColor.LoadString(IDS_BAR_COLOR);
	GetDlgItem(IDC_STATIC_UP)->SetWindowText(strColor);
	
	// 일반거래량 색은 1번째로 한다.
	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
	m_btnUp.Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::OnUpdownVolume() 
{
	GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FILL)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FILL_DOWN)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_SAME_CANDLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_DOWN)->ShowWindow(SW_HIDE);

// 2008.01.25 by LYH >>
	//GetDlgItem(IDC_STATIC_UP)->SetWindowText("상승");
	//GetDlgItem(IDC_STATIC_DOWN)->SetWindowText("하락");
	GetDlgItem(IDC_STATIC_UP)->SetWindowText(m_strUp);
	GetDlgItem(IDC_STATIC_DOWN)->SetWindowText(m_strDown);
// 2008.01.25 by LYH <<

	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1) );
	m_btnUp.Invalidate();
	m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3) );
	m_btnDown.Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Modifier		: 
// Comments		: "캔들색과 같이"일 경우 색변경을 못하게 한다.
//-----------------------------------------------------------------------------
void CSetting_Volume::OnSameBong() 
{
	GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FILL)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FILL_DOWN)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SAME_CANDLE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_DOWN)->ShowWindow(SW_SHOW);
}

//sy 2005.11.02.
void CSetting_Volume::OnUpdownPrice() 
{
	GetDlgItem(IDC_STATIC_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_UP)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FILL)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_DOWN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_DOWN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_FILL_DOWN)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_SAME_CANDLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_DOWN)->ShowWindow(SW_HIDE);

// 2008.01.25 by LYH >>	
	//GetDlgItem(IDC_STATIC_UP)->SetWindowText("상승");
	//GetDlgItem(IDC_STATIC_DOWN)->SetWindowText("하락");
	GetDlgItem(IDC_STATIC_UP)->SetWindowText(m_strUp);
	GetDlgItem(IDC_STATIC_DOWN)->SetWindowText(m_strDown);
// 2008.01.25 by LYH <<

//	m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1);
	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1) );
	m_btnUp.Invalidate();
	m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3) );
	m_btnDown.Invalidate();
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Volume::OnApply(IGraphInfo* p_pIGraphInfo)
//void CSetting_Volume::OnApply(IGraphInfo* p_pIGraphInfo)
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;

	//condition
	IDoubleList *pIDrawCondition = m_pIGraphInfo->GetDrawConditionList();
	if( pIDrawCondition)
	{
		POSITION psn = pIDrawCondition->GetHeadPosition( );
		if( psn)
		{
			pIDrawCondition->SetAt( psn, GetCheckToRadio());
			double dDrawType = -1;
			pIDrawCondition->GetNext( psn, dDrawType);
		}
		if( psn) pIDrawCondition->SetAt( psn, m_nTitleColorDrawCond);
		pIDrawCondition->Release();
	}

	OnApply_ColorButton( m_pIGraphInfo, m_nSubGraphIndex );
//	OnApply_FillUp(m_nSubGraphIndex, m_Fill_Bt);
//	OnApply_FillDown(m_nSubGraphIndex, m_FillDown_Bt);

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Param  int m_nSubGraphIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::OnApply_ColorButton( IGraphInfo *p_pIGraphInfo, int m_nSubGraphIndex)
{
	if( p_pIGraphInfo == NULL
		|| m_nSubGraphIndex < 0) return;

	int nDrawType = 1;
	IDoubleList *pIDrawCondition = m_pIGraphInfo->GetDrawConditionList();
	if( pIDrawCondition)
	{
		double dDrawType = -1;
		POSITION psn = pIDrawCondition->GetHeadPosition( );
		if( psn)
		{
			pIDrawCondition->GetNext( psn, dDrawType);
			if( 1 <= dDrawType) nDrawType = ( int)dDrawType;
		}
		pIDrawCondition->Release();
	}


	switch( nDrawType)
	{
		case 1 :		// 일반
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, m_btnUp.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 6, m_btnUp.GetColor());
			//OnApply_FillUp(m_nSubGraphIndex, m_Fill_Bt);
			//OnApply_FillDown(m_nSubGraphIndex, m_FillDown_Bt);
			OnApply_FillUp(m_nSubGraphIndex, m_Fill_Bt);			
			OnApply_FillDown(m_nSubGraphIndex, m_Fill_Bt);
			break;
		}

		case 2 :		// 상승 / 하락
		case 4 :		//전일종가대비
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, m_btnUp.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, m_btnUp.GetColor());
			
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, m_btnDown.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, m_btnDown.GetColor());
			OnApply_FillUp(m_nSubGraphIndex, m_Fill_Bt);
			OnApply_FillDown(m_nSubGraphIndex, m_FillDown_Bt);
			break;
		}

		case 3:
		{
			OnApply_FillUp(m_nSubGraphIndex, m_btnFillUp);
			OnApply_FillDown(m_nSubGraphIndex, m_btnFillDown);
			break;
		}
	}
}	
	
	
//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 22
// Return int : 
// Comments		: 
//-----------------------------------------------------------------------------
int CSetting_Volume::GetCheckToRadio()
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_SIMPLE_VOLUME);	
	CButton *pUpDownButton = (CButton *)GetDlgItem(IDC_UPDOWN_VOLUME);
	CButton *pSameBongButton = (CButton *)GetDlgItem(IDC_SAME_BONG);
	CButton *pUpDownPriceButton = (CButton *)GetDlgItem(IDC_UPDOWN_PRICE);

	if(pButton != NULL && pButton->GetCheck())
		return 1;
	else if(pUpDownButton != NULL && pUpDownButton->GetCheck())
		return 2;
	else if(pSameBongButton != NULL && pSameBongButton->GetCheck())
		return 3;
	//sy 2005.11.02.
	else if(pUpDownPriceButton != NULL && pUpDownPriceButton->GetCheck())
		return 4;
	//sy end

	return 1;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::OnBtnUp() 
{
	if(m_nSubGraphIndex < 0)
		return;

	int nType = GetCheckToRadio();
	
	
	COLORREF crColor = m_btnUp.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		if( nType == 1 )
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 6, color.GetColor());
			m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
		}

		else
		{
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
			m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
			m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
		}
	
		m_btnUp.Invalidate();
	}	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Volume::OnBtnDown() 
{
	if(m_nSubGraphIndex < 0)
		return;
	
	COLORREF crColor = m_btnDown.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());
		m_btnDown.SetColor(  m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
		m_btnDown.Invalidate();
	}	
}


void CSetting_Volume::OnFill() 
{
	// TODO: Add your control notification handler code here
	m_btnFillUp.SetCheck( m_Fill_Bt.GetCheck() );
}

void CSetting_Volume::OnFillDown() 
{
	// TODO: Add your control notification handler code here
	m_btnFillDown.SetCheck( m_FillDown_Bt.GetCheck() );
}

void CSetting_Volume::OnCheckUp() 
{
	// TODO: Add your control notification handler code here
	m_Fill_Bt.SetCheck( m_btnFillUp.GetCheck() );
	
}

void CSetting_Volume::OnCheckDown() 
{
	// TODO: Add your control notification handler code here
	m_FillDown_Bt.SetCheck( m_btnFillDown.GetCheck() );
}

BOOL CSetting_Volume::PreTranslateMessage(MSG* pMsg) 
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
