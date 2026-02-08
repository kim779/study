#include "stdafx.h"
#include "ExGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CExGrid::CExGrid()
	:CfxGrid()
{
}

CExGrid::~CExGrid()
{
}

void CExGrid::OnDraw(CDC* pDC)
{
	CRect rect;
	int row, col;

	CRect clipRect;
	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;

	EraseBkgnd(pDC);

	int nFixedRowHeight = GetFixedRowHeight();
	int nFixedColWidth  = GetFixedColumnWidth();

	CIdCell tlCell = GetTopleftNonFixedCell();
	int minVisibleRow = tlCell.row;
	int minVisibleCol = tlCell.col;

	CRect visRect;
	CRangeCell visCellRange = GetVisibleNonFixedCellRange(visRect);
	int maxVisibleRow = visCellRange.GetMaxRow();
	int maxVisibleCol = visCellRange.GetMaxCol();

	// draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;
		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right +1;
			rect.right = rect.left + GetColumnWidth(col) -1;  

			DrawFixedCell(pDC, row, col, rect);
		}
	}
     
	// draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
	rect.bottom = nFixedRowHeight -1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;

		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top)	continue;

		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right) break;
			if (rect.right < clipRect.left) continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
    
	// draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;

		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top) continue;

		rect.right = nFixedColWidth -1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right + 1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right)	break;
			if (rect.right < clipRect.left)	continue;

			DrawFixedCell(pDC, row, col, rect);
		}
	}
	
	// draw rest of non-fixed cells
	rect.bottom = nFixedRowHeight-1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		rect.top = rect.bottom +1;
		rect.bottom = rect.top + GetRowHeight(row) -1;

		// rect.bottom = bottom pixel of previous row
		if (rect.top > clipRect.bottom) break;
		if (rect.bottom < clipRect.top) continue;

		rect.right = nFixedColWidth -1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (IsCellAttribute(CIdCell(row, col), GVAT_HIDDEN))
				continue;

			rect.left = rect.right +1;
			rect.right = rect.left + GetColumnWidth(col) -1;

			if (rect.left > clipRect.right)	break;
			if (rect.right < clipRect.left)	continue;

			DrawCell(pDC, row, col, rect);
		}
	}

	CPen pen; pen.CreatePen(m_gridLineType, 0, m_gridCOL);
	pDC->SelectObject(&pen); pDC->SetBkMode(TRANSPARENT);

	bool bFixed = (m_nFixedRows - 1 >= 0 ?(IsCellAttribute(CIdCell(m_nFixedRows - 1, minVisibleCol), GVAT_3DHEAD)?true:false): false);
	// draw vertical lines (drawn at ends of cells)
	if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
	{
		int xx = 0;
		for (col = 0; col < m_nFixedCols-1; col++)
		{
			xx += GetColumnWidth(col);
			pDC->MoveTo(xx-1, bFixed?nFixedRowHeight:0);
			pDC->LineTo(xx-1, visRect.bottom);   
		}

		pDC->MoveTo(0, bFixed?nFixedRowHeight:0); pDC->LineTo(0, visRect.bottom);
		pDC->MoveTo(nFixedColWidth-1, bFixed?nFixedRowHeight:0); pDC->LineTo(nFixedColWidth-1, visRect.bottom);

		int x = nFixedColWidth;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			x += GetColumnWidth(col);
			pDC->MoveTo(x-1, bFixed?nFixedRowHeight:0);
			pDC->LineTo(x-1, visRect.bottom);   
		}
	}
    
	// draw horizontal lines (drawn at bottom of each cell)
	if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
	{
		pDC->MoveTo(0, 0); pDC->LineTo(visRect.right,  0);
		pDC->MoveTo(0, nFixedRowHeight-1); pDC->LineTo(visRect.right,  nFixedRowHeight-1);

		int y = nFixedRowHeight;
		for (row = minVisibleRow; row <= maxVisibleRow; row++)
		{
			y += GetRowHeight(row);
			pDC->MoveTo(0, y-1);
			pDC->LineTo(visRect.right, y-1);
		}
	}
	else
	{
		int y = nFixedRowHeight;
		for (row = minVisibleRow; row <= maxVisibleRow; row++)
			y += GetRowHeight(row);
		if (m_nRows - m_nFixedRows == row - 1)
		{
			pDC->MoveTo(0, y-1);
			pDC->LineTo(visRect.right, y-1);
		}
	}

	CFont font, *oldfont; int key; _fixMerge fixMerge;
	for (POSITION pos = m_mapMerge.GetStartPosition(); pos != NULL; )
	{
		m_mapMerge.GetNextAssoc(pos, key, (_fixMerge &)fixMerge);

		pDC->FillSolidRect(fixMerge.rect, fixMerge.bkcol == CLR_DEFAULT ? m_fixbkCOL : fixMerge.bkcol);

		pDC->MoveTo(fixMerge.rect.right,fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.left, fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.left, fixMerge.rect.bottom);

		pDC->MoveTo(fixMerge.rect.right, fixMerge.rect.top);
		pDC->LineTo(fixMerge.rect.right, fixMerge.rect.bottom);
		pDC->LineTo(fixMerge.rect.left,  fixMerge.rect.bottom);

		fixMerge.rect.DeflateRect(1+m_margin, 1);

		font.CreatePointFontIndirect(&fixMerge.font);
		oldfont = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fixMerge.fgcol);
		fixMerge.rect.top += 2;

		DrawText(pDC->m_hDC, fixMerge.text, -1, fixMerge.rect, fixMerge.format);

		pDC->SelectObject(oldfont); font.DeleteObject();
	}

	pDC->SelectStockObject(NULL_PEN);
}

