// Color.cpp: implementation of the CColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Color.h"

#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// closs CGraphRegionColor

CGraphRegionColor& CGraphRegionColor::operator=(const CGraphRegionColor& data)
{
	if(this == &data)
		return *this;

	m_backgroundColor = data.GetBackgroundColor();
	m_lineColor = data.GetLineColor();
	m_frameColor = data.GetFrameColor();
	return *this;
}

CGraphRegionColor::CGraphRegionColor():
	m_backgroundColor(RGB(255, 255, 255)),
	m_lineColor(RGB(255, 255, 255)),
	m_frameColor(RGB(121, 121, 121))
{
}

CGraphRegionColor::CGraphRegionColor(const COLORREF& backgroundColor, const COLORREF& lineColor, const COLORREF& frameColor):
	m_backgroundColor(backgroundColor),
	m_lineColor(lineColor),
	m_frameColor(frameColor)
{
}

// public =================================================================================
// graph 영역의 바탕색
void CGraphRegionColor::SetBackgroundColor(const COLORREF& backgroundColor)
{
	m_backgroundColor = backgroundColor;
}

// graph 영역의 선색
void CGraphRegionColor::SetLineColor(const COLORREF& lineColor)
{
	m_lineColor = lineColor;
}

// graph 영역의 테두리색
void CGraphRegionColor::SetFrameColor(const COLORREF& frameColor)
{
	m_frameColor = frameColor;
}

// ----------------------------------------------------------------------------------------
COLORREF CGraphRegionColor::GetBackgroundColor() const
{
	return m_backgroundColor;
}

COLORREF CGraphRegionColor::GetLineColor() const
{
	return m_lineColor;
}

COLORREF CGraphRegionColor::GetFrameColor() const
{
	return m_frameColor;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CScaleColor

CScaleColor& CScaleColor::operator=(const CScaleColor& data)
{
	if(this == &data)
		return *this;

	m_horzGridColor = data.GetHorzScaleColor();
	m_vertGridColor = data.GetVertScaleColor();
	m_textColor = data.GetScaleTextColor();
	return *this;
}

CScaleColor::CScaleColor():
	m_horzGridColor(RGB(202, 202, 202)),
	m_vertGridColor(RGB(202, 202, 202)),
	m_textColor(RGB(0, 0, 0))
{
}

CScaleColor::CScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor):
	m_horzGridColor(horzGridColor),
	m_vertGridColor(vertGridColor),
	m_textColor(textColor)
{
}

// public =================================================================================
void CScaleColor::SetHorzScaleColor(const COLORREF& horzGridColor)
{
	m_horzGridColor = horzGridColor;
}

void CScaleColor::SetVertScaleColor(const COLORREF& vertGridColor)
{
	m_vertGridColor = vertGridColor;
}

void CScaleColor::SetScaleTextColor(const COLORREF& textColor)
{
	m_textColor = textColor;
}

// ----------------------------------------------------------------------------------------
COLORREF CScaleColor::GetHorzScaleColor() const
{
	return m_horzGridColor;
}

COLORREF CScaleColor::GetVertScaleColor() const
{
	return m_vertGridColor;
}

COLORREF CScaleColor::GetScaleTextColor() const
{
	return m_textColor;
}

COLORREF& CScaleColor::GetHorzScaleColor()
{
	return m_horzGridColor;
}

COLORREF& CScaleColor::GetVertScaleColor()
{
	return m_vertGridColor;
}

