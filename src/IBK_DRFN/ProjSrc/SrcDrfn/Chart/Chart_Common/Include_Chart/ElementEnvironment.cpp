// ElementEnvironment.cpp: implementation of the CElementEnvironment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementEnvironment.h"

#include <assert.h>
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* ----------------------------------------------------------------------------
  ** 저장 순서 **
  <Column>
  <Row>
  <Element>
  <Index>0<\\Index>
  <ToolTyle>0<\\ToolType>
  <Points>
  <Point>0,0<\\Point>
  ...
  <\\Points>
  <CommonEnvironment>
  <AutoPirce>0<\\AutoPirce>
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
///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CPenEnvironment

CPenEnvironment& CPenEnvironment::operator=(const CPenEnvironment& environment)
{
	if(this == &environment)
		return *this;

	m_color = environment.GetColor();
	m_nWidth = environment.GetWidth();
	m_eStyle = environment.GetStyle();
	return *this;
}

// ----------------------------------------------------------------------------
CPenEnvironment::CPenEnvironment():
	m_color(RGB(0, 0, 0)),
	m_nWidth(1),
	m_eStyle(CToolEnumData::Solid)
{
}

CPenEnvironment::CPenEnvironment(const COLORREF& color, const int nWidth, const CToolEnumData::PenStyle eStyle):
	m_color(color),
	m_nWidth(nWidth),
	m_eStyle(eStyle)
{
}

CPenEnvironment::CPenEnvironment(CPenEnvironment* pEnvironment)
{
	if(pEnvironment == NULL)
		return;

	m_color = pEnvironment->GetColor();
	m_nWidth = pEnvironment->GetWidth();
	m_eStyle = pEnvironment->GetStyle();
}


// public =====================================================================
void CPenEnvironment::CreatePen(const bool bIsElement, CPen& pen)
{
	if(bIsElement)
		pen.CreatePen(GetStyle_Draw(), m_nWidth, m_color);
	else
		pen.CreatePen(PS_SOLID, 1, m_color);
}

void CPenEnvironment::Set(const CPenEnvironment& environment)
{
	m_color = environment.GetColor();
	m_nWidth = environment.GetWidth();
	m_eStyle = environment.GetStyle();
}

void CPenEnvironment::SetColor(const COLORREF& color)
{
	m_color = color;
}

void CPenEnvironment::SetWidth(const int nWidth)
{
	m_nWidth = nWidth;
}

void CPenEnvironment::SetStyle(const CToolEnumData::PenStyle eStyle)
{
	m_eStyle = eStyle;
}

// ----------------------------------------------------------------------------
COLORREF CPenEnvironment::GetColor() const
{
	return m_color;
}

int CPenEnvironment::GetWidth() const
{
	return m_nWidth;
}

CToolEnumData::PenStyle CPenEnvironment::GetStyle() const
{
	return m_eStyle;
}

// <Pen>
// <Color>0,0,0<\\Color>
// <Width>1<\\Width>
// <Style>0<\\Style>
// <\\Pen>
CString CPenEnvironment::GetSavedData() const
{
	CString strSaved;
	strSaved.Format("<Pen>\r\n%s%s%s<\\Pen>\r\n", 
		GetSavedColor(), GetSavedWidth(), GetSavedStyle());
	return strSaved;
}


// private ====================================================================
int CPenEnvironment::GetStyle_Draw() const
{
	switch(m_eStyle){
	case CToolEnumData::Solid:
		return PS_SOLID;
	case CToolEnumData::Dot:
		return PS_DOT;
	case CToolEnumData::Dash:
		return PS_DASH;
	case CToolEnumData::DashDot:
		return PS_DASHDOT;
	case CToolEnumData::DashDotDot:
		return PS_DASHDOTDOT;
	}
	assert(false);

	return PS_SOLID;
}

CString CPenEnvironment::GetSavedColor() const
{
	return ("<Color>" + CDataConversion::RGB2Str(m_color) + "<\\Color>\r\n");
}

CString CPenEnvironment::GetSavedWidth() const
{
	return ("<Width>" + CDataConversion::IntToString(m_nWidth) + "<\\Width>\r\n");
}

