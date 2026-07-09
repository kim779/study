// colorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "colorDlg.h"

#include "../../h/axisvar.h"
#include "../../dll/lib/axislib.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	winMARGIN	5
#define	winGAP		1
#define	itemWCOUNT	8
#define	itemHCOUNT	8
#define	itemWIDTH	17
#define	itemHEIGHT	15

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

CColorDlg::CColorDlg(CWnd* pParent, DWORD color, bool user /*=true*/, CRect voidRect/*=CRect(0)*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	m_color = color;
	m_user  = user;
	m_pUserColor  = new CColorBox(pParent, color, user);
	m_pUserColor2 = new CColorBox(pParent, color, user, 64);
	m_pSysColor   = new CColorBox(pParent, color, false, 192);
	m_voidRect    = voidRect;
	m_capture     = true;
	m_bDrag       = false;

	//{{AFX_DATA_INIT(CColorDlg)
	//}}AFX_DATA_INIT
}

CColorDlg::~CColorDlg()
{
	FreeRes();
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	DDX_Control(pDX, IDC_TABCTRL, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCTRL, OnSelchangeTabctrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 1, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	cRc, wRc;
	CPoint Pos;
	int	nClrTabPos = 0;
	CString sCurrPath;

	char cPath[1024];
	GetCurrentDirectory(1024, cPath);
	sCurrPath = cPath;
	m_nClrTabPos = GetPrivateProfileInt(ENVIRONMENT, "CLRTABPOS", 0, sCurrPath + "\\workshop.ini");

	GetClientRect(&cRc);
	GetWindowRect(&wRc);
	int dW = wRc.Width() - cRc.Width();
	int dH = wRc.Height() - cRc.Height();
	dW += (itemWCOUNT * itemWIDTH);
	dW += ((itemWCOUNT - 1) * winGAP);
	dW += winMARGIN * 2;
	dH += (itemHCOUNT * itemHEIGHT);
	dH += ((itemHCOUNT - 1) * winGAP);
	dH += winMARGIN * 2;

	dH += winMARGIN;
	dH += (int)(itemHEIGHT * 1.5);

	Pos.x = m_voidRect.left;
	Pos.y = m_voidRect.bottom;
	if (dW + m_voidRect.left > GetSystemMetrics(SM_CXFULLSCREEN))
		Pos.x = GetSystemMetrics(SM_CXSCREEN) - dW;
	if (dH + m_voidRect.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		Pos.y = m_voidRect.top - dH - 20;

	SetWindowPos(NULL, Pos.x, Pos.y, dW + 10, dH + 20, NULL);
	m_tabCtrl.SetWindowPos(NULL, 0, 0, dW, dH + 15, SWP_NOMOVE);
	m_tabCtrl.InsertItem(0, "System");
	m_tabCtrl.InsertItem(1, "User");
	m_tabCtrl.InsertItem(2, "User2");

	m_pUserColor->Create(NULL, NULL, WS_CHILD|(m_nClrTabPos == 1?WS_VISIBLE:NULL), CRect(5, 22, dW, dH + 20), this, 0);
	m_pSysColor->Create(NULL, NULL, WS_CHILD|(m_nClrTabPos == 0?WS_VISIBLE:NULL), CRect(5, 22, dW, dH + 20), this, 1);
	m_pUserColor2->Create(NULL, NULL, WS_CHILD|(m_nClrTabPos == 2?WS_VISIBLE:NULL), CRect(5, 22, dW, dH + 20), this, 2);

	m_tabCtrl.SetCurSel(m_nClrTabPos);

	SetCapture();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CColorDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	ReleaseCapture();
	m_color = lParam;
	CDialog::OnOK();
	return 0;
}

void CColorDlg::OnSize(UINT nType, int cx, int cy) 
{

	CDialog::OnSize(nType, cx, cy);
}

void CColorDlg::OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pUserColor->ShowWindow(SW_HIDE);
	m_pUserColor2->ShowWindow(SW_HIDE);
	m_pSysColor->ShowWindow(SW_HIDE);

	switch(m_tabCtrl.GetCurSel())
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

void CColorDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		ReleaseCapture();
		m_capture = false;
		CRect rc;
		CPoint pnt = point;
		GetClientRect(&rc);
		if (!rc.PtInRect(pnt))
		{
			PostMessage(WM_CLOSE);
			return;
		}
		else 
		{
			m_pSysColor->GetClientRect(&rc);
			ClientToScreen(&pnt);
			m_pSysColor->ScreenToClient(&pnt);
			if (rc.PtInRect(pnt))
			{
				switch(m_tabCtrl.GetCurSel())
				{
				case 0:
					m_pSysColor->SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pnt.x, pnt.y));
					break;
				case 1:
					m_pUserColor->SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pnt.x, pnt.y));
					break;
				case 2:
					m_pUserColor2->SendMessage(WM_LBUTTONDOWN, 0, MAKELONG(pnt.x, pnt.y));
					break;
				}
			}
			else
			{
				pnt = point;
				ClientToScreen(&pnt);
				m_tabCtrl.ScreenToClient(&pnt);
				TCHITTESTINFO tti;
				tti.pt.x = point.x; tti.pt.y = point.y;
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
	CDialog::OnLButtonDown(nFlags, point);
}

void CColorDlg::FreeRes()
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
