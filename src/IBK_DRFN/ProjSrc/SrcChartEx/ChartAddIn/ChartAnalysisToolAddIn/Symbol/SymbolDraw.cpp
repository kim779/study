// SymbolDraw.cpp: implementation of the CSymbolDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "SymbolDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "SymbolBaseData.h"
#include "Conversion.h"

#include "TextEdit.h"
//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
// class CDraw
// 그리는 영역을 정한다.
void CSymbolDraw::SelectClipRegion(CDC* pDC, const CRect& rect)
{
	CRgn rgn;
	rgn.CreateRectRgn((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom);
	pDC->SelectClipRgn(&rgn);
}

// 그리는 영역기본값으로 돌린다.
void CSymbolDraw::SelectClipRegion(CDC* pDC)
{
	pDC->SelectClipRgn(NULL);
}

// grip 영역
CRect CSymbolDraw::GetGripRegion(const CPoint& centerPoint)
{
	CRect gripRegion(centerPoint, CSize(1, 1));
	gripRegion.InflateRect(3, 3);
	return gripRegion;
}


COLORREF CSymbolDraw::GetComplementaryColor(const COLORREF& color)
{
	int rValue = GetRValue(color);
	int gValue = GetGValue(color);
	int bValue = GetBValue(color);
	
	return RGB(255 - rValue, 255 - gValue, 255- bValue);
}

///////////////onepoint_size
CSize CSymbolDraw_OnePoint_Size::DrawSpecialText(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion, const CString& drawingString)
{
	if(pDC == NULL)
		return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->SetBkColor(color);
	
	COLORREF crText(textColor);
	if( drawingString != "☞" &&
		drawingString != "☜" &&
		drawingString != "◆" &&
		drawingString != "★" &&
		drawingString != "●" &&
		drawingString != "■")
	{
		if(color == textColor)
			crText = (~crText) & 0x00FFFFFF;
	}
	else
	{
		crText = color;
	}

	COLORREF oldTextColor = pDC->SetTextColor(crText);

	CSize size;
	CSize onetextsize = GetOneTextSize(pDC, logfont);
	if(drawingString == "10")
	{
		onetextsize.cx = onetextsize.cx*2;
	}

	if(sizetype == CSymbolBaseData::Small)
		size = Draw_by_Size(pDC, logfont, onetextsize.cy, onetextsize.cx, onetextsize.cy + 3, startPt, drawingRegion, drawingString); //small
	else if(sizetype == CSymbolBaseData::Medium)
		size = Draw_by_Size(pDC, logfont, (int)(onetextsize.cy * 1.3), (int)(onetextsize.cx * 1.3), (int)(onetextsize.cy * 1.3) + 3, startPt, drawingRegion, drawingString); //medium
	else if(sizetype == CSymbolBaseData::Large)
		size = Draw_by_Size(pDC, logfont, (int)(onetextsize.cy * 1.5), (int)(onetextsize.cx * 1.5), (int)(onetextsize.cy * 1.5) + 3, startPt, drawingRegion, drawingString); //Large
		
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetTextColor(oldTextColor);
	return size;
}

CSize CSymbolDraw_OnePoint_Size::Draw_by_Size(CDC *pDC,const LOGFONT& logfont, const int nfontHeight, const int nfontWidth, const int totalsymbolsize, const CPoint &startPt, const CRect& drawingRegion, const CString& drawingString)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf = logfont;

	lf.lfHeight = nfontHeight; 
	lf.lfWidth = nfontWidth;  
//	lf.lfWeight = FW_NORMAL;

	CFont newfont;
	newfont.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&newfont);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);	
	if(drawingString == "10")
	{
		pDC->Ellipse(startPt.x, startPt.y, startPt.x + (long)(totalsymbolsize*1.5), startPt.y + (long)(totalsymbolsize*1.5)); 
		pDC->DrawText(drawingString, 2, &CRect(startPt.x, startPt.y, startPt.x + (long)(totalsymbolsize*1.5), startPt.y + (long)(totalsymbolsize*1.5)), DT_SINGLELINE | DT_CENTER | DT_VCENTER );   
	}
	else
	{
		if( drawingString != "☞" &&
			drawingString != "☜" &&
			drawingString != "◆" &&
			drawingString != "★" &&
			drawingString != "●" &&
			drawingString != "■")
		{
			pDC->Ellipse(startPt.x-totalsymbolsize/2+1, startPt.y-totalsymbolsize/2, startPt.x + totalsymbolsize/2+1, startPt.y + totalsymbolsize/2); 
		}		
		else
		{
			pDC->SetBkMode(TRANSPARENT);
		}
		pDC->DrawText(drawingString,&CRect(startPt.x-totalsymbolsize/2+1, startPt.y-totalsymbolsize/2, startPt.x + totalsymbolsize/2+1, startPt.y + totalsymbolsize/2), DT_SINGLELINE | DT_CENTER | DT_VCENTER);   
	}	
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(pOldFont);
	newfont.DeleteObject();

	return CSize(totalsymbolsize, totalsymbolsize);
}

CSize CSymbolDraw_OnePoint_Size::GetOneTextSize(CDC *pDC, const LOGFONT &logfont)
{
	CFont font;
	font.CreateFontIndirect(&logfont);
	CFont* pOldFont = pDC->SelectObject(&font);

	CSize size(pDC->GetTextExtent(" "));
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
	return size;
}

void CSymbolDraw_OnePoint_Size::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion,BOOL bTextDraw)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	if(bTextDraw == FALSE)
	{
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx/2,startPt.y - symbolsize.cy/2)));
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx/2, startPt.y + symbolsize.cy/2)));
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx/2, startPt.y - symbolsize.cy/2)));
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx/2, startPt.y + symbolsize.cy/2)));
	}
	else
	{
		pDC->Rectangle(CSymbolDraw::GetGripRegion(startPt));
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x, startPt.y + symbolsize.cy)));
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx, startPt.y)));
		pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx, startPt.y + symbolsize.cy)));
	}
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*CRect CSymbolDraw_TwoPoint::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion)
{
	CDC dcCompatible;
    CBitmap bm;
	//비트맵, DC 생성.
    dcCompatible.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, Endpt.x - Startpt.x, Endpt.y - Startpt.y);
    CBitmap *pBitmapOld = dcCompatible.SelectObject(&bm);

	RealDraw();

	dcCompatible.SelectObject(pBitmapOld);
	bm.DeleteObject();
	dcCompatible.DeleteDC();

	CRect DrawingRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
	return DrawingRect;
}*/

void CSymbolDraw_TwoPoint::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CPoint& endPt, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, RGB(0,0,0));
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	pDC->Rectangle(CSymbolDraw::GetGripRegion(startPt));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x, endPt.y)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(endPt.x, startPt.y)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(endPt));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

//////////////////////////////////////////////////////////////////////
// CDrawLine
//////////////////////////////////////////////////////////////////////

void CDrawLine::DrawRectangle(CDC* pDC, const CPoint& startPt, const CPoint& endPt)
{
	DrawLine(pDC, CPoint(endPt.x, endPt.y), CPoint(startPt.x, endPt.y),
			 CPoint(startPt.x, startPt.y), CPoint(endPt.x, startPt.y), CPoint(endPt.x, endPt.y));
}

