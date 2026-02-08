// ElementTreeBuilder.cpp: implementation of the CElementTreeBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementTreeBuilder.h"

#include "AnalysisData.h"
#include "../Include_Analysis/ElementEnvironment.h"
#include "Coordinate.h"
#include "ElementFactory.h"
#include "ElementTree.h"
#include "Conversion.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CElementAndOrder

// public =====================================================================
CElementAndOrder::CElementAndOrder(const int nIndex, CElement* pElement):
	m_nIndex(nIndex),
	m_pElement(pElement)
{
}

CElementAndOrder& CElementAndOrder::operator=(const CElementAndOrder& elementAndOrder)
{
	if(this == &elementAndOrder)
		return *this;

	m_nIndex = elementAndOrder.GetIndex();
	m_pElement = elementAndOrder.GetElement();
	return *this;
}

int CElementAndOrder::GetIndex() const
{
	return m_nIndex;
}

CElement* CElementAndOrder::GetElement() const
{
	return m_pElement;
}

///////////////////////////////////////////////////////////////////////////////
// class CElementAndOrderSet

void CElementAndOrderSet::AddTail(const CElementAndOrder& elementAndOrder)
{
	m_ElementAndOrderSet.AddTail(elementAndOrder);
}

// ----------------------------------------------------------------------------
// list의 총 data 갯수.
int CElementAndOrderSet::GetCount() const
{
	return m_ElementAndOrderSet.GetCount();
}

// list에 저장된 data 의 index 중 가장 큰 값.
int CElementAndOrderSet::GetMaxIndex() const
{
	int nMaxIndex = -1;
	for(int nIndex = 0; nIndex < m_ElementAndOrderSet.GetCount(); nIndex++)
	{
		int nData = m_ElementAndOrderSet.GetAt(nIndex).GetIndex();
		if(nData > nMaxIndex)
			nMaxIndex = nData;
	}
	return nMaxIndex;
}

// 해당 index의 data.
CElementAndOrder CElementAndOrderSet::GetData(const int nIndex) const
{
	if(nIndex < 0 || nIndex >= m_ElementAndOrderSet.GetCount())
		return CElementAndOrder(-1, NULL);

	return m_ElementAndOrderSet.GetAt(nIndex);
}

