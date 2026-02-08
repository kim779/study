// EnvironmentData.cpp: implementation of the CEnvironmentData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnvironmentData.h"

#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEnvironmentData::CEnvironmentData():
	m_pParent(NULL),
	m_defaultTab(0),
	m_bUseOneChart(false)
{

}

CEnvironmentData::CEnvironmentData(CWnd* pParent,
								   const int defaultTab,
								   const bool bUseOneChart,
								   const CModifyPriceData& modifypricedata,
								   const CInquiryViewData& inquiryviewdata,
								   const CScaleData& scaledata,
								   const CColorData& colordata,
								   const CMouseEventData& mouseeventdata,
								   const CTitleData& titledata,
								   const CStyleData& styledata,
								   // 0114
								   const CAnalysisToolData& analysistooldata,
								   //sy 2005.04.19.
								   const CMATitleListData& maTitleListData, 
								   const CString defaultPath,
								   const CString userPath):
	m_pParent(NULL),
	m_defaultTab(0),
	m_bUseOneChart(false),
	m_defaultPath(""),
	m_userPath("")
{
	m_pParent = pParent;
	m_defaultTab = (defaultTab - 1);
	m_bUseOneChart = bUseOneChart;

	m_modifypricedata.SetSavedData(modifypricedata);
	m_inquiryviewdata.SetSavedData(inquiryviewdata);
	m_scaledata.SetSavedData(scaledata);
	m_colordata.SetSavedData(colordata);
	m_mouseevetdata.SetSavedData(mouseeventdata);
	m_titledata.SetSavedData(titledata);
	m_styledata.SetSavedData(styledata);
	m_defaultPath = defaultPath;
	m_userPath = userPath;
	// 0114
	m_analysistooldata = analysistooldata;
	//sy 2005.04.19.
	//m_titleListData.SetMATitleListData(maTitleListData);
}

CWnd* CEnvironmentData::GetParent() const
{
	return m_pParent;
}

int CEnvironmentData::GetDefaultTab() const
{
	return m_defaultTab;
}

CString	CEnvironmentData::GetDefaultPath() const
{
	return m_defaultPath;
}

CString CEnvironmentData::GetUserPath() const
{
	return m_userPath;
}

void CEnvironmentData::SetSavedData(const CEnvironmentData& environmentdata)
{
	m_pParent = environmentdata.GetParent();
	m_defaultTab = environmentdata.GetDefaultTab();
	if(m_defaultTab < 0)
		m_defaultTab = 0;
	m_bUseOneChart = environmentdata.DoUseOneChart(); 

	m_defaultPath = environmentdata.GetDefaultPath();
	m_userPath = environmentdata.GetUserPath();

	m_modifypricedata.SetSavedData(environmentdata.GetModifyPriceData());
	m_inquiryviewdata.SetSavedData(environmentdata.GetInquiryViewData());
	m_scaledata.SetSavedData(environmentdata.GetScaleData());
	m_colordata.SetSavedData(environmentdata.GetColorData());
	m_mouseevetdata.SetSavedData(environmentdata.GetMouseEventData());
	m_titledata.SetSavedData(environmentdata.GetTitleData());
	m_styledata.SetSavedData(environmentdata.GetStyleData());
	// 0114
	m_analysistooldata.SetSavedData(environmentdata.GetAnalysisToolData());
	//sy 2005.04.19.
	//m_titleListData.SetMATitleListData(environmentdata.GetMATitleListData());
}

CModifyPriceData* CEnvironmentData::GetModifyPriceData()
{
	return &m_modifypricedata;
}

CInquiryViewData* CEnvironmentData::GetInquiryViewData()
{
	return &m_inquiryviewdata;
}

CScaleData* CEnvironmentData::GetScaleData()
{
	return &m_scaledata;
}

CColorData* CEnvironmentData::GetColorData()
{
	return &m_colordata;
}

CMouseEventData* CEnvironmentData::GetMouseEventData()
{
	return &m_mouseevetdata;
}

CTitleData* CEnvironmentData::GetTitleData()
{
	return &m_titledata;
}

CStyleData*	CEnvironmentData::GetStyleData()
{
	return &m_styledata;
}

// 0114
CAnalysisToolData* CEnvironmentData::GetAnalysisToolData()
{
	return &m_analysistooldata;
}

//sy 2005.04.19.
CMATitleListData& CEnvironmentData::GetMATitleListData()
{
	return m_styledata.GetMATitleListData();
}

const CMATitleListData& CEnvironmentData::GetMATitleListData() const
{
	return m_styledata.GetMATitleListData();
}

void CEnvironmentData::SetMATitleListData(const CMATitleListData& sourceTitleListData)
{
	m_styledata.SetMATitleListData(sourceTitleListData);
}
//sy end

CModifyPriceData CEnvironmentData::GetModifyPriceData() const
{
	return m_modifypricedata;
}

CInquiryViewData CEnvironmentData::GetInquiryViewData() const
{
	return m_inquiryviewdata;
}

CScaleData CEnvironmentData::GetScaleData() const
{
	return m_scaledata;
}

CColorData CEnvironmentData::GetColorData() const
{
	return m_colordata;
}

CMouseEventData CEnvironmentData::GetMouseEventData() const
{
	return m_mouseevetdata;
}

CTitleData CEnvironmentData::GetTitleData() const
{
	return m_titledata;
}

CStyleData CEnvironmentData::GetStyleData() const
{
	return m_styledata;
}

