// fxIGrid.cpp : implementation file
//


#include "pch.h"
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

const int	timerID = 1002;
const int	maxLINE = 9999;
/////////////////////////////////////////////////////////////////////////////
// CfxIGrid

CfxIGrid::CfxIGrid(CWnd* parent)
{
	RegisterWndClass();

	m_parent	  = parent;

	m_StartPnt        = CPoint(0, 0);
	m_lOriHeadHeight     = 20;
	m_lOriDataHeight     = 20;
	m_lSumColumnWidth = 0;
	m_lSumVisColWidth = 0;

	m_lMoveWPos       = 0;
	m_lMoveWColumn    = -1;

	m_nSelectedRow    = -1;

	m_lineColor    = PALETTERGB(240, 240, 240);
	m_headBgColor  = PALETTERGB(128, 128, 255);
	m_headFgColor  = PALETTERGB(255, 255, 255);

	m_dataBgColor  = PALETTERGB(255, 255, 255);
	m_dataBgColor1 = PALETTERGB(255, 255, 255);
	m_dataBgColor2 = PALETTERGB(240, 240, 240);
	m_dataFgColor  = PALETTERGB(0,   0,   0);

	m_upColor      = PALETTERGB(255, 0,   0);
	m_dnColor      = PALETTERGB(0,   0,   255);
	m_eqColor      = PALETTERGB(0,   0,   0);

	m_selFgColor   = GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_selBgColor   = GetSysColor(COLOR_HIGHLIGHT);

	m_b3DHead      = FALSE; // TRUE;
	m_bMemDC       = TRUE;
	m_bFixScroll   = TRUE;
	m_b2RowColor   = FALSE;
	m_bFillEmpty   = FALSE;

	m_bColumnResize  = TRUE;
	m_bFirstRowColor = FALSE;
	m_bSelection     = TRUE;
	m_wDisableBar    = NULL;

	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}

CfxIGrid::~CfxIGrid()
{
	m_pFont->DeleteObject(); delete m_pFont;

	m_aryColumn.RemoveAll();

m_sync.Lock();
	while (m_aryData.GetSize())
	{
		delete m_aryData.GetAt(0);
		m_aryData.RemoveAt(0);
	}
	while (m_aryHold.GetSize())
	{
		delete m_aryHold.GetAt(0);
		m_aryHold.RemoveAt(0);
	}
m_sync.Unlock();
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
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxIGrid message handlers

void CfxIGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect clipRc, bkRect;
	CDC *pMemDC = &dc;
	CFont* pOldFont{};

	dc.GetClipBox(&clipRc);

	if (m_bMemDC)
		pMemDC = new CMemDCx(&dc);

	pMemDC->FillSolidRect(clipRc, RGB(255, 255, 255));

	if ((m_aryData.GetSize() - m_StartPnt.y) * m_lDataHeight + m_lHeadHeight < clipRc.bottom)
	{
		bkRect = clipRc;
		bkRect.top = ((LONG)m_aryData.GetSize() - m_StartPnt.y) * m_lDataHeight + m_lHeadHeight;
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
	for (int ii = 0; ii < m_aryData.GetSize(); ii++)
		if (!DrawRow(pMemDC, ii, clipRc))
			break;

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
}

void CfxIGrid::DrawCellData(CDC *pDC, CRect cellRc, CString sDat, DWORD dwStyle, BOOL bHead /*= FALSE*/, BOOL bSelected /*= FALSE*/)
{
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
			pDC->SetTextColor(m_selFgColor);
		}
		else
		{
			pDC->FillSolidRect(cellRc, m_dataBgColor);
			pDC->SetTextColor(m_dataFgColor);
		}
	}

	const int margineR = 3;
	cellRc.DeflateRect(1, 0, margineR, 0);
	pDC->DrawText(sDat, cellRc, dwStyle | DT_VCENTER | DT_SINGLELINE);
	cellRc.InflateRect(1, 0, margineR, 0);

	cellRc.InflateRect(0, 0, 1, 1);
	pDC->Draw3dRect(cellRc, m_lineColor, m_lineColor);

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
}

