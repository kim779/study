// BondDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "BondDlg.h"
#include "jongmukdlg.h"
#include "../../h/axisfire.h"
#include <imm.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBondDlg dialog


CBondDlg::CBondDlg(CPoint pt, int nType, CWnd* pParent /*=nullptr*/)
	: CDialog(CBondDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBondDlg)
	m_nRadio = 0;
	m_szEditSearch = _T("");
	//}}AFX_DATA_INIT

	m_point = pt;
	m_gsort = FALSE;
	m_nType = nType;
}


void CBondDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBondDlg)
	DDX_Control(pDX, IDC_LIST_BOND, m_ListCtrl);
	DDX_Radio(pDX, IDC_RADIO_ALL, m_nRadio);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edSearch);
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
	ON_BN_CLICKED(IDC_CHKLAST, OnChklast)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_BOND, &CBondDlg::OnGetdispinfoListBond)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBondDlg message handlers

BOOL CBondDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgPos(m_point.x, m_point.y);

	m_Font.CreatePointFont(9*10, "굴림체");
	m_ListCtrl.SetFont(&m_Font);
	m_ListCtrl.SetBkColor(COR_WHITE);
	m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0,0,0));
	
	m_ListCtrl.InsertColumn(1, "코드", LVCFMT_LEFT, 90);
	m_ListCtrl.InsertColumn(2, "채권명", LVCFMT_LEFT, 148);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()&~LVS_EX_GRIDLINES);

	((CButton *)GetDlgItem(IDC_CHKLAST))->SetCheck(TRUE);	// 최근 default

	if (m_nType != 0)
	{
		m_nRadio = m_nType;
		GetDlgItem(IDC_RADIO_SACHE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SMALLBOND)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ALL)->EnableWindow(FALSE);

		((CButton*)GetDlgItem(IDC_RADIO_ALL))->SetCheck(0);
		
		CString strTitle;

		if (m_nType == 1)
		{
			((CButton*)GetDlgItem(IDC_RADIO_SACHE))->SetCheck(1);
			GetWindowText(strTitle);
			strTitle += _T(" - 전환사채");
			SetWindowText(strTitle);
		}
		else if (m_nType == 2)
		{
			((CButton*)GetDlgItem(IDC_RADIO_SMALLBOND))->SetCheck(1);
			GetWindowText(strTitle);
			strTitle += _T(" - 소액채권");
			SetWindowText(strTitle);
		}
	}

	AddRow(m_nRadio);
	Set_KS_HanMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBondDlg::SetDlgPos(int x, int y)
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

void CBondDlg::OnRadio() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	AddRow(m_nRadio);
}

void CBondDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here	
}

void CBondDlg::AddRow(int sort)
{
	//2016.11.07 KSJ 추가하기 전에 지워준다.
	_vBond.clear();
	m_ListCtrl.DeleteAllItems();
	const BOOL bChkLast = ((CButton *)GetDlgItem(IDC_CHKLAST))->GetCheck();

	for_each(_vABond.begin(), _vABond.end(), [&](const auto code){	
		if (bChkLast && std::get<3>(code) == '0')				// 최근
			return;
		switch(sort)
		{
		case 0:
			_vBond.emplace_back(std::get<0>(code), std::get<1>(code));
			break;
		case 1:
			if (std::get<2>(code) == cjCh)
				_vBond.emplace_back(std::get<0>(code), std::get<1>(code));
			break;
		case 2:
			if (std::get<2>(code) == cjSo)
				_vBond.emplace_back(std::get<0>(code), std::get<1>(code));

			break;
		} 
	});

	m_ListCtrl.SetItemCountEx(_vBond.size());
	m_ListCtrl.Invalidate();
	
	//2016.11.07 KSJ 검색단어가 있으면 검색 
	OnChangeEditSearch();
}

void CBondDlg::OnDblclkListBond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	const int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);
	
	OnOK();
	*pResult = 0;
}

void CBondDlg::OnClickListBond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	const int item = m_ListCtrl.GetSelectionMark();
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
			
			const int sub = (LPARAM)pnm->iSubItem;
			switch (sub)
			{
 			case 0:
 				m_gsort = !m_gsort;
 				break;
 			case 1:
 				m_gsort = !m_gsort;
				break;
 			}
			
			listsort(sub);
			

 			#undef pnm

			break;
 		}
 	}

	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CBondDlg::listsort(int type)
{
	std::sort(_vBond.begin(), _vBond.end(), [type](const auto item, const auto item2) {
		if (type == 0)
		{
			if (m_gsort)
				return item.first.Compare(item2.first) >= 0;
			else
				return item.first.Compare(item2.first) < 0;
		}
		else
		{
			if (m_gsort)
				return item.second.Compare(item2.second) >= 0;
			else
				return item.second.Compare(item2.second) < 0;
		}
	});
	m_ListCtrl.Invalidate();
}

void CBondDlg::OnChangeEditSearch() 
{
	//2016.11.08 KSJ 조합완료되면 서치하지 않음
	if(!m_edSearch.m_bComposition)	return;

	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData();

	m_gListsort = FALSE;
	
	const int n = m_szEditSearch.GetLength();

	//2016.11.07 KSJ 종목 검색을 했으면 종목명 정렬, 안했으면 코드 정렬  
	if (n > 0)		listsort(1);
	else			listsort(0);

	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		CString szName = m_ListCtrl.GetItemText(ii, 1);
		szName = szName.Left(n);
		
		if (szName == m_szEditSearch)
		{
			m_ListCtrl.Selected(ii);
			m_szCode = m_ListCtrl.GetItemText(ii, 0);
			m_szName = m_ListCtrl.GetItemText(ii, 1);
			break;
		}
	}
}

BOOL CBondDlg::FindCode(CString szCode)
{
	auto it = std::find_if(_vABond.begin(), _vABond.end(), [&szCode](const auto code){
		if (szCode.CompareNoCase(std::get<0>(code)) == 0)
			return true;
		return false;
	});

	if (it != _vABond.end())
	{
		SetName(std::get<1>(*it));
		switch (const int type = std::get<2>(*it); type)
		{
		case cjCh:
			m_nType = 1;
			break;
		case cjSo:
			m_nType = 2;
			break;
		default:
			m_nType = 0;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

void CBondDlg::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc); 
}

void CBondDlg::OnChklast() 
{
	AddRow(m_nRadio);
}

int CBondDlg::GetCodeType()
{
	if (m_nType == 1)
		return conbondType;
	else
		return babybondType;
}

void CBondDlg::OnGetdispinfoListBond(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	
	const auto& [code, name] = _vBond.at(pItem->iItem);
	
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		switch (pItem->iSubItem)
		{
		case 0:
			lstrcpy(pItem->pszText, code);
			break;
		case 1:
			lstrcpy(pItem->pszText, name);
			break;
		}
	}
	*pResult = 0;
}
