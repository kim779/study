// CodeList.cpp : implementation file
//

#include "stdafx.h"
#include "CodeList.h"
#include "fxcodectrl.h"
#include "ControlWnd.h"
#include "../../H/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* constClassName = "CodeListClass";

#define ROW_HEIGHT 15
#define BUTTON_HEIGHT 20
#define GAP 2

#define IDC_BUTTON_ALLDELETE    0x01
#define IDC_BUTTON_DELETE	0x02

extern "C" __declspec(dllexport) void InstallMouseHook(HWND hWnd, HINSTANCE hInst);  //훅 프로시저를 설치하는 함수
extern "C" __declspec(dllexport) void UninstallMouseHook(HWND hWnd);		     //메모리에 설치된 훅 프로시저를 해제하는 함수

HINSTANCE	hModule	   =	NULL;
HHOOK           hMouseHook =	NULL;
HWND            hWndRecv   =	NULL;        //어플리케이션 핸들 저장 변수

#define WM_MOUSEHOOK    (WM_USER+100)

CArray<HWND, HWND> g_arr;

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT *pm = NULL;
	if ( nCode > 0 ) 
	{
		pm = (MOUSEHOOKSTRUCT*)lParam;
		switch (wParam)
		{
		case WM_NCMBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_LBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_MBUTTONUP:
		case WM_NCRBUTTONUP:
		case WM_RBUTTONUP:     			
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			return 0;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			{
				for (int ii = 0; ii < g_arr.GetSize(); ii++)
				{
					HWND hWnd = g_arr.GetAt(ii);
					if (IsWindow(hWnd) && pm)	
						SendMessage(hWnd, WM_MOUSEHOOK, wParam, lParam); //전역 후킹 사용
				}
			}
			break;
		}
	}
   
	return CallNextHookEx(hMouseHook,nCode,wParam,lParam);
}

extern "C" __declspec(dllexport) void InstallMouseHook(HWND hWnd, HINSTANCE hInst)
{
	hWndRecv = hWnd;
	hModule = hInst;
	//훅 프로시저의 설치
	if (g_arr.GetSize() == 0)
		hMouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseHookProc, hModule, GetCurrentThreadId());
	
	g_arr.Add(hWnd);
	if ( hMouseHook == NULL ) 
		MessageBox( NULL, "자원 리소스가 부족합니다.", "Error Message", MB_OK );
}

extern "C" __declspec(dllexport) void UninstallMouseHook(HWND hWnd)
{
	//훅 프로시저의 설치 해제
	for (int ii = 0; ii < g_arr.GetSize(); ii++)
	{
		if (hWnd == g_arr.GetAt(ii))
		{		
			g_arr.RemoveAt(ii);	
			break;
		}
	}

	if (g_arr.GetSize() == 0)
		UnhookWindowsHookEx(hMouseHook);
}


/////////////////////////////////////////////////////////////////////////////
// CCodeList

CCodeList::CCodeList(CWnd* pWnd, CWnd* pWizard, CString home)
{
	m_pParent = (CfxCodeCtrl*)pWnd;
	m_pWizard = pWizard;
	m_bTracking = false;
	m_pCtlBtnDeleteAll = nullptr;
	m_pCtlBtnDelete	   = nullptr;
	m_Vscroll = m_Hscroll = 0;
	m_iStart = 0;
	m_iSelRow = -1;
	m_rcSelRow = CRect(0, 0, 0, 0);
	m_bWheel = false;
	m_home = home;
	m_RowHeight = ROW_HEIGHT;
	m_bFocus = TRUE;
	m_pointPrev = CPoint(-1,-1);

	WNDCLASS wndcls{};
	HINSTANCE hInst = AfxGetInstanceHandle();
		
	if (!(::GetClassInfo(hInst, constClassName, &wndcls)))
	{
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = (HBRUSH)NULL_BRUSH;
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = constClassName;
			
		if (!AfxRegisterClass(&wndcls))
		{
		    AfxThrowResourceException();
		    return;
		}
	}
}

CCodeList::~CCodeList()
{
	m_arrData.RemoveAll();
	m_pCtlBtnDeleteAll.reset();
	m_pCtlBtnDelete.reset();
}

BEGIN_MESSAGE_MAP(CCodeList, CWnd)
	//{{AFX_MSG_MAP(CCodeList)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATEAPP()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ALLDELETE, OnAllDeleteBtn)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDeleteBtn)
	ON_MESSAGE(WM_MOUSEHOOK, OnMouseHook)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeList message handlers

