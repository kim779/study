// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Page1.h"
#include "page4.h"
#include "NewGroup.h"
#include "EditGroup.h"
#include "ShareGroupDlg.h"
#include "ioformat.h"
#include "imm.h"


#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	STANDARDNUM	11

#define SAVELOGFILE

//#define ucKOSPI		"0"
//#define ucKOSDAQ		"2"

// tree_setitemdata()
constexpr	int xALLCODE = 1, xKOSPI = 2, xKOSDAQ = 3, xFUTURE = 4, xCALLCODE = 5, xPUTCODE = 6;
constexpr	int xKOPSI200 = 7,  xKOSDAQ50 = 8, xKRX100 = 9, xFREECODE = 10, xETFCODE = 11, xELWCODE = 12, xELWBASE = 13, xSINJUCODE = 14;
constexpr	int xKOSPIJISU = 15, xKOSDAQJISU = 16, xTHEMACODE = 17, xGROUPCODE = 18, xINTEREST = 19;
constexpr	int xELWISSUE = 20, xELWASSET = 21;   //22는 ELW의 풋이 사용
constexpr	int xRecommand = 22, xETFALLCODE = 31, xELWSelect = 40, xELWPB = 41;
constexpr	int xKOSPIFUTURE = 42, xSTOCKFUTURE = 43, xINTERESTRATEFUTURE = 44, xCURRENCYFUTURE = 45;
constexpr	int xKONEX = 46;		//2013.06.25 코넥스 추가
constexpr	int xETNCODE = 47;		//2014.11.06 KSJ ETN 추가
constexpr	int xSPACCODE = 48;		//2015.11.03 KSJ 스팩 추가
constexpr	int xRITS = 49;					//20240422 리츠 //20240422
constexpr	int xALLNEWCIDE = 50;   //20240422 신종증권전체 //20240422
constexpr	int xSINTAKST = 51;        //20240422 신탁수익증권 //20240422
constexpr	int xTUSACONST = 52;   //20240422 투자계약증권 //20240422

constexpr	int xKBc = 101, xSECc = 102, xSKc = 103, xPOSCOc = 104, xKTc = 105, xKEPCOc = 106, xHYUNDAIc = 107;
constexpr	int xKBp = 201, xSECp = 202, xSKp = 203, xPOSCOp = 204, xKTp = 205, xKEPCOp = 206, xHYUNDAIp = 207;
constexpr	int xKOSPIUP = 50, xKOSDAQUP = 51, xKOSPIex = 52, xKOSDAQex = 53;

constexpr       int xGOODSTOCK = 122;
constexpr       int xNXT = 123;

constexpr struct _exKospi {
	char* code{};
	char* name{};
} exKospi[] = {
	{"101", "KOSPI200"},	{"401", "KOSPI100"},
	{"402", "KOSPI50"},	{"403", "KOSPI IT"},
};

constexpr int cnt_exKospi = sizeof(exKospi) / sizeof(struct _exKospi);

constexpr struct _exKosdaq {
	char* code{};
	char* name{};
} exKosdaq[] = {
	//{"301", "KOSDAQ50"},		// 20070608
//	{"242", "프리미어"},
	{"303", "KOSTAR"},
};

constexpr int cnt_exKosdaq = sizeof(exKosdaq) / sizeof(struct _exKosdaq);
constexpr char* xxKOSPI[] = {
     "",
     "KGG01P", //코스피         1
     "KGZ01P", //코스피 대형주	2	
     "KGZ02P", //코스피 중형주	3
     "KGZ03P", //코스피 소형주	4
     "KGS01P", //음식료품	5
     "KGS02P", //섬유, 의복	6
     "KGS03P", //종이, 목재	7
     "KGS04P", //화학		8
     "KGS05P", //의약품		9	
     "KGS06P", //비금속광물	10
     "KGS07P", //철강및금속	11
     "KGS08P", //기계		12
     "KGS09P", //전기, 전자	13
     "KGS19P", //의료정밀	14
     "KGS10P", //운수장비	15
     "KGS11P", //유통업		16
     "KGS20P", //전기가스업	17
     "KGS12P", //건설업		18	
     "KGS13P", //운수창고	19
     "KGS21P", //통신업		20
     "KGS14P", //금융업		21
     "KGS15P", //은행		22
     "",
     "KGS16P", //증권		24
     "KGS17P", //보험		25
     "KGS22P", //서비스업	26
     "KGS18P"  //제조업		27
};

constexpr char* xxKOSDAQ[] = { "", "", "", "", "", "",

     "QGS02P",    // 제조 6
     "QGS03P",    // 건설 7
     "QGS04P",	  // 유통 8
     "",
     "QGS31P",	  // 운송 10 
     "QGS05P",    // 금융 11
     "QGS06P",    // 통신방송 12
     "QGS07P",	  // IT S / W & SVC  13
     "QGS08P",	  // IT H / W  14      
     "QGS09P",    // 음식료·담배  15 
     "QGS10P",	  // 섬유·의류  16
     "QGS11P",    // 종이 목재   17
     "QGS12P",	  // 출판·매체복제  18
     "QGS13P",	  // 화학         19
     "QGS33P",	  // 제약         20
     "QGS14P",    // 비금속	  21
     "QGS15P",	  // 금 속        22
     "QGS16P",    // 기계·장비   23
     "QGS17P",	  // 일반전기전자 24
     "QGS18P",	  // 의료·정밀기기   25
     "QGS19P",	//   운송장비·부품   26
     "QGS20P",  //   기타 제조        27
     "QGS21P",  //   통신서비스       28
     "QGS22P",  //   방송서비스       29
     "QGS23P",	//   인터넷	      30
     "QGS24P",  //   디지털컨텐츠     31
     "QGS25P",	//	소프트웨어    32
     "QGS26P",   //   컴퓨터서비스    33
     "QGS27P",	//   통신장비	      34
     "QGS28P",  //	정보기기      35
     "QGS29P",	//	반도체        36
     "QGS30P",        //IT부품        37
     "QGS87P",	//         
     "",
     "",	//		     39
     "",	//		     40
     "QGS34P",	//오락·문화         41
     "QGG03P",  //코스닥 우량기업부  42
     "QGG04P",  //코스닥 벤처기업부  43
     "QGG05P",  //코스닥 중견기업부  44
     "QGG06P"    //코스닥 기술성장기업부 45 

};


#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ
#define	SK_JONGMOK	0	// 종목검색
#define	SK_WORD		1	// 단어검색

#define ALCODE "alcode.dat"


BOOL	m_listsort;

/////////////////////////////////////////////////////////////////////////////
// CPage1 property page

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CPage1, CPropertyPage)

CPage1::CPage1(CWnd* parent) : CPropertyPage(CPage1::IDD)
{
	//{{AFX_DATA_INIT(CPage1)
	//}}AFX_DATA_INIT

	m_parent = parent;
	m_activegno = -1;

	m_autoScroll = false;
	m_dragImage = nullptr;

	m_activePage = false;
	m_bAddAll = true;

	m_appendGI = _T("");
	m_selectGroup = 0;

	m_bCol0Asc = TRUE;
	m_bCol1Asc = FALSE;
	m_nPrevCol = 0;

	m_nCol0Asc   = -1;
	m_nCol1Asc   = -1;
	m_nPrevList2 = -1;

	isClickServerDN = FALSE;
	m_bIsEdit = FALSE;
	m_isDuplicateCode = FALSE;


	m_isChangeSequence = false;
	m_bUpdate = FALSE;

	m_bFirstDuplicateNO = false;	//2012.02.13 KSJ
	m_nResult = 0;			//2012.04.02 KSJ

	m_cicode.RemoveAll();
	m_cccode.RemoveAll();
	m_sfcode.RemoveAll();

	m_gListsort = FALSE;
 
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
	DDX_Control(pDX, IDC_MOVETOP, m_btnMoveTop);
	DDX_Control(pDX, IDC_MOVEBOTTOM, m_btnMoveBtm);
	DDX_Control(pDX, IDC_DELALL, m_btn_DelAll);
	DDX_Control(pDX, IDC_SHAREIMPORT, m_btn_ShareImport);
	DDX_Control(pDX, IDC_SHAREEXPORT, m_btn_ShareExport);
	DDX_Control(pDX, IDC_NEWGROUP, m_btn_newGroup);
	DDX_Control(pDX, IDC_MOVEUP, m_btn_MoveUp);
	DDX_Control(pDX, IDC_MOVEDN, m_btn_MoveDn);
	DDX_Control(pDX, IDC_BLANK, m_btn_Blank);
	DDX_Control(pDX, IDC_ALLSAVE, m_btn_AllSave);
	DDX_Control(pDX, IDC_ADDALL, m_btn_AddAll);
	DDX_Control(pDX, IDC_ADD, m_btn_Add);
	DDX_Control(pDX, IDC_EDITGROUP, m_editGroup);
	DDX_Control(pDX, IDC_DNLOADR, m_btn_dnLoadR);
	DDX_Control(pDX, IDC_DNLOAD, m_btn_dnLoad);
	DDX_Control(pDX, IDC_DEL, m_btn_Del);
	DDX_Control(pDX, IDC_BTN_SORT22, m_btnSort22);
	DDX_Control(pDX, IDC_BTN_SORT21, m_btnSort21);
	DDX_Control(pDX, IDC_BTN_SORT20, m_btnSort20);
	DDX_Control(pDX, IDC_BTN_SORT19, m_btnSort19);
	DDX_Control(pDX, IDC_BTN_SORT18, m_btnSort18);
	DDX_Control(pDX, IDC_BTN_SORT17, m_btnSort17);
	DDX_Control(pDX, IDC_BTN_SORT16, m_btnSort16);
	DDX_Control(pDX, IDC_BTN_SORT15, m_btnSort15);
	DDX_Control(pDX, IDC_BTN_SORT14, m_btnSort14);
	DDX_Control(pDX, IDC_BTN_SORT13, m_btnSort13);
	DDX_Control(pDX, IDC_BTN_SORT12, m_btnSort12);
	DDX_Control(pDX, IDC_BTN_SORT11, m_btnSort11);
	DDX_Control(pDX, IDC_BTN_SORT10, m_btnSort10);
	DDX_Control(pDX, IDC_BTN_SORT09, m_btnSort09);
	DDX_Control(pDX, IDC_BTN_SORT08, m_btnSort08);
	DDX_Control(pDX, IDC_BTN_SORT07, m_btnSort07);
	DDX_Control(pDX, IDC_BTN_SORT06, m_btnSort06);
	DDX_Control(pDX, IDC_BTN_SORT05, m_btnSort05);
	DDX_Control(pDX, IDC_BTN_SORT04, m_btnSort04);
	DDX_Control(pDX, IDC_BTN_SORT03, m_btnSort03);
	DDX_Control(pDX, IDC_BTN_SORT02, m_btnSort02);
	DDX_Control(pDX, IDC_BTN_SORT01, m_btnSort01);
	DDX_Control(pDX, IDC_GNAME, m_gname);
	DDX_Control(pDX, IDC_SUM, m_sum);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_cbSearch);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_RADIO_ELWALL, m_Radio_ELWAll);
	DDX_Control(pDX, IDC_RADIO_ELWCALL, m_Radio_ELWCall);
	DDX_Control(pDX, IDC_RADIO_ELWPUT, m_Radio_ELWPut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_ADDALL, OnAddall)
	ON_BN_CLICKED(IDC_DELALL, OnDelall)
	ON_BN_CLICKED(IDC_BLANK, OnBlank)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDN, OnMovedn)
	ON_BN_CLICKED(IDC_NEWGROUP, OnNewgroup)
	ON_BN_CLICKED(IDC_UPLOAD, OnUpload)
//	ON_BN_CLICKED(IDC_DNLOAD, OnDnload)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, OnBegindragList2)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ALLSAVE, OnAllsave)
	ON_CBN_SELCHANGE(IDC_GNAME, OnSelchangeGname)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCH, OnSelchangeComboSearch)
	ON_BN_CLICKED(IDC_EDITGROUP, OnEditgroup)
//	ON_BN_CLICKED(IDC_CNVINTEREST, OnCnvinterest)
	ON_BN_CLICKED(IDC_DNLOADR, OnDnloadr)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, OnColumnclickList2)
	ON_BN_CLICKED(IDC_SHAREIMPORT, OnShareimport)
	ON_BN_CLICKED(IDC_SHAREEXPORT, OnShareexport)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MOVETOP, OnMovetop)
	ON_BN_CLICKED(IDC_MOVEBOTTOM, OnMovebottom)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_ELWALL, OnRadioClick)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
	ON_BN_CLICKED(IDC_RADIO_ELWCALL, OnRadioClick)
	ON_BN_CLICKED(IDC_RADIO_ELWPUT, OnRadioClick)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, OnGetdispinfoList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST2, OnGetdispinfoList2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
	ON_MESSAGE(WM_TOPAGE1, OnAppendGroup)
	ON_MESSAGE(WM_TOPAGE2, OnAppendParentGroup)
	ON_MESSAGE(WM_TOSELECTPAGE, OnSelectGroup)
	ON_MESSAGE(WM_TOSELECTGROUPNCODE, OnSelectGroupNCode)
	ON_COMMAND_RANGE(IDC_BTN_SORT01, IDC_BTN_SORT22, OnClickBtnSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers

BOOL CPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_search.SetPromptText(_T("종목명, 종목코드, 초성 입력"));
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");

	
	CString userD; 
	userD.Format("%s/%s/%s", m_root, USRDIR, m_name);

	CreateDirectory(userD, nullptr); 

	AxStd::async([this]() {  	
		loadingHJcode();
		loadingFJcode();
		loadingOJcode();
		loadingPJcode();
		loadingUPcode();
		loadingETFcode();
		loadingELWcode();
		loadingCFcode(true);
		loadingCFcode(false);
		loadingSFcode();
		loadingnameex();
	});


	initList(); 
	initTree(); 

	// 2025.08.27 단어우선으로 변경
	m_cbSearch.SetCurSel(1);	

	//사이즈 설정
	SetSize();
	loadConfig();

	selectKospi();	
	
	m_Radio_ELWAll.SetCheck(TRUE);
	DWORD	dwConversion{}, dwSentence{};
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		dwConversion |= IME_CMODE_HANGEUL; 
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	}
	m_gListsort = FALSE;

	CRect rec;
	((CWnd*)GetDlgItem(IDC_BTN_CLEAR))->GetWindowRect(rec);
	ScreenToClient(rec);

	CString strimagepath;
	strimagepath = m_root + "\\image\\GRAY_BAR_CLOSE.BMP";

	m_btnClear = new CBmpButton;

	m_btnClear->SetImgBitmap(strimagepath, 1);
	m_btnClear->SubclassWindow(GetDlgItem(IDC_BTN_CLEAR)->m_hWnd);
	rec.OffsetRect(-15, 0);
	m_btnClear->MoveWindow(rec);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPage1::loadingnameex()
{
	if (_mapExCode.size() > 0)
		return;

	CString sBuffer;
	CString path = AxStd::FORMAT(_T("%s\\tab\\%s"), m_root, ALCODE);

	try {
		CFile	file(path, CFile::modeRead | CFile::shareDenyRead);
		UINT	nBytesRead = file.GetLength();

		file.Read(sBuffer.GetBuffer(nBytesRead + 1), nBytesRead);
		file.Close();
		sBuffer.ReleaseBuffer();
	}
	catch (CFileException& e)
	{

	}

	if (!sBuffer.IsEmpty())
	{
		vector<CString> vData;
		AxStd::SplitSub(vData, sBuffer, _T("\n"));
		vData.erase(vData.begin());
		auto it = std::remove_if(vData.begin(), vData.end(), [](const CString& str) {
			return (str.Find(_T("=||")) != -1);
			});
		vData.erase(it, vData.end());

		for (auto name : vData)
		{
			const CString code = AxStd::Parser(name, _T("="));
			name.TrimRight(_T("|"));
			_mapExCode.emplace(std::move(code.GetString()), std::move(name.GetString()));
		}
		return;
	}
}

void CPage1::loadConfig()
{
	CString tempN; 
	tempN.Format("%s/%s/%s/interField.ini.tmp", m_root, USRDIR, m_name);

	CFileFind finder;
	if (!finder.FindFile(tempN))
	{
		tempN.Format("%s/%s/%s/interField.ini", m_root, USRDIR, m_name);
		if (!finder.FindFile(tempN))
		{
			return;
		}
	}
		
	int readL = 0;
	readL = GetPrivateProfileInt("INTERFIELD", "OVERLAP", 0, tempN);
	m_isDuplicateCode = readL;
}


void CPage1::selectKospi()
{
	HTREEITEM root_Item{}, child_Item{};
	CString root_name, child_name;
	
	root_Item = m_tree.GetRootItem();
	
	while(root_Item)
	{
		root_name = m_tree.GetItemText(root_Item);
		
		if(root_name == "주식종목")
		{
			child_Item = m_tree.GetChildItem(root_Item);
			
			while(child_Item)
			{
				child_name = m_tree.GetItemText(child_Item);
				
				if(child_name == "전종목")
				{
					m_tree.SelectItem(child_Item);
					xAllCodeToList();
					break;
				}	
				child_Item = m_tree.GetNextSiblingItem(child_Item);
				
			}
			break;
		}	
		root_Item = m_tree.GetNextSiblingItem(root_Item);
	}
}

void CPage1::selectElw()
{
	HTREEITEM root_Item{}, child_Item{};
	CString root_name, child_name;
	
	root_Item = m_tree.GetRootItem();
	
	while(root_Item)
	{
		root_name = m_tree.GetItemText(root_Item);
			
		if(root_name == "ELW")
		{
			child_Item = m_tree.GetChildItem(root_Item);
			
			m_Radio_ELWAll.ShowWindow(SW_SHOW);
			m_Radio_ELWCall.ShowWindow(SW_SHOW);
			m_Radio_ELWPut.ShowWindow(SW_SHOW);

			while(child_Item)
			{
				child_name = m_tree.GetItemText(child_Item);
				
				if(child_name == "전종목")
				{
					m_tree.SelectItem(child_Item);
					xELWCodeToList();
					break;
				}
				
				child_Item = m_tree.GetNextSiblingItem(child_Item);
				
			}
			break;
		}
		
		root_Item = m_tree.GetNextSiblingItem(root_Item);
	}
}

void CPage1::ExceptionMsg()
{
	const int nCnt = m_gname.GetCount();
	int nIdx = -1;

	for (int i = 0; i < nCnt; i++)
	{
		if (m_activegno == (int)m_gname.GetItemData(i))
		{
			nIdx = i;
			break;
		}
	}

	if (-1 != nIdx)
	{
		CString strName;
		m_gname.GetLBText(nIdx, strName);
		MessageBox(strName + " 종목 코드에 오류가 있습니다.\n\n서버에서 받기 또는 관심종목복구\n하시기 바랍니다.", "IBK투자증권");
	}
}
 
BOOL CPage1::OnSetActive() 
{
	m_activePage = true;

	if (m_activePage && m_activegno != -1)
	{ 
		loadConfig();
		selectComboBox();
	}	
	return CPropertyPage::OnSetActive();
}

BOOL CPage1::OnKillActive() 
{
	CString	gname;
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);

		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
	}
	return CPropertyPage::OnKillActive();
}

BOOL CPage1::endSendAction() //저 장 버튼 클릭시
{
	CString	gname;
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);
		
		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEndACTION, m_activegno-1), 0);
	}
	else if(GetGroupCount() == 0)	//2015.04.09 KSJ 선택된 그룹이 없고 그룹카운트가 0 이라면 전체삭제된것으로 파악하고 창을 닫는다.
	{
		CString string; 
		string = "OnPortfolio\tok";
		m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
		m_parent->SendMessage(WM_USER, closeDLL, 0);	
	}	
	return CPropertyPage::OnKillActive();
}

void CPage1::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}

void CPage1::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDel();	
	*pResult = 0;
}

void CPage1::OnAdd() 
{
	insertList1toList2();
}

void CPage1::OnDel() 
{
	deleteList2();
}

void CPage1::OnAddall() 
{
	if (m_activegno == -1)
	{
		GuideMsg("선택된 그룹이 없습니다. 새그룹을 등록하세요", false);
		return;
	}

	m_bIsEdit = TRUE;
	m_bFirstDuplicateNO = false;		//2012.04.02 KSJ 처음에 디폴트로..

	deselcMark(&m_list2);
	
	CString code, name;
	const int gubn = m_tree.GetItemData(m_tree.GetSelectedItem());
	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		code = m_list1.GetItemText(ii, 0);
		name = m_list1.GetItemText(ii, 1);

		if (existCodeInList2(code))
			continue;
		if (m_list2.GetItemCount() >= maxJONGMOK)
		{	MessageBox("최대 100 종목만 등록 가능 합니다.", "IBK투자증권", MB_OK);
			break;
		}
		AppendItemToList2(ii, gubn, code, name);
	}
	
	selcMark(&m_list2, m_inters.size() - 1);
	InitEdit();
}

void CPage1::OnDelall() 
{
	if (m_activegno == -1)
	{
		GuideMsg("선택된 그룹이 없습니다. 새그룹을 등록하세요", false);
		return;
	}
	
	m_bIsEdit = TRUE;

	ClearInterest();
	ClearList2item();
	m_list2.DeleteAllItems();
}

void CPage1::OnBlank() 
{
	if (m_activegno == -1)
	{
		GuideMsg("선택된 그룹이 없습니다. 새그룹을 등록하세요", false);
		return;
	}
	
	m_bIsEdit = TRUE;

	if (m_list2.GetItemCount() >= maxJONGMOK)
	{
	//	GuideMsg(idTOstring(IDS_GUIDE1));
 		MessageBox("최대 100 종목만 등록 가능 합니다.", "IBK투자증권", MB_OK);
		return;
	}

	int	nitem{};
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		nitem = m_list2.GetNextSelectedItem(pos);
		AppendItemToList2(nitem, 0, _T("      "), _T("      "), _T("0"), false);
	}
	else
	{
		nitem = m_list2.GetItemCount();
		AppendItemToList2(nitem, 0, _T("      "), _T("      "), _T("0"), true);
	}
	InitEdit();
}

void CPage1::OnMoveup() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;

	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem == 0) 
		return;

	auto pinter = m_inters.at(nitem);
	m_inters.erase(m_inters.begin() + nitem);

	if (pinter.get()->gubn == ROW_COMMENT)
	{
		CString stmp;
		stmp.Format("m%05d", nitem - 1);
		strncpy((char*)pinter.get()->code.GetBuffer(0), (char*)stmp.GetBuffer(0), stmp.GetLength());
	}

	m_inters.emplace(m_inters.begin() + nitem - 1, pinter);
	m_list2.Invalidate();

	deselcMark(&m_list2); 
	selcMark(&m_list2, nitem-1);
}

void CPage1::OnMovedn() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;

	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem >= m_list2.GetItemCount()-1) 
		return;
	
	
	auto pinter = m_inters.at(nitem);
	m_inters.erase(m_inters.begin() + nitem);

	if (pinter.get()->gubn == ROW_COMMENT)
	{
		CString stmp;
		stmp.Format("m%05d", nitem + 1);
		strncpy((char*)pinter.get()->code.GetBuffer(0), (char*)stmp.GetBuffer(0), stmp.GetLength());
	}

	m_inters.emplace(m_inters.begin() + nitem + 1, pinter);
	m_list2.Invalidate();
	
	deselcMark(&m_list2); 
	selcMark(&m_list2, nitem+1);
}

