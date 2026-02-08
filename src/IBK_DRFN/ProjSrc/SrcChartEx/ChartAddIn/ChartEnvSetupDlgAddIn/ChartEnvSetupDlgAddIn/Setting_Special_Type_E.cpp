// Setting_Special_Type_E.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Special_Type_E.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
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
// CSetting_Special_Type_E dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_E::CSetting_Special_Type_E( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd )
	: CBaseDialog( CSetting_Special_Type_E::IDD, pParent, p_pIChartOCX, NULL)
{
	//{{AFX_DATA_INIT(CSetting_Special_Type_E)
	m_bIsIndividually = FALSE;
	//}}AFX_DATA_INIT
	m_nSubGraphIndex = -1;
}


void CSetting_Special_Type_E::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Special_Type_E)
	DDX_Control(pDX, IDC_BTN_COLOR6, m_btnColor6);
	DDX_Control(pDX, IDC_BTN_COLOR5, m_btnColor5);
	DDX_Control(pDX, IDC_BTN_COLOR4, m_btnColor4);
	DDX_Control(pDX, IDC_BTN_COLOR3, m_btnColor3);
	DDX_Control(pDX, IDC_BTN_COLOR2, m_btnColor2);
	DDX_Control(pDX, IDC_BTN_COLOR1, m_btnColor1);
	DDX_Control(pDX, IDC_LINE_TYPE, m_cmbLineType);
	DDX_Radio(pDX, IDC_RADIO1, m_nTextType);
	DDX_Control(pDX, IDC_SPIN1, m_spnCond1);
	DDX_Control(pDX, IDC_EDIT1, m_edtCond1);
	DDX_Radio(pDX, IDC_GUGAN_RADIO, m_nHorizontalBarType);
	DDX_Check(pDX, IDC_EACH_BUY_SELL, m_bIsIndividually);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_Special_Type_E, CDialog)
	//{{AFX_MSG_MAP(CSetting_Special_Type_E)
	ON_BN_CLICKED(IDC_BTN_COLOR1, OnBtnColor1)
	ON_BN_CLICKED(IDC_BTN_COLOR2, OnBtnColor2)
	ON_BN_CLICKED(IDC_BTN_COLOR3, OnBtnColor3)
	ON_BN_CLICKED(IDC_BTN_COLOR4, OnBtnColor4)
	ON_BN_CLICKED(IDC_BTN_COLOR5, OnBtnColor5)
	ON_BN_CLICKED(IDC_BTN_COLOR6, OnBtnColor6)
	ON_BN_CLICKED(IDC_GUGAN_RADIO, OnGuganRadio)
	ON_BN_CLICKED(IDC_HOGA_RADIO, OnHogaRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_E message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Special_Type_E::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::Initial(IGraphInfo *p_pIGraphInfo)
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
	ILPCSTR szKey;
	// (2008/6/11 - Seung-Won, Bae) Support Buy/Sell Overhanging Supply.
	//		Support Hoga Setting UI for Design Time.
	BOOL bHogaUsed = !m_pIChartOCX->IsRuntimeMode();
	if( !bHogaUsed && m_pIPacketManager)
	{
		IHelpMessageMap *pHelpMessageMap = m_pIPacketManager->GetHelpMessageMap();
		if( pHelpMessageMap)
		{
			bHogaUsed = pHelpMessageMap->Lookup("HBPRICE",szKey);
			pHelpMessageMap->Release();
		}
	}
	if( !bHogaUsed) GetDlgItem(IDC_HOGA_RADIO)->EnableWindow( FALSE);

// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	IDoubleList *pIDblCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	POSITION psnCnd = pIDblCnd->GetHeadPosition();
	if( pIDblCnd)
	{
		double dData = 0;
//		pIDblCnd->GetHead( dData);
		pIDblCnd->GetNext(psnCnd, dData);
		Initial_Spin( m_spnCond1, m_edtCond1, dData);
//2006.09.28 , ASH 매물대 호가별 옵션 추가
		if(psnCnd)
		{
			pIDblCnd->GetNext(psnCnd, dData);
			m_nHorizontalBarType = (int)dData;
		}
//2006.09.28 , ASH 매물대 호가별 옵션 추가
		pIDblCnd->Release();
	}

	Init_ColorButton();
	Initial_DrawStyleComboBox(m_nSubGraphIndex, m_cmbLineType);

	pIDblCnd = m_pIGraphInfo->GetDrawConditionList();
	if( pIDblCnd) if( !pIDblCnd->IsEmpty())
	{
		double dData = 0;
		pIDblCnd->GetHead( dData);
		m_nTextType = (int)dData;
	}

	//매수매도 대기매물
	ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
	if( !strcmp( szIndicatorName, _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY )))
	{
		CString strBuy, strBuyText, strSell, strSellText, strTotalText;
		strBuy.LoadString(IDS_BUY);
		strBuyText.LoadString(IDS_BUY_TEXT);
		strSell.LoadString(IDS_SELL);
		strSellText.LoadString(IDS_SELL_TEXT);
		strTotalText.LoadString( IDS_TOTAL_TEXT);
		(GetDlgItem(IDC_BYFONT_STATIC))->SetWindowText(strBuyText);
		(GetDlgItem(IDC_SELLFONT_STATIC))->SetWindowText(strSellText);
		(GetDlgItem(IDC_BY_STATIC))->SetWindowText(strBuy);
		(GetDlgItem(IDC_SELL_STATIC))->SetWindowText(strSell);
		(GetDlgItem(IDC_MAX_STATIC))->ShowWindow(SW_HIDE);
		m_btnColor3.ShowWindow(SW_HIDE);
		(GetDlgItem(IDC_MAXFONT_STATIC))->SetWindowText(strTotalText);
		// (2008/6/11 - Seung-Won, Bae) Support Buy/Sell Overhanging Supply.
		//		Support Hoga Setting UI for Design Time.
		GetDlgItem( IDC_BARTYPE_STATIC)->ShowWindow( SW_HIDE);
		GetDlgItem( IDC_LINE_TYPE)->ShowWindow( SW_HIDE);
		GetDlgItem( IDC_EACH_BUY_SELL)->ShowWindow( SW_SHOW);
		if( pIDblCnd) if( !pIDblCnd->IsEmpty())
		{
			double dData = 1;
			pIDblCnd->GetTail( dData);
			m_bIsIndividually = ( int( dData) == 1);
		}
	}
	else
	{
		CString strBasic, strBasicText, strCur, strCurText, strMaxText;
		strBasic.LoadString(IDS_BASIC_OFFER);
		strBasicText.LoadString(IDS_BASIC_OFFER_TEXT);
		strCur.LoadString(IDS_CUR_OFFER);
		strCurText.LoadString(IDS_CUR_OFFER_TEXT);
		strMaxText.LoadString( IDS_MAX_TEXT);
		(GetDlgItem(IDC_BYFONT_STATIC))->SetWindowText(strBasicText);
		(GetDlgItem(IDC_SELLFONT_STATIC))->SetWindowText(strCurText);
		(GetDlgItem(IDC_BY_STATIC))->SetWindowText(strBasic);
		(GetDlgItem(IDC_SELL_STATIC))->SetWindowText(strCur);
		(GetDlgItem(IDC_MAX_STATIC))->ShowWindow(SW_SHOW);
		m_btnColor3.ShowWindow(SW_SHOW);
		(GetDlgItem(IDC_MAXFONT_STATIC))->SetWindowText(strMaxText);
		// (2008/6/11 - Seung-Won, Bae) Support Buy/Sell Overhanging Supply.
		//		Support Hoga Setting UI for Design Time.
		GetDlgItem( IDC_BARTYPE_STATIC)->ShowWindow( SW_SHOW);
		GetDlgItem( IDC_LINE_TYPE)->ShowWindow( SW_SHOW);
		GetDlgItem( IDC_EACH_BUY_SELL)->ShowWindow( SW_HIDE);
	}

	if( pIDblCnd) pIDblCnd->Release();

	UpdateData(FALSE);

	// (2008/6/11 - Seung-Won, Bae) Support Buy/Sell Overhanging Supply.
	//		Support Hoga Setting UI for Design Time.
	if( m_nHorizontalBarType)	OnHogaRadio();
	else						OnGuganRadio();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Special_Type_E::OnApply( IGraphInfo* p_pIGraphInfo )
