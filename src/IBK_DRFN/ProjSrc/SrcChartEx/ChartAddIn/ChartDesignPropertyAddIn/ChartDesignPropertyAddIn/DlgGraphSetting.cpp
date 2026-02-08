// DlgGraphSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgGraphSetting.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
#include "Conversion.h"
#include "ChartLink.h"
#include "DlgGraphSetting2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting dialog


CDlgGraphSetting::CDlgGraphSetting(CChartLink *pItem, CLineColor* LineColor, IGraphInfoManager *p_pIGraphInfoManager, HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSetting::IDD, pParent)
{
	// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( m_hOcxWnd);

	//{{AFX_DATA_INIT(CDlgGraphSetting)
	m_strIndicatorName = _T("");
	m_strCheckedPacketItems = _T("");
	m_bSignalShow = FALSE;
	//}}AFX_DATA_INIT
	m_pItem = pItem;
	m_pLineColor = LineColor;
	m_GraphColorCollection = new CGraphColorCollection( m_hOcxWnd);
	
	m_bIsSelectedPacketData = false;
	m_bCheckComboState = false;

	m_strOldIndicatorName = pItem->m_strIndicatorName;
	m_strIndicatorName = pItem->m_strIndicatorName;

	m_strCheckedPacketItems = _MTOLS( pItem->m_strCheckedPacketItems, ",", 0);
    m_strSubGraphDatas = pItem->m_strSubGraphDatas;
	m_strIndicatorType = _LTOM5( pItem->m_strName);
	//sy 2006.04.17.
	m_bSignalUse = pItem->m_bSignalUse;
	m_bSignalShow = pItem->m_bSignalShow;
	//sy end

	//sy test
	m_graphSetting2Data.SetHighNow(pItem->m_bHighNowUpDown);
	m_graphSetting2Data.SetLowHigh(pItem->m_bLowHighUpDown);
	m_graphSetting2Data.SetLowNow(pItem->m_bLowNowUpDown);
	m_graphSetting2Data.SetPrevNow(pItem->m_bPreNowContrast);
	m_graphSetting2Data.SetCurDataType(pItem->m_nCurData);
	//sy end

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	m_pIGraphInfoManager = p_pIGraphInfoManager;
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->AddRef();
}

CDlgGraphSetting::~CDlgGraphSetting()
{
	if( m_GraphColorCollection != NULL) delete m_GraphColorCollection;

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
}

void CDlgGraphSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSetting)
	DDX_Control(pDX, IDC_SIGNAL, m_Signal_Bt);
	DDX_Control(pDX, IDC_COMBO_PACKETLIST, m_pPacketComboBox);
	DDX_Control(pDX, IDC_INDICATORFUNCTIONNAME, m_IndicatorFuctionComboBox);
	DDX_CBString(pDX, IDC_INDICATORFUNCTIONNAME, m_strIndicatorName);
	DDX_CBString(pDX, IDC_COMBO_PACKETLIST, m_strCheckedPacketItems);
	DDX_Check(pDX, IDC_SIGNAL, m_bSignalShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSetting)
	ON_CBN_EDITCHANGE(IDC_INDICATORFUNCTIONNAME, OnEditchangeIndicatorfunctionname)
	ON_CBN_SELCHANGE(IDC_INDICATORFUNCTIONNAME, OnSelchangeIndicatorfunctionname)
	ON_CBN_SELCHANGE(IDC_COMBO_PACKETLIST, OnSelchangeComboPacketlist)
	ON_BN_CLICKED(IDC_ETCSET_BT, OnEtcsetBt)
	ON_BN_CLICKED(IDC_BTN_NEXT_SUBGRAPH, OnBtnNextSubgraph)
	ON_BN_CLICKED(IDC_BTN_PREV_SUBGRAPH, OnBtnPrevSubgraph)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetting message handlers

BOOL CDlgGraphSetting::PreTranslateMessage(MSG* pMsg) 
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

BOOL CDlgGraphSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//사용할 Data 콤보박스 리스트채우기.
	AddStringToPacketCombo();
	m_pPacketComboBox.m_hListBox = 0;

	//Graph설정 Dlg의 초기화.
	if(!m_strIndicatorName.GetLength() && !m_bIsSelectedPacketData) 
 		m_ExpandDialog.Initialize(this, FALSE, IDC_SHRINKMARK);
	else {
 		m_ExpandDialog.Initialize(this, TRUE, IDC_SHRINKMARK);
		SetSubGraphDlgOnGraphSettingDlg();
	}

	//함수명 콤보박스 리스트채우기.
	AddStringToIndicatorFunctionCombo();
