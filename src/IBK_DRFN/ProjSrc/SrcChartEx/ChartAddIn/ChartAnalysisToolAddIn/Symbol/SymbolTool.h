// SymbolTool.h: interface for the CSymbolTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLTOOL_H__D8FA6505_C879_45D2_9F4C_D40DC73E3397__INCLUDED_)
#define AFX_SYMBOLTOOL_H__D8FA6505_C879_45D2_9F4C_D40DC73E3397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/CoordinateCalculatorData.h"
#include "CoordinateCalculator.h"

#include "SymbolBaseData.h"
#include "BlockBaseData.h"

#include "SymbolDraw.h"
#include "Coordinate.h"	// Added by ClassView

class CDrawingSymbolRow;
class CDrawingSymbol;
class CSymbolData;
class CSymbolTool  
{
public:
	CSymbolData* GetSymbolData();
	CSymbolTool();

	virtual void Initial() = 0;
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point) = 0;
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point) = 0;
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point) = 0;
	virtual bool IsPointInBlock(const CRect &drawingRegion, const CPoint &point);
	virtual void OnDraw(CDC* pDC) = 0;

	void SetDrawStart(const bool bDrawStart);
	void SetSymbolData(CSymbolData* pSymbolData);

	bool IsDrawStart();

	CCoordinateCalculatorData GetCoordinateCalculatorData(const CPoint& point) const;
protected:
	void OnLButtonUp(CDC* pDC, const CPoint& startpoint, const CPoint& endpoint, const CSymbolBaseData::TOOLTYPE toolType);
	bool m_bDrawStart;

	void ChangeCursor(const CCursorShape::CHARTOBJECT cursorShape);

	COLORREF GetColor();
	LOGFONT GetLogFont();

	CRect GetCurDrawingRegion(const CPoint &point);
	virtual void SavePointAndDraw(CDC* pDC, const CPoint& point, const int drawMode) = 0;
	virtual void AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint) = 0;
	CCoordinate GetSavingPoint(const CPoint& point);
	CPoint GetDrawingPoint(const CPoint& point);

private:	
	CSymbolData* m_pSymbolData;

protected:
	HWND m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

//////////////////////////////////////////////////////////////////////
// OnePoint and Size Tool
//////////////////////////////////////////////////////////////////////
class CSymbolTool_OnePoint_Size : public CSymbolTool
{
public:
	virtual void Initial();
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point)=0;
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);
	virtual void OnDraw(CDC* pDC);

	CPoint GetStartPoint();
	CSize GetSymbolSize();

	void SetStartPoint(const CPoint& startpt);
	CPoint GetDrawingPoint();
protected:
	void OnLButtonUp(CDC* pDC, const CPoint& point, const CSymbolBaseData::TOOLTYPE toolType);
	CPoint m_startPt;
	CSize m_SymbolSize;

	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer() =0;
	virtual void SavePointAndDraw(CDC* pDC, const CPoint& point, const int drawMode);
	virtual void AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint);

private:	
	CPoint m_ptDrawingPoint;
	bool GetBlockIndex(IChartManager* pMainBlock, const CPoint& point, CBlockIndex& blockIndex) const;
	CDrawingSymbol* MakeDrawingSymbol(CSymbolData* pSymbolData, const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint);
};

//////////////////////////////////////////////////////////////////////
// TwoPoint
//////////////////////////////////////////////////////////////////////
class CSymbolTool_TwoPoint : public CSymbolTool
{
public:
	virtual void OnDraw(CDC* pDC);
	virtual void Initial();
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point) =0;
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);

	CPoint GetEndPoint();
	CPoint GetStartPoint();

	void SetStartPoint(const CPoint& startpt);
	void SetEndPoint(const CPoint& endpt);

protected:
	void OnLButtonUp(CDC* pDC, const CPoint& point, const CSymbolBaseData::TOOLTYPE toolType);
	CPoint m_EndPt;
	CPoint m_StartPt;

	virtual CSymbolDraw_TwoPoint& GetTwoPointDrawer() = 0;
	virtual void SavePointAndDraw(CDC* pDC, const CPoint& point, const int drawMode);
	virtual void AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint);
