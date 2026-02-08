// USCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "USCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	US_TOPCODE_FILE	"uscode_top.dat"

#define NAME_KOR	0
#define NAME_ENG	1

#define BTN_KOR		_T("한글")
#define BTN_ENG		_T("영문")

#define I_JISU		'0'	// 지수
#define I_ITEM		'1'	// 종목
#define I_FUTURE	'2'	// 선물
#define I_RATE		'3'	// 금리
#define I_DEBT		'4'	// 국채
#define I_EXCHANGE	'5'	// 환율
#define I_ETC		'6'	// 기타

#define I_JISU_USA	'1'	// 미국
#define I_JISU_ASIA	'2'	// 아시아
#define I_JISU_EUR	'3'	// 유럽
#define I_JISU_MSCI	'4'	// MSCI
#define I_JISU_ETC	'5'	// 기타

#define I_ITEM_USA	'1'	// 미국
#define I_ITEM_JAPAN	'2'	// 일본
#define I_ITEM_HONGKONG	'3'	// 홍콩
#define I_ITEM_TAIWAN	'4'	// 대만
#define I_ITEM_UK	'5'	// 영국
#define I_ITEM_FRANCE	'6'	// 프랑스
#define I_ITEM_GERMAN	'7'	// 독일
#define I_ITEM_ADR	'8'	// ADR
#define I_ITEM_GDR	'9'	// GDR

#define I_FUTURE_CME	'1'	// CME
#define I_FUTURE_ENERGY	'2'	// 에너지
#define I_FUTURE_GOODS	'3'	// 상품선물

#define I_RATE_LIBO	'1'	// 리보금리
#define I_RATE_USA	'2'	// USA

#define I_DEBT_USA	'1'	// 미국국채
#define I_DEBT_JAPAN	'2'	// 일본
#define I_DEBT_UK	'3'	// 영국
#define I_DEBT_GERMAN	'4'	// 독일

#define I_EXCHANGE_INT	'1'	// 국제환율

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg dialog

bool	m_listsort;

CUSCodeDlg::CUSCodeDlg(CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(CUSCodeDlg::IDD, pParent)
{
	m_point = pt;

	m_listsort = true;
	m_nameFlag = NAME_KOR;
	m_CodeKind = ckTop;

	m_root = ((CAxisCodeApp*)AfxGetApp())->m_root;

	m_rtnData.code = _T("");
	m_rtnData.name = _T("");

	//{{AFX_DATA_INIT(CUSCodeDlg)
	//}}AFX_DATA_INIT
}


void CUSCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSCodeDlg)
	DDX_Control(pDX, IDC_BTN_TOP, m_btn_top);
	DDX_Control(pDX, IDC_BTN_RATE, m_btn_rate);
	DDX_Control(pDX, IDC_BTN_JISU, m_btn_jisu);
	DDX_Control(pDX, IDC_BTN_ITEM, m_btn_item);
	DDX_Control(pDX, IDC_BTN_FUTURE, m_btn_future);
	DDX_Control(pDX, IDC_BTN_EXCHANGE, m_btn_exchange);
	DDX_Control(pDX, IDC_BTN_ENG, m_btn_eng);
	DDX_Control(pDX, IDC_BTN_DEBT, m_btn_debt);
	DDX_Control(pDX, IDC_CBO_SLIST, m_cbo_slist);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edit_search);
	DDX_Control(pDX, IDC_BTN_SORT14, m_btn_sort14);
	DDX_Control(pDX, IDC_BTN_SORT13, m_btn_sort13);
	DDX_Control(pDX, IDC_BTN_SORT12, m_btn_sort12);
	DDX_Control(pDX, IDC_BTN_SORT11, m_btn_sort11);
	DDX_Control(pDX, IDC_BTN_SORT10, m_btn_sort10);
	DDX_Control(pDX, IDC_BTN_SORT09, m_btn_sort09);
	DDX_Control(pDX, IDC_BTN_SORT08, m_btn_sort08);
	DDX_Control(pDX, IDC_BTN_SORT07, m_btn_sort07);
	DDX_Control(pDX, IDC_BTN_SORT06, m_btn_sort06);
	DDX_Control(pDX, IDC_BTN_SORT05, m_btn_sort05);
	DDX_Control(pDX, IDC_BTN_SORT04, m_btn_sort04);
	DDX_Control(pDX, IDC_BTN_SORT03, m_btn_sort03);
	DDX_Control(pDX, IDC_BTN_SORT02, m_btn_sort02);
	DDX_Control(pDX, IDC_BTN_SORT01, m_btn_sort01);
	DDX_Control(pDX, IDC_BTN_ALL, m_btn_all);
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CUSCodeDlg)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_CBN_SELCHANGE(IDC_CBO_SLIST, OnSelchangeCboSlist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_ALL, IDC_BTN_TOP, OnClickBtnKind)
	ON_COMMAND_RANGE(IDC_BTN_SORT01, IDC_BTN_ENG, OnClickBtnSort)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_CODE, &CUSCodeDlg::OnGetdispinfoListCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSCodeDlg message handlers

void CUSCodeDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	m_CtrlFont.DeleteObject();
	_sBuffer.Empty();
}

BOOL CUSCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));
	
	LoadTopCode();
	LoadUSCode();
	InitCtrl();

	SetBtn();
	OnClickBtnKind(IDC_BTN_TOP);

	SetDlgPos(m_point.x, m_point.y);

	return TRUE;
}

void CUSCodeDlg::SetDlgPos(int x, int y)
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

CString CUSCodeDlg::GetCountry(CString ncod)
{
	const struct _ncodInfo {
		char*	code;
		char*	name;
	} ncodInfo[] = {
		{"100", "한국"}, {"101", "일본"}, {"102", "홍콩"}, {"103", "대만"},
		{"104", "중국"}, {"105", "싱가폴"}, {"106", "호주"}, {"107", "인도네시아"},
		{"108", "태국"}, {"109", "필리핀"}, {"110", "말레이지아"}, {"111", "인도"},
		{"112", "스리랑카"}, {"113", "파키스탄"}, {"202", "뉴질랜드"}, {"301", "영국"},
		{"302", "프랑스"}, {"303", "독일"}, {"304", "이탈리아"}, {"305", "네델란드"},
		{"306", "스위스"}, {"307", "스페인"}, {"308", "포르투갈"}, {"309", "벨기에"},
		{"310", "덴마크"}, {"311", "그리스"}, {"312", "폴란드"}, {"313", "터키"},
		{"314", "필란드"}, {"315", "러시아"}, {"316", "스웨덴"}, {"317", "오스트리아"},
		{"318", "크로아티아"}, {"319", "헝가리"}, {"320", "리투아니아"}, {"321", "노르웨이"},
		{"322", "루마니아"}, {"501", "미국"}, {"502", "캐나다"}, {"503", "멕시코"},
		{"504", "브라질"}, {"505", "아르헨티나"}, {"506", "칠레"}, {"507", "페루"},
		{"609", "짐바브웨"}
	};

	CString rValue = _T("");
	for (int ii = 0 ; ii < sizeof(ncodInfo) / sizeof(_ncodInfo) ; ii++)
	{
		if (ncodInfo[ii].code == ncod)
		{
			rValue = ncodInfo[ii].name;
			break;
		}
	}

	return rValue;
}

