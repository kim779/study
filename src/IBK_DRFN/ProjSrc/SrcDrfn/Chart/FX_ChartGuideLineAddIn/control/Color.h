// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLOR_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
#define AFX_COLOR_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// graph region 색
class CGraphRegionColor  
{
public:
	CGraphRegionColor& operator=(const CGraphRegionColor& data);

public:
	CGraphRegionColor();
	CGraphRegionColor(const COLORREF& backgroundColor, const COLORREF& lineColor, const COLORREF& frameColor);

	void SetBackgroundColor(const COLORREF& backgroundColor);
	void SetLineColor(const COLORREF& lineColor);
	void SetFrameColor(const COLORREF& frameColor);

	COLORREF GetBackgroundColor() const;
	COLORREF GetLineColor() const;
	COLORREF GetFrameColor() const;

private:
	COLORREF m_backgroundColor;	// 바탕색
	COLORREF m_lineColor;		// 모눈색
	COLORREF m_frameColor;		// 테두리색
};

// scale 색
class CScaleColor
{
public:
	CScaleColor& operator=(const CScaleColor& data);

public:
	CScaleColor();
	CScaleColor(const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor);

	void SetHorzScaleColor(const COLORREF& horzGridColor);
	void SetVertScaleColor(const COLORREF& vertGridColor);
	void SetScaleTextColor(const COLORREF& textColor);

	COLORREF GetHorzScaleColor() const;
	COLORREF GetVertScaleColor() const;
	COLORREF GetScaleTextColor() const;
	COLORREF& GetHorzScaleColor();
	COLORREF& GetVertScaleColor();
	COLORREF& GetScaleTextColor();

private:
	COLORREF m_horzGridColor;
	COLORREF m_vertGridColor;
	COLORREF m_textColor;
};

// 기타 graph 색
class CEtcGraphColor
{
public:
	CEtcGraphColor& operator=(const CEtcGraphColor& data);

public:
	CEtcGraphColor();
	CEtcGraphColor(const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6);

	COLORREF GetGraphColor1() const;
	COLORREF GetGraphColor2() const;
	COLORREF GetGraphColor3() const;
	COLORREF GetGraphColor4() const;
	COLORREF GetGraphColor5() const;
	COLORREF GetGraphColor6() const;
 
	void SetGraphColor1(const COLORREF& graphColor);
	void SetGraphColor2(const COLORREF& graphColor);
	void SetGraphColor3(const COLORREF& graphColor);
	void SetGraphColor4(const COLORREF& graphColor);
	void SetGraphColor5(const COLORREF& graphColor);
	void SetGraphColor6(const COLORREF& graphColor);

private:
	// 선 색들
	COLORREF m_graphColor1;
	COLORREF m_graphColor2;
	COLORREF m_graphColor3;
	COLORREF m_graphColor4;
	COLORREF m_graphColor5;
	COLORREF m_graphColor6;
};

// 기타 색
class CEtcColor
{
public:
	CEtcColor& operator=(const CEtcColor& data);

public:
	CEtcColor();
	CEtcColor(const COLORREF& toolColor, const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6);

	void SetToolColor(const COLORREF& toolColor);
	void SetEtcGraphColor(const CEtcGraphColor& etcGraphColor);
	void SetEtcGraphColor(const int count, const COLORREF& graphColor);

	COLORREF GetToolColor() const;
	CEtcGraphColor GetEtcGraphColor() const;

private:
	// 분석도구 색
	COLORREF m_toolColor;
	// 기타 그래프 색
	CEtcGraphColor m_etcGraphColor;
};

// graph 색
class CGraphColor  
{
public:
	CGraphColor& operator=(const CGraphColor& data);

public:
	CGraphColor();
	CGraphColor(const CString& strData);
	CGraphColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, const COLORREF& color4, const COLORREF& color5, const COLORREF& color6);
	CGraphColor(const CGraphColor& data);

	void SetData(const CString& strData);
	void SetData(const CGraphColor& graphColor);
	void SetColor(const COLORREF& color1, const COLORREF& color2, const COLORREF& color3, 
		const COLORREF& color4, const COLORREF& color5, const COLORREF& color6);
	void SetColor1(const COLORREF& color);
	void SetColor2(const COLORREF& color);
	void SetColor3(const COLORREF& color);
	void SetColor4(const COLORREF& color);
	void SetColor5(const COLORREF& color);
	void SetColor6(const COLORREF& color);
	void SetUpGraphFill(const bool isFill);
	void SetDownGraphFill(const bool isFill);

	COLORREF GetColor1() const;
	COLORREF GetColor2() const;
	COLORREF GetColor3() const;
	COLORREF GetColor4() const;
	COLORREF GetColor5() const;
	COLORREF GetColor6() const;
	bool GetUpGraphFill() const;
	bool GetDownGraphFill() const;

	CString GetStringData() const;
 
private:
	COLORREF m_color1;	// 상승 테두리 색
	COLORREF m_color2;	// 상승 색
	COLORREF m_color3;	// 하락 테두리 색
	COLORREF m_color4;	// 하락 색
	COLORREF m_color5;	// 보합 테두리 색
	COLORREF m_color6;	// 보합 색

	bool m_bUpGraphFill;	// 상승의 채움
	bool m_bDownGraphFill;	// 하락의 채움

	void SetColorData(const CString& strData, COLORREF& clrColor);
	void SetBoolData(const CString& strData, bool& bData);
};

#endif // !defined(AFX_COLOR_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