void CPage1::SelectTree(int selItem)
{
	CString itemname;
	CString cg_item, result_item;

	switch (selItem)
	{
	case xALLCODE:			// all_coode
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xAllCodeToList();
		m_bAddAll = true;
		break;
	case xKOSPI:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKospiToList();
		m_bAddAll = true;
		break;
	case xKOSDAQ:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKosdaqToList();
		m_bAddAll = true;
		break;
	case xKOSPIFUTURE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xFutureToList();
		m_bAddAll = true;
		break;
	case xSTOCKFUTURE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xSFutureToList();
		m_bAddAll = true;
		break;
	case xINTERESTRATEFUTURE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xCFutureToList(false);
		m_bAddAll = true;
		break;
	case xCURRENCYFUTURE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xCFutureToList(true);
		m_bAddAll = true;
		break;
	case xFUTURE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xFutureToList();
		m_bAddAll = true;
		break;
	case xCALLCODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xCallCodeToList();
		m_bAddAll = true;
		break;
	case xPUTCODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPutCodeToList();
		m_bAddAll = true;
		break;
	case xKOPSI200:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKospi200ToList();
		m_bAddAll = true;
		break;
	case xKOSDAQ50:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKosdaq50ToList();
		m_bAddAll = true;
		break;
	case xKRX100:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKRX100ToList();
		m_bAddAll = true;
		break;
	case xFREECODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xFreeCodeToList();
		m_bAddAll = true;
		break;
	case xETFCODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		//xETFCodeToList();
		ETFInitialList();
		m_bAddAll = true;
		break;
	case xELWCODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_SHOW);
		m_Radio_ELWCall.ShowWindow(SW_SHOW);
		m_Radio_ELWPut.ShowWindow(SW_SHOW);
		xELWCodeToList();
		m_bAddAll = true;

		break;
	case xELWBASE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_SHOW);
		m_Radio_ELWCall.ShowWindow(SW_SHOW);
		m_Radio_ELWPut.ShowWindow(SW_SHOW);
		xELWBaseToList();
		m_bAddAll = true;

		break;

	case xSINJUCODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xSinjuCodeToList();
		m_bAddAll = true;
		break;
	case xKOSPIJISU:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKospiJisuToList();
		m_bAddAll = true;
		break;
	case xKOSDAQJISU:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKosdaqJisuToList();
		m_bAddAll = true;
		break;
	case xRecommand:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xRecommandtoList();
		m_bAddAll = true;
		break;

	case xKBc: case xKBp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("10", selItem == xKBc ? '2' : '3');
		break;
	case xSECc: case xSECp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("11", selItem == xSECc ? '2' : '3');

		break;
	case xSKc: case xSKp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("12", selItem == xSKc ? '2' : '3');
		break;
	case xPOSCOc: case xPOSCOp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("13", selItem == xPOSCOc ? '2' : '3');
		break;
	case xKTc: case xKTp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("14", selItem == xKTc ? '2' : '3');
		break;
	case xKEPCOc: case xKEPCOp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("15", selItem == xKEPCOc ? '2' : '3');
		break;
	case xHYUNDAIc: case xHYUNDAIp:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xPjcodeToList("16", selItem == xHYUNDAIc ? '2' : '3');
		break;
	case xETFALLCODE:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		OnClickTree(nullptr, 0);  //0422 pNMHDR -> nullptr
		m_bAddAll = true;
		break;
	case xELWSelect:
		m_Radio_ELWAll.ShowWindow(SW_SHOW);
		m_Radio_ELWCall.ShowWindow(SW_SHOW);
		m_Radio_ELWPut.ShowWindow(SW_SHOW);
		break;
	case xELWPB:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_SHOW);
		m_Radio_ELWCall.ShowWindow(SW_SHOW);
		m_Radio_ELWPut.ShowWindow(SW_SHOW);
		OnClickELWTree(nullptr, 0);  //0422 pNMHDR -> nullptr
		break;
	case xKONEX:	//2013.06.25 코넥스 추가
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xKonexToList();
		m_bAddAll = true;
		break;
	case xETNCODE:	//2014.11.06 ETN 추가
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xETNCodeToList();
		m_bAddAll = true;
		break;
	case xSPACCODE:	//2015.11.03 spac 추가
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xSPACCodeToList();
		m_bAddAll = true;
		break;
	case xRITS: //리츠 0422
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xRITSCodeToList();
		m_bAddAll = true;
		break;
	case xALLNEWCIDE: //신종증권 0422
	case xSINTAKST:
	case xTUSACONST:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xNewCodeToList(selItem - xALLNEWCIDE);
		m_bAddAll = true;
		break;
	case xGOODSTOCK:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xGoodStockToList();
		m_bAddAll = true;
		break;
	case xNXT:
		m_search.SetWindowText("");
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		xNxtList();
		break;
	default:
		m_Radio_ELWAll.ShowWindow(SW_HIDE);
		m_Radio_ELWCall.ShowWindow(SW_HIDE);
		m_Radio_ELWPut.ShowWindow(SW_HIDE);
		if (selItem <= 0)
			break;
		switch (selItem / 100)
		{
		case xKOSPIUP:
			m_search.SetWindowText("");
			xKospiUpToList(selItem);
			m_bAddAll = true;
			break;
		case xKOSDAQUP:
			m_search.SetWindowText("");
			xKosdaqUpToList(selItem);
			m_bAddAll = true;
			break;
		case xINTEREST:
			//m_search.SetWindowText("");
			//xInterestToList(selItem);
			//m_bAddAll = true;
			break;
		}

		switch (selItem / 1000)
		{
		case xKOSPIex:
			m_search.SetWindowText("");
			xKospiExToList(selItem);
			m_bAddAll = true;
			break;
		case xKOSDAQex:
			m_search.SetWindowText("");
			xKosdaqExToList(selItem);
			m_bAddAll = true;
			break;
		case xTHEMACODE:
			m_search.SetWindowText("");
			xThemaCodeToList(selItem);
			m_bAddAll = true;
			break;
		case xGROUPCODE:
			m_search.SetWindowText("");
			xGroupCodeToList(selItem);
			m_bAddAll = true;
			break;
		case xELWISSUE:								//ELW발행회사별 종목검색 추가 081219
			m_search.SetWindowText("");

			cg_item.Format("%05d", selItem);
			result_item = cg_item.Mid(2, 3);
			ELWIssueDnload(atoi(result_item));				//selItem : 선택한 treeID.value				
			m_bAddAll = true;
			break;
		}


		switch (selItem / 10000)
		{
		case xELWASSET:								//ELW기초자산별 종목검색 추가 081219
			m_search.SetWindowText("");

			//코드값은 ELWISSUE(21) + 콜풋(1, 2) + 종목번호(001 - 999)

			cg_item.Format("%06d", selItem);
			result_item = cg_item.Mid(2, 1);

			if (result_item == "1")
			{
				itemname = _T("콜");
			}
			else if (result_item == "2")
			{
				itemname = _T("풋");
			}

			result_item = cg_item.Mid(3, 3);
			ELWAssetDnload(atoi(result_item), itemname);			//selItem : 선택한 treeID.value
			m_bAddAll = true;
			break;
		}

		switch (selItem / 100000)
		{
		case xELWASSET:								//ELW기초자산별 종목검색 추가 081219
			m_search.SetWindowText("");

			//코드값은 ELWISSUE(21) + 콜풋(1, 2) + 종목번호(001 - 999)

			cg_item.Format("%06d", selItem);
			result_item = cg_item.Mid(2, 1);

			if (result_item == "1")
			{
				itemname = _T("콜");
			}
			else if (result_item == "2")
			{
				itemname = _T("풋");
			}

			result_item = cg_item.Mid(3, 3);
			ELWAssetDnload(atoi(result_item), itemname);			//selItem : 선택한 treeID.value
			m_bAddAll = true;
			break;
		}

		break;
	}
	_vListCopy.clear();
	_vListCopy = _vList;
	if (m_btnClear)
		m_btnClear->Invalidate();
}

BOOL CPage1::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMTREEVIEW* nmtv{};
	NMHDR* pNMHDR{};
	
	int	selItem{};
	CString itemname;
	CString cg_item, result_item;

	switch (wParam)
	{
	case IDC_TREE:
		nmtv = (NMTREEVIEW *) lParam;
		if (nmtv->hdr.code != TVN_SELCHANGED)
			break;
		
		selItem = m_tree.GetItemData(nmtv->itemNew.hItem);
		SelectTree(selItem);
	case IDC_LIST1:
		{
			switch(((LPNMHDR) lParam)->code)
			{
			case LVN_COLUMNCLICK:
				#define pnm ((LPNMLISTVIEW)lParam)
				m_gListsort = !m_gListsort;
				listsort(pnm->iSubItem);
				#undef pnm
				
				break;
 			}
		}
		break;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

long CPage1::OnEditXMsg(WPARAM wParam, LPARAM lParam)
{
	CString	string;
	m_search.GetWindowText(string);
	CString temp;
	switch ((int) wParam)
	{
	case wpDown:
		m_list1.SetFocus();
		break;
		
	case wpBLANK:				
		searchAction(0);	// blank name search by kwon
		break;
	case wpENTER:		m_search.GetWindowText(string);
		if (!string.IsEmpty())
		{
			m_search.SetSel(0, -1);
			insertList1toList2();
		}
		break;
	case wpSEARCH1:
		//코드 분석하여 ELW인지 판단
		searchAction(0);	// number search by kwon
		break;
	case wpSEARCH2:		
		m_search.GetWindowText(string);
		//temp = string.GetAt(0);
		//if(atoi(temp) >= 5 && atoi(temp) < 9)
		if (string.IsEmpty())
		{
			searchAction(0);	// char search by kwon
		}
		else
		{
			searchAction(1);	// char search by kwon
		}	
		break;
	case wpBTNREDRAW://0422
	{
		if (m_btnClear)
			m_btnClear->Invalidate();
	}
	break;
	case wpBTNCLEAR://0422
	{
		ClearSearch();
	}
	break;
	}

	return 0;
}


void CPage1::ClearSearch()
{
	m_search.SetWindowText("");
	//_vList = _vListCopy;  
	
	int selItem = m_tree.GetItemData(m_tree.GetSelectedItem());
	SelectTree(selItem);
	//searchAction(0);
}

void CPage1::OnClickBtnSort(UINT nID)	// name btn click search by kwon
{
	CString	sName, sCode, sTitle;
	CWnd	*pBtn = GetDlgItem(nID);
	pBtn->GetWindowText(sTitle);

//m_slog.Format("[interest][IB202201][search1][%s]   sTitle =[%s] m_bUpdate=[%d]", __FUNCTION__, sTitle, m_bUpdate);
//OutputDebugString(m_slog);

	if (sTitle == _T("A"))
	{
		ClearSearch();
		std::vector<CITEMS> vTemp;
		vTemp = _vList;
		_vList.clear();

		for_each(vTemp.begin(), vTemp.end(), [&](const auto item) {
			sName = item.name;	
//			if (0x41 > sName.GetAt(0) || sName.GetAt(0) > 0x7A)
			if (IS_WITHIN(0x41, 0x7A, sName.GetAt(0)))
				_vList.emplace_back(item);
		});
				
		m_list1.SetItemCountEx(_vList.size());
		m_list1.Invalidate();

		if (m_list1.GetItemCount() > 0)
		{
			m_listsort = false;
			listsort(1);

			deselcMark(&m_list1);
			selcMark(&m_list1, 0);
		}
		m_bAddAll = false;
		m_search.SetWindowText("");
	}
	else if (sTitle == _T("지우기"))
	{
		m_search.SetWindowText("");
		_vList = _vListCopy;
		searchAction(0);
	}
	else if (sTitle == _T("←"))
	{
		m_search.GetWindowText(sName);
		sName.Trim();
		if (sName.GetLength() == 1)	// 문자
		{
			m_search.SetWindowText("");
			searchAction(0);
		}
		else if (!sName.IsEmpty())
		{
			const char cChar = sName.GetAt(sName.GetLength()-1);
			if (cChar & HANGULMSK)	sName = sName.Left(sName.GetLength() - 2);
			else			sName = sName.Left(sName.GetLength() - 1);

			m_search.SetWindowText(sName);
			if (sName.IsEmpty())
				searchAction(0);
			else
				searchAction(1);
		}
	}
	else
	{
		if(m_bUpdate)
		{
			m_bUpdate = FALSE;
			
			m_search.SetWindowText("");
		}

		m_search.GetWindowText(sName);
		m_search.SetWindowText(sName + sTitle);
		searchAction(1);
	}

	m_search.SetFocus();
	m_search.SetSel(0, -1);
	m_search.SetSel(-1, -1);

	if (m_btnClear)
		m_btnClear->Invalidate();
}

int	CPage1::getManageCount()
{
	CString sequence;
	int i = 0;
	for(i=0 ; i< 100 ; i++)
	{
		sequence = m_manageGroup[i][0];	
		if(!sequence.IsEmpty())
			continue;

		break;
	}

	return i;
}

bool CPage1::checkChangeSequence()
{
	const int count = getManageCount();	
	bool result = false;

	for(int i=0 ; i<count ; i++)
	{
		if(m_manageGroup[i][0] != m_manageGroup[i][2])	
		{
			result = true;
			break;
		}
	}

	return result;
}

CString	CPage1::getManageGroupdata(int row, int col)
{
	return m_manageGroup[row][col];
}

void CPage1::selectComboBox()
{
	m_list2.DeleteAllItems();
	ClearList2item();
	m_activegno = m_gname.GetItemData(m_gname.GetCurSel());

	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnGROUPCODE, 0), m_activegno);
}

void CPage1::OnSelchangeGname()
{
	CString	gname, tempS;
	
	//전에 선택한 그룹 일단 저장
	if (m_activegno != -1)
	{

		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);	

		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
	}

	selectComboBox();
}

void CPage1::OnSelchangeComboSearch()
{
	CString	string;
	m_search.GetWindowText(string);
	
	if (string.IsEmpty())
		searchAction(0);
	else
		searchAction(1);
	
	m_search.SetFocus();
	m_search.SetSel(0, -1);

	if (m_btnClear)
		m_btnClear->Invalidate();
}

void CPage1::OnAllsave() 
{
//XMSG(...);
	OnKillActive();

	//DWORD	fLen = 0; 
	CFileFind finder;
	CString	filePath, fileTemp;
	//int itemcount = 0;

	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	//if (finder.FindFile(fileTemp))
	//	CopyFile(fileTemp, filePath, FALSE);

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	CopyFile(fileTemp, filePath, FALSE);
	//}
}

void CPage1::OnNewgroup()
{
	CString	gname;
	m_bIsEdit = TRUE;

// XMSG(할필요없음)
//	if (m_activegno != -1)
//	{
//		gname = GetGroupName(m_activegno);
//		savingGroupFile(m_activegno, gname);

		//서버에 전송
//		CWnd* wnd = GetParent()->GetParent();
//		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
//	}

	const int cnt = m_gname.GetCount();
	gname.Format("그룹%03d", cnt+1);

	CNewGroup newGroup(this, 0, gname);

	if (newGroup.DoModal() == IDOK)
	{
		//Send 하여 시퀀스 정렬시킨다
		CWnd* wnd = GetParent()->GetParent();

		gname = newGroup.m_defName;
		if (savingGroupOrder(gname))
		{
			OnDelall();
			savingGroupFile(m_activegno, gname);

		}
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);	
	}
}

void CPage1::resettingGroup()
{
	m_activegno = -1; 
	initCombo();
	m_list2.DeleteAllItems();

	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnGROUPCODE, 0), m_activegno);	
}

void CPage1::OnEditgroup() 
{
	CString gname;
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);

		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
	}

	m_bIsEdit = TRUE;

	CEditGroup editGroup(this, m_root, m_name);
	if (editGroup.DoModal() == IDOK)
	{		
		//Send 하여 시퀀스 정렬시킨다
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upsequenceACTION, 0), 0);			
	}
}

void CPage1::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();	

	if (m_dragImage)
	{
		m_dragImage->DragLeave(GetDesktopWindow());
		m_dragImage->EndDrag();
		m_dragImage->DeleteImageList();
		m_dragImage.reset();
				
		OnEndDrag(point);
	}

	CPropertyPage::OnLButtonUp(nFlags, point);
}

void CPage1::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	list2RC, labelRC;

	m_list2.GetClientRect(&list2RC);
	m_list2.GetItemRect(0, labelRC, LVIR_LABEL);
	m_list2.ClientToScreen(&list2RC);
	const CRect	topRC	 = CRect(list2RC.left, list2RC.top - 45, list2RC.right, list2RC.top + 5);
	const CRect	bottomRC = CRect(list2RC.left, list2RC.bottom,   list2RC.right, list2RC.bottom + 70);

	if (m_dragImage)
	{
		CPoint	ptMove = point;
		ClientToScreen(&ptMove);

		m_dragImage->DragMove(ptMove);
		
		if (topRC.PtInRect(ptMove))
		{
			SetAutoScroll(&m_list2, -1);
		}
		else if (bottomRC.PtInRect(ptMove))
		{
			SetAutoScroll(&m_list2, +1);
		}
		else if (m_autoScroll)
		{
			CPoint	pt = ptMove;
			LVHITTESTINFO	lvhti;
			m_list2.ScreenToClient(&pt);
			lvhti.pt = pt;

			const int pItem = m_list2.HitTest(&lvhti);

			m_list2.Invalidate();
			
			m_autoScroll = false;
		}
	}
	
	CPropertyPage::OnMouseMove(nFlags, point);
}

void CPage1::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;

	if (m_list1.GetSelectedCount() == 0)
		return;

	m_dragData.list = 1;
	m_dragData.item = pNMListView->iItem;

	m_dragImage.reset();

	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list1);

	if (m_dragImage == nullptr)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_list1.ClientToScreen(&ptStart);

	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CPage1::OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (m_list2.GetSelectedCount() == 0)
		return;

	m_dragData.list = 2;
	m_dragData.item = pNMListView->iItem;

	m_dragImage.reset();

	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list2);

	if (m_dragImage == nullptr)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_list2.ClientToScreen(&ptStart);

	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CPage1::OnEndDrag(CPoint point)
{
	CRect	list1RC, list2RC;
	
	m_list1.GetClientRect(&list1RC); 
	m_list1.ClientToScreen(&list1RC);
	m_list2.GetClientRect(&list2RC); 
	m_list2.ClientToScreen(&list2RC);
	
	ClientToScreen(&point);
	m_bIsEdit = TRUE;	

	if (list1RC.PtInRect(point))
	{
		if (m_dragData.list == 2)
			deleteList2();
	}
	else if (list2RC.PtInRect(point))
	{
		LVHITTESTINFO lvhti;
		m_list2.ScreenToClient(&point);
		lvhti.pt = point;
		
		int nitem = m_list2.HitTest(&lvhti);
		if (nitem < 0)	nitem = m_list2.GetItemCount()-1;
		
		if (m_dragData.list == 2)
		{
			auto pinter = m_inters.at(m_dragData.item);
			m_inters.erase(m_inters.begin() + m_dragData.item);
			m_inters.emplace(m_inters.begin()+ nitem, pinter);

			m_list2.Invalidate();
	
			deselcMark(&m_list2); 
			selcMark(&m_list2, nitem);
		}
		else if (m_dragData.list == 1)
		{	
			insertList1toList2(nitem);
		}
	}
}


void CPage1::OnUpload() 
{
	OnKillActive();

	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(upACTION, 0), 0);
}

void CPage1::OnDnload()
{	
	//isClickServerDN = TRUE;
	//m_bIsEdit = TRUE;

	//CWnd* wnd = GetParent()->GetParent();
	//wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnACTION, 0), 0);

	//((CWnd*)(wnd->GetDlgItem(IDOK)))->EnableWindow(FALSE);	//2016.07.12 KSJ 복구 버튼 누르면 전부 받을때 까지 확인버튼을 false 해준다.
}


void CPage1::xRecommandtoList()
{
	isClickServerDN = TRUE;
	
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, RecommnadDnACTION, 0);
}

void CPage1::ELWIssueDnload(int selItem)
{
	
	isClickServerDN = TRUE;
	
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(ELWIssueDndnACTION, selItem), 0);
}

void CPage1::ELWAssetDnload(int selItem, CString item)
{
	
	isClickServerDN = TRUE;
	
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(ELWAssetDndnACTION, selItem), (LPARAM)((char*)item.operator LPCTSTR()));
}

void CPage1::OnDnloadr() 
{
XMSG();
	CString msg = "서버에 마지막으로 저장한 관심종목으로 복구합니다.\n\n그러나 현재 저장되어 있는 관심종목이 모두 지워질 수 있습니다.\n\n\n관심종목을 복구하시겠습니까?";
	CString caption = "관심종목복구";
	constexpr UINT type = MB_OKCANCEL|MB_ICONEXCLAMATION;
	if (MessageBox(msg, caption, type) == IDCANCEL)
		return;
	
	m_bIsEdit = TRUE;

//	tempSaveGroupIndex();
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnACTIONR, 0), 0);

	((CWnd*)(wnd->GetDlgItem(IDOK)))->EnableWindow(FALSE);	//2016.07.12 KSJ 복구 버튼 누르면 전부 받을때 까지 확인버튼을 false 해준다.
}


