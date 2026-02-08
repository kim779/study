// Setting_PriceMA_Condition.cpp : implementation file
//

#include "stdafx.h"
#include "chartenvsetupdlgaddin.h"
#include "Setting_PriceMA.h"
#include "Setting_PriceMA_Condition.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CSetting_PriceMA_Condition dialog


CSetting_PriceMA_Condition::CSetting_PriceMA_Condition( CWnd *pTopWnd /*=NULL*/ )
	: CDialog(CSetting_PriceMA_Condition::IDD, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_PriceMA_Condition)
	//}}AFX_DATA_INIT

	if( pTopWnd )
		m_pParentDlg = (CBaseDialog *)pTopWnd;
	else
		m_pParentDlg = NULL;

	m_nSubGraphCount = 0;

	m_pIGraphInfo = NULL;

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}


CSetting_PriceMA_Condition::~CSetting_PriceMA_Condition()
{
	if( m_pIGraphInfo)
	{
		m_pIGraphInfo->Release();
		m_pIGraphInfo = NULL;
	}
}


void CSetting_PriceMA_Condition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_PriceMA_Condition)
	DDX_Control(pDX, IDC_THICKNESS9, m_cmbThicknessGraph9);
	DDX_Control(pDX, IDC_THICKNESS8, m_cmbThicknessGraph8);
	DDX_Control(pDX, IDC_THICKNESS7, m_cmbThicknessGraph7);
	DDX_Control(pDX, IDC_THICKNESS6, m_cmbThicknessGraph6);
	DDX_Control(pDX, IDC_THICKNESS5, m_cmbThicknessGraph5);
	DDX_Control(pDX, IDC_THICKNESS4, m_cmbThicknessGraph4);
	DDX_Control(pDX, IDC_THICKNESS3, m_cmbThicknessGraph3);
	DDX_Control(pDX, IDC_THICKNESS2, m_cmbThicknessGraph2);
	DDX_Control(pDX, IDC_THICKNESS19, m_cmbThicknessGraph19);
	DDX_Control(pDX, IDC_THICKNESS18, m_cmbThicknessGraph18);
	DDX_Control(pDX, IDC_THICKNESS17, m_cmbThicknessGraph17);
	DDX_Control(pDX, IDC_THICKNESS16, m_cmbThicknessGraph16);
	DDX_Control(pDX, IDC_THICKNESS15, m_cmbThicknessGraph15);
	DDX_Control(pDX, IDC_THICKNESS14, m_cmbThicknessGraph14);
	DDX_Control(pDX, IDC_THICKNESS13, m_cmbThicknessGraph13);
	DDX_Control(pDX, IDC_THICKNESS12, m_cmbThicknessGraph12);
	DDX_Control(pDX, IDC_THICKNESS11, m_cmbThicknessGraph11);
	DDX_Control(pDX, IDC_THICKNESS10, m_cmbThicknessGraph10);
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
	DDX_Control(pDX, IDC_SPIN_COND19, m_spnGraph19);
	DDX_Control(pDX, IDC_SPIN_COND18, m_spnGraph18);
	DDX_Control(pDX, IDC_SPIN_COND17, m_spnGraph17);
	DDX_Control(pDX, IDC_SPIN_COND16, m_spnGraph16);
	DDX_Control(pDX, IDC_SPIN_COND15, m_spnGraph15);
	DDX_Control(pDX, IDC_SPIN_COND14, m_spnGraph14);
	DDX_Control(pDX, IDC_SPIN_COND13, m_spnGraph13);
	DDX_Control(pDX, IDC_SPIN_COND12, m_spnGraph12);
	DDX_Control(pDX, IDC_SPIN_COND11, m_spnGraph11);
	DDX_Control(pDX, IDC_SPIN_COND10, m_spnGraph10);
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
	DDX_Control(pDX, IDC_DRAWSTYLE19, m_cmbDrawStyleGraph19);
	DDX_Control(pDX, IDC_DRAWSTYLE18, m_cmbDrawStyleGraph18);
	DDX_Control(pDX, IDC_DRAWSTYLE17, m_cmbDrawStyleGraph17);
	DDX_Control(pDX, IDC_DRAWSTYLE16, m_cmbDrawStyleGraph16);
	DDX_Control(pDX, IDC_DRAWSTYLE15, m_cmbDrawStyleGraph15);
	DDX_Control(pDX, IDC_DRAWSTYLE14, m_cmbDrawStyleGraph14);
	DDX_Control(pDX, IDC_DRAWSTYLE13, m_cmbDrawStyleGraph13);
	DDX_Control(pDX, IDC_DRAWSTYLE12, m_cmbDrawStyleGraph12);
	DDX_Control(pDX, IDC_DRAWSTYLE11, m_cmbDrawStyleGraph11);
	DDX_Control(pDX, IDC_DRAWSTYLE10, m_cmbDrawStyleGraph10);
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
	DDX_Control(pDX, IDC_COND_GRAPH19, m_edtGraph19);
	DDX_Control(pDX, IDC_COND_GRAPH18, m_edtGraph18);
	DDX_Control(pDX, IDC_COND_GRAPH17, m_edtGraph17);
	DDX_Control(pDX, IDC_COND_GRAPH16, m_edtGraph16);
	DDX_Control(pDX, IDC_COND_GRAPH15, m_edtGraph15);
	DDX_Control(pDX, IDC_COND_GRAPH14, m_edtGraph14);
	DDX_Control(pDX, IDC_COND_GRAPH13, m_edtGraph13);
	DDX_Control(pDX, IDC_COND_GRAPH12, m_edtGraph12);
	DDX_Control(pDX, IDC_COND_GRAPH11, m_edtGraph11);
	DDX_Control(pDX, IDC_COND_GRAPH10, m_edtGraph10);
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
	DDX_Control(pDX, IDC_COLOR_BUTTON19, m_btnColor19);
	DDX_Control(pDX, IDC_COLOR_BUTTON18, m_btnColor18);
	DDX_Control(pDX, IDC_COLOR_BUTTON17, m_btnColor17);
	DDX_Control(pDX, IDC_COLOR_BUTTON16, m_btnColor16);
	DDX_Control(pDX, IDC_COLOR_BUTTON15, m_btnColor15);
	DDX_Control(pDX, IDC_COLOR_BUTTON14, m_btnColor14);
	DDX_Control(pDX, IDC_COLOR_BUTTON13, m_btnColor13);
	DDX_Control(pDX, IDC_COLOR_BUTTON12, m_btnColor12);
	DDX_Control(pDX, IDC_COLOR_BUTTON11, m_btnColor11);
	DDX_Control(pDX, IDC_COLOR_BUTTON10, m_btnColor10);
	DDX_Control(pDX, IDC_COLOR_BUTTON1, m_btnColor1);
	DDX_Control(pDX, IDC_COLOR_BUTTON0, m_btnColor0);
	DDX_Control(pDX, IDC_CMB_PACKET9, m_cmbPacket9);
	DDX_Control(pDX, IDC_CMB_PACKET8, m_cmbPacket8);
	DDX_Control(pDX, IDC_CMB_PACKET7, m_cmbPacket7);
	DDX_Control(pDX, IDC_CMB_PACKET6, m_cmbPacket6);
	DDX_Control(pDX, IDC_CMB_PACKET5, m_cmbPacket5);
	DDX_Control(pDX, IDC_CMB_PACKET4, m_cmbPacket4);
	DDX_Control(pDX, IDC_CMB_PACKET3, m_cmbPacket3);
	DDX_Control(pDX, IDC_CMB_PACKET2, m_cmbPacket2);
	DDX_Control(pDX, IDC_CMB_PACKET19, m_cmbPacket19);
	DDX_Control(pDX, IDC_CMB_PACKET18, m_cmbPacket18);
	DDX_Control(pDX, IDC_CMB_PACKET17, m_cmbPacket17);
	DDX_Control(pDX, IDC_CMB_PACKET16, m_cmbPacket16);
	DDX_Control(pDX, IDC_CMB_PACKET15, m_cmbPacket15);
	DDX_Control(pDX, IDC_CMB_PACKET14, m_cmbPacket14);
	DDX_Control(pDX, IDC_CMB_PACKET13, m_cmbPacket13);
	DDX_Control(pDX, IDC_CMB_PACKET12, m_cmbPacket12);
	DDX_Control(pDX, IDC_CMB_PACKET11, m_cmbPacket11);
	DDX_Control(pDX, IDC_CMB_PACKET10, m_cmbPacket10);
	DDX_Control(pDX, IDC_CMB_PACKET1, m_cmbPacket1);
	DDX_Control(pDX, IDC_CMB_PACKET0, m_cmbPacket0);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD9, m_cmbCalcMethod9);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD8, m_cmbCalcMethod8);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD7, m_cmbCalcMethod7);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD6, m_cmbCalcMethod6);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD5, m_cmbCalcMethod5);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD4, m_cmbCalcMethod4);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD3, m_cmbCalcMethod3);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD2, m_cmbCalcMethod2);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD19, m_cmbCalcMethod19);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD18, m_cmbCalcMethod18);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD17, m_cmbCalcMethod17);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD16, m_cmbCalcMethod16);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD15, m_cmbCalcMethod15);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD14, m_cmbCalcMethod14);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD13, m_cmbCalcMethod13);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD12, m_cmbCalcMethod12);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD11, m_cmbCalcMethod11);
	DDX_Control(pDX, IDC_CMB_CALCMETHOD10, m_cmbCalcMethod10);
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
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH19, m_btnShowGraph19);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH18, m_btnShowGraph18);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH17, m_btnShowGraph17);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH16, m_btnShowGraph16);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH15, m_btnShowGraph15);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH14, m_btnShowGraph14);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH13, m_btnShowGraph13);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH12, m_btnShowGraph12);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH11, m_btnShowGraph11);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH10, m_btnShowGraph10);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH1, m_btnShowGraph1);
	DDX_Control(pDX, IDC_CHECK_SHOW_GRAPH0, m_btnShowGraph0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_PriceMA_Condition, CDialog)
	//{{AFX_MSG_MAP(CSetting_PriceMA_Condition)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_COLOR_BUTTON0, OnColorButton0)
	ON_BN_CLICKED(IDC_COLOR_BUTTON1, OnColorButton1)
	ON_BN_CLICKED(IDC_COLOR_BUTTON10, OnColorButton10)
	ON_BN_CLICKED(IDC_COLOR_BUTTON11, OnColorButton11)
	ON_BN_CLICKED(IDC_COLOR_BUTTON12, OnColorButton12)
	ON_BN_CLICKED(IDC_COLOR_BUTTON13, OnColorButton13)
	ON_BN_CLICKED(IDC_COLOR_BUTTON14, OnColorButton14)
	ON_BN_CLICKED(IDC_COLOR_BUTTON15, OnColorButton15)
	ON_BN_CLICKED(IDC_COLOR_BUTTON16, OnColorButton16)
	ON_BN_CLICKED(IDC_COLOR_BUTTON17, OnColorButton17)
	ON_BN_CLICKED(IDC_COLOR_BUTTON18, OnColorButton18)
	ON_BN_CLICKED(IDC_COLOR_BUTTON19, OnColorButton19)
	ON_BN_CLICKED(IDC_COLOR_BUTTON2, OnColorButton2)
	ON_BN_CLICKED(IDC_COLOR_BUTTON3, OnColorButton3)
	ON_BN_CLICKED(IDC_COLOR_BUTTON4, OnColorButton4)
	ON_BN_CLICKED(IDC_COLOR_BUTTON5, OnColorButton5)
	ON_BN_CLICKED(IDC_COLOR_BUTTON6, OnColorButton6)
	ON_BN_CLICKED(IDC_COLOR_BUTTON7, OnColorButton7)
	ON_BN_CLICKED(IDC_COLOR_BUTTON8, OnColorButton8)
	ON_BN_CLICKED(IDC_COLOR_BUTTON9, OnColorButton9)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE0, OnSelchangeDrawstyle0)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE1, OnSelchangeDrawstyle1)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE10, OnSelchangeDrawstyle10)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE11, OnSelchangeDrawstyle11)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE12, OnSelchangeDrawstyle12)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE13, OnSelchangeDrawstyle13)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE14, OnSelchangeDrawstyle14)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE15, OnSelchangeDrawstyle15)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE16, OnSelchangeDrawstyle16)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE17, OnSelchangeDrawstyle17)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE18, OnSelchangeDrawstyle18)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE19, OnSelchangeDrawstyle19)
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
	ON_CBN_SELCHANGE(IDC_THICKNESS10, OnSelchangeThickness10)
	ON_CBN_SELCHANGE(IDC_THICKNESS11, OnSelchangeThickness11)
	ON_CBN_SELCHANGE(IDC_THICKNESS12, OnSelchangeThickness12)
	ON_CBN_SELCHANGE(IDC_THICKNESS13, OnSelchangeThickness13)
	ON_CBN_SELCHANGE(IDC_THICKNESS14, OnSelchangeThickness14)
	ON_CBN_SELCHANGE(IDC_THICKNESS15, OnSelchangeThickness15)
	ON_CBN_SELCHANGE(IDC_THICKNESS16, OnSelchangeThickness16)
	ON_CBN_SELCHANGE(IDC_THICKNESS17, OnSelchangeThickness17)
	ON_CBN_SELCHANGE(IDC_THICKNESS18, OnSelchangeThickness18)
	ON_CBN_SELCHANGE(IDC_THICKNESS19, OnSelchangeThickness19)
	ON_CBN_SELCHANGE(IDC_THICKNESS2, OnSelchangeThickness2)
	ON_CBN_SELCHANGE(IDC_THICKNESS3, OnSelchangeThickness3)
	ON_CBN_SELCHANGE(IDC_THICKNESS4, OnSelchangeThickness4)
	ON_CBN_SELCHANGE(IDC_THICKNESS5, OnSelchangeThickness5)
	ON_CBN_SELCHANGE(IDC_THICKNESS6, OnSelchangeThickness6)
	ON_CBN_SELCHANGE(IDC_THICKNESS7, OnSelchangeThickness7)
	ON_CBN_SELCHANGE(IDC_THICKNESS8, OnSelchangeThickness8)
	ON_CBN_SELCHANGE(IDC_THICKNESS9, OnSelchangeThickness9)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH0, OnCheckShowGraph0)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH1, OnCheckShowGraph1)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH10, OnCheckShowGraph10)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH11, OnCheckShowGraph11)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH12, OnCheckShowGraph12)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH13, OnCheckShowGraph13)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH14, OnCheckShowGraph14)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH15, OnCheckShowGraph15)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH16, OnCheckShowGraph16)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH17, OnCheckShowGraph17)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH18, OnCheckShowGraph18)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH19, OnCheckShowGraph19)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH2, OnCheckShowGraph2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH3, OnCheckShowGraph3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH4, OnCheckShowGraph4)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH5, OnCheckShowGraph5)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH6, OnCheckShowGraph6)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH7, OnCheckShowGraph7)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH8, OnCheckShowGraph8)
	ON_BN_CLICKED(IDC_CHECK_SHOW_GRAPH9, OnCheckShowGraph9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_PriceMA_Condition message handlers

BOOL CSetting_PriceMA_Condition::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	// save the original size
	GetWindowRect(m_rcOriginalRect);

	// initial scroll position
	m_nScrollPos = 0; 

// 2008.01.25 by LYH >>
	m_strMsgLineThick.LoadString(IDS_MSG_LINE_THICK);
	m_strMsgLineShape.LoadString(IDS_MSG_LINE_SHAPE);
	m_strMsgExistMin.LoadString(IDS_MSG_EXIST_MIN);
// 2008.01.25 by LYH <<

	CComboBox *pPacketCombo = NULL;
	CString strStdHL, strStdHLC;
	strStdHL.LoadString( IDS_STANDART_HL);
	strStdHLC.LoadString( IDS_STANDART_HLC);
	for( int i = 0; i < IDC_CMB_PACKET19 - IDC_CMB_PACKET0 + 1; i++)
	{
		pPacketCombo = ( CComboBox *)GetDlgItem( IDC_CMB_PACKET0 + i);
		if( !pPacketCombo) continue;
		pPacketCombo->ResetContent();
		pPacketCombo->AddString( _LTEXT0( C0_OPEN));
		pPacketCombo->AddString( _LTEXT0( C0_HIGH));
		pPacketCombo->AddString( _LTEXT0( C0_LOW));
		pPacketCombo->AddString( _LTEXT0( C0_CLOSE));
		pPacketCombo->AddString( strStdHL);
		pPacketCombo->AddString( strStdHLC);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetting_PriceMA_Condition::Initial(IGraphInfo* p_pIGraphInfo)
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
int CSetting_PriceMA_Condition::GetSubGraphCount( void )
{
	return m_nSubGraphCount;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return void	: 
// Param  int nCount : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_PriceMA_Condition::SetSubGraphCount( int nSubGraphCount )
{
	m_nSubGraphCount = nSubGraphCount;

	CRect rcRect;
	CWnd *pWnd = GetDlgItem( IDC_THICKNESS0 + nSubGraphCount - 1 );
	if( pWnd) pWnd->GetWindowRect(rcRect);

	GetWindowRect( m_rcOriginalRect);
	CWnd *pParent = GetParent();
	pParent->ScreenToClient( &m_rcOriginalRect);
	
	if( nSubGraphCount < PRICE_MA_CONDITION_COUNT) m_rcOriginalRect.bottom = rcRect.bottom + 1;
	
	MoveWindow( m_rcOriginalRect);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CButton*	: 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CButton* CSetting_PriceMA_Condition::GetCheckButton(const int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CButton *)GetDlgItem( IDC_CHECK_SHOW_GRAPH0 + nSubGraphIndex );

	return (CButton *)GetDlgItem( IDC_CHECK_SHOW_GRAPH0 );
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CNumEdit*	: 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CNumEdit* CSetting_PriceMA_Condition::GetEditCtrl(const int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CNumEdit *)GetDlgItem( IDC_COND_GRAPH0 + nSubGraphIndex );

	return (CNumEdit *)GetDlgItem( IDC_COND_GRAPH0 );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CNumSpinCtrl*	: 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CNumSpinCtrl* CSetting_PriceMA_Condition::GetSpinCtrl(const int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CNumSpinCtrl *)GetDlgItem( IDC_SPIN_COND0 + nSubGraphIndex );

	return (CNumSpinCtrl *)GetDlgItem( IDC_SPIN_COND0 );
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 10 / 30
// Return CBitmapItemComboBox*	: 
// Param  int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CBitmapItemComboBox* CSetting_PriceMA_Condition::GetDrawStyleCombo(int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CBitmapItemComboBox *)GetDlgItem( IDC_DRAWSTYLE0 + nSubGraphIndex );

	return (CBitmapItemComboBox *)GetDlgItem( IDC_DRAWSTYLE0 );
}

void CSetting_PriceMA_Condition::SetDrawStyle( int nSubGraphIndex, int nCmbIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
	{
		CBitmapItemComboBox *pBitmapItemComboBox = ( CBitmapItemComboBox *)GetDlgItem( IDC_DRAWSTYLE0 + nSubGraphIndex );
		if( pBitmapItemComboBox) pBitmapItemComboBox->SetCurSel( nCmbIndex );
	}
}


void CSetting_PriceMA_Condition::SetLineThickness( int nSubGraphIndex, int nCmbIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
	{
		CBitmapItemComboBox *pBitmapItemComboBox = ( CBitmapItemComboBox *)GetDlgItem( IDC_THICKNESS0 + nSubGraphIndex);
		if( pBitmapItemComboBox) pBitmapItemComboBox->SetCurSel( nCmbIndex );
	}
}


CBitmapItemComboBox* CSetting_PriceMA_Condition::GetThicknessCombo(int nSubGraphIndex)
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CBitmapItemComboBox *)GetDlgItem( IDC_THICKNESS0 + nSubGraphIndex );

	return (CBitmapItemComboBox *)GetDlgItem( IDC_THICKNESS0 );
}


void CSetting_PriceMA_Condition::SetLineColor( int nSubGraphIndex, int nColorIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
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


CComboBox *CSetting_PriceMA_Condition::GetPacketCombo( int nSubGraphIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CComboBox *)GetDlgItem(IDC_CMB_PACKET0 + nSubGraphIndex );

	return (CComboBox *)GetDlgItem(IDC_CMB_PACKET0);
}


CComboBox *CSetting_PriceMA_Condition::GetCalcMethodCombo( int nSubGraphIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CComboBox *)GetDlgItem(IDC_CMB_CALCMETHOD0 + nSubGraphIndex );

	return (CComboBox *)GetDlgItem(IDC_CMB_CALCMETHOD0);
}


