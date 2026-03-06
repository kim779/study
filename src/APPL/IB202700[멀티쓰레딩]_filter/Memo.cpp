// Memo.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "Memo.h"
#include "sharemsg.h"
#include "interSt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int headerH = 20, dataH = 20;
const int titleH = 18, tabW = 150, gap1 = 1, exitW = 14;

#define TM_RECEIVEMEMO             9996

/////////////////////////////////////////////////////////////////////////////
// CEditX

CEditX::CEditX()
{
}

CEditX::~CEditX()
{
}


BEGIN_MESSAGE_MAP(CEditX, CEdit)
	//{{AFX_MSG_MAP(CEditX)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditX message handlers
BOOL CEditX::Create(CWnd* parent, UINT nID)
{
	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));

	constexpr DWORD	dwStyle = WS_CHILD|WS_VISIBLE|ES_LEFT|ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE;
	return CEdit::Create(dwStyle, CRect(), parent, nID);
}

HBRUSH CEditX::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkColor(GetSysColor(COLOR_INFOBK));

	return m_brush;
}


/////////////////////////////////////////////////////////////////////////////
// CMemo

CMemo::CMemo(CString code, CString jnam, CString root, CString name) :
_pApp(dynamic_cast<CIB202700App*>(AfxGetApp()))
{
	m_edit = nullptr;

	m_code = code;
	m_root = root;
	m_name = name;

	m_jnam.Format("%s 메모", jnam);
}

CMemo::~CMemo()
{
}


BEGIN_MESSAGE_MAP(CMemo, CWnd)
	//{{AFX_MSG_MAP(CMemo)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMemo message handlers
BOOL CMemo::createMemo(CWnd* parent, CWnd* sendW, CWnd* mainW, CRect rect)
{
	m_parent = parent;
	m_sendW  = sendW;
	m_pMainWnd = mainW;

	CString clsName = AfxRegisterWndClass(0);
	constexpr UINT	dwStyle = WS_POPUP|WS_BORDER;
	constexpr UINT	dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_DLGMODALFRAME;

	if (!CreateEx(dwExStyle, clsName, nullptr, dwStyle, rect, nullptr, NULL, nullptr))
	{
//		TRACE("Create memo error....!!\n");
		return FALSE;
	}

	return TRUE;
}

int CMemo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_font = getAxFont("굴림체", 9, 0);
	m_edit = std::make_unique<CEditX>();
	m_edit->Create(this, 100); m_edit->SetFont(m_font);

	m_bMemoFile = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOFILEPROCESSCHECK, 0), 0);
	return 0;
}

void CMemo::ShowMemo(CString code, CString name, CRect rect)
{
	m_edit->SetWindowText("");
	m_code = code;
	m_jnam = name;
	CClientDC dc(this);
	drawCaption(&dc);
	_pApp->setMainWnd(m_pMainWnd);
	_pApp->getMemo(code);
	openMemo(code);
	MoveWindow(rect, FALSE);
	if (GetSafeHwnd())
		ShowWindow(SW_SHOW);
}

void CMemo::OnSetFocus(CWnd* pOldWnd)
{
	MSG msg{};
	m_edit->SetFocus();
	for(m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (!::IsWindowVisible(msg.hwnd) && GetSafeHwnd() == msg.hwnd)
			{
				m_fDone = TRUE; 
				::PostMessage(msg.hwnd, WM_LBUTTONDOWN, 0, 0);
				break;
			}

			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) ||
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && (msg.hwnd != this->m_hWnd && msg.hwnd != m_edit->m_hWnd)) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				if ((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && (GetSafeHwnd() == msg.hwnd))
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = TRUE;
				break;
			}

			if (/*true ||*/ !IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}
	ShowWindow(SW_HIDE);
	if (!m_code.IsEmpty())
	{
		saveMemo(m_code);		
		m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 1), (LPARAM)m_code.GetBuffer(0));
	}
	CWnd::OnSetFocus(pOldWnd);
}

void CMemo::procedureDeleteMemo(CString code)
{
	code.TrimRight(); code.TrimLeft();
	if (!code.IsEmpty())
		deleteMemo(code);
}


BOOL CMemo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint point; GetCursorPos(&point); ScreenToClient(&point);
	CRect rect;   GetClientRect(&rect);

	rect.left = rect.right - 20;
	rect.top  = rect.bottom - 20;

	if (rect.PtInRect(point))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CMemo::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE) PostMessage(WM_USER, 0, 0);
}

void CMemo::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CMemDC	mem(dc, this);
	CDC& memdc = mem.GetDC();
	
	drawCaption(&memdc);
}