void CPage1::SetSize()
{	
	constexpr int x = 10;
	constexpr int y = 20;
	int btnx{}, btny{}, funtionx{}, funtiony{}, gx{}, gy{};
	m_tree.SetWindowPos(nullptr,x, y, 240, 305, SWP_NOSIZE);				//트리메뉴
//	m_btn_dnLoadR.SetWindowPos(nullptr, x-6, y+290, 240, 23, SWP_NOSIZE);			//관심종목복구
//	m_btn_dnLoad.SetWindowPos(nullptr, (x-6)+240, y+290, 240, 23, SWP_NOSIZE);		//서버에서받기

	m_cbSearch.SetWindowPos(nullptr, x+247, y-16, 46, 20, SWP_NOSIZE);			//2015.11.02 KSJ 종목검색콤보박스
	m_search.SetWindowPos(nullptr,  x+247, y-16, 222, 20, SWP_NOZORDER);			//종목검색
	m_list1.SetWindowPos(nullptr, x + 247, y + 23, 222, 240, SWP_NOSIZE);			//종목리스트 박스
	

	btnx = x+246;
	btny = y + 268;
	m_btnSort01.SetWindowPos(nullptr, btnx, btny, 26, 21, SWP_NOSIZE);				//"ㄱ"
	m_btnSort02.SetWindowPos(nullptr, btnx+28, btny, 26, 21, SWP_NOSIZE);
	m_btnSort03.SetWindowPos(nullptr, btnx+56, btny, 26, 21, SWP_NOSIZE);
	m_btnSort04.SetWindowPos(nullptr, btnx+84, btny, 26, 21, SWP_NOSIZE);
	m_btnSort05.SetWindowPos(nullptr, btnx+112, btny, 26, 21, SWP_NOSIZE);
	m_btnSort06.SetWindowPos(nullptr, btnx+140, btny, 26, 21, SWP_NOSIZE);
	m_btnSort07.SetWindowPos(nullptr, btnx+168, btny, 26, 21, SWP_NOSIZE);
	m_btnSort08.SetWindowPos(nullptr, btnx+196, btny, 26, 21, SWP_NOSIZE);
	m_btnSort09.SetWindowPos(nullptr, btnx, btny+23, 26, 21, SWP_NOSIZE);
	m_btnSort10.SetWindowPos(nullptr, btnx+28, btny+23, 26, 21, SWP_NOSIZE);
	m_btnSort11.SetWindowPos(nullptr, btnx+56, btny+23, 26, 21, SWP_NOSIZE);
	m_btnSort12.SetWindowPos(nullptr, btnx+84, btny+23, 26, 21, SWP_NOSIZE);
	m_btnSort13.SetWindowPos(nullptr, btnx+112, btny+23, 26, 21, SWP_NOSIZE);
	m_btnSort14.SetWindowPos(nullptr, btnx+140, btny+23, 26, 21, SWP_NOSIZE);
	m_btnSort15.SetWindowPos(nullptr, btnx + 168, btny + 23, 26, 21, SWP_NOSIZE); //"A"
	m_btnSort21.SetWindowPos(nullptr, btnx + 196, btny + 23, 26, 21, SWP_NOSIZE);
	m_btnSort21.SetWindowText("1");
	
	m_btnSort16.ShowWindow(SW_HIDE);
	m_btnSort17.ShowWindow(SW_HIDE);
	m_btnSort18.ShowWindow(SW_HIDE);
	m_btnSort19.ShowWindow(SW_HIDE);
	m_btnSort20.ShowWindow(SW_HIDE);
	m_btnSort22.ShowWindow(SW_HIDE);

	

	funtionx = x+490;
	funtiony = y+20;
	m_btn_Add.SetWindowPos(nullptr, funtionx, funtiony, 63, 26, SWP_NOSIZE);		//추가
	m_btn_Del.SetWindowPos(nullptr, funtionx, funtiony+28, 63, 26, SWP_NOSIZE);
	m_btn_AddAll.SetWindowPos(nullptr, funtionx, funtiony+70, 63, 26, SWP_NOSIZE);
	m_btn_DelAll.SetWindowPos(nullptr, funtionx, funtiony+98, 63, 26, SWP_NOSIZE);
	m_btn_Blank.SetWindowPos(nullptr, funtionx, funtiony+140, 63, 26, SWP_NOSIZE);
	m_btn_MoveUp.SetWindowPos(nullptr, funtionx, funtiony+168, 63, 26, SWP_NOSIZE);
	m_btn_MoveDn.SetWindowPos(nullptr, funtionx, funtiony+196, 63, 26, SWP_NOSIZE);
	m_btnMoveTop.SetWindowPos(nullptr, funtionx, funtiony+224, 63, 26, SWP_NOSIZE);
	m_btnMoveBtm.SetWindowPos(nullptr, funtionx, funtiony+252, 63, 26, SWP_NOSIZE);

	gx = x+572;
	gy = y+270;
	m_gname.SetWindowPos(nullptr, gx+60, y-18, 200, 20, SWP_NOSIZE);				//그룹명
//	m_list2.SetWindowPos(nullptr, gx, y+23, 219, 243, SWP_NOSIZE);
	m_list2.SetWindowPos(nullptr, gx, y+23, 219, 253, SWP_NOSIZE);
	m_btn_newGroup.SetWindowPos(nullptr, gx, gy, 109, 21, SWP_NOSIZE);				//새그룹
	m_editGroup.SetWindowPos(nullptr, gx+110, gy, 109, 21, SWP_NOSIZE);			//그룹편집
	m_btn_ShareImport.SetWindowPos(nullptr, gx, gy+22, 109, 21, SWP_NOSIZE);		//그룹불러오기
	m_btn_ShareExport.SetWindowPos(nullptr, gx+110, gy+22, 109, 21, SWP_NOSIZE);	//그룹내보내기
}

void CPage1::initList()
{
	AddColumn(&m_list1, "코드 ▲", 0, 46);
	AddColumn(&m_list1, "종목명", 1, 150);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	AddColumn(&m_list2, "코드", 0, 46);
	AddColumn(&m_list2, "종목명", 1, 150);
	m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

#ifdef DF_NEWTREE
void CPage1::initTree()
{
	HTREEITEM hRoot{};
	HTREEITEM hItem{}, hItem2{};
	codelistitem pbcode;
	CString code, name;
	codelistitem sjcode;

	hRoot = m_tree.InsertItem(_T("주식종목"), 0, 1);
	m_tree.SetItemData(m_tree.InsertItem(_T("전종목"), hRoot, TVI_LAST), xALLCODE);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스피"), hRoot, TVI_LAST), xKOSPI);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스닥"), hRoot, TVI_LAST), xKOSDAQ);
	m_tree.SetItemData(m_tree.InsertItem(_T("NXT"), hRoot, TVI_LAST), xNXT);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스피200"), hRoot, 0), xKOPSI200);
	m_tree.SetItemData(m_tree.InsertItem(_T("KRX100"), hRoot, 0), xKRX100);

	hItem = m_tree.InsertItem(_T("코스피업종"), hRoot, TVI_LAST);
	SetItemDataUJ(hItem, xKOSPIUP);
	hItem = m_tree.InsertItem(_T("코스닥업종"), hRoot, TVI_LAST);
	SetItemDataUJ(hItem, xKOSDAQUP);

	m_tree.SetItemData(m_tree.InsertItem(_T("코넥스"), hRoot, TVI_LAST), xKONEX);	//2013.06.25 KSJ 코넥스 추가
	m_tree.SetItemData(m_tree.InsertItem(_T("K-OTC"), hRoot, TVI_LAST), xFREECODE);

	hItem = m_tree.InsertItem(_T("ETF"), hRoot, TVI_LAST);
	m_tree.SetItemData(hItem, 0);
	ETFInitialTree(hItem);

	m_tree.SetItemData(m_tree.InsertItem(_T("ETN"), hRoot, TVI_LAST), xETNCODE);	//2013.06.25 KSJ 코넥스 추가

	m_tree.SetItemData(m_tree.InsertItem(_T("상장형수익증권"), hRoot, TVI_LAST), xGOODSTOCK);	//25.06.12 상장형수익증권 추가

	m_tree.SetItemData(m_tree.InsertItem(_T("리츠"), hRoot, TVI_LAST), xRITS);	//20240422 리츠
	
	m_tree.SetItemData(m_tree.InsertItem(_T("스팩"), hRoot, TVI_LAST), xSPACCODE);	//2015.11.03 KSJ spac 추가

	// 섹터종목
	hItem = m_tree.InsertItem(_T("섹터종목"), hRoot, TVI_LAST);
	SetItemDataX(hItem, xTHEMACODE);

	// 그룹사별
	hItem = m_tree.InsertItem(_T("그룹사별"), hRoot, TVI_LAST);
	SetItemDataX(hItem, xGROUPCODE);

	//ELW start
	hRoot = m_tree.InsertItem(_T("ELW"), 0, 0);
	m_tree.SetItemData(hRoot, 0);

	hItem = m_tree.InsertItem("전종목", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);

	hItem2 = m_tree.InsertItem("일반ELW", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	hItem2 = m_tree.InsertItem("조기종료ELW", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	hItem = m_tree.InsertItem("기초자산종목", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);

	hItem2 = m_tree.InsertItem("코스피200", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	hItem2 = m_tree.InsertItem("BASKET", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	const int nelwbase = m_elwbase.GetSize();
	for (int ii = 0; ii < nelwbase; ii++)
	{
		sjcode = m_elwbase.GetAt(ii);
		code = sjcode.code;
		name = sjcode.name;

		hItem2 = m_tree.InsertItem(name, 0, 1, hItem);
		m_tree.SetItemData(hItem2, xELWPB);
	}

	hItem = m_tree.InsertItem("발행사", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);

	const int nPBcode = m_arrayPBCode.GetSize();
	for (int ii = 0; ii < nPBcode; ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);

		code = CString(pbcode.code, 12);
		name = CString(pbcode.name, 20);

		name.TrimRight();

		hItem2 = m_tree.InsertItem(name, 0, 1, hItem);
		m_tree.SetItemData(hItem2, xELWPB);
	}
	//ELW end

	m_tree.SetItemData(m_tree.InsertItem(_T("신주인수권"), 0, 0), xSINJUCODE);

	hRoot = m_tree.InsertItem(_T("신종증권"), 0, 0);
	m_tree.SetItemData(m_tree.InsertItem(_T("전체"), hRoot, TVI_LAST), xALLNEWCIDE);
	m_tree.SetItemData(m_tree.InsertItem(_T("신탁수익증권"), hRoot, TVI_LAST), xSINTAKST);
	m_tree.SetItemData(m_tree.InsertItem(_T("투자계약증권"), hRoot, TVI_LAST), xTUSACONST);

	m_tree.SetItemData(m_tree.InsertItem(_T("코스피선물"), 0, 0), xKOSPIFUTURE);
	//m_tree.SetItemData(m_tree.InsertItem(_T("주식선물"), 0, 0), xSTOCKFUTURE);
	//m_tree.SetItemData(m_tree.InsertItem(_T("금리선물"), 0, 0), xINTERESTRATEFUTURE);
	//m_tree.SetItemData(m_tree.InsertItem(_T("통화선물"), 0, 0), xCURRENCYFUTURE);

	hRoot = m_tree.InsertItem(_T("코스피옵션"), 0, 0);
	m_tree.SetItemData(m_tree.InsertItem(_T("콜옵션"), hRoot, TVI_SORT), xCALLCODE);
	m_tree.SetItemData(m_tree.InsertItem(_T("풋옵션"), hRoot, TVI_SORT), xPUTCODE);

	m_tree.SetItemData(m_tree.InsertItem(_T("주식선물"), 0, 0), xSTOCKFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("금리선물"), 0, 0), xINTERESTRATEFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("통화선물"), 0, 0), xCURRENCYFUTURE);


	/*hRoot = m_tree.InsertItem(_T("KOSPI업종"), 0, 0);
	SetItemDataUJ(hRoot, xKOSPIUP);

	hRoot = m_tree.InsertItem(_T("KOSDAQ업종"), 0, 0);
	SetItemDataUJ(hRoot, xKOSDAQUP);*/

	/*m_tree.SetItemData(m_tree.InsertItem(_T("KOSPI200"), 0, 0), xKOPSI200);
	m_tree.SetItemData(m_tree.InsertItem(_T("KRX100"), 0, 0), xKRX100);*/
//	m_tree.SetItemData(m_tree.InsertItem(_T("신주인수권"), 0, 0), xSINJUCODE);

	hRoot = m_tree.InsertItem(_T("업종지수"), 0, 0);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스피"), hRoot, TVI_SORT), xKOSPIJISU);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스닥"), hRoot, TVI_SORT), xKOSDAQJISU);

	//// 섹터종목
	//hRoot = m_tree.InsertItem(_T("섹터종목"), 0, 0);
	//SetItemDataX(hRoot, xTHEMACODE);

	//// 그룹사별
	//hRoot = m_tree.InsertItem(_T("그룹사별"), 0, 0);
	//SetItemDataX(hRoot, xGROUPCODE);
}
#else
void CPage1::initTree()
{
	HTREEITEM hRoot{};
	HTREEITEM hItem{}, hItem2{};
	codelistitem pbcode;
	CString code, name;
	codelistitem sjcode;

	hRoot = m_tree.InsertItem(_T("주식종목"), 0, 1);
	m_tree.SetItemData(m_tree.InsertItem(_T("전종목"), hRoot, TVI_LAST), xALLCODE);
	m_tree.SetItemData(m_tree.InsertItem(_T("거래소"), hRoot, TVI_LAST), xKOSPI);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스닥"), hRoot, TVI_LAST), xKOSDAQ);
	m_tree.SetItemData(m_tree.InsertItem(_T("코넥스"), hRoot, TVI_LAST), xKONEX);	//2013.06.25 KSJ 코넥스 추가
	m_tree.SetItemData(m_tree.InsertItem(_T("K-OTC"), hRoot, TVI_LAST), xFREECODE);
	
	hItem = m_tree.InsertItem(_T("ETF"), hRoot, TVI_LAST);
	m_tree.SetItemData(hItem, 0);
	ETFInitialTree(hItem);
	
	m_tree.SetItemData(m_tree.InsertItem(_T("ETN"), hRoot, TVI_LAST), xETNCODE);	//2013.06.25 KSJ 코넥스 추가
	m_tree.SetItemData(m_tree.InsertItem(_T("스팩"), hRoot, TVI_LAST), xSPACCODE);	//2015.11.03 KSJ spac 추가

	hRoot = m_tree.InsertItem(_T("ELW"), 0, 0);
	m_tree.SetItemData(hRoot, 0);
// 	m_tree.SetItemData(m_tree.InsertItem(_T("전종목"), hRoot, TVI_LAST), xELWCODE);
// 	m_tree.SetItemData(m_tree.InsertItem(_T("기초자산종목"), hRoot, TVI_LAST), xELWBASE);
// 	m_tree.SetItemData(m_tree.InsertItem(_T("발행회사별"), hRoot, TVI_LAST), xELWISSUE);

	hItem = m_tree.InsertItem("전종목", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);
	
	hItem2 = m_tree.InsertItem("일반ELW", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	hItem2 = m_tree.InsertItem("조기종료ELW", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	hItem = m_tree.InsertItem("기초자산종목", 0, 1, hRoot);
	m_tree.SetItemData(hItem, 0);
	
	hItem2 = m_tree.InsertItem("KOSPI200", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);
	
	hItem2 = m_tree.InsertItem("BASKET", 0, 1, hItem);
	m_tree.SetItemData(hItem2, xELWPB);

	const int nelwbase = m_elwbase.GetSize();
	for(int ii = 0; ii < nelwbase; ii++)
	{
		sjcode = m_elwbase.GetAt(ii);
		code = sjcode.code;
		name = sjcode.name;

		hItem2 = m_tree.InsertItem(name, 0, 1, hItem);
		m_tree.SetItemData(hItem2, xELWPB);
	}


	hItem = m_tree.InsertItem("발행사",0,1, hRoot);
	m_tree.SetItemData(hItem,0);
	
	const int nPBcode = m_arrayPBCode.GetSize();
	for(int ii = 0; ii < nPBcode; ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		
		code = CString(pbcode.code, 12);
		name = CString(pbcode.name, 20);

		name.TrimRight();
		
		hItem2 = m_tree.InsertItem(name, 0, 1, hItem);		
		m_tree.SetItemData(hItem2, xELWPB);		
	}

/*	m_tree.SetItemData(m_tree.InsertItem(_T("기초자산별"), hRoot, TVI_LAST), xELWASSET);

	HTREEITEM hChild = m_tree.GetChildItem(hRoot);
	CString child_name;
	while(hChild)
	{
		child_name = m_tree.GetItemText(hChild);
		
		if(child_name == "발행회사별")
		{
			SetItemDataX(hChild, xELWISSUE);
		}
		else if(child_name == "기초자산별")
		{
			SetItemDataX(hChild, xELWASSET);
		}
		
		hChild = m_tree.GetNextSiblingItem(hChild);
	}
*/
	m_tree.SetItemData(m_tree.InsertItem(_T("KOSPI선물"), 0, 0), xKOSPIFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("주식선물"), 0, 0), xSTOCKFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("금리선물"), 0, 0), xINTERESTRATEFUTURE);
	m_tree.SetItemData(m_tree.InsertItem(_T("통화선물"), 0, 0), xCURRENCYFUTURE);

	hRoot = m_tree.InsertItem(_T("KOSPI옵션"), 0, 0);
	m_tree.SetItemData(m_tree.InsertItem(_T("콜옵션"), hRoot, TVI_SORT), xCALLCODE);
	m_tree.SetItemData(m_tree.InsertItem(_T("풋옵션"), hRoot, TVI_SORT), xPUTCODE);

	hRoot = m_tree.InsertItem(_T("KOSPI업종"), 0, 0);
	SetItemDataUJ(hRoot, xKOSPIUP);
	
	hRoot = m_tree.InsertItem(_T("KOSDAQ업종"), 0, 0);
	SetItemDataUJ(hRoot, xKOSDAQUP);

	m_tree.SetItemData(m_tree.InsertItem(_T("KOSPI200"), 0, 0), xKOPSI200);
	m_tree.SetItemData(m_tree.InsertItem(_T("KRX100"), 0, 0), xKRX100);
	m_tree.SetItemData(m_tree.InsertItem(_T("신주인수권"), 0, 0), xSINJUCODE);

	hRoot = m_tree.InsertItem(_T("업종지수"), 0, 0);
	m_tree.SetItemData(m_tree.InsertItem(_T("거래소"),   hRoot, TVI_SORT), xKOSPIJISU);
	m_tree.SetItemData(m_tree.InsertItem(_T("코스닥"), hRoot, TVI_SORT), xKOSDAQJISU);

	// 섹터종목
	hRoot = m_tree.InsertItem(_T("섹터종목"), 0, 0);
	SetItemDataX(hRoot, xTHEMACODE);
	
	// 그룹사별
	hRoot = m_tree.InsertItem(_T("그룹사별"), 0, 0);
	SetItemDataX(hRoot, xGROUPCODE);

	// IBK추천종목
	//m_tree.SetItemData(m_tree.InsertItem(TVIF_TEXT, _T("IBK추천종목"), 0, 0, 0, 0, 0, nullptr, nullptr), xRecommand);
}
#endif

void CPage1::ETFInitialTree(HTREEITEM hRoot)
{	
	_efopitem* efopitem{};
	_efoptema* efoptema{};
	_efopfore* efopfore{};
	
	CString data, line;
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	HTREEITEM hItem2{}, hItem3{};
	hItem2 = m_tree.InsertItem("전체종목",0,1, hRoot);
	m_tree.SetItemData(hItem2, xETFALLCODE);
	
	hItem2 = m_tree.InsertItem("운용사별",0,1, hRoot);
	m_tree.SetItemData(hItem2,xETFALLCODE);
	
	for (const auto& item : m_ETFitem)
	{
		type = CString(item->type, sizeof(item->type));
		opcd = CString(item->opcd, sizeof(item->opcd));
		hnam = CString(item->hnam, sizeof(item->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		hItem3 = m_tree.InsertItem(hnam, 0, 1, hItem2);
		m_tree.SetItemData(hItem3, xETFALLCODE);
	}

	//const int nETFitem = m_ETFitem.size();
	//for(int ii = 0; ii < nETFitem; ii++)
	//{
	//	efopitem = m_ETFitem.GetAt(ii);
	//	
	//	type = CString(efopitem->type, sizeof(efopitem->type));
	//	opcd = CString(efopitem->opcd, sizeof(efopitem->opcd));
	//	hnam = CString(efopitem->hnam, sizeof(efopitem->hnam));
	//	hnam.TrimLeft(); hnam.TrimRight();
	//	
	//	
	//	hItem3 = m_tree.InsertItem(hnam, 0, 1, hItem2);		
	//	m_tree.SetItemData(hItem3, xETFALLCODE);
	//	
	//}
	
	hItem2 = m_tree.InsertItem("기초자산별",0,1, hRoot);	//2015.11.04 KSJ 테마별 --> 기초자산별로 변경
	m_tree.SetItemData(hItem2, xETFALLCODE);
	
	for (const auto& item : m_ETFtema)
	{
		type = CString(item->type, sizeof(item->type));
		tmcd = CString(item->tmcd, sizeof(item->tmcd));
		hnam = CString(item->tmnm, sizeof(item->tmnm));
		hnam.TrimLeft(); hnam.TrimRight();
		hItem3 = m_tree.InsertItem(hnam, 0, 1, hItem2);
		m_tree.SetItemData(hItem3, xETFALLCODE);
	}

//	const int nETFtema = m_ETFtema.GetSize();
	//for(int ii = 0; ii < nETFtema; ii++)
	//{
	//	efoptema = m_ETFtema.GetAt(ii);
	//	
	//	type = CString(efoptema->type, sizeof(efoptema->type));
	//	tmcd = CString(efoptema->tmcd, sizeof(efoptema->tmcd));
	//	hnam = CString(efoptema->tmnm, sizeof(efoptema->tmnm));
	//	hnam.TrimLeft(); hnam.TrimRight();
	//	
	//		
	//	hItem3 = m_tree.InsertItem(hnam, 0, 1, hItem2);		
	//	m_tree.SetItemData(hItem3, xETFALLCODE);
	//	
	//}
	
	hItem2 = m_tree.InsertItem("국내/외",0,1,hRoot);
	m_tree.SetItemData(hItem2, xETFALLCODE);
	
	for (const auto& item : m_ETFfore)
	{
		type = CString(item->type, sizeof(item->type));
		frcd = CString(item->frcd, sizeof(item->frcd));
		frnm = CString(item->frnm, sizeof(item->frnm));
		frnm.TrimLeft(); frnm.TrimRight();

		hItem3 = m_tree.InsertItem(frnm, 0, 1, hItem2);
		m_tree.SetItemData(hItem3, xETFALLCODE);
	}

	//const int nETFfore = m_ETFfore.GetSize();
	//for(int ii = 0; ii < nETFfore; ii++)	
	//{
	//	efopfore = m_ETFfore.GetAt(ii);
	//	
	//	type = CString(efopfore->type, sizeof(efopfore->type));
	//	frcd = CString(efopfore->frcd, sizeof(efopfore->frcd));
	//	frnm = CString(efopfore->frnm, sizeof(efopfore->frnm));
	//	frnm.TrimLeft(); frnm.TrimRight();
	//	
	//	hItem3 = m_tree.InsertItem(frnm, 0, 1, hItem2);		
	//	m_tree.SetItemData(hItem3, xETFALLCODE);
	//}	
}



void CPage1::initCombo(bool isEditOk)
{//********************************************
	m_gname.ResetContent();
	for (int ii = 0; ii < MAXGROUP; ii++)
	{
		const auto& key = m_manageGroup[ii][0];
		if (key.IsEmpty())
			break;
		const int idx = m_gname.AddString(m_manageGroup[ii][3]);
		m_gname.SetItemData(idx, atoi(key));
	}

	int idx = m_gname.SetCurSel(0);
	if (m_activegno == -1 && m_gname.GetCount() > 0)
	{
		if (m_selectGroup > 0 )
			idx = m_gname.SetCurSel(m_selectGroup);
		m_activegno = m_gname.GetItemData(idx);
	}
	
	const int nowCursel = m_gname.GetCurSel();
	if(nowCursel == -1)
	{
		if(m_activegno > 0)
		{
			if (m_selectGroup < 0 )
				idx = m_gname.SetCurSel(0);
			else
				idx = m_gname.SetCurSel(m_selectGroup);//BUFFET 0
		}
		else
		{
			idx = m_gname.SetCurSel(0);
		}
		m_activegno = m_gname.GetItemData(idx);
	}
}

void CPage1::SetItemDataUJ(HTREEITEM hItem, int gubn)
{
	int jgub{};
	switch (gubn)
	{
	case xKOSPIUP:
		jgub = upKOSPI;
		break;
	case xKOSDAQUP:
		jgub = upKOSDAQ;
		break;
	}

	CString name, string;
	struct	upcode	upcode;
	for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
	{
		upcode = m_upcode.GetAt(ii);
		if (upcode.jgub == jgub)
		{
			name = CString(upcode.hnam, UNameLen);
			name.TrimLeft(); name.TrimRight();
		
			string.Format("%d%02d", gubn, (int)upcode.ucod);
			m_tree.SetItemData(m_tree.InsertItem(name, hItem), atoi(string));
		}
	}

	if (jgub == upKOSPI)
	{
		for (int ii = 0; ii < cnt_exKospi; ii++)
		{
			string.Format("%d%s", xKOSPIex, exKospi[ii].code);
			CString sname = exKospi[ii].name;
			sname.Replace("KOSPI", "코스피");
			m_tree.SetItemData(m_tree.InsertItem(sname, hItem), atoi(string));
		}
	}
/*	
	else if (jgub == upKOSDAQ)
	{
		for (ii = 0; ii < cnt_exKosdaq; ii++)
		{
			string.Format("%d%s", xKOSDAQex, exKosdaq[ii].code);
			m_tree.SetItemData(m_tree.InsertItem(exKosdaq[ii].name, hItem, nullptr), atoi(string));
		}
	}
*/	//2016.05.16 KSJ KOSTAR 지수 삭제
}

void CPage1::SetItemDataX(HTREEITEM hItem, int gubn)
{
	switch (gubn)
	{
	case xTHEMACODE:
		{
		int readL = 0; char readB[256]{};
			CString	filePath, string, stringx, data, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");

			for (int ii = 0; ii < 500; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL); 
				stringx = parseX(string, ";"); 
				stringx.TrimRight();
				data.Format("%02d%03d", xTHEMACODE, atoi(string));
				m_tree.SetItemData(m_tree.InsertItem(stringx, hItem), atoi(data));
			}
		}
		break;

	case xGROUPCODE:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, stringx, data, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "gropcd.ini");

			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(_T("FOLDER"), skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL);	
				stringx = parseX(string, ";"); 
				stringx.TrimRight();
				data.Format("%02d%03d", xGROUPCODE, atoi(string));
				m_tree.SetItemData(m_tree.InsertItem(stringx, hItem), atoi(data));
			}
		}
		break;
	case xELWISSUE:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey, data;
			
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWLP.CBS");
			
			for(int ii=0 ; ii<200 ; ii++)
			{
				skey.Format("%d", ii);
				readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
				if(readL <= 0) break;

				string = CString(readB, readL);
				string.TrimRight();
				string_code = string.Left(12);
				parseX(string, string_code); 
				
				if(ii>0)		//전체는 표시안함
				{
					data.Format("%02d%03d", xELWISSUE, ii);
					m_tree.SetItemData(m_tree.InsertItem(string, hItem, TVI_LAST), atoi(data));
				}
			}
		}
		break;

	case xELWASSET:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey, data;
			HTREEITEM hMid = TVI_LAST;
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWUD.CBS");

			m_tree.SetItemData(m_tree.InsertItem(_T("콜"), hItem, TVI_LAST), xELWASSET);
			m_tree.SetItemData(m_tree.InsertItem(_T("풋"), hItem, TVI_LAST), xELWASSET);

			HTREEITEM hChild = m_tree.GetChildItem(hItem);
			CString child_name;
			
			while(hChild)
			{
				child_name = m_tree.GetItemText(hChild);
				if(child_name == "콜")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						parseX(string, string_code); 
						
						if(ii>1) //전체는 표시안함
						{
							data.Format("%02d%d%03d", xELWASSET, 1, ii);
							m_tree.SetItemData(m_tree.InsertItem(string, hChild, TVI_LAST), atoi(data));
						}
					}
				}
				else if(child_name == "풋")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						parseX(string, string_code); 
						
						if(ii>1) //전체는 표시안함
						{
							data.Format("%02d%d%03d", xELWASSET, 2, ii);
							m_tree.SetItemData(m_tree.InsertItem(string, hChild, TVI_LAST), atoi(data));
						}
					}
				}

				hChild = m_tree.GetNextSiblingItem(hChild);
			}

			
		}
		break;

	case xINTEREST:
		{
//		XMSG(...);
			//int readL = 0; 
			//char readB[512];  //관심그룹 많을때 256->512
			//CString	filePath, string, gno, gname, data;

			//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

			//readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
			//if (readL <= 0)	return;

			//string = CString(readB, readL);

			//while (!string.IsEmpty())
			//{
			//	gno = parseX(string, ";");
			//	if (gno.IsEmpty() || gno.GetLength() != 2)
			//		break;

			//	readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), filePath);
			//	gname = CString(readB, readL); 
			//	data.Format("%02d%s", xINTEREST, gno);

			//	if (ExistFile(atoi(gno)))
			//		m_tree.SetItemData(m_tree.InsertItem(gname, hItem), atoi(data));
			//}
		}
		break;
	}
}

