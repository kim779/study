// PreView.cpp : implementation file
//

#include "stdafx.h"
#include "symbol.h"
#include "PreView.h"

#include "SymbolDraw.h"

//속성Data
#include "PropertiesData_text.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreView

CPreView::CPreView() :
	m_pProperties(NULL), 
	m_eSymbolTool(CSymbolBaseData::SYMBOL_ARROW_LEFT), 
	m_backgroundColor(RGB(255,255,255))
{
}

CPreView::~CPreView()
{
}


BEGIN_MESSAGE_MAP(CPreView, CStatic)
	//{{AFX_MSG_MAP(CPreView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreView message handlers

void CPreView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	GetClientRect(&m_rcThis);
	dc.FillSolidRect(&m_rcThis, m_backgroundColor);
	dc.DrawEdge(m_rcThis, BDR_SUNKENINNER | BDR_SUNKENOUTER, BF_TOPLEFT | BF_BOTTOMRIGHT);  

	CFont newfont;
	newfont.CreateFontIndirect(&GetLogFont());
	CFont* pOldFont = dc.SelectObject(&newfont);

	Draw(&dc);

	dc.SelectObject(pOldFont);
	newfont.DeleteObject();
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CPreView::OnEraseBkgnd(CDC* pDC) 
{
	return CStatic::OnEraseBkgnd(pDC);
//	return 0;
}

void CPreView::ModifySymbolType(const CSymbolBaseData::TOOLTYPE symboltool)
{
	m_eSymbolTool = symboltool;
	RedrawWindow();
}

void CPreView::Draw(CDC *pDC)
{
	CSize OneTextSize = GetOneTextSize(pDC);
	int drawptX = (int)(OneTextSize.cx / 2.0);
	int drawptY = (int)(OneTextSize.cy / 2.0);
	CPoint ptDrawing = m_rcThis.CenterPoint();
	if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT) {
		CTextDraw().DrawText(pDC, GetColor(), GetLogFont(), GetSizeType(), CPoint((m_rcThis.Width() / 2) - ((OneTextSize.cx * 15) / 2), m_rcThis.Height() / 2 - (OneTextSize.cy /2)), m_rcThis, "123ABC"); //"123가나다"
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_LEFT) {
		//45,30
		CArrow_Left_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) - 6, m_rcThis.Height() / 2), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_RIGHT) {
		//57,30
		CArrow_Right_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) + 6, m_rcThis.Height() / 2), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_UP) {
		//51,24
		CArrow_Up_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) + 6, m_rcThis.Height() / 2 - 6), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_DOWN) {
		//51,36
		CArrow_Down_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) + 6, m_rcThis.Height() / 2 + 6), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_LEFTTOP) {
		//45,24
		CArrow_LeftTop_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) - 6, m_rcThis.Height() / 2 - 6), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM) {
		//45,36
		CArrow_LeftBottom_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) - 6, m_rcThis.Height() / 2 + 6), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP) {
		//57, 24
		CArrow_RightTop_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) + 6, m_rcThis.Height() / 2 - 6), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM) {
		//57, 36
		CArrow_RightBottom_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), CPoint((m_rcThis.Width() / 2) + 6, m_rcThis.Height() / 2 + 6), m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_ONE) { //-6, -8
		//45,22
		CNumber_One_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_TWO) {
		CNumber_Two_Draw draw; 
		draw.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_THREE) {
		CNumber_Three_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_FOUR) {
		CNumber_Four_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_FIVE) {
		CNumber_Five_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_SIX) {
		CNumber_Six_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_SEVEN) {
		CNumber_Seven_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_EIGHT) {
		CNumber_Eight_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_NINE) {
		CNumber_Nine_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_NUM_TEN) {
		CNumber_Ten_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_A) {
		CText_A_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_B) {
		CText_B_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_C) {
		CText_C_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_D) {
		CText_D_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_E) {
		CText_E_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_F) {
		CText_F_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_G) {
		CText_G_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_TEXT_H) {
		CText_H_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_CHAR_1) {
		CChar_1_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_CHAR_2) {
		CChar_2_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_CHAR_3) {
		CChar_3_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_CHAR_4) {
		CChar_4_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_CHAR_5) {
		CChar_5_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
	else if(m_eSymbolTool == CSymbolBaseData::SYMBOL_CHAR_6) {
		CChar_6_Draw().Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), ptDrawing, m_rcThis);
	}
}

//dlg가 한번 close한 후에 다시 띄우면 미리보기에는 이전에 선택되어 있던 아이템이 그려져 있기때문에.
//사용자는 선택이 되어 있는줄 안다..
//그래서 dlg에서 symbolType을 얻어감.
CSymbolBaseData::TOOLTYPE CPreView::GetSymbolType()
{
	return m_eSymbolTool;
}

CSize CPreView::GetOneTextSize(CDC* pDC)
{
	//해당 font의 크기를 얻지 못하니까..logfont를 적용시킴.
	CFont font;
	font.CreateFontIndirect(&GetLogFont());
	CFont* oldFont = pDC->SelectObject(&font);

	CSize size(pDC->GetTextExtent(" "));
	pDC->SelectObject(oldFont);
	font.DeleteObject();
	return size;
}

void CPreView::SetBackgroundColor(const COLORREF& color)
{
	m_backgroundColor = color;
}

void CPreView::SetPropertiesData(CPropertiesData_text *pProperties)
{
	m_pProperties = pProperties;
}

COLORREF CPreView::GetColor() const
{
	return m_pProperties->GetColor();
}

LOGFONT CPreView::GetLogFont() const
{
	return m_pProperties->GetLogFont();
}

//sy 2005.04.06
COLORREF CPreView::GetTextColor() const
{
	return m_pProperties->GetTextColor();
}

CSymbolBaseData::SIZETYPE CPreView::GetSizeType() const
{
	return m_pProperties->GetSizeType();
}
