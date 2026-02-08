// Xedit.cpp : implementation file
//

#include "stdafx.h"
#include "Xedit.h"
#include "screenlist.h"
#include "../../axis/axmsg.hxx"
#include "../../h/axisvar.h"

#include <Imm.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool listCallback(int, WPARAM, LPARAM);
static CXedit* myEdit = NULL;
/////////////////////////////////////////////////////////////////////////////
// CXedit

CXedit::CXedit()
{
	m_code = _T("");
	m_sdlg = NULL;
	m_ctrlKey = 0;
	m_shiftKey = 0;
	m_bSelect= false;

	m_textColor = COLOR_BARTEXT;
	m_bkColor = COLOR_BARBACKGROUND;
	m_brush.CreateSolidBrush(m_bkColor);

	m_ToolTip = NULL;
}

CXedit::~CXedit()
{
#ifndef DF_USESTL_XEDIT
	if (m_sdlg)	delete m_sdlg;
#endif
	m_brush.DeleteObject();

#ifdef DF_USESTL_XCOM
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			m_ToolTip->DestroyWindow();
		}
		m_ToolTip = NULL;
}
#else
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			//m_ToolTip->RelayEvent(NULL);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
		}
		delete m_ToolTip;
		m_ToolTip = NULL;
	}
#endif
	
}


BEGIN_MESSAGE_MAP(CXedit, CEdit)
	//{{AFX_MSG_MAP(CXedit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXedit message handlers

BOOL CXedit::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
		/*
	case WM_CHAR:
		switch (pMsg->wParam)
		{
		case VK_TAB:
		case VK_RETURN:
			break;
		default:
			if (datCheck(pMsg->wParam))
			{
				parse();
				return TRUE;
			}
			break;
		}
		break;
		*/
	case WM_SETFOCUS:
		break;
	case WM_KEYUP:
		m_ctrlKey = m_shiftKey = 0;
		break;
	case WM_KEYDOWN:
	//**case WM_SYSKEYDOWN:	// for F10
		if (GetKeyState(VK_CONTROL) & 0x8000)
			m_ctrlKey = 1;
		if (GetKeyState(VK_SHIFT) & 0x8000)
			m_shiftKey = 1;

		if (!m_ctrlKey && !m_shiftKey && (pMsg->wParam == VK_RETURN))
		{
			parse();
		}
		
		if (m_shiftKey)			break;
		if (m_ctrlKey && m_shiftKey)	break;
		if (!m_ctrlKey)			break;
		
		if ((pMsg->wParam >= 0x3a && pMsg->wParam <= 0x5a) || pMsg->wParam == VK_ADD || pMsg->wParam == VK_MULTIPLY || pMsg->wParam == VK_DIVIDE || pMsg->wParam == VK_SUBTRACT)
		{
			CString map;
			
			if(pMsg->wParam == VK_ADD)
			{
				map = getHotCode(0x2b);
			}
			else if(pMsg->wParam == VK_MULTIPLY)
			{
				map = getHotCode(0x2a);
			}
			else if(pMsg->wParam == VK_DIVIDE)
			{
				map = getHotCode(0x2f);
			}
			else if(pMsg->wParam == VK_SUBTRACT)
			{
				map = getHotCode(0x2d);
			}
			else
			{
				map = getHotCode(pMsg->wParam);
			}
			
			map.TrimLeft();		map.TrimRight();
			if (!map.IsEmpty())    
				(*m_axiscall)(AXI_HOTKEY, pMsg->wParam, (LPARAM)(const char*)map);
			
			m_ctrlKey = m_shiftKey = 0;
			return TRUE;
		}
		break;
	default:
		break;
	}	
	return CEdit::PreTranslateMessage(pMsg);
}

void CXedit::parse()
{
	int	editL = 0;
	const int	screeenN = L_SCRNO;
	char	editB[128];
	editL = GetWindowText(editB, sizeof(editB));
	if (editL <= 0)	return;

	editB[editL] = '\0';
	if (editL == screeenN)
	{
		int	val{};
		bool	bDigit = true;
		for (int ii = 0; ii < editL; ii++)
		{
			if (editB[ii] > 0xa0 || editB[ii] < 0x20)
			{
				bDigit = false;
				break;
			}

			val = isdigit((int)editB[ii]);
			if (val >= 0 && val <= 9)
				continue;
			bDigit = false;
			break;
		}

		if (bDigit)
		{
			if (!(*m_axiscall)(AXI_INPUTSCREENNO, 0, (LPARAM) editB))
			{
				SetSel(0, -1);
				SetFocus();
			}
			return;
		}
	}

	m_list.RemoveAll();
	if ((*m_axiscall)(AXI_SELCLIST, 0, (LPARAM)editB))
		show_List();
	else	SetWindowText(_T(""));
}