void CPage1::ClearListitem()
{
	m_gListsort = FALSE;
	_vList.clear();
//	for (int ii = 0; ii < m_listitem.GetCount(); ii++)
//		delete m_listitem.GetAt(ii);
//	m_listitem.RemoveAll();
//	m_arList.RemoveAll();  //speed
}

void CPage1::ClearList2item()
{
	//for (int ii = 0; ii < m_list2item.GetSize(); ii++)
	//	delete m_list2item.GetAt(ii);
	//m_list2item.RemoveAll();
}

void CPage1::ClearInterest()
{
	m_inters.clear();
	//for (int ii = 0; ii < m_inters.GetSize(); ii++)
	//	delete m_inters.GetAt(ii);
	//m_inters.RemoveAll();
}

void CPage1::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
{
	CRect	rc; CSize size;

	listctrl->SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	if (scroll)
	{
		listctrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - listctrl->GetTopIndex());
		listctrl->Scroll(size);
	}
}

void CPage1::deselcMark(CListCtrl* listctrl)
{
	int nItem{};
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = listctrl->GetNextSelectedItem(pos);
			listctrl->SetItemState(nItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

void CPage1::insertList1toList2(int sitem)
{
	if (m_activegno == -1)
	{
		GuideMsg("선택된 그룹이 없습니다. 새그룹을 등록하세요", false);
		return;
	}
		
	m_bIsEdit = TRUE;
	m_bFirstDuplicateNO = false;		//2012.04.02 KSJ 처음에 디폴트로..

	deselcMark(&m_list2);

	int	nitem{};
	CString	code, name;

	POSITION pos = m_list1.GetFirstSelectedItemPosition();

	while (pos)
	{
		nitem = m_list1.GetNextSelectedItem(pos);

		code = m_list1.GetItemText(nitem, 0);
		name = m_list1.GetItemText(nitem, 1);

		
		if (existCodeInList2(code))
		{	
//			GuideMsg("동일한 종목이 관심그룹 내에 존재합니다.");
			continue;
		}

		if (m_list2.GetItemCount() >= maxJONGMOK)
		{
//			GuideMsg(idTOstring(IDS_GUIDE1));
			MessageBox("최대 100 종목만 등록 가능 합니다.", "IBK투자증권", MB_OK);
			break;
		}

		int gubn = m_tree.GetItemData(m_tree.GetSelectedItem());

 		CString temp;
		if(gubn < 100000 && gubn > 10000)
		{
			temp.Format("%05d", gubn);
			temp = temp.Mid(0,2);
			gubn = atoi(temp);
		}
		else if(gubn >= 100000)
		{
			
			temp.Format("%06d", gubn);
			temp = temp.Mid(0,2);
			gubn = atoi(temp);
		}
		
		if (sitem < 0)
		{
			const int position = m_list2.GetItemCount();
			AppendItemToList2(position, gubn, code, name);
			selcMark(&m_list2, position);
		}
		else	// drag
		{
			AppendItemToList2(sitem++, gubn, code, name, _T("0"), false);
		}
	}
	InitEdit();
}

void CPage1::deleteList2()
{
	if (m_activegno == -1)
	{
		GuideMsg("선택된 그룹이 없습니다. 새그룹을 등록하세요", false);
		return;
	}
	
	m_bIsEdit = TRUE;

	int nItem = -1;
	POSITION pos{};
	int count = m_list2.GetItemCount();
	for (int ii = 0; ii < count; ii++)
	{
		pos = m_list2.GetFirstSelectedItemPosition();
		if (pos != nullptr)
		{
			nItem = m_list2.GetNextSelectedItem(pos);
			DeleteItemToList2(nItem);
		}
	}

	count = m_list2.GetItemCount();
	if (count > nItem)
		selcMark(&m_list2, nItem, false);
	else
		selcMark(&m_list2, nItem -1, false);
}

void CPage1::searchAction(int column)
{
	CString	string; 
	m_search.GetWindowText(string);
	m_list1.UpdateData(false);

//m_slog.Format("[interest][IB202201][search2][%s]   sTitle =[%s] column=[%d]", __FUNCTION__, string, column);
//OutputDebugString(m_slog);

	_listitem* lim{};
	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{
			_vList = _vListCopy;

			m_list1.SetItemCountEx(_vList.size());
			m_list1.Invalidate();
		
			m_bAddAll = true;
			//xAllCodeToList();
		}

		if (m_list1.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_listsort = false;
				//listsort(column);

				deselcMark(&m_list1);

				idx = findMatchIndex(string, column);
				if (idx >= 0) selcMark(&m_list1, idx);
			}
			else
			{
				deselcMark(&m_list1);
				selcMark(&m_list1, 0);

			}
		}
	}
	else			// name search
	{
		m_bAddAll = SearchData(m_cbSearch.GetCurSel(), string, m_bAddAll);
		const int icnt = m_list1.GetItemCount();

//		m_slog.Format("[interest][IB202201][search2][%s]   sTitle =[%s] icnt=[%d] m_bAddAll=[%d]", __FUNCTION__, string, icnt, m_bAddAll);
//		OutputDebugString(m_slog);

		if (m_list1.GetItemCount() > 0)
		{
			m_listsort = false;
			std::vector<CITEMS> vTrash;			
			auto first = std::remove_if(_vList.begin(), _vList.end(), [&vTrash](const auto item){
				const CString& name = item.name;
				if (name.GetAt(0) == 88 || name.GetAt(0) == 64 || name.GetAt(0) == 47 || name.GetAt(0) == 36 || name.GetAt(0) == 37 || name.GetAt(0) == 38 || name.GetAt(0) == 33)
				{
					vTrash.push_back(item);
					return true;
				}
				else 
			 		return false;
			});
			_vList.erase(first, _vList.end());

			m_list1.SetItemCountEx(_vList.size());
			//listsort(column);

			_vList.insert(_vList.end(), vTrash.begin(), vTrash.end());
			m_list1.SetItemCountEx(_vList.size());

 			deselcMark(&m_list1);
 			selcMark(&m_list1, 0);
			vTrash.clear();
		}
	}
	m_list1.UpdateData(true);
}



int CPage1::findMatchIndex(CString text)
{
	const int nameCOL = 1;

	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		string = m_list1.GetItemText(ii, nameCOL);
		string = string.Mid(1);

		if (strncmp(text, string, textL) <= 0)
			return ii;
	}
	return -1;
}

int CPage1::findMatchIndex(CString text, int column)
{
	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		string = m_list1.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

void CPage1::updateList2sum()
{
	CString string;
	string.Format("%d", m_inters.size());
	m_sum.SetWindowText(string);

// 	if(m_inters.GetSize() >= 100)
// 		MessageBox("최대 100 종목만 등록 가능 합니다.", "IBK투자증권", MB_OK);
}


std::shared_ptr<CImageList> CPage1::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl)
{
	const int count = listctrl->GetSelectedCount();
	if (count < 1) return nullptr;

	
	CRect itemRC, totalRC; totalRC.SetRectEmpty();
	
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (!pos) return nullptr;

	int item = listctrl->GetNextSelectedItem(pos);
	listctrl->GetItemRect(item, itemRC, LVIR_BOUNDS);
	totalRC = itemRC;
	totalRC.bottom = totalRC.top + totalRC.Height() * count;

	CDC memDC; CClientDC dcClient(this);
	if (!memDC.CreateCompatibleDC(&dcClient))
		return nullptr;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, totalRC.Width(), totalRC.Height()))
		return nullptr;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, totalRC.Width(), totalRC.Height(), RGB(255, 255, 255));

	auto pCompleteImageList = std::make_shared<CImageList>();
	pCompleteImageList->Create(totalRC.Width(), totalRC.Height(), ILC_COLOR|ILC_MASK, 0, 1);

	int index = 0;
	pos = listctrl->GetFirstSelectedItemPosition();
	while (pos)
	{
		item = listctrl->GetNextSelectedItem(pos);
		listctrl->GetItemRect(item, itemRC, LVIR_LABEL);

		CPoint	pt;
		CImageList* pimglstSingle = listctrl->CreateDragImage(item, &pt);
		
		if (pimglstSingle)
		{
			pimglstSingle->Draw(&memDC, 0, CPoint(0, itemRC.Height() * index++), ILD_MASK);
			delete pimglstSingle; pimglstSingle = nullptr;
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = itemRC.left;
		lpPoint->y = itemRC.top;
	}

	return pCompleteImageList;
}

void CPage1::SetAutoScroll(CListCtrl* listctrl, int direct)
{
	CRect	rc;
	CSize	size;

	listctrl->GetItemRect(0, rc, LVIR_LABEL);

	const int pos = listctrl->GetTopIndex();

	size.cx = 0;
	size.cy = rc.Height() * ((pos + direct) - pos);

	listctrl->Scroll(size);

	if (direct < 0)
		listctrl->Invalidate();

	m_autoScroll = true;
}

///////////////////////////////////////////////////////////////////////////////
// 
BOOL CPage1::AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = ctrl->GetStringWidth(lvc.pszText) + width;

	return ctrl->InsertColumn(item, &lvc);
}

int CPage1::AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem < 0) // code
		return ctrl->InsertItem(LVIF_TEXT, nItem, string, 0, 0, 0, 0);
	else		  // name
		return ctrl->SetItemText(nItem, nSubItem, string);
}

bool CPage1::ExistFile(int gno, bool temp /*=false*/)
{
	CString	filePath;

	if (temp)
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	else
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	return true;
}

void CPage1::loadingHJcode()
{
	if (m_hjcode.GetSize() > 0) 
		return;
	DllInvoker dll("AxisCode");
	if (!dll.IsLoaded())
		return;

	auto map = (std::map<CString, struct hjcodex*>*)dll.Function<char* WINAPI(int)>("getArray")(1);
	if (map->size() <= 0)
		return;
	for_each(map->begin(), map->end(), [&](const auto item) {
		const auto& codeinfo = item.second;	
		struct _shjcode hjcode;

		hjcode.name = CString(codeinfo->hnam, HNameLen).Trim().MakeUpper();	
		hjcode.symb = CString(codeinfo->symb, HSymbolLen).Trim();
		hjcode.ecng = codeinfo->ecng;
		hjcode.size = codeinfo->size;
		hjcode.ucdm = CString(codeinfo->acdm, 6).Trim();
		hjcode.ucds = CString(codeinfo->acds, 6).Trim();
		hjcode.acdl = CString(codeinfo->acdl, 6).Trim();
		hjcode.jjug = codeinfo->jjug;
		hjcode.kpgb = codeinfo->kpgb;
		hjcode.ucmd = codeinfo->ucmd;
		hjcode.kosd = (char)codeinfo->kosd;
		hjcode.ssgb = codeinfo->ssgb;
		hjcode.wsgb = codeinfo->wsgb;
		hjcode.jsiz = codeinfo->jsiz;
		hjcode.itgb = codeinfo->itgb;
		hjcode.star = codeinfo->star;
		hjcode.unio = codeinfo->unio;
		hjcode.prmr = codeinfo->kqpg;
		hjcode.nxtf = codeinfo->nxtf;

		switch (codeinfo->ssgb)
		{
		case jmSINJU:
		case jmSINJS:
		case jmHYFND:
			hjcode.code = CString(codeinfo->code, HCodeLen);
			break;
		case jmTJCONJS:
		case jmSINSUJS:
			hjcode.code = CString(codeinfo->code, 13);
			break;
		default:
			hjcode.code = CString(codeinfo->code, HCodeLen).Mid(1);
			break;
		}
		m_hjcode.Add(hjcode);	
	});
}

void CPage1::loadingFJcode()
{
	if (m_fjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;

	filePath.Format("%s/%s/%s", m_root, TABDIR, FJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox(idTOstring(IDS_EM_FJCODE));
		return;
	}

	CString sBuffer;
	const int filelen = gsl::narrow_cast<int>(fileH.GetLength());
	const int readlen = fileH.Read(sBuffer.GetBuffer(filelen + 1), filelen + 1);
	sBuffer.ReleaseBuffer();
	fileH.Close();

	if (filelen == readlen)
	{
		const gsl::span<struct  fjcode> spanArr((struct  fjcode*)sBuffer.GetBuffer(), readlen / sizeof(struct fjcode));
		for_each(spanArr.begin(), spanArr.end(), [&](auto& item) {
			struct codelistitem jcode;
		
			jcode.code = CString(item.cod2, FCodeLen).Trim();
			jcode.name = CString(item.hnam, FNameLen).Trim();

			m_fjcode.Add(jcode);
		});
	}
}

void CPage1::loadingOJcode()
{
	if (m_ojcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;

	struct  ojcodh  ojcodH;
	struct  ojcode  ojcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, OJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox(idTOstring(IDS_EM_OPCODE));
		return;
	}

	fileH.Read(&ojcodH, sizeof(struct ojcodh));

	const int countC = gsl::narrow_cast<int>((fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&ojcode, sizeof(struct ojcode));
		m_ojcode.Add(ojcode);
	}
	fileH.Close();
}

void CPage1::loadingPJcode()
{
	if (m_pjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  pjcode  pjcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, PJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox(idTOstring(IDS_EM_PJCODE));
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct pjcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&pjcode, sizeof(struct pjcode));
		m_pjcode.Add(pjcode);
	}
	fileH.Close();
}

void CPage1::loadingUPcode()
{
	if (m_upcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  upcode  upcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, UPCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox(idTOstring(IDS_EM_UPCODE));
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct upcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&upcode, sizeof(struct upcode));
		m_upcode.Add(upcode);
	}

	fileH.Close();
}

void CPage1::loadingELWcode()
{
	if (m_elwbase.GetSize() > 0)
		return;

	int	codeN{};
	CFile	file;
	codelistitem	sjcode;
	struct  elwcode  ELWCode;
	_elwdata elwdata;
	codelistitem pbcode;
	//CString code, name, ktype, pcode, pname, mjmt, bkoba;
	int idx{};
	CString bkoba;
	ITEMS item;

	CString path = m_root + "\\tab\\elwcod.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))	return;

	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));

//	SORTSTR *pBaseSortArr = new SORTSTR[codeN];
	
	bool	bFound{};
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));
		
		//elwdata = new _elwdata();

		//item = new ITEMS;
		elwdata.code = CString(ELWCode.codx, sizeof(ELWCode.codx));
		elwdata.code.TrimRight();	
		elwdata.name.Format("%s", CString(ELWCode.hnam, ELWHNameLen).Mid(1));
		elwdata.name.TrimRight(); elwdata.name.TrimLeft();
		elwdata.ktype = CString(ELWCode.krye, 2);
		elwdata.pcode = CString(ELWCode.bgce, 12);
		elwdata.pname = CString(ELWCode.hbge, 20);
		elwdata.mjmt  = CString(ELWCode.mjmt, 8);
		elwdata.bkoba = (ELWCode.koba[0]==2) ? TRUE : FALSE;

		for (int jj = 0; jj < 5; jj ++)
		{
			elwdata.bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			elwdata.bcode[jj].TrimRight();
		}

		if(elwdata.bkoba == 0)
			bkoba = "0";
		else
			bkoba = "1";

// 		item.code = elwdata.code;
// 		item.name = elwdata.name;
// 		item.bkoba = elwdata.bkoba;
// 		item.ktype = elwdata.ktype;

		m_ELWdata.Add(elwdata);
		//m_ELWcompany.SetAt(item.code, &item);

	
		idx = FindPBCode(elwdata.pcode);
	
		if (idx < 0)
		{
			pbcode.code = elwdata.pcode;
			pbcode.name = elwdata.pname;
			m_arrayPBCode.Add(pbcode);
		}

		
		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
			bcode.TrimRight();
			if (bcode == "") break;

			bcode = bcode.Mid(1);
			
			bFound = false;
			for (int kk = 0; kk < m_elwbase.GetSize(); kk++)
			{
				if (bcode.CompareNoCase(m_elwbase.GetAt(kk).code) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				CString	bname = GetCodeName(bcode);
				//TRACE("base name : %s\n", bname);
				if (!bname.IsEmpty())	
					;
				else if (bcode.CompareNoCase("OSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("ASKET") == 0)
					break;
				else
					continue;


				sjcode.code = bcode;
				sjcode.name = bname;

			
				sjcode.name.TrimRight();

				
				
//				m_ELWbase.SetAt(sjcode.code, sjcode);
				m_elwbase.Add(sjcode);
			}
		}

// 		if(elwdata != nullptr)
// 		{
// 			elwdata = nullptr;
// 			delete elwdata;
// 		}
	}

	//QSortCArray( m_elwbase, compareSortName); 
	
	file.Close();
}

void CPage1::loadingCFcode(bool bCurrency)//2012.10.04 KSJ 금리(6x), 통화(7x)
{	
	if(bCurrency)
	{
		if (m_cccode.GetSize() > 0) return;	
		
		CFile	fileH;
		CString	filePath, tjgb;
		struct  ccode  ccode;
		
		filePath.Format("%s/%s/%s", m_root, TABDIR, CFCODE);
		if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
		{
			AfxMessageBox("금리/통화선물코드 파일을 읽을 수 없습니다.");
			return;
		}
		
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct ccode));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&ccode, sizeof(struct ccode));
			
			tjgb = CString(ccode.tjgb, 2);
			
			if(tjgb.GetAt(0) == '7')
				m_cccode.Add(ccode);
		}
		
		fileH.Close();
	}
	else
	{
		if (m_cicode.GetSize() > 0) return;	
		
		CFile	fileH;
		CString	filePath, tjgb;
		struct  ccode  ccode;
		
		filePath.Format("%s/%s/%s", m_root, TABDIR, CFCODE);
		if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
		{
			AfxMessageBox("금리/통화선물코드 파일을 읽을 수 없습니다.");
			return;
		}
		
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct ccode));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&ccode, sizeof(struct ccode));
			
			tjgb = CString(ccode.tjgb, 2);
			
			if(tjgb.GetAt(0) == '6' || (ccode.codx[1] == '7') && (ccode.codx[2] == '0'))
				m_cicode.Add(ccode);
		}
		
		fileH.Close();
	}
}

void CPage1::loadingSFcode()	//2012.10.04 KSJ 주식선물
{
	if (m_sfcode.GetSize() > 0) return;	
	
	CFile	fileH;
	CString	filePath, strCode;
	struct  sfcode  sfcode;
	
	filePath.Format("%s/%s/%s", m_root, TABDIR, SFCODEDAT);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox("주식선물코드 파일을 읽을 수 없습니다.");
		return;
	}
	
	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct sfcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&sfcode, sizeof(struct sfcode));

// 		strCode = CString(sfcode.codx, 8);
// 		
// 		if(strCode.GetAt(0) == '1')
			m_sfcode.Add(sfcode);
	}
	
	fileH.Close();
}

int CPage1::FindPBCode(CString code)
{
	const bool bRegistered = false;
	codelistitem pbcode;
	for (int ii = 0; ii < m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		if (pbcode.code == code)
		{
			return ii;
		}
	}
	return -1;
}


