// multiPropGrid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "multiPropGrid.h"
#include "MPropGridButton.h"
#include "MPropGridCombo.h"
#include "sepWnd.h"

#include "cellEdit.h"
#include "attrDlg.h"
#include "colorDlg.h"
#include "comboItem.h"
#include "../h/mainvar.h"
#include "../h/keywords.h"
#include "../awTool/awObjectLoad.h"
#include "../h/formOb.h"
#include "../h/mapform.H"

#define	WM_APPLYMULTISEL	WM_USER + 2
#define	lastRows	m_gridAttr.rows - m_startPnt.y
#define	allCols		(int)m_writeInfo.GetSize()

#define	CS_DEFAULT	0
#define	CS_IBEAM	1
#define	CS_SIZEWE	2
#define	CS_DROPMOVE	3
#define	CS_NODROPMOVE	4

#define CLR_MULTISEL	RGB(140, 230, 140)

struct _field {
	int	num;
	char*	name;
} fieldINFO[] = {	// Cell (grid column, table row)
	{100,	"cellname"},
	{101,	"cellcharType"},
	{102,	"cellioType"},
	{103,	"cellkind"},
	{104,	"cellsize"},
	{105,	"cellwidth"},
	{106,	"cellheadText"},
	{107,	"cellalignmentHead"},
	{108,	"cellalignmentText"},
	{109,	"cellheadPaintColor"},
	{110,	"cellheadTextColor"},
	{111,	"cellpaintColor"},
	{112,	"celltextColor"},
	{113,	"celloffsets"},
	{114,	"cellstrings"},
	{115,	"celldominoName"},
	{116,	"cellreferenceName"},
	{117,	"cellstyles"},
	{118,	"cellattributes"},
	{119,	"allcellwidth"},
	//{120,	"cellstr2"},
	//{121,	"celldat"},
	{122,	"entryCombo"},
	{123,	"cellkeyParam"},
	{124,	"cellalignmentImage"},
	{125,	"cellimages"},
	{126,	"celldisplays"},
	{127,	"celltypeHint"},
	{128,	"cellhintText"},
	{129,	"cellauxAttributes"},
	{130,	"cellExFormula"},
};

// CMultiPropGrid

IMPLEMENT_DYNAMIC(CMultiPropGrid, CWnd)

CMultiPropGrid::CMultiPropGrid()
{
	LOGFONT lf;

	m_edit       = NULL;
	m_combo      = NULL;
	m_button     = NULL;
	m_bitmap     = NULL;
	m_invalidate = true;
	m_move       = false;
	m_startPnt    = CPoint(0, 0);
	m_selectedPnt = CPoint(-1, -1);
	m_inputPnt    = CPoint(0, 0);

	m_writeInfo.RemoveAll();
	m_proMap.RemoveAll();

	m_gridAttr.headInfo  = 0;
	m_gridAttr.CellSz    = CSize(100, 16);
	m_gridAttr.HeadSz    = CSize(80, 16);
	m_gridAttr.lineColor = RGB(190, 190, 190);
	m_gridAttr.cols      = 0;
	m_gridAttr.rows      = 0;

	setFont(&lf, 9);
	m_font.CreatePointFontIndirect(&lf);

	m_fieldMap.RemoveAll();
	for (int ii = 0; ii < sizeof(fieldINFO)/sizeof(_field); ii++)
		m_fieldMap.SetAt(fieldINFO[ii].name, (void*)fieldINFO[ii].num);

	m_ctlBkColor = ::GetSysColor(COLOR_WINDOW);
	m_ctlBrush.DeleteObject();
	m_ctlBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

	m_noDropMoveCursor = AfxGetApp()->LoadCursor(IDR_NO_DROPMOVE);
	m_dropMoveCursor = AfxGetApp()->LoadCursor(IDR_DROPMOVE);

	m_drag    = false;
	m_dragCol = -2;
	m_sepWnd  = NULL;
}

CMultiPropGrid::~CMultiPropGrid()
{
	m_fieldMap.RemoveAll();
	m_multiSelMap.RemoveAll();

	if (m_bitmap)
		delete m_bitmap;

	if (m_edit)
	{
		m_edit->DestroyWindow();
		delete m_edit;
	}

	if (m_combo)
	{
		m_combo->DestroyWindow();
		delete m_combo;
	}

	if (m_button)
	{
		m_button->DestroyWindow();
		delete m_button;
	}

	int	key;
	CProArr* value;
	for (POSITION pos = m_proMap.GetStartPosition(); pos; )
	{
		m_proMap.GetNextAssoc(pos, key, value);
		delete value;
	}
	m_proMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMultiPropGrid, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_CBN_SELENDOK(ID_COMBOCTL, &CMultiPropGrid::OnSelEndOk)
	ON_BN_CLICKED(ID_BUTTONCTL, &CMultiPropGrid::OnBtnClick)
	ON_MESSAGE(WM_USER+1, &CMultiPropGrid::OnEraseCtrl)
	ON_MESSAGE(WM_APPLYMULTISEL, &CMultiPropGrid::OnApplyMultiSel)
END_MESSAGE_MAP()

// CMultiPropGrid 메시지 처리기입니다.

BOOL CMultiPropGrid::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && !(::GetKeyState(VK_CONTROL) & 0x8000))
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			m_keyDown = true;
			EraseCtrls();
			if (++m_selectedPnt.y >= m_gridAttr.rows)
				m_selectedPnt.y = 0;
			setInputState();
			return TRUE;

		case VK_TAB:
			m_keyDown = true;
			EraseCtrls();
			if (++m_selectedPnt.x >= allCols)
				m_selectedPnt.x = 0;
				
			if (m_selectedPnt.x + 1 > m_startPnt.x + getVisibleCols())
			{
				OnHScroll(SB_LINEDOWN, 0, NULL);
				m_invalidate = true;
				Invalidate();
			}
			else if (m_selectedPnt.x < m_startPnt.x)
			{
				OnHScroll(SB_THUMBTRACK, 0, NULL);
				m_invalidate = true;
				Invalidate();
			}
			setInputState();
			return TRUE;

		case VK_DELETE:
			if (m_selectedPnt.y == -1 && m_selectedPnt.x >= 0)
			{
				DeleteCol(m_selectedPnt.x);
				return TRUE;
			}
			break;

		case VK_INSERT:
			if (m_selectedPnt.y == -1 && m_selectedPnt.x >= 0)
			{
				InsertCol(m_selectedPnt.x);
				return TRUE;
			}
			break;
		}
	}
	else if (pMsg->message == WM_KEYDOWN && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		if (m_edit)
		{
			switch (pMsg->wParam)
			{
			case 'c':
			case 'C':
				m_edit->Copy();
				return TRUE;
			case 'v':
			case 'V':
				m_edit->Paste();
				return TRUE;
			case 'x':
			case 'X':
				m_edit->Cut();
				return TRUE;
			case 'z':
			case 'Z':
				m_edit->Undo();
				return TRUE;
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}


void CMultiPropGrid::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rc = m_clientRc;

	if (m_invalidate)
		drawBitmap(&dc);
	
	if (m_bitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap = NULL;
		
		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_bitmap);
		
		dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}

	if (m_move)
	{
		rc.left = m_gridAttr.HeadSz.cx;
		rc.right = rc.left + 1;
		dc.FillSolidRect(rc, RGB(0, 0, 0));
	}
}


void CMultiPropGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_gridAttr.rows)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	CRect	rc;
	CPoint	oldSelected = m_selectedPnt;

	EraseCtrls();
	m_dragCol = -2;
	GetClientRect(&rc);
	rc.left  = m_gridAttr.HeadSz.cx - 1;
	rc.right = m_gridAttr.HeadSz.cx + 1;
	if (rc.PtInRect(point))
	{
		SetCapture();
		m_move = true;
		Invalidate();
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	rc.SetRect(0, 0, m_gridAttr.HeadSz.cx + m_gridAttr.CellSz.cx * allCols, m_gridAttr.HeadSz.cy + m_gridAttr.CellSz.cy * lastRows);
	if (rc.PtInRect(point))
	{
		if (point.x < m_gridAttr.HeadSz.cx)
			m_selectedPnt.x = -1;
		else 
		{
			m_selectedPnt.x = 0;
			while((m_selectedPnt.x + 1) * m_gridAttr.CellSz.cx + m_gridAttr.HeadSz.cx < point.x)
				m_selectedPnt.x++;
			m_selectedPnt.x += m_startPnt.x;
			if (m_selectedPnt.x >= allCols)
				m_selectedPnt.x = -1;
		}
		
		if (point.y < m_gridAttr.HeadSz.cy)
			m_selectedPnt.y = -1;
		else 
		{
			m_selectedPnt.y = 0;
			while((m_selectedPnt.y + 1) * m_gridAttr.CellSz.cy + m_gridAttr.HeadSz.cy < point.y)
				m_selectedPnt.y++;
			m_selectedPnt.y += m_startPnt.y;
		}

		if (m_selectedPnt == CPoint(-1, -1))
		{
			if (m_multiSelMap.GetCount())
				m_multiSelMap.RemoveAll();
			else
			{
				for (int nSel = 0; nSel < m_gridAttr.cols; nSel++)
					m_multiSelMap.SetAt(nSel, 1);
			}
			m_invalidate = true;
			Invalidate();
			return;
		}

		invalidateRect2(oldSelected);
		invalidateRect2(CPoint(m_selectedPnt.x, -1));
		
		// column drag
		if (m_selectedPnt.x != -1 && m_selectedPnt.y == -1)
		{
			SetCapture();
			m_dragPnt = point;
			m_dragCol = m_selectedPnt.x;
			m_drag = false;
		}

		invalidateRect2(CPoint(-1, oldSelected.y));
		invalidateRect2(CPoint(-1, m_selectedPnt.y));
		invalidateRect2(m_selectedPnt);
		if (m_selectedPnt.x >= 0)
			setInputState();
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CMultiPropGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_dragCol >= 0)
		::ReleaseCapture();

	if (m_drag)
	{
		CRect	rc;
		int	nTarget = -2;

		GetClientRect(&rc);
		m_cursor = CS_DEFAULT;
		if (m_sepWnd)
		{
			m_sepWnd->DestroyWindow();
			delete m_sepWnd;
			m_sepWnd = NULL;
		}

		if (m_gridAttr.HeadSz.cy >= point.y && point.x <= rc.Width() && point.x >= m_gridAttr.HeadSz.cx)
		{
			int	nCol, cx = m_gridAttr.HeadSz.cx + m_gridAttr.CellSz.cx / 2;
			for (nCol = 0; nCol < m_gridAttr.cols; nCol++)
			{
				if (point.x <= cx)
					break;
				cx += m_gridAttr.CellSz.cx;
			}
			nTarget = m_startPnt.x + nCol;
			if (m_dragCol + 1 != nTarget)
			{
				CString	str;
				AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SWAPCELL, m_index), MAKELPARAM(m_dragCol, nTarget));
				str = m_writeInfo.GetAt(m_dragCol);
				m_writeInfo.RemoveAt(m_dragCol);
				m_writeInfo.InsertAt((m_dragCol < nTarget? nTarget - 1: nTarget), str);
				m_invalidate = true;
				Invalidate();
			}
		}
		m_drag = false;
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	if (m_move)
	{
		ReleaseCapture();
		m_invalidate = true;
		Invalidate();
		m_move = false;
	}

	CWnd::OnLButtonUp(nFlags, point);
}


void CMultiPropGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dragCol >= 0 && !m_move && !m_drag && (nFlags & MK_LBUTTON) && abs((m_dragPnt.x - point.x) * (m_dragPnt.y - point.y)) > 4)
	{
		m_drag = true;
		m_cursor = CS_DROPMOVE;
		::SetCursor(m_dropMoveCursor);
		if (m_sepWnd)
		{
			m_sepWnd->DestroyWindow();
			delete m_sepWnd;
		}
		m_sepWnd = new CSepWnd();
		m_sepWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 6, m_gridAttr.HeadSz.cy + 4), GetParent(), 3);
		CWnd::OnMouseMove(nFlags, point);
		return;
	}
	
	if (m_drag)
	{
		int	nCol, cx = m_gridAttr.HeadSz.cx, nTarget = -2;
		for (nCol = 0; nCol < m_gridAttr.cols; nCol++)
		{
			if (point.x <= cx)
				break;
			cx += m_gridAttr.CellSz.cx;
		}
		if (nCol > 0)
		{
			nTarget = m_startPnt.x + nCol - 1;
			if (m_sepWnd)
			{
				CRect	rc;

				rc.SetRect(cx - m_gridAttr.CellSz.cx, 0, cx, m_gridAttr.HeadSz.cy);
				ClientToScreen(&rc);
				GetParent()->ScreenToClient(&rc);
				m_sepWnd->ShowWindow(SW_SHOW);
				if (rc.Width() / 2 + rc.left < point.x)
					m_sepWnd->MoveWnd(rc.right - 3, rc.top - 3);
				else
					m_sepWnd->MoveWnd(rc.left - 3, rc.top - 3);
			}
			m_cursor = CS_DROPMOVE;
			::SetCursor(m_dropMoveCursor);
			CWnd::OnMouseMove(nFlags, point);
			return;
		}
		if (m_sepWnd)
			m_sepWnd->ShowWindow(SW_HIDE);
		m_cursor = CS_NODROPMOVE;
		::SetCursor(m_noDropMoveCursor);
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (m_move)
	{
		m_gridAttr.HeadSz.cx = point.x;
		Invalidate();
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (m_gridAttr.rows)
	{
		CRect	rc;

		m_cursor = CS_DEFAULT;
		if (m_selectedPnt.y >= 0)
		{
			rc.top    = (m_selectedPnt.y - m_startPnt.y) * m_gridAttr.CellSz.cy + m_gridAttr.HeadSz.cy;
			rc.bottom = (m_selectedPnt.y - m_startPnt.y + 1) * m_gridAttr.CellSz.cy + m_gridAttr.HeadSz.cy;
			rc.left   = m_gridAttr.HeadSz.cx;
			rc.right  = m_gridAttr.HeadSz.cx + m_gridAttr.CellSz.cx * (allCols - m_startPnt.x);
			if (rc.PtInRect(point))
			{
				if (getType(m_selectedPnt.y) == 1)
					m_cursor = CS_IBEAM;
			}
		}
		GetClientRect(&rc);
		rc.left = m_gridAttr.HeadSz.cx - 1;
		rc.right = m_gridAttr.HeadSz.cx + 1;
		if (rc.PtInRect(point))
			m_cursor = CS_SIZEWE;
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CMultiPropGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_gridAttr.rows)
	{
		CWnd::OnRButtonDown(nFlags, point);
		return;
	}

	if (point.x < m_gridAttr.HeadSz.cx && m_gridAttr.cols > 1)
	{
		CMenu	menu;
		int	cmd;
		
		OnLButtonDown(nFlags, point);
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 1, _T("Common apply"));
		
		ClientToScreen(&point);
		cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
		if (cmd == 1)
		{
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
			for (int ii = 1; ii < m_gridAttr.cols; ii++)
				setLikeCell(ii);
		}
	}
	CWnd::OnRButtonDown(nFlags, point);
}


void CMultiPropGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO	info;

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

	case SB_LINEDOWN:	// move 1
		nPos++;
		break;

	case SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;

	case SB_PAGEDOWN:	// move nPage
		nPos += 2;
		break;

	case SB_THUMBTRACK:
		break;

	default:
		return;
	}

	if (nPos > (UINT)info.nMax)
		nPos = info.nMax;
	info.nPos = nPos;
	info.fMask = SIF_POS;
	SetScrollInfo(SB_HORZ, &info);
	
	m_startPnt.x = nPos;
	EraseCtrls();
	m_invalidate = true;
	Invalidate();

//	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMultiPropGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO	info;

	GetScrollInfo(SB_VERT, &info);
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

	case SB_LINEDOWN:	// move 1
		nPos++;
		break;

	case SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;

	case SB_PAGEDOWN:	// move nPage
		nPos += 2;
		break;

	case SB_THUMBTRACK:
		break;

	default:
		return;
	}

	if (nPos > (UINT)info.nMax)
		nPos = info.nMax;
	info.nPos = nPos;
	info.fMask = SIF_POS;
	/*m_pVScroll->*/SetScrollInfo(SB_VERT, &info);
	
	m_startPnt.y = nPos;
	EraseCtrls();
	m_invalidate = true;
	Invalidate();

//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


HBRUSH CMultiPropGrid::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_EDIT && pWnd->GetDlgCtrlID() == ID_EDITCTL)
	{
		if ((GetRValue(m_ctlBkColor) + GetGValue(m_ctlBkColor) + GetBValue(m_ctlBkColor)) / 3 >= 192)
			pDC->SetTextColor(RGB(0, 0, 0));
		else
			pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(m_ctlBkColor);
		hbr = (HBRUSH) m_ctlBrush;
	}
	return hbr;
}


BOOL CMultiPropGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT && pWnd->GetSafeHwnd() == this->GetSafeHwnd())
	{
		switch (m_cursor)	
		{
		case CS_IBEAM:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
			return TRUE;
		case CS_SIZEWE:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
		case CS_DROPMOVE:
			::SetCursor(m_dropMoveCursor);
			return TRUE;
		case CS_NODROPMOVE:
			::SetCursor(m_noDropMoveCursor);
			return TRUE;
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CMultiPropGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	GetClientRect(&m_clientRc);
	EraseCtrls(FALSE);
	m_invalidate = true;
}


void CMultiPropGrid::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE)
	{
		if (m_selectedPnt.y == -1 && m_selectedPnt.x >= 0)
		{
			int	value;

			if (m_multiSelMap.Lookup(m_selectedPnt.x, value))
				m_multiSelMap.RemoveKey(m_selectedPnt.x);
			else
				m_multiSelMap.SetAt(m_selectedPnt.x, 1);
		}
	}

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void  CMultiPropGrid::OnSelEndOk()
{
	if (m_combo)
	{
		setData(m_combo->GetText(), m_inputPnt.x, m_inputPnt.y);
		saveData(m_index, m_inputPnt.x, m_inputPnt.y);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
	}
}