BOOL CXedit::make_MapName(char* src, char* selc)
{
	char	choiceB[16], mapN[16];
	ZeroMemory(choiceB, sizeof(choiceB));
	FillMemory(choiceB, L_SCRNO, '0');
	CopyMemory(&choiceB[L_SCRNO - L_SCRNO], selc, L_SCRNO);

	CString	file;
	file.Format("%s\\%s\\screen.mp%c", Axis::home, MTBLDIR, choiceB[0]);
	DWORD dw = GetPrivateProfileString("SCREEN", selc, "", mapN, sizeof(mapN), file);
	if (dw > 0)	sprintf(src, "%s", mapN);
	else
	{
		sprintf(src, "%s%s00", m_code, choiceB);
		file.Format("%s\\%s\\screen.mp", Axis::home, MTBLDIR);
		dw = GetPrivateProfileString("SCREEN", src, "", mapN, sizeof(mapN), file);
		if (dw > 0)	return FALSE;
	}
	return TRUE;
}

bool CXedit::datCheck(int key)
{
	int	editL = 0;
	char	editB[32];
	editL = GetWindowText(editB, sizeof(editB));
	if (editL != L_SCRNO -1)	return false;
	for (int ii = 0; ii < editL; ii++)
	{
		if ((int) editB[ii] < 0x30)	return false;
		if ((int) editB[ii] > 0x39)	return false;
	}

	if (key < 0x30)	return false;
	if (key > 0x39)	return false;

	editB[editL] = (char)key;
	editB[editL +1] = '\0';
	SetWindowText(editB);
	UpdateWindow();
	return true;
}

void CXedit::set_Code(CString code)
{
	if (code.IsEmpty())	return;
	code.Replace(" ", "");
	m_code = code.Left(2);
}

void CXedit::setaxiscall(bool (*axiscall)(int, WPARAM, LPARAM))
{
	m_axiscall = axiscall;

#ifdef DF_USESTL_XEDIT
	m_sdlg = std::make_unique<CScreenList>(listCallback);
#else
	m_sdlg = new CScreenList(listCallback);
#endif
	
	m_sdlg->Create(IDD_SCREENLIST);
	myEdit = this;	// 한개 이상의 에디트 컨트롤 사용시 개별 설정
}

void CXedit::set_Owner()
{
	myEdit = this;
}

void CXedit::show_List()
{
	if (!m_sdlg || !IsWindow(m_sdlg->GetSafeHwnd()))
		return;
	if (m_list.GetSize() <= 0)	return;

	set_Owner();
	m_sdlg->updateCtrl(m_list, SEARCH);

	CRect	winRC, listRC;
	this->GetWindowRect(&winRC);
	m_sdlg->GetWindowRect(&listRC);

	int sx{}, sy{};
	CRect rcWA; 
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWA, 0);

	if (winRC.bottom + listRC.Height() > rcWA.bottom)
	{
		sx = winRC.left < rcWA.left ? rcWA.left : winRC.left;
		sy = winRC.top - listRC.Height();
	}
	else
	{
		sx = winRC.left < rcWA.left ? rcWA.left : winRC.left;
		sy = winRC.bottom;
	}

	m_sdlg->MoveWindow(sx-3, sy+3, listRC.Width(), listRC.Height());
	m_sdlg->ShowWindow(SW_SHOW);
	m_sdlg->SetForegroundWindow();
}

CString CXedit::get_mapN(int index)
{
	CString mapN;
	mapN = m_list.GetAt(index);
	return mapN.Mid(4, L_MAPN);
}

int CXedit::get_ListSize()
{
	return m_sdlg->m_listctrl.GetItemCount();
}

void CXedit::show_History()
{
	myEdit = this;
	m_sdlg->show_Histroy();
	
	CRect	winRC, listRC;
	this->GetWindowRect(&winRC);
	m_sdlg->GetWindowRect(&listRC);

	int sx{}, sy{};
	CRect rcWA; ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWA, 0);

	if (winRC.bottom + listRC.Height() > rcWA.bottom)
	{
		sx = winRC.left < rcWA.left ? rcWA.left : winRC.left;
		sy = winRC.top - listRC.Height();
	}
	else
	{
		sx = winRC.left < rcWA.left ? rcWA.left : winRC.left;
		sy = winRC.bottom;
	}

	m_sdlg->MoveWindow(sx-3, sy+3, listRC.Width(), listRC.Height());
	m_sdlg->ShowWindow(SW_SHOW);
	m_sdlg->SetForegroundWindow();
}