void CPage1::savingInterest(int gno)
{	
//	m_slog.Format("\r\n[interest][IB202201][%s]  gno = [%d] checkbook =[%d]", __FUNCTION__, gno , CheckBookFileProcess());
//	OutputDebugString(m_slog);

	if (!CheckBookFileProcess())
		return;

	struct  _bookmarkinfo* bInfo{};
	BOOL bExistBook = FALSE;

	CString	fileBook = AxStd::FORMAT("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, gno);	
	::DeleteFile(fileBook);
	CFile	fileB(fileBook, CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone);
	if (fileB.m_hFile != CFile::hFileNull) 
	{
		for (size_t ii = 0; ii < m_inters.size(); ii++)// 
		{		
			auto& pinter = m_inters.at(ii);
			
			auto bInfo = std::make_unique<_bookmarkinfo>();
			ZeroMemory(bInfo.get(), sz_bookmark);
			
//			m_slog.Format("\r\n[interest][IB202201] c=[%s] n=[%s] p = [%s] n=[%s]", pinter->code, pinter->name, pinter->xprc, pinter->xnum);
//			OutputDebugString(m_slog);

			//북마크 기능 추가
			if(pinter->code.IsEmpty())
			{
				FillMemory(bInfo->code, sizeof(bInfo->code),' ');
				bInfo->bookmark[0] = '0';
			}
			else
			{
				CopyMemory(bInfo->code, pinter->code, 12);
				if (pinter->code[0] == 'm')
				{
					bInfo->gubn[0] = ROW_COMMENT;
					CopyMemory(bInfo->name, pinter->name, min(pinter->name.GetLength(), sizeof(bInfo->name)));
					bExistBook = TRUE;
				}
				bInfo->bookmark[0] = pinter->bookmark == '1' ? '1' : '0';	//2015.04.03 KSJ 1이아니면 0으로 해준다.
				if (pinter->bookmark == '1')
				{
					bInfo->gubn[0] = ROW_BOOKMARK;
					bExistBook = TRUE;
				}
			}
			
			fileB.Write(bInfo.get(), sz_bookmark);		
		}
	}		
 	fileB.Close();
	if (bExistBook == FALSE)
		::DeleteFile(fileBook);
	else
	{
//		m_slog.Format("\r\n[interest][IB202201][%s]  북마크파일[%s] 저장 ", __FUNCTION__, fileBook);
//		OutputDebugString(m_slog);
	}

}

void CPage1::savingGroupFile(int gno, CString gname)
{
	savingInterest(gno);
}

bool CPage1::savingGroupOrder(CString gname)
{
	if (!gname.IsEmpty())
	{	
		const int idx = m_gname.AddString(gname);
		m_activegno = idx + 1;
		m_gname.SetItemData(idx, m_activegno);		
		m_gname.SetCurSel(idx); 
		return true;
	}
	return false;
}

int CPage1::xAllCodeToList()
{
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed

	_vListCopy = _vList;
	return 0;
}

int CPage1::xKospiToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}


		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

//2013.06.25 코넥스 추가
int CPage1::xKonexToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmKONEX:
				break;

		default:
			continue;
		}
		
		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}
//2013.06.25 코넥스 추가 end

int CPage1::xGoodStockToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmGOODSTOCK:
				break;

		default:
			continue;
		}
		
		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}
//2014.11.06 ETN 추가
int CPage1::xETNCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmETN:
			break;
			
		default:
			continue;
		}
		
		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed	
	return 0;
}
//2014.11.06 ETN 추가 end

//2015.11.03 spac 추가
int CPage1::xSPACCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();
	
	_shjcode   hjcode; 
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		
		switch (hjcode.ssgb)
		{
		case jmSPAC:
			break;
			
		default:
			continue;
		}
		
		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed	
	return 0;
}
//2015.11.03 spac 추가 end
// 0422 신규종목추가
int CPage1::xNewCodeToList(int igubn)
{//igubn = 0 전체 , 1 신탁수익증권, 2 투자계약증권
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode;

	bool badd{};
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);
		badd = false;
		if (igubn == 0)
		{
			if (hjcode.ssgb == jmTJCONJS || hjcode.ssgb == jmSINSUJS)
			{
				badd = true;
			}
		}
		else if (igubn == 1)
		{
			if (hjcode.ssgb == jmTJCONJS)
			{
				badd = true;
			}
		}
		else if (igubn == 2)
		{
			if (hjcode.ssgb == jmSINSUJS)
			{
				badd = true;
			}
		}

		if (badd)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = CString(hjcode.code, 13);
			item.name = CString(hjcode.name, HNameLen);
		}
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed	
	return 0;
}

//0422 리츠 추가 
int CPage1::xRITSCodeToList()
{
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode;

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.ssgb)
		{
		case jmREITS:
			break;

		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed	
	return 0;
}


int CPage1::xKosdaqToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSDAQ:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				continue;
			default:
				break;
			}
			break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xKospi200ToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
			if (hjcode.kpgb >= 1)
				break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);
	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xKosdaq50ToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSDAQ:
			if (hjcode.kpgb >= 1)
				break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xKRX100ToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.unio != 1)
			continue;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xFreeCodeToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jm3RD)
			continue;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xELWCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmELW)
				continue;
			break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xELWBaseToList()
{
//	loadingELWcode();

	ClearListitem(); 
	m_list1.DeleteAllItems();

	codelistitem   sjcode;
	CString code, name;

	for (int ii = 0; ii < m_elwbase.GetSize(); ii++)
	{
		sjcode = m_elwbase.GetAt(ii);

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = sjcode.code;
		item.name = sjcode.name;

	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed

	m_listsort = false;
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.name.Compare(item2.name) > 0);
		return item2.name.Compare(item1.name) > 0;
	});
	return 0;
}

int CPage1::xETFCodeToList()
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			if (hjcode.ssgb != jmETF)
				continue;
			break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
	return 0;
}

int CPage1::xSinjuCodeToList()
{
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode; 

	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		switch (hjcode.kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode.ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			//case jmHYFND:	//2015.03.30 KSJ 장내수익증권종목 신주인수권 리스트에서 삭제
				break;
			default:
				continue;
			}
			break;
		default:
			continue;
		}

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = CString(hjcode.code, HCodeLen);
		item.name = CString(hjcode.name, HNameLen);
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

void CPage1::parsingRecommandDN(char* datB, int datL)
{
XMSG("...");
	m_list1.DeleteAllItems();

	struct  mod
	{
		char    nrec[4];					/*	갯수			*/
		char    acod[1024][12];					/*  종목코드 리스트 */
	}* mod;

	mod = (struct mod*)datB;
	

	CString strCnt = CString(mod->nrec, 4);
	const int nCnt = atoi(strCnt);
	
	CString code, name;

	for(int i=0 ; i<nCnt ; i++)
	{
		code = CString(mod->acod[i], 12);
		code.TrimLeft();
		code.TrimRight();


		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);
			
			name = GetCodeName(code);
			name.TrimLeft();
			name.TrimRight();


			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
}

void CPage1::parsingELWIssueDN(char* datB, int datL)
{
XMSG("...");
	m_list1.DeleteAllItems();

	struct  grid 
	{
		char    seqn[4];
		char    cod2[7];            /* RTS COD      */
		char    code[6];            /* 종목코드     */
		char    hnam[20];           /* 종목명       */
		char    curr[8];            /* 현재가       */
		char    diff[7];            /* 대비         */
		char    rate[6];            /* 등락율       */
		char    mdga[8];            /* 매도호가     */
		char    msga[8];            /* 매수호가     */
		char    cvol[9];            /* 체결량       */
		char    gvol[9];            /* 거래량       */
		char    gamt[12];           /* 거래대금     */
		char    dvol[8];            /* 순매도잔량   */
		char    svol[8];            /* 순매수잔량   */
		char    irga[7];            /* 이론가       */
		char    grat[7];            /* 괴리율       */
		char    prty[7];            /* 패리티       */
		char    gerr[7];            /* 기어링비율   */
		char    brer[7];            /* 손익분기율   */
		char    cfpv[7];            /* 자본지지점   */
		char    gcjs[18];           /* 기초자산     */
		char    bhsa[20];           /* 발행사       */
		char    jjis[6];            /* 잔존일수     */
		char    hsgg[9];            /* 환산가       */
		char    hsga[9];            /* 행사가격     */
		char    jhre[9];            /* 전환비율     */
		char    delt[9];            /* 델타         */
		char    eger[9];            /* e*기어링     */
	};
	
	typedef struct  {		/* GRID form output format  */
		char    aflg[1];		/* add action to top or bottom  */
		/* '0':replace '1':top, '2':bottom */
		char    sdir[1];		/* sort direction       */
		/* '1':asc, '2':desc    */
		char    scol[16];		/* sorting column symbol    */
		char    xpos[1];		/* continuous status        */
		/* 0x40: default(normal)    */
		/* 0x01: PgUp, ScrUp enable */
		/* 0x02: PgDn, ScrDn enable */
		/* 0x04: no local sorting   */
		char    page[4];		/* # of page            */
		char    save[80];		/* grid inout save field    */
	} GRID_O;			/* grid header */
	
	struct mod
	{
		char flag[1];	
		GRID_O  grid_o;
		char nrec[4];				//gr7id row 개수
		struct grid grid[1];
	}* mod;
	
	mod = (struct mod*)datB;
	
	CString oflag, nrec, code, name;
	GRID_O m_grido{};
	int ncnt = 0;
	const int nsize = sizeof(struct grid);
	int position = 0;

	oflag.Format("%.*s", sizeof(mod->flag), mod->flag);
	CopyMemory(&m_grido, &mod->grid_o, sizeof(GRID_O));
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);
	
	for(int ii=0 ; ii<ncnt ; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		name.Format("%.*s", sizeof(mod->grid[ii].hnam), mod->grid[ii].hnam);
		name.TrimRight();
		
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);

			AppendItemToList1(position, code, " " + name);
			position += 1;


			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = " " + name;
		}
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
}

void CPage1::parsingELWAssetDN(char* datB, int datL)
{
	m_list1.DeleteAllItems();

	struct  grid 
	{
		char    seqn[4];
		char    cod2[7];            /* RTS COD      */
		char    code[6];            /* 종목코드     */
		char    hnam[20];           /* 종목명       */
		char    curr[8];            /* 현재가       */
		char    diff[7];            /* 대비         */
		char    rate[6];            /* 등락율       */
		char    mdga[8];            /* 매도호가     */
		char    msga[8];            /* 매수호가     */
		char    cvol[9];            /* 체결량       */
		char    gvol[9];            /* 거래량       */
		char    gamt[12];           /* 거래대금     */
		char    dvol[8];            /* 순매도잔량   */
		char    svol[8];            /* 순매수잔량   */
		char    irga[7];            /* 이론가       */
		char    grat[7];            /* 괴리율       */
		char    prty[7];            /* 패리티       */
		char    gerr[7];            /* 기어링비율   */
		char    brer[7];            /* 손익분기율   */
		char    cfpv[7];            /* 자본지지점   */
		char    gcjs[18];           /* 기초자산     */
		char    bhsa[20];           /* 발행사       */
		char    jjis[6];            /* 잔존일수     */
		char    hsgg[9];            /* 환산가       */
		char    hsga[9];            /* 행사가격     */
		char    jhre[9];            /* 전환비율     */
		char    delt[9];            /* 델타         */
		char    eger[9];            /* e*기어링     */
	};
	
	typedef struct  {		/* GRID form output format  */
		char    aflg[1];		/* add action to top or bottom  */
		/* '0':replace '1':top, '2':bottom */
		char    sdir[1];		/* sort direction       */
		/* '1':asc, '2':desc    */
		char    scol[16];		/* sorting column symbol    */
		char    xpos[1];		/* continuous status        */
		/* 0x40: default(normal)    */
		/* 0x01: PgUp, ScrUp enable */
		/* 0x02: PgDn, ScrDn enable */
		/* 0x04: no local sorting   */
		char    page[4];		/* # of page            */
		char    save[80];		/* grid inout save field    */
	} GRID_O;			/* grid header */
	
	struct mod
	{
		char flag[1];	
		GRID_O  grid_o;
		char nrec[4];				//gr7id row 개수
		struct grid grid[1];
	}* mod;
	
	mod = (struct mod*)datB;
	
	CString oflag, nrec, code, name;
	GRID_O m_grido{};
	int ncnt = 0;
	const int nsize = sizeof(struct grid);
	
	oflag.Format("%.*s", sizeof(mod->flag), mod->flag);
	CopyMemory(&m_grido, &mod->grid_o, sizeof(GRID_O));
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);
	
	for(int ii=0 ; ii<ncnt ; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		name.Format("%.*s", sizeof(mod->grid[ii].hnam), mod->grid[ii].hnam);
		name.TrimRight();
		
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);
			
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = " " + name;
		}
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
}

int CPage1::xFutureToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	codelistitem   scode;
	CString	code, name;

	for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
	{
		scode = m_fjcode.GetAt(ii);

		code = CString(scode.code, FCodeLen);
		name = CString(scode.name, FNameLen);

		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xCallCodeToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct ojcode	ojcode;
//	int position = 0;
	CString	code, name;

	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < STANDARDNUM; jj++)
		{
			if (ojcode.call[jj].yorn != '1')
				continue;

			code = CString(ojcode.call[jj].cod2, OCodeLen);
			name = CString(ojcode.call[jj].hnam, ONameLen);
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}

	m_listsort = false;
	
	//speed  
	m_list1.SetItemCountEx(_vList.size());
	
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.code.Compare(item2.code) > 0);
		return item1.code.Compare(item2.code) < 0;
	});
	
	m_list1.Invalidate();
	//@speed
//	ListView_SortItems(m_list1, ListViewCompareFunc, 1);
	return 0;
}

int CPage1::xPutCodeToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct ojcode	ojcode;
	CString	code, name;

	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < STANDARDNUM; jj++)
		{
			if (ojcode.put[jj].yorn != '1')
				continue;

			code = CString(ojcode.put[jj].cod2, OCodeLen);
			name = CString(ojcode.put[jj].hnam, ONameLen);

			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}
	m_listsort = false;
	//speed
	m_list1.SetItemCountEx(_vList.size());
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.code.Compare(item2.code) > 0);
		return item2.code.Compare(item1.code) > 0;
	});
	m_list1.Invalidate();
	//@speed
//	m_listsort = false;
//	ListView_SortItems(m_list1, ListViewCompareFunc, 1);

	return 0;
}

int CPage1::xKospiJisuToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct upcode	upcode;
	CString	code, name;


	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub != upKOSPI)
			continue;

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}

	for (int ii = 0; ii < cnt_exKospi; ii++)
	{
		code = exKospi[ii].code;
		name = exKospi[ii].name;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}

	m_listsort = false;
	//speed
	m_list1.SetItemCountEx(_vList.size());
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.code.Compare(item2.code) > 0);
		return item2.code.Compare(item1.code) > 0;
	});
	m_list1.Invalidate();
	//@speed

	return 0;
}

int CPage1::xKosdaqJisuToList()
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct upcode	upcode;
	CString	code, name;

	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (upcode.jgub != upKOSDAQ)
			continue;

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		name = CString(upcode.hnam, UNameLen);

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}

/*
	for (ii = 0; ii < cnt_exKosdaq; ii++)
	{
		code = exKosdaq[ii].code;
		name = exKosdaq[ii].name;

		AppendItemToList1(position, code, name);
		position += 1;
	}
*/
	//speed
	m_listsort = false;
	m_list1.SetItemCountEx(_vList.size());
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.code.Compare(item2.code) > 0);
		return item2.code.Compare(item1.code) > 0;
		});
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xPjcodeToList(CString code, char kind)
{
	ClearListitem(); m_list1.DeleteAllItems();

	struct	pjcode pjcode;
	CString	codx, name, tjgb;
	for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
	{
		pjcode = m_pjcode.GetAt(ii);

		codx = CString(pjcode.codx, PCodeLen);
		tjgb = CString(pjcode.tjgb, 2);

		if (codx.GetAt(0) == kind && tjgb == code)
		{
			name = CString(pjcode.hnam, sizeof(pjcode.hnam));

			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = codx;
			item.name = name;
		}
	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed

	m_bAddAll = true;
	return 0;
}

int CPage1::xKospiUpToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 100;

//	int position = 0;
	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSPI)
			continue;

		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}

		append = false;

		if (kind == 1)
		{
			append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind < 27)
		{
			const CString xxx = xxKOSPI[kind];		
			if (hjcode.ucds.CompareNoCase(xxx) == 0 || hjcode.ucdm.CompareNoCase(xxx) == 0)
				append = true;

		}
		else if (kind == 27)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind)
				append = true;
		}
		else if (kind == 41)
		{
			if ((int)hjcode.jsiz >= kind + 38 && (int)hjcode.jsiz <= kind + 41)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.wsgb == 1)
				append = true;
		}

		if (!append) continue;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = hjcode.code;
		item.name = hjcode.name;
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xKosdaqUpToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 100;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;

		switch (hjcode.ssgb)
		{
		case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
			continue;
		}

		append = false;

		if (kind == 1)
		{
			append = true;
		}
		else if (kind == 2)
		{
			if ((int)hjcode.jjug == kind)
				append = true;
		}
		else if (kind < 5)
		{
			if ((int)hjcode.size == kind)
				append = true;
		}
		else if (kind == 5)
		{
			if ((int)hjcode.size == 4 && (int)hjcode.jjug == 2)
				append = true;
		}
		else if (kind < 38 || kind == 41)
		{
			const CString xxx = xxKOSDAQ[kind];
			if (hjcode.ucds.CompareNoCase(xxx) == 0 || hjcode.ucdm.CompareNoCase(xxx) == 0)
				append = true;
		}
		else if (kind < 41)
		{
			if ((int)hjcode.jsiz == kind - 37)
				append = true;
		}
		else if (kind == 42)
		{
			if ((int)hjcode.prmr == kind - 41)
				append = true; 
		}
		else if (IS_WITHINx(42, 46, kind))
		{
			const CString xxx = xxKOSDAQ[kind];
			if (hjcode.acdl.CompareNoCase(xxx) == 0)
				append = true;
		}

		if (!append) continue;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = hjcode.code;
		item.name = hjcode.name;
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
	return 0;
}