COLORREF& CScaleColor::GetScaleTextColor()
{
	return m_textColor;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CEtcColor

CEtcColor& CEtcColor::operator=(const CEtcColor& data)
{
	if(this == &data)
		return *this;

	m_toolColor = data.GetToolColor();
	m_etcGraphColor = data.GetEtcGraphColor();
	return *this;
}

CEtcColor::CEtcColor():
	m_toolColor(RGB(224, 224, 224))
{
}

CEtcColor::CEtcColor(const COLORREF& toolColor, const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6)
{
	m_toolColor = toolColor;
	SetEtcGraphColor(1, graphColor1);
	SetEtcGraphColor(2, graphColor2);
	SetEtcGraphColor(3, graphColor3);
	SetEtcGraphColor(4, graphColor4);
	SetEtcGraphColor(5, graphColor5);
	SetEtcGraphColor(6, graphColor6);
}

// public =================================================================================
void CEtcColor::SetToolColor(const COLORREF& toolColor)
{
	m_toolColor = toolColor;
}

void CEtcColor::SetEtcGraphColor(const CEtcGraphColor& etcGraphColor)
{
	m_etcGraphColor = etcGraphColor;
}

void CEtcColor::SetEtcGraphColor(const int count, const COLORREF& graphColor)
{
	switch(count){
	case 1:
		m_etcGraphColor.SetGraphColor1(graphColor);
		break;
	case 2:
		m_etcGraphColor.SetGraphColor2(graphColor);
		break;
	case 3:
		m_etcGraphColor.SetGraphColor3(graphColor);
		break;
	case 4:
		m_etcGraphColor.SetGraphColor4(graphColor);
		break;
	case 5:
		m_etcGraphColor.SetGraphColor5(graphColor);
		break;
	case 6:
		m_etcGraphColor.SetGraphColor6(graphColor);
		break;
	default:
		break;
	}
}

COLORREF CEtcColor::GetToolColor() const
{
	return m_toolColor;
}

CEtcGraphColor CEtcColor::GetEtcGraphColor() const
{
	return m_etcGraphColor;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CEtcGraphColor

CEtcGraphColor& CEtcGraphColor::operator=(const CEtcGraphColor& data)
{
	if(this == &data)
		return *this;

	m_graphColor1 = data.GetGraphColor1();
	m_graphColor2 = data.GetGraphColor2();
	m_graphColor3 = data.GetGraphColor3();
	m_graphColor4 = data.GetGraphColor4();
	m_graphColor5 = data.GetGraphColor5();
	m_graphColor6 = data.GetGraphColor6();
	return *this;
}

CEtcGraphColor::CEtcGraphColor():
	m_graphColor1(RGB(204, 204, 204)),
	m_graphColor2(RGB(204, 204, 204)),
	m_graphColor3(RGB(204, 204, 204)),
	m_graphColor4(RGB(204, 204, 204)),
	m_graphColor5(RGB(204, 204, 204)),
	m_graphColor6(RGB(204, 204, 204))
{
}

CEtcGraphColor::CEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6):
	m_graphColor1(graphColor1),
	m_graphColor2(graphColor2),
	m_graphColor3(graphColor3),
	m_graphColor4(graphColor4),
	m_graphColor5(graphColor5),
	m_graphColor6(graphColor6)
{
}

// public =================================================================================
void CEtcGraphColor::SetGraphColor1(const COLORREF& graphColor)
{
	m_graphColor1 = graphColor;
}

void CEtcGraphColor::SetGraphColor2(const COLORREF& graphColor)
{
	m_graphColor2 = graphColor;
}

void CEtcGraphColor::SetGraphColor3(const COLORREF& graphColor)
{
	m_graphColor3 = graphColor;
}

void CEtcGraphColor::SetGraphColor4(const COLORREF& graphColor)
{
	m_graphColor4 = graphColor;
}

void CEtcGraphColor::SetGraphColor5(const COLORREF& graphColor)
{
	m_graphColor5 = graphColor;
}

void CEtcGraphColor::SetGraphColor6(const COLORREF& graphColor)
{
	m_graphColor6 = graphColor;
}

// ----------------------------------------------------------------------------------------
COLORREF CEtcGraphColor::GetGraphColor1() const
{
	return m_graphColor1;
}

COLORREF CEtcGraphColor::GetGraphColor2() const
{
	return m_graphColor2;
}

COLORREF CEtcGraphColor::GetGraphColor3() const
{
	return m_graphColor3;
}

COLORREF CEtcGraphColor::GetGraphColor4() const
{
	return m_graphColor4;
}

COLORREF CEtcGraphColor::GetGraphColor5() const
{
	return m_graphColor5;
}

COLORREF CEtcGraphColor::GetGraphColor6() const
{
	return m_graphColor6;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CGraphColor

CGraphColor& CGraphColor::operator=(const CGraphColor& data)
{
	if(this == &data)
		return *this;

	m_color1 = data.GetColor1();
	m_color2 = data.GetColor2();
	m_color3 = data.GetColor3();
	m_color4 = data.GetColor4();
	m_color5 = data.GetColor5();
	m_color6 = data.GetColor6();
	m_bUpGraphFill = data.GetUpGraphFill();
	m_bDownGraphFill = data.GetDownGraphFill();
	return *this;
}

CGraphColor::CGraphColor() :
	m_color1(RGB(204, 204, 204)),
	m_color2(RGB(204, 204, 204)),
	m_color3(RGB(204, 204, 204)),
	m_color4(RGB(204, 204, 204)),
	m_color5(RGB(204, 204, 204)),
	m_color6(RGB(204, 204, 204)),
	m_bUpGraphFill(true),
	m_bDownGraphFill(true)
{
}

CGraphColor::CGraphColor(const CString& strData) :
	m_color1(RGB(204, 204, 204)),
	m_color2(RGB(204, 204, 204)),
	m_color3(RGB(204, 204, 204)),
	m_color4(RGB(204, 204, 204)),
	m_color5(RGB(204, 204, 204)),
	m_color6(RGB(204, 204, 204)),
	m_bUpGraphFill(true),
	m_bDownGraphFill(true)
{
	SetData(strData);
}

CGraphColor::CGraphColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, const COLORREF& color4, const COLORREF& color5, const COLORREF& color6):
	m_color1(color1),
	m_color2(color2),
	m_color3(color3),
	m_color4(color4),
	m_color5(color5),
	m_color6(color6),
	m_bUpGraphFill(true),
	m_bDownGraphFill(true)
{
}

CGraphColor::CGraphColor(const CGraphColor& data):
	m_color1(data.GetColor1()),
	m_color2(data.GetColor2()),
	m_color3(data.GetColor3()),
	m_color4(data.GetColor4()),
	m_color5(data.GetColor5()),
	m_color6(data.GetColor6()),
	m_bUpGraphFill(data.GetUpGraphFill()),
	m_bDownGraphFill(data.GetDownGraphFill())
{
}

// public =================================================================================
void CGraphColor::SetData(const CString& strData)
{
	if(strData.IsEmpty())
		return;

	CString data = strData;
	// graph color
	SetColorData(CDataConversion::GetStringData(data, ";"), m_color1);
	SetColorData(CDataConversion::GetStringData(data, ";"), m_color2);
	SetColorData(CDataConversion::GetStringData(data, ";"), m_color3);
	SetColorData(CDataConversion::GetStringData(data, ";"), m_color4);
	SetColorData(CDataConversion::GetStringData(data, ";"), m_color5);
	SetColorData(CDataConversion::GetStringData(data, ";"), m_color6);
	// 채움
	SetBoolData(CDataConversion::GetStringData(data, ";"), m_bUpGraphFill);
	SetBoolData(CDataConversion::GetStringData(data, ";"), m_bDownGraphFill);
}

void CGraphColor::SetData(const CGraphColor& graphColor)
{
	m_color1 = graphColor.GetColor1();
	m_color2 = graphColor.GetColor2();
	m_color3 = graphColor.GetColor3();
	m_color4 = graphColor.GetColor4();
	m_color5 = graphColor.GetColor5();
	m_color6 = graphColor.GetColor6();
	m_bUpGraphFill = graphColor.GetUpGraphFill();
	m_bDownGraphFill = graphColor.GetDownGraphFill();
}

void CGraphColor::SetColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, 
		const COLORREF& color4, const COLORREF& color5, const COLORREF& color6)
{
	m_color1 = color1;
	m_color2 = color2;
	m_color3 = color3;
	m_color4 = color4;
	m_color5 = color5;
	m_color6 = color6;
}

