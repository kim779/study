// ScreenSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ScreenSetDlg.h"
#include "afxdialogex.h"


// CScreenSetDlg dialog

IMPLEMENT_DYNAMIC(CScreenSetDlg, CDialogEx)

CScreenSetDlg::CScreenSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenSetDlg::IDD, pParent)
{

}

CScreenSetDlg::~CScreenSetDlg()
{
}

void CScreenSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CScreenSetDlg, CDialogEx)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CScreenSetDlg message handlers


BOOL CScreenSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tab.InsertItem( 0, _T("보기"), 0 );
	m_tab.InsertItem( 1, _T("메신저"), 1 );
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CScreenSetDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt;
	CRect rc;

	pt = point;
	ClientToScreen(&pt);
	GetWindowRect(&rc);
	rc.DeflateRect(5,5);
	
	if(!rc.PtInRect(pt))
		SendMessage(WM_CLOSE, 0, 0);
	CDialogEx::OnMouseMove(nFlags, point);
}
