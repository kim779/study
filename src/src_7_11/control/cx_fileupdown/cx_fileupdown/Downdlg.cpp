// Downdlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "cx_fileupdown.h"
#include "Downdlg.h"
#include "afxdialogex.h"
#include "Controlwnd.h"

// CDowndlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDowndlg, CDialogEx)

CDowndlg::CDowndlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDowndlg::IDD, pParent)
{
	m_pParent = (CControlwnd*)pParent;
}

CDowndlg::~CDowndlg()
{
}

void CDowndlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DOWN, m_Dnlist);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btndown);
}


BEGIN_MESSAGE_MAP(CDowndlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CDowndlg::OnBnClickedBtnDown)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDowndlg 메시지 처리기입니다.

void CDowndlg::OnBnClickedBtnDown()
{
	
}


BOOL CDowndlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CPoint pt;
	GetCursorPos(&pt);
	
	CRect rc;
	GetClientRect(rc);
	SetWindowPos(NULL, pt.x, pt.y, rc.Width(), rc.Height() + 25, SWP_SHOWWINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDowndlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if(cx > 0 && cy > 0 && m_Dnlist.GetSafeHwnd() && m_btndown.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);

		rc.DeflateRect(2,2,2,27);
		m_Dnlist.MoveWindow(rc);

		rc.top = rc.bottom + 2;
		rc.bottom = rc.top + 25;
		m_btndown.MoveWindow(rc);
	}
}