void  CMultiPropGrid::OnBtnClick()
{
	int	dlgnum = atoi(getValue2(m_inputPnt.y));

	switch (dlgnum)
	{
	case 1002:	//	Color Dialog
		{
			CRect	cellRc;
			DWORD	rgb = atol(getData(m_inputPnt.x, m_inputPnt.y));

			cellRc.SetRect(m_gridAttr.HeadSz.cx + (m_selectedPnt.x - m_startPnt.x) * m_gridAttr.CellSz.cx + 1, 
					m_gridAttr.HeadSz.cy + (m_selectedPnt.y - m_startPnt.y) * m_gridAttr.CellSz.cy + 1,
					m_gridAttr.HeadSz.cx + (m_selectedPnt.x - m_startPnt.x + 1) * m_gridAttr.CellSz.cx, 
					m_gridAttr.HeadSz.cy + (m_selectedPnt.y - m_startPnt.y + 1) * m_gridAttr.CellSz.cy);
			ClientToScreen(&cellRc);
			CColorDlg* dlg = new CColorDlg(this, rgb, true, cellRc);
			if (dlg->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				if (m_edit)
				{
					CString tmps;

					tmps.Format(_T("%d"), dlg->m_color);
					m_edit->SetSel(0, -1);
					m_edit->ReplaceSel(tmps);
					setData(tmps, m_inputPnt.x, m_inputPnt.y);

					COLORREF rgbValue = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, atoi(getData(m_inputPnt.x, m_inputPnt.y)));
					m_ctlBkColor  = rgbValue;
					m_ctlBrush.DeleteObject();
					m_ctlBrush.CreateSolidBrush(rgbValue);
				}
				saveData(m_index, m_inputPnt.x, m_inputPnt.y);
			}
			dlg->DestroyWindow();
			delete dlg;
		}
		break;

	case 1003:	//	File Dialog
		{
			CFileDialog	dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
						"Bitmap Files (*.bmp)|*.bmp|Gif Files (*.gif)|*.gif||");
			if (dlg.DoModal() != IDOK)
				return;

			if (m_edit)
			{
				m_mapH->modified = true;
				m_edit->SetSel(0, -1);
				m_edit->ReplaceSel(dlg.GetFileName());

				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();

				Prop->CellSelected = m_selectedPnt.x;
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
				Prop->images = dlg.GetFileName();
			}
		}
		break;

	case 1004:	//	attributes
		if (m_index >= 0 || m_index == -2)
		{
			int	idx = m_index;

			if (idx == -2)
			{
				for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
				{
					CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(jj);
					CformProp*	Prop = (CformProp*)ObjLoad->getProp();
					if (Prop->bCommon)
					{
						idx = jj;
						break;
					}
				}
			}

			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
			CformProp*	Prop = (CformProp*)ObjLoad->getProp();

			Prop->CellSelected = m_selectedPnt.x;
			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);

			CAttrDlg dlg(this, Prop->kind, (BYTE)cellProp->charType, (BYTE)cellProp->ioType,
				cellProp->attributes, cellProp->auxAttributes);
			if (dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				cellProp->attributes = dlg.m_attributes;
				cellProp->auxAttributes = dlg.m_auxAttributes;
			}
			if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
				checkGridMultiLine(idx);
			SendMessage(WM_APPLYMULTISEL, m_selectedPnt.x);
		}
		break;

	case 1007:	// edit format
		{
			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
			CformProp*	Prop = (CformProp*)ObjLoad->getProp();
			
			Prop->CellSelected = m_inputPnt.x;
			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);

			CCellEdit dlg(this, fmGRID, (BYTE)cellProp->charType,
				(BYTE)cellProp->whatEdit,
				(DWORD)cellProp->auxValues,
				cellProp->editFormat,
				cellProp->name,
				cellProp->attributes,
				cellProp->size);
			if (dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				cellProp->attributes = dlg.m_attributes;
				cellProp->whatEdit = dlg.m_whatEdit;
				cellProp->auxValues = dlg.m_auxValues;
				cellProp->editFormat = dlg.m_editFormat;
			}
			SendMessage(WM_APPLYMULTISEL, m_selectedPnt.x);
		}
		break;

	case 1011:	// cell combo str
		{
			int	idx = m_index;

			if (m_index < 0)
				break;

			CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
			CformProp*	Prop = (CformProp*)ObjLoad->getProp();

			if (Prop->cellProperties == NULL || Prop->cellProperties->IsEmpty()) return;
			Prop->CellSelected = m_inputPnt.x;
			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);

			if (cellProp->kind != ckCOMBO)
				break;

			CComboItem	dlg(this, cellProp->entryCombo, cellProp->size);
			if (cellProp->displays)
				dlg.m_disB = cellProp->displays;
			if (cellProp->strings)
				dlg.m_datB = cellProp->strings;
			if (dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				if (m_edit)
				{
					CString tmps;

					tmps.Format(_T("%d"), dlg.m_count);
					m_edit->SetSel(0, -1);
					m_edit->ReplaceSel(tmps);
				}
				cellProp->entryCombo = dlg.m_count;
				cellProp->displays = dlg.m_disB;
				cellProp->strings = dlg.m_datB;
			}
		}
		break;
	}
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
	m_invalidate = true;
	Invalidate();
}

LRESULT CMultiPropGrid::OnEraseCtrl(WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_ESCAPE || wParam == VK_RETURN)
		EraseCtrls();
	return 0;
}


LRESULT CMultiPropGrid::OnApplyMultiSel(WPARAM wParam, LPARAM lParam)
{
	int	value, key, col = (int)wParam;
	if (m_multiSelMap.Lookup(col, value) && m_multiSelMap.GetCount())
	{
		for (POSITION pos = m_multiSelMap.GetStartPosition(); pos; )
		{
			m_multiSelMap.GetNextAssoc(pos, key, value);
			if (key != col)
				setLikeCell(key, col);
		}
	}
	return 0;
}

void CMultiPropGrid::Initialize(class mapForm* mapForm)
{

	m_mapH = mapForm;
}

void CMultiPropGrid::LoadData(int idx, int col, int row)
{
	CString tmps, text;
	LONG_PTR fnum;
	
	for (int ii = 0; ii < m_gridAttr.rows; ii++)
	{
		tmps = _T("");
		if (row >= 0)
			ii = row;

		if (!m_fieldMap.Lookup(getFormM(ii), (void *&)fnum))
			fnum = -1;
		
		for (int iCol = 0; iCol < allCols; iCol++)
		{
			if (col	>= 0)
				iCol = col;

			if (idx >= -1)
				tmps = getFormValue((int)fnum, idx, ii, iCol);
			else if (idx == -2)
			{
				text = _T("");
				for (int jj = m_mapH->forms.GetCount() - 1; jj >= 0; jj--)
				{
					CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(jj);
					CformProp*	Prop = (CformProp*)ObjLoad->getProp();
					if (Prop->bCommon)
					{
						if (text.IsEmpty())
						{
							text = getFormValue((int)fnum, jj, ii);
							tmps = text;
							if (text.IsEmpty())
								break;
						}
						else
						{
							text = getFormValue((int)fnum, jj, ii);
							if (text.Compare(tmps))
							{
								tmps = _T("");
								break;
							}
						}
					}
				}
			}
			
			if (getType(ii) == 3)
			{
				CString	comboStr = getValue1(ii);
				int	pos = -1;

				if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == _T('$'))
					comboStr = getReservedVal1(comboStr);
				tmps += _T('-');
				comboStr = _T(";") + comboStr;
				pos = comboStr.Find(_T(";") + tmps);
				if (pos >= 0)
				{
					pos += tmps.GetLength();
					tmps = comboStr.Mid(pos + 1, comboStr.Find(_T(";"), pos) - pos - 1);
				}
				
			}
			setData(tmps, iCol, ii);
			if (col >= 0)
				break;
		}
		if (row >= 0)
			break;
	}
}

void CMultiPropGrid::SetHeadInfo(int headNum)
{
	CProArr* proArr;

	m_gridAttr.headInfo = headNum;
	m_gridAttr.rows = 0;
	if (m_proMap.Lookup(headNum, proArr))
		m_gridAttr.rows = (int)proArr->GetSize();

	if (m_index >= 0)
	{
		CformProp*	Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
		if (Prop->cellProperties)
		{
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
				addCol(_T(""));
		}
		m_gridAttr.cols = allCols;
	}

	if (m_startPnt.y > m_gridAttr.rows - getVisibleRows() + 1)
		m_startPnt.y = m_gridAttr.rows - getVisibleRows() + 1;
	if (m_startPnt.y < 0)
		m_startPnt.y = 0;
	calScrollSz();
}

void CMultiPropGrid::SetHeadInfo(int idx, CString info)
{
	CString	tmps;
	CProArr* proArr;

	if (!m_proMap.Lookup(idx, proArr))
		proArr = new CProArr();
	proArr->RemoveAll();
	for ( ; true; )
	{
		tmps = parse(info, _T("\t"));
		if (tmps.IsEmpty())
			break;

		CProHead phead;

		phead.headCaption = parse(tmps, _T("|"));
		phead.type	= atoi(parse(tmps, _T("|")));
		phead.value1	= parse(tmps, _T("|"));
		phead.value2	= parse(tmps, _T("|"));
		phead.formM	= parse(tmps, _T("|"));
		phead.desc	= parse(tmps, _T("|"));
		proArr->Add(phead);
	}
	m_proMap.SetAt(idx, proArr);
}

void CMultiPropGrid::SetCellSize(long cx, long cy)
{
	m_gridAttr.CellSz.cx = cx;
	m_gridAttr.CellSz.cy = cy;
}

