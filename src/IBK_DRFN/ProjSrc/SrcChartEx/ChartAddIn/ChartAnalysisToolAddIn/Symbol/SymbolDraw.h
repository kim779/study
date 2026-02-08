// SymbolDraw.h: interface for the CSymbolDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLDRAW_H__2CC980B5_2282_4EC5_B1E9_32AEB007CBCE__INCLUDED_)
#define AFX_SYMBOLDRAW_H__2CC980B5_2282_4EC5_B1E9_32AEB007CBCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Coordinate.h"
#include "SymbolBaseData.h"

class CSymbolDraw  
{
public:

protected:
	void SelectClipRegion(CDC* pDC, const CRect& rect);
	void SelectClipRegion(CDC* pDC);

	CRect GetGripRegion(const CPoint& centerPoint);
	COLORREF GetComplementaryColor(const COLORREF& color);
};


/////////////onepoint_size
///////////////////////////////
class CSymbolDraw_OnePoint_Size : public CSymbolDraw
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion) = 0;
	CSize DrawSpecialText(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion, const CString& drawingString);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion,BOOL bTextDraw = FALSE);

//protected:
//	virtual CSize RealDraw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion, const CString& drawingString) = 0;

private:
	CSize GetOneTextSize(CDC* pDC, const LOGFONT& logfont);
	CSize Draw_by_Size(CDC *pDC,const LOGFONT& logfont, const int nfontHeight, const int nfontWidth, const int totalsymbolsize, const CPoint &startPt, const CRect& drawingRegion, const CString& drawingString);

};

///////////twopoint_size
////////////////////////////
class CSymbolDraw_TwoPoint : public CSymbolDraw 
{
public:
	virtual CRect Draw(CDC *pDC, const COLORREF& color,
						const int& nLineStyle,
						const int& nLineWeight,
						const LOGFONT& logfont,
						const bool bFill,
						const CPoint &Startpt,
						const CPoint &Endpt,
						const CRect& drawingRegion,
						const bool bDrawFromTop) = 0;
	virtual void DrawRectangle(CDC* pDC, const CPoint& startPt, const CPoint& endPt, const CRect& drawingRegion) = 0;
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CPoint& endPt, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//DrawLine
//////////////////////////////////////////////////////////////////////
class CDrawLine  
{
public:
	static void DrawDashRectangle(CDC* pDC, const CPoint& startPt, const CPoint& endPt);
	static void DrawRectangle(CDC* pDC, const CPoint& startPt, const CPoint& endPt);

	static void DrawArrowHead(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& pt4, const CPoint& pt5);
	static void DrawArrowShaft(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& pt4);
	
private:
	static void DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& pt4, const CPoint& pt5);

};
/*
//SelectSymbolDraw
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CSelectDraw 
{
public:
	//선택되었을때 그린다.
	static void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& gap, const CSize& symbolsize, const CRect& drawingRegion);
};
*/
//도형 - 네모
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CFigure_Square_Draw : public CSymbolDraw_TwoPoint
{
public:
	virtual CRect Draw(CDC *pDC, 
						const COLORREF& color,
						const int& nLineStyle,
						const int& nLineWeight,
						const LOGFONT& logfont, 
						const bool bFill,
						const CPoint &Startpt,
						const CPoint &Endpt,
						const CRect& drawingRegion,
						const bool bDrawFromTop);

	virtual void DrawRectangle(CDC* pDC, const CPoint &startPt, const CPoint &endPt, const CRect& drawingRegion);
private:
	CRect DrawNoFill(CDC *pDC, const COLORREF& color,const int& nLineStyle,
					 const int& nLineWeight,const CPoint &Startpt, 
					 const CPoint &Endpt, const CRect& drawingRegion);
	CRect DrawFill(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion);
};

