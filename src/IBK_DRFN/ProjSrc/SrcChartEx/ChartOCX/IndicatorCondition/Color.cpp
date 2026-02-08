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
// graph ¿µ¿ªÀÇ ¹ÙÅÁ»ö
void CGraphRegionColor::SetBackgroundColor(const COLORREF& backgroundColor)
{
	m_backgroundColor = backgroundColor;
}

// graph ¿µ¿ªÀÇ ¼±»ö
void CGraphRegionColor::SetLineColor(const COLORREF& lineColor)
{
	m_lineColor = lineColor;
}

// graph ¿µ¿ªÀÇ Å×µÎ¸®»ö
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
//@Solomon	m_bUpGraphFill = data.GetUpGraphFill();
//@Solomon	m_bDownGraphFill = data.GetDownGraphFill();
	//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_color7 = data.GetColor7();
	m_color8 = data.GetColor8();
	m_color9 = data.GetColor9();
	m_color10 = data.GetColor10();

	m_bUpPositiveGraphFill = data.GetUpPositiveGraphFill();
	m_bUpNegativeGraphFill = data.GetUpNegativeGraphFill();
	m_bDownPositiveGraphFill = data.GetDonwPositiveGraphFill();
	m_bDownNegativeGraphFill = data.GetDownNegativeGraphFill();

	return *this;
}

CGraphColor::CGraphColor() :
	m_color1( RGB( 204, 204, 204)),
	m_color2( RGB( 204, 204, 204)),
	m_color3( RGB( 204, 204, 204)),
	m_color4( RGB( 204, 204, 204)),
	m_color5( RGB( 204, 204, 204)),
	m_color6( RGB( 204, 204, 204)),
//@Solomon	m_bUpGraphFill(true),
//@Solomon	m_bDownGraphFill(true)
	//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_color7( RGB( 204, 204, 204)),
	m_color8( RGB( 204, 204, 204)),
	m_color9( RGB( 204, 204, 204)),
	m_color10( RGB( 204, 204, 204)),
	m_bUpPositiveGraphFill(true),
	m_bUpNegativeGraphFill(true),
	m_bDownPositiveGraphFill(true),
	m_bDownNegativeGraphFill(true)
{
}

CGraphColor::CGraphColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, const COLORREF& color4, const COLORREF& color5, const COLORREF& color6):
	m_color1(color1),
	m_color2(color2),
	m_color3(color3),
	m_color4(color4),
	m_color5(color5),
	m_color6(color6),
//@Solomon	m_bUpGraphFill(true),
//@Solomon	m_bDownGraphFill(true)
	//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_color7(color3),
	m_color8(color4),
	m_color9(color1),
	m_color10(color2),
	m_bUpPositiveGraphFill(true),
	m_bUpNegativeGraphFill(true),
	m_bDownPositiveGraphFill(true),
	m_bDownNegativeGraphFill(true)
{
}

CGraphColor::CGraphColor(const CGraphColor& data):
	m_color1(data.GetColor1()),
	m_color2(data.GetColor2()),
	m_color3(data.GetColor3()),
	m_color4(data.GetColor4()),
	m_color5(data.GetColor5()),
	m_color6(data.GetColor6()),
//@Solomon	m_bUpGraphFill(data.GetUpGraphFill()),
//@Solomon	m_bDownGraphFill(data.GetDownGraphFill()),

	//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_color7(data.GetColor7()),
	m_color8(data.GetColor8()),
	m_color9(data.GetColor9()),
	m_color10(data.GetColor10()),
	m_bUpPositiveGraphFill(data.GetUpPositiveGraphFill()),
	m_bUpNegativeGraphFill(data.GetUpNegativeGraphFill()),
	m_bDownPositiveGraphFill(data.GetDonwPositiveGraphFill()),
	m_bDownNegativeGraphFill(data.GetDownNegativeGraphFill())
{
}

