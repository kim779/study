// fxIGrid.cpp : implementation file
//

#include "stdafx.h"
#include "fxIGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ARR_UP		"↑"
#define	ARR_DN		"↓"
#define	TRI_UP		"▲"
#define	TRI_DN		"▼"

/////////////////////////////////////////////////////////////////////////////
// CfxIGrid

CfxIGrid::CfxIGrid()
{
	wLen = 0;
	RegisterWndClass();
	m_StartPnt        = CPoint(0, 0);
	m_lHeadHeight     = 18;
	m_lDataHeight     = 18;
	m_lSumColumnWidth = 0;
	m_lSumVisColWidth = 0;
	m_pTipCtrl        = NULL;
	m_bInCellMargin   = FALSE;

	m_bShowEmptyText   = FALSE;

	m_lMoveWPos       = 0;
	m_lMoveWColumn    = -1;

	m_nSelectedRow    = -1;

	m_nGubnLineIdx    = 4;

	m_headBgColor  = PALETTERGB(128, 128, 255);
	m_headFgColor  = PALETTERGB(255, 255, 255);

	m_dataBgColor  = PALETTERGB(255, 255, 255);
	m_dataBgColor1 = PALETTERGB(255, 255, 255);
	m_dataBgColor2 = PALETTERGB(240, 240, 240);
	m_dataFgColor  = PALETTERGB(0,   0,   0);

	m_upColor      = PALETTERGB(255, 0,   0);
	m_dnColor      = PALETTERGB(0,   0,   255);
	m_eqColor      = PALETTERGB(0,   0,   0);

	m_upRvColor    = PALETTERGB(255, 255, 255);
	m_dnRvColor    = PALETTERGB(255, 255, 255);

	m_firstRowColor = PALETTERGB(255, 255, 255);

	m_selFgColor   = GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_selBgColor   = GetSysColor(COLOR_HIGHLIGHT);

	m_b3DHead     = TRUE;
	m_bMemDC      = TRUE;
	m_bFixScroll  = TRUE;
	m_b2RowColor  = TRUE;
	m_bFillEmpty  = TRUE;
	m_bFixFirstRow = FALSE;

	m_bColumnResize  = TRUE;
	m_bFirstRowColor = FALSE;
	m_bSelection     = FALSE;
	m_wDisableBar    = NULL;
	m_bDataLine	 = TRUE;
	m_bRowGubnLine   = FALSE;
	m_bColGubnLine   = FALSE;

	m_bEnableTip     = TRUE;
	m_bTracking      = FALSE;
	m_bHover         = FALSE;

	m_sEmptyText.Empty();

	m_hCursor     = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));
}

CfxIGrid::~CfxIGrid()
{
	if (m_hCursorHand)
		DestroyCursor(m_hCursorHand);
	m_arColumn.RemoveAll();

#ifdef DF_USESTL_FXGRID 
	m_arData.RemoveAll();
#else
	while (m_arData.GetSize())
	{
		m_arData.GetAt(0)->RemoveAll();

		delete m_arData.GetAt(0);
		m_arData.RemoveAt(0);
	}
#endif


}


BEGIN_MESSAGE_MAP(CfxIGrid, CWnd)
	//{{AFX_MSG_MAP(CfxIGrid)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxIGrid message handlers

void CfxIGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect clipRc, bkRect;
#ifdef DF_USESTL_FXGRID
	std::unique_ptr<CMemDC> pMemDC;
#else
	CDC* pMemDC = &dc;
#endif
	
	CFont* pOldFont{};

	dc.GetClipBox(&clipRc);

	
#ifdef DF_USESTL_FXGRID
		if (m_bMemDC)
			pMemDC = std::make_unique<CMemDC>(&dc);

		if ((GetRowCount() - m_StartPnt.y) * m_lDataHeight + m_lHeadHeight < clipRc.bottom)
		{
			bkRect = clipRc;
			bkRect.top = (GetRowCount() - m_StartPnt.y) * m_lDataHeight + m_lHeadHeight;
			pMemDC->FillSolidRect(bkRect, GetSysColor(COLOR_WINDOW));
		}
		if (m_lSumVisColWidth < clipRc.right)
		{
			bkRect = clipRc;
			bkRect.left = m_lSumVisColWidth;
			pMemDC->FillSolidRect(bkRect, GetSysColor(COLOR_WINDOW));
		}

		pOldFont = (CFont*)pMemDC->SelectObject(m_pFont);
		DrawHead(pMemDC.get(), clipRc);
		for (int ii = 0; ii < GetRowCount(); ii++)
			if (!DrawRow(pMemDC.get(), ii, clipRc))
				break;

		if (m_bFixFirstRow && GetRowCount())
		{
			DrawRow(pMemDC.get(), -m_StartPnt.y, clipRc);
		}

		if (m_bFillEmpty)
			DrawEmptyRow(pMemDC.get(), clipRc);

		if (m_bColumnResize && m_lMoveWColumn >= 0)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.left = m_lMoveWPos;
			rc.right = rc.left + 1;
			pMemDC->FillSolidRect(rc, RGB(0, 0, 0));
		}

		pMemDC->SelectObject(pOldFont);
#else
		if (m_bMemDC)
			pMemDC = new CMemDC(&dc);

		if ((GetRowCount() - m_StartPnt.y) * m_lDataHeight + m_lHeadHeight < clipRc.bottom)
		{
			bkRect = clipRc;
			bkRect.top = (GetRowCount() - m_StartPnt.y) * m_lDataHeight + m_lHeadHeight;
			pMemDC->FillSolidRect(bkRect, GetSysColor(COLOR_WINDOW));
		}
		if (m_lSumVisColWidth < clipRc.right)
		{
			bkRect = clipRc;
			bkRect.left = m_lSumVisColWidth;
			pMemDC->FillSolidRect(bkRect, GetSysColor(COLOR_WINDOW));
		}

		pOldFont = (CFont*)pMemDC->SelectObject(m_pFont);
		DrawHead(pMemDC, clipRc);
		for (int ii = 0; ii < GetRowCount(); ii++)
			if (!DrawRow(pMemDC, ii, clipRc))
				break;

		if (m_bFixFirstRow && GetRowCount())
		{
			DrawRow(pMemDC, -m_StartPnt.y, clipRc);
		}

		if (m_bFillEmpty)
			DrawEmptyRow(pMemDC, clipRc);

		if (m_bColumnResize && m_lMoveWColumn >= 0)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.left = m_lMoveWPos;
			rc.right = rc.left + 1;
			pMemDC->FillSolidRect(rc, RGB(0, 0, 0));
		}

		pMemDC->SelectObject(pOldFont);
		if (m_bMemDC && pMemDC && pMemDC->m_hDC)
			delete pMemDC;
#endif

	

	// Do not call CWnd::OnPaint() for painting messages
}