CColorButton* CSetting_PriceMA_Condition::GetColorButton(int nSubGraphIndex )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
		return (CColorButton *)GetDlgItem(IDC_COLOR_BUTTON0 + nSubGraphIndex );

	return (CColorButton *)GetDlgItem(IDC_COLOR_BUTTON0);
}



void CSetting_PriceMA_Condition::ShowHideSubGraph( int nSubGraphIndex, int nCmdShow )
{
	if( nSubGraphIndex >= 0 && nSubGraphIndex < PRICE_MA_CONDITION_COUNT )
	{
		CWnd *pWnd = GetDlgItem( IDC_CHECK_SHOW_GRAPH0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_COND_GRAPH0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_SPIN_COND0 + nSubGraphIndex );
		if( pWnd) pWnd->ShowWindow( nCmdShow );
		pWnd = GetDlgItem( IDC_CMB_PACKET0 + nSubGraphIndex );
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

BOOL CSetting_PriceMA_Condition::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentDlg )
			{
				((CSetting_PriceMA *)m_pParentDlg)->OK();
			}
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentDlg )
			{
				((CSetting_PriceMA *)m_pParentDlg)->Cancel();
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSetting_PriceMA_Condition::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	if( pScrollBar != NULL || m_nSubGraphCount < 13) return;
	
	int nNewPos = m_nScrollPos;
	int nDelta =  m_rcOriginalRect.Height() / m_nSubGraphCount;

	switch (nSBCode)
	{
		case SB_LINEDOWN:	break;
		case SB_LINEUP:		nDelta *= -1;
							break;
		case SB_PAGEDOWN:	nDelta *= 13;
							break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:	nDelta = (int)nPos - m_nScrollPos;
								break;
		case SB_PAGEUP:		nDelta *= -13;
							break;
		default:			return;
	}

	nNewPos = m_nScrollPos + nDelta;
	if( nNewPos < 0) nNewPos = 0;
	else if( m_rcOriginalRect.Height() < nNewPos) nNewPos = m_rcOriginalRect.Height();
	SetScrollPos( SB_VERT, nNewPos, TRUE);
	nNewPos = GetScrollPos( SB_VERT);

	ScrollWindow( 0, m_nScrollPos - nNewPos);
	m_nScrollPos = nNewPos;
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSetting_PriceMA_Condition::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	m_nCurHeight = cy;

	if( m_nSubGraphCount < 13 )
		return;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	si.nMax = m_rcOriginalRect.Height();
	si.nPage = cy;
	si.nPos = 0;
    SetScrollInfo(SB_VERT, &si, TRUE); 		
}


