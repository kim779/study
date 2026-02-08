// EtfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "EtfDlg.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtfDlg dialog
#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ

#define	SK_JONGMOK	0	// 종목검색
#define	SK_WORD		1	// 단어검색

#define	COLUMN1_WIDTH	132

CEtfDlg::CEtfDlg(CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(CEtfDlg::IDD, pParent)
{	
	m_point = pt;
	m_eListsort = FALSE;
	m_search = FALSE;	

	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	auto future = std::async([&]() {
		for (auto mt : pApp->_mapCODEx)
			_mapCODE.emplace(mt);
		});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");
}


void CEtfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtfDlg)
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_EditSearch);
	DDX_Control(pDX, IDC_TREE_BUNRYU, m_Tree);
	DDX_Control(pDX, IDC_LIST_CTRL, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtfDlg, CDialog)
	//{{AFX_MSG_MAP(CEtfDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL, OnClickListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CTRL, OnDblclkListCtrl)
	ON_CBN_SELCHANGE(IDC_STATIC_SEARCH, OnSelchangeStaticSearch)
	ON_NOTIFY(NM_CLICK, IDC_TREE_BUNRYU, OnClickTreeBunryu)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CTRL, &CEtfDlg::OnGetdispinfoListCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtfDlg message handlers
BOOL CEtfDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetDlgPos(m_point.x, m_point.y);

	m_Font.CreateFont(12, "굴림체");
	m_ListCtrl.SetFont(&m_Font);
	
	m_ListCtrl.SetBkColor(COR_WHITE);
	m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0,0,0));
	m_ListCtrl.InsertColumn(1, "코드", LVCFMT_LEFT, 60);
	m_ListCtrl.InsertColumn(2, "종목명", LVCFMT_LEFT, COLUMN1_WIDTH);
	
	m_Tree.ModifyStyle(0,TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP |TVS_SHOWSELALWAYS);
	m_Tree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	
	InitialTree();
	InitialList(acETFETN);
//	m_ComboSearchKind.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtfDlg::InitialTree()
{		
	CString data, line;	
	HTREEITEM hItem1{}, hItem2{}, hItem3{};
	
	hItem2 = m_Tree.InsertItem("ETF/ETN/리츠 전종목",0,1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, tree_etfetn));

	hItem1 = m_Tree.InsertItem("ETF",0,1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));

	hItem2 = m_Tree.InsertItem("전체종목",0,1,hItem1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, tree_etf));
	
	hItem2 = m_Tree.InsertItem("운용사별",0,1,hItem1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));
	

	for_each(_vETFitem.begin(), _vETFitem.end(), [&](const auto item){
		hItem3 = m_Tree.InsertItem(std::get<2>(item), 0, 1, hItem2);		
		m_Tree.SetItemData(hItem3, MAKELONG(0, tree_etf_item));
	});
	
	hItem2 = m_Tree.InsertItem("기초자산별",0,1,hItem1);	//2015.11.04 KSJ 테마별 --> 기초자산별로 변경
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));
	

	for_each(_vETFtema.begin(), _vETFtema.end(), [&](const auto item){
		hItem3 = m_Tree.InsertItem(std::get<2>(item), 0, 1, hItem2);		
		m_Tree.SetItemData(hItem3, MAKELONG(0, tree_etf_tema));
	});
	
	hItem2 = m_Tree.InsertItem("국내/외",0,1,hItem1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, 10000));
	
	
	for_each(_vETFfore.begin(), _vETFfore.end(), [&](const auto item){
		hItem3 = m_Tree.InsertItem(std::get<2>(item), 0, 1, hItem2);		
		m_Tree.SetItemData(hItem3, MAKELONG(0, tree_etf_fore));
	});
	
	hItem1 = m_Tree.InsertItem("ETN",0,1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, tree_etn));

	hItem2 = m_Tree.InsertItem("리츠종목", 0, 1);
	m_Tree.SetItemData(hItem2, MAKELONG(0, tree_reits));

	//	m_Tree.Select(hItem2, 2);
	GetDlgItem(IDC_EDIT_SEARCH)->SetFocus();
}

void CEtfDlg::InitReitList()
{
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);	//2015.09.30 KSJ 컬럼사이즈 내용에 맞게 사이즈 조절
	m_search = FALSE;
	_vData.clear();

	for_each(_mapCODE.begin(), _mapCODE.end(), [&](auto item) {
		if ((item.second->ssgb == jmREITS || item.second->ssgb == jmSHIP || item.second->ssgb == jmINFRA))
			_vData.emplace_back(std::move(std::make_pair(CString(item.second->code, 12).TrimRight().Mid(1), CString(item.second->hnam, 40).TrimRight())));
		});

	std::copy(_vData.begin(), _vData.end(), std::back_inserter(_vSelect));

	m_search = TRUE;

	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();
	m_EditSearch.SetFocus();
}

