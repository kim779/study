#include "stdafx.h"
#include "AxisCode.h"
#include "JongmukDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	cTAB	'\t'

// button select
#define	BTN_TOTAL	1
#define BTN_KOSPI	2
#define BTN_KOSDAQ	3
#define BTN_FUTURE	4
#define BTN_USCODE	5
#define BTN_KPIUP	6
#define BTN_KDQUP	7
#define BTN_FAVOR	8
#define	BTN_SECUP	9
#define	BTN_ELW		10

// combo선택
#define	ALLCODE	       	0
#define KOSPICODE	1
#define KOSDAQCODE     -1
#define FAVORITECODE	2
#define OTHERCODE	3
#define	ELWCODE		4
#define FREEBOARDCODE	5

// 검색combo선택
#define	SR_FRONT	0
#define	SR_WORD		1

#define UPCODE_KOSPI	"0"
#define UPCODE_KOSDAQ	"2"
#define	UPCODE_SECTOR	"5"
#define	UPCODE_INTEGRATE "9"
#define UPCODE_FREEBOARD "6"

#define KPI_TOTAL	_T("001")
#define KPI_TOTAL_STR	_T("종       합")
#define KDQ_TOTAL	_T("201")
#define KDQ_TOTAL_STR	_T("종합")
#define KPI200		_T("101")
#define CODE_DOW	_T(".DJI")
#define CODE_NAS	_T("COMP")
#define STR_KOSPI	_T("(거)")
#define STR_KOSDAQ	_T("(코)")

#define EXKOSPI		0
#define EXKOSDAQ	1

#define	secCODE		ecnCODE

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ


int CALLBACK ListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) ;

CJongmukDlg::CJongmukDlg(int kind, int type, CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CJongmukDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJongmukDlg)
	//}}AFX_DATA_INIT
	
	// test
	//kind = kospi2CODE;	// all2CODE, kkCODE, kospiCODE, allCODE, indexCODE, secCODE, thirdCODE, kospi2CODE

	m_aTemp.RemoveAll();
	m_arrayFavor.RemoveAll();
	m_arSelCode.RemoveAll();

	m_Select = 0;
	m_szCode = "";
	m_szName = "";
	m_kind = kind;
	m_Type = type;

	m_gListsort = FALSE;
	
	m_Point = pt;

	m_nRadioOption = 0;
	m_bOption = FALSE;

	m_btnSel = BTN_TOTAL;

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;
	m_sUser = ((CAxisCodeApp*)AfxGetApp())->m_Myfolder;
	m_bAddAll = true;
}

void CJongmukDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJongmukDlg)
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_ctlComboSearch);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edtSearch);
	DDX_Control(pDX, IDC_BUTTON_9, m_ctlBtn9);
	DDX_Control(pDX, IDC_BUTTON_8, m_ctlBtn8);
	DDX_Control(pDX, IDC_BUTTON_7, m_ctlBtn7);
	DDX_Control(pDX, IDC_BUTTON_6, m_ctlBtn6);
	DDX_Control(pDX, IDC_BUTTON_5, m_ctlBtn5);
	DDX_Control(pDX, IDC_BUTTON_4, m_ctlBtn4);
	DDX_Control(pDX, IDC_BUTTON_3, m_ctlBtn3);
	DDX_Control(pDX, IDC_BUTTON_22, m_ctlBtn22);
	DDX_Control(pDX, IDC_BUTTON_21, m_ctlBtn21);
	DDX_Control(pDX, IDC_BUTTON_20, m_ctlBtn20);
	DDX_Control(pDX, IDC_BUTTON_2, m_ctlBtn2);
	DDX_Control(pDX, IDC_BUTTON_19, m_ctlBtn19);
	DDX_Control(pDX, IDC_BUTTON_18, m_ctlBtn18);
	DDX_Control(pDX, IDC_BUTTON_17, m_ctlBtn17);
	DDX_Control(pDX, IDC_BUTTON_16, m_ctlBtn16);
	DDX_Control(pDX, IDC_BUTTON_15, m_ctlBtn15);
	DDX_Control(pDX, IDC_BUTTON_14, m_ctlBtn14);
	DDX_Control(pDX, IDC_BUTTON_13, m_ctlBtn13);
	DDX_Control(pDX, IDC_BUTTON_12, m_ctlBtn12);
	DDX_Control(pDX, IDC_BUTTON_10, m_ctlBtn10);
	DDX_Control(pDX, IDC_BUTTON_11, m_ctlBtn11);
	DDX_Control(pDX, IDC_BUTTON_1, m_ctlBtn1);
	DDX_Control(pDX, IDC_BUTTON_ETF, m_ctlButtonETF);
	DDX_Control(pDX, IDC_BUTTON_UPFREEBOARD, m_ctlFreeBoard);
	DDX_Control(pDX, IDC_LIST_PUT, m_ListPut);
	DDX_Control(pDX, IDC_LIST_CALL, m_ListCall);
	DDX_Control(pDX, IDC_BUTTON_FUTURE4, m_CtlButFuture4);
	DDX_Control(pDX, IDC_BUTTON_FUTURE3, m_CtlButFuture3);
	DDX_Control(pDX, IDC_BUTTON_FUTURE2, m_CtlButFuture2);
	DDX_Control(pDX, IDC_BUTTON_FUTURE1, m_CtlButFuture1);
	DDX_Control(pDX, IDC_LIST_CTRL, m_ListCtrl);
	DDX_Control(pDX, IDC_BUTTON_THIRDMARKET, m_ctlButtonTHIRD);
	DDX_Control(pDX, IDC_BUTTON_ELW, m_ctlButtonELW);
	DDX_Control(pDX, IDC_BUTTON_KOSPIWORK, m_ctlButtonKOSPIWORK);
	DDX_Control(pDX, IDC_BUTTON_KOSPICODE, m_ctlButtonKOSPI);
	DDX_Control(pDX, IDC_BUTTON_KOSDAQWORK, m_ctlButtonKOSDAQWORK);
	DDX_Control(pDX, IDC_BUTTON_KOSDAQCODE, m_ctlButtonKOSDAQ);
	DDX_Control(pDX, IDC_BUTTON_FAVORITES, m_ctlButtonFavor);
	DDX_Control(pDX, IDC_BUTTON_ECNCODE, m_ctlButtonECN);
	DDX_Control(pDX, IDC_BUTTON_ALLCODE, m_ctlButtonAll);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_ComboGroup);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJongmukDlg, CDialog)
	//{{AFX_MSG_MAP(CJongmukDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ALLCODE, OnButtonAllcode)
	ON_BN_CLICKED(IDC_BUTTON_KOSPICODE, OnButtonKospicode)
	ON_BN_CLICKED(IDC_BUTTON_KOSDAQCODE, OnButtonKosdaqcode)
	ON_BN_CLICKED(IDC_BUTTON_ECNCODE, OnButtonSecCode)
	ON_BN_CLICKED(IDC_BUTTON_THIRDMARKET, OnButtonThirdmarket)
	ON_BN_CLICKED(IDC_BUTTON_ELW, OnButtonElw)
	ON_BN_CLICKED(IDC_BUTTON_KOSPIWORK, OnButtonKospiwork)
	ON_BN_CLICKED(IDC_BUTTON_KOSDAQWORK, OnButtonKosdaqwork)
	ON_BN_CLICKED(IDC_BUTTON_FAVORITES, OnButtonFavorites)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelchangeComboGroup)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH, OnKillfocusEditSearch)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL, OnClickListCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CTRL, OnDblclkListCtrl)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_FUTURE1, OnButtonFuture1)
	ON_BN_CLICKED(IDC_BUTTON_FUTURE2, OnButtonFuture2)
	ON_BN_CLICKED(IDC_BUTTON_FUTURE3, OnButtonFuture3)
	ON_BN_CLICKED(IDC_BUTTON_FUTURE4, OnButtonFuture4)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE1, OnDoubleclickedButtonFuture1)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE2, OnDoubleclickedButtonFuture2)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE3, OnDoubleclickedButtonFuture3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_FUTURE4, OnDoubleclickedButtonFuture4)
	ON_LBN_DBLCLK(IDC_LIST_CALL, OnDblclkListCall)
	ON_LBN_DBLCLK(IDC_LIST_PUT, OnDblclkListPut)
	ON_BN_CLICKED(IDC_BUTTON_UPFREEBOARD, OnButtonUpfreeboard)
	ON_BN_CLICKED(IDC_BUTTON_ETF, OnButtonEtf)
	ON_CBN_SELENDOK(IDC_COMBO_SEARCH, OnSelendokComboSearch)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO4, OnRadio)
	ON_COMMAND_RANGE(IDC_BUTTON_1, IDC_BUTTON_22, OnClickBtnSort)
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJongmukDlg message handlers

BOOL CJongmukDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rect(100, 40, 304, 292);

	SetDlgPos(m_Point.x, m_Point.y);

	m_Font.CreateFont(12, "굴림체");
	m_ListCtrl.SetFont(&m_Font);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListCtrl.SetBkColor(COR_WHITE);
	m_ListCtrl.SetTextBkColor(COR_WHITE);
	m_ListCtrl.SetTextColor(RGB(0,0,0));
	m_ListCtrl.InsertColumn(1, "코드", LVCFMT_LEFT, 50);
	m_ListCtrl.InsertColumn(2, "종목명", LVCFMT_LEFT, 132);	
	m_ListCtrl.SetCustomDrawList(FALSE);


	CString strImg("");
	strImg.Format("%s/%s/", m_sRoot, "image");
	m_ctlComboSearch.SetFont(&m_Font);
	m_ctlComboSearch.LoadImg(strImg);
	m_ctlComboSearch.SetItemHeight(-1,14);
	m_ctlComboSearch.SetCurSel(0);

	BtnSetup();
	Part();

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	Set_KS_HanMode();

	return TRUE;
}

void CJongmukDlg::GetExCode()
{
	m_exCode[EXKOSPI].RemoveAll();
	m_exCode[EXKOSDAQ].RemoveAll();

	struct _infop {
		char* code;
		char* name;
	} infop[] = {
		{"101", "KOSPI200"},	{"401", "KOSPI100"},
		{"402", "KOSPI50"}
	};

	struct _infod {
		char* code;
		char* name;
	} infod[] = {
		{"303", "KOSTAR"}
	};

	_exCode	exCode;

	for (int ii = 0 ; ii < sizeof(infop) / sizeof(_infop) ; ii++)
	{
		exCode.code = infop[ii].code;
		exCode.name = infop[ii].name;

		m_exCode[EXKOSPI].Add(exCode);
	}

	for (int jj = 0 ; jj < sizeof(infod) / sizeof(_infod) ; jj++)
	{
		exCode.code = infod[jj].code;
		exCode.name = infod[jj].name;

		m_exCode[EXKOSDAQ].Add(exCode);
	}
}

void CJongmukDlg::SetDlgPos(int x, int y)
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

void CJongmukDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	DeleteAllData();

	m_aTemp.RemoveAll();
	ListRemoveAll();
	m_arSelCode.RemoveAll();

	DeleteObject(m_Font);
	DeleteObject(m_hBrush);
}

void CJongmukDlg::OnRadio(UINT nID)
{
	switch (nID)
	{
	case IDC_RADIO1:
		m_nRadioOption = 0;
		break;
	case IDC_RADIO2:
		m_nRadioOption = 1;
		break;
	case IDC_RADIO3:
		m_nRadioOption = 2;
		break;
	case IDC_RADIO4:
		m_nRadioOption = 3;
		break;
	}
	
	Filllistoption(m_nRadioOption);
}

BOOL CJongmukDlg::CodeListAll()			//모든 종목을 그리드 컨트롤에 보여줌...
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int length = (int)pApp->m_arrayDlgJCode->GetSize();
	CString str;
	int	idx = 0;

	if (length == 0)		
		return FALSE;

	str = "전종목%KRX100%종  합%대형주%중형주%소형주%음식료업%섬유, 의복%종이, 목재%화  학%의약품%비금속광물%철강, 금속%기  계%전기, 전자%의료정밀%운수장비%유통업%전기가스업%건설업%운수창고%통신업%금융업%은  행%종합 금융업%증  권%보  험%서비스업%제조업%%%%배당지수%지배구조우수%KOSPI 200%KOSPI 100%KOSPI 50%코스닥 종합 %코스닥 벤처%코스닥 기타서비스%코스닥 IT종합%코스닥 IT벤처%코스닥 제조%코스닥 건설%코스닥 유통%코스닥 숙박, 음식%코스닥 운송%코스닥 금융%코스닥 통신방송%코스닥 IT S/W%코스닥 IT H/W%코스닥 음식료, 담배%코스닥 섬유, 의류%코스닥 종이, 목재%코스닥 출판, 매체%코스닥 화학%코스닥 제약%코스닥 비금속%코스닥 금속%코스닥 기계장비%코스닥 전기전자%코스닥 의료, 정밀%코스닥 운송, 부품%코스닥 기타 제조%코스닥 통신서비스%코스닥 방송서비스%코스닥 인터넷%코스닥 디지털%코스닥 소프트웨어%코스닥 컴퓨터 서비스%코스닥 통신장비%코스닥 정보기기%코스닥 반도체%코스닥 IT부품%코스닥 100%코스닥 MID 300%코스닥 SMALL%코스닥 오락, 문화%KOSTAR%";
	AddCombo(str);

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();	// 리스트 삭제.
	m_arSelCode.RemoveAll();

	struct JCode* jcode;
	for (int ii = 0; ii < length; ii++)
	{
		jcode =  (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		switch (m_kind)
		{
		case kkCODE:
			{
				if (jcode->ssgb != jmSINJU && jcode->ssgb != jmSINJS &&	jcode->ssgb != jmELW && 
					jcode->kosd == jmKOSPI || jcode->kosd == jmKOSDAQ)
				{
					AddItem(idx, jcode->Code, jcode->Name);
					m_arSelCode.Add(jcode);
					idx++;
				}
			}
			break;
		default:
			{
				if (jcode->ssgb != jmSINJU && jcode->ssgb != jmSINJS)
				{
					AddItem(idx, jcode->Code, jcode->Name);
					m_arSelCode.Add(jcode);
					idx++;
				}
			}
			break;
		}
	}
	/*
	// ELW추가
	if (m_kind != kkCODE)
	{
		struct _elwcode* elwCode;
		for (ii = 0; ii < m_arrayELWCode.GetSize(); ii++, idx++)
		{
			elwCode = m_arrayELWCode.GetAt(ii);
			AddItem(idx, elwCode->code, elwCode->name);	
		}
	}
	*/
	m_ListCtrl.SortCustomItem(0);
	m_ListCtrl.SetItemCount(idx);
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
	m_ListCtrl.SetRedraw(TRUE);

	return TRUE;
}