//void CSetting_Special_Type_E::OnApply( IGraphInfo* p_pIGraphInfo )
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
//2006.09.28 , ASH 매물대 호가별 옵션 추가
			pIDblCnd->GetNext( psnCnd, dNewData);
			if( psnCnd)
			{
				pIDblCnd->GetAt( psnCnd, dData);
				pIDblCnd->SetAt( psnCnd, double( m_nHorizontalBarType));
				if( !m_bDoCalculate ) m_bDoCalculate = ( dData != double( m_nHorizontalBarType));
			}
//2006.09.28 , ASH 매물대 호가별 옵션 추가
		}
		pIDblCnd->Release();
	}

	OnApply_DrawStyle( m_nSubGraphIndex, m_cmbLineType);

	pIDblCnd = m_pIGraphInfo->GetDrawConditionList();
	if( pIDblCnd)
	{
		POSITION psnDrawCnd = pIDblCnd->GetHeadPosition();
		pIDblCnd->SetAt( psnDrawCnd, double( m_nTextType));

		// (2008/6/11 - Seung-Won, Bae) Support Buy/Sell Overhanging Supply.
		ILPCSTR szIndicatorName = m_pIGraphInfo->GetIndicatorNameString();
		if( !strcmp( szIndicatorName, _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY )))
		{
			psnDrawCnd = pIDblCnd->GetTailPosition();
			pIDblCnd->SetAt( psnDrawCnd, ( m_bIsIndividually ? 1 : 0));
		}

		pIDblCnd->Release();
	}

	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Return void : 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::Init_ColorButton( void )
{
	if( m_nSubGraphIndex < 0) return;

	m_btnColor1.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	m_btnColor1.Invalidate();

	m_btnColor2.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	m_btnColor2.Invalidate();
	
	m_btnColor3.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 5));
	m_btnColor3.Invalidate();
	
	m_btnColor4.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 2));
	m_btnColor4.Invalidate();
	
	m_btnColor5.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 4));
	m_btnColor5.Invalidate();
	
	m_btnColor6.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 6));
	m_btnColor6.Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::OnBtnColor1() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColor1.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());

	m_btnColor1.SetColor( color.GetColor() );
	m_btnColor1.Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::OnBtnColor2() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColor2.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());

	m_btnColor2.SetColor( color.GetColor() );
	m_btnColor2.Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::OnBtnColor3() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColor3.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 5, color.GetColor());

	m_btnColor3.SetColor( color.GetColor() );
	m_btnColor3.Invalidate();		
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::OnBtnColor4() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColor4.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());

	m_btnColor4.SetColor( color.GetColor() );
	m_btnColor4.Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::OnBtnColor5() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColor5.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());

	m_btnColor5.SetColor( color.GetColor() );
	m_btnColor5.Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 04 / 19
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_E::OnBtnColor6() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnColor6.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 6, color.GetColor());

	m_btnColor6.SetColor( color.GetColor() );
	m_btnColor6.Invalidate();		
}


