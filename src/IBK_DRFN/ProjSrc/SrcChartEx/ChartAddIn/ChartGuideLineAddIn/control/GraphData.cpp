// GraphData.cpp: implementation of the CGraphData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphData.h"

#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CPenThickness

CPenThickness& CPenThickness::operator=(const CPenThickness& data)
{
	if(this == &data)
		return *this;

	m_nThickness1 = data.GetThickness1();
	m_nThickness2 = data.GetThickness2();
	m_nThickness3 = data.GetThickness3();
	m_bIsHightlight = data.IsHightlight();
	return *this;
}

bool CPenThickness::operator==(const CPenThickness& data) const
{
	if(m_nThickness1 != data.GetThickness1())
		return false;
	if(m_nThickness2 != data.GetThickness2())
		return false;
	if(m_nThickness3 != data.GetThickness3())
		return false;
	return (m_bIsHightlight == data.IsHightlight());
}

CPenThickness::CPenThickness():
	m_nThickness1(1),
	m_nThickness2(1),
	m_nThickness3(1),
	m_bIsHightlight(false)
{
}

CPenThickness::CPenThickness(const CString& strOnlyThicknessData):
	m_nThickness1(1),
	m_nThickness2(1),
	m_nThickness3(1),
	m_bIsHightlight(false)
{
	if(strOnlyThicknessData.IsEmpty())
		return;

	SetData(strOnlyThicknessData);
}

CPenThickness::CPenThickness(const int nThickness1, const int nThickness2, const int nThickness3, const bool bIsHightlight):
	m_nThickness1(nThickness1),
	m_nThickness2(nThickness2),
	m_nThickness3(nThickness3),
	m_bIsHightlight(bIsHightlight)
{
}

CPenThickness::CPenThickness(const CPenThickness& data):
	m_nThickness1(data.GetThickness1()),
	m_nThickness2(data.GetThickness2()),
	m_nThickness3(data.GetThickness3()),
	m_bIsHightlight(data.IsHightlight())
{
}

// public =====================================================================
void CPenThickness::SetData(const CString& strData)
{
	if(strData.IsEmpty())
		return;

	// 선 굵기
	CString strRealData = strData;
	if(strRealData.Right(1) != ';')
		strRealData += ';';

	SetIntData(CDataConversion::GetStringData(strRealData, ";"), m_nThickness1);
	SetIntData(CDataConversion::GetStringData(strRealData, ";"), m_nThickness2);
	SetIntData(CDataConversion::GetStringData(strRealData, ";"), m_nThickness3);

	// 강조
	m_bIsHightlight = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ";"));
}

void CPenThickness::SetHightlight(const bool bIsHightlight)
{
	m_bIsHightlight = bIsHightlight;
}

void CPenThickness::SetThickness(const int nThickness1, const int nThickness2, const int nThickness3)
{
	m_nThickness1 = nThickness1;
	m_nThickness2 = nThickness2;
	m_nThickness3 = nThickness3;
}

void CPenThickness::SetThickness1(const int nThickness)
{
	m_nThickness1 = nThickness;
}

void CPenThickness::SetThickness2(const int nThickness)
{
	m_nThickness2 = nThickness;
}

void CPenThickness::SetThickness3(const int nThickness)
{
	m_nThickness3 = nThickness;
}

void CPenThickness::ReversedHightlight()
{
	m_bIsHightlight = !m_bIsHightlight;
}

// ----------------------------------------------------------------------------
bool CPenThickness::IsHightlight() const
{
	return m_bIsHightlight;
}

int CPenThickness::GetThickness1() const
{
	return m_nThickness1;
}

int CPenThickness::GetThickness2() const
{
	return m_nThickness2;
}

int CPenThickness::GetThickness3() const
{
	return m_nThickness3;
}

int CPenThickness::GetDrawingThickness1() const
{
	return GetDrawingThickness(m_nThickness1);
}

int CPenThickness::GetDrawingThickness2() const
{
	return GetDrawingThickness(m_nThickness2);
}

int CPenThickness::GetDrawingThickness3() const
{
	return GetDrawingThickness(m_nThickness3);
}

CString CPenThickness::GetStringData() const
{
	CString strPenThickness;
	// 선 굵기
	strPenThickness += CDataConversion::IntToString(m_nThickness1) + ';';
	strPenThickness += CDataConversion::IntToString(m_nThickness2) + ';';
	strPenThickness += CDataConversion::IntToString(m_nThickness3) + ';';
	// 강조
	strPenThickness += CDataConversion::Bool2Str(m_bIsHightlight) + ';';

	return strPenThickness;
}


