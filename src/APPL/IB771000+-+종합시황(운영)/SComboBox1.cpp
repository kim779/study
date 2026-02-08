// SComboBox1.cpp : implementation file
//

#include "stdafx.h"
#include "IB771000.h"
#include "SComboBox1.h"
#include "MapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSComboBox

CSComboBox::CSComboBox()
{
	m_pedSearch = NULL;
}

CSComboBox::~CSComboBox()
{
	if (m_pedSearch)
	{
		if (m_pedSearch->GetSafeHwnd())
			m_pedSearch->DestroyWindow();
	//	delete m_pedSearch;	m_pedSearch = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSComboBox)
	ON_CONTROL_REFLECT(CBN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSComboBox message handlers

BOOL CSComboBox::PreTranslateMessage(MSG* pMsg) 
{
	return CComboBox::PreTranslateMessage(pMsg);
}

void CSComboBox::OnSetfocus() 
{
	if (m_pedSearch)
	{
		m_pedSearch->SetFocus();
	}
}

void CSComboBox::OnKillfocus() 
{
	CString szText = "";
}

void CSComboBox::InitEdit()
{
	CRect rc;
	GetClientRect(&rc);
	//m_pedSearch = new CSEdit(m_pMapDlg);
	m_pedSearch = std::make_unique<CSEdit>(m_pMapDlg);
	m_pedSearch->Create(WS_CHILD|WS_VISIBLE|ES_LEFT,CRect(
		rc.left+3,rc.top+3,rc.right-20,rc.bottom-2),this,108);
			
	m_pedSearch->ShowWindow(SW_SHOW);
	m_pedSearch->SetWindowPos(&CWnd::wndTop, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	m_pedSearch->SetFont( m_pMapDlg->GetAxFont(9,false,"±¼¸²Ã¼") );
}

void CSComboBox::OnDropdown() 
{
	CString str;
	m_pedSearch->GetWindowText(str);
	SetWindowText(str);
}

void CSComboBox::OnSelchange() 
{
	CString str;
	const int nSel = GetCurSel();
	if( nSel != CB_ERR)
	{	
		str = m_pMapDlg->m_SearchWordArray.GetAt(nSel);
		GetWindowText(str);
		m_pedSearch->SetWindowText(str);
		SetWindowText(str);
	}
}

void CSComboBox::OnSelendok() 
{
	CString str;
	const int nSel = GetCurSel();
	if( nSel != CB_ERR)
	{	
		str = m_pMapDlg->m_SearchWordArray.GetAt(nSel);
		GetWindowText(str);
		m_pedSearch->SetWindowText(str);
		SetWindowText(str);
	}
}
