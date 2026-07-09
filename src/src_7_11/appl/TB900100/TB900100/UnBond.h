#pragma once


// CUnBond 대화 상자입니다.
#include "resource.h"
#include "NButton.h"
#include "Panel.h"
#include "SEdit.h"

#include "../../../h/jmcode.h"

#define WM_BTNMSG WM_USER + 888
#define ALLCODE		0
#define GUKCODE		1
#define TONGCODE	2
#define TUKCODE		3
#define JICODE		4
#define GUMCODE		5
#define WHECODE		6
#define DANGCODE	7
#define BALCODE		8
#define INTERCODE	9

#define btnAll	1
#define btnGuk	2
#define btnGum		3
#define btnHeui	4
#define btnJi  5
#define btnTong		6
#define btnTuk    7
#define btnBal     8
#define btnDang   9
#define btnApply   10
#define btnleft   11
#define btnright   12
#define btnlook   13

#define HNameLen	20

#define _TRIMDATA(data) \
		data.TrimLeft(); \
		data.TrimRight();

struct ListThreadData {
	int icnt;
	//CXListCtrl *p_list;
	void* papp;
	void* pmain;
	int igubn; //시장종류
};

typedef struct _CJcode{
	CString code;	// 채권 코드... 
	CString name;	// 채권이름...
	CString		kind;  // 채권종류
	CString     idct;  //지표구분
	CString		valday;  //발행일
	CString		mtday; //만기일
	CString	    mtry; //만기구분	       
	CString		itrt;  //금리 
	CString		intertype; //이자유형
	CString		credit;  //신용등급
	CString		minthree;  //민평3
	CString		minfore;  //민평4
	CString		update;  //업데이트시각
}CJCODESTR;

class CCodeData
{
public:
	CString strName;
	CString strCode;
	CString strsuik;
	CString strMangi;
	bool	bFlag;
	COLORREF clrData;
};

static bool		m_sort;

class CUnBond : public CDialogEx
{
	DECLARE_DYNAMIC(CUnBond)

public:
	CUnBond(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnBond();
	CFont*		m_pfont;
	CPanel		*m_pPanel;

	int			DeleteSpace(char* pBuff, int nLen);

	bool		m_bWorking;
	bool		m_bAllcheck;
	CCriticalSection	m_section;

	BOOL		IsNumeric(CString strVal);
	BOOL		LoadCJCode();
	BOOL		FindCode(CString szCode);
	BOOL		IsExistInBalDate(CString strdata);
	BOOL		IsExistInManDate(CString strdata);

	CString		GetCode(){ return m_szCode; }
	CString		GetName(){ return m_szName; }
	CString		Parser(CString &strSrc, CString strSub = _T("\t"));
	CString		GetExcelTile();

	void		Setdatapointer(char* pdata);
	void		AllItemRedraw();
	void		Reposition();
	void		InitBtn();
	void		Set_KS_HanMode();
	void		SetDlgPos(int x, int y);
	void		SetName(CString szName){ m_szName = szName; }
	void		RelocationControls();
	void		InitList();
	void		MoveAllLeft();
	void		MoveAllRight();
	void		LoadGroupOfFile();	
	void		AddGroup(CString strGroup);
	void		addlistGroup(CString strgroup);
	void		LoadFile(CString strgroup = _T(""));
	void		AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strFlag, CString strClr);
	void		ReflashSelectList();
	void		FillCodelistByGroup(CString strgroup);
	void		SearchData(bool beditdata = false);
	void		deletethread()	{ h_thread = NULL;};
	void		FillListdata_thread(int igubn);
	void		SetButtonPress(int type);
	void		FilAllList_thread();
	void		AllListSet();
	void		FillListByGubn(int igubn);
	void		SetDate(CDateTimeCtrl* pDate, CString strDate = _T(""));
	void		FillListByArr(CListCtrl* plist, CArray<CJCODESTR*, CJCODESTR*>&	tmpArr);
	void		SetHeaderCheck(BOOL bFlag);
	void		MakePopup();