BOOL CJongmukDlg::CodeListSome() // 선택된 종목을 그리드에..
{
	int length = (int)m_aTemp.GetSize();
	
	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	m_arSelCode.RemoveAll();

	if (length == 0)
	{
		m_ListCtrl.SetRedraw(TRUE);
		return FALSE;
	}


	int ii = 0;
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode =  m_aTemp.GetAt(ii);
		AddItem(ii, jcode->Code, jcode->Name);
		m_arSelCode.Add(jcode);
	}
	m_ListCtrl.SortCustomItem(0);
	m_ListCtrl.SetItemCount(ii);
	m_ListCtrl.SetRedraw(TRUE);
	
	return TRUE;
}

// 전종목
void CJongmukDlg::OnButtonAllcode()
{
	m_btnSel = BTN_TOTAL; 
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonAll.SetBold(TRUE);
	m_ctlButtonAll.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonAll.SetSelected(TRUE);

	m_Select = 0;

	CString strImg("");
	strImg.Format("%s/%s/", m_sRoot, "image");
	m_ComboGroup.SetFont(&m_Font);
	m_ComboGroup.LoadImg(strImg);
	m_ComboGroup.SetItemHeight(-1,14);

	ShowCombo();
	CodeListAll();
	MoveGrid();
	HideButton();

	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);

}

// KOSPI
void CJongmukDlg::OnButtonKospicode()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_KOSPI; clearEditSearch();
	Hideall(0);

	int length = (int)pApp->m_arrayDlgJCode->GetSize();
	CString str;

	InitBtn();
	m_ctlButtonKOSPI.SetBold(TRUE);
	m_ctlButtonKOSPI.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKOSPI.SetSelected(TRUE);

	m_Select = KOSPICODE;
	m_aTemp.RemoveAll();
	str = "KRX100%종  합%대형주%중형주%소형주%음식료업%섬유, 의복%종이, 목재%화  학%의약품%비금속광물%철강, 금속%기  계%전기, 전자%의료정밀%운수장비%유통업%전기가스업%건설업%운수창고%통신업%금융업%은  행%종합 금융업%증  권%보  험%서비스업%제조업%%%%배당지수%지배구조우수%KOSPI 200%KOSPI 100%KOSPI 50%";
	AddCombo(str);
	ShowCombo();

	// 2006.07.24 kospi선택인 경우 종합이 기본이 되도록
	m_ComboGroup.SetCurSel(1);

	MoveGrid();
	HideButton();

	// 2006.07.24 kospi선택인 경우 종합이 기본이 되도록
	for (int ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb != jmSINJU && jcode->ssgb != jmSINJS && 
			jcode->kosd == jmKOSPI && jcode->ssgb != jmELW)	// KOSPI종합
			m_aTemp.Add(jcode);
	}
	
	CodeListSome();

//	m_ListCtrl.SetFocus();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
}

// KOSDAQ
void CJongmukDlg::OnButtonKosdaqcode()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_KOSDAQ; clearEditSearch();
	Hideall(0);

	CString str;
	int length = (int)pApp->m_arrayDlgJCode->GetSize();

	InitBtn();
	m_ctlButtonKOSDAQ.SetBold(TRUE);
	m_ctlButtonKOSDAQ.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKOSDAQ.SetSelected(TRUE);

	m_Select = KOSDAQCODE;
	m_aTemp.RemoveAll();
	
	str = "종  합%벤  처%기타서비스%IT종합%IT벤처%제  조%건  설%유  통%숙박, 음식%운  송%금  융%통신방송%IT S/W%IT H/W%음식료, 담배%섬유, 의류%종이, 목재%출판, 매체%화  학%제  약%비금속%금  속%기계장비 %전기전자%의료, 정밀%운송, 부품%기타 제조%통신서비스%방송서비스%인터넷%디지털%소프트웨어%컴퓨터 서비스%통신장비%정보기기%반도체%IT부품%KOSDAQ 100%KOSDAQ MID 300%KOSDAQ SMALL%오락, 문화%KOSTAR%";
	AddCombo(str);
	MoveGrid();
	ShowCombo();
	HideButton();

	for (int ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb != jmSINJU && jcode->ssgb != jmSINJS &&
			jcode->kosd == jmKOSDAQ && jcode->ssgb != jmELW)
			m_aTemp.Add(jcode);
	}

	CodeListSome();

//	m_ListCtrl.SetFocus();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
}

// ELW
void CJongmukDlg::OnButtonElw() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	PBCODE pbcode;
	//struct _elwcode* elwCode;

	m_btnSel = BTN_ELW; 
	clearEditSearch();
	Hideall(0);
	
	InitBtn();
	m_ctlButtonELW.SetBold(TRUE);
	m_ctlButtonELW.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonELW.SetSelected(TRUE);

	m_Select = ELWCODE;
	m_aTemp.RemoveAll();
	//loadELWCode();
	
	// 발행사 combo
	m_ComboGroup.ResetContent();
	m_ComboGroup.AddString(_T("전체"));
	int ii = 0;
	for (ii = 0; ii < pApp->m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = pApp->m_arrayPBCode.GetAt(ii);
		m_ComboGroup.AddString(pbcode.name);
	};
	m_ComboGroup.SetCurSel(0);

	MoveGrid();
	ShowCombo();
	HideButton();

	// ELW추가
	int idx = 0;
	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	m_arSelCode.RemoveAll();

	struct JCode* jcode;
	int nCount = (int)pApp->m_arrayDlgJCode->GetSize();
	for (ii = 0; ii < nCount; ii++)
	{
		jcode =  (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb == jmELW)
		{
			AddItem(idx, jcode->Code, jcode->Name);
			m_arSelCode.Add(jcode);
			idx++;
		}
	}
	m_ListCtrl.SetItemCount(idx);

	if (m_ListCtrl.GetItemCount() > 0)
	{
		m_ListCtrl.Selected(0);
		m_szCode = m_ListCtrl.GetItemText(0, 0);
		m_szName = m_ListCtrl.GetItemText(0, 1);
	}
	m_ListCtrl.SetRedraw(TRUE);
}

// 제3시장
void CJongmukDlg::OnButtonThirdmarket()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_USCODE; clearEditSearch();

	InitBtn();
	GetExCode();
	m_ctlButtonTHIRD.SetBold(TRUE);
	m_ctlButtonTHIRD.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonTHIRD.SetSelected(TRUE);
	
	CString str, tmp;
	UPCODE  UPcode;
	int	ct = 0;
	int	size = (int)pApp->m_arrayUpcode.GetSize();
	CString	strCode;


	for (int ii = 0; ii < size; ii++)
	{
		UPcode = pApp->m_arrayUpcode.GetAt(ii);
		if (UPcode.kind == upFREEBOARD)
		{	
			char *pbuffer;
			pbuffer = new char[50];
			ZeroMemory(pbuffer, 50);
			CopyMemory(pbuffer, UPcode.name, UPcode.name.GetLength());
			str += CString(pbuffer) + "%";
			delete pbuffer;
		}
	}

	AddCombo(str);
	if (m_kind == all2CODE)	// 일때만 해외 지수 ...
	{	
		m_bOption = FALSE;
		Hideall(2);
		HideButton();
		MoveGrid(2);
		
		Invalidate();

		m_ListCtrl.SetRedraw(FALSE);
		ListRemoveAll();

		int nSize = (int)pApp->m_arrayUSCode.GetSize();
		int ii = 0;
		for (ii = 0 ; ii < nSize ; ii++)
		{
			CString symb = CString(pApp->m_arrayUSCode[ii]->symb, sizeof(pApp->m_arrayUSCode[ii]->symb));
			CString knam = CString(pApp->m_arrayUSCode[ii]->knam, sizeof(pApp->m_arrayUSCode[ii]->knam));
			AddItem(ii, symb, knam);
		}
		m_ListCtrl.SortCustomItem(0);
		m_ListCtrl.SetItemCount(ii);
		m_ListCtrl.SetRedraw(TRUE);
	}
	else
	{
		int length = (int)pApp->m_arrayDlgJCode->GetSize();
		
		m_Select = FREEBOARDCODE;
		m_aTemp.RemoveAll();
		ShowCombo();
		MoveGrid();
		HideButton();

		if (m_kind == kospiCODE || m_kind == allCODE || (m_kind == indexCODE && m_Type == 99))
		{
			InitBtn();
			m_ctlButtonTHIRD.SetBold(TRUE);
			m_ctlButtonTHIRD.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
			m_ctlButtonTHIRD.SetSelected(TRUE);
		}
		
		for (int ii = 0; ii < length; ii++)
		{
			struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
			if (jcode->kosd == jm3RD && jcode->ssgb != jmELW)
				m_aTemp.Add(jcode);
		}

		CodeListSome();

//		m_ListCtrl.SetFocus();
		m_ListCtrl.Selected(0);
		
		m_szCode = m_ListCtrl.GetItemText(0, 0);
		m_szName = m_ListCtrl.GetItemText(0, 1);
	}	
}


// Sector지수
void CJongmukDlg::OnButtonSecCode()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_SECUP; 
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonECN.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonECN.SetSelected(TRUE);

	CRect rect(100, 20, 304, 340);
	m_ListCtrl.MoveWindow(rect);
        
	ShowCombo(FALSE);
	HideButton(TRUE);

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	GetExCode();
	
	if (m_kind == allCODE || (m_kind == indexCODE && m_Type == 99))
	{
		InitBtn();
		m_ctlButtonECN.SetBold(TRUE);
		m_ctlButtonECN.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlButtonECN.SetSelected(TRUE);
	}

	UPCODE  UPcode;
	int	ct = 0;
	int	size = (int)pApp->m_arrayUpcode.GetSize();
	CString	strCode;
	for (int ii = 0; ii < size; ii++)
	{
		UPcode = pApp->m_arrayUpcode.GetAt(ii);

		if (UPcode.kind == 5)
		{	
			strCode.Format("%d%02d", UPcode.kind, atoi(UPcode.ucod));
			AddItem(ct++, strCode, UPcode.name); // 리스트 추가 함수 
		}
	}
	m_ListCtrl.SetItemCount(ct);
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
	if (m_ListCtrl.GetItemCount() > 0)
	{
		m_ListCtrl.Selected(0);
		m_szCode = m_ListCtrl.GetItemText(0, 0);
		m_szName = m_ListCtrl.GetItemText(0, 1);
		m_ListCtrl.SortCustomItem(0);
	}
	m_ListCtrl.SetRedraw(TRUE);
}

// 프리보드 업종...
void CJongmukDlg::OnButtonUpfreeboard() 
{
	// TODO: Add your control notification handler code here
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	clearEditSearch();
	Hideall(0);
	
	GetExCode();
	InitBtn();
	m_ctlFreeBoard.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlFreeBoard.SetSelected(TRUE);

	CRect rect(100, 20, 304, 340);
	m_ListCtrl.MoveWindow(rect);

	ShowCombo(FALSE);
	HideButton(TRUE);

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type == 99))
	{
		InitBtn();
		m_ctlFreeBoard.SetBold(TRUE);
		m_ctlFreeBoard.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlFreeBoard.SetSelected(TRUE);
	}

	UPCODE  UPcode;
	int	ct = 0;
	int	size = (int)pApp->m_arrayUpcode.GetSize();
	CString	strCode;
	for (int ii = 0; ii < size; ii++)
	{
		UPcode = pApp->m_arrayUpcode.GetAt(ii);

		if (UPcode.kind == upFREEBOARD)
		{	
			strCode.Format("%d%02d", UPcode.kind, atoi(UPcode.ucod));
			AddItem(ct++, strCode, UPcode.name); // 리스트 추가 함수
		}
	}
	m_ListCtrl.SetItemCount(ct);
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
	if (m_ListCtrl.GetItemCount() > 0)
	{
		m_ListCtrl.Selected(0);

		m_szCode = m_ListCtrl.GetItemText(0, 0);
		m_szName = m_ListCtrl.GetItemText(0, 1);

		m_ListCtrl.SortCustomItem(0);
		m_ListCtrl.SetItemCount(ct);
		m_ListCtrl.SetRedraw(TRUE);
	}

	//601 프리보드 종합
	//602 프리보드 벤처 
	//603 프리보드 제조 
	//604 프리보드 정보/컴퓨터운영
}

// 코스피 업종...
void CJongmukDlg::OnButtonKospiwork()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_KPIUP; 
	clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonKOSPIWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKOSPIWORK.SetSelected(TRUE);

	CRect rect(100, 20, 304, 340);
	m_ListCtrl.MoveWindow(rect);
        
	ShowCombo(FALSE);
	HideButton(TRUE);

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	GetExCode();
	
	if (m_kind == allCODE || (m_kind == indexCODE && m_Type == 99))
	{
		InitBtn();
		m_ctlButtonKOSPIWORK.SetBold(TRUE);
		m_ctlButtonKOSPIWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlButtonKOSPIWORK.SetSelected(TRUE);
	}

	UPCODE  UPcode;
	int	ct = 0;
	int	size = (int)pApp->m_arrayUpcode.GetSize();
	CString	strCode;
	for (int ii = 0; ii < size; ii++)
	{
		UPcode = pApp->m_arrayUpcode.GetAt(ii);

		if (UPcode.kind == 0 || UPcode.kind == 1 || UPcode.kind == 4)
		{	
			strCode.Format("%d%02d", UPcode.kind, atoi(UPcode.ucod));
			AddItem(ct++, strCode, UPcode.name);

		}
		else if (UPcode.kind == 9)
		{
			strCode.Format("%d%02d", UPcode.kind, atoi(UPcode.ucod));
			AddItem(0, strCode, UPcode.name); 
			ct++;
		}
	}
	m_ListCtrl.SetItemCount(ct);
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
	if(m_ListCtrl.GetItemCount() > 0)
	{
		m_ListCtrl.Selected(0);

		m_szCode = m_ListCtrl.GetItemText(0, 0);
		m_szName = m_ListCtrl.GetItemText(0, 1);
		m_ListCtrl.SetRedraw(TRUE);
	}
}

