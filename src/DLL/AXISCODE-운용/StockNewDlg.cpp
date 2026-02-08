// StockNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "StockNewDlg.h"
#include "user.h"
#include <Winuser.h>

#pragma comment(lib, "imm32.lib") //라이브러리를 추가해야 하고 

#include <imm.h>  //이 헤더를 인트루드 해야 합니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListCtrl* g_pListCtrl;
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#define ASCENDING	0
#define	DESCENDING	1

/////////////////////////////////////////////////////////////////////////////
// CStockNewDlg dialog


CStockNewDlg::CStockNewDlg(CPoint pt, int nType, CWnd* pParent /*=nullptr*/)
	: CDialog(CStockNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_arrayNCode.RemoveAll();
	//m_ListCtrl.SetHeaderUse();

	m_point = pt;
	m_nType = nType;
	m_nSearchStart = 0;
}


void CStockNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockNewDlg)
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_editSearch);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_LIST_NEW, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStockNewDlg, CDialog)
	//{{AFX_MSG_MAP(CStockNewDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_NEW, OnDblclkListNew)
	ON_WM_CTLCOLOR()
	ON_WM_CANCELMODE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_NEW, OnColumnclickListNew)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_WM_KEYUP()
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStockNewDlg message handlers

BOOL CStockNewDlg::loadJCode()
{
	NCODE	ncode;
	auto codemap = ((CAxisCodeApp*)AfxGetApp())->_mapCODEx;
	for (auto& HJCode : codemap)
	{
		if (HJCode.second->ssgb == jmSINJU || HJCode.second->ssgb == jmSINJS || HJCode.second->ssgb == jmHYFND || HJCode.second->ssgb == jmELW)
		{
			ncode.name = CString(HJCode.second->hnam, HNameLen);
			ncode.code = CString(HJCode.second->code, HCodeLen);
			if (HJCode.second->ssgb == jmELW)
				ncode.code = ncode.code.Mid(1);
			ncode.fill.Format("%c", HJCode.second->jsiz);

			if (HJCode.second->ssgb == jmSINJU)
				ncode.kind = "증권";
			else if (HJCode.second->ssgb == jmSINJS)
				ncode.kind = "증서";
			else if (HJCode.second->ssgb == jmHYFND)
				ncode.kind = "수익증권";
			else if (HJCode.second->ssgb == jmELW)
				ncode.kind = "ELW";

			if (m_nType == 1 && HJCode.second->ssgb != jmHYFND)
				continue;

			if (m_nType == 2 && HJCode.second->ssgb != jmELW)
				continue;

			if (m_nType == 3 && (HJCode.second->ssgb != jmSINJU && HJCode.second->ssgb != jmSINJS))
				continue;

			if (m_nType == 4 && (HJCode.second->ssgb != jmSINJU && HJCode.second->ssgb != jmSINJS && HJCode.second->ssgb != jmHYFND))
				continue;

			m_arrayNCode.Add(ncode);
		}
	}	
	return TRUE;
}

void CStockNewDlg::AddItem(int row, CString szCode, CString szName, CString szTcode, CString kind)
{
	m_ListCtrl.SetItemData(m_ListCtrl.InsertItem(row, ""), row);

	m_ListCtrl.SetItemText(row, 0, szCode);

	szName.TrimRight();
	m_ListCtrl.SetItemText(row, 1, szName);
	m_ListCtrl.SetItemText(row, 2, szTcode);
	m_ListCtrl.SetItemText(row, 3, kind);
}

void CStockNewDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	EndDialog(1);
}

BOOL CStockNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HIMC himc{};
	DWORD dwConversion{}, dwSentence{};
	
	himc = ImmGetContext(this->GetSafeHwnd());
	ImmGetConversionStatus(himc, &dwConversion, &dwSentence);
	
	SetDlgPos(m_point.x, m_point.y);

	loadJCode();

	const DWORD dwStyle = m_ListCtrl.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE) | LVS_EX_FULLROWSELECT;
	m_ListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	
	m_ListCtrl.SetBkColor(RGB(237,237,237));
	m_ListCtrl.SetTextBkColor(RGB(237,237,237));
	m_ListCtrl.SetTextColor(RGB(0,0,0));

	m_ListCtrl.InsertColumn(1, "종목코드", LVCFMT_LEFT, 69);
	m_ListCtrl.InsertColumn(2, "한글종목명", LVCFMT_LEFT, 145);
	m_ListCtrl.InsertColumn(3, "표준코드", LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(4, "구분", LVCFMT_LEFT, 60);

	CountData();

	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, dwSentence); 

	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStockNewDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth{}, scHeight{};
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();
	const int virScrX = pApp->GetVirtualScreenX();
	if (virScrX > 0)
		scWidth = virScrX;

	CRect	rc;
	GetClientRect(&rc);

	const int gabX = scWidth - (x + rc.Width());
	const int gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

