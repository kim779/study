// ColorSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ColorSetDlg.h"

#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "ChartPropertySheet.h"
#include "PropertiesData.h"
#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorSetDlg dialog


CColorSetDlg::CColorSetDlg( HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CColorSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorSetDlg)
	//}}AFX_DATA_INIT

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( m_hOcxWnd);
	m_Lattice.SetOcxHwnd( m_hOcxWnd);
	m_NumericalInquiryColor.SetOcxHwnd( m_hOcxWnd);
	m_clrVerticalScaleLine.SetOcxHwnd( m_hOcxWnd);
	m_TwoLine.SetOcxHwnd( m_hOcxWnd);
	m_ThreeLine.SetOcxHwnd( m_hOcxWnd);
	m_SixLIne.SetOcxHwnd( m_hOcxWnd);
	m_ScaleTextColor.SetOcxHwnd( m_hOcxWnd);
	m_OneLine.SetOcxHwnd( m_hOcxWnd);
	m_GridColor.SetOcxHwnd( m_hOcxWnd);
	m_FourLine.SetOcxHwnd( m_hOcxWnd);
	m_FiveLine.SetOcxHwnd( m_hOcxWnd);
	m_BlockColor.SetOcxHwnd( m_hOcxWnd);
	m_BlockBorderColor.SetOcxHwnd( m_hOcxWnd);
	m_AnalyzeToolColor.SetOcxHwnd( m_hOcxWnd);
	m_BackBorderColor.SetOcxHwnd( m_hOcxWnd);
	m_BackGroundColor.SetOcxHwnd( m_hOcxWnd);
}


void CColorSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorSetDlg)
	DDX_Control(pDX, IDC_LATTICE, m_Lattice);
	DDX_Control(pDX, IDC_NUMERICALCOLOR, m_NumericalInquiryColor);
	DDX_Control(pDX, IDC_VERTICALGRID, m_clrVerticalScaleLine);
	DDX_Control(pDX, IDC_TWOLINE, m_TwoLine);
	DDX_Control(pDX, IDC_THREELINE, m_ThreeLine);
	DDX_Control(pDX, IDC_SIXLINE, m_SixLIne);
	DDX_Control(pDX, IDC_SCALETEXTCOLOR, m_ScaleTextColor);
	DDX_Control(pDX, IDC_ONELINE, m_OneLine);
	DDX_Control(pDX, IDC_GRIDCOLOR, m_GridColor);
	DDX_Control(pDX, IDC_FOURLINE, m_FourLine);
	DDX_Control(pDX, IDC_FIVELINE, m_FiveLine);
	DDX_Control(pDX, IDC_BLOCKCOLOR, m_BlockColor);
	DDX_Control(pDX, IDC_BLOCKBORDERCOLOR, m_BlockBorderColor);
	DDX_Control(pDX, IDC_ANALYZETOOLCOLOR, m_AnalyzeToolColor);
	DDX_Control(pDX, IDC_BACKBORDERCOLOR, m_BackBorderColor);
	DDX_Control(pDX, IDC_BACKGROUNDCOLOR, m_BackGroundColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorSetDlg, CDialog)
	//{{AFX_MSG_MAP(CColorSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorSetDlg message handlers

BOOL CColorSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CreateToolTip();
	InitColortoButtonbyPicker();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Create tooltip object and Add text to StringArray 