CString CPenEnvironment::GetSavedStyle() const
{
	return ("<Style>" + CDataConversion::IntToString((int)m_eStyle) + "<\\Style>\r\n");
}


///////////////////////////////////////////////////////////////////////////////
// class CDataTypeEnvironment

CDataTypeEnvironment& CDataTypeEnvironment::operator=(const CDataTypeEnvironment& environment)
{
	if(this == &environment)
		return *this;

	m_eCoordinateType = environment.GetCoordinateType();
	m_dVert = environment.GetVert();
	m_strHorz = environment.GetHorz();
	return *this;
}


// ----------------------------------------------------------------------------
CDataTypeEnvironment::CDataTypeEnvironment():
	m_eCoordinateType(CAnalysis::RELATIVE_POINT),
	m_dVert(1.0)
{
}

CDataTypeEnvironment::CDataTypeEnvironment(const CAnalysis::COORDINATETYPE eCoordinateType, const double& dVert, const CString& strHorz):
	m_eCoordinateType(eCoordinateType),
	m_dVert(dVert),
	m_strHorz(strHorz)
{
}

CDataTypeEnvironment::CDataTypeEnvironment(const CDataTypeEnvironment* pEnvironment)
{
	if(pEnvironment == NULL)
		return;

	m_eCoordinateType = pEnvironment->GetCoordinateType();
	m_dVert = pEnvironment->GetVert();
	m_strHorz = pEnvironment->GetHorz();
}


// public =====================================================================
void CDataTypeEnvironment::Reset_AbsolutePoint()
{
	m_eCoordinateType = CAnalysis::ABSOLUTE_POINT;
	m_dVert = 1.0;
	m_strHorz = "";
}

void CDataTypeEnvironment::SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType)
{
	m_eCoordinateType = eCoordinateType;
}

void CDataTypeEnvironment::SetVert(const double& dDataType)
{
	m_dVert = dDataType;
}

void CDataTypeEnvironment::SetHorz(const CString& strDataType)
{
	m_strHorz = strDataType;
}

// ----------------------------------------------------------------------------
bool CDataTypeEnvironment::IsRelativePointType() const
{
	return (m_eCoordinateType == CAnalysis::RELATIVE_POINT);
}

bool CDataTypeEnvironment::IsDateType() const
{
	if(m_strHorz.IsEmpty())
		return false;

	return (m_strHorz.Find("1") < 0);
}

CAnalysis::COORDINATETYPE CDataTypeEnvironment::GetCoordinateType() const
{
	return m_eCoordinateType;
}

double CDataTypeEnvironment::GetVert() const
{
	return m_dVert;
}

CString CDataTypeEnvironment::GetHorz() const
{
	return m_strHorz;
}

// ----------------------------------------------------------------------------
CString CDataTypeEnvironment::GetDateCompart() const
{
	if(m_strHorz.GetLength() > 0)
		return "/";
	return "";
}

CString CDataTypeEnvironment::GetTimeCompart() const
{
	if(m_strHorz.GetLength() > 0)
		return ":";
	return "";
}

// <DataType>
// <Coordinate>0<\\Coordinate>
// <Vert>1<\\Vert>
// <Horz>YYYYMMDD<\\Horz>
// <\\DataType>
CString CDataTypeEnvironment::GetSavedData() const
{
	CString strSaved("<DataType>\r\n");
	strSaved.Format("<DataType>\r\n%s%s%s<\\DataType>\r\n", 
		GetSavedCoordinateType(), GetSavedVert(), GetSavedHorz());
	return strSaved;
}


// private ====================================================================
CString CDataTypeEnvironment::GetSavedCoordinateType() const
{
	return ("<Coordinate>" + CDataConversion::IntToString((int)m_eCoordinateType) + "<\\Coordinate>\r\n");
}

CString CDataTypeEnvironment::GetSavedVert() const
{
	return ("<Vert>" + CDataConversion::DoubleToString(m_dVert) + "<\\Vert>\r\n");
}

CString CDataTypeEnvironment::GetSavedHorz() const
{
	return ("<Horz>" + m_strHorz + "<\\Horz>\r\n");
}


