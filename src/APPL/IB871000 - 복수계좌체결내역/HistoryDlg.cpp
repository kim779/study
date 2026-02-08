// HistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB871000.h"
#include "HistoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryDlg dialog


CHistoryDlg::CHistoryDlg(CWnd* pWizard, CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryDlg::IDD, pParent), CAxisExt(pWizard)
{
	//{{AFX_DATA_INIT(CHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistoryDlg)
	DDX_Control(pDX, IDC_HISTLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CHistoryDlg)
	ON_LBN_KILLFOCUS(IDC_HISTLIST, OnKillfocusHistlist)
	ON_WM_SIZE()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryDlg message handlers

void CHistoryDlg::OnKillfocusHistlist() 
{
	EndDialog(IDCANCEL);
	
}

void CHistoryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_list.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_list.MoveWindow(1, 1, rc.Width()-2, rc.Height()-2);
	}
}

void CHistoryDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (nIDCtl==IDC_HISTLIST)
	{
		lpMeasureItemStruct->itemWidth = HISTORY_WIDTH;
		lpMeasureItemStruct->itemHeight = 20;
	}
	
	//CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CHistoryDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (nIDCtl==IDC_HISTLIST)
	{
		const int idx = m_list.GetItemData(lpDrawItemStruct->itemID);
		if (idx<0 || idx>=m_arList.GetSize()) return;
		CRect iRc = lpDrawItemStruct->rcItem;
		CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		
		pDC->FillSolidRect(&iRc, m_crLine);

		--iRc.bottom;
		pDC->FillSolidRect(&iRc, (lpDrawItemStruct->itemID%2==0) ? GetIndexColor(77) : GetIndexColor(68));
		pDC->SetTextColor(m_arColor[idx]);
		pDC->DrawText(m_arList[idx], iRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}
	//CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CHistoryDlg::AddData( LPCSTR text, COLORREF color )
{
	m_arList.Add(text);
	m_arColor.Add(color);
	const int id = m_list.InsertString(0, text);
	m_list.SetItemData(id, m_arList.GetSize()-1);
}

BOOL CHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	m_list.SetItemHeight()
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