// private ====================================================================
void CPenThickness::SetIntData(const CString& strData, int& nData)
{
	if(strData.IsEmpty())
		return;
	nData = atoi(strData);
}

int CPenThickness::GetDrawingThickness(const int orgPenThickness) const
{
	if(!m_bIsHightlight)
		return orgPenThickness;

	if(orgPenThickness < 3)
		return orgPenThickness *2;
	return orgPenThickness +2;
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphDataDrawingData

CGraphDataDrawingData& CGraphDataDrawingData::operator=(const CGraphDataDrawingData& data)
{
	if(this == &data)
		return *this;

	m_bCalcRiseFallRate_LowHigh = data.DoesCalcRiseFallRate_LowHigh();
	m_bCalcRiseFallRate_LowCur = data.DoesCalcRiseFallRate_LowCur();
	m_bCalcRiseFallRate_HighCur = data.DoesCalcRiseFallRate_HighCur();
	m_bCalcRiseFallRate_PrevCur = data.DoesCalcRiseFallRate_PrevCur();
	m_bCalcRate_PrevCur = data.DoesCalcRate_PrevCur();
	return *this;
}

CGraphDataDrawingData::CGraphDataDrawingData():
	m_bCalcRiseFallRate_LowHigh(false),
	m_bCalcRiseFallRate_LowCur(false),
	m_bCalcRiseFallRate_HighCur(false),
	m_bCalcRiseFallRate_PrevCur(false),
	m_bCalcRate_PrevCur(false)
{
}

CGraphDataDrawingData::CGraphDataDrawingData(const CString& strData):
	m_bCalcRiseFallRate_LowHigh(false),
	m_bCalcRiseFallRate_LowCur(false),
	m_bCalcRiseFallRate_HighCur(false),
	m_bCalcRiseFallRate_PrevCur(false),
	m_bCalcRate_PrevCur(false)
{
	SetData(strData);
}

CGraphDataDrawingData::CGraphDataDrawingData(const bool bCalcRiseFallRate_LowHigh, const bool bCalcRiseFallRate_LowCur, 
	const bool bCalcRiseFallRate_HighCur, const bool bCalcRiseFallRate_PrevCur, const bool bCalcRate_PrevCur):
	m_bCalcRiseFallRate_LowHigh(bCalcRiseFallRate_LowHigh),
	m_bCalcRiseFallRate_LowCur(bCalcRiseFallRate_LowCur),
	m_bCalcRiseFallRate_HighCur(bCalcRiseFallRate_HighCur),
	m_bCalcRiseFallRate_PrevCur(bCalcRiseFallRate_PrevCur),
	m_bCalcRate_PrevCur(bCalcRate_PrevCur)
{
}

CGraphDataDrawingData::CGraphDataDrawingData(const CGraphDataDrawingData& data):
	m_bCalcRiseFallRate_LowHigh(data.DoesCalcRiseFallRate_LowHigh()),
	m_bCalcRiseFallRate_LowCur(data.DoesCalcRiseFallRate_LowCur()),
	m_bCalcRiseFallRate_HighCur(data.DoesCalcRiseFallRate_HighCur()),
	m_bCalcRiseFallRate_PrevCur(data.DoesCalcRiseFallRate_PrevCur()),
	m_bCalcRate_PrevCur(data.DoesCalcRate_PrevCur())
{
}


// public =====================================================================
void CGraphDataDrawingData::SetDoesCalcRiseFallRate_LowHigh(const bool bCalcData)
{
	m_bCalcRiseFallRate_LowHigh = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRiseFallRate_LowCur(const bool bCalcData)
{
	m_bCalcRiseFallRate_LowCur = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRiseFallRate_HighCur(const bool bCalcData)
{
	m_bCalcRiseFallRate_HighCur = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRiseFallRate_PrevCur(const bool bCalcData)
{
	m_bCalcRiseFallRate_PrevCur = bCalcData;
}

void CGraphDataDrawingData::SetDoesCalcRate_PrevCur(const bool bCalcData)
{
	m_bCalcRate_PrevCur = bCalcData;
}

// ----------------------------------------------------------------------------
bool CGraphDataDrawingData::DoesAllFalseData() const
{
	return (!m_bCalcRiseFallRate_LowHigh && !m_bCalcRiseFallRate_LowCur &&
		!m_bCalcRiseFallRate_HighCur && !m_bCalcRiseFallRate_PrevCur && 
		!m_bCalcRate_PrevCur);
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_LowHigh() const
{
	return m_bCalcRiseFallRate_LowHigh;
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_LowCur() const
{
	return m_bCalcRiseFallRate_LowCur;
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_HighCur() const
{
	return m_bCalcRiseFallRate_HighCur;
}

bool CGraphDataDrawingData::DoesCalcRiseFallRate_PrevCur() const
{
	return m_bCalcRiseFallRate_PrevCur;
}

bool CGraphDataDrawingData::DoesCalcRate_PrevCur() const
{
	return m_bCalcRate_PrevCur;
}

// graph data 가져오기
CString CGraphDataDrawingData::GetData() const
{
	CString strDataView;
	strDataView.Format("%d;%d;%d;%d;%d;", (int)m_bCalcRiseFallRate_LowHigh, 
		(int)m_bCalcRiseFallRate_LowCur, (int)m_bCalcRiseFallRate_HighCur, 
		(int)m_bCalcRiseFallRate_PrevCur, (int)m_bCalcRate_PrevCur);
	return strDataView;
}


// private ====================================================================
void CGraphDataDrawingData::SetData(const CString& strData)
{
	if(strData.IsEmpty())
		return;

	CString strRealData = strData;
	if(strRealData.Right(1) != ';')
		strRealData += ";";

	m_bCalcRiseFallRate_LowHigh = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ';'));
	m_bCalcRiseFallRate_LowCur = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ';'));
	m_bCalcRiseFallRate_HighCur = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ';'));
	m_bCalcRiseFallRate_PrevCur = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ';'));
	m_bCalcRate_PrevCur = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ';'));
}