// 코스닥 업종...
void CJongmukDlg::OnButtonKosdaqwork()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_KDQUP; clearEditSearch();
	Hideall(0);

	InitBtn();
	m_ctlButtonKOSDAQWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonKOSDAQWORK.SetSelected(TRUE);

	CRect rect(100, 20, 304, 340);
	m_ListCtrl.MoveWindow(rect);
	
	ShowCombo(FALSE);
	HideButton(TRUE);

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	GetExCode();

	if (m_kind == allCODE ||(m_kind == indexCODE && m_Type == 99))
	{
		InitBtn();
		m_ctlButtonKOSDAQWORK.SetBold(TRUE);
		m_ctlButtonKOSDAQWORK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
		m_ctlButtonKOSDAQWORK.SetSelected(TRUE);
	}		

	UPCODE  UPcode;
	int	ct = 0;
	int	size = (int)pApp->m_arrayUpcode.GetSize();
	CString	strCode;
	for (int ii = 0; ii < size; ii++)
	{
		UPcode = pApp->m_arrayUpcode.GetAt(ii);
	
		if (UPcode.kind == 2 || UPcode.kind == 3)
		{		
			strCode.Format("%d%02d", UPcode.kind, atoi(UPcode.ucod));
			AddItem(ct++, strCode, UPcode.name);
		}
	}
	
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 0);
	m_ListCtrl.Selected(0);
	m_ListCtrl.SetRedraw(TRUE);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
}

// 관심종목...
void CJongmukDlg::OnButtonFavorites()
{
	m_btnSel = BTN_FAVOR; clearEditSearch();
	Hideall(0);
	
	m_Select = FAVORITECODE;
		
	LoadFavorites();

	InitBtn();
	m_ctlButtonFavor.SetBold(TRUE);
	m_ctlButtonFavor.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonFavor.SetSelected(TRUE);
	
	int len = m_ComboGroup.GetCount();
	int ii = 0;
	for (ii = 0; ii < len ; ii++)
	{
		m_ComboGroup.DeleteString(0);
	}

	HideButton(TRUE);
	ShowCombo();
	ListRemoveAll();

	MoveGrid(3);

	FAVOR	fa;
	CString str;
	for (ii = 0; ii < m_arrayFavor.GetSize(); ii++)
	{
		fa = m_arrayFavor[ii];
		str = fa.fkey + " "+ fa.name;
		int item = m_ComboGroup.AddString(str);
	}

	m_ComboGroup.SetCurSel(0);

	ListFavor(0);
}

void CJongmukDlg::ShowCombo(BOOL flag)
{
	if (flag)
		m_ComboGroup.ShowWindow(SW_SHOW);
	else
		m_ComboGroup.ShowWindow(SW_HIDE);
}

void CJongmukDlg::MoveGrid(int kind)
{
	CRect rect1(100, 17, 304, 266);
	CRect rect2(100, 40, 304, 257);
	CRect rect3(100, 17, 304, 257);
	CRect rect4(100, 40, 304, 345);

	switch (kind)
	{
	case 0:
		m_ListCtrl.MoveWindow(rect1);
		break;
	case 1:
		m_ListCtrl.MoveWindow(rect2);
		break;
	case 2:
		m_ListCtrl.MoveWindow(rect3);
		break;
	case 3:
		m_ListCtrl.MoveWindow(rect4);
		break;
	}
}

void CJongmukDlg::HideButton(BOOL flag)
{
	if (flag)
		for (int ii = IDC_BUTTON_1; ii <= IDC_COMBO_SEARCH; ii++)
			((CButton*)GetDlgItem(ii))->ShowWindow(SW_HIDE);

	else
		for (int ii = IDC_BUTTON_1; ii <= IDC_COMBO_SEARCH; ii++)
			((CButton*)GetDlgItem(ii))->ShowWindow(SW_SHOW);

}

void CJongmukDlg::AddCombo(CString& szText)
{
	int pos;
	CString temp;

	int len = m_ComboGroup.GetCount(), nCnt = 0;

	for (int ii = 0; ii < len ; ii++)
	{
		int s = m_ComboGroup.DeleteString(0);
	}

	while(!szText.IsEmpty())
	{	
		pos = szText.Find('%');
		nCnt++;
		if (pos != 0)
		{
			temp = szText.Left(pos);
			m_ComboGroup.SetItemData(m_ComboGroup.AddString(temp), nCnt);
		}
		szText = szText.Mid(pos + 1);
	}
	m_ComboGroup.SetCurSel(0);
}

void CJongmukDlg::AddGrid(CString& str, int kind)
{
	int num, pos, ii = 0;
	CString temp, code;

	ListRemoveAll();

	while(!str.IsEmpty())
	{	
		++ii;
		pos  = str.Find('%');
		temp = str.Left(pos);
		num  = str.Find(' ');
		code = str.Left(num);
		temp = temp.Mid(num + 1);
		
		if(ii <=  kind)
			AddItem(ii - 1, code, temp);
		str = str.Mid(pos + 1);
	}

	m_ComboGroup.SetCurSel(0);
}

BOOL CJongmukDlg::ListElwSort(int nItem)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	m_arSelCode.RemoveAll();

	struct _elwcode*	elwcode;
	struct	JCode*	jcode;
	struct _pbcode pbcode;
	CString pcode = _T("");
	int	idx = 0;
	int	sel = 0;
	
	int ct = (int)pApp->m_arrayDlgJCode->GetSize();
	if (ct <= 0)
	{
		m_ListCtrl.SetRedraw(TRUE);
		return FALSE;
	}
	
	sel = m_ComboGroup.GetCurSel();
	if (sel <= 0)
	{
		pcode = "";
	}
	else
	{
		pbcode = pApp->m_arrayPBCode.GetAt(sel-1);
		pcode = pbcode.code;
	}

	for (int ii = 0; ii < ct; ii++)
	{
		jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb != jmELW)
			continue;

		elwcode = (struct _elwcode*)jcode->pInf;
		if (elwcode == NULL || (!pcode.IsEmpty() && elwcode->pcode != pcode)) 
			continue;

		AddItem(idx, elwcode->code, elwcode->name);
		m_arSelCode.Add(jcode);
		idx++;
	}

	int len = m_ListCtrl.GetItemCount();
	if (len < 18)
	{
		m_ListCtrl.SetColumnWidth(1, 148);
	}
	m_ListCtrl.SetRedraw(TRUE);

	return TRUE;
	
}

BOOL CJongmukDlg::ListFreeBoard(int nItem)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int length = (int)pApp->m_arrayDlgJCode->GetSize();
	m_arSelCode.RemoveAll();
	int ii = 0;
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb == jmELW)
			continue;
		
		switch(nItem)
		{
		case 1:	// 프리보드 종합
			if (jcode->kosd == jm3RD)
				m_arSelCode.Add(jcode);  // 통합지수
			break;
		case 2: // 프리보드 벤처
			if (jcode->kosd == jm3RD && jcode->jjug == 3)
				m_arSelCode.Add(jcode);
			break;
		case 3: // 프리보드 제조
			if (jcode->kosd == jm3RD && jcode->ucdm == 3)
				m_arSelCode.Add(jcode);  
			break;
		case 4:	// 프리보드 정보/컴퓨터 운영
			if (jcode->kosd == jm3RD && jcode->ucdm == 4)
				m_arSelCode.Add(jcode);  
			break;
		}
	}

	length = (int)m_arSelCode.GetSize();
	
	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();
	if (length == 0)
	{
		m_ListCtrl.SetColumnWidth(1, 148);
		m_ListCtrl.SetRedraw(TRUE);
		return false;
	}

	if (length < 18)
		m_ListCtrl.SetColumnWidth(1, 148);
	
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)m_arSelCode.GetAt(ii);
		AddItem(ii, jcode->Code, jcode->Name);
	}

	m_ListCtrl.SetRedraw(TRUE);
	return TRUE;
}

BOOL CJongmukDlg::ListAllWorkSort(int nItem)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	struct JCode* jcode;
	int	idx = 0;
	int	length = (int)pApp->m_arrayDlgJCode->GetSize();
	
	m_arSelCode.RemoveAll();
	int ii = 0;
	for (ii = 0; ii < length; ii++)
	{
		jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);

		if (jcode->ssgb == jmELW)
			continue;

		switch(nItem)
		{
		case 1:
			m_arSelCode.Add(jcode);	// 전종목
			break;
		case 2:				
			if (jcode->unio == 1)
				m_arSelCode.Add(jcode);  // 통합지수
			break;
		case 3:
			if (jcode->kosd == jmKOSPI)
				m_arSelCode.Add(jcode);  // KOSPI 종합...
			break;
		case 4:
			if (jcode->kosd == jmKOSPI && jcode->size == 2)
				m_arSelCode.Add(jcode);  // 대형주...
			break;
		case 5:
			if (jcode->kosd == jmKOSPI && jcode->size == 3)
				m_arSelCode.Add(jcode);  // 중형주...
			break;
		case 6: 
			if (jcode->kosd == jmKOSPI && jcode->size == 4)
				m_arSelCode.Add(jcode);  // 소형주...
			break;

		case 7:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 5)
				m_arSelCode.Add(jcode);   // 음식료업...		
			break;
		case 8:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 6)
				m_arSelCode.Add(jcode);  // 섬유 , 의복
			break;
		case 9: 
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 7)
				m_arSelCode.Add(jcode);  // 종이
			break;	
		case 10:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 8)
				m_arSelCode.Add(jcode);  // 화 학 
			break;
		case 11:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 9)
				m_arSelCode.Add(jcode);  // 의약품/숙박   
			break;
			
		case 12:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 10)
				m_arSelCode.Add(jcode);  // 비금속광물    
			break; 
		case 13:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 11)
				m_arSelCode.Add(jcode);  // 철 강     
			break;
		case 14:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 12)
				m_arSelCode.Add(jcode);  // 기 계      
			break;
		case 15:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 13)
				m_arSelCode.Add(jcode);  // 전기 전자/IT       
			break;
		case 16:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 14)
				m_arSelCode.Add(jcode);  // 의료정밀        
			break;
		case 17:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 15)
				m_arSelCode.Add(jcode);  // 운수장비         
			break;
		case 18:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 16)
				m_arSelCode.Add(jcode);  // 유통업       
			break;
		case 19:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 17)
				m_arSelCode.Add(jcode);  // 전기 가스업      
			break;
		case 20:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 18)
				m_arSelCode.Add(jcode);  // 건설업       
			break;
		case 21:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 19)
				m_arSelCode.Add(jcode);  // 운수창고        
			break;
		case 22:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 20)
				m_arSelCode.Add(jcode);  // 통신업         
			break;
		case 23:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 21)
				m_arSelCode.Add(jcode);  // 금융업          
			break;
		case 24:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 22)
				m_arSelCode.Add(jcode);  // 은 행           
			break;
		case 25:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 23)
				m_arSelCode.Add(jcode);  // 종합금융업           
			break;
		case 26:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 24)
				m_arSelCode.Add(jcode);  // 증 권            
			break;
		case 27:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 25)
				m_arSelCode.Add(jcode);  // 보 험          
			break;
		case 28:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 26)
				m_arSelCode.Add(jcode);  // 서비스업           
			break;
		case 29:
			if (jcode->kosd == jmKOSPI && jcode->jjug == 27)
				m_arSelCode.Add(jcode);  // 제조업            
			break;
		case 33:
			if (jcode->kosd == jmKOSPI 
				&& (jcode->jsiz == 41))
				m_arSelCode.Add(jcode);  // 배당지수
			break;
		case 34:
			if (jcode->kosd == jmKOSPI && (jcode->wsgb == 1))
				m_arSelCode.Add(jcode);  // 지배구조우수
			break;
		case 35:
			if (jcode->kosd == jmKOSPI && (jcode->kpgb == 1 || jcode->kpgb == 2 || jcode->kpgb == 3))
				m_arSelCode.Add(jcode);  // 코스피 200            
			break;
		case 36:
			if (jcode->kosd == jmKOSPI && (jcode->kpgb == 3 || jcode->kpgb == 2))
				m_arSelCode.Add(jcode);  // 코스피 100
			break;
		case 37:
			if (jcode->kosd == jmKOSPI && jcode->kpgb == 3)
				m_arSelCode.Add(jcode);    // 코스피 50
			break;
		case 38:
			if (jcode->kosd == jmKOSDAQ)
				m_arSelCode.Add(jcode);  // 코스닥 종합 ...
			break;
		case 39:
			if (jcode->kosd == jmKOSDAQ && jcode->jjug == 2)
				m_arSelCode.Add(jcode);  // 벤처지수...
			break;
		case 40:
			if (jcode->kosd == jmKOSDAQ && jcode->size == 3)
				m_arSelCode.Add(jcode);  // 기타 서비스 
			break;
		case 41: 
			if (jcode->kosd == jmKOSDAQ && jcode->size == 4)
				m_arSelCode.Add(jcode);  // 코스닥 IT 종합...
			break;
		case 42:
			if (jcode->kosd == jmKOSDAQ && jcode->size == 4 && jcode->jjug == 2)
				m_arSelCode.Add(jcode);  // 코스닥 IT 벤처...
			break;
		case 43:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 6)
				m_arSelCode.Add(jcode);  // 제 조 
			break;

		case 44: 
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 7)
				m_arSelCode.Add(jcode);  // 목재/건설 
			break;	
		case 45:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 8)
				m_arSelCode.Add(jcode);  // 유 통 
			break;
		case 46:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 9)
				m_arSelCode.Add(jcode);  // 음 식 
			break;
			
		case 47:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 10)
				m_arSelCode.Add(jcode);  // 운 송  
			break; 
		case 48:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 11)
				m_arSelCode.Add(jcode);  // 금 속/ 금 융 
			
			break;
		case 49:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 12)
				m_arSelCode.Add(jcode);  // 통신방송서비스 
			break;
		case 50:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 13)
				m_arSelCode.Add(jcode);  // S/W & SVC
			break;
		case 51:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 14)
				m_arSelCode.Add(jcode);  // IT H/W
			break;
		case 52:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 15)
				m_arSelCode.Add(jcode);  // 음식료, 담배 
			break;
		case 53:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 16)
				m_arSelCode.Add(jcode);  // 섬유 , 의 류  
			break;
		case 54:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 17)
				m_arSelCode.Add(jcode);  // 종이 , 목재   
			break;
		case 55:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 18)
				m_arSelCode.Add(jcode);  // 출판 매체복제  
			break;
		case 56:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 19)
				m_arSelCode.Add(jcode);  // 화 학  
			break;
		case 57:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 20)
				m_arSelCode.Add(jcode);  // 제약 
			break;
		case 58:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 21)
				m_arSelCode.Add(jcode);  // 비금속 
			break;
		case 59:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 22)
				m_arSelCode.Add(jcode);  // 금 속 
			break;

		case 60:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 23) 
				m_arSelCode.Add(jcode);  // 기계 장비 
			break;
		case 61:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 24)
				m_arSelCode.Add(jcode);  //  일반 전기 전자 
			break;
		case 62:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 25)
				m_arSelCode.Add(jcode);  // 의 료 , 정밀기기 
			break;
		case 63:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 26)
				m_arSelCode.Add(jcode);  // 운송장비 부품 
			break;
		case 64:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 27)
				m_arSelCode.Add(jcode);  // 기타 제조  
			break;
		case 65:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 28)
				m_arSelCode.Add(jcode);  // 통신 서비스 
			break;
		case 66:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 29)
				m_arSelCode.Add(jcode);  // 방송 서비스 
			break;
		case 67:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 30)
				m_arSelCode.Add(jcode);  // 인터넷 
			break;
		case 68:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 31)
				m_arSelCode.Add(jcode);  // 디지털 컨텐츠   
			break;
		case 69:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 32)
				m_arSelCode.Add(jcode);  // 소프트웨어    
			break;
		case 70:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 33)
				m_arSelCode.Add(jcode);  // 컴퓨터 서비스    
			break;
		case 71:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 34)
				m_arSelCode.Add(jcode);  // 통신장비  
			break;
		case 72:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 35)
				m_arSelCode.Add(jcode);  // 정보기기  
			break;
		case 73:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 36)
				m_arSelCode.Add(jcode);  // 반도체  
			break;
		case 74:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 37)
				m_arSelCode.Add(jcode);  // IT 부품 
			break;
		case 75:
			if (jcode->kosd == jmKOSDAQ && jcode->jsiz == 1)
				m_arSelCode.Add(jcode);  // KOSDAQ 100
			break;
		case 76:
			if (jcode->kosd == jmKOSDAQ && jcode->jsiz == 2)
				m_arSelCode.Add(jcode);  // KOSDAQ MID 300
			break;
		case 77:
			if (jcode->kosd == jmKOSDAQ && jcode->jsiz == 3)
				m_arSelCode.Add(jcode);  // KOSDAQ SMALL
			break;
		case 78:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 41)
				m_arSelCode.Add(jcode);  // KOSDAQ 오락/문화
			break;
		case 79:			  // KOSTAR
			if (jcode->kosd == jmKOSDAQ && jcode->star == 1)
				m_arSelCode.Add(jcode);  
			break;
		}
	}

	length = (int)m_arSelCode.GetSize();
	
	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();

	if (length == 0)
	{
		m_ListCtrl.SetColumnWidth(1, 148);
		m_ListCtrl.SetRedraw(TRUE);
		return FALSE;
	}

	if (length < 18)
		m_ListCtrl.SetColumnWidth(1, 148);

	for (ii = 0; ii < length; ii++, idx++)
	{
		struct JCode* jcode = (struct JCode*)m_arSelCode.GetAt(ii);
		AddItem(idx, jcode->Code, jcode->Name);
	}
	
	/*
	if (nItem == 1)	// 전체인 경우
	{
		struct _elwcode* elwCode;
		for (ii = 0; ii < m_arrayELWm_arSelCode.GetSize(); ii++, idx++)
		{
			elwCode = m_arrayELWm_arSelCode.GetAt(ii);
			AddItem(idx, elwCode->code, elwCode->name);	
		}
	}
	*/
	m_ListCtrl.SetRedraw(TRUE);

	m_bAddAll = true;

	return TRUE;
}