// 해당 index의 element.
CElement* CElementAndOrderSet::GetElement(const int nIndex) const
{
	for(int i = 0; i < m_ElementAndOrderSet.GetCount(); i++)
	{
		CElementAndOrder elementAndOrder = GetData(i);
		if(elementAndOrder.GetIndex() == nIndex)
			return elementAndOrder.GetElement();
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// class CElementTreeBuilder

/* ----------------------------------------------------------------------------
  ** 저장 순서 **
  <Column>
  <Row>
  <Element>
  <Index>0<\\Index>
  <ToolType>0<\\ToolType>
  <Points>
  <Point>0,0<\\Point>
  ...
  <\\Points>
  <CommonEnvironment>
  <AutoPirce>0<\\AutoPirce>
  <LineExt>
  <LeftExt>1<\\LeftExt>
  <RightExt>0<\\RightExt>
  <\\LineExt>
  <EquiDiv>0<\\EquiDiv>
  <PriceRangeDiv>0<\\PriceRangeDiv>
  <Pen>
  <Color>0,0,0<\\Color>
  <Width>1<\\Width>
  <Style>0<\\Style>
  <\\Pen>
  <DataType>
  <Coordinate>0<\\Coordinate>
  <Vert>1<\\Vert>
  <Horz>YYYYMMDD<\\Horz>
  <\\DataType>
  <\\CommonEnvironment>
  <PartEnvironment>
  <TextEnvironment>
  <Color>0,0,0<\\Color>
  <\\TextEnvironment>
  <TextPosition>
  <Horz>
  <Show>0<\\Show>
  <\\Horz>
  <Horz>
  <Left>0<\\Left>
  <Right>0<\\Right>
  <\\Horz>
  <Vert>
  <Top>0<\\Top>
  <Bottom>0<\\Bottom>
  <\\Vert>
  <HorzVert>
  <Left>0<\\Left>
  <Right>0<\\Right>
  <Top>0<\\Top>
  <Bottom>0<\\Bottom>
  <\\HorzVert>
  <\\TextPosition>
  <\\PartEnvironment>
  <\\Element>
  <\\Row>
  <\\Column>
------------------------------------------------------------------------------*/
// element 만들기.
bool CElementTreeBuilder::BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData, CTree& elementTree, COrder& elementOrder)
{
	CElementAndOrderSet elementAndOrderSet;
	// CTree에 저장
	if(!BuildElementsFromString(analysisData,strElementsData, elementTree, elementAndOrderSet))
		return false;

	// COrder에 저장
	BuildElementsFromString(analysisData,elementAndOrderSet, elementOrder);

	return true;
}

long CElementTreeBuilder::BuildElementsFromString_ForUndoRedo(CAnalysisData& analysisData,const CBlockIndex& blockIndex, const CString& strElementData, 
		CTree& elementTree, COrder& elementOrder)
{
	if(strElementData.IsEmpty())		return 0;

	CString elementData = GetData(strElementData, "<Element>\r\n", "<\\Element>\r\n");

	// Get Index
	int nIndex = GetIndex(elementData);

	// Tree에 Element Add
	// 20081007 JS.Kim	사용자시간대 선택. <<	
	//CElement *pElement = MakeElement(elementData);
	IChartManager *pIChartManager = analysisData.GetMainBlock();
	IBlock *pIBlock = pIChartManager->GetBlock( blockIndex.GetRow(), blockIndex.GetColumn());
	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 현재 블럭의 날자 패킷을 찾는다.
	IPacket *pIPacket = pIBlock->GetHorzScaleDataPacket();
	//	IPacket *pIPacket = pIBlock->GetDatePacket();

	CElement *pElement = MakeElement(elementData, pIPacket->GetBaseTimeDifference());

	pIPacket->Release();
	pIBlock->Release();	
	pIChartManager->Release();	
	//>>

	analysisData.RegisterElementToOrderManager((long)pElement);
	CElementAndOrderSet elementAndOrderSet;
	AddElement(blockIndex, nIndex,pElement,elementTree, elementAndOrderSet);

	// Order에 Element Add
	BuildElementsFromString(analysisData,elementAndOrderSet, elementOrder);

	return (long)pElement;
}

// private ====================================================================
// element 만들기.
// --> CTree
bool CElementTreeBuilder::BuildElementsFromString(CAnalysisData& analysisData,const CString& strElementsData, CTree& elementTree, CElementAndOrderSet& elementAndOrderSet)
{
	int nTotalCount = CDataConversion::GetStringDataCount(strElementsData, "<Column>\r\n");
	CString columnsData = strElementsData;
	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
		CString columnData = GetTrimData(columnsData, "<\\Column>\r\n");
		BuildElementsFromString_Column(analysisData,nIndex, columnData, elementTree, elementAndOrderSet);
	}

	return (nTotalCount > 0);
}

void CElementTreeBuilder::BuildElementsFromString_Column(CAnalysisData& analysisData,const int nColumnIndex, const CString& strColumnData, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet)
{
	int nTotalCount = CDataConversion::GetStringDataCount(strColumnData, "<Row>\r\n");
	CString rowsData = GetData(strColumnData, "<Column>\r\n", "<\\Column>\r\n");
	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
		CString rowData = GetTrimData(rowsData, "<\\Row>\r\n");
		BuildElementsFromString_Row(analysisData,CBlockIndex(nIndex, nColumnIndex), rowData, elementTree, elementAndOrderSet);
	}
}

void CElementTreeBuilder::BuildElementsFromString_Row(CAnalysisData& analysisData,const CBlockIndex& blockIndex, const CString& strRowData,
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet)
{
	int nTotalCount = CDataConversion::GetStringDataCount(strRowData, "<Element>\r\n");
	CString elementsData = GetData(strRowData, "<Row>\r\n", "<\\Row>\r\n");
	for(int nIndex = 0; nIndex < nTotalCount; nIndex++)
	{
		CString elementData = GetTrimData(elementsData, "<\\Element>\r\n");
		BuildElementsFromString_Element(analysisData,blockIndex, elementData, elementTree, elementAndOrderSet);
	}
}