// 0114
CAnalysisToolData CEnvironmentData::GetAnalysisToolData() const
{
	return m_analysistooldata;
}

bool CEnvironmentData::DoUseOneChart() const
{
	return m_bUseOneChart;
}

//수정주가
///////////////////////////////////////////////////
CModifyPriceData::CModifyPriceData():
	m_bModifyPrice(false),
	m_bDevidedlak(false)
{

}

CModifyPriceData::CModifyPriceData(const bool bModifyPrice, const bool bDevidedlak):
	m_bModifyPrice(false),
	m_bDevidedlak(false)
{
	m_bModifyPrice = bModifyPrice;
	m_bDevidedlak = bDevidedlak;
}

void CModifyPriceData::SetSavedData(const CModifyPriceData& modifypricedata)
{
	m_bModifyPrice = modifypricedata.IsModifyPrice();
	m_bDevidedlak = modifypricedata.IsDevidedlak();
}

void CModifyPriceData::SetModifyPrice(const bool bmodifyprice)
{
	m_bModifyPrice = bmodifyprice;
}

bool CModifyPriceData::IsModifyPrice() const
{
	return m_bModifyPrice;
}

void CModifyPriceData::SetDevidedlak(const bool bDevidedlak)
{
	m_bDevidedlak = bDevidedlak;
}

bool CModifyPriceData::IsDevidedlak() const
{
	return m_bDevidedlak;
}

///수치조회, 데이타보기
//////////////////////
CInquiryViewData::CInquiryViewData()
{

}

CInquiryViewData::CInquiryViewData(const int nNumericalInquiryDlgType, // 종찬(05/06/08) 수치조회창 타입.
								 const int nInquiryDataView, const bool bViewTotalData, const int nDataCountPerOnePage, const int nViewTool, const bool bShowInquiry, const int nDataCount/*=500*/, const int nMinDataCount/*=20*/,
 								 const bool bViewLowHigh/*=false*/, const bool bViewLow/*=false*/, const bool bViewHigh/*=false*/,
								 const bool bViewPrice/*=false*/, const bool bViewPercent/*=false*/,
								 const bool bShowTrendLine/*=true*/)
{
	m_nNumericalInquiryDlgType = nNumericalInquiryDlgType;
	m_nInquiryDataView = nInquiryDataView;
	m_bViewTotalData = bViewTotalData;
	m_nDataCountPerOnePage = nDataCountPerOnePage;
	m_nViewTool = nViewTool;
	m_bShowInquiry = bShowInquiry;
	m_nDataCount = nDataCount;
	m_nMinDataCount = nMinDataCount;
	// 1212
	m_bViewLowHigh = bViewLowHigh;
	m_bViewLow = bViewLow;
	m_bViewHigh = bViewHigh;
	m_bViewPrice = bViewPrice;
	m_bViewPercent = bViewPercent;
	// 0310 추세선추가
	m_bShowTrendLine = bShowTrendLine;
}

void CInquiryViewData::SetSavedData(const CInquiryViewData& viewdata)
{
	m_nNumericalInquiryDlgType = viewdata.GetNumericalInquiryDlgType();	// 종찬(05/06/08)
	m_nInquiryDataView = viewdata.GetInquiryDataView();
	m_bViewTotalData = viewdata.IsViewTotalData();
	m_nDataCountPerOnePage = viewdata.GetDataCountPerOnePage();
	m_nViewTool = viewdata.GetViewTool();
	m_bShowInquiry = viewdata.GetShowInquiry();
	m_nDataCount = viewdata.GetDataCount();
	m_nMinDataCount = viewdata.GetMinDataCount();
	// 1212
	m_bViewLowHigh = viewdata.GetViewLowHigh();
	m_bViewLow = viewdata.GetViewLow();
	m_bViewHigh = viewdata.GetViewHigh();
	m_bViewPrice = viewdata.GetViewPrice();
	m_bViewPercent = viewdata.GetViewPercent();
	// 0310 추세선추가
	m_bShowTrendLine = viewdata.GetShowTrendLine();
}

void CInquiryViewData::SetViewTotalData(const bool bViewTotalData)
{
	m_bViewTotalData = bViewTotalData;
}

bool CInquiryViewData::IsViewTotalData() const
{
	return m_bViewTotalData;
}

void CInquiryViewData::SetDataCountPreOnePage(const int nDataCount)
{
	m_nDataCountPerOnePage = nDataCount;
}

int CInquiryViewData::GetDataCountPerOnePage() const
{
	return m_nDataCountPerOnePage;
}

void CInquiryViewData::SetDataCount(const int nDataCount)
{
	m_nDataCount = nDataCount;
}

int CInquiryViewData::GetDataCount() const
{
	return m_nDataCount;
}

void CInquiryViewData::SetMinDataCount(const int nMinDataCount)
{
	m_nMinDataCount = nMinDataCount;
}

int CInquiryViewData::GetMinDataCount() const
{
	return m_nMinDataCount;
}

/*-------------------------------------------------------------------------------
 - Function    :  SetNumericalInquiryDlgType
 - Created at  :  2005-04-13   09:55
 - Author      :  최종찬
 - Description :  수치조회창타입 지정.
 -------------------------------------------------------------------------------*/
void CInquiryViewData::SetNumericalInquiryDlgType(const int nNumericalInquiryDlgType)
{
	m_nNumericalInquiryDlgType = nNumericalInquiryDlgType;
}