void CJongmukDlg::ListKospiSort(int nItem)	// 코스피 업종별로..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int length = (int)pApp->m_arrayDlgJCode->GetSize();

	m_arSelCode.RemoveAll();
	int ii = 0;
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb == jmELW)
			continue;
		
		switch(nItem)
		{
		case 1:	// 통합지수
			if (jcode->unio == 1)
				m_arSelCode.Add(jcode);  // 통합지수
			break;
		case 2:
			// 2006.07.24 kospi종목 check안한 오류 수정
			if (jcode->kosd == jmKOSPI)
				m_arSelCode.Add(jcode);
			break;
		case 3:
			if (jcode->kosd == jmKOSPI && jcode->size == 2)
				m_arSelCode.Add(jcode);  // 대형주...
			break;
		case 4:
			if (jcode->kosd == jmKOSPI && jcode->size == 3)
				m_arSelCode.Add(jcode);  // 중형주...
			break;
		case 5: 
			if (jcode->kosd == jmKOSPI && jcode->size == 4)
				m_arSelCode.Add(jcode);  // 소형주...
			break;
		case 6:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 5)
				m_arSelCode.Add(jcode);   // 음식료업...	
			break;
		case 7:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 6)
				m_arSelCode.Add(jcode);  // 섬유 , 의복
			break;
		case 8: 
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 7)
				m_arSelCode.Add(jcode);  // 종이
			break;	
		case 9:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 8)
				m_arSelCode.Add(jcode);  // 화 학 
			break;
		case 10:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 9)
				m_arSelCode.Add(jcode);  // 의약품/숙박   
			break;
		case 11:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 10)
				m_arSelCode.Add(jcode);  // 비금속광물    
			break; 
		case 12:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 11)
				m_arSelCode.Add(jcode);  // 철 강     
			break;
		case 13:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 12)
				m_arSelCode.Add(jcode);  // 기 계      
			break;
		case 14:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 13)
				m_arSelCode.Add(jcode);  // 전기 전자/IT       
			break;
		case 15:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 14)
				m_arSelCode.Add(jcode);  // 의료정밀        
			break;
		case 16:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 15)
				m_arSelCode.Add(jcode);  // 운수장비         
			break;
		case 17:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 16)
				m_arSelCode.Add(jcode);  // 유통업       
			break;
		case 18:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 17)
				m_arSelCode.Add(jcode);  // 전기 가스업      
			break;
		case 19:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 18)
				m_arSelCode.Add(jcode);  // 건설업       
			break;
		case 20:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 19)
				m_arSelCode.Add(jcode);  // 운수창고        
			break;
		case 21:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 20)
				m_arSelCode.Add(jcode);  // 통신업         
			break;
		case 22:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 21)
				m_arSelCode.Add(jcode);  // 금융업          
			break;
		case 23:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 22)
				m_arSelCode.Add(jcode);  // 은 행           
			break;
		case 24:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 23)
				m_arSelCode.Add(jcode);  // 종합 금 융  
			break;
		case 25:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 24)
				m_arSelCode.Add(jcode);  // 증 권            
			break;
		case 26:
			if (jcode->kosd == jmKOSPI && jcode->ucds == 25)
				m_arSelCode.Add(jcode);  // 보 험          
			break;
		case 27:
			if (jcode->kosd == jmKOSPI && jcode->ucdm == 26)
				m_arSelCode.Add(jcode);  // 서비스업           
			break;
		case 28:
			if (jcode->kosd == jmKOSPI && jcode->jjug == 27)
				m_arSelCode.Add(jcode);  // 제조업            
			break;
		case 32:
			if (jcode->kosd == jmKOSPI 
				&& (jcode->jsiz == 41))//>= 38 + 41 && jcode->jsiz <= 41 + 41))
				m_arSelCode.Add(jcode);  // 배당지수         
			break;
		case 33:
			if (jcode->kosd == jmKOSPI && jcode->wsgb == 1)
				m_arSelCode.Add(jcode);  // 지배구조우수
			break;
		case 34:
			if (jcode->kosd == jmKOSPI && (jcode->kpgb == 1 || jcode->kpgb == 2 || jcode->kpgb == 3))
				m_arSelCode.Add(jcode);  // 코스피 200            
			break;
		case 35:
			if (jcode->kosd == jmKOSPI && (jcode->kpgb == 3 || jcode->kpgb == 2))
				m_arSelCode.Add(jcode);  // 코스피 100
			break;
		case 36:
			if (jcode->kosd == jmKOSPI && jcode->kpgb == 3)
				m_arSelCode.Add(jcode);  // 코스피 50
			break;
		case 37:
			if (jcode->kosd == jmKOSPI && jcode->itgb == 1)
				m_arSelCode.Add(jcode);  // 코스피 IT       
			break;
		}
	}

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();

	length = (int)m_arSelCode.GetSize();	
	if (length == 0)
	{
		m_ListCtrl.SetColumnWidth(1, 148);
		m_ListCtrl.SetRedraw(TRUE);
		return;
	}

	if (length < 18)
		m_ListCtrl.SetColumnWidth(1, 148);
	
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)m_arSelCode.GetAt(ii);
		AddItem(ii, jcode->Code, jcode->Name);
	}
	m_ListCtrl.SetRedraw(TRUE);
}

void CJongmukDlg::ListKosDaqSort(int nItem)		// 코스닥 업종별을 리스트에...
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int length = (int)pApp->m_arrayDlgJCode->GetSize();
	
	m_arSelCode.RemoveAll();
	int ii = 0;
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->ssgb == jmELW)
			continue;
		
		switch(nItem)
		{
		case 1:
			if (jcode->kosd == jmKOSDAQ)
				m_arSelCode.Add(jcode);
			break;
		case 2:
			if (jcode->kosd == jmKOSDAQ && jcode->jjug == 2)
				m_arSelCode.Add(jcode);  // 벤처지수
			break;
		case 3:
			if (jcode->kosd == jmKOSDAQ && jcode->size == 3)
				m_arSelCode.Add(jcode);  // 기타 서비스 
			break;
		case 4: 
			if (jcode->kosd == jmKOSDAQ && jcode->size == 4)
				m_arSelCode.Add(jcode);  // 코스닥 IT 종합
			break;
		case 5:
			if (jcode->kosd == jmKOSDAQ && jcode->size == 4 && jcode->jjug == 2)
				m_arSelCode.Add(jcode);  // 코스닥 IT 벤처
			break;
		case 6:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 6)
				m_arSelCode.Add(jcode);  // 제 조 
			break;
		case 7: 
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 7)
				m_arSelCode.Add(jcode);  // 목재/건설 
			break;	
		case 8:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 8)
				m_arSelCode.Add(jcode);  // 유 통 
			break;
		case 9:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 9)
				m_arSelCode.Add(jcode);  // 음 식 
			break;
		case 10:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 10)
				m_arSelCode.Add(jcode);  // 운 송  
			break; 
		case 11:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 11)
				m_arSelCode.Add(jcode);  // 금 속/ 금 융 
			break;
		case 12:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 12)
				m_arSelCode.Add(jcode);  // 통신방송서비스 
			break;
		case 13:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 13)
				m_arSelCode.Add(jcode);  // S/W & SVC
			break;
		case 14:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 14)
				m_arSelCode.Add(jcode);  // IT H/W
			break;
		case 15:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 15)
				m_arSelCode.Add(jcode);  // 음식료, 담배 
			break;
		case 16:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 16)
				m_arSelCode.Add(jcode);  // 섬유 , 의 류  
			break;
		case 17:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 17)
				m_arSelCode.Add(jcode);  // 종이 , 목재   
			break;
		case 18:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 18)
				m_arSelCode.Add(jcode);  // 출판 매체복제  
			break;
		case 19:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 19)
				m_arSelCode.Add(jcode);  // 화 학  
			break;
		case 20:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 20)
				m_arSelCode.Add(jcode);  // 제약 
			break;
		case 21:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 21)
				m_arSelCode.Add(jcode);  // 비금속 
			break;
		case 22:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 22)
				m_arSelCode.Add(jcode);  // 금 속 
			break;
		case 23:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 23) 
				m_arSelCode.Add(jcode);  // 기계 장비 
			break;
		case 24:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 24)
				m_arSelCode.Add(jcode);  // 일반 전기 전자 
			break;
		case 25:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 25)
				m_arSelCode.Add(jcode);  // 의 료, 정밀기기 
			break;
		case 26:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 26)
				m_arSelCode.Add(jcode);  // 운송장비 부품 
			break;
		case 27:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 27)
				m_arSelCode.Add(jcode);  // 기타 제조  
			break;
		case 28:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 28)
				m_arSelCode.Add(jcode);  // 통신 서비스 
			break;
		case 29:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 29)
				m_arSelCode.Add(jcode);  // 방송 서비스 
			break;
		case 30:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 30)
				m_arSelCode.Add(jcode);  // 인터넷 
			break;
		case 31:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 31)
				m_arSelCode.Add(jcode);  // 디지털 컨텐츠   
			break;
		case 32:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 32)
				m_arSelCode.Add(jcode);  // 소프트웨어    
			break;
		case 33:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 33)
				m_arSelCode.Add(jcode);  // 컴퓨터 서비스    
			break;
		case 34:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 34)
				m_arSelCode.Add(jcode);  // 통신장비  
			break;
		case 35:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 35)
				m_arSelCode.Add(jcode);  // 정보기기  
			break;
		case 36:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 36)
				m_arSelCode.Add(jcode);  // 반도체  
			break;
		case 37:
			if (jcode->kosd == jmKOSDAQ && jcode->ucds == 37)
				m_arSelCode.Add(jcode);  // IT 부품 
			break;
		case 38:
			if (jcode->kosd == jmKOSDAQ && jcode->jsiz == 1)
				m_arSelCode.Add(jcode);  // KOSDAQ 100
			break;
		case 39:
			if (jcode->kosd == jmKOSDAQ && jcode->jsiz == 2)
				m_arSelCode.Add(jcode);  // KOSDAQ MID 300
			break;
		case 40:
			if (jcode->kosd == jmKOSDAQ && jcode->jsiz == 3)
				m_arSelCode.Add(jcode);  // KOSDAQ SMALL
			break;
		case 41:
			if (jcode->kosd == jmKOSDAQ && jcode->ucdm == 41)
				m_arSelCode.Add(jcode);  // KOSDAQ 오락/문화
			break;
		case 42:			  
			if (jcode->kosd == jmKOSDAQ && jcode->star == 1)
				m_arSelCode.Add(jcode);  // KOSTAR
			break;
		}
	}

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();

	length = (int)m_arSelCode.GetSize();
	if (length == 0)
	{
		m_ListCtrl.SetColumnWidth(1, 148);
		m_ListCtrl.SetRedraw(TRUE);
		return;
	}

	if (length < 18)
		m_ListCtrl.SetColumnWidth(1, 148);
	
	for (ii = 0; ii < length; ii++)
	{
		struct JCode* jcode =  (struct JCode*)m_arSelCode.GetAt(ii);
		AddItem(ii, jcode->Code, jcode->Name);
	}
	m_ListCtrl.SetRedraw(TRUE);
}

