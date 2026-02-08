// BarDraw.cpp: implementation of the CBarDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarDraw::CBarDraw()
{
	m_eGraphType = CGraphBaseData::Bar_Type;
}

CBarDraw::~CBarDraw()
{
}
//public ====================================================================================
bool CBarDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	return false;
}


bool CBarDraw::CheckBoxBarType(CPacket* pPacket)
{
	if(!pPacket)	return false;
	///분, 틱이면 forward		일, 주, 월이면 backward
	CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType = 
		(CScaleBaseData::HORZSCALEDRAWERTYPE)pPacket->GetDateUnitType();

	CPacket* highPacket = NULL, *lowPacket = NULL;	
	switch(scaleDrawerType){
	case CScaleBaseData::HORZ_TIME_ONE:	//시간
	case CScaleBaseData::HORZ_TIME_TWO:	//시간
	case CScaleBaseData::HORZ_TICK:		//틱
		return true;
	case CScaleBaseData::HORZ_DAILY:	//일간
	case CScaleBaseData::HORZ_WEEKLEY:	//주간
	case CScaleBaseData::HORZ_MONTHLY:	//월간
		return false;
	}

	return false;
}

//sy 2005.01.26. -> <대기매물>의 text를 옵션으로 처리
int CBarDraw::GetTextType(const CIndicatorInfo* pIndicatorInfo) const
{
	//<대기매물> 이 아닌경우에는 text를 모두 보여준다.
	if(pIndicatorInfo == NULL || pIndicatorInfo->GetIndicatorName() != _MTEXT( C2_OVERHANGING_SUPPLY))
		return 0;

	//0 : 모두 1 : 숨김  2 : 비율만  3 : Data만
	CList<double, double>* pDrawCondList = pIndicatorInfo->GetDrawConditionList();
	if(pDrawCondList == NULL || pDrawCondList->GetHeadPosition() == NULL)
		return 0;

	return ((int) pDrawCondList->GetHead());
}

void CBarDraw::GetBarWidth(const CRect& graphRegion, const int nDataCount, 
		double& dBarMaxWidth, int& nBarWidth)
{
	dBarMaxWidth = (double)graphRegion.Height()/(double)nDataCount;
	nBarWidth = int((dBarMaxWidth * 0.8)) / 2;
	if(nBarWidth <= 0)
		nBarWidth = 1;
}

// 실제 그림을 그릴 영역을 구한다.
//-> "대기매물" : "종가"의 min/max 영역.
CRect CBarDraw::GetDrawingRegion(CPacket* pPacket, const CRect& graphRegion, 
		const CString& strSubGraphName, const int nStartIndex, const int nEndIndex, 
		const double& dViewMin, const double& dViewMax) const
{
	if(strSubGraphName != _MTEXT( C2_OVERHANGING_SUPPLY))
		return graphRegion;

	// "대기매물" drawing region.
	CPacket* pClosePacket = GetPacket(pPacket, _MTEXT( C0_CLOSE));
	if(pClosePacket == NULL)
		return graphRegion;

	double dMin = 0.0, dMax = 0.0;
	if(pClosePacket->GetMinMax(nStartIndex, nEndIndex, dMin, dMax) < 0)
		return graphRegion;

	CRect drawingRegion = graphRegion;
	int nMinPosition = CDataConversion::RelativeToAbsolute_Y(graphRegion, dMin, dViewMin, dViewMax);
	if(graphRegion.bottom > nMinPosition)
		drawingRegion.bottom = nMinPosition;
	int nMaxPosition = CDataConversion::RelativeToAbsolute_Y(graphRegion, dMax, dViewMin, dViewMax);
	if(graphRegion.top < nMaxPosition)
		drawingRegion.top = nMaxPosition;

	return drawingRegion;
}