///////////////////////////////////////////////////////////////////////////////
// class CElementCommonEnvironment

CElementCommonEnvironment& CElementCommonEnvironment::operator=(const CElementCommonEnvironment& environment)
{
	if(this == &environment)
		return *this;

	m_bIsAutoPrice = environment.IsAutoPrice();
	m_pen = environment.GetPen();
	m_dataType = environment.GetDataType();
	return *this;
}


// public =====================================================================
CElementCommonEnvironment::CElementCommonEnvironment():
	m_bIsAutoPrice(false)
{
}

CElementCommonEnvironment::CElementCommonEnvironment(const bool bIsAutoPrice, const CPenEnvironment& pen, const CDataTypeEnvironment& dataType):
	m_bIsAutoPrice(bIsAutoPrice),
	m_pen(pen),
	m_dataType(dataType)
{
}

// ----------------------------------------------------------------------------
void CElementCommonEnvironment::SetAutoPrice(const bool bIsAutoPrice)
{
	m_bIsAutoPrice = bIsAutoPrice;
}

// ----------------------------------------------------------------------------
bool CElementCommonEnvironment::IsAutoPrice() const
{
	return m_bIsAutoPrice;
}

CPenEnvironment CElementCommonEnvironment::GetPen() const
{
	return m_pen;
}

CDataTypeEnvironment CElementCommonEnvironment::GetDataType() const
{
	return m_dataType;
}

CPenEnvironment& CElementCommonEnvironment::GetPen()
{
	return m_pen;
}

CDataTypeEnvironment& CElementCommonEnvironment::GetDataType()
{
	return m_dataType;
}

// <CommonEnvironment>
// <AutoPirce>0<\\AutoPirce>
// <Pen>
// <Color>0,0,0<\\Color>
// <Width>1<\\Width>
// <Style>0<\\Style>
// <\\Pen>
// <DataType>
// <Coordinate>0<\\Coordinate>
// <Vert>1<\\Vert>
// <Horz>YYYYMMDD<\\Horz>
// <\\DataType>
// <\\CommonEnvironment>
CString CElementCommonEnvironment::GetSavedData() const
{
	CString strSaved;
	strSaved.Format("<CommonEnvironment>\r\n%s%s%s<\\CommonEnvironment>\r\n", 
		GetSaveData_AutoPrice(), m_pen.GetSavedData(), m_dataType.GetSavedData());
	return strSaved;
}


// private ====================================================================
CString CElementCommonEnvironment::GetSaveData_AutoPrice() const
{
	CString strSaved;
	strSaved.Format("<AutoPirce>%d<\\AutoPirce>\r\n", m_bIsAutoPrice);
	return strSaved;
}


/*///////////////////////////////////////////////////////////////////////////////
// class CTextPosition

CString CTextPosition::GetSavedData() const
{
	return ("<TextPosition>\r\n" + GetSavedRealData() + "<\\TextPosition>\r\n");
}


// protected ==================================================================
CString CTextPosition::GetData(const bool bData, const CString& strData) const
{
	CString data;
	data.Format("<%s>%d<\\%s>\r\n", strData, bData, strData);
	return data;
}*/


///////////////////////////////////////////////////////////////////////////////
// class CTextEnvironment

CTextEnvironment& CTextEnvironment::operator=(const CTextEnvironment& environment)
{
	if(this == &environment)
		return *this;

	m_color = environment.GetColor();
	return *this;
}

// ----------------------------------------------------------------------------
CTextEnvironment::CTextEnvironment():
	m_color(RGB(0, 0, 0))
{
}

CTextEnvironment::CTextEnvironment(const COLORREF& color):
	m_color(color)
{
}

CTextEnvironment::CTextEnvironment(const CTextEnvironment& environment)
{
	m_color = environment.GetColor();
}

CTextEnvironment::CTextEnvironment(CTextEnvironment* pEnvironment)
{
	if(pEnvironment == NULL)
		return;

	m_color = pEnvironment->GetColor();
}


// public =====================================================================
void CTextEnvironment::Set(const CTextEnvironment& environment)
{
	m_color = environment.GetColor();
}

void CTextEnvironment::SetColor(const COLORREF& color)
{
	m_color = color;
}

