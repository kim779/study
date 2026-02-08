// DataHorzScaleDrawer.cpp: implementation of the CDataHorzScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataHorzScaleDrawer.h"

#include "Conversion.h"
#include "DataMath.h"
#include "BaseData.h"
#include "PacketBase.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CDataHorzScaleDrawer

CDataHorzScaleDrawer::CDataHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// protected ==================================================================
void CDataHorzScaleDrawer::DrawTextAndCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const bool bIsTopPos, const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos)
{
	// text구분선 그리기
	CHorzScaleDrawer::DrawTextCompartLine(pDC, scaleRegion, nowPos, bIsTopPos, gridColor);

	// text 그리기
	CPoint textPosPt(GetTextPosition_x(nowPos.x, textSize.cx, true), nowPos.y);
	if(scaleRegion.Height() > textSize.cy)
		CDrawer::DrawTextOut(pDC, textPosPt, data, textColor);
	textEndPos = textPosPt.x + textSize.cx + MARGIN;
}

// scale text의 끝 위치
int CDataHorzScaleDrawer::GetTextEndPosition_x(const CPoint& nowPos, const CSize& textSize) const
{
	CPoint textPosPt(GetTextPosition_x(nowPos.x, textSize.cx, true), nowPos.y);
	return (textPosPt.x + textSize.cx + MARGIN);
}

// scale text의 그리는 위치
int CDataHorzScaleDrawer::GetTextPosition_x(const int org_x, const int textWidth, const bool isCenter) const
{
	if(isCenter){
		return(org_x - CMath::Round(textWidth / 2.0));
	}
	return org_x +MARGIN;
}

// text 가 이전 text 와 겹치는지를 체크한다.
// return : true => 겹친다. => 안그린다.
bool CDataHorzScaleDrawer::IsOverlap(const int nTextWidth, const int nNowPos_x, const int nPrevPos_x) const
{
	int nMinNowPos_x = nPrevPos_x + MARGIN * 2;
	if(nMinNowPos_x > nNowPos_x)
		return true;
	
	int nRealNowPos_x = GetTextPosition_x(nNowPos_x, nTextWidth);
	return (nMinNowPos_x > nRealNowPos_x);
}


///////////////////////////////////////////////////////////////////////////////
// class CPriceZoneHorzScaleDrawer

CPriceZoneHorzScaleDrawer::CPriceZoneHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CDataHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

void CPriceZoneHorzScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale)
{
	CPacket* pPacket = drawingHorzScaleData.GetPacket();
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return;

	CList<double, double>* pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return;

	// 그리기 위한 기본 data를 가져온다.	
	CString strDataType = CDrawer::GetPacketDataType(pPacket);
	double dOnePtWidth = 0.0;
	int nPosPt_y = 0;	
	// (2007/10/10 - Seung-Won, Bae) Check Floating Overflow.
	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion, drawingHorzScaleData.GetDisplayAttributes(), 
		pDC->GetTextExtent(CDataConversion::DoubleToString( drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMax())).cy, dOnePtWidth, nPosPt_y);

	CRect realScaleRegion = scaleRegion;
	realScaleRegion.DeflateRect(CMath::Round(dOnePtWidth/2.0), 0);

	int nCount = 0, nPre_textEndPos = 0;
	POSITION pos = pDataList->FindIndex(drawingHorzScaleData.GetDisplayAttributes().GetDataStartInBlock());
	while(pos != NULL){
		double dData = pDataList->GetNext(pos);
		CString strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dData), strDataType);
		CPoint posPt = CDataConversion::GetHorizontalPoint(realScaleRegion, nCount, scaleRegion.Width(), 
			drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMin(),
			drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMax(), dData);

		if(posPt.x >= scaleRegion.right)
			break;

		CSize textSize = pDC->GetTextExtent(strData);
		if(!CDataHorzScaleDrawer::IsOverlap(textSize.cx, posPt.x, nPre_textEndPos)){
			CDataHorzScaleDrawer::DrawTextAndCompartLine(pDC, realScaleRegion, CPoint(posPt.x, nPosPt_y), dOnePtWidth,
				(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP ? true: false), 
				strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
				drawingHorzScaleData.GetCompartLineColor(), nPre_textEndPos);

			CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), posPt.x);
		}

		nCount++;
	}
}

void CPriceZoneHorzScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale)
{
}

void CPriceZoneHorzScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale)
{

}

// private ====================================================================


///////////////////////////////////////////////////////////////////////////////
// class CEtcDataHorzScaleDrawer

CEtcDataHorzScaleDrawer::CEtcDataHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CDataHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

void CEtcDataHorzScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale)
{
	// 그리기 위한 기본 data를 가져온다.	
	CString dataType = CDrawer::GetPacketDataType(drawingHorzScaleData.GetPacket());
	double onePtWidth = 0.0;
	int posPt_y = 0, pre_textEndPos = 0;	
	// (2007/10/10 - Seung-Won, Bae) Check Floating Overflow.
	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion, drawingHorzScaleData.GetDisplayAttributes(), 
		pDC->GetTextExtent(CDataConversion::DoubleToString( drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMax())).cy, onePtWidth, posPt_y);

	// data 를 표현하는 경우 graph 를 draw 할때는 중심부터 그린다.
	// 그러므로 양쪽을 Width/2.0 만큼 줄여줘야 한다.!!
	CRect realScaleRegion = scaleRegion;
	realScaleRegion.DeflateRect(CMath::Round(onePtWidth/2.0), 0);

	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;
	// 세워쓰기
	CFont scaleFont;
	CFont* pOldFont = NULL;
	if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::VERT_TEXT_COMPART)	{
		if(!CHorzScaleDrawer::GetDrawingEnvironment(pDC, drawingHorzScaleData.GetPacket(), realScaleRegion, 
			drawingHorzScaleData.GetDisplayDataMinMax(), onePtWidth, start, end, dGap, bround, true))
			return;
		GetFont(scaleFont);
		pOldFont = (CFont*) pDC->SelectObject(scaleFont);
	}
	else{
		if(!CHorzScaleDrawer::GetDrawingEnvironment(pDC, drawingHorzScaleData.GetPacket(), realScaleRegion, 
			drawingHorzScaleData.GetDisplayDataMinMax(), onePtWidth, start, end, dGap, bround))
			return;
	}

	double dMin = drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMin();
	// rounding
	/*double dModulus = CMath::Modulus(dMin, dGap);
	if(dModulus > 0){
		dMin += dGap - dModulus;
	}*/

	CPoint posPt = CPoint(0,0);
	int nCount = 0;
	while(posPt.x < realScaleRegion.right){
		double value = dMin + dGap * nCount;
		if(value > drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMax())
			break;
		CString strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(value), dataType);

		posPt = CDataConversion::GetHorizontalPoint(realScaleRegion, nCount, scaleRegion.Width(), drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMin(),
					drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMax(), value);

		// 세워쓰기
		if(horzScale.GetScaleCompart().GetScaleTextType() == CScaleBaseData::VERT_TEXT_COMPART){
			DrawTextAndCompartLine_stand(pDC, realScaleRegion, CPoint(posPt.x, posPt_y), onePtWidth,
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
		}
		else{
			CDataHorzScaleDrawer::DrawTextAndCompartLine(pDC, realScaleRegion, CPoint(posPt.x, posPt_y), onePtWidth,
					(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP ? true: false), 
					strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
					drawingHorzScaleData.GetCompartLineColor(), pre_textEndPos);
		}
		CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), posPt.x);
		nCount++;		
	}

	if(pOldFont != NULL){
		pDC->SelectObject(pOldFont);
		scaleFont.DeleteObject();
	}
}

void CEtcDataHorzScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale)
{
}

void CEtcDataHorzScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale)
{
}

// private ====================================================================
// 세워쓰기 : 일반 세로쓰기가 아니다 
void CEtcDataHorzScaleDrawer::DrawTextAndCompartLine_stand(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const double& textRegionWidth,
		const CString& data, const CSize& textSize, const COLORREF textColor, const COLORREF gridColor, int& textEndPos)
{
	// 가로스케일 영역에 구분선을 1/3만 그린다
	// 08.24.2001
	CDrawer::DrawLine(pDC, CPoint(nowPos.x, scaleRegion.top), 
						   CPoint(nowPos.x, scaleRegion.top + (scaleRegion.bottom - scaleRegion.top) / 3), gridColor);
	CPoint textPosPt(CDataHorzScaleDrawer::GetTextPosition_x(nowPos.x, (int)textRegionWidth), scaleRegion.bottom);//nowPos.y);
	if(scaleRegion.Height() > textSize.cy)	{
		CFont            m_fontVertCaption;
		CFont			 font;
		if (font.CreateStockObject (DEFAULT_GUI_FONT))
		{
     			LOGFONT lf;
       			memset (&lf, 0, sizeof (LOGFONT));
       			if (font.GetLogFont (&lf) != 0)
       			{
       				lf.lfOrientation = 900;
       				lf.lfEscapement = 900;
       				lf.lfHeight = 12;
       				lf.lfWidth = 0;
       				lf.lfWeight = 400;
//       				lf.lfItalic = TRUE;
//       				_tcscpy (lf.lfFaceName, _T("Arial"));
       				 m_fontVertCaption.CreateFontIndirect (&lf);
       			}
		}
		CFont* pOldFont = pDC->SelectObject (pDC->GetCurrentFont());
		pDC->SelectObject(m_fontVertCaption);
		int MODE = pDC->SetBkMode(TRANSPARENT);
		CDrawer::DrawTextOut(pDC, textPosPt, data, textColor);
		pDC->SelectObject(pOldFont);
		m_fontVertCaption.DeleteObject();
	}
	textEndPos = textPosPt.x + textSize.cx;
}