// 그릴 text 구하기.
CString CBarDraw::GetDrawingText(const int nTextType, const double& dTotalData, const double& dData) const
{
	double dPercentData = 0.0;
	if(dTotalData != 0.0)
		dPercentData = (dData * 100.0) / dTotalData;

	//0 : 모두 1 : 숨김  2 : 비율만  3 : Data만
	CString strData;
	if(nTextType == 1)
	{
		strData.Empty();
	}
	else if(nTextType == 2)
	{
		strData.Format("%s%%", CDataConversion::DoubleToString(dPercentData, 2));
	}
	else if(nTextType == 3)
	{
		strData.Format("%s", CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dData)));
	}
	else
	{
		strData.Format("%s%% (%s)", 
			CDataConversion::DoubleToString(dPercentData, 2), 
			CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dData)));
	}

	return strData;
}

void CBarDraw::CreatePenAndBrush(const CGraphColor& graphColor, 
		CPen& pen1, CPen& pen2, CPen& pen3, CBrush& brush1, CBrush& brush2, CBrush& brush3)
{
	//기본
	pen1.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	brush1.CreateSolidBrush(graphColor.GetColor1());

	//현재
	pen2.CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	brush2.CreateSolidBrush(graphColor.GetColor3());

	//최대
	pen3.CreatePen(PS_SOLID, 1, graphColor.GetColor5());
	brush3.CreateSolidBrush(graphColor.GetColor5());
}

void CBarDraw::SelectPenAndBrushObject(CDC* pDC, CPen* pPen1, CPen* pPen2, CPen* pPen3, 
		CBrush* pBrush1, CBrush* pBrush2, CBrush* pBrush3, const CGraphColor& graphColor, 
		const int nIndex, const int nCurrentIndex, const int nHighestIndex)
{
	//현재
	if(nIndex == nCurrentIndex)
	{
		if(graphColor.GetDownGraphFill())
			pDC->SelectObject(pBrush2);
		else
			pDC->SelectStockObject(NULL_BRUSH);
		
		pDC->SelectObject(pPen2);
		pDC->SetTextColor(graphColor.GetColor4());
	}
	//최대
	else if(nIndex == nHighestIndex)
	{
		if(graphColor.GetDownGraphFill())
			pDC->SelectObject(pBrush3);
		else
			pDC->SelectStockObject(NULL_BRUSH);
		
		pDC->SelectObject(pPen3);
		pDC->SetTextColor(graphColor.GetColor6());
	}
	//기본
	else
	{
		if(graphColor.GetUpGraphFill())
			pDC->SelectObject(pBrush1);
		else
			pDC->SelectStockObject(NULL_BRUSH);
		
		pDC->SelectObject(pPen1);
		pDC->SetTextColor(graphColor.GetColor2());
	}
}

// ----------------------------------------------------------------------------
// <거래량 차트>, <거래대금> 일 경우
bool CBarDraw::DoesVolumeDrawingType(CIndicatorInfo* pIndicatorInfo) const
{
	if(pIndicatorInfo == NULL)
		return false;

	return g_iMetaTable.IsDrawGroup_VolumeType(pIndicatorInfo->GetIndicatorName());
}

// data를 text로 그릴지의 여부
bool CBarDraw::DoesTextDrawing(const CGraphBaseData::BARDRAWSTYLE eDrawStyle) const
{
	return (eDrawStyle == CGraphBaseData::TextBar_Draw);
}

int CBarDraw::GetDrawType(CIndicatorInfo* pIndicatorInfo, const bool bIsDrawType) const
{
	if(pIndicatorInfo == NULL)
		return 0;

	CList<double, double>* pDoubleList = NULL;
	if(bIsDrawType)
		pDoubleList = pIndicatorInfo->GetDrawConditionList();
	else
		pDoubleList = pIndicatorInfo->GetCommonCalConditionList();

	if(pDoubleList == NULL || pDoubleList->GetCount() <= 0)
		return 0;

	return (int)pDoubleList->GetHead();
}

//sy 2004.05.17.
double CBarDraw::GetTotalData(CList<double, double>* pDataList, const int nEndIndex) const
{
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return 0.0;

	double dTotalData = 0.0;
	POSITION pos = pDataList->GetHeadPosition();
	for(int nIndex = 0; nIndex < nEndIndex; nIndex++)
	{
		if(pos == NULL)
			break;

		double dData = pDataList->GetNext(pos);
		if(dData < 0)
			dData = fabs(dData);
		dTotalData += dData;
	}

	return dTotalData;
}