int CCodeList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	InstallMouseHook(GetSafeHwnd(),	AfxGetInstanceHandle());
	Init();

	return 0;
}

void CCodeList::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc, rccell;
	xxx::CMemDC memDC(&dc);

	GetClientRect(rc);
	memDC.Rectangle(&rc);
	rccell.SetRect(rc.left + GAP, rc.top, rc.right, m_RowHeight);

	for (int ii = m_iStart; ii < GetVisibleRow(); ii++)
	{
		DrawCell(&memDC, rccell, ii);
		rccell.OffsetRect(0, m_RowHeight);
	}
	// Do not call CWnd::OnPaint() for painting messages
	
	if (m_bFocus)
	{
		if ((CWnd*)GetFocus() != this && (CWnd*)GetFocus() != m_pParent->m_pEdit.get())
		{
			SetFocus();

			m_bFocus = FALSE;
		}
	}
}

void CCodeList::DrawCell(CDC *pDC, CRect rc, int row)
{
	if (row < 0)
		return;

	CString str = m_arrData.GetAt(row);
	CBrush	br;
	CfxCodeCtrl* pWnd = (CfxCodeCtrl*)m_pParent;
	CControlWnd* pCtrl = (CControlWnd*)pWnd->m_pParent;
	CFont*	pFont = ((CControlWnd*)pCtrl)->GetAxFont(((CControlWnd*)pCtrl)->m_Param.fonts, ((CControlWnd*)pCtrl)->m_Param.point, ((CControlWnd*)pCtrl)->m_Param.style);
	CFont*	poldfont = pDC->SelectObject(pFont);//&m_Font);

	pDC->SetBkMode(TRANSPARENT);
	br.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));

	// 선택영역을 그림..
	if (m_iSelRow == row)
	{
		CRect	SelRc;

		CBrush*	poldbr = pDC->SelectObject(&br);
		SelRc.CopyRect(rc);
		SelRc.InflateRect(2, 0);
		pDC->Rectangle(SelRc);
		pDC->SelectObject(poldbr);
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else 
	{
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	}

	rc.DeflateRect(2, 2);
	str.Replace("\t", "   ");

	pDC->DrawText(str, rc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_NOPREFIX);
	pDC->SelectObject(poldfont);
}

void CCodeList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CString szCode;
	szCode = GetItem(GetCurSel());
	
	m_pParent->SetValidCheck(false);
	if (!szCode.IsEmpty())
	m_pParent->SetEditData(szCode); 
	m_pCtlBtnDelete->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	m_pParent->SetVisible(false);

	CWnd::OnLButtonDown(nFlags, point);
}

void CCodeList::AddItem(CString str)
{
	m_arrData.Add(str);
}

void CCodeList::AddSFItem(CString str)
{
	for (int i=0;i<m_arrData.GetSize();i++)
	{
		CString strItem;
		strItem = m_arrData.GetAt(i);
		strItem.TrimRight();
		
		if (strItem == str)
		{
			m_arrData.RemoveAt(i);
		}
	}

	m_arrData.InsertAt(0,str);
}

CString CCodeList::GetItem(int item)
{
	CString szCode;
	if (m_arrData.GetSize() > 0 && m_arrData.GetSize() > item)
		szCode = m_arrData.GetAt(item);
	return Parser(szCode, "\t");
}

CString CCodeList::GetRawItem(int item)
{
	CString rawData;

	if (m_arrData.GetSize() > 0 && m_arrData.GetSize() > item)
		rawData = m_arrData.GetAt(item);

	return rawData;
}

int CCodeList::GetScrollPos32(int nBar, BOOL bGetTrackPos)
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

BOOL CCodeList::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;

	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

void CCodeList::ResetScrollBars()
{
	if (!::IsWindow(GetSafeHwnd())) 
		return;

	CRect rect, visibleRC, virtualRC;

	GetClientRect(rect);
	visibleRC.CopyRect(rect);
	virtualRC.CopyRect(rect);

	virtualRC.bottom = GetVirtualHeight();
	m_Hscroll = m_Vscroll = 0;	// HScroll remove...

	if (visibleRC.Height() < virtualRC.Height())	
		m_Vscroll = virtualRC.Height();
	
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_PAGE|SIF_RANGE;
//	if (m_DIscroll) sinfo.fMask |= SIF_DISABLENOSCROLL;

	sinfo.nPage  = visibleRC.Height() - BUTTON_HEIGHT;
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_Vscroll; 
	SetScrollInfo(SB_VERT, &sinfo, TRUE);

	sinfo.nPage  = visibleRC.Width();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_Hscroll; 
	SetScrollInfo(SB_HORZ, &sinfo, TRUE);
}