void CElementTreeBuilder::BuildElementsFromString_Element(CAnalysisData& analysisData,const CBlockIndex& blockIndex, const CString& strElementData, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet)
{
	if(strElementData.IsEmpty())
		return;

	CString elementData = GetData(strElementData, "<Element>\r\n", "<\\Element>\r\n");
	// index
	int nIndex = GetIndex(elementData);

	// 20081007 JS.Kim	사용자시간대 선택. <<	
	//CElement *pElement = MakeElement(elementData);
	int nTimeDiff = 0;
	IChartManager *pIChartManager = analysisData.GetMainBlock();
	IBlock *pIBlock = pIChartManager->GetBlock( blockIndex.GetRow(), blockIndex.GetColumn());
	IPacket *pIPacket = NULL;
	if( pIBlock )
	{
		// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리. 현재 블럭의 날자 패킷을 찾는다.
		pIPacket = pIBlock->GetHorzScaleDataPacket();
		// pIPacket = pIBlock->GetDatePacket();
		if( pIPacket) nTimeDiff = pIPacket->GetBaseTimeDifference();
	}

	CElement *pElement = MakeElement(elementData, nTimeDiff);

	if( pIPacket )		pIPacket->Release();
	if( pIBlock	)		pIBlock->Release();	
	pIChartManager->Release();	
	//>>

	analysisData.RegisterElementToOrderManager((long)pElement);
	// element 추가.
	AddElement(blockIndex, nIndex,pElement,elementTree, elementAndOrderSet);
	
	// Undo / Redo
	analysisData.GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)pElement, "", strElementData);
}

// ----------------------------------------------------------------------------
// element 만들기.
// --> COrder
void CElementTreeBuilder::BuildElementsFromString(CAnalysisData& analysisData,const CElementAndOrderSet& elementAndOrderSet, COrder& elementOrder)
{
	for(int nIndex = 0; nIndex <= elementAndOrderSet.GetMaxIndex(); nIndex++)
	{
		CElement* pElement = elementAndOrderSet.GetElement(nIndex);
		if( pElement) elementOrder.AddTail(pElement);
	}
}

// ----------------------------------------------------------------------------
// element 추가.
void CElementTreeBuilder::AddElement(const CBlockIndex& blockIndex, const int nIndex, CElement* pElement, 
		CTree& elementTree, CElementAndOrderSet& elementAndOrderSet)
{
	// CTree에 element 추가.
	if(pElement == NULL)
		return;

	elementTree.Add(blockIndex, pElement);

	// element에 따른 index 추가.
	elementAndOrderSet.AddTail(CElementAndOrder(nIndex, pElement));
	CElementAndOrder elementAndOrder = elementAndOrderSet.GetData(0);
}

CElement* CElementTreeBuilder::MakeElement(const CString& strElementData, const int& nBlockTimeDiff)
{
	CString strData = strElementData;
	// tool type
	CAnalysis::TOOLTYPE eToolType = GetToolType(strData);
	// point
	CDequeList<CCoordinate> pointList;

	// 20081007 JS.Kim	사용자시간대 선택
	GetPoint(strData, nBlockTimeDiff, pointList);
	// environment
	CElementCommonEnvironment environment = GetEnvironment(strData);

	return MakeElement(eToolType, pointList, strData, environment);
}

CElement* CElementTreeBuilder::MakeElement(const CAnalysis::TOOLTYPE eToolType, const CDequeList<CCoordinate>& pointList, const CString& strTextPosition, const CElementCommonEnvironment& environment)
{
	CString strData = GetData(strTextPosition, "<PartEnvironment>\r\n", "<\\PartEnvironment>\r\n");

	if(strData.IsEmpty())
		return MakeElement(eToolType, pointList, NULL, environment);

	CTextShowPosition showPosition;
	if(GetTextShowPosition(strData, showPosition))
		return MakeElement(eToolType, pointList, &showPosition, environment);

	CTextHorzPosition horzPosition;
	if(GetTextHorzPosition(strData, horzPosition))
		return MakeElement(eToolType, pointList, &horzPosition, environment);

	CTextVertPosition vertPosition;
	if(GetTextVertPosition(strData, vertPosition))
		return MakeElement(eToolType, pointList, &vertPosition, environment);

	CTextHorzVertPosition horzVertPosition;
	if(GetTextHorzVertPosition(strData, horzVertPosition))
		return MakeElement(eToolType, pointList, &horzVertPosition, environment);

	return MakeElement(eToolType, pointList, NULL, environment);
}