void CUSCodeDlg::ResetCboItem(enum _codeKind codeKind)
{
	m_cbo_slist.ResetContent();
	switch (codeKind)
	{
	case ckAll:
		m_cbo_slist.AddString("전종목");
		break;
	case ckJisu:
		m_cbo_slist.AddString("전체");
		m_cbo_slist.AddString("미국");
		m_cbo_slist.AddString("아시아");
		m_cbo_slist.AddString("유럽");
		m_cbo_slist.AddString("MSCI");
		m_cbo_slist.AddString("기타");
		break;
	case ckItem:
		m_cbo_slist.AddString("전체");
		m_cbo_slist.AddString("미국");
		m_cbo_slist.AddString("일본");
		m_cbo_slist.AddString("홍콩");
		m_cbo_slist.AddString("대만");
		m_cbo_slist.AddString("영국");
		m_cbo_slist.AddString("프랑스");
		m_cbo_slist.AddString("독일");
		m_cbo_slist.AddString("ADR");
		m_cbo_slist.AddString("GDR");
		break;
	case ckFuture:
		m_cbo_slist.AddString("전체");
		m_cbo_slist.AddString("CME지수선물");
		m_cbo_slist.AddString("에너지");
		m_cbo_slist.AddString("상품선물");
		break;
	case ckRate:
		m_cbo_slist.AddString("전체");
		m_cbo_slist.AddString("리보금리");
		m_cbo_slist.AddString("미국금리");
		break;
	case ckDebt:
		m_cbo_slist.AddString("전체");
		m_cbo_slist.AddString("미국");
		m_cbo_slist.AddString("일본");
		m_cbo_slist.AddString("영국");
		m_cbo_slist.AddString("독일");
		break;
	case ckExchange:
		m_cbo_slist.AddString("전체"); // 국제환율
		break;
	case ckTop:
		m_cbo_slist.AddString("주요지수"); // 국제환율
		break;
	//case ckEtc:
	//	break;
	}
	m_cbo_slist.SetCurSel(0);
}

void CUSCodeDlg::SelectCode(enum _codeKind codeKind, short subKind)
{
	_vSelect.clear();
	if (codeKind == ckAll)
	{
		for_each(_vUSCode.begin(), _vUSCode.end(), [&](const auto usCode){
			if (usCode->dat1 == I_JISU || usCode->dat1 == I_ITEM
			  || usCode->dat1 == I_FUTURE || usCode->dat1 == I_RATE
			  || usCode->dat1 == I_DEBT || usCode->dat1 == I_EXCHANGE
			  /*|| m_aUSCode[ii]->dat1 == I_ETC*/)
				_vSelect.emplace_back(std::move(std::make_tuple(
					CString(usCode->symb, sizeof(usCode->symb)).Trim(),
					CString(usCode->knam, sizeof(usCode->knam)).Trim(),
					CString(usCode->enam, sizeof(usCode->enam)).Trim(),
					GetCountry(CString(usCode->ncod, sizeof(usCode->ncod)).Trim())
				)));
		});
	}
	else if (codeKind == ckTop)
	{

		for_each(_vUSCode.begin(), _vUSCode.end(), [&](const auto usCode){
			if (usCode->dat1 == I_JISU || usCode->dat1 == I_ITEM
				|| usCode->dat1 == I_FUTURE || usCode->dat1 == I_RATE
				|| usCode->dat1 == I_DEBT || usCode->dat1 == I_EXCHANGE
				/*|| m_aUSCode[ii]->dat1 == I_ETC*/)
			{
				CString symb = CString(usCode->symb, sizeof(usCode->symb)).Trim();
				const auto ft = _mTop.find(symb);
				if (ft != _mTop.end())
				{
					_vSelect.emplace_back(std::move(std::make_tuple(
						CString(usCode->symb, sizeof(usCode->symb)).Trim(),
						CString(usCode->knam, sizeof(usCode->knam)).Trim(),
						CString(usCode->enam, sizeof(usCode->enam)).Trim(),
						GetCountry(CString(usCode->ncod, sizeof(usCode->ncod)).Trim())
					)));
				}
			}
		});
	}
	else
	{
		char dat1{}, dat2{};
		dat1 = '0' + (int)codeKind;
		dat2 = '0' + (int)subKind;
		for_each(_vUSCode.begin(), _vUSCode.end(), [&](const auto usCode) {
			if (usCode->dat1 == dat1 && (subKind == 0 || usCode->dat2 == dat2))
				_vSelect.emplace_back(std::move(std::make_tuple(
					CString(usCode->symb, sizeof(usCode->symb)).Trim(),
					CString(usCode->knam, sizeof(usCode->knam)).Trim(),
					CString(usCode->enam, sizeof(usCode->enam)).Trim(),
					GetCountry(CString(usCode->ncod, sizeof(usCode->ncod)).Trim())
				)));
			});
	}
	
	_vData.clear();
	std::copy(_vSelect.begin(), _vSelect.end(), std::back_inserter(_vData));

}