///////////////////////////////////////////////////////////////////////////////
// class CGraphData

CGraphData& CGraphData::operator=(const CGraphData& data)
{
	if(this == &data)
		return *this;

	m_nFunctionIndex = data.GetFunctionIndex();
	m_strFunctionName = data.GetFunctionName();
	m_strPacketName = data.GetPacketNames();
	m_bSignal = data.IsSignal();
	m_drawingData = data.GetGraphDataDrawingData();
	return *this;
}

CGraphData::CGraphData():
	m_nFunctionIndex(0),
	m_strFunctionName(""),
	m_strPacketName(""),
	m_bSignal(false)
{
}

CGraphData::CGraphData(const CString& strData):
	m_nFunctionIndex(0),
	m_bSignal(false)
{
	SetData(strData);
}

CGraphData::CGraphData(const int nFunctionIndex, const CString& functionName, const CString& packetNames, 
		const bool bSignal, const CGraphDataDrawingData& drawingData):
	m_nFunctionIndex(nFunctionIndex),
	m_strFunctionName(functionName),
	m_strPacketName(packetNames),
	m_bSignal(bSignal),
	m_drawingData(drawingData)
{
}

CGraphData::CGraphData(const CGraphData& data):
	m_nFunctionIndex(data.GetFunctionIndex()),
	m_strFunctionName(data.GetFunctionName()),
	m_strPacketName(data.GetPacketNames()),
	m_bSignal(data.IsSignal()),
	m_drawingData(data.GetGraphDataDrawingData())
{
}

// public =====================================================================
void CGraphData::SetFunctionIndex(const int nFunctionIndex)
{
	if(m_nFunctionIndex == nFunctionIndex)
		return;

	m_nFunctionIndex = nFunctionIndex;
}

void CGraphData::SetFunctionName(const CString& functionName)
{
	if(m_strFunctionName == functionName)
		return;

	m_strFunctionName = functionName;
}

void CGraphData::SetPacketNames(const CString& packetNames)
{
	if(m_strPacketName == packetNames)
		return;

	m_strPacketName = packetNames;
}

void CGraphData::SetSignal(const bool bSignal)
{
	m_bSignal = bSignal;
}

void CGraphData::SetGraphDataDrawingData(const CGraphDataDrawingData& drawingData)
{
	m_drawingData = drawingData;
}

// ----------------------------------------------------------------------------
int CGraphData::GetFunctionIndex() const
{
	return m_nFunctionIndex;
}