// ----------------------------------------------------------------------------
COLORREF CTextEnvironment::GetColor() const
{
	return m_color;
}

// <TextEnvironment>
// <Color>0,0,0<\\Color>
// <\\TextEnvironment>
CString CTextEnvironment::GetSavedData() const
{
	CString strSaved;
	strSaved.Format("<TextEnvironment>\r\n%s<\\TextEnvironment>\r\n", GetSavedColor());
	return strSaved;
}


// private ====================================================================
CString CTextEnvironment::GetSavedColor() const
{
	return ("<Color>" + CDataConversion::RGB2Str(m_color) + "<\\Color>\r\n");
}


///////////////////////////////////////////////////////////////////////////////
// class CTextPosition

CTextPosition& CTextPosition::operator=(const CTextPosition& environment)
{
	if(this == &environment)
		return *this;

	m_textEnvironment = environment.GetTextEnvironment();
	return *this;
}

// ----------------------------------------------------------------------------
CTextPosition::CTextPosition()
{
}

CTextPosition::CTextPosition(const CTextEnvironment& textEnvironment):
	m_textEnvironment(textEnvironment)
{
}


// public =====================================================================
void CTextPosition::Set(const CTextPosition* pTextPosition)
{
	if(pTextPosition == NULL)
		return;
	SetTextEnvironment(pTextPosition->GetTextEnvironment());
}

void CTextPosition::SetTextEnvironment(const CTextEnvironment& textEnvironment)
{
	m_textEnvironment = textEnvironment;
}

// ----------------------------------------------------------------------------
CTextEnvironment CTextPosition::GetTextEnvironment() const
{
	return m_textEnvironment;
}

CTextEnvironment& CTextPosition::GetTextEnvironment()
{
	return m_textEnvironment;
}

// <PartEnvironment>
// <TextEnvironment>
// <Color>0,0,0<\\Color>
// <\\TextEnvironment>
// <TextPosition>
// ...
// <\\TextPosition>
// <\\PartEnvironment>
CString CTextPosition::GetSavedData() const
{
	CString strSaved;
	strSaved.Format("<PartEnvironment>\r\n%s%s<\\PartEnvironment>\r\n", 
		m_textEnvironment.GetSavedData(), GetSavedData_TextPosition());
	return strSaved;
}


// protected ==================================================================
CString CTextPosition::GetDataOfOneLine(const bool bData, const CString& strData) const
{
	CString data;
	data.Format("<%s>%d<\\%s>\r\n", strData, bData, strData);
	return data;
}

// private ====================================================================
// <TextPosition>
// <Horz>
// <Show>0<\\Show>
// <\\Horz>
// <Horz>
// <Left>0<\\Left>
// <Right>0<\\Right>
// <\\Horz>
// <Vert>
// <Top>0<\\Top>
// <Bottom>0<\\Bottom>
// <\\Vert>
// <\\TextPosition>
CString CTextPosition::GetSavedData_TextPosition() const
{
	CString strSaved;
	strSaved.Format("<TextPosition>\r\n%s<\\TextPosition>\r\n", 
		GetSavedRealTextPositionData());
	return strSaved;
}


///////////////////////////////////////////////////////////////////////////////
// class CTextShowPosition

CTextShowPosition::CTextShowPosition():
	m_bShow(false)
{
}

CTextShowPosition::CTextShowPosition(const CTextEnvironment& environment, const bool bShow):
	CTextPosition(environment), 
	m_bShow(bShow)
{
}

CTextShowPosition::CTextShowPosition(const CTextShowPosition& position):
	CTextPosition(position.GetTextEnvironment()), 
	m_bShow(position.IsShow())
{
}


// public =====================================================================
void CTextShowPosition::Set(const CTextPosition* pTextPosition)
{
	if(pTextPosition == NULL)
		return;

	CTextShowPosition* pTextShowPosition = (CTextShowPosition*) pTextPosition;
	assert(pTextShowPosition != NULL);
	SetShow(pTextShowPosition->IsShow());

	CTextPosition::Set(pTextPosition);
}

void CTextShowPosition::SetShow(const bool bShow)
{
	m_bShow = bShow;
}