BOOL CSetting_Special_Type_E::PreTranslateMessage(MSG* pMsg) 
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

// (2008/6/11 - Seung-Won, Bae) Support Buy/Sell Overhanging Supply.
//		Support Hoga Setting UI for Design Time.
void CSetting_Special_Type_E::OnGuganRadio() 
{
	// TODO: Add your control notification handler code here
	ML_SET_LANGUAGE_RES();
	CString strTitle, strUnit;
	strTitle.LoadString(	IDS_OVERHANGING_BAR_COUNT);
	strUnit.LoadString(		IDS_COUNT);
	GetDlgItem( IDC_COND_TITLE)->SetWindowText(	strTitle);
	GetDlgItem( IDC_COND_UNIT)->SetWindowText(	strUnit);
}
void CSetting_Special_Type_E::OnHogaRadio() 
{
	// TODO: Add your control notification handler code here
	ML_SET_LANGUAGE_RES();
	CString strTitle, strUnit;
	strTitle.LoadString(	IDS_OVERHANGING_BAR_WIDTH);
	strUnit.LoadString(		IDS_PRICE_UNIT);
	GetDlgItem( IDC_COND_TITLE)->SetWindowText(	strTitle);
	GetDlgItem( IDC_COND_UNIT)->SetWindowText(	strUnit);
}