CElement* CElementTreeBuilder::MakeElement(const CAnalysis::TOOLTYPE eToolType, const CDequeList<CCoordinate>& pointList, const CTextPosition* pTextPosition, const CElementCommonEnvironment& environment)
{
	switch(pointList.GetCount())
	{
	case 1:
		return CElementFactory::Make(eToolType,pointList.GetAt(0),pTextPosition,environment);
	case 2:
		return CElementFactory::Make(eToolType,pointList.GetAt(0),pointList.GetAt(1),pTextPosition,environment);
	case 3:
		return CElementFactory::Make(eToolType,pointList.GetAt(0),pointList.GetAt(1),pointList.GetAt(2),pTextPosition,environment);
	default:
		return CElementFactory::Make(eToolType,pointList,environment);
	}

	return NULL;
}

// ----------------------------------------------------------------------------
CString CElementTreeBuilder::GetData(const CString& strOrgData, const CString& strTrimStart, const CString& strTrimEnd) const
{
	CString strNewData = strOrgData;
	CDataConversion::GetStringData(strNewData, strTrimStart);
	return CDataConversion::GetStringData(strNewData, strTrimEnd);
}

// ----------------------------------------------------------------------------
int CElementTreeBuilder::GetIndex(const CString& strData) const
{
	return atoi(GetData(strData, "<Index>", "<\\Index>\r\n"));
}

/////
CAnalysis::TOOLTYPE CElementTreeBuilder::GetToolType(const CString& strData) const
{
	CString strToolType = GetData(strData, "<ToolType>", "<\\ToolType>\r\n");
	CToolEnumData::ElementType eElementType = (CToolEnumData::ElementType) atoi(strToolType);
	switch(eElementType)
	{
	case CToolEnumData::Cross_Line:		// 십자선(mouse move를 기준으로 한다.)
		//return CAnalysis::EN_ANALYSIS_DOWN_CROSS;
		return CAnalysis::EN_ANALYSIS_MOVE_CROSS;
	case CToolEnumData::Vert_Line:		// 수직선
		return CAnalysis::EN_ANALYSIS_VERT_LINE;
	case CToolEnumData::Horz_Line:		// 수평선
		return CAnalysis::EN_ANALYSIS_HORZ_LINE;
	case CToolEnumData::Line:			// 추세선
		return CAnalysis::EN_ANALYSIS_LINE;
	case CToolEnumData::Trisection:		// 삼등분선
		return CAnalysis::EN_ANALYSIS_TRISECT;
	case CToolEnumData::Quadrisect:		// 사등분선
		return CAnalysis::EN_ANALYSIS_QUARTER;
	case CToolEnumData::Fibo_Semicircle:// 피보 반원
		return CAnalysis::EN_ANALYSIS_FIBO_SEMICIRCLE;
	case CToolEnumData::Fibo_Circle:	// 피보 원
		return CAnalysis::EN_ANALYSIS_FIBO_CIRCLE;
	case CToolEnumData::Fibo_Fan_Line:	// 피보 펜라인
		return CAnalysis::EN_ANALYSIS_FIBO_FAN_LINE;
	case CToolEnumData::Fibo_Retrace:	// 피보 되돌림(조정대)
		return CAnalysis::EN_ANALYSIS_FIBO_RETRACE;
	case CToolEnumData::Fibo_Time:		// 피보 시간대
		return CAnalysis::EN_ANALYSIS_FIBO_TIME;
	case CToolEnumData::Gann_Line1:		// 갠라인(우상45도를 기본으로 한다.)
		return CAnalysis::EN_ANALYSIS_GANN_LINE45;
	case CToolEnumData::Gann_Line2:		// 갠라인(우하45도를 기본으로 한다.)
		return CAnalysis::EN_ANALYSIS_GANN_LINE315;
	case CToolEnumData::Gann_Fan1:		// 갠펜(우상45도를 기본으로 한다.)
		return CAnalysis::EN_ANALYSIS_GANN_FAN45;
	case CToolEnumData::Gann_Fan2:		// 갠펜(좌상45도를 기본으로 한다.)
		return CAnalysis::EN_ANALYSIS_GANN_FAN135;
	case CToolEnumData::Gann_Fan3:		// 갠펜(좌하45도를 기본으로 한다.)
		return CAnalysis::EN_ANALYSIS_GANN_FAN225;
	case CToolEnumData::Gann_Fan4:		// 갠펜(우하45도를 기본으로 한다.)
		return CAnalysis::EN_ANALYSIS_GANN_FAN315;
	case CToolEnumData::Gann_Grid:		// 갠그리드
		return CAnalysis::EN_ANALYSIS_GANN_GRID;
	case CToolEnumData::AndrewsPitchfork:// 엔드류 피치포크
		return CAnalysis::EN_ANALYSIS_ANDREWS_PITCHFORK;
	case CToolEnumData::ElliotWave_Line:// 엘리오트 파동선
		return CAnalysis::EN_ANALYSIS_ELLIOT_WAVE_LINE;
	case CToolEnumData::Cycle_Line:		// 사이클 구간
		return CAnalysis::EN_ANALYSIS_CYCLE_LINES;
	case CToolEnumData::Cut_Line:		// 꺽은선 분석라인
		return CAnalysis::EN_ANALYSIS_CUT_LINE;
	case CToolEnumData::Speed_Line:		// 스피드 라인
		return CAnalysis::EN_ANALYSIS_SPEED_LINE;
	case CToolEnumData::Speed_Fan:		// 스피드 FAN
		return CAnalysis::EN_ANALYSIS_SPEED_FAN;
	case CToolEnumData::Speed_Arc:			// 스피드 호
		return CAnalysis::EN_ANALYSIS_SPEED_ARC;		
	case CToolEnumData::Linear_Regression:// 선형 회귀선(회귀선)
		return CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION;
	case CToolEnumData::Raff_Regression:	// Raff 채널선(회귀채널)
		return CAnalysis::EN_ANALYSIS_RAFF_REGRESSION;
	case CToolEnumData::Angle_Line:			// 각도추세선
		return CAnalysis::EN_ANALYSIS_ANGLE_LINE;
	case CToolEnumData::Candle_Line:			// 봉중심선추세선
		return CAnalysis::EN_ANALYSIS_CANDLE_LINE;
	case CToolEnumData::Candle_Bong:			// 봉가상
		return CAnalysis::EN_ANALYSIS_CANDLE_BONG;
	case CToolEnumData::Adjust_Ratio:			// 조정백분율
		return CAnalysis::EN_ANALYSIS_ADJUST_RATIO;
	case CToolEnumData::Period_Summary:			// 기간요약
		return CAnalysis::EN_ANALYSIS_PERIOD_SUMMARY;
	case CToolEnumData::Tracing_ToolTip:		// 자동툴팁
		return CAnalysis::EN_ANALYSIS_TRACING_TT;
	case CToolEnumData::Pencil_Line:			// 연필추세선
		return CAnalysis::EN_ANALYSIS_PENCIL_LINE;
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 >>
	case CToolEnumData::Balance_Basic:// 일목수치
		return CAnalysis::EN_ANALYSIS_BALANCE_BASIC;
	// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청 <<
	};
		
	return CAnalysis::EN_ANALYSIS_NONE;
}