void CUSCodeDlg::SetCodeList(int kind)
{
	ReSizeCodeList(kind);	

	switch (kind)
	{
	case IDC_BTN_ALL:
		m_CodeKind = ckAll;
		break;
	case IDC_BTN_JISU: // 지수
		m_CodeKind = ckJisu;
		break;
	case IDC_BTN_ITEM: // 종목
		m_CodeKind = ckItem;
		break;
	case IDC_BTN_FUTURE: // 선물
		m_CodeKind = ckFuture;
		break;
	case IDC_BTN_RATE: // 금리
		m_CodeKind = ckRate;
		break;
	case IDC_BTN_DEBT: // 국채
		m_CodeKind = ckDebt;
		break;
	case IDC_BTN_EXCHANGE: // 환율
		m_CodeKind = ckExchange;
		break;
	case IDC_BTN_TOP: // 주요지수
		m_CodeKind = ckTop;
		break;
	}
	ResetCboItem(m_CodeKind);
	SelectCode(m_CodeKind, 0);


	m_listsort = false;
	listsort(0);

	m_ListCtlCode.SetItemCountEx(_vData.size());
	m_ListCtlCode.Invalidate();
}

void CUSCodeDlg::OnSelchangeCboSlist() 
{
	SelectCode(m_CodeKind, m_cbo_slist.GetCurSel());
	m_ListCtlCode.SetItemCountEx(_vData.size());
	m_ListCtlCode.Invalidate();
	
}

void CUSCodeDlg::ReSizeCodeList(int kind)
{
	switch (kind)
	{
	case IDC_BTN_ALL:
	case IDC_BTN_TOP:
		{
			CRect rcList;
			m_ListCtlCode.GetWindowRect(rcList); ScreenToClient(rcList);
			rcList.top = 21;
			
			m_ListCtlCode.MoveWindow(rcList);
			m_cbo_slist.ShowWindow(SW_HIDE);
		}
		break;
	default:
		{
			CRect rcList;
			m_ListCtlCode.GetWindowRect(rcList); ScreenToClient(rcList);
			rcList.top = 41;
			
			m_ListCtlCode.MoveWindow(rcList);
			m_cbo_slist.ShowWindow(SW_SHOW);
		}
		break;
	}
}


void CUSCodeDlg::InitCtrl()
{
	m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "코드", LVCFMT_LEFT, 80);
	m_ListCtlCode.InsertColumn(2, "한글명", LVCFMT_LEFT, 117);
	m_ListCtlCode.InsertColumn(3, "영문명", LVCFMT_LEFT, 110);

	m_cbo_slist.AddString("전종목");
	m_cbo_slist.SetCurSel(0);
}

BOOL CUSCodeDlg::LoadUSCode()
{
	if (_sBuffer.GetLength() > 0)
		return TRUE;

	TRY
	{
		CString		path = m_root + "/tab/" + USCODE_FILE;
		CFile		file(path, CFile::modeRead | CFile::shareDenyNone);

		const UINT  size = gsl::narrow_cast<UINT>(file.GetLength());		
		const UINT  nBytesRead = file.Read(_sBuffer.GetBuffer(size + 1), size);
		file.Close();
		
		if (size > nBytesRead)
	 		return FALSE;

		const int count = size / sizeof(struct _USCode); 
		_USCode *pCode = (struct _USCode*)_sBuffer.GetBuffer();
		for (int ii = 0; ii < count; ii++, pCode++)
			_vUSCode.emplace_back(std::move(pCode));

	}CATCH(CFileException, e)
	{
		#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
		#endif
	}
	END_CATCH
		
	return TRUE;
}