/*-------------------------------------------------------------------------------
 - Function    :  GetNumericalInquiryDlgType
 - Created at  :  2005-04-13   09:56
 - Author      :  최종찬
 - Description :  수치조회창타입 지정.
 -------------------------------------------------------------------------------*/
int CInquiryViewData::GetNumericalInquiryDlgType() const
{
	return m_nNumericalInquiryDlgType;
}

void CInquiryViewData::SetInquiryDataView(const int ninquirydataview)
{
	m_nInquiryDataView = ninquirydataview;
}

int CInquiryViewData::GetInquiryDataView() const
{
	return m_nInquiryDataView;
}

void CInquiryViewData::SetViewTool(const int nViewTool)
{
	m_nViewTool = nViewTool;
}

int CInquiryViewData::GetViewTool() const
{
	return m_nViewTool;
}

void CInquiryViewData::SetShowInquiry(const bool bShowInquiry)
{
	m_bShowInquiry = bShowInquiry;
}

bool CInquiryViewData::GetShowInquiry() const
{
	return m_bShowInquiry;
}

// 1212
void CInquiryViewData::SetViewLowHigh(const bool bViewLowHigh)
{
	m_bViewLowHigh = bViewLowHigh;
}

bool CInquiryViewData::GetViewLowHigh() const
{
	return m_bViewLowHigh;
}

void CInquiryViewData::SetViewLow(const bool bViewLow)
{
	m_bViewLow = bViewLow;
}

bool CInquiryViewData::GetViewLow() const
{
	return m_bViewLow;
}

void CInquiryViewData::SetViewHigh(const bool bViewHigh)
{
	m_bViewHigh = bViewHigh;
}

bool CInquiryViewData::GetViewHigh() const
{
	return m_bViewHigh;
}

void CInquiryViewData::SetViewPrice(const bool bViewPrice)
{
	m_bViewPrice= bViewPrice;
}

bool CInquiryViewData::GetViewPrice() const
{
	return m_bViewPrice;
}

void CInquiryViewData::SetViewPercent(const bool bViewPercent)
{
	m_bViewPercent = bViewPercent;
}

bool CInquiryViewData::GetViewPercent() const
{
	return m_bViewPercent;
}

// 0310 추세선추가
void CInquiryViewData::SetShowTrendLine(const bool bShowTrendLine)
{
	m_bShowTrendLine = bShowTrendLine;
}

bool CInquiryViewData::GetShowTrendLine() const
{
	return m_bShowTrendLine;
}

//Scale
//////////////////////////
CScaleData::CScaleData():
	m_nRightMargin(0),
	m_nScalePosition(0)
{

}

	CScaleData::CScaleData(const int nRightMargin, const int nScalePosition):
	m_nRightMargin(0),
	m_nScalePosition(0)
{
	m_nRightMargin = nRightMargin;
	m_nScalePosition = nScalePosition;
}

void CScaleData::SetSavedData(const CScaleData& scaledata)
{
	m_nRightMargin = scaledata.GetRightMargin();
	m_nScalePosition = scaledata.GetScalePosition();
}

void CScaleData::SetRightMargin(const int nRightMargin)
{
	m_nRightMargin = nRightMargin;
}

int CScaleData::GetRightMargin() const
{
	return m_nRightMargin;
}

void CScaleData::SetScalePosition(const int nScalePosition)
{
	m_nScalePosition = nScalePosition;
}

int CScaleData::GetScalePosition() const
{
	return m_nScalePosition;
}

//CColorData
////////////////////////////
CColorData::CColorData()
{

}

CColorData::CColorData(const COLORREF& BackGroundColor, const COLORREF& BackBorderColor,
					   const COLORREF& BlockGroundColor, const COLORREF& BlockBorderColor,
					   const COLORREF& HorizonScaleLineColor, const COLORREF& VerticalScaleLineColor,
					   const COLORREF& ScaleTextColor, const COLORREF& AnalyzeToolColor,
					   const COLORREF& NumericalColor, const COLORREF& LatticeColor)
{
	m_BackGroundColor = BackGroundColor;
	m_BackBorderColor = BackBorderColor;
	m_BlockGroundColor = BlockGroundColor;
	m_BlockBorderColor = BlockBorderColor;
	m_HorizonScaleLineColor = HorizonScaleLineColor;
	m_VerticalScaleLineColor = VerticalScaleLineColor;
	m_ScaleTextColor = ScaleTextColor;
	m_AnalyzeToolColor = AnalyzeToolColor;
	m_NumericalColor = NumericalColor;
	m_LatticeColor = LatticeColor;
}

void CColorData::SetSavedData(const CColorData& colordata)
{
	m_BackGroundColor = colordata.GetBackGroundColor();
	m_BackBorderColor = colordata.GetBackBorderColor();
	m_BlockGroundColor = colordata.GetBlockGroundColor();
	m_BlockBorderColor = colordata.GetBlockBorderColor();
	m_HorizonScaleLineColor = colordata.GetHorizonScaleLineColor();
	m_VerticalScaleLineColor = colordata.GetVerticalScaleLineColor();
	m_ScaleTextColor = colordata.GetScaleTextColor();
	m_AnalyzeToolColor = colordata.GetAnalyzeToolColor();
	m_NumericalColor = colordata.GetNumericalViewColor();
	m_LatticeColor = colordata.GetLatticeColor();
}

COLORREF CColorData::GetBackGroundColor() const
{
	return m_BackGroundColor;
}