CString CGraphData::GetFunctionName() const
{
	return m_strFunctionName;
}

CString CGraphData::GetPacketNames() const
{
	return m_strPacketName;
}

bool CGraphData::IsSignal() const
{
	return m_bSignal;
}

CGraphDataDrawingData CGraphData::GetGraphDataDrawingData() const
{
	return m_drawingData;
}

// graph data 가져오기
CString CGraphData::GetData() const
{
	CString strGraphData;
	strGraphData.Format("%d;%s;%s;%d;", m_nFunctionIndex, m_strFunctionName, m_strPacketName, (int)m_bSignal);
	return strGraphData + m_drawingData.GetData() + "\r\n";
}

// private ====================================================================
void CGraphData::SetData(const CString& strData)
{
	if(strData.IsEmpty())
		return;

	// "index;가격차트;시가,고가,저자,종가;signal여부;
	//  최저가최고가등락률;최저가현재가등락률;최고가현재가등락률;전일가현재가등락률;전일비;\r\n"
	CString strRealData = strData;
	m_nFunctionIndex = atoi(CDataConversion::GetStringData(strRealData, ";")); // function index
	m_strFunctionName = CDataConversion::GetStringData(strRealData, ";"); // graph function name
	m_strPacketName = CDataConversion::GetStringData(strRealData, ";"); // packet name
	m_bSignal = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ";"));
	m_drawingData = CGraphDataDrawingData(strRealData);
}


///////////////////////////////////////////////////////////////////////////////
// class CSubGraphData

CSubGraphData& CSubGraphData::operator=(const CSubGraphData& data)
{
	if(this == &data)
		return *this;

	m_nTitleEndPosition = data.GetTitleEndPosition();
	m_TYPEType = data.GetType();
	m_nStyle = data.GetStyle();
	m_nDrawStyle = data.GetDrawStyle();
	m_strName = data.GetName();
	m_strTitle = data.GetTitle();
	m_bHiding = data.GetHiding();
	m_clrColor = data.GetColor();
	m_penThickness = data.GetPenThickness();
	return *this;
}

CSubGraphData::CSubGraphData():
	m_nTitleEndPosition(0),
	m_TYPEType(CGraphBaseData::GTNONE),
	m_bHiding(true)
{
}

CSubGraphData::CSubGraphData(const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness):
	m_nTitleEndPosition(0),
	m_TYPEType(type),
	m_nStyle(style),
	m_nDrawStyle(drawStyle),
	m_strName(name),
	m_strTitle(title),
	m_bHiding(hiding),
	m_clrColor(color),
	m_penThickness(penThickness)
{
}

CSubGraphData::CSubGraphData(const CSubGraphData& data):
	m_nTitleEndPosition(data.GetTitleEndPosition()),
	m_TYPEType(data.GetType()),
	m_nStyle(data.GetStyle()),
	m_nDrawStyle(data.GetDrawStyle()),
	m_strName(data.GetName()),
	m_strTitle(data.GetTitle()),
	m_bHiding(data.GetHiding()),
	m_clrColor(data.GetColor()),
	m_penThickness(data.GetPenThickness())
{
}

CSubGraphData::CSubGraphData(const CString& strData):
	m_nTitleEndPosition(0),
	m_TYPEType(CGraphBaseData::GTNONE),
	m_bHiding(true)
{
	SetData(strData);
}

// ----------------------------------------------------------------------------
void CSubGraphData::SetTitleEndPosition(const int nTitleEndPosition)
{
	m_nTitleEndPosition = nTitleEndPosition;
}

void CSubGraphData::SetData(const CString& strData)
{
	if(strData.IsEmpty())
		return;

	// "그래프타입;그래프스타일;그리는스타일;이름;title;숨김;색1;색2;색3;색4;색5;색6;
	//  상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n"
	CString temp = strData;
	m_TYPEType = GetType(CDataConversion::GetStringData(temp, ";"));
	m_nStyle = (CGraphStyle) atoi(CDataConversion::GetStringData(temp, ";"));
	m_nDrawStyle = (CGraphDrawStyle) atoi(CDataConversion::GetStringData(temp, ";"));
	//testing -> 프로퍼티 수정되면 삭제하기
	if(m_TYPEType == CGraphBaseData::Bong_Type && m_nStyle.GetGraphStyle() != m_nDrawStyle.GetDrawStyle())
		m_nDrawStyle.SetDrawStyle(m_nStyle.GetGraphStyle());

	m_strName = CDataConversion::GetStringData(temp, ";");
	m_strTitle = CDataConversion::GetStringData(temp, ";");
	m_bHiding = CDataConversion::atob(CDataConversion::GetStringData(temp, ";"));
	m_clrColor.SetData(CDataConversion::GetStringData(temp, ";", 8));
	m_penThickness.SetData(temp);
}