void CCodeList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())	
		SetFocus();

	CRect	rect;
	int	scrollPos;

	scrollPos = GetScrollPos32(SB_VERT);
	GetClientRect(&rect);
	//CString tmp; tmp.Format("%d", nSBCode);
	//AfxMessageBox(tmp);
	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos < m_Vscroll) 
		{
			int Yscroll = 0;
			if (m_iStart < (GetCount() - 10))
			{
				Yscroll = m_RowHeight;
				m_iStart++;
			}
			SetScrollPos32(SB_VERT, scrollPos + Yscroll);

			rect.top = Yscroll;
			ScrollWindow(0, -Yscroll, rect);
			rect.top = rect.bottom - (Yscroll+1);
			//InvalidateRect(rect);

		}
		break;
    case SB_LINEUP:
		if (scrollPos == 0)
		{
			m_iStart =0;
			Invalidate();
		}
		if (scrollPos > 0 && m_iStart > 0)
		{
			if (m_iStart < 0)
				break;

			const	int	Yscroll = m_RowHeight;
			m_iStart--;

			SetScrollPos32(SB_VERT, max(0, scrollPos - Yscroll));
			rect.top = 0;
			ScrollWindow(0, Yscroll, rect);
			rect.bottom = rect.top + (Yscroll +1);
			//InvalidateRect(rect);		
		}
		break;
    case SB_PAGEDOWN:
		
		if (scrollPos < m_Vscroll)
		{
			rect.top = 0;
			scrollPos = min(m_Vscroll, scrollPos + rect.Height());
			SetScrollPos32(SB_VERT, scrollPos);
			//InvalidateRect(rect);
			//scrollPos = GetScrollPos32(SB_VERT, TRUE);
			//SetScrollPos32(SB_VERT, scrollPos);
			

		}
		break;
		
    case SB_PAGEUP:
		
		if (scrollPos > 0)
		{
			rect.top = 0;
			const	int	offset = -rect.Height();
			const	int	pos = max(0, scrollPos + offset);
			SetScrollPos32(SB_VERT, pos);
			//InvalidateRect(rect);	

		}
		break;
		
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{	
			scrollPos = GetScrollPos32(SB_VERT, TRUE);
			SetScrollPos32(SB_VERT, scrollPos);

			m_iStart = (scrollPos / m_RowHeight);
			if (m_iStart > GetCount() - 20)
				m_iStart++;

			//Invalidate();
		}
		break;
        case SB_TOP:
	
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_VERT, 0);

			//Invalidate();
		}
		break;
        case SB_BOTTOM:
		if (scrollPos < m_Vscroll)
		{
			SetScrollPos32(SB_VERT, m_Vscroll);
			//Invalidate();

		}
		break;
	default:
		break;
	}
	
	m_iStart = (scrollPos / m_RowHeight);
	if (m_iStart > GetCount() - 20)
		m_iStart++;
	
	Invalidate();

	if (m_pCtlBtnDelete->GetSafeHwnd() && m_pCtlBtnDelete->IsWindowVisible())
	{
		if (m_iSelRow - m_iStart < 0 || m_iSelRow - m_iStart > GetVisibleRow())
		{
			m_rcSelRow.top = - m_RowHeight - 1;
			m_rcSelRow.bottom = m_rcSelRow.top + m_RowHeight - 1;
		}
		else
		{
			m_rcSelRow.top = (m_iSelRow - m_iStart) * m_RowHeight;
			m_rcSelRow.bottom = m_rcSelRow.top + m_RowHeight;
		}
		m_pCtlBtnDelete->SetWindowPos(NULL, m_rcSelRow.right - m_RowHeight - GAP, m_rcSelRow.top + 1, m_RowHeight, m_RowHeight-2, SWP_SHOWWINDOW);
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCodeList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

long CCodeList::GetVirtualHeight()
{
	const	long	height = GetCount()*m_RowHeight;
	return height;
}

BOOL CCodeList::Create(CRect rc, CWnd* parent)
{
	const	BOOL	bResult = CWnd::CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST, 
						constClassName, 
						"", 
						WS_POPUP,
						rc, 
						parent, 
						0);

	if (bResult)
	{
		SetWindowPos(&CWnd::wndTop, 0, 0, rc.Width(), rc.Height(), SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		CenterWindow();
	}
	return bResult;
}

void CCodeList::Init()
{
	CRect	rc;
	CString imgF;

	m_Font.CreatePointFont(90, "굴림체");
	m_pCtlBtnDeleteAll = std::make_unique<CNButton>(IDC_BUTTON_ALLDELETE);
	m_pCtlBtnDelete = std::make_unique<CNButton>(IDC_BUTTON_DELETE);

	if (m_pCtlBtnDeleteAll->Create(NULL, "전부 삭제", WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, IDC_BUTTON_ALLDELETE))
	{
		m_pCtlBtnDeleteAll->set_ToolTipText("종목을 삭제합니다.");
		imgF.Format("%s\\%s\\%s.bmp", m_home, "image", "종목전부삭제_뉴");
		m_pCtlBtnDeleteAll->set_Image(imgF);
		m_pCtlBtnDeleteAll->set_Expand(true);
		m_pCtlBtnDeleteAll->SetCButtomMsg(TRUE);	
	}
	if (m_pCtlBtnDelete->Create(NULL, "종목 삭제", WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, IDC_BUTTON_DELETE))
	{
		m_pCtlBtnDelete->set_ToolTipText("종목을 삭제합니다.");
		imgF.Format("%s\\%s\\%s.bmp", m_home, "image", "종목닫기");
		m_pCtlBtnDelete->set_Image(imgF);
		m_pCtlBtnDelete->set_Expand(true);
		m_pCtlBtnDelete->SetCButtomMsg(TRUE);
	}

	m_pCtlBtnDeleteAll->SetFont(&m_Font);
	m_pCtlBtnDelete->SetFont(&m_Font);

	GetClientRect(rc);
	m_pCtlBtnDeleteAll->SetWindowPos(NULL, 
					 GAP, 
					 rc.bottom - BUTTON_HEIGHT, 
					 rc.Width()- 2*GAP - 16, 
					 BUTTON_HEIGHT - GAP, SWP_SHOWWINDOW);
	//ResetScrollBars();
	//Invalidate();
}

void CCodeList::OnDestroy() 
{
	CWnd::OnDestroy();
	// TODO: Add your message handler code here
	m_Font.DeleteObject();
	UninstallMouseHook(GetSafeHwnd());
}

void CCodeList::Update()
{
	ResetScrollBars();
	Invalidate();
}

BOOL CCodeList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CRect	rc;

	InvalidateRect(m_rcSelRow);
	m_iSelRow = GetCurrentRow(pt, &rc);
	InvalidateRect(rc);
	m_rcSelRow.CopyRect(rc);
	
	if (zDelta > 0)
		PostMessage(WM_VSCROLL, SB_LINEUP, 0);		
	else
		PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

int CCodeList::GetCurrentRow(CPoint pt, CRect* pRect)
{
	int	index = 0;
	CRect	rc, rowrc;

	GetClientRect(rc);
	rowrc.CopyRect(rc);
	rowrc.bottom = m_RowHeight;

	for (int ii = 0;  rowrc.bottom <= rc.bottom - BUTTON_HEIGHT; ii++) 
	{
		if (rowrc.PtInRect(pt))
		{
			pRect->CopyRect(rowrc);
			index = m_iStart + ii;
			break;
		}
		rowrc.OffsetRect(0, m_RowHeight);
	}
	return index;
}

int CCodeList::GetVisibleRow()
{
	CRect	rc;
	int	row = 0, tHeight = 0;

	GetClientRect(rc);
	tHeight = rc.bottom - BUTTON_HEIGHT;
	row = tHeight / m_RowHeight + m_iStart;
	
	if (row > GetCount())
		row = GetCount();

	return row;
}

void CCodeList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	
	//InvalidateRect(m_rcSelRow);
	m_iSelRow = GetCurrentRow(point, &rc);
	
	//InvalidateRect(rc);
	m_rcSelRow.CopyRect(rc);
	CString szCode;
	szCode = GetItem(m_iSelRow);
	if (szCode.IsEmpty()) 
	{
		if (m_pCtlBtnDelete->GetSafeHwnd()) m_pCtlBtnDelete->ShowWindow(SW_HIDE);
		return;
	}

	if (m_pCtlBtnDelete->GetSafeHwnd())
	{
		m_pCtlBtnDelete->SetWindowPos(NULL, rc.right - m_RowHeight - GAP, 
					            rc.top + 1, 
						    m_RowHeight, 
						    m_RowHeight-2, 
						    SWP_SHOWWINDOW);
		//asdfasdf//m_iSelRow
		
		
	}

	if (m_pointPrev.x > -1)
	{
		if (m_pointPrev.x != point.x || m_pointPrev.y != point.y)
		{
			if (m_bFocus)
			{
				if ((CWnd*)GetFocus() != this)
				{
					SetFocus();

					m_bFocus = FALSE;
				}
			}
		}
	}

	m_pointPrev = point;

	Invalidate();

	CWnd::OnMouseMove(nFlags, point);
}