CRect CfxIGrid::DrawCellData(CDC *pDC, CRect cellRc, CString sDat, DWORD dwStyle, BOOL bHead /*= FALSE*/, BOOL bSelected /*= FALSE*/, BOOL bHidden /*= FALSE*/, BOOL bDotLine /*= FALSE*/, BOOL bBottomLine /*= FALSE*/)
{
	CRect drawRect = cellRc;
	pDC->SetBkMode(TRANSPARENT);
	if (bHead)
	{
		pDC->FillSolidRect(cellRc, m_headBgColor);
		pDC->SetTextColor(m_headFgColor);
	}
	else
	{
		if (bSelected)
		{
			pDC->FillSolidRect(cellRc, m_selBgColor);
			//pDC->SetTextColor(m_selFgColor);
			pDC->SetTextColor(m_dataFgColor);
		}
		else
		{
			pDC->FillSolidRect(cellRc, m_dataBgColor);
			pDC->SetTextColor(m_dataFgColor);
		}
	}

	cellRc.DeflateRect(1, 0, 1, 0);
	if (!bHidden)
	{
		if (m_bFixFirstRow && m_bShowEmptyText && !bHead && m_clientRc.top + m_lHeadHeight + m_lDataHeight > cellRc.top)
		{
			sDat = m_sEmptyText;
			cellRc.left = 0;
			cellRc.right = m_clientRc.right;
			dwStyle = DT_CENTER;
		}
		pDC->DrawText(sDat, cellRc, dwStyle | DT_VCENTER | DT_SINGLELINE|DT_NOPREFIX);
		pDC->DrawText(sDat, &drawRect, dwStyle | DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_CALCRECT);
	}
	cellRc.InflateRect(1, 0, 1, 0);

	/*if (wLen)
	{
		//for (int xx = 0; xx < wLen; xx++)
			//if (isalnum((char)wTest[xx]))
			//	wTest[xx] -= 30;
		
		ExtTextOutW(pDC->m_hDC, drawRect.left, drawRect.top, ETO_GLYPH_INDEX, drawRect, wTest, wLen, NULL);
	}*/

	CRect rc;
	cellRc.InflateRect(0, 0, 1, 1);
	GetClientRect(&rc);
	cellRc.top = rc.top;
	cellRc.bottom = rc.bottom;
	if (m_bDataLine)
		pDC->Draw3dRect(cellRc, m_lineColor, m_lineColor);

	if (bDotLine)
		Line(pDC, drawRect.left, drawRect.bottom, drawRect.right, drawRect.bottom, RGB(0, 0, 0), TRUE);

	if (m_bRowGubnLine && bBottomLine)
		Line(pDC, cellRc.left, drawRect.bottom + 1, cellRc.right, drawRect.bottom + 1, RGB(150, 150, 150));

	if (m_bColGubnLine && !bHead)
	{
		Line(pDC, cellRc.right - 1, drawRect.top, cellRc.right - 1, drawRect.bottom + 3, m_lineColor);
		Line(pDC, cellRc.left, drawRect.top, cellRc.left, drawRect.bottom + 3, m_lineColor);
	}

	if (bHead && m_b3DHead)
	{
		cellRc.DeflateRect(1, 1, 1, 1);
		pDC->Draw3dRect(cellRc, 
		RGB(	min(255, GetRValue(m_headBgColor) * 1.3), 
			min(255, GetGValue(m_headBgColor) * 1.3), 
			min(255, GetBValue(m_headBgColor) * 1.3)), 
		RGB(	max(0, GetRValue(m_headBgColor) * 0.7), 
			max(0, GetGValue(m_headBgColor) * 0.7), 
			max(0, GetBValue(m_headBgColor) * 0.7)));
	}
	else if (bHead)
	{
		cellRc.bottom = drawRect.bottom;
		cellRc.DeflateRect(0, 0, 0, -2);
		Line(pDC, cellRc.left, cellRc.top, cellRc.right, cellRc.top, m_lineColor);
		Line(pDC, cellRc.left, cellRc.bottom, cellRc.right, cellRc.bottom, m_lineColor);
		Line(pDC, cellRc.right - 1, cellRc.top , cellRc.right - 1, cellRc.bottom, m_lineColor);
		Line(pDC, cellRc.left, cellRc.top , cellRc.left, cellRc.bottom, m_lineColor);
	}
	return drawRect;
}

void CfxIGrid::DrawHead(CDC *pDC, CRect visibleRc)
{
	CRect cellRc = CRect (-m_StartPnt.x, 0, -m_StartPnt.x, m_lHeadHeight);

	for (int ii = 0; ii < m_arColumn.GetSize(); ii++)
	{
		CIColumn qCol = m_arColumn.GetAt(ii);
		cellRc.right += qCol.m_lWidth;
		DrawCellData(pDC, cellRc, qCol.m_sHeadCaption, DT_CENTER, TRUE, FALSE, (qCol.m_dAttr & IGAT_HIDDEN)?TRUE:FALSE);
		cellRc.left += qCol.m_lWidth;
		if (cellRc.left > visibleRc.right)
			break;
	}
}

