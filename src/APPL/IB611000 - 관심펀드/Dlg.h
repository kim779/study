#pragma once
// Dlg.h : header file
//
#include "../../h/axisvar.h"
#include <afxtempl.h>
#include "../../h/jmcode.h"
#include "sharemsg.h"
#include "ioformat.h"
#include "EditX.h"

/////////////////////////////////////////////////////////////////////////////
// CDlg dialog
#define FUNDSEARCH		1
#define FUNDNEWGROUP	2
#define FUNDINIT		3
#define FUNDCREATEGROUP 4
#define FUNDLIST		5
#define FUNDDELETEGROUP 6
#define FUNDBACKUP		7
#define	FUNDRESTORE		8
#define FUNDUPDATEGROUP 9

#define DELETEGROUP		1
#define UPDATEGROUP		12

#define GETLASTPAGE		100

#define GROUPINIT		1

struct _interFD
{
	char	code[12];			/*펀드코드*/
	char	name[40];			/*펀드명*/
	char	rt06[9];			/*6개월 수익률*/
	char	rate[9];			/*누적 수익률*/
	char	idtp[20];			/*간접투자유형*/
	char	sdat[8];			/*설정일*/
	char	rknm[20];			/*투자등급명*/
	char	shgb[1];			/*조회구분*/
	/*'1': 인터넷거래가능*/
	/*'0': 인터넷거래불가능*/
	char	lnfd[1];			/*라인피드*/
};

struct _inters
{
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수	
	char	code[12];	// 종목코드
	char	name[32];	// 종목명
	char	xprc[10];	// 보유단가
	char	xnum[10];	// 보유수량
	char	xupnum[16];	// 상장 주식수
	char	bookmark[1];	// 북마크
	char	futureGubn[1];	// 신용매매 구분
	double  creditPrc;		// 신용금액
	double	maeipPrc;		// 매입금액
	char	filler[6];		// Reserved
};

struct _listitem {		
	char	code[12];			/*펀드코드*/
	char	name[40];			/*펀드명*/
	char	rt06[9];			/*6개월 수익률*/
	char	rate[9];			/*누적 수익률*/
	char	idtp[20];			/*간접투자유형*/
	char	sdat[10];			/*설정일*/
	char	rknm[20];			/*투자등급명*/
	char	shgb[16];			/*조회구분*/

	_inters* pInter;
};

struct _listitem2 {		
	char	code[12];			/*펀드코드*/
	char	name[40];			/*펀드명*/	
};


struct _grouplist
{
	char gno[4];			// 그룹번호
	char gnm[20];			// 그룹명

};

struct _groupEditlist
{
	char oldgno[4];			// 그룹번호
	char newgno[4];
	char oldgnm[20];			// 그룹명
	
};

#define	sz_inters	sizeof(struct _inters)
#define	sz_bookmark	sizeof(struct _bookmarkinfo)
#define sz_interFd	sizeof(struct _interFD)