int CPage1::xKospiExToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 1000;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSPI)
			continue;

		append = false;

		switch (kind)
		{
		case 101: // KOSPI200
			if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2 || (int)hjcode.kpgb == 1)
				append = true;
			break;
		case 401: // KOSPI100
			if ((int)hjcode.kpgb == 3 || (int)hjcode.kpgb == 2)
				append = true;
			break;
		case 402: // KOSPI50
			if ((int)hjcode.kpgb == 3)
				append = true;
			break;
		case 403: // KOSPI IT
			if ((int)hjcode.itgb == 1)
				append = true;
			break;
		}

		if (!append) continue;

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = hjcode.code;
		item.name = hjcode.name;
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xKosdaqExToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	const int kind = selItem % 1000;

	CString	code, name;

	bool append = false;
	struct _shjcode hjcode; 
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.kosd != jmKOSDAQ)
			continue;

		append = false;

		switch (kind)
		{
		case 301: // KOSDAQ50
			if ((int)hjcode.kpgb == 1)
				append = true;
			break;
		case 302: // KOSDAQ IT
			if ((int)hjcode.itgb == 1)
				append = true;
			break;
		case 303: // KOSTAR
			if ((int)hjcode.star == 1)
				append = true;
			break;				
		}

		if (!append) continue;


		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = hjcode.code;
		item.name = hjcode.name;
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xThemaCodeToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	CString	skey; skey.Format("%03d", selItem % 1000);

	char	readB[512]{};
	UINT	readL{};
	CString	filePath, code, name, string = _T("");

	filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");

	readL = GetPrivateProfileString(_T("TMCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = parseX(string, ";");
		name = GetCodeName(code);


		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xGroupCodeToList(int selItem)
{
	ClearListitem(); 
	m_list1.DeleteAllItems();

	CString	skey; skey.Format("%03d", selItem % 1000);

	char	readB[512]{};
	UINT	readL{};
	CString	filePath, code, name, string = _T("");

	filePath.Format("%s/%s/%s", m_root, TABDIR, "gropcd.ini");

	readL = GetPrivateProfileString(_T("GRCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = parseX(string, ";");
		name = GetCodeName(code);


		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}


int CPage1::xCFutureToList(bool bCurrency)	//2012.10.04 KSJ 금리/통화 통화일때는 true
{
	ClearListitem(); m_list1.DeleteAllItems();
	
	ccode   ccode;
	CString	code, name;
//	int position = 0;
	
	if(bCurrency)
	{
		for (int ii = 0; ii < m_cccode.GetSize(); ii++)
		{
			ccode = m_cccode.GetAt(ii);
			
			code = CString(ccode.codx, FCodeLen);
			name = CString(ccode.hnam, FNameLen*2 + 10);
			
			code.TrimLeft(); code.TrimRight();
			if (code.IsEmpty()) continue;
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}
	else
	{
		for (int ii = 0; ii < m_cicode.GetSize(); ii++)
		{
			ccode = m_cicode.GetAt(ii);
			
			code = CString(ccode.codx, FCodeLen);
			name = CString(ccode.hnam, FNameLen*2 + 10);
			
			code.TrimLeft(); code.TrimRight();
			if (code.IsEmpty()) continue;
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}
	

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

int CPage1::xSFutureToList()	//2012.10.04 KSJ 주식선물
{
	ClearListitem(); m_list1.DeleteAllItems();
	
	sfcode   sfcode;
	CString	code, name;
//	int position = 0;
	
	for (int ii = 0; ii < m_sfcode.GetSize(); ii++)
	{
		sfcode = m_sfcode.GetAt(ii);
		
		code = CString(sfcode.codx, FCodeLen);
		name = CString(sfcode.hnam, FNameLen*2 + 10);
		
		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;
		
		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = name;
	}
	
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}

void CPage1::OKaction()
{
	//XMSG(...);
//	DWORD	fLen = 0; 
	CFileFind finder;
	CString	filePath, fileTemp;

	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	//if (finder.FindFile(fileTemp))
	//{
	//	DeleteFile(filePath); 
	//	CopyFile(fileTemp, filePath, FALSE);
	//}

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	CopyFile(fileTemp, filePath, FALSE);		
	//}

//	OnAllsave();
	
	CWnd* wnd = GetParent()->GetParent();

	CCaction();
}

void CPage1::OKactionOne(int gnum)
{
//	DWORD	fLen = 0; 
	CFileFind finder;
	CString	filePath, fileTemp;

	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, gnum);
	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gnum);
	
	CString strTemp;
	strTemp.Format("gnum[%d] filepath[%s]", gnum, filePath);

	CWnd* wnd = GetParent()->GetParent();

	if(finder.FindFile(fileTemp))
	{
		CopyFile(fileTemp, filePath, FALSE);
		DeleteFile(fileTemp); 
				
	}
}

void CPage1::CCaction()
{
	//XMSG(...);
	//CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	//DeleteFile(tempN);

	////DWORD	fLen = 0;
	//CFileFind finder;
	//CString	fileTemp;
	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	DeleteFile(fileTemp);
	//}
	//CWnd* wnd = GetParent()->GetParent();
}

int CPage1::GetUploadData(int gno, CString& name, char* datB)
{
//	m_slog.Format("\r\n[interest][IB202201][%s]  gno = [%d]", __FUNCTION__, gno);
//	OutputDebugString(m_slog);

	gsl::span<struct _jinfo> spanjinfo = gsl::span<struct _jinfo>((struct _jinfo*)datB, maxJONGMOK);	
	int ii = 0;
	for_each(m_inters.begin(), m_inters.end(), [&](const auto item){
		auto& info = spanjinfo.at(ii++);
		FillMemory(&info, sizeof(struct	_jinfo), ' ');

		info.gubn[0] = item->gubn;
		CopyMemory(info.code, item->code, item->code.GetLength());
		CopyMemory(info.xprc, item->xprc, item->xprc.GetLength());
		CopyMemory(info.xnum, item->xnum, item->xnum.GetLength());		
		CopyMemory(info.bookrgb, item->bookrgb, item->bookrgb.GetLength());
	});

	// for_each(m_inters.begin(), m_inters.end(), [&](auto item) {
	// 	m_slog.Format("\r\n[interest][IB202201] c=[%s] n=[%s] p = [%s] n=[%s] book=[%c] rgb=[%s]",
	// 		item->code, item->name, item->xprc, item->xnum, item->bookmark, item->bookrgb);
	// 	OutputDebugString(m_slog);
	// });
	return m_inters.size();
}

int CPage1::SetDnloadData(int gno, CString gname, int count, char* datB)
{
//	m_slog.Format("\r\n[interest][IB202201][%s]  gno = [%d] gname=[%s]", __FUNCTION__, gno, gname);
//	OutputDebugString(m_slog);

	const int index = gno - 1;

	if (atoi(m_manageGroup[index][0]) != gno)
		m_manageGroup[index][0] = AxStd::FORMAT("%02d");

	if (gname.CompareNoCase(m_manageGroup[index][3]) != 0)
		m_manageGroup[index][3] = gname;
	
	if (m_activegno == gno)
	{
		ClearInterest();

		const gsl::span<struct _jinfo> codelist((struct _jinfo*)&datB[0], count);
		for_each(codelist.begin(), codelist.end(), [&](auto item) {
			auto inter = m_inters.emplace_back(std::make_shared<_intersx>());
			inter->copy(&item);
			inter->name = GetCodeName(inter->code);

			});

		// for_each(m_inters.begin(), m_inters.end(), [&](auto item) {
		// 	m_slog.Format("\r\n [interest][IB202201] c=[%s] n=[%s] p = [%s] n=[%s] book=[%c] rg1n=[%s]",
		// 		item->code, item->name, item->xprc, item->xnum, item->bookmark, item->bookrgb);
		// 	OutputDebugString(m_slog);
		// 	});

		m_list2.SetItemCountEx(m_inters.size());
		m_list2.Invalidate();

		updateList2sum();
	}
	return count;
}

void CPage1::enableButton(BOOL enable)
{
	CButton* btn = nullptr;
	
	btn = (CButton *) GetDlgItem(IDC_UPLOAD); btn->EnableWindow(enable);
	btn = (CButton *) GetDlgItem(IDC_DNLOAD); btn->EnableWindow(enable);
}

int CPage1::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;

	const int result = m_parent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
	case etfType:				// etf code
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		return	futureCODE;		// 선물
	case sfutureType:
		return sfCODE;		
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// 업종
		return	indexCODE;
	case thirdType:				// 제3시장
		return	thirdCODE;
	case sinjuType:				// 신주인수권
		return	sinjuCODE;
	case mufundType:			// 뮤츄얼펀드
	case reitsType:				// 리츠
	case hyfundType:			// 하이일드펀드
		return 200;
	}
	
	return 0;
}

CString CPage1::GetCodeName(CString code)
{
	CString name;
	code = code.Left(12);
	code.TrimRight();
	code.TrimLeft();
//	code.Remove(0);	
//	int codeL = code.GetLength();
	const int codeL = strlen(code);

	if (codeL == 6)					// kospi code
	{
		_shjcode   hjcode; 
		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;

			switch (hjcode.kosd)
			{
			case jmKOSPI:
			case jmKOSDAQ:
			case jm3RD:
				if (hjcode.ssgb == jmELW)
					return hjcode.name;
				break;
			}
			name = hjcode.name;
			if(name.GetLength() > 32)
				name = name.Mid(0,32);
			return name;
		}

	}
	else if (codeL == 9)				// 신주인수권 추가
	{
		_shjcode   hjcode; 
		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;
			
			switch (hjcode.kosd)
			{
			case jmSINJU:
				if (hjcode.ssgb == jmSINJU)
					return hjcode.name;
				break;
			}

			name = hjcode.name;
			if(name.GetLength() > 32)
				name = name.Mid(0,32);
			return name;
		}
	}
	else if (codeL <= 3)				// upjong code
	{
		struct	upcode	upcode;
		for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
		{
			upcode = m_upcode.GetAt(ii);

			CString upCodeStr;
			int upCodeInt;
			upCodeStr.Format("%d%02d", upcode.jgub, upcode.ucod);
			upCodeInt = atoi(upCodeStr);

			if (atoi(code) != upCodeInt)
				continue;

			name = CString(upcode.hnam, UNameLen);
			if(name.GetLength() > 32)
				name = name.Mid(0,32);
			return name;
		}
	}
	else if (code[0] == '1' || code[0] == '4' || code[0] == 'A' || code[0] == 'D')	// future code  //파생상품 코드개편
	{
		int nGubn = atoi(code.Mid(1,2));
		
		if(nGubn > 10 && nGubn < 60)	//주식선물
		{
			sfcode   sfcode;
			CString strCode;
			for (int ii = 0; ii < m_sfcode.GetSize(); ii++)
			{
				sfcode = m_sfcode.GetAt(ii);
				
				strCode = CString(sfcode.codx , 8);
				if (code != strCode)
					continue;
				
				name = CString(sfcode.hnam, 50);
				if(name.GetLength() > 32)
					name = name.Mid(0,32);
				return name;
			}
		}
		else if(nGubn > 60 && nGubn <= 70)	//금리선물
		{
			ccode   ccode;
			CString strCode;
			for (int ii = 0; ii < m_cicode.GetSize(); ii++)
			{
				ccode = m_cicode.GetAt(ii);

				strCode = CString(ccode.codx , 8);
				if (code != strCode)
					continue;
				
				name = CString(ccode.hnam, 50);
				if(name.GetLength() > 32)
					name = name.Mid(0,32);

				return name;
			}
		}
		else if(nGubn > 70 && nGubn < 80)	//통화선물
		{
			ccode   ccode;
			CString strCode;
			for (int ii = 0; ii < m_cccode.GetSize(); ii++)
			{
				ccode = m_cccode.GetAt(ii);

				strCode = CString(ccode.codx , 8);
				if (code != strCode)
					continue;
				
				name = CString(ccode.hnam, 50);
				if(name.GetLength() > 32)
					name = name.Mid(0,32);
				
				return name;
			}
		}
		else if ((code.Mid(1, 2) >= "B0" && code.Mid(1, 2) <= "ZZ") || (code.Mid(1, 2) >= "0A" && code.Mid(1, 2) <= "0Z") || (code.Mid(1, 2) >= "1A" && code.Mid(1, 2) <= "1Z")
			|| (code.Mid(1, 2) >= "2A" && code.Mid(1, 2) <= "2Z") || (code.Mid(1, 2) >= "3A" && code.Mid(1, 2) <= "3Z") || (code.Mid(1, 2) >= "4A" && code.Mid(1, 2) <= "4Z")
			|| (code.Mid(1, 2) >= "5A" && code.Mid(1, 2) <= "5Z") || (code.Mid(1, 2) >= "6A" && code.Mid(1, 2) <= "6Z") || (code.Mid(1, 2) >= "7A" && code.Mid(1, 2) <= "7Z")
			|| (code.Mid(1, 2) >= "8A" && code.Mid(1, 2) <= "8Z") || (code.Mid(1, 2) >= "9A" && code.Mid(1, 2) <= "9Z"))
		{
			sfcode   sfcode;
			CString strCode;
			for (int ii = 0; ii < m_sfcode.GetSize(); ii++)
			{
				sfcode = m_sfcode.GetAt(ii);

				strCode = CString(sfcode.codx, 8);
				if (code != strCode)
					continue;

				name = CString(sfcode.hnam, 50);
				if (name.GetLength() > 32)
					name = name.Mid(0, 32);
				return name;
			}
		}
		else
		{
			codelistitem   sjcode;
			for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
			{
				sjcode = m_fjcode.GetAt(ii);
				if (code != sjcode.code)
					continue;
				
				name = sjcode.name;
				if(name.GetLength() > 32)
					name = name.Mid(0,32);
				
				return name;
			}
		}
	}
	else if (code[0] == '2' || code[0] == '3'  || code[0] == 'B' || code[0] == 'C')  //파생상품 코드개편
	{
		if (code[1] == '0')			// future option code
		{
			struct ojcode	ojcode;
			for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
			{
				ojcode = m_ojcode.GetAt(ii);

				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.call[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.call[jj].cod2, OCodeLen -1))
						continue;

					name = CString(ojcode.call[jj].hnam, ONameLen);
					if(name.GetLength() > 32)
						name = name.Mid(0,32);
					
					return name;
				}
				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.put[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.put[jj].cod2, OCodeLen -1))
						continue;
					
					name = CString(ojcode.put[jj].hnam, ONameLen);
					if(name.GetLength() > 32)
						name = name.Mid(0,32);
					
					return name;
				}
			}
		}
		else					// 
		{
			struct	pjcode pjcode;
			for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
			{
				pjcode = m_pjcode.GetAt(ii);
				if (code != CString(pjcode.codx, PCodeLen))
					continue;

				name = CString(pjcode.hnam, sizeof(pjcode.hnam));
				if(name.GetLength() > 32)
					name = name.Mid(0,32);
				
				return name;
			}
		}
	}

	return _T("");
}

CString	CPage1::GetGroupORder2(CString& data)
{
	CString temp1, temp2, string;
	temp1 = "";
	temp2 = "";
	string = "";
	
	data = "";
	for(int i =0 ; i<MAXGROUP ; i++)
	{
		temp1 = m_manageGroup[i][0];
		temp2 = m_manageGroup[i][1];

		if(!temp1.IsEmpty())
		{
			string += temp1;
			string += ";";

			data += temp2;
			data += ";";
		}
	}

	return string;
}

LRESULT CPage1::OnSelectGroup(WPARAM wParam, LPARAM lParam)
{
	SetTimer(WM_TOSELECTPAGE, 400, nullptr);	
	return 0;
}

LRESULT CPage1::OnSelectGroupNCode(WPARAM wParam, LPARAM lParam)
{
	SetTimer(WM_TOSELECTGROUPNCODE, 400, nullptr);	
	return 0;
}

//2012.02.13 KSJ
void CPage1::AppendItem(CString sCodes)
{
	//해당그룹에 종목넣기
	insertList2(sCodes);	
}

//종목 넣기
void CPage1::insertList2(CString sCodes)
{	
	m_bIsEdit = TRUE;
	m_bFirstDuplicateNO = false;		//2012.04.02 KSJ 처음에 디폴트로..

	_vAddInter.clear();
	std::vector<CString> vCodes;
	AxStd::SplitSub(vCodes, sCodes, "\t");

	CString name;
	vCodes.resize(100);

	for (auto code : vCodes)
	{
		if (code.GetLength() < 6)
			return;

		code.TrimLeft("A");
		code.Trim();
		const int nitem = findMatchIndex(code, 0);
		if (nitem == -1)
			return;

		name = m_list1.GetItemText(nitem, 1);
		const int gubn = m_tree.GetItemData(m_tree.GetSelectedItem());

		auto pinter = _vAddInter.emplace_back(std::make_shared<_intersx>());
		pinter->gubn = (char)gubn;
		pinter->code = code;
		pinter->name = name;
		pinter->bookmark = '0';
	}
}

//종목 넣기
void CPage1::insertList2x(CString sCodes)
{
	m_bIsEdit = TRUE;
	m_bFirstDuplicateNO = false;		//2012.04.02 KSJ 처음에 디폴트로..

	_vAddInter.clear();
	std::vector<CString> vCodes;
	AxStd::SplitSub(vCodes, sCodes, "\t");

	CString name;
	if (vCodes.size() > 100)
		vCodes.resize(100);

	for (auto code : vCodes)
	{
		if (code.GetLength() < 6)
			continue;

		code.TrimLeft("A");
		code.Trim();
		const int nitem = findMatchIndex(code, 0);
		if (nitem == -1)
			continue;

		name = m_list1.GetItemText(nitem, 1);
		const int gubn = m_tree.GetItemData(m_tree.GetSelectedItem());

		auto pinter = m_inters.emplace_back(std::make_shared<_intersx>());
		pinter->gubn = (char)gubn;
		pinter->code = code;
		pinter->name = name;
		pinter->bookmark = '0';
	}

	m_list2.SetItemCountEx(m_inters.size());
	m_list2.Invalidate();

}


//새그룹 생성
void CPage1::AppendNewgroup()
{
	CString	gname;	
	m_bIsEdit = TRUE;

	if (m_activegno != -1)
	{
		const int cnt = m_gname.GetCount();
		m_selectGroup = cnt;

		gname.Format("그룹%03d", cnt+1);

		//메세징 배열 수정	NewGroup에서 OnOk일때 해주던 작업을 해준다. KSJ
		CString sequence, temp;
//		int index = 0;
		for(int i=0 ; i < 100 ; i++)
		{
			sequence = m_manageGroup[i][0];	
			if(!sequence.IsEmpty())
				continue;
			
			temp.Format("%02d", i+1);
			
			if(i != 100)
			{
				m_manageGroup[i][0] = temp;
				m_manageGroup[i][1] = "N";
				m_manageGroup[i][2] = temp;
				m_manageGroup[i][3] = gname;				
				break;
			}
		}
		// 끝

		if (savingGroupOrder(gname))
		{
			OnDelall();
			savingGroupFile(m_activegno, gname);
		}
		
		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);
	}
}

// KSJ

long CPage1::OnAppendParentGroup(WPARAM wParam, LPARAM lParam)
{
	const char gnCHAR = 0x7f;		//DEL
	const char gnTab = 0x09;		//TAB



	if (m_appendGI.IsEmpty())
		return 0;

	CString	gname = parseX(m_appendGI, gnCHAR);

	

	ShowWindow(SW_SHOW);
	CNewGroup newGroup(this, 0, gname);

	if (newGroup.DoModal() != IDOK)
	{
		m_appendGI = _T("");
		return 0;
	}
	
	gname = newGroup.m_defName;
	if (savingGroupOrder(gname))
	{
		OnDelall();
		savingGroupFile(m_activegno, gname);
	}

	//Send 하여 시퀀스 정렬시킨다
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);	

	char	wB[64]{}, * xxP{};
	int	position{};
	CString	code, name, temp;
	char bookmark;
	bookmark = '0';

	while (!m_appendGI.IsEmpty())
	{
		if (m_list2.GetItemCount() >= maxJONGMOK)
			break;

		char iii[64];

		memcpy( iii, m_appendGI, 64 );

	
		code = parseX(m_appendGI, gnCHAR);
		temp = parseX(m_appendGI, gnCHAR);
		CopyMemory(&bookmark, temp, min(strlen(temp), sizeof(bookmark)));
//		code = temp;
		
		strcpy(wB, code);
		xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
		name = xxP; 
		name.TrimLeft(); 
		name.TrimRight();

		position = m_list2.GetItemCount();
		AppendItemToList2(position, 0, code, name, bookmark);
	}

	m_appendGI = _T(""); 
	return 0;
}

long CPage1::OnAppendGroup(WPARAM wParam, LPARAM lParam)
{
	const char gnCHAR = 0x7f;
	
	if (m_appendGI.IsEmpty())
		return 0;
	
	CString	gname = parseX(m_appendGI, gnCHAR);
	
	ShowWindow(SW_SHOW);
	CNewGroup newGroup(this, 0, gname);
	
	if (newGroup.DoModal() != IDOK)
	{
		m_appendGI = _T("");
		return 0;
	}
	
	gname = newGroup.m_defName;
	if (savingGroupOrder(gname))
	{
		OnDelall();
		savingGroupFile(m_activegno, gname);
	}

	//Send 하여 시퀀스 정렬시킨다
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);		

	char	wB[64]{}, * xxP{};
	int	position{};
	CString	code, name;
	while (!m_appendGI.IsEmpty())
	{
		if (m_list2.GetItemCount() >= maxJONGMOK)
			break;
		
		code = parseX(m_appendGI, gnCHAR);
		
		strcpy(wB, code);

		const int codeSize = strlen(wB);

		if(codeSize > 0)
		{
			xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
			name = xxP; 
			name.TrimLeft(); 
			name.TrimRight();
			
			position = m_list2.GetItemCount();
			AppendItemToList2(position, 0, code, name);
		}

	}
	
	m_appendGI = _T(""); 
	return 0;
}


void CPage1::AppendItemToList1(int ipos, CString code, CString name,  _listitem *pListItem)
{
	//speed
	CITEMS item;
	item.code = code;
	item.name = name;

	_vList.emplace(_vList.begin() + ipos,item);	
}

void CPage1::AppendItemToList2(int ipos, std::shared_ptr<struct _intersx> inter, bool add)
{
	int	nItem = 0;
 	if(m_list2.GetItemCount() >= 100)
 		MessageBox("최대 100 종목만 등록 가능 합니다.", "IBK투자증권", MB_OK);
	
	// MODIFY PSH 20070918
	//nItem = AddItem(&m_list2, code, ipos);
	CString code, name;

	code = inter->code;
	name = inter->name;

	if(code.Left(1) != " ")
	{
		code.TrimRight();
		if(code.GetAt(0) == 'm')
		{
			name.TrimRight();
		}
		else
		{
			name = GetCodeName(code);
			name.TrimRight();
		}
	}

	CString strCode = ('m' == code.GetAt(0)) ? "" : code;
	nItem = AddItem(&m_list2, strCode, ipos);
	// END MODIFY
	AddItem(&m_list2, name, nItem, 1);
		
	if (add) m_inters.emplace_back(inter);
	else     m_inters.emplace(m_inters.begin() + ipos, inter);
	
	updateList2sum();
	
	m_nCol0Asc = -1;
	m_nCol1Asc = -1;
	SetListColumnText(&m_list2, 1, -1);
	SetListColumnText(&m_list2, 0, -1);
}

void CPage1::AppendItemToList2(int ipos, int gubn, CString code, CString name, CString bookmark, bool add /*=true*/)
{
	if(name.Left(1) != " ")
		name.TrimRight();

	std::shared_ptr<_intersx> pinter = std::make_shared<_intersx>();
	if (add)
		m_inters.emplace_back(pinter);
	else
		m_inters.emplace(m_inters.begin() + ipos, pinter);

	pinter->gubn = (char)gubn;
	pinter->code = ('m' == code.GetAt(0)) ? "" : code;
	pinter->name = name;
	pinter->bookmark = bookmark[0];

	m_list2.SetItemCountEx(m_inters.size());
	m_list2.Invalidate();

	updateList2sum();

	m_nCol0Asc = -1;
	m_nCol1Asc = -1;
	SetListColumnText(&m_list2, 1, -1);
	SetListColumnText(&m_list2, 0, -1);
}

void CPage1::DeleteItemToList2(int ipos)
{
	if (m_list2.GetItemCount() > ipos)
		m_list2.DeleteItem(ipos);

	if (gsl::narrow_cast<int>(m_inters.size()) > ipos)
		m_inters.erase(m_inters.begin() + ipos);

	updateList2sum();
}

bool CPage1::existCodeInList2(CString code)
{
	CString	chkcode;
	int result = -1;

	if(m_isDuplicateCode == true)	// 종목 중복등록 허용
	{
		
	}
	else
	{
		for (int ii = 0 ; ii < m_list2.GetItemCount() ; ii++)
		{
			chkcode = m_list2.GetItemText(ii, 0);
			chkcode.TrimRight();
			chkcode.TrimLeft();
			
			if (code.Compare(chkcode) == 0)
			{
				if(!m_bFirstDuplicateNO)	//2012.04.02 KSJ 한번만 물어보게
				{
					result = MessageBox("동일한 종목이 관심그룹 내에 존재합니다. 중복등록을 허용하시겠습니까?", "관심종목 설정", MB_YESNO);
					m_nResult = result;	//2012.04.02 KSJ 결과값 저장.
				}
				else 
				{
					result = m_nResult;	//2012.04.02 KSJ 이전 결과값으로 다시 세팅
				}
				
				if(result == IDYES)
				{
					m_bFirstDuplicateNO = true;		//2012.04.02 KSJ 한번만 물어보게
					break;
				}
				else
				{
					m_bFirstDuplicateNO = true;			//2012.04.02 KSJ 한번만 물어보게
					if (m_btnClear)
						m_btnClear->Invalidate();
					return true;	//2012.04.02 KSJ 중복등록 허용안함.
				}
			}
		}
	}
	// 2012.02.13 KSJ
// 	else
// 		return true;
	// KSJ
	if (m_btnClear)
		m_btnClear->Invalidate();
	return false;
}

CString	CPage1::GetGroupName(int gno)
{
	gno = max(gno - 1, 0);
	return m_manageGroup.at(gno).at(3);
}

void CPage1::GuideMsg(CString msg)
{
//	MessageBox("["+msg+"]", "IBK투자증권");
	Variant(guideCC, msg);
}

bool CPage1::GuideMsg(CString msg, bool question)
{
	UINT nType = MB_OK|MB_ICONEXCLAMATION;
	if (question)
		nType = MB_OKCANCEL|MB_ICONEXCLAMATION;

	if (MessageBox(msg, idTOstring(IDS_REGISTERINTER), nType) == IDCANCEL)
		return false;

	return true;
}

CString CPage1::parseX(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CPage1::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

CString CPage1::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

void CPage1::SetOther(class CPage4* page)
{
	m_page = page; 
}

int CPage1::GetActiveGroup()
{
	return m_activegno;
}

void CPage1::SetArray(std::vector<std::shared_ptr<_intersx>>& arInters)
{
//XMSG(사용안함 변경됨));
//	struct	_inters*	pinternew = nullptr;
//	struct	_inters*	pinterold = nullptr;
//	const int ncnt = m_inters.size();

	//for ( int ii = 0 ; ii < ncnt ; ii++ )
	//{
	//	auto& pinterold = m_inters.at(ii);
	//	pinternew = arInters.GetAt(ii);

	//	if (strcmp(pinternew->code, pinterold->code) == 0)
	//	{
	//		memset(pinterold->xnum, 0x00, sizeof(pinterold->xnum));
	//		memcpy(pinterold->xnum, pinternew->xnum, sizeof(pinternew->xnum));
	//		memset(pinterold->xprc, 0x00, sizeof(pinterold->xprc));
	//		memcpy(pinterold->xprc, pinternew->xprc, sizeof(pinternew->xprc));
	//		memset(pinterold->bookmark, 0x00, sizeof(pinterold->bookmark));
	//		memcpy(pinterold->bookmark, pinternew->bookmark, sizeof(pinternew->bookmark));
	//	}
	//}

	std::copy(arInters.begin(), arInters.end(), std::back_inserter(m_inters));
}

void CPage1::InitEdit()
{
	m_search.SetWindowText("");
	m_search.SetFocus();
}


bool CPage1::ExistCodeList(CString code)
{
	CString listcode;
	
	const int ncnt = m_list1.GetItemCount();
	for(int ii = 0; ii < ncnt; ii++)
	{
		listcode = m_list1.GetItemText(ii, 0);
		listcode.TrimLeft(); listcode.TrimRight();
		
		if(code.Compare(listcode) == 0)
			return true;
	}
	return false;
}

void CPage1::EnableButton(BOOL bEnable)
{
	constexpr UINT btns[] = { IDC_DNLOADR, IDC_DNLOAD, IDC_NEWGROUP, IDC_EDITGROUP, IDC_ALLSAVE,
		IDC_GNAME, IDC_ADD, IDC_DEL, IDC_ADDALL, IDC_DELALL, IDC_BLANK, IDC_MOVEUP, IDC_MOVEDN,
		IDC_SEARCH, IDC_BTN_SORT01, IDC_BTN_SORT02, IDC_BTN_SORT03, IDC_BTN_SORT04, IDC_BTN_SORT05, 
		IDC_BTN_SORT06, IDC_BTN_SORT07, IDC_BTN_SORT08, IDC_BTN_SORT09, IDC_BTN_SORT10, 
		IDC_BTN_SORT11, IDC_BTN_SORT12, IDC_BTN_SORT13, IDC_BTN_SORT14, IDC_BTN_SORT15, 
		IDC_BTN_SORT16, IDC_BTN_SORT17, IDC_BTN_SORT18, IDC_BTN_SORT19, IDC_BTN_SORT20, 
		IDC_BTN_SORT21, IDC_BTN_SORT22};

	for (int ii = 0; ii < sizeof(btns)/sizeof(UINT); ii++)
	{
		GetDlgItem(btns[ii])->EnableWindow(bEnable);
	}

}

void CPage1::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (nullptr != pNMListView)
	{
		switch(pNMListView->iSubItem) {
		case 0:
			{
				if (0 != m_nPrevCol)
				{
					m_bCol0Asc = FALSE;
				}

				m_bCol0Asc = !m_bCol0Asc;
				m_listsort = !m_bCol0Asc;

				listsort(0);

				SetListColumnText(&m_list1, 0, m_bCol0Asc);
				SetListColumnText(&m_list1, 1, -1);
			}
			break;
		case 1:
			{
				if (1 != m_nPrevCol)
				{
					m_bCol1Asc = FALSE;
				}

				m_bCol1Asc = !m_bCol1Asc;
				m_listsort = !m_bCol1Asc;

				listsort(1);

				SetListColumnText(&m_list1, 1, m_bCol1Asc);
				SetListColumnText(&m_list1, 0, -1);
			}
			break;
		} 
		m_nPrevCol = pNMListView->iSubItem;
	}
	*pResult = 0;
}

