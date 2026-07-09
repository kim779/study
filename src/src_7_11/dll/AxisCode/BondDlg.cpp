// BondDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "BondDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBondDlg dialog

int CALLBACK ListViewCompareFunc1(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	_listItem *pItem1 = (_listItem *) lParam1; 
	_listItem *pItem2 = (_listItem *) lParam2; 

	int	iCmp=0;

	switch ((int)lParamSort)
	{
	case 0:
		iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 
		break;
	case 1:
		iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
		break;
	}

	if (m_gsort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}

	return iCmp;
}

CBondDlg::CBondDlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CBondDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBondDlg)
	m_nRadio = 0;
	m_Point = pt;
	m_gsort = FALSE;
	m_szEditSearch = _T("");
	//}}AFX_DATA_INIT
}


void CBondDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBondDlg)
	DDX_Control(pDX, IDC_LIST_BOND, m_ListCtrl);
	DDX_Radio(pDX, IDC_RADIO_ALL, m_nRadio);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_szEditSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBondDlg, CDialog)
	//{{AFX_MSG_MAP(CBondDlg)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadio)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BOND, OnDblclkListBond)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BOND, OnClickListBond)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_RADIO_SACHE, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_SMALLBOND, OnRadio)
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBondDlg message handlers

BOOL CBondDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	if (m_Point.x > 0 || m_Point.y > 0)
//		SetWindowPos(NULL, m_Point.x, m_Point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	SetDlgPos(m_Point.x, m_Point.y);

	m_Font.CreatePointFont(9*10, "굴림체");
	m_ListCtrl.SetFont(&m_Font);
	m_ListCtrl.SetBkColor(COR_WHITE);
	m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0,0,0));
	
	m_ListCtrl.InsertColumn(1, "코드", LVCFMT_LEFT, 90);
	m_ListCtrl.InsertColumn(2, "채권명", LVCFMT_LEFT, 148);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()&~LVS_EX_GRIDLINES);

	AddRow(0);

	Set_KS_HanMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBondDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth, scHeight;
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect	rc;
	GetClientRect(&rc);

	int	gabX = scWidth - (x + rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void CBondDlg::OnRadio() 
{
	// TODO: Add your control notification handler code here
	LISTITEM* pitem;

	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		pitem = (LISTITEM*)m_ListCtrl.GetItemData(ii);
		delete pitem;
	}
	m_ListCtrl.DeleteAllItems();

	UpdateData();
	AddRow(m_nRadio);
}



void CBondDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	LISTITEM* pitem;
	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		pitem = (LISTITEM*)m_ListCtrl.GetItemData(ii);
		delete pitem;
	}
	
}

void CBondDlg::AddRow(int sort)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	CJCODE *code;
	int count = 0;

	//m_ListCtrl.SetItemCount(pApp->m_arrayCJCode.GetSize());

	for (int ii = 0; ii < pApp->m_arrayCJCode.GetSize(); ii ++)
	{
		/*
		code = pApp->m_arrayCJCode.GetAt(ii);	
		switch(sort)
		{
		case 0:
			{
				AddItem(ii, code->code, code->name);
			}
			break;
		case 1:
			{
				if (code->gubn == cjCh)
				{
					AddItem(count, code->code, code->name);
					count++;
				}
			}
			break;
		case 2:
			{
				if (code->gubn == cjSo)
				{
					AddItem(count, code->code, code->name);
					count++;
				}
			}
			break;
		}
		*/
	}

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);

}


void CBondDlg::AddItem(int row, CString szCode, CString szName)
{

	LISTITEM* pitem;		

	m_ListCtrl.InsertItem(row, "");
	m_ListCtrl.SetItemText(row, 0, szCode);
	m_ListCtrl.SetItemText(row, 1, szName);
	
	pitem = new LISTITEM;

	strcpy((char*)pitem->Code, szCode);

	strcpy((char*)pitem->Name, szName);
	
	m_ListCtrl.SetItemData(row, (LPARAM)pitem);
	
}


void CBondDlg::OnDblclkListBond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);
	
	OnOK();
	
	*pResult = 0;
}

void CBondDlg::OnClickListBond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);

	*pResult = 0;
}

BOOL CBondDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	static int row;
 
 	if (wParam == IDC_LIST_BOND)// <-- 여기서 그리드 컨트롤을 구분..
 	{
 		switch(((LPNMHDR) lParam)->code)
 		{
 			case LVN_COLUMNCLICK:
 			#define pnm ((LPNMLISTVIEW)lParam)
			switch ((LPARAM)pnm->iSubItem)
			{
 			case 0:
 				m_gsort = !m_gsort;
 				break;
 			case 1:
 				m_gsort = !m_gsort;
				break;
 			}
			
			ListView_SortItems((HWND)pnm->hdr.hwndFrom, ListViewCompareFunc1, (LPARAM)pnm->iSubItem);
 			
 			#undef pnm

			break;
 		}
 	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

#include "jongmukdlg.h"
void CBondDlg::OnChangeEditSearch() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();

	m_gListsort = FALSE;
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc1, 1);
	
	int n = m_szEditSearch.GetLength();

	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		CString szName = m_ListCtrl.GetItemText(ii, 1);
		szName = szName.Left(n);
		
		if (szName == m_szEditSearch)
		{
			m_ListCtrl.Selected(ii);
		}
	}
}

BOOL CBondDlg::FindCode(CString szCode)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int nCount = (int)pApp->m_arrayCJCode.GetSize();
	CJCODE *code;

	for (int ii = 0; ii < nCount; ii++)
	{
		code = pApp->m_arrayCJCode.GetAt(ii);

		if (szCode == code->code)
		{
			SetName(code->name);
			return TRUE;
		}
	
	}

	return FALSE;
}

void CBondDlg::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc); 
}
/*
HBRUSH CBondDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here	
	DeleteObject(m_hBrush);

	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_hBrush = CreateSolidBrush(USER_BACKCOLOR);
		
		return m_hBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
*/