void CCodeList::OnDeleteBtn()
{
	CfxCodeCtrl* pWnd = (CfxCodeCtrl*)m_pParent;
	CControlWnd* pCtrl = (CControlWnd*)pWnd->m_pParent;
	CString	     symbol, code;

	if (m_iSelRow >= 0 && GetCount() > 0 && m_iSelRow < GetCount())
	{
		symbol = pWnd->GetSymbol();
		code = m_arrData.GetAt(m_iSelRow);
		symbol.Format("%s\t%s", pWnd->GetSymbol(), pCtrl->Parser(code, "\t"));
		pCtrl->RemoveHistory(symbol);
		m_arrData.RemoveAt(m_iSelRow);
	}
	Update();

	pWnd->WriteHistory(pWnd->m_sPath);
}

void CCodeList::OnAllDeleteBtn()
{
	if (MessageBox("모두 삭제하시겠습니까??", "IBK투자증권", MB_YESNO) == IDYES)
	{
		CfxCodeCtrl* pWnd = (CfxCodeCtrl*)m_pParent;
		CControlWnd* pCtrl = (CControlWnd*)pWnd->m_pParent;
		CString symbol;

		symbol.Format("%s\t", pWnd->GetSymbol());
		pCtrl->RemoveHistory(symbol);
		m_arrData.RemoveAll();
		Update();

		pWnd->WriteHistory(pWnd->m_sPath);
	}
}

