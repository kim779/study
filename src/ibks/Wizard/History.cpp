// History.cpp : implementation file
//

#include "pch.h"
#include "Wizard.h"
#include "History.h"
#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistory

CHistory::CHistory(CClient* client, CString items)
{
	m_client = client;
	m_guard  = client->m_guard;
	m_items = items;
	m_items.TrimLeft();

	m_tRGB = PALETTERGB(0, 0, 0);
	m_pRGB = PALETTERGB(255, 255, 255);
	m_height = 16;
}

CHistory::~CHistory()
{
	
}


BEGIN_MESSAGE_MAP(CHistory, CListBox)
	//{{AFX_MSG_MAP(CHistory)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
//	ON_WM_DRAWITEM()
//	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistory message handlers

BOOL CHistory::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (WS_BORDER|WS_VSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS);  //test
	return CListBox::PreCreateWindow(cs);
}

int CHistory::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (m_items.IsEmpty())
		return -1;

	lpCreateStruct->style |= (LBS_OWNERDRAWFIXED|LBS_HASSTRINGS); //test
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_items.IsEmpty())
		return -1;

	LOGFONT	lf;

	lf.lfHeight         = 9 * 10;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = FW_NORMAL;
	lf.lfItalic         = false;
	lf.lfUnderline      = false;
	lf.lfStrikeOut      = false;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(lf.lfFaceName, LF_FACESIZE, "±¼¸²Ã¼");

	m_font.DeleteObject();
	m_font.CreatePointFontIndirect(&lf);

	SetFont(&m_font);

	CString	item, text;
	CSize	size = CSize(0, 0);
	CDC*	dc = GetDC();
	CFont* font = (CFont *)dc->SelectObject(&m_font);
	int	width = 0, ndc = dc->SaveDC();

	text = m_items;
	for (int idx; (idx = text.Find('\t')) != -1; )
	{
		item = text.Left(idx++);
		text = text.Mid(idx);
		AddString(item);
		size = dc->GetOutputTextExtent(item);
		width = max(width, size.cx);
	}
	dc->SelectObject(font);
	dc->RestoreDC(ndc);
	ReleaseDC(dc);

	SetCurSel(0);
	SetCaretIndex(0);

	if (m_client->m_status & S_CHILD)
	  SetFocus();

	width = max(lpCreateStruct->cx, width+GetSystemMetrics(SM_CXVSCROLL)+8);
	SetWindowPos(&wndTop, 0, 0, width, lpCreateStruct->cy, SWP_NOMOVE);

	return 0;
}

void CHistory::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDown(nFlags, point);

	BOOL	ous;
	ItemFromPoint(point, ous);
	m_client->History(ous ? false : true);
}

void CHistory::OnMouseMove(UINT nFlags, CPoint point) 
{
	CListBox::OnMouseMove(nFlags, point);

	BOOL	ous;
	int idx = ItemFromPoint(point, ous);

	if (!ous && idx != GetCurSel())
	{
		SetCurSel(idx);
		SetCaretIndex(idx);
	}
}

void CHistory::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case VK_RETURN:
		m_client->History(true);
		break;
	case VK_ESCAPE:
		m_client->History(false);
		break;
	}
}

void CHistory::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
m_slog.Format("[WIZARD][KillFocus][COMBO][%s]<%d>", __FUNCTION__, __LINE__);
OutputDebugString(m_slog);
	m_client->History(false);  
}

bool CHistory::GetItem(CString& item)
{
	int idx = GetCurSel();
	if (idx != LB_ERR)
	{
		GetText(idx, item);
		idx = item.Find(' ');
		if (idx != -1)
			item = item.Left(idx);
		return true;
	}
	return false;
}

void CHistory::FindItem(CString item, bool self)
{
	m_slog.Format("[WIZARD][COMBO][%s]<%d> item=[%s]", __FUNCTION__, __LINE__, item);
	OutputDebugString(m_slog);
	int	find;

	item.TrimLeft();
	if (self)
	{
		find = m_items.Find("  " + item);
		if (find == -1)
		{
			item.MakeUpper();
			find = m_items.Find("  " + item);
		}
	}
	else
	{
		find = m_items.Find(item);
		if (find > 0)
			find = m_items.Find("\t" + item);
	}

	if (find != -1)
	{
		int	pos = 0;

		for (int idx = 0; pos != -1; idx++, pos++)
		{
			pos = m_items.Find('\t', pos);
			if (pos > find)
			{
				SetTopIndex(idx);
				SetCurSel(idx);
				SetCaretIndex(idx);
				return;
			}
		}
	}
}

bool CHistory::OnKey(WPARAM wParam)
{
	int	idx;
	idx = GetCurSel();

	switch (wParam)
	{
	case VK_UP:
		idx--;
		break;
	case VK_DOWN:
		idx++;
		break;
	case VK_HOME:
		idx = 0;
		break;
	case VK_END:
		idx = GetCount()-1;
		break;
	case VK_PRIOR:
		idx = max(0, idx-8);
		break;
	case VK_NEXT:
		idx = min(GetCount()-1, idx+8);
		break;
	case VK_ESCAPE:
		return false;
	default:
		return true;
	}

	if (idx >= 0 && idx < GetCount())
	{
		SetCurSel(idx);
		SetCaretIndex(idx);
	}
	return true;
}

void CHistory::OnWheel(int delta)
{
	int	idx;
	idx = GetCurSel();

	idx -= delta;
	idx = min(max(0, idx), GetCount()-1);
	SetCurSel(idx);
	SetCaretIndex(idx);
}

void CHistory::DrawItemX(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC	dc;

	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	CFont* font = dc.SelectObject(&m_font);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		dc.SetTextColor(m_tRGB);
		dc.SetBkColor(m_pRGB);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, m_pRGB);
	}

	if (lpDrawItemStruct->itemID != -1)
	{
		CString	text;
		CRect	rc;

		rc = lpDrawItemStruct->rcItem;
		GetText(lpDrawItemStruct->itemID, text);

		dc.SetBkMode(TRANSPARENT);
		rc.left += 3;
		text.Replace("&", "&&");
		dc.DrawText(text, rc, DT_VCENTER|DT_SINGLELINE); 
	}

	dc.SelectObject(font);
	dc.Detach();
}

void CHistory::MeasureItemX(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (lpMeasureItemStruct->CtlType == ODT_LISTBOX)
		lpMeasureItemStruct->itemHeight = m_height;
}