void CPage1::SetListColumnText(CListCtrl* pList, int nCol, int nAsc)
{
	CHeaderCtrl* pCtrl = pList->GetHeaderCtrl();

	if (nullptr != pCtrl)
	{
		CString strText;

		switch(nCol) {
		case 0:
			strText = "코드";
			break;
		case 1:
			strText = "종목명";
			break;
		}

		switch (nAsc) {
		case 0:
			{
				strText += " ▼";
			}
			break;
		case 1:
			{
				strText += " ▲";
			}
			break;
		}

		HDITEM hdItem;
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = strText.GetBuffer(0);

		pCtrl->SetItem(nCol, &hdItem);
	}
}

void CPage1::OnColumnclickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_orginters.size() == 0)
		m_orginters = m_inters;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (nullptr != pNMListView)
	{
		BOOL bBase = FALSE;

		switch(pNMListView->iSubItem) {
		case 0:
			{
				if (0 != m_nPrevList2)
				{
					m_nCol0Asc = -1;
				}

				m_nCol0Asc = m_nCol0Asc - 1;

				switch (m_nCol0Asc) {
				case -1:
					break;
				case 0:
					m_listsort = TRUE;
					break;
				default:
					m_listsort = FALSE;
					m_nCol0Asc = 1;
					break;
				}

				if (-1 != m_nCol0Asc)
				{
					listsort2(0);
				}
				else
				{
					bBase = TRUE;
				}

				SetListColumnText(&m_list2, 0, m_nCol0Asc);
				SetListColumnText(&m_list2, 1, -1);
			}
			break;
		case 1:
			{
				if (1 != m_nPrevList2)
				{
					m_nCol1Asc = -1;
				}

				m_nCol1Asc = m_nCol1Asc - 1;

				switch (m_nCol1Asc) {
				case -1:
					break;
				case 0:
					m_listsort = TRUE;
					break;
				default:
					m_listsort = FALSE;
					m_nCol1Asc = 1;
					break;
				}

				if (-1 != m_nCol1Asc)
				{
					listsort2(1);
				}
				else
				{
					bBase = TRUE;
				}

				SetListColumnText(&m_list2, 1, m_nCol1Asc);
				SetListColumnText(&m_list2, 0, -1);
			}
			break;
		} 

		if (TRUE == bBase)
		{
			if (m_orginters.size() != 0)
			{
				m_inters = m_orginters;
				m_orginters.clear();
			}
			m_list2.Invalidate();
		}

		m_nPrevList2 = pNMListView->iSubItem;
	}
	
	*pResult = 0;
}

void CPage1::OnShareimport() 
{
	const char szFilter[] = "Share Interest Group Files (*.sig)|*.sig||";
	int nPos = -1;
	//int nCount = 0;
	//int nItem = -1;
	//int nIndex = 0;
	UINT	readL = 0;
	struct	_inters* pinter = nullptr;

	CFileDialog dlg(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST,
                      szFilter, this);
	
	if (IDOK == dlg.DoModal())
	{
		m_bIsEdit = TRUE;

		CString strFile = dlg.GetPathName();
		
		if (strFile.IsEmpty())
			return;

		CWnd* wnd = GetParent()->GetParent();

		CString	strGroupName;
		if (m_activegno != -1)
		{
			strGroupName = GetGroupName(m_activegno);
			savingGroupFile(m_activegno, strGroupName);

			//서버에 전송
			wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEachGroupACTION, m_activegno-1), 0);
		}

		strGroupName = dlg.GetFileName();
		nPos = strGroupName.Find(_T(".sig"));

		if (nPos == -1)
			return;

		strGroupName.Delete(nPos, 4);

		if (savingGroupOrder(strGroupName))
		{
			OnDelall();
			savingGroupFile(m_activegno, strGroupName);
		}

		CString temp;	
		temp.Format("%02d", m_activegno);
		
		if (m_activegno > 1)
		{
			m_manageGroup[m_activegno - 1][0] = temp;
			m_manageGroup[m_activegno - 1][1] = "N";
			m_manageGroup[m_activegno - 1][2] = temp;
			m_manageGroup[m_activegno - 1][3] = strGroupName;
		}
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upnewgroupACTION, 0), 0);

//		int count = 0;
		CFile fileH;	
		if (!fileH.Open(strFile, CFile::modeRead | CFile::shareDenyNone))
			return;
	
		const UINT len = gsl::narrow_cast<int>(fileH.GetLength());
		CString sBuffer;
		readL = fileH.Read(sBuffer.GetBuffer(len + 1), len + 1);
		sBuffer.ReleaseBuffer();
		fileH.Close();
	
		if (len == readL)
		{
			ClearInterest();

			gsl::span<_inters> spanInter((_inters*)sBuffer.GetBuffer(), maxJONGMOK);	
			for (auto& item :spanInter)
			{
				auto &inter = m_inters.emplace_back(std::make_shared<_intersx>());	
				inter->gubn = item.gubn[0];
				inter->code = CString(item.code, codelen).Trim();
				inter->name = CString(item.name, namelen).Trim();
				inter->xprc = CString(item.xprc, 10).Trim();
				inter->xnum = CString(item.xnum, 10).Trim();
				inter->xupnum = CString(item.xupnum, 10).Trim();
				inter->bookmark = item.bookmark[0];
				inter->futureGubn = item.futureGubn[0];
				inter->creditPrc = item.creditPrc;
				inter->maeipPrc = item.maeipPrc;
			}
		
		}

		m_list2.SetItemCountEx(m_inters.size());
		updateList2sum();

		
		//int i = 0;
		//for (i = 0; i < maxJONGMOK; i++)
		//{
		//	pinter = (_inters *) new char[sz_inters];
		//	ZeroMemory(pinter, sz_inters);
		//	
		//	readL = fileH.Read(pinter, sz_inters);
		//	if (readL < sz_inters)
		//	{
		//		delete[] pinter;
		//		break;
		//	}


		//	/ maxJONGMOK
		//}	
	}
}

void CPage1::OnShareexport() 
{
	CString strGroupName = GetGroupName(m_activegno);

	const char szFilter[] =
		"Share Interest Group Files (*.sig)|*.sig||";

	strGroupName.Replace("/","-");

	CFileDialog dlg(FALSE, nullptr, (LPCTSTR)strGroupName, OFN_FILEMUSTEXIST,
                      szFilter, this);

	if (IDOK == dlg.DoModal())
	{
		m_bIsEdit = TRUE;
	
		CString	strFile = dlg.GetPathName();
		
		if (strFile.IsEmpty())
			return;

		strFile += _T(".sig");

		CFile	fileH(strFile, CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone);
		if (fileH.m_hFile == CFile::hFileNull) 
			return;
		
		for_each(m_inters.begin(), m_inters.end(), [&](const auto& item){
			struct	_inters pinter;
			FillMemory(&pinter, sizeof(_inters), ' ');

			pinter.gubn[0] = item->gubn;
			CopyMemory(pinter.code,   item->code, min(item->code.GetLength(), 12));
			CopyMemory(pinter.name,   item->name, min(item->name.GetLength(), 32));
			CopyMemory(pinter.xprc,   item->xprc, min(item->xprc.GetLength(), 10));
			CopyMemory(pinter.xnum,   item->xnum, min(item->xnum.GetLength(), 10));
			CopyMemory(pinter.xupnum, item->xupnum, min(item->xupnum.GetLength(), 16));
			pinter.bookmark[0]   = item->bookmark;
			pinter.futureGubn[0] = item->futureGubn;
			pinter.creditPrc = item->creditPrc;
			pinter.maeipPrc = item->maeipPrc;
			
			fileH.Write(&pinter, sizeof(_inters));
		});		
		fileH.Close();
	} 
}

void CPage1::ExactGroupData()
{
	m_list2.DeleteAllItems();
	ClearList2item();
	
	if(m_selectGroup != -1)
	{
		m_gname.SetCurSel(m_selectGroup);
		m_activegno = (int)m_gname.GetItemData(m_selectGroup);
		selectComboBox();
	}
}

void CPage1::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == WM_TOSELECTPAGE)
	{
		KillTimer(WM_TOSELECTPAGE);

		ExactGroupData();
	}
	else if(nIDEvent == WM_TOSELECTGROUPNCODE)
	{
		KillTimer(WM_TOSELECTGROUPNCODE);
		
		//ELW인지 파악
		analyzeELWCode();
			
		//그룹선택
		ExactGroupData();
	
		//종목선택
		searchAction(0);
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CPage1::analyzeELWCode()
{
	//ELW인지 파악
// 	CString	string; 
// 	int code_first = 0;
// 	m_search.GetWindowText(string);
// 	
// 	code_first = atoi(string.Left(1));
// 	if(code_first >= 5 && code_first < 9)
// 	{
// 		selectElw();
// 		m_search.SetWindowText(string);
// 	}
	//ELW인지 파악
	CString	string; 
	int code_first = -1;
	m_search.GetWindowText(string);
		
	
	if(string.GetLength() <= 0)
	{
		return;
	}
	
	const BYTE test = string.GetAt(0);
	if(IsDBCSLeadByte(test))
	{
		//한글이 있음
		return;
	}
	else
	{
		code_first = atoi(string.Left(1));
	}
	
	if(string.GetLength() == 1 || string.GetLength() == 6)
	{
		if(code_first >= 5 && code_first < 9)
		{
			selectElw();
			m_search.SetWindowText(string);
		}
		else if(code_first >=0 && code_first < 5 || code_first == 9)
		{
			selectKospi();
			m_search.SetWindowText(string);
		}
		
		m_search.SetSel(0, -1);
		m_search.ReplaceSel(string);
		m_search.SetFocus();
	}
}

int	CPage1::GetGroupCount()
{
	const int result = m_gname.GetCount();

	return result;
}

void CPage1::setManageGroup(char* datB)
{
	#define MAX_JM  120               /* 최대 관심그룹 갯수   */
	#define MAX_SIZE  4084            /* 최대 관심그룹 갯수   */
	
	struct  glist {
		char    ngrs[2];           /* New Group Seqn       */                                               
		char    ogrs[2];           /* New Group Seqn       */
		char    gnam[30];          /* Group Name           */
	};
	
	struct  grpfold {
		char    nrec[4];           /* Group Count          */
		struct  glist   glist[MAX_JM];
	};

	struct grpfold* gFold;
	gFold = (struct grpfold *)(datB);


	m_manageGroup.fill({ "", "", "", "" });

	int nCount = atoi(CString(gFold->nrec, sizeof(gFold->nrec)));		
	const gsl::span<struct glist> spanlist = gsl::span<struct glist>((struct glist*)gFold->glist, nCount);
	
	int ii = 0;
	for_each(spanlist.begin(), spanlist.end(), [&](const auto& item) {
		auto& group = m_manageGroup.at(ii++);
		group[0] = CString(item.ngrs, 2).Trim();
		group[2] = CString(item.ogrs, 2).Trim();
		group[3] = CString(item.gnam, 30).Trim();
	});

	if (nCount == 0)
	{
		auto& group = m_manageGroup.at(0);
		group[0].Empty();
		group[2].Empty();
		group[3].Empty();
	}
}

void CPage1::traceManageGroup()
{
//	for(int i=0 ; i< MAXGROUP ; i++)
//	{
// 		if(!m_manageGroup[i][0].IsEmpty())
// 		{
// 			for(int j=0 ; j<4 ; j++)
// 			{
// 				TRACE("[%d][%d] : %s\t", i, j, m_manageGroup[i][j]);
// 			}		
// 			TRACE("\n");
// 		}
// 		
//	}
}

void CPage1::sortManageGroup(int drag, int drop)
{
	const int maxCount = getManageCount();
	CString drag1, drag2, drag3, temp1, temp2, temp3;

	drag1 = ""; drag2 = ""; drag3 = "";
	temp1 = ""; temp2 = ""; temp3 = "";

	if(drop == 0)
	{
		drag1 = m_manageGroup[drag][0];
		drag2 = m_manageGroup[drag][1];
		drag3 = m_manageGroup[drag][3];	
		
		//밑으로 하나씩 복사
		for(int i=drag ; i>drop ; i--)
		{
			m_manageGroup[i][0] = m_manageGroup[i-1][0]; 
			m_manageGroup[i][1] = m_manageGroup[i-1][1]; 
			m_manageGroup[i][3] = m_manageGroup[i-1][3];							
		}
		
		//옮길 자리에 복사
		m_manageGroup[drop][0] = drag1;
		m_manageGroup[drop][1] = drag2;
		m_manageGroup[drop][3] = drag3;			
	}
	else if(drop == maxCount)
	{
		//일단 옮길 놈을 복사 
		drag1 = m_manageGroup[drag][0];
		drag2 = m_manageGroup[drag][1];
		drag3 = m_manageGroup[drag][3];
		
		//위로 하나씩 복사
		for(int i=drag ; i<drop ; i++)
		{
			m_manageGroup[i][0] = m_manageGroup[i+1][0]; 
			m_manageGroup[i][1] = m_manageGroup[i+1][1]; 
			m_manageGroup[i][3] = m_manageGroup[i+1][3];							
		}
		
		//옮길 자리에 복사
		m_manageGroup[drop][0] = drag1;
		m_manageGroup[drop][1] = drag2;
		m_manageGroup[drop][3] = drag3;	
	}
	else
	{
		if(drag < drop)
		{	
			//일단 옮길 놈을 복사 
			drag1 = m_manageGroup[drag][0];
			drag2 = m_manageGroup[drag][1];
			drag3 = m_manageGroup[drag][3];

			//위로 하나씩 복사
			for(int i=drag ; i<drop ; i++)
			{
				m_manageGroup[i][0] = m_manageGroup[i+1][0]; 
				m_manageGroup[i][1] = m_manageGroup[i+1][1]; 
				m_manageGroup[i][3] = m_manageGroup[i+1][3];							
			}

			//옮길 자리에 복사
			m_manageGroup[drop][0] = drag1;
			m_manageGroup[drop][1] = drag2;
			m_manageGroup[drop][3] = drag3;			
		}
		else if(drag > drop)
		{
			drag1 = m_manageGroup[drag][0];
			drag2 = m_manageGroup[drag][1];
			drag3 = m_manageGroup[drag][3];	
			
			//밑으로 하나씩 복사
			for(int i=drag ; i>drop ; i--)
			{
				m_manageGroup[i][0] = m_manageGroup[i-1][0]; 
				m_manageGroup[i][1] = m_manageGroup[i-1][1]; 
				m_manageGroup[i][3] = m_manageGroup[i-1][3];							
			}
			
			//옮길 자리에 복사
			m_manageGroup[drop][0] = drag1;
			m_manageGroup[drop][1] = drag2;
			m_manageGroup[drop][3] = drag3;				
		}
	}
}

void CPage1::OnMovetop() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;
	
	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem == 0) 
		return;
		
	auto pinter = m_inters.at(nitem);
	m_inters.erase(m_inters.begin() + nitem);
	m_inters.emplace(m_inters.begin(), pinter);
	m_list2.Invalidate();

	deselcMark(&m_list2); 
	selcMark(&m_list2, 0);	
}

void CPage1::OnMovebottom() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_bIsEdit = TRUE;
	
	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem >= gsl::narrow_cast<int>(m_inters.size())) 
		return;
	
	auto pinter = m_inters.at(nitem);
	m_inters.erase(m_inters.begin() + nitem);
	m_inters.emplace_back(pinter);
	m_list2.Invalidate();

	deselcMark(&m_list2);
	selcMark(&m_list2, m_list2.GetItemCount() - 1);	
}

void CPage1::loadingETFcode()
{
CString ETFile;

	char* buf{};
	_efopitem* efop_item{};
	_efoptema* efop_tema{};
	_efopfore* efop_fore{};
	_efopcode* efop_code{};

	CString data, line;
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	CString gubn;
	
	ETFile.Format("%s/%s/efcode.dat", m_root, TABDIR);
	CStdioFile file(ETFile, CFile::modeRead|CFile::shareDenyNone);
	
	while(file.ReadString(line))
	{
		gubn = line.Left(1);		
		buf = line.GetBuffer(line.GetLength());
		line.ReleaseBuffer();

		if(strcmp(gubn, "1") == 0)
		{
			auto& efopitem = m_ETFitem.emplace_back(std::make_unique<struct _efopitem>());
			CopyMemory(efopitem.get(), buf, sizeof(struct _efopitem));					
		}
		else if(strcmp(gubn, "2") == 0)
		{			
			auto& efoptema = m_ETFtema.emplace_back(std::make_unique<struct _efoptema>());
			CopyMemory(efoptema.get(), buf, sizeof(struct _efoptema));
		}
		else if(strcmp(gubn, "3") == 0)
		{
			auto& efopfore = m_ETFfore.emplace_back(std::make_unique<struct _efopfore>());
			CopyMemory(efopfore.get(), buf, sizeof(struct _efopfore));
		}
		else if(strcmp(gubn, "4") == 0)
		{			
			auto& efopcode = m_ETFcode.emplace_back(std::make_unique<struct _efopcode>());
			CopyMemory(efopcode.get(), buf, sizeof(struct _efopcode));
		}
	
	}

	file.Close();
}

void CPage1::ETFInitialList()
{
	_efopcode* efopcode{};
	
	CString type, opcd, hnam;
	CString frcd, code, etfm, etfl;
	
//	int position = 0;	
	ClearListitem();
	m_list1.DeleteAllItems();
		
	for (auto& efopcode : m_ETFcode)
	{
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));

		auto& item = _vList.emplace_back(std::move(CITEMS()));
		item.code = code;
		item.name = hnam;

	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed

	
}

void CPage1::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
		
	CPoint      point;   
	UINT        nFlags = 0;   
    
	GetCursorPos(&point);   
	::ScreenToClient(m_tree.m_hWnd, &point);   
	
        HTREEITEM   hItem = m_tree.HitTest(point, &nFlags);   
	HTREEITEM	hChildItem = m_tree.GetChildItem(hItem);	
	CString str = m_tree.GetItemText(hItem);	
	

	if((hItem = m_tree.GetSelectedItem()) != nullptr)
	{
		hChildItem = m_tree.GetChildItem(hItem);	
		str = m_tree.GetItemText(hItem);

		if(str.Compare("전체종목") == 0)
		{
			ETFInitialList();
			//m_List.Selected(0);
		}
		
		if(hChildItem == nullptr && str.Compare("전체종목") != 0)
		{
			DataGubn(hItem);
			//m_ListCtrl.Selected(0);
			//			ListView_SortItems(m_ListCtrl.GetSafeHwnd(), ETFCompareFunc, 0);
		}

	}

}

void CPage1::OnClickELWTree(NMHDR* pNMHDR, LRESULT* pResult) 
{		
    CPoint      point;   
    
    GetCursorPos(&point);   
    ::ScreenToClient(m_tree.m_hWnd, &point);   
	
	HTREEITEM hItem;

//     HTREEITEM   hItem = m_tree.HitTest(point, &nFlags);   
// 	HTREEITEM	hParent = m_tree.GetParentItem(hItem);
// 	
// 
// 	CString str = m_tree.GetItemText(hItem);	
// 	CString parent = m_tree.GetItemText(hParent);
	
//	m_list1.UpdateData(FALSE);
		
// 	if (hItem != nullptr && (nFlags == 4 && TVHT_ONITEMSTATEICON != 0))   
// 	{   
// 		if(parent.Compare("전종목") == 0)
// 		{
// 			DataReloadELWAll(str);
// 			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
// 		}
// 		else if(parent.Compare("기초자산종목") == 0)			
// 		{
// 			DataReloadELWBase(str);
// 			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
// 		}
// 		else if(parent.Compare("발행사") == 0)
// 		{	
// 			DataReloadELWPB(str);
// 			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
// 		}
// 	}	

	if((hItem = m_tree.GetSelectedItem()) != nullptr)
	{
		HTREEITEM hParent = m_tree.GetParentItem(hItem);	
		
		CString str = m_tree.GetItemText(hItem);
		CString parent = m_tree.GetItemText(hParent);
		str.Replace("코스피", "KOSPI");

		if(parent.Compare("전종목") == 0)
		{
			DataReloadELWAll(str);
			listsort(0);
		}
		else if(parent.Compare("기초자산종목") == 0)			
		{
			DataReloadELWBase(str);
			listsort(0);
		}
		else if(parent.Compare("발행사") == 0)
		{	
			DataReloadELWPB(str);
			listsort(0);
		}

	}
}

void CPage1::DataReloadELWAll(CString str)
{
	_elwdata elwdata;
	_elwdata elw;
	CString code, name, ktype, pcode, pname, mjmt;
	int bkoba{};
//	int row = 0;
	CString rdocheck;
	codelistitem item{};
	CPtrArray arCode;
	CPtrArray gbCode;

	if(str.Compare("일반ELW") == 0)
		bkoba = 0;
	else
		bkoba = 1;
	
	
	m_list1.DeleteAllItems();
	ClearListitem();

	if(m_Radio_ELWAll.GetCheck())
	{
		rdocheck.Format("%02d", 3);
	}
	else if(m_Radio_ELWCall.GetCheck())
	{
		rdocheck.Format("%02d", 1);
	}
	else
		rdocheck.Format("%02d", 2);
	
	const int ncnt = m_ELWdata.GetSize();
	for(int ii = 0; ii < ncnt; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
		
		//item = new _listitem2;
		
		code = CString(elwdata.code, 12);
		name = CString(elwdata.name, ELWHNameLen);
		
		code.TrimRight(); code.TrimRight();
		name.TrimLeft(); name.TrimRight();

//		strcpy(item->code, code);
//		strcpy(item->name, name);

		if(bkoba == elwdata.bkoba && rdocheck.Compare(elwdata.ktype) == 0)
			_vList.push_back(item);
		else if(bkoba == elwdata.bkoba && rdocheck.Compare("03") == 0)
			_vList.push_back(item);
	//		else
//			gbCode.Add(item);


	}

	LockWindowUpdate();



	m_gListsort = false;
	//speed
	m_list1.SetItemCountEx(_vList.size());

	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.name.Compare(item2.name) > 0);
		return item2.name.Compare(item1.name) > 0;
	});

//	qsort(static_cast<void*>(&m_arList[0]), m_arList.GetSize(), sizeof( CITEMS), compareSortCode2);
	m_list1.Invalidate();
//@speed

	UnlockWindowUpdate();

	//for(int ii = 0; ii < arCode.GetSize(); ii++)
	//	delete arCode.GetAt(ii);
	//arCode.RemoveAll();
	//
	//for(int ii = 0; ii < gbCode.GetSize(); ii++)
	//	delete gbCode.GetAt(ii);
	//gbCode.RemoveAll();
}