void CfxIGrid::DrawHead(CDC *pDC, CRect visibleRc)
{
	CRect cellRc = CRect (0, 0, 0, m_lHeadHeight);
	for (int ii = m_StartPnt.x; ii < m_aryColumn.GetSize(); ii++)
	{
		CIColumn qCol = m_aryColumn.GetAt(ii);
		cellRc.right += qCol.m_lWidth;
		DrawCellData(pDC, cellRc, qCol.m_sHeadCaption, DT_CENTER, TRUE);
		cellRc.left += qCol.m_lWidth;
		if (cellRc.left > visibleRc.right)
			break;
	}
}

BOOL CfxIGrid::DrawRow(CDC *pDC, long nRow, CRect visibleRc, BOOL ShowLOG)
{
	if (nRow + m_StartPnt.y >= m_aryData.GetSize())
		return FALSE;
	if (m_lHeadHeight + m_lDataHeight * nRow > visibleRc.bottom)
		return FALSE;

	m_sync.Lock();  

	CRect cellRc = CRect (0, m_lHeadHeight + (m_lDataHeight * nRow), 0, m_lHeadHeight + (m_lDataHeight * (nRow + 1)));
	CStringArray*	pAryData = m_aryData.GetAt(nRow + m_StartPnt.y);
	if (!pAryData)
	{
		m_sync.Unlock(); return FALSE;
	}

	if (ShowLOG)
	{
		m_slog.Format("nRow=[%d], m_StartPnt.y=[%d], nRow + m_StartPnt.y = [%d], cellRc.top=[%d]   [%s]",
			nRow, m_StartPnt.y, nRow + m_StartPnt.y, cellRc.top, pAryData->GetAt(0));
	//	LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);
	}
	

	const long nSize = (long)pAryData->GetSize();
	CString sData, sTmp;
	//COLORREF tmpFgColor = m_dataFgColor; 
	DWORD	dwStyle = DT_LEFT;

	for (int nCol = m_StartPnt.x; nCol < m_aryColumn.GetSize(); nCol++)
	{
		m_dataFgColor = m_gridFgColor;
		CIColumn qCol = m_aryColumn.GetAt(nCol);
		sData.Empty();
		if (nCol < nSize)
			sData = pAryData->GetAt(nCol);

		if (qCol.m_dAttr & IGAT_CENTER)
			dwStyle = DT_CENTER;
		else if (qCol.m_dAttr & IGAT_RIGHT)
			dwStyle = DT_RIGHT;
		else 
			dwStyle = DT_LEFT;

		if (m_b2RowColor)
			m_dataBgColor = ((nRow + m_StartPnt.y)%2 ^ m_bFirstRowColor ? m_dataBgColor1:m_dataBgColor2);

		if (qCol.m_dAttr & (IGAT_CONDITION|IGAT_CONDITIONx))
		{
			if (!sData.IsEmpty())
			{
				switch (sData.GetAt(0))
				{
				case '+': m_dataFgColor = m_upColor;	break;
				case '-': m_dataFgColor = m_dnColor;	break;
				default:  m_dataFgColor = m_eqColor;	break;
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
					sTmp = ARR_UP; m_dataFgColor = m_upColor;
					break;
				case '+':
				case '2':
					sTmp = TRI_UP; m_dataFgColor = m_upColor;
					break;
				case '5':
					sTmp = TRI_DN; m_dataFgColor = m_dnColor;
					break;
				case '-':
				case '4':
					sTmp = ARR_DN; m_dataFgColor = m_dnColor;
					break;
				default:
					sTmp = _T(""); m_dataFgColor = m_eqColor;
					break;
				}
				sData = sData.Mid(1);
			}
		}
		if ((qCol.m_dAttr & IGAT_EDITFORMAT) && !qCol.m_sEditFormat.IsEmpty())
		{
			m_dataFgColor = m_gridFgColor;
			sData = SetEditMask(sData, qCol.m_sEditFormat);
		}
		sData = getFormatData(sData, qCol.m_dAttr);

		cellRc.right += qCol.m_lWidth;
		DrawCellData(pDC, cellRc, sData, dwStyle, FALSE, m_nSelectedRow == m_StartPnt.y + nRow);

		if (qCol.m_dAttr & IGAT_CONTRAST)
		{
			const CSize	size = pDC->GetOutputTextExtent(sTmp);
			const int	topx = (cellRc.Height() - size.cy) / 2;
			CRect	contrastRC(cellRc.left, cellRc.top + topx, cellRc.left + size.cx, cellRc.top + topx + size.cy);

			contrastRC.OffsetRect(4, 0);
			drawContrast(pDC, contrastRC, sTmp, m_dataFgColor);
		}

		cellRc.left += qCol.m_lWidth;
		if (cellRc.left > visibleRc.right)
			break;
	}
	m_sync.Unlock();  
	return TRUE;
}