BOOL CExGrid::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	GVITEM gvitem;

	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_IMAGE|GVMK_BKCOLOR|GVMK_FGCOLOR|GVMK_STATE|GVMK_ATTR|GVMK_PARAM|GVMK_MISC;
	gvitem.row  = nRow;
	gvitem.col  = nCol;

	if (!GetItem(&gvitem)) return FALSE;

	int savedDC = pDC->SaveDC();

	// HDRMERGE column rectangle calculate
	if ((gvitem.attr & GVAT_HDRMERGE) == GVAT_HDRMERGE)
	{
		rect.top = rect.top + (rect.Height() / 2);

		CRect Rect; _fixMerge fixMerge;
		if (m_mapMerge.Lookup(nCol, fixMerge))
		{
			for (int ii = 0; ii < 5; ii++)
			{
				if (fixMerge.cols[ii] == -1)
					break;
				if (!GetCellRect(nRow, fixMerge.cols[ii], Rect))
					continue;
				
				if (!ii)
				{
					Rect.bottom = Rect.bottom - (Rect.Height() / 2);
					if (nCol > m_nFixedCols) Rect.left  -= 1;
					fixMerge.rect = Rect;
				}
				else
				{
					fixMerge.rect.right = Rect.right;
				}
			}

			fixMerge.rect.bottom -= 2;
			m_mapMerge.SetAt(nCol, fixMerge);
		}
	}

	bEraseBk = TRUE;
	if (gvitem.state & GVST_DROPHILITED && !pDC->IsPrinting())
	{
		rect.right++; rect.bottom++;
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(LTGRAY_BRUSH); //NULL_BRUSH);
		pDC->Rectangle(rect);
		rect.right--; rect.bottom--;
	}
	else if (gvitem.state & GVST_FOCUSED && !pDC->IsPrinting()) 
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		if (bEraseBk) 
		{
			COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
			CBrush brush(color); pDC->FillRect(rect, &brush);
		}
		rect.right--; rect.bottom--;

		CRect	chkRC = rect;
		if (chkRC.left <= 0) chkRC.left = 1;
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(chkRC);
	}
	else if (m_selectLine && gvitem.state & GVST_SELECTED && !pDC->IsPrinting() && nRow >= m_nFixedRows)
	{
		rect.right++; rect.bottom++;	// FillRect doesn't draw RHS or bottom
		pDC->FillSolidRect(rect, m_bkSELCOL);
		rect.right--; rect.bottom--;
		pDC->SetTextColor(PALETTERGB(255, 255, 255));
	}
	else if (bEraseBk && !pDC->IsPrinting())
	{
		rect.right++; rect.bottom++;
		COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
		pDC->FillSolidRect(rect, color);
		rect.right--; rect.bottom--;
	}

	pDC->SetTextColor((gvitem.fgcol == CLR_DEFAULT) ? m_fixfgCOL : gvitem.fgcol);
    
	LOGFONT *logfont  = GetItemFont(nRow, nCol);
	CIdCell focusCell = GetFocusCell();
   
	CFont font; font.CreatePointFontIndirect(logfont);
	pDC->SelectObject(&font); pDC->SetBkMode(TRANSPARENT);
	rect.DeflateRect(m_margin, 1);

	if (m_imageList && gvitem.image >= 0)
	{
		IMAGEINFO Info;
		if (m_imageList->GetImageInfo(gvitem.image, &Info))
		{
			rect.InflateRect(m_margin, 0);
			int nImageWidth  = Info.rcImage.right - Info.rcImage.left + 1;
			int nImageHeight = Info.rcImage.bottom - Info.rcImage.top;

			CPoint	drawPT(rect.left, rect.top + ((rect.Height() - nImageHeight) / 2));

			m_imageList->Draw(pDC, gvitem.image, drawPT, ILD_NORMAL);
			rect.left += nImageWidth /*+ m_margin*/;
		}
	}

	if ((gvitem.attr & GVAT_CHECK) == GVAT_CHECK)
	{
		int nWidth = 15; int nHeight = 15;
		int topx = (rect.Height() - nHeight) / 2;
		CRect	checkRC(rect.left, rect.top+topx, rect.left+nWidth, rect.top+topx+nHeight);
		if (gvitem.text.GetLength() <= 0)
		{
			int offset = (rect.Width() - nWidth) / 2;
			checkRC.SetRect(rect.left+offset, rect.top+topx, rect.left+offset+nWidth, rect.top+topx+nHeight);
		}

		UINT state = (gvitem.param) ? DFCS_BUTTONCHECK|DFCS_CHECKED : DFCS_BUTTONCHECK;
		pDC->DrawFrameControl(checkRC, DFC_BUTTON, state);
		rect.left += nWidth+m_margin;
	}

	if ((gvitem.attr & GVAT_3DHEAD) == GVAT_3DHEAD)
	{
		COLORREF color = (gvitem.bkcol == CLR_DEFAULT) ? m_fixbkCOL : gvitem.bkcol;
		rect.InflateRect(2, 0, 3, 1);
		pDC->Draw3dRect(rect, 
			RGB(min(255, GetRValue(color) * 1.3), min(255, GetGValue(color) * 1.3), min(255, GetBValue(color) * 1.3)), 
			RGB(max(0, GetRValue(color) * 0.7), max(0, GetGValue(color) * 0.7), max(0, GetBValue(color) * 0.7)));
		rect.DeflateRect(2, 0, 3, 1);
	}

	if (++rect.top < rect.bottom) DrawText(pDC->m_hDC, gvitem.text, -1, rect, gvitem.format);

	pDC->RestoreDC(savedDC);
	font.DeleteObject();

	return TRUE;
}

