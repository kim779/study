// autoCmplDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "autoCmplDlg.h"
#include "afxdialogex.h"


// CAutoCmplDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoCmplDlg, CDialogEx)

CAutoCmplDlg::CAutoCmplDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoCmplDlg::IDD, pParent)
{
	m_parent = pParent;
}

CAutoCmplDlg::~CAutoCmplDlg()
{
}

void CAutoCmplDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACLIST, m_aclist);
}


BEGIN_MESSAGE_MAP(CAutoCmplDlg, CDialogEx)
	ON_WM_MOUSEACTIVATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER, &CAutoCmplDlg::OnMessage)
END_MESSAGE_MAP()


// CAutoCmplDlg 메시지 처리기입니다.


BOOL CAutoCmplDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	createImageList();
	m_aclist.SetImageList(&m_ImageList);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


int CAutoCmplDlg::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;	
//	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CAutoCmplDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (IsWindow(m_aclist.m_hWnd))
		m_aclist.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);	
}


LRESULT CAutoCmplDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	m_parent->PostMessage(WM_KEYDOWN, wParam, 0);
	return 0;
}

void CAutoCmplDlg::AddString(CString str, int type)
{
	m_aclist.AddString(str, type);
}

void CAutoCmplDlg::FindString(CString str)
{
	if (!str.IsEmpty())
	{
		int	nIndex = m_aclist.SelectString(0, str);
		if (nIndex != LB_ERR)
			m_aclist.SetCurSel(nIndex);
	}
}

BOOL CAutoCmplDlg::IsEmpty()
{
	return m_aclist.GetCount() ? FALSE : TRUE;
}

void CAutoCmplDlg::Reset()
{
	m_aclist.ResetContent();
}

int CAutoCmplDlg::GetWidth()
{
	CString str;
	int	nMax = 0;

	for (int ii= 0; ii < m_aclist.GetCount(); ii++)
	{
		m_aclist.GetText(ii, str);
		nMax = max(nMax, str.GetLength());
	}
	return nMax * 8 + 40;
}

CString CAutoCmplDlg::GetSelString()
{
	CString text;

	if (m_aclist.GetCurSel() != LB_ERR)
		m_aclist.GetText(m_aclist.GetCurSel(), text);

	return text;
}

void CAutoCmplDlg::createImageList()
{
	BOOL	bRetValue = FALSE;
	HICON	hIcon = NULL;

	// Create image list
	bRetValue = m_ImageList.Create(IDB_AUTOCMPLIMG, 12, 1, RGB(192,192,192));
	ASSERT(bRetValue == TRUE);
}