void CEtfDlg::InitialList(int nETF)
{
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);	//2015.09.30 KSJ 컬럼사이즈 내용에 맞게 사이즈 조절
	m_search = FALSE;
	_vData.clear();

	if(nETF == acETF || nETF == acETFETN)
	{
		for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item){
			auto [type, code, hnam, opcd, etfm, etfl] = item;
			_vData.emplace_back(std::move(std::make_pair(code, hnam)));
		});
	}

	if(nETF == acETN || nETF == acETFETN)
	{
		for_each(_vETNx.begin(), _vETNx.end(), [&](const auto hjc){
			_vData.emplace_back(std::move(std::make_pair( CString(hjc->code, sizeof(hjc->code)).Mid(1),  
			                                              CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1))));
		});		
	}

	if (nETF == acETFETN) 
	{
		for_each(_mapCODE.begin(), _mapCODE.end(), [&](auto item) {
			if ((item.second->ssgb == jmREITS || item.second->ssgb == jmSHIP || item.second->ssgb == jmINFRA))
				_vData.emplace_back(std::move(std::make_pair(CString(item.second->code, 12).TrimRight().Mid(1), CString(item.second->hnam, 40).TrimRight())));
			});
	}

	std::copy(_vData.begin(), _vData.end(), std::back_inserter(_vSelect));

	m_search = TRUE;

	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();
	m_EditSearch.SetFocus();

	if (nETF == acETFETN)
		listsort(0);
}

void CEtfDlg::searchAction(int column)
{
	CString	string; 
	CArray <_efopcode*,_efopcode*> searchData;
	std::vector<std::pair<CString, CString>> vSearch;
	
	if(m_search == TRUE)
		std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(vSearch));
	else
		std::copy(_vData.begin(), _vData.end(), std::back_inserter(vSearch));

	m_EditSearch.GetWindowText(string);

	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{
			CString		code, name;
			ListRemoveAll();
			
			for_each(_vSelect.begin(), _vSelect.end(), [&](const auto item){
				_vData.emplace_back(item);
			});	
			m_ListCtrl.SetItemCountEx(_vData.size());
			m_ListCtrl.Invalidate();

			m_bAddAll = true;
			listsort(0);
		}

		if (m_ListCtrl.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_eListsort = FALSE;
				listsort(0);

				idx = findMatchIndex(string, column);
				if (idx >= 0) 
				{
					m_ListCtrl.Selected(idx);
					m_szCode = m_ListCtrl.GetItemText(idx, 0);
					m_szName = m_ListCtrl.GetItemText(idx, 1);
					m_szName.TrimLeft();
				}
			}
			else
			{
				m_ListCtrl.Selected(0);
				m_szCode = m_ListCtrl.GetItemText(0, 0);
				m_szName = m_ListCtrl.GetItemText(0, 1);
				m_szName.TrimLeft();
			
			}
		}
		if(m_search == FALSE)
			InitialList(acETFETN);
		else
			SearchList();	
	}
	else			// name search
	{
		m_bAddAll = SearchData(0, string, m_bAddAll);
		if (m_ListCtrl.GetItemCount() > 0)
		{
			m_eListsort = FALSE;
			listsort(0);
			m_ListCtrl.Selected(0);
			m_szCode = m_ListCtrl.GetItemText(0, 0);
			m_szName = m_ListCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}
	}
}

