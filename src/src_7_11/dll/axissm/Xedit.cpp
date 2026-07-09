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

CXedit::CXedit(CString home)
{
	m_home = home;
	m_code = _T("");
	m_sdlg = NULL;
	m_ctrlKey = 0;
	m_shiftKey = 0;
	m_bSelect= false;

	m_textColor = RGB(45, 54, 139);	//COLOR_BARTEXT;
	m_bkColor = COLOR_BARBACKGROUND;
	m_brush.CreateSolidBrush(m_bkColor);

	CString fontName;
	fontName = _T("돋음");
	m_font.CreatePointFont(100, fontName);

	m_user = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	int pos  = m_user.Find('|');
	if (pos < 0)	
		m_user = _T("Anonymous");
	else		
		m_user = m_user.Mid(pos+1);

	if (m_user.IsEmpty())	
		m_user = _T("Anonymous");
}

CXedit::~CXedit()
{
	if (m_sdlg)	delete m_sdlg;
	m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CXedit, CEdit)
	//{{AFX_MSG_MAP(CXedit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXedit message handlers
LRESULT CXedit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_CHAR:

		switch (message)
		{
		case VK_TAB:
		case VK_RETURN:
			break;
		default:
			if (datCheck((int)wParam))
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


BOOL CXedit::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:	
			parse();	
			return TRUE;
		default:			break;
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
	int	screeenN = L_SCRNO;
	char	editB[128];
	editL = GetWindowText(editB, sizeof(editB));
	if (editL <= 0)	return;

	editB[editL] = '\0';
	if (editL == screeenN)
	{
		int	val;
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
	file.Format("%s\\%s\\screen.mp%c", m_home, MTBLDIR, choiceB[0]);
	DWORD dw = GetPrivateProfileString("SCREEN", selc, "", mapN, sizeof(mapN), file);
	if (dw > 0)	sprintf_s(src, sizeof(char) * 16, "%s", mapN); // kjs 64bit 확인 
	else
	{
		// kjs 확인 필요 64bit
		sprintf_s(src, m_code.GetLength() + sizeof(char)*16 + 2, "%s%s00", m_code, choiceB);
		file.Format("%s\\%s\\screen.mp", m_home, MTBLDIR);
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

void CXedit::set_Code(CString code, CString home)
{
//	m_home = home;
	if (code.IsEmpty())	return;
	code.Replace(" ", "");
	m_code = code.Left(2);
}

void CXedit::setaxiscall(bool (*axiscall)(int, WPARAM, LPARAM))
{
	m_axiscall = axiscall;
	m_sdlg = new CScreenList(listCallback);
	m_sdlg->Create(IDD_SCREENLIST);
	myEdit = this;	// 한개 이상의 에디트 컨트롤 사용시 개별 설정
}

void CXedit::set_Owner()
{
	myEdit = this;
}

void CXedit::show_List()
{
	if (m_list.GetSize() <= 0)	return;

	set_Owner();
	m_sdlg->updateCtrl(m_list, SEARCH);

	CRect	winRC, listRC;
	this->GetWindowRect(&winRC);
	m_sdlg->GetWindowRect(&listRC);

	int sx, sy;
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

	int sx, sy;
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
	char	buf[256];
	CString	file, keys;

	file.Format("%s\\%s\\%s\\hkey.ini", m_home, USRDIR, m_user);
	if(!GetPrivateProfileInt("HKEY", "state", 0, file))
		return false;

	keys.Format("%c", key);
	keys.MakeUpper();
	DWORD dw = GetPrivateProfileString("HKEY", keys, "", buf, sizeof(buf), file);
	if (dw < 4)	return false;
	return true;
}

CString CXedit::getHotcode(int key)
{
	char	buf[256];
	CString	code, keys, file;

	file.Format("%s\\%s\\%s\\hkey.ini", m_home, USRDIR, m_user);
	keys.Format("%c", key);
	keys.MakeUpper();

	DWORD dw = GetPrivateProfileString("HKEY", keys, "", buf, sizeof(buf), file);
	if (dw > 4)
	{
		code = buf;
		code = code.Left(4);
	}
	return code;
}

bool listCallback(int msg, WPARAM wParam, LPARAM lParam)
{
	int	index;
	char	mapname[16];
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
	DWORD	dwConversion,dwSentence;

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


void CXedit::OnChange() 
{
	CString text, code;
	GetWindowText(text);

	if (text.GetLength() != 1)
		return;

	int	key = text.GetAt(0);
	if (checkHotkey(key))
	{
		char	mapname[80];
		code = getHotcode(key);
		SetWindowText(code);
		if (!make_MapName(mapname, (char *) code.operator LPCTSTR()))
		{
			SetWindowText("");
			SetFocus();
		}
		else if (!(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)mapname))
		{
			SetWindowText("");
			SetFocus();
		}
	}	
}

int CXedit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetFont(&m_font);
	return 0;
}
