#pragma once
// Page1.h : header file
//
#include "afxtempl.h"
#include "editx.h"
#include "../../h/jmcode.h"
#include "../../h/interst.h"
#include "BmpButton.h"

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	char	ecng{};		// ECN 구분
	char	size{};		// 자본금규모	2:대, 3:중, 4:소
	char     acdl[6];       //업종대분류
	char	ucdm[6];		// 업종중분류 
	char	ucds[6];		// 업종소분류
	char	jjug{};		// 제조업 구분(27: 제조업)
	char	kpgb{};		// KOSPI200 구분
	char	kosd{};		// 종목구분
	char	ssgb{};		// 소속구분
	char	ucmd{};		// 0:보통주, 5:우선주
	char	wsgb{};		// 0:일반 1:지배구조우수기업
	char	jsiz{};		// 업종자본금 대중소
	char	itgb{};		// KOSPI-IT/KQ-IT50구분(1:채택)
	char	star{};		// KOSDAQ 스타지수
	char	prmr{};		// 프리미어지수
	char	unio{};		// 통합지수 (KRX100)
	char    nxtf{};		// nxt
};

struct _sjcode			// semi code
{
	CString code;
	CString name;
};

struct _listitem {		// tree1 선택시 보여지는 list1 item
	char	code[12]{};
	char	name[36]{};
	_inters* pInter{};
};

struct _listitem2 {		// tree1 선택시 보여지는 list1 item
	char	code[12]{};
	char	name[36]{};
};

struct _elwdata
{
	CString		code;	// ELW 종목코드
	CString		name;	// 종목명
	CString		ktype;	// 권리유형 01:Call, 02:Put, 03:기타
	CString		pcode;	// 발행사 코드
	CString		pname;	// 발행사명
	CString		mjmt;	// 만기월 (YYYYMMDD)
	CString		bcode[5];
	BOOL		bkoba{};		// 조기종료ELW여부
};



struct _efopcode
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	code[6]{};			/*종목코드*/
	char	hnam[39]{};			/*종목명*/
	char	opcd[6]{};			/*운용사 코드*/
	char	etfm[1]{};			/*테마코드*/
	char	etfl[1]{};			/*국내외 구분*/
	char	lnfd[1]{};
};

struct _efopitem
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	opcd[6]{};			/*운용사 코드*/
	char	hnam[30]{};			/*운용사 한글약명*/
	char	lnfd[1]{};			/*라인피드*/
	
	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	tmcd[1]{};			/*테마코드*/
	char	tmnm[20]{};			/*테마명*/
	char	lnfd[1]{};			/*라인피드*/
};

struct _efopfore
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	frcd[1]{};			/*국내외 구분('D':국내, 'F':해외)*/
	char	frnm[10]{};			/*국내외 구분명*/	//2015.11.18 KSJ 자릿수 4 -> 10
	char	lnfd[1]{};			/*라인피드*/
};

typedef struct _pbcode
{
	CString		code;	// 발행사 코드
	CString		name;	// 발행사명
}PBCODE;


typedef struct _sortStr
{
	CString code;
	CString name;
}SORTSTR;

typedef struct listitem
{
	CString code;
	CString name;
	int bkoba{};
	CString ktype;
}ITEMS;

typedef struct codelistitem
{
	CString code;
	CString name;
}CITEMS;

//추가
template<class T> 
void QSortCArray( T& t, int (__cdecl *compare )(const void *elem1, const void *elem2 ) ) 
{ 	
    if( t.GetSize() <= 0 ) return; 
    qsort( t.GetData(), t.GetSize(), sizeof( t[0] ), compare ); 	
} 

// int 자료형비교함수 
int CompareChar( const void *elem1, const void *elem2 );
#define	WM_TOPAGE1	(WM_USER + 119)
#define	WM_TOPAGE2	(WM_USER + 139)
#define WM_TOSELECTPAGE (WM_USER + 129)
#define WM_TOSELECTGROUPNCODE (WM_USER + 149)
#define MAXGROUP	100

static BOOL m_gListsort;
/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog

class CPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1)

// Construction
public:
	CPage1(CWnd* parent = nullptr);
	~CPage1();