/////
void CElementTreeBuilder::GetPoint(const CString& strData, const int& nBlockTimeDiff, CDequeList<CCoordinate>& pointList) const
{
	CString strPoints = GetData(strData, "<Points>\r\n", "<\\Points>\r\n");
	while(!strPoints.IsEmpty())
	{
		CString strPoint = GetTrimData(strPoints, "<\\Point>\r\n");
		if(strPoint.IsEmpty())
			return;
		// 20081007 JS.Kim	사용자시간대 선택
		pointList.AddTail(GetPoint(strPoint, nBlockTimeDiff));
	}
}

CCoordinate CElementTreeBuilder::GetPoint(const CString& strData, const int& nBlockTimeDiff) const
{
	// 분차트 근접시간 적용 - ojtaso (20080811)
	CString strType = GetData(strData, "<Point>", "<\\Point>\r\n");
	CString strX = CDataConversion::GetStringData(strType, ",");
	CString strY = CDataConversion::GetStringData(strType, ",");

	// 20081007 JS.Kim	사용자시간대 선택
	CString strTimeDiff = CDataConversion::GetStringData(strType, ",");

	// 분차트 근접시간 적용 - ojtaso (20080811)
	// 20081007 JS.Kim	사용자시간대 선택 <<
	//return CCoordinate(atof(strX), atof(strY), strType, 0);
	double dX = atof(strX);

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	// - 날짜시간 패킷이 14Byte 형식이 아닌 경우 비교 처리
	// 1. YYYYMMDD 형식 : 시분초를 0값으로 초기화
	// 2. YYYYMM   형식 : 일시분초를 1일 0시 0분 0초 로 초기화
	// 3. MMDDHHMM 형식 : 로컬PC 년도 및 00 초로 초기화
	// 	  단, 분석툴에 저장된 날짜정보가 로컬PC의 년월일보다 이후 날짜면... 
	// 	  로컬PC의 년정보를 -1 해준다.
	// 4. DDHHMMSS 형식 : 로컬PC 년/월로 초기화
	// 	  단, 분석툴에 저장된 날짜정보가 로컬PC의 년월일보다 이후 날짜면... 
	// 	  로컬PC의 월정보을 -1 해준다.
	if (strType != _T("YYYYMMDDHHMMSS"))
	{
		// 14Byte 형식이 아니라면 기존 분석툴 파일 형식이라고 본다.
		// 기존 저장파일과의 호환을 위해 수정 필요.
		
		CTime timeCur = CTime::GetCurrentTime();
		CString strCurYear		= timeCur.Format("%Y");
		CString strCurMon		= timeCur.Format("%m");
		CString strCurYearMon	= timeCur.Format("%Y%m");
		CString strCurDate		= timeCur.Format("%Y%m%d");
		CString strCurFull		= timeCur.Format("%Y%m%d%H%M%S");

		CString strMakeTime = _T("");

		// 날짜시간 타입별로 14 Byte 형식으로 만든다.
		if (strType == _T("YYYYMMDD"))
		{
			// 일간 또는 주간인 경우 : 0시 0분 0초를 덧붙인다.
			if( strX.GetLength() == strType.GetLength() )
				strMakeTime = strX + _T("000000");
			else
				strMakeTime = strX; 
		}
		else if (strType == _T("YYYYMM"))
		{
			// 월간인 경우 : 1일 0시 0분 0초를 덧붙인다.
			if( strX.GetLength() == strType.GetLength() )
				strMakeTime = strX + _T("01000000");
			else
				strMakeTime = strX; 
		}
		else if (strType == _T("MMDDHHMM"))
		{
			// 분간인 경우 : 로컬PC년도 + 월일시분 + 0초
			
			CString strMon;
			CString strDay;

			if (strX.GetLength() == 7)
			{
				// 월Data가 %02d 형태가 아니고 한자리만 저장된 경우 (예, 5월이면 05가 아닌 5가 저장된 경우임)
				strMon.Format("%02d", atoi(strX.Left(1)));
				strDay.Format("%02d", atoi(strX.Mid(1,2)));
			}
			else
			{
				strMon.Format("%02d", atoi(strX.Left(2)));
				strDay.Format("%02d", atoi(strX.Mid(2,2)));
			}

			CString strTemp1	= strCurYear + strMon + strDay;
			CString strYear		= strCurYear;

			// 분석툴에 저장된 월일정보가 로컬PC의 년월일보다 이후 날짜면... 로컬PC년도를 -1 해준다.
			if (_atoi64(strTemp1) > _atoi64(strCurDate))
			{
				strYear.Format("%04d", atoi(strCurYear) -1);
			}

			strMakeTime = strYear + strMon + strDay + strX.Right(4) + _T("00");
		}
		else if (strType == _T("DDHHMMSS"))
		{
			// 틱/초인 경우 : 로컬PC년월 + 일시분초

			CString strDay;

			if (strX.GetLength() == 7)
				strDay.Format("%02d", atoi(strX.Left(1)));	// 일data가 %02d 형태가 아니고 한자리만 저장된 경우 (예, 5일이면 05가 아닌 5가 저장된 경우임)
			else
				strDay.Format("%02d", atoi(strX.Left(2)));

			CString strTemp1	= strCurYearMon + strDay;
			CString strYearMon	= strCurYearMon;
			
			// 분석툴에 저장된 일정보가 로컬PC의 년월일보다 이후 날짜면... 로컬PC 월정보를 -1 해준다.
			if (_atoi64(strTemp1) > _atoi64(strCurDate))
			{
				int nYear	= atoi(strCurYear);
				int nMon	= atoi(strCurMon);
				if (nMon == 1)
				{
					// 로컬pc날짜가 1월인 경우...  년도를 -1 하고 월은 12월로 처리...
					nMon = 12;
					nYear--;
				}
				else
				{
					nMon--;
				}
				strYearMon.Format("%02d%02d", nYear, nMon);
			}

			strMakeTime = strYearMon + strDay + strX.Right(6);
		}
		else
		{
			strMakeTime = strCurFull;
		}

		//
		__int64 lMakeTime = _atoi64(strMakeTime);
		dX = lMakeTime;
		strType = _T("YYYYMMDDHHMMSS");
	}
	// 2011.02.07 by SYS <<


	int nTimeDiff = atoi(strTimeDiff);
	if( nBlockTimeDiff != nTimeDiff )		
		dX = CDataConversion::CalcTimeDiff(dX, nBlockTimeDiff - nTimeDiff, nBlockTimeDiff, strType);

	return CCoordinate(dX, atof(strY), nBlockTimeDiff, strType, 0);
	//>>
}