void CXedit::add_History(CString dat)
{
	m_sdlg->add_History(dat);
	m_sdlg->copy_History(m_list);
}

bool CXedit::checkHotkey(int key)
{
	CProfile profile(pkHotKey);
	if (!profile.GetInt(szHKey, "state"))
		return false;

	CString keys;
	keys.Format("%c", key);
	keys.MakeUpper();
	return profile.GetString(szHKey, keys).GetLength() >= 4;
}

bool listCallback(int msg, WPARAM wParam, LPARAM lParam)
{
	int	index{};
	char	mapname[16]{};
	CString	string;

	switch (msg)
	{
	case LBM_SELECTION:
		if ((index = (int) wParam) < 0 || index >= myEdit->get_ListSize())
			break;
		
		string = myEdit->get_mapN(index);
		CopyMemory(mapname, string, L_MAPN);
		mapname[L_MAPN] = '\0';

		if (!(*myEdit->m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)mapname))
		{
			myEdit->SetWindowText("");
			myEdit->SetFocus();
		}
		break;
	case LBM_NOSELECTION:
		myEdit->SetWindowText("");
		myEdit->SetFocus();
		break;
	}
	return true;
}

HBRUSH CXedit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(m_textColor);
	pDC->SetBkColor(m_bkColor);
	return (HBRUSH)m_brush;
}

void CXedit::OnSetFocus(CWnd* pOldWnd) 
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(this->m_hWnd);
	if ( ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		m_Conversion = dwConversion;
		m_Sentence = dwSentence;
#if 1	// for korean
		dwConversion |= IME_CMODE_HANGEUL;
#else	// for english
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
#endif
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	}
	if (this != pOldWnd && !m_bSelect)
		GetParent()->PostMessage(WM_SELECT, 0, 0);
	CEdit::OnSetFocus(pOldWnd);
}

void CXedit::set_ToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;
	
	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, rectBtn, ID_TOOLTIP);
	}
	
	m_ToolTip->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

void CXedit::InitToolTip()
{
#ifdef DF_USESTL_XEDIT
	if (!m_ToolTip)
	{
		m_ToolTip = std::make_unique<CToolTipCtrl>();
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
}
#else
	if (!m_ToolTip)
	{
		m_ToolTip = new CToolTipCtrl();
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
#endif
}


LRESULT CXedit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_ToolTip || !IsWindow(m_ToolTip->GetSafeHwnd()))
				break;
			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			
			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
			
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	case WM_CHAR:
		
		switch (wParam)
		{
		case VK_TAB:
			break;
		case VK_RETURN:
			//break;
		default:
			if (datCheck(wParam))
			{
				parse();
				return TRUE;
			}
			break;
		}
		break;
	default:
		break;
		
	}		
	return CEdit::WindowProc(message, wParam, lParam);
}

LRESULT CXedit::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHover)
		Invalidate();
	m_bTracking = FALSE;
	m_bHover = FALSE;

	m_bkColor = COLOR_BARBACKGROUND;
	Invalidate();

	return 0;
}

LRESULT CXedit::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	if (!m_bHover)
		Invalidate();
	m_bHover = TRUE;

	m_bkColor = RGB(245, 245, 255);
	Invalidate();

	return 0;
}


void CXedit::OnMouseMove(UINT nFlags, CPoint point) 
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
	CEdit::OnMouseMove(nFlags, point);
}

CString CXedit::getHotCode(int key)
{
	CProfile profile(pkHotKey);
	CString code, keys;

	const int state = profile.GetInt(szHKey, "state");
	if (!state)
		return "";

	keys.Format("%c", key);
	keys.MakeUpper();

	code = profile.GetString(szHKey, keys);
	if (code.GetLength() > 4)
		return code.Left(4);
	else
		return code;
}

void CXedit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnLButtonDown(nFlags, point);
}

void CXedit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetSel(0, -1);
	SetFocus();
	CEdit::OnLButtonUp(nFlags, point);
}

void CXedit::OnSize(UINT nType, int cx, int cy) 
{
	CEdit::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect	cRc;
	GetClientRect(cRc);
	
	if (m_ToolTip)			m_ToolTip->SetToolRect(this, ID_TOOLTIP, cRc);
}