void CMemo::drawCaption(CDC *pDC)
{
	CFont* pOldFont{};
	CRect	clientRc, rect, rc;

	GetClientRect(&rc);
	rc.DeflateRect(0, 18, 0, 0);
	pDC->FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));


	GetClientRect(&clientRc); rect = clientRc;
	//pDC->FillSolidRect(rect, GetSysColor(COLOR_INACTIVEBORDER));

	rect.top += gap1; rect.bottom = rect.top + titleH;
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));

	pDC->SetTextColor(RGB(38, 55, 100));
	pOldFont = (CFont*)pDC->SelectObject(getAxFont("굴림체", 9, 3));

	rect.right = rect.left + tabW;
	pDC->DrawText(m_jnam, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	rect.left = rect.right; rect.right = clientRc.right; rect.DeflateRect(0, 2, 2, 2);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_HIGHLIGHT));

	CPen* pOldPen = (CPen*)pDC->SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

	// exit button
	GetClientRect(&rc);
	rc.left = rc.right - exitW;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	pDC->Rectangle(rc);
	rc.DeflateRect(1, 1);
	pDC->FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);
	pDC->MoveTo(rc.left, rc.top);
	pDC->LineTo(rc.right - 1, rc.bottom - 1);
	pDC->MoveTo(rc.right - 1, rc.top);
	pDC->LineTo(rc.left, rc.bottom - 1);

	pDC->SelectObject(pOldPen);
}



void CMemo::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect; GetClientRect(&rect);
	rect.DeflateRect(1, headerH+1, 1, 2);
	m_edit->MoveWindow(&rect);
}

void CMemo::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect	rect, crect; GetClientRect(&crect);

	rect = crect;
	rect.top   += gap1;
	rect.bottom = rect.top + titleH;
	rect.right -= exitW;

	if (rect.PtInRect(point))
	{
		CWnd::OnLButtonDown(nFlags, point);
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		return;
	}

	rect = crect;
	rect.left   = rect.right - exitW;
	rect.top    = rect.top   +  4;
	rect.bottom = rect.top   + 10;
	rect.right  = rect.left  + 10;

	if (rect.PtInRect(point)) ShowWindow(SW_HIDE);

	CWnd::OnLButtonDown(nFlags, point);
}



void CMemo::deleteServerMemo(CString skey)
{
	mid_SDEmemo* pmid = new mid_SDEmemo;
	pmid->chgubn[0] = 'D';

	char* pData = (char*)m_sendW->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		memcpy(pmid->chusid, pData, strlen(pData));

	memcpy(pmid->chcode, (char*)skey.GetBuffer(0), skey.GetLength());
	CSendData sdata;
	char ckey{};
	_trkey* trkey = (struct _trkey*)&ckey;

	trkey->kind = TRKEY_INTER_MEMO_DELETE;

	sdata.SetData("pidomemo", ckey, (char*)pmid, sizeof(mid_SDEmemo), "");
	m_parent->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

void CMemo::deleteMemo(CString skey)
{
	if (skey.IsEmpty() || !m_edit)
		return;
	_pApp->getMemoMap(nullptr, "").erase(skey.GetString());
	deleteServerMemo(skey);
	m_parent->SendMessage(WM_MANAGE, MAKEWPARAM(MK_MEMOUPDATE, 0), (LPARAM)skey.GetBuffer(0));
}

void CMemo::saveMemo(CString skey)
{
	if (skey.IsEmpty() || !m_edit->GetSafeHwnd())		// || m_memoState.IsEmpty())
		return;

	CString str, sSave;
	m_edit->GetWindowText(str);
	str.Trim();

	// 지우지 안기로 결정...
	if (str.IsEmpty())
	{
	 //	deleteMemo(skey);
	 	return;
	}	

	char ckey{};
	_trkey* trkey = (struct _trkey*)&ckey;
	trkey->kind = TRKEY_INTER_MEMO_INSERT;	

	char type = 'I';
	auto& map = _pApp->getMemoMap(this, skey);
	auto&[itmap, inserted] = map.emplace(skey.GetString(), str);		
	if (!inserted && !str.IsEmpty())
	{	
		if (str.Compare(itmap->second) == 0)
			return; 		
		
		itmap->second = str;	
		type = 'U';
		trkey->kind = TRKEY_INTER_MEMO_UPDATE;	
	}


	CString user = (char*)m_sendW->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);	
	std::unique_ptr<mid_memo> pmid = std::make_unique<mid_memo>(type, user, skey, str);	
	
	CSendData sdata;	
	if (type != 'S')
		sdata.SetData("pidomemo", ckey, (char*)pmid.get(), sizeof(mid_memo), "");
	
	m_parent->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);

}

CPen* CMemo::getAxPen(COLORREF clr, int width, int style)
{
	_penR penR;
	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_sendW->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CMemo::getAxBrush(COLORREF clr)
{
	return (CBrush*) m_sendW->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CFont* CMemo::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch(style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_sendW->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMemo::OnDestroy()
{
	KillTimer(TM_RECEIVEMEMO);
	CWnd::OnDestroy();
}

void CMemo::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default	
	if (nIDEvent == TM_RECEIVEMEMO)
	{
		if (openMemo(m_code))
			KillTimer(TM_RECEIVEMEMO);
	}

	CWnd::OnTimer(nIDEvent);
}

bool CMemo::openMemo(CString sCode)
{
	// TODO: Implement the logic to open a memo by its code	
	if (sCode.IsEmpty())	
		return true;

	auto& memoMap = _pApp->getMemoMap(this, sCode);
	auto it = memoMap.find(sCode.GetString());
	if (it != memoMap.end())
	{
		m_code = sCode;		
		CString stmp = it->second;
		m_edit->SetWindowText(stmp);
		m_edit->SetSel(stmp.GetLength(), stmp.GetLength());
		return true;
	}
	else 
	{	
		SetTimer(TM_RECEIVEMEMO, 50, nullptr);
	}
	return false;
}