int CMultiPropGrid::InsertCol(int pos)
{
	if (pos < -2)
		return -1;

	m_keyDown = true;
	EraseCtrls();

	int	value;

	CformProp*	Prop = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();

	if (Prop->cellProperties == NULL) return -1;
	if (Prop->kind == fmGRID)
	{
		bool bInsert = false;
		if (pos == -1 || (pos == -2 && (m_selectedPnt.x < 0 || m_selectedPnt.y != -1)))
		{
			pos = Prop->cellProperties->GetCount();
		}
		else if (pos == -2)
		{
			bInsert = true;
			pos = m_selectedPnt.x;
		}

		cellOb* cellProp = new cellOb();
		cellProp->paintColor  = getDefaultValue(_T("GRID"), _T("DPRGB"), basINPUT);
		cellProp->textColor  = getDefaultValue(_T("GRID"), _T("DTRGB"), 232);
		cellProp->headPaintColor = getDefaultValue(_T("GRID"), _T("HPRGB"), basPRGB);
		cellProp->headTextColor = getDefaultValue(_T("GRID"), _T("HTRGB"), 232);
		cellProp->size  = 0;
		cellProp->charType  = ctNUMERIC;
		cellProp->ioType   = ioOUTPUT;
		cellProp->kind = ckNORMAL;
		cellProp->name = getInitSymbol(_T("Col"));
		if (pos == 0)
			cellProp->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), 80);
		else
			cellProp->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), 
			((cellOb*)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->cellProperties->GetAt(0))->width);
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		if (bInsert)
			Prop->cellProperties->InsertAt(pos, cellProp);
		else
			Prop->cellProperties->Add(cellProp);
		
		m_writeInfo.InsertAt(pos, _T(""));
		//repR->cols++;
		m_gridAttr.cols++;
		LoadData(m_index, pos);
	}
	else if (Prop->kind == fmGRIDEX)
	{
		bool bInsert = false;
		if (pos == -1 || (pos == -2 && (m_selectedPnt.x < 0 || m_selectedPnt.y != -1)))
		{
			pos = Prop->cellProperties->GetCount();
		}
		else if (pos == -2)
		{
			bInsert = true;
			pos = m_selectedPnt.x;
		}

		cellOb* cellProp = new cellOb();
		cellProp->paintColor  = getDefaultValue(_T("GRIDEX"), _T("DPRGB"), basINPUT);
		cellProp->textColor  = getDefaultValue(_T("GRIDEX"), _T("DTRGB"), 232);
		cellProp->headPaintColor = getDefaultValue(_T("GRIDEX"), _T("HPRGB"), basPRGB);
		cellProp->headTextColor = getDefaultValue(_T("GRIDEX"), _T("HTRGB"), 232);
		cellProp->size  = 0;
		cellProp->charType  = ctNUMERIC;
		cellProp->ioType   = ioOUTPUT;
		cellProp->kind = ckNORMAL;
		cellProp->name = getInitSymbol(_T("Col"));
		if (pos == 0)
			cellProp->width = getDefaultValue(_T("GRIDEX"), _T("COLWIDTH"), 80);
		else
			cellProp->width = getDefaultValue(_T("GRIDEX"), _T("COLWIDTH"), 
			((cellOb*)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->cellProperties->GetAt(0))->width);
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		if (bInsert)
			Prop->cellProperties->InsertAt(pos, cellProp);
		else
			Prop->cellProperties->Add(cellProp);
		
		m_writeInfo.InsertAt(pos, _T(""));
		//repR->cols++;
		m_gridAttr.cols++;
		LoadData(m_index, pos);
	}
	else
	{
		bool bInsert = false;
		if (pos == -1 || (pos == -2 && (m_selectedPnt.x < 0 || m_selectedPnt.y != -1)))
		{
			pos = Prop->cellProperties->GetCount();
		}
		else if (pos == -2)
		{
			bInsert = true;
			pos = m_selectedPnt.x;
		}
		
		cellOb* cellProp = new cellOb();
		cellProp->paintColor = getDefaultValue(_T("TABLE"), _T("DPRGB"), basINPUT);
		cellProp->textColor = getDefaultValue(_T("TABLE"), _T("DTRGB"), 232);
		cellProp->headPaintColor = getDefaultValue(_T("TABLE"), _T("HPRGB"), basPRGB);
		cellProp->headTextColor = getDefaultValue(_T("TABLE"), _T("HTRGB"), 232);
		cellProp->size  = 0;
		cellProp->charType = ctNUMERIC;
		cellProp->name = getInitSymbol(_T("Row"));
		if (pos == 0)
			cellProp->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), 80);
		else
			cellProp->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), 
			((cellOb*)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->cellProperties->GetAt(0))->width);
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		//Prop->size++;
		if (bInsert)
			Prop->cellProperties->InsertAt(pos, cellProp);
		else
			Prop->cellProperties->Add(cellProp);
		
		m_writeInfo.InsertAt(pos, _T(""));
		LoadData(m_index, pos);
		//repR->rows++;
		m_gridAttr.cols++;
	}

	for (int ii = m_gridAttr.cols - 2; ii >= pos; ii--)
	{
		if (m_multiSelMap.Lookup(ii, value))
			m_multiSelMap.SetAt(ii + 1, value);
		else
			m_multiSelMap.RemoveKey(ii + 1);
	}
	m_multiSelMap.RemoveKey(pos);
	m_invalidate = true;
	Invalidate();
	return pos;
}

void CMultiPropGrid::DeleteCol(int col)
{
	if (col == -1)
		col = m_selectedPnt.x;
	if (m_selectedPnt.y != -1 || m_selectedPnt.x == -1 || m_gridAttr.cols <= 0 || col >= m_gridAttr.cols)
		return;
	
	m_keyDown = true;
	EraseCtrls();

	int	value;
	CformProp*	Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
	cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(col);
	Prop->cellProperties->RemoveAt(col);
	delete cellProp;
	//cellItem->RemoveCell(col);
	//delete cellItem;

	m_writeInfo.RemoveAt(col);
	//if (Prop->kind == fmGRID)
	//	repR->cols--;
	//else
	//	repR->rows--;
	m_gridAttr.cols--;
	//repR->selected--;
	Prop->CellSelected--;
	m_selectedPnt.x--;

	for (int ii = col; ii < m_gridAttr.cols; ii++)
	{
		if (m_multiSelMap.Lookup(ii + 1, value))
			m_multiSelMap.SetAt(ii, value);
		else
			m_multiSelMap.RemoveKey(ii);
	}
	while (m_startPnt.x > 0 && m_gridAttr.cols - m_startPnt.x < getVisibleCols())
		m_startPnt.x--;
	//AdjustScroll();
	m_invalidate = true;
	Invalidate();
}

bool CMultiPropGrid::SetIndex(int idx)
{
	if (m_index == idx)
		return false;

	m_index = idx;
	return true;
}

void CMultiPropGrid::EraseCtrls(bool bFocus)
{
	m_ctlBkColor  = ::GetSysColor(COLOR_WINDOW);
	m_ctlBrush.DeleteObject();
	m_ctlBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

	if (m_edit)
	{
		CString tmps;

		m_edit->GetWindowText(tmps);
		if ((m_beforeVal.IsEmpty() && tmps.IsEmpty()) || (!m_beforeVal.IsEmpty() && !m_beforeVal.Compare(tmps))
			|| (!tmps.IsEmpty() && !tmps.Compare(m_beforeVal))) // 같으면 적용 안함
		{
			m_edit->DestroyWindow();
			delete m_edit;
			m_edit = NULL;
		}
		else
		{
			if (!tmps.IsEmpty())
				setData(tmps, m_inputPnt.x, m_inputPnt.y);
			saveData(m_index, m_inputPnt.x, m_inputPnt.y);
			m_edit->DestroyWindow();
			delete m_edit;
			m_edit = NULL;
			
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
		}
	}

	if (m_combo)
	{
		CString	tmps = m_combo->GetText();
		if ((m_beforeVal.IsEmpty() && tmps.IsEmpty()) || (!m_beforeVal.IsEmpty() && !m_beforeVal.Compare(tmps))
			|| (!tmps.IsEmpty() && !tmps.Compare(m_beforeVal))) // 같으면 적용 안함
		{
			m_combo->DestroyWindow();
			delete m_combo;
			m_combo = NULL;
		}
		else
		{
			setData(tmps, m_inputPnt.x, m_inputPnt.y);
			saveData(m_index, m_inputPnt.x, m_inputPnt.y);
			m_combo->DestroyWindow();
			delete m_combo;
			m_combo = NULL;
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
		}
	}
	if (m_button)
	{
		m_button->DestroyWindow();
		delete m_button;
		m_button = NULL;
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
	}
	if (bFocus)
		SetFocus();

	invalidateRect2(m_inputPnt);
}

void CMultiPropGrid::ResetGrid()
{
	m_gridAttr.headInfo = 0;
	m_gridAttr.cols = 0;
	m_gridAttr.rows = 0;

	m_writeInfo.RemoveAll();
	calScrollSz();
}

void CMultiPropGrid::saveData(int idx, int col, int row, bool bMulti)
{
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);

	CString	tmps, data, comboStr;
	int	pos;
	LONG_PTR fnum;

	for (int ii = 0; ii < m_gridAttr.rows; ii++)
	{
		if (row >= 0)
			ii = row;
		
		if (!m_fieldMap.Lookup(getFormM(ii), (void *&)fnum))
			fnum = -1;

		if (getType(ii) == 3)
		{
			data = _T("-") + getData(col, ii) + _T(";");
			comboStr = getValue1(ii);
			pos = -1;

			if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == _T('$'))
				comboStr = getReservedVal1(comboStr);
			pos = comboStr.Find(data);
			if (pos >= 0)
			{
				while (pos >= 0 && comboStr.GetAt(pos) != _T(';'))
					pos--;
				pos++;
				data = comboStr.Mid(pos, comboStr.Find(_T(";"), pos) - pos);
			}
		}
		else
			data = getData(col, ii);

		if (idx >= -1)
			setFormValue((int)fnum, idx, col, ii, data);
		else if (idx == -2)
		{
			for (int jj = 0; jj < m_mapH->forms.GetCount(); jj++)
			{
				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(jj);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();
				if (Prop->bCommon)
					setFormValue((int)fnum, jj, col, ii, data);
			}
		}

		if (row >= 0)
		{
			if (bMulti)
				SendMessage(WM_APPLYMULTISEL, col);
			break;
		}
	}
}