void CfxIGrid::DrawEmptyRow(CDC *pDC, CRect visibleRc)
{
	int nRow = 0;
	while (nRow + m_StartPnt.y < m_aryData.GetSize())
	{
		if (m_lHeadHeight + m_lDataHeight * nRow > visibleRc.bottom)
			return;
		nRow++;
	}

	while (m_lHeadHeight + m_lDataHeight * nRow <= visibleRc.bottom)
	{
		CRect cellRc = CRect (0, m_lHeadHeight + m_lDataHeight * nRow, 0, m_lHeadHeight + m_lDataHeight * (nRow + 1));
		
		for (int nCol = m_StartPnt.x; nCol < m_aryColumn.GetSize(); nCol++)
		{
			CIColumn qCol = m_aryColumn.GetAt(nCol);
			cellRc.right += qCol.m_lWidth;
			DrawCellData(pDC, cellRc, " ", DT_LEFT, FALSE, m_nSelectedRow == m_StartPnt.y + nRow);
			cellRc.left += qCol.m_lWidth;
			if (cellRc.left > visibleRc.right)
				break;
		}
		nRow++;
	}
}

void CfxIGrid::DrawAddRow(CDC *pDC, bool bTop)
{
	CRect clientRc;
	GetClientRect(&clientRc);
	clientRc.top = m_lHeadHeight;
	if (bTop && !(m_bFixScroll && m_StartPnt.y))
		ScrollWindow(0, m_lDataHeight, &clientRc);

	if (bTop)
	{
		if (m_b2RowColor)
			m_dataBgColor = m_bFirstRowColor ? m_dataBgColor2 : m_dataBgColor1;

		clientRc.bottom = m_lHeadHeight + m_lDataHeight;
		DrawRow(pDC, 0, clientRc);
	}
	else
	{
		m_slog.Format("size=[%d] m_StartPnt.y=[%d]    [%d] ", m_aryData.GetSize(),m_StartPnt.y, m_aryData.GetSize() - 1 - m_StartPnt.y);
		//LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);
		DrawRow(pDC, (long)m_aryData.GetSize() - 1 - m_StartPnt.y, clientRc, TRUE);
	}
}

void CfxIGrid::SetData(long nRow, long nCol, CString sDat)
{
	if (nCol >= m_aryColumn.GetSize())
		return;

	CStringArray* pAryData = nullptr;
	if (nRow >= m_aryData.GetSize())
	{
		for (int ii = m_aryData.GetSize(); ii <= nRow; ii++)
		{
			pAryData = new CStringArray;
			for (int jj = 0; jj < m_aryColumn.GetSize(); jj++)
				pAryData->Add(_T(""));
			m_aryData.Add(pAryData);
		}

		adjustScroll(SB_VERT);
	}

	pAryData = m_aryData.GetAt(nRow);
	while (pAryData->GetSize() <= nCol)
		pAryData->Add(_T(""));

	pAryData->SetAt(nCol, sDat);
}

