// DlgSubGraphSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgSubGraphSetting.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _CRTM1

#include "Conversion.h"
#include "DlgGraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubGraphSetting dialog


CDlgSubGraphSetting::CDlgSubGraphSetting( HWND p_hOcxWnd, CWnd* pDlgChartWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubGraphSetting::IDD, pParent)
{
	// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( m_hOcxWnd);
	m_clrEqualEdge.SetOcxHwnd( m_hOcxWnd);
	m_clrUp.SetOcxHwnd( m_hOcxWnd);
	m_clrUpEdge.SetOcxHwnd( m_hOcxWnd);
	m_clrDown.SetOcxHwnd( m_hOcxWnd);
	m_clrDownEdge.SetOcxHwnd( m_hOcxWnd);
	m_clrEqual.SetOcxHwnd( m_hOcxWnd);

	//{{AFX_DATA_INIT(CDlgSubGraphSetting)
	m_bFillUp = FALSE;
	m_nUpThickNess = 0;
	m_bFillDown = FALSE;
	m_nDownThickNess = 0;
	m_nBorderWidth = 0;
	m_strTitle = _T("");
	m_bGraphHide = FALSE;
	m_nGraphType = -1;
	m_nGraphStyle = -1;
	m_nDrawStyle = -1;
	//}}AFX_DATA_INIT
	m_pDlgGraphSetting = (CDlgGraphSetting* )pDlgChartWnd;
	m_strclrUp = "255,0,0";
	m_strclrUpEdge= "255,0,0";
	m_bFillUp = FALSE;
	m_strclrDown= "0,0,255";
	m_strclrDownEdge= "0,0,255";
	m_bFillDown = FALSE;
	m_strclrEqual= "255,0,255";
	m_strclrEqualEdge = "255,0,255";
	m_nBorderWidth = 1;
	m_nUpThickNess = 1;
	m_nDownThickNess = 1;
	m_strSubGraphName = "";

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	m_pIGraphInfoManager = NULL;
}

CDlgSubGraphSetting::~CDlgSubGraphSetting()											// standard destructor
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
}

void CDlgSubGraphSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubGraphSetting)
	DDX_Control(pDX, IDC_EQUALEDGECOLOR, m_clrEqualEdge);
	DDX_Control(pDX, IDC_TITLE, m_TitleEdit);
	DDX_Control(pDX, IDC_UP_BORDERTHICKNESS, m_UpThicknessEdit);
	DDX_Control(pDX, IDC_DOWN_BORDERTHICKNESS, m_DownThicknessEdit);
	DDX_Control(pDX, IDC_BORDERWIDTH, m_EqualThicknessEdit);
	DDX_Control(pDX, IDC_UPCOLOR, m_clrUp);
	DDX_Control(pDX, IDC_UPEDGECOLOR, m_clrUpEdge);
	DDX_Control(pDX, IDC_DOWNCOLOR, m_clrDown);
	DDX_Control(pDX, IDC_DOWNEDGECOLOR, m_clrDownEdge);
	DDX_Control(pDX, IDC_EQUALCOLOR, m_clrEqual);
	DDX_Control(pDX, IDC_GRAPH_TYPE, m_TypeCombo);
	DDX_Control(pDX, IDC_GRAPHDRAWSTYLE, m_DrawStyleCombo);
	DDX_Control(pDX, IDC_GRAPHSTYLE, m_GraphStyleCombo);
	DDX_Check(pDX, IDC_FILLUPCOLOR, m_bFillUp);
	DDX_Text(pDX, IDC_UP_BORDERTHICKNESS, m_nUpThickNess);
	DDX_Check(pDX, IDC_FILLDOWNCOLOR, m_bFillDown);
	DDX_Text(pDX, IDC_DOWN_BORDERTHICKNESS, m_nDownThickNess);
	DDX_Text(pDX, IDC_BORDERWIDTH, m_nBorderWidth);
	DDX_Text(pDX, IDC_TITLE, m_strTitle);
	DDX_Check(pDX, IDC_CHECK_HIDE, m_bGraphHide);
	DDX_CBIndex(pDX, IDC_GRAPH_TYPE, m_nGraphType);
	DDX_CBIndex(pDX, IDC_GRAPHSTYLE, m_nGraphStyle);
	DDX_CBIndex(pDX, IDC_GRAPHDRAWSTYLE, m_nDrawStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubGraphSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSubGraphSetting)
	ON_CBN_SELCHANGE(IDC_GRAPH_TYPE, OnSelchangeGraphType)
	ON_CBN_SELCHANGE(IDC_GRAPHSTYLE, OnSelchangeGraphstyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubGraphSetting message handlers

BOOL CDlgSubGraphSetting::PreTranslateMessage(MSG* pMsg) 
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

BOOL CDlgSubGraphSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CreateToolTip();
	InitButtonColor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSubGraphSetting::OnSelchangeGraphType() 
{
	UpdateData(true);

	//type가 바뀌면 graphStyle 와 drawStyle를 초기값으로 돌린다.
	m_nGraphStyle = 0;
	m_nDrawStyle = 0;
	Set_GraphStyleCombo();
}

//test -ok
void CDlgSubGraphSetting::OnSelchangeGraphstyle() 
{
//	m_DrawStyleCombo.SetCurSel(m_GraphStyleCombo.GetCurSel());
	UpdateData(true);
	m_nDrawStyle = 0;
	Set_GraphDrawStyleCombo();
	UpdateData(FALSE);//sy 2004.1.7.
}


void CDlgSubGraphSetting::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_TITLE_TOOLTIP);
	m_toolTip.AddControlInfo(IDC_STATIC10, dwaInfo);
	m_toolTip.AddControlInfo(IDC_TITLE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_GRAPHTYPE_TOOLTIP);
	m_toolTip.AddControlInfo(IDC_STATIC10, dwaInfo);
	m_toolTip.AddControlInfo(IDC_GRAPH_TYPE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_UPFILLCOLOR_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC14, dwaInfo);
	m_toolTip.AddControlInfo( IDC_UPCOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_UPBORDERCOLOR_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC15, dwaInfo);
	m_toolTip.AddControlInfo( IDC_UPEDGECOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_UPFILLCHECK_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_FILLUPCOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_UPBORDERTHICK_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC21, dwaInfo);
	m_toolTip.AddControlInfo( IDC_UP_BORDERTHICKNESS, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_DOWNFILLCOLOR_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC16, dwaInfo);
	m_toolTip.AddControlInfo( IDC_DOWNCOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_DOWNBORDERCOLOR_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC17, dwaInfo);
	m_toolTip.AddControlInfo( IDC_DOWNEDGECOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_DOWNFILLCHECK_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_FILLDOWNCOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_DOWNBORDERTHICK_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC22, dwaInfo);
	m_toolTip.AddControlInfo( IDC_DOWN_BORDERTHICKNESS, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_STEADYFILLCOLOR_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_EQUALCOLOR, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_STEADYBORDERTHICK_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC18, dwaInfo);
	m_toolTip.AddControlInfo( IDC_BORDERWIDTH, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SUBGRAPHSET_HIDECHECK_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_CHECK_HIDE, dwaInfo);
}

void CDlgSubGraphSetting::MakeFromPickerColorToStrColor()
{
	m_strclrUpEdge = CDataConversion::RGB2Str(m_clrUpEdge.GetColour());
	m_strclrUp = CDataConversion::RGB2Str(m_clrUp.GetColour());
	m_strclrEqual = CDataConversion::RGB2Str(m_clrEqual.GetColour());
	m_strclrEqualEdge = CDataConversion::RGB2Str(m_clrEqualEdge.GetColour());
	m_strclrDownEdge = CDataConversion::RGB2Str(m_clrDownEdge.GetColour());
	m_strclrDown = CDataConversion::RGB2Str(m_clrDown.GetColour());
}

void CDlgSubGraphSetting::InitButtonColor()
{
	m_clrUpEdge.InitColorPicker(CDataConversion::Str2RGB(m_strclrUpEdge));	
	m_clrUp.InitColorPicker(CDataConversion::Str2RGB(m_strclrUp));	
	m_clrEqual.InitColorPicker(CDataConversion::Str2RGB(m_strclrEqual));	
	m_clrEqualEdge.InitColorPicker(CDataConversion::Str2RGB(m_strclrEqualEdge));	
	m_clrDownEdge.InitColorPicker(CDataConversion::Str2RGB(m_strclrDownEdge));	
	m_clrDown.InitColorPicker(CDataConversion::Str2RGB(m_strclrDown));	
}