BOOL CUSCodeDlg::LoadTopCode()
{
	vector<CString> ss;
	CString sBuffer;
	CString	path = m_root + "/tab/" + US_TOPCODE_FILE;
	CFile file(path, CFile::modeRead|CFile::shareDenyNone);
	
	const int size = gsl::narrow_cast<int>(file.GetLength());
	const int nRead = file.Read(sBuffer.GetBuffer(size + 1), size);
	sBuffer.ReleaseBuffer();
	file.Close();
	if (nRead < size) return FALSE;

	AxStd::SplitSub(ss, sBuffer, "\r\n");

	for_each(ss.begin(), ss.end(), [&](const auto item){
		_mTop.emplace(std::move(std::make_pair(item, "")));
	});

	return TRUE;
}


LOGFONT CUSCodeDlg::SetFont(int Size)
{
	LOGFONT		Font;

	ZeroMemory(&Font, sizeof(LOGFONT));
	Font.lfHeight		= Size * 10;
	Font.lfCharSet		= DEFAULT_CHARSET;
	Font.lfWeight		= FW_NORMAL;
	Font.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	Font.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	Font.lfQuality		= DEFAULT_QUALITY;
	Font.lfPitchAndFamily	= DEFAULT_PITCH; 
	Font.lfItalic		= false;
	strcpy_s(Font.lfFaceName, "굴림");

	return Font;
}


