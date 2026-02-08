#pragma once

// JongmukDlg.h : header file
//

#include "resource.h"
#include "user.h"
#include "btnst.h"
#include "sfont.h"
#include "skinlistctrl.h"
#include "EditX.h"
#include "../../h/jmcode.h"
#include "axiscode.h"
#include <afxtempl.h>
//#include "BmpButton.h"
#include "AxTree.h"

#define	ALLTYPE	       	0
#define KOSPITYPE	1
#define KOSDAQTYPE     -1
#define	ELWTYPE		2
#define REITSTYPE	4
#define OTHERTYPE	3
#define KONEXTYPE	5	//2013.06.05 KSJ 코넥스 추가

#define	USCODE_FILE	"uscode.dat"
struct  _USCode {			/* 해외 SYMBOL 정보             */
        char    symb[16];               /* 종목 SYMBOL                  */
        char    dat1;                   /* 대분류			*/
					/* 0: 지수      1: 종목		*/
					/* 2: 선물      3: 금리		*/
					/* 4: 국채      5: 환율		*/
					/* 7: others			*/
        char    dat2;                   /* 중분류(Combo Box)            */
					/* code09.jsgb                  */
					/* [지수]                       */
					/* 1. 미국 2.아시아 3.유럽 4. MSCI 5. 기타 */
					/* [종목]                       */
					/* 1. 미국 2.일본 3.홍콩 4.대만 5.영국 6.프랑스 7.독일 8.ADR 9.GDR */
					/* [선물]                       */
					/* 1.CME지수선물 2.에너지 3.상품선물	*/
					/* [금리]                       */
					/* 1.리보금리   2.미국금리	*/
					/* [국채]                       */
					/* 1.미국 2.일본 3.영국 4.독일	*/
					/* [환율]                       */
					/* 국제환율                     */
        char    knam[40];               /* 한글 SYMBOL명                */
        char    enam[40];               /* 영문 SYMBOL명                */
        char    icod[6];                /* 종목 업종코드                */
        char    ncod[3];                /* 국가코드                     */
        char    excd[4];                /* 거래소코드                   */
};

typedef struct flist
{
	CString code;
	CString name;
} FLIST;


static BOOL m_gListsort;

/////////////////////////////////////////////////////////////////////////////
// CJongmukDlg dialog

class CJongmukDlg : public CDialog
{
// Construction
public:
	void ListReitsSort(int nItem);
	CJongmukDlg(int kind, int type, CPoint pt = CPoint(-1, -1), CWnd* pParent = nullptr);   // standard constructor

	void	DeleteAllItem();
	void	Filllistoption(int sel);
	void	Hideall(int flag);
	void	InitOption();
	
	void	Set_KS_HanMode();
	bool	IsCodeType(CString code, int type);
	void	SetDlgPos(int x, int y);
	void	SetListCtrl();
	void	DeleteAllData();
	CString GetCode() { return m_szCode; }
	CString GetName() { return m_szName; }
	int     GetJongmuk() {	return m_jongmuk; }
	void	SetCode(CString szCode){ m_szCode = szCode; }
	void	SetName(CString szName){ m_szName = szName; }
	BOOL	FindCode(int kind, CString szCode, int type, bool bNoDelete = false);
	BOOL	LoadUPCode();
	BOOL	LoadELWCode();
	void	SelectETF();
	void	ApplyELW(CString code);
	void	ApplyELWBase(CString code);
	hjcodex* FindHJCode(CString sCode);
	void	ListRemoveAll(bool select = true);
	void	InitBtn();
	BOOL	LoadUSCode();
	BOOL	loadFjCode();
	BOOL	loadOjCode();
	bool    loadNameEx();

	//CString	CompareQuote(CString szCode);
	CString	CompareFuture(CString szCode);
	void	clearEditSearch();
	bool	CheckDigit(CString code);
	hjcodex* FindOnlyELW(CString sCode);
	int	FindPBCode(CString code);
	CString FindBaseCode(CString company);
	CString FindPBcompany(CString company);
	void	ELWKospi200();
	void	ElwLoadGen();
	void	ElwLoadJogi();

