// DlgMainTabStyleSetUp.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "DlgMainTabStyleSetUp.h"

#include <IO.H>
#include "DlgEnvSetUpAll.h"
#include "Conversion.h"
// 2008.01.25 by LYH >>
//#include "DefineData.h"
// 2008.01.25 by LYH <<
#include "TabMainChartEnvSetUp.h"

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
// 2008.01.25 by LYH <<

#define TYPE_DEFAULT 0
#define	TYPE_USER_SAVE 1

#define	NUM_ITEM	32

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabStyleSetUp dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CStyleData *pStyleData : 
// Param  CWnd *pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CDlgMainTabStyleSetUp::CDlgMainTabStyleSetUp( IChartOCX *p_pIChartOCX, CWnd *pParent /*=NULL*/ )
	: CBaseTabDlg(CDlgMainTabStyleSetUp::IDD, pParent)
//	: CDialog(CDlgMainTabStyleSetUp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMainTabStyleSetUp)
	m_grid_width = 0;
	m_grid_height = 0;
	m_divide_width = -1;
	m_divide_height = -1;
	m_divide_radio = -1;
	//}}AFX_DATA_INIT

	m_pIChartOCX = p_pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();

	m_nStyle = 0;
	m_styleIndex = 0;
	m_pTitleFont = NULL;
	m_divideIndex = 0;
	m_bPreview = FALSE;
	m_pStyleData = NULL;
	m_bColorChanged = FALSE;

	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	m_bSkipAnalysisColor = TRUE;

	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	m_bSkipVerticalScaleGrid = TRUE;
	m_bSkipHorizontalScaleGrid = TRUE;

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}

IMPLEMENT_DYNAMIC( CDlgMainTabStyleSetUp, CBaseTabDlg)

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CDlgMainTabStyleSetUp::~CDlgMainTabStyleSetUp()
{
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}

	while( !m_lstTempUserStyle.IsEmpty()) delete m_lstTempUserStyle.RemoveHead();
}


void CDlgMainTabStyleSetUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMainTabStyleSetUp)
	DDX_Control(pDX, IDC_EQUALBUTTON, m_equal_Bt);
	DDX_Control(pDX, IDC_NUMERICAL_COLOR, m_numerical_Bt);
	DDX_Control(pDX, IDC_ANALYZE_COLOR, m_analyze_Bt);
	DDX_Control(pDX, IDC_DIVIDECOMBO, m_divideCombo);
	DDX_Control(pDX, IDC_DEL_STYLE, m_del_style_Bt);
	DDX_Control(pDX, IDC_RESET_STYLE, m_reset_style_Bt);
	DDX_Control(pDX, IDC_STYLECOMBO, m_styleCombo);
	DDX_Control(pDX, IDC_INDICESCOMBO, m_indicesCombo);
	DDX_Control(pDX, IDC_AVERAGECOMBO, m_averageCombo);
	DDX_Control(pDX, IDC_UPBUTTON, m_up_Bt);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_down_Bt);
	DDX_Control(pDX, IDC_AVERAGE_BT, m_average_Bt);
	DDX_Control(pDX, IDC_DOWNFILL, m_bDownFill);
	DDX_Control(pDX, IDC_UPFILL, m_bUpFill);
	DDX_Control(pDX, IDC_GRAPH_BACKGROUND, m_graph_background_Bt);
	DDX_Control(pDX, IDC_GRAPH_BORDER, m_graph_border_Bt);
	DDX_Control(pDX, IDC_GRAPH_GRID, m_graph_grid_Bt);
	DDX_Control(pDX, IDC_SCALE_BACKGROUND, m_scale_background_Bt);
	DDX_Control(pDX, IDC_SCALE_BORDER, m_scale_border_Bt);
	DDX_Control(pDX, IDC_SCALE_TEXT, m_scale_text_Bt);
	DDX_Control(pDX, IDC_SCALE_GRID, m_scale_grid_Bt);
	DDX_Text(pDX, IDC_GRID_WIDTH, m_grid_width);
	DDV_MinMaxInt(pDX, m_grid_width, 0, 100);
	DDX_Text(pDX, IDC_GRID_HEIGHT, m_grid_height);
	DDV_MinMaxInt(pDX, m_grid_height, 0, 100);
	DDX_Radio(pDX, IDC_DIVIDE_RADIO, m_divide_radio);

	DDX_Control(pDX, IDC_CMB_HSCALE_GRIDTYPE_DAY, m_cmbHScaleGridType_Day);
	DDX_Control(pDX, IDC_CMB_HSCALE_GRIDTYPE_WEEK, m_cmbHScaleGridType_Week);
	DDX_Control(pDX, IDC_CMB_HSCALE_GRIDTYPE_MON, m_cmbHScaleGridType_Mon);
	DDX_Control(pDX, IDC_CMB_HSCALE_GRIDTYPE_YEAR, m_cmbHScaleGridType_Year);

	DDX_Control(pDX, IDC_SCALELINE_THICKNESS_DAY, m_cmbScaleGridThickness_Day);
	DDX_Control(pDX, IDC_SCALELINE_THICKNESS_MON, m_cmbScaleGridThickness_Mon);
	DDX_Control(pDX, IDC_SCALELINE_THICKNESS_YEAR, m_cmbScaleGridThickness_Year);
	DDX_Control(pDX, IDC_SCALELINE_THICKNESS_WEEK, m_cmbScaleGridThickness_Week);

	DDX_Control(pDX, IDC_BTN_HSCALE_GRIDCOLOR_DAY, m_btnHScaleGridColor_Day);
	DDX_Control(pDX, IDC_BTN_HSCALE_GRIDCOLOR_WEEK, m_btnHScaleGridColor_Week);
	DDX_Control(pDX, IDC_BTN_HSCALE_GRIDCOLOR_MON, m_btnHScaleGridColor_Mon);
	DDX_Control(pDX, IDC_BTN_HSCALE_GRIDCOLOR_YEAR, m_btnHScaleGridColor_Year);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMainTabStyleSetUp, CDialog)
	//{{AFX_MSG_MAP(CDlgMainTabStyleSetUp)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_STYLECOMBO, OnSelchangeStylecombo)
	ON_CBN_SELCHANGE(IDC_INDICESCOMBO, OnSelchangeIndicescombo)
	ON_CBN_SELCHANGE(IDC_AVERAGECOMBO, OnSelchangeAveragecombo)
	ON_BN_CLICKED(IDC_UPBUTTON, OnUpbutton)
	ON_BN_CLICKED(IDC_DOWNBUTTON, OnDownbutton)
	ON_BN_CLICKED(IDC_AVERAGE_BT, OnAverageBt)
	ON_BN_CLICKED(IDC_UPFILL, OnUpfill)
	ON_BN_CLICKED(IDC_DOWNFILL, OnDownfill)
	ON_BN_CLICKED(IDC_GRAPH_BACKGROUND, OnGraphBackground)
	ON_BN_CLICKED(IDC_GRAPH_BORDER, OnGraphBorder)
	ON_BN_CLICKED(IDC_GRAPH_GRID, OnGraphGrid)
	ON_BN_CLICKED(IDC_SCALE_BACKGROUND, OnScaleBackground)
	ON_BN_CLICKED(IDC_SCALE_BORDER, OnScaleBorder)
	ON_BN_CLICKED(IDC_ADD_STYLE, OnAddStyle)
	ON_BN_CLICKED(IDC_DEL_STYLE, OnDelStyle)
	ON_BN_CLICKED(IDC_RESET_STYLE, OnResetStyle)
	ON_EN_CHANGE(IDC_GRID_WIDTH, OnChangeGridWidth)
	ON_EN_CHANGE(IDC_GRID_HEIGHT, OnChangeGridHeight)
	ON_BN_CLICKED(IDC_SCALE_GRID, OnScaleGrid)
	ON_BN_CLICKED(IDC_SCALE_TEXT, OnScaleText)
	ON_CBN_EDITCHANGE(IDC_STYLECOMBO, OnEditchangeStylecombo)
	ON_CBN_SELCHANGE(IDC_DIVIDECOMBO, OnSelchangeDividecombo)
	ON_BN_CLICKED(IDC_DIVIDE_RADIO, OnDivideRadio)
	ON_BN_CLICKED(IDC_DIVIDE_R1, OnDivideR1)
	ON_BN_CLICKED(IDC_DIVIDE_R2, OnDivideR2)
	ON_BN_CLICKED(IDC_DIVIDE_R3, OnDivideR3)
	ON_BN_CLICKED(IDC_ANALYZE_COLOR, OnAnalyzeColor)
	ON_BN_CLICKED(IDC_NUMERICAL_COLOR, OnNumericalColor)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EQUALBUTTON, OnEqualbutton)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBtnRecover)
	ON_BN_CLICKED(IDC_BTN_PREVIEW, OnBtnPreview)
	ON_WM_CREATE()

	ON_CBN_SELCHANGE(IDC_CMB_HSCALE_GRIDTYPE_DAY, OnSelchangeCmbHscaleGridtype_Day)
	ON_CBN_SELCHANGE(IDC_CMB_HSCALE_GRIDTYPE_WEEK, OnSelchangeCmbHscaleGridtype_Week)
	ON_CBN_SELCHANGE(IDC_CMB_HSCALE_GRIDTYPE_MON, OnSelchangeCmbHscaleGridtype_Mon)
	ON_CBN_SELCHANGE(IDC_CMB_HSCALE_GRIDTYPE_YEAR, OnSelchangeCmbHscaleGridtype_Year)

	ON_CBN_SELCHANGE(IDC_SCALELINE_THICKNESS_DAY, OnSelchangeScalelineThickness_Day)
	ON_CBN_SELCHANGE(IDC_SCALELINE_THICKNESS_WEEK, OnSelchangeScalelineThickness_Week)
	ON_CBN_SELCHANGE(IDC_SCALELINE_THICKNESS_MON, OnSelchangeScalelineThickness_Mon)
	ON_CBN_SELCHANGE(IDC_SCALELINE_THICKNESS_YEAR, OnSelchangeScalelineThickness_Year)

	ON_BN_CLICKED(IDC_BTN_HSCALE_GRIDCOLOR_DAY,		OnBtnHScaleGridColor_Day)
	ON_BN_CLICKED(IDC_BTN_HSCALE_GRIDCOLOR_WEEK,	OnBtnHScaleGridColor_Week)
	ON_BN_CLICKED(IDC_BTN_HSCALE_GRIDCOLOR_MON,		OnBtnHScaleGridColor_Mon)
	ON_BN_CLICKED(IDC_BTN_HSCALE_GRIDCOLOR_YEAR,	OnBtnHScaleGridColor_Year)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabStyleSetUp message handlers


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return BOOL : 
// Comments		: Style SetUp Page 초기화
//-----------------------------------------------------------------------------
BOOL CDlgMainTabStyleSetUp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( !m_pIChartOCX) return FALSE;
	
	// 1. Initialize the Style Manger with Defualt and User
	m_nStyle = m_styleDataManager.Initialize( m_pIChartOCX, TRUE);
	m_styleDataManager.Initialize( m_pIChartOCX);

	// 2. Add Current Style to User Style List
	m_styleDataManager.CreateDummyUserStyle( m_pIChartOCX);

	// 2.5 Create Temp StyleData List of User
	m_styleDataManager.CopyStyleDataListTo( m_lstTempUserStyle);