//	OnScalejoin();	
	CreateToolTip();
	
	//sy 2006.04.17.
	//signal 체크 표시 보이기, 안보이기
	CheckSignal( m_bSignalUse ? true : false);
	//if(!DoThisIndicatorNeedSignal(m_strIndicatorName))
	//	m_Signal_Bt.ShowWindow(SW_HIDE);
	//sy end

	//data 보기 enable, disable
	CheckDataViewItems();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGraphSetting::OnEtcsetBt() 
{	
	ML_SET_LANGUAGE_RES();
	CDlgGraphSetting2 etcDlg(&m_graphSetting2Data);
	if(IDOK != etcDlg.DoModal())
		return;
}

void CDlgGraphSetting::OnOK() 
{
	UpdateData(TRUE);

	// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	//		In the changing of Indicator, The Graph Name must be resetted for rebinding with Indicator Info.
	if( m_pItem->m_strIndicatorName != _LTOM2( m_strIndicatorName))
	{
		m_pItem->m_strIndicatorName = _LTOM2( m_strIndicatorName);
		m_pItem->m_strGraphName = _LTOM2( m_strIndicatorName);
	}

	m_pItem->m_strSubGraphDatas = GetSubGraphDatas( m_pItem->m_strIndicatorName);
//	m_pItem->m_strCheckedPacketItems = m_strCheckedPacketItems;
	if(!m_strCheckedPacketItems.IsEmpty())
	{
		if(m_strCheckedPacketItems.Right(1) != ",")
			m_pItem->m_strCheckedPacketItems = CString( _LTOM0( m_strCheckedPacketItems)) + ",";
		else
			m_pItem->m_strCheckedPacketItems = _LTOMS( m_strCheckedPacketItems, ",", 0);

		//<<20100323_JS.Kim 솔로몬.FX 맵차트콘트롤에서 매도/매수를 동시에 그리기 위해 종가 데이타가 매수종가/매도종가 2가지가 있다.
		//	이를 구분하기 위해 GraphName 을 PacketName으로 넣어준다
		CString strPacketItem = m_pItem->m_strCheckedPacketItems.Left(m_pItem->m_strCheckedPacketItems.GetLength()-1);
		if( m_pItem->m_strIndicatorName == "_Price MA_" && (strPacketItem == "_BuyClose_" || strPacketItem == "_SellClose_") )
			m_pItem->m_strGraphName = strPacketItem;
		//>>
	}
	else
		m_pItem->m_strCheckedPacketItems = m_strCheckedPacketItems;

	//sy 2006.04.17.
	SetSignal(m_pItem);
	//m_pItem->m_bSignalShow = m_bSignalShow == 1 ? true : false;
	//sy end

	//sy test
	m_pItem->m_bHighNowUpDown = m_graphSetting2Data.IsHighNow();
	m_pItem->m_bLowHighUpDown = m_graphSetting2Data.IsLowHigh();
	m_pItem->m_bLowNowUpDown = m_graphSetting2Data.IsLowNow();
	m_pItem->m_bPreNowContrast = m_graphSetting2Data.IsPrevNow();
	m_pItem->m_nCurData = m_graphSetting2Data.GetCurDataType();
	//sy end

	CDialog::OnOK();
}

//Packet페이지에서 받은 string을 이용해서 packetlist의 콤보를 채움.
void CDlgGraphSetting::AddStringToPacketCombo()
{
	CString _szData = m_strAllItemInfoOfPacketPage;

	if(_szData.Right(2) != "\r\n")
		_szData += "\r\n";

	int nCount = CDataConversion::GetStringDataCount(_szData, "\r\n");
	for(int i =0 ; i< nCount; i++)
	{
		CString szItemData = CDataConversion::GetStringData(_szData, "\r\n");
 		int nCheck= atoi(CDataConversion::GetStringData(szItemData, ";"));
		if(nCheck)
		{
			CString selectedData = CDataConversion::GetStringData(szItemData, ";");
			m_pPacketComboBox.AddString( _MTOL0( selectedData));
		}
	}
	CheckPacketbyCheckedPacketItems();
}