// public =================================================================================
/*
void CGraphColor::SetData( const char *p_szGraphColor, BOOL p_bWithInit)
{
	if( p_bWithInit)
	{
		m_color1			= RGB(204, 204, 204);
		m_color2			= RGB(204, 204, 204);
		m_color3			= RGB(204, 204, 204);
		m_color4			= RGB(204, 204, 204);
		m_color5			= RGB(204, 204, 204);
		m_color6			= RGB(204, 204, 204);

		m_bUpGraphFill		= false;
		m_bDownGraphFill	= false;
	}

	if( !p_szGraphColor) return;
	if( !*p_szGraphColor) return;

	CString strGraphColor;
	int nGraphColorLen = strlen( p_szGraphColor);
	char *szGraphColor = strGraphColor.GetBufferSetLength( nGraphColorLen);
	strcpy( szGraphColor, p_szGraphColor);

	char *szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color1 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color2 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color3 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color4 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color5 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color6 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_bUpGraphFill = ( atoi( szItem) != 0);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	m_bDownGraphFill = ( atoi( szItem) != 0);
}
*/

void CGraphColor::SetData( const char *p_szGraphColor, BOOL p_bWithInit)
{
	if( p_bWithInit)
	{
		m_color1			= RGB(204, 204, 204);
		m_color2			= RGB(204, 204, 204);
		m_color3			= RGB(204, 204, 204);
		m_color4			= RGB(204, 204, 204);
		m_color5			= RGB(204, 204, 204);
		m_color6			= RGB(204, 204, 204);
		// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
		m_color7			= RGB(204, 204, 204);
		m_color8			= RGB(204, 204, 204);
		m_color9			= RGB(204, 204, 204);
		m_color10			= RGB(204, 204, 204);
//		m_bUpGraphFill		= false;
//		m_bDownGraphFill	= false;
		// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
		m_bUpPositiveGraphFill	= false;
		m_bUpNegativeGraphFill	= false;
		m_bDownPositiveGraphFill	= false;
		m_bDownNegativeGraphFill	= false;
	}

	if( !p_szGraphColor) return;
	if( !*p_szGraphColor) return;

	CString strGraphColor;
	int nGraphColorLen = strlen( p_szGraphColor);
	char *szGraphColor = strGraphColor.GetBufferSetLength( nGraphColorLen);
	strcpy( szGraphColor, p_szGraphColor);

	char *szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color1 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color2 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color3 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color4 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color5 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color6 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_bUpPositiveGraphFill = ( atoi( szItem) != 0);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_bUpNegativeGraphFill = ( atoi( szItem) != 0);
	
/*
	// ÀÓ½Ã
	m_color7 = m_color3;
	m_color8 = m_color4;
	m_color9 = m_color1;
	m_color10 = m_color2;

	m_bDownPositiveGraphFill = m_bUpPositiveGraphFill;
	m_bDownNegativeGraphFill = m_bUpNegativeGraphFill;
	return;
*/

	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	char* szColor = strchr( szGraphColor, ',');
	if( !szGraphColor || !*szGraphColor || !szColor || !*szColor || !::lstrcmp(szGraphColor, "1,0,0;1,0,0;1,0,0;0,0,0;0;1"))
	{
		m_color7 = m_color3;
		m_color8 = m_color4;
		m_color9 = m_color1;
		m_color10 = m_color2;

		m_bDownPositiveGraphFill = m_bUpPositiveGraphFill;
		m_bDownNegativeGraphFill = m_bUpNegativeGraphFill;
		return;
	}

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color7 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color8 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color9 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_color10 = GetColorValue( szItem);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	szGraphColor = strchr( szGraphColor, ';');
	if( szGraphColor) *szGraphColor++ = '\0';
	m_bDownPositiveGraphFill = ( atoi( szItem) != 0);
	if( !szGraphColor) return;
	if( !*szGraphColor) return;

	szItem = szGraphColor;
	m_bDownNegativeGraphFill = ( atoi( szItem) != 0);
}

