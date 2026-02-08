// CDlgSinJong.cpp: 구현 파일
//

#include "stdafx.h"
#include "AxisCode.h"
#include "CDlgSinJong.h"
#include "afxdialogex.h"
#include <imm.h>


// CDlgSinJong 대화 상자

IMPLEMENT_DYNAMIC(CDlgSinJong, CDialog)

//CDlgSinJong::CDlgSinJong(CWnd* pParent /*=nullptr*/)
//	: CDialog(IDD_DIALOG_SINJONG, pParent)
//{
//
//}

CDlgSinJong::CDlgSinJong(int kind, int type, CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SINJONG, pParent)
	, m_szEditSearch(_T(""))
{
	m_pApp = (CAxisCodeApp*)AfxGetApp();

	auto future = std::async([&]() {
		for (auto mt : m_pApp->_mapCODEx)
			_mapCODE.emplace(mt);
		});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");

	m_point = pt;
	m_kind = kind;
	m_Type = type;
}

CDlgSinJong::~CDlgSinJong()
{
}

void CDlgSinJong::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_GUBN, m_cbGubn);
	DDX_Control(pDX, IDC_LIST_SINJONG, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_szEditSearch);
}


BEGIN_MESSAGE_MAP(CDlgSinJong, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSinJong::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CDlgSinJong::OnChangeEditSearch)
//	ON_NOTIFY(HDN_GETDISPINFO, 0, &CDlgSinJong::OnGetdispinfoListSinjong)
ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SINJONG, &CDlgSinJong::OnGetdispinfoListSinjong)
ON_CBN_SELCHANGE(IDC_COM_GUBN, &CDlgSinJong::OnSelchangeComGubn)
ON_NOTIFY(NM_CLICK, IDC_LIST_SINJONG, &CDlgSinJong::OnClickListSinjong)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_SINJONG, &CDlgSinJong::OnDblclkListSinjong)
ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDlgSinJong::OnBnClickedBtnConfirm)
END_MESSAGE_MAP()


// CDlgSinJong 메시지 처리기


void CDlgSinJong::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


BOOL CDlgSinJong::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgPos(m_point.x, m_point.y);

	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDCANCEL))->ShowWindow(SW_HIDE);


	m_cbGubn.AddString("전체");
	m_cbGubn.AddString("신탁수익증권");
	m_cbGubn.AddString("투자계약증권");
	m_cbGubn.SetCurSel(0);

	m_Font.CreatePointFont(9 * 10, "굴림체");
	m_ListCtrl.SetFont(&m_Font);
	//m_ListCtrl.SetBkColor(COR_WHITE);
	//m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0, 0, 0));

	m_ListCtrl.InsertColumn(1, "종목코드", LVCFMT_CENTER, 80);
	m_ListCtrl.InsertColumn(2, "종목명", LVCFMT_LEFT, 185);
	m_ListCtrl.InsertColumn(3, "구분", LVCFMT_LEFT, 100);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() & ~LVS_EX_GRIDLINES);

	ProcPart(0);
	Set_KS_HanMode();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSinJong::listsort(const int column)
{
	std::sort(_vData.begin(), _vData.end(), [column](const auto item1, const auto item2) {
		switch (column)
		{
		case 1:
			if (m_gsortSinjong)
				return (std::get<1>(item1).Compare(std::get<1>(item2)) > 0);
			else
				return (std::get<1>(item1).Compare(std::get<1>(item2)) < 0);
			break;
		case 0:
		default:
			if (m_gsortSinjong)
				return (std::get<0>(item1).Compare(std::get<0>(item2)) > 0);
			else
				return (std::get<0>(item1).Compare(std::get<0>(item2)) < 0);
			break;
		}
		});
	m_ListCtrl.Invalidate();
}

void CDlgSinJong::OnChangeEditSearch()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	listsort(1);

	const int n = m_szEditSearch.GetLength();

	auto ft = std::find_if(_vData.begin(), _vData.end(), [&](const auto item) {
		const CString sName = std::get<1>(item).Left(n);
		if (sName.CollateNoCase(m_szEditSearch) == 0)
			return true;
		return false;
		});

	if (ft != _vData.end())
		m_ListCtrl.Selected(ft - _vData.begin());

}

void CDlgSinJong::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth{}, scHeight{};
	scWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
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

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

BOOL CDlgSinJong::ProcPart(int ntype)
{
	std::vector<HCODEX*> vCode;
	_vData.clear();

	if (ntype == 0)
	{
		for_each(_mapCODE.begin(), _mapCODE.end(), [&](auto item) {
			if (item.second->ssgb == jmTJCONJS) 
			{
				_vData.emplace_back(std::move(std::make_tuple(
					CString(item.second->code, 12).Trim(),
					CString(item.second->hnam, 40).Trim(),
					"투자계약증권"
				))); 
			}

			if (item.second->ssgb == jmSINSUJS)
			{
				_vData.emplace_back(std::move(std::make_tuple(
					CString(item.second->code, 12).Trim(),
					CString(item.second->hnam, 40).Trim(),
					"신탁수익증권"
				)));
			}
		});
	}
	else if (ntype == 1)
	{
		for_each(_mapCODE.begin(), _mapCODE.end(), [&](auto item) {
			if (item.second->ssgb == jmSINSUJS)
			{
				_vData.emplace_back(std::move(std::make_tuple(
					CString(item.second->code, 12).Trim(),
					CString(item.second->hnam, 40).Trim(),
					"신탁수익증권"
				)));
			}
			/*else if (item.second->ssgb == jmMUFND)
			{
				_vData.emplace_back(std::move(std::make_tuple(
					CString(item.second->code, 12).Trim(),
					CString(item.second->hnam, 40).Trim(),
					"신탁수익증권"
				)));
			}*/
		});
	}
	else if (ntype == 2)
	{
		for_each(_mapCODE.begin(), _mapCODE.end(), [&](auto item) {
			if (item.second->ssgb == jmTJCONJS) 
			{
				_vData.emplace_back(std::move(std::make_tuple(
					CString(item.second->code, 12).Trim(),
					CString(item.second->hnam, 40).Trim(),
					"투자계약증권"
				)));
			}
			/*else if (item.second->ssgb == jmDEF)
			{
				_vData.emplace_back(std::move(std::make_tuple(
					CString(item.second->code, 12).Trim(),
					CString(item.second->hnam, 40).Trim(),
					"신탁수익증권"
				)));
			}*/
		});
	}

	//m_ListCtrl.SetColumnWidth(1, COLUMN1_WIDTH);
	m_ListCtrl.DeleteAllItems();
	_vCODE.clear();
	
	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();
	return TRUE;
}

BOOL CDlgSinJong::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	static int row;

	if (wParam == IDC_LIST_SINJONG)// <-- 여기서 그리드 컨트롤을 구분..
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_COLUMNCLICK:
#define pnm ((LPNMLISTVIEW)lParam)
			m_gsortSinjong = !m_gsortSinjong;
			listsort((LPARAM)pnm->iSubItem);
#undef pnm

			break;
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlgSinJong::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc);
}

void CDlgSinJong::OnGetdispinfoListSinjong(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	const auto& [code, name, cgub] = _vData.at(pItem->iItem);

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
		case 2:
			lstrcpy(pItem->pszText, cgub);
			break;
		}
	}
	*pResult = 0;
}


void CDlgSinJong::OnSelchangeComGubn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int igubn = m_cbGubn.GetCurSel();
	ProcPart(igubn);
}


void CDlgSinJong::OnClickListSinjong(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	const int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);

	*pResult = 0;
}


void CDlgSinJong::OnDblclkListSinjong(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	const int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);

	OnOK();
	*pResult = 0;
}

void CDlgSinJong::OnBnClickedBtnConfirm()
{
	const int item = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(item, 0);
	m_szName = m_ListCtrl.GetItemText(item, 1);
	m_szCode.Trim();
	if (m_szCode.IsEmpty())
		OnCancel();
	OnOK();
}