//툴팁
void CDlgGraphSetting::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_INDICATORFUNCTIONNAME_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC11, dwaInfo);
	m_toolTip.AddControlInfo( IDC_INDICATORFUNCTIONNAME, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_PACKETLIST_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC12, dwaInfo);
	m_toolTip.AddControlInfo( IDC_COMBO_PACKETLIST, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_SACLESTYLE_TOOLTIP);
	m_toolTip.AddControlInfo( IDC_STATIC20, dwaInfo);
	m_toolTip.AddControlInfo( IDC_COMBO_SACLESTYLE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_GRAPHSET_STATIC13_TOOLTIP_1);
	dwaInfo.Add( IDS_GRAPHSET_STATIC13_TOOLTIP_2);
	m_toolTip.AddControlInfo( IDC_STATIC13, dwaInfo);
}

void CDlgGraphSetting::OnEditchangeIndicatorfunctionname() 
{
	m_IndicatorFuctionComboBox.GetWindowText(m_strIndicatorName);
	m_strIndicatorName = _LTOM2( m_strIndicatorName);

	int nIndex = m_IndicatorFuctionComboBox.GetCurSel();

	if(nIndex == -1 || !m_strIndicatorName.GetLength() && !m_bCheckComboState) {
		m_ExpandDialog.OnExpandButton(false);
	}
	else if(nIndex == -1 || !m_strIndicatorName.GetLength() && m_bCheckComboState) {
		m_ExpandDialog.OnExpandButton(true);
		SetSubGraphDlgOnGraphSettingDlg();
	}
	else {
		m_ExpandDialog.OnExpandButton(true);
	}
	CheckDataViewItems();
	
}

void CDlgGraphSetting::OnSelchangeIndicatorfunctionname() 
{
	int nIndex = m_IndicatorFuctionComboBox.GetCurSel();
	m_IndicatorFuctionComboBox.GetLBText(nIndex, m_strIndicatorName);

	if(nIndex == -1) {
		m_ExpandDialog.OnExpandButton(false);
	}
	else{
		m_ExpandDialog.OnExpandButton(true);
		//sy 2006.04.17.
		//CheckDefaultPacketbyIndicatorName(m_strIndicatorName);
		//sy end
		SetSubGraphDlgOnGraphSettingDlg();
	}	

	//sy 2006.04.17.
	CheckGraphSettingbyIndicatorName( _LTOM2( m_strIndicatorName));
	UpdateData(FALSE);

	//if(!DoThisIndicatorNeedSignal(m_strIndicatorName))
	//	m_Signal_Bt.ShowWindow(SW_HIDE);
	//else
	//	m_Signal_Bt.ShowWindow(SW_SHOW);
	//sy end
}

void CDlgGraphSetting::OnSelchangeComboPacketlist() 
{
	m_bCheckComboState = m_pPacketComboBox.GetCurState();	
	if(m_bCheckComboState && !m_strIndicatorName.GetLength()) {
		m_ExpandDialog.OnExpandButton(true);
		SetSubGraphDlgOnGraphSettingDlg();
	}
	else if(!m_bCheckComboState && !m_strIndicatorName.GetLength())
		m_ExpandDialog.OnExpandButton(false);

	//콤보리스트가 닫히고 DlgGraphSetting에 Focus 보내기.
	PostMessage(WM_SETFOCUS);	
}

//체크 콤보박스에서 선택되어 있었던 Item에 체크처리
void CDlgGraphSetting::CheckPacketbyCheckedPacketItems()
{
	CString _szData = m_strCheckedPacketItems;

	if(_szData.Right(1) != ",")
		_szData += ",";

	CString PacketName = "";
	m_bIsSelectedPacketData = false;
	int nCount = CDataConversion::GetStringDataCount(_szData, ",");
	for(int i = 0 ; i < nCount ; i++) {
       CString szItemData = CDataConversion::GetStringData(_szData, ",");
	   szItemData.TrimLeft();
	   for(int j = 0 ; j < m_pPacketComboBox.GetCount(); j++) {
		  m_pPacketComboBox.GetLBText( j, PacketName);
		  if(PacketName.CompareNoCase(szItemData) == 0) {
				m_bIsSelectedPacketData = true;
				m_pPacketComboBox.SetCheck( j, TRUE);
		  }
	   }
	}

	if(!m_strCheckedPacketItems.GetLength()){
		m_pPacketComboBox.SetWindowText("");
		UpdateData(FALSE);
	}
}