void CDrawLine::DrawArrowHead(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& pt4, const CPoint& pt5)
{
	DrawLine(pDC, pt1, pt2, pt3, pt4, pt5);
}

void CDrawLine::DrawArrowShaft(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& pt4)
{
	pDC->MoveTo(pt1.x, pt1.y);
	pDC->LineTo(pt2.x, pt2.y);
	pDC->LineTo(pt3.x, pt3.y);
	pDC->LineTo(pt4.x, pt4.y);
}

void CDrawLine::DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2, const CPoint& pt3, const CPoint& pt4, const CPoint& pt5)
{
	pDC->MoveTo(pt1.x, pt1.y);
	pDC->LineTo(pt2.x, pt2.y);
	pDC->LineTo(pt3.x, pt3.y);
	pDC->LineTo(pt4.x, pt4.y);
	pDC->LineTo(pt5.x, pt5.y);
}

//////////////////////////////////////////////////////////////////////
// SelectDraw
//////////////////////////////////////////////////////////////////////
/*
//////////////////////////////////////////////////////////////////////
void CSelectDraw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& gap, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 중심으로 잡기 위해서 Lbutton한 시점과 저장되어 있던 시작점의 차이를 뺀다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - gap.x, oldMovepoint.y - gap.y), 
				 CPoint(oldMovepoint.x - gap.x + symbolsize.cx + CSymbolBaseData::interval, oldMovepoint.y - gap.y + symbolsize.cy + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - gap.x, MovePt.y - gap.y), 
			 CPoint(MovePt.x - gap.x + symbolsize.cx + CSymbolBaseData::interval, MovePt.y - gap.y + symbolsize.cy + CSymbolBaseData::interval));
	 pDC->SetROP2( ropOld );
//point가 중심.
/////////////
	int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - (symbolsize.cx / 2) - CSymbolBaseData::interval, oldMovepoint.y - (symbolsize.cy / 2) - CSymbolBaseData::interval), 
			 CPoint(oldMovepoint.x + (symbolsize.cx / 2) + CSymbolBaseData::interval, oldMovepoint.y + (symbolsize.cy / 2) + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - (symbolsize.cx / 2) - CSymbolBaseData::interval, MovePt.y - (symbolsize.cy / 2) - CSymbolBaseData::interval), 
			 CPoint(MovePt.x + (symbolsize.cx / 2) + CSymbolBaseData::interval, MovePt.y + (symbolsize.cy / 2) + CSymbolBaseData::interval));
     pDC->SetROP2( ropOld );
////////////////
}
*/
//도형 - 네모
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CFigure_Square_Draw::DrawRectangle(CDC* pDC, const CPoint &startPt, const CPoint &endPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	pDC->MoveTo(endPt.x, endPt.y);
	pDC->LineTo(startPt.x, endPt.y);
	pDC->LineTo(startPt.x, startPt.y);
	pDC->LineTo(endPt.x, startPt.y);
	pDC->LineTo(endPt.x, endPt.y);
	CSymbolDraw::SelectClipRegion(pDC);
}

CRect CFigure_Square_Draw::Draw(CDC *pDC,
								const COLORREF& color,
								const int& nLineStyle,
								const int& nLineWeight,
								const LOGFONT& logfont,
								const bool bFill,
								const CPoint &Startpt,
								const CPoint &Endpt,
								const CRect& drawingRegion,
								const bool bDrawFromTop)
{
	if(bFill == true) 
		return DrawFill(pDC, color, logfont, Startpt, Endpt, drawingRegion);

	return DrawNoFill(pDC, color, nLineStyle,nLineWeight, Startpt, Endpt, drawingRegion);
}

CRect CFigure_Square_Draw::DrawFill(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion)
{
	CDC dcCompatible;
    CBitmap bm;
	//비트맵, DC 생성.
    dcCompatible.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, Endpt.x - Startpt.x, Endpt.y - Startpt.y);
    CBitmap *pBitmapOld = dcCompatible.SelectObject(&bm);

    dcCompatible.FillSolidRect(CRect(0, 0, Endpt.x - Startpt.x, Endpt.y - Startpt.y), color); //RGB(0,200,255));//RGB(0x7F, 0x7F, 0x7F)

    CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
    pDC->BitBlt(Startpt.x, Startpt.y, Endpt.x - Startpt.x, Endpt.y - Startpt.y, &dcCompatible, 0, 0, SRCAND);
	//nami 10.14
	//같은 color로 하면 테두리가 보이지 않는다.
	//그래서 보합색으로 처리
	//DrawNoFill(pDC, GetComplementaryColor(color), logfont, Startpt, Endpt, drawingRegion);
	//DrawNoFill(pDC,color, logfont, Startpt, Endpt, drawingRegion);

	CPen pen(PS_SOLID, 1, color);
	CPen* pOldPen = pDC->SelectObject(&pen);

	DrawRectangle(pDC, Startpt, Endpt, drawingRegion);
	pDC->SelectObject(pOldPen);

	CSymbolDraw::SelectClipRegion(pDC);

	dcCompatible.SelectObject(pBitmapOld);
	bm.DeleteObject();
	dcCompatible.DeleteDC();

	return CRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);

}

CRect CFigure_Square_Draw::DrawNoFill(CDC *pDC,const COLORREF& color,const int& nLineStyle,
									  const int& nLineWeight,const CPoint &Startpt, 
									  const CPoint &Endpt, const CRect& drawingRegion)
{
	CPen pen(nLineStyle,nLineWeight,color);
	CPen* pOldPen = pDC->SelectObject(&pen);

    CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	DrawRectangle(pDC, Startpt, Endpt, drawingRegion);
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(pOldPen);

	CRect DrawingRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
	return DrawingRect;
}

//도형 - 원형
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CFigure_Circle_Draw::RealDrawSelectedMovingMark(CDC* pDC,
													 const COLORREF& color,
													 const CPoint &oldMovepoint,
													 const CPoint& MovePt, 
													 const CPoint& gap,
													 const CSize& symbolsize, 
													 const CRect& drawingRegion,
													 const bool bDrawFromTop)
{
	 //point가 시작점.
	 //마우스를 중심으로 잡기 위해서 Lbutton한 시점과 저장되어 있던 시작점의 차이를 뺀다.
	 int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     /*if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - gap.x, oldMovepoint.y - gap.y), 
				 CPoint(oldMovepoint.x - gap.x + symbolsize.cx, oldMovepoint.y - gap.y + symbolsize.cy));

		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
  	    pDC->Ellipse(oldMovepoint.x - gap.x +1, oldMovepoint.y - gap.y+1, oldMovepoint.x - gap.x + symbolsize.cx, oldMovepoint.y - gap.y + symbolsize.cy);
		pDC->SelectObject(pOldPen);
	 }*/
	 //CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - gap.x, MovePt.y - gap.y), 
	 //		 CPoint(MovePt.x - gap.x + symbolsize.cx, MovePt.y - gap.y + symbolsize.cy));

	 CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	 CPen* pOldPen = pDC->SelectObject(&pen);
	 pDC->Ellipse(MovePt.x - gap.x+1, MovePt.y - gap.y+1, MovePt.x - gap.x + symbolsize.cx, MovePt.y - gap.y + symbolsize.cy);
	 pDC->SelectObject(pOldPen);
	 pDC->SetROP2( ropOld );
} 


