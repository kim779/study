#if !defined(AFX_JONGMUKDLG_H__FE07D254_034B_41C2_8672_85A11F070694__INCLUDED_)
#define AFX_JONGMUKDLG_H__FE07D254_034B_41C2_8672_85A11F070694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fxButton.h"
#include "fxComboBox.h"

// JongmukDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJongmukDlg dialog



static BOOL m_gListsort;

class CJongmukDlg : public CDialog		//	주식 다이얼로그 ...
{
// Construction
public:
	CJongmukDlg(int kind, int type, CPoint pt = CPoint(-1, -1), CWnd* pParent = NULL);   // standard constructor

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
	int     GetJongmuk() {	return m_nJongmuk; }
	void	SetCode(CString szCode){ m_szCode = szCode; }
	void	SetName(CString szName){ m_szName = szName; }
	BOOL	FindCode(int kind, CString szCode, int type);
	void	ListRemoveAll();
	void	InitBtn();
	BOOL	FindELWJSCode(CString code);

	BOOL	SetOptionMonth();

	CString	GetGroup(CString szCode);
	CString	CompareQuote(CString szCode);
	CString	CompareFuture(CString szCode);
	CString CompareIndex(CString szCode, int gubun, bool index = true);
	bool	GetLinkCode(CString szCode, char* code);
	void	clearEditSearch();
	bool	CheckDigit(CString code);
	
private:
	bool		m_bAddAll;	// ListCtrl의 중복 클리어를 방지하기 위해
	CPtrArray	m_arSelCode;	// 현재 선택된 종목의 목록
	void	searchAction(int column);
	void	searchWord();
	int		findMatchIndex(CString text, int column);
	bool	SearchData(CString sName, bool bAddAll);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);
	WORD	MakeHangul(char cChar1, char cChar2);

	// Dialog Data
	//{{AFX_DATA(CJongmukDlg)
	enum { IDD = IDD_DIALOG_STOCKCODEINFO };
	CfxComboBox	m_ctlComboSearch;
	CEditX	m_edtSearch;
	CfxButton	m_ctlBtn9;
	CfxButton	m_ctlBtn8;
	CfxButton	m_ctlBtn7;
	CfxButton	m_ctlBtn6;
	CfxButton	m_ctlBtn5;
	CfxButton	m_ctlBtn4;
	CfxButton	m_ctlBtn3;
	CfxButton	m_ctlBtn22;
	CfxButton	m_ctlBtn21;
	CfxButton	m_ctlBtn20;
	CfxButton	m_ctlBtn2;
	CfxButton	m_ctlBtn19;
	CfxButton	m_ctlBtn18;
	CfxButton	m_ctlBtn17;
	CfxButton	m_ctlBtn16;
	CfxButton	m_ctlBtn15;
	CfxButton	m_ctlBtn14;
	CfxButton	m_ctlBtn13;
	CfxButton	m_ctlBtn12;
	CfxButton	m_ctlBtn10;
	CfxButton	m_ctlBtn11;
	CfxButton	m_ctlBtn1;
	CfxButton       m_ctlButtonETF;
	CfxButton	m_ctlFreeBoard;
	CListBox	m_ListPut;
	CListBox	m_ListCall;
	CfxButton	m_CtlButFuture4;
	CfxButton	m_CtlButFuture3;
	CfxButton	m_CtlButFuture2;
	CfxButton	m_CtlButFuture1;
	CSkinListCtrl	m_ListCtrl;
	CfxButton	m_ctlButtonTHIRD;
	CfxButton	m_ctlButtonELW;
	CfxButton	m_ctlButtonKOSPIWORK;
	CfxButton	m_ctlButtonKOSPI;
	CfxButton	m_ctlButtonKOSDAQWORK;
	CfxButton	m_ctlButtonKOSDAQ;
	CfxButton	m_ctlButtonFavor;
	CfxButton	m_ctlButtonECN;
	CfxButton	m_ctlButtonAll;
	CfxComboBox	m_ComboGroup;
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
	void	SearchString(CString szText);
	void	Part();	
	void	ListKosDaqSort(int nItem);
	void	ListKospiSort(int nItem);
	BOOL	ListAllWorkSort(int nItem);
	BOOL	ListElwSort(int nItem);
	BOOL	ListFreeBoard(int nItem);
	void	AddGrid(CString& str, int flag);
	void	AddCombo(CString& szText);
	void	HideButton(BOOL flag = FALSE);
	void	MoveGrid(int kind = 1);
	void	ShowCombo(BOOL flag = TRUE);
	BOOL	CodeListAll();
	BOOL	CodeListSome();
	void	LoadFavorites();
	void	AddItem(int row, CString szCode, CString szName);
	void	ListFavor(int key);
	CString Parser(CString &srcstr, CString substr);
	bool	FileCheck(int fNo);
	void	GetExCode();

	int		m_nRadioOption;	
	CSFont		m_Font;
	int		m_Select;
	
	//CArray<JCode*, JCode*>		m_aJCode;
	CArray<JCode*, JCode*>		m_aTemp;
	CArray<FAVOR, FAVOR>		m_arrayFavor;

	//CArray<ELWCODE*, ELWCODE*>	m_arrayELWCode;		// ELW
	//CArray<PBCODE, PBCODE>		m_arrayPBCode;		// 발행사

	CString		m_szCode;
	CString		m_szName;
	int		m_nJongmuk;

	int		m_kind;
	int		m_Type;
	HBRUSH		m_hBrush;
	CPoint		m_Point;
	BOOL		m_bOption;
	
	int		m_btnSel;

	CString	m_sRoot, m_sUser;
	
	struct _exCode {
		CString code;
		CString name;
	};
	CArray <_exCode, _exCode> m_exCode[2];
	
	// Generated message map functions
	//{{AFX_MSG(CJongmukDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonAllcode();
	afx_msg void OnButtonKospicode();
	afx_msg void OnButtonKosdaqcode();
	afx_msg void OnButtonSecCode();
	afx_msg void OnButtonThirdmarket();
	afx_msg void OnButtonElw();
	afx_msg void OnButtonKospiwork();
	afx_msg void OnButtonKosdaqwork();
	afx_msg void OnButtonFavorites();
	afx_msg void OnButtonOk();
	afx_msg void OnSelchangeComboGroup();
	afx_msg void OnKillfocusEditSearch();
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
	afx_msg void OnButtonUpfreeboard();
	afx_msg void OnButtonEtf();
	afx_msg void OnSelendokComboSearch();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnRadio(UINT nID);
	afx_msg void OnClickBtnSort(UINT nID);
	afx_msg LPARAM OnEditXMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JONGMUKDLG_H__FE07D254_034B_41C2_8672_85A11F070694__INCLUDED_)