int CEtfDlg::findMatchIndex(CString text, int column)
{
	CString	string;
	const int textL = text.GetLength();
	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		string = m_ListCtrl.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

bool CEtfDlg::SearchData(int kind, CString sName, bool bAddAll)
{
	bool result{};
	result = SearchJongmok(sName, bAddAll);
	if(result == false)
		return SearchWord(sName, bAddAll);

	return false;
}

bool CEtfDlg::SearchJongmok(CString sName, bool bAddAll)
{
	CString	code, name;
	std::vector<std::pair<CString, CString>> vSearch;
	
	bool bsearch = false;

	if(m_search == TRUE)
		std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(vSearch));
	else
		std::copy(_vData.begin(), _vData.end(), std::back_inserter(vSearch));

	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			ListRemoveAll();	
			std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(_vData));
		}

		m_ListCtrl.SetItemCountEx(_vData.size());
		m_ListCtrl.Invalidate();
		return true;
	}

	ListRemoveAll();
	_vData.clear();

	char cName{};
	const int sLen = sName.GetLength();
	if (sLen == 1)
	{	// 일반검색적용(길이만큼 맞는것만 적용)
		auto ft = std::remove_if(vSearch.begin(), vSearch.end(), [&](const auto item){
			name = item.second;

			CString strUpperS = sName;
			CString strUpperO = name;
			strUpperS.MakeUpper();
			strUpperO.MakeUpper();

			if (strncmp(strUpperS, strUpperO, sLen) != 0)
				return true;
			return false;	
		});

		if (ft != vSearch.end())
		{
			vSearch.erase(ft, vSearch.end());
			std::copy(vSearch.begin(), vSearch.end(), std::back_inserter(_vData));


			m_ListCtrl.SetItemCountEx(_vData.size());
			m_ListCtrl.Invalidate();
			return true;
		}
		return false;		
	}
	else
	{	// 한글 검색
		WORD	wHangul{}, wHangul2{}, wStart{}, wEnd{};		
		std::vector<std::pair<CString, CString>> vText;
		CPtrArray	arSearch;
		arSearch.RemoveAll();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
					if (item.second.GetLength() < 2)	
						return;
					wHangul2 = MakeHangul(item.second.GetAt(ii), item.second.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)			
						vText.emplace_back(item);			
				});
			}
			else
			{	// 한글 비교
				for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
					if (item.second.GetLength() < 2)	return;
					wHangul2 = MakeHangul(item.second.GetAt(ii), item.second.GetAt(ii+1));
					if (wHangul == wHangul2)	
						vText.emplace_back(item);	
				});
			}
			ii += 2;
		}
		else			// 그외는 1Byte
		{
			for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
				if (item.second.GetLength() < 1)	
					return;
				
				CString strUpperS, strUpperO;
				strUpperS = cName;

				if (ii < item.second.GetLength())
				{
					//strUpperO = item.second.GetAt(ii);
					//strUpperS.MakeUpper();
					//strUpperO.MakeUpper();

					//if (strUpperS == strUpperO)	
					//	vText.emplace_back(item);
				}
			});
			ii += 1;
		}

		for (; ii < sLen && vText.size();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// 한글일땐 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// 초성 비교
					auto ft = std::remove_if(vText.begin(), vText.end(), [&](const auto item){
						if (item.second.GetLength() < ii+2)	
							return true;
						
						wHangul2 = MakeHangul(item.second.GetAt(ii), item.second.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)
							return true;	

						return false;
					});
					
					if (ft != vText.end())
					{
						vText.erase(ft, vText.end());
					}
				}
				else
				{	// 한글 비교
					auto ft = std::remove_if(vText.begin(), vText.end(), [&](const auto item){
						if (item.second.GetLength() < ii+2)	
							return true;
						wHangul2 = MakeHangul(item.second.GetAt(ii), item.second.GetAt(ii+1));
						if (wHangul != wHangul2)
							return true;	

						return false;
					});
					
					if (ft != vText.end())
					{
						vText.erase(ft, vText.end());
					}
				}
				ii += 2;
			}
			else			// 그외는 1Byte
			{
				auto ft = std::remove_if(vText.begin(), vText.end(), [&](const auto item){
					if (item.second.GetLength() < ii+1)	
						return true;	
					CString strUpperS, strUpperO;
					strUpperS = cName;
					strUpperO = item.second.GetAt(ii);
					strUpperS.MakeUpper();
					strUpperO.MakeUpper();
					
					if (strUpperS != strUpperO)	
						return true;	

					return false;
				});

				if (ft != vText.end())
				{
					vText.erase(ft, vText.end());
				}
				ii += 1;
			}
		}


		if (vText.size() > 0)
		{
			bsearch = true;
			std::copy(vText.begin(), vText.end(), std::back_inserter(_vData));
		}

		m_ListCtrl.SetItemCountEx(_vData.size());
		m_ListCtrl.Invalidate();

		if (bsearch == true)
			return true;
		else
			return false;
	}	

	
	return false;
}

