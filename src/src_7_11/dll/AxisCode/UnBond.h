#pragma once


// CUnBond dialog

#include "XListCtrl.h"
#include "SkinButton.h"
#include "SkinDialog.h"
#include "NButton.h"
#include "Panel.h"
#include "SEdit.h"

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


struct ListThreadData {
	int icnt;
	//CXListCtrl *p_list;
	void* papp;
	void* pmain;
	int igubn; //시장종류
};

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
//class CUnBond : public CDialogEx
class CUnBond : public CDialogEx
{
	DECLARE_DYNAMIC(CUnBond)

	
public:
	CUnBond(CWnd* pParent = NULL);
	CUnBond(CPoint pt, CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnBond();
	int			DeleteSpace(char* pBuff, int nLen);
	int			m_iSelectRow;

	int			m_iTotalCnt;
	bool		m_bDonwlading;
	bool		m_bWorking;
	CCriticalSection	m_section;
	CPanel		*m_pPanel;

	bool		GetCodeData(CString strcode, CJcode *code);

	BOOL		IsNumeric(CString strVal);
	BOOL		LoadCJCode();
	BOOL		FindCode(CString szCode);
	BOOL		IsExistInBalDate(CString strdata);
	BOOL		IsExistInManDate(CString strdata);

	CString		GetCode(){ return m_szCode; }
	CString		GetName(){ return m_szName; }
	CString		Parser(CString &strSrc, CString strSub = _T("\t"));
	CString		GetCodeData(CString strcode);
	CString		GetExcelTile();

	void		Setdatapointer(char* pdata);
	void		AllItemRedraw();
	void		Reposition();
	void		InitBtn();
	void		Set_KS_HanMode();
	void		SetDlgPos(int x, int y);
	void		SetName(CString szName){ m_szName = szName; }
	void		InitList(CXListCtrl &list, const TCHAR**pszTitle);
	void		ResizeListColumn(CXListCtrl &list, const int *arrWidth);
	void		RelocationControls();
	void		InitInterList();
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
	void		FillListByArr(CListCtrl* plist, CArray<CJCODESTR*, CJCODESTR*>&	tmpArr);
	bool		checkstrip(bool bstrip, CJCODESTR* CJCode);
	void		deletethread()	{ h_thread = NULL;};
	void		FillListdata_thread(int igubn);
	void		SetButtonPress(int type);
	void		FilAllList_thread();
	void		FillListByGubn(int igubn);
	void		AllListSet();
	void		WriteLastbtn();
	void		MakePopup();
	void		RepositionBtn(CNButton* pbtn, CNButton* pupbtn);
	CListCtrl* GetVisibleList();
	BOOL				m_bINTERMODE;
// Dialog Data
	enum { IDD = IDD_DIALOG_ALLBOND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	CWnd*				m_pMain;
	BOOL				m_bDraging;	
	CBrush*				m_pCtlBrush, *m_pInterBrusl;
	
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

	CImageList *m_pImageList;
	CArray<CJCODE*, CJCODE*>	m_arrayCJCode;
	CArray<CJCODESTR*, CJCODESTR*>	m_cpArr;
	
public:
	CArray<CJCODESTR*, CJCODESTR*>	m_arrlist;
	

	afx_msg void OnDblclkListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditUnbondFind();
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	CFont* m_pfont;

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
	CNButton m_btnexcel;
	
	CListCtrl m_UnBListCtrl;
	CListCtrl m_InterList;
	CListCtrl m_AllList;

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
	CButton m_btnleft;
	CButton m_btnright;
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
	afx_msg void OnBnClickedCheckStrip();
	afx_msg void OnCustomdrawListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnExcel();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRclickListAllbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickListUnbond(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate1man(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate1val(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate2man(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDatetimechangeDate2val(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