CPacket* CBarDraw::GetPacket(CPacket* pBasePacket, const CString& strPacketName) const
{
	if(pBasePacket == NULL)
		return NULL;

	CPacketList* pPacketList = pBasePacket->GetPacketList();
	if(pPacketList == NULL)
		return NULL;

	return pPacketList->GetBaseData(strPacketName);
}
//sy end

bool CBarDraw::GetDatasInPacket(CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		const int nDataIndex, int& nDrawingIndex, 
		CList<double, double>*& pVolumeList, POSITION& volumePos, 
		CList<double, double>*& pOpenList, POSITION& openPos, 
		CList<double, double>*& pCloseList, POSITION& closePos) const
{
	if(pVolumePacket == NULL || pOpenPacket == NULL || pClosePacket == NULL)
		return false;

	if(pVolumePacket->GetStartPeriod() > nDataIndex)
		nDrawingIndex = pVolumePacket->GetStartPeriod() - nDataIndex;

	if(!GetDatasInPacket(pVolumePacket, nDataIndex, pVolumeList, volumePos))
		return false;
	if(!GetDatasInPacket(pOpenPacket, nDataIndex, pOpenList, openPos))
		return false;
	return GetDatasInPacket(pClosePacket, nDataIndex, pCloseList, closePos);
}

//sy 2005.11.02.
bool CBarDraw::GetDatasInPacket(CPacket* pVolumePacket, CPacket* pClosePacket, 
		const int nDataIndex, int& nDrawingIndex, 
		CList<double, double>*& pVolumeList, POSITION& volumePos, 
		CList<double, double>*& pCloseList, POSITION& closePos) const
{
	if(pVolumePacket == NULL || pClosePacket == NULL)
		return false;

	if(pVolumePacket->GetStartPeriod() > nDataIndex)
		nDrawingIndex = pVolumePacket->GetStartPeriod() - nDataIndex;

	if(!GetDatasInPacket(pVolumePacket, nDataIndex, pVolumeList, volumePos))
		return false;
	return GetDatasInPacket(pClosePacket, nDataIndex, pCloseList, closePos);
}
//sy end

bool CBarDraw::GetDatasInPacket(CPacket* pPacket, const int nDataIndex, int& nDrawingIndex, CList<double, double>*& pDataList, POSITION& pos) const
{
	if(pPacket == NULL)
		return false;

	if(pPacket->GetStartPeriod() > nDataIndex)
		nDrawingIndex = pPacket->GetStartPeriod() - nDataIndex;

	return GetDatasInPacket(pPacket, nDataIndex, pDataList, pos);
}

bool CBarDraw::GetDatasInPacket(CPacket* pPacket, const int nDataIndex, CList<double, double>*& pDataList, POSITION& pos) const
{
	if(pPacket == NULL)
		return false;

	pDataList = pPacket->GetnumericDataList();	
	if(pDataList == NULL)
		return false;
	pos = pDataList->FindIndex(nDataIndex);
	return (pos != NULL);
}

bool CBarDraw::GetPrevData(CList<double, double>* pDataList, const POSITION& curPos, const int nIndex, double& dPrevData) const
{
	if(pDataList == NULL || curPos == NULL || nIndex <= 0)
		return false;

	POSITION prePos = curPos;
	for(int i = 0; i < nIndex; i++){
		pDataList->GetPrev(prePos);
		if(prePos == NULL)
			return false;
	}
	dPrevData = pDataList->GetAt(prePos);
	return true;
}


void CBarDraw::CreateBrushsAndPens(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor, 
		const double& dWidth, const CPenThickness& penThickness, const bool bIsDrawingThickness)
{
	pBrush[UP_BRUSH].CreateSolidBrush(graphColor.GetColor2());
	pBrush[DOWN_BRUSH].CreateSolidBrush(graphColor.GetColor4());
	pBrush[BOHAP_BRUSH].CreateSolidBrush(graphColor.GetColor6());

	if(bIsDrawingThickness){
		pPen[UP_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetDrawingThickness1(), dWidth), graphColor.GetColor1());
		pPen[DOWN_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetDrawingThickness1(), dWidth), graphColor.GetColor3());
		pPen[BOHAP_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetDrawingThickness1(), dWidth), graphColor.GetColor5());
	}
	else{
		pPen[UP_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor1());
		pPen[DOWN_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor3());
		pPen[BOHAP_PEN].CreatePen(PS_SOLID, GetThickness(penThickness.GetThickness1(), dWidth), graphColor.GetColor5());
	}
}