void CSetting_PriceMA_Condition::SetOriginalRect(CRect rcOriginRect )
{
	m_rcOriginalRect = rcOriginRect;
}

void CSetting_PriceMA_Condition::OnColorButton0() 
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

void CSetting_PriceMA_Condition::OnColorButton1() 
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

void CSetting_PriceMA_Condition::OnColorButton10() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor10.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 10, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 10, 2, color.GetColor());
		m_btnColor10.SetColor( m_pIGraphInfo->GetSubGraphColor( 10, 1));
		m_btnColor10.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton11() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor11.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 11, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 11, 2, color.GetColor());
		m_btnColor11.SetColor( m_pIGraphInfo->GetSubGraphColor( 11, 1));
		m_btnColor11.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton12() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor12.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 12, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 12, 2, color.GetColor());
		m_btnColor12.SetColor( m_pIGraphInfo->GetSubGraphColor( 12, 1));
		m_btnColor12.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton13() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor13.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 13, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 13, 2, color.GetColor());
		m_btnColor13.SetColor( m_pIGraphInfo->GetSubGraphColor( 13, 1));
		m_btnColor13.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton14() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor14.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 14, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 14, 2, color.GetColor());
		m_btnColor14.SetColor( m_pIGraphInfo->GetSubGraphColor( 14, 1));
		m_btnColor14.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton15() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor15.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 15, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 15, 2, color.GetColor());
		m_btnColor15.SetColor( m_pIGraphInfo->GetSubGraphColor( 15, 1));
		m_btnColor15.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton16() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor16.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 16, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 16, 2, color.GetColor());
		m_btnColor16.SetColor( m_pIGraphInfo->GetSubGraphColor( 16, 1));
		m_btnColor16.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton17() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor17.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 17, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 17, 2, color.GetColor());
		m_btnColor17.SetColor( m_pIGraphInfo->GetSubGraphColor( 17, 1));
		m_btnColor17.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton18() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor18.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 18, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 18, 2, color.GetColor());
		m_btnColor18.SetColor( m_pIGraphInfo->GetSubGraphColor( 18, 1));
		m_btnColor18.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton19() 
{
	// TODO: Add your control notification handler code here
	COLORREF crColor = m_btnColor19.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( 19, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( 19, 2, color.GetColor());
		m_btnColor19.SetColor( m_pIGraphInfo->GetSubGraphColor( 19, 1));
		m_btnColor19.Invalidate();
	}	
}