void CPage1::DataReloadELWPB(CString str)
{
	_elwdata elwdata;
//	_elwdata *elw;
	CString code, name, ktype, pcode, pname, mjmt;
//	BOOL bkoba;
//	int row = 0;
	CString rdocheck;

	if(m_Radio_ELWAll.GetCheck())
	{
		rdocheck.Format("%02d", 3);
	}
	else if(m_Radio_ELWCall.GetCheck())
	{
		rdocheck.Format("%02d", 1);
	}
	else
		rdocheck.Format("%02d", 2);

	m_list1.DeleteAllItems();
	ClearListitem();
	
	const int nelwdata = m_ELWdata.GetSize();
	for(int ii = 0; ii < nelwdata; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
			
		code = CString(elwdata.code, ELWCodeLen);
		name = CString(elwdata.name, ELWHNameLen);
		ktype = CString(elwdata.ktype, 2);
		pcode = CString(elwdata.pcode, 12);
		pname = CString(elwdata.pname, 20);
		mjmt = CString(elwdata.mjmt, 12);
		//bkoba = CString(elwdata->bkoba);


		if(pname.Compare(str) == 0 && ktype == rdocheck)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
		else if(pname.Compare(str) == 0 && rdocheck.Compare("03") == 0)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}
	m_gListsort = false;
	//speed
	m_list1.SetItemCountEx(_vList.size());
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.name.Compare(item2.name) > 0);
		return item2.name.Compare(item1.name) > 0;
	});
	m_list1.Invalidate();
//@speed
}

void CPage1::DataReloadELWBase(CString str)
{
	_elwdata elwdata;
//	_elwdata *elw;
	CString code, name, ktype, pcode, pname, mjmt, bcode[5];
//	BOOL bkoba;
//	int row = 0;
	CString rdocheck;
	CString bscode, bsname, basename;
	codelistitem sjcode;

	if(m_Radio_ELWAll.GetCheck())
	{
		rdocheck.Format("%02d", 3);
	}
	else if(m_Radio_ELWCall.GetCheck())
	{
		rdocheck.Format("%02d", 1);
	}
	else
		rdocheck.Format("%02d", 2);
	
	m_list1.DeleteAllItems();
	ClearListitem();

	const int nelwdata = m_ELWdata.GetSize();
	for(int ii = 0; ii < nelwdata; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
		
		code = CString(elwdata.code, ELWCodeLen);
		name = CString(elwdata.name, ELWHNameLen);
		ktype = CString(elwdata.ktype, 2);
		pcode = CString(elwdata.pcode, 12);
		pname = CString(elwdata.pname, 20);
		mjmt = CString(elwdata.mjmt, 12);
		
		for (int jj = 0; jj < 5; jj ++)
		{
			bcode[jj] = CString(elwdata.bcode[jj], 12);
			bcode[jj].TrimRight();
			bcode[jj] = bcode[jj].Mid(1);
		}

	
		if(bcode[0].Compare("OSPI200") == 0)
		{
			basename = "KOSPI200";
			
		}
		else if(bcode[0].Compare("ASKET") == 0)
		{
			basename = "BASKET";
		
		}
		else
		{
			const int nelwbase = m_elwbase.GetSize();
			for(int jj = 0; jj < nelwbase; jj++)
			{
				sjcode = m_elwbase.GetAt(jj);
				bscode = sjcode.code;
				bsname = sjcode.name;
				
				if(bscode.Compare(bcode[0]) == 0)
				{
					basename = bsname;
					break;
				}
			
			}
		}
		//bkoba = CString(elwdata->bkoba);
		
		
		if(basename.Compare(str) == 0 && ktype == rdocheck)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
		else if(basename.Compare(str) == 0 && rdocheck.Compare("03") == 0)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = name;
		}
	}
	//speed
	m_gListsort = false;
	m_list1.SetItemCountEx(_vList.size());
	std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
		if (m_gListsort)
			return (item1.name.Compare(item2.name) > 0);
		return item2.name.Compare(item1.name) > 0;
	});
	m_list1.Invalidate();
//@speed
}

void CPage1::DataGubn(HTREEITEM hItem)
{
	CString		gubn;
	CString		data;
	ClearListitem();
	m_list1.DeleteAllItems();
		
	HTREEITEM hParentItem;
	
	hParentItem = m_tree.GetParentItem(hItem);
	
//	m_searchData.RemoveAll();
	m_list1.DeleteAllItems();

	gubn = m_tree.GetItemText(hParentItem);
	data = m_tree.GetItemText(hItem);

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

//	m_search = TRUE;

 }

void CPage1::DataReloadItem(CString data)
{
	_efopcode* efopcode{};
	_efopitem* efopitem{};

	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
//	int row = 0;

	for (auto& efopitem : m_ETFitem)
	{
				type = CString(efopitem->type, sizeof(efopitem->type));
		opcd = CString(efopitem->opcd, sizeof(efopitem->opcd));
		hnam = CString(efopitem->hnam, sizeof(efopitem->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		if(hnam.Compare(data) == 0)
		{
			data = opcd;
			break;
		}
	}

	for (auto& efopcode : m_ETFcode)
	{
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));

		if (data.Compare(opcd) == 0)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = hnam;
		}
	}
	
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
	
}

void CPage1::DataReloadTema(CString data)
{
	_efopcode* efopcode{};
	_efoptema* efoptema{};
	
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	//int row = 0;

	for (auto& efoptema : m_ETFtema)
	{
		type = CString(efoptema->type, sizeof(efoptema->type));
		tmcd = CString(efoptema->tmcd, sizeof(efoptema->tmcd));
		hnam = CString(efoptema->tmnm, sizeof(efoptema->tmnm));
		hnam.TrimLeft(); hnam.TrimRight();

		if (hnam.Compare(data) == 0)
		{
			data = tmcd;
			break;
		}
	}

	for (auto& efopcode : m_ETFcode)
	{
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));

		if (data.Compare(etfm) == 0)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = hnam;
		}
	}

	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed
}

void CPage1::DataReloadFore(CString data)
{
	_efopcode* efopcode{};
	_efopfore* efopfore{};
	
	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	//int row = 0;
	
	for (auto& efopfore : m_ETFfore)
	{
		type = CString(efopfore->type, sizeof(efopfore->type));
		frcd = CString(efopfore->frcd, sizeof(efopfore->frcd));
		frnm = CString(efopfore->frnm, sizeof(efopfore->frnm));
		frnm.TrimLeft(); frnm.TrimRight();

		if (frnm.Compare(data) == 0)
		{
			data = frcd;
		}
	}

	for (auto& efopcode : m_ETFcode)
	{
		type = CString(efopcode->type, sizeof(efopcode->type));
		code = CString(efopcode->code, sizeof(efopcode->code));
		hnam = CString(efopcode->hnam, sizeof(efopcode->hnam));
		hnam.TrimLeft(); hnam.TrimRight();

		opcd = CString(efopcode->opcd, sizeof(efopcode->opcd));
		etfm = CString(efopcode->etfm, sizeof(efopcode->etfm));
		etfl = CString(efopcode->etfl, sizeof(efopcode->etfl));

		if (data.Compare(etfl) == 0)
		{
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = code;
			item.name = hnam;
		}
	}
	
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
//@speed

}

void CPage1::OnDestroy() 
{
	CPropertyPage::OnDestroy();
		
	ClearListitem(); 
	ClearInterest(); 
	ClearList2item();
	
	m_ETFtema.clear();
	m_ETFitem.clear();		
	m_ETFfore.clear();
	m_ETFcode.clear();

	m_elwbase.RemoveAll();
	m_ELWdata.RemoveAll();
	m_arrayPBCode.RemoveAll();
}

void CPage1::OnRadioClick() 
{
	HTREEITEM hItem{};
	HTREEITEM hParent{};
	CString str;
	CString parent;

	hItem = m_tree.GetSelectedItem();
	hParent = m_tree.GetParentItem(hItem);

	str = m_tree.GetItemText(hItem);
	parent = m_tree.GetItemText(hParent);

	if(parent.Compare("전종목") == 0)
	{
		DataReloadELWAll(str);
		listsort(0);
	}
	else if(parent.Compare("기초자산종목") == 0)
	{
		DataReloadELWBase(str);
		listsort(0);
	}
	else if(parent.Compare("발행사") == 0)
	{
		DataReloadELWPB(str);
		listsort(0);
	}
}

void CPage1::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
	if(pLVKeyDow->wVKey == VK_RETURN)
		OnAdd();
	else if(pLVKeyDow->wVKey == VK_TAB)
		m_gname.SetFocus();
	*pResult = 0;
}

void CPage1::OnGetdispinfoList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM* pItem= &(pDispInfo)->item;
	
	if (_vList.size() > 0)
	{
		auto& rLabel = _vList.at(pItem->iItem);
		if (pItem->mask & LVIF_TEXT) //valid text buffer?
		{
			switch (pItem->iSubItem)
			{
			case 0:
				lstrcpy(pItem->pszText, rLabel.code);
				break;
			case 1:
				lstrcpy(pItem->pszText, rLabel.name);
				break;
			}
		}
	}
	*pResult = 0;
}

void CPage1::OnGetdispinfoList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM* pItem = &(pDispInfo)->item;

	if (m_inters.size() > 0)
	{
		auto& item = m_inters.at(pItem->iItem);
		if (pItem->mask & LVIF_TEXT) //valid text buffer?
		{
			switch (pItem->iSubItem)
			{
			case 0:
				if (item->code[0] != 'm')
					lstrcpy(pItem->pszText, item->code);
				break;
			case 1:
				lstrcpy(pItem->pszText, item->name.Trim());
				break;
			}
		}
	}
	*pResult = 0;
}


void CPage1::listsort(int column)
{
	switch (column)
	{
	case 0:
		std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
			if (m_gListsort)
				return (item1.code.Compare(item2.code) > 0);
			return item1.code.Compare(item2.code) < 0;
			});
		break;
	case 1:
		std::sort(_vList.begin(), _vList.end(), [&](const auto item1, const auto item2)->bool {
			if (m_gListsort)
				return (item1.name.Compare(item2.name) > 0);
			return item1.name.Compare(item2.name) < 0;
			});
		break;
	}

	m_list1.Invalidate();
}

void CPage1::listsort2(int column)
{
	switch (column)
	{
	case 0:
		std::sort(m_inters.begin(), m_inters.end(), [&](const auto item1, const auto item2)->bool {
			if (m_listsort)
				return (item1->code.Compare(item2->code) > 0);
			return item1->code.Compare(item2->code) < 0;
			});
		break;
	case 1:
		std::sort(m_inters.begin(), m_inters.end(), [&](const auto item1, const auto item2)->bool {
			if (m_listsort)
				return (item1->name.Compare(item2->name) > 0);
			return item1->name.Compare(item2->name) < 0;
			});
		break;
	}

	m_list2.Invalidate();
}

void  _intersx::copy(void* item)
{
	const auto tt = (struct _jinfo*)item;
	gubn = tt->gubn[0];
	code = CString(tt->code, 12).Trim();
	xprc = CString(tt->xprc, 10).Trim();
	xnum = CString(tt->xnum, 10).Trim();
	bookrgb = CString(tt->bookrgb, 2).Trim();
}

void CPage1::receiveOub(int key, CString data)
{
//	m_slog.Format("\r\n[interest][IB202201] -------------------------------------");
//	OutputDebugString(m_slog);
//	m_slog.Format("\r\n[interest][IB202201][%s]", __FUNCTION__, data.Left(22));
//	OutputDebugString(m_slog);

	switch (key)
	{
	case dnGROUPLIST:
		{
			int cnt = atoi(data.Left(4));
			data = data.Mid(4);
			const gsl::span<_gnewlistex> groupList((_gnewlistex*)data.GetString(), cnt);
			for_each(groupList.begin(), groupList.end(), [&](auto item) {
				const CString groupkey = CString(item.ngrs, 2).Trim();
				const int ii = atoi(groupkey) - 1;
				if (ii < 0)
					return;

				m_manageGroup[ii][0] = groupkey;
				m_manageGroup[ii][2] = CString(item.ogrs, 2).Trim();
				m_manageGroup[ii][3] = CString(item.gnam, 30).Trim();

				// m_slog.Format("\r\n[interest][IB202201] groupkey=[%s] item.ngrs=[%s] item.ogrs=[%s] item.gnam = [%s] ", 
				// 	groupkey, CString(item.ngrs, 2).Trim() , CString(item.ogrs, 2).Trim(), CString(item.gnam, 30).Trim());
				// OutputDebugString(m_slog);

			});		
			initCombo();
			m_activegno = 1; //test mod
			CWnd* wnd = GetParent()->GetParent();
			wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnGROUPCODE, 0), m_activegno);
		}
		break;
	case dnGROUPCODE:
		{
			m_inters.clear();
			const CString skey = data.Left(2);
			if (atoi(skey) < 0 || data.GetLength() < 20)
				return;

			const CString sGroupName = data.Mid(2, 20).Trim();
			const CString sCount = data.Mid(22, 4);			
			data = data.Mid(26);

			const gsl::span<_jinfo> codelist((_jinfo*)data.GetString(), min(atoi(sCount), 100));
			for_each(codelist.begin(), codelist.end(), [&](auto item) {
				auto inter = m_inters.emplace_back(std::make_shared<_intersx>());		
				inter->copy(&item);	
				//test mod
				if (inter->code[0] == 'm')
					inter->name = CString(item.xprc, sizeof(item.xprc) + sizeof(item.xnum)).Trim();
				else
					inter->name = GetCodeName(inter->code);

				if (inter->name.Find("0  ") == 0)
					inter->name.Empty();

				if (item.gubn[0] == ROW_BOOKMARK)
					inter->bookmark = '1';

				inter->bookrgb = CString(item.bookrgb, sizeof(item.bookrgb)).TrimRight();  //test IO
			});

			// for_each(m_inters.begin(), m_inters.end(), [&](auto item) {
			// 	m_slog.Format("\r\n[interest][IB202201] c=[%s] n=[%s] p = [%s] n=[%s] bookmark =[%c] rgb=[%s]",
			// 		item->code, item->name, item->xprc, item->xnum, item->bookmark, item->bookrgb);
			// 	OutputDebugString(m_slog);
			// 	});

			CString	fileBook = AxStd::FORMAT("%s/%s/%s/bookmark.i%s", m_root, USRDIR, m_name, skey);
			CFileFind find;

			if (find.FindFile(fileBook))
			{
				CFile	fileB(fileBook, CFile::modeRead);
				if (fileB.m_hFile != CFile::hFileNull)
				{
					CString bookbuffer;
					const size_t size = gsl::narrow<size_t>(fileB.GetLength());

					if (size == fileB.Read(bookbuffer.GetBuffer(size + 1), size))
					{
						bookbuffer.ReleaseBuffer();
						const gsl::span<_bookmarkinfo> booklist((_bookmarkinfo*)bookbuffer.GetString(), size / sizeof(_bookmarkinfo));

						int ii = 0;
						for (const auto& item : booklist)
						{
						//	if (item.code[0] == 'm')
						//	{
								if (ii < (int)m_inters.size())
								{
									auto inter = m_inters.at(ii);
									if (inter->code[0] == 'm')
									{
										inter->name = CString(item.name, 32).Trim();
										if (inter->name.Find("0  ") == 0)
											inter->name.Empty();
									}
									inter->bookmark = item.bookmark[0];
								}
						//	}
							ii++;
						}
					}
					fileB.Close();
				}
			}
			else
			{
				// m_slog.Format("\r\n[interest][IB202201] 파일없음 fileBook=[%s]", fileBook);
				// OutputDebugString(m_slog);
			}
			
			if (_vAddInter.size() > 0)
				m_inters.insert(m_inters.end(), _vAddInter.begin(), _vAddInter.end());

			if (m_inters.size() > 100)
				m_inters.resize(100);

			m_list2.SetItemCountEx(m_inters.size());
			m_list2.Invalidate();

			if (_vAddInter.size() > 0)
			{
				selcMark(&m_list2, m_inters.size() - 1);
				_vAddInter.clear();
			}
			
			updateList2sum();
		}
		break;
	default:
		break;
	}
}

BOOL CPage1::CheckBookFileProcess()
{
	CWnd* wnd = GetParent()->GetParent();
	BOOL bRet = wnd->SendMessage(TOmapWnd, MAKEWPARAM(getBOOLFILEPROCESS, 0), 0);
	return bRet;
}

int CPage1::xNxtList()
{
	ClearListitem();
	m_list1.DeleteAllItems();

	_shjcode   hjcode;
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{
		hjcode = m_hjcode.GetAt(ii);

		if (hjcode.nxtf == 'Y')
		{	
			auto& item = _vList.emplace_back(std::move(CITEMS()));
			item.code = CString(hjcode.code, HCodeLen);
			item.name = CString(hjcode.name, HNameLen);
		}
	}
	//speed
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	//@speed
	return 0;
}


bool CPage1::isInitialConsonant(WORD ch, WORD &start, WORD &end)	// 초성
{
	if (0xA4A1 > ch || ch > 0xA4BE)
		return false;


	const WORD	aInitial[19] =
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
		0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
		0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};
	const WORD	aStart[19] =
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
		0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
		0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	const WORD	aEnd[19] =
	{
		0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
		0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
		0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};

	int ii = 0;
	for (ii = 0; ii < 19; ii++)
	{
		if (ch == aInitial[ii])
		{
			start = aStart[ii];
			end   = aEnd[ii];
			return true;
		}			
	}

	start = WORD(aStart[ii] - 1);
	end   = WORD(aEnd[ii] + 1);
	return true;
}

WORD CPage1::makeMultiByte(char ch1, char ch2)	// make multibyte with 2 bytes
{
	return WORD((ch1 << 8) | (0x00FF & ch2));
}

std::pair<bool, int> CPage1::findFirst(const CString& sFind, CString& sName)
{
	int lenFind = sFind.GetLength();
	int lenName = sName.GetLength();

	if (lenFind == 0 || lenName < lenFind)
		return std::make_pair(false, 0);
	
	for (int offset = 0; offset <= lenName - lenFind; sName[offset]&0x80 ? offset+=2 : offset++)
	{
		bool matched = true;
		int find = 0;
		while (find < lenFind)
		{
			// 한글 2바이트
			if (sFind[find] & 0x80 && sName[offset + find] & 0x80 && sName[offset + find + 1] & 0x80)
			{
				//offset++;
				if (find + 1 >= lenFind || offset + find + 1 >= lenName) {
					matched = false;
					break;
				}

				WORD input = makeMultiByte(sFind.GetAt(find), sFind.GetAt(find + 1));
				WORD start = 0, end = 0;
				if (isInitialConsonant(input, start, end))
				{
					WORD wname = makeMultiByte(sName.GetAt(offset + find), sName.GetAt(offset + find + 1));
					if (!IS_WITHIN(start, end, wname))
					{
						matched = false;
						break;
					}
				}
				else
				{
					WORD wfind = input;
					WORD wname = makeMultiByte(sName.GetAt(offset + find), sName.GetAt(offset + find + 1));
					if (wfind != wname)
					{
						matched = false;
						break;
					}
				}
				find += 2;
			}
			else // 한글이 아니면 1바이트 비교
			{
				// 영문, 숫자, 특수문자`
				if (sFind[find] != sName[offset + find])
				{
					matched = false;
					break;
				}
				find += 1;
			}
		}
		if (matched)
			return std::make_pair(true, offset);
	}
	return std::make_pair(false, 0);
}

bool CPage1::SearchData(int kind, CString sName, bool bAddAll)
{
	if (sName.IsEmpty())
	{
		if (!bAddAll)
		{
			m_list1.DeleteAllItems();
			_vList.clear();


			_vList = _vListCopy;

			//speed
			m_list1.SetItemCountEx(_vList.size());
			m_list1.Invalidate();
			//@speed
		}
		return true;
	}
	_vList.clear();
	
	std::vector<CITEMS> vSearch, vSame, vOther, vExName, vFirst;
	CString name;
	if (sName.GetAt(0) & 0x80000000)	// 한글로 시작하는...
	{	
		for_each(_vListCopy.begin(), _vListCopy.end(), [&](const auto& hjc){
			name = hjc.name;
			name.TrimRight();
			const int result = name.Find(sName);
			const int sLen = (sName.GetLength() / 2) * 2;
			CString nameSub = name.Left(sLen);
			CString code = hjc.code;
			//code.Trim();
			//if (code.GetLength() > 6)
			//	code = code.Mid(1);
			sName.MakeUpper();
			// 초성검색

			const auto&[bflag, index] = findFirst(sName, name.Trim());

			if (bflag && index == 0)
				vFirst.push_back(hjc);
			else if (bflag)
				vSearch.push_back(hjc);	
			else if (0 == result)
			{
			 	vSearch.push_back(hjc);
			}
			else if (sName.CompareNoCase(nameSub) == 0)
			{
			 	vSame.push_back(hjc);
			}
			else if (_mapExCode.find(code.GetString()) != _mapExCode.end())
			{
				CString exname = _mapExCode.at(code.GetString()).c_str();
				vector<CString> vEx;
				AxStd::SplitSub(vEx, exname, "|");
				for (auto& ex : vEx)
				{
					ex.Trim();
					const auto&[bflagx, xindex] = findFirst(sName, ex);
					if (bflagx)
					{
						vExName.push_back(hjc);	
						break;
					}
					else if (sName.Find(ex) > 0)
					{
						vExName.push_back(hjc);
						break;
					}
				}	

			}
			else if (result > 0)
			{	
				vOther.push_back(hjc);
			}
		});

		std::sort(vFirst.begin(), vFirst.end(), [&](const auto item1, const auto item2)->bool {
			return item1.name.Compare(item2.name) < 0;
		});


		vSearch.insert(vSearch.begin(), vFirst.begin(), vFirst.end());
		vSearch.insert(vSearch.end(), vSame.begin(), vSame.end());
		vSearch.insert(vSearch.end(), vExName.begin(), vExName.end());
		vSearch.insert(vSearch.end(), vOther.begin(), vOther.end());
	}
	else	// 종목코드순 후에 영문명순
	{
	 	sName.MakeUpper();
	 	for_each(_vListCopy.begin(), _vListCopy.end(), [&](const auto& hjc){
			name = hjc.name;
			name.TrimRight();
	 		const int result = name.Find(sName);
			CString code = hjc.code;
			code.Trim();
			//if (code.GetLength() > 6)
			//	code = code.Mid(1);

	 		if (0 == result)
	 			vSearch.push_back(hjc);
			else if (result >= 0)
	 			vOther.push_back(hjc);	
			else if (_mapExCode.find(code.GetString()) != _mapExCode.end())
			{
				CString exname = _mapExCode.at(code.GetString()).c_str();
				//sName.MakeUpper()
				
				vector<CString> vEx;
				AxStd::SplitSub(vEx, exname, "|");
				for (auto& ex : vEx)
				{
					ex.Trim();
					ex.MakeUpper();
					const int result = ex.Find(sName);

					if (sName.CompareNoCase(ex) == 0)
					{
						vExName.push_back(hjc);
						break;
					}
					else if (result >= 0)
					{
						vExName.push_back(hjc);
						break;
					}
				}
			}
	 		
	 	});
		vSearch.insert(vSearch.end(), vExName.begin(), vExName.end());
	 	vSearch.insert(vSearch.end(), vOther.begin(), vOther.end());
	}

	_vList = vSearch;
	m_list1.SetItemCountEx(_vList.size());
	m_list1.Invalidate();
	return false;


}