BOOL CfxIGrid::DrawRow(CDC *pDC, long nRow, CRect visibleRc)
{
	if (nRow + m_StartPnt.y >= GetRowCount())
		return FALSE;

	if (m_lHeadHeight + m_lDataHeight * nRow > visibleRc.bottom)
		return FALSE;

	CRect cellRc = CRect (0, m_lHeadHeight + m_lDataHeight * nRow, 0, m_lHeadHeight + m_lDataHeight * (nRow + 1));

	if (m_bFixFirstRow && nRow == -m_StartPnt.y)
	{
		cellRc = CRect (0, m_lHeadHeight, 0, m_lHeadHeight + m_lDataHeight * 1);
	}

	CString sData, sTmp;
	const  COLORREF tmpFgColor = m_dataFgColor;
	const COLORREF tmpBgColor = m_dataBgColor;
	DWORD	dwStyle = DT_LEFT;

	cellRc.OffsetRect(-m_StartPnt.x, 0);
	for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
	{
		CIColumn qCol = m_arColumn.GetAt(nCol);
		sData.Empty();
		
		const CStringArray *pArData = GetColPtr(nCol);
		if (pArData && pArData->GetSize() > nRow + m_StartPnt.y)
			sData = pArData->GetAt(nRow + m_StartPnt.y);
		
		if (qCol.m_dAttr & IGAT_CENTER)
			dwStyle = DT_CENTER;
		else if (qCol.m_dAttr & IGAT_RIGHT)
			dwStyle = DT_RIGHT;
		else 
			dwStyle = DT_LEFT;

		if (m_b2RowColor)
			m_dataBgColor = ((nRow + m_StartPnt.y)%2 ^ m_bFirstRowColor?m_dataBgColor1:m_dataBgColor2);

		if (qCol.m_dAttr & (IGAT_CONDITION|IGAT_CONDITIONx))
		{
			if (!sData.IsEmpty())
			{
				switch (sData.GetAt(0))
				{
				case '+':
					m_dataFgColor = m_upColor;	break;
				case '-':
					m_dataFgColor = m_dnColor;	break;
				default:
					m_dataFgColor = m_eqColor;	break;
				}
			}
		}

		if (qCol.m_dAttr & IGAT_CONTRAST)
		{
			if (!sData.IsEmpty())
			{
				switch (sData.GetAt(0))
				{
				case '1':	
					m_dataFgColor = m_upRvColor;	
					m_dataBgColor = m_upColor;
					break;
				case '+':
				case '2':
					m_dataFgColor = m_upColor;	break;
				case '4':
					m_dataFgColor = m_dnRvColor;
					m_dataBgColor = m_upColor;
					break;
				case '-':
				case '5':
					m_dataFgColor = m_dnColor;	break;
				default:
					m_dataFgColor = m_eqColor;	break;
				}
				sTmp = sData;
				sData = sData.Mid(1);
			}
		}

		if (qCol.m_dAttr & IGAT_USERCOLOR)
		{
			m_dataFgColor = GetCellFgColor(m_StartPnt.y + nRow, nCol);
			m_dataBgColor = GetCellBgColor(m_StartPnt.y + nRow, nCol);
		}

		if ((qCol.m_dAttr & IGAT_EDITFORMAT) && !qCol.m_sEditFormat.IsEmpty())
			sData = SetEditMask(sData, qCol.m_sEditFormat);
		sData = getFormatData(sData, qCol.m_dAttr);

		cellRc.right += qCol.m_lWidth;
		
		if (m_bFixFirstRow && nRow == -m_StartPnt.y)
		{
			m_dataBgColor = m_firstRowColor;
		}

		BOOL bDotLine = FALSE;
		if ((qCol.m_dAttr & IGAT_MOUSELINE) && nCol == m_mouseOverPnt.x && 
			(m_mouseOverPnt.y == m_StartPnt.y + nRow || (m_bFixFirstRow && nRow == -m_StartPnt.y && m_mouseOverPnt.y == m_StartPnt.y)))
		{
			if (!m_bInCellMargin)
				bDotLine = TRUE;
		}

		if ((qCol.m_dAttr & IGAT_USERIMAGE) && qCol.m_lReference == -2)	// 자신의 이미지만 표시할것인가.
		{
			CRect drawRc = cellRc;
			DrawCellData(pDC, cellRc, "", dwStyle, FALSE, m_nSelectedRow == m_StartPnt.y + nRow, FALSE, bDotLine,
				(((nRow + m_StartPnt.y) % 5) == m_nGubnLineIdx));
			drawRc.left += (cellRc.Width() - 15) / 2;
			DrawImage(pDC, drawRc, m_StartPnt.y + nRow, nCol, nCol);
		}
		else
		{
			CRect drawRc = DrawCellData(pDC, cellRc, sData, dwStyle, FALSE, m_nSelectedRow == m_StartPnt.y + nRow, FALSE, bDotLine,
				(((nRow + m_StartPnt.y) % 5) == m_nGubnLineIdx));
			
			if (qCol.m_dAttr & IGAT_USERIMAGE)
			{
				drawRc.left = drawRc.right + 5;
				drawRc.right = cellRc.right;
				if (drawRc.right - drawRc.left > 15)
					DrawImage(pDC, drawRc, m_StartPnt.y + nRow, nCol, qCol.m_lReference);
			}
		}

		if (qCol.m_dAttr & IGAT_CONTRAST)
		{
			cellRc.DeflateRect(1, 2, 1, 2);
			drawContrast(pDC, cellRc, sTmp, m_dataFgColor);
			cellRc.InflateRect(1, 2, 1, 2);
		}
		m_dataFgColor = tmpFgColor;
		m_dataBgColor = tmpBgColor;
		cellRc.left += qCol.m_lWidth;
		if (cellRc.left > visibleRc.right)
			break;
	}

	//m_dataBgColor = tmpBgColor;

	return TRUE;
}