void CJongmukDlg::OnClickBtnSort(UINT nID)
{
	CString	sName, sCode, sTitle;
	CWnd	*pBtn = GetDlgItem(nID);
	pBtn->GetWindowText(sTitle);

	if (sTitle == _T("영문"))
	{
		struct JCode* jcode;
		int	position = 0;

		m_ListCtrl.SetRedraw(FALSE);
		ListRemoveAll();
		for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
		{
			jcode = (struct JCode*)m_arSelCode.GetAt(ii);

			sName = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	sName = sName.Mid(1);
			if (0x41 >  sName.GetAt(1) || sName.GetAt(1) > 0x7A)
				continue;

			sCode = jcode->Code;
			sName.TrimRight();
			AddItem(position, sCode, sName);
			position += 1;
		}
 
		if (m_ListCtrl.GetItemCount() > 0)
		{
			m_gListsort = FALSE;
			ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 1);

			m_ListCtrl.Selected(0);
			m_szCode = m_ListCtrl.GetItemText(0, 0);
			m_szName = m_ListCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}

		m_ListCtrl.SetRedraw(TRUE);
		m_edtSearch.SetWindowText("");
	}
	else if (sTitle == _T("지우기"))
	{
		m_edtSearch.SetWindowText("");
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(0);
		else
		{
			m_bAddAll = false;
			searchWord();
		}
	}
	else if (sTitle == _T("←"))
	{
		m_edtSearch.GetWindowText(sName);
		if (sName.IsEmpty())
		{
			if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			{
				searchAction(0);
			}
			else
			{
				m_bAddAll = false;
				searchWord();
			}
		}
		else if (sName.GetLength() == 1)	// 문자
		{
			m_edtSearch.SetWindowText("");
			if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
				searchAction(0);
			else
			{
				m_bAddAll = false;
				searchWord();
			}
		}
		else
		{
			char	cChar = sName.GetAt(sName.GetLength()-1);
			if (cChar & HANGULMSK)	sName = sName.Left(sName.GetLength() - 2);
			else			sName = sName.Left(sName.GetLength() - 1);

			m_edtSearch.SetWindowText(sName);
			if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			{
				if (sName.IsEmpty())
					searchAction(0);
				else
					searchAction(1);
			}
			else
			{
				searchWord();
			}
		}
	}
	else
	{
		m_edtSearch.GetWindowText(sName);
		m_edtSearch.SetWindowText(sName + sTitle);

		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(1);
		else
			searchWord();
	}

	m_edtSearch.SetFocus();
	m_edtSearch.SetSel(0, -1);
	m_edtSearch.SetSel(-1, -1);
}

LRESULT CJongmukDlg::OnEditXMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	CString	string;

	switch ((int) wParam)
	{
	case wpBLANK:
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(0);
		else
			searchWord();
		break;
	case wpENTER:
		break;
	case wpSEARCH1:
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(0);	// number search by kwon
		else
			searchWord();
		break;
	case wpSEARCH2:
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(1);	// char search by kwon
		else
			searchWord();
		break;
	case wpUP:
		if (m_ListCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
			if (pos == NULL)
				break;
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			nItem--;
			if (nItem < 0)	nItem = 0;
			m_ListCtrl.Selected(nItem);
			m_szCode = m_ListCtrl.GetItemText(nItem, 0);
			m_szName = m_ListCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_edtSearch.SetFocus();
			m_edtSearch.SetSel(0, -1);
			m_edtSearch.SetSel(-1, -1);
		}
		break;
	case wpDOWN:
		if (m_ListCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_ListCtrl.GetFirstSelectedItemPosition();
			if (pos == NULL)
				break;
			int nItem = m_ListCtrl.GetNextSelectedItem(pos);
			nItem++;
			if (nItem >= m_ListCtrl.GetItemCount())	nItem = m_ListCtrl.GetItemCount() - 1;
			m_ListCtrl.Selected(nItem);
			m_szCode = m_ListCtrl.GetItemText(nItem, 0);
			m_szName = m_ListCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_edtSearch.SetFocus();
			m_edtSearch.SetSel(0, -1);
			m_edtSearch.SetSel(-1, -1);
		}
		break;
	}

	return 0;
}

void CJongmukDlg::OnButtonOk() 
{
	OnOK();
}

void CJongmukDlg::Part()
{
	CString szText;
	Hideall(0);
	
	switch (m_kind)
	{
	case all2CODE:
		{
			InitOption();
			OnButtonAllcode();
		}
		break;
	case secCODE: // 섹터지수 
			((CButton*)GetDlgItem(IDC_BUTTON_ALLCODE))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPICODE))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQCODE))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_FAVORITES))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_THIRDMARKET))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_UPFREEBOARD))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON_ELW))->EnableWindow(FALSE);
			szText = "섹터지수 안내";
			OnButtonSecCode();

		break;
	case kospiCODE:
		((CButton*)GetDlgItem(IDC_BUTTON_ECNCODE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_UPFREEBOARD))->EnableWindow(FALSE);
		szText = "주식 코드 안내";
		OnButtonAllcode();
		break;
		
	case thirdCODE:
		((CButton*)GetDlgItem(IDC_BUTTON_ECNCODE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_KOSPIWORK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQWORK))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_ALLCODE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_KOSPICODE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_KOSDAQCODE))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_FAVORITES))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_ELW))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_UPFREEBOARD))->EnableWindow(FALSE);
		OnButtonThirdmarket();
		szText = "프리보드 코드 안내";
		break;

	case indexCODE:
		GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FAVORITES)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ETF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
		if (m_Type == 0)	// 코스피 업종...
		{
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPFREEBOARD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
			OnButtonKospiwork();
		}
		else if (m_Type == 5)   
		{
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPFREEBOARD)->EnableWindow(FALSE);
			OnButtonSecCode();
		}
		else if (m_Type == 9)	// 코스닥 업종...
		{	
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPFREEBOARD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
			OnButtonKosdaqwork();
		}
		else if (m_Type == 6)	// 프리보드 업종...
		{
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
			OnButtonUpfreeboard();
		}
		else if (m_Type == 99)
		{	
			OnButtonKospiwork();
		}

		szText = "업종 코드 안내";
		
		break;
	case kospi2CODE:
		{	
			GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_FAVORITES)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPFREEBOARD)->EnableWindow(FALSE);
			CString str;
			CRect rect(100, 17, 304, 323);

			m_ComboGroup.ShowWindow(SW_HIDE);
			HideButton(TRUE);
			m_ListCtrl.MoveWindow(rect);
			szText = "KOSPI 200 코드 안내";
			ListKospiSort(34);
		}
		break;
	case allCODE:
		{
			szText = "코드 안내";
			OnButtonAllcode();	
		}
		break;
	case kkCODE:
		{
			GetDlgItem(IDC_BUTTON_FAVORITES)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPFREEBOARD)->EnableWindow(FALSE);

			szText = "코드 안내";

			OnButtonAllcode();
		}
		break;
	case etfCODE:
		{
			GetDlgItem(IDC_BUTTON_FAVORITES)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPFREEBOARD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ALLCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSPICODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQCODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_FAVORITES)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_THIRDMARKET)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSDAQWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_KOSPIWORK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ELW)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ECNCODE)->EnableWindow(FALSE);

			OnButtonEtf();
		}
		break;
	}

	SetWindowText(szText);
}

void CJongmukDlg::SearchString(CString szText)
{
	int len = szText.GetLength();

	m_gListsort = FALSE;
	ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 1);

	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		CString str = m_ListCtrl.GetItemText(ii, 1);
		
		str = str.Left(len);

		if (szText.CompareNoCase(str) == 0)
		{
			m_ListCtrl.Selected(ii);
			
			m_szCode = m_ListCtrl.GetItemText(ii, 0);
			m_szName = m_ListCtrl.GetItemText(ii, 1);
			return;
		}
	}
}

/*
HBRUSH CJongmukDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
void CJongmukDlg::BtnSetup()
{
	short	shBtnColor = 245;
	int nheight	= 9;
	CString sztext	= "굴림";
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	UINT button[] = {
		IDC_BUTTON_FUTURE1, IDC_BUTTON_FUTURE2, IDC_BUTTON_FUTURE3, IDC_BUTTON_FUTURE4,
		IDC_BUTTON_ALLCODE, IDC_BUTTON_KOSPICODE, IDC_BUTTON_KOSDAQCODE, IDC_BUTTON_ECNCODE, 
		IDC_BUTTON_THIRDMARKET, IDC_BUTTON_KOSPIWORK, IDC_BUTTON_KOSDAQWORK, IDC_BUTTON_FAVORITES, 
		IDC_BUTTON_1, IDC_BUTTON_2, IDC_BUTTON_3, IDC_BUTTON_4, IDC_BUTTON_5, 
		IDC_BUTTON_6, IDC_BUTTON_7, IDC_BUTTON_8, IDC_BUTTON_9, IDC_BUTTON_10, 
		IDC_BUTTON_11, IDC_BUTTON_12, IDC_BUTTON_13, IDC_BUTTON_14, IDC_BUTTON_15, 
		IDC_BUTTON_16, IDC_BUTTON_17, IDC_BUTTON_18, IDC_BUTTON_19, IDC_BUTTON_20, 
		IDC_BUTTON_21, IDC_BUTTON_22, IDC_BUTTON_ELW, IDC_BUTTON_UPFREEBOARD, IDC_BUTTON_ETF
	};

	for (int ii = 0 ; ii < sizeof(button)/sizeof(UINT) ; ii++)
	{
		//((CButtonST*)GetDlgItem(button[ii]))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		//((CButtonST*)GetDlgItem(button[ii]))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		//((CButtonST*)GetDlgItem(button[ii]))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		//((CButtonST*)GetDlgItem(button[ii]))->SetWnd(this);
		((CfxButton*)GetDlgItem(button[ii]))->SetBtnCursor(IDC_CURSOR_HAND);
		((CfxButton*)GetDlgItem(button[ii]))->LoadImg(strImg);
	}
}


void CJongmukDlg::OnSelchangeComboGroup() 
{
	int item = m_ComboGroup.GetCurSel();
	if (item == CB_ERR)
		return;
	item = (int)m_ComboGroup.GetItemData(item);

	switch (m_Select)
	{
 	case	ALLCODE:
		ListAllWorkSort(item);
		if (m_ListCtrl.GetItemCount() > 0)
			m_ListCtrl.Selected(0);
 		break;
	case	KOSPICODE:
		ListKospiSort(item);
		if (m_ListCtrl.GetItemCount() > 0)
			m_ListCtrl.Selected(0);
 		break;
 	case    KOSDAQCODE:
 		ListKosDaqSort(item);
		if (m_ListCtrl.GetItemCount() > 0)
			m_ListCtrl.Selected(0);
 		break;
	case    FAVORITECODE:	
		item = m_ComboGroup.GetCurSel();
		ListFavor(item);
		if (m_ListCtrl.GetItemCount() > 0)
			m_ListCtrl.Selected(0);
		break;
	case	ELWCODE:
		ListElwSort(item);
		if (m_ListCtrl.GetItemCount() > 0)
			m_ListCtrl.Selected(0);
		break;
	case    FREEBOARDCODE:
		ListFreeBoard(item);
		if (m_ListCtrl.GetItemCount() > 0)
			m_ListCtrl.Selected(0);
		break;
	case    OTHERCODE:
		break;
 	default:
 		break;
	}
}

void CJongmukDlg::LoadFavorites()	// 관심종목 로드..
{
	if (m_arrayFavor.GetSize() > 0)
		return;

	char		inBuffer[50];
	DWORD		dwReturn;
	CString		szKey, szTemp, path;
	FAVOR		stFavor;

	path = m_sRoot + "/user/"
		+ m_sUser + "/PortFolio.ini";

	for (int ii = 0 ; ii < 100 ; ii++)
	{
		szKey.Format("%02d", ii);
	
		ZeroMemory(inBuffer, sizeof(inBuffer));
		dwReturn = GetPrivateProfileString("GROUPNAME", szKey, "", inBuffer, 20, path);
		if (dwReturn)
		{
			stFavor.name = inBuffer;
			stFavor.fkey = szKey;
			stFavor.count = ii;

			if (FileCheck(ii))
				m_arrayFavor.Add(stFavor);	
		}
	}
}

bool CJongmukDlg::FileCheck(int fNo)
{
	bool rValue = true;

	CString filename, path;
	filename.Format("/PortFolio.i%02d", fNo);

	path = m_sRoot + "/user/"
		+ m_sUser + filename;

	CFileFind finder;
	if (finder.FindFile(path))
		rValue = true;
	else
		rValue = false;

	return rValue;
}

void CJongmukDlg::ListFavor(int key)
{
	CString	path;
	struct _inters interfile;
	struct _alarms	alarm;
	FLIST fl;
	int n = (int)m_arrayFavor.GetSize(), count = 0;

//	m_ListCtrl.DeleteAllItems();

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();

	if (n <= 0)
		return;

	path = m_sRoot + "/user/"
		+ m_sUser + "/PortFolio.i" + m_arrayFavor[key].fkey;

	try
	{
		CFile file(path, CFile::modeRead);

		UINT nRead;
		do
		{
			nRead = file.Read(&interfile, sizeof(interfile));
			if (nRead == sizeof(interfile))
			{
				fl.code = CString(interfile.code, sizeof(interfile.code));
				fl.name = CString(interfile.name, sizeof(interfile.name));
				fl.code.TrimRight(); fl.name.TrimRight();
				AddItem(count, fl.code, fl.name);
				count++;

				for (int jj = 0; jj < interfile.a_num; jj++)
					file.Read(&alarm, sizeof(alarm));
			}

		}while(nRead);

		file.Close();
	}
	catch(CFileException e)
	{
		e.Delete();
//		MessageBox("파일이 존재하지 않습니다.");
	}
	m_ListCtrl.SetRedraw(TRUE);
	int len = m_ListCtrl.GetItemCount();
	if (len < 18)
	{
		m_ListCtrl.SetColumnWidth(1, 148);
	}
}

void SwapData(int *a, int *b) /* 데이터 교환 함수 */
{
int temp;

temp = *a;
*a = *b;
*b = temp;
}