void CSetting_PriceMA_Condition::OnColorButton2() 
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

void CSetting_PriceMA_Condition::OnColorButton3() 
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

void CSetting_PriceMA_Condition::OnColorButton4() 
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

void CSetting_PriceMA_Condition::OnColorButton5() 
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

void CSetting_PriceMA_Condition::OnColorButton6() 
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

void CSetting_PriceMA_Condition::OnColorButton7() 
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

void CSetting_PriceMA_Condition::OnColorButton8() 
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

void CSetting_PriceMA_Condition::OnColorButton9() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle0() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle1() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle10() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph10.GetCurSel() != 0 && m_cmbDrawStyleGraph10.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph10.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle11() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph11.GetCurSel() != 0 && m_cmbDrawStyleGraph11.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph11.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle12() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph12.GetCurSel() != 0 && m_cmbDrawStyleGraph12.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph12.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle13() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph13.GetCurSel() != 0 && m_cmbDrawStyleGraph13.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph13.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle14() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph14.GetCurSel() != 0 && m_cmbDrawStyleGraph14.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph14.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle15() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph15.GetCurSel() != 0 && m_cmbDrawStyleGraph15.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph15.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle16() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph16.GetCurSel() != 0 && m_cmbDrawStyleGraph16.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph16.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle17() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph17.GetCurSel() != 0 && m_cmbDrawStyleGraph17.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph17.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle18() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph18.GetCurSel() != 0 && m_cmbDrawStyleGraph18.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph18.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle19() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbThicknessGraph19.GetCurSel() != 0 && m_cmbDrawStyleGraph19.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선굵기가 \"1pt\"인 경우만 선모양을 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineShape);
// 2008.01.25 by LYH <<
		m_cmbThicknessGraph19.SetCurSel(0);
		return;
	}	
}


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle2() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle3() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle4() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle5() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle6() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle7() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle8() 
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