void CfxIGrid::DrawEmptyRow(CDC *pDC, CRect visibleRc)
{
	int nRow = 0;
	while (nRow + m_StartPnt.y < GetRowCount())
	{
		if (m_lHeadHeight + m_lDataHeight * nRow > visibleRc.bottom)
			return;
		nRow++;
	}

	while (m_lHeadHeight + m_lDataHeight * nRow <= visibleRc.bottom)
	{
		CRect cellRc = CRect (0, m_lHeadHeight + m_lDataHeight * nRow, 0, m_lHeadHeight + m_lDataHeight * (nRow + 1));
		
		if (m_b2RowColor)
			m_dataBgColor = ((nRow + m_StartPnt.y)%2 ^ m_bFirstRowColor?m_dataBgColor1:m_dataBgColor2);
		cellRc.OffsetRect(-m_StartPnt.x, 0);
		for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
		{
			CIColumn qCol = m_arColumn.GetAt(nCol);
			cellRc.right += qCol.m_lWidth;
			DrawCellData(pDC, cellRc, " ", DT_LEFT, FALSE, m_nSelectedRow == m_StartPnt.y + nRow);
			cellRc.left += qCol.m_lWidth;
			if (cellRc.left > visibleRc.right)
				break;
		}
		nRow++;
	}

	if (GetRowCount() == 0)
	{
		CRect firstRowRc;
		GetClientRect(&firstRowRc);
		firstRowRc.top = m_lHeadHeight;
		firstRowRc.bottom = m_lHeadHeight + m_lDataHeight;

		pDC->DrawText(m_sEmptyText, firstRowRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CfxIGrid::DrawAddRow(CDC *pDC, bool bTop)
{
	CRect clientRc;

	GetClientRect(&clientRc);
	clientRc.top = m_lHeadHeight + 1;
	if (bTop && !(m_bFixScroll && m_StartPnt.y))
		ScrollWindow(0, m_lDataHeight, &clientRc);

	if (bTop)
	{
		if (m_b2RowColor)
			m_dataBgColor = m_bFirstRowColor?m_dataBgColor2:m_dataBgColor1;
		clientRc.bottom = m_lHeadHeight + m_lDataHeight;
		
		if (m_bFixFirstRow)
		{
			DrawRow(pDC, -m_StartPnt.y, clientRc);
			clientRc.OffsetRect(0, m_lDataHeight);
			DrawRow(pDC, 1, clientRc);
		}
		else
			DrawRow(pDC, 0, clientRc);
	}
	else
	{
		DrawRow(pDC, GetRowCount() - 1 - m_StartPnt.y, clientRc);
	}
}

void CfxIGrid::SetData(long nRow, long nCol, CString sDat)
{
	if (nRow >= GetRowCount() || nCol >= m_arData.GetSize())
		return;
	CStringArray *pArData = GetColPtr(nCol);
	
	while (pArData->GetSize() <= nRow)
		pArData->Add("");
	pArData->SetAt(nRow, sDat);
}

CString CfxIGrid::GetData(long nRow, long nCol)
{
	if (nRow >= GetRowCount() || nCol >= m_arData.GetSize())
		return "";
	const CStringArray *pArData = GetColPtr(nCol);
	if (pArData->GetSize() <= nRow)
		return "";
	return pArData->GetAt(nRow);
}

void CfxIGrid::AddRow(CString sDat, bool bTop)
{
	int nCol = 0;
	CDC *pDC = GetDC();
	CFont	*pOldFont;
	pOldFont = pDC->SelectObject(m_pFont);
	const BOOL bEmpty = GetRowCount()?TRUE:FALSE;

	while (!sDat.IsEmpty())
	{
		CStringArray *pArData = GetColPtr(nCol++);
		CString sData = Parser(sDat, "\t");
		if (pArData)
		{
			if (bTop)
				pArData->InsertAt(0, sData);
			else
				pArData->Add(sData);
		}
	}

	while (nCol < GetColCount())
	{
		CStringArray *pArData = GetColPtr(nCol++);
		CString sData = "";
		if (pArData)
		{
			if (bTop)
				pArData->InsertAt(0, sData);
			else
				pArData->Add(sData);
		}
	}


	if (bTop)
	{
		m_bFirstRowColor = !m_bFirstRowColor;
		if (m_nSelectedRow >= 0)
			m_nSelectedRow++;
		if (m_bRowGubnLine)
			m_nGubnLineIdx = ++m_nGubnLineIdx % 5;
	}
	
	adjustScroll(SB_VERT);
	if (bTop && m_bFixScroll && m_StartPnt.y)
	{
		m_StartPnt.y++;
		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT) + 1);
	}

	if (bEmpty)
		Invalidate();
	else
		DrawAddRow(pDC, bTop);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

void CfxIGrid::DeleteRow(long nRow)
{
	if (nRow >= GetRowCount())
		return;
	for (int nCol = 0; nCol < GetColCount(); nCol++)
	{
		CStringArray *pArData = GetColPtr(nCol);
		if (pArData && pArData->GetSize() > nRow)
			pArData->RemoveAt(nRow);
	}
	
	adjustScroll(SB_VERT);

	if (nRow >= m_StartPnt.y)
	{
		CRect clientRc;
		GetClientRect(&clientRc);
		if (m_lHeadHeight + m_lDataHeight * (nRow - m_StartPnt.y) < clientRc.Height())
		{
			clientRc.top = m_lHeadHeight + m_lDataHeight * (nRow - m_StartPnt.y);
			InvalidateRect(clientRc);
		}
	}
}

void CfxIGrid::ResetContent()
{
#ifdef DF_USESTL_FXGRID 
	m_arData.RemoveAll();
#else
	while (m_arData.GetSize())
	{
		m_arData.GetAt(0)->RemoveAll();
		delete m_arData.GetAt(0);
		m_arData.RemoveAt(0);
	}
#endif
	
	m_StartPnt.x = 0; m_StartPnt.y = 0;
	SetScrollPos32(SB_VERT, 0, TRUE);
	SetScrollPos32(SB_HORZ, 0, TRUE);
	adjustScroll(SB_VERT);
	Invalidate();
}


CString CfxIGrid::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}


BOOL CfxIGrid::RegisterWndClass()
{
	WNDCLASS windowclass{};
	HINSTANCE hInst = AfxGetInstanceHandle();
	
	if (!(::GetClassInfo(hInst, IGRIDCLASS, &windowclass)))
	{
		windowclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		windowclass.lpfnWndProc = ::DefWindowProc;
		windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
		windowclass.hInstance = hInst;
		windowclass.hIcon = NULL;
		windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
		windowclass.lpszMenuName = NULL;
		windowclass.lpszClassName = IGRIDCLASS;
		
		if (!AfxRegisterClass(&windowclass))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	
	return TRUE;
}

void CfxIGrid::AddQColumn(CIColumn qCol, BOOL bMakeDataArray /*= TRUE*/)
{
	m_lSumColumnWidth += qCol.m_lWidth;
	m_lSumVisColWidth += qCol.m_lWidth;
	m_arColumn.Add(qCol);
	if (bMakeDataArray)
	{
		
#ifdef DF_USESTL_FXGRID 
		auto pArData = std::make_shared<CStringArray>();
#else
		CStringArray* pArData = new CStringArray;
#endif
		m_arData.Add(pArData);
	}
	adjustScroll();
}



CString CfxIGrid::CommaModify(CString string)
{
	if (string.GetLength() <= 3)
		return string;

	CString floatStr;
	string.Replace(",", "");

	string.TrimLeft();
	if (string.Find('.') >= 0)
	{
		floatStr = string.Mid(string.Find('.'));
		string = string.Left(string.Find('.'));
	}
	else
		floatStr.Empty();
	
	if (string.GetLength() <= 3)
		return string + floatStr;
	
	for (int ii = string.GetLength() - 4; ii >= 0; ii -= 3)
	{
		if (string.GetAt(ii) >= '0' && string.GetAt(ii) <= '9')
			string.Insert(ii + 1, ',');
	}
	
	string += floatStr;
	
	return string;
}

void CfxIGrid::adjustScroll(int wBar)
{
	CRect clientRc;
	GetClientRect(&clientRc);
	if (wBar == SB_HORZ || wBar == SB_BOTH)
	{
		if (m_lSumColumnWidth > clientRc.Width())
		{
			SCROLLINFO info;
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
			GetScrollInfo(SB_HORZ, &info);
			info.fMask  |= (m_wDisableBar & SB_HORZ? SIF_DISABLENOSCROLL:NULL);
			info.cbSize = sizeof(SCROLLINFO);
			info.nMin = 0; info.nMax = m_lSumColumnWidth - clientRc.Width();
			info.nPage     = 1;
			info.nPos      = m_StartPnt.x;
			info.nTrackPos = m_StartPnt.x;
			
			SetScrollInfo(SB_HORZ, &info);
			ShowScrollBar(SB_HORZ);
		}
		else
		{
			if (m_wDisableBar & SB_HORZ)
			{
				SCROLLINFO info;
				info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
				SetScrollInfo(SB_HORZ, &info);
			}
			else
			{
				ShowScrollBar(SB_HORZ, FALSE);
				m_StartPnt.x = 0;
			}
		}
	}

	if (wBar == SB_VERT || wBar == SB_BOTH)
	{
		if (m_lHeadHeight + m_lDataHeight * GetRowCount() > clientRc.Height())
		{
			SCROLLINFO info;
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &info);
			info.fMask |= (m_wDisableBar & SB_VERT?SIF_DISABLENOSCROLL:NULL); 
			info.cbSize = sizeof(SCROLLINFO);
			info.nMin = 0; info.nMax = GetRowCount() - (clientRc.Height() - m_lHeadHeight) / m_lDataHeight;
			info.nPage     = 1;
			info.nPos      = m_StartPnt.y;
			info.nTrackPos = m_StartPnt.y;
			
			SetScrollInfo(SB_VERT, &info);
			ShowScrollBar(SB_VERT);
		}
		else
		{
			if (m_wDisableBar & SB_VERT)
			{
				SCROLLINFO info;
				info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
				SetScrollInfo(SB_VERT, &info);
			}
			else
			{
				ShowScrollBar(SB_VERT, TRUE);
			}
		}
	}
}

void CfxIGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int	scrollPos{}, scrollMax{};
	CRect clientRc;

	if (m_bEnableTip && m_pTipCtrl)
		m_pTipCtrl->Pop();

	GetClientRect(&clientRc);
	clientRc.top = m_lHeadHeight;
	scrollPos = GetScrollPos32(SB_VERT);
	scrollMax = GetScrollMax32(SB_VERT);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos + 1 <= scrollMax)
		{
			scrollPos = min(scrollPos + 1, scrollMax);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			clientRc.top += m_lDataHeight + 1;
			if (m_bFixFirstRow)
				clientRc.top += m_lDataHeight + 1;
			clientRc.bottom --;
			ScrollWindow(0, -m_lDataHeight, &clientRc);
			clientRc.top = clientRc.bottom - m_lDataHeight;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_LINEUP:
		if (scrollPos - 1 >= 0)
		{
			scrollPos = max(scrollPos - 1, 0);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			clientRc.bottom -= (m_lDataHeight + 1);
			clientRc.top++;
			if (m_bFixFirstRow)
				clientRc.top += m_lDataHeight + 1;
			ScrollWindow(0, m_lDataHeight, &clientRc);
			clientRc.bottom = clientRc.top + m_lDataHeight;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEDOWN:
		if (scrollPos < scrollMax)
		{
			scrollPos += (clientRc.Height() / m_lDataHeight - (m_bFixFirstRow?1:0));
			scrollPos = min(scrollPos, scrollMax);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEUP:
		if (scrollPos > 0)
		{
			scrollPos -= (clientRc.Height() / m_lDataHeight - (m_bFixFirstRow?1:0));
			scrollPos = max(scrollPos, 0);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		if (nPos >= 0 && (long)nPos <= scrollMax)
		{
			const long nDelta = nPos - m_StartPnt.y;
			if (nDelta > 0 && nDelta < clientRc.Height() / m_lDataHeight)
			{
				clientRc.top += m_lDataHeight * nDelta + 1;
				clientRc.bottom--;
				if (m_bFixFirstRow)
					clientRc.top += m_lDataHeight + 1;
				ScrollWindow(0, (-1 * m_lDataHeight) * nDelta, &clientRc);
				clientRc.top = clientRc.bottom - m_lDataHeight * nDelta;
				
			}
			else if (nDelta < 0 && -nDelta < clientRc.Height() / m_lDataHeight)
			{
				clientRc.bottom += (m_lDataHeight * nDelta - 1);
				clientRc.top++;
				if (m_bFixFirstRow)
					clientRc.top += m_lDataHeight + 1;
				ScrollWindow(0, (-1 * m_lDataHeight) * nDelta, &clientRc);
				clientRc.bottom = clientRc.top - m_lDataHeight * nDelta;
			}
			m_StartPnt.y = nPos;
			SetScrollPos32(SB_VERT, nPos);
			InvalidateRect(&clientRc);
		}
		break;
	case SB_TOP:
		break;
	case SB_BOTTOM:
		break;
	default:
		break;
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CfxIGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int	scrollPos{}, scrollMax{};
	CRect clientRc;

	if (m_bEnableTip && m_pTipCtrl)
		m_pTipCtrl->Pop();
	
	GetClientRect(&clientRc);
	scrollPos = GetScrollPos32(SB_HORZ);
	scrollMax = GetScrollMax32(SB_HORZ);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos + 1 <= scrollMax)
		{
			int nTWidth = 0;
			for (int ii = 0; ii < GetColCount() && m_StartPnt.x >= nTWidth; ii++)
				nTWidth += GetColumnWidth(ii);
			const long nDelta = min(nTWidth - m_StartPnt.x, scrollMax - m_StartPnt.x);
			SetScrollPos32(SB_HORZ, scrollPos + nDelta);
			m_StartPnt.x += nDelta;
			m_lSumVisColWidth = m_lSumColumnWidth - m_StartPnt.x;
			clientRc.left = nDelta;
			ScrollWindow(-clientRc.left, 0, &clientRc);
			clientRc.left = clientRc.right - nDelta;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_LINEUP:
		if (scrollPos - 1 >= 0)
		{
			int nTWidth = 0;
			int ii;
			for (ii = 0; ii < GetColCount() && m_StartPnt.x > nTWidth; ii++)
				nTWidth += GetColumnWidth(ii);
			for (--ii; ii > 0 && m_StartPnt.x <= nTWidth; ii--)
				nTWidth -= GetColumnWidth(ii);

			const long nDelta = max(m_StartPnt.x - nTWidth, 0);
			SetScrollPos32(SB_HORZ, scrollPos - nDelta);
			m_StartPnt.x -= nDelta;
			m_lSumVisColWidth = m_lSumColumnWidth - m_StartPnt.x;
			clientRc.right -= nDelta;
			ScrollWindow(nDelta, 0, &clientRc);
			clientRc.right = clientRc.left + nDelta;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEDOWN:
		if (scrollPos < scrollMax)
		{
			const long nDelta = min(clientRc.Width(), scrollMax - scrollPos);
			
			scrollPos += nDelta;
			scrollPos = min(scrollPos, scrollMax);
			SetScrollPos32(SB_HORZ, scrollPos);
			m_StartPnt.x = scrollPos;
			m_lSumVisColWidth = m_lSumColumnWidth - m_StartPnt.x;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEUP:
		if (scrollPos > 0)
		{
			const long nDelta = min(clientRc.Width(), scrollPos);

			scrollPos -= nDelta;
			scrollPos = max(scrollPos, 0);

			SetScrollPos32(SB_HORZ, scrollPos);
			m_StartPnt.x = scrollPos;
			m_lSumVisColWidth = m_lSumColumnWidth - m_StartPnt.x;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		if (nPos >= 0 && (long)nPos <= scrollMax)
		{
			m_StartPnt.x = nPos;
			m_lSumVisColWidth = m_lSumColumnWidth - nPos;

			SetScrollPos32(SB_HORZ, nPos);
			InvalidateRect(&clientRc);
		}
		break;
	case SB_TOP:
		break;
	case SB_BOTTOM:
		break;
	default:
		break;
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

long CfxIGrid::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_TRACKPOS))
			return sinfo.nTrackPos;
	}
	else 
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_POS))
			return sinfo.nPos;
	}

	return 0;
}