void MaxHeap(int list[], int root, int n)
{
	int child, rootkey, temp;

	temp = list[root];
	rootkey = list[root];
	child = 2 * root;
	while(child <= n) 
	{
		if((child < n) && (list[child] < list[child+1]))
			child++;
		if(rootkey > list[child])
			break;
		else {
			list[child/2] = list[child];
			child *= 2;
		}
	}
	list[child/2] = temp;
}

void CJongmukDlg::AddItem(int row, CString szCode, CString szName)
{
	LISTITEM* pitem;		
	LVITEM item;
	
	pitem = new LISTITEM;
	
	strcpy((char*)pitem->Code, szCode);
	strcpy((char*)pitem->Name, szName);

	item.lParam = (LPARAM)pitem;
	m_ListCtrl.AddCustomItem((DWORD)pitem);

	return;
	
/*	if (row >= m_ListCtrl.GetItemCount())
	{
		m_ListCtrl.InsertItem(row, "");
		pitem = new LISTITEM;
	}
	else
		pitem = (LISTITEM*)m_ListCtrl.GetItemData(row);

	m_ListCtrl.SetItemText(row, 0, szCode);
	m_ListCtrl.SetItemText(row, 1, szName);
	
	strcpy((char*)pitem->Code, szCode);
	strcpy((char*)pitem->Name, szName);

	item.lParam = (LPARAM)pitem;
	m_ListCtrl.SetItemData(row, (LPARAM)pitem);*/
}

void CJongmukDlg::OnKillfocusEditSearch() 
{
	// TODO: Add your control notification handler code here
//	UpdateData(FALSE);
}

void CJongmukDlg::clearEditSearch()
{
	m_edtSearch.SetWindowText("");
	((CEdit*)GetDlgItem(IDC_EDIT_SEARCH))->SetFocus();
}

void CJongmukDlg::OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int idx = m_ListCtrl.GetSelectionMark();

	if (m_kind == allCODE || (m_kind == indexCODE && m_Type == 99))
	{
		m_szCode = m_ListCtrl.GetItemText(idx, 0);
		m_szName = m_ListCtrl.GetItemText(idx, 1);
	}
	else
	{
		m_szCode = m_ListCtrl.GetItemText(idx, 0);
		m_szName = m_ListCtrl.GetItemText(idx, 1);
	}
		
	*pResult = 0;
}

void CJongmukDlg::InitBtn()
{
	UINT button[] = {
		IDC_BUTTON_ALLCODE, IDC_BUTTON_KOSPICODE, IDC_BUTTON_KOSDAQCODE, IDC_BUTTON_ECNCODE, 
		IDC_BUTTON_THIRDMARKET, IDC_BUTTON_KOSPIWORK, IDC_BUTTON_KOSDAQWORK, IDC_BUTTON_FAVORITES, 
		IDC_BUTTON_ELW, IDC_BUTTON_UPFREEBOARD, IDC_BUTTON_ETF
	};

	for (int ii = 0 ; ii < sizeof(button)/sizeof(UINT) ; ii++)
	{
		((CfxButton*)GetDlgItem(button[ii]))->SetBold(FALSE);
		((CfxButton*)GetDlgItem(button[ii]))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		((CfxButton*)GetDlgItem(button[ii]))->SetSelected(FALSE);
	}
	
}

void CJongmukDlg::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClickListCtrl(pNMHDR, pResult);
	OnOK();

	*pResult = 0;
}

BOOL CJongmukDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	static int row;
  	if (wParam == IDC_LIST_CTRL)// <-- 여기서 그리드 컨트롤을 구분..
 	{
 		switch(((LPNMHDR) lParam)->code)
 		{
 			case LVN_COLUMNCLICK:
 				#define pnm ((LPNMLISTVIEW)lParam)
				switch ((LPARAM)pnm->iSubItem)
				{
 				case 0:
 					m_gListsort = !m_gListsort;
 					break;
 				case 1:
 					m_gListsort = !m_gListsort;
					break;
 				}
				m_ListCtrl.SortCustomItem((LPARAM)pnm->iSubItem);
				ListView_SortItems((HWND)pnm->hdr.hwndFrom, ListViewCompareFunc, (LPARAM)pnm->iSubItem);
 				#undef pnm
				break;
 		}
 	}			
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CJongmukDlg::ListRemoveAll()
{
	m_ListCtrl.RemoveAllCustomItem();
	return;
/*	
	int count = m_ListCtrl.GetItemCount();

	LISTITEM* pitem;

	m_ListCtrl.SetColumnWidth(1, 132);

	if (count <= 0)
	{
		return;
	}
	for (int ii = 0; ii < count; ii++)
	{
		pitem =(LISTITEM*)m_ListCtrl.GetItemData(ii);
		SAFE_DELETE(pitem);
	}
	m_ListCtrl.SetColumnWidth(1, 132);
	m_ListCtrl.DeleteAllItems();	*/
}


void CJongmukDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CPen	pen(PS_SOLID, 1, RGB(128,128,128)), *poldpen;
	CFont	*poldfont;

	poldpen = dc.SelectObject(&pen);
	poldfont = dc.SelectObject(&m_Font);

	dc.MoveTo(10, 109);
	dc.LineTo(80, 109);
	dc.MoveTo(10, 182);
	dc.LineTo(80, 182);
	dc.MoveTo(10, 277);
	dc.LineTo(80, 277);

	dc.SelectObject(poldpen);
	dc.SelectObject(poldfont);
	pen.DeleteObject();
}

BOOL CJongmukDlg::FindCode(int kind, CString szCode, int type)		// GetName()함수에서 사용..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	JCode* jcode;
	UPCODE ucode;

	szCode.TrimRight();

	switch(kind)
	{
	case whichTYPE:
		{
			if (pApp->m_arrayDlgJCode->GetSize() <= 0)
				return FALSE;
				
			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize(); ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
					
				if (szCode == jcode->Code)
				{
					//SetName(jcode->Name);
					// 점검...
					if (jcode->ssgb == jmELW)
					{
						m_nJongmuk = jmELW;
					}
					else
					{
						switch(jcode->kosd)
						{
						case jmKOSPI:
							m_nJongmuk = 1;
							break;
						case jmKOSDAQ:
							m_nJongmuk = 2;
							break;
						case jm3RD:
							m_nJongmuk = 3;
							break;
						case jmMUFND:
//						case jmMUFND2:
							m_nJongmuk = 4;
							break;
						case jmREITS:
							m_nJongmuk = 5;
							break;
						/*
						case jmETF:
							m_nJongmuk = 6;
							break;
						*/	
						default:
							break;
						}
					}

					DeleteAllData();
					return TRUE;
				}
			}
			/*
			for (ii = 0; ii < m_arrayELWCode.GetSize(); ii++)
			{
				ecode = m_arrayELWCode.GetAt(ii);
				if (szCode == ecode->code)
				{
					SetName(ecode->name);
					m_nJongmuk = jmELW;
					DeleteAllData();
					return TRUE;
				}
			}
			*/
		}
		break;
	case codeNAME:
		{
			if (pApp->m_arrayDlgJCode->GetSize() <= 0)
				return FALSE;

			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize(); ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
				if (szCode == jcode->Code)
				{
					SetName(jcode->Name);
					// 점검...
					if (jcode->ssgb == jmELW)
						m_nJongmuk = jmELW;
					else
						m_nJongmuk = jcode->kosd;

					DeleteAllData();
					return TRUE;
				}
			}
			/*
			for (ii = 0; ii < m_arrayELWCode.GetSize(); ii++)
			{
				ecode = m_arrayELWCode.GetAt(ii);
				if (szCode == ecode->code)
				{
					SetName(ecode->name);
					m_nJongmuk = jmELW;
					DeleteAllData();
					return TRUE;
				}
			}
			*/
		}
		break;

	case indexNAME:
		{	
			if (pApp->m_arrayUpcode.GetSize() <= 0)
				return FALSE;

			CString rValue = _T("");
			bool	findFlag = false;
			UPCODE  UPCode;
			CString	ucod; 

			int ii = 0;
			for (ii = 0; ii < pApp->m_arrayUpcode.GetSize(); ii++)
			{
				UPCode = pApp->m_arrayUpcode.GetAt(ii);

				switch (UPCode.kind)
				{
				case upKOSPI:
					ucod.Format("%s%02d", UPCODE_KOSPI,atoi(UPCode.ucod));
					break;
				case upINTEGRATE:
					ucod.Format("%s%02d", UPCODE_INTEGRATE,atoi(UPCode.ucod));
					break;
				case upKOSDAQ:
					ucod.Format("%s%02d", UPCODE_KOSDAQ,atoi(UPCode.ucod));
					break;
				case upSECTOR:
					ucod.Format("%s%02d", UPCODE_SECTOR,atoi(UPCode.ucod));
					break;
				case upFREEBOARD:
					ucod.Format("%s%02d", UPCODE_FREEBOARD, atoi(UPCode.ucod));
					break;
				default:
					ucod = _T("");
					break;
				}

				if (szCode.Compare(ucod) == 0)
				{
					rValue = UPCode.name;
					findFlag = true;
					break;
				}
			}
			if (!findFlag)
			{
				for (ii = 0 ; ii < m_exCode[EXKOSPI].GetSize() ; ii++)
				{
					if (szCode.Compare(m_exCode[EXKOSPI][ii].code) == 0)
					{
						rValue = m_exCode[EXKOSPI][ii].name;
						findFlag = true;
						break;
					}
				}
			}
			if (!findFlag)
			{
				for (ii = 0 ; ii < m_exCode[EXKOSDAQ].GetSize() ; ii++)
				{
					if (szCode.Compare(m_exCode[EXKOSDAQ][ii].code) == 0)
					{
						rValue = m_exCode[EXKOSDAQ][ii].name;
						findFlag = true;
						break;
					}
				}
			}
			if (findFlag)
			{
				SetName(rValue);
				return TRUE;
			}
/*		
				ucode = m_aUpcode.GetAt(ii);
				if (szCode == ucode.name && type == ucode.kind)
				{
					SetName(ucode.name);
					
					return TRUE;
				}
*/
		}
		break;

	case thirdNAME: 
		break;
	case ecnNAME: 
		break;
	}
	
	return FALSE;
}

void CJongmukDlg::DeleteAllData()
{
	m_arSelCode.RemoveAll();
}

void CJongmukDlg::SetListCtrl()
{
//	m_ListCtrl.SetFocus();
//	((CEdit*)GetDlgItem(IDC_EDIT_SEARCH))->SetFocus();
}
/////////////////////////////////////////////////////////////////////////////////////////////

void CJongmukDlg::InitOption()
{ 
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_ctlButtonECN.SetWindowText("섹터지수");
	m_ctlButtonTHIRD.SetWindowText("해외종목");
//	m_ctlButtonFavor.ShowWindow(SW_HIDE);
//	m_ctlButtonFavor.EnableWindow(FALSE);
//	m_ctlButtonKOSPIWORK.EnableWindow(TRUE);
//	m_ctlButtonKOSDAQWORK.EnableWindow(TRUE);

	FCODE fcode;
	CString strName("");
	SetOptionMonth();
			
	fcode = pApp->m_arrayFcode[0];
	fcode.name.TrimRight();
	m_CtlButFuture1.SetWindowText(fcode.name);
	fcode = pApp->m_arrayFcode[1];
	fcode.name.TrimRight();
	m_CtlButFuture2.SetWindowText(fcode.name);
	fcode = pApp->m_arrayFcode[2];
	fcode.name.TrimRight();
	m_CtlButFuture3.SetWindowText(fcode.name);
	fcode = pApp->m_arrayFcode[3];
	fcode.name.TrimRight();
	m_CtlButFuture4.SetWindowText(fcode.name);

}