private:
	bool m_bDrawFromTop;
	void ReplaceStartandEndpt(const CPoint& currentPt);
};

//Select
//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

class CSelectTool : public CSymbolTool 
{
public:
	CSelectTool();
	virtual void Initial();
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);

	virtual void OnDraw(CDC* pDC);

protected:
	virtual void SavePointAndDraw(CDC* pDC, const CPoint& point, const int drawMode);
	virtual void AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint);

private:
	bool IsFigure();
	void OnMouseMove_Symbol(CDC *pDC, const CPoint &point);
	void OnMouseMove_Figure(CDC* pDC, const CPoint& point);
	void ChangeMousePosition(const CPoint& curpos);
	CPoint m_ptOldMouse;
	CPoint m_oldMovePoint;
	CPoint m_LButtonPoint;
	//선택된 Symbol의 포인터.
	CDrawingSymbol* m_pDrawingSymbol;

	CSymbolBaseData::ModifyType m_eModifyType;

	CRect Draw(CDC* pDC, CDrawingSymbolRow* pDrawingSymbolMgr);
	CPoint GetGapMouseMove(const CPoint& pt1, const CPoint& pt2);
};

//도형 - 네모
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CFigure_Square_Tool : public CSymbolTool_TwoPoint  
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);

protected:
	CSymbolDraw_TwoPoint& GetTwoPointDrawer();

private:
	bool GetRightPoint(const CPoint &startpoint, const CPoint &endpoint);

	CFigure_Square_Draw m_squaredrawer;
};

//도형 - 동그라미
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CFigure_Circle_Tool : public CSymbolTool_TwoPoint  
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);

protected:
	virtual CSymbolDraw_TwoPoint& GetTwoPointDrawer();

private:
	CFigure_Circle_Draw m_circledrawer;
};

//도형 - 삼각형
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CFigure_Triangle_Tool : public CSymbolTool_TwoPoint  
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);
	virtual bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);

protected:
	virtual CSymbolDraw_TwoPoint& GetTwoPointDrawer();

private:
	CFigure_Triangle_Draw m_triangleDrawer;
};

//text
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CTextDraw;
class CTextTool : public CSymbolTool_OnePoint_Size  
{
public:
	CWnd* GetParent();
	void SetEditText(const CString& strText);
	bool Delete();

	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

	virtual bool IsPointInBlock(const CRect &drawingRegion, const CPoint &point);
	CPoint GetStartPoint();

	LOGFONT GetLogFont();
protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CTextDraw m_textDrawer;
};

//LeftArrow
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CArrow_Left_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_Left_Draw m_arrowleftDrawer;
};

//rightArrow
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CArrow_Right_Tool : public CSymbolTool_OnePoint_Size  
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_Right_Draw m_arrowrightDrawer;
};

//////////////////////////////////////////////////////////////////////
////ARROW_UP 
//////////////////////////////////////////////////////////////////////

class CArrow_Up_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_Up_Draw m_arrowUpDrawer;
};

//////////////////////////////////////////////////////////////////////
////ARROW_DOWN 
//////////////////////////////////////////////////////////////////////
class CArrow_Down_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_Down_Draw m_arrowdownDrawer;
};

//////////////////////////////////////////////////////////////////////
////ARROW_LEFTTOP 
//////////////////////////////////////////////////////////////////////

class CArrow_LeftTop_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_LeftTop_Draw m_arrowLeftTopDrawer;
};

//////////////////////////////////////////////////////////////////////
////ARROW_LEFTBOTTOM
//////////////////////////////////////////////////////////////////////

class CArrow_LeftBottom_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_LeftBottom_Draw m_arrowLeftBottomDrawer;
};

//////////////////////////////////////////////////////////////////////
////ARROW_RIGHTTOP
//////////////////////////////////////////////////////////////////////

class CArrow_RightTop_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_RightTop_Draw m_arrowRightTopDrawer;
};

