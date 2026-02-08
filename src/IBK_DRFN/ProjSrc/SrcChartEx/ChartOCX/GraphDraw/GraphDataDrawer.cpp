// GraphDataDrawer.cpp: implementation of the CGraphDataDrawer class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphDataDrawer.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
#include "../Include_Chart/Dll_Load/PacketList.h"			// for CPacketList
#include "DataMath.h"
#include "Conversion.h"
#include "BaseData.h"
#include "GraphData.h"
#include "DrawingPacketFinder.h"
#include "PacketPointerList.h"

#include "PacketBase.h"
#include "IndicatorInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CPrices

CPrices& CPrices::operator=(const CPrices& prices)
{
	if(this == &prices)
		return *this;

	m_dPrevPrice = prices.GetPrev();
	m_dCurPrice = prices.GetCur();
	m_nDecimalDownCount = prices.GetDecimalDownCount();
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	m_dPipSize = prices.GetPipSize();
	return *this;
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
CPrices::CPrices():
	m_dPrevPrice(0.0),
	m_dCurPrice(0.0),
	m_nDecimalDownCount(0),
	m_dPipSize(0.0)
{
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
CPrices::CPrices(const double& dPrev, const double& dCur, const double& dPipSize):
	m_dPrevPrice(dPrev),
	m_dCurPrice(dCur),
	m_nDecimalDownCount(0),
	m_dPipSize(dPipSize)
{
}

void CPrices::SetPrev(const double& dPrice)
{
	m_dPrevPrice = dPrice;
}

void CPrices::SetCur(const double& dPrice)
{
	m_dCurPrice = dPrice;
}

void CPrices::SetDecimalDownCount(const int nDecimalDownCount)
{
	m_nDecimalDownCount = nDecimalDownCount;
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
void CPrices::SetPipSize(const double& dPipSize)
{
	m_dPipSize = dPipSize;
}

void CPrices::SetDataUnit(const CString& strDataUnit)
{
	m_strDataUnit = strDataUnit;
}

void CPrices::SetPacketType(const CString& strPacketType)
{
	m_strPacketType = strPacketType;
}

double CPrices::GetPrev() const
{
	return m_dPrevPrice;
}

double CPrices::GetCur() const
{
	return m_dCurPrice;
}

double CPrices::GetDifference() const
{
	return (m_dCurPrice - m_dPrevPrice);
}

int CPrices::GetDecimalDownCount() const
{
	return m_nDecimalDownCount;
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
double CPrices::GetPipSize() const
{
	return m_dPipSize;
}

CString CPrices::GetDataUnit() const
{
	return m_strDataUnit;
}

CString CPrices::GetPacketType() const
{
	return m_strPacketType;
}

bool CPrices::IsEmpty() const
{
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	return (m_dCurPrice == 0 && m_dPrevPrice == 0 && m_dPipSize == 0 && m_nDecimalDownCount == 0 && 
		m_strDataUnit.IsEmpty() && m_strPacketType.IsEmpty());
}

bool CPrices::IsRose() const
{
	return (m_dCurPrice > m_dPrevPrice);
}

bool CPrices::IsFall() const
{
	return (m_dCurPrice < m_dPrevPrice);
}


///////////////////////////////////////////////////////////////////////////////
// class CTextData


CTextData& CTextData::operator=(const CTextData& data)
{
	if(this == &data)
		return *this;

	m_strTitle = data.GetTitle();
	m_strData = data.GetData();
	m_color = data.GetColor();
	return *this;
}

CTextData::CTextData() :
	m_color(RGB(0, 0, 0))
{
}

CTextData::CTextData(const CString& strTitle, const CString& strData, const COLORREF& color) :
	m_strTitle(strTitle),
	m_strData(strData),
	m_color(color)
{
}

void CTextData::Set(const CString& strTitle, const CString& strData, const COLORREF& color)
{
	m_strTitle = strTitle;
	m_strData = strData;
	m_color = color;
}

void CTextData::SetTitle(const CString& strTitle)
{
	m_strTitle = strTitle;
}

void CTextData::SetData(const CString& strData)
{
	m_strData = strData;
}

void CTextData::SetColor(const COLORREF& color)
{
	m_color = color;
}

CString CTextData::GetTitle() const
{
	return m_strTitle;
}

CString CTextData::GetData() const
{
	return m_strData;
}

COLORREF CTextData::GetColor() const
{
	return m_color;
}

bool CTextData::IsEmpty() const
{
	return (m_strTitle.IsEmpty() || m_strData.IsEmpty());
}


///////////////////////////////////////////////////////////////////////////////
// class CRateDrawer

// text 쓰기
void CRateDrawer::DrawTextOut(CDC* pDC, const CPoint& point, const CString& strData, int& nPosition_Y)
{
	CSize size = pDC->GetTextExtent(strData);
	pDC->TextOut(point.x - size.cx, point.y + nPosition_Y, strData);

	nPosition_Y += size.cy;
}

void CRateDrawer::DrawTextOut(CDC* pDC, const COLORREF& textColor, const CPoint& point, const CString& strData, int& nPosition_Y)
{
	COLORREF orgTextColor = pDC->SetTextColor(textColor);

	DrawTextOut(pDC, point, strData, nPosition_Y);

	pDC->SetTextColor(orgTextColor);
}

// 사각형 그리기
void CRateDrawer::DrawRegion(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor)
{
	CBrush newBrush(brushColor);
	CBrush* pOldBrush = pDC->SelectObject(&newBrush);
	CPen newPen(PS_INSIDEFRAME, 1, penColor);
	CPen* pOldPen = pDC->SelectObject(&newPen);

	pDC->Rectangle(region);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	newBrush.DeleteObject();
	newPen.DeleteObject();
}

// 비율 -> dBaseData 대한 dData 의 비
CString CRateDrawer::GetRateText(const double& dBaseData, const double& dData) const
{
	double dDataRate = 0.0;
	if(dBaseData != 0.0)
		dDataRate = dData / dBaseData * 100.0;

	return CDataConversion::DoubleToString(dDataRate, 2) + "%";
}

// ----------------------------------------------------------------------------
// "현재가" 를 화면상의 데이타!!
// 현재 보이고 있는 data들 중 가장 오른쪽 data -> 현재가
// 현재가의 해당 날짜 이전의 종가 -> 전일가(전일종가)
// 현재 종가의 화면에 보여주는 data type -> data type의 소숫점 좌표
bool CRateDrawer::GetPrices(CPacket* pPacket, const int nDataEnd, CPrices& prices) const
{
	if(pPacket == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	if( nDataEnd < 0) return false;
	if( pDataList->GetCount() <= nDataEnd) return false;

	POSITION pos = pDataList->FindIndex(nDataEnd);
	if(pos == NULL)
		return false;

	prices.SetCur(pDataList->GetPrev(pos));
	if(pos == NULL)
		return false;

	prices.SetPrev(pDataList->GetPrev(pos));

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	CRateDrawer::GetDecimalDownCountAndDataType(pPacket, prices);
	prices.SetPacketType(pPacket->GetType());

	prices.SetPipSize(CDataConversion::GetDataType(prices.GetPacketType()));
	if( prices.GetPipSize() <= 0) prices.SetPipSize(0.00001);

	return true;
}

bool CRateDrawer::GetDecimalDownCountAndDataType(CPacket* pPacket, CPrices& prices) const
{
	CString strType;
	int nDecimalDownCount = 0;
	if(!GetDecimalDownCount(pPacket, nDecimalDownCount, strType))
		return false;

	prices.SetDecimalDownCount(nDecimalDownCount);
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	prices.SetDataUnit(strType);
	return true;
}

// ----------------------------------------------------------------------------
int CRateDrawer::GetRealDataEnd(CPacket* pPacket, const int nDataEnd) const
{
	if(pPacket == NULL || pPacket->GetnumericDataList() == NULL)
		return nDataEnd;

	int nDataCount = pPacket->GetnumericDataList()->GetCount();
	if(nDataEnd >= nDataCount)
		return (nDataCount - 1);

	return nDataEnd;
}


// private ====================================================================
bool CRateDrawer::GetDecimalDownCount(CPacket* pPacket, int& nDecimalDownCount, CString& strType) const
{
	if(pPacket == NULL)
		return false;

	CString strDataType = pPacket->GetType(false);
	if(strDataType.Find(".") < 0)
		return false;

	// 20080910 JS.Kim		8진법/32진법 처리, 소숫점 이하 자리수 최대자리로 Return <<
	if( CDataConversion::IsBaseFormat(strDataType) )
	{
		nDecimalDownCount = 0;
		strType = "";//strDataType;
		int nBase, nDecLen1, nDecLen2;
		if( CDataConversion::GetBaseFormatInfo(strDataType, nBase, nDecLen1, nDecLen2) )
			nDecimalDownCount = nDecLen1 + nDecLen2;
		return true;
	}
	//>>

	CDataConversion::GetStringData(strDataType, ".");
	CString strRight = strDataType.Right(1);
	int nRight = atoi(strRight);
	if(nRight == 0 && strRight != "0")
		strType = strRight;

	strDataType.TrimRight(strType);
	strDataType.TrimRight("0");
	nDecimalDownCount = strDataType.GetLength();
	return true;
}

/*bool CRateDrawer::GetDecimalDownCount(CPacket* pPacket, int& nDecimalDownCount, CString& strType) const
{
	if(pPacket == NULL)
		return false;

	CString strDataType = pPacket->GetType(false);
	double dDataType = pPacket->GetTypeFormatDouble(false);
	// data type 이 소숫점일때만 구하기.
	if(dDataType >= 1)
		return false;

	strDataType = CDataConversion::DoubleToString(dDataType);
	if(strDataType.IsEmpty())
		return false;

	CDataConversion::GetStringData(strDataType, ".");
	strDataType.TrimRight("0");
	nDecimalDownCount = strDataType.GetLength();
	return true;
}*/


///////////////////////////////////////////////////////////////////////////////
// class CDataInGraphRegionDrawer

void CDataInGraphRegionDrawer::DrawData(CDC* pDC, const CRect& region, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData, CPrices& prices)
{
	// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
	m_hOcxWnd = drawingGraphData.GetOcxHwnd();

	CDisplayDataMinMax curMinMax;
	//2007.06.11 by LYH 복수종목때문에 지표명으로 넘김
	//if(!GetDrawingDatas(drawingGraphData.GetGraphName(), subGraphData.GetStyle(), 
	if(!GetDrawingDatas(drawingGraphData.GetIndicatorName(), subGraphData.GetStyle(), 
		drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList(), displayAttributes, curMinMax, prices))
		return;

	// (2008/11/3 - Seung-Won, Bae) Do not support Graph Data for Buy/Sell Item.
	CString strItemName( subGraphData.GetItemName());
	int nItemLen = strItemName.GetLength();
	if( 3 < nItemLen) if( strItemName.GetAt( nItemLen - 1 - 2) == '(') return;

	if( IsGraphDataDraw(region, displayAttributes))
		DrawData(pDC, region, drawingGraphData, curMinMax, prices, subGraphData.GetColor());
}


// private ====================================================================
void CDataInGraphRegionDrawer::DrawData(CDC* pDC, const CRect& region, const CDrawingGraphData& drawingGraphData, 
		const CDisplayDataMinMax& curMinMax, const CPrices& prices, const CGraphColor& graphColor)
{
	CList<CTextData, CTextData&> textDataList;
	if(!GetDataTexts(drawingGraphData.GetGraphDataDrawingData(), curMinMax, prices, graphColor, textDataList))
		return;

	int nOrgMode = pDC->SetBkMode(TRANSPARENT);

	DrawData(pDC, region, drawingGraphData, prices, textDataList);

	pDC->SetBkMode(nOrgMode);
}

void CDataInGraphRegionDrawer::DrawData(CDC* pDC, const CRect& region, const CDrawingGraphData& drawingGraphData, 
		const CPrices& prices, const CList<CTextData, CTextData&>& textDataList)
{
	int nPosition_Y = 0;
	if(textDataList.GetCount() > 1)
	{
		nPosition_Y = GetPosition_Y(pDC, textDataList.GetCount(), region.Height(), 
			drawingGraphData.DoesVertScaleInvertType(), drawingGraphData.GetDisplayDataVertMinMax(), prices.GetCur());
	}

	POSITION pos = textDataList.GetHeadPosition();
	while(pos != NULL)
	{
		const CTextData& textData = textDataList.GetNext(pos);
		CRateDrawer::DrawTextOut(pDC, textData.GetColor(), 
			CPoint(region.right, region.top + 1), GetDrawingText(textData), nPosition_Y);
	}
}

// ----------------------------------------------------------------------------
bool CDataInGraphRegionDrawer::IsGraphDataDraw(const CRect& region, const CDisplayAttributes& displayAttributes) const
{
	return (region.Height() > 30 && region.Width() > 100 && displayAttributes.GetDataCountInBlock() >= 4);
}

// ----------------------------------------------------------------------------
// 그리기 위해 필요한 data 들을 가져오기. -> 화면상의 데이타!!
bool CDataInGraphRegionDrawer::GetDrawingDatas(const CString& strIndicatorName, const CGraphStyle& graphStyle, const CList<CPacket*, CPacket*>& packetList, 
		const CDisplayAttributes& displayAttributes, CDisplayDataMinMax& curMinMax, CPrices& prices) const
{
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pPacket = packetList.GetHead();
	if(pPacket == NULL)
		return false;

	CDisplayAttributes realDisplayAttributes = displayAttributes;
	realDisplayAttributes.SetDataEndInBlock(CRateDrawer::GetRealDataEnd(pPacket, displayAttributes.GetDataEndInBlock()));
	
	if(!CRateDrawer::GetPrices(CDrawingPacketFinder().GetPracticalPacket(strIndicatorName, packetList), 
		realDisplayAttributes.GetDataEndInBlock(), prices))
		return false;

	return GetHighLow(strIndicatorName, graphStyle, packetList, realDisplayAttributes, curMinMax);
}

// "최대값/최저값" 을 화면상의 데이타!!
bool CDataInGraphRegionDrawer::GetHighLow(const CString& strIndicatorName, const CGraphStyle& graphStyle, 
		const CList<CPacket*, CPacket*>& packetList, const CDisplayAttributes& displayAttributes, CDisplayDataMinMax& curMinMax) const
{
	CDrawingPacketFinder drawingPacketFinder;

	// "가격차트" -> !Line_Draw : max = "고가" min = "저가"
	//            ->  Line_Draw : max = min = "종가"
	if(strIndicatorName == _MTEXT( C2_PRICE_CHART) 
		&& graphStyle.GetGraphBongStyle() != CGraphBaseData::Line_Draw)
	{
		CPacket* pPacket_High = drawingPacketFinder.GetPacket(packetList, _MTEXT( C0_HIGH));
		CPacket* pPacket_Low = drawingPacketFinder.GetPacket(packetList, _MTEXT( C0_LOW));
		if(pPacket_High == NULL || pPacket_Low == NULL)
			return false;

		double dTemp = 0.0;
		double dMin = curMinMax.GetDisplayMin();
		double dMax = curMinMax.GetDisplayMax();
		if( pPacket_High->GetMinMax(	displayAttributes.GetDataStartInBlock(), displayAttributes.GetDataEndInBlock(), dTemp,	dMax, TRUE)	< 0) return false;
		if( pPacket_Low->GetMinMax(		displayAttributes.GetDataStartInBlock(), displayAttributes.GetDataEndInBlock(), dMin,	dTemp, TRUE)	< 0) return false;
		curMinMax.SetDisplayData( dMin, dMax);

		return true;
	}
	
	CPacket* pPacket = drawingPacketFinder.GetPracticalPacket(strIndicatorName, packetList);
	if(pPacket == NULL)
		return false;

	double dMin = curMinMax.GetDisplayMin();
	double dMax = curMinMax.GetDisplayMax();
	bool bResult = ((pPacket->GetMinMax(displayAttributes.GetDataStartInBlock(), displayAttributes.GetDataEndInBlock(), dMax, dMin) >= 0) ? true: false);
	curMinMax.SetDisplayData( dMin, dMax);

	return bResult;
}

// ----------------------------------------------------------------------------
bool CDataInGraphRegionDrawer::GetDataTexts(const CGraphDataDrawingData& dataDrawing, const CDisplayDataMinMax& curMinMax, 
		const CPrices& prices, const CGraphColor& graphColor, CList<CTextData, CTextData&>& textDataList) const
{
	int nMaxLength_Title(0), nMaxLength_Data(0);
	if(!GetDataTexts(dataDrawing, curMinMax, prices, graphColor, nMaxLength_Title, nMaxLength_Data, textDataList))
		return false;

	ChangeData(nMaxLength_Title, nMaxLength_Data, textDataList);
	return (textDataList.GetCount() > 0);
}

bool CDataInGraphRegionDrawer::GetDataTexts(const CGraphDataDrawingData& dataDrawing, const CDisplayDataMinMax& curMinMax, 
		const CPrices& prices, const CGraphColor& graphColor, int& nMaxLength_Title, int& nMaxLength_Data, CList<CTextData, CTextData&>& textDataList) const
{
	textDataList.RemoveAll();
	nMaxLength_Title = nMaxLength_Data = 0;

	// (2007/1/7 - Seung-Won, Bae) Change Color for samsung.
	// 최저가 대비 최고가 등락률 (보합색)
	if(dataDrawing.DoesCalcRiseFallRate_LowHigh())
	{
		AddTailAndGetMaxLength("LH", GetRiseFallRateText_LowHigh(curMinMax), graphColor.GetColor5(), 
			nMaxLength_Title, nMaxLength_Data, textDataList);
	}
	// 최저가 대비 현재가 등락률 (하락색)
	if(dataDrawing.DoesCalcRiseFallRate_LowCur())
	{
		AddTailAndGetMaxLength("LC", GetRiseFallRateText_LowCur(curMinMax.GetDisplayMin(), prices.GetCur()), graphColor.GetColor1(), 
			nMaxLength_Title, nMaxLength_Data, textDataList);
	}
	// 최고가 대비 현재가 등락률 (상승색)
	if(dataDrawing.DoesCalcRiseFallRate_HighCur())
	{
		AddTailAndGetMaxLength("HC", GetRiseFallRateText_HighCur(curMinMax.GetDisplayMax(), prices.GetCur()), graphColor.GetColor3(), 
			nMaxLength_Title, nMaxLength_Data, textDataList);
	}
	// 전일비 (보합색)
	if(dataDrawing.DoesCalcRate_PrevCur())
	{
		AddTailAndGetMaxLength( _LTEXT6( C6_PREVIOUS_DAY_COMP), GetRateText_PrevCur(prices), graphColor.GetColor5(), 
			nMaxLength_Title, nMaxLength_Data, textDataList);
	}

	return (textDataList.GetCount() > 0);
}

// ----------------------------------------------------------------------------
// 그릴 시작 Y좌표를 구하기
int CDataInGraphRegionDrawer::GetPosition_Y(CDC* pDC, const int nDataCount, const int nRegionHeight, const bool bIsInvert, const CDisplayDataMinMax& curMinMax, const double& dCur) const
{
	// 현재값이 최대값에 가까우면 -> 0
	if(curMinMax.IsCloseToMaxValue(dCur, bIsInvert))
		return 0;

	int nTextHeight = pDC->GetTextExtent("0").cy * nDataCount;
	if(nTextHeight <= 0 || nRegionHeight <= nTextHeight)
		return 0;

	return (nRegionHeight - nTextHeight);
}

CString CDataInGraphRegionDrawer::GetDrawingText(const CTextData& textData) const
{
	if(textData.IsEmpty())
		return "";

	CString strDrawingText;
	strDrawingText.Format("%s : %s", textData.GetTitle(), textData.GetData());
	return strDrawingText;
}

// ----------------------------------------------------------------------------
bool CDataInGraphRegionDrawer::AddTailAndGetMaxLength(const CString& strTitle, const CString& strData, const COLORREF& color, 
		int& nMaxLength_Title, int& nMaxLength_Data, CList<CTextData, CTextData&>& textDataList) const
{
	if(strTitle.IsEmpty() || strData.IsEmpty())
		return false;

	ChangeMaxLength(strTitle, nMaxLength_Title);
	ChangeMaxLength(strData, nMaxLength_Data);

	textDataList.AddTail(CTextData(strTitle, strData, color));
	return true;
}

void CDataInGraphRegionDrawer::ChangeData(const int nMaxLength_Title, const int nMaxLength_Data, CList<CTextData, CTextData&>& textDataList) const
{
	if((nMaxLength_Title <= 0 && nMaxLength_Data <= 0) || textDataList.GetCount() <= 0)
		return;

	POSITION pos = textDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CTextData& textData = textDataList.GetNext(pos);
		CString strNewData;
		if(GetChangingData(textData.GetTitle(), nMaxLength_Title, true, strNewData))
			textData.SetTitle(strNewData);
		if(GetChangingData(textData.GetData(), nMaxLength_Data, false, strNewData))
			textData.SetData(strNewData);
	}
}

void CDataInGraphRegionDrawer::ChangeMaxLength(const CString& strData, int& nMaxLength) const
{
	if(strData.IsEmpty())
		return;

	int nLength = strData.GetLength();
	if(nMaxLength < nLength)
		nMaxLength = nLength;
}

bool CDataInGraphRegionDrawer::GetChangingData(const CString& strData, const int nMaxLength, const bool bIsTail, CString& strChangeData) const
{
	strChangeData = strData;
	for(int nIndex = (strData.GetLength()); nIndex < nMaxLength; nIndex++){
		if(bIsTail)
			strChangeData = strChangeData + ' ';
		else
			strChangeData = ' ' + strChangeData;
	}

	return (strData != strChangeData);
}

// ----------------------------------------------------------------------------
// 최저가 대비 최고가 등락률
CString CDataInGraphRegionDrawer::GetRiseFallRateText_LowHigh(const CDisplayDataMinMax& curMinMax) const
{
	return CRateDrawer::GetRateText(curMinMax.GetDisplayMin(), curMinMax.GetCount());
}

// 최저가 대비 현재가 등락률
CString CDataInGraphRegionDrawer::GetRiseFallRateText_LowCur(const double& dLow, const double& dCur) const
{
	return CRateDrawer::GetRateText(dLow, (dCur - dLow));
}

// 최고가 대비 현재가 등락률
CString CDataInGraphRegionDrawer::GetRiseFallRateText_HighCur(const double& dHigh, const double& dCur) const
{
	return CRateDrawer::GetRateText(dHigh, (dCur - dHigh));
}

// 전일비
CString CDataInGraphRegionDrawer::GetRateText_PrevCur(const CPrices& prices) const
{
	return CRateDrawer::GetRateText(prices.GetPrev(), prices.GetCur());
}


///////////////////////////////////////////////////////////////////////////////
// class CDataInRightScaleRegionDrawer

void CDataInRightScaleRegionDrawer::DrawData(CDC* pDC, const CRect& region, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CGraphColor& graphColor, const CPrices& pricesInView)
{
	CPrices prices;
	CString strPacketType;
	if(!GetDrawingDatas(displayAttributes, drawingGraphData, pricesInView, prices, strPacketType))
		return;

	DrawData(pDC, region, drawingGraphData, graphColor, prices, strPacketType);
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
bool CDataInRightScaleRegionDrawer::GetText(const CTextInRightScaleRegionDrawer& textData, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const
{
// 	CPacket* pPacket = NULL;
// 	int nRealDataEnd = textData.GetDataEnd();
// 	if(!GetPacketAndDataEnd(textData, pPacket, nRealDataEnd))
// 		return false;
// 
// 	CPrices pricesInView;
// 	if(!CRateDrawer::GetPrices(pPacket, nRealDataEnd, pricesInView))
// 		return false;
// 
// 	CPrices prices;
// 	if(!GetDrawingDatas(pPacket, nRealDataEnd, textData.GetCurDataType(), pricesInView, prices))
// 		return false;
// 
// 	CString strPacketType;
// 	if( strPacketType) strPacketType = pPacket->GetType();
// 	return GetText(textData.GetRegion(), prices, textData.GetCurDataType(), strCurPrice, strRiseFallRate, strPacketType);

	// Source Data for calculation.
	CPacket *pPacket = NULL;
	if( 0 < textData.GetSubGraphPacketList()->GetSubGraphPacketList().GetCount())
		pPacket = CDrawingPacketFinder().GetPracticalPacket( textData.GetGraphName(), textData.GetSubGraphPacketList()->GetSubGraphPacketList());
	if( !pPacket) return false;
	int nDTI = CRateDrawer::GetRealDataEnd( pPacket, textData.GetDataEnd());

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if( pDataList == NULL || pDataList->GetCount() <= 0) return false;

	POSITION pos = NULL;
	int nCurDataType = textData.GetCurDataType();
	CPrices prices;
	if( 1 == nCurDataType % 2) 
	{
		// -> 마지막 데이타(전일종가대비)!!
		pos = pDataList->GetTailPosition();
		prices.SetCur(pDataList->GetPrev( pos));
		if( pPacket->IsUseStandardValue())	prices.SetPrev(pPacket->GetStandardValue());
		else if( pos)						prices.SetPrev(pDataList->GetPrev( pos));
		else								return false;
	}
	else
	{
		// -> 화면상의 데이타!!
		pos = pDataList->FindIndex( nDTI);
		prices.SetCur(pDataList->GetPrev( pos));
		if( pos)							prices.SetPrev(pDataList->GetPrev( pos));
		else								return false;
	}

	CRateDrawer::GetDecimalDownCountAndDataType( pPacket, prices);
	prices.SetPacketType(pPacket->GetType());

	prices.SetPipSize(CDataConversion::GetDataType(prices.GetPacketType()));
	if( prices.GetPipSize() <= 0) prices.SetPipSize(0.00001);


	// (2010/8/4 - Seung-Won, Bae) for '%' Scale.
	if( 0 < prices.GetPacketType().Find( '%'))
	{
		if( 0 == nCurDataType % 2)	nCurDataType = 0;
		else						nCurDataType = 3;
	}

	return GetText(textData.GetRegion(), prices, nCurDataType, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate);
}


// private ====================================================================
void CDataInRightScaleRegionDrawer::DrawData(CDC* pDC, const CRect& region, 
		const CDrawingGraphData& drawingGraphData, const CGraphColor& graphColor, const CPrices& prices, CString &p_strPacketType)
{
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	if(drawingGraphData.GetVertScalePosition() == CScaleBaseData::VERTSCALEPOSITION::VERT_BOTH)
	{
		DrawData(pDC, region, drawingGraphData, graphColor, prices, p_strPacketType, drawingGraphData.GetLeftScaleTextRegion());
		DrawData(pDC, region, drawingGraphData, graphColor, prices, p_strPacketType, drawingGraphData.GetRightScaleTextRegion());
	}
	else
	{
		DrawData(pDC, region, drawingGraphData, graphColor, prices, p_strPacketType, drawingGraphData.GetScaleTextRegion());
	}
}

// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
void CDataInRightScaleRegionDrawer::DrawData(CDC* pDC, const CRect& region, 
		const CDrawingGraphData& drawingGraphData, const CGraphColor& graphColor, const CPrices& prices, CString &p_strPacketType,
		CRect& rcScaleText)
{
	// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	CString strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate;

	int nCurDataType = drawingGraphData.GetGraphDataDrawingData().GetCalcRiseFallRate_CurData();
	if(nCurDataType <= 0)
		return;

	if(!GetText(rcScaleText, prices, nCurDataType, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate))
		return;

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	CDataConversion::SelectClipRegion(pDC, GetGripRegion(drawingGraphData, rcScaleText));

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	CRect textRegion = GetTextRegion(pDC, drawingGraphData, prices, strCurPrice, strDiffPrice, strPipPrice, strRiseFallRate, rcScaleText);
	COLORREF dataColor1 = GetCurrentPriceTextColor(graphColor, prices);
	COLORREF dataColor2 = GetTextColor(dataColor1);
	int nPrevGap_Y = 0;

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	DrawData(pDC, textRegion, strCurPrice, dataColor1, dataColor2, nPrevGap_Y);
// 	// (2007/2/22 - Seung-Won, Bae) Support Ratio Skip.
// 	if( drawingGraphData.GetGraphDataDrawingData().GetCalcRiseFallRate_CurData() < 3)
// 		DrawData(pDC, textRegion, strRiseFallRate, dataColor2, dataColor1, nPrevGap_Y);

	// 이상신 : 현재가 표시가 Y Scale 하단에 가릴때 하단을 Bottom 으로 설정하여 가리지 않게 함. ---------
	// 0012648: [2200] Y축 현재가 박스 일부가 잘리는 현상
	CRect rcTemp = GetDrawingTextRegion(textRegion, nPrevGap_Y);
	int nCheckBottom = textRegion.bottom + rcTemp.Height();
	if ( 5 == nCurDataType || 6 == nCurDataType)
		nCheckBottom += rcTemp.Height();
	if ( 1 == nCurDataType || 2 == nCurDataType || 5 == nCurDataType || 6 == nCurDataType)
		if ( strRiseFallRate.IsEmpty())
			nCheckBottom -= rcTemp.Height();

	if (rcScaleText.bottom < nCheckBottom)
	{
		int nGap = nCheckBottom - rcScaleText.bottom;
		textRegion -= CPoint(0, nGap);
	}
	// -------------------------------------------------------------------------------------------------

	// nCurDataType : 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	DrawData( pDC, textRegion, strCurPrice, dataColor1, dataColor2, nPrevGap_Y);
	// nCurDataType : 5, 6, 9, 10
	if( 5 == nCurDataType || 6 == nCurDataType || 9 == nCurDataType || 10 == nCurDataType)
		DrawData( pDC, textRegion, strDiffPrice, dataColor1, dataColor2, nPrevGap_Y);
	// nCurDataType : 7, 8
	if( 7 == nCurDataType || 8 == nCurDataType)
		DrawData( pDC, textRegion, strPipPrice, dataColor1, dataColor2, nPrevGap_Y);
	// nCurDataType : 1, 2, 5, 6
	if( 1 == nCurDataType || 2 == nCurDataType || 5 == nCurDataType || 6 == nCurDataType)
		if( !strRiseFallRate.IsEmpty())
			DrawData(pDC, textRegion, strRiseFallRate, dataColor1, dataColor2, nPrevGap_Y);

	CDataConversion::SelectClipRegion(pDC);
}

void CDataInRightScaleRegionDrawer::DrawData(CDC* pDC, const CRect& textRegion, const CString& strText, 
		const COLORREF& bkColor, const COLORREF& textColor, int& nPrevGap_Y)
{
	int nOrgMode = pDC->SetBkMode(TRANSPARENT);

	CRect drawingTextRegion = GetDrawingTextRegion(textRegion, nPrevGap_Y);
	CRateDrawer::DrawRegion(pDC, drawingTextRegion, bkColor, bkColor);
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	int nTemp = nPrevGap_Y;
	CRateDrawer::DrawTextOut(pDC, textColor, CPoint(textRegion.right, textRegion.top), strText, nTemp);
	nPrevGap_Y += drawingTextRegion.Height();

	pDC->SetBkMode(nOrgMode);
}

// ----------------------------------------------------------------------------
// 그리기 위해 필요한 data 들을 가져오기.
bool CDataInRightScaleRegionDrawer::GetDrawingDatas(const CDisplayAttributes& displayAttributes,
		const CDrawingGraphData& drawingGraphData, const CPrices& pricesInView, CPrices& prices, CString &p_strPacketType) const
{
	int nCurDataType = drawingGraphData.GetGraphDataDrawingData().GetCalcRiseFallRate_CurData();
	if(nCurDataType <= 0)
		return false;
	
	CPacket* pPacket = GetPacket(drawingGraphData);
	int nDataEnd = CRateDrawer::GetRealDataEnd(pPacket, displayAttributes.GetDataEndInBlock());

	if( pPacket) p_strPacketType = pPacket->GetType();
	return GetDrawingDatas(pPacket, nDataEnd, nCurDataType, pricesInView, prices);
}

bool CDataInRightScaleRegionDrawer::GetDrawingDatas(CPacket* pPacket, const int nDataEnd, const int nCurDataType, 
		const CPrices& pricesInView, CPrices& prices) const
{
	if(nCurDataType <= 0)
		return false;

	// -> 마지막 데이타(전일종가대비)!!
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
// 	if( nCurDataType == 1 || nCurDataType == 3) return GetPricesInFull(pPacket, prices);
	if( 1 == nCurDataType % 2) 
		return GetPricesInFull(pPacket, prices);

	// -> 화면상의 데이타!!
	return GetPricesInView(pPacket, nDataEnd, pricesInView, prices);
}

// "현재가"와 "등락율" -> data의 마지막과 전일종가
// "현재가" : 마지막 데이타!!
// "전일가" : 전일 종가 or 마지막이전 data
bool CDataInRightScaleRegionDrawer::GetPricesInFull(CPacket* pPacket, CPrices& prices) const
{
	if(pPacket == NULL)
		return false;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	POSITION pos = pDataList->GetTailPosition();
	if(pos == NULL)
		return false;

	prices.SetCur(pDataList->GetPrev(pos));

	if(pPacket->IsUseStandardValue())
	{
		prices.SetPrev(pPacket->GetStandardValue());
	}
	else
	{
		if(pos != NULL)
			prices.SetPrev(pDataList->GetPrev(pos));
	}
	
	CRateDrawer::GetDecimalDownCountAndDataType(pPacket, prices);
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	prices.SetPacketType(pPacket->GetType());

	prices.SetPipSize(CDataConversion::GetDataType(prices.GetPacketType()));
	if( prices.GetPipSize() <= 0) prices.SetPipSize(0.00001);
	return true;
}

// "현재가"와 "등락율" -> 화면에 보이는 data의 마지막과 그전값
// "현재가" : 화면에 보이는 마지막 data!!
// "전일가" : 화면에 보이는 마지막이전 data
bool CDataInRightScaleRegionDrawer::GetPricesInView(CPacket* pPacket, const int nDataEnd, const CPrices& pricesInView, CPrices& prices) const
{
	prices = pricesInView;
	if(!pricesInView.IsEmpty())
		return true;

	if(pPacket == NULL)
		return false;

	return CRateDrawer::GetPrices(pPacket, nDataEnd, prices);
}

CPacket* CDataInRightScaleRegionDrawer::GetPacket(const CDrawingGraphData& drawingGraphData) const
{
	const CList< CPacket*, CPacket*> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if( packetList.GetCount() <= 0)
		return NULL;

	//2007.06.11 by LYH 복수종목때문에 지표명으로 넘김
//	return CDrawingPacketFinder().GetPracticalPacket( drawingGraphData.GetGraphName(), packetList);
	return CDrawingPacketFinder().GetPracticalPacket( drawingGraphData.GetIndicatorName() , packetList);
}

// ----------------------------------------------------------------------------
// 그릴 시작 Y좌표를 구하기
// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
int CDataInRightScaleRegionDrawer::GetPosition_Y(const CSize& oneTextSize, const CDrawingGraphData& drawingGraphData, const double& dCur, const CRect& rcScaleText) const
{
	double dRealMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dRealMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax();
	double dRealCur = dCur;

	// 뒤집기
	if(drawingGraphData.DoesVertScaleInvertType()){
		dRealMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax();
		dRealMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	}

	// log
	if(drawingGraphData.DoesVertScaleLogType()){
		dRealMin = CMath::Log(dRealMin);
		dRealMax = CMath::Log(dRealMax);
		dRealCur = CMath::Log(dRealCur);
	}

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	int nPosition_Y = CDataConversion::RelativeToAbsolute_Y(rcScaleText, dRealCur, dRealMin, dRealMax);

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	nPosition_Y -= (rcScaleText.top + oneTextSize.cy/2);

	return nPosition_Y;
}

// grip이 될 영영
// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
CRect CDataInRightScaleRegionDrawer::GetGripRegion(const CDrawingGraphData& drawingGraphData, const CRect& rcScaleText) const
{
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	CRect gripRegion = rcScaleText;
	CRect noFrameRegion = drawingGraphData.GetGraphRegions().GetNoFrameRegion();
	gripRegion.top = noFrameRegion.top;
	gripRegion.bottom = noFrameRegion.bottom;
	gripRegion.right += 1;

	return gripRegion;
}

// text가 쓰여질 영역
// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
CRect CDataInRightScaleRegionDrawer::GetTextRegion(CDC* pDC, const CDrawingGraphData& drawingGraphData, const CPrices& prices, const CString& strCurPrice, const CString& strDiffPrice, const CString& strPipPrice, const CString& strRiseFallRate, const CRect& rcScaleText) const
{
	CString strMax = strCurPrice;
	if(strCurPrice.GetLength() < strDiffPrice.GetLength())
		strMax = strDiffPrice;
	if(strCurPrice.GetLength() < strPipPrice.GetLength())
		strMax = strPipPrice;
	if(strCurPrice.GetLength() < strRiseFallRate.GetLength())
		strMax = strRiseFallRate;

	CSize strMaxSize = pDC->GetTextExtent(strMax);
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	int nPosition_Y = GetPosition_Y(pDC->GetTextExtent("0"), drawingGraphData, prices.GetCur(), rcScaleText);
	CPoint rightTopPt(rcScaleText.right, (rcScaleText.top + nPosition_Y));

	CRect region(rightTopPt, rightTopPt);
	region.InflateRect((strMaxSize.cx), 0, 0, (strMaxSize.cy));
	region.NormalizeRect();
	return region;
}

// text가 그려질 영역
CRect CDataInRightScaleRegionDrawer::GetDrawingTextRegion(const CRect& textRegion, const int nPrevGap_Y) const
{
	CRect drawingRegion = textRegion;
	drawingRegion.InflateRect(1, 2, 1, 0);
	drawingRegion.NormalizeRect();
	drawingRegion.OffsetRect(0, nPrevGap_Y);
	return drawingRegion;
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
CRect CDataInRightScaleRegionDrawer::GetTextRegion(CDC* pDC, const CPoint& rightTopPt, const CString& strCurPrice, const CString& strDiffPrice, const CString& strPipPrice, const CString& strRiseFallRate) const
{
	CString strMax = strCurPrice;
	if(strCurPrice.GetLength() < strDiffPrice.GetLength())
		strMax = strDiffPrice;
	if(strCurPrice.GetLength() < strPipPrice.GetLength())
		strMax = strPipPrice;
	if(strCurPrice.GetLength() < strRiseFallRate.GetLength())
		strMax = strRiseFallRate;

	int nLineCount = 0;
	if(!strCurPrice.IsEmpty())
		nLineCount++;
	if(!strRiseFallRate.IsEmpty())
		nLineCount++;

	CSize strMaxSize = pDC->GetTextExtent(strMax);
	CRect region(rightTopPt, rightTopPt);
	region.InflateRect((strMaxSize.cx + 2), 2, 2, (strMaxSize.cy*nLineCount +1));
	region.NormalizeRect();

	return region;
}

// ----------------------------------------------------------------------------
CPoint CDataInRightScaleRegionDrawer::GetTextPosition(const int nOrg_x, const int nOrg_y, const CSize& textSize, const int textPos) const
{
	if(textPos == DT_BOTTOM)
		return CPoint((nOrg_x - textSize.cx), (nOrg_y - textSize.cy));

	return CPoint((nOrg_x - textSize.cx), (nOrg_y - textSize.cy/2));
}

COLORREF CDataInRightScaleRegionDrawer::GetCurrentPriceTextColor(const CGraphColor& graphColor, const CPrices& prices) const
{
	// 상승
	if(prices.IsRose())
		return graphColor.GetColor1();
	// 하락
	else if(prices.IsFall())
		return graphColor.GetColor3();
	// 보합
	return graphColor.GetColor5();
}

//sy 2004.07.08.
//-> 바탕의 색에 따라 text 색을 흰색/검정으로 한다.
COLORREF CDataInRightScaleRegionDrawer::GetTextColor(const COLORREF& bkColor) const
{
	int nRed = GetRValue(bkColor);
	int nRGreen = GetGValue(bkColor);
	int nBlue = GetBValue(bkColor);
	if(nRed > 200 && nRGreen > 200 && nBlue > 200)
		return RGB(0, 0, 0);

	return RGB(255, 255, 255);
}

// ----------------------------------------------------------------------------
// "현재가", "전일가 대비 현재가 등락률" 의 string 가져오기// "현재가", "전일가 대비 현재가 등락률" 의 string 가져오기
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
bool CDataInRightScaleRegionDrawer::GetText(const CRect& region, const CPrices& prices, int nCurDataType, CString& strCurPrice, CString& strDiffPrice, CString& strPipPrice, CString& strRiseFallRate) const
{
	if(!IsGraphDataDraw(region))
		return false;

// 	// 현재가
// 	strCurPrice = GetText_CurrentPrice(prices, p_strPacketType);
// 
// 	// 전일가 대비 현재가 등락률
// 	if(IsGraphDataDraw_Rate(region, prices))
// 		strRiseFallRate = CRateDrawer::GetRateText(prices.GetPrev(), prices.GetDifference());
// 
// 	return (!strCurPrice.IsEmpty() || !strRiseFallRate.IsEmpty());

	CString strMaxLenValue;

	// 현재가
	// nCurDataType : 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	strCurPrice = GetText_Price( prices.GetCur(), prices.GetPacketType(), prices.GetDataUnit(), prices.GetDecimalDownCount());
	strMaxLenValue = strCurPrice;

	// (2010/5/12 - Seung-Won, Bae) Add Diff Value.
	// nCurDataType : 5, 6, 9, 10
	if( 5 == nCurDataType || 6 == nCurDataType || 9 == nCurDataType || 10 == nCurDataType)
	{
		strDiffPrice = GetText_Price( prices.GetDifference(), prices.GetPacketType(), prices.GetDataUnit(), prices.GetDecimalDownCount());
		if( strMaxLenValue.GetLength() < strDiffPrice.GetLength())
			strMaxLenValue = strDiffPrice;
	}

	// nCurDataType : 7, 8
	if( 0 <= prices.GetPipSize()) if( 7 == nCurDataType || 8 == nCurDataType)
	{
		strPipPrice = GetText_Price( ( prices.GetDifference()) / ( prices.GetPipSize() * 10), "X0.1", "", 1);
		if( strMaxLenValue.GetLength() < strPipPrice.GetLength())
			strMaxLenValue = strPipPrice;
	}

	// 전일가 대비 현재가 등락률
	// nCurDataType : 1, 2, 5, 6
	if( 1 == nCurDataType || 2 == nCurDataType || 5 == nCurDataType || 6 == nCurDataType)
		//이전값이 0 이거나 type 이 "%" 인경우 -> 표현하지 않음.
		if( prices.GetPrev() != 0.0 && prices.GetPacketType() != "%")
		{
			double dPrevPrice = fabs(prices.GetPrev());
			strRiseFallRate = "(" + CRateDrawer::GetRateText( dPrevPrice, prices.GetDifference()) + ")";
			if( strMaxLenValue.GetLength() < strRiseFallRate.GetLength())
				strMaxLenValue = strRiseFallRate;
		}

	return !strMaxLenValue.IsEmpty();
}

// 현재가
CString CDataInRightScaleRegionDrawer::GetText_CurrentPrice( const CPrices& prices, const CString &p_strPacketType) const
{
	// 20080910 JS.Kim		8진법/32진법 처리	<<
	/*
	CString strCurPrice = CDataConversion::DoubleToString(prices.GetCur(), prices.GetDecimalDownCount());

	if( p_strPacketType.Find( '.') < 0)
	{
		CString strPacketType( p_strPacketType);
		int nFactor = strPacketType.Replace( '0', '_');
		strCurPrice = strCurPrice.Left( strCurPrice.GetLength() - nFactor);
	}

	return (CDataConversion::GetStringThousandFormat(strCurPrice) + prices.GetDataType());
	*/
	CString strCurPrice;

	if( CDataConversion::IsBaseFormat(p_strPacketType) )
	{
		strCurPrice = CDataConversion::DoubleToString(prices.GetCur(), p_strPacketType);
	}
	else
	{
		strCurPrice = CDataConversion::DoubleToString(prices.GetCur(), prices.GetDecimalDownCount());

		if( p_strPacketType.Find( '.') < 0)
		{
			CString strPacketType( p_strPacketType);
			int nFactor = strPacketType.Replace( '0', '_');
			strCurPrice = strCurPrice.Left( strCurPrice.GetLength() - nFactor);
		}
	}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	return (CDataConversion::GetStringThousandFormat(strCurPrice) + prices.GetDataUnit());
	//>>
}

// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
CString CDataInRightScaleRegionDrawer::GetText_Price( double dPrice, const CString &p_strPacketType, const CString &p_strUnit, int nDecimalDownCount) const
{
	CString strCurPrice;

	if( CDataConversion::IsBaseFormat( p_strPacketType) )
	{
		strCurPrice = CDataConversion::DoubleToString( dPrice, p_strPacketType);
	}
	else
	{
		strCurPrice = CDataConversion::DoubleToString( dPrice, nDecimalDownCount);

		if( p_strPacketType.Find( '.') < 0)
		{
			CString strPacketType( p_strPacketType);
			int nFactor = strPacketType.Replace( '0', '_');
			strCurPrice = strCurPrice.Left( strCurPrice.GetLength() - nFactor);
		}
	}

	return ( CDataConversion::GetStringThousandFormat( strCurPrice) + p_strUnit);
	//>>
}

// ----------------------------------------------------------------------------
bool CDataInRightScaleRegionDrawer::IsGraphDataDraw(const CRect& region) const
{
	return (region.Width() > 10);
}

bool CDataInRightScaleRegionDrawer::IsGraphDataDraw_Rate(const CRect& region, const CPrices& prices) const
{
	//2007.06.21 by LYH (높이 상관 없이 등락률 계산하도록 수정)
	//if(region.Height() < 20)
	//	return false;

	if(prices.GetPrev() != 0.0)
		return true;

	//이전값이 0 이고 type 이 "%" 인경우 -> 표현하지 않음.
// vntsorl_20110609:[A00000259] 가격눈금 위의 현재가 표시 수정
	return (prices.GetDataUnit() != "%");
}

// ----------------------------------------------------------------------------
bool CDataInRightScaleRegionDrawer::GetPacketAndDataEnd(const CTextInRightScaleRegionDrawer& textData, CPacket*& pPacket, int& nRealDataEnd) const
{
	if(!textData.IsTextDrawing())
		return false;

	pPacket = CDrawingPacketFinder().GetPracticalPacket(textData.GetGraphName(), textData.GetSubGraphPacketList()->GetSubGraphPacketList());
	nRealDataEnd = CRateDrawer::GetRealDataEnd(pPacket, textData.GetDataEnd());
	return true;
}


//////////////////////////////////////////////////////////////////////
// class CGraphDataDrawer

void CGraphDataDrawer::DrawGraphDatas(CDC* pDC, const CRect& realRegion, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData)
{
	// "등락율"
	CPrices pricesInView;
	m_dataInGraphRegionDrawer.DrawData(pDC, realRegion, displayAttributes, drawingGraphData, subGraphData, pricesInView);

	// (2007/2/22 - Seung-Won, Bae) Support Base Line Price in Compare Line
	if( subGraphData.GetType() == CGraphBaseData::Line_Type && subGraphData.GetStyle().GetGraphLineStyle() == CGraphBaseData::ComparisonLine)
	{
		CPacket* pPacket = m_dataInRightScaleRegionDrawer.GetPacket( drawingGraphData);
		if( pPacket) pricesInView.SetPrev( pPacket->GetStandardValue());
	}
	// (2007/2/22 - Seung-Won, Bae) Do not support the Up/Down Info, But only Candle Graph
	else if( subGraphData.GetType() != CGraphBaseData::Bong_Type) pricesInView.SetPrev( pricesInView.GetCur() - 1);

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	if(drawingGraphData.GetTextDrawing() && drawingGraphData.GetDisplayDataVertMinMax().IsInitialized())
	{
		// "전일가 대비 현재가 등락률 (현재가)"
		m_dataInRightScaleRegionDrawer.DrawData(pDC, realRegion, displayAttributes, drawingGraphData, subGraphData.GetColor(), pricesInView);
	}
}
