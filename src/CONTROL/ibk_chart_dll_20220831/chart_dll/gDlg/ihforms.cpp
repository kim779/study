#include "stdafx.h"
#include "ihforms.h"

//////////////////////////////////////////////////////////////////////////
// CIHBoardDialog Dialog based class
// made by Inho, lee;
// function
// 1. Have ShowMessage box
//////////////////////////////////////////////////////////////////////////
CIHBoardDialog::CIHBoardDialog(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	//{{AFX_DATA_INIT(CIHBoardDialog)
	//}}AFX_DATA_INIT
	m_pMessage = NULL;
	m_brMessage.DeleteObject();
	m_ftDefault.DeleteObject();
	m_messageID = 999999;
}


void CIHBoardDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIHBoardDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIHBoardDialog, CDialog)
	//{{AFX_MSG_MAP(CIHBoardDialog)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER, OnManage)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CIHBoardDialog::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);

	if (m_pMessage)
	{
		m_pMessage->SetWindowText("");
	}

	CDialog::OnTimer(nIDEvent);
}

int CIHBoardDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	int	nHeight = 0;
	GetWindowRect(m_rcOrigin);
	nHeight = m_rcOrigin.Height();
	m_brMessage.CreateSolidBrush(RGB(255, 255, 255));
	ShowMessage("", 10);
	CenterWindow();
	GetWindowRect(m_rcOrigin);
	m_rcOrigin.bottom = m_rcOrigin.top + nHeight;	
	return 0;
}

BOOL CIHBoardDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			OnCancel();
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CIHBoardDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
		
	if (m_pMessage)
	{
		CRect	rect, rcWnd;
		m_pMessage->GetWindowRect(rect);
		GetWindowRect(rcWnd);
		int	nHeight = rect.Height();
		rect = rcWnd;
		rect.top = rcWnd.bottom - (m_nGab + nHeight);
		rect.bottom = rect.top + nHeight;
		rect.left += m_nGab;
		rect.right -= m_nGab;
		ScreenToClient(rect);
		m_pMessage->MoveWindow(rect);

	}
		
}

HBRUSH CIHBoardDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		if (pWnd)
		{
			if (pWnd->GetDlgCtrlID() == m_messageID)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 0, 0));
				hbr = m_brMessage;
				break;
			}
		}		
		break;
	}	
	return hbr;
}

void CIHBoardDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	LIB_DeleteWnd(m_pMessage);
	
	m_brMessage.DeleteObject();
	m_ftDefault.DeleteObject();
}

void CIHBoardDialog::ShowMessage(CString message, int nSize)
{
	if (nSize > 0)
	{
		SetMsgFont(nSize);
	}

	if (m_pMessage)
	{
		if (message.IsEmpty())
			return;

		m_pMessage->SetWindowText("    " + message);
		SetTimer(99999, 2000, NULL);
	}
}

void CIHBoardDialog::SetMsgFont(int nSize)
{
	CRect	rect;
	rect = m_rcOrigin;

	CString	fName = CString(_T("±¼¸²Ã¼"));
	LOGFONT	logfont;
	logfont.lfHeight         = nSize * 10;
	logfont.lfWidth          = 0;
	logfont.lfEscapement     = 0;
	logfont.lfOrientation    = 0;
	logfont.lfWeight         = 0;
	logfont.lfItalic         = 0;
	logfont.lfUnderline      = false;
	logfont.lfStrikeOut      = false;
	logfont.lfCharSet        = HANGUL_CHARSET;
	logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality        = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(logfont.lfFaceName, fName);
	m_ftDefault.DeleteObject();
	m_ftDefault.CreatePointFontIndirect(&logfont);	
	rect.bottom += (nSize * 3);
	MoveWindow(rect);

	rect.top = rect.bottom - (nSize * 3);
	rect.DeflateRect(nSize, 0, nSize, nSize);
	m_nGab = nSize;	
	ScreenToClient(rect);

	if (m_pMessage == NULL)
	{		
		m_pMessage = new CStatic();
		m_pMessage->Create("", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_SUNKEN | SS_CENTERIMAGE, rect, this, m_messageID);			
	}
	else
	{
		m_pMessage->MoveWindow(rect);
	}
	m_pMessage->SetFont(&m_ftDefault);	
}

LONG CIHBoardDialog::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IHBD_SHOWMSG:
		
		if (HIWORD(wParam) != 0)
			break;

		CString tmpstr;
		tmpstr.Format("%s", (char*)lParam);		

		if (tmpstr.IsEmpty())
			break;

		ShowMessage(tmpstr);
		break;
	}
	return 0;
}