void CMultiPropGrid::setFormValue(int fnum, int idx, int col, int row, CString data)
{
	if (m_mapH->forms.GetCount() <= idx)
		return;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();
	if (Prop->cellProperties == NULL || Prop->cellProperties->IsEmpty()) return;

	m_mapH->modified = true;
	if (fnum == 119)	//	allcellwidth
	{
		for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
		{
			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
			cellProp->width = atoi(data);
		}
	}

	if ((Prop->CellSelected >= 0 || col >= 0))
	{
		if (col < 0)
			col = Prop->CellSelected;
		if (col == -1)
			return;

		cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(col);

		switch (fnum)
		{
		case 100:	// cellname
			cellProp->name = data;
			break;

		case 101:	// cellcharType
			cellProp->charType = atoi(data);
			if (cellProp->charType != ctNUMERIC)
				cellProp->attributes &= ~(faCOMMA | faRIGHT);
			break;

		case 102:	// cellioType
			if (atoi(data) == ioOUTPUT && cellProp->kind == ckCHECK)
			{
				cellProp->ioType = ioBOTH;
				LoadData(m_index, m_inputPnt.x, m_inputPnt.y);
				updateCombo();
			}
			else if ((atoi(data) == ioOUTPUT || atoi(data) == ioBOTH || atoi(data) == ioINPUT) && cellProp->kind == ckBUTTON)
			{
				cellProp->ioType = ioFREE;
				LoadData(m_index, m_inputPnt.x, m_inputPnt.y);
				updateCombo();
			}
			else
				cellProp->ioType = atoi(data);

			if (!((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL))	// No edit
			{
				cellProp->attributes &= ~faDATE;
				cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
			}
			break;

		case 103:	// cellkind
			if (cellProp->ioType == ioOUTPUT && atoi(data) == ckCHECK)
			{
				cellProp->kind = ckNORMAL;
				LoadData(m_index, m_inputPnt.x, m_inputPnt.y);
				updateCombo();
			}
			else if ((cellProp->ioType == ioOUTPUT || cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && atoi(data) == ckBUTTON)
			{
				cellProp->kind = ckNORMAL;
				LoadData(m_index, m_inputPnt.x, m_inputPnt.y);
				updateCombo();
			}
			else
				cellProp->kind = atoi(data);
			if (!((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL))	// No edit
			{
				cellProp->attributes  &= ~faDATE;
				cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
			}
			break;

		case 104:	// cellsize
			cellProp->size = atoi(data);
			break;

		case 105:	// cellwidth
			cellProp->width = atoi(data);
			break;

		case 106:	// cellheadText
			cellProp->headText = data;
			break;

		case 107:	// cellalignmentHead
			cellProp->alignmentHead = atoi(data);
			break;

		case 108:	// cellalignmentText
			cellProp->alignmentText = atoi(data);
			break;

		case 109:	// cellheadPaintColor
			cellProp->headPaintColor = atoi(data);
			break;

		case 110:	// cellheadTextColor
			cellProp->headTextColor = atoi(data);
			break;

		case 111:	// cellpaintColor
			cellProp->paintColor = atoi(data);
			break;

		case 112:	// celltextColor
			cellProp->textColor = atoi(data);
			break;

		case 113:	// celloffsets
			cellProp->offsets = atoi(data);
			break;

		case 114:	// cellstrings
			cellProp->strings = data;
			break;

		case 115:	// celldominoName
			cellProp->dominoName = data;
			break;

		case 116:	// cellreferenceName
			cellProp->referenceName = data;
			break;

		case 117:	// cellstyles
			{
				CString	tmps, str = getValue2(row), text = data;
				text = parse(text, _T("-"));
				int	nCnt = 1;
				while (!str.IsEmpty())
				{
					tmps = parse(str, _T(";"));
					if (nCnt == atoi(text))
						cellProp->styles |= hex2Int(tmps);
					else
						cellProp->styles &= ~hex2Int(tmps);
					nCnt++;
				}
			}
			break;

		case 118:	// cellattributes
			if (data.GetAt(0) == _T('0'))
				cellProp->attributes &= ~hex2Int(getValue2(row));
			else
				cellProp->attributes |= hex2Int(getValue2(row));
			break;

		//case 120:	// cellstr2
		//	cellProp->strings = data;
		//	break;

		//case 121:	// celldat
		//	cellProp->displays = data;
		//	break;

		case 122:	// entryCombo
			cellProp->entryCombo = atoi(data);
			break;

		case 123:	// cellkeyParam
			cellProp->keyParam = atoi(data);
			break;

		case 124:	// cellalignmentImage
			cellProp->alignmentImage = atoi(data);
			break;

		case 125:	// cellimages
			cellProp->images = data;
			break;

		case 126:	// celldisplays
			cellProp->displays = data;
			break;

		case 127:	// celltypeHint
			cellProp->typeHint = atoi(data);
			break;

		case 128:	// cellhintText
			cellProp->hintText = data;
			break;

		case 129:	// cellauxAttributes
			if ((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL)	// edit
			{
				if ((atoi(data) & getTypeOfDateFormat(cellProp->auxAttributes)) == getTypeOfDateFormat(0xffffffff))
				{
					cellProp->attributes &= ~faDATE;
					cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
				}
				else 
				{
					cellProp->attributes |= faDATE;
					cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
					cellProp->auxAttributes |= getTypeOfDateFormat(atoi(data));
				}
				break;
			}
			cellProp->attributes &= ~faDATE;
			cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
			LoadData(m_index, m_inputPnt.x, m_inputPnt.y);
			updateCombo();
			break;
		case 130:	// formula
			cellProp->formula = data;
			break;
		case 131:	// rts
			cellProp->flashKeys = data;
			break;
		}
	}
}

CString CMultiPropGrid::getFormValue(int fnum, int idx, int row, int col)
{
	CString	tmps;

	if (fnum == 119)	// allcellwidth
	{
		CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
		CformProp*	Prop = (CformProp*)ObjLoad->getProp();
		if (Prop->size > 0)
		{
			cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(0);
			tmps.Format(_T("%d"), cellProp->width);
		}
		else
			tmps = _T("80");
	}

	if (idx < 0)  return "";
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	if ( Prop->CellSelected >= 0 || col >= 0)
	{
		if (col < 0)
			col = Prop->CellSelected;
			
		cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(col);

		switch (fnum)
		{
		case 100:	// cellname
			tmps = cellProp->name;
			break;

		case 101:	// celltype
			tmps.Format(_T("%d"), cellProp->charType);
			break;

		case 102:	// celliok
			if (cellProp->ioType == ioOUTPUT && cellProp->kind == ckCHECK)
				cellProp->ioType = ioBOTH;
			if ((cellProp->ioType == ioOUTPUT || cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckBUTTON)
				cellProp->ioType = ioFREE;
			tmps.Format(_T("%d"), cellProp->ioType);
			break;

		case 103:	// cellkind
			if (cellProp->ioType == ioOUTPUT && cellProp->kind == ckCHECK)
				cellProp->kind = ckNORMAL;
			if ((cellProp->ioType == ioOUTPUT || cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckBUTTON)
				cellProp->kind = ckNORMAL;
			tmps.Format(_T("%d"), cellProp->kind);
			break;

		case 104:	// cellsize
			tmps.Format(_T("%d"), cellProp->size);
			break;

		case 105:	// cellwidth
			tmps.Format(_T("%d"), cellProp->width);
			break;

		case 106:	// cellhead
			tmps = cellProp->headText;
			break;

		case 107:	// cellalignH
			tmps.Format(_T("%d"), cellProp->alignmentHead);
			break;

		case 108:	// cellalignC
			tmps.Format(_T("%d"), cellProp->alignmentText);
			break;

		case 109:	// cellhpRGB
			tmps.Format(_T("%d"), cellProp->headPaintColor);
			break;

		case 110:	// cellhtRGB
			tmps.Format(_T("%d"), cellProp->headTextColor);
			break;

		case 111:	// cellpRGB
			tmps.Format(_T("%d"), cellProp->paintColor);
			break;

		case 112:	// celltRGB
			tmps.Format(_T("%d"), cellProp->textColor);
			break;

		case 113:	// cellvals
			tmps.Format(_T("%d"), cellProp->offsets);
			break;
		case 114:	// cellstr
			if (cellProp->kind == ckCHECK)
				tmps = cellProp->strings;
			break;

		case 115:	// ausX // cellauxs
			tmps = cellProp->dominoName;
			break;

		case 116:	// cellrefs
			tmps = cellProp->referenceName;
			break;

		case 117:	// cellproperties
			{
				CString text, str = getValue2(row);
				int	nCnt = 1;

				tmps = _T("0");
				while (!str.IsEmpty())
				{
					text = parse(str, _T(";"));
					if (hex2Int(text) & cellProp->styles)
						tmps.Format(_T("%d"), nCnt);
					nCnt++;
				}
			}
			break;

		case 118:	// cellattributes
			if (hex2Int(getValue2(row)) & cellProp->attributes)
				tmps = _T("1");
			else
				tmps = _T("0");
			break;

		case 120:	// cellstr2
			tmps = cellProp->strings;
			break;

		case 121:	// celldat
			tmps = cellProp->displays;
			break;

		case 122:	// cellkeys
			tmps.Format(_T("%d"), cellProp->entryCombo);
			break;

		case 123:	// cellvals2
			tmps.Format(_T("%ld"), cellProp->keyParam);
			break;

		case 124:	// cellalignI
			tmps.Format(_T("%d"), cellProp->alignmentImage);
			break;

		case 125:	// cellstr3
			tmps = cellProp->images;
			break;

		case 126:	// celltxt
			if (cellProp->kind == ckBUTTON)
				tmps = cellProp->displays;
			break;

		case 127:	// cellisHint
			tmps.Format(_T("%d"), cellProp->typeHint);
			break;

		case 128:	// cellonHint
			tmps = cellProp->hintText;
			break;

		case 129:	// cellattr2
			if ((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL)	// edit
			{
				if (cellProp->attributes & faDATE)
					tmps.Format(_T("%d"), getTypeOfDateFormat(cellProp->auxAttributes));
				else
					tmps.Format(_T("%d"), getTypeOfDateFormat(0xffffffff));
				break;
			}
			tmps.Format(_T("%d"), getTypeOfDateFormat(0xffffffff));
			break;
		case 130:	// formula
			tmps = cellProp->formula;
			break;
		case 131:	// rts
			tmps = cellProp->flashKeys;
			break;
		}
	}
	return tmps;
}

void CMultiPropGrid::setData(CString str, long col, long row)
{
	if (col < 0 || row < 0)
		return;

	int	pos = -1;
	CString	tmps, text, retv = _T("");

	if (col < allCols)
		tmps = m_writeInfo.GetAt(col);
	else
		return;

	for (int ii = 0; ii < row; ii++)
	{
		text = parse(tmps, _T("\t"));
		retv += text;
		retv += _T("\t");
	}
	retv += str;
	retv += _T("\t");
	parse(tmps, _T("\t"));
	retv += tmps;
	m_writeInfo.SetAt(col, retv);
}

CString CMultiPropGrid::getData(int col, int row)
{
	if (col < 0 || row < 0)
		return _T("");

	CString	text, tmps = (col < allCols ? m_writeInfo.GetAt(col) : _T(""));
	if (tmps.IsEmpty())
		return _T("");
	
	text = parse(tmps, _T("\t"));
	for (int ii = 0; ii < row; ii++)
		text = parse(tmps, _T("\t"));

	return text;
}

void CMultiPropGrid::setLikeCell(int nCol, int nSrcCol)
{
	if (getType(m_selectedPnt.y) != 4)
	{
		CString	tmps = getData(nSrcCol, m_selectedPnt.y);
		setData(tmps, nCol, m_selectedPnt.y);
		saveData(m_index, nCol, m_selectedPnt.y, false);
		invalidateRect2(CPoint(nCol, m_selectedPnt.y));
	}
	else
	{
		switch (atoi(getValue2(m_selectedPnt.y)))
		{
		case 1004:	// attributes
			{
				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(nSrcCol);

				CAttrDlg dlg(this, Prop->kind, cellProp->charType, cellProp->ioType,
					cellProp->attributes, cellProp->auxAttributes);
				
				cellOb* dstProp = (cellOb*)Prop->cellProperties->GetAt(nCol);
				dstProp->attributes = dlg.m_attributes;
				dstProp->auxAttributes = dlg.m_auxAttributes;
			}
			break;

		case 1007:	// edit format
			{

				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();

				Prop->CellSelected = m_inputPnt.x;
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(nSrcCol);

				CCellEdit dlg(this, fmGRID, (BYTE)cellProp->charType,
				(BYTE)cellProp->whatEdit,
				(DWORD)cellProp->auxValues,
				cellProp->editFormat,
				cellProp->name,
				cellProp->attributes,
				cellProp->size);

				

				cellOb* dstProp = (cellOb*)Prop->cellProperties->GetAt(nCol);
				
				dstProp->attributes = dlg.m_attributes;
				dstProp->whatEdit = dlg.m_whatEdit;
				dstProp->auxValues = dlg.m_auxValues;
				dstProp->editFormat = dlg.m_editFormat;
			}
			break;
		}
	}
}

CString CMultiPropGrid::getReservedVal1(CString str)
{
	CString	field = _T("00$bool\t01$fontStyle\t02$alignment\t03$typeHint\t");
	int	pos = field.Find(str);

	if (pos == -1)
		return _T("");

	switch (atoi(field.Mid(pos - 2, 2)))
	{
	case 0:	// $bool
		return _T("0-False;1-True;");
	case 1:	// $fontStyle
		return _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
	case 2:	// $alignment
		return _T("0-Center;1-Left;2-Right;");
	case 3:	// $typeHint
		return _T("0-None;1-String;2-Reference;");
	}
	return _T("");
}

CString CMultiPropGrid::getFormM(int pos)
{
	if (pos < 0)
		return _T("");

	CProArr* proArr;
	if (m_proMap.Lookup(m_gridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).formM;
	}
	return _T("");
}

CString CMultiPropGrid::getValue2(int pos)
{
	if (pos < 0)
		return _T("");

	CProArr* proArr;
	if (m_proMap.Lookup(m_gridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).value2;
	}
	return _T("");
}

CString CMultiPropGrid::getValue1(int pos)
{
	if (pos < 0)
		return _T("");

	CProArr* proArr;
	if (m_proMap.Lookup(m_gridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).value1;
	}
	return _T("");
}

int CMultiPropGrid::getType(int pos)
{
	if (pos < 0)
		return 0;

	CProArr* proArr;
	if (m_proMap.Lookup(m_gridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).type;
	}
	return 0;
}

CString CMultiPropGrid::getHeadCaption(int pos)
{
	if (pos < 0)
		return _T("");

	CProArr* proArr;
	if (m_proMap.Lookup(m_gridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).headCaption;
	}
	return _T("");
}

int CMultiPropGrid::getDefaultValue(CString sSection, CString sKey, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\tab\\awDefault.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sSection, sKey, nDefault, path);
}

bool CMultiPropGrid::checkReservedWord(CString name)
{
	CString	sKeywords = szKeywords;

	name.MakeLower();
	if (sKeywords.Find(_T(" ") + name + _T(" ")) >= 0)
		return true;

	sKeywords = szConstants;
	if (sKeywords.Find(_T(" ") + name + _T(" ")) >= 0)
		return true;

	return false;
}

CString CMultiPropGrid::getInitSymbol(CString sSymbol)
{
	CString	sym, symList;
	int	initN;
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
	CformProp*	Prop = ObjLoad->getProp();

	if (Prop->cellProperties == NULL)
		return sSymbol;

	for (int kk = 0; kk < Prop->cellProperties->GetCount(); kk++)
	{
		cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(kk);
		if (!cellProp->name.IsEmpty())
		{
			symList += cellProp->name;
			symList += _T('\t');
		}
	}

	initN = 0;
	symList.MakeUpper();
	for ( ;; )
	{
		sym.Format(_T("%s%d"), sSymbol, initN);
		sym.MakeUpper();
		if (symList.Find(sym + _T("\t")) == -1)
			break;
		else
			initN++;
	}
	
	sym.Format(_T("%s%d"), sSymbol, initN);
	return sym;
}

void CMultiPropGrid::addCol(CString str)
{
	m_writeInfo.Add(str);
}

void CMultiPropGrid::setInputState()
{
	CRect	cellRc;

	cellRc.SetRect(m_gridAttr.HeadSz.cx + (m_selectedPnt.x - m_startPnt.x) * m_gridAttr.CellSz.cx + 1, 
			m_gridAttr.HeadSz.cy + (m_selectedPnt.y - m_startPnt.y) * m_gridAttr.CellSz.cy + 1,
			m_gridAttr.HeadSz.cx + (m_selectedPnt.x - m_startPnt.x + 1) * m_gridAttr.CellSz.cx, 
			m_gridAttr.HeadSz.cy + (m_selectedPnt.y - m_startPnt.y + 1) * m_gridAttr.CellSz.cy);

	if (m_combo || m_edit)
		EraseCtrls();

	m_beforeVal = getData(m_selectedPnt.x, m_selectedPnt.y);
	switch (getType(m_selectedPnt.y))
	{
	case 1:		// edit
		{
			CString	data;
			int	nLimitText = 0;
			DWORD	dwStyle = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL;

			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
			data = getData(m_selectedPnt.x, m_selectedPnt.y);
			if (!getValue1(m_selectedPnt.y).IsEmpty() && getValue1(m_selectedPnt.y).GetAt(0) == _T('2'))
				dwStyle |= ES_NUMBER;

			m_edit = new CEdit;
			m_edit->Create(dwStyle, cellRc, this, ID_EDITCTL);
			m_edit->SetFont(&m_font);
			if (!getValue1(m_selectedPnt.y).IsEmpty() && getValue1(m_selectedPnt.y).GetAt(0) == _T('1'))
				m_edit->EnableWindow(FALSE);

			if (!getFormM(m_selectedPnt.y).CompareNoCase(_T("cellstrings")))
			{
				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();

				Prop->CellSelected = m_selectedPnt.x;
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
				if (cellProp->kind != ckCHECK)
					m_edit->EnableWindow(FALSE);
			}

			if (!getFormM(m_selectedPnt.y).CompareNoCase(_T("celldisplays")))
			{
				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();

				Prop->CellSelected = m_selectedPnt.x;
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
				if (cellProp->kind != ckBUTTON)
					m_edit->EnableWindow(FALSE);
			}
			
			if (getValue1(m_selectedPnt.y).GetLength() >= 3)
				nLimitText = atoi(getValue1(m_selectedPnt.y).Mid(1, 3));
			if (nLimitText)
				m_edit->LimitText(nLimitText);
			m_edit->SetWindowText(data);
			m_edit->SetSel(0, -1);
			if (!m_keyDown)
			{
				CPoint	pnt;

				GetCursorPos(&pnt);
				m_edit->ScreenToClient(&pnt);
				m_edit->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pnt.x, pnt.y));
			}
			m_edit->SetFocus();
			m_inputPnt = m_selectedPnt;
			m_keyDown = false;
		}
		break;

	case 2: // edit combo
		{
			CString data = getData(m_selectedPnt.x, m_selectedPnt.y);
			cellRc.DeflateRect(0, 0, 20, 0);

			m_edit = new CEdit;
			m_edit->Create(WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NOHIDESEL | ES_AUTOHSCROLL, cellRc, this, ID_EDITCTL);
			m_edit->SetFont(&m_font);
			if (!getValue1(m_selectedPnt.y).IsEmpty() && getValue1(m_selectedPnt.y).GetAt(0) == _T('1'))
				m_edit->EnableWindow(FALSE);
			m_edit->SetWindowText(data);
			if (m_edit->IsWindowEnabled())
				m_edit->SetSel(0, -1);

			m_button = new CMPropGridButton();
			if (cellRc.Width() > 20)
				cellRc.DeflateRect(cellRc.Width(), 0, -20, 0);
			m_button->Create(NULL, _T("..."), WS_CHILD | WS_VISIBLE, cellRc, this, ID_BUTTONCTL);
			m_button->SetFont(&m_font);
			m_button->SetFocus();

			m_inputPnt = m_selectedPnt;
			if (atoi(getValue2(m_inputPnt.y)) == 1002)
			{
				COLORREF rgbValue = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, atoi(getData(m_inputPnt.x, m_inputPnt.y)));
				m_ctlBkColor  = rgbValue;
				m_ctlBrush.DeleteObject();
				m_ctlBrush.CreateSolidBrush(rgbValue);
			}
			m_edit->SetFocus();
		}
		break;

	case 3:		// combo
		{
			int	sel, iData;
			CString	comboStr, str, data = getData(m_selectedPnt.x, m_selectedPnt.y);

			m_combo = new CMPropGridCombo();
			m_combo->SetFont(&m_font);
			m_combo->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, cellRc, this, ID_COMBOCTL);
			comboStr = getValue1(m_selectedPnt.y);
			if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == _T('$'))
				comboStr = getReservedVal1(comboStr);
			while (!comboStr.IsEmpty())
			{
				str = parse(comboStr, _T(";"));
				iData = atoi(parse(str, _T("-")));
				m_combo->SetItemData(m_combo->AddString(str), iData);
			}

			//조건부
			if (!getFormM(m_selectedPnt.y).CompareNoCase(_T("cellstyles")) && !getValue2(m_selectedPnt.y).CompareNoCase(_T("0x00000008;0x00000010")))
			{
				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_index);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();

				Prop->CellSelected = m_selectedPnt.x;
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
				if (cellProp->kind != ckBUTTON)
					m_combo->EnableWindow(FALSE);
			}
			sel = m_combo->m_inCombo->FindStringExact(0, data);
			if (sel < 0)
				sel = 0;
			if (m_combo->m_inCombo->GetCount())
				m_combo->m_inCombo->SetCurSel(sel);
			m_combo->SetFocus();
			m_inputPnt = m_selectedPnt;
		}
		break;

	case 4:		// only button
		{
			const	MSG*	pmsg;
			m_button = new CMPropGridButton();
			m_button->Create(NULL, _T("..."), WS_CHILD | WS_VISIBLE, cellRc, this, ID_BUTTONCTL);
			m_button->SetFont(&m_font);
			m_button->SetFocus();

			m_inputPnt = m_selectedPnt;
			pmsg = GetCurrentMessage();
			if (pmsg->message == WM_LBUTTONDOWN)
				OnBtnClick();
		}
		break;
	}
}