void CColorSetDlg::CreateToolTip()
{
	StringArrayAdd( IDC_STATIC2,			IDS_BACKGROUNDCOLOR_TOOLTIP);
	StringArrayAdd( IDC_BACKGROUNDCOLOR,	IDS_BACKGROUNDCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC3,			IDS_BACKBORDERCOLOR_TOOLTIP);
	StringArrayAdd( IDC_BACKBORDERCOLOR,	IDS_BACKBORDERCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC4,			IDS_HORIZONTAL_GRIDCOLOR_TOOLTIP);
	StringArrayAdd( IDC_GRIDCOLOR,			IDS_HORIZONTAL_GRIDCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC19,			IDS_VERTICAL_GRIDCOLOR_TOOLTIP);
	StringArrayAdd( IDC_VERTICALGRID,		IDS_VERTICAL_GRIDCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC5,			IDS_SCALETEXTCOLOR_TOOLTIP);
	StringArrayAdd( IDC_SCALETEXTCOLOR,		IDS_SCALETEXTCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC6,			IDS_BLOCKCOLOR_TOOLTIP);
	StringArrayAdd( IDC_BLOCKCOLOR,			IDS_BLOCKCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC7,			IDS_BLOCKBORDERCOLOR_TOOLTIP);
	StringArrayAdd( IDC_BLOCKBORDERCOLOR,	IDS_BLOCKBORDERCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC8,			IDS_ANALYZETOOLCOLOR_TOOLTIP);
	StringArrayAdd( IDC_ANALYZETOOLCOLOR,	IDS_ANALYZETOOLCOLOR_TOOLTIP);
	StringArrayAdd( IDC_STATIC9,			IDS_6_LINE_COLOR_TOOLTIP);
	StringArrayAdd( IDC_ONELINE,			IDS_6_LINE_COLOR_TOOLTIP);
	StringArrayAdd( IDC_TWOLINE,			IDS_6_LINE_COLOR_TOOLTIP);
	StringArrayAdd( IDC_THREELINE,			IDS_6_LINE_COLOR_TOOLTIP);
	StringArrayAdd( IDC_FOURLINE,			IDS_6_LINE_COLOR_TOOLTIP);
	StringArrayAdd( IDC_FIVELINE,			IDS_6_LINE_COLOR_TOOLTIP);
	StringArrayAdd( IDC_SIXLINE,			IDS_6_LINE_COLOR_TOOLTIP);
}

BOOL CColorSetDlg::PreTranslateMessage(MSG* pMsg) 
{

	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN )	{
		m_toolTip.ShowToolTip( (CPoint)pt );
	}
	if(pMsg->message == WM_MOUSEMOVE){
		m_toolTip.HideToolTip( (CPoint)pt );
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//init Color of Button.
void CColorSetDlg::InitColortoButtonbyPicker()
{
	m_BackGroundColor.InitColorPicker(CDataConversion::Str2RGB(m_strBackGroundColor));	
	m_TwoLine.InitColorPicker(CDataConversion::Str2RGB(m_strTwoLine));
	m_ThreeLine.InitColorPicker(CDataConversion::Str2RGB(m_strThreeLine));	
	m_SixLIne.InitColorPicker(CDataConversion::Str2RGB(m_strSixLIne));	
	m_ScaleTextColor.InitColorPicker(CDataConversion::Str2RGB(m_strScaleTextColor));	
	m_OneLine.InitColorPicker(CDataConversion::Str2RGB(m_strOneLine));	
	m_GridColor.InitColorPicker(CDataConversion::Str2RGB(m_strGridColor));	
	m_FourLine.InitColorPicker(CDataConversion::Str2RGB(m_strFourLine));	
	m_FiveLine.InitColorPicker(CDataConversion::Str2RGB(m_strFiveLine));	
	m_BlockColor.InitColorPicker(CDataConversion::Str2RGB(m_strBlockColor));	
	m_Lattice.InitColorPicker(CDataConversion::Str2RGB(m_strLattice));	
	m_BlockBorderColor.InitColorPicker(CDataConversion::Str2RGB(m_strBlockBorderColor));	
	m_AnalyzeToolColor.InitColorPicker(CDataConversion::Str2RGB(m_strAnalyzeToolColor));	
	m_BackBorderColor.InitColorPicker(CDataConversion::Str2RGB(m_strBackBorderColor));	
	m_clrVerticalScaleLine.InitColorPicker(CDataConversion::Str2RGB(m_strVerticalScaleLine));	
	m_NumericalInquiryColor.InitColorPicker(CDataConversion::Str2RGB(m_strNumericalInquiry));	
}
//init tooltip's String.
void CColorSetDlg::StringArrayAdd(UINT ControlID, UINT uMsgID)
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create(this);

	CDWordArray dwaInfo;
	dwaInfo.Add( uMsgID);
	m_toolTip.AddControlInfo(ControlID, dwaInfo);
}
//this function call when push this Dlg's OK to get SettingColor.
void CColorSetDlg::GetStringColorOfButton()
{
	m_strBackGroundColor = CDataConversion::RGB2Str(m_BackGroundColor.GetColour());
	m_strTwoLine = CDataConversion::RGB2Str(m_TwoLine.GetColour());
	m_strThreeLine = CDataConversion::RGB2Str(m_ThreeLine.GetColour());
	m_strSixLIne = CDataConversion::RGB2Str(m_SixLIne.GetColour());
	m_strScaleTextColor = CDataConversion::RGB2Str(m_ScaleTextColor.GetColour());
	m_strOneLine = CDataConversion::RGB2Str(m_OneLine.GetColour());
	m_strGridColor = CDataConversion::RGB2Str(m_GridColor.GetColour());
	m_strFourLine = CDataConversion::RGB2Str(m_FourLine.GetColour());
	m_strFiveLine = CDataConversion::RGB2Str(m_FiveLine.GetColour());
	m_strBlockColor = CDataConversion::RGB2Str(m_BlockColor.GetColour());
	m_strLattice = CDataConversion::RGB2Str(m_Lattice.GetColour());
	m_strBlockBorderColor = CDataConversion::RGB2Str(m_BlockBorderColor.GetColour());
	m_strAnalyzeToolColor = CDataConversion::RGB2Str(m_AnalyzeToolColor.GetColour());
	m_strBackBorderColor = CDataConversion::RGB2Str(m_BackBorderColor.GetColour());
	m_strVerticalScaleLine = CDataConversion::RGB2Str(m_clrVerticalScaleLine.GetColour());
	m_strNumericalInquiry = CDataConversion::RGB2Str(m_NumericalInquiryColor.GetColour());
}