bool CTextShowPosition::IsShow() const
{
	return m_bShow;
}

// protected ==================================================================
// <Horz>
// <Show>0<\\Show>
// <\\Horz>
CString CTextShowPosition::GetSavedRealTextPositionData() const
{
	CString strSaved;
	strSaved.Format("<Horz>\r\n%d<\\Horz>\r\n", 
		CTextPosition::GetDataOfOneLine(m_bShow, "Show"));
	return strSaved;
}


///////////////////////////////////////////////////////////////////////////////
// class CTextHorzPosition

CTextHorzPosition::CTextHorzPosition():
	m_bLeft(false),
	m_bRight(false)
{
}

CTextHorzPosition::CTextHorzPosition(const CTextEnvironment& environment, const bool bLeft, const bool bRight):
	CTextPosition(environment), 
	m_bLeft(bLeft),
	m_bRight(bRight)
{
}

CTextHorzPosition::CTextHorzPosition(const CTextHorzPosition& position):
	CTextPosition(position.GetTextEnvironment()), 
	m_bLeft(position.GetLeft()),
	m_bRight(position.GetRight())
{
}


// public =====================================================================
void CTextHorzPosition::Set(const CTextPosition* pTextPosition)
{
	if(pTextPosition == NULL)
		return;

	CTextHorzPosition* pTextHorzPosition = (CTextHorzPosition*) pTextPosition;
	assert(pTextHorzPosition != NULL);
	SetLeft(pTextHorzPosition->GetLeft());
	SetRight(pTextHorzPosition->GetRight());

	CTextPosition::Set(pTextPosition);
}

void CTextHorzPosition::SetLeft(const bool bLeft)
{
	m_bLeft = bLeft;
}

void CTextHorzPosition::SetRight(const bool bRight)
{
	m_bRight = bRight;
}

bool CTextHorzPosition::GetLeft() const
{
	return m_bLeft;
}

bool CTextHorzPosition::GetRight() const
{
	return m_bRight;
}

// protected ==================================================================
// <Horz>
// <Left>0<\\Left>
// <Right>0<\\Right>
// <\\Horz>
CString CTextHorzPosition::GetSavedRealTextPositionData() const
{
	CString strSaved;
	strSaved.Format("<Horz>\r\n%s%s%s<\\Horz>\r\n", 
		CTextPosition::GetDataOfOneLine(m_bLeft, "Left"), 
		CTextPosition::GetDataOfOneLine(m_bRight, "Right"),
		CTextPosition::GetDataOfOneLine(m_bRightRate, "Rate"));
	return strSaved;
}


///////////////////////////////////////////////////////////////////////////////
// class CTextVertPosition

CTextVertPosition::CTextVertPosition():
	m_bTop(false),
	m_bBottom(false)
{
}

CTextVertPosition::CTextVertPosition(const CTextEnvironment& environment, const bool bTop, const bool bBottom):
	CTextPosition(environment), 
	m_bTop(bTop),
	m_bBottom(bBottom)
{
}

CTextVertPosition::CTextVertPosition(const CTextVertPosition& position):
	CTextPosition(position.GetTextEnvironment()), 
	m_bTop(position.GetTop()),
	m_bBottom(position.GetBottom())
{
}


// public =====================================================================
void CTextVertPosition::Set(const CTextPosition* pTextPosition)
{
	if(pTextPosition == NULL)
		return;

	CTextVertPosition* pTextVertPosition = (CTextVertPosition*) pTextPosition;
	assert(pTextVertPosition != NULL);
	SetTop(pTextVertPosition->GetTop());
	SetBottom(pTextVertPosition->GetBottom());

	CTextPosition::Set(pTextPosition);
}

void CTextVertPosition::SetTop(const bool bTop)
{
	m_bTop = bTop;
}

void CTextVertPosition::SetBottom(const bool bBottom)
{
	m_bBottom = bBottom;
}

bool CTextVertPosition::GetTop() const
{
	return m_bTop;
}

bool CTextVertPosition::GetBottom() const
{
	return m_bBottom;
}