// Dialog Data
	//{{AFX_DATA(CPage1)
	enum { IDD = IDD_PAGE01 };
	CButton	m_btnMoveBtm;
	CButton	m_btnMoveTop;
	CButton	m_btn_DelAll;
	CButton	m_btn_ShareImport;
	CButton	m_btn_ShareExport;
	CButton	m_btn_newGroup;
	CButton	m_btn_MoveUp;
	CButton	m_btn_MoveDn;
	CButton	m_btn_Blank;
	CButton	m_btn_AllSave;
	CButton	m_btn_AddAll;
	CButton	m_btn_Add;
	CButton	m_editGroup;
	CButton	m_btn_dnLoadR;
	CButton	m_btn_dnLoad;
	CButton	m_btn_Del;
	CButton	m_btnSort22;
	CButton	m_btnSort21;
	CButton	m_btnSort20;
	CButton	m_btnSort19;
	CButton	m_btnSort18;
	CButton	m_btnSort17;
	CButton	m_btnSort16;
	CButton	m_btnSort15;
	CButton	m_btnSort14;
	CButton	m_btnSort13;
	CButton	m_btnSort12;
	CButton	m_btnSort11;
	CButton	m_btnSort10;
	CButton	m_btnSort09;
	CButton	m_btnSort08;
	CButton	m_btnSort07;
	CButton	m_btnSort06;
	CButton	m_btnSort05;
	CButton	m_btnSort04;
	CButton	m_btnSort03;
	CButton	m_btnSort02;
	CButton	m_btnSort01;
	CComboBox	m_gname;
	CStatic		m_sum;
	CComboBox	m_cbSearch;
	CXEditPrompt	m_search;
	CTreeCtrl	m_tree;
	CListCtrl	m_list2;
	CListCtrl	m_list1;
	CButton	m_Radio_ELWAll;
	CButton	m_Radio_ELWCall;
	CButton	m_Radio_ELWPut;
	//}}AFX_DATA
public:
	class CPage4*	m_page{};
	void SetOther(class CPage4*	page);
	int GetActiveGroup();
	void SetArray(CArray<struct _inters*, struct _inters*>& arInters);

private:
	BOOL	m_bCol0Asc{};
	BOOL	m_bCol1Asc{};
	int		m_nPrevCol{};

	int		m_nCol0Asc{};
	int		m_nCol1Asc{};
	int		m_nPrevList2{};
	bool		isClickServerDN{};
public:
	CWnd*		m_parent{};
	CString		m_root;
	CString		m_name;
	bool		m_activePage{};

	bool		m_bIsEdit{}; 	//수정했는지 여부
	bool		m_autoScroll{};

	struct	_dragdata {		// drag item struct
		int	list{};
		int	item{};
		CString	code;
		CString	name;
	};

	_dragdata	m_dragData;
	std::shared_ptr<CImageList>	m_dragImage;
	int		m_activegno{};

	CArray <_shjcode, _shjcode>	m_hjcode;		// 현물종목
	CArray <_sjcode, _sjcode>	m_fjcode;		// 선물종목
	CArray <ojcode, ojcode>		m_ojcode;		// 옵션종목
	CArray <pjcode, pjcode>		m_pjcode;		// 현물옵션
	CArray <upcode, upcode>		m_upcode;		// 업종코드
	CArray <_sjcode, _sjcode>	m_elwbase;		// elwbase

	CArray <ccode, ccode>		m_cicode;		// 2012.10.04 KSJ 금리	
	CArray <ccode, ccode>		m_cccode;		// 2012.10.04 KSJ 통화	
	CArray <sfcode, sfcode>		m_sfcode;		// 2012.10.04 KSJ 주식선물	

	CArray <_efopitem*,_efopitem*> m_ETFitem;
	CArray <_efoptema*,_efoptema*> m_ETFtema;
	CArray <_efopfore*,_efopfore*> m_ETFfore;
	CArray <_efopcode*,_efopcode*> m_ETFcode;

	CArray <_listitem*, _listitem*> m_listitem;		// m_list1 item
	CArray <_listitem*, _listitem*> m_list2item;		// m_list2 item
	CArray <_inters*,   _inters*>   m_inters;		// file_load	

	// CArray<ITEMS, ITEMS> m_arList;
	CArray<CITEMS, CITEMS> m_arList;
	
	CMapStringToPtr m_listdata;

	CArray <_elwdata, _elwdata> m_ELWdata;
	CArray<PBCODE, PBCODE> m_arrayPBCode;


	CString		m_appendGI;
	int		m_selectGroup{};

	CString		m_strGroupNumber;
	bool		m_isDuplicateCode{};		//종목 중복 등록 여부

	CString		m_manageGroup[MAXGROUP][4];	// 0 : 수정전 시퀀스 
																			// 1 : M 그룹명 수정 D : 삭제 N : 그룹 추가
																			// 2 : 수정된 시퀀스 
																			// 3 : 그룹명

											// 관심 백업('b') 원복('R') 시퀀스변경 S 

	bool		m_isChangeSequence{};			// 그룹 순서 변경하였는지

	bool		m_bFirstDuplicateNO{};		// 2012.02.13 KSJ 처음 중복여부 물어봤을때 다시는 묻지않기 위해서.
	int			m_nResult{};					// 2012.04.02 KSJ 처음 중복여부 물어봤을때의 결과값 저장.
	char     m_pdata[1024 * 4]{};

	CString m_slog;
	BOOL CheckBookFileProcess();
	void ClearSearch();
	CBmpButton* m_btnClear{};  //0422
	void SelectTree(int selItem);
public:
	void	makingBackupFile();
	void	recoveryFile();
	void	deleteBackupFile();

	BOOL	endSendAction();

	void	sortManageGroup(int drag, int drop);

	bool	checkChangeSequence();
	int		getManageCount();
	
	CString	getManageGroupdata(int row, int col);
	void	setManageGroup(char* datB);
	
	bool	GetIsEdit() { return m_bIsEdit; }			//수정여부 
	void    SetSize();
	void	initList();
	void	initTree();
	void	ETFInitialTree(HTREEITEM hRoot);
	void	initCombo(bool isEditOk = false);

	void	resettingGroup();

	void	OnEndDrag(CPoint point);
	
	void	selectComboBox();

	void	SetItemDataUJ(HTREEITEM hItem, int gubn);
	void	SetItemDataX(HTREEITEM hItem, int gubn);
	void	ClearListitem();
	void	ClearList2item();
	void	ClearInterest();
	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);
	void	insertList1toList2(int sitem = -1);
	void	deleteList2();
	void	searchAction(int column);
	int		findMatchIndex(CString text);
	int		findMatchIndex(CString text, int column);
	void	updateList2sum();
	std::shared_ptr<CImageList> CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl);
	void	SetAutoScroll(CListCtrl* listctrl, int direct);

	BOOL	AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	int		AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem = -1);
	bool	ExistFile(int gno, bool temp = false);
	
	void	loadConfig();
	void	loadingHJcode();
	void	loadingFJcode();
	void	loadingOJcode();
	void	loadingPJcode();
	void	loadingUPcode();
	void	loadingETFcode();
	void	loadingELWcode();

	int		loadingInterest(int gno);

	void	loadingCFcode(bool bCurrency);	//2012.10.04 KSJ 금리(6x), 통화(7x) 통화면 true
	void	loadingSFcode();	//2012.10.04 KSJ 주식선물

//	void	saveConfig();
	void	DataGubn(HTREEITEM hItem);	
	void	DataReloadItem(CString data);
	void	DataReloadTema(CString data);
	void	DataReloadFore(CString data);

	void	ETFInitialList();


	void	savingInterest(int gno);
	void	savingGroupFile(int gno, CString gname);
	bool	savingGroupOrder(CString gname);

	int	xAllCodeToList();
	int	xKospiToList();
	int	xKosdaqToList();
	int	xKospi200ToList();
	int	xKosdaq50ToList();
	int	xKRX100ToList();
	int	xFreeCodeToList();
	int	xELWCodeToList();
	int	xELWBaseToList();
	int	xETFCodeToList();
	int	xSinjuCodeToList();
	int	xFutureToList();
	int	xCallCodeToList();
	int	xPutCodeToList();
	int	xKospiJisuToList();
	int	xKosdaqJisuToList();
	int	xPjcodeToList(CString code, char kind);
	int	xKospiUpToList(int selItem);
	int	xKosdaqUpToList(int selItem, CString sData);
	int	xKospiExToList(int selItem);
	int	xKosdaqExToList(int selItem);
	int	xThemaCodeToList(int selItem);
	int	xGroupCodeToList(int selItem);
	int	xInterestToList(int selItem);
	int	xKonexToList();	//2013.06.25 코넥스 추가
	int	xETNCodeToList();	//2014.11.06 ENT 추가
	int xSPACCodeToList();	//2015.11.03 spac 추가
	int xRITSCodeToList();
	int xNewCodeToList(int igubn = 0);  //0422 신종증권
	int	xCFutureToList(bool bCurrency);	//2012.10.04 KSJ 금리/통화 통화일때는 true
	int	xSFutureToList();	//2012.10.04 KSJ 주식선물
	int	xGoodStockToList();     //25.06.13 상장형 주식추가
	void	OKaction();
	void	OKactionOne(int gnum);
	void	CCaction();
	int     xNxtList();

	int		GetUploadData(int gno, CString& name, char* datB);
	CString	GetTempData(int gno, CString& name, int& count);

	int		SetDnloadData(int gno, CString gname, int count, char* datB);
	void	reInitInterest();
	void	clearAllFile();
	void	enableButton(BOOL enable);
	int		GetCodeType(CString code);
	CString	GetCodeName(CString code);

	CString	GetGroupORder();
	CString	GetGroupORder2(CString& data);

	void	EnableButton(BOOL bEnable);

	void	SetListColumnText(CListCtrl* pList, int nCol, int nAsc);

	//ELW 추가 모듈
	void	ELWIssueDnload(int selItem);
	void	ELWAssetDnload(int selItem, CString item);
	void	parsingELWAssetDN(char* datB, int datL);
	void	parsingELWIssueDN(char* datB, int datL);
	void	parsingRecommandDN(char* datB, int datL);
	int		FindPBCode(CString code);

	void	DataReloadELWPB(CString str);	
	void	DataReloadELWBase(CString str);
	void	DataReloadELWAll(CString str);


	void	ExactGroupData();
	void	tempSaveGroupIndex();		//임시로 그룹 INDEX 저장
	
	int		SaveGroup(); 
	void	xRecommandtoList();

	int		GetGroupCount();
	CString	Variant(int comm, CString data);
	void	OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	void	OnClickELWTree(NMHDR* pNMHDR, LRESULT* pResult);	

	void SendMsgToPage(int igubn, CString sData = "");
	void CheckPage4();
protected:
	void	AppendItemToList1(int ipos, CString code, CString name, _listitem *pListItem = nullptr);
	void	AppendItemToList1(int ipos, CString code, CString name, CString bookmark, _listitem *pListItem = nullptr);
	void	AppendItemToList2(int ipos, int gubn, CString code, CString name, CString bookmark = "", bool add = true);
	void	AppendItemToList2(int ipos, int gubn, CString code, CString name, CString xprc, CString xnum, CString xupnum, CString filter, CString bookmark = "", bool add = true);
	void	DeleteItemToList2(int ipos);
	bool	existCodeInList2(CString code);
	void	InitEdit();

	void	selectKospi();		//주식 전종목 선택하기
	void	selectElw();		//ELW 전종목 선택하기
	void	analyzeELWCode();	//코드가 ELW인지 주식인지 판단

	CString	GetGroupName(int gno);
	void	GuideMsg(CString msg);
	bool	GuideMsg(CString msg, bool question);
	CString	parseX(CString &srcstr, CString substr);
	
	CString	idTOstring(UINT id);

	// new added
	bool	m_bAddAll;		// 숫자입력시 매번 클리어 하는것 방지하기 위해
	bool	SearchData(int kind, CString sName, bool bAddAll);
	bool	SearchJongmok(CString sName, bool bAddAll);
	bool	SearchWord(CString sName, bool bAddAll);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);
	WORD	MakeHangul(char cChar1, char cChar2);

	void	ExceptionMsg();

	
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool IsWin8();
	// Generated message map functions
	//{{AFX_MSG(CPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnAddall();
	afx_msg void OnDelall();
	afx_msg void OnBlank();
	afx_msg void OnMoveup();
	afx_msg void OnMovedn();
	afx_msg void OnNewgroup();
	afx_msg void OnUpload();
	afx_msg void OnDnload();
	afx_msg void OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnAllsave();
	afx_msg void OnSelchangeGname();
	afx_msg void OnSelchangeComboSearch();
	afx_msg void OnEditgroup();
	afx_msg void OnCnvinterest();
	afx_msg void OnDnloadr();
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShareimport();
	afx_msg void OnShareexport();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMovetop();
	afx_msg void OnMovebottom();
	afx_msg void OnRadioClick();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	afx_msg long OnAppendGroup(WPARAM wParam, LPARAM lParam);
	afx_msg long OnAppendParentGroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectGroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelectGroupNCode(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickBtnSort(UINT nID);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnGetdispinfoList1(NMHDR* pNMHDR, LRESULT* pResult);

private:

	void loadingnameex();
	bool isInitialConsonant(WORD ch, WORD &start, WORD &end);
	WORD makeMultiByte(char ch1, char ch2);	// make multibyte with 2 bytes
	std::pair<bool, int> findFirst(const CString& sFind, CString& sName);
	std::unordered_map<std::string, std::string> _mapExCode;
};