CString CDlgSubGraphSetting::GetAllSettingData( const char *p_szIndicatorName)
{
	UpdateData(TRUE);
	//sy 2004.1.7.
	// graphType/Style/DrawStyle 값의 체크 요망.
	CheckingGraphTyleAndStyle();
	//end sy
	MakeFromPickerColorToStrColor();
	CString strColor = "";
	strColor += CDataConversion::IntToString(m_nGraphType) +";";
	strColor += CDataConversion::IntToString(m_nGraphStyle) +";";
	strColor += CDataConversion::IntToString(m_nDrawStyle) +";";
	strColor += m_strSubGraphName + ";";
	if( g_iMetaTable.IsTitleIncludeCondition( p_szIndicatorName))
			strColor += m_strTitle + ";";
	else	strColor += CString( _CRTM1( m_strTitle)) + ";";
	strColor += CDataConversion::IntToString(m_bGraphHide)+";";		
	strColor += m_strclrUpEdge +";";
	strColor += m_strclrUp +";";
	strColor += m_strclrDownEdge +";";
	strColor += m_strclrDown +";";
	strColor += m_strclrEqualEdge +";";
	strColor += m_strclrEqual +";";
	strColor += CDataConversion::IntToString(m_bFillUp) +";";
	strColor += CDataConversion::IntToString(m_bFillDown) +";";
	strColor += CDataConversion::IntToString(m_nUpThickNess) + ";";
	strColor += CDataConversion::IntToString(m_nDownThickNess) +";";
	strColor += CDataConversion::IntToString(m_nBorderWidth)  +";";
	strColor += CDataConversion::IntToString(0) +"; \r\n"; //강조
	return strColor;
}

void CDlgSubGraphSetting::SetSubGraphDatas(const CString& strSubGraphDatas)
{
	CString data = strSubGraphDatas;
	if(data.GetLength()) {
		m_nGraphType = GetGraphType(data);
		m_nGraphStyle = GetGraphStyle(data);
		//sy 2004.1.7. type, style, drawStyle 검증.
		//m_nDrawStyle = GetDrawStyle(data);
		//콤보에 GraphType/Style추가 후 drawStyle체크(drawStyle 값 체크를 위함.
		Set_GraphTypeCombo();

		// (2006/9/10 - Seung-Won, Bae) Check Exception of No IndiCond.cfg
		if( !m_TypeCombo.GetCount()) return;

		m_nDrawStyle = GetDrawStyle(data);
		Set_GraphDrawStyleCombo();
		//sy 2004.1.7. type, style, drawStyle 검증.
		CheckingGraphTyleAndStyleAndCombo();
		//sy end
		CDataConversion::GetStringData(data, ";");
		CString strTitle = CDataConversion::GetStringData(data, ";");
		m_strTitle = _MTOL1_N( strTitle);
		if( m_strTitle.IsEmpty()) m_strTitle = _MTOL3( strTitle);
		m_bGraphHide = atoi(CDataConversion::GetStringData(data, ";"));
		m_strclrUpEdge = CDataConversion::GetStringData(data, ";");
		m_strclrUp = CDataConversion::GetStringData(data, ";");
		m_strclrDownEdge = CDataConversion::GetStringData(data, ";");
		m_strclrDown = CDataConversion::GetStringData(data, ";");
		m_strclrEqualEdge = CDataConversion::GetStringData(data, ";");
		m_strclrEqual = CDataConversion::GetStringData(data, ";");
		m_bFillUp = atoi(CDataConversion::GetStringData(data, ";"));
		m_bFillDown = atoi(CDataConversion::GetStringData(data, ";"));
		m_nUpThickNess = atoi(CDataConversion::GetStringData(data, ";"));
		m_nDownThickNess = atoi(CDataConversion::GetStringData(data, ";"));
		m_nBorderWidth = atoi(CDataConversion::GetStringData(data, ";"));
		InitButtonColor();
		UpdateData(FALSE);
	}
	else
		Set_GraphTypeCombo();
}

void CDlgSubGraphSetting::SetSubGraphName(const CString &strSubGraphName)
{
	m_strSubGraphName = strSubGraphName;
}

void CDlgSubGraphSetting::SetLineColor(const CString &strLineColor)
{
	m_strclrUp = strLineColor;
	m_strclrUpEdge = strLineColor;
	InitButtonColor();
}

void CDlgSubGraphSetting::SetGraphStyle(const int nStyle)
{
	m_nGraphStyle = nStyle;
}


void CDlgSubGraphSetting::SetGraphType(const int graphtype)
{
	m_nGraphType = graphtype;
}

//봉형일때 graphstyle이 미국식이면 drawstyle도 미국식이어야 한다.
int CDlgSubGraphSetting::GetDrawStyle()
{
	if(m_pDlgGraphSetting->GetIndicatorName().CompareNoCase( _MTEXT( C2_PRICE_CHART)) == 0 && m_nGraphType == 0)
		return m_nGraphStyle;
	return 0;
}

void CDlgSubGraphSetting::SetDrawStyle(const int nDrawStyle)
{
	m_nDrawStyle = nDrawStyle;
}