void CFigure_Circle_Draw::DrawbyMovedSize(CDC* pDC,
										  const CPoint& start_pt,
										  const CPoint& oldpoint,
										  const CPoint &Endpoint,
										  const CRect& drawingRegion)
{
	int ropOld = pDC->SetROP2(R2_NOTXORPEN);
	/*if(oldpoint.x != 0 && oldpoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, start_pt, oldpoint);
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->Ellipse(start_pt.x + 1, start_pt.y + 1 , oldpoint.x, oldpoint.y);
		pDC->SelectObject(pOldPen);
	}*/
	//CDrawLine::DrawRectangle(pDC, start_pt, Endpoint);

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Ellipse(start_pt.x + 1, start_pt.y + 1, Endpoint.x, Endpoint.y);
	pDC->SelectObject(pOldPen);
    pDC->SetROP2( ropOld );
}

//처음으로 마우스로 그림을 그릴때 호출 되는 함수.
void CFigure_Circle_Draw::DrawRectangle(CDC* pDC, const CPoint &startPt, const CPoint &endPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	pDC->Ellipse(startPt.x, startPt.y, endPt.x, endPt.y);
	CSymbolDraw::SelectClipRegion(pDC);
}

CRect CFigure_Circle_Draw::Draw(CDC *pDC,
								const COLORREF& color,
								const int& nLineStyle,
								const int& nLineWeight,
								const LOGFONT& logfont,
								const bool bFill,
								const CPoint &Startpt,
								const CPoint &Endpt,
								const CRect& drawingRegion,
								const bool bDrawFromTop)
{
	if(bFill == true) 
		return DrawFill(pDC, color, logfont, Startpt, Endpt, drawingRegion);

	return DrawNoFill(pDC, color, nLineStyle,nLineWeight, Startpt, Endpt, drawingRegion);
}

CRect CFigure_Circle_Draw::DrawFill(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion)
{
	CDC dcCompatible;
    CBitmap bm;
	//비트맵, DC 생성.
    dcCompatible.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, Endpt.x - Startpt.x, Endpt.y - Startpt.y);
    CBitmap *pBitmapOld = dcCompatible.SelectObject(&bm);

	//펜, 브러쉬생성.
	CBrush brush(color);
	//CPen pen(PS_SOLID, 1, GetComplementaryColor(color)); //color로 펜을 생성하면 테두리가 안보인다.
	CPen pen(PS_SOLID, 1, color); //color로 펜을 생성하면 테두리가 안보인다.
	CPen* pOldPen = dcCompatible.SelectObject(&pen);
	CBrush* pOldBrush = dcCompatible.SelectObject(&brush);

	//화면 메모리 Dc에 복사하기.
	dcCompatible.BitBlt(0, 0, Endpt.x - Startpt.x, Endpt.y - Startpt.y,
						pDC, Startpt.x, Startpt.y, SRCCOPY);
	dcCompatible.Ellipse(CRect(0, 0, Endpt.x - Startpt.x, Endpt.y - Startpt.y)); 
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
    pDC->BitBlt(Startpt.x, Startpt.y, Endpt.x - Startpt.x, Endpt.y - Startpt.y, &dcCompatible, 0, 0, SRCAND);
	//nami 10.14
	//같은 color로 하면 테두리가 보이지 않는다.
	//그래서 보합색으로 처리
	//DrawNoFill(pDC, GetComplementaryColor(color), logfont, Startpt, Endpt, drawingRegion);
	//DrawNoFill(pDC,color, logfont, Startpt, Endpt, drawingRegion);

	CPen penArc(PS_SOLID, 1, color);
	CPen* pOldArcPen = pDC->SelectObject(&penArc);

   	//왼쪽 아래
	pDC->Arc(Startpt.x, Startpt.y, Endpt.x, Endpt.y,
			 (Endpt.x - Startpt.x) / 2, Endpt.x,
			 Endpt.x, (Endpt.y - Startpt.y) / 2);

	//왼쪽 위
	pDC->Arc(Startpt.x, Startpt.y, Endpt.x, Endpt.y,
			 Endpt.x, (Endpt.y - Startpt.y) / 2, 
			 (Endpt.x - Startpt.x) / 2, Endpt.x);
	
	pDC->SelectObject(pOldArcPen);

//////////////////
/*
	// 원래 Bitmap을 AlphaBlending한다.
	BLENDFUNCTION blendFunc;
	blendFunc.AlphaFormat = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = (BYTE)128;
	::AlphaBlend(
		pDC->m_hDC, Startpt.x, Startpt.y, Endpt.x - Startpt.x, Endpt.y - Startpt.y,
		dcCompatible.m_hDC, 0, 0, Endpt.x - Startpt.x, Endpt.y - Startpt.y,
		blendFunc);
*/
////////////
	CSymbolDraw::SelectClipRegion(pDC);

	dcCompatible.SelectObject(pOldPen);
	dcCompatible.SelectObject(pOldBrush);
	dcCompatible.SelectObject(pBitmapOld);
	bm.DeleteObject();
	dcCompatible.DeleteDC();

	CRect DrawingRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
	return DrawingRect;
}

CRect CFigure_Circle_Draw::DrawNoFill(CDC *pDC, const COLORREF& color, const int& nLineStyle,
					 const int& nLineWeight, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion)
{
	CPen pen(nLineStyle, nLineWeight, color);
	CPen* pOldPen = pDC->SelectObject(&pen);

    CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//왼쪽 아래
	pDC->Arc(Startpt.x, Startpt.y, Endpt.x, Endpt.y,
			 (Endpt.x - Startpt.x) / 2, Endpt.x,
			 Endpt.x, (Endpt.y - Startpt.y) / 2);

	//왼쪽 위
	pDC->Arc(Startpt.x, Startpt.y, Endpt.x, Endpt.y,
			 Endpt.x, (Endpt.y - Startpt.y) / 2, 
			 (Endpt.x - Startpt.x) / 2, Endpt.x);
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(pOldPen);

	return CRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
}

//도형 - 삼각형
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CFigure_Triangle_Draw::RealDrawSelectedMovingMark(CDC* pDC, 
													   const COLORREF& color,
													   const CPoint &oldMovepoint,
													   const CPoint& MovePt,
													   const CPoint& gap, 
													   const CSize& symbolsize, 
													   const CRect& drawingRegion,
													   const bool bDrawFromTop)
{
	 //point가 시작점.
	 //마우스를 중심으로 잡기 위해서 Lbutton한 시점과 저장되어 있던 시작점의 차이를 뺀다.
	 int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     /*if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - gap.x, oldMovepoint.y - gap.y), 
				 CPoint(oldMovepoint.x - gap.x + symbolsize.cx, oldMovepoint.y - gap.y + symbolsize.cy));

		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		DrawNoFillTriangle(pDC, CPoint(oldMovepoint.x - gap.x +1, oldMovepoint.y - gap.y+1),
						 CPoint(oldMovepoint.x - gap.x + symbolsize.cx, oldMovepoint.y - gap.y + symbolsize.cy),
						 bDrawFromTop);
		pDC->SelectObject(pOldPen);
	 }*/
	 //CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - gap.x, MovePt.y - gap.y), 
	 //		 CPoint(MovePt.x - gap.x + symbolsize.cx, MovePt.y - gap.y + symbolsize.cy));

	 CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	 CPen* pOldPen = pDC->SelectObject(&pen);
     DrawNoFillTriangle(pDC, CPoint(MovePt.x - gap.x+1, MovePt.y - gap.y+1),
	 				  CPoint(MovePt.x - gap.x + symbolsize.cx, MovePt.y - gap.y + symbolsize.cy),
					  bDrawFromTop);
	 pDC->SelectObject(pOldPen);
	 pDC->SetROP2( ropOld );
} 