void CJongmukDlg::Hideall(int flag)
{
	m_ListCtrl.ShowWindow(SW_HIDE);
	m_ComboGroup.ShowWindow(SW_HIDE);

	m_CtlButFuture1.ShowWindow(SW_HIDE);
	m_CtlButFuture2.ShowWindow(SW_HIDE);
	m_CtlButFuture3.ShowWindow(SW_HIDE);
	m_CtlButFuture4.ShowWindow(SW_HIDE);
	m_ListCall.ShowWindow(SW_HIDE);
	m_ListPut.ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_OPTION))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_CALL))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_PUT))->ShowWindow(SW_HIDE);

	switch (flag)
	{
	case 1: // 보통
		m_CtlButFuture1.ShowWindow(SW_SHOW);
		m_CtlButFuture2.ShowWindow(SW_SHOW);
		m_CtlButFuture3.ShowWindow(SW_SHOW);
		m_CtlButFuture4.ShowWindow(SW_SHOW);
		m_ListCall.ShowWindow(SW_SHOW);
		m_ListPut.ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO1))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO2))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO3))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_RADIO4))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_STATIC_OPTION))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_STATIC_CALL))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_STATIC_PUT))->ShowWindow(SW_SHOW);
		break;
	case 0: // 옵션
		m_ListCtrl.ShowWindow(SW_SHOW);
		m_ComboGroup.ShowWindow(SW_SHOW);
		break;
	case 2: // 해외
		m_ListCtrl.ShowWindow(SW_SHOW);
		break;
	}
}

void CJongmukDlg::OnButtonFuture1() 
{
	// TODO: Add your control notification handler code here
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode = pApp->m_arrayFcode[0];
	
	SetCode(fcode.code);
	SetName(fcode.name);
}

void CJongmukDlg::OnButtonFuture2() 
{
	// TODO: Add your control notification handler code here
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode = pApp->m_arrayFcode[1];
	
	SetCode(fcode.code);
	SetName(fcode.name);
}

void CJongmukDlg::OnButtonFuture3() 
{
	// TODO: Add your control notification handler code here
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode = pApp->m_arrayFcode[2];
	
	SetCode(fcode.code);
	SetName(fcode.name);
}

void CJongmukDlg::OnButtonFuture4() 
{
	// TODO: Add your control notification handler code here
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode = pApp->m_arrayFcode[3];
	
	SetCode(fcode.code);
	SetName(fcode.name);
}

void CJongmukDlg::OnDoubleclickedButtonFuture1() 
{
	// TODO: Add your control notification handler code here
	OnButtonFuture1();
	OnOK();
}

void CJongmukDlg::OnDoubleclickedButtonFuture2() 
{
	// TODO: Add your control notification handler code here
	OnButtonFuture2();
	OnOK();
}

void CJongmukDlg::OnDoubleclickedButtonFuture3() 
{
	// TODO: Add your control notification handler code here
	OnButtonFuture3();
	OnOK();
}

void CJongmukDlg::OnDoubleclickedButtonFuture4() 
{
	// TODO: Add your control notification handler code here
	OnButtonFuture4();
	OnOK();
}

BOOL CJongmukDlg::SetOptionMonth()
{									// 옵션 코드 종목...
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	if (m_kind != codeNAME)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetWindowText(pApp->m_ArrayOMonth[3]);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetWindowText(pApp->m_ArrayOMonth[2]);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetWindowText(pApp->m_ArrayOMonth[1]);
		((CButton*)GetDlgItem(IDC_RADIO4))->SetWindowText(pApp->m_ArrayOMonth[0]);
	}
	return TRUE;
}

void CJongmukDlg::Filllistoption(int sel)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	struct  ojcode  OJCode;
	DeleteAllItem();
	
	int csel = 0;
	int ct = 0;
	CString szCall, szPut, szCallnam, szPutnam, szTempcall, szTempput;

	csel = sel - 3;
	csel = abs(csel);

	int count = (int)pApp->m_arrayOcode.GetSize();

	for (int ii = 0; ii < count; ii++)
	{
		OJCode = pApp->m_arrayOcode.GetAt(ii);
		
		//int n = OJCode.call[csel].yorn;
		if (OJCode.call[csel].yorn == '1')
		{
			szCall = CString(OJCode.call[csel].cod2, 8);
			szCall.TrimRight();
			szPut = CString(OJCode.put[sel].cod2, 8);
			szPut.TrimRight();

			szCallnam = CString(OJCode.call[csel].hnam, 20);
			szCallnam.TrimRight();
			szPutnam = CString(OJCode.put[sel].hnam, 20);
			szPutnam.TrimRight();

			szTempcall = szCall + " " + szCallnam;
			szTempput = szPut + " " + szPutnam;

			m_ListCall.InsertString(ct, szTempcall);
			m_ListPut.InsertString(ct, szTempput);

			ct++;
		}
	}
}

void CJongmukDlg::DeleteAllItem()
{
	int nCount = m_ListCall.GetCount();
	
	for (int ii = 0; ii < nCount; ii++)
	{
		m_ListCall.DeleteString(0);
		m_ListPut.DeleteString(0);
	}
}

void CJongmukDlg::OnDblclkListCall() 
{
	// TODO: Add your control notification handler code here
	CString szTemp;
	int item = m_ListCall.GetCurSel();
	
	m_ListCall.GetText(item , szTemp);

	SetCode(Parser(szTemp, " "));
	SetName(szTemp);

	OnOK();
}

void CJongmukDlg::OnDblclkListPut() 
{
	// TODO: Add your control notification handler code here
	CString szTemp;
	int item = m_ListPut.GetCurSel();
	
	m_ListPut.GetText(item , szTemp);

	SetCode(Parser(szTemp, " "));
	SetName(szTemp);

	OnOK();
}

CString CJongmukDlg::Parser(CString &srcstr, CString substr)
{
	CString temp;
	
	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	
	return "";
}

bool CJongmukDlg::CheckDigit(CString code)
{
	bool rValue = true;

	for (int ii = 0 ; ii < code.GetLength() ; ii++)
	{
		char c = code[ii];
		if (!isdigit(c))
		{
			rValue = false;
			break;
		}
	}

	return rValue;
}

bool CJongmukDlg::GetLinkCode(CString szCode, char* code)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	int	kind = -1;
	CString rName, result = _T("");

	if (szCode.GetLength() == 3 && CheckDigit(szCode))
		kind = indexNAME;
	else if (szCode.GetLength() == 6 && CheckDigit(szCode))
		kind = codeNAME;
	else if (szCode.GetLength() == 8 && szCode[0] == '1' || szCode[0] == '4')
		kind = futureNAME;
	else if (szCode.GetLength() == 8 && szCode[0] == '2' || szCode[0] == '3')
		kind = optionNAME;
	else
		kind = 1;

	bool	bReg = false;
	switch (kind)
	{
	case 1:	// 해외종목
		{
			for (int ii = 0 ; ii < pApp->m_arrayUSCode.GetSize(); ii++)
			{
				if (pApp->m_arrayUSCode[ii]->symb == szCode)
				{
					rName = pApp->m_arrayUSCode[ii]->knam;
					bReg = true;
					break;
				}
			}
			DeleteAllData();
		}
		break;
	case codeNAME:
		{
			GetExCode();
			struct JCode* jcode;
			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize() ; ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
				if (jcode->Code == szCode)
				{
					rName = jcode->Name;
					result = CompareQuote(szCode);
					bReg = true;
					break;
				}
			}
			DeleteAllData();
		}
		break;
	case indexNAME:
		{
			szCode.Format("%03d", atoi(szCode));

			GetExCode();

			int	gubun1 = -1, gubun2 = -1;
			CString	G_STR = _T("");
//			if (szCode[0] == '0' || szCode[0] == '1' || szCode[0] == '4')
			if (szCode[0] == '0')
			{
				gubun1 = upKOSPI;
				gubun2 = BTN_KPIUP;
				G_STR = STR_KOSPI;
			}
//			else if (szCode[0] == '2' || szCode[0] == '3')
			else if (szCode[0] == '2')
			{
				gubun1 = upKOSDAQ;
				gubun2 = BTN_KDQUP;
				G_STR = STR_KOSDAQ;
			}

			for (int ii = 0 ; ii < pApp->m_arrayUpcode.GetSize() ; ii++)
			{
				if (atoi(pApp->m_arrayUpcode[ii].ucod) == atoi(szCode.Right(2))
					 && pApp->m_arrayUpcode[ii].kind == gubun1)
				{
//					rName = m_aUpcode[ii].name;
					rName.Format("%s%s", G_STR, pApp->m_arrayUpcode[ii].name);
					result = CompareIndex(szCode, gubun2, false);

					bReg = true;
					break;
				}
			}

			struct _exUP {
				char*	code;
				char*	name;
				int	gubun2;
				char*	g_str;
			} exUP[] = {
				{"101", "KOSPI200", BTN_KPIUP, STR_KOSPI}
				, {"401", "KOSPI100", BTN_KPIUP, STR_KOSPI}
				, {"402", "KOSPI50", BTN_KPIUP, STR_KOSPI}
				, {"303", "KOSTAR", BTN_KDQUP, STR_KOSDAQ}
			};
			
			if (!bReg)
			{
				for (int ii = 0 ; ii < sizeof(exUP)/sizeof(_exUP) ; ii++)
				{
					if (szCode.Compare(exUP[ii].code) == 0)
					{
//						rName = exUP[ii].name;
						rName.Format("%s%s", exUP[ii].g_str, exUP[ii].name);
						result = CompareIndex(szCode, exUP[ii].gubun2, false);

						bReg = true;
						break;
					}
				}
			}
		}
		break;
	case futureNAME:
		{
			for (int ii = 0 ; ii < pApp->m_arrayFcode.GetSize() ; ii++)
			{
				if (pApp->m_arrayFcode[ii].code == szCode)
				{
					rName = pApp->m_arrayFcode[ii].name;
					result = CompareFuture(szCode);
					bReg = true;
					break;
				}
			}
		}
		break;
	case optionNAME:
		{
			for (int ii = 0 ; ii < pApp->m_arrayOcode.GetSize() ; ii++)
			{
				for (int jj = 0 ; jj < 4 ; jj++)
				{
					if (szCode[0] == '2')
					{
						if (pApp->m_arrayOcode[ii].call[jj].cod2 == szCode 
							&& pApp->m_arrayOcode[ii].call[jj].yorn == '1')
						{
							rName = pApp->m_arrayOcode[ii].call[jj].hnam;
							bReg = true;
						}
					}
					else if (szCode[0] == '3')
					{
						if (pApp->m_arrayOcode[ii].put[jj].cod2 == szCode
							&& pApp->m_arrayOcode[ii].put[jj].yorn == '1')
						{
							rName = pApp->m_arrayOcode[ii].put[jj].hnam;
							bReg = true;
						}
					}

					if (bReg)
					{
						result = CompareFuture(szCode);
						break;
					}
				}

				if (bReg)
					break;
			}
		}
		break;
	}

	if (bReg)
	{
		CString rValue;
		if (result.IsEmpty())
		{
			rValue.Format("%s%c%s", szCode, cTAB, rName);
		}
		else
			rValue.Format("%s%c%s%c%s", szCode, cTAB, rName, cTAB, result);

		strcpy(code, rValue);
		return true;
	}
	else
	{
		return false;
	}
}

CString CJongmukDlg::GetGroup(CString szCode)
{
	CString rValue = _T("");

	switch (m_btnSel)
	{
	case BTN_TOTAL:
	case BTN_KOSPI:
	case BTN_KOSDAQ:
		GetExCode();
		rValue = CompareQuote(szCode);
		DeleteAllData();
		break;
	case BTN_FUTURE:
		rValue = CompareFuture(szCode);
		break;
	case BTN_USCODE:
		break;
	case BTN_KPIUP:
	case BTN_KDQUP:
		rValue = CompareIndex(szCode, m_btnSel);
		break;
	case BTN_FAVOR:
		{
			char	link[1024];
			ZeroMemory(link, sizeof(link));

			GetLinkCode(szCode, link);

			rValue = CString(link);
			CString strTmp;
			strTmp = Parser(rValue, cTAB);
			strTmp = Parser(rValue, cTAB);
		}
		break;
	}

	return rValue;
}

CString CJongmukDlg::CompareIndex(CString szCode, int gubun, bool index/* = true */)
{
	CString rValue = _T("");
	CString sgCode, sgName, G_CODE, G_NAME, G_STR = _T("");

	switch (gubun)
	{
	case BTN_KPIUP:
		G_CODE = CODE_DOW;
		G_NAME = _T("다우존스");
		if (index)	G_STR = _T("$1");

		if (szCode == KPI_TOTAL)
		{
			sgCode = KDQ_TOTAL;
			sgName.Format("%s%s", STR_KOSDAQ, KPI_TOTAL_STR);
		}
		else
		{
			sgCode = KPI_TOTAL;
			sgName.Format("%s%s", STR_KOSPI, KDQ_TOTAL_STR);
		}

		break;
	case BTN_KDQUP:
		G_CODE = CODE_NAS;
		G_NAME = _T("나스닥");
		if (index)	G_STR = _T("$2");
		
		if (szCode == KDQ_TOTAL)
		{
			sgCode = KPI_TOTAL;
			sgName.Format("%s%s", STR_KOSPI, KDQ_TOTAL_STR);
		}
		else
		{
			sgCode = KDQ_TOTAL;
			sgName.Format("%s%s", STR_KOSDAQ, KPI_TOTAL_STR);
		}

		break;
	}

	rValue.Format("%s%s%c%s%c%s%c%s", G_STR, sgCode, cTAB, sgName, cTAB, G_CODE, cTAB, G_NAME);

	return rValue;
}

CString CJongmukDlg::CompareFuture(CString szCode)
{
	CString rValue = _T("");
	CString sgCode, sgName, G_CODE, G_NAME;

	sgCode = KPI200;
	sgName.Format("%s코스피200", STR_KOSPI);
	G_CODE = CODE_DOW;
	G_NAME = _T("다우존스");

	rValue.Format("%s%c%s%c%s%c%s", sgCode, cTAB, sgName, cTAB, G_CODE, cTAB, G_NAME);

	return rValue;
}

