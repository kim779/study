// History.cpp : implementation file
//

#include "pch.h"
//#include "Wizard.h"
#include "History.h"
//#include "Client.h"

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
	//m_guard  = client->m_guard;
	m_items = items;
	m_items.TrimLeft();

	m_tRGB = PALETTERGB(0, 0, 0);
	m_pRGB = PALETTERGB(255, 255, 255);
	m_height = 30;
}

CHistory::~CHistory()
{
	GetParent()->PostMessage(WM_CLOSE, 0, 0);
}

BEGIN_MESSAGE_MAP(CHistory, CListBox)
	//{{AFX_MSG_MAP(CHistory)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_RBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistory message handlers

BOOL CHistory::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (WS_BORDER | WS_VSCROLL | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS);  //test
	return CListBox::PreCreateWindow(cs);
}

void CHistory::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);

	BOOL	ous;
	ItemFromPoint(point, ous);

	//test
#define ID_MENU_ITEM1 1001
#define ID_MENU_ITEM2 1002
#define ID_NEW_MENU_ITEM 1003


	//m_client->History(ous ? false : true);
}

UINT th_popmenu(LPVOID lparam)
{
	CHistory* pwnd = (CHistory*)lparam;
	CPoint pt, wpt;
	CString slog;
	while (1)
	{
		if (pwnd->m_bThreadEnd)
			return 0;
		//GetCursorPos(&pt);
		
		pwnd->ScreenToClient(&pt);
		BOOL	ous;
		int idx = pwnd->ItemFromPoint(pt, ous);
		slog.Format("\r\n[menu]@@@@@@@@@@@@@@@@@@@@@@@@idx=[%d]   m_isel=[%d]  [%s]", 
			idx, pwnd->m_isel, pwnd->m_bShowMenu== true? "show = TRUE":"show FALSE");
		OutputDebugString(slog);
		if (pwnd->m_bShowMenu)
		{
			//pwnd->m_isel = idx;
			pwnd->m_bShowMenu = false;
			pwnd->ClientToScreen(&wpt);
			//pwnd->ShowMenu(pwnd->m_ptmouse);
			pwnd->PostMessage(WM_USER, 1, 0);
		}
		Sleep(1000);
	}
	return 0;
}

LRESULT CHistory::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case 1:
		{
			ShowMenu(m_ptmouse);
		}
		break;
	}
	return 0;
}

void CHistory::AdjustPopUpPos(CPoint& point)
{
	CRect prRect{};
	GetParent()->GetWindowRect(prRect);
	point.x = prRect.right;
}

void CHistory::ShowMenu(CPoint point)
{
CString slog;
slog.Format("\r\n[menu]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ShowMenu[%d][%d]", point.x, point.y);
OutputDebugString(slog);

	if (m_menu.GetSafeHmenu())
	{
		m_menu.DestroyMenu();
		m_menu.CreatePopupMenu();
	}
	else
		m_menu.CreatePopupMenu();

	// 기존 메뉴 항목 추가
	m_menu.AppendMenu(MF_STRING, ID_MENU_ITEM1, _T("기존 항목 1"));
	m_menu.AppendMenu(MF_STRING, ID_MENU_ITEM2, _T("기존 항목 2"));

	// 새 메뉴 항목 추가
	m_menu.AppendMenu(MF_SEPARATOR); // 구분선
	m_menu.AppendMenu(MF_STRING, ID_NEW_MENU_ITEM, _T("새 항목"));

	// 메뉴 표시
	/*CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);*/
	AdjustPopUpPos(point);
	m_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CHistory::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint wpt;
	wpt = point;
	ClientToScreen(&wpt);
	ShowMenu(wpt);
	CListBox::OnRButtonUp(nFlags, point);
}

#define TM_POPMENU 99
void CHistory::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_ptmouse = point;
	ClientToScreen(&m_ptmouse);


	CString slog;
	CPoint wpt;
	wpt = point;
	ClientToScreen(&wpt);
	slog.Format("\r\n[menu]OnMouseMove@@@@@@@@@@@@@@@@@@@@@@@@[%d][%d]    [%d][%d]", point.x, point.y , wpt.x, wpt.y);
	OutputDebugString(slog);

	CListBox::OnMouseMove(nFlags, point);

	BOOL	ous;
	int idx = ItemFromPoint(point, ous);

	slog.Format("\r\n onMouseMove-----------------------------%d", idx);
	OutputDebugString(slog);

	if (!ous && idx != GetCurSel())
	{
		m_isel = idx;
		SetCurSel(idx);
		SetCaretIndex(idx);

			m_bTracking = TRUE;
			m_lastPoint = point;
			if (!m_bShowMenu)
				m_bShowMenu = true;
		
	}
}

void CHistory::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (TM_POPMENU)
	{
	case TM_POPMENU:
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		BOOL	ous;
		int idx = ItemFromPoint(point, ous);

		CString slog;
		slog.Format("\r\n[menu] -----------------------------[%d] [%d]---------", m_isel ,idx);
	//OutputDebugString(slog);

		// 마우스가 이동한 경우 메뉴를 닫고 새로 표시
		if (m_isel != idx)
		{
			m_isel = idx;
			m_menu.DestroyMenu();
			m_bMenuVisible = FALSE;
			KillTimer(TM_POPMENU);

	
		}
	}
	break;
	}
	CListBox::OnTimer(nIDEvent);
}

void CHistory::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case VK_RETURN:
	//	m_client->History(true);
		break;
	case VK_ESCAPE:
		//m_client->History(false);
		break;
	}
}

void CHistory::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
	SendMessage(WM_CLOSE, 0, 0);

	//m_client->History(false);  //test  WS_POPUP으로 하면 죽는다
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

void CHistory::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
		dc.DrawText(text, rc, DT_VCENTER | DT_SINGLELINE);
	}

	dc.SelectObject(font);
	dc.Detach();

}

void CHistory::MeasureItem(LPMEASUREITEMSTRUCT pMeasureItemStruct)
{
	if (pMeasureItemStruct->CtlType == ODT_LISTBOX)
		pMeasureItemStruct->itemHeight = m_height;
}

BOOL CHistory::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CListBox::OnNotify(wParam, lParam, pResult);
}

void CHistory::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bThreadEnd = TRUE;
	if (th_popmenu)
		TerminateThread(th_popmenu, 0);
	GetParent()->PostMessage(WM_CLOSE, 0, 0);
	CListBox::OnClose();
}

int CHistory::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (m_items.IsEmpty())
		return -1;

	lpCreateStruct->style |= (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS); //test
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_items.IsEmpty())
		return -1;

	LOGFONT	lf;

	lf.lfHeight = 9 * 10;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(lf.lfFaceName, LF_FACESIZE, "굴림체");

	m_font.DeleteObject();
	m_font.CreatePointFontIndirect(&lf);

	SetFont(&m_font);

	CString	item, text;
	CSize	size = CSize(0, 0);
	CDC* dc = GetDC();
	CFont* font = (CFont*)dc->SelectObject(&m_font);
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

	//if (m_client->m_status & S_CHILD)
		//  SetFocus();

	width = max(lpCreateStruct->cx, width + GetSystemMetrics(SM_CXVSCROLL) + 8);
	SetWindowPos(&wndTop, 0, 0, width, lpCreateStruct->cy, SWP_NOMOVE);
	m_isel = -1;
	AfxBeginThread(th_popmenu, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	return 0;
}

void CHistory::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
		dc.DrawText(text, rc, DT_VCENTER | DT_SINGLELINE);
	}

	dc.SelectObject(font);
	dc.Detach();

	CListBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CHistory::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CListBox::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}