void CFigure_Triangle_Draw::DrawbyMovedSize(CDC* pDC,
											const CPoint& start_pt,
											const CPoint& oldpoint,
											const CPoint &Endpoint,
											const CRect& drawingRegion,
											const bool bDrawFromTop)
{
	int ropOld = pDC->SetROP2(R2_NOTXORPEN);
	/*if(oldpoint.x != 0 && oldpoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, start_pt, oldpoint);
		CPen pen(PS_DOT, 1, RGB(0, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		DrawNoFillTriangle(pDC, start_pt, oldpoint, bDrawFromTop);
		pDC->SelectObject(pOldPen);
	}*/

	//CDrawLine::DrawRectangle(pDC, start_pt, Endpoint);
	CPen pen(PS_SOLID,1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	DrawNoFillTriangle(pDC, start_pt, Endpoint, bDrawFromTop);
	pDC->SelectObject(pOldPen);
    pDC->SetROP2( ropOld );
}

void CFigure_Triangle_Draw::DrawRectangle(CDC* pDC, const CPoint &startPt, const CPoint &endPt, const CRect& drawingRegion)
{
    CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	pDC->MoveTo(endPt.x, endPt.y);
	pDC->LineTo(startPt.x + (endPt.x - startPt.x) / 2, startPt.y);
	pDC->LineTo(startPt.x, endPt.y);
	pDC->LineTo(endPt.x, endPt.y);
	CSymbolDraw::SelectClipRegion(pDC);
}

CRect CFigure_Triangle_Draw::Draw(CDC *pDC, 
								  const COLORREF& color,
								  const int& nLineStyle,
								  const int& nLineWeight,
								  const LOGFONT& logfont,
								  const bool bFill, 
								  const CPoint &Startpt,
								  const CPoint &Endpt,
								  const CRect& drawingRegion,
								  const bool bDrawFromTop)
{
	//아래에서부터 그림을 그렸을 경우.
	if(bFill == true)
		return DrawFill(pDC, color, logfont, Startpt, Endpt, drawingRegion, bDrawFromTop);

	return DrawNoFill(pDC, color,nLineStyle,nLineWeight, Startpt, Endpt, drawingRegion, bDrawFromTop);
}

CRect CFigure_Triangle_Draw::DrawFill(CDC *pDC, const COLORREF& color,
									  const LOGFONT& logfont,
									  const CPoint &Startpt,
									  const CPoint &Endpt,
									  const CRect& drawingRegion,
									  const bool bFromTop)
{
	CDC dcCompatible;
    CBitmap bm;
	//비트맵, DC 생성.
    dcCompatible.CreateCompatibleDC(pDC);

	int nDifferX = Endpt.x - Startpt.x;
	int nDifferY = Endpt.y - Startpt.y;

    bm.CreateCompatibleBitmap(pDC, nDifferX, nDifferY);
    CBitmap *pBitmapOld = dcCompatible.SelectObject(&bm);

	//펜, 브러쉬생성.
	//CPen pen(PS_SOLID, 1, GetComplementaryColor(color));
	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = dcCompatible.GetCurrentPen();
	CBrush* pOldBrush = dcCompatible.SelectObject(&brush);

	//화면 메모리 Dc에 복사하기.
	dcCompatible.BitBlt(0, 0, nDifferX, nDifferY,
						pDC, Startpt.x, Startpt.y, SRCCOPY);

	DrawFillTriangle(&dcCompatible, CPoint(0, 0), CPoint(nDifferX, nDifferY), bFromTop);

    CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
    pDC->BitBlt(Startpt.x, Startpt.y, nDifferX, nDifferY, &dcCompatible, 0, 0, SRCAND);
	//nami 10.14
	//같은 color로 하면 테두리가 보이지 않는다.
	//그래서 보합색으로 처리
	pDC->SelectObject(&pen);
	DrawNoFillTriangle(pDC, Startpt, Endpt, bFromTop);

	CSymbolDraw::SelectClipRegion(pDC);

	dcCompatible.SelectObject(pOldPen);
	dcCompatible.SelectObject(pOldBrush);
	dcCompatible.SelectObject(pBitmapOld);
	bm.DeleteObject();
	dcCompatible.DeleteDC();

	return CRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
}

CRect CFigure_Triangle_Draw::DrawNoFill(CDC *pDC,
										const COLORREF& color,
										const int& nLineStyle,
										const int& nLineWeight,
										const CPoint &Startpt,
										const CPoint &Endpt,
										const CRect& drawingRegion,
									    const bool bFromTop)
{
	CPen pen(nLineStyle, nLineWeight, color);
	CPen* pOldPen = pDC->SelectObject(&pen);

    CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	DrawNoFillTriangle(pDC, Startpt, Endpt, bFromTop);
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(pOldPen);

	return CRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
}

void CFigure_Triangle_Draw::DrawFillTriangle(CDC* pDC,
											 const CPoint& startPt,
											 const CPoint& endPt,
											 const bool bFromTop)
{
	CPoint pts[3];
	if(bFromTop)
	{
		pts[0] = CPoint(startPt.x, endPt.y);
		pts[1] = CPoint((endPt.x - startPt.x) / 2, startPt.y);
		pts[2] = CPoint(endPt.x, endPt.y);
	}
	else 
	{
		pts[0] = CPoint((endPt.x - startPt.x) / 2, endPt.y);
		pts[1] = CPoint(startPt.x, startPt.y);
		pts[2] = CPoint(endPt.x, startPt.y);
	}
	pDC->Polygon(pts, 3);
}

void CFigure_Triangle_Draw::DrawNoFillTriangle(CDC* pDC,
											   const CPoint& startPt,
											   const CPoint& endPt,
											   const bool bFromTop)
{
	if(bFromTop) 
	{
		pDC->MoveTo(endPt.x, endPt.y);
		pDC->LineTo(startPt.x + (endPt.x - startPt.x) / 2, startPt.y);
		pDC->LineTo(startPt.x, endPt.y);
		pDC->LineTo(endPt.x, endPt.y);
	}
	else
	{
		pDC->MoveTo(startPt.x, startPt.y);
		pDC->LineTo(startPt.x + (endPt.x - startPt.x) / 2, endPt.y);
		pDC->LineTo(endPt.x, startPt.y);
		pDC->LineTo(startPt.x, startPt.y);
	}
}
//////////////////////////////////////////////////////////////////////
// //textDraw
//////////////////////////////////////////////////////////////////////
CTextDraw::CTextDraw():
	m_pTextTool(NULL)
{

}

CTextDraw::~CTextDraw()
{
	m_font.DeleteObject();
}

CSize CTextDraw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);
	
	CSize editsize = GetEditSize(pDC, logfont, startPt);
	CRect DrawingRect(startPt.x, startPt.y, editsize.cx, editsize.cy); 

	COLORREF oldTextColor = pDC->SetTextColor(color);

	CTextEdit* pEdit = new CTextEdit(color);
	pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_WANTRETURN | ES_MULTILINE | ES_LEFT,
		DrawingRect, m_pParent, 1); //

	pEdit->SetFont(&m_font, false);
	pEdit->SetTextToolObject(m_pTextTool);
	pEdit->SetDrawingRegion(m_drawingRegion);
	pEdit->SetFocus();

	pDC->SetTextColor(oldTextColor);

	return CSize(editsize.cx - startPt.x, editsize.cy - startPt.y);
}

