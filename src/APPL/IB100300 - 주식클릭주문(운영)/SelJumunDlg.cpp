// SelJumunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelJumunDlg.h"
#include "HogaWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelJumunDlg dialog


CSelJumunDlg::CSelJumunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelJumunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelJumunDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_cnt = 0;
}

void CSelJumunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelJumunDlg)
	DDX_Control(pDX, IDC_LIST, m_listJumun);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelJumunDlg, CDialog)
	//{{AFX_MSG_MAP(CSelJumunDlg)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelJumunDlg message handlers

void CSelJumunDlg::SetList(CString juno, CString vol)
{
	CString str;
	vol.TrimLeft(' ');
	m_vol.Add(vol);
	str.Format("%10d", atoi(juno));	//2015.05.28 KSJ 번호 %05d -> %10d
	m_juno.Add(str);
}

void CSelJumunDlg::Init(CPoint pos)
{
	m_ptStart = pos;
	m_junos = _T("-1");
}

BOOL CSelJumunDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rect, cRc;
	GetClientRect(&rect);
	cRc.SetRect(m_ptStart.x, m_ptStart.y, m_ptStart.x + rect.Width()+5, m_ptStart.y + rect.Height()+5);

	int nDx = 0, nDy = 0;
	if (cRc.right > GetSystemMetrics(SM_CXFULLSCREEN))
		nDx = GetSystemMetrics(SM_CXFULLSCREEN) - cRc.right;
	if (cRc.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		nDy = GetSystemMetrics(SM_CYFULLSCREEN) - cRc.bottom;
	cRc.OffsetRect(nDx, nDy - rect.Height());

	MoveWindow(cRc);

	m_listJumun.GetClientRect(&rect);
	if (m_juno.GetSize() > 4) 
	{
		m_listJumun.InsertColumn(0, "주문번호", LVCFMT_CENTER, 60);
		m_listJumun.InsertColumn(1, "수량", LVCFMT_CENTER, 55);
	}
	else
	{
		m_listJumun.InsertColumn(0, "주문번호", LVCFMT_CENTER, rect.Width()/2);
		m_listJumun.InsertColumn(1, "수량", LVCFMT_CENTER, rect.Width()/2);
	}
	int index = 0;
	const int size = m_juno.GetSize() - 1;

	for (int ii = size; ii >= 0; ii--)
	{
		m_listJumun.InsertItem(index, m_juno.GetAt(ii));
		m_listJumun.SetItemText(index, 1, m_vol.GetAt(ii));
		index++;
	}

	m_listJumun.SetExtendedStyle(m_listJumun.GetExtendedStyle() | LVS_EX_CHECKBOXES); 	

	for (int ii = 0; ii < m_juno.GetSize(); ii++)
	{
		m_listJumun.SetCheck(ii, true);
	}

	return TRUE;  
}

void CSelJumunDlg::OnOK() 
{
	CString juno = _T(""), temp;
	m_cnt = 0;

	int bCheck{}, index = m_juno.GetUpperBound();
	for (int ii=0; ii<m_juno.GetSize(); ii++)
	{
		bCheck = m_listJumun.GetCheck(ii);
		if (bCheck)
		{
			m_cnt++;
			temp = m_juno.GetAt(index) + _T("\t");
			juno += temp;
		}
		index--;
	}
	m_junos = juno;

	CDialog::OnOK();
}

void CSelJumunDlg::OnPaint() 
{
	CPaintDC dc(this); 
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RGB(203, 227, 200));
}

void CSelJumunDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState == 3)
	{
		const int check = m_listJumun.GetCheck(pNMListView->iItem);
		m_listJumun.SetCheck(pNMListView->iItem, check ? 0 : 1);
	}

	*pResult = 0;
}

CString CSelJumunDlg::GetJunoString()
{
	if (m_junos == "-1")
	{
		m_cnt = 0;
		m_junos = "";
		for (int ii = 0; ii < m_juno.GetSize(); ii++)
		{
			m_junos += m_juno.GetAt(ii) + "\t";
			m_cnt++;
		}
	}

	CString juno = "";
	if (m_cnt == 1)
	{
		juno = m_junos;
		juno.Remove('\t');
	}
	else if (m_cnt != 0)
	{
		const int pos = m_junos.Find("\t");
		juno.Format("%s 외 %d", m_junos.Left(pos), m_cnt - 1);
	}
	

	return juno;
}

