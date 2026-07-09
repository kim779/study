// SingleProperty.cpp : implementation file
//

#include "stdafx.h"
#include "../axiswork.h"
#include "../mainvar.h"
#include "SingleProperty.h"
#include "colorDlg.h"
#include "MGColumnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	lastRows	m_GridAttr.rows - m_StartPnt.y
#define	allCols		m_writeInfo.GetSize()

/////////////////////////////////////////////////////////////////////////////
// CSingleProperty

CSingleProperty::CSingleProperty()
{
	m_pEdit       = NULL;
	m_pCombo      = NULL;
	m_pButton     = NULL;
	m_pBitmap     = NULL;
	m_bInvalidate = true;
	m_bVScroll    = false;
	m_bMove       = false;
	m_StartPnt    = CPoint(0, 0);
	m_SelectedPnt = CPoint(-1, -1);
	m_InputPnt    = CPoint(0, 0);
	m_sData       = "";

	m_writeInfo.RemoveAll();

	m_GridAttr.headInfo  = 0;
	m_GridAttr.CellSz    = CSize(100, 16);
	m_GridAttr.HeadSz    = CSize(130, 16);
	m_GridAttr.lineColor = RGB(190, 190, 190);
	m_GridAttr.multiCol  = false;
	m_GridAttr.cols      = 0;
	m_GridAttr.rows      = 0;

	//m_font.CreatePointFont(90, "굴림체");
	LOGFONT lf = setFont(9);
	m_font.CreatePointFontIndirect(&lf);

	m_ctlBkColor  = ::GetSysColor(COLOR_WINDOW);
	m_ctlBrush.DeleteObject();
	m_ctlBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

	m_proMap.RemoveAll();
}

CSingleProperty::~CSingleProperty()
{
	if (m_pBitmap)
		delete m_pBitmap;
	if (m_pEdit)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
	}
	if (m_pCombo)
	{
		m_pCombo->DestroyWindow();
		delete m_pCombo;
	}
	if (m_pButton)
	{
		m_pButton->DestroyWindow();
		delete m_pButton;
	}
	
	POSITION pos = m_proMap.GetStartPosition();
	while(pos)
	{
		int key;
		CProArr *value;

		m_proMap.GetNextAssoc(pos, key, value);
		delete value;
	}

	m_proMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CSingleProperty, CWnd)
	//{{AFX_MSG_MAP(CSingleProperty)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(ID_EDITCTL, OnChange)
	ON_CBN_SELENDOK(ID_COMBOCTL, OnSelEndOk)
	ON_BN_CLICKED(ID_BUTTONCTL, OnBtnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSingleProperty message handlers

void CSingleProperty::DrawGridData(CDC *pDC)
{
	long col = 0, row = 0, cx = m_GridAttr.HeadSz.cx;
	CRect rt = m_ClientRt;

	for (row = 0; row < lastRows; row++)
	{
		for (col = 0; col < allCols; col++)
			if (!DrawCellData(pDC, col, row, getData(col + m_StartPnt.x, row + m_StartPnt.y)))
				break;
	}
}

void CSingleProperty::DrawHead(CDC *pDC)
{
	int ii = 0;
	if (m_GridAttr.multiCol)
	{
		for (int col = 0; col <= allCols; col++)
		{
			CRect cellRt;
			CString data;
			if (col == 0)
			{
				data="*";
				cellRt.SetRect(0, 0, m_GridAttr.HeadSz.cx, m_GridAttr.CellSz.cy);
			}
			else
			{
				data.Format("%d", col + m_StartPnt.x);
				cellRt.SetRect(m_GridAttr.HeadSz.cx + (col - 1) * m_GridAttr.CellSz.cx, 0, m_GridAttr.HeadSz.cx + col * m_GridAttr.CellSz.cx, m_GridAttr.CellSz.cy);
			}

			if (m_SelectedPnt.y == -1 && col > 0 && m_SelectedPnt.x == col + m_StartPnt.x - 1)
				DrawCellData(pDC, cellRt, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT), DT_CENTER);
			else
				DrawCellData(pDC, cellRt, data, RGB(255, 255, 255), RGB(0,0,0), DT_CENTER);
		}
	}
	for (int row = (m_GridAttr.multiCol?1:0); row < lastRows + (m_GridAttr.multiCol?1:0); row++)
	{
		CRect cellRt;
		CString data;
		cellRt.SetRect(0, row * m_GridAttr.CellSz.cy,
			m_GridAttr.HeadSz.cx, (row + 1) * m_GridAttr.CellSz.cy);
		data = getHeadCaption(ii++ + m_StartPnt.y);
		if (!data.IsEmpty())
		{
			if (m_SelectedPnt.y + (m_GridAttr.multiCol?1:0) == row + m_StartPnt.y)
				DrawCellData(pDC, cellRt, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT));
			else
				DrawCellData(pDC, cellRt, data);
		}
	}
}

bool CSingleProperty::DrawCellData(CDC *pDC, int col, int row, CString data) // col, row 상대위치
{
	CRect cellRt = m_ClientRt;
	COLORREF bkColor;

	if (m_GridAttr.multiCol)
		cellRt.SetRect(m_GridAttr.HeadSz.cx + col * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + row * m_GridAttr.CellSz.cy,
			m_GridAttr.HeadSz.cx + (col + 1) * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + (row + 1) * m_GridAttr.CellSz.cy);
	else
		cellRt.SetRect(m_GridAttr.HeadSz.cx, row * m_GridAttr.CellSz.cy,
			cellRt.right, (row + 1) * m_GridAttr.CellSz.cy);

	if (cellRt.top > m_ClientRt.Height())
		return false;

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	if (getValue1(row + m_StartPnt.y).GetLength() > 4 && atoi(getValue1(row + m_StartPnt.y).Mid(4)) == 1002)
	{
		bkColor = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, 
						atoi(getData(col + m_StartPnt.x, row + m_StartPnt.y)));
		pDC->FillSolidRect(cellRt, bkColor);
	}
	else
		pDC->FillSolidRect(cellRt, RGB(255, 255, 255));
	cellRt.DeflateRect(1, 1, 0, 0);
	if (getType(row + m_StartPnt.y) == 4)
	{
		pDC->DrawFrameControl(cellRt, DFC_BUTTON, DFCS_BUTTONPUSH);
		pDC->DrawText("...", cellRt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (getValue1(row + m_StartPnt.y).GetLength() > 4 && atoi(getValue1(row + m_StartPnt.y).Mid(4)) == 1002)
		{
			if ((GetRValue(bkColor) + GetGValue(bkColor) + GetBValue(bkColor)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
		}
		pDC->DrawText(data, cellRt, DT_LEFT);
	}
	cellRt.InflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(cellRt, m_GridAttr.lineColor, m_GridAttr.lineColor);
	return true;
}

void CSingleProperty::DrawCellData(CDC *pDC, CRect rc, CString data, COLORREF bkcolor, COLORREF frcolor, DWORD dwStyle)
{
	pDC->SetTextColor(frcolor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rc, bkcolor);
	rc.DeflateRect(1, 0, 0, 0);
	pDC->DrawText(data, rc, dwStyle | DT_VCENTER | DT_SINGLELINE);
	rc.InflateRect(1, 0, 1, 1);
	pDC->Draw3dRect(rc, m_GridAttr.lineColor, m_GridAttr.lineColor);
}

void CSingleProperty::DrawBitmap(CDC *pDC)
{
	CDC	memDC;
	CBitmap*	pOldBitmap;

	calScrollSz();

	if (m_pBitmap)
		delete m_pBitmap;

	m_pBitmap = new CBitmap();

	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_ClientRt.Width(), m_ClientRt.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	CFont	font, *pOldFont;
	// set font
	LOGFONT lf = setFont(9);
	font.CreatePointFontIndirect(&lf, &memDC);
	pOldFont = (CFont*)memDC.SelectObject(&font);

	memDC.FillSolidRect(m_ClientRt, RGB(255, 255, 255));
	DrawHead(&memDC);
	DrawGridData(&memDC);

	memDC.SelectObject(pOldFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	m_bInvalidate = false;
}

void CSingleProperty::DrawBitmap2(int col, int row, CRect rc)
{
	CDC *pDC = GetDC(), memDC;
	CBitmap*	pOldBitmap;

	memDC.CreateCompatibleDC(pDC);
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	CFont	font, *pOldFont;
	// set font
	LOGFONT lf = setFont(9);
	font.CreatePointFontIndirect(&lf, &memDC);
	pOldFont = (CFont*)memDC.SelectObject(&font);

	if (col >= 0 && row >= 0)
		DrawCellData(&memDC, col, row, getData(col + m_StartPnt.x, row + m_StartPnt.y));
	else
	{
		CString data;
		if (m_GridAttr.multiCol && col >= 0)
		{
			data.Format("%d", col + m_StartPnt.x + 1);
			
			if (m_SelectedPnt.y == -1 && m_SelectedPnt.x == col + m_StartPnt.x)
				DrawCellData(&memDC, rc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT), DT_CENTER);
			else
				DrawCellData(&memDC, rc, data, RGB(255, 255, 255), RGB(0,0,0), DT_CENTER);
		}
		
		if (row >= 0)
		{
			data = getHeadCaption(row + m_StartPnt.y);
			if (!data.IsEmpty())
			{
				if (m_SelectedPnt.y == row + m_StartPnt.y)
					DrawCellData(&memDC, rc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT));
				else
					DrawCellData(&memDC, rc, data);
			}
		}
	}

	memDC.SelectObject(pOldFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

//
// Window Messages
//

void CSingleProperty::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rt = m_ClientRt;

	if (m_bInvalidate)
		DrawBitmap(&dc);
	
	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap;
		
		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		
		dc.BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	if (m_bMove)
	{
		rt.left = m_GridAttr.HeadSz.cx;
		rt.right = rt.left + 1;
		dc.FillSolidRect(rt, RGB(0, 0, 0));
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void CSingleProperty::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(&m_ClientRt);
	
	EraseCtrls();
	m_bInvalidate = true;
	Invalidate();
}

void CSingleProperty::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	CPoint oldSelected = m_SelectedPnt;

	EraseCtrls();
	CRect validRt;

	GetClientRect(&validRt);
	validRt.left = m_GridAttr.HeadSz.cx - 1;
	validRt.right = m_GridAttr.HeadSz.cx + 1;
	if (validRt.PtInRect(point))
	{
		SetCapture();
		m_bMove = true;
		Invalidate();
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	if (m_GridAttr.multiCol)
		validRt.SetRect(
			0, 
			0, 
			m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx * allCols,
			m_GridAttr.HeadSz.cy + m_GridAttr.CellSz.cy * lastRows);
	else
		validRt.SetRect(
			0, 
			0, 
			m_ClientRt.Width(),
			m_GridAttr.CellSz.cy * lastRows);
	if (validRt.PtInRect(point))
	{
		if (point.x < m_GridAttr.HeadSz.cx)
			m_SelectedPnt.x = -1;
		else 
		{
			m_SelectedPnt.x = 0;
			if (m_GridAttr.multiCol)
				while((m_SelectedPnt.x + 1) * m_GridAttr.CellSz.cx + m_GridAttr.HeadSz.cx < point.x)
					m_SelectedPnt.x++;
			m_SelectedPnt.x += m_StartPnt.x;
		}
		
		if (m_GridAttr.multiCol && point.y < m_GridAttr.HeadSz.cy)
			m_SelectedPnt.y = -1;
		else 
		{
			m_SelectedPnt.y = 0;
			if (m_GridAttr.multiCol)
				while((m_SelectedPnt.y + 1) * m_GridAttr.CellSz.cy + m_GridAttr.HeadSz.cy < point.y)
					m_SelectedPnt.y++;
			else
				while((m_SelectedPnt.y + 1) * m_GridAttr.CellSz.cy < point.y)
					m_SelectedPnt.y++;
			m_SelectedPnt.y += m_StartPnt.y;
		}
		if (m_GridAttr.multiCol)
		{
			InvalidateRect2(oldSelected);
			InvalidateRect2(CPoint(m_SelectedPnt.x, -1));
		}

		InvalidateRect2(CPoint(-1, oldSelected.y));
		InvalidateRect2(CPoint(-1, m_SelectedPnt.y));
		InvalidateRect2(m_SelectedPnt);
		if (m_SelectedPnt.x >= 0)
			SetInputState();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CSingleProperty::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bMove)
	{
		m_GridAttr.HeadSz.cx = point.x;
		Invalidate();
		CWnd::OnMouseMove(nFlags, point);
		return;
	}
	CRect rc;
	m_nCursor = 0;
	if (m_SelectedPnt.y >= 0)
	{
		rc.top = (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + (m_GridAttr.multiCol?m_GridAttr.HeadSz.cy:0);
		rc.bottom = (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy + (m_GridAttr.multiCol?m_GridAttr.HeadSz.cy:0);
		if (m_GridAttr.multiCol)
		{
			rc.left = m_GridAttr.HeadSz.cx;
			rc.right = m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx * (allCols - m_StartPnt.x);
		}
		else
		{
			rc.left = m_GridAttr.HeadSz.cx;
			rc.right = m_ClientRt.right;
		}
		if (rc.PtInRect(point))
		{
			if (getType(m_SelectedPnt.y) == 1)
				m_nCursor = 1;
		}
	}
	GetClientRect(&rc);
	rc.left = m_GridAttr.HeadSz.cx - 1;
	rc.right = m_GridAttr.HeadSz.cx + 1;
	if (rc.PtInRect(point))
		m_nCursor = 2;

	CWnd::OnMouseMove(nFlags, point);
}

void CSingleProperty::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bMove)
	{
		ReleaseCapture();
		m_bInvalidate = true;
		Invalidate();
		m_bMove = false;
	}
	else
	{
		if (m_SelectedPnt.x == -1 && m_SelectedPnt.y >= 0 && !m_GridAttr.multiCol)
		{
			EraseCtrls();
			m_SelectedPnt.x = 0;
			m_bKeyDown = true;
			SetInputState();
		}
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CSingleProperty::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO info;
	/*m_pVScroll->*/GetScrollInfo(SB_VERT, &info);

	if (nSBCode != SB_THUMBTRACK)
		nPos = info.nPos;

	switch (nSBCode)
	{
	case SB_LINEUP:
		if (nPos == 0)
			nPos = 0;
		else
			nPos--;
		break;
	case	SB_LINEDOWN:		// move 1
		nPos++;
		break;
	case	SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;
	case	SB_PAGEDOWN:		// move nPage
		nPos += 2;
		break;
	case	SB_THUMBTRACK:
		break;
	default:
		return;
		break;		
	}
	if (nPos > (UINT)info.nMax)
		nPos = info.nMax;
	info.nPos = nPos;
	info.fMask = SIF_POS;
	/*m_pVScroll->*/SetScrollInfo(SB_VERT, &info);
	
	m_StartPnt.y = nPos;
	EraseCtrls();
	m_bInvalidate = true;
	Invalidate();
	
	//CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSingleProperty::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO info;
	GetScrollInfo(SB_HORZ, &info);

	if (nSBCode != SB_THUMBTRACK)
		nPos = info.nPos;

	switch (nSBCode)
	{
	case SB_LINEUP:
		if (nPos == 0)
			nPos = 0;
		else
			nPos--;
		break;
	case	SB_LINEDOWN:		// move 1
		nPos++;
		break;
	case	SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;
	case	SB_PAGEDOWN:		// move nPage
		nPos += 2;
		break;
	case	SB_THUMBTRACK:
		break;
	default:
		return;
		break;		
	}
	if (nPos > (UINT)info.nMax)
		nPos = info.nMax;
	info.nPos = nPos;
	info.fMask = SIF_POS;
	SetScrollInfo(SB_HORZ, &info);
	
	m_StartPnt.x = nPos;

	EraseCtrls();
	m_bInvalidate = true;
	Invalidate();
	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

//
//	Grid Info
//
CString CSingleProperty::getHeadCaption(int pos)
{
	if (pos < 0)
		return "";
	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).headCaption;
	}
	return "";
}

int CSingleProperty::getType(int pos)
{
	if (pos < 0)
		return 0;

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).type;
	}
	return 0;
}

CString CSingleProperty::getValue1(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).value1;
	}
	return "";
}

CString CSingleProperty::getValue2(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).value2;
	}
	return "";
}

CString CSingleProperty::getFormM(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).formM;
	}
	return "";
}

CString CSingleProperty::getDesc(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).desc;
	}
	return "";
}

int CSingleProperty::getVisibleRows()
{
	int nRows = 0;
	while(m_GridAttr.CellSz.cy * nRows < m_ClientRt.Height() - (m_GridAttr.multiCol?m_GridAttr.HeadSz.cy:0))
		nRows++;
	return nRows;
}

int CSingleProperty::getVisibleCols()
{
	int nCols = 0, width = getGridWidth(), nCnt = 0;
	while(nCnt < allCols && width > m_ClientRt.Width() - m_GridAttr.HeadSz.cx)
	{
		nCnt++;
		width -= m_GridAttr.CellSz.cx;
		nCols++;
	}

	nCols -= 2;
	return nCols;
}

long CSingleProperty::getGridHeight()
{
	if (m_GridAttr.multiCol)
		return m_GridAttr.HeadSz.cy + m_GridAttr.CellSz.cy * m_GridAttr.rows;
	else
		return m_GridAttr.CellSz.cy * m_GridAttr.rows;
}

long CSingleProperty::getGridWidth()
{
	return m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx * m_GridAttr.cols;
}

//
//	Grid Methods
//
CString CSingleProperty::getData(int col, int row)
{
	if (col < 0 || row < 0)
		return "";
	CString tmp = (col < allCols ? m_writeInfo.GetAt(col) : ""), temp;

	if (tmp.IsEmpty())
		return getValue2(row);
	
	temp = Parser(tmp, "\t");
	for (int ii = 0; ii < row; ii++)
		temp = Parser(tmp, "\t");

	if (temp.IsEmpty())
		temp = getValue2(row);

	return temp;
}

void CSingleProperty::setData(CString str, long col, long row)
{
	if (col < 0 || row < 0)
		return;
	int pos = -1;
	CString tmp, temp = "", retv = "";
	if (col < allCols)
		tmp = m_writeInfo.GetAt(col);
	else
		return;

	str.Replace("*sl", "/");

	for (int ii = 0; ii < row; ii++)
	{
		temp = Parser(tmp, "\t");
		retv += temp;
		retv += "\t";
	}
	retv += str;
	retv += "\t";
	Parser(tmp, "\t");
	retv += tmp;

	m_writeInfo.SetAt(col, retv);
}

void CSingleProperty::AddCol(CString str)
{
	str.Replace("*sl", "/");
	m_writeInfo.Add(str);
	m_GridAttr.cols++;
}

void CSingleProperty::ResetGrid()
{
	m_GridAttr.headInfo = 0;
	m_GridAttr.cols = 0;
	m_GridAttr.rows = 0;
	m_writeInfo.RemoveAll();
	calScrollSz();
}

//
// Utilities
//
CString CSingleProperty::Parser(CString &srcstr, CString substr)
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

void CSingleProperty::calScrollSz()
{
	CRect rc = m_ClientRt;
	if (getGridHeight() > rc.Height())
	{
		SCROLLINFO info;
		GetScrollInfo(SB_VERT, &info);
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0; info.nMax = m_GridAttr.rows - getVisibleRows() + 1;
		info.nPage = 1;
		info.nTrackPos = 1;
		info.nPos = m_StartPnt.y;
		m_bVScroll = true;
		SetScrollInfo(SB_VERT, &info);
		ShowScrollBar(SB_VERT);
		SetScrollRange(SB_VERT, 0, m_GridAttr.rows - getVisibleRows() + 1);
	}
	else if (getGridHeight() <= rc.Height())
	{
		m_bVScroll = false;
		ShowScrollBar(SB_VERT, FALSE);
	}

	if (getGridWidth() > rc.Width())
	{
		SCROLLINFO info;
		GetScrollInfo(SB_HORZ, &info);
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0; info.nMax = /*m_GridAttr.cols -*/ getVisibleCols();
		info.nPage = 1;
		info.nTrackPos = 1;
		info.nPos = m_StartPnt.x;
		if (info.nPos > info.nMax)
			info.nPos = info.nMax;
		SetScrollInfo(SB_HORZ, &info);
		ShowScrollBar(SB_HORZ);
		SetScrollRange(SB_HORZ, 0, /*m_GridAttr.cols -*/ getVisibleCols());
	}
	else if (getGridWidth() <= rc.Width())
		ShowScrollBar(SB_HORZ, FALSE);

	if (m_GridAttr.multiCol && getVisibleCols() >= 0 && m_StartPnt.x > getVisibleCols())
		m_StartPnt.x = getVisibleCols();
	/*if (m_GridAttr.cols > getVisibleCols() && m_GridAttr.cols - m_StartPnt.x < getVisibleCols())
		m_StartPnt.x = m_GridAttr.cols - getVisibleCols();*/
}


void CSingleProperty::SetInputState()
{
	if (!m_GridAttr.multiCol)
		GetParent()->SendMessage(WM_USER, 2, MAKELPARAM(m_SelectedPnt.x, m_SelectedPnt.y));
	CRect cellRt;
	if (m_GridAttr.multiCol)
		cellRt.SetRect(m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x) * m_GridAttr.CellSz.cx + 1, 
			m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
			m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x + 1) * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);

	else
		cellRt.SetRect(m_GridAttr.HeadSz.cx + 1,
			(m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
			m_ClientRt.right,
			(m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
	if (m_pCombo || m_pEdit)
		EraseCtrls();

	switch(getType(m_SelectedPnt.y))
	{
	case 1:		// edit
		{ 
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
			CString data = getData(m_SelectedPnt.x, m_SelectedPnt.y);
			int nLimitText = 0;
			DWORD	dwStyle = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL;

			if (!getValue1(m_SelectedPnt.y).IsEmpty() &&
				getValue1(m_SelectedPnt.y).GetAt(0) == '2')
				dwStyle |= ES_NUMBER;

			m_pEdit = new CEdit;
			m_pEdit->Create(dwStyle, cellRt, this, ID_EDITCTL);
			m_pEdit->SetFont(&m_font);
			if (!getValue1(m_SelectedPnt.y).IsEmpty() &&
				getValue1(m_SelectedPnt.y).GetAt(0) == '1')
				m_pEdit->EnableWindow(FALSE);
			
			if (getValue1(m_SelectedPnt.y).GetLength() >= 3)
				nLimitText = atoi(getValue1(m_SelectedPnt.y).Mid(1, 3));

			if (nLimitText)
				m_pEdit->LimitText(nLimitText);
			m_pEdit->SetWindowText(data);
			m_pEdit->SetSel(0, -1);
			if (!m_bKeyDown)
			{
				CPoint pnt;
				GetCursorPos(&pnt);
				m_pEdit->ScreenToClient(&pnt);
				m_pEdit->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKEWPARAM(pnt.x, pnt.y));
			}
			m_pEdit->SetFocus();
			m_InputPnt = m_SelectedPnt;
			m_bKeyDown = false;
		}
		break;
	case 2: // edit combo
		{
			CString data = getData(m_SelectedPnt.x, m_SelectedPnt.y);

			cellRt.DeflateRect(0, 0, 20, 0);
			m_pEdit = new CEdit;
			m_pEdit->Create(WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NOHIDESEL | ES_AUTOHSCROLL, cellRt, this, ID_EDITCTL);
			m_pEdit->SetFont(&m_font);
			if (!getValue1(m_SelectedPnt.y).IsEmpty() &&
				getValue1(m_SelectedPnt.y).GetAt(0) == '1')
				m_pEdit->EnableWindow(FALSE);
			m_pEdit->SetWindowText(data);
			if (m_pEdit->IsWindowEnabled())
				m_pEdit->SetSel(0, -1);
			//m_pEdit->SetFocus();

			m_pButton = new CNFBtn;
			if (cellRt.Width() > 20)
				cellRt.DeflateRect(cellRt.Width(), 0, -20, 0);
			m_pButton->Create(NULL, "...", WS_CHILD | WS_VISIBLE, cellRt, this, ID_BUTTONCTL);
			m_pButton->SetFont(&m_font);
			m_pButton->SetFocus();

			m_InputPnt = m_SelectedPnt;
			m_pEdit->SetFocus();
		}
		break;
	case 3:		// combo
		{
			CString data = getData(m_SelectedPnt.x, m_SelectedPnt.y), comboStr;
			m_pCombo = new CBtnCombo;
			m_pCombo->setFont(&m_font);
			m_pCombo->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, cellRt, this, ID_COMBOCTL);
			comboStr = getValue1(m_SelectedPnt.y);
			if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
				comboStr = getReservedVal1(comboStr);
			while (!comboStr.IsEmpty())
			{
				CString str = Parser(comboStr, ";");
				int iData = atoi(Parser(str, "-"));
				m_pCombo->SetItemData(m_pCombo->AddString(str), iData);
			}
			
			int sel = m_pCombo->m_pCombo->FindStringExact(0, data);
			if (sel < 0)
				sel = 0;
			if (m_pCombo->m_pCombo->GetCount())
				m_pCombo->m_pCombo->SetCurSel(sel);

			m_pCombo->SetFocus();
			m_InputPnt = m_SelectedPnt;
		}
		break;
	case 4:		// only button
		{
			m_pButton = new CNFBtn;
			
			m_pButton->Create(NULL, "...", WS_CHILD | WS_VISIBLE, cellRt, this, ID_BUTTONCTL);
			m_pButton->SetFont(&m_font);
			m_pButton->SetFocus();

			m_InputPnt = m_SelectedPnt;
			const MSG *pmsg;
			pmsg = GetCurrentMessage();
			if (pmsg->message == WM_LBUTTONDOWN)
				OnBtnClick();
		}
		break;
	}
}

void CSingleProperty::OnChange()
{
}

HBRUSH CSingleProperty::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case ID_EDITCTL:
			if ((GetRValue(m_ctlBkColor) + GetGValue(m_ctlBkColor) + GetBValue(m_ctlBkColor)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkColor(m_ctlBkColor);
			hbr = (HBRUSH) m_ctlBrush;
			break;
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CSingleProperty::EraseCtrls(bool bFocus)
{
	m_ctlBkColor  = ::GetSysColor(COLOR_WINDOW);
	m_ctlBrush.DeleteObject();
	m_ctlBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

	if (m_pEdit)
	{
		CString tmp;
		m_pEdit->GetWindowText(tmp);
		tmp.TrimLeft(); tmp.TrimRight();
		setData(tmp, m_InputPnt.x, m_InputPnt.y);

		m_pEdit->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	if (m_pCombo)
	{
		setData(m_pCombo->GetText(), m_InputPnt.x, m_InputPnt.y);
		m_pCombo->DestroyWindow();
		delete m_pCombo;
		m_pCombo = NULL;
	}
	if (m_pButton)
	{
		m_pButton->DestroyWindow();
		delete m_pButton;
		m_pButton = NULL;
	}
	if (bFocus)
		SetFocus();

	InvalidateRect2(m_InputPnt);
}

BOOL CSingleProperty::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && !(::GetKeyState(VK_CONTROL) & 0x8000))
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			EraseCtrls();
			return TRUE;
		}
		else if (pMsg->wParam == VK_TAB)
		{
			m_bKeyDown = true;
			EraseCtrls();
			if (::GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (--m_SelectedPnt.y < 0)
					m_SelectedPnt.y = m_GridAttr.rows - 1;
			}
			else
			{
				if (++m_SelectedPnt.y >= m_GridAttr.rows)
					m_SelectedPnt.y = 0;
			}
			//m_SelectedPnt.x = 0;
			//AdjustScroll();
			m_bInvalidate = true;
			Invalidate();
			SetInputState();
			return TRUE;
		}
		
		if (m_InputPnt.x >= 0 && m_InputPnt.y >= 0 && 
			(getType(m_InputPnt.y) == 2 || getType(m_InputPnt.y) == 3))
		{
			if (getType(m_InputPnt.y) == 2 && m_pEdit && pMsg->hwnd != m_pEdit->m_hWnd && atoi(getValue1(m_InputPnt.y)) != 1)
			{
				m_pEdit->SetFocus();
				m_pEdit->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
		if ((m_SelectedPnt.x == -1 || (m_SelectedPnt.x == 0 && !m_pEdit)) && m_SelectedPnt.y >= 0 && 
			getType(m_SelectedPnt.y) == 1)
		{
			if (isalnum((char)pMsg->wParam))
			{
				m_SelectedPnt.x = 0;
				AdjustScroll();
				m_bInvalidate = true;
				Invalidate();
				m_bKeyDown = true;
				SetInputState();
				if (m_pEdit)
					m_pEdit->PostMessage(WM_KEYDOWN, (char)pMsg->wParam, (LPARAM)this);
			}
		}
		if (m_SelectedPnt.x <= 0 && m_SelectedPnt.y >= 0 && getType(m_SelectedPnt.y) == 3)
		{
			m_SelectedPnt.x = 0;
			AdjustScroll();
			m_bInvalidate = true;
			Invalidate();
			SetInputState();
			return TRUE;
		}
	}
	else if (pMsg->message == WM_KEYDOWN && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		if (m_pEdit && (pMsg->wParam == 'c' || pMsg->wParam == 'C'))
		{
			m_pEdit->Copy();
			return TRUE;
		}
		else if (m_pEdit && (pMsg->wParam == 'v' || pMsg->wParam == 'V'))
		{
			m_pEdit->Paste();
			return TRUE;
		}
		else if (m_pEdit && (pMsg->wParam == 'x' || pMsg->wParam == 'X'))
		{
			m_pEdit->Cut();
			return TRUE;
		}
		else if (m_pEdit && (pMsg->wParam == 'z' || pMsg->wParam == 'Z'))
		{
			m_pEdit->Undo();
			return TRUE;
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

LOGFONT CSingleProperty::setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	//strcpy(lf.lfFaceName, fname);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic = bItalic;
	return lf;
}

void CSingleProperty::setHeadInfo(int headNum)
{
	CProArr *proArr;

	m_GridAttr.headInfo = headNum;
	m_GridAttr.rows = 0;
	
	if (m_proMap.Lookup(headNum, proArr))
		m_GridAttr.rows = proArr->GetSize();
	
	if (!m_GridAttr.multiCol)
		AddCol(m_sData);

	if (m_StartPnt.y > m_GridAttr.rows - getVisibleRows() + 1)
		m_StartPnt.y = m_GridAttr.rows - getVisibleRows() + 1;
	if (m_StartPnt.y < 0)
		m_StartPnt.y = 0;

	calScrollSz();
}

void CSingleProperty::setHeadInfo(int idx, CString headStr)
{
	CString temp;
	CProArr *proArr;
	if (!m_proMap.Lookup(idx, proArr))
		proArr = new CProArr();
	proArr->RemoveAll();
	
	do
	{
		temp = Parser(headStr, "\t");
		if (!temp.IsEmpty())
		{
			CProHead tmp;
			tmp.headCaption	= Parser(temp, "|");
			tmp.type	= atoi(Parser(temp, "|"));
			tmp.value1	= Parser(temp, "|");
			tmp.value2	= Parser(temp, "|");
			tmp.formM	= Parser(temp, "|");
			tmp.desc	= Parser(temp, "|");
			proArr->Add(tmp);
			temp = "^^";
		}
	}while(!temp.IsEmpty());

	m_proMap.SetAt(idx, proArr);
}

CString CSingleProperty::getReservedVal1(CString str)
{
	CString field = "00$bool\t01$fstyle\t02$alignment\t";
	int pos = field.Find(str);
	if (pos == -1)
		return "";
	switch(atoi(field.Mid(pos - 2, 2)))
	{
	case 0:	//	$bool
		return "0-False;1-True;";
	case 1:	//	$fstyle
		return "0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;";
	case 2:	//	$alignment
		return "0-Center;1-Left;2-Right;";
	}
	return "";
}

void CSingleProperty::Initialize(CString sData)
{
	m_sData = sData;
}

void CSingleProperty::OnSelEndOk()
{
	if (m_pCombo)
		setData(m_pCombo->GetText(), m_InputPnt.x, m_InputPnt.y);
}


BOOL CSingleProperty::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT && pWnd->GetSafeHwnd() == this->GetSafeHwnd())
	{
		switch(m_nCursor)	
		{
		case 1:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
			return TRUE;
		case 2:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSingleProperty::OnBtnClick()
{
	if (getValue1(m_InputPnt.y).GetLength() <= 4)
		return;
	int dlgnum = atoi(getValue1(m_InputPnt.y).Mid(4));
	switch(dlgnum)
	{
	case 1002:	//	Color Dialog
		{
			DWORD rgb = atol(getData(m_InputPnt.x, m_InputPnt.y));
			CRect cellRt;
			if (m_GridAttr.multiCol)
				cellRt.SetRect(m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x) * m_GridAttr.CellSz.cx + 1, 
				m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
				m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x + 1) * m_GridAttr.CellSz.cx, 
				m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
			
			else
				cellRt.SetRect(m_GridAttr.HeadSz.cx + 1,
				(m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
				m_ClientRt.right,
				(m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
			ClientToScreen(&cellRt);
			CColorDlg *clDlg = new CColorDlg(this, rgb, true, cellRt);
			if (clDlg->DoModal() == IDOK)
			{
				//m_mapH->modified = true;
				if (m_pEdit)
				{
					CString tmp;
					tmp.Format("%d", clDlg->m_color);
					m_pEdit->SetSel(0, -1);
					m_pEdit->ReplaceSel(tmp);
					setData(tmp, m_InputPnt.x, m_InputPnt.y);

					COLORREF rgbValue = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, 
						atoi(getData(m_InputPnt.x, m_InputPnt.y)));
					m_ctlBkColor  = rgbValue;
					m_ctlBrush.DeleteObject();
					m_ctlBrush.CreateSolidBrush(rgbValue);
				}
				//SaveData(m_index, m_InputPnt.x, m_InputPnt.y);
			}
		
			delete clDlg;
		}
		break;
	case 1003:	//	File Dialog
		{
			CFileDialog	fileD(TRUE, NULL, NULL,
				OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"Bitmap Files (*.bmp)|*.bmp|Gif Files (*.gif)|*.gif||");
			if (fileD.DoModal() != IDOK)
				return;
			if (m_pEdit)
			{
				CString tmp = fileD.GetPathName();
				int	offs = tmp.ReverseFind('\\');
				if (offs != -1)
					tmp = tmp.Mid(offs + 1);
				m_pEdit->SetSel(0, -1);
				m_pEdit->ReplaceSel(tmp);
			}
		}
		break;
	case 1100:	//	MultiGrid Edit Dialog
		{
			CMGColumnDlg	Dlg;
			Dlg.m_sData = getData(m_InputPnt.x, m_InputPnt.y);
			if (Dlg.DoModal() == IDOK)
			{
				if (m_pEdit)
				{
					m_pEdit->SetSel(0, -1);
					m_pEdit->ReplaceSel(Dlg.m_sData);
					setData(Dlg.m_sData, m_InputPnt.x, m_InputPnt.y);
				}
			}
		}
		break;
	case 1101:	//	control Dialog
		{
			HINSTANCE	handle;
			CString		path = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR),
				sFileName = getData(m_InputPnt.x, m_InputPnt.y),
				sMtblPath = "";
			if (!path.IsEmpty() && path.GetAt(path.GetLength() - 1) != '\\')
				path += "\\";

			sMtblPath = path;
			sMtblPath += "mtbl\\";

			path += "dev\\";
			SetCurrentDirectory(path);
			path += m_sControlName;
			path += ".dll";

			if (m_pEdit)
				m_pEdit->GetWindowText(sFileName);
			
			handle = LoadLibrary(path);
			if (handle)
			{
				char*	(APIENTRY *axPropDlg)(const char*, const char*);
				axPropDlg = (char* (APIENTRY*)(const char*, const char*))GetProcAddress(handle,
					_T("axPropDlg"));
				if (axPropDlg != NULL)
				{
					if (m_pEdit)
					{
						CString sData = CString(axPropDlg(sFileName.operator LPCTSTR(), sMtblPath.operator LPCTSTR()));
						m_pEdit->SetSel(0, -1);
						m_pEdit->ReplaceSel(sData);
						
						setData(sData, m_InputPnt.x, m_InputPnt.y);
					}
				}
				
				FreeLibrary(handle);
			}
		}
		break;
	}
	m_bInvalidate = true;
	Invalidate();
}

BOOL CSingleProperty::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (!m_GridAttr.multiCol && m_bVScroll)
	{
		if (zDelta < 0)
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), 0);
		else
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), 0);
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CSingleProperty::InvalidateRect2(CPoint pnt)
{
	if (!IsWindow(m_hWnd))
		return;
	CRect cellRt;
	GetClientRect(&cellRt);
	if (pnt.x >= 0)
		pnt.x -= m_StartPnt.x;
	if (pnt.y >= 0)
		pnt.y -= m_StartPnt.y;
	if (m_GridAttr.multiCol)
		cellRt.SetRect(m_GridAttr.HeadSz.cx + pnt.x * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + pnt.y * m_GridAttr.CellSz.cy,
			m_GridAttr.HeadSz.cx + (pnt.x + 1) * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + (pnt.y + 1) * m_GridAttr.CellSz.cy);
	else
		cellRt.SetRect(m_GridAttr.HeadSz.cx, pnt.y * m_GridAttr.CellSz.cy,
			cellRt.right, (pnt.y + 1) * m_GridAttr.CellSz.cy);
	if (pnt.x < 0)
	{
		cellRt.left = 0;
		cellRt.right = m_GridAttr.HeadSz.cx;
	}
	if (pnt.y < 0)
	{
		cellRt.top = 0;
		cellRt.bottom = m_GridAttr.HeadSz.cy;
	}
	
	if (pnt.x < -1 || pnt.y < -1)
		return;
	DrawBitmap2(pnt.x, pnt.y, cellRt);
	m_bInvalidate = false;
	InvalidateRect(&cellRt);
}

// multi column 관련


void CSingleProperty::SetKeyIn(char nChar)
{
	for (int ii = 0; ii < m_GridAttr.rows; ii++)
		if (atoi(getValue2(ii)) == 1)
		{
			m_SelectedPnt.y = ii;
			m_SelectedPnt.x = 0;
			AdjustScroll();
			m_bInvalidate = true;
			Invalidate();
			m_bKeyDown = true;
			SetInputState();
			if (m_pEdit)
			{
				m_pEdit->PostMessage(WM_KEYDOWN, nChar, (LPARAM)this);
				m_pEdit->SetFocus();
			}
			break;
		}
}

void CSingleProperty::AdjustScroll()
{
	if (m_GridAttr.rows < 0 || m_GridAttr.cols < 0)
		return;
	if (m_GridAttr.multiCol)
	{
		if (m_SelectedPnt.x + 1 > m_StartPnt.x + getVisibleCols())
		{
			OnHScroll(SB_LINEDOWN, 0, NULL);
			m_bInvalidate = true;
			Invalidate();
		}
		else if (m_SelectedPnt.x < m_StartPnt.x)
		{
			OnHScroll(SB_THUMBTRACK, 0, NULL);
			m_bInvalidate = true;
			Invalidate();
		}
	}
	while(m_SelectedPnt.y + 2 > m_StartPnt.y + getVisibleRows())
		OnVScroll(SB_LINEDOWN, 0, NULL);

	while(m_SelectedPnt.y < m_StartPnt.y)
		OnVScroll(SB_LINEUP, 0, NULL);

	m_bInvalidate = true;
	Invalidate();
}

void CSingleProperty::refreshData()
{
	int lastRow = m_StartPnt.y + getVisibleRows();
	for (int ii = m_StartPnt.y; ii <= lastRow; ii++)
		InvalidateRect2(CPoint(0, ii));
}


CString CSingleProperty::getValues()
{
	EraseCtrls();
	CString tmp = "", sTmp;
	for (int ii = 0; ii < m_GridAttr.rows; ii++)
	{
		CString data;
		if (getType(ii) == 3)
		{
			data = "-" + getData(0, ii) + ";";
			CString comboStr = getValue1(ii);
			int pos = -1;
			if (!comboStr.CompareNoCase("$bool"))
			{
				if (!getData(0, ii).CompareNoCase("false"))
					continue;
				else
				{
					tmp += "/";
					tmp += getFormM(ii);
					continue;
				}
			}

			if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
				comboStr = getReservedVal1(comboStr);
			pos = comboStr.Find(data);
			if (pos >= 0)
			{
				while(pos >= 0 && comboStr.GetAt(pos) != ';')pos--;
				pos++;
				data = comboStr.Mid(pos, comboStr.Find(";", pos) - pos);
			}
		}
		else
			data = getData(0, ii);
		data.Replace("/", "*sl");
		tmp += "/";
		tmp += getFormM(ii);
		if (getType(ii) == 3)
			tmp += Parser(data, "-");
		else
			tmp += data;
	}
	return tmp;
}

CString CSingleProperty::getAllData()
{
	CString tmp = "";
	for (int ii = 0; ii < m_GridAttr.rows; ii++)
	{
		tmp += getData(0, ii);
		tmp += "\t";
	}
	return tmp;
}

CString CSingleProperty::convertData(CString str)
{
	CString tmp = "";
	for (int ii = 0; ii < m_GridAttr.rows; ii++)
	{
		CString sTmp = "/" + getFormM(ii);
		int pos = str.Find(sTmp);
		if (pos == -1)
		{
			if (getType(ii) == 3 && !getValue1(ii).CompareNoCase("$bool"))
				tmp += "False\t";
			else
				tmp += "\t";
		}
		else
		{
			if (getType(ii) == 3)
			{
				CString sData;
				if (!getValue1(ii).CompareNoCase("$bool"))
					sData = "True";
				else
				{
					CString comboStr = getValue1(ii);
					sData = str.Mid(pos + sTmp.GetLength());;
					int pos = -1;
					sData = Parser(sData, "/");;
					if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
						comboStr = getReservedVal1(comboStr);
					sData += '-';
					pos = comboStr.Find(sData);
					if (pos >= 0)
					{
						pos += sData.GetLength();
						sData = comboStr.Mid(pos, comboStr.Find(";", pos) - pos);
					}
				}
				tmp += sData;
				tmp += "\t";
			}
			else
			{
				CString sData = str.Mid(pos + sTmp.GetLength());
				tmp += Parser(sData, "/");
				tmp += "\t";
			}
		}
	}
	TRACE("%s\n", tmp);
	return tmp;
}

void CSingleProperty::SetMulti(bool bMulti)
{
	m_GridAttr.multiCol = bMulti;
}

int CSingleProperty::insertCol(int pos)
{
	if (!m_GridAttr.multiCol || pos < -2)
		return -1;

	EraseCtrls();

	if (pos == -1)
		pos = m_GridAttr.cols + 1;
	else if (pos == -2)
		pos = m_SelectedPnt.x;
	if (pos < 0)
		pos = 0;

	m_writeInfo.InsertAt(pos, "");
	m_GridAttr.cols++;
	m_bInvalidate = true;
	Invalidate();

	return pos;
}

void CSingleProperty::deleteCol(int col)
{
	if (col == -1)
		col = m_SelectedPnt.x;
	if (!m_GridAttr.multiCol || m_SelectedPnt.y != -1 || m_SelectedPnt.x == -1 || m_GridAttr.cols <= 0 || col >= m_GridAttr.cols)
		return;
	if (col >= 0 && col < m_writeInfo.GetSize())
		m_writeInfo.RemoveAt(col);
	m_GridAttr.cols--;
	m_SelectedPnt.x--;
	m_bInvalidate = true;
	Invalidate();
}

CString CSingleProperty::getData2(int col, int row)
{
	CString val = getData(col, row);
	if (val.IsEmpty())
		return "";
	if (getType(row) == 3)
	{
		CString comboStr = getValue1(row), data = val;
		int pos;
		if (!comboStr.CompareNoCase("$bool"))
		{
			if (!val.CompareNoCase("false"))
				return "0";
			return "1";
		}

		if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
			comboStr = getReservedVal1(comboStr);
		if (comboStr.GetAt(comboStr.GetLength() - 1) != ';')
			comboStr += ';';
		pos = comboStr.Find("-" + data + ";");
		if (pos >= 0)
		{
			while(pos >= 0 && comboStr.GetAt(pos) != ';')pos--;
			pos++;
			data = comboStr.Mid(pos, comboStr.Find(";", pos) - pos);
			data = Parser(data, "-");
		}
		return data;
	}
	return val;
}