//////////////////////////////////////////////////////////////////////
////ARROW_RIGHTBOTTOM
//////////////////////////////////////////////////////////////////////

class CArrow_RightBottom_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CArrow_RightBottom_Draw m_arrowRightBottomDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_ONE
//////////////////////////////////////////////////////////////////////

class CNumber_One_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_One_Draw m_numberOneDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_TWO
//////////////////////////////////////////////////////////////////////

class CNumber_Two_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Two_Draw m_numberTwoDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_THREE
//////////////////////////////////////////////////////////////////////

class CNumber_Three_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Three_Draw m_numberThreeDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_FOUR
//////////////////////////////////////////////////////////////////////

class CNumber_Four_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Four_Draw m_numberFourDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_Five
//////////////////////////////////////////////////////////////////////

class CNumber_Five_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Five_Draw m_numberFiveDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_Six
//////////////////////////////////////////////////////////////////////

class CNumber_Six_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Six_Draw m_numberSixDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_Seven
//////////////////////////////////////////////////////////////////////

class CNumber_Seven_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Seven_Draw m_numberSevenDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_Eight
//////////////////////////////////////////////////////////////////////

class CNumber_Eight_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Eight_Draw m_numberEightDrawer;
};

//////////////////////////////////////////////////////////////////////
////NUM_Nine
//////////////////////////////////////////////////////////////////////

class CNumber_Nine_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CNumber_Nine_Draw m_numberNineDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_A
//////////////////////////////////////////////////////////////////////

class CText_A_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_A_Draw m_textADrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_B
//////////////////////////////////////////////////////////////////////

class CText_B_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_B_Draw m_textBDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_C
//////////////////////////////////////////////////////////////////////

class CText_C_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_C_Draw m_textCDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_D
//////////////////////////////////////////////////////////////////////

class CText_D_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_D_Draw m_textDDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_E
//////////////////////////////////////////////////////////////////////

class CText_E_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_E_Draw m_textEDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_F
//////////////////////////////////////////////////////////////////////

class CText_F_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_F_Draw m_textFDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_G
//////////////////////////////////////////////////////////////////////

class CText_G_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_G_Draw m_textGDrawer;
};

//////////////////////////////////////////////////////////////////////
////TEXT_H
//////////////////////////////////////////////////////////////////////

class CText_H_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CText_H_Draw m_textHDrawer;
};

//////////////////////////////////////////////////////////////////////
////CHAR_1
//////////////////////////////////////////////////////////////////////

class CChar_1_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CChar_1_Draw m_char1Drawer;
};

//////////////////////////////////////////////////////////////////////
////CHAR_2
//////////////////////////////////////////////////////////////////////

class CChar_2_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CChar_2_Draw m_char2Drawer;
};

//////////////////////////////////////////////////////////////////////
////CHAR_3
//////////////////////////////////////////////////////////////////////

class CChar_3_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CChar_3_Draw m_char3Drawer;
};

//////////////////////////////////////////////////////////////////////
////CHAR_4
//////////////////////////////////////////////////////////////////////

class CChar_4_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CChar_4_Draw m_char4Drawer;
};

//////////////////////////////////////////////////////////////////////
////CHAR_5
//////////////////////////////////////////////////////////////////////

class CChar_5_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CChar_5_Draw m_char5Drawer;
};

//////////////////////////////////////////////////////////////////////
////CHAR_6
//////////////////////////////////////////////////////////////////////

class CChar_6_Tool : public CSymbolTool_OnePoint_Size
{
public:
	virtual CDrawingSymbol* OnLButtonDown(CDC *pDC, const CPoint &point);
	virtual void OnLButtonUp(CDC *pDC, const CPoint &point);

protected:
	virtual CSymbolDraw_OnePoint_Size& GetOnePointDrawer();

private:
	CChar_6_Draw m_char6Drawer;
};
#endif // !defined(AFX_SYMBOLTOOL_H__D8FA6505_C879_45D2_9F4C_D40DC73E3397__INCLUDED_)