void CDlgGraphSetting::AddStringToIndicatorFunctionCombo()
{

	if(m_strIndicatorType.Compare( _MTEXT( C5_SPECIAL_TYPE)) == 0)
		AddStringToIndicatorFunctionComboWhenSpecialGraphType();
	else
		AddStringToIndicatorFunctionComboWhenNoSpecialGraphType();

	int nIndex = m_IndicatorFuctionComboBox.FindString(0, _MTOL2( m_strIndicatorName));
	m_IndicatorFuctionComboBox.SetCurSel(nIndex);
}

CString CDlgGraphSetting::GetSubGraphDatas( const char *p_szIndicatorName)
{
	return m_GraphColorCollection->GetSubGraphDatas( p_szIndicatorName);
}


void CDlgGraphSetting::ButtonClicked(const int &nIndex)
{
	m_GraphColorCollection->ShowSubGraphSettingDlg(nIndex);
}

void CDlgGraphSetting::SetDisableWindow(UINT nRadioButton, const BOOL bCheck)
{
	CButton* pButton = (CButton*) GetDlgItem(nRadioButton);
	pButton->EnableWindow(bCheck);
/*
	if(m_strIndicatorFunction.GetLength()) 
		m_pPacketComboBox.EnableWindow(FALSE);

	bool bState = m_pPacketComboBox.GetCurState();	
	if(bState) 
		m_IndicatorFuctionComboBox.EnableWindow(FALSE);
	else if(!bState)
		m_IndicatorFuctionComboBox.EnableWindow(TRUE);*/
}

void CDlgGraphSetting::SetIndicatorNameToEditFromDlgGraphColor(const CString &strGraphStyle)
{
	m_strIndicatorName = strGraphStyle;
	UpdateData(FALSE);
}
/*
void CDlgGraphSetting::OnScalejoin() 
{
	CButton* pButton = (CButton*) GetDlgItem(IDC_SCALEJOIN);
	if(pButton != NULL)
		m_bScaleJoin = pButton->GetCheck();

	if(m_bScaleJoin == TRUE) {
		UpdateData(TRUE);
//		CheckRadioButton(IDC_SCALEPOS, IDC_RADIO_NOSCALE, IDC_RADIO_HIDE);
//		SetRadioDisableWindow(FALSE);
/////////
		CString strScaledata = GetScaleData();
		int nIndex = m_ScaleDataComboBox.FindString(0, strScaledata);
		m_ScaleDataComboBox.SetCurSel(nIndex);
		m_ScaleDataComboBox.EnableWindow(FALSE);
/////////
	}
	else {
//		SetRadioDisableWindow(TRUE);
		m_ScaleDataComboBox.EnableWindow(TRUE);
	}
}
*/
/*
void CDlgGraphSetting::Test(const int nChangeNum)
{
	CButton* pScalePositionButton = NULL;
	if(nChangeNum == 0)
		pScalePositionButton = (CButton*) GetDlgItem(IDC_SCALEPOS);
	else if(nChangeNum == 1)
		pScalePositionButton = (CButton*) GetDlgItem(IDC_RADIO_RIGHT);
	else if(nChangeNum == 2)
		pScalePositionButton = (CButton*) GetDlgItem(IDC_RADIO_LEFT);
	else if(nChangeNum == 3)
		pScalePositionButton = (CButton*) GetDlgItem(IDC_RADIO_HIDE);
	else if(nChangeNum == 4)
		pScalePositionButton = (CButton*) GetDlgItem(IDC_RADIO_NOSCALE);

	if(pScalePositionButton != NULL && m_bScaleJoin == TRUE){
		pScalePositionButton->SetCheck(0);
		pScalePositionButton = (CButton*) GetDlgItem(IDC_RADIO_HIDE);
		pScalePositionButton->SetCheck(1);
		m_nScalePosition = 3;
	}
}
*/
void CDlgGraphSetting::SetRadioDisableWindow(const BOOL bCheck)
{
	SetDisableWindow(IDC_SCALEPOS, bCheck);
	SetDisableWindow(IDC_RADIO_RIGHT, bCheck);
	SetDisableWindow(IDC_RADIO_LEFT, bCheck);
	SetDisableWindow(IDC_RADIO_NOSCALE, bCheck);
}


void CDlgGraphSetting::SetAllItemInfoOfPacketPage(const CString &strAllItemInfoOfPacketPage)
{
	m_strAllItemInfoOfPacketPage = strAllItemInfoOfPacketPage;
}