//sy 2004.1.7.
//GrapyType/Style 검사.
void CDlgSubGraphSetting::CheckingGraphTyleAndStyleAndCombo()
{
	bool bChange = false;
	if(m_nGraphStyle < 0){
		m_nGraphType = 0;
		bChange = true;
		Set_GraphTypeCombo();
	}

	if(m_nGraphStyle < 0){
		m_nGraphStyle = 0;
		bChange = true;
		Set_GraphStyleCombo();
	}

	if(m_nDrawStyle < 0){
		m_nDrawStyle = 0;
		bChange = true;
		Set_GraphDrawStyleCombo();
	}

	if(bChange)
		UpdateData(FALSE);
}

void CDlgSubGraphSetting::CheckingGraphTyleAndStyle()
{
	if(m_nGraphStyle < 0){
		m_nGraphType = 0;
	}

	if(m_nGraphStyle < 0){
		m_nGraphStyle = 0;
	}

	if(m_nDrawStyle < 0){
		m_nDrawStyle = 0;
	}
}
//end sy

//graphDrawStyle 관련 콤보의 enable 여부
bool CDlgSubGraphSetting::IsEnable_GraphDrawStyleCombo(const CGraphBaseData::GRAPHTYPE eGraphType) const
{
	switch(eGraphType){
	case CGraphBaseData::Bong_Type:		// 봉형
	case CGraphBaseData::Pie_Type:		// 파이형
	case CGraphBaseData::Special_Type:	// 특수형
	case CGraphBaseData::Fund_Type:		// 펀드형
	case CGraphBaseData::FuOp_Type:		// 선물옵션형
		return false;
	}

	return true;
}

//graphDrawStyle 이 없는 타입
bool CDlgSubGraphSetting::IsNoGraphDrawStyle(const CGraphBaseData::GRAPHTYPE eGraphType) const
{
	switch(eGraphType){
	case CGraphBaseData::Special_Type:	// 특수형
	case CGraphBaseData::Fund_Type:		// 펀드형
	case CGraphBaseData::FuOp_Type:		// 선물옵션형
		return true;
	}

	return false;
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CDlgSubGraphSetting::SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager)
{
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
	m_pIGraphInfoManager = p_pIGraphInfoManager;
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->AddRef();
}

//test -ok
void CDlgSubGraphSetting::Set_GraphStyleCombo()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	m_GraphStyleCombo.ResetContent();
	if( m_pIGraphInfoManager == NULL || m_TypeCombo.GetCount() <= 0) return;

	CString szGraphType;
	m_TypeCombo.GetLBText(m_nGraphType, szGraphType);
	szGraphType = _LTOM5( szGraphType);

	CList<CString, CString>* pStyleList = m_pIGraphInfoManager->GetAllGraphTypeStyleNameList( szGraphType);
	if(pStyleList == NULL) return;

	POSITION pos = pStyleList->GetHeadPosition();
	while(pos) m_GraphStyleCombo.AddString( _MTOL5( pStyleList->GetNext(pos)));

	m_GraphStyleCombo.SetCurSel(m_nGraphStyle);
	
	Set_GraphDrawStyleCombo();
	UpdateData(FALSE);//sy 2004.1.7.
}

//test -ok
void CDlgSubGraphSetting::Set_GraphTypeCombo()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( m_pIGraphInfoManager == NULL) return;

	CList<CString, CString> graphTypeList;
	if( !m_pIGraphInfoManager->GetAllGraphTypeNames( graphTypeList)) return;

	int nCount = m_TypeCombo.GetCount();
	for(int i = 0; i< nCount ; i++) 
		m_TypeCombo.DeleteString(i);

	POSITION pos = graphTypeList.GetHeadPosition();
	while(pos) m_TypeCombo.AddString( _MTOL5( graphTypeList.GetNext(pos)));

	m_TypeCombo.SetCurSel(m_nGraphType);
	Set_GraphStyleCombo();
}

//test -ok
// "0;0;0;가격;가격;0;255,0,0;...;\r\n" 중 첫음 ";"이전 data : "0"
int CDlgSubGraphSetting::GetGraphType(CString &strControlValue)
{
	CString strType = CDataConversion::GetStringData(strControlValue, ";");
	int nType = atoi(strType);

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if(nType > 0 || (strType == '0' && nType == 0)) return nType;

	if( m_pIGraphInfoManager == NULL) return -1;

	int nStyle = -1;
	m_pIGraphInfoManager->GetGraphTypeAndStyle(strType, "", nType, nStyle);
	return nType;
}