BOOL CExGrid::RedrawCell(int nRow, int nCol, CDC* pDC)
{
	BOOL bResult = TRUE;
	if (!IsCellVisible(nRow, nCol))
		return FALSE;

	CRect rect;
	if (!GetCellRect(nRow, nCol, rect))
		return FALSE;

	if (!pDC) pDC = GetDC();

	if (pDC)
	{
		CRect rectOffset = rect;
		rectOffset.OffsetRect(-(rect.left - 1), -(rect.top - 1));
		
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, rect.Width() + 1, rect.Height() + 1);

		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap* pbitmapOld = dcMem.SelectObject(&bitmap);

		CPen pen; pen.CreatePen(m_gridLineType, 0, m_gridCOL);
		CPen* pOldPen = (CPen*) dcMem.SelectObject(&pen);
		dcMem.SetBkMode(TRANSPARENT);

		if (nRow < m_nFixedRows || nCol < m_nFixedCols)
		{
			bResult = DrawFixedCell(&dcMem, nRow, nCol, rectOffset, TRUE);
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				if (rect.top > 0)
				{
					rectOffset.top -= 1;
				}
				dcMem.MoveTo(rectOffset.left, rectOffset.top);
				dcMem.LineTo(rectOffset.right+1, rectOffset.top);
				dcMem.MoveTo(rectOffset.left, rectOffset.bottom);
				dcMem.LineTo(rectOffset.right+1, rectOffset.bottom);
			}
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				if (!IsCellAttribute(CIdCell(nRow, nCol), GVAT_3DHEAD))
				{
					if ((m_nFixedCols != 0 && nCol == 0) || (m_nFixedCols == 0 && nRow == 0 && nCol == 0))
					{
						dcMem.MoveTo(rectOffset.left, rectOffset.top);
						dcMem.LineTo(rectOffset.left, rectOffset.bottom + 1);
					}
					dcMem.MoveTo(rectOffset.right, rectOffset.top);
					dcMem.LineTo(rectOffset.right, rectOffset.bottom + 1);
				}
			}
		}
		else
		{
			bResult = DrawCell(&dcMem, nRow, nCol, rectOffset, TRUE);
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_HORZ) 
			{
				if (rect.top > 0)
				{
					rectOffset.top -= 1;
				}
				dcMem.MoveTo(rectOffset.left, rectOffset.top);
				dcMem.LineTo(rectOffset.right + 1, rectOffset.top);
				dcMem.MoveTo(rectOffset.left, rectOffset.bottom);
				dcMem.LineTo(rectOffset.right + 1, rectOffset.bottom);
			} 
			if (m_gridLine == GVLN_BOTH || m_gridLine == GVLN_VERT) 
			{
				if (rect.left == 0)
				{
					dcMem.MoveTo(rectOffset.left, rectOffset.top);
					dcMem.LineTo(rectOffset.left, rectOffset.bottom + 1);
				}
				dcMem.MoveTo(rectOffset.right, rectOffset.top);
				dcMem.LineTo(rectOffset.right, rectOffset.bottom + 1);
			}
		}
		dcMem.SelectObject(pOldPen);

		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height() + 1, &dcMem, 1, 1, SRCCOPY);

		dcMem.SelectObject(pbitmapOld);
		bitmap.DeleteObject();
		dcMem.DeleteDC();
	}
	else
	{
		InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
						// and hope that OnPaint manages to get one
	}

	if (pDC != NULL)
		ReleaseDC(pDC);

	return bResult;
}