	bool		checkstrip(bool bstrip, CJCODESTR* CJCode);

	CListCtrl* GetVisibleList();
	CJCODESTR* GetCodeData(CString strcode);
	BOOL				m_bINTERMODE;

	CArray<CJCODESTR*, CJCODESTR*>	m_cpArr;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ALLBOND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	CWnd*				m_pMain;
	BOOL				m_bDraging;	
	CBrush				*m_pCtlBrush, *m_pBrush;

	CImageList			m_cbimagelist;
	
	ListThreadData      m_thread_data;
	HANDLE				h_thread, h_Allthread;
	int					m_typecode;
	int					m_iListcnt;
	CRect				m_rcList;
	CFont				m_Font;
	CPoint				m_Point;
	CString				m_szCode;
	CString				m_szName;
	CString				m_szIntergroup;
	HBRUSH				m_hBrush;
	CString				m_sRoot;
	CString				m_sUser;
	CString				m_slastbtn;
	CString				m_title;
	CMap <CString, LPCTSTR, CMap<CString,LPCTSTR, CCodeData, CCodeData>*,  CMap<CString,LPCTSTR, CCodeData, CCodeData>*>  m_mapGroupCodemap;
	CStringArray  m_arrGroup;

	CArray<CJCODE*, CJCODE*>	m_arrayCJCode;

	CArray<CJCODE*, CJCODE*>	m_arrayGukCode;
	CArray<CJCODE*, CJCODE*>	m_arrayTongCode;
	CArray<CJCODE*, CJCODE*>	m_arrayTukCode;
	CArray<CJCODE*, CJCODE*>	m_arrayJiCode;
	CArray<CJCODE*, CJCODE*>	m_arrayGumCode;
	CArray<CJCODE*, CJCODE*>	m_arrayHeuiCode;
	CArray<CJCODE*, CJCODE*>	m_arrayMyCode;

	CArray<CJCODE*, CJCODE*>	m_arrayDangCode;
	CArray<CJCODE*, CJCODE*>	m_arrayBalCode;
	 
	CArray<CJCODESTR*, CJCODESTR*>	m_arrlist;

	CArray<CJCODESTR*, CJCODESTR*>	m_arrGuklist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrTonglist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrTuklist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrJilist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrGumlist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrHeuilist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrMylist;

	CArray<CJCODESTR*, CJCODESTR*>	m_arrDanglist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrBallist;
public:

	afx_msg void OnDblclkListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	CNButton m_btnAll;
	CNButton m_btnGuk;
	CNButton m_btnGum;
	CNButton m_btnHeui;
	CNButton m_btnJi;
	CNButton m_btnTong;
	CNButton m_btnTuk;
	CNButton m_btnBal;
	CNButton m_btnDang;
	CNButton m_btnApply;
	CNButton m_btnLook;
	CNButton m_btnleft;
	CNButton m_btnright;
	
	CListCtrl m_UnBListCtrl;
	CListCtrl m_InterList;
	CListCtrl m_AllList;

	bool		m_bDonwlading;
	int			m_iSelectRow;
	int			m_iTotalCnt;
	char*	m_pcode;

	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditCode();
	SEdit m_Search;
	CDateTimeCtrl m_Date1Val;
	CDateTimeCtrl m_Date2Val;
	CDateTimeCtrl m_Date1Man;
	CDateTimeCtrl m_Date2Man;
	afx_msg void OnBnClickedChkVal();
	afx_msg void OnBnClickedChkMan();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnGuk();
	afx_msg void OnBnClickedBtnTong();
	afx_msg void OnBnClickedBtnTuk();
	afx_msg void OnBnClickedBtnJi();
	afx_msg void OnBnClickedBtnGum();
	afx_msg void OnBnClickedBtnHeui();
	afx_msg void OnBnClickedBtnDang();
	afx_msg void OnBnClickedBtnBal();
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);

	afx_msg void OnDblclkListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_ckman;
	CButton m_ckval;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnLook();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnItemstateiconclickListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemstateiconclickListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate1val(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate2man(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate2val(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate1man(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
