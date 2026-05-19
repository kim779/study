// colorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "colorDlg.h"
#include "afxdialogex.h"
#include "colorBox.h"

#include "../../h/axisvar.h"
#include "../h/mainvar.h"

#define	winMARGIN	5
#define	winGAP		1
#define	itemWCOUNT	8
#define	itemHCOUNT	8
#define	itemWIDTH	17
#define	itemHEIGHT	15


// CColorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColorDlg, CDialogEx)

CColorDlg::CColorDlg(CWnd* pParent, DWORD color, bool user, CRect rect)
	: CDialogEx(CColorDlg::IDD, pParent)
{
	m_color = color;
	m_user  = user;
	m_rect  = rect;

	m_capture = true;
	m_tabPos  = 0;
	m_pUserColor  = new CColorBox(pParent, color, user);
	m_pUserColor2 = new CColorBox(pParent, color, user, 64);
	m_pSysColor   = new CColorBox(pParent, color, false, 192);
}

CColorDlg::~CColorDlg()
{
	freeRes();
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCTRL, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_USER+1, &CColorDlg::OnMessage)
//	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, &CColorDlg::OnSelchangeTabctrl)
END_MESSAGE_MAP()


// CColorDlg 메시지 처리기입니다.


BOOL CColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect	cRc, wRc;
	CPoint	pt;
	int	dW, dH, pos = 0;
	CString path;
	char	wb[256];

	GetCurrentDirectory(sizeof(wb), wb);
	path = wb;
	m_tabPos = GetPrivateProfileInt(ENVIRONMENT, _T("CLRTABPOS"), 0, path + _T("\\workshop.ini"));

	GetClientRect(&cRc);
	GetWindowRect(&wRc);
	dW = wRc.Width() - cRc.Width();
	dH = wRc.Height() - cRc.Height();
	dW += (itemWCOUNT * itemWIDTH);
	dW += ((itemWCOUNT - 1) * winGAP);
	dW += winMARGIN * 2;
	dH += (itemHCOUNT * itemHEIGHT);
	dH += ((itemHCOUNT - 1) * winGAP);
	dH += winMARGIN * 2;

	dH += winMARGIN;
	dH += (int)(itemHEIGHT * 1.5);

	pt.x = m_rect.left;
	pt.y = m_rect.bottom;
	if (dW + m_rect.left > GetSystemMetrics(SM_CXFULLSCREEN))
		pt.x = GetSystemMetrics(SM_CXSCREEN) - dW;
	if (dH + m_rect.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		pt.y = m_rect.top - dH - 20;

	SetWindowPos(NULL, pt.x, pt.y, dW + 10, dH + 20, NULL);
	m_tabCtrl.SetWindowPos(NULL, 0, 0, dW, dH + 15, SWP_NOMOVE);
	m_tabCtrl.InsertItem(0, _T("System"));
	m_tabCtrl.InsertItem(1, _T("User"));
	m_tabCtrl.InsertItem(2, _T("User2"));

	m_pUserColor->Create(NULL, NULL, WS_CHILD|(m_tabPos == 1?WS_VISIBLE:NULL), CRect(5, 22, dW, dH + 20), this, 0);
	m_pSysColor->Create(NULL, NULL, WS_CHILD|(m_tabPos == 0?WS_VISIBLE:NULL), CRect(5, 22, dW, dH + 20), this, 1);
	m_pUserColor2->Create(NULL, NULL, WS_CHILD|(m_tabPos == 2?WS_VISIBLE:NULL), CRect(5, 22, dW, dH + 20), this, 2);

	m_tabCtrl.SetCurSel(m_tabPos);

	SetCapture();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_capture)
	{
		CRect	rc;
		CPoint	pt;

		ReleaseCapture();

		pt = point;
		m_capture = false;
		GetClientRect(&rc);
		if (!rc.PtInRect(pt))
		{
			PostMessage(WM_CLOSE);
			return;
		}
		else 
		{
			m_pSysColor->GetClientRect(&rc);
			ClientToScreen(&pt);
			m_pSysColor->ScreenToClient(&pt);
			if (rc.PtInRect(pt))
			{
				switch (m_tabCtrl.GetCurSel())
				{
				case 0:
					m_pSysColor->SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y));
					break;
				case 1:
					m_pUserColor->SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y));
					break;
				case 2:
					m_pUserColor2->SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y));
					break;
				}
			}
			else
			{
				TCHITTESTINFO tti;

			//	pt = point;
			//	ClientToScreen(&pt);

				m_tabCtrl.ScreenToClient(&pt);
				tti.pt.x = point.x;
				tti.pt.y = point.y;
				tti.flags = TCHT_ONITEMLABEL;
				if (m_tabCtrl.HitTest(&tti) >= 0)
					m_tabCtrl.SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(point.x, point.y));
				else
				{
					PostMessage(WM_CLOSE);
					return;
				}
			}
			SetCapture();
			m_capture = true;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

LRESULT CColorDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	m_color = (DWORD)lParam;

	CDialogEx::OnOK();
	return 0;
}


void CColorDlg::OnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_pUserColor->ShowWindow(SW_HIDE);
	m_pUserColor2->ShowWindow(SW_HIDE);
	m_pSysColor->ShowWindow(SW_HIDE);

	switch (m_tabCtrl.GetCurSel())
	{
	case 0:	
		m_pSysColor->ShowWindow(SW_SHOW);
		break;
	case 1:	
		m_pUserColor->ShowWindow(SW_SHOW);
		break;
	case 2:	
		m_pUserColor2->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}

void CColorDlg::freeRes()
{
	if (m_pUserColor)
	{
		if (::IsWindow(m_pUserColor->m_hWnd))
			m_pUserColor->DestroyWindow();
		delete m_pUserColor;
		m_pUserColor = NULL;
	}
	if (m_pUserColor2)
	{
		if (::IsWindow(m_pUserColor2->m_hWnd))
			m_pUserColor2->DestroyWindow();
		delete m_pUserColor2;
		m_pUserColor2 = NULL;
	}
	if (m_pSysColor)
	{
		if (::IsWindow(m_pSysColor->m_hWnd))
			m_pSysColor->DestroyWindow();
		delete m_pSysColor;
		m_pSysColor = NULL;
	}
}