void CGraphColor::SetColor1(const COLORREF& color)
{
	m_color1 = color;
}

void CGraphColor::SetColor2(const COLORREF& color)
{
	m_color2 = color;
}

void CGraphColor::SetColor3(const COLORREF& color)
{
	m_color3 = color;
}

void CGraphColor::SetColor4(const COLORREF& color)
{
	m_color4 = color;
}

void CGraphColor::SetColor5(const COLORREF& color)
{
	m_color5 = color;
}

void CGraphColor::SetColor6(const COLORREF& color)
{
	m_color6 = color;
}

void CGraphColor::SetUpGraphFill(const bool isFill)
{
	m_bUpGraphFill = isFill;
}

void CGraphColor::SetDownGraphFill(const bool isFill)
{
	m_bDownGraphFill = isFill;
}

// ----------------------------------------------------------------------------------------
COLORREF CGraphColor::GetColor1() const
{
	return m_color1;
}

COLORREF CGraphColor::GetColor2() const
{
	return m_color2;
}

COLORREF CGraphColor::GetColor3() const
{
	return m_color3;
}

COLORREF CGraphColor::GetColor4() const
{
	return m_color4;
}

COLORREF CGraphColor::GetColor5() const
{
	return m_color5;
}

COLORREF CGraphColor::GetColor6() const
{
	return m_color6;
}

bool CGraphColor::GetUpGraphFill() const
{
	return m_bUpGraphFill;
}

bool CGraphColor::GetDownGraphFill() const
{
	return m_bDownGraphFill;
}

CString CGraphColor::GetStringData() const
{
	CString strGraphColor;
	// 상승색
	strGraphColor = CDataConversion::RGB2Str(m_color1) + ';';
	strGraphColor += CDataConversion::RGB2Str(m_color2) + ';';

	// 하락색
	strGraphColor += CDataConversion::RGB2Str(m_color3) + ';';
	strGraphColor += CDataConversion::RGB2Str(m_color4) + ';';

	// 보합색
	strGraphColor += CDataConversion::RGB2Str(m_color5) + ';';
	strGraphColor += CDataConversion::RGB2Str(m_color6) + ';';

	// 채움
	strGraphColor += CDataConversion::IntToString((int)m_bUpGraphFill) + ';';
	strGraphColor += CDataConversion::IntToString((int)m_bDownGraphFill) + ';';

	return strGraphColor;
}

// private ================================================================================
void CGraphColor::SetColorData(const CString& strData, COLORREF& clrColor)
{
	if(strData.IsEmpty())
		return;

	clrColor = CDataConversion::Str2RGB(strData);
}

void CGraphColor::SetBoolData(const CString& strData, bool& bData)
{
	if(strData.IsEmpty())
		return;

	bData = CDataConversion::atob(strData);
}