void CTextDraw::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

CSize CTextDraw::DrawText(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &point, const CRect& drawingRegion, const CString& strText)
{
	if(pDC == NULL || strText.IsEmpty())
		return CSize(0,0);

	CFont font;
	font.CreateFontIndirect(&logfont);
	CFont* pOldFont = pDC->SelectObject(&font);
	int oldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(color);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CString total = strText;
	CString strEach = "";
	CSize textSize = pDC->GetTextExtent(" ");
	int nCount = CDataConversion::GetStringDataCount(strText, "\r\n");
	for(int i = 0 ; i< nCount  ; i++) {
		strEach = CDataConversion::GetStringData(total, "\r\n");
		pDC->TextOut(point.x, point.y + (textSize.cy * i), strEach);
	}
	if(!total.IsEmpty())
		pDC->TextOut(point.x, point.y + (textSize.cy * nCount), total);
	CSymbolDraw::SelectClipRegion(pDC);

	CSize size(pDC->GetTextExtent(GetExtremeLongWidthofText(strText)).cx, textSize.cy * (nCount + 1));
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
	pDC->SetBkMode(oldMode);
	pDC->SetTextColor(oldTextColor);
	return size;
}

void CTextDraw::SetTextToolObject(CTextTool *pTextTool)
{
	m_pTextTool = pTextTool;
}

CSize CTextDraw::GetEditSize(CDC *pDC, const LOGFONT& logfont, const CPoint &startPt)
{

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&logfont);
	CFont* pOldFont = pDC->SelectObject(&m_font);

	CSize size;
	CSize tempSize = pDC->GetTextExtent("ABCD");//"글자"
	size.cx = (tempSize.cx + startPt.x);
	size.cy = (tempSize.cy < 16 ? (20 + startPt.y) : (tempSize.cy + 4 + startPt.y));
//	size.cx = (pDC->GetTextExtent("      ").cx + startPt.x);
//	size.cy = (pDC->GetTextExtent("   ").cy < 16 ? (20 + startPt.y) : (pDC->GetTextExtent("   ").cy + 4 + startPt.y));

	pDC->SelectObject(pOldFont);
	return size;
}

void CTextDraw::SetDrawingRegion(const CRect& drawingRegion)
{
	m_drawingRegion = drawingRegion;
}

void CTextDraw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion,BOOL bTextDraw)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion,bTextDraw);
}

CString CTextDraw::GetExtremeLongWidthofText(const CString &text)
{
	CString total = text;
	CString strEach = "";
	CString strLongWidth = "";
	int nCount = CDataConversion::GetStringDataCount(total, "\r\n");
	for(int i = 0 ; i< nCount  ; i++) {
		strEach = CDataConversion::GetStringData(total, "\r\n");
		if(strEach.GetLength() > strLongWidth.GetLength())
			strLongWidth = strEach;
	}
		if(total.GetLength() > strLongWidth.GetLength())
			strLongWidth = total;

	return strLongWidth;	
}

//////////////////////////////////////////////////////////////////////
//LeftArrowDraw
//////////////////////////////////////////////////////////////////////
CSize CArrow_Left_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

CSize CArrow_Left_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 6, startPt.y - 6);
	pts[2] = CPoint(startPt.x + 6, startPt.y - 2);
	pts[3] = CPoint(startPt.x + 12, startPt.y - 2);
	pts[4] = CPoint(startPt.x + 12, startPt.y + 2);
	pts[5] = CPoint(startPt.x + 6, startPt.y + 2);
	pts[6] = CPoint(startPt.x + 6, startPt.y + 6);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(12, 12);
}

CSize CArrow_Left_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 9, startPt.y - 9);
	pts[2] = CPoint(startPt.x + 9, startPt.y - 3);
	pts[3] = CPoint(startPt.x + 18, startPt.y - 3);
	pts[4] = CPoint(startPt.x + 18, startPt.y + 3);
	pts[5] = CPoint(startPt.x + 9, startPt.y + 3);
	pts[6] = CPoint(startPt.x + 9, startPt.y + 9);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(18, 18);
}

CSize CArrow_Left_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 12, startPt.y - 12);
	pts[2] = CPoint(startPt.x + 12, startPt.y - 4);
	pts[3] = CPoint(startPt.x + 24, startPt.y - 4);
	pts[4] = CPoint(startPt.x + 24, startPt.y + 4);
	pts[5] = CPoint(startPt.x + 12, startPt.y + 4);
	pts[6] = CPoint(startPt.x + 12, startPt.y + 12);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(24, 24);
}

void CArrow_Left_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	//그래서 Top, Bottom은 6만큼 빼고, 더해야 한다. 

	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - CSymbolBaseData::interval, startPt.y - (symbolsize.cy/2) - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - CSymbolBaseData::interval, startPt.y + (symbolsize.cy/2) + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx + CSymbolBaseData::interval, startPt.y - (symbolsize.cy/2) - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx + CSymbolBaseData::interval, startPt.y + (symbolsize.cy/2) + CSymbolBaseData::interval)));

	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_Left_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x, oldMovepoint.y - (symbolsize.cy/2)), 
				 CPoint(oldMovepoint.x + symbolsize.cx + CSymbolBaseData::interval, oldMovepoint.y + (symbolsize.cy/2) + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x, MovePt.y - (symbolsize.cy/2)), 
			 CPoint(MovePt.x + symbolsize.cx + CSymbolBaseData::interval, MovePt.y + (symbolsize.cy/2) + CSymbolBaseData::interval));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//ArrowRightDraw
//////////////////////////////////////////////////////////////////////
CSize CArrow_Right_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}


CSize CArrow_Right_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 6, startPt.y + 6);
	pts[2] = CPoint(startPt.x - 6, startPt.y + 2);
	pts[3] = CPoint(startPt.x - 12, startPt.y + 2);
	pts[4] = CPoint(startPt.x - 12, startPt.y - 2);
	pts[5] = CPoint(startPt.x - 6, startPt.y - 2);
	pts[6] = CPoint(startPt.x - 6, startPt.y - 6);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(12, 12);
}

CSize CArrow_Right_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 9, startPt.y + 9);
	pts[2] = CPoint(startPt.x - 9, startPt.y + 3);
	pts[3] = CPoint(startPt.x - 18, startPt.y + 3);
	pts[4] = CPoint(startPt.x - 18, startPt.y - 3);
	pts[5] = CPoint(startPt.x - 9, startPt.y - 3);
	pts[6] = CPoint(startPt.x - 9, startPt.y - 9);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(18, 18);
}