//	// 3. Init Font
//	if( !m_pTitleFont)
//	{
//		m_pTitleFont = new CFont();
//
//		LOGFONT	logFont;
//// 2008.01.25 by LYH >>
//		//logFont.lfHeight = 12;
//// 2008.01.25 by LYH <<
//		logFont.lfWeight = 0;
//		logFont.lfEscapement = 0;
//		logFont.lfOrientation = 0;
//		logFont.lfWeight = FW_THIN;
//		logFont.lfItalic = FALSE;
//		logFont.lfUnderline = FALSE;
//		logFont.lfStrikeOut = FALSE;
//// 2008.01.25 by LYH >>
//		//logFont.lfCharSet = DEFAULT_CHARSET;
//		//strcpy(logFont.lfFaceName, "굴림체");
//		CString strFont;
//		strFont.LoadString(IDS_STRING_FONT);
//
//		if(strFont.Find("MS") < 0)	//굴림체
//		{
//			logFont.lfHeight = 12;
//			logFont.lfCharSet = DEFAULT_CHARSET;
//		}
//		else
//		{
//			logFont.lfHeight = 14;
//		}
//		strcpy(logFont.lfFaceName, strFont);
//// 2008.01.25 by LYH <<
//
//		m_pTitleFont->CreateFontIndirect( &logFont );
//	}

	// 4. Init the Style Name Combo Control
	CStyleData *pStyleData = NULL;
	POSITION pos = m_lstTempUserStyle.GetHeadPosition();
	while( pos)
	{
		pStyleData = m_lstTempUserStyle.GetNext(pos);
		m_styleCombo.AddString( pStyleData->GetStyleName());
	}

	// 5. Init Chart Combo
// 2008.01.25 by LYH >>
	//m_indicesCombo.AddString("가격");
	//m_indicesCombo.AddString("거래량");
	m_indicesCombo.AddString(_LTEXT3(C3_PRICE));
	// (2009/9/21 - Seung-Won, Bae) for FX
	if( CChartInfo::FX != m_pIChartOCX->GetChartMode())
		m_indicesCombo.AddString(_LTEXT3(C3_VOLUME));
// 2008.01.25 by LYH <<

//	0023294: [7111] 시간 구분선 설정 반영 가능 여부 문의 ----->>
	Initial_ThicknessComboBox( m_cmbScaleGridThickness_Day,		5,	1);
	Initial_ThicknessComboBox( m_cmbScaleGridThickness_Week,	5,	1);
	Initial_ThicknessComboBox( m_cmbScaleGridThickness_Mon,		5,	1);
	Initial_ThicknessComboBox( m_cmbScaleGridThickness_Year,	5,	1);

	Initial_LineStyleComboBox( m_cmbHScaleGridType_Day,		3,	1,	1);
	Initial_LineStyleComboBox( m_cmbHScaleGridType_Week,	3,	1,	1);
	Initial_LineStyleComboBox( m_cmbHScaleGridType_Mon,		3,	1,	1);
	Initial_LineStyleComboBox( m_cmbHScaleGridType_Year,	3,	1,	1);
//	0023294: [7111] 시간 구분선 설정 반영 가능 여부 문의 <<-----

	// 6. Init Grid Line Combo
// 2008.01.25 by LYH >>
//	m_divideCombo.AddString("가로 구분선");
//	m_divideCombo.AddString("세로 구분선");

	CString strHorz, strVert;
	strHorz.LoadString(IDS_HORZ_LINE);
	strVert.LoadString(IDS_VERT_LINE);
	m_divideCombo.AddString(strHorz);
	m_divideCombo.AddString(strVert);
