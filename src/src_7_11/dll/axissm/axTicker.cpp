// axTicker.cpp : implementation file
//

#include "stdafx.h"
#include "axTicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxTicker dialog


CAxTicker::CAxTicker(CWnd* pParent /*=NULL*/)
	: CDialog(CAxTicker::IDD, pParent)
{
	m_count  = 0;
	m_parent = pParent;
}


void CAxTicker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxTicker)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxTicker, CDialog)
	//{{AFX_MSG_MAP(CAxTicker)
	ON_WM_SIZE()
	ON_LBN_DBLCLK(IDC_LIST, OnListDblClk)
	ON_LBN_SELCHANGE(IDC_LIST, OnListSelChange)
	ON_LBN_KILLFOCUS(IDC_LIST, OnListKillFocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxTicker message handlers

BOOL CAxTicker::Create(CWnd* pParentWnd)
{
	m_parent = pParentWnd;
	BOOL	Response = CDialog::Create(CAxTicker::IDD, pParentWnd);

	m_list.SetItemHeight(0, 15);
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	return Response;
}

bool CAxTicker::UpdateListBox(CStringArray& array)
{
	m_count = (int)array.GetSize();
	if (m_count <= 0)
		return false;

	m_list.ResetContent();

	int	idx;
	CString	string;
	for (int ii = 0; ii < m_count; ii++)
	{
		string = array.GetAt(ii);
		if (string.IsEmpty())
			continue;

		idx = string.Find('\n');
		if (idx != -1)
			m_list.AddString(string.Left(idx));
		else
			m_list.AddString(string);
	}

	m_selection = -1;
	return true;
}

void CAxTicker::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_list.m_hWnd != NULL)
		m_list.MoveWindow(0, 0, cx, cy);
}

void CAxTicker::OnListDblClk() 
{
	CString	str;

	m_selection = m_list.GetCurSel();
	m_list.GetText(m_list.GetCurSel(), str);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(TBM_SELECTION, m_selection), (LPARAM)str.operator LPCTSTR());
	ShowWindow(SW_HIDE);
}

void CAxTicker::OnListSelChange() 
{
	m_selection = m_list.GetCurSel();
}

void CAxTicker::OnListKillFocus() 
{
	m_selection = -1;
	m_parent->SendMessage(WM_USER, MAKEWPARAM(TBM_NOSELECTION, m_selection), (LPARAM)0);
	ShowWindow(SW_HIDE);
}

BOOL CAxTicker::PreTranslateMessage(MSG* pMsg) 
{
	CString	str;

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			m_list.GetText(m_list.GetCurSel(), str);
			if (str.IsEmpty())
				break;
			m_parent->SendMessage(WM_USER, MAKEWPARAM(TBM_SELECTION, m_selection), (LPARAM)str.operator LPCTSTR());
			break;

		case VK_ESCAPE:
			m_selection = -1;
			m_parent->SendMessage(WM_USER, MAKEWPARAM(TBM_NOSELECTION, m_selection), (LPARAM)0);
			break;

		default:
			break;
		}
	default:
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