void CSetting_PriceMA_Condition::OnSelchangeDrawstyle9() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness0() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness1() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness10() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph10.GetCurSel() != 0 && m_cmbThicknessGraph10.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph10.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness11() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph11.GetCurSel() != 0 && m_cmbThicknessGraph11.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph11.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness12() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph12.GetCurSel() != 0 && m_cmbThicknessGraph12.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph12.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness13() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph13.GetCurSel() != 0 && m_cmbThicknessGraph13.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph13.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness14() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph14.GetCurSel() != 0 && m_cmbThicknessGraph14.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph14.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness15() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph15.GetCurSel() != 0 && m_cmbThicknessGraph15.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph15.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness16() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph16.GetCurSel() != 0 && m_cmbThicknessGraph16.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph16.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness17() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph17.GetCurSel() != 0 && m_cmbThicknessGraph17.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph17.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness18() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph18.GetCurSel() != 0 && m_cmbThicknessGraph18.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph18.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness19() 
{
	// TODO: Add your control notification handler code here
	if( m_cmbDrawStyleGraph19.GetCurSel() != 0 && m_cmbThicknessGraph19.GetCurSel() != 0 )
	{
// 2008.01.25 by LYH >>
		//AfxMessageBox("선모양이 \"실선\"인 경우만 굵기를 변경할 수 있습니다.");
		AfxMessageBox(m_strMsgLineThick);
// 2008.01.25 by LYH <<
		m_cmbDrawStyleGraph19.SetCurSel(0);
		return;
	}
}