/////
// 2008.08.20 김진순 피보나치 들의 Text 보임 옵션 버그 수정 <<
bool CElementTreeBuilder::GetTextShowPosition(const CString& strData, CTextShowPosition& position) const
{
	//color
	CString data = GetData(strData, "<Horz>\r\n", "<\\Horz>\r\n");
	if(data.IsEmpty())
		return false;
	
	CString show = GetData(data, "<Show>", "<\\Show>\r\n");
	if(show.IsEmpty())
		return false;

	position.SetTextEnvironment(GetTextEnvironment(strData));
	position.SetShow(CDataConversion::atob(show));
	return true;
}
//>>

bool CElementTreeBuilder::GetTextHorzPosition(const CString& strData, CTextHorzPosition& position) const
{
	//color
	CString data = GetData(strData, "<Horz>\r\n", "<\\Horz>\r\n");
	if(data.IsEmpty())
		return false;

	position.SetTextEnvironment(GetTextEnvironment(strData));
	position.SetLeft(CDataConversion::atob(GetData(data, "<Left>", "<\\Left>\r\n")));
	position.SetRight(CDataConversion::atob(GetData(data, "<Right>", "<\\Right>\r\n")));
	position.SetRightRate(CDataConversion::atob(GetData(data, "<Rate>", "<\\Rate>\r\n")));
	return true;
}