void CEtcDataHorzScaleDrawer::GetFont(CFont& fontVertCaption)
{
	CFont font;
	if(font.CreateStockObject (DEFAULT_GUI_FONT))
	{
     	LOGFONT lf;
       	memset (&lf, 0, sizeof (LOGFONT));
       	if (font.GetLogFont (&lf) != 0)
       	{
       		lf.lfOrientation = 900;
       		lf.lfEscapement = 900;
       		lf.lfHeight = 12;
       		lf.lfWidth = 0;
       		lf.lfWeight = 400;
//       	lf.lfItalic = TRUE;
//       	_tcscpy (lf.lfFaceName, _T("Arial"));
			fontVertCaption.CreateFontIndirect (&lf);
       	}
	}
	font.DeleteObject();
}


///////////////////////////////////////////////////////////////////////////////
// class CLeftRightScaleDrawer

CLeftRightScaleDrawer::CLeftRightScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CDataHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

void CLeftRightScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale)
{
	// 그리기 위한 기본 data를 가져온다.
	CList<double, double> dataList;
	double dMin(0.0), dMax(0.0), dOnePtWidth(0.0);
	int nPosPt_y(0);	
	if(!GetDrawingEnvironment(drawingHorzScaleData, scaleRegion, pDC->GetTextExtent("0").cy, 
		dataList, dMin, dMax, dOnePtWidth, nPosPt_y))
		return;

	double dPreData;
	int nPre_textEndPos(0);
	POSITION pos = dataList.GetHeadPosition();
	while(pos != NULL){
		double dData = dataList.GetNext(pos);
		CString strData = GetData(dData, dPreData);
		if(strData.IsEmpty())
			continue;

		int nPosPt_x = CDataConversion::GetHorizontalPoint_X(scaleRegion, dMin, dMax, dData);
		CDataHorzScaleDrawer::DrawTextAndCompartLine(pDC, scaleRegion, CPoint(nPosPt_x, nPosPt_y), dOnePtWidth,
			(horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP ? true: false), 
			strData, pDC->GetTextExtent(strData), drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
			drawingHorzScaleData.GetCompartLineColor(), nPre_textEndPos);

		CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), nPosPt_x);
	}
}

void CLeftRightScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale)
{
}

void CLeftRightScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale)
{
	CRect realScaleRegion = drawingHorzScaleData.GetDrawingGraphRegion();

	// 그리기 위한 기본 data를 가져온다.
	CList<double, double> dataList;
	double dMin(0.0), dMax(0.0), dOnePtWidth(0.0);
	int nPosPt_y(0);	
	if(!GetDrawingEnvironment(drawingHorzScaleData, realScaleRegion, pDC->GetTextExtent("0").cy, 
		dataList, dMin, dMax, dOnePtWidth, nPosPt_y))
		return;

	double dPreData;
	int nPre_textEndPos(0);
	POSITION pos = dataList.GetHeadPosition();
	while(pos != NULL){
		double dData = dataList.GetNext(pos);
		CString strData = GetData(dData, dPreData);
		if(strData.IsEmpty())
			continue;

		int nPosPt_x = CDataConversion::GetHorizontalPoint_X(realScaleRegion, dMin, dMax, dData);
		CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), nPosPt_x);
	}
}


// private ====================================================================
bool CLeftRightScaleDrawer::GetDrawingEnvironment(const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const int nTextHeight, 
		CList<double, double>& dataList, double& dMin, double& dMax, double& dOnePtWidth, int& nPosPt_y) const
{
	if(!GetDataListAndMinMax(drawingHorzScaleData, dataList, dMin, dMax))
		return false;

	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion, drawingHorzScaleData.GetDisplayAttributes(), 
		nTextHeight, dOnePtWidth, nPosPt_y);
	return true;
}