CSize CArrow_Right_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 12, startPt.y + 12);
	pts[2] = CPoint(startPt.x - 12, startPt.y + 4);
	pts[3] = CPoint(startPt.x - 24, startPt.y + 4);
	pts[4] = CPoint(startPt.x - 24, startPt.y - 4);
	pts[5] = CPoint(startPt.x - 12, startPt.y - 4);
	pts[6] = CPoint(startPt.x - 12, startPt.y - 12);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(24, 24);
}

void CArrow_Right_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	//그래서 Top, Bottom은 6만큼 빼고, 더해야 한다.
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + CSymbolBaseData::interval, startPt.y - (symbolsize.cy/2) - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + CSymbolBaseData::interval, startPt.y + (symbolsize.cy/2) + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx - CSymbolBaseData::interval, startPt.y - (symbolsize.cy/2) - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx - CSymbolBaseData::interval, startPt.y + (symbolsize.cy/2) + CSymbolBaseData::interval)));

	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_Right_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x, oldMovepoint.y - (symbolsize.cy / 2)), 
				 CPoint(oldMovepoint.x - symbolsize.cx - CSymbolBaseData::interval, oldMovepoint.y + (symbolsize.cy / 2) + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x, MovePt.y - (symbolsize.cy / 2)), 
			 CPoint(MovePt.x - symbolsize.cx - CSymbolBaseData::interval, MovePt.y + (symbolsize.cy / 2) + CSymbolBaseData::interval));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//ArrowUpDraw
//////////////////////////////////////////////////////////////////////
CSize CArrow_Up_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

CSize CArrow_Up_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 6, startPt.y + 6);
	pts[2] = CPoint(startPt.x - 2, startPt.y + 6);
	pts[3] = CPoint(startPt.x - 2, startPt.y + 12);
	pts[4] = CPoint(startPt.x + 2, startPt.y + 12);
	pts[5] = CPoint(startPt.x + 2, startPt.y + 6);
	pts[6] = CPoint(startPt.x + 6, startPt.y + 6);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(12, 12);
}

CSize CArrow_Up_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 9, startPt.y + 9);
	pts[2] = CPoint(startPt.x - 3, startPt.y + 9);
	pts[3] = CPoint(startPt.x - 3, startPt.y + 18);
	pts[4] = CPoint(startPt.x + 3, startPt.y + 18);
	pts[5] = CPoint(startPt.x + 3, startPt.y + 9);
	pts[6] = CPoint(startPt.x + 9, startPt.y + 9);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(18, 18);
}

CSize CArrow_Up_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 12, startPt.y + 12);
	pts[2] = CPoint(startPt.x - 4, startPt.y + 12);
	pts[3] = CPoint(startPt.x - 4, startPt.y + 24);
	pts[4] = CPoint(startPt.x + 4, startPt.y + 24);
	pts[5] = CPoint(startPt.x + 4, startPt.y + 12);
	pts[6] = CPoint(startPt.x + 12, startPt.y + 12);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(24, 24);
}

void CArrow_Up_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	//그래서 left, right은 6만큼 빼고, 더해야 한다.
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - (symbolsize.cx/2) - CSymbolBaseData::interval, startPt.y - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - (symbolsize.cx/2) - CSymbolBaseData::interval, startPt.y + symbolsize.cy + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + (symbolsize.cx/2) + CSymbolBaseData::interval, startPt.y - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + (symbolsize.cx/2) + CSymbolBaseData::interval, startPt.y + symbolsize.cy + CSymbolBaseData::interval)));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_Up_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - (symbolsize.cx/2) - CSymbolBaseData::interval, oldMovepoint.y), 
				 CPoint(oldMovepoint.x + (symbolsize.cx/2), oldMovepoint.y + symbolsize.cy + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - (symbolsize.cx/2) - CSymbolBaseData::interval, MovePt.y), 
			 CPoint(MovePt.x + (symbolsize.cx/2), MovePt.y + symbolsize.cy + CSymbolBaseData::interval));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//ArrowDownDraw
//////////////////////////////////////////////////////////////////////
CSize CArrow_Down_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

void CArrow_Down_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	//그래서 Top, Bottom은 6만큼 빼고, 더해야 한다.
	//오른쪽 아래
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + (symbolsize.cx/2) + CSymbolBaseData::interval, startPt.y + CSymbolBaseData::interval)));
	//왼쪽 위
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - (symbolsize.cx/2) - CSymbolBaseData::interval, startPt.y - symbolsize.cy - CSymbolBaseData::interval)));
	//왼쪽 위
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + (symbolsize.cx/2) + CSymbolBaseData::interval, startPt.y - symbolsize.cy - CSymbolBaseData::interval)));
	//왼쪽 아래
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - (symbolsize.cx/2) - CSymbolBaseData::interval, startPt.y + CSymbolBaseData::interval)));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}


void CArrow_Down_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - (symbolsize.cx / 2) - CSymbolBaseData::interval, oldMovepoint.y - symbolsize.cy - CSymbolBaseData::interval), 
				 CPoint(oldMovepoint.x + (symbolsize.cx/2), oldMovepoint.y));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - (symbolsize.cx / 2) - CSymbolBaseData::interval, MovePt.y - symbolsize.cy - CSymbolBaseData::interval), 
			 CPoint(MovePt.x + (symbolsize.cx/2), MovePt.y));
	 pDC->SetROP2( ropOld );

//point가 중심.
/*
	int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - (symbolsize.cx / 2) - CSymbolBaseData::interval, oldMovepoint.y - (symbolsize.cy / 2) - CSymbolBaseData::interval), 
			 CPoint(oldMovepoint.x + (symbolsize.cx / 2) + CSymbolBaseData::interval, oldMovepoint.y + (symbolsize.cy / 2) + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - (symbolsize.cx / 2) - CSymbolBaseData::interval, MovePt.y - (symbolsize.cy / 2) - CSymbolBaseData::interval), 
			 CPoint(MovePt.x + (symbolsize.cx / 2) + CSymbolBaseData::interval, MovePt.y + (symbolsize.cy / 2) + CSymbolBaseData::interval));
     pDC->SetROP2( ropOld );
*/
}

//////////////////////////////////////////////////////////////////////
//Arrow_LeftTop_Draw
//////////////////////////////////////////////////////////////////////
CSize CArrow_LeftTop_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

CSize CArrow_LeftTop_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 9, startPt.y);
	pts[2] = CPoint(startPt.x + 7, startPt.y + 4);
	pts[3] = CPoint(startPt.x + 11, startPt.y + 8);
	pts[4] = CPoint(startPt.x + 8, startPt.y + 11);
	pts[5] = CPoint(startPt.x + 4, startPt.y + 7);
	pts[6] = CPoint(startPt.x, startPt.y + 9);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(11, 11);
}

CSize CArrow_LeftTop_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 15, startPt.y);
	pts[2] = CPoint(startPt.x + 10, startPt.y + 5);
	pts[3] = CPoint(startPt.x + 16, startPt.y + 11);
	pts[4] = CPoint(startPt.x + 11, startPt.y + 16);
	pts[5] = CPoint(startPt.x + 5, startPt.y + 10);
	pts[6] = CPoint(startPt.x, startPt.y + 15);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(16, 16);
}