int CCodeList::GetCurSel()
{
	return m_iSelRow;
}

void CCodeList::SetTopIndex(int index)
{
	m_iStart = index;

	SetScrollPos32(SB_VERT, m_iStart*m_RowHeight);
	Invalidate();
}

void CCodeList::SetCurSel(int cur)
{
	m_iSelRow = cur;
	if (m_iSelRow < 0)
		m_iSelRow = 0;

//	m_iSelRow - m_iStart;
	Invalidate();
}

int CCodeList::GetTopIndex()
{
	return m_iStart;
}

CString CCodeList::Parser(CString &srcstr, CString substr)
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

void CCodeList::RemoveAll()
{
	m_arrData.RemoveAll();
}

LRESULT CCodeList::OnMouseHook(WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT *pm = NULL;
	CRect rc;

	pm = (MOUSEHOOKSTRUCT*) lParam;
	GetWindowRect(rc);

	switch (wParam)
	{
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
		if (!rc.PtInRect(pm->pt))
		{
			ShowWindow(SW_HIDE);
			m_pCtlBtnDelete->ShowWindow(SW_HIDE);
			m_pParent->SetVisible(false);
		}
		break;
	}
	return 0;
}

void CCodeList::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here

	m_bFocus = TRUE;
}

BOOL CCodeList::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

void CCodeList::OnActivateApp(BOOL bActive, DWORD hTask) 
{
	CWnd::OnActivateApp(bActive, hTask);	
	if (!bActive)
	{
		ShowWindow(SW_HIDE);
		m_pCtlBtnDelete->ShowWindow(SW_HIDE);
	}
}

void CCodeList::SetItemFont(CFont *m_font)
{
	if (m_font)
	{
		LOGFONT	lf{};

		m_font->GetLogFont(&lf);
		m_RowHeight = abs(lf.lfHeight) +4;
	}
}

void CCodeList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if (!bShow)
	{
		m_pointPrev = CPoint(-1,-1);
	}
	CWnd::OnShowWindow(bShow, nStatus);
}

void CCodeList::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
}


void CCodeList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_UP:
	case VK_PRIOR:
//		if (m_pParent->OnBtnUp())
//			return;
		m_pParent->GoUp();
		break;
	case VK_DOWN:
	case VK_NEXT:
//		if (m_pParent->OnBtnDown())
//			return;
		m_pParent->GoDown();
		break;
	}
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