void CMultiPropGrid::updateCombo()
{
	if (m_combo)
	{
		int	sel = m_combo->m_inCombo->FindStringExact(0, getData(m_inputPnt.x, m_inputPnt.y));
		if (sel < 0)
			sel = 0;
		if (m_combo->m_inCombo->GetCount())
			m_combo->m_inCombo->SetCurSel(sel);
	}
}

int CMultiPropGrid::getVisibleCols()
{
	int	nCols = 0;

	while (m_gridAttr.CellSz.cx * nCols < m_clientRc.Width() - m_gridAttr.HeadSz.cx)
		nCols++;
	return nCols;
}

int CMultiPropGrid::getVisibleRows()
{
	int	nRows = 0;

	while (m_gridAttr.CellSz.cy * nRows < m_clientRc.Height() - m_gridAttr.HeadSz.cy)
		nRows++;
	return nRows;
}


long CMultiPropGrid::getGridWidth()
{
	return m_gridAttr.HeadSz.cx + m_gridAttr.CellSz.cx * m_gridAttr.cols;
}

long CMultiPropGrid::getGridHeight()
{
	return m_gridAttr.HeadSz.cy + m_gridAttr.CellSz.cy * m_gridAttr.rows;
}

void CMultiPropGrid::setFont(LOGFONT* lf, int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	ZeroMemory(lf, sizeof(LOGFONT));
	lf->lfHeight = fsize * 10; 
	lf->lfCharSet        = DEFAULT_CHARSET;
	lf->lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf->lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf->lfQuality        = DEFAULT_QUALITY;
	lf->lfPitchAndFamily = DEFAULT_PITCH;
	lf->lfWeight         = (bBold ? FW_BLACK : FW_NORMAL);
	lf->lfItalic         = bItalic;
}