BOOL CUSCodeDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	switch(wParam)
	{
	case IDC_LIST_CODE:
		{
			switch(((LPNMHDR) lParam)->code)
			{
			case LVN_COLUMNCLICK:
				#define pnm ((LPNMLISTVIEW)lParam)
				m_listsort = !m_listsort;
				listsort((LPARAM)pnm->iSubItem);
				#undef pnm
				break;
			}			
		}
		break;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CUSCodeDlg::listsort(int type)
{
	std::sort(_vData.begin(), _vData.end(), [type](const auto item1, const auto item2) {
		switch (type)
		{
		case 1:
			if (m_listsort)
				return (std::get<1>(item1).Compare(std::get<1>(item2)) > 0);
			else 
				return (std::get<1>(item1).Compare(std::get<1>(item2)) < 0);
			break;
		case 2:
			if (m_listsort)
				return (std::get<2>(item1).Compare(std::get<2>(item2)) > 0);
			else 
				return (std::get<2>(item1).Compare(std::get<2>(item2)) < 0);
			break;
		case 0:
		default:
			if (m_listsort)
				return (std::get<0>(item1).Compare(std::get<0>(item2)) > 0);
			else
				return (std::get<0>(item1).Compare(std::get<0>(item2)) < 0);
			break;
		}
	});

	m_ListCtlCode.Invalidate();
}


void CUSCodeDlg::SetBtn()
{
	const short	shBtnColor = 245;
	
	for (int ii = IDC_BTN_SORT01 ; ii <= IDC_BTN_TOP ; ii++)
	{
		((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CButtonST*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
	}
}

void CUSCodeDlg::OnClickBtnKind(UINT nID)
{
	for (int ii = IDC_BTN_ALL ; ii <= IDC_BTN_TOP ; ii++)
	{
		if ((int)nID == ii)
		{
			((CButtonST*)GetDlgItem(ii))->SetBold(TRUE);
			((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
			((CButtonST*)GetDlgItem(ii))->SetSelected(TRUE);
		}
		else
		{
			((CButtonST*)GetDlgItem(ii))->SetBold(FALSE);
			((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
			((CButtonST*)GetDlgItem(ii))->SetSelected(FALSE);
		}
	}	
	SetCodeList(nID);
}

void CUSCodeDlg::OnClickBtnSort(UINT nID)
{
	switch (nID)
	{
	case IDC_BTN_ENG:
		{
			m_nameFlag = !m_nameFlag;
			if (m_nameFlag)
				((CButtonST*)GetDlgItem(nID))->SetWindowText(BTN_KOR);
			else
				((CButtonST*)GetDlgItem(nID))->SetWindowText(BTN_ENG);
		}
		break;
	default:
		{
			if (!m_nameFlag)
			{
				const struct _sortInfo {
					char*	str;
					int	len;
				} sortInfo[] = {
					{"가", 2}, {"나", 2}, {"다", 2}, {"라", 2}, {"마", 2},
					{"바", 2}, {"사", 2}, {"아", 2}, {"자", 2}, {"차", 2},
					{"카", 2}, {"타", 2}, {"파", 2}, {"하", 2}, {"A", 1}, {"1", 1}
				};

				const int bSel = nID - IDC_BTN_SORT01;

				m_listsort = FALSE;
				listsort(1);				
				setSelected(&m_ListCtlCode, findIndex(sortInfo[bSel].str, sortInfo[bSel].len));
			}
		}
		break;
	}
}

int CUSCodeDlg::findIndex(CString text, int len, int column /* = 1 */)
{
	auto ft = std::find_if(_vData.begin(), _vData.end(), [&text, len](const auto item){
		CString name = std::get<1>(item);
		if (strncmp(text, name, len) <= 0)
			return true;
		return false;
	});
	if (ft != _vData.end())
	{
		killSelected(&m_ListCtlCode);
		return (ft - _vData.begin());
	}
	
	return -1;
}

void CUSCodeDlg::setSelected(CListCtrl* ListCtrl, int nItem, BOOL scroll /* = TRUE */)
{
	if (nItem < 0)
	{
		m_rtnData.code = _T("");
		m_rtnData.name = _T("");
		return;
	}

	CRect	rc;
	CSize	size;

	ListCtrl->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		ListCtrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - ListCtrl->GetTopIndex());
		ListCtrl->Scroll(size);
	}

	CString code = m_ListCtlCode.GetItemText(nItem, 0);
	CString	name = m_ListCtlCode.GetItemText(nItem, 1);

	m_rtnData.code = code;
	m_rtnData.name = name;
}

void CUSCodeDlg::killSelected(CListCtrl* ListCtrl)
{
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			int	nItem;
			nItem = ListCtrl->GetNextSelectedItem(pos);
			ListCtrl->SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CUSCodeDlg::OnChangeEditSearch() 
{
	CString str;
	m_edit_search.GetWindowText(str);

	m_listsort = FALSE;
	listsort(1);
	str.MakeUpper();
	setSelected(&m_ListCtlCode, findIndex(str, str.GetLength()));	
}

void CUSCodeDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem{};
	CString	code, name;
	POSITION pos = m_ListCtlCode.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = m_ListCtlCode.GetNextSelectedItem(pos);

			code = m_ListCtlCode.GetItemText(nItem, 0);
			name = m_ListCtlCode.GetItemText(nItem, 1);

			m_rtnData.code = code;
			m_rtnData.name = name;
		}
	}

	OnOK();
	
	*pResult = 0;
}

CString CUSCodeDlg::GetRtnData(bool dialog /* = true */, CString code /* = _T("") */)
{
	CString rValue = _T("");

	if (dialog)
	{
		if (!m_rtnData.code.IsEmpty() && !m_rtnData.name.IsEmpty())
			rValue.Format("%s\t%s", m_rtnData.code, m_rtnData.name);
	}
	else
	{
		CString	symb, name;
		const int	nSize = m_aUSCode.GetSize();
		for (int ii = 0 ; ii < nSize ; ii++)
		{
			symb = CString(m_aUSCode[ii]->symb, sizeof(m_aUSCode[ii]->symb));
			if (code.Compare(symb) == 0)
			{
				name = CString(m_aUSCode[ii]->knam, sizeof(m_aUSCode[ii]->knam));
				rValue.Format("%s\t%s", code, name);
			}
		}
	}
	
	return rValue;
}

void CUSCodeDlg::OnGetdispinfoListCode(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;

	const auto& [code, name, ename, country] = _vData.at(pItem->iItem);
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
			lstrcpy(pItem->pszText, ename);
			break;
		break;
		}
	}
	*pResult = 0;
}