// protected ==================================================================
// <Vert>
// <Top>0<\\Top>
// <Bottom>0<\\Bottom>
// <\\Vert>
CString CTextVertPosition::GetSavedRealTextPositionData() const
{
	CString strSaved("<Vert>\r\n");
	strSaved += CTextPosition::GetDataOfOneLine(m_bTop, "Top");
	strSaved += CTextPosition::GetDataOfOneLine(m_bBottom, "Bottom");
	strSaved += "<\\Vert>\r\n";
	return strSaved;
}


///////////////////////////////////////////////////////////////////////////////
// class CTextHorzVertPosition

CTextHorzVertPosition& CTextHorzVertPosition::operator=(const CTextHorzVertPosition& position)
{
	if(this == &position)
		return *this;

	CTextPosition::SetTextEnvironment(position.GetTextEnvironment());

	m_bLeft = position.GetLeft();
	m_bRight = position.GetRight();
	m_bTop = position.GetTop();
	m_bBottom = position.GetBottom();
	return *this;
}

CTextHorzVertPosition::CTextHorzVertPosition():
	m_bLeft(false), 
	m_bRight(false),
	m_bTop(false),
	m_bBottom(false)
{
}
	
CTextHorzVertPosition::CTextHorzVertPosition(const CTextEnvironment& environment, 
		const bool bLeft, const bool bRight, const bool bTop, const bool bBottom):
	CTextPosition(environment), 
	m_bLeft(bLeft), 
	m_bRight(bRight),
	m_bTop(bTop),
	m_bBottom(bBottom)
{
}


// public =====================================================================
void CTextHorzVertPosition::Set(const CTextPosition* pTextPosition)
{
	if(pTextPosition == NULL)
		return;

	CTextHorzVertPosition* pTextHorzVertPosition = (CTextHorzVertPosition*) pTextPosition;
	assert(pTextHorzVertPosition != NULL);
	SetLeft(pTextHorzVertPosition->GetLeft());
	SetRight(pTextHorzVertPosition->GetRight());
	SetTop(pTextHorzVertPosition->GetTop());
	SetBottom(pTextHorzVertPosition->GetBottom());

	CTextPosition::Set(pTextPosition);
}

void CTextHorzVertPosition::SetLeft(const bool bLeft)
{
	m_bLeft = bLeft;
}

void CTextHorzVertPosition::SetRight(const bool bRight)
{
	m_bRight = bRight;
}

void CTextHorzVertPosition::SetTop(const bool bTop)
{
	m_bTop = bTop;
}

void CTextHorzVertPosition::SetBottom(const bool bBottom)
{
	m_bBottom = bBottom;
}

// ----------------------------------------------------------------------------
bool CTextHorzVertPosition::GetLeft() const
{
	return m_bLeft;
}

bool CTextHorzVertPosition::GetRight() const
{
	return m_bRight;
}

bool CTextHorzVertPosition::GetTop() const
{
	return m_bTop;
}

bool CTextHorzVertPosition::GetBottom() const
{
	return m_bBottom;
}

// ----------------------------------------------------------------------------
CTextHorzPosition CTextHorzVertPosition::GetHorz() const
{
	return CTextHorzPosition(CTextPosition::GetTextEnvironment(), m_bLeft, m_bRight);
}

CTextVertPosition CTextHorzVertPosition::GetVert() const
{
	return CTextVertPosition(CTextPosition::GetTextEnvironment(), m_bTop, m_bBottom);
}


// protected ==================================================================
// <HorzVert>
// <Left>0<\\Left>
// <Right>0<\\Right>
// <Top>0<\\Top>
// <Bottom>0<\\Bottom>
// <\\HorzVert>
CString CTextHorzVertPosition::GetSavedRealTextPositionData() const
{
	CString strSaved("<HorzVert>\r\n");
	strSaved += CTextPosition::GetDataOfOneLine(m_bLeft, "Left");
	strSaved += CTextPosition::GetDataOfOneLine(m_bRight, "Right");
	strSaved += CTextPosition::GetDataOfOneLine(m_bTop, "Top");
	strSaved += CTextPosition::GetDataOfOneLine(m_bBottom, "Bottom");
	strSaved += "<\\HorzVert>\r\n";
	return strSaved;
}