CString CfxIGrid::GetData(long nRow, long nCol)
{
	if (nRow >= m_aryData.GetSize() || nCol >= m_aryColumn.GetSize())
		return _T("");

	CStringArray* pAryData = m_aryData.GetAt(nRow);
	if (pAryData->GetSize() <= nCol)
		return _T("");

	return pAryData->GetAt(nCol);
}

void CfxIGrid::AddRow(CString sDat, bool bTop)
{
	CStringArray* pAryData = new CStringArray;
	CDC*	pDC = GetDC();
	CFont*	pOldFont = pDC->SelectObject(m_pFont);

	while (!sDat.IsEmpty())
		pAryData->Add(Parser(sDat, "\t"));

m_sync.Lock();
	if (bTop)
	{
		m_bFirstRowColor = !m_bFirstRowColor;
		if (m_nSelectedRow >= 0)
			m_nSelectedRow++;
		m_aryData.InsertAt(0, pAryData);
	}
	else
	{
		m_aryData.Add(pAryData);
	}

	while (m_aryData.GetSize() > maxLINE)
	{
		delete m_aryData.GetAt(maxLINE);
		m_aryData.RemoveAt(maxLINE);
	}
m_sync.Unlock();
	
	adjustScroll(SB_VERT);

	if (bTop && m_bFixScroll && m_StartPnt.y)
	{
		m_StartPnt.y++;
		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT) + 1);
	}

	DrawAddRow(pDC, bTop);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

void CfxIGrid::AddRowHold(CString sDat)
{
	CStringArray* pAryData = new CStringArray;

	while (!sDat.IsEmpty())
		pAryData->Add(Parser(sDat, "\t"));

m_sync.Lock();

	m_aryHold.Add(pAryData);
	while (m_aryData.GetSize() > maxLINE)
	{
		delete m_aryData.GetAt(maxLINE);
		m_aryData.RemoveAt(maxLINE);
	}

	if (m_aryHold.GetSize() < m_holdCOUNT)
	{
m_sync.Unlock();
		return;
	}

	while (m_aryHold.GetSize())
	{
		m_aryData.InsertAt(0, m_aryHold.GetAt(0));
		m_aryHold.RemoveAt(0);
	}
	m_aryHold.RemoveAll();

m_sync.Unlock();

	if (m_nSelectedRow >= 0)
		m_nSelectedRow += m_holdCOUNT;

	adjustScroll(SB_VERT);

	if (m_bFixScroll && m_StartPnt.y)
	{
		m_StartPnt.y += m_holdCOUNT;
		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT) + m_holdCOUNT);
	}

	Invalidate();
}

void CfxIGrid::DeleteRow(long nRow)
{
	if (nRow >= m_aryData.GetSize())
		return;

	m_aryData.GetAt(nRow)->RemoveAll();
	delete m_aryData.GetAt(nRow);
	m_aryData.RemoveAt(nRow);
	adjustScroll(SB_VERT);

	if (nRow >= m_StartPnt.y)
	{
		CRect clientRc; GetClientRect(&clientRc);
		if (m_lHeadHeight + m_lDataHeight * (nRow - m_StartPnt.y) < clientRc.Height())
		{
			clientRc.top = m_lHeadHeight + m_lDataHeight * (nRow - m_StartPnt.y);
			InvalidateRect(clientRc);
		}
	}
}