class CDlg : public CDialog
{
// Construction
public:
	CDlg(CWnd* pParent = NULL, CWnd *main = NULL);   // standard constructor
	~CDlg();
// Dialog Data
	//{{AFX_DATA(CDlg)
	enum { IDD = IDD_IBSETUP };
	CStatic	m_errormsg;
	CEdit	m_setrt06;
	CEdit	m_setrate;
	CButton	m_ratecheck;
	CButton	m_rt06check;
	CComboBox	m_fdtype;
	CComboBox	m_gubn;
	CComboBox	m_ivgrade;
	CEditX		m_search;
	CComboBox	m_groupselect;
	CListCtrl	m_list2;
	CListCtrl	m_list1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg)
	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnAlladd();
	afx_msg void OnAlldelete();
	afx_msg void OnLineadd();
	afx_msg void OnUp();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNewgroup();
	afx_msg void OnDown();
	afx_msg void OnGroupedit();
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOk();
	afx_msg void OnSelchangeGroupselect();
	afx_msg void OnSelchangeFdtype();
	afx_msg void OnSelchangeGubn();
	afx_msg void OnSelchangeIvgrade();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnSearch();
	//}}AFX_MSG
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnEditGroup(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomDrawList( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnCustomDrawList2( NMHDR* pNMHDR, LRESULT* pResult );

	

	

	DECLARE_MESSAGE_MAP()
public:
	CWnd*	m_parent;
	CWnd*	m_main;
	CImageList*	m_dragImage;
	

	CString m_root;
	CString m_name;
	int		m_activegno{};
	int		m_selectGroup{};
	bool	m_bIsEdit{};	//수정했는지 여부
	int		m_nCol0Asc{};
	int		m_nCol1Asc{};
	int		m_nCol2Asc{};
	bool		m_isDuplicateCode{};		//종목 중복 등록 여부
	bool		m_activePage{};
	int		m_selindex{};
	int		m_nItem{};
	bool	m_bAddAll{};		// 숫자입력시 매번 클리어 하는것 방지하기 위해
	
	int		m_fdtypeno{};
	int		m_gubnno{};
	int		m_ivgradeno{};
	
	int		m_gcnt[MYFD_MAXGROUP]{};
	int		m_gdnt{};

	bool	m_newGroup{};
	bool	m_editGroup{};
	bool	m_deleteGroup{};
	bool	m_clickdel{};
	bool	m_clickadd{};
	char	m_afcd[MYFD_MAXFUND][12]{};
	bool	m_createGrp{};
	bool	m_bSearch{};		//2017.01.20 KSJ 조회시 체크

	CArray <_inters*,   _inters*>   m_inters;		// file_load	
	CArray <_listitem*, _listitem*> m_list2item;

	CArray <_listitem*, _listitem*> m_listitem;
	CArray <_listitem*, _listitem*> m_listitem2;
	CArray <_listitem*, _listitem*> m_searchlist;
	CArray <_listitem*, _listitem*> m_searchlist2;
	CArray <_grouplist*, _grouplist*> m_grouplist;
	CArray <_grouplist*, _grouplist*> m_Newgrouplist;

	std::vector<std::unique_ptr<_grouplist>> _vNewgrouplist;

	struct	_dragdata {		// drag item struct
		int	list{};
		int	item{};
		CString	code;
		CString	name;
	};

	_dragdata	m_dragData;
	int		m_nPrevCol{};
	BOOL	m_bCol0Asc{};
	BOOL	m_bCol1Asc{};
	BOOL	m_bCol2Asc{};
	BOOL	m_bCol3Asc{};
	BOOL	m_bCol4Asc{};
	BOOL	m_bCol5Asc{};
	BOOL	m_bCol6Asc{};
	BOOL	m_bCol7Asc{};
	bool	m_autoScroll{};
	int		m_nPrevList2{};
	bool	m_dlginit{};
	bool	comboselect{};
	bool	editboxselect{};
	bool 	m_ok{};
	bool	m_up{};
	bool	m_down{};

	int rmlist[MYFD_MAXCNT]{};
	int _rmlist[MYFD_MAXCNT]{};
	int		m_delcnt{};

	int		m_confirm{};


//	int		m_nPrevCol;

public:
	void	QueryExcelSise();
	CString	GetGroupName(int gno);
	LRESULT SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);





protected:
	BOOL	AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width, int textfmt = LVCFMT_LEFT);
	void initList();
	void initCombo();
	void LoadFile();
	CString	parseX(CString &srcstr, CString substr);
	CString Variant(int nComm, CString strData = _T("") );
	int		AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem = -1);
	CImageList* CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl);
	void	OnEndDrag(CPoint point);
	CString	GetCodeName(CString code);
	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);
	void	deleteList2();
	void	insertList1toList2(int sitem = -1);
	void	GuideMsg(CString msg);
	bool	GuideMsg(CString msg, bool question);
	CString	idTOstring(UINT id);
	void	DeleteItemToList2(int ipos);
	void	SetListColumnText(CListCtrl* pList, int nCol, int nAsc);
	bool	existCodeInList2(CString code);
	void	InitEdit();
	void	loadConfig();

	int		loadingInterest(int gno);
	int		addItem(const CString& strItem, int nItem);
	void	KillSelected(CListCtrl *ListCtrl);
	void	SetSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll = TRUE);
	
	void	savingGroupFile(int gno, CString gname);
	void	savingInterest(int gno);
	bool	savingGroupOrder(CString gname);
	void	ClearList2item();
	void	SetAutoScroll(CListCtrl* listctrl, int direct);

	void	searchAction(int column);
	void	AppendItemToList1(int ipos, CString code, CString name, _listitem *pListItem = NULL);
	void	AppendItemToList1(int ipos, CString code, CString name, CString rt06, CString rate, CString idtp, CString sdat, CString rknm, CString shgb, _listitem *pListItem = NULL, BOOL serarch = FALSE);

	void	AppendItemToList2(int ipos, CString code, CString name, CString rt06, CString rate, CString idtp, CString sdat, CString rknm, CString shgb, _listitem *pListItem = NULL);

	int		findMatchIndex(CString text, int column);
	bool	SearchData(int kind, CString sName, bool bAddAll);
	bool	SearchJongmok(CString sName, bool bAddAll);
	bool	SearchWord(CString sName, bool bAddAll);
	WORD	MakeHangul(char cChar1, char cChar2);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);


	void	ParsingData(char* datB, int datL);
	void	ParsingDataNGroup(char* datB, int datL);
	void	ParsingCreateNGroup(char* datB, int datL);
	void	ParsingGroupList(char* datB, int datL);
	void	ParsingDeleteGroup(char* datB, int datL);
	void	ParsingBackup(char* datB, int datL);
	void	ParsingRestore(char* datB, int datL);

	void	SendQueryDeleteGroup(char* datB, int datL);
	void	SendQueryUpdateGroup(char* datB, int datL);

	CString	getFDtypeName(int num);
	CString	getGubnName(int num);
	CString	getIVGradeName(int num);
	void	LoadList1(CString fdtype, CString gubn, CString ivgrade, float _rt06 = 0, float _rate = 0);

	void QueryBackup();
	void QueryRestore();

	void SendNewGroup(int gno);
	void SendDelNewGroup(int gno);
	void SendCreateGroup(CString gnm);
	void SendInterestData(int gno);

	void DeleteGroupList();
	void DeleteList2Item();
	void DeleteListItem();

	
};