COLORREF CColorData::GetBackBorderColor() const
{
	return m_BackBorderColor;
}

COLORREF CColorData::GetBlockGroundColor() const
{
	return m_BlockGroundColor;
}

COLORREF CColorData::GetBlockBorderColor() const
{
	return m_BlockBorderColor;
}

COLORREF CColorData::GetHorizonScaleLineColor() const
{
	return m_HorizonScaleLineColor;
}

COLORREF CColorData::GetVerticalScaleLineColor() const
{
	return m_VerticalScaleLineColor;
}

COLORREF CColorData::GetScaleTextColor() const
{
	return m_ScaleTextColor;
}

COLORREF CColorData::GetAnalyzeToolColor() const
{
	return m_AnalyzeToolColor;
}

COLORREF CColorData::GetNumericalViewColor() const
{
	return m_NumericalColor;
}

void CColorData::SetBackGroundColor(const COLORREF& clrbackground)
{
	m_BackGroundColor = clrbackground;
}

void CColorData::SetBackBorderColor(const COLORREF &clrBackBorder)
{
	m_BackBorderColor = clrBackBorder;
}

void CColorData::SetBlockGroundColor(const COLORREF &clrBlockGround)
{
	m_BlockGroundColor = clrBlockGround;
}

void CColorData::SetBlockBorderColor(const COLORREF& clrBorderColor)
{
	m_BlockBorderColor = clrBorderColor;
}

void CColorData::SetHorizonScaleLineColor(const COLORREF &clrHorizonScaleLine)
{
	m_HorizonScaleLineColor = clrHorizonScaleLine;
}

void CColorData::SetVerticalScaleLineColor(const COLORREF &clrVerticalScaleLine)
{
	m_VerticalScaleLineColor = clrVerticalScaleLine;
}

void CColorData::SetScaleTextColor(const COLORREF &clrScaleText)
{
	m_ScaleTextColor = clrScaleText;
}

void CColorData::SetAnalyzeToolColor(const COLORREF &clrAnalyzeTool)
{
	m_AnalyzeToolColor = clrAnalyzeTool;
}

void CColorData::SetNumericalViewColor(const COLORREF &clrNumericalView)
{
	m_NumericalColor = clrNumericalView;
}

COLORREF CColorData::GetLatticeColor() const
{
	return m_LatticeColor;
}

void CColorData::SetLatticeColor(const COLORREF &clrLattice)
{
	m_LatticeColor = clrLattice;
}

////////////////////
///mouse DragDrop시
CMouseEventData::CMouseEventData(const int nMouseEvent):
	m_nMouseEvent(1)
{
	m_nMouseEvent = nMouseEvent;
}
	
CMouseEventData::CMouseEventData():
	m_nMouseEvent(1)
{
}

CMouseEventData::~CMouseEventData()
{
}

int CMouseEventData::GetMouseEvent() const
{
	return m_nMouseEvent;
}

void CMouseEventData::SetMouseEvent(const int bMouseEvent)
{
	m_nMouseEvent = bMouseEvent;
}

void CMouseEventData::SetSavedData(const CMouseEventData& mouseeventdata)
{
	m_nMouseEvent = mouseeventdata.GetMouseEvent();
}

////////////////////
///title 설정
CTitleData::CTitleData(const bool bUseCondition, const bool bUseRecentData):
	m_bUseConditiontoTitle(false),
	m_bUseRecentDatatoTitle(false)
{
	m_bUseConditiontoTitle = bUseCondition;
	m_bUseRecentDatatoTitle = bUseRecentData;
}
	
CTitleData::CTitleData():
	m_bUseConditiontoTitle(false),
	m_bUseRecentDatatoTitle(false)
{
}

CTitleData::~CTitleData()
{
}

bool CTitleData::IsDisplayCondition() const
{
	return m_bUseConditiontoTitle;
}

bool CTitleData::IsDisplayRecentData() const
{
	return m_bUseRecentDatatoTitle;
}

void CTitleData::SetYesorNotDisplayCondition(const int bUseCondition)
{
	m_bUseConditiontoTitle = CDataConversion::atob(bUseCondition);
}

void CTitleData::SetYesorNotDisplayRecentData(const int bUseRecentData)
{
	m_bUseRecentDatatoTitle = CDataConversion::atob(bUseRecentData);
}

void CTitleData::SetSavedData(const CTitleData& titledata)
{
	m_bUseConditiontoTitle = titledata.IsDisplayCondition();
	m_bUseRecentDatatoTitle = titledata.IsDisplayRecentData();
}