//들어오는 순서에 따라서 어느 속성페이지의 속성을 Set해야하는지 결정
//order: 들어오는 속성의 순서
//szPropName: 속성명
//szPropValue: 속성값
void CColorSetDlg::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		// (2006/7/5 - Seung-Won, Bae) Manage Color with String Type
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBACKGROUND		], m_strBackGroundColor,	""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBACKBORDER		], m_strBackBorderColor,	""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBLOCK			], m_strBlockColor,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRBLOCKBORDER		], m_strBlockBorderColor,	""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE		], m_strGridColor,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE2		], m_strVerticalScaleLine,	""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRSCALETEXT		], m_strScaleTextColor,		""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE1		], m_strOneLine,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE2		], m_strTwoLine,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE3		], m_strThreeLine,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE4		], m_strFourLine,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE5		], m_strFiveLine,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE6		], m_strSixLIne,			""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRANALYTICSTOOL	], m_strAnalyzeToolColor,	""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRINQUIRYWINDOW	], m_strNumericalInquiry,	""	);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRRATTICE			], m_strLattice,			""	);
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

void CColorSetDlg::OnOK() 
{
	UpdateData( TRUE);
	GetStringColorOfButton();

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		// (2006/7/5 - Seung-Won, Bae) Manage Color with String Type
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRBACKGROUND		], m_strBackGroundColor,	""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRBACKBORDER		], m_strBackBorderColor,	""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRBLOCK			], m_strBlockColor,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRBLOCKBORDER		], m_strBlockBorderColor,	""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE		], m_strGridColor,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRSCALELINE2		], m_strVerticalScaleLine,	""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRSCALETEXT		], m_strScaleTextColor,		""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE1		], m_strOneLine,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE2		], m_strTwoLine,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE3		], m_strThreeLine,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE4		], m_strFourLine,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE5		], m_strFiveLine,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE6		], m_strSixLIne,			""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRANALYTICSTOOL	], m_strAnalyzeToolColor,	""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRINQUIRYWINDOW	], m_strNumericalInquiry,	""	);
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CLRRATTICE			], m_strLattice,			""	);
	CHART_ADDTOSTRLIST_END

	CDialog::OnOK();
}