CSize CArrow_LeftTop_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 20, startPt.y);
	pts[2] = CPoint(startPt.x + 13, startPt.y + 7);
	pts[3] = CPoint(startPt.x + 21, startPt.y + 15);
	pts[4] = CPoint(startPt.x + 15, startPt.y + 21);
	pts[5] = CPoint(startPt.x + 7, startPt.y + 13);
	pts[6] = CPoint(startPt.x, startPt.y + 20);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(21, 21);
}

void CArrow_LeftTop_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - CSymbolBaseData::interval, startPt.y - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - CSymbolBaseData::interval, startPt.y + symbolsize.cy + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx + CSymbolBaseData::interval, startPt.y - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx + CSymbolBaseData::interval, startPt.y + symbolsize.cy + CSymbolBaseData::interval)));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_LeftTop_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x, oldMovepoint.y), 
				 CPoint(oldMovepoint.x + symbolsize.cx + CSymbolBaseData::interval, oldMovepoint.y + symbolsize.cy + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x, MovePt.y), 
			 CPoint(MovePt.x + symbolsize.cx + CSymbolBaseData::interval, MovePt.y + symbolsize.cy + CSymbolBaseData::interval));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//Arrow_LeftBottom_Draw
//////////////////////////////////////////////////////////////////////
CSize CArrow_LeftBottom_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

CSize CArrow_LeftBottom_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 9, startPt.y);
	pts[2] = CPoint(startPt.x + 7, startPt.y - 4);
	pts[3] = CPoint(startPt.x + 11, startPt.y - 8);
	pts[4] = CPoint(startPt.x + 8, startPt.y - 11);
	pts[5] = CPoint(startPt.x + 4, startPt.y - 7);
	pts[6] = CPoint(startPt.x, startPt.y - 9);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(11, 11);
}

CSize CArrow_LeftBottom_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 15, startPt.y);
	pts[2] = CPoint(startPt.x + 10, startPt.y - 5);
	pts[3] = CPoint(startPt.x + 16, startPt.y - 11);
	pts[4] = CPoint(startPt.x + 11, startPt.y - 16);
	pts[5] = CPoint(startPt.x + 5, startPt.y - 10);
	pts[6] = CPoint(startPt.x, startPt.y - 15);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(16, 16);
}

CSize CArrow_LeftBottom_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 20, startPt.y);
	pts[2] = CPoint(startPt.x + 13, startPt.y - 7);
	pts[3] = CPoint(startPt.x + 21, startPt.y - 15);
	pts[4] = CPoint(startPt.x + 15, startPt.y - 21);
	pts[5] = CPoint(startPt.x + 7, startPt.y - 13);
	pts[6] = CPoint(startPt.x, startPt.y - 20);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(21, 21);
}

void CArrow_LeftBottom_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - CSymbolBaseData::interval, startPt.y + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - CSymbolBaseData::interval, startPt.y - symbolsize.cy - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx + CSymbolBaseData::interval, startPt.y + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + symbolsize.cx + CSymbolBaseData::interval, startPt.y - symbolsize.cy - CSymbolBaseData::interval)));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_LeftBottom_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x, oldMovepoint.y - symbolsize.cy - CSymbolBaseData::interval), 
				 CPoint(oldMovepoint.x + symbolsize.cx + CSymbolBaseData::interval, oldMovepoint.y));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x, MovePt.y - symbolsize.cy - CSymbolBaseData::interval), 
			 CPoint(MovePt.x + symbolsize.cx + CSymbolBaseData::interval, MovePt.y));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//Arrow_RightTop_Draw
//////////////////////////////////////////////////////////////////////
CSize CArrow_RightTop_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

CSize CArrow_RightTop_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 9, startPt.y);
	pts[2] = CPoint(startPt.x - 7, startPt.y + 4);
	pts[3] = CPoint(startPt.x - 11, startPt.y + 8);
	pts[4] = CPoint(startPt.x - 8, startPt.y + 11);
	pts[5] = CPoint(startPt.x - 4, startPt.y + 7);
	pts[6] = CPoint(startPt.x, startPt.y + 9);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(11, 11);
}

CSize CArrow_RightTop_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 15, startPt.y);
	pts[2] = CPoint(startPt.x - 10, startPt.y + 5);
	pts[3] = CPoint(startPt.x - 16, startPt.y + 11);
	pts[4] = CPoint(startPt.x - 11, startPt.y + 16);
	pts[5] = CPoint(startPt.x - 5, startPt.y + 10);
	pts[6] = CPoint(startPt.x, startPt.y + 15);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(16, 16);
}

CSize CArrow_RightTop_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 20, startPt.y);
	pts[2] = CPoint(startPt.x - 13, startPt.y + 7);
	pts[3] = CPoint(startPt.x - 21, startPt.y + 15);
	pts[4] = CPoint(startPt.x - 15, startPt.y + 21);
	pts[5] = CPoint(startPt.x - 7, startPt.y + 13);
	pts[6] = CPoint(startPt.x, startPt.y + 20);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(21, 21);
}

void CArrow_RightTop_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID, 1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + CSymbolBaseData::interval, startPt.y - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + CSymbolBaseData::interval, startPt.y + symbolsize.cy + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx - CSymbolBaseData::interval, startPt.y - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx - CSymbolBaseData::interval, startPt.y + symbolsize.cy + CSymbolBaseData::interval)));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_RightTop_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - symbolsize.cx - CSymbolBaseData::interval, oldMovepoint.y), 
				 CPoint(oldMovepoint.x, oldMovepoint.y + symbolsize.cy + CSymbolBaseData::interval));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - symbolsize.cx - CSymbolBaseData::interval, MovePt.y), 
			 CPoint(MovePt.x, MovePt.y + symbolsize.cy + CSymbolBaseData::interval));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//Arrow_RightBottom_Draw
//////////////////////////////////////////////////////////////////////
CSize CArrow_RightBottom_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& /*textColor*/, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	if(pDC == NULL) return CSize(0,0);

	CPen pen(PS_SOLID, 1, color);
	CBrush brush(color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	CSize size;
	if(sizetype == CSymbolBaseData::Small) 
		size = Draw_Small(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Medium) 
		size = Draw_Medium(pDC, startPt, drawingRegion);
	else if(sizetype == CSymbolBaseData::Large) 
		size = Draw_Large(pDC, startPt, drawingRegion);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	return size;
}

CSize CArrow_RightBottom_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 9, startPt.y);
	pts[2] = CPoint(startPt.x - 7, startPt.y - 4);
	pts[3] = CPoint(startPt.x - 11, startPt.y - 8);
	pts[4] = CPoint(startPt.x - 8, startPt.y - 11);
	pts[5] = CPoint(startPt.x - 4, startPt.y - 7);
	pts[6] = CPoint(startPt.x, startPt.y - 9);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(11, 11);
}

CSize CArrow_RightBottom_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 15, startPt.y);
	pts[2] = CPoint(startPt.x - 10, startPt.y - 5);
	pts[3] = CPoint(startPt.x - 16, startPt.y - 11);
	pts[4] = CPoint(startPt.x - 11, startPt.y - 16);
	pts[5] = CPoint(startPt.x - 5, startPt.y - 10);
	pts[6] = CPoint(startPt.x, startPt.y - 15);
    pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(16, 16);
}