CString CJongmukDlg::CompareQuote(CString szCode)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	CString rValue = _T("");

	int kosd=0, size=0, ucdm=0, jjug=0, ucds=0;
	struct JCode* jcode=NULL;
	int ii = 0;
	for (ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize() ; ii++)
	{
		jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
		if (jcode->Code == szCode)
		{
			kosd = (int)jcode->kosd;
			size = (int)jcode->size;
			ucdm = (int)jcode->ucdm;
			jjug = (int)jcode->jjug;
			ucds = (int)jcode->ucds;
			break;
		}
	}
	
	int	comp = -1, upgub = -1;
	CString F_UPCODE, G_NAME, G_CODE, G_STR;
	if (kosd == jmKOSPI)
	{
		upgub = upKOSPI;
		F_UPCODE = UPCODE_KOSPI;
		G_CODE = KPI_TOTAL;
		G_STR = STR_KOSPI;
		G_NAME.Format("%s종합주가지수", STR_KOSPI);

		// 제조업(27)은 제외
		if (ucds < 99)				comp = ucds;
		else if (jjug < 99 && jjug != 27)	comp = jjug;
		else if (ucdm < 99)			comp = ucdm;
		else if (size < 99)			comp = size;

//		TRACE("[%s] >> [%d][%d][%d][%d] >> [%d]\n", szCode, ucds, jjug, ucdm, size, comp);
	}
	else if (kosd == jmKOSDAQ)
	{
		upgub = upKOSDAQ;
		F_UPCODE = UPCODE_KOSDAQ;
		G_CODE = KDQ_TOTAL;
		G_STR = STR_KOSDAQ;
		G_NAME.Format("%s종합지수", STR_KOSDAQ);

		if (ucds < 99)			comp = ucds;
		else if (ucdm < 99)		comp = ucdm;
		else if (size < 99)		comp = size;
		else if (jjug < 99)		comp = jjug;

//		TRACE("[%s] >> [%d][%d][%d][%d] >> [%d]\n", szCode, ucds, ucdm, size, jjug, comp);
	}

	CString sgCode = _T("");
	CString sgName = _T("");
	for (ii = 0 ; ii < pApp->m_arrayUpcode.GetSize() ; ii++)
	{
		if (atoi(pApp->m_arrayUpcode[ii].ucod) == comp && pApp->m_arrayUpcode[ii].kind == upgub)
		{
			sgCode.Format("%s%02d", F_UPCODE, comp);
			sgName.Format("%s", pApp->m_arrayUpcode[ii].name);
			
//			TRACE("result1 >> [%s][%s]\n", sgCode, sgName);
//			TRACE("result2 >> [%s][%s]\n", G_CODE, G_NAME);

			rValue.Format("%s%c%s%s%c%s%c%s", sgCode, cTAB, G_STR, sgName, cTAB, G_CODE, cTAB, G_NAME);
//			MessageBox(rValue);
			break;
		}
	}

	return rValue;
}

bool CJongmukDlg::IsCodeType(CString code, int type)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	bool	rValue = false; 

	switch (type)
	{
	case 1: // Sector 지수
		{
			JCode*	jcode;
			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize() ; ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
				if (jcode->Code.Compare(code) == 0)
				{
					if (*(unsigned short*)jcode->stgb != 0)
						rValue = true;

					break;
				}
			}
			DeleteAllData();
		}
		break;
	case 2: // ETF
		{
			JCode*	jcode;
			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize() ; ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
				if (jcode->Code.Compare(code) == 0)
				{
					if (jcode->ssgb == jmETF)
						rValue = true;

					break;
				}
			}
			DeleteAllData();
		}
		break;
	case 3: // 해외 ETF
		{
			JCode*	jcode;
			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize() ; ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
				if (jcode->Code.Compare(code) == 0)
				{
					if (jcode->ssgb == jmFETF)
						rValue = true;

					break;
				}
			}
			DeleteAllData();
		}
		break;
	case 4: // 해외원주
		{
			JCode*	jcode;
			for (int ii = 0 ; ii < pApp->m_arrayDlgJCode->GetSize() ; ii++)
			{
				jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
				if (jcode->Code.Compare(code) == 0)
				{
					if (jcode->ssgb == jmFREG)
						rValue = true;

					break;
				}
			}
			DeleteAllData();
		}
		break;
	}

	return rValue;
}

void CJongmukDlg::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc); 
}


int CALLBACK ListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	_listItem *pItem1 = (_listItem *) lParam1; 
	_listItem *pItem2 = (_listItem *) lParam2; 

	int	iCmp = 0;
	switch ((int)lParamSort)
	{
	case 0:
		iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 
		if (!strcmp((char*)pItem1->Code, "999") || !strcmp((char*)pItem2->Code, "999"))
			iCmp *= -1;
		break;
	case 1:
		iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
		break;
	}

	if (m_gListsort)
		iCmp *= -1;

	return iCmp;
}

BOOL CJongmukDlg::FindELWJSCode(CString code)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	SetCode("");
	CString path;
	CString cmpCode, cmpJS, nameS;
	int cnt = 0;
	bool bFind = false;
	struct elwcode  ELWCode;
	
	cnt = (int)pApp->m_arrayELWCode.GetSize();
	
	for (int ii = 0; ii < cnt; ii++)
	{
		ELWCode = pApp->m_arrayELWCode.GetAt(ii);
		cmpCode = CString(ELWCode.codx, ELWCodeLen);
		cmpCode.TrimRight();

		if (code.CompareNoCase(cmpCode) == 0)
		{
			nameS = CString(ELWCode.hnam, ELWHNameLen);
			nameS.TrimRight();
			bFind = true;
			break;
		}
	}

	if (!bFind)
		return FALSE;

	int pos = 0;
	if ((pos = nameS.Find("바스켓")) > 0)
	{
		SetCode("000000");	// 기초자산코드없는것과 비교하기위함
		return TRUE;
	}

	for (int jj = 0; jj < 5; jj++)
	{
		cmpJS = CString((char*)ELWCode.gcjs[jj], 12);
		cmpJS.TrimRight();
		if (cmpJS.GetLength() > 0)
		{
			if (cmpJS.GetAt(0) == 'A')
				cmpJS = cmpJS.Mid(1);
			SetCode(cmpJS);
			return TRUE;
		}
	}
	return TRUE;
}

void CJongmukDlg::OnButtonEtf() 
{
	// TODO: Add your control notification handler code here
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	m_btnSel = BTN_KOSDAQ; clearEditSearch();
	Hideall(0);

	CString str;
	int length = (int)pApp->m_arrayDlgJCode->GetSize();

	InitBtn();
	m_ctlButtonETF.SetBold(TRUE);
	m_ctlButtonETF.SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
	m_ctlButtonETF.SetSelected(TRUE);

	m_aTemp.RemoveAll();
	
	ShowCombo(FALSE);
	HideButton();

	for (int ii = 0; ii < length; ii++)
	{
		struct JCode* jcode = (struct JCode*)pApp->m_arrayDlgJCode->GetAt(ii);
			
		if (jcode->ssgb == jmETF || jcode->ssgb == jmFETF)
			m_aTemp.Add(jcode);
	}

	CodeListSome();
	m_ListCtrl.Selected(0);

	m_szCode = m_ListCtrl.GetItemText(0, 0);
	m_szName = m_ListCtrl.GetItemText(0, 1);
}

void CJongmukDlg::OnSelendokComboSearch() 
{
	//m_bAddAll = false;
	if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
		searchAction(1);
	else
		searchWord();
}

void CJongmukDlg::searchAction(int column)
{
	CString	string; m_edtSearch.GetWindowText(string);

	string.MakeUpper();
	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{
			CString		code, name;
			struct JCode*	jcode;

			m_ListCtrl.SetRedraw(FALSE);
			ListRemoveAll();
			for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(ii);

				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)	name = name.Mid(1);

				code = jcode->Code;
				name.TrimRight();
				AddItem(ii, code, name);
			}
			m_bAddAll = true;
			m_ListCtrl.SetRedraw(TRUE);
		}

		if (m_ListCtrl.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_gListsort = FALSE;
				ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, column);

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
	}
	else			// name search
	{
		m_bAddAll = SearchData(string, m_bAddAll);
		if (m_ListCtrl.GetItemCount() > 0)
		{
			m_gListsort = FALSE;
			ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, column);
			m_ListCtrl.Selected(0);
			m_szCode = m_ListCtrl.GetItemText(0, 0);
			m_szName = m_ListCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}
	}
}

void CJongmukDlg::searchWord()
{
	CString		word;
	CString		code, name;
	struct JCode*	jcode;

	int idx = 0;
	m_edtSearch.GetWindowText(word);
	word.TrimRight();
	word.MakeUpper();

	if (word.IsEmpty())
	{
		if (m_ListCtrl.GetItemCount() != m_arSelCode.GetSize())
		{
			CString		code, name;
			struct JCode*	jcode;

			m_ListCtrl.SetRedraw(FALSE);
			ListRemoveAll();
			for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(ii);

				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)	name = name.Mid(1);

				code = jcode->Code;
				name.TrimRight();
				AddItem(ii, code, name);
			}
			m_bAddAll = true;

			if (m_ListCtrl.GetItemCount() > 0)
			{
				m_gListsort = FALSE;
				ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ListViewCompareFunc, 1);
				m_ListCtrl.Selected(0);
				m_szCode = m_ListCtrl.GetItemText(0, 0);
				m_szName = m_ListCtrl.GetItemText(0, 1);
				m_szName.TrimLeft();
			}
			m_ListCtrl.SetRedraw(TRUE);
		}
	}
	else
	{
		m_ListCtrl.SetRedraw(FALSE);
		ListRemoveAll();
		for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
		{
			jcode = (struct JCode*)m_arSelCode.GetAt(ii);

			name = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	name = name.Mid(1);

			if (name.Find(word, 0) != -1)
			{
				code = jcode->Code;
				name.TrimRight();
				AddItem(idx, code, name);
				idx++;
			}
		}
		m_ListCtrl.SetRedraw(TRUE);
	}
}

int CJongmukDlg::findMatchIndex(CString text, int column)
{
	CString	string;
	int	textL = text.GetLength();
	for (int ii = 0; ii < m_ListCtrl.GetItemCount(); ii++)
	{
		string = m_ListCtrl.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

bool CJongmukDlg::SearchData(CString sName, bool bAddAll)
{
	CString	code, name;
	struct JCode*	jcode;

	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			m_ListCtrl.SetRedraw(FALSE);
			ListRemoveAll();
			for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(ii);
				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)	name = name.Mid(1);

				code = jcode->Code;
				name.TrimRight();
				AddItem(ii, code, name);
			}
			m_ListCtrl.SetRedraw(TRUE);
		}
		return true;
	}

	m_ListCtrl.SetRedraw(FALSE);
	ListRemoveAll();

	int	idx = 0;
	char	cName;
	int	sLen = sName.GetLength();
	if (sLen == 1)
	{	// 일반검색적용(길이만큼 맞는것만 적용)
		int	sLen = sName.GetLength();
		for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
		{
			jcode = (struct JCode*)m_arSelCode.GetAt(ii);
			name = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	name = name.Mid(1);
			if (strncmp(sName, name, sLen) != 0)
				continue;

			code = jcode->Code;
			name.TrimRight();
			AddItem(idx++, code, name);
		}
	}
	else
	{	// 한글 검색
		WORD	wHangul, wHangul2, wStart, wEnd;
		CPtrArray	arSearch;
		arSearch.RemoveAll();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for (int jj = 0; jj < m_arSelCode.GetSize(); jj++)
				{
					jcode = (struct JCode*)m_arSelCode.GetAt(jj);
					name = CString(jcode->Name, jcode->Name.GetLength());
					//if (jcode->ssgb != jmELW)		name = name.Mid(1);

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)	arSearch.Add(jcode);
				}
			}
			else
			{	// 한글 비교

				for (int jj = 0; jj < m_arSelCode.GetSize(); jj++)
				{
					jcode = (struct JCode*)m_arSelCode.GetAt(jj);
					name = CString(jcode->Name, jcode->Name.GetLength());
					//if (jcode->ssgb != jmELW)		name = name.Mid(1);

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wHangul == wHangul2)	arSearch.Add(jcode);
				}
			}
			ii += 2;
		}
		else			// 그외는 1Byte
		{
			for (int jj = 0; jj < m_arSelCode.GetSize(); jj++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(jj);
				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)		name = name.Mid(1);

				if (name.GetLength() < 1)	continue;
				if (cName == name.GetAt(ii))	arSearch.Add(jcode);
			}
			ii += 1;
		}

		for (; ii < sLen && arSearch.GetSize();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// 한글일땐 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// 초성 비교
					for (int jj = (int)arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						jcode = (struct JCode*)arSearch.GetAt(jj);
						name = CString(jcode->Name, jcode->Name.GetLength());
						//if (jcode->ssgb != jmELW)	name = name.Mid(1);

						if (name.GetLength() < ii+2)
						{
							arSearch.RemoveAt(jj);
							continue;
						}
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)	arSearch.RemoveAt(jj);
					}
				}
				else
				{	// 한글 비교
					for (int jj = (int)arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						jcode = (struct JCode*)arSearch.GetAt(jj);
						name = CString(jcode->Name, jcode->Name.GetLength());
						//if (jcode->ssgb != jmELW)	name = name.Mid(1);

						if (name.GetLength() < ii+2)
						{
							arSearch.RemoveAt(jj);
							continue;
						}
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wHangul != wHangul2)
							arSearch.RemoveAt(jj);
					}
				}
				ii += 2;
			}
			else			// 그외는 1Byte
			{
				for (int jj = (int)arSearch.GetUpperBound(); jj >= 0; jj--)
				{
					jcode = (struct JCode*)arSearch.GetAt(jj);
					name = CString(jcode->Name, jcode->Name.GetLength());
					//if (jcode->ssgb != jmELW)	name = name.Mid(1);

					if (name.GetLength() < ii+1)
					{
						arSearch.RemoveAt(jj);
						continue;
					}
					if (cName != name.GetAt(ii))	arSearch.RemoveAt(jj);
				}
				ii += 1;
			}
		}

		for (ii = 0; ii < arSearch.GetSize(); ii++)
		{
			jcode = (struct JCode*)arSearch.GetAt(ii);
			name = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	name = name.Mid(1);

			code = jcode->Code;
			name.TrimRight();
			AddItem(idx++, code, name);
		}
	}
	m_ListCtrl.SetRedraw(TRUE);

	return false;
}

bool CJongmukDlg::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// 초성만 가진 한글인지~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;

	WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
		0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
		0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};

	WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
		0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
		0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	WORD	pHangulEnd[19] = 
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

WORD CJongmukDlg::MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	WORD	wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}