void CDlgGraphSetting::UnCheckAllCheckedPacketItems()
{
	//Check되어 있던 항목 모두 지우기.
	int count = m_pPacketComboBox.GetCount();
	for(int i = 0 ; i< count; i++) {
		if(m_pPacketComboBox.GetCheck(i))
			m_pPacketComboBox.SetCheck(i,FALSE);
	}
}


CString CDlgGraphSetting::GetIndicatorName()
{
	return m_strIndicatorName;
}

/*
CString CDlgGraphSetting::GetScaleData()
{

	bool bMyJoinGraph = true;
	CString strScale = "";
	CChartLink* pLink = m_pItem->m_Parent;
	if(pLink == NULL) return "";
	CChartLink* pItem = pLink->m_Child ;
	while(pItem) {
		if(pItem == NULL) return "";
		if(pItem->m_hItem == m_pItem->m_hItem)
			bMyJoinGraph = false;
		if(pItem->m_bScaleJoin == false && pItem->m_hItem != m_pItem->m_hItem && bMyJoinGraph == true) 
			strScale = pItem->m_strScaleStyle;
		pItem = pItem->m_Next;
	}
	return strScale;

// Root에서 얻을때..
//////////////////////
	bool bMyBlock = false;
	CString strScale = "";
	CChartLink* pLink = m_pItem->m_Child;
	while(pLink) {
		if(strScale.IsEmpty()) {
			CChartLink* pItem = pLink->m_Child ;
			while(pItem) {
				if(pItem == NULL) return "";
				if(pItem->m_hItem == m_pItem->m_hItem)
					bMyBlock = true;
				else
					strScale = pItem->m_strScaleStyle;
				pItem = pItem->m_Next;
			}
			if(bMyBlock == false)
				strScale = "";
		}
		pLink = pLink->m_Next ;
	}
	return strScale;
///////////////////////
}
*/

//signal이 필요한 지표인지..아닌지 확인한다.
bool CDlgGraphSetting::DoThisIndicatorNeedSignal(const CString &strIndicatorName)
{
	static IMetaTable::E_METACODE	eIndicators[] =
	{
		IMetaTable::C2_CCI,
		IMetaTable::C2_DISPARITY,
		IMetaTable::C2_PSYCHOLOGICAL_LINE,
		IMetaTable::C2_DMI,
		IMetaTable::C2_MFI,
		IMetaTable::C2_MOMENTUM,
		IMetaTable::C2_MASS_INDEX,
		IMetaTable::C2_RSI,
		IMetaTable::C2_PERCENT_R,
		IMetaTable::C2_VR,
		IMetaTable::C2_STOCHASTIC_FAST,
		IMetaTable::C2_STOCHASTIC_FAST2,
		IMetaTable::C2_STOCHASTIC_FAST3,
		IMetaTable::C2_STOCHASTIC_SLOW,
		IMetaTable::C2_STOCHASTIC_SLOW2,
		IMetaTable::C2_STOCHASTIC_SLOW3,
		IMetaTable::C2_ROC,
		IMetaTable::C2_AB_RATIO,
		IMetaTable::C2_ADR,
		IMetaTable::C2_BREADTH_THRUST,
		IMetaTable::C2_MCCLELLAN_OSCILLATOR,
		IMetaTable::C2_STIX,
		IMetaTable::C2_MACD
	};

	for( int i = 0; i < sizeof( eIndicators) / sizeof( IMetaTable::E_METACODE) ; i++)
	{
		if( strIndicatorName.CompareNoCase( _MTEXTN( eIndicators[ i])) == 0)
			return true;
	}
	return false;
}

void CDlgGraphSetting::CheckDataViewItems()
{
	// (2006/10/18 - Seung-Won, Bae) Enable ETC Data Setting in PriceChart
	GetDlgItem( IDC_ETCSET_BT)->EnableWindow( !strcmp( m_strIndicatorName, _MTEXT( C2_PRICE_CHART)) || !strcmp( m_strIndicatorName, _MTEXT( C2_VOLUME_CHART)));
}

//sy 2004.1.7.
// 일반지표 : 봉형, 선형, 바형
bool CDlgGraphSetting::IsGeneralIndicatorType() const
{
	return (m_strIndicatorType == _MTEXTN( pGraphTypeItem[0]) || 
		m_strIndicatorType == _MTEXTN( pGraphTypeItem[1]) || 
		m_strIndicatorType == _MTEXTN( pGraphTypeItem[2]));
}

