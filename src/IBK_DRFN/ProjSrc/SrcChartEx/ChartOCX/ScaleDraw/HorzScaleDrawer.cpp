// HorzScaleDrawer.cpp: implementation of the CHorzScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HorzScaleDrawer.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"
#include "ScaleCalculator.h"
#include "VertScaleDrawer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class CHorzScaleDrawer

CHorzScaleDrawer::CHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode) : CDrawer( p_hOcxWnd, p_eChartMode)
{
}

// protected ==========================================================================
bool CHorzScaleDrawer::IsKoreanStyle(const CString& packetType) const
{
	return (packetType.Find('Y') <= 0);
}

bool CHorzScaleDrawer::IsTextPosion(const int nPrevPos, const int nCurPos, const int nIndex, BOOL bReverse) const
{
	if(nIndex < 0) return false;

	if(bReverse == FALSE)
	{
		if(nPrevPos < nCurPos) return true;
	}
	else
	{
		if(nPrevPos > nCurPos) return true;
	}

	if(nPrevPos == nCurPos) return (nIndex == 0);
	
	return false;
}

// text를 그리기 위한 값들을 가져온다.
void CHorzScaleDrawer::GetDrawingEnvironment(const CRect& region, const CDisplayAttributes& displayAttributes, 
		const int textHeight, double& onePtWidth, int& posPt_y) const
{
	onePtWidth = CDrawer::GetOnePointWidth(region.Width(), displayAttributes.GetDataCountInBlock());
	posPt_y = GetHorzScalePosition_y(region.top, region.Height(), textHeight);
}

void CHorzScaleDrawer::GetDrawingEnvironment(const CRect& region1, const CRect& region2, const CDisplayAttributes& displayAttributes, 
		const int textHeight, double& onePtWidth, int& posPt_y1, int& posPt_y2) const
{
	onePtWidth = CDrawer::GetOnePointWidth(region1.Width(), displayAttributes.GetDataCountInBlock());
	posPt_y1 = GetHorzScalePosition_y(region1.top, region1.Height(), textHeight);
	posPt_y2 = GetHorzScalePosition_y(region2.top, region2.Height(), textHeight);
}

//	가로 스케일에 숫자를 표현할 경우 사용될 변수들을 구하는 함수
/*bool CHorzScaleDrawer::GetDrawingEnvironment(CDC* pDC, CPacket* pPacket, const CRect& scaleTextRegion, const CDisplayDataMinMax& displayDataMinMax,
		int& start, int& end, double& dGap, bool& bround, bool bstand) const
{
	SIZE textsize ;
	CString strData = CDataConversion::DoubleToString(displayDataMinMax.GetDisplayMax());
	strData = CDataConversion::GetStringFormatTrans(strData, CDrawer::GetPacketDataType(pPacket));
	::GetTextExtentPoint32(pDC->m_hDC , strData, strData.GetLength(), &textsize) ;
	int totalCount = scaleTextRegion.Width() / (textsize.cx * 2);
	if(bstand)
		totalCount = scaleTextRegion.Width() / (textsize.cy * 2);
	if(totalCount <= 0)
		return false;

	dGap = CMath::Round(displayDataMinMax.GetCount() / (double)totalCount);
	CString strGap = CDataConversion::IntToString(nGap);
	//nGap = CMath::Round(nGap, strGap.GetLength() - 1);
	// 반올림 된 값도 5, 10 등으로 라운딩한다	
	//sy 2002.11.12. -> 값이 10 이하인 경우
	if(nGap <= 10){
		double dCount = displayDataMinMax.GetCount();
		if(dCount <= 10)
			nGap = 1;
		else if(dCount <= 20)
			nGap = 5;
		else
			nGap = 10;
	}
	else if(nGap / pow(10, strGap.GetLength() - 1) == 1)
		nGap = nGap;
	else if(nGap / pow(10, strGap.GetLength() - 1) < 5)
		nGap = (int) pow(10, strGap.GetLength()) / 2;
	else if(nGap / pow(10, strGap.GetLength() - 1) < 10)
		nGap = (int) pow(10, strGap.GetLength());
	else
		nGap = CMath::Round(nGap, strGap.GetLength() - 1);
	
	if(totalCount > 0 && nGap != 0)
		totalCount = CMath::Round(displayDataMinMax.GetCount() / (double)nGap) +1;

	GetStartAndEnd(displayDataMinMax, totalCount, nGap, start, end);

	//백만단위 이상 절사 루틴 넣어봤음
	if(strGap.GetLength() >= 7)
		bround = true;
	else
		bround = false;

	return true;
}*/