void CfxIGrid::ResetContent()
{
m_sync.Lock();
	while (m_aryData.GetSize())
	{
		m_aryData.GetAt(0)->RemoveAll();
		delete m_aryData.GetAt(0);
		m_aryData.RemoveAt(0);
	}
m_sync.Unlock();

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

void CfxIGrid::AddQColumn(CIColumn qCol)
{
	m_lSumColumnWidth += qCol.m_lWidth;
	m_lSumVisColWidth += qCol.m_lWidth;
	m_aryColumn.Add(qCol);
	CalCulRatio();
	adjustScroll();
	CRect rec;
	GetClientRect(rec);
}

CString CfxIGrid::CommaModify(CString string)
{
	CString stripData = string;
	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return stripData;

	if (stripData.Find('.') != -1)
		return stripData;

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);
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
			info.nMin = 0; info.nMax = GetLastColCnt();
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
				ShowScrollBar(SB_HORZ, FALSE);
		}
	}

	if (wBar == SB_VERT || wBar == SB_BOTH)
	{
		if (m_lHeadHeight + m_lDataHeight * m_aryData.GetSize() > clientRc.Height())
		{
			SCROLLINFO info;
			info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
			GetScrollInfo(SB_VERT, &info);
			info.fMask |= (m_wDisableBar & SB_VERT?SIF_DISABLENOSCROLL:NULL); 
			info.cbSize = sizeof(SCROLLINFO);
			info.nMin = 0; info.nMax = (int)m_aryData.GetSize() - (clientRc.Height() - m_lHeadHeight) / m_lDataHeight;
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
				ShowScrollBar(SB_VERT, FALSE);
		}
	}
}

void CfxIGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int	scrollPos{}, scrollMax{};
	CRect clientRc;

	GetClientRect(&clientRc);
	clientRc.top = m_lHeadHeight;
	scrollPos = GetScrollPos32(SB_VERT);
	scrollMax = GetScrollMax32(SB_VERT);

	m_slog.Format("[cx_grid][vscroll] nSBCode=[%d] nPos=[%d] scrollPos=[%d] scrollMax=[%d]", nSBCode, nPos, scrollPos, scrollMax);
	OutputDebugString(m_slog);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos + 1 <= scrollMax)
		{
			scrollPos = min(scrollPos + 1, scrollMax);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			clientRc.top += m_lDataHeight;
			ScrollWindow(0, -m_lDataHeight, &clientRc);
			clientRc.top = clientRc.bottom - m_lDataHeight;
			InvalidateRect(&clientRc);
		}

		// SB_BOTTOM
		if (scrollPos == scrollMax && m_parent)
			m_parent->SendMessage(WM_FROMGRID, SB_BOTTOM, (LPARAM)scrollPos);

		break;
	case SB_LINEUP:
		if (scrollPos - 1 >= 0)
		{
			scrollPos = max(scrollPos - 1, 0);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			clientRc.bottom -= m_lDataHeight;
			ScrollWindow(0, m_lDataHeight, &clientRc);
			clientRc.bottom = clientRc.top + m_lDataHeight;
			InvalidateRect(&clientRc);
		}
		else if(scrollPos == 0)
			m_parent->SendMessage(WM_FROMGRID, SB_TOP, (LPARAM)scrollPos);
		break;
	case SB_PAGEDOWN:
		if (scrollPos < scrollMax)
		{
			scrollPos += clientRc.Height() / m_lDataHeight;
			scrollPos = min(scrollPos, scrollMax);
			SetScrollPos32(SB_VERT, scrollPos);
			m_StartPnt.y = scrollPos;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEUP:
		if (scrollPos > 0)
		{
			scrollPos -= clientRc.Height() / m_lDataHeight;
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
				clientRc.top += m_lDataHeight * nDelta;
				ScrollWindow(0, (-1 * m_lDataHeight) * nDelta, &clientRc);
				clientRc.top = clientRc.bottom - m_lDataHeight * nDelta;
				
			}
			else if (nDelta < 0 && -nDelta < clientRc.Height() / m_lDataHeight)
			{
				clientRc.bottom += m_lDataHeight * nDelta;
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
	int	scrollPos, scrollMax;
	CRect clientRc;
	
	GetClientRect(&clientRc);
	scrollPos = GetScrollPos32(SB_HORZ);
	scrollMax = GetScrollMax32(SB_HORZ);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos + 1 <= scrollMax)
		{
			const long nDelta = GetColumnWidth(m_StartPnt.x);
			SetScrollPos32(SB_HORZ, scrollPos + 1);
			m_StartPnt.x++;
			m_lSumVisColWidth -= nDelta;
			clientRc.left = nDelta;
			ScrollWindow(-clientRc.left, 0, &clientRc);
			clientRc.left = clientRc.right - nDelta;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_LINEUP:
		if (scrollPos - 1 >= 0)
		{
			const long nDelta = GetColumnWidth(m_StartPnt.x - 1);
			SetScrollPos32(SB_HORZ, scrollPos - 1);
			m_StartPnt.x--;
			m_lSumVisColWidth += nDelta;
			clientRc.right -= nDelta;
			ScrollWindow(nDelta, 0, &clientRc);
			clientRc.right = clientRc.left + nDelta;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEDOWN:
		if (scrollPos < scrollMax)
		{
			long nDelta = 0;
			int ii = 0;
			for (ii = m_StartPnt.x; ii <= scrollMax&& nDelta < clientRc.Width(); ii++)
				nDelta += GetColumnWidth(ii);
			

			scrollPos = ii - 1;
			scrollPos = min(scrollPos, scrollMax);
			for (ii = m_StartPnt.x; ii < scrollPos; ii++)
				m_lSumVisColWidth -= GetColumnWidth(ii);
			SetScrollPos32(SB_HORZ, scrollPos);
			m_StartPnt.x = scrollPos;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_PAGEUP:
		if (scrollPos > 0)
		{
			long nDelta = 0;
			int ii = 0;
			for (ii = scrollPos - 1; ii >= 0 && nDelta < clientRc.Width(); ii--)
				nDelta += GetColumnWidth(ii);

			scrollPos = (ii > 0 ? ii + 2 : 0);
			scrollPos = max(scrollPos, 0);

			for (ii = (ii > 0 ? ii + 2 : 0); ii < scrollPos; ii++)
				m_lSumVisColWidth += GetColumnWidth(ii);

			SetScrollPos32(SB_HORZ, scrollPos);
			m_StartPnt.x = scrollPos;
			InvalidateRect(&clientRc);
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		if (nPos >= 0 && (long)nPos <= scrollMax)
		{
			m_StartPnt.x = nPos;
			m_lSumVisColWidth = 0;
			for (int ii = nPos; ii < scrollMax; ii++)
				m_lSumVisColWidth += GetColumnWidth(ii);
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
	if (m_aryColumn.GetSize() <= nCol)
		return 0;
	return m_aryColumn.GetAt(nCol).m_lWidth;
}

int CfxIGrid::GetLastColCnt()
{
	CRect clientRc;
	GetClientRect(&clientRc);
	int nCol = 0;
	for (nCol = (int)m_aryColumn.GetSize() - 1; nCol >= 0 && clientRc.right >= 0; nCol--)
		clientRc.right -= GetColumnWidth(nCol);
	return nCol + 2;
}

void CfxIGrid::drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr)
{
	if (sText.IsEmpty())
		return;

	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height();
	CSize	size;
	CPoint	pts[7];
	CPen	pen, * pOldPen;
	CBrush	brush, * pOldBrush{};

	pen.CreatePen(PS_SOLID, 1, clr);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	brush.CreateSolidBrush(clr); // bkcolor
	pOldBrush = (CBrush*)pDC->SelectObject(&brush);

	pRc.top = (cellRc.top + cellRc.Height() / 2) - (int)(hh / 2);
	pRc.bottom = (int)(pRc.top + hh);

	size = pDC->GetOutputTextExtent(ARR_UP);
	if (size.cx > 10)
		pRc.left = (int)(pRc.right - (size.cx * 0.8));
	else
		pRc.left = pRc.right - size.cx;

	hh = (float)(int)(pRc.Height() + 1) / 2;
	hh = (float)(int)((hh + 1) / 2) * 2 - 1;
	int ww = pRc.Width();
	pRc.right = pRc.left + 8;
	ww = pRc.Width() / 2 - 1;

	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		if (!sText.Compare(ARR_UP))
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;

			pts[0].x = pRc.left + (pRc.Width() / 2);
			pts[1].x = pRc.left + 1;
			pts[6].x = pRc.right - 1;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(ARR_DN))
		{
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;

			pts[0].x = pRc.left + (pRc.Width() / 2);
			pts[1].x = pRc.right - 1;
			pts[6].x = pRc.left + 1;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
		}
		else if (!sText.Compare(TRI_UP))
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;

			pts[0].x = pRc.left + (pRc.Width() / 2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
		}
		else if (!sText.Compare(TRI_DN))
		{
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;

			pts[0].x = pRc.left + (pRc.Width() / 2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
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
	if (dAttr & IGAT_COMMA)
		sRtn = CommaModify(sRtn);
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
	if (m_bColumnResize)
	{
		if (nFlags == MK_LBUTTON && m_lMoveWColumn >= 0)
		{
			m_lMoveWPos = point.x;
			Invalidate();
		}
		else if (nFlags != MK_LBUTTON && m_lMoveWColumn < 0)
		{
			CRect clientRc;
			long nColumnLine = 0;
			
			GetClientRect(&clientRc);
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			for (int nCol = m_StartPnt.x; nCol < m_aryColumn.GetSize(); nCol++)
			{
				nColumnLine += GetColumnWidth(nCol);
				clientRc.left  = nColumnLine - 2;
				clientRc.right = nColumnLine + 2;
				if (clientRc.PtInRect(point))
					m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
			}
		}
		else
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CfxIGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ShowGridInfo();
	if (m_bColumnResize)
	{
		CRect clientRc;
		long nColumnLine = 0;
		
		GetClientRect(&clientRc);
		
		for (int nCol = m_StartPnt.x; nCol < m_aryColumn.GetSize(); nCol++)
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

	if (m_bSelection && m_lMoveWColumn < 0 && point.y > m_lHeadHeight)
	{
		const long beforeSelectedRow = m_nSelectedRow;
		CRect clientRc;
		GetClientRect(&clientRc);

		clientRc.top = m_lHeadHeight;
		clientRc.bottom = m_lHeadHeight + m_lDataHeight;

		for (m_nSelectedRow = 0; !clientRc.PtInRect(point); clientRc.OffsetRect(0, m_lDataHeight))
			m_nSelectedRow++;

		m_nSelectedRow += m_StartPnt.y;
		InvalidateRect(clientRc);
		if (beforeSelectedRow - m_StartPnt.y >= 0)
		{
			clientRc.OffsetRect(0, m_lDataHeight * (beforeSelectedRow - m_nSelectedRow));
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
			CIColumn qCol = m_aryColumn.GetAt(m_lMoveWColumn);
			for (int nCol = m_StartPnt.x; nCol < m_lMoveWColumn; nCol++)
				m_lMoveWPos -= GetColumnWidth(nCol);

			m_lSumColumnWidth -= qCol.m_lWidth;
			m_lSumVisColWidth -= qCol.m_lWidth;
			
			if (m_lMoveWPos < 5)
				m_lMoveWPos = 5;
			qCol.m_lWidth = m_lMoveWPos;

			m_lSumColumnWidth += qCol.m_lWidth;
			m_lSumVisColWidth += qCol.m_lWidth;
			
			m_aryColumn.SetAt(m_lMoveWColumn, qCol);
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
	return (long)m_aryData.GetSize();
}


void CfxIGrid::ClearColumn()
{
	m_aryColumn.RemoveAll();
	m_lSumColumnWidth = 0;
	m_lSumVisColWidth = 0;
}

void CfxIGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	m_dHeightRatio =(double)cy/m_iOriHeight ;
	m_lHeadHeight =((double)m_lOriHeadHeight *m_dHeightRatio);
	m_lDataHeight =((double)m_lOriDataHeight * m_dHeightRatio);

	m_slog.Format("\r\n[cx_grid] onsize  m_lHeadHeight =[%f] m_dHeightRatio=[%f]", m_lHeadHeight, m_dHeightRatio);
	OutputDebugString(m_slog);

	ReSizeGrid();
	adjustScroll(SB_BOTH);	
}

int CfxIGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (lpCreateStruct->style & WS_VSCROLL)
		m_wDisableBar = SB_VERT;
	if (lpCreateStruct->style & WS_HSCROLL)
		m_wDisableBar |= SB_HORZ;

	SetTimer(timerID, m_elapse, NULL);

	CRect rec;
	GetClientRect(&rec);
	m_iOriHeight = rec.Height();
	m_dHeightRatio = 1;
	return 0;
}

void CfxIGrid::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == timerID)
	{
		const int count = m_aryHold.GetSize();

m_sync.Lock();
		while (m_aryHold.GetSize())
		{
			m_aryData.InsertAt(0, m_aryHold.GetAt(0));
			m_aryHold.RemoveAt(0);
		}
m_sync.Unlock();

		//if (count > 0)
		//{
		//	if (m_nSelectedRow >= 0)
		//		m_nSelectedRow += count;

		//	adjustScroll(SB_VERT);
		//	if (m_bFixScroll && m_StartPnt.y)
		//	{
		//		m_StartPnt.y += count;
		//		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT) + count);
		//	}

		//	Invalidate();
		//}
	}

	CWnd::OnTimer(nIDEvent);
}

void CfxIGrid::ReSizeGrid()
{
	//int iwidthScrollthumb = GetSystemMetrics(SM_CXVSCROLL);
	//iwidthScrollthumb = 0;
	CRect rec;
	GetClientRect(&rec);
//	int iWidth = rec.Width() - iwidthScrollthumb;
	const int iWidth = rec.Width();
	
	double dCulRatio{};
	for (int ii = 0; ii < m_aryColumn.GetSize(); ii++)
	{
		dCulRatio = m_arrColumnRatio.GetAt(ii);
		CIColumn qCol = m_aryColumn.GetAt(ii);
		qCol.m_lWidth = (int)(iWidth * dCulRatio);
		m_aryColumn.SetAt(ii, qCol);
	}

	adjustScroll(SB_BOTH);
	Invalidate();
}

void CfxIGrid::CalCulRatio()
{
	const int iwidthScrollthumb = GetSystemMetrics(SM_CXVSCROLL);
	CRect rec;
	GetClientRect(&rec);
	double dData{};
	m_arrColumnRatio.RemoveAll();

	for (int ii = 0; ii < m_aryColumn.GetSize(); ii++)
	{
		CIColumn qCol = m_aryColumn.GetAt(ii);
		dData = (double)qCol.m_lWidth / (double)(rec.Width() - iwidthScrollthumb);
		m_arrColumnRatio.Add(dData);
	}
}

void CfxIGrid::ShowGridInfo()
{
	CRect rec;
	GetClientRect(&rec);
	m_slog.Format("Client width = [%d]", rec.Width());
	//LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);

	for (int ii = 0; ii < m_arrColumnRatio.GetSize(); ii++)
	{
		m_slog.Format("%.2f", m_arrColumnRatio.GetAt(ii));
//		LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);
	}
}

void CfxIGrid::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_slog.Format("m_StartPnt.x=[%d] m_StartPnt.y=[%d] GetScrollPos32 V=[%d] H=[%d]", m_StartPnt.x, m_StartPnt.y, GetScrollPos32(SB_VERT), GetScrollPos32(SB_HORZ));
	LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);

	m_slog.Format("getrowcount =[%d", GetRowCount());
	LOG_OUTP(3, "cx_grid", __FUNCTION__, m_slog);
	CString sdata;
	sdata.Format("%s\t%s\t%s\t%s\t%s", "09481300", "+20000", "1919", "22222", "33333");
	//AddRow(sdata, FALSE);
	//09481300	-66800	5300	+100	1996067
	sdata.Format("%s\t%s\t%s\t%s\t%s", "+11:11:11", "+20000", "2919", "+22222", "+33333");
	//AddRow(sdata, FALSE);
	SetScrollPos32(SB_VERT, 0);
	CWnd::OnRButtonUp(nFlags, point);
}