BOOL CStockNewDlg::CountData()
{
	NCODE ncode;
	const int ct = m_arrayNCode.GetSize();
	CString tcode;

	CString word;

	m_editSearch.GetWindowText(word);
	word.TrimLeft();
	word.TrimRight();
	
	if (ct <= 0)	return FALSE;
	
	int nRow = 0;

	for (int ii = 0; ii < ct ; ii++)
	{
		ncode = m_arrayNCode[ii];
		
		if (ncode.kind == "수익증권")
			tcode.Format("KRO%s%s", ncode.code.Mid(1), ncode.fill);
		else if (ncode.kind == "증서" || ncode.kind == "증권")
			tcode.Format("KRA%s%s", ncode.code.Mid(1), ncode.fill);
		
		if(ncode.name.Find(word) > -1 || word == "")
		{
			AddItem(nRow, ncode.code, ncode.name, tcode, ncode.kind);

			nRow++;
		}
	}
	
	if (m_ListCtrl.GetItemCount() > 18)
		m_ListCtrl.SetColumnWidth(4, 40);

	g_pListCtrl = &m_ListCtrl;
	m_ListCtrl.SortItems(CompareFunc, MAKELPARAM(0, DESCENDING));
	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
		m_ListCtrl.SetItemData(ii, ii);

	return TRUE;
}

void CStockNewDlg::OnDblclkListNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const int item = m_ListCtrl.GetSelectionMark();
	
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);
	
	OnOK();
		
	*pResult = 0;
}

void CStockNewDlg::SearchName(CString word)
{
	m_ListCtrl.DeleteAllItems();

	CountData();
// 	int nCount = m_ListCtrl.GetItemCount();
// 
// 	for(int i=m_nSearchStart;i<nCount;i++)
// 	{
// 		CString name = m_ListCtrl.GetItemText(i,1);
// 
// 		if(name.Find(word) < 0)
// 		{
// // 			m_ListCtrl.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
// // 			m_ListCtrl.EnsureVisible(i,FALSE);
// // 			m_ListCtrl.SetFocus();
// 
// 			// break;
// 			m_ListCtrl.DeleteItem(i);
// 			i--;
// 			nCount--;
// 		}
// 	}

// 	m_nSearchStart = i + 1;
// 	
// 	if(m_nSearchStart >= m_ListCtrl.GetItemCount())
// 	{
// 		m_nSearchStart = 0;
// 	}
}

HBRUSH CStockNewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	/*DeleteObject(m_Br);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		m_Br = CreateSolidBrush(USER_BACKCOLOR);
		return m_Br;
	}*/

        return hbr;
}

void CStockNewDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
}

CString CStockNewDlg::GetName(char* code)
{
	CString rValue = _T("");

	for (int ii = 0 ; ii < m_arrayNCode.GetSize() ; ii++)
	{
		if (m_arrayNCode[ii].code.Compare(code) == 0)
		{
			rValue = m_arrayNCode[ii].name;
			break;
		}
	}

	return rValue;
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString    strItem1 = g_pListCtrl->GetItemText(lParam1, LOWORD(lParamSort));
	CString    strItem2 = g_pListCtrl->GetItemText(lParam2, LOWORD(lParamSort));

	if (HIWORD(lParamSort) == ASCENDING)
		return strcmp(strItem2, strItem1);
	return strcmp(strItem1, strItem2);
}

void CStockNewDlg::OnColumnclickListNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	m_bSortDirect = !m_bSortDirect;
	m_ListCtrl.SortItems(CompareFunc, MAKELPARAM(pNMListView->iSubItem, (m_bSortDirect?ASCENDING:DESCENDING)));
	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
		m_ListCtrl.SetItemData(ii, ii);
	
	*pResult = 0;
}

void CStockNewDlg::OnButtonSearch() 
{
	CString word;

	m_editSearch.GetWindowText(word);
	
	SearchName(word);
}

void CStockNewDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CStockNewDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN	&& pMsg->wParam == VK_RETURN)
	{
		const int item = m_ListCtrl.GetSelectionMark();
		
		m_szCode = m_ListCtrl.GetItemText(item, 0);
		m_szName = m_ListCtrl.GetItemText(item, 1);
		
		OnOK();

		return true;
	}
	else if(pMsg->message == WM_KEYDOWN	&& (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN))
	{
		if(m_ListCtrl.GetFocus()->m_hWnd != m_ListCtrl.m_hWnd)
		{
			m_ListCtrl.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			m_ListCtrl.EnsureVisible(0,FALSE);
			m_ListCtrl.SetFocus();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CStockNewDlg::OnChangeEditSearch() 
{
	CString word;
	
	m_editSearch.GetWindowText(word);
	
	SearchName(word);
	m_ListCtrl.SetSelectionMark(0);
}