// none 지표 : 선형, 데이터표현형, 선물옵션형
bool CDlgGraphSetting::IsNoneIndicatorType() const
{
	return (m_strIndicatorType == _MTEXTN( pGraphTypeItem[1]) || 
		m_strIndicatorType == _MTEXTN( pGraphTypeItem[2]) ||			// (2007/3/21 - Seung-Won, Bae) Add 'Bar' for '매수매도매물대'
		m_strIndicatorType == _MTEXTN( pGraphTypeItem[6]) || 
		m_strIndicatorType == _MTEXTN( pGraphTypeItem[7]));
}
//end sy

//signal 보기 enable, disable
void CDlgGraphSetting::CheckSignal(const bool bIsShow)
{
	//signal 체크 표시 보이기, 안보이기
	if(bIsShow)
		m_Signal_Bt.ShowWindow(SW_SHOW);
	else
		m_Signal_Bt.ShowWindow(SW_HIDE);
}

void CDlgGraphSetting::SetSignal(CChartLink* pItem)
{
	if(pItem == NULL)
		return;

	//m_pItem->m_bSignalShow = m_bSignalShow == 1 ? true : false;
	pItem->m_bSignalUse		= ( m_bSignalUse	? true : false);
	pItem->m_bSignalShow	= ( m_bSignalShow	? true : false);
}
//sy end


void CDlgGraphSetting::SetSubGraphDlgOnGraphSettingDlg()
{
	if(m_strOldIndicatorName != m_strIndicatorName)
		m_strSubGraphDatas = "";
	m_strOldIndicatorName = m_strIndicatorName;

	if(m_strSubGraphDatas.IsEmpty())
		SetSubGraphDatas_Indicater();

	CRect rect;
    (GetDlgItem(IDC_SUBFORM_FRAME))->GetWindowRect( &rect); // get the position for the subforms
	CRect buttonrect;
    (GetDlgItem(IDC_BUTTON_FRAME))->GetWindowRect(&buttonrect); // get the position for the subforms
	ScreenToClient( buttonrect);
	ML_SET_LANGUAGE_RES();
	m_GraphColorCollection->CreateSubGraphSettingDialogandButton( rect, buttonrect, _LTOM5( m_strIndicatorType), _LTOM2( m_strIndicatorName), m_strSubGraphDatas, m_pLineColor, m_pIGraphInfoManager, IDD_DIALOG_SUBGRAPHSETTING, (CWnd* )this);
}

void CDlgGraphSetting::SetSubGraphDatas_Indicater()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( m_pIGraphInfoManager == NULL) return;
	
	// (2006/11/23 - Seung-Won, Bae) Use User Indicator Info in Design Time.
	IGraphInfo * pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, _LTOM2( m_strIndicatorName), IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET_AND_SET);
	if( pIGraphInfo == NULL) return;
	pIGraphInfo->GetAllSubGraphDataForMaking( m_strSubGraphDatas);
	pIGraphInfo->Release();
}

//test -ok
void CDlgGraphSetting::AddStringToIndicatorFunctionComboWhenSpecialGraphType()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager

	if(m_pIGraphInfoManager == NULL) return;

	CList<CString, CString> specialIndiNameList;
	if( !m_pIGraphInfoManager->GetIndicatorNameList( IGraphInfoManager::IEILT_SITE_DEFAULT, IGraphInfoManager::IEIIT_SPECIAL, specialIndiNameList)) return;

	POSITION pos = specialIndiNameList.GetHeadPosition();
	while(pos != NULL) m_IndicatorFuctionComboBox.AddString( _MTOL2( specialIndiNameList.GetNext(pos)));
}