//도형 - 원형
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CFigure_Circle_Draw : public CSymbolDraw_TwoPoint
{
public:
	virtual CRect Draw(CDC *pDC,
						const COLORREF& color,
						const int& nLineStyle,
						const int& nLineWeight,
						const LOGFONT& logfont,
						const bool bFill, 
						const CPoint &Startpt,
						const CPoint &Endpt,
						const CRect& drawingRegion,
						const bool bDrawFromTop);

	virtual void DrawRectangle(CDC* pDC, const CPoint &startPt, const CPoint &endPt, const CRect& drawingRegion);

	void RealDrawSelectedMovingMark(CDC* pDC,
									const COLORREF& color,
									const CPoint &oldMovepoint, 
									const CPoint& MovePt,
									const CPoint& gap,
									const CSize& symbolsize,
									const CRect& drawingRegion,
									const bool bDrawFromTop);
	void DrawbyMovedSize(CDC* pDC, 
						 const CPoint& start_pt,
						 const CPoint& oldpoint,
						 const CPoint &Endpoint,
						 const CRect& drawingRegion);
private:
	CRect DrawNoFill(CDC *pDC, const COLORREF& color, const int& nLineStyle,
					 const int& nLineWeight,const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion);
	CRect DrawFill(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion);
};

//도형 - 삼각형
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CFigure_Triangle_Draw : public CSymbolDraw_TwoPoint
{
public:
	virtual CRect Draw(CDC *pDC, const COLORREF& color,
						const int& nLineStyle,
						const int& nLineWeight,
						const LOGFONT& logfont,
						const bool bFill,
						const CPoint &Startpt,
						const CPoint &Endpt,
						const CRect& drawingRegion,
						const bool bDrawFromTop);
	
	virtual void DrawRectangle(CDC* pDC, const CPoint &startPt, const CPoint &endPt, const CRect& drawingRegion);

	void RealDrawSelectedMovingMark(CDC* pDC,
									const COLORREF& color,
									const CPoint &oldMovepoint, 
									const CPoint& MovePt,
									const CPoint& gap, 
									const CSize& symbolsize, 
									const CRect& drawingRegion,
									const bool bDrawFromTop);
	void DrawbyMovedSize(CDC* pDC,
						 const CPoint& start_pt, 
						 const CPoint& oldpoint, 
						 const CPoint &Endpoint,
						 const CRect& drawingRegion,
						 const bool bDrawFromTop);
private:
	void DrawFillTriangle(CDC* pDC,
						  const CPoint& startPt,
						  const CPoint& endPt,
						  const bool bFromTop);

	void DrawNoFillTriangle(CDC* pDC, 
							const CPoint& startPt,
							const CPoint& endPt,
							const bool bFromTop);

	CRect DrawNoFill(CDC *pDC, const COLORREF& color,
					 const int& nLineStyle,
					 const int& nLineWeight,
					 const CPoint &Startpt,
					 const CPoint &Endpt, 
					 const CRect& drawingRegion,
					 const bool bFromTop);

	CRect DrawFill(CDC *pDC, const COLORREF& color,
					const LOGFONT& logfont, 
					const CPoint &Startpt,
					const CPoint &Endpt,
					const CRect& drawingRegion,
					const bool bFromTop);

};

//textDraw
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CTextTool;
class CDrawingSymbol;
class CTextEdit;
class CTextDraw : public CSymbolDraw_OnePoint_Size
{
public:
	CTextDraw();
	virtual ~CTextDraw();

	//Edit 생성하고 그린다.
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	//TextTool에 Edit를 Delete해야 할 시점을 알려준다.
	void SetTextToolObject(CTextTool* pTextTool);
	//Edit를 Delete하고 Edit에서 입력된text만 찍는다.
	CSize DrawText(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &point, const CRect& drawingRegion, const CString& strText);
	//Edit 생성을 위한 부모 윈도우
	void SetParent(CWnd* pParent);
	void SetDrawingRegion(const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion,BOOL bTextDraw = FALSE);
	void DeleteTextEdit();

private:
	CRect m_drawingRegion;
	CTextTool * m_pTextTool;
	CFont m_font;
	CWnd* m_pParent;

