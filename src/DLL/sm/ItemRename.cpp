// ItemRename.cpp : implementation file
//

#include "stdafx.h"
#include "ItemRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemRename dialog


CItemRename::CItemRename(CWnd* pParent /*=NULL*/)
	: CDialog(CItemRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemRename)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CItemRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemRename)
	DDX_Text(pDX, IDC_ITEMNAME, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemRename, CDialog)
	//{{AFX_MSG_MAP(CItemRename)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemRename message handlers

BOOL CItemRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CEdit *)GetDlgItem(IDC_ITEMNAME))->SetFocus();
	((CEdit *)GetDlgItem(IDC_ITEMNAME))->SetSel(0, -1, FALSE);
	((CEdit *)GetDlgItem(IDC_ITEMNAME))->SetLimitText(8);
	
	return TRUE;
}

void CItemRename::SetName(CString name)
{
	m_edit = name;
}

void CItemRename::OnOK() 
{
	UpdateData();

	if (m_edit.IsEmpty())
	{
		::MessageBox(m_hWnd, "새이름을 입력하십시오.", "확인", MB_ICONSTOP);
		return;
	}
	
	CDialog::OnOK();
}

BOOL CItemRename::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CItemRename::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