//CStyleData
////////////////////////////
CStyleData::CStyleData():
	m_priceUpColor(RGB(255, 0, 0)),
	m_priceDownColor(RGB(0, 0, 255)),
	m_priceEqualColor(RGB(0, 0, 255)),
	m_priceUpFill(true),
	m_priceDownFill(true),
	m_tradeUpColor(RGB(255, 0, 0)),
	m_tradeDownColor(RGB(0, 0, 255)),
	m_tradeEqualColor(RGB(0, 0, 255)),
	m_tradeUpFill(true),
	m_tradeDownFill(true),
	m_priceColor1(RGB(204, 204, 204)),
	m_priceColor2(RGB(204, 204, 204)),
	m_priceColor3(RGB(204, 204, 204)),
	m_priceColor4(RGB(204, 204, 204)),
	m_priceColor5(RGB(204, 204, 204)),
	m_priceColor6(RGB(204, 204, 204)),
	m_priceColor7(RGB(204, 204, 204)),
	m_priceColor8(RGB(204, 204, 204)),
	m_priceColor9(RGB(204, 204, 204)),
	m_priceColor10(RGB(204, 204, 204)),
	m_tradeColor1(RGB(204, 204, 204)),
	m_tradeColor2(RGB(204, 204, 204)),
	m_tradeColor3(RGB(204, 204, 204)),
	m_tradeColor4(RGB(204, 204, 204)),
	m_graphBGColor(RGB(204, 204, 204)),
	m_graphBorderColor(RGB(204, 204, 204)),
	m_graphGridColor(RGB(204, 204, 204)),
	m_scaleBGColor(RGB(204, 204, 204)),
	m_scaleBorderColor(RGB(204, 204, 204)),
	m_scaleTextColor(RGB(204, 204, 204)),
	m_scaleGridColor(RGB(204, 204, 204)),
	m_gridWidth(0), 
	m_gridHeight(0), 
	m_divideWidth(1),
	m_divideHeight(1),
	m_analyzeColor(RGB(0, 0, 0)),
	m_numericalColor(RGB(0, 0, 0))
{
}

CStyleData::CStyleData(const CStyleData& styleData)
{
	SetSavedData(styleData);
}

CStyleData::CStyleData(const CString& styleName,
			   const COLORREF& priceUpColor, const COLORREF& priceDownColor, const COLORREF& priceEqualColor,
			   const bool& priceUpFill, const bool& priceDownFill,
			   const COLORREF& tradeUpColor, const COLORREF& tradeDownColor, const COLORREF& tradeEqualColor,
			   const bool& tradeUpFill, const bool& tradeDownFill,
			   const COLORREF& priceColor1, const COLORREF& priceColor2, const COLORREF& priceColor3, const COLORREF& priceColor4, const COLORREF& priceColor5, 
			   const COLORREF& priceColor6, const COLORREF& priceColor7, const COLORREF& priceColor8, const COLORREF& priceColor9, const COLORREF& priceColor10, 
			   const COLORREF& tradeColor1, const COLORREF& tradeColor2, const COLORREF& tradeColor3, const COLORREF& tradeColor4, 
			   const COLORREF& graphBGColor, const COLORREF& graphBorderColor, const COLORREF& graphGridColor,
			   const COLORREF& scaleBGColor, const COLORREF& scaleBorderColor, const COLORREF& scaleTextColor, const COLORREF& scaleGridColor,
			   const int& gridWidth, const int& gridHeight,
			   const int& divideWidth, const int& divideHeight,
			   const COLORREF& analyzeColor/*=RGB(0,0,255)*/, const COLORREF& numericalColor/*=RGB(255,255,255)*/)
{
	m_styleName			= styleName;
	m_priceUpColor		= priceUpColor;
	m_priceDownColor	= priceDownColor;
	m_priceEqualColor	= priceEqualColor;
	m_priceUpFill		= priceUpFill;
	m_priceDownFill		= priceDownFill;
	m_tradeUpColor		= tradeUpColor;
	m_tradeDownColor	= tradeDownColor;
	m_tradeEqualColor	= tradeEqualColor;
	m_tradeUpFill		= tradeUpFill;
	m_tradeDownFill		= tradeDownFill;
	m_priceColor1		= priceColor1;
	m_priceColor2		= priceColor2;
	m_priceColor3		= priceColor3;
	m_priceColor4		= priceColor4;
	m_priceColor5		= priceColor5;
	m_priceColor6		= priceColor6;
	m_priceColor7		= priceColor7;
	m_priceColor8		= priceColor8;
	m_priceColor9		= priceColor9;
	m_priceColor10		= priceColor10;
	m_tradeColor1		= tradeColor1;
	m_tradeColor2		= tradeColor2;
	m_tradeColor3		= tradeColor3;
	m_tradeColor4		= tradeColor4;
	m_graphBGColor		= graphBGColor;
	m_graphBorderColor	= graphBorderColor;
	m_graphGridColor	= graphGridColor;
	m_scaleBGColor		= scaleBGColor;
	m_scaleBorderColor	= scaleBorderColor;
	m_scaleTextColor	= scaleTextColor;
	m_scaleGridColor	= scaleGridColor;
	m_gridWidth			= gridWidth;
	m_gridHeight		= gridHeight;
	m_divideWidth		= divideWidth;
	m_divideHeight		= divideHeight;
	m_analyzeColor		= analyzeColor;
	m_numericalColor	= numericalColor;
}