	CSize GetEditSize(CDC* pDC, const LOGFONT& logfont, const CPoint& startPt);
	CString GetExtremeLongWidthofText(const CString &text);
};

//////////////////////////////////////////////////////////////////////
//LeftArrowDraw
//////////////////////////////////////////////////////////////////////

class CArrow_Left_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
};

//////////////////////////////////////////////////////////////////////
//ArrowRightDraw
//////////////////////////////////////////////////////////////////////

class CArrow_Right_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);

};


//////////////////////////////////////////////////////////////////////
//ArrowUPDraw
//////////////////////////////////////////////////////////////////////

class CArrow_Up_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);

};


//////////////////////////////////////////////////////////////////////
//ArrowDownDraw
//////////////////////////////////////////////////////////////////////

class CArrow_Down_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//ArrowLEFTTOPDraw
//////////////////////////////////////////////////////////////////////

class CArrow_LeftTop_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//ArrowLEFTBottomDraw
//////////////////////////////////////////////////////////////////////

class CArrow_LeftBottom_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//ArrowRightTopDraw
//////////////////////////////////////////////////////////////////////

class CArrow_RightTop_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);

private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);

};


//////////////////////////////////////////////////////////////////////
//ArrowRightBottomDraw
//////////////////////////////////////////////////////////////////////

class CArrow_RightBottom_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
	void DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion);


private:
	CSize Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
	CSize Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//Num_One_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_One_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//Num_Two_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Two_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//Num_Three_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Three_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};



//////////////////////////////////////////////////////////////////////
//Num_Four_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Four_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};


//////////////////////////////////////////////////////////////////////
//Num_Five_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Five_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Num_Six_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Six_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Num_Seven_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Seven_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Num_Eight_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Eight_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Num_Nine_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Nine_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Num_Ten_Draw
//////////////////////////////////////////////////////////////////////

class CNumber_Ten_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Text_A_Draw
//////////////////////////////////////////////////////////////////////

class CText_A_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};


//////////////////////////////////////////////////////////////////////
//Text_B_Draw
//////////////////////////////////////////////////////////////////////

class CText_B_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};


//////////////////////////////////////////////////////////////////////
//Text_C_Draw
//////////////////////////////////////////////////////////////////////

class CText_C_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Text_D_Draw
//////////////////////////////////////////////////////////////////////

class CText_D_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Text_E_Draw
//////////////////////////////////////////////////////////////////////

class CText_E_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Text_F_Draw
//////////////////////////////////////////////////////////////////////

class CText_F_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Text_G_Draw
//////////////////////////////////////////////////////////////////////

class CText_G_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Text_H_Draw
//////////////////////////////////////////////////////////////////////

class CText_H_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);

};

//////////////////////////////////////////////////////////////////////
//Char_1_Draw
//////////////////////////////////////////////////////////////////////

class CChar_1_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};

//////////////////////////////////////////////////////////////////////
//Char_2_Draw
//////////////////////////////////////////////////////////////////////

class CChar_2_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};

//////////////////////////////////////////////////////////////////////
//Char_3_Draw
//////////////////////////////////////////////////////////////////////

class CChar_3_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};

//////////////////////////////////////////////////////////////////////
//Char_4_Draw
//////////////////////////////////////////////////////////////////////

class CChar_4_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};

//////////////////////////////////////////////////////////////////////
//Char_5_Draw
//////////////////////////////////////////////////////////////////////

class CChar_5_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};

//////////////////////////////////////////////////////////////////////
//Char_6_Draw
//////////////////////////////////////////////////////////////////////

class CChar_6_Draw : public CSymbolDraw_OnePoint_Size
{
public:
	virtual CSize Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion);
	void DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion);
};

#endif // !defined(AFX_SYMBOLDRAW_H__2CC980B5_2282_4EC5_B1E9_32AEB007CBCE__INCLUDED_)