//sy 2004.1.7. : 지표들은 모든 GraphType을 쓰지 않으므로 쓰는 부분만
//GraphType(8) : 봉형, 선형, 바형, 파이형, 특수형, 펀드형, 데이터표현형, 선물옵션형
//특수지표 제외시 지표관련 GraphType(6) : 봉형, 선형, 바형
//test -ok
void CDlgGraphSetting::AddStringToIndicatorFunctionComboWhenNoSpecialGraphType()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	m_IndicatorFuctionComboBox.AddString("");
	if( m_pIGraphInfoManager == NULL) return;

	// 일반지표 : 봉형, 선형, 바형
	if( IsGeneralIndicatorType())
	{
		CList<CString, CString> indiNameList;
		if( m_pIGraphInfoManager->GetIndicatorNameList( IGraphInfoManager::IEILT_SITE_DEFAULT, IGraphInfoManager::IEIIT_GENERAL, indiNameList))
		{
			POSITION pos = indiNameList.GetHeadPosition();
			while( pos != NULL) m_IndicatorFuctionComboBox.AddString( _MTOL2( indiNameList.GetNext( pos)));
		}
	}

	// none 지표 : 선형, 데이터표현형, 선물옵션형
	if(IsNoneIndicatorType())
	{
		CList<CString, CString> indiNameList;
		if( m_pIGraphInfoManager->GetIndicatorNameList( IGraphInfoManager::IEILT_SITE_DEFAULT, IGraphInfoManager::IEIIT_NONE, indiNameList))
		{
			POSITION pos = indiNameList.GetHeadPosition();
			while( pos != NULL) m_IndicatorFuctionComboBox.AddString( _MTOL2( indiNameList.GetNext(pos)));
		}
	}
}

//sy 2006.04.17.
void CDlgGraphSetting::CheckGraphSettingbyIndicatorName(const CString& strIndicatorName)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	// (2006/11/23 - Seung-Won, Bae) Use User Indicator Info in Design Time.
	IGraphInfo *pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, strIndicatorName, IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET_AND_SET);
	CheckDefaultPacketbyIndicatorInfo(	pIGraphInfo);
	CheckGraphSetting2byIndicatorInfo(	pIGraphInfo, strIndicatorName);
	CheckSignalbyIndicatorInfo(			pIGraphInfo);
	if( pIGraphInfo) pIGraphInfo->Release();
}

//선택된 FunctionName에 따라 Packet다시 체크하기.
void CDlgGraphSetting::CheckDefaultPacketbyIndicatorInfo( IGraphInfo *p_pIGraphInfo)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	UnCheckAllCheckedPacketItems();
	if( p_pIGraphInfo == NULL) return;

	//sy 2003.6.3 -> 콤보의 내용을 콤보내에서 처리한다.
	m_pPacketComboBox.SetItemCheck( _MTOLS( p_pIGraphInfo->GetCommonCalculatePacketNames(), ",", 0));
}


// DlgGraphSetting2 data 변경
void CDlgGraphSetting::CheckGraphSetting2byIndicatorInfo( IGraphInfo *p_pIGraphInfo, const char *p_szIndicatorName)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	BOOL m_bHighNow( FALSE), m_bLowHigh( FALSE), m_bLowNow( FALSE), m_bPrevNow( FALSE);
	int m_nCurDataType = 0;
	if( p_pIGraphInfo) p_pIGraphInfo->GetGraphDataDrawingData( m_bLowHigh, m_bLowNow, m_bHighNow, m_bPrevNow, m_nCurDataType);
	m_graphSetting2Data.SetData( m_bHighNow ? true : false, m_bLowHigh ? true : false, m_bLowNow ? true : false, m_bPrevNow ? true : false, m_nCurDataType);

	// (2006/10/18 - Seung-Won, Bae) Enable ETC Data Setting in PriceChart
	GetDlgItem( IDC_ETCSET_BT)->EnableWindow( !strcmp( p_szIndicatorName, _MTEXT( C2_PRICE_CHART)) || !strcmp( p_szIndicatorName, _MTEXT( C2_VOLUME_CHART)));
}

void CDlgGraphSetting::CheckSignalbyIndicatorInfo( IGraphInfo *p_pIGraphInfo)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( p_pIGraphInfo == NULL) m_bSignalUse = m_bSignalShow = false;
	else p_pIGraphInfo->GetSignalInfo( m_bSignalUse, m_bSignalShow);

	CheckSignal( m_bSignalUse ? true : false);
}

// (2006/11/23 - Seung-Won, Bae) Support Scroll in SubGraph Tab Button
void CDlgGraphSetting::OnBtnNextSubgraph() 
{
	// TODO: Add your control notification handler code here
	if( !m_GraphColorCollection) return;
	m_GraphColorCollection->SetButtonPos( this, 1);
}
void CDlgGraphSetting::OnBtnPrevSubgraph() 
{
	// TODO: Add your control notification handler code here
	if( !m_GraphColorCollection) return;
	m_GraphColorCollection->SetButtonPos( this, -1);
}