void CSubGraphData::SetType(const CGraphBaseData::GRAPHTYPE type)
{
	m_TYPEType = type;
}

void CSubGraphData::SetStyle(const CGraphStyle& style)
{
	m_nStyle = style;
}

void CSubGraphData::SetDrawStyle(const CGraphDrawStyle& drawStyle)
{
	m_nDrawStyle = drawStyle;
}

void CSubGraphData::SetName(const CString& name)
{
	m_strName = name;
}

void CSubGraphData::SetTitle(const CString& title)
{
	m_strTitle = title;
}

void CSubGraphData::SetHiding(const bool hiding)
{
	m_bHiding = hiding;
}

void CSubGraphData::SetColor(const CGraphColor& color)
{
	m_clrColor = color;
}

void CSubGraphData::SetPenThickness(const CPenThickness& penThickness)
{
	m_penThickness = penThickness;
}

// ----------------------------------------------------------------------------
int CSubGraphData::GetTitleEndPosition() const
{
	return m_nTitleEndPosition;
}

CGraphBaseData::GRAPHTYPE CSubGraphData::GetType() const
{
	return m_TYPEType;
}

CGraphStyle CSubGraphData::GetStyle() const
{
	return m_nStyle;
}

CGraphDrawStyle CSubGraphData::GetDrawStyle() const
{
	return m_nDrawStyle;
}

CString CSubGraphData::GetName() const
{
	return m_strName;
}

CString CSubGraphData::GetTitle() const
{
	return m_strTitle;
}

bool CSubGraphData::GetHiding() const
{
	return m_bHiding;
}

CGraphColor CSubGraphData::GetColor() const
{
	return m_clrColor;
}

CGraphColor* CSubGraphData::GetColor_PointerType()
{
	return &m_clrColor;
}

CGraphColor& CSubGraphData::GetColor()
{
	return m_clrColor;
}

CPenThickness CSubGraphData::GetPenThickness() const
{
	return m_penThickness;
}

CPenThickness& CSubGraphData::GetPenThickness()
{
	return m_penThickness;
}

CString CSubGraphData::GetStringData() const
{
	CString subGraphData;
	subGraphData = CDataConversion::IntToString((int)m_TYPEType) + ';';
	subGraphData += CDataConversion::IntToString((int)m_nStyle.GetGraphStyle()) + ';';
	subGraphData += CDataConversion::IntToString((int)m_nDrawStyle.GetDrawStyle()) + ';';
	subGraphData += m_strName + ';';
	subGraphData += m_strTitle + ';';
	subGraphData += CDataConversion::IntToString((int)m_bHiding) + ';';
	subGraphData += m_clrColor.GetStringData();
	subGraphData += m_penThickness.GetStringData();
	return subGraphData + "\r\n";
}

CString CSubGraphData::GetStringData(const CString& strTitle) const
{
	CString subGraphData;
	subGraphData = CDataConversion::IntToString((int)m_TYPEType) + ';';
	subGraphData += CDataConversion::IntToString((int)m_nStyle.GetGraphStyle()) + ';';
	subGraphData += CDataConversion::IntToString((int)m_nDrawStyle.GetDrawStyle()) + ';';
	subGraphData += m_strName + ';';
	subGraphData += strTitle + ';';
	subGraphData += CDataConversion::IntToString((int)m_bHiding) + ';';
	subGraphData += m_clrColor.GetStringData();
	subGraphData += m_penThickness.GetStringData();
	return subGraphData + "\r\n";
}


// private ====================================================================
CGraphBaseData::GRAPHTYPE CSubGraphData::GetType(const CString& strType) const
{
	if(strType.IsEmpty())
		return CGraphBaseData::GTNONE;

	int nType = atoi(strType);
	for(int i = 0; i < CGraphBaseData::GraphTypeCount; i++){
		if(nType == i)
			return (CGraphBaseData::GRAPHTYPE) nType;
	}

	return CGraphBaseData::GTNONE;
}