/*
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
*/
void CGraphColor::SetData(const CGraphColor& graphColor)
{
	m_color1 = graphColor.GetColor1();
	m_color2 = graphColor.GetColor2();
	m_color3 = graphColor.GetColor3();
	m_color4 = graphColor.GetColor4();
	m_color5 = graphColor.GetColor5();
	m_color6 = graphColor.GetColor6();
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_color7 = graphColor.GetColor7();
	m_color8 = graphColor.GetColor8();
	m_color9 = graphColor.GetColor9();
	m_color10 = graphColor.GetColor10();
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	//	m_bUpGraphFill = graphColor.GetUpGraphFill();
	//	m_bDownGraphFill = graphColor.GetDownGraphFill();
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_bUpPositiveGraphFill = graphColor.GetUpPositiveGraphFill();
	m_bUpNegativeGraphFill = graphColor.GetUpNegativeGraphFill();
	m_bDownPositiveGraphFill = graphColor.GetDonwPositiveGraphFill();
	m_bDownNegativeGraphFill = graphColor.GetDownNegativeGraphFill();
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

	//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_color7 = color3;
	m_color8 = color4;
	m_color9 = color1;
	m_color10 = color2;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, 
						   const COLORREF& color4, const COLORREF& color5, const COLORREF& color6,
						   const COLORREF& color7, const COLORREF& color8, const COLORREF& color9, const COLORREF& color10)
{
	m_color1 = color1;
	m_color2 = color2;
	m_color3 = color3;
	m_color4 = color4;
	m_color5 = color5;
	m_color6 = color6;
	m_color7 = color7;
	m_color8 = color8;
	m_color9 = color9;
	m_color10 = color10;
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

//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetColor7(const COLORREF& color)
{
	m_color7 = color;
}

//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetColor8(const COLORREF& color)
{
	m_color8 = color;
}

//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetColor9(const COLORREF& color)
{
	m_color9 = color;
}

//@Solomon »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetColor10(const COLORREF& color)
{
	m_color10 = color;
}

//@Solomon void CGraphColor::SetUpGraphFill(const bool isFill)
//@Solomon {
//@Solomon 	m_bUpGraphFill = isFill;
//@Solomon }
//@Solomon 
//@Solomon void CGraphColor::SetDownGraphFill(const bool isFill)
//@Solomon {
//@Solomon 	m_bDownGraphFill = isFill;
//@Solomon }

void CGraphColor::SetUpGraphFill(const bool isFill)
{
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_bUpPositiveGraphFill = isFill;
}

void CGraphColor::SetDownGraphFill(const bool isFill)
{
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	m_bUpNegativeGraphFill = isFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetUpPositiveGraphFill(const bool isFill)
{
	m_bUpPositiveGraphFill = isFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetUpNegativeGraphFill(const bool isFill)
{
	m_bUpNegativeGraphFill = isFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetDownPositiveGraphFill(const bool isFill)
{
	m_bDownPositiveGraphFill = isFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
void CGraphColor::SetDownNegativeGraphFill(const bool isFill)
{
	m_bDownNegativeGraphFill = isFill;
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

/*@Solomon
bool CGraphColor::GetUpGraphFill() const
{
	return m_bUpGraphFill;
}

bool CGraphColor::GetDownGraphFill() const
{
	return m_bDownGraphFill;
}
*/

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
COLORREF CGraphColor::GetColor7() const
{
	return m_color7;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
COLORREF CGraphColor::GetColor8() const
{
	return m_color8;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
COLORREF CGraphColor::GetColor9() const
{
	return m_color9;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
COLORREF CGraphColor::GetColor10() const
{
	return m_color10;
}

bool CGraphColor::GetUpGraphFill() const
{
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	return m_bUpPositiveGraphFill;
}

bool CGraphColor::GetDownGraphFill() const
{
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	return m_bUpNegativeGraphFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
bool CGraphColor::GetUpPositiveGraphFill() const
{
	return m_bUpPositiveGraphFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
bool CGraphColor::GetUpNegativeGraphFill() const
{
	return m_bUpNegativeGraphFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
bool CGraphColor::GetDonwPositiveGraphFill() const
{
	return m_bDownPositiveGraphFill;
}

// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
bool CGraphColor::GetDownNegativeGraphFill() const
{
	return m_bDownNegativeGraphFill;
}

/*@Solomon
void CGraphColor::GetStringData( CString &p_strGraphColor) const
{
	char szColor1[ 15], szColor2[ 15], szColor3[ 15], szColor4[ 15], szColor5[ 15], szColor6[ 15];

	int nColorLen1 = GetColorString( m_color1, szColor1);
	int nColorLen2 = GetColorString( m_color2, szColor2);
	int nColorLen3 = GetColorString( m_color3, szColor3);
	int nColorLen4 = GetColorString( m_color4, szColor4);
	int nColorLen5 = GetColorString( m_color5, szColor5);
	int nColorLen6 = GetColorString( m_color6, szColor6);

	int nGraphColorLen = 0;
	nGraphColorLen += nColorLen1;
	nGraphColorLen++;
	nGraphColorLen += nColorLen2;
	nGraphColorLen++;
	nGraphColorLen += nColorLen3;
	nGraphColorLen++;
	nGraphColorLen += nColorLen4;
	nGraphColorLen++;
	nGraphColorLen += nColorLen5;
	nGraphColorLen++;
	nGraphColorLen += nColorLen6;
	nGraphColorLen++;
	nGraphColorLen += 4;	// ?;?;	(Do not add NULL Terminator size. It has that with default)

	char *szGraphColor = p_strGraphColor.GetBufferSetLength( nGraphColorLen);

	strcpy( szGraphColor, szColor1);
	szGraphColor += nColorLen1;
	*szGraphColor++ = ';';

	strcpy( szGraphColor, szColor2);
	szGraphColor += nColorLen2;
	*szGraphColor++ = ';';

	strcpy( szGraphColor, szColor3);
	szGraphColor += nColorLen3;
	*szGraphColor++ = ';';

	strcpy( szGraphColor, szColor4);
	szGraphColor += nColorLen4;
	*szGraphColor++ = ';';

	strcpy( szGraphColor, szColor5);
	szGraphColor += nColorLen5;
	*szGraphColor++ = ';';

	strcpy( szGraphColor, szColor6);
	szGraphColor += nColorLen6;
	*szGraphColor++ = ';';

	*szGraphColor++ = ( m_bUpGraphFill ? '1' : '0');
	*szGraphColor++ = ';';

	*szGraphColor++ = ( m_bDownGraphFill ? '1' : '0');
	*szGraphColor++ = ';';
}
*/
void CGraphColor::GetStringData( CString &p_strGraphColor) const
{
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	char szColor1[ 15], szColor2[ 15], szColor3[ 15], szColor4[ 15], szColor5[ 15], szColor6[ 15], szColor7[ 15], szColor8[ 15], szColor9[ 15], szColor10[ 15];
	
	int nColorLen1 = GetColorString( m_color1, szColor1);
	int nColorLen2 = GetColorString( m_color2, szColor2);
	int nColorLen3 = GetColorString( m_color3, szColor3);
	int nColorLen4 = GetColorString( m_color4, szColor4);
	int nColorLen5 = GetColorString( m_color5, szColor5);
	int nColorLen6 = GetColorString( m_color6, szColor6);
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	int nColorLen7 = GetColorString( m_color7, szColor7);
	int nColorLen8 = GetColorString( m_color8, szColor8);
	int nColorLen9 = GetColorString( m_color9, szColor9);
	int nColorLen10 = GetColorString( m_color10, szColor10);
	
	int nGraphColorLen = 0;
	nGraphColorLen += nColorLen1;
	nGraphColorLen++;
	nGraphColorLen += nColorLen2;
	nGraphColorLen++;
	nGraphColorLen += nColorLen3;
	nGraphColorLen++;
	nGraphColorLen += nColorLen4;
	nGraphColorLen++;
	nGraphColorLen += nColorLen5;
	nGraphColorLen++;
	nGraphColorLen += nColorLen6;
	nGraphColorLen++;
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	nGraphColorLen += nColorLen7;
	nGraphColorLen++;
	nGraphColorLen += nColorLen8;
	nGraphColorLen++;
	nGraphColorLen += nColorLen9;
	nGraphColorLen++;
	nGraphColorLen += nColorLen10;
	nGraphColorLen++;
	nGraphColorLen += 4;	// ?;?;	(Do not add NULL Terminator size. It has that with default)
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	nGraphColorLen += 4;	// ?;?;	(Do not add NULL Terminator size. It has that with default)
	
	char *szGraphColor = p_strGraphColor.GetBufferSetLength( nGraphColorLen);
	
	strcpy( szGraphColor, szColor1);
	szGraphColor += nColorLen1;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor2);
	szGraphColor += nColorLen2;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor3);
	szGraphColor += nColorLen3;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor4);
	szGraphColor += nColorLen4;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor5);
	szGraphColor += nColorLen5;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor6);
	szGraphColor += nColorLen6;
	*szGraphColor++ = ';';
	
	*szGraphColor++ = ( m_bUpPositiveGraphFill ? '1' : '0');
	*szGraphColor++ = ';';
	
	*szGraphColor++ = ( m_bUpNegativeGraphFill ? '1' : '0');
	*szGraphColor++ = ';';
	
	// »ó½Â, ¾çºÀ, ÇÏ¶ô, À½ºÀ »ö»ó ºÐ¸® - ojtaso (20080707)
	strcpy( szGraphColor, szColor7);
	szGraphColor += nColorLen7;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor8);
	szGraphColor += nColorLen8;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor9);
	szGraphColor += nColorLen9;
	*szGraphColor++ = ';';
	
	strcpy( szGraphColor, szColor10);
	szGraphColor += nColorLen10;
	*szGraphColor++ = ';';
	
	*szGraphColor++ = ( m_bDownPositiveGraphFill ? '1' : '0');
	*szGraphColor++ = ';';
	
	*szGraphColor++ = ( m_bDownNegativeGraphFill ? '1' : '0');
	*szGraphColor++ = ';';
}

// private ================================================================================
COLORREF CGraphColor::GetColorValue( const char *p_szColorData)
{
	COLORREF clrResult = 0;

	if( !p_szColorData)		return clrResult;
	if( !*p_szColorData)	return clrResult;

	CString strColorData;
	int nColorDataLen = strlen( p_szColorData);
	char *szColorData = strColorData.GetBufferSetLength( nColorDataLen);
	strcpy( szColorData, p_szColorData);

	char *szItem = szColorData;
	szColorData = strchr( szColorData, ',');
	if( szColorData) *szColorData++ = '\0';
	clrResult += atoi( szItem);
	if( !szColorData) return clrResult;
	if( !*szColorData) return clrResult;

	szItem = szColorData;
	szColorData = strchr( szColorData, ',');
	if( szColorData) *szColorData++ = '\0';
	clrResult += atoi( szItem) * 256;
	if( !szColorData) return clrResult;
	if( !*szColorData) return clrResult;

	szItem = szColorData;
	clrResult += atoi( szItem) * 65536;
	return clrResult;
}

int CGraphColor::GetColorString( COLORREF p_clrColor, char *p_szBuffer) const
{
	char *szBuffer = p_szBuffer;
	itoa( GetRValue( p_clrColor), szBuffer, 10);
	while( *szBuffer) szBuffer++;
	*szBuffer++ = ',';
	itoa( GetGValue( p_clrColor), szBuffer, 10);
	while( *szBuffer) szBuffer++;
	*szBuffer++ = ',';
	itoa( GetBValue( p_clrColor), szBuffer, 10);
	while( *szBuffer) szBuffer++;
	return szBuffer - p_szBuffer;
}
