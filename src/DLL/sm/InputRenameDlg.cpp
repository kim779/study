// InputRenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputRenameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRenameDlg dialog


CInputRenameDlg::CInputRenameDlg(CWnd* pParent /*=NULL*/, CString sName /*= ""*/)
	: CDialog(CInputRenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRenameDlg)
	m_sName = sName;
	//}}AFX_DATA_INIT
	m_point.x = -1;
	m_point.y = -1;
}


void CInputRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRenameDlg)
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDV_MaxChars(pDX, m_sName, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRenameDlg, CDialog)
	//{{AFX_MSG_MAP(CInputRenameDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRenameDlg message handlers

void CInputRenameDlg::OnOK() 
{
	UpdateData();	
	CDialog::OnOK();
}

BOOL CInputRenameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_point.x != -1 && m_point.y != -1)
	{
		SetWindowPos(NULL, m_point.x, m_point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInputRenameDlg::OnEraseBkgnd(CDC* pDC) 
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

HBRUSH CInputRenameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
