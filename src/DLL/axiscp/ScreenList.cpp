// ScreenList.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	HMAX		64
#define	SCROLLW		GetSystemMetrics(SM_CXVSCROLL);
/////////////////////////////////////////////////////////////////////////////
// CScreenList dialog


CScreenList::CScreenList(bool (*listcall)(int, WPARAM, LPARAM), CWnd* pParent /*=NULL*/)
	: CDialog(CScreenList::IDD, pParent)
{
	m_listcall = listcall;
	m_history.RemoveAll();
	//{{AFX_DATA_INIT(CScreenList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScreenList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenList)
	DDX_Control(pDX, IDC_LIST, m_listctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreenList, CDialog)
	//{{AFX_MSG_MAP(CScreenList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillfocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenList message handlers

BOOL CScreenList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CScreenList::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
			int	nItem = -1, ii{};
			const UINT	uSelectedCount = m_listctrl.GetSelectedCount();
				for (ii = 0; ii < (int)uSelectedCount; ii++)
				{
					nItem = m_listctrl.GetNextItem(nItem, LVNI_SELECTED);
				}
				(*m_listcall)(LBM_SELECTION, (WPARAM)nItem, (LPARAM)0);
			}
			
			break;
		case VK_ESCAPE:
			(*m_listcall)(LBM_NOSELECTION, (WPARAM)0, (LPARAM)0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CScreenList::OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	(*m_listcall)(LBM_NOSELECTION, (WPARAM) 0, (LPARAM)0);
	EndDialog(IDCANCEL);
}

	
void CScreenList::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int kk = m_listctrl.GetItemCount();
	(*m_listcall)(LBM_SELECTION, (WPARAM) pNMListView->iItem, (LPARAM)0);
	 kk = m_listctrl.GetItemCount();
	EndDialog(IDCANCEL);
}

void CScreenList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	fit();
}

void CScreenList::Init()
{
	LV_COLUMN col{};
	CRect	cRc;
	CString	string;

	CClientDC	dc(this);
	int		trW = dc.GetTextExtent(" 9999 ").cx;
	col.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	col.fmt = LVCFMT_CENTER;
	string.LoadString(ST_TSETUP_TEXT_TR);
	col.pszText = (char *) string.operator LPCTSTR();
	col.iSubItem = 0;
	m_listctrl.GetClientRect(cRc);
	col.cx = trW;
	m_listctrl.InsertColumn(0, &col);

	col.fmt = LVCFMT_LEFT;
	string.LoadString(ST_TSETUP_TEXT_MAPNAME);
	col.pszText = (char *) string.operator LPCTSTR();
	col.iSubItem = 1;
	m_listctrl.GetClientRect(cRc);
	col.cx = cRc.Width() - trW - SCROLLW;

	m_listctrl.InsertColumn(1, &col);
	m_listctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	fit();
}

void CScreenList::fit()
{
	if (m_listctrl.GetSafeHwnd())
	{
		CRect	cRc;
		
		GetClientRect(cRc);
		m_listctrl.MoveWindow(0, 0, cRc.Width(), cRc.Height());
		const int count = m_listctrl.GetItemCount();
		const int fitN  = m_listctrl.GetCountPerPage();
		int W0{}, W1{};
		if (count > fitN)
		{
			W0 = m_listctrl.GetColumnWidth(0) - SCROLLW;
			W1 = cRc.Width() - W0;
		}
		else
		{
			W0 = m_listctrl.GetColumnWidth(0);
			W1 = cRc.Width() - W0;
		}
		W1 -= 4;
		m_listctrl.SetColumnWidth(1, W1);
	}
}

void CScreenList::add_History(CString dat)
{
	CString		key, tmpS;
	
	key = dat.Left(L_MAPN);
	m_history.InsertAt(0, dat);
	if (m_history.GetSize() > HMAX)	m_history.RemoveAt(HMAX);
	for (int ii = m_history.GetSize() -1; ii >= 1; ii--)
	{
		tmpS = m_history.GetAt(ii).Left(L_MAPN);
		if (tmpS != key)	continue;
		m_history.RemoveAt(ii);
		break;
	}
}

void CScreenList::copy_History(CStringArray& list)
{
	list.Copy(m_history);
}

void CScreenList::show_Histroy()
{
	updateCtrl(m_history);
}

bool CScreenList::updateCtrl(const CStringArray& array, int type, bool bParse)
{
	CString		trN, screenN, tmpS;
	LV_ITEM		item;
	
	item.mask = LVIF_TEXT;
	m_listctrl.DeleteAllItems();
	for (int ii = 0; ii < array.GetSize(); ii++)
	{
		screenN = tmpS = array.GetAt(ii);
		if (screenN.GetLength() < 12)	continue;
		item.iItem = m_listctrl.GetItemCount();
		item.iSubItem = 0;

		trN = screenN.Left(4);
		screenN = screenN.Mid(L_MAPN + 4);
		item.pszText = (char *) trN.operator LPCTSTR();
		m_listctrl.InsertItem(&item);

		item.pszText = (char *) screenN.operator LPCTSTR();
		m_listctrl.SetItemText(item.iItem, 1, (char *) screenN.operator LPCTSTR());
	}
	return true;
}