CSize CArrow_RightBottom_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x - 20, startPt.y);
	pts[2] = CPoint(startPt.x - 13, startPt.y - 7);
	pts[3] = CPoint(startPt.x - 21, startPt.y - 15);
	pts[4] = CPoint(startPt.x - 15, startPt.y - 21);
	pts[5] = CPoint(startPt.x - 7, startPt.y - 13);
	pts[6] = CPoint(startPt.x, startPt.y - 20);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(21, 21);
}

void CArrow_RightBottom_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CBrush brush(RGB(255, 255, 255));
	CPen pen(PS_SOLID,1, color);
	CBrush *oldbrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	//화살표의 촉이 StartPoint이다.
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + CSymbolBaseData::interval, startPt.y + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x + CSymbolBaseData::interval, startPt.y - symbolsize.cy - CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx - CSymbolBaseData::interval, startPt.y + CSymbolBaseData::interval)));
	pDC->Rectangle(CSymbolDraw::GetGripRegion(CPoint(startPt.x - symbolsize.cx - CSymbolBaseData::interval, startPt.y - symbolsize.cy - CSymbolBaseData::interval)));
	CSymbolDraw::SelectClipRegion(pDC);

	pDC->SelectObject(oldbrush);
	pDC->SelectObject(pOldPen);
}

void CArrow_RightBottom_Draw::DrawSelectedMovingMark(CDC* pDC, const COLORREF& color, const CPoint &oldMovepoint, const CPoint& MovePt, const CSize& symbolsize, const CRect& drawingRegion)
{
	//point가 시작점.
	//마우스를 오른쪽 중심으로 잡는다.
     int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - symbolsize.cx - CSymbolBaseData::interval, oldMovepoint.y - symbolsize.cy - CSymbolBaseData::interval), 
				 CPoint(oldMovepoint.x, oldMovepoint.y));
	 }
	 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - symbolsize.cx - CSymbolBaseData::interval, MovePt.y - symbolsize.cy - CSymbolBaseData::interval), 
			 CPoint(MovePt.x, MovePt.y));
	 pDC->SetROP2( ropOld );
}

//////////////////////////////////////////////////////////////////////
//NUM_ONE_Draw
//////////////////////////////////////////////////////////////////////

CSize CNumber_One_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "1");
}

void CNumber_One_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Two_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Two_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "2");
}

void CNumber_Two_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Three_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Three_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "3");
}

void CNumber_Three_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Four_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Four_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "4");
}

void CNumber_Four_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Five_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Five_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "5");
}

void CNumber_Five_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Six_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Six_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "6");
}

void CNumber_Six_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Seven_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Seven_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "7");
}

void CNumber_Seven_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Eight_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Eight_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "8");
}

void CNumber_Eight_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Nine_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Nine_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "9");
}

void CNumber_Nine_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//NUM_Ten_Draw
//////////////////////////////////////////////////////////////////////
CSize CNumber_Ten_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "10");
}

void CNumber_Ten_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_A_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_A_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "a");
}

void CText_A_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_B_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_B_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "b");
}

void CText_B_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_C_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_C_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "c");
}

void CText_C_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_D_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_D_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "d");
}

void CText_D_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_E_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_E_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "e");
}

void CText_E_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_F_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_F_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "f");
}

void CText_F_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_G_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_G_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "g");
}

void CText_G_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Text_H_Draw
//////////////////////////////////////////////////////////////////////
CSize CText_H_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "h");
}

void CText_H_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Char_1_Draw
//////////////////////////////////////////////////////////////////////
CSize CChar_1_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "☞");
}

void CChar_1_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Char_2_Draw
//////////////////////////////////////////////////////////////////////
CSize CChar_2_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "☜");
}

void CChar_2_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Char_3_Draw
//////////////////////////////////////////////////////////////////////
CSize CChar_3_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "◆");
}

void CChar_3_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Char_4_Draw
//////////////////////////////////////////////////////////////////////
CSize CChar_4_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "★");
}

void CChar_4_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Char_5_Draw
//////////////////////////////////////////////////////////////////////
CSize CChar_5_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "●");
}

void CChar_5_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Char_6_Draw
//////////////////////////////////////////////////////////////////////
CSize CChar_6_Draw::Draw(CDC *pDC, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor, const CSymbolBaseData::SIZETYPE& sizetype, const CPoint &startPt, const CRect& drawingRegion)
{
	return CSymbolDraw_OnePoint_Size::DrawSpecialText(pDC, color, logfont, textColor, sizetype, startPt, drawingRegion, "■");
}

void CChar_6_Draw::DrawSelectedMark(CDC* pDC, const COLORREF& color, const CPoint& startPt, const CSize& symbolsize, const CRect& drawingRegion)
{
	CSymbolDraw_OnePoint_Size::DrawSelectedMark(pDC, color, startPt, symbolsize, drawingRegion);
}

//////////////////////////////////////////////////////////////////////
//Arrow_Draw
//////////////////////////////////////////////////////////////////////
CSize CArrow_Down_Draw::Draw_Small(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 6, startPt.y - 6);
	pts[2] = CPoint(startPt.x + 2, startPt.y - 6);
	pts[3] = CPoint(startPt.x + 2, startPt.y - 12);
	pts[4] = CPoint(startPt.x - 2, startPt.y - 12);
	pts[5] = CPoint(startPt.x - 2, startPt.y - 6);
	pts[6] = CPoint(startPt.x - 6, startPt.y - 6);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(12, 12);
}

CSize CArrow_Down_Draw::Draw_Medium(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 9, startPt.y - 9);
	pts[2] = CPoint(startPt.x + 3, startPt.y - 9);
	pts[3] = CPoint(startPt.x + 3, startPt.y - 18);
	pts[4] = CPoint(startPt.x - 3, startPt.y - 18);
	pts[5] = CPoint(startPt.x - 3, startPt.y - 9);
	pts[6] = CPoint(startPt.x - 9, startPt.y - 9);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(18, 18);
}

CSize CArrow_Down_Draw::Draw_Large(CDC *pDC, const CPoint &startPt, const CRect& drawingRegion)
{
	CSymbolDraw::SelectClipRegion(pDC, drawingRegion);
	CPoint pts[7];
	pts[0] = CPoint(startPt.x, startPt.y);
	pts[1] = CPoint(startPt.x + 12, startPt.y - 12);
	pts[2] = CPoint(startPt.x + 4, startPt.y - 12);
	pts[3] = CPoint(startPt.x + 4, startPt.y - 24);
	pts[4] = CPoint(startPt.x - 4, startPt.y - 24);
	pts[5] = CPoint(startPt.x - 4, startPt.y - 12);
	pts[6] = CPoint(startPt.x - 12, startPt.y - 12);
	pDC->Polygon(pts, 7);
	CSymbolDraw::SelectClipRegion(pDC);
	return CSize(24, 24);
}

void CDrawLine::DrawDashRectangle(CDC *pDC, const CPoint &startPt, const CPoint &endPt)
{
	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	DrawLine(pDC, CPoint(endPt.x, endPt.y), CPoint(startPt.x, endPt.y),
			 CPoint(startPt.x, startPt.y), CPoint(endPt.x, startPt.y), CPoint(endPt.x, endPt.y));
	pDC->SelectObject(pOldPen);
}