bool CElementTreeBuilder::GetTextVertPosition(const CString& strData, CTextVertPosition& position) const
{
	CString data = GetData(strData, "<Vert>\r\n", "<\\Vert>\r\n");
	if(data.IsEmpty())
		return false;

	position.SetTextEnvironment(GetTextEnvironment(strData));
	position.SetTop(CDataConversion::atob(GetData(strData, "<Top>", "<\\Top>\r\n")));
	position.SetBottom(CDataConversion::atob(GetData(strData, "<Bottom>", "<\\Bottom>\r\n")));
	return true;
}

bool CElementTreeBuilder::GetTextHorzVertPosition(const CString& strData, CTextHorzVertPosition& position) const
{
	CString data = GetData(strData, "<HorzVert>\r\n", "<\\HorzVert>\r\n");
	if(data.IsEmpty())
		return false;

	position.SetTextEnvironment(GetTextEnvironment(strData));
	position.SetLeft(CDataConversion::atob(GetData(data, "<Left>", "<\\Left>\r\n")));
	position.SetRight(CDataConversion::atob(GetData(data, "<Right>", "<\\Right>\r\n")));
	position.SetTop(CDataConversion::atob(GetData(strData, "<Top>", "<\\Top>\r\n")));
	position.SetBottom(CDataConversion::atob(GetData(strData, "<Bottom>", "<\\Bottom>\r\n")));
	return true;
}

CTextEnvironment CElementTreeBuilder::GetTextEnvironment(const CString& strData) const
{
	CString data = GetData(strData, "<TextEnvironment>\r\n", "<\\TextEnvironment>\r\n");
	if(data.IsEmpty())
		return CTextEnvironment();

	return CTextEnvironment(GetColor(data));
}

/////
CElementCommonEnvironment CElementTreeBuilder::GetEnvironment(const CString& strData) const
{
	CString strEnvironment = GetData(strData, "<CommonEnvironment>\r\n", "<\\CommonEnvironment>\r\n");
	bool bIsAutoPrice = GetAutoPrice(strEnvironment);
	int nEquiDiv = GetEquiDivEnvironment(strEnvironment);
	int nPriceRangeDiv = GetPriceRangeDivEnvironment(strEnvironment);	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	CLineExtEnvironment lineExt = GetLineExtEnvironment(strEnvironment);
	CPenEnvironment pen = GetPenEnvironment(strEnvironment);	
	CDataTypeEnvironment dataType = GetDataTypeEnvironment(strEnvironment);
	return CElementCommonEnvironment(bIsAutoPrice,nEquiDiv,nPriceRangeDiv,pen, dataType,lineExt);
}

