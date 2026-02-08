// RetailBondDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "RetailBondDlg.h"
#include "jongmukdlg.h"
#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRetailBondDlg dialog

CRetailBondDlg::CRetailBondDlg(CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(CRetailBondDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRetailBondDlg)
	m_nRadio = 0;
	m_szEditSearch = _T("");
	//}}AFX_DATA_INIT
	
	m_point = pt;
	m_gsortReTail = FALSE;
}


void CRetailBondDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRetailBondDlg)
	DDX_Control(pDX, IDC_LIST_RETAILBOND, m_ListCtrl);
	DDX_Radio(pDX, IDC_RADIO_ALL_RETAILBOND, m_nRadio);
	DDX_Text(pDX, IDC_EDIT_SEARCH_RETAILBOND, m_szEditSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRetailBondDlg, CDialog)
	//{{AFX_MSG_MAP(CRetailBondDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RETAILBOND, OnDblclkListRetailbond)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RETAILBOND, OnClickListRetailbond)
	ON_EN_CHANGE(IDC_EDIT_SEARCH_RETAILBOND, OnChangeEditSearch)
	ON_BN_CLICKED(IDC_CHKLAST_RETAILBOND, OnChklast)
	ON_BN_CLICKED(IDC_RADIO_ALL_RETAILBOND, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_PUBLIC, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_CURRENCY, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_BANK, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_NOT_BANK, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_CORP, OnRadio)
	ON_BN_CLICKED(IDC_RADIO_ETC, OnRadio)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RETAILBOND, &CRetailBondDlg::OnGetdispinfoListRetailbond)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRetailBondDlg message handlers

BOOL CRetailBondDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgPos(m_point.x, m_point.y);

	m_Font.CreatePointFont(9*10, "굴림체");
	m_ListCtrl.SetFont(&m_Font);
	m_ListCtrl.SetBkColor(COR_WHITE);
	m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0,0,0));
	
	m_ListCtrl.InsertColumn(1, "코드", LVCFMT_LEFT, 90);
	m_ListCtrl.InsertColumn(2, "채권명", LVCFMT_LEFT, 171);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()&~LVS_EX_GRIDLINES);

	LoadSJCode();
	((CButton *)GetDlgItem(IDC_CHKLAST_RETAILBOND))->SetCheck(TRUE);	// 최근 default
	AddRow(0);

	Set_KS_HanMode();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRetailBondDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

BOOL CRetailBondDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static int row;
 
 	if (wParam == IDC_LIST_RETAILBOND)// <-- 여기서 그리드 컨트롤을 구분..
 	{
 		switch(((LPNMHDR) lParam)->code)
 		{
 			case LVN_COLUMNCLICK:
 			#define pnm ((LPNMLISTVIEW)lParam)
			m_gsortReTail = !m_gsortReTail;
 			listsort((LPARAM)pnm->iSubItem);
 			#undef pnm

			break;
 		}
 	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CRetailBondDlg::AddRow(int sort)
{
	const BOOL bChkLast = ((CButton *)GetDlgItem(IDC_CHKLAST_RETAILBOND))->GetCheck();
	_vData.clear();
	for_each(m_RetailBondArray.begin(), m_RetailBondArray.end(), [&](const auto code){
		
		const char type = std::get<2>(code);
		switch(sort)
		{		
		case 1:		// 국채
			if (type == sjKC)
				_vData.emplace_back(code);
			break;
		case 2:		// 통안채
			if (type == sjTA)
				_vData.emplace_back(code);
			break;
		case 3:		// 금융채
			if (type == sjKY)
				_vData.emplace_back(code);
			break;
		case 4:		// 비금융특수채
			if (type == sjNK)
				_vData.emplace_back(code);
			break;
		case 5:		// 회사채
			if (type == sjCS	)
				_vData.emplace_back(code);
			break;
		case 6:		// 기타
			if (type == sjET)
				_vData.emplace_back(code);
			break;
		case 0:
		default:
			_vData.emplace_back(code);
			break;
		}
	});
	
	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();

	m_ListCtrl.Selected(0);
	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);	
}


void CRetailBondDlg::SetDlgPos(int x, int y)
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

	const int	gabX = scWidth - (x + rc.Width());
	const int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void CRetailBondDlg::OnDblclkListRetailbond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);
	
	OnOK();
	
	*pResult = 0;
}

void CRetailBondDlg::OnClickListRetailbond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);

	*pResult = 0;
}

void CRetailBondDlg::OnChangeEditSearch() 
{
	UpdateData();
	m_gListsort = FALSE;
	listsort(1);

	const int n = m_szEditSearch.GetLength();

	auto ft = std::find_if(_vData.begin(), _vData.end(), [&](const auto item){
		const CString sName = std::get<1>(item).Left(n);
		if (sName.CollateNoCase(m_szEditSearch) == 0)
			return true;
		return false;
	});
	
	if (ft != _vData.end())
		m_ListCtrl.Selected(ft - _vData.begin());
}


BOOL CRetailBondDlg::FindCode(CString szCode)
{
	auto ft = std::find_if(m_RetailBondArray.begin(), m_RetailBondArray.end(), [&szCode](const auto item){
		if (szCode == std::get<0>(item))
			return true;
		return false;
	});

	if (ft != m_RetailBondArray.end())
	{
		SetName(std::get<1>(*ft));
		return TRUE;
	}
	return FALSE;
}

void CRetailBondDlg::DeleteAllData()
{
//	m_RetailBondArray.clear();
}

void CRetailBondDlg::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc); 
}

void CRetailBondDlg::OnChklast() 
{
	AddRow(m_nRadio);
}

void CRetailBondDlg::OnRadio() 
{
	m_ListCtrl.DeleteAllItems();
	UpdateData();
	AddRow(m_nRadio);
}

BOOL CRetailBondDlg::LoadSJCode()
{
	CString path;
	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\sjcode.dat";
	CFile file(path, CFile::modeRead | CFile::shareDenyNone);	
	const int size = gsl::narrow_cast<int>(file.GetLength());
	const int readN = file.Read(_szData.GetBuffer(size + 1), size);
	_szData.ReleaseBuffer();
	file.Close();

	if (readN < size)
		return FALSE;

	const int codeN = gsl::narrow_cast<int>(size / sizeof(struct sjcode));
	struct sjcode* code = (struct sjcode*)_szData.GetString();

	for (int ii = 0; ii < codeN; ii++, code++)
	{
		m_RetailBondArray.emplace_back(std::move(std::make_tuple(
			CString(code->code, 12).Trim(),
			CString(code->hnam, 20).Trim(),
			code->cgub,
			code->ngub
		)));
	}
	return TRUE;
}

void CRetailBondDlg::listsort(const int column)
{
	std::sort(_vData.begin(), _vData.end(), [column](const auto item1, const auto item2) {
		switch (column)
		{
		case 1:
			if (m_gsortReTail)
				return (std::get<1>(item1).Compare(std::get<1>(item2)) > 0);
			else 
				return (std::get<1>(item1).Compare(std::get<1>(item2)) < 0);
			break;
		case 0:
		default:
			if (m_gsortReTail)
				return (std::get<0>(item1).Compare(std::get<0>(item2)) > 0);
			else 
				return (std::get<0>(item1).Compare(std::get<0>(item2)) < 0);
			break;
		}
	});
	m_ListCtrl.Invalidate();
}

void CRetailBondDlg::OnGetdispinfoListRetailbond(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	const auto& [code, name, cgub, ngub] = _vData.at(pItem->iItem);

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