//test -ok
// "0;0;가격;가격;0;255,0,0;...;\r\n" 중 첫음 ";"이전 data : "0"
int CDlgSubGraphSetting::GetGraphStyle(CString &strControlValue)
{
	//sy 2003.3.14. graph style 갯수가 9개로 고정되어 있던 코드 수정
	CString strStyle = CDataConversion::GetStringData(strControlValue, ";");
	int nStyle = atoi(strStyle);

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if(nStyle > 0 || (strStyle == '0' && nStyle == 0)) return nStyle;

	// 만약 strControlValue 가 숫자형태가 아닌 경우 ex)"일반식", "수평식"
	if(m_pIGraphInfoManager == NULL) return -1;
	
	CString strType;
	m_TypeCombo.GetLBText(m_nGraphType, strType);

	int nType = -1;
	if( m_pIGraphInfoManager->GetGraphTypeAndStyle(strType, strStyle, nType, nStyle)) return -1;
	return nStyle;
}

//test -ok
//sy 2004.1.7.
void CDlgSubGraphSetting::Set_GraphDrawStyleCombo()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	bool bEnable = IsEnable_GraphDrawStyleCombo( ( CGraphBaseData::GRAPHTYPE)m_nGraphType);
	m_DrawStyleCombo.EnableWindow(bEnable);
	m_DrawStyleCombo.ResetContent();

	CString szType;
	// 봉형(0), 선형(1), 바형(2), 파이형(3)
	if(m_nGraphType == 0 || m_nGraphType == 1 || m_nGraphType == 2 || m_nGraphType == 3)
	{
		//선형,바형,파이형
		m_TypeCombo.GetLBText(m_nGraphType, szType);
		szType = _LTOM5( szType);
	}
	// 데이터표현형(6)
	else if(m_nGraphType == 6)
	{
		if(m_nGraphStyle == 0 || m_nGraphStyle == 1)
			szType = _MTEXT( C5_DATA_BAR_TYPE);
		else if(m_nGraphStyle == 2 || m_nGraphStyle == 3 || m_nGraphStyle == 4)
			szType = _MTEXT( C5_DATA_SIGNAL_TYPE);
		else if(m_nGraphStyle == 5)
			szType = _MTEXT( C5_CLASSIFICATION_TEXT_TYPE);
		else 
			return;
	}
	else
	{
		// m_nGraphType : 특수형(4), 펀드형(5), 선물옵션형(7)
		return;
	}

	if( m_pIGraphInfoManager == NULL) return;
	CList<CString, CString>* pDrawStyleList = m_pIGraphInfoManager->GetAllGraphDrawStyleNameList( szType);
	if(pDrawStyleList == NULL) return;

	POSITION pos = pDrawStyleList->GetHeadPosition();
	while(pos) m_DrawStyleCombo.AddString( _MTOL5( pDrawStyleList->GetNext(pos)));

	// <봉형>, <파이형> 은 style == drawStyle !!
	if((CGraphBaseData::GRAPHTYPE)m_nGraphType == CGraphBaseData::Bong_Type || 
		(CGraphBaseData::GRAPHTYPE)m_nGraphType == CGraphBaseData::Pie_Type){
		m_nDrawStyle = m_nGraphStyle;
	}

	if(m_nDrawStyle < 0 && !IsNoGraphDrawStyle((CGraphBaseData::GRAPHTYPE)m_nGraphType))
		m_nDrawStyle = 0;

	m_DrawStyleCombo.SetCurSel(m_nDrawStyle);
}

//test -ok
// "0;가격;가격;0;255,0,0;...;\r\n" 중 첫음 ";"이전 data : "0"
int CDlgSubGraphSetting::GetDrawStyle(CString &strStringValue)
{
	CString strDrawStyle = CDataConversion::GetStringData(strStringValue, ";");
	int nDrawStyle = atoi(strDrawStyle);

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if(nDrawStyle > 0 || (strDrawStyle == '0' && nDrawStyle == 0)) return nDrawStyle;
	if(m_pIGraphInfoManager == NULL) return 0;

	CString strType, strStyle;
	m_TypeCombo.GetLBText( m_nGraphType, strType);
	m_GraphStyleCombo.GetLBText( m_nGraphStyle, strStyle);

	int nType = -1, nStyle = -1;
	if( !m_pIGraphInfoManager->GetGraphTypeAndStyle( strType, strStyle, strDrawStyle, nType, nStyle, nDrawStyle))
		return 0;

	// sy 2004.1.7. nDrawStyle 는 무조건 기본이 "0"으로 한다!!
	return (nDrawStyle < 0 ? 0: nDrawStyle);
}