//
bool CElementTreeBuilder::GetAutoPrice(const CString& strData) const
{
	int nData = atoi(GetData(strData, "<AutoPirce>", "<\\AutoPirce>\r\n"));
	return (nData == 0 ? false: true);
}

//
int CElementTreeBuilder::GetEquiDivEnvironment(const CString& strData) const
{
	int nData = atoi(GetData(strData, "<EquiDiv>", "<\\EquiDiv>\r\n"));
	return nData;
}

// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
int CElementTreeBuilder::GetPriceRangeDivEnvironment(const CString& strData) const
{
	int nData = atoi(GetData(strData, "<PriceRangeDiv>", "<\\PriceRangeDiv>\r\n"));
	return nData;
}

//
CLineExtEnvironment CElementTreeBuilder::GetLineExtEnvironment(const CString& strData) const
{
	CString strLineExtData = GetData(strData, "<LineExt>\r\n", "<\\LineExt>\r\n");
	int nLeftExt = GetLeftLineExt(strLineExtData);
	int nRightExt = GetRightLineExt(strLineExtData);
	return CLineExtEnvironment(nLeftExt,nRightExt);
}

int CElementTreeBuilder::GetLeftLineExt(const CString& strData) const
{
	return atoi(GetData(strData, "<LeftExt>", "<\\LeftExt>\r\n"));
}

int CElementTreeBuilder::GetRightLineExt(const CString& strData) const
{
	return atoi(GetData(strData, "<RightExt>", "<\\RightExt>\r\n"));
}

//
CPenEnvironment CElementTreeBuilder::GetPenEnvironment(const CString& strData) const
{
	CString strPenData = GetData(strData, "<Pen>\r\n", "<\\Pen>\r\n");
	COLORREF color = GetColor(strPenData);
	int nWidth = GetWidth(strPenData);
	CToolEnumData::PenStyle eStyle = GetStyle(strPenData);
	return CPenEnvironment(color, nWidth, eStyle);
}

COLORREF CElementTreeBuilder::GetColor(const CString& strData) const
{
	return CDataConversion::Str2RGB(GetData(strData, "<Color>", "<\\Color>\r\n"));
}

int CElementTreeBuilder::GetWidth(const CString& strData) const
{
	return atoi(GetData(strData, "<Width>", "<\\Width>\r\n"));
}

CToolEnumData::PenStyle CElementTreeBuilder::GetStyle(const CString& strData) const
{
	return (CToolEnumData::PenStyle) atoi(GetData(strData, "<Style>", "<\\Style>\r\n"));
}

//
CDataTypeEnvironment CElementTreeBuilder::GetDataTypeEnvironment(const CString& strData) const
{
	CString strDataTypeData = GetData(strData, "<DataType>\r\n", "<\\DataType>\r\n");
	CAnalysis::COORDINATETYPE eCoordinateType = GetCoordinateType(strDataTypeData);
	double dVert = GetVertDataType(strDataTypeData);
	CString strHorz = GetData(strDataTypeData, "<Horz>", "<\\Horz>\r\n");
	// 2008.08.20 김진순 피보나치 확장<<
	//return CDataTypeEnvironment(eCoordinateType, dVert, strHorz);
	CString strExtData = GetData(strDataTypeData, "<ExtData>", "<\\ExtData>\r\n");
	// 2008.10.15 by LYH >> 8,32bit지원
	CString strStrVert = GetData(strDataTypeData, "<StrVert>", "<\\StrVert>\r\n");
	return CDataTypeEnvironment(eCoordinateType, dVert, strHorz, strExtData, strStrVert, m_hOcxWnd, m_eChartMode);
	//>>
}

CAnalysis::COORDINATETYPE CElementTreeBuilder::GetCoordinateType(const CString& strData) const
{
	return ((CAnalysis::COORDINATETYPE) atoi(GetData(strData, "<Coordinate>", "<\\Coordinate>\r\n")));
}

double CElementTreeBuilder::GetVertDataType(const CString& strData) const
{
	return atof(GetData(strData, "<Vert>", "<\\Vert>\r\n"));
}

// ----------------------------------------------------------------------------
CString CElementTreeBuilder::GetTrimData(CString& strData, const CString& strTrimData) const
{
	if(strTrimData.IsEmpty())
		return strData;

	return CDataConversion::GetStringData(strData, strTrimData, false);
}