bool CLeftRightScaleDrawer::GetDataListAndMinMax(const CDrawingHorzScaleData& drawingHorzScaleData, CList<double, double>& dataList, double& dMin, double& dMax) const
{
	CPacket* pPacket = drawingHorzScaleData.GetPacket();
	if(pPacket == NULL || pPacket->GetCount() <= 0)
		return false;

	double dStandardData = pPacket->GetStandardValue();
	if(dStandardData < 0)
		dStandardData = 0;

	double dMiddle_Min(0.0), dMiddle_Max(0.0), dRealMin(0.0), dRealMax(0.0);
	GetRealMinMax(drawingHorzScaleData, pPacket->GetType(), dMiddle_Min, dMiddle_Max, dRealMin, dRealMax);

	dataList.AddTail(dRealMin);
	dataList.AddTail(dMiddle_Min);
	dataList.AddTail(dStandardData);
	dataList.AddTail(dMiddle_Max);
	dataList.AddTail(dRealMax);

	// 화면에 여백을 준다. (6%)
	CDisplayDataMinMax displayDataMinMax(dRealMin, dRealMax);
	dMin = displayDataMinMax.GetMarginDisplayMin();
	dMax = displayDataMinMax.GetMarginDisplayMax();
	return true;
}

void CLeftRightScaleDrawer::GetRealMinMax(const CDrawingHorzScaleData& drawingHorzScaleData, const CString& strPacketType, double& dMiddle_Min, double& dMiddle_Max, double& dMin, double& dMax) const
{
	int nRoundPosition = CDataConversion::GetDecimalDownCount(strPacketType) +1;

	// 고정비율좌우식
	if(drawingHorzScaleData.GetHeadGraphStyle().GetGraphBarStyle() == CGraphBaseData::FixedRateLeftRightBar){
		GetRealMinMax_FixedRate(drawingHorzScaleData.GetDisplayDataMinMax(), dMin, dMax);
		dMiddle_Max = CMath::Floor(dMax/2.0, nRoundPosition);
		dMiddle_Min = -dMiddle_Max;
	}
	else{
		dMin = drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMin();
		dMax = drawingHorzScaleData.GetDisplayDataMinMax().GetDisplayMax();
		dMiddle_Max = CMath::Floor(dMax/2.0, nRoundPosition);
		dMiddle_Min = CMath::Floor(dMin/2.0, nRoundPosition);
	}
}

void CLeftRightScaleDrawer::GetRealMinMax_FixedRate(const CDisplayDataMinMax& displayDataMinMax, double& dMin, double& dMax) const
{
	// 고정비율좌우식
	double absMin = fabs(displayDataMinMax.GetDisplayMin());
	double absMax = fabs(displayDataMinMax.GetDisplayMax());

	if(absMin > absMax){
		dMin = -absMin;
		dMax = absMin;
	}
	else{
		dMin = -absMax;
		dMax = absMax;
	}
}

CString CLeftRightScaleDrawer::GetData(const double& dData, double& dPreData) const
{
	if(dPreData == dData)
		return "";

	dPreData = dData;
	
	if(dData == 0.0)
		return "0";

	CString strData = CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dData));
	if(!strData.IsEmpty() && strData.Left(1) != '-')
		strData = '+' + strData;

	return strData;
}


////////////////////////////////////////////////////////////////////////////////////
// class CMarketProfileHorzScaleDrawer

CMarketProfileHorzScaleDrawer::CMarketProfileHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// 모든 data 를 표현하다. (일자) - 균등간격
void CMarketProfileHorzScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& scaleRegion, const CHorizontalScale& horzScale)
{
	DrawScale(pDC, drawingHorzScaleData, scaleRegion, CRect(0, 0, 0, 0), horzScale);
}

void CMarketProfileHorzScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData,
		const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale)
{
	DrawScale(pDC, drawingHorzScaleData, horzScaleRegion.GetTopScaleRegion(), horzScaleRegion.GetBottomScaleRegion(), horzScale);
}

void CMarketProfileHorzScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale)
{
	DrawScale(pDC, drawingHorzScaleData, drawingHorzScaleData.GetDrawingGraphRegion(), CRect(0, 0, 0, 0), horzScale);
}


// private ====================================================================
void CMarketProfileHorzScaleDrawer::DrawScale(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& scaleRegion1, const CRect& scaleRegion2, const CHorizontalScale& horzScale)
{
	if(drawingHorzScaleData.GetPacket() == NULL || scaleRegion1.IsRectEmpty())
		return;

	CList<CMProfile*, CMProfile*>* pMProfileList = NULL;
	double dOnePtWidth = 0.0;
	int nPosPt_y1 = 0, nPosPt_y2 = 0;
	if(!GetEnvironmentData((CMPPacket*) drawingHorzScaleData.GetPacket(), 
		scaleRegion1, scaleRegion2, pDC->GetTextExtent("0").cy, pMProfileList, 
		dOnePtWidth, nPosPt_y1, nPosPt_y2))
		return;

	int nIndex = 0, nPre_textEndPos = 0;
	POSITION pos = pMProfileList->GetHeadPosition();
	while(pos != NULL){
		CMProfile* pMProfile = pMProfileList->GetNext(pos);
		if(pMProfile == NULL)
			break;

		int nPosPt_x = CHorzScaleDrawer::GetHorzScalePosition_x(nIndex, scaleRegion1.left, dOnePtWidth);
		// pre text와 겹칠 경우엔 그리지 않는다.
		if(IsDrawText(horzScale.GetHorzScalePosition(), nPre_textEndPos, nPosPt_x)){
			// (2004.02.03, 배승원) Market Profile Chart의 X Scale로 일자를 출력코자하는데,
			//		MP ChartObject는 DDHHMM의 6자리 시간으로 기억하여 이를 환산시킨다.
			// (2004.02.10, 배승원) 일시 Packet에 '월'도 받아 X Scale에 출력토록 한다.
			//		MP ChartObject는 MMDDHHMM의 8자리 시간으로 기억하여 이를 환산시킨다.
			int nDate = pMProfile->GetDate();
			CString strDate;
			strDate.Format( "%02d/%02d", ( nDate / 1000000) % 100, ( nDate / 10000) % 100);
			DrawTextAndCompartLine(pDC, scaleRegion1, CPoint(nPosPt_x, nPosPt_y1), 
				IsTopPostiont(horzScale.GetHorzScalePosition(), true), 
				strDate, drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
				drawingHorzScaleData.GetCompartLineColor(), nPre_textEndPos);
			DrawTextAndCompartLine(pDC, scaleRegion2, CPoint(nPosPt_x, nPosPt_y2), 
				IsTopPostiont(horzScale.GetHorzScalePosition(), false), 
				strDate, drawingHorzScaleData.GetScaleColor().GetScaleTextColor(), 
				drawingHorzScaleData.GetCompartLineColor(), nPre_textEndPos);
		}
		if(nIndex != 0)
			CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), nPosPt_x);

		nIndex++;
	}
}

// scale text와 scale text 구분선 그리기
void CMarketProfileHorzScaleDrawer::DrawTextAndCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const bool bIsTopPos, 
		const CString& data, const COLORREF textColor, const COLORREF gridColor, int& textEndPos)
{
	if(scaleRegion.IsRectEmpty())
		return;

	// text구분선 그리기
	CHorzScaleDrawer::DrawTextCompartLine(pDC, scaleRegion, nowPos, bIsTopPos, gridColor);

	// text 그리기
	CPoint textPosPt(nowPos.x +MARGIN, nowPos.y);
	// scale text 적을 공간 비교
	CSize textSize = pDC->GetTextExtent(data);
	if(scaleRegion.Height() > textSize.cy)
		CDrawer::DrawTextOut(pDC, textPosPt, data, textColor);
	textEndPos = textPosPt.x + textSize.cx + MARGIN;
}

bool CMarketProfileHorzScaleDrawer::GetEnvironmentData(CMPPacket* pMPPacket, const CRect& scaleRegion1, const CRect& scaleRegion2, const int nSize_Y, 
		CList<CMProfile*, CMProfile*>*& pMProfileList, double& dOnePtWidth, int& nPosPt_y1, int& nPosPt_y2) const
{
	if(pMPPacket == NULL)
		return false;

	pMProfileList = pMPPacket->GetMarketProfileList();
	if(pMProfileList == NULL || pMProfileList->GetCount() <= 0)
		return false;

	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion1, scaleRegion2, 
		CDisplayAttributes(0, pMProfileList->GetCount() -1), 
		nSize_Y, dOnePtWidth, nPosPt_y1, nPosPt_y2);

	return (dOnePtWidth > 0 ? true: false);
}

bool CMarketProfileHorzScaleDrawer::IsDrawText(const CScaleBaseData::HORZSCALEPOSITION eHorzScalePos, const int nPre_textEndPos, const int nPosPt_x) const
{
	if(eHorzScalePos == CScaleBaseData::HORZ_HIDE)
		return false;

	return (nPre_textEndPos < nPosPt_x);
}

bool CMarketProfileHorzScaleDrawer::IsTopPostiont(const CScaleBaseData::HORZSCALEPOSITION eHorzScalePos, const bool bIsScaleRegion1) const
{
	if(eHorzScalePos == CScaleBaseData::HORZ_BOTTOM){
		return !bIsScaleRegion1;
	}

	return bIsScaleRegion1;
}