// 2008.01.25 by LYH <<
	// 7. Show Last Style (Current Style)
	m_styleCombo.SetCurSel( m_lstTempUserStyle.GetCount() - 1);
	OnSelchangeStylecomboMain();

	UpdateData(FALSE);
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnResetStyle() 
{
	int nIndex = m_styleCombo.GetCurSel();
	CStyleData* pDefaultStyleData	= m_styleDataManager.GetStyleData( nIndex, TRUE);
	POSITION psnStyle = m_lstTempUserStyle.FindIndex( nIndex);
	if( !psnStyle) return;
	CStyleData* pUserStyleData		= m_lstTempUserStyle.GetAt( psnStyle);
	if( pDefaultStyleData == NULL || pUserStyleData == NULL) return;
	*pUserStyleData = *pDefaultStyleData;

	OnSelchangeStylecombo();
	UpdateData( FALSE);
	Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnSelchangeStylecomboMain( void) 
{
	m_styleIndex = m_styleCombo.GetCurSel();
	POSITION psnStyle = m_lstTempUserStyle.FindIndex( m_styleIndex);
	if( !psnStyle) return;
	m_pStyleData = m_lstTempUserStyle.GetAt( psnStyle);
	if( !m_pStyleData) return;

	if( m_styleIndex < m_nStyle)
	{
		m_del_style_Bt.EnableWindow(FALSE);
		m_reset_style_Bt.EnableWindow(TRUE);
	}
	else			 		// 기타 사용자 추가된 것들
	{
		m_del_style_Bt.EnableWindow(TRUE);
		m_reset_style_Bt.EnableWindow(FALSE);
	}

	SetColor(&m_graph_background_Bt,	m_pStyleData->GetGraphBGColor());
	SetColor(&m_graph_border_Bt,		m_pStyleData->GetGraphBorderColor());
	SetColor(&m_graph_grid_Bt,			m_pStyleData->GetGraphGridColor());
	SetColor(&m_scale_background_Bt,	m_pStyleData->GetScaleBGColor());
	SetColor(&m_scale_border_Bt,		m_pStyleData->GetScaleBorderColor());
	SetColor(&m_scale_text_Bt,			m_pStyleData->GetScaleTextColor());
	SetColor(&m_scale_grid_Bt,			m_pStyleData->GetScaleGridColor());
	SetColor(&m_analyze_Bt,				m_pStyleData->GetAnalyzeColor());
	SetColor(&m_numerical_Bt,			m_pStyleData->GetNumericalColor());

	m_grid_width = m_pStyleData->GetGridWidth();
	m_grid_height = m_pStyleData->GetGridHeight();
	m_divide_height = m_pStyleData->GetDivideWidth();
	m_divide_width = m_pStyleData->GetDivideHeight();


	//KHD : 시간구분
	m_cmbHScaleGridType_Day.SetCurSel(		m_pStyleData->GetHScaleGridType_Day());	//? m_pStyleData->GetHScaleGridType_Mon() : ( 3 != m_pStyleData->GetHScaleGridType() ? m_pStyleData->GetHScaleGridType() : 1));
	m_cmbHScaleGridType_Week.SetCurSel(		m_pStyleData->GetHScaleGridType_Week());	//? m_pStyleData->GetHScaleGridType_Day() : ( 3 != m_pStyleData->GetHScaleGridType() ? m_pStyleData->GetHScaleGridType() : 1));
	m_cmbHScaleGridType_Mon.SetCurSel(		m_pStyleData->GetHScaleGridType_Mon());	//? m_pStyleData->GetHScaleGridType_Day() : ( 3 != m_pStyleData->GetHScaleGridType() ? m_pStyleData->GetHScaleGridType() : 1));
	m_cmbHScaleGridType_Year.SetCurSel(		m_pStyleData->GetHScaleGridType_Year());	//? m_pStyleData->GetHScaleGridType_Day() : ( 3 != m_pStyleData->GetHScaleGridType() ? m_pStyleData->GetHScaleGridType() : 1));

	m_btnHScaleGridColor_Day.SetColor(		m_pStyleData->GetHScaleGridColor_Day());
	m_btnHScaleGridColor_Week.SetColor(		m_pStyleData->GetHScaleGridColor_Week());
	m_btnHScaleGridColor_Mon.SetColor(		m_pStyleData->GetHScaleGridColor_Mon());
	m_btnHScaleGridColor_Year.SetColor(		m_pStyleData->GetHScaleGridColor_Year());

	//굵기 
	m_cmbScaleGridThickness_Day.SetCurSel(		m_pStyleData->GetScaleLineThickness_Day());
	m_cmbScaleGridThickness_Week.SetCurSel(		m_pStyleData->GetScaleLineThickness_Week());
	m_cmbScaleGridThickness_Mon.SetCurSel(		m_pStyleData->GetScaleLineThickness_Mon());
	m_cmbScaleGridThickness_Year.SetCurSel(		m_pStyleData->GetScaleLineThickness_Year());

	m_indicesCombo.SetCurSel(0);
	OnSelchangeIndicescombo();

	m_divideCombo.SetCurSel(0);
	OnSelchangeDividecombo();

	UpdateData(FALSE);

	Invalidate();
}
void CDlgMainTabStyleSetUp::OnSelchangeStylecombo() 
{
	OnSelchangeStylecomboMain();

	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	m_bSkipAnalysisColor = FALSE;

	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	m_bSkipVerticalScaleGrid = FALSE;
	m_bSkipHorizontalScaleGrid = FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnSelchangeIndicescombo() 
{
	int nIndex = m_indicesCombo.GetCurSel();
	if(nIndex == 0)
	{
		SetColor(&m_up_Bt,		m_pStyleData->GetPriceUpColor());
		SetColor(&m_down_Bt,	m_pStyleData->GetPriceDownColor());
		SetColor(&m_equal_Bt,	m_pStyleData->GetPriceEqualColor());//sy 2005.04.19.
		m_bUpFill.SetCheck(m_pStyleData->GetPriceUpFill());
		m_bDownFill.SetCheck(m_pStyleData->GetPriceDownFill());
		//sy 2005.04.19.
		AddString_Combo(0);
	}
	else if(nIndex == 1)
	{
		SetColor(&m_up_Bt,		m_pStyleData->GetTradeUpColor());
		SetColor(&m_down_Bt,	m_pStyleData->GetTradeDownColor());
		SetColor(&m_equal_Bt,	m_pStyleData->GetTradeEqualColor());
		m_bUpFill.SetCheck(m_pStyleData->GetTradeUpFill());
		m_bDownFill.SetCheck(m_pStyleData->GetTradeDownFill());
		//sy 2005.04.19.
		AddString_Combo(1);
	}
	OnSelchangeAveragecombo();

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnSelchangeAveragecombo() 
{
	int nIndices = m_indicesCombo.GetCurSel();
	int nAverage = m_averageCombo.GetCurSel();
	// 가격
	if( nIndices == 0)		SetColor( &m_average_Bt, m_pStyleData->GetPriceMAColor( nAverage));
	// 거래량
	else if(nIndices == 1)	SetColor( &m_average_Bt, m_pStyleData->GetTradeMAColor( nAverage));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnSelchangeDividecombo() 
{
	m_divideIndex = m_divideCombo.GetCurSel();

	if(m_divideIndex == 0)		// 가로구분선
		m_divide_radio = m_pStyleData->GetDivideHeight();
	else if(m_divideIndex == 1)	// 세로구분선
		m_divide_radio = m_pStyleData->GetDivideWidth();
	
	UpdateData(FALSE);

	InvalidateRect(CRect(10,11,176,269));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return BOOL	: 
// Param  MSG* pMsg : 
// Comments		: Enter Key 처리
//-----------------------------------------------------------------------------
BOOL CDlgMainTabStyleSetUp::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN )
		{
			//OnBtnSave();
			if( m_pParentWnd != NULL )
				( (CDlgEnvSetUpAll *)m_pParentWnd)->OK();
			return TRUE;
		}
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
				( (CDlgEnvSetUpAll *)m_pParentWnd)->Cancel();
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return int : 
// Param  const CString &strFilePath : 
// Comments		: 
//-----------------------------------------------------------------------------
int CDlgMainTabStyleSetUp::WriteUserStyleFile( void)
{
	// <현화면스타일>은 저장하지 안는다.
	m_styleDataManager.CopyStyleDataListFrom( m_lstTempUserStyle);
	ML_SET_LANGUAGE_RES();
	m_styleDataManager.RemoveDummyUserStyle();
	return m_styleDataManager.WriteStyleFile( m_pIChartOCX);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CColorButton* pButton : 
// Param  const COLORREF &color : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::SetColor( CColorButton *pButton, const COLORREF &color )
{
	pButton->SetColor( color );
	pButton->Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnPaint() 
{
	CRect rect( 10, 11, 176, 269 );
	CButton *pButton = ( CButton * )GetDlgItem( IDC_DRAW_STATIC );
	
	if( pButton != NULL )
	{
		// ( 2006 / 10 / 31 - Sang-Woo, Cho ) - IDC_DRAW_STATIC을 따라 움직이게 수정.
		pButton->GetWindowRect(rect);
		ScreenToClient(rect);

		rect.OffsetRect( 5, 5 );
		rect.DeflateRect( 0, 7, 10, 12 );
	}

	CPaintDC dc(this);
	dc.SetMapMode(MM_TEXT);

	CChartInfo::CHART_MODE eChartMode = ( CChartInfo::CHART_MODE)m_pIChartOCX->GetChartMode();
	m_styleDataManager.DrawSampleImage( &dc, rect, m_pStyleData, eChartMode, m_pTitleFont, m_pIChartOCX->GetOcxHwnd(), 42);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  const int nType : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::AddString_Combo(const int nType)
{
	m_averageCombo.ResetContent();

	CList<CString, CString>* pTitleDataList = NULL;
	
	// Sang-Woo, Cho - Test ~ because the title is not update in Evey Style Data.
	CStyleData tmpStyleData;
	m_styleDataManager.GetStyleDataFromOCX( tmpStyleData, m_pIChartOCX);

	if(nType == 0)
	{
		pTitleDataList = tmpStyleData.GetMATitleListData().GetPriceMATitleList_Pointer();
	}
	else if(nType == 1)
	{
		pTitleDataList = tmpStyleData.GetMATitleListData().GetTradeMATitleList_Pointer();
	}

	if(pTitleDataList == NULL)
		return;

	POSITION pos = pTitleDataList->GetHeadPosition();
	while(pos != NULL)
	{
// 2008.01.25 by LYH >>
		//m_averageCombo.AddString(pTitleDataList->GetNext(pos));
		CString strTemp = pTitleDataList->GetNext(pos);
		CString strTitle = _MTOL1(strTemp);
		
		m_averageCombo.AddString(strTitle);
// 2008.01.25 by LYH <<
	}

	m_averageCombo.SetCurSel(0);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnAverageBt() 
{
	int nIndices = m_indicesCombo.GetCurSel();
	int nAverage = m_averageCombo.GetCurSel();

	//sy 2005.04.19.
	COLORREF	color = DoModal_ColorDialog(&m_average_Bt);
	if(m_bColorChanged)
	{
		if(nIndices == 0) m_pStyleData->SetPriceMAColor( nAverage, color);
		else if(nIndices == 1) m_pStyleData->SetTradeMAColor( nAverage, color);

		m_bColorChanged = FALSE;
	}
	//sy end

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnUpbutton() 
{
	COLORREF	color = DoModal_ColorDialog(&m_up_Bt);
	if(m_bColorChanged)
	{
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceUpColor(color);
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeUpColor(color);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDownbutton() 
{
	COLORREF	color = DoModal_ColorDialog(&m_down_Bt);
	if(m_bColorChanged)
	{
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceDownColor(color);	
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeDownColor(color);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnEqualbutton() 
{
	COLORREF	color = DoModal_ColorDialog(&m_equal_Bt);
	if(m_bColorChanged)
	{
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceEqualColor(color);	
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeEqualColor(color);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnUpfill() 
{
	if(m_bUpFill.GetCheck())
	{
		m_bUpFill.SetCheck(1);
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceUpFill(1);
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeUpFill(1);
	}
	else
	{
		m_bUpFill.SetCheck(0);
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceUpFill(0);
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeUpFill(0);
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDownfill() 
{
	if(m_bDownFill.GetCheck())
	{
		m_bDownFill.SetCheck(1);
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceDownFill(1);
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeDownFill(1);
	}
	else
	{
		m_bDownFill.SetCheck(0);
		if(m_indicesCombo.GetCurSel() == 0)
			m_pStyleData->SetPriceDownFill(0);
		else if(m_indicesCombo.GetCurSel() == 1)
			m_pStyleData->SetTradeDownFill(0);
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnGraphBackground() 
{
	COLORREF	color = DoModal_ColorDialog(&m_graph_background_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetGraphBGColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnGraphBorder() 
{
	COLORREF	color = DoModal_ColorDialog(&m_graph_border_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetGraphBorderColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnGraphGrid() 
{
	COLORREF	color = DoModal_ColorDialog(&m_graph_grid_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetGraphGridColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnScaleBackground() 
{
	COLORREF	color = DoModal_ColorDialog(&m_scale_background_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetScaleBGColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnScaleBorder() 
{
	COLORREF	color = DoModal_ColorDialog(&m_scale_border_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetScaleBorderColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();	
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnScaleText() 
{
	COLORREF	color = DoModal_ColorDialog(&m_scale_text_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetScaleTextColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnScaleGrid() 
{
	COLORREF	color = DoModal_ColorDialog(&m_scale_grid_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetScaleGridColor(color);	
		m_bColorChanged = FALSE;
	}

	Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return COLORREF : 
// Param  CColorButton* pButton : 
// Comments		: 
//-----------------------------------------------------------------------------
COLORREF CDlgMainTabStyleSetUp::DoModal_ColorDialog(CColorButton* pButton)
{
	CColorDialog dlgColor;
	COLORREF	 color;

	if(dlgColor.DoModal() == IDOK)
	{
		color = dlgColor.GetColor();
		SetColor(pButton, color);
		m_bColorChanged = TRUE;
	}

	return color;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return bool : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CDlgMainTabStyleSetUp::IsAddStyle() const
{
	CString strCurText;
	m_styleCombo.GetWindowText(strCurText);
	
	if(m_styleCombo.FindStringExact(0, strCurText) == CB_ERR)
		return true;

	ML_SET_LANGUAGE_RES();
	AfxMessageBox( IDS_MSG_SAME_NAME );
	return false;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnAddStyle() 
{
	if(!IsAddStyle())
		return;

	AddUserStyle();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::AddUserStyle()
{
	UpdateData(TRUE);
	CString strCurText;
	m_styleCombo.GetWindowText(strCurText);

	CStyleData* pNewStyleData = new CStyleData(*m_pStyleData);
	pNewStyleData->SetStyleName(strCurText);
	m_lstTempUserStyle.AddTail( pNewStyleData);

	m_styleCombo.AddString(strCurText);

	m_styleCombo.SetCurSel( m_styleCombo.GetCount() - 1);
	OnSelchangeStylecombo();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDelStyle() 
{
	int nIndex = m_styleCombo.GetCurSel();
	if(!DeleteUserStyle(nIndex))
		return;

	m_styleCombo.DeleteString(nIndex);
	if(nIndex == m_lstTempUserStyle.GetCount())
		m_styleCombo.SetCurSel(nIndex -1);
	else
		m_styleCombo.SetCurSel(nIndex);

	OnSelchangeStylecombo();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnChangeGridWidth() 
{
	UpdateData(TRUE);
	m_pStyleData->SetGridWidth(m_grid_width);

	UpdateData(FALSE);
	Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnChangeGridHeight() 
{
	UpdateData(TRUE);
	m_pStyleData->SetGridHeight(m_grid_height);

	UpdateData(FALSE);
	Invalidate();		
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnEditchangeStylecombo() 
{
	m_del_style_Bt.EnableWindow(FALSE);
	m_reset_style_Bt.EnableWindow(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDivideRadio() 
{
	if(m_divideIndex == 0)
	{
		m_divide_width = 0;
		m_pStyleData->SetDivideHeight(m_divide_width);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipVerticalScaleGrid = FALSE;
	}
	else
	{
		m_divide_height = 0;
		m_pStyleData->SetDivideWidth(m_divide_height);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipHorizontalScaleGrid = FALSE;
	}

	InvalidateRect(CRect(10,11,176,269));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDivideR1() 
{
	if(m_divideIndex == 0)
	{
		m_divide_width = 1;
		m_pStyleData->SetDivideHeight(m_divide_width);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipVerticalScaleGrid = FALSE;
	}
	else
	{
		m_divide_height = 1;
		m_pStyleData->SetDivideWidth(m_divide_height);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipHorizontalScaleGrid = FALSE;
	}

	InvalidateRect(CRect(10,11,176,269));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDivideR2() 
{
	if(m_divideIndex == 0)
	{
		m_divide_width = 2;
		m_pStyleData->SetDivideHeight(m_divide_width);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipVerticalScaleGrid = FALSE;
	}
	else
	{
		m_divide_height = 2;
		m_pStyleData->SetDivideWidth(m_divide_height);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipHorizontalScaleGrid = FALSE;
	}

	InvalidateRect(CRect(10,11,176,269));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDivideR3() 
{
	if(m_divideIndex == 0)	// 가로구분선
	{
		m_divide_width = 3;
		m_pStyleData->SetDivideHeight(m_divide_width);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipVerticalScaleGrid = FALSE;
	}
	else					// 세로구분선
	{
		m_divide_height = 3;
		m_pStyleData->SetDivideWidth(m_divide_height);

		// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
		m_bSkipHorizontalScaleGrid = FALSE;
	}

	InvalidateRect(CRect(10,11,176,269));
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnAnalyzeColor() 
{
	COLORREF	color = DoModal_ColorDialog(&m_analyze_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetAnalyzeColor(color);
		m_bColorChanged = FALSE;

		// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
// 2008.01.25 by LYH >>
		//if( m_pStyleData->GetStyleName() == "현화면스타일")
		CString strStyle;
		strStyle.LoadString(IDS_CURRENT_STYLE);
		if( m_pStyleData->GetStyleName() == strStyle)
// 2008.01.25 by LYH <<
			( ( CTabMainChartEnvSetUp *)GetParent())->UpdateAnalysisColorInAnalysisTool( color);

		// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
		m_bSkipAnalysisColor = FALSE;
	}

	Invalidate();
}

// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
void CDlgMainTabStyleSetUp::UpdateAnalysisColor( COLORREF p_clrAnalysis)
{
	CStyleData *pStyleData = NULL;
	POSITION psnStyle = m_lstTempUserStyle.GetHeadPosition();

// 2008.01.25 by LYH >>
	CString strStyle;
	strStyle.LoadString(IDS_CURRENT_STYLE);
// 2008.01.25 by LYH <<
	while( psnStyle)
	{
		pStyleData = m_lstTempUserStyle.GetNext( psnStyle);
// 2008.01.25 by LYH >>
		//if( pStyleData->GetStyleName() == "현화면스타일")
		if( pStyleData->GetStyleName() == strStyle)
// 2008.01.25 by LYH <<
			pStyleData->SetAnalyzeColor( p_clrAnalysis);
	}

// 2008.01.25 by LYH >>
	//if( m_pStyleData->GetStyleName() == "현화면스타일")
	if( m_pStyleData->GetStyleName() == strStyle)
// 2008.01.25 by LYH <<
		SetColor( &m_analyze_Bt, p_clrAnalysis);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnNumericalColor() 
{
	COLORREF	color = DoModal_ColorDialog(&m_numerical_Bt);
	if(m_bColorChanged)
	{
		m_pStyleData->SetNumericalColor(color);
		m_bColorChanged = FALSE;
	}

	Invalidate();			
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return CColorButton * : 
// Comments		: 
//-----------------------------------------------------------------------------
CColorButton *CDlgMainTabStyleSetUp::GetAnalyzeButton()
{
	return &m_analyze_Bt;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return CStyleData* : 
// Param  const CList<CStyleData*, CStyleData*>& styleList : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
CStyleData* CDlgMainTabStyleSetUp::GetStyleData(const CList<CStyleData*, CStyleData*>& styleList, const int nIndex) const
{
	POSITION pos = styleList.FindIndex(nIndex);
	if(pos == NULL)
		return NULL;

	return styleList.GetAt(pos);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  CList<CStyleData*, CStyleData*>& styleList : 
// Comments		: styleList를 초기화 한다.
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::DeleteAll( CList<CStyleData*, CStyleData*>& styleList )
{
	POSITION pos = styleList.GetHeadPosition();
	while( pos != NULL )
	{
		CStyleData *pStyleData = styleList.GetNext( pos );
		delete pStyleData;
	}
	
	styleList.RemoveAll();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return bool : 
// Param  const int nIndex : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CDlgMainTabStyleSetUp::DeleteUserStyle(const int nIndex)
{
	POSITION psnStyle = m_lstTempUserStyle.FindIndex( nIndex);
	if( !psnStyle) return false;
	delete m_lstTempUserStyle.GetAt( psnStyle);
	m_lstTempUserStyle.RemoveAt( psnStyle);
	return true;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  const CList<CStyleData*, CStyleData*>& sourceList : 
// Param  CList<CStyleData* ,CStyleData*>& targetList : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::CopyStyleList( const CList<CStyleData*, CStyleData*>& sourceList, CList<CStyleData*, CStyleData*>& targetList )
{
	// 기존의 target 내용은 모두 삭제한다.
	DeleteAll(targetList);

	POSITION sourcePos = sourceList.GetHeadPosition();
	while(sourcePos != NULL)
	{
		CStyleData* pSourceStyleData = sourceList.GetNext(sourcePos);
		if(pSourceStyleData == NULL)
			continue;

		CStyleData* pNewStyleData = new CStyleData(*pSourceStyleData);
		targetList.AddTail(pNewStyleData);
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnDestroy() 
{
	CDialog::OnDestroy();

	if(m_pTitleFont != NULL)
	{
		m_pTitleFont->DeleteObject();
		delete m_pTitleFont;
		m_pTitleFont = NULL;
	}
}


void CDlgMainTabStyleSetUp::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = FALSE;

	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	m_styleDataManager.SetStyleDataToOCX( *m_pStyleData, m_pIChartOCX, FALSE, m_bSkipAnalysisColor, m_bSkipVerticalScaleGrid, m_bSkipHorizontalScaleGrid);

	if( !WriteUserStyleFile() )
	{
		AfxMessageBox("Error: Cannot save User Style");
	}
}

void CDlgMainTabStyleSetUp::OnBtnRecover() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = FALSE;

	// (2006/8/21 - Seung-Won, Bae) Original Backup is in User - Style Data Manager
// 2008.01.25 by LYH >>
//	CStyleData *pStyleData = m_styleDataManager.GetStyleData( "현화면스타일");
	CString strStyle;
	strStyle.LoadString(IDS_CURRENT_STYLE);
	CStyleData *pStyleData = m_styleDataManager.GetStyleData( strStyle);
// 2008.01.25 by LYH <<
	if( !m_pStyleData || !pStyleData) return;
	*m_pStyleData = *pStyleData;
	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	m_styleDataManager.SetStyleDataToOCX( *m_pStyleData, m_pIChartOCX, FALSE, m_bSkipAnalysisColor, m_bSkipVerticalScaleGrid, m_bSkipHorizontalScaleGrid);
}

void CDlgMainTabStyleSetUp::OnBtnPreview() 
{
	// TODO: Add your control notification handler code here
	m_bPreview = TRUE;

	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	m_styleDataManager.SetStyleDataToOCX( *m_pStyleData, m_pIChartOCX, FALSE, m_bSkipAnalysisColor, m_bSkipVerticalScaleGrid, m_bSkipHorizontalScaleGrid);
}

void CDlgMainTabStyleSetUp::Recover()
{
	OnBtnRecover();
}

void CDlgMainTabStyleSetUp::Save()
{
	OnBtnSave();
}

void CDlgMainTabStyleSetUp::PreView()
{
	OnBtnPreview();
}

//sy 2005.10.12
CBaseTabDlg::DLG_TYPE CDlgMainTabStyleSetUp::GetDialogType() const
{
	return CBaseTabDlg::STYLE_SET;
}
//sy end

int CDlgMainTabStyleSetUp::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 3. Init Font
	if( !m_pTitleFont)
	{
		m_pTitleFont = new CFont();

		LOGFONT	logFont;
		memset( &logFont, 0, sizeof(LOGFONT));       // zero out structure
		logFont.lfHeight = 12;
		logFont.lfCharSet = DEFAULT_CHARSET;

		CString strFont;
		strFont.LoadString(IDS_STRING_FONT);
		strcpy(logFont.lfFaceName, strFont);

		m_pTitleFont->CreateFontIndirect( &logFont );
	}

	return 0;
}

//KHD : 시간구분
void CDlgMainTabStyleSetUp::OnSelchangeCmbHscaleGridtype_Day()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetHScaleGridType_TimeGubun( m_cmbHScaleGridType_Day.GetCurSel(), CScaleBaseData::MINTICK_DAY_GUBUN);
	Invalidate( FALSE);
}

void CDlgMainTabStyleSetUp::OnSelchangeCmbHscaleGridtype_Week()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetHScaleGridType_TimeGubun( m_cmbHScaleGridType_Week.GetCurSel(), CScaleBaseData::DAYCHART_WEEK_GUBUN);
	Invalidate( FALSE);
}

void CDlgMainTabStyleSetUp::OnSelchangeCmbHscaleGridtype_Mon()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetHScaleGridType_TimeGubun( m_cmbHScaleGridType_Mon.GetCurSel(), CScaleBaseData::DAYCHART_MON_GUBUN);
	Invalidate( FALSE);
}

void CDlgMainTabStyleSetUp::OnSelchangeCmbHscaleGridtype_Year()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetHScaleGridType_TimeGubun( m_cmbHScaleGridType_Year.GetCurSel(), CScaleBaseData::DAYCHART_YEAR_GUBUN);
	Invalidate( FALSE);
}

//KHD : 굵기
void CDlgMainTabStyleSetUp::OnSelchangeScalelineThickness_Day()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetScaleLineThickness_TimeGubun(CScaleBaseData::MINTICK_DAY_GUBUN, m_cmbScaleGridThickness_Day.GetCurSel());
	Invalidate( FALSE);
}
void CDlgMainTabStyleSetUp::OnSelchangeScalelineThickness_Week()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetScaleLineThickness_TimeGubun( CScaleBaseData::DAYCHART_WEEK_GUBUN, m_cmbScaleGridThickness_Week.GetCurSel());
	Invalidate( FALSE);
}
void CDlgMainTabStyleSetUp::OnSelchangeScalelineThickness_Mon()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetScaleLineThickness_TimeGubun( CScaleBaseData::DAYCHART_MON_GUBUN, m_cmbScaleGridThickness_Mon.GetCurSel());
	Invalidate( FALSE);
}
void CDlgMainTabStyleSetUp::OnSelchangeScalelineThickness_Year()
{
	// TODO: Add your control notification handler code here
	m_pStyleData->SetScaleLineThickness_TimeGubun( CScaleBaseData::DAYCHART_YEAR_GUBUN, m_cmbScaleGridThickness_Year.GetCurSel());
	Invalidate( FALSE);
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CDlgMainTabStyleSetUp::OnBtnHScaleGridColor_Day() 
{
	COLORREF color = DoModal_ColorDialog(&m_btnHScaleGridColor_Day);
	if (m_bColorChanged)
	{
		m_pStyleData->SetHScaleGridColor_TimeGubun(color, CScaleBaseData::MINTICK_DAY_GUBUN);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}

void CDlgMainTabStyleSetUp::OnBtnHScaleGridColor_Week() 
{
	COLORREF color = DoModal_ColorDialog(&m_btnHScaleGridColor_Week);
	if (m_bColorChanged)
	{
		m_pStyleData->SetHScaleGridColor_TimeGubun(color, CScaleBaseData::DAYCHART_WEEK_GUBUN);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}

void CDlgMainTabStyleSetUp::OnBtnHScaleGridColor_Mon() 
{
	COLORREF color = DoModal_ColorDialog(&m_btnHScaleGridColor_Mon);
	if (m_bColorChanged)
	{
		m_pStyleData->SetHScaleGridColor_TimeGubun(color, CScaleBaseData::DAYCHART_MON_GUBUN);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}

void CDlgMainTabStyleSetUp::OnBtnHScaleGridColor_Year() 
{
	COLORREF color = DoModal_ColorDialog(&m_btnHScaleGridColor_Year);
	if (m_bColorChanged)
	{
		m_pStyleData->SetHScaleGridColor_TimeGubun(color, CScaleBaseData::DAYCHART_YEAR_GUBUN);
		m_bColorChanged = FALSE;
	}

	Invalidate();
}