long CfxIGrid::GetScrollMax32(int nBar)
{
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (GetScrollInfo(nBar, &sinfo, SIF_RANGE))
		return sinfo.nMax;
	return 0;
}

BOOL CfxIGrid::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;

	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

BOOL CfxIGrid::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

long CfxIGrid::GetColumnWidth(long nCol)
{
	if (m_arColumn.GetSize() <= nCol)
		return 0;
	return m_arColumn.GetAt(nCol).m_lWidth;
}

int CfxIGrid::GetLastColCnt()
{
	CRect clientRc;
	GetClientRect(&clientRc);
	int nCol;
	for (nCol = m_arColumn.GetSize() - 1; nCol >= 0 && clientRc.right >= 0; nCol--)
		clientRc.right -= GetColumnWidth(nCol);
	return nCol + 2;
}

void CfxIGrid::drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr)
{
	if (sText.IsEmpty())
		return;
	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height() - 4;
	CSize	size;
	CPoint	pts[7];
	CPen* pOldPen{}, pen;
	CBrush* pOldBrush{}, brush;
	
	pen.CreatePen(PS_SOLID, 1, clr);
	pOldPen = (CPen *) pDC->SelectObject(&pen);

	brush.CreateSolidBrush(clr); // bkcolor
	pOldBrush = (CBrush *) pDC->SelectObject(&brush);

	pRc.top = (cellRc.top + cellRc.Height() / 2) - (int)(hh / 2);
	pRc.bottom = (int)(pRc.top + hh);
	
	size = pDC->GetOutputTextExtent("9");
	pRc.left = pRc.right - sText.GetLength() * size.cx -10;

	if (size.cx > 10)
		pRc.right = (int)(pRc.left + (size.cx * 0.8));
	else
		pRc.right = pRc.left + size.cx;
	
	hh = (float)(int)(pRc.Height()+1)/2;
	hh = (float)(int)((hh+1)/2) * 2 - 1;
	int	ww = pRc.Width();
	if (ww >= 4)
		ww = (ww / 2 * 2);
	pRc.right = pRc.left + ww;
	const int	rr = ww % 3;
	ww /= 3;
	if (rr >= 2 && pRc.Width() > 3)
		ww++;
	
	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		switch(sText.GetAt(0))
		{
		case '1':	// 상한
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left + 1;
			pts[6].x = pRc.right - 1;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
			break;
		case '+':
		case '2':	// triangle up
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
			break;
		case '4':	// 하한
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right - 1;
			pts[6].x = pRc.left + 1;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
		case '-':
		case '5':	// triangle down
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
			break;
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	pen.DeleteObject();
}

CString CfxIGrid::getFormatData(CString src, DWORD dAttr)
{
	CString sRtn = src;
	
	if ((dAttr & IGAT_CONDITION) && !sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	
	if ((dAttr & IGAT_CONDITIONx) && !sRtn.IsEmpty() && sRtn.GetAt(0) == '+')
		sRtn = sRtn.Mid(1);
	
	if (dAttr & IGAT_ZEROSUPx)
	{
		sRtn.TrimLeft();
		sRtn.TrimLeft('0');
	}

	if (dAttr & IGAT_ZEROSUP)
	{
		sRtn.TrimLeft();
		sRtn.TrimLeft('0');
		if (sRtn.IsEmpty())
			sRtn = "0";
	}
	if (!sRtn.IsEmpty() && sRtn.GetAt(0) == '.')
		sRtn = '0' + sRtn; 
	
	if (dAttr & IGAT_COMMA)
		sRtn = CommaModify(sRtn);
	
	sRtn.TrimLeft();
	
	return sRtn;
}

CString CfxIGrid::SetEditMask(CString sSrc, CString sMask)
{
	int nLen = 0;
	CString sRtn = "";

	for (int ii = 0; ii < sMask.GetLength() && sSrc.GetLength() > nLen; ii++)
	{
		if (sMask.GetAt(ii) == '9')
		{
			if (sSrc.GetAt(nLen) >= '0' && sSrc.GetAt(nLen) <= '9')
				sRtn += sSrc.GetAt(nLen++);
		}
		else if (sMask.GetAt(ii) == 'A')
			sRtn += sSrc.GetAt(nLen++);
		else if (sMask.GetAt(ii) == sSrc.GetAt(nLen))
			sRtn += sSrc.GetAt(nLen++);
		else
			sRtn += sMask.GetAt(ii);
	}
	return sRtn;
}

BOOL CfxIGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (GetScrollMax32(SB_VERT))
	{
		if (zDelta < 0)
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
		}
		else
		{
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
		}
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CfxIGrid::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	BOOL bMouseArrow = TRUE;
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	if (m_bColumnResize)
	{
		if (nFlags == MK_LBUTTON && m_lMoveWColumn >= 0)
		{
			m_lMoveWPos = point.x;
			Invalidate();
		}
		else if (nFlags != MK_LBUTTON && m_lMoveWColumn < 0)
		{
			CRect clientRc = m_clientRc;
			long nColumnLine = -m_StartPnt.x;
			
			for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
			{
				if (m_arColumn.GetAt(nCol).m_dAttr & IGAT_HIDDEN)
					continue;
				nColumnLine += GetColumnWidth(nCol);
				clientRc.left  = nColumnLine - 2;
				clientRc.right = nColumnLine + 2;
				if (clientRc.PtInRect(point))
				{
					m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
					bMouseArrow = FALSE;
				}
			}
		}
	}

	BOOL bMousePosition = FALSE;
	CRect cellRc = m_clientRc;
	CIColumn qCol;
	const CPoint beforeMousePnt = m_mouseOverPnt;

	if (nFlags != MK_LBUTTON && point.y > m_lHeadHeight)
	{
		long nColumnLine = -m_StartPnt.x;
		
		for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
		{
			qCol = m_arColumn.GetAt(nCol);
			if (qCol.m_dAttr & IGAT_HIDDEN)
				continue;
			
			cellRc.left  = nColumnLine;
			nColumnLine += GetColumnWidth(nCol);
			cellRc.right = nColumnLine;
			if (cellRc.PtInRect(point))
			{
				const int nRow = (point.y - m_lHeadHeight) / m_lDataHeight + m_StartPnt.y;
				
				if (nRow < 0)
					break;

				m_mouseOverPnt.x = nCol;
				m_mouseOverPnt.y = nRow;
				bMousePosition = TRUE;

				if (!m_beforeMouseOverRect.IsRectEmpty())
					InvalidateRect(m_beforeMouseOverRect);

				if (qCol.m_dAttr & IGAT_MOUSELINE)
				{
					m_beforeMouseOverRect = cellRc;
					InvalidateRect(cellRc);
				}
				if ((qCol.m_dAttr & IGAT_MOUSEHAND) && bMouseArrow)
				{
					if (qCol.m_dAttr & IGAT_MOUSELINE)
					{
						cellRc.top = m_lHeadHeight + (nRow - m_StartPnt.y) * m_lDataHeight;
						cellRc.bottom += cellRc.top + m_lDataHeight;

						if (!GetDrawRect(m_mouseOverPnt.y, m_mouseOverPnt.x, cellRc).PtInRect(point))
						{
							m_bInCellMargin = TRUE;
							break;
						}
						else
							m_bInCellMargin = FALSE;
					}
					m_hCursor = m_hCursorHand;
				}

				break;
			}
		}
	}

	SetCursor(m_hCursor);

	if (m_bEnableTip && bMousePosition)
	{
		const int nCol = m_mouseOverPnt.x;
		const int nRow = m_mouseOverPnt.y;
		CString sData = GetData((m_bFixFirstRow && m_StartPnt.y == nRow)?0:nRow, nCol);
		CRect textRc;

		cellRc.top = m_lHeadHeight + (nRow - m_StartPnt.y) * m_lDataHeight;
		cellRc.bottom = cellRc.top + m_lDataHeight;
		
		textRc = cellRc;
		textRc.right = textRc.left + GetSystemMetrics(SM_CXSCREEN);
		
		CDC *pDC = GetDC();
		CFont *pOldFont = (CFont*)pDC->SelectObject(m_pFont);
		pDC->DrawText(sData, &textRc, DT_LEFT| DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_CALCRECT);
		pDC->SelectObject(pOldFont);
		
		ReleaseDC(pDC);
		
		if (!sData.IsEmpty() && textRc.Width() > cellRc.Width())
		{
			if ((qCol.m_dAttr & IGAT_EDITFORMAT) && !qCol.m_sEditFormat.IsEmpty())
				sData = SetEditMask(sData, qCol.m_sEditFormat);
			
			CString sTip;
			StripTag(getFormatData(sData, qCol.m_dAttr), sTip);
			if (m_pTipCtrl)
			{
				m_pTipCtrl->AddTool(this, sTip, textRc, GetDlgCtrlID());
				m_pTipCtrl->Activate(TRUE);
			}
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CfxIGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bColumnResize)
	{
		CRect clientRc;
		long nColumnLine = -m_StartPnt.x;
		
		GetClientRect(&clientRc);
		clientRc.top    = 2;
		clientRc.bottom = m_lHeadHeight;
		
		for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
		{
			nColumnLine += GetColumnWidth(nCol);
			clientRc.left  = nColumnLine - 2;
			clientRc.right = nColumnLine + 2;
			if (clientRc.PtInRect(point))
			{
				SetCapture();
				m_lMoveWColumn = nCol;
				m_lMoveWPos    = nColumnLine;
				m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
				Invalidate();
				break;
			}
		}
	}

	if (m_bSelection && m_lMoveWColumn < 0 && point.y > m_lHeadHeight && point.x <= m_lSumVisColWidth)
	{
		const long beforeSelectedRow = m_nSelectedRow;
		CRect clientRc = m_clientRc;

		clientRc.top = m_lHeadHeight;
		clientRc.bottom = m_lHeadHeight + m_lDataHeight;

		for (m_nSelectedRow = 0; !clientRc.PtInRect(point); clientRc.OffsetRect(0, m_lDataHeight))
			m_nSelectedRow++;

		if (!m_bFixFirstRow || m_nSelectedRow != 0)
			m_nSelectedRow += m_StartPnt.y;

		if (m_nSelectedRow > GetRowCount())
			m_nSelectedRow = -1;

		if (m_nSelectedRow >= 0)
		{
			long nColumnLine = -m_StartPnt.x;
			CIColumn qCol;
			CRect cellRc = clientRc;
			
			for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
			{
				qCol = m_arColumn.GetAt(nCol);
				if (qCol.m_dAttr & IGAT_HIDDEN)
					continue;
				
				cellRc.left  = nColumnLine;
				nColumnLine += GetColumnWidth(nCol);
				cellRc.right = nColumnLine;
				if (((qCol.m_dAttr & (IGAT_MOUSEHAND|IGAT_MOUSELINE)) == (IGAT_MOUSEHAND|IGAT_MOUSELINE)) &&
					cellRc.PtInRect(point))
				{
					const CRect drawRc = GetDrawRect(m_nSelectedRow, nCol, cellRc);
					if (!drawRc.PtInRect(point))
						m_nSelectedRow = -1;
					break;
				}
			}
		}

		InvalidateRect(clientRc);

		if (beforeSelectedRow - m_StartPnt.y >= 0)
		{
			if (!m_bFixFirstRow || m_nSelectedRow != 0)
				clientRc.OffsetRect(0, m_lDataHeight * (beforeSelectedRow - m_nSelectedRow));
			else
				clientRc.OffsetRect(0, m_lDataHeight * (beforeSelectedRow - m_nSelectedRow - m_StartPnt.y));
			InvalidateRect(clientRc);
		}
		else if (m_bFixFirstRow && beforeSelectedRow == 0)
		{
			clientRc.top = m_lHeadHeight;
			clientRc.bottom = clientRc.top + m_lDataHeight;
			InvalidateRect(clientRc);
		}

	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CfxIGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bColumnResize)
	{
		if (m_lMoveWColumn >= 0)
		{
			CIColumn qCol = m_arColumn.GetAt(m_lMoveWColumn);
			for (int nCol = 0; nCol < m_lMoveWColumn; nCol++)
				m_lMoveWPos -= GetColumnWidth(nCol);
			m_lMoveWPos += m_StartPnt.x;

			m_lSumColumnWidth -= qCol.m_lWidth;
			m_lSumVisColWidth -= qCol.m_lWidth;
			
			if (m_lMoveWPos < 5)
				m_lMoveWPos = 5;
			qCol.m_lWidth = m_lMoveWPos;

			m_lSumColumnWidth += qCol.m_lWidth;
			m_lSumVisColWidth += qCol.m_lWidth;
			
			m_arColumn.SetAt(m_lMoveWColumn, qCol);
			m_lMoveWColumn = -1;
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			adjustScroll(SB_HORZ);
			ReleaseCapture();
			Invalidate();
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CfxIGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest)
	{
	case HTCLIENT:
		SetCursor(m_hCursor);
		return TRUE;
	case HTHSCROLL:
	case HTVSCROLL:
		break;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

long CfxIGrid::GetRowCount()
{
	if (!m_arData.GetSize())
		return 0;
	return m_arData.GetAt(0)->GetSize();
}

int CfxIGrid::GetColCount()
{
	return m_arColumn.GetSize();
}


void CfxIGrid::ClearColumn()
{
	m_arColumn.RemoveAll();
	m_lSumColumnWidth = 0;
	m_lSumVisColWidth = 0;
}

void CfxIGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	adjustScroll(SB_BOTH);
	GetClientRect(&m_clientRc);
}

int CfxIGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (lpCreateStruct->style & WS_VSCROLL)
		m_wDisableBar = SB_VERT;
	if (lpCreateStruct->style & WS_HSCROLL)
		m_wDisableBar |= SB_HORZ;

	if (m_bEnableTip)
	{
		CreateTooltip();
	}

	return 0;
}

COLORREF CfxIGrid::GetCellFgColor(int nRow, int nCol)
{
	return m_dataFgColor;
}

COLORREF CfxIGrid::GetCellBgColor(int nRow, int nCol)
{
	return m_dataBgColor;
}

CString CfxIGrid::GetRowData(int nRow, BOOL bFormat /*= FALSE*/)
{
	if (nRow >= GetRowCount())
		return "";
	CString sData = "";

	for (int ii = 0; ii < GetColCount(); ii++)
	{
		CIColumn qCol = m_arColumn.GetAt(ii);
		if (!(qCol.m_dAttr & IGAT_HIDDEN))
		{
			if (nRow < GetColPtr(ii)->GetSize())
			{
				if (bFormat && (qCol.m_dAttr & IGAT_EDITFORMAT) && !qCol.m_sEditFormat.IsEmpty())
					sData += SetEditMask(GetColPtr(ii)->GetAt(nRow), qCol.m_sEditFormat) + "\t";
				else
					sData += GetColPtr(ii)->GetAt(nRow) + "\t";
			}
			else
				sData += "\t";
		}
	}
	return sData;
}



void CfxIGrid::DrawImage(CDC *pDC, CRect drawRc, int nRow, int nCol, int nRef)
{
	// user image draw
}

CStringArray* CfxIGrid::GetColPtr(int nCol)
{
	if (nCol < 0 || nCol >= GetColCount())
		return NULL;

	while (nCol >= m_arData.GetSize())
	{
#ifdef DF_USESTL_FXGRID 
		auto pArData = std::make_shared<CStringArray>();
#else
		CStringArray* pArData = new CStringArray;
#endif
		m_arData.Add(pArData);
	}

#ifdef DF_USESTL_FXGRID 
	return m_arData.GetAt(nCol).get();
#else
	return m_arData.GetAt(nCol);
#endif
	
}

void CfxIGrid::RedrawRow(int nRow)
{
	CRect clientRc;
	CDC *pDC = GetDC();
	
	GetClientRect(&clientRc);
	clientRc.top = m_lHeadHeight + 1;

	CFont *pOldFont = (CFont*)pDC->SelectObject(m_pFont);
	
	DrawRow(pDC, nRow - m_StartPnt.y, clientRc);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

void CfxIGrid::Line(CDC *pDC, int fromL, int fromT, int toR, int toB, COLORREF clr, BOOL bStrip /*= FALSE*/)
{
	CPen* pOldPen{}, pen;
	if (!bStrip)
		pen.CreatePen(PS_SOLID, 1, clr);
	else
		pen.CreatePen(PS_DOT, 1, clr);
	pOldPen = (CPen *) pDC->SelectObject(&pen);

	pDC->MoveTo(fromL, fromT);
	pDC->LineTo(toR, toB);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CfxIGrid::OnDestroy() 
{
	if (m_pTipCtrl && IsWindow(m_pTipCtrl->GetSafeHwnd()))
	{
		m_pTipCtrl->DestroyWindow();
#ifndef DF_USESTL_FXGRID 
		delete m_pTipCtrl;
#endif
		m_pTipCtrl = NULL;
	}
	else if (m_pTipCtrl)
	{
#ifndef DF_USESTL_FXGRID 
		delete m_pTipCtrl;
#endif
		m_pTipCtrl = NULL;
	}

	CWnd::OnDestroy();	
}

void CfxIGrid::SetEmptyText(CString sEmptyText)
{
	m_sEmptyText = sEmptyText;
}

#define MAXINT	2147483647
void CfxIGrid::StripTag(LPCTSTR source, CString &dest)
{
	CString src(source), tag;
	int find{}, idx = 0;
	
	while (TRUE) {
		find = src.Find("<", idx);
		
		if (find < 0) break;
		
		idx = find;
		
		CString sTmp((LPCTSTR)src + find + 1);
		sTmp.TrimLeft();

		if (!sTmp.IsEmpty() && !((sTmp.GetAt(0) >= 'a' && sTmp.GetAt(0) <= 'z') || 
					(sTmp.GetAt(0) >= 'A' && sTmp.GetAt(0) <= 'Z') || sTmp.GetAt(0) == '/'))
		{
			idx++;
			continue;
		}

		GetTag((LPCTSTR)src + find + 1, tag);
				
		find = src.Find(">", idx);
		if (find < 0) break;
		
		src.Delete(idx, find - idx + 1);
	}
	
	dest = src;
}

void CfxIGrid::GetTag(LPCSTR source, CString &tag)
{
	CString src(source);
	
	int nFind1 = src.Find(' '), nFind2 = src.Find('>');
	
	if ((nFind1 < 0) && (nFind2 < 0))
		return;
	
	if (nFind1 < 0) 
		nFind1 = MAXINT;
	if (nFind2 < 0)
		nFind2 = MAXINT;
	
	tag = src.Left(min(nFind1, nFind2));
	if (tag[0] == '/') 
		tag.Delete(0, 1);
}

LRESULT CfxIGrid::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;

	CPoint curPos;
	CRect gridRc = m_clientRc;
	ClientToScreen(&gridRc);
	GetCursorPos(&curPos);

	if (!gridRc.PtInRect(curPos))
	{
		if (!m_beforeMouseOverRect.IsRectEmpty())
			InvalidateRect(m_beforeMouseOverRect);
		m_mouseOverPnt = CPoint(-1, -1);
	}

	return 0;
}

LRESULT CfxIGrid::OnMouseHover(WPARAM wparam, LPARAM lparam)  //vc2019 void->LRESULT
{
	m_bHover = TRUE;
	return 0;
}

BOOL CfxIGrid::CreateTooltip()
{
	if (m_pTipCtrl)
		return FALSE;
	CRect clientRc;
	GetClientRect(&clientRc);

#ifdef DF_USESTL_FXGRID 
	m_pTipCtrl = std::make_unique<CToolTipCtrl>();
#else
	m_pTipCtrl = new CToolTipCtrl;
#endif
	const BOOL bCreate = m_pTipCtrl->Create(this, TTS_NOPREFIX|TTS_ALWAYSTIP);
	m_pTipCtrl->AddTool(this, "", &clientRc, GetDlgCtrlID());
	m_pTipCtrl->Activate(TRUE);
	m_pTipCtrl->SetDelayTime(TTDT_INITIAL, 0);
	m_pTipCtrl->SetDelayTime(TTDT_AUTOPOP, 100000);
	m_pTipCtrl->SetDelayTime(TTDT_RESHOW, 0);
	return bCreate;
}

LRESULT CfxIGrid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl && IsWindow(m_pTipCtrl->m_hWnd))
			m_pTipCtrl->RelayEvent(&msg);
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CfxIGrid::PreTranslateMessage(MSG* pMsg) 
{
	
	return CWnd::PreTranslateMessage(pMsg);
}

CRect CfxIGrid::GetDrawRect(int nRow, int nCol, CRect cellRc, DWORD dwStyle /*= DT_LEFT*/)
{
	CRect drawRect = cellRc;
	CDC *pDC = GetDC();
	CString sDat = GetData(nRow, nCol);
	CFont *pOldFont = (CFont*)pDC->SelectObject(m_pFont);

	if (sDat.IsEmpty())
	{
		drawRect.right = drawRect.left;
		return drawRect;
	}
	
	cellRc.DeflateRect(1, 0, 1, 0);
	pDC->DrawText(sDat, &drawRect, dwStyle | DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_CALCRECT);
	

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	return drawRect;
}

int CfxIGrid::GetColByPt(CPoint point)
{
	long nColumnLine = -m_StartPnt.x;
	CIColumn qCol;
	CRect cellRc = m_clientRc;
	
	for (int nCol = 0; nCol < m_arColumn.GetSize(); nCol++)
	{
		qCol = m_arColumn.GetAt(nCol);
		if (qCol.m_dAttr & IGAT_HIDDEN)
			continue;
		
		cellRc.left  = nColumnLine;
		nColumnLine += GetColumnWidth(nCol);
		cellRc.right = nColumnLine;
		if (cellRc.PtInRect(point))
			return nCol;
	}
	return -1;
}

void CfxIGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	NMHDR hdr;
	hdr.code = NM_DBLCLK;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();
	const CWnd *pParent = GetParent();
	if (pParent)
		pParent->SendMessage(WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr);

	CWnd::OnLButtonDblClk(nFlags, point);
}

int CfxIGrid::FindValue(int nCol, CString sDat)
{
	if (GetRowCount() == 0)
		return -1;
	const CStringArray *pArData = GetColPtr(nCol);
	if (pArData)
	{
		for (int ii = 0; ii < pArData->GetSize(); ++ii) 
		{
			CString sData = pArData->GetAt(ii);
			if (!sDat.Compare(sData))
				return ii;
		}
	}
	return -1;
}