bool CHorzScaleDrawer::GetDrawingEnvironment(CDC* pDC, CPacket* pPacket, const CRect& scaleTextRegion, const CDisplayDataMinMax& displayDataMinMax,
		const double& dOnPtWidth, int& start, int& end, double& dGap, bool& bround, bool bstand) const
{
	if(pPacket == NULL || displayDataMinMax.GetCount() == 0)
		return false;

	CString strPacketType = CDrawer::GetPacketDataType(pPacket);
	CString strData = CDataConversion::DoubleToString(displayDataMinMax.GetDisplayMax());
	strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strPacketType);
	CSize size = pDC->GetTextExtent(strData);

	int totalCount = scaleTextRegion.Width() / (size.cx * 2);
	if(bstand)
		totalCount = scaleTextRegion.Width() / (size.cy * 2);
	if(totalCount <= 0)
		return false;

	if( !CVertScaleDrawer( m_hOcxWnd, m_eChartMode).GetDrawingEnvironment(displayDataMinMax, strPacketType, 
		totalCount, dGap, start, end, bround))
		return false;

	dGap = GetGap(pDC, pPacket, scaleTextRegion, displayDataMinMax, dOnPtWidth, dGap);
	return true;
}

void CHorzScaleDrawer::GetStartAndEnd(const CDisplayDataMinMax& displayDataMinMax, const int totalCount, const int nGap, int& start, int& end) const
{
	if(displayDataMinMax.GetDisplayMin() < 0){
		if(nGap > 0){
			start = (int) displayDataMinMax.GetDisplayMin() / nGap -1;
			end = (int) displayDataMinMax.GetDisplayMax() / nGap +1;
		}
		else{
			start = totalCount;
			end = 0;
		}
	}
	else
		end = totalCount;
}

// text의 위치(orgX)를 가져온다. -> 왼쪽으로 맞추기
int CHorzScaleDrawer::GetHorzScalePosition_x(const int count, const int orgPt_x, const double& onePtWidth) const
{
	return (orgPt_x + CMath::Round(onePtWidth * (double)(count)));
}

// ------------------------------------------------------------------------------------
// int 형 data를 원하는 형태의 int형 data로 변경
int CHorzScaleDrawer::GetChangeNumbericData(CPacket* pPacket, const int findIndex, const CString& packetType, const CString& trimStr) const
{
	int nData = 0;
	if(CDrawer::GetDateData(pPacket, findIndex, nData))
		return GetChangeNumbericData2(nData, packetType, trimStr);
	return 0;
}

int CHorzScaleDrawer::GetChangeNumbericData2(const int orgData, const CString& packetType, const CString& trimStr) const
{
	//sy 2002.7.10
	if(trimStr.IsEmpty() || packetType.Find(trimStr) == -1)
		return orgData;

	if(IsKoreanStyle(packetType)){
		int count = packetType.GetLength() - packetType.Find(trimStr);
		int nData = CMath::Power(10, count);
		if(nData > 0)
			return orgData/nData;
	}
	else{
		int count = packetType.GetLength() - (GetFindEndIndex(packetType, trimStr) +1);
		int nPower = CMath::Power(10, count);
		int nData = orgData/nPower*nPower;
		if(nData > 0)
			return orgData%nData;
	}

	return orgData;
}

// int 형 data를 원하는 형태의 string형 data로 변경
CString CHorzScaleDrawer::GetChangeStringData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, 
		const int nData, const CString& packetType, const CString& compartType, const CString& trim) const
{
	//sy 2002.7.10
	if(packetType.Find(trim) == -1 || trim.IsEmpty())
		return CDataConversion::IntToString(nData);

	CString changePacketType = packetType;
	CString strOrgData;
	if(IsKoreanStyle(packetType)){
		int index = packetType.Find(trim);
		strOrgData = CDataConversion::IntToString(nData, index);
		changePacketType = packetType.Left(strOrgData.GetLength());
	}
	else{
		int index = GetFindEndIndex(packetType, trim) +1;
		strOrgData = CDataConversion::IntToString(nData, packetType.GetLength()).Right(packetType.GetLength() - index);
		changePacketType = packetType.Right(strOrgData.GetLength());
	}

	return CScaleCalculator( m_hOcxWnd, m_eChartMode).GetCompartScaleData_Date(strOrgData, changePacketType, compartType);
}

// int 형 data를 원하는 형태의 string형 data로 변경
CString CHorzScaleDrawer::GetChangeStringData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, 
		const int nData, const CString& packetType, const CString& trim, const CScaleCompart& scaleCompart) const
{
	int index = packetType.Find(trim);
	if(index < 0) index = packetType.GetLength();
	CString strOrgData;
	if(index > 0)	strOrgData = CDataConversion::IntToString( nData, index);
	else			strOrgData = CDataConversion::IntToString( nData);

	CString strTimeCode = strOrgData.Right( 4);
	if( strTimeCode == "6666") return _MTEXT( C6_END_OF_OPENING_TRADE);
	if( strTimeCode == "7777") return _MTEXT( C6_END_OF_CLOSING_TRADE);

	// (2006/11/19 - Seung-Won, Bae) Support samsung code. 장마감 009991, 시간외마감 009999
	if( strTimeCode == "9999") return _MTEXT( C6_OVERTIME_TRADE);
	
	// (2006/12/29 - Seung-Won, Bae) Support koscom code. 장마감 310000
	if( strTimeCode == "8888" || strTimeCode == "0099" || strTimeCode == "3100") return _MTEXT( C6_END_OF_TRADING);

	// 시간외마감 추가 - ojtaso (20071130)
	if( strTimeCode == "4100") return _MTEXT( C6_END_OF_OVERTIME_TRADE);

	// (2009/3/19 - Seung-Won, Bae) for After Closing.
	if( strTimeCode == "7100") return _LTEXT6( C6_AFTER_CLOSING);
	if( strTimeCode == "8100") return _LTEXT6( C6_END_OF_SINGLE_PRICE);

	CString changePacketType = packetType.Left(strOrgData.GetLength());
	return CScaleCalculator( m_hOcxWnd, m_eChartMode).GetCompartScaleData(scaleDrawerType, strOrgData, changePacketType, scaleCompart);
}

// ------------------------------------------------------------------------------------
// 다중으로 text 그리기 
void CHorzScaleDrawer::DrawMultiLineTextOut(CDC* pDC, const CPoint& point, const int textHeight, const CString& data, const CString& preData, const COLORREF color)
{
	CString strData = data, preStrData = preData;
	strData.TrimRight("\r\n");
	preStrData.TrimRight("\r\n");
	strData += "\r\n";
	preStrData += "\r\n";
	if(strData.IsEmpty() == TRUE)
		return;

	int lineCount = CDataConversion::GetStringDataCount(strData, "\r\n");
	int nGap = textHeight/lineCount;
	for(int i = 0; i < lineCount; i++){
		CString temp = CDataConversion::GetStringData(strData, "\r\n");
		CString preTemp = CDataConversion::GetStringData(preStrData, "\r\n");
		if(temp != preTemp || strData != preStrData)
			CDrawer::DrawTextOut(pDC, CPoint(point.x, point.y + nGap*i), temp, color);
	}
}

void CHorzScaleDrawer::DrawMultiLineTextOut(CDC* pDC, const CPoint& point, const int textHeight, const CString& data, const COLORREF color)
{
	CString strData = data;
	strData.TrimRight("\r\n");
	if(strData.IsEmpty())
		return;

	strData += "\r\n";
	int lineCount = CDataConversion::GetStringDataCount(strData, "\r\n");
	int nGap = textHeight/lineCount;
	for(int i = 0; i < lineCount; i++){
		CString strDrawingText = CDataConversion::GetStringData(strData, "\r\n");
		if(!strDrawingText.IsEmpty())
			CDrawer::DrawTextOut(pDC, CPoint(point.x, point.y + nGap*i), strDrawingText, color);
	}
}

// scale grid를 그린다.
void CHorzScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
			const CScaleBaseData::SCALEGRIDTYPE gridType, const int pt_x, const int nWidth)
{
	//sy 2004.10.5. pen의 굵기를 받는다.
	//-> 분Type에서 일자와 시간이 같이 나올 경우 일자를 굵게 표현하기 위함.
	CDrawer::DrawGrid(pDC, gridType,
		CPoint(pt_x, drawingHorzScaleData.GetDrawingGraphRegion().top), 
		CPoint(pt_x, drawingHorzScaleData.GetDrawingGraphRegion().bottom), 
		drawingHorzScaleData.GetScaleColor().GetHorzScaleColor(), nWidth);
}

// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
void CHorzScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData,
			const CScaleBaseData::SCALEGRIDTYPE gridType, const COLORREF clrGridColor, const int pt_x, const int nWidth)
{
	//sy 2004.10.5. pen의 굵기를 받는다.
	//-> 분Type에서 일자와 시간이 같이 나올 경우 일자를 굵게 표현하기 위함.
	CDrawer::DrawGrid(pDC, gridType,
		CPoint(pt_x, drawingHorzScaleData.GetDrawingGraphRegion().top),
		CPoint(pt_x, drawingHorzScaleData.GetDrawingGraphRegion().bottom),
		clrGridColor, nWidth);
}

// text compart line 그리기
void CHorzScaleDrawer::DrawTextCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const bool bIsTopPos, const COLORREF gridColor)
{
	int nHight = scaleRegion.Height() / 3;
	if(nHight > 6)
		nHight = 6;
	else if(nHight < 2 && scaleRegion.Height() > 6)
		nHight = 3;

	if(bIsTopPos){ //위쪽
		CDrawer::DrawLine(pDC, CPoint(nowPos.x, scaleRegion.bottom), 
			CPoint(nowPos.x, (scaleRegion.bottom - nHight)), gridColor);
	}
	else{	// 아래쪽
		CDrawer::DrawLine(pDC, CPoint(nowPos.x, scaleRegion.top), 
			CPoint(nowPos.x, (scaleRegion.top + nHight)), gridColor);
	}
}


// private ============================================================================
int CHorzScaleDrawer::GetFindEndIndex(const CString& data, const CString& findData) const
{
	int nFindIndex = data.Find(findData);
	if(nFindIndex < 0)
		return -1;

	CString tempData = data.Right(data.GetLength() - (nFindIndex + findData.GetLength()));
	while(data.GetLength() > nFindIndex){
		if(tempData.Find(findData) != 0)
			break;
		nFindIndex++;
		tempData.TrimLeft(findData);
	}

	return nFindIndex;
}

// text의 위치(orgY)를 가져온다.
int CHorzScaleDrawer::GetHorzScalePosition_y(const int org_y, const int orgHight, const int textHeight) const
{
	if(orgHight > (textHeight +3)){
		if(orgHight/2 >= textHeight){
			//return (org_y + (orgHight/2 - textHeight)/2);
			return (org_y + (orgHight - textHeight)/2);
		}
		return (org_y + (orgHight - textHeight)/2 +3);
	}
	return org_y;
}

// x축 scale에 맞는 gap을 구한다.
// <gap으로 인한 차>가 <두 data의 차>의 약수이어야 한다.
double CHorzScaleDrawer::GetGap(CDC* pDC, CPacket* pPacket, const CRect& scaleTextRegion, 
		const CDisplayDataMinMax& displayDataMinMax, const double& dOnPtWidth, const double& dOrgGap) const
{
	return dOrgGap;
}