	void	ETFLoadItem(CString code);
	void	ETFLoadTema(CString tmcd);
	void	ETFLoadFore(CString frcd);

	CString	FindETFItem(CString company);
	CString FindETFTema(CString tema);
	CString FindETFFore(CString fore);

	void	InitialETFTree(xnode& node);
	void 	ClearSearch();//0422

	// Dialog Data
	//{{AFX_DATA(CJongmukDlg)
	enum { IDD = IDD_DIALOG_STOCKCODEINFO };
	CButtonST	m_ctlButtonKONEXWORK;
	CButtonST	m_ctlBtnPremier;
	CComboBox	m_ComboSearchKind;
	CButtonST	m_BtnHangul22;
	CButtonST	m_BtnHangul21;
	CButtonST	m_BtnHangul20;
	CButtonST	m_BtnHangul9;
	CButtonST	m_BtnHangul8;
	CButtonST	m_BtnHangul7;
	CButtonST	m_BtnHangul6;
	CButtonST	m_BtnHangul5;
	CButtonST	m_BtnHangul4;
	CButtonST	m_BtnHangul3;
	CButtonST	m_BtnHangul2;
	CButtonST	m_BtnHangul14;
	CButtonST	m_BtnHangul13;
	CButtonST	m_BtnHangul12;
	CButtonST	m_BtnHangul11;
	CButtonST	m_BtnHangul10;
	CButtonST	m_BtnHangul1;

	CButtonST	m_BtnHangul16; //0422
	CButtonST	m_BtnHangul15;
	CButtonST	m_BtnHangul19;
	CButtonST	m_BtnHangul18;
	CButtonST	m_BtnHangul17;


	CXEditPrompt	m_EditSearch;

	CButtonST	m_CtlButtonThirdJS;
	CButtonST	m_CtlButtonSector;
	AxTree		m_Tree;
	CButtonST	m_ctlButtonELW;
	CButtonST	m_ctlButtonSINJU;
	CListBox	m_ListPut;
	CListBox	m_ListCall;
	CButtonST	m_CtlButFuture4;
	CButtonST	m_CtlButFuture3;
	CButtonST	m_CtlButFuture2;
	CButtonST	m_CtlButFuture1;
	CSkinListCtrl	m_ListCtrl;
	CButtonST	m_ctlButtonTHIRD;
	CButtonST	m_ctlButtonKOSPIWORK;
	CButtonST	m_ctlButtonKOSPI;
	CButtonST	m_ctlButtonKOSDAQWORK;
	CButtonST	m_ctlButtonKOSDAQ;
	CButtonST	m_ctlButtonECN;
	CButtonST	m_ctlButtonAll;
	CComboBox	m_ComboGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJongmukDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	BtnSetup();
	void	Part();	

	void	ListKosDaqSort(int nItem);
	void	ListKospiSort(int nItem);
	BOOL	ListAllWorkSort(int nItem);
	void	AddGrid(CString& str, int flag);
	void	AddCombo(CString& szText);
	void	HideButton(BOOL flag = FALSE);
	void	MoveGrid(int kind = 1);
	BOOL	CodeListAll();
	BOOL	CodeListSome(vector<HCODEX*>& arr, bool bflag = true);
	void	AddItem(int row, CString szCode, CString szName);
	void    InsertItem(int row, CString szCode, CString szName);
	CString Parser(CString &srcstr, CString substr);
	bool	FileCheck(int fNo);
	void	GetExCode();
	void	ShowTreeView();

	void	LoadFolder(CString path, xnode& node, CString sSection, int nKey);
//	void	LoadGroup(CString path, HTREEITEM hItem);
	void	baseCX_Category();

	CString LoadCodeList(int nGroup);
	//void	ListUpjongSort(int nidx, hjcode* hjc, int ntype,CPtrArray& arr);
	void	ProcPart(int ntype,int nSubType);

	CAxisCodeApp* m_pApp;

	int	m_nRadioOption;	
	CSFont	m_Font;
	int	m_select;

	enum ELW_FILTER { ewALL, ewNormal, ewKoba };
	ELW_FILTER m_ewFilter;

	char*	m_fcB;
	char*	m_ocB;

	std::map<CString, struct hjcodex*> _mapCODE;
	
	CPtrArray m_arFCODE;
	CStringArray m_arMONTH;
	CPtrArray m_arOCODE;
	CPtrArray m_arUPCODE;
	//CArray	<CStringArray*, CStringArray*>	m_codeArr;
	std::vector<std::vector<CString>> m_codeArr;
	CArray<_elwcode, _elwcode>   m_arrayELWCode;
	CArray<_elwcode, _elwcode>   m_arrayELWGenCode;
	CArray<_elwcode, _elwcode>   m_arrayELWJogiCode;
	CArray<PBCODE, PBCODE>       m_arrayPBCode;
	std::vector<_basecode>	     m_arrayBASECode;


	CString	m_szCode;
	CString	m_szName;
	int	m_jongmuk;
	int	m_kind;
	int	m_Type;
	HBRUSH	m_hBrush;
	CPoint	m_point;
	BOOL	m_bOption;
	
	std::vector <std::shared_ptr<_USCode>> m_arUSCODE;

	CString	m_root, m_user;
	
	struct _exCode {
		CString code;
		CString name;
		CString nxt;
	};
	CArray <_exCode, _exCode> m_exCode[2];
	
	// Generated message map functions
	//{{AFX_MSG(CJongmukDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonKospiwork();
	afx_msg void OnButtonKosdaqwork();
	afx_msg void OnButtonOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnButtonFuture1();
	afx_msg void OnButtonFuture2();
	afx_msg void OnButtonFuture3();
	afx_msg void OnButtonFuture4();
	afx_msg void OnDoubleclickedButtonFuture1();
	afx_msg void OnDoubleclickedButtonFuture2();
	afx_msg void OnDoubleclickedButtonFuture3();
	afx_msg void OnDoubleclickedButtonFuture4();
	afx_msg void OnDblclkListCall();
	afx_msg void OnDblclkListPut();

	afx_msg void OnDblclkTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSector();
	afx_msg void OnButtonThirdjs();
	afx_msg void OnSelchangeComboGroup();
	afx_msg void OnSelchangeStaticSearch();
	afx_msg void OnButtonPremier();
	afx_msg void OnButtonKonexwork();
	afx_msg void OnGetdispinfoListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio(UINT nID);
	afx_msg void OnClickBtnSort(UINT nID);
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheckNXT();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	// added by kwon
	bool	m_bAddAll;	// ListCtrl의 중복 클리어를 방지하기 위해
	void	searchAction(int column);
	int	findMatchIndex(CString text, int column);
	bool	SearchData(int kind, CString sName, bool bAddAll);
	bool	SearchJongmok(CString sName, bool bAddAll);
	bool	SearchWord(CString sName, bool bAddAll);
	void    listsort(int column);

	std::pair<bool, int> 	findFirst(const CString& sFind, CString& sName);
	WORD 	makeMultiByte(char ch1, char ch2);	// make multibyte with 2 bytes
	bool 	isInitialConsonant(WORD ch, WORD &start, WORD &end);	// 초성

	std::vector<HCODEX*> _vSCODE;
	std::vector<std::tuple<CString, CString, CString>> _vCODE;
	
	// nxt만 
	std::vector<std::tuple<CString, CString, CString>> _vNCODE;
	std::unordered_map<std::string, std::string> _mapExCode;
	bool    _bNxt = false;
	CString _sData;
	CString _userfile;

	void SetTree();
	CString m_slog{};
	void InitClearBtn();
public:
	//CButton m_groupGubn;
};