void CMultiPropGrid::invalidateRect2(CPoint pnt)
{
	CRect	cellRc;

	GetClientRect(&cellRc);
	if (pnt.x >= 0)
		pnt.x -= m_startPnt.x;
	if (pnt.y >= 0)
		pnt.y -= m_startPnt.y;

	cellRc.SetRect(m_gridAttr.HeadSz.cx + pnt.x * m_gridAttr.CellSz.cx, 
			m_gridAttr.HeadSz.cy + pnt.y * m_gridAttr.CellSz.cy,
			m_gridAttr.HeadSz.cx + (pnt.x + 1) * m_gridAttr.CellSz.cx, 
			m_gridAttr.HeadSz.cy + (pnt.y + 1) * m_gridAttr.CellSz.cy);
	if (pnt.x < 0)
	{
		cellRc.left = 0;
		cellRc.right = m_gridAttr.HeadSz.cx;
	}
	if (pnt.y < 0)
	{
		cellRc.top = 0;
		cellRc.bottom = m_gridAttr.HeadSz.cy;
	}
	
	if (pnt.x < -1 || pnt.y < -1)
		return;

	drawBitmap2(pnt.x, pnt.y, cellRc);
	m_invalidate = false;
	InvalidateRect(&cellRc);
}

void CMultiPropGrid::drawCellData(CDC *pDC, CRect rc, CString data, COLORREF bkcolor, COLORREF frcolor, DWORD dwStyle)
{
	pDC->SetTextColor(frcolor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rc, bkcolor);
	rc.DeflateRect(1, 0, 0, 0);
	pDC->DrawText(data, rc, dwStyle | DT_VCENTER | DT_SINGLELINE);
	rc.InflateRect(1, 0, 1, 1);
	pDC->Draw3dRect(rc, m_gridAttr.lineColor, m_gridAttr.lineColor);
}

bool CMultiPropGrid::drawCellData(CDC *pDC, int col, int row, CString data)
{
	CRect	cellRc = m_clientRc;
	COLORREF bkColor = RGB(255, 255, 255);

	cellRc.SetRect(m_gridAttr.HeadSz.cx + col * m_gridAttr.CellSz.cx, 
			m_gridAttr.HeadSz.cy + row * m_gridAttr.CellSz.cy,
			m_gridAttr.HeadSz.cx + (col + 1) * m_gridAttr.CellSz.cx, 
			m_gridAttr.HeadSz.cy + (row + 1) * m_gridAttr.CellSz.cy);

	if (cellRc.top > m_clientRc.Height())
		return false;

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	if (atoi(getValue2(row + m_startPnt.y)) == 1002)
	{
		bkColor = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, atoi(getData(col + m_startPnt.x, row + m_startPnt.y)));
		pDC->FillSolidRect(cellRc, bkColor);
	}
	else
		pDC->FillSolidRect(cellRc, RGB(255, 255, 255));

	cellRc.DeflateRect(1, 1, 0, 0);
	if (getType(row + m_startPnt.y) == 4)
	{
		pDC->DrawFrameControl(cellRc, DFC_BUTTON, DFCS_BUTTONPUSH);
		pDC->DrawText(_T("..."), cellRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (atoi(getValue2(row + m_startPnt.y)) == 1002)
		{
			if ((GetRValue(bkColor) + GetGValue(bkColor) + GetBValue(bkColor)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
		}
		pDC->DrawText(data, cellRc, DT_LEFT);
	}
	cellRc.InflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(cellRc, m_gridAttr.lineColor, m_gridAttr.lineColor);
	return true;
}

void CMultiPropGrid::drawHead(CDC *pDC)
{
	CRect	cellRc;
	CString	data;
	int	value;

	for (int col = 0; col <= allCols; col++)
	{
		if (col == 0)
		{
			data = _T("*");
			cellRc.SetRect(0, 0, m_gridAttr.HeadSz.cx, m_gridAttr.CellSz.cy);
		}
		else
		{
			data.Format(_T("%d"), col + m_startPnt.x);
			cellRc.SetRect(m_gridAttr.HeadSz.cx + (col - 1) * m_gridAttr.CellSz.cx, 0, m_gridAttr.HeadSz.cx + col * m_gridAttr.CellSz.cx, m_gridAttr.CellSz.cy);
		}

		if (m_selectedPnt.y == -1 && col > 0 && m_selectedPnt.x == col + m_startPnt.x - 1)
			drawCellData(pDC, cellRc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT), DT_CENTER);
		else if (m_multiSelMap.Lookup(col + m_startPnt.x - 1, value))
			drawCellData(pDC, cellRc, data, CLR_MULTISEL, RGB(0,0,0), DT_CENTER);
		else
			drawCellData(pDC, cellRc, data, RGB(255, 255, 255), RGB(0,0,0), DT_CENTER);
	}

	for (int ii = 0, row = 1; row < lastRows + 1; row++)
	{
		cellRc.SetRect(0, row * m_gridAttr.CellSz.cy, m_gridAttr.HeadSz.cx, (row + 1) * m_gridAttr.CellSz.cy);
		data = getHeadCaption(ii++ + m_startPnt.y);
		if (!data.IsEmpty())
		{
			if (m_selectedPnt.y + 1== row + m_startPnt.y)
				drawCellData(pDC, cellRc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT));
			else
				drawCellData(pDC, cellRc, data);
		}
	}
}

void CMultiPropGrid::drawGridData(CDC *pDC)
{
	CString	tmps, text;
	CRect	rc;

	for (int row, col = 0; col < allCols; col++)
	{
		tmps = (col + m_startPnt.x < allCols ? m_writeInfo.GetAt(col + m_startPnt.x) : _T(""));
		if (col + m_startPnt.x >= allCols)
		{
			rc.SetRect(m_gridAttr.HeadSz.cx + col * m_gridAttr.CellSz.cx, 0, m_clientRc.right, m_clientRc.bottom);
			drawCellData(pDC, rc, _T(""));
			break;
		}

		text = parse(tmps, _T("\t"));
		for (row = 0; row < m_startPnt.y; row++)
			parse(tmps, _T("\t"));
		for (row = 0; row < lastRows; row++)
		{
			text = parse(tmps, _T("\t"));
			if (!drawCellData(pDC, col, row, getData(col + m_startPnt.x, row + m_startPnt.y)))
				break;
		}
	}
}

void CMultiPropGrid::drawBitmap(CDC *pDC)
{
	CDC	memDC;
	CBitmap* pOldBitmap;
	CFont	font, *pFont;
	LOGFONT	lf;

	calScrollSz();
	if (m_bitmap)
		delete m_bitmap;

	m_bitmap = new CBitmap();
	memDC.CreateCompatibleDC(pDC);
	m_bitmap->CreateCompatibleBitmap(pDC, m_clientRc.Width(), m_clientRc.Height());
	pOldBitmap = (CBitmap *)memDC.SelectObject(m_bitmap);

	setFont(&lf, 9);
	font.CreatePointFontIndirect(&lf, &memDC);
	pFont = (CFont *)memDC.SelectObject(&font);

	memDC.FillSolidRect(m_clientRc, RGB(255, 255, 255));
	drawHead(&memDC);
	drawGridData(&memDC);

	memDC.SelectObject(pFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	m_invalidate = false;
}

void CMultiPropGrid::drawBitmap2(int col, int row, CRect rc)
{
	CDC	memDC, *pDC = GetDC();
	CBitmap* pOldBitmap;
	CFont	font, *pFont;
	LOGFONT	lf;

	memDC.CreateCompatibleDC(pDC);
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_bitmap);

	setFont(&lf, 9);
	font.CreatePointFontIndirect(&lf, &memDC);
	pFont = (CFont*)memDC.SelectObject(&font);

	if (col >= 0 && row >= 0)
		drawCellData(&memDC, col, row, getData(col + m_startPnt.x, row + m_startPnt.y));
	else
	{
		CString	data;
		int	value;

		if (col >= 0)
		{
			data.Format(_T("%d"), col + m_startPnt.x + 1);
			if (m_selectedPnt.y == -1 && m_selectedPnt.x == col + m_startPnt.x)
				drawCellData(&memDC, rc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT), DT_CENTER);
			else if (m_multiSelMap.Lookup(col + m_startPnt.x, value))
				drawCellData(&memDC, rc, data, CLR_MULTISEL, RGB(0,0,0), DT_CENTER);
			else
				drawCellData(&memDC, rc, data, RGB(255, 255, 255), RGB(0,0,0), DT_CENTER);
		}
		
		if (row >= 0)
		{
			data = getHeadCaption(row + m_startPnt.y);
			if (!data.IsEmpty())
			{
				if (m_selectedPnt.y == row + m_startPnt.y)
					drawCellData(&memDC, rc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT));
				else
					drawCellData(&memDC, rc, data);
			}
		}
	}

	memDC.SelectObject(pFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

void CMultiPropGrid::checkGridMultiLine(int idx)
{
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = ObjLoad->getProp();

	if (( Prop->kind == fmGRID || Prop->kind == fmGRIDEX ) && (Prop->auxAttributes & gaMULTI))
	{
		if (Prop->auxAttributes & gaMERGE)
		{
			Prop->auxAttributes &= ~gaMULTI;
			return;
		}

		CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
		CformProp*	Prop = (CformProp*)ObjLoad->getProp();
		if (Prop->cellProperties != NULL)
		{
			int	line, colN;
			line = colN = 0;
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				colN++;
				cellOb* cellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				cellProp->lineN = (BYTE)line;
				if (cellProp->attributes & faWRAP)
					line++;
			}
		}
	}
}

void CMultiPropGrid::adjustScroll()
{
	if (m_gridAttr.rows < 0 || m_gridAttr.cols < 0)
		return;

	if (m_selectedPnt.x + 1 > m_startPnt.x + getVisibleCols())
	{
		OnHScroll(SB_LINEDOWN, 0, NULL);
		m_invalidate = true;
		Invalidate();
	}
	else if (m_selectedPnt.x < m_startPnt.x)
	{
		OnHScroll(SB_THUMBTRACK, 0, NULL);
		m_invalidate = true;
		Invalidate();
	}

	while (m_startPnt.y < m_gridAttr.rows - 1 && m_selectedPnt.y + 2 > m_startPnt.y + getVisibleRows())
		OnVScroll(SB_LINEDOWN, 0, NULL);

	while (m_startPnt.y > 0 && m_selectedPnt.y < m_startPnt.y)
		OnVScroll(SB_LINEUP, 0, NULL);

	m_invalidate = true;
	Invalidate();
}

void CMultiPropGrid::calScrollSz()
{
	CRect	rc = m_clientRc;

	if (getGridHeight() > rc.Height())
	{
		SCROLLINFO info;

		GetScrollInfo(SB_VERT, &info);
		info.cbSize    = sizeof(SCROLLINFO);
		info.fMask     = SIF_ALL;
		info.nMin      = 0;
		info.nMax      = m_gridAttr.rows - getVisibleRows() + 1;
		info.nPage     = 1;
		info.nTrackPos = 1;
		info.nPos      = m_startPnt.y;
		SetScrollInfo(SB_VERT, &info);
		ShowScrollBar(SB_VERT);
		SetScrollRange(SB_VERT, 0, m_gridAttr.rows - getVisibleRows() + 1);
	}
	else if (getGridHeight() <= rc.Height())
		ShowScrollBar(SB_VERT, FALSE);

	if (getGridWidth() > rc.Width())
	{
		SCROLLINFO info;

		GetScrollInfo(SB_HORZ, &info);
		info.cbSize    = sizeof(SCROLLINFO);
		info.fMask     = SIF_ALL;
		info.nMin      = 0; 
		info.nMax      = m_gridAttr.cols - getVisibleCols() + 1;
		info.nPage     = 1;
		info.nTrackPos = 1;
		info.nPos      = m_startPnt.x;
		if (info.nPos > info.nMax)
			info.nPos = info.nMax;
		SetScrollInfo(SB_HORZ, &info);
		ShowScrollBar(SB_HORZ);
		SetScrollRange(SB_HORZ, 0, m_gridAttr.cols - getVisibleCols() + 1);
	}
	else if (getGridWidth() <= rc.Width())
		ShowScrollBar(SB_HORZ, FALSE);
}

CString CMultiPropGrid::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src.Empty();
		return tmps;
	}
	else
	{
		tmps = src.Left(pos);
		src = src.Mid(pos + sub.GetLength());
		return tmps;
	}
	return _T("");
}

int CMultiPropGrid::hex2Int(CString value)
{
	if (value.IsEmpty())
		return 0;

	struct _hexMap
	{
		TCHAR	chr;
		int	value;
	};
	const int hexMapL = 16;
	_hexMap hexMap[hexMapL] =
	{
		{_T('0'), 0},	{_T('1'), 1},
		{_T('2'), 2},	{_T('3'), 3},
		{_T('4'), 4},	{_T('5'), 5},
		{_T('6'), 6},	{_T('7'), 7},
		{_T('8'), 8},	{_T('9'), 9},
		{_T('A'), 10},	{_T('B'), 11},
		{_T('C'), 12},	{_T('D'), 13},
		{_T('E'), 14},	{_T('F'), 15}
	};
	TCHAR	*mstr, *s;
	int	result;
	bool	firsttime = true;

	value.MakeUpper();
	mstr = (TCHAR *)value.operator LPCSTR();
	s = mstr;
	
	result = 0;
	if (*s == _T('0') && *(s + 1) == _T('X'))
		s += 2;
	while (*s != _T('\0'))
	{
		bool	found = false;
		for (int ii = 0; ii < hexMapL; ii++)
		{
			if (*s == hexMap[ii].chr)
			{
				if (!firsttime)
					result <<= 4;
				result |= hexMap[ii].value;
				found = true;
				break;
			}
		}
		if (!found)
			break;
		s++;
		firsttime = false;
	}
//	free(mstr);
	return result;
}