bool CEtfDlg::SearchWord(CString sName, bool bAddAll)
{
	CString	code, name;

	std::vector<std::pair<CString, CString>> vSearch;
	
	if(m_search == TRUE)
	{
	//	searchData.Copy(m_searchData);
		std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(vSearch));
	}
	else
	{
	//	searchData.Copy(m_listData);
		std::copy(_vData.begin(), _vData.end(), std::back_inserter(vSearch));
	}	

	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			ListRemoveAll();
			
			for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
				_vData.emplace_back(item);
			});

			m_ListCtrl.SetItemCountEx(_vData.size());
			m_ListCtrl.Invalidate();
			// for (int ii = 0; ii < searchData.GetSize(); ii++)
			// {
			// 	efopcode = (_efopcode*)searchData.GetAt(ii);
			// 	name = CString(efopcode->hnam, sizeof(efopcode->hnam));
			// 	//if (hjc->ssgb != jmELW)	
			// 	//	name = name.Mid(1);

			// 	code = CString(efopcode->code, sizeof(efopcode->code));
			// 	name.TrimRight();
			// 	AddItem(ii, code, name);
			// }
		}
		return true;
	}

	ListRemoveAll();

	std::vector<std::pair<CString, CString>> vText;

	for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
		name = item.second;
		name.MakeUpper();
		sName.MakeUpper();
		if (name.Find(sName) >= 0)
			vText.emplace_back(item);
	});

	for_each(vText.begin(), vText.end(), [&](const auto item){
		_vData.emplace_back(item);
	});

	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();

	return false;
}

bool CEtfDlg::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// 초성만 가진 한글인지~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;

	const WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
		0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
		0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};

	const WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
		0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
		0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	const WORD	pHangulEnd[19] = 
	{
			0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
		0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
		0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};
	int ii = 0;
	for (ii = 0; ii < 19; ii++)
	{
		if (wHangul == pChosung[ii])
		{
			wStart = pHangulStart[ii];
			wEnd = pHangulEnd[ii];
			return true;
		}			
	}

	wStart = pHangulStart[ii] - 1;
	wEnd = pHangulEnd[ii] + 1;

	return true;
}

WORD CEtfDlg::MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	const WORD wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}

void CEtfDlg::ListRemoveAll()
{
	m_ListCtrl.DeleteAllItems();
}

long CEtfDlg::OnEditXMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	CString	string;
	switch ((int) wParam)
	{
	case wpBLANK:
		{
			LRESULT* pResult = 0;
			OnClickTreeBunryu(nullptr,pResult);
		}
		//searchAction(0);	// blank name search by kwon
		break;
	case wpENTER:
		break;
	case wpSEARCH1:
		searchAction(/*0*/1);	// number search by kwon
		break;
	case wpSEARCH2:
		searchAction(1);	// char search by kwon
		break;
	case wpUP:
		if (m_ListCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
			if (pos == nullptr)
				break;
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			nItem--;
			if (nItem < 0)	nItem = 0;
			m_ListCtrl.Selected(nItem);
			m_szCode = m_ListCtrl.GetItemText(nItem, 0);
			m_szName = m_ListCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();
			
			m_EditSearch.SetFocus();
			m_EditSearch.SetSel(0, -1);
			m_EditSearch.SetSel(-1, -1);
		}
		break;
	case wpDOWN:
		if (m_ListCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
			if (pos == nullptr)
				break;
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			nItem++;
			if (nItem >= m_ListCtrl.GetItemCount())	nItem = m_ListCtrl.GetItemCount() - 1;
			m_ListCtrl.Selected(nItem);
			m_szCode = m_ListCtrl.GetItemText(nItem, 0);
			m_szName = m_ListCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_ListCtrl.SetFocus();
// 			m_EditSearch.SetFocus();
// 			m_EditSearch.SetSel(0, -1);
// 			m_EditSearch.SetSel(-1, -1);
		}
		break;
	}
	
	return 0;
}

BOOL CEtfDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static int row;
	
	if (wParam == IDC_LIST_CTRL)// <-- 여기서 그리드 컨트롤을 구분..
	{
		switch(((LPNMHDR) lParam)->code)
		{
		case LVN_COLUMNCLICK:
			m_eListsort = !m_eListsort;

			#define pnm ((LPNMLISTVIEW)lParam)
			listsort((LPARAM)pnm->iSubItem);	
			#undef pnm
			
			break;
		}
 	}			
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CEtfDlg::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const int idx = m_ListCtrl.GetSelectionMark();
	m_szCode = m_ListCtrl.GetItemText(idx, 0);
	m_szName = m_ListCtrl.GetItemText(idx, 1);
	
	SetCode(m_szCode);
	SetName(m_szName);
	
	m_szName.TrimLeft();	

}


void CEtfDlg::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClickListCtrl(pNMHDR, pResult);
	OnOK();
	

}

void CEtfDlg::OnSelchangeStaticSearch() 
{
	CString	string;
	m_EditSearch.GetWindowText(string);
	
	if (string.IsEmpty())
		searchAction(0);
	else
		searchAction(1);
	
	m_EditSearch.SetFocus();
	m_EditSearch.SetSel(0, -1);
	
}

void CEtfDlg::OnClickTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CPoint      point;   
    UINT        nFlags = 0;   
    
        GetCursorPos(&point);   
        ::ScreenToClient(m_Tree.m_hWnd, &point);   

        HTREEITEM   hItem = m_Tree.HitTest(point, &nFlags);   

	//2015.10.05 KSJ 직접호출해줄때는 NULL로 세팅해서 선택되어 있는 항목을 가져온다.
	if(pNMHDR == nullptr)
	{
		hItem = m_Tree.GetSelectedItem();
	}
	else
	{
		*pResult = 0;
	}

	const _TREEITEM* hChildItem = m_Tree.GetChildItem(hItem);	
	CString str = m_Tree.GetItemText(hItem);	

	_vSelect.clear();
	


	if (pNMHDR == nullptr || (hItem != nullptr && (nFlags == 4 && TVHT_ONITEMSTATEICON != 0)))   //2015.10.05 KSJ 직접호출해줄때는 NULL로 세팅해서 선택되어 있는 항목을 가져온다
	{   
		m_EditSearch.SetWindowText("");
		if(str.Compare("전체종목") == 0)
		{				
			InitialList(acETF);
			m_ListCtrl.Selected(0);
		}
		else if(str.Compare("ETF/ETN/리츠 전종목") == 0)
		{				
			InitialList(acETFETN);
			m_ListCtrl.Selected(0);
		}
		else if(str.Compare("ETN") == 0)
		{				
			InitialList(acETN);
			m_ListCtrl.Selected(0);
		}
		else if(str.Compare("ETF") == 0)
		{				
			m_ListCtrl.DeleteAllItems();
			m_ListCtrl.Selected(0);
		}
		else if (str.Compare("리츠종목") == 0)	 //test reit
		{
			InitReitList();
			m_ListCtrl.Selected(0);
		}

		if(hChildItem == nullptr && (str.Compare("전체종목") != 0) && (str.Compare("ETF/ETN/리츠 전종목") != 0) && (str.Compare("ETN") != 0) && (str.Compare("리츠종목") != 0))
		{
			DataGubn(hItem);
			m_ListCtrl.Selected(0);
			listsort(0);
		}
	}	
}

void CEtfDlg::DataGubn(HTREEITEM hItem)
{
	CString		gubn;
	CString		data;

	HTREEITEM hParentItem;
	
	hParentItem = m_Tree.GetParentItem(hItem);
	
	//m_searchData.RemoveAll();
	_vData.clear();
	m_ListCtrl.DeleteAllItems();

	gubn = m_Tree.GetItemText(hParentItem);
	data = m_Tree.GetItemText(hItem);

	if(gubn.Compare("운용사별") == 0)
	{
		DataReloadItem(data);
	}
	else if(gubn.Compare("기초자산별") == 0)	//2015.11.04 KSJ 테마별 --> 기초자산별로 변경
	{
		DataReloadTema(data);
	}
	else if(gubn.Compare("국내/외") == 0)
	{
		DataReloadFore(data);
	}

	m_search = TRUE;

 }

void CEtfDlg::DataReloadItem(CString data)
{
	CString cd;
	auto ft = std::find_if(_vETFitem.begin(), _vETFitem.end(), [&data](const auto item){
		if (data.CompareNoCase(std::get<2>(item)) == 0)
			return true;
		return false;
	});
	
	if (ft != _vETFitem.end())
		cd = std::get<1>(*ft);

	for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item){
		auto [type, code, hnam, opcd, etfm, etfl] = item;
		if (cd.CompareNoCase(opcd) == 0)
		{
			_vData.emplace_back(std::move(std::make_pair(code, hnam)));
			_vSelect.emplace_back(std::move(std::make_pair(code, hnam)));
		}
	});

	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();	
}

void CEtfDlg::DataReloadTema(CString data)
{
	char cd{};
	auto ft = std::find_if(_vETFtema.begin(), _vETFtema.end(), [&data](const auto item){
		if (data.CompareNoCase(std::get<2>(item)) == 0)
			return true;
		return false;
	});
	
	if (ft != _vETFtema.end())
		cd = std::get<1>(*ft);
	
	for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item){
		auto [type, code, hnam, opcd, etfm, etfl] = item;
		if (cd == etfm)
		{
			_vData.emplace_back(std::move(std::make_pair(code, hnam)));
			_vSelect.emplace_back(std::move(std::make_pair(code, hnam)));
		}
	});

	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();
}

void CEtfDlg::DataReloadFore(CString data)
{
	char cd{};
	auto ft = std::find_if(_vETFfore.begin(), _vETFfore.end(), [&data](const auto item){
		if (data.CompareNoCase(std::get<2>(item)) == 0)
			return true;
		return false;
	});
	
	if (ft != _vETFfore.end())
		cd = std::get<1>(*ft);
	
	for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item){
		auto [type, code, hnam, opcd, etfm, etfl] = item;
		if (cd == etfl)
		{
			_vData.emplace_back(std::move(std::make_pair(code, hnam)));
			_vSelect.emplace_back(std::move(std::make_pair(code, hnam)));
		}
	});

	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();
}

bool CEtfDlg::ExistCodeList(CString code)
{
	CString listcode;

	for(int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		listcode = m_ListCtrl.GetItemText(ii, 0);
		listcode.TrimLeft(); listcode.TrimRight();

		if(code.Compare(listcode) == 0)
			return true;
	}
	return false;
}

void CEtfDlg::SearchList()
{
	std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(_vData));
	m_ListCtrl.SetItemCountEx(_vData.size());
	m_ListCtrl.Invalidate();
}	


void CEtfDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	EndDialog(1);
}

BOOL CEtfDlg::PreTranslateMessage(MSG* pMsg) 
{
	NMHDR* pNMHDR{};
	LRESULT* pResult{};
	
	if(pMsg->message == WM_KEYDOWN)
	{
		
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			if(pMsg->hwnd == GetDlgItem(IDC_TREE_BUNRYU)->GetSafeHwnd())
			{
				HTREEITEM hItem = m_Tree.GetSelectedItem();
				CString str = m_Tree.GetItemText(hItem);

				if(str.Compare("전체종목") == 0)
				{
					InitialList(acETF);
				}
				else if(str.Compare("ETF/ETN/리츠 전종목") == 0)
				{				
					InitialList(acETFETN);
					m_ListCtrl.Selected(0);
				}
				else if(str.Compare("ETN") == 0)	
				{				
					InitialList(acETN);
					m_ListCtrl.Selected(0);
				}
				else if(str.Compare("ETF") == 0)
				{				
					m_ListCtrl.DeleteAllItems();
					m_ListCtrl.Selected(0);
				}

				if(!m_Tree.ItemHasChildren(hItem) && (str.Compare("전체종목") != 0) && (str.Compare("acETN") != 0) && (str.Compare("ETN") != 0)) 
				{
					DataGubn(hItem);
					m_ListCtrl.Selected(0);
				}
				else
				{
					if(m_Tree.GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED)
					{
						pMsg->wParam = VK_LEFT;
						PreTranslateMessage(pMsg);
					}
					else
					{
						pMsg->wParam = VK_RIGHT;
						PreTranslateMessage(pMsg);
					}

				}

					
			}
			if(pMsg->hwnd == GetDlgItem(IDC_EDIT_SEARCH)->GetSafeHwnd())
			{
				OnClickListCtrl(pNMHDR, pResult);
				OnOK();
			}
			if(pMsg->hwnd == GetDlgItem(IDC_LIST_CTRL)->GetSafeHwnd())
			{
				
				OnClickListCtrl(pNMHDR, pResult);
				OnOK();
			}
			return TRUE;
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		default:
			m_EditSearch.SetFocus();
			break;

		}

	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CEtfDlg::SetDlgPos(int x, int y)
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


void CEtfDlg::OnGetdispinfoListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;

	const auto& [code, name] = _vData.at(pItem->iItem);

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

void CEtfDlg::listsort(int colum)
{
	std::sort(_vData.begin(), _vData.end(), [colum](const auto item1, const auto item2){
		switch (colum)
		{
		case 1:
			if (m_eListsort)
				return (item1.second.Compare(item2.second) > 0);
			else 
				return (item1.second.Compare(item2.second) < 0);

			break;

		case 0:
		default:
			if (m_eListsort)
				return (item1.first.Compare(item2.first) > 0);
			else 
				return (item1.first.Compare(item2.first) < 0);

			break;
		}
	});

	m_ListCtrl.Invalidate();
}