void CBarDraw::DeleteBrushsAndPens(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen)
{
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	for(int i = 0; i < PEN_COUNT; i++)
		pPen[i].DeleteObject();

	for(int i = 0; i < BRUSH_COUNT; i++)
		pBrush[i].DeleteObject();
}

void CBarDraw::SelectGdiObject_VolumeChart(CDC* pDC, const double& dCurVolume, const double& dPrevVolume, 
		const CGraphColor& graphColor, CBrush* pBrush, CPen* pPen)
{
	//상승
	if(dCurVolume > dPrevVolume){
		SelectGdiObject_HighLow(pDC, graphColor.GetUpGraphFill() == 0 ? false : true, UP_BRUSH, UP_PEN, pBrush, pPen);
	}
	//하락
	else if(dCurVolume < dPrevVolume){
		SelectGdiObject_HighLow(pDC, graphColor.GetDownGraphFill() == 0 ? false : true, DOWN_BRUSH, DOWN_PEN, pBrush, pPen);
	}
	//보합
	else{
		SelectGdiObject_HighLow(pDC, graphColor.GetUpGraphFill() == 0 ? false : true, BOHAP_BRUSH, BOHAP_PEN, pBrush, pPen);
	}
}

void CBarDraw::SelectGdiObject_PriceChart(CDC* pDC, const int nBohapType, const double& dOpen, const double& dClose, 
		const double& dPreClose, const CGraphColor& graphColor, CBrush* pBrush, CPen* pPen)
{
	//상승
	if(dOpen < dClose){
		SelectGdiObject_HighLow(pDC, graphColor.GetUpGraphFill() == 0 ? false : true, UP_BRUSH, UP_PEN, pBrush, pPen);
	}
	//하락
	else if(dOpen > dClose){
		SelectGdiObject_HighLow(pDC, graphColor.GetDownGraphFill() == 0 ? false : true, DOWN_BRUSH, DOWN_PEN, pBrush, pPen);
	}
	//보합
	else{
		SelectGdiObject_Bohap(pDC, nBohapType, dClose, dPreClose, graphColor.GetUpGraphFill() == 0 ? false : true, pBrush, pPen);
	}
}

// 상승/하락
void CBarDraw::SelectGdiObject_HighLow(CDC* pDC, const bool bFill, const int nBrushIndex, 
		const int nPenIndex, CBrush* pBrush, CPen* pPen)
{
	SelectBrushObject(pDC, bFill, nBrushIndex, pBrush);

	if(nPenIndex >= 0 && nPenIndex < PEN_COUNT)
		pDC->SelectObject(pPen[nPenIndex]);
}

// 보합
void CBarDraw::SelectGdiObject_Bohap(CDC* pDC, const int nBohapType, const double& dClose, 
		const double& dPreClose, const bool bFill, CBrush* pBrush, CPen* pPen)
{
	if(nBohapType == 2){ //전일종가 보합
		if(dClose > dPreClose){
			pDC->SelectObject(pPen[UP_PEN]);
			SelectBrushObject(pDC, bFill, UP_BRUSH, pBrush);
		}
		else{
			pDC->SelectObject(pPen[DOWN_PEN]);
			SelectBrushObject(pDC, bFill, DOWN_BRUSH, pBrush);
		}
	}
	else{ //일반보합
		SelectBrushObject(pDC, bFill, BOHAP_BRUSH, pBrush);
		pDC->SelectObject(pPen[BOHAP_PEN]);
	}
}

void CBarDraw::SelectBrushObject(CDC* pDC, const bool bFill, const int nIndex, CBrush* pBrush)
{
	if(bFill){
		if(nIndex >= 0 && nIndex < BRUSH_COUNT)
			pDC->SelectObject(pBrush[nIndex]);
	}
	else
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
}