void CSetting_PriceMA_Condition::OnSelchangeThickness2() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness3() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness4() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness5() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness6() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness7() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness8() 
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


void CSetting_PriceMA_Condition::OnSelchangeThickness9() 
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


void CSetting_PriceMA_Condition::OnCheckShowGraph0() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH0 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph1() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH1 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph10() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH10 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph11() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH11 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph12() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH12 );	
}


void CSetting_PriceMA_Condition::OnCheckShowGraph13() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH13 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph14() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH14 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph15() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH15 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph16() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH16 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph17() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH17 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph18() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH18 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph19() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH19 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph2() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH2 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph3() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH3 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph4() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH4 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph5() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH5 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph6() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH6 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph7() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH7 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph8() 
{
	// TODO: Add your control notification handler code here
	WhenAllSubGraphisHide( IDC_CHECK_SHOW_GRAPH8 );
}


void CSetting_PriceMA_Condition::OnCheckShowGraph9() 
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
void CSetting_PriceMA_Condition::WhenAllSubGraphisHide( const int p_nCheckID )
{
	CButton *pButton = ( CButton *)GetDlgItem( p_nCheckID );
	if( !pButton) return;
	if( pButton->GetCheck() ) return;

	int nCheckCount = 0;
	
	CButton *pBtn = NULL;
	for( int i = 0 ; i < m_nSubGraphCount ; i++ )
	{
		pBtn = GetCheckButton(i);
		if( pBtn) if( pBtn->GetCheck()) nCheckCount++;
	}

	if( 0 < nCheckCount)
		return;

// 2008.01.25 by LYH <<
	//AfxMessageBox("최소 하나 이상의 구성설정이 존재해야 됩니다.");
	AfxMessageBox(m_strMsgExistMin);
// 2008.01.25 by LYH >>
	pButton->SetCheck(1);
	
	SendMessage( WM_COMMAND, p_nCheckID);
}