void CStyleData::SetSavedData(const CStyleData& styleData)
{
	m_styleName			= styleData.GetStyleName();
	m_priceUpColor		= styleData.GetPriceUpColor();
	m_priceDownColor	= styleData.GetPriceDownColor();
	m_priceEqualColor	= styleData.GetPriceEqualColor();
	m_priceUpFill		= styleData.GetPriceUpFill();
	m_priceDownFill		= styleData.GetPriceDownFill();
	m_tradeUpColor		= styleData.GetTradeUpColor();
	m_tradeDownColor	= styleData.GetTradeDownColor();
	m_tradeEqualColor	= styleData.GetTradeEqualColor();
	m_tradeUpFill		= styleData.GetTradeUpFill();
	m_tradeDownFill		= styleData.GetTradeDownFill();
	m_priceColor1		= styleData.GetPriceColor1();
	m_priceColor2		= styleData.GetPriceColor2();
	m_priceColor3		= styleData.GetPriceColor3();
	m_priceColor4		= styleData.GetPriceColor4();
	m_priceColor5		= styleData.GetPriceColor5();
	m_priceColor6		= styleData.GetPriceColor6();
	m_priceColor7		= styleData.GetPriceColor7();
	m_priceColor8		= styleData.GetPriceColor8();
	m_priceColor9		= styleData.GetPriceColor9();
	m_priceColor10		= styleData.GetPriceColor10();
	m_tradeColor1		= styleData.GetTradeColor1();
	m_tradeColor2		= styleData.GetTradeColor2();
	m_tradeColor3		= styleData.GetTradeColor3();
	m_tradeColor4		= styleData.GetTradeColor4();
	m_graphBGColor		= styleData.GetGraphBGColor();
	m_graphBorderColor	= styleData.GetGraphBorderColor();
	m_graphGridColor	= styleData.GetGraphGridColor();
	m_scaleBGColor		= styleData.GetScaleBGColor();
	m_scaleBorderColor	= styleData.GetScaleBorderColor();
	m_scaleTextColor	= styleData.GetScaleTextColor();
	m_scaleGridColor	= styleData.GetScaleGridColor();
	m_gridWidth			= styleData.GetGridWidth();
	m_gridHeight		= styleData.GetGridHeight();
	m_divideWidth		= styleData.GetDivideWidth();
	m_divideHeight		= styleData.GetDivideHeight();
	m_analyzeColor		= styleData.GetAnalyzeColor();
	m_numericalColor	= styleData.GetNumericalColor();
	
	m_titleListData.SetMATitleListData(styleData.GetMATitleListData());
}

///////////////////////////////////
// Get Methods
CString	CStyleData::GetStyleName() const
{
	return m_styleName;
}

COLORREF CStyleData::GetPriceUpColor() const
{
	return m_priceUpColor;
}

COLORREF CStyleData::GetPriceDownColor() const
{
	return m_priceDownColor;
}

COLORREF CStyleData::GetPriceEqualColor() const
{
	return m_priceEqualColor;
}

bool CStyleData::GetPriceUpFill() const
{
	return m_priceUpFill;
}

bool CStyleData::GetPriceDownFill() const
{
	return m_priceDownFill;
}

COLORREF CStyleData::GetTradeUpColor() const
{
	return m_tradeUpColor;
}

COLORREF CStyleData::GetTradeDownColor() const
{
	return m_tradeDownColor;
}

COLORREF CStyleData::GetTradeEqualColor() const
{
	return m_tradeEqualColor;
}

bool CStyleData::GetTradeUpFill() const
{
	return m_tradeUpFill;
}

bool CStyleData::GetTradeDownFill() const
{
	return m_tradeDownFill;
}

COLORREF CStyleData::GetPriceColor1() const
{
	return m_priceColor1;
}

COLORREF CStyleData::GetPriceColor2() const
{
	return m_priceColor2;
}

COLORREF CStyleData::GetPriceColor3() const
{
	return m_priceColor3;
}

COLORREF CStyleData::GetPriceColor4() const
{
	return m_priceColor4;
}

COLORREF CStyleData::GetPriceColor5() const
{
	return m_priceColor5;
}

COLORREF CStyleData::GetPriceColor6() const
{
	return m_priceColor6;
}

COLORREF CStyleData::GetPriceColor7() const
{
	return m_priceColor7;
}

COLORREF CStyleData::GetPriceColor8() const
{
	return m_priceColor8;
}

COLORREF CStyleData::GetPriceColor9() const
{
	return m_priceColor9;
}

COLORREF CStyleData::GetPriceColor10() const
{
	return m_priceColor10;
}

COLORREF CStyleData::GetTradeColor1() const
{
	return m_tradeColor1;
}

COLORREF CStyleData::GetTradeColor2() const
{
	return m_tradeColor2;
}

COLORREF CStyleData::GetTradeColor3() const
{
	return m_tradeColor3;
}

COLORREF CStyleData::GetTradeColor4() const
{
	return m_tradeColor4;
}

COLORREF CStyleData::GetGraphBGColor() const
{
	return m_graphBGColor;
}

COLORREF CStyleData::GetGraphBorderColor() const
{
	return m_graphBorderColor;
}

COLORREF CStyleData::GetGraphGridColor() const
{
	return m_graphGridColor;
}

COLORREF CStyleData::GetScaleBGColor() const
{
	return m_scaleBGColor;
}

COLORREF CStyleData::GetScaleBorderColor() const
{
	return m_scaleBorderColor;
}

COLORREF CStyleData::GetScaleTextColor() const
{
	return m_scaleTextColor;
}

COLORREF CStyleData::GetScaleGridColor() const
{
	return m_scaleGridColor;
}

int CStyleData::GetGridWidth() const
{
	return m_gridWidth;
}

int CStyleData::GetGridHeight() const
{
	return m_gridHeight;
}

int CStyleData::GetDivideWidth() const
{
	return m_divideWidth;
}

int CStyleData::GetDivideHeight() const
{
	return m_divideHeight;
}

COLORREF CStyleData::GetAnalyzeColor() const
{
	return m_analyzeColor;
}

COLORREF CStyleData::GetNumericalColor() const
{
	return m_numericalColor;
}

CMATitleListData& CStyleData::GetMATitleListData()
{
	return m_titleListData;
}

const CMATitleListData& CStyleData::GetMATitleListData() const
{
	return m_titleListData;
}

void CStyleData::SetMATitleListData(const CMATitleListData& sourceTitleListData)
{
	m_titleListData.SetMATitleListData(sourceTitleListData);
}


///////////////////////////////////
// Set Methods
void CStyleData::SetStyleName(const CString& name)
{
	m_styleName = name;
}

void CStyleData::SetPriceUpColor(const COLORREF& clrPriceUp)
{
	m_priceUpColor = clrPriceUp;
}

void CStyleData::SetPriceDownColor(const COLORREF& clrPriceDown)
{
	m_priceDownColor = clrPriceDown;
}

void CStyleData::SetPriceEqualColor(const COLORREF& clrPriceEqual)
{
	m_priceEqualColor = clrPriceEqual;
}

void CStyleData::SetPriceUpFill(const bool& priceUpFill)
{
	m_priceUpFill = priceUpFill;
}

void CStyleData::SetPriceDownFill(const bool& priceDownFill)
{
	m_priceDownFill = priceDownFill;
}

void CStyleData::SetTradeUpColor(const COLORREF& clrTradeUp)
{
	m_tradeUpColor = clrTradeUp;
}

void CStyleData::SetTradeDownColor(const COLORREF& clrTradeDown)
{
	m_tradeDownColor = clrTradeDown;
}

void CStyleData::SetTradeEqualColor(const COLORREF& clrTradeEqual)
{
	m_tradeEqualColor = clrTradeEqual;
}

void CStyleData::SetTradeUpFill(const bool& tradeUpFill)
{
	m_tradeUpFill = tradeUpFill;
}

void CStyleData::SetTradeDownFill(const bool& tradeDownFill)
{
	m_tradeDownFill = tradeDownFill;
}

void CStyleData::SetPriceColor1(const COLORREF& clrPrice)
{
	m_priceColor1 = clrPrice;
}

void CStyleData::SetPriceColor2(const COLORREF& clrPrice)
{
	m_priceColor2 = clrPrice;
}

void CStyleData::SetPriceColor3(const COLORREF& clrPrice)
{
	m_priceColor3 = clrPrice;
}

void CStyleData::SetPriceColor4(const COLORREF& clrPrice)
{
	m_priceColor4 = clrPrice;
}

void CStyleData::SetPriceColor5(const COLORREF& clrPrice)
{
	m_priceColor5 = clrPrice;
}

void CStyleData::SetPriceColor6(const COLORREF& clrPrice)
{
	m_priceColor6 = clrPrice;
}

void CStyleData::SetPriceColor7(const COLORREF& clrPrice)
{
	m_priceColor7 = clrPrice;
}

void CStyleData::SetPriceColor8(const COLORREF& clrPrice)
{
	m_priceColor8 = clrPrice;
}

void CStyleData::SetPriceColor9(const COLORREF& clrPrice)
{
	m_priceColor9 = clrPrice;
}

void CStyleData::SetPriceColor10(const COLORREF& clrPrice)
{
	m_priceColor10 = clrPrice;
}

void CStyleData::SetTradeColor1(const COLORREF& clrTrade)
{
	m_tradeColor1 = clrTrade;
}

void CStyleData::SetTradeColor2(const COLORREF& clrTrade)
{
	m_tradeColor2 = clrTrade;
}

void CStyleData::SetTradeColor3(const COLORREF& clrTrade)
{
	m_tradeColor3 = clrTrade;
}

void CStyleData::SetTradeColor4(const COLORREF& clrTrade)
{
	m_tradeColor4 = clrTrade;
}

void CStyleData::SetGraphBGColor(const COLORREF& clrGraphBG)
{
	m_graphBGColor = clrGraphBG;
}

void CStyleData::SetGraphBorderColor(const COLORREF& clrGraphBorder)
{
	m_graphBorderColor = clrGraphBorder;
}

void CStyleData::SetGraphGridColor(const COLORREF& clrGraphGrid)
{
	m_graphGridColor = clrGraphGrid;
}

void CStyleData::SetScaleBGColor(const COLORREF& clrScaleBG)
{
	m_scaleBGColor = clrScaleBG;
}

void CStyleData::SetScaleBorderColor(const COLORREF& clrScaleBorder)
{
	m_scaleBorderColor = clrScaleBorder;
}

void CStyleData::SetScaleTextColor(const COLORREF& clrScaleText)
{
	m_scaleTextColor = clrScaleText;
}

void CStyleData::SetScaleGridColor(const COLORREF& clrScaleGrid)
{
	m_scaleGridColor = clrScaleGrid;
}

void CStyleData::SetGridWidth(const int& gridWidth)
{
	m_gridWidth = gridWidth;
}

void CStyleData::SetGridHeight(const int& gridHeight)
{
	m_gridHeight = gridHeight;
}

void CStyleData::SetDivideWidth(const int &divideWidth)
{
	m_divideWidth = divideWidth;
}

void CStyleData::SetDivideHeight(const int &divideHeight)
{
	m_divideHeight = divideHeight;
}

void CStyleData::SetAnalyzeColor(COLORREF clrAnalyze)
{
	m_analyzeColor = clrAnalyze;
}

void CStyleData::SetNumericalColor(COLORREF clrNumerical)
{
	m_numericalColor = clrNumerical;
}

////////////////////
///분석도구 설정시
// 0114
CAnalysisToolData::CAnalysisToolData()
{
	m_bShowTrendLine	= false;
	m_bShowVertical		= false;
	m_bShowHorizonLeft	= false;
	m_bShowHorizonRight = false;
	m_bShowThree		= false;
	m_bShowFour			= false;
	m_bShowFibo			= false;
}

// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
CAnalysisToolData::	CAnalysisToolData(const bool bShowTrendLine,
					const bool bShowVertical/*=true*/,
					const bool bShowHorizonLeft/*=true*/,
					const bool bShowHorizonRight/*=true*/,
					const bool bShowThree/*=true*/,
					const bool bShowFour/*=true*/,
					const bool bShowFibo/*=true*/)
{
	m_bShowTrendLine	= bShowTrendLine;
	m_bShowVertical		= bShowVertical;
	m_bShowHorizonLeft	= bShowHorizonLeft;
	m_bShowHorizonRight = bShowHorizonRight;
	m_bShowThree		= bShowThree;
	m_bShowFour			= bShowFour;
	m_bShowFibo			= bShowFibo;
}
	
CAnalysisToolData::~CAnalysisToolData()
{
}

int CAnalysisToolData::GetShowTrendLine() const
{
	return m_bShowTrendLine;
}

void CAnalysisToolData::SetShowTrendLine(const bool bShowTrendLine)
{
	m_bShowTrendLine = bShowTrendLine;
}

int CAnalysisToolData::GetShowVertical() const
{
	return m_bShowVertical;
}

void CAnalysisToolData::SetShowVertical(const bool bShowVertical)
{
	m_bShowVertical = bShowVertical;
}

// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
int CAnalysisToolData::GetShowHorizonLeft() const
{
	return m_bShowHorizonLeft;
}

// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
void CAnalysisToolData::SetShowHorizonLeft(const bool bShowHorizonLeft)
{
	m_bShowHorizonLeft = bShowHorizonLeft;
}

// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
int CAnalysisToolData::GetShowHorizonRight() const
{
	return m_bShowHorizonRight;
}

// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
void CAnalysisToolData::SetShowHorizonRight(const bool bShowHorizonRight)
{
	m_bShowHorizonRight = bShowHorizonRight;
}

int CAnalysisToolData::GetShowThree() const
{
	return m_bShowThree;
}

void CAnalysisToolData::SetShowThree(const bool bShowThree)
{
	m_bShowThree = bShowThree;
}

int CAnalysisToolData::GetShowFour() const
{
	return m_bShowFour;
}

void CAnalysisToolData::SetShowFour(const bool bShowFour)
{
	m_bShowFour = bShowFour;
}

int CAnalysisToolData::GetShowFibo() const
{
	return m_bShowFibo;
}

void CAnalysisToolData::SetShowFibo(const bool bShowFibo)
{
	m_bShowFibo = bShowFibo;
}

void CAnalysisToolData::SetSavedData(const CAnalysisToolData& analysistooldata)
{
	m_bShowTrendLine	= analysistooldata.GetShowTrendLine();
	m_bShowVertical		= analysistooldata.GetShowVertical();
	// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
	m_bShowHorizonLeft	= analysistooldata.GetShowHorizonLeft();
	m_bShowHorizonRight	= analysistooldata.GetShowHorizonRight();

	m_bShowThree		= analysistooldata.GetShowThree();
	m_bShowFour			= analysistooldata.GetShowFour();
	m_bShowFibo			= analysistooldata.GetShowFibo();
}


///////////////////////////////////////////////////////////////////////////////
// class CMATitleListData

const CList<CString, CString>& CMATitleListData::GetPriceMATitleList() const
{
	return m_priceMATitleList;
}

const CList<CString, CString>& CMATitleListData::GetTradeMATitleList() const
{
	return m_tradeMATitleList;
}

CList<CString, CString>& CMATitleListData::GetPriceMATitleList()
{
	return m_priceMATitleList;
}

CList<CString, CString>& CMATitleListData::GetTradeMATitleList()
{
	return m_tradeMATitleList;
}

CList<CString, CString>* CMATitleListData::GetPriceMATitleList_Pointer()
{
	return &m_priceMATitleList;
}

CList<CString, CString>* CMATitleListData::GetTradeMATitleList_Pointer()
{
	return &m_tradeMATitleList;
}

void CMATitleListData::SetMATitleListData(const CMATitleListData& sourceMATitleListData)
{
	SetTitleListData(sourceMATitleListData.GetPriceMATitleList(), m_priceMATitleList);
	SetTitleListData(sourceMATitleListData.GetTradeMATitleList(), m_tradeMATitleList);
}

void CMATitleListData::AddTail_PriceMA(const CString& strTitle)
{
	m_priceMATitleList.AddTail(strTitle);
}

void CMATitleListData::AddTail_TradeMA(const CString& strTitle)
{
	m_tradeMATitleList.AddTail(strTitle);
}

void CMATitleListData::RemoveAll()
{
	RemovePriceMA();
	RemoveTradeMA();
}

void CMATitleListData::RemovePriceMA()
{
	m_priceMATitleList.RemoveAll();
}

void CMATitleListData::RemoveTradeMA()
{
	m_tradeMATitleList.RemoveAll();
}

void CMATitleListData::SetTitleListData(const CList<CString, CString>& sourceData, CList<CString, CString>& targetData)
{
	targetData.RemoveAll();
	POSITION pos = sourceData.GetHeadPosition();
	while(pos != NULL)
	{
		CString strTitle = sourceData.GetNext(pos);
		targetData.AddTail(strTitle);
	}
}
