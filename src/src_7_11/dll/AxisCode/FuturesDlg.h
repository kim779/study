#if !defined(AFX_FUTURESDLG_H__8B1BD4B4_0720_405F_9C0F_9B7932AFD202__INCLUDED_)
#define AFX_FUTURESDLG_H__8B1BD4B4_0720_405F_9C0F_9B7932AFD202__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FuturesDlg.h : header file
//
#include "fxButton.h"

/////////////////////////////////////////////////////////////////////////////
// CFuturesDlg dialog

class CFuturesDlg : public CDialog		// 선물 옵션 다이얼로그 
{
// Construction
public:
	CString Parser(CString &srcstr, CString substr);
	void DisableSpread(BOOL bflag);
	void AdjustControls();
	void SetFutureCodes();
	int AddListCtlCode(int nItem, CString szCode, CString szName);
	void SetCodeList(int nMonth);
	void OnButtonOPMonth4();
	void OnButtonOPMonth3();
	void OnButtonOPMonth2();
	void OnButtonOPMonth1();
	void ShowJOption(BOOL bShow);
	void HideFutureBtns(BOOL bHide = TRUE);
	void SetButton(CButtonST *pButton);
	CString GetString(char *pChar, int nMinLen);

	CFuturesDlg(int kind, int type = 0, CPoint pt = CPoint(-1, -1), CWnd* pParent = NULL);   // standard constructor

	bool	GetCodeOrderable(CString code, int* type);
	void	SetDlgPos(int x, int y);
	CString	CommaModify(CString string);
	void	SetButton();
	void	DisableFuture();
	BOOL	FindCode(int kind, CString szCode);
	void	MonthInit();
	void	InitBtn();
	void	SetCode(CString szCode){ m_szEditCode = m_szCode = szCode; }
	void	SetName(CString szName){ m_szEditName = m_szName = szName; }
	CString GetCode(){ return m_szCode; }
	CString GetName(){ return m_szName; }
	int		GetKind(){ return m_Kind; }
	int		GetCountMonth(){ return m_nCountMonth; }
	
	CArray<CString, CString>* GetArray();
	void SetArray();

	void	SetSFCodes();	// 주식옵션...
	BOOL	SetPjCodes(); // 현물파일...
	void    Sort();
	
// Dialog Data
	//{{AFX_DATA(CFuturesDlg)
	enum { IDD = IDD_DIALOG_FUTURES };
	CfxButton	m_ctlButtonJFuture;
	CSkinListCtrl	m_codeList;
	CButtonST	m_ctlButtonOPMonth4;
	CButtonST	m_ctlButtonOPMonth3;
	CButtonST	m_ctlButtonOPMonth2;
	CfxButton	m_ctlButtonOPMonth1;
	CfxButton	m_ctlButtonKostar;
	CfxButton	m_ctlButtonSPMonth9;
	CfxButton	m_ctlButtonSPMonth6;
	CfxButton	m_ctlButtonSPMonth3;
	CListBox	m_plistbox;
	CMyEdit		m_ctlEditName;
	CMyEdit		m_ctlEditCode;
	CfxButton	m_ctlButtonMonth12;
	CfxButton	m_ctlButtonMonth3;
	CfxButton	m_ctlButtonMonth6;
	CfxButton	m_ctlButtonMonth9;
	CButtonST	m_ctlButtonName;
	CfxButton	m_ctlButtonKospi;
	CString		m_szEditCode;
	CString		m_szEditName;
	int		m_kindList;
	int		m_nGubn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFuturesDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
// Implementation
protected:

	void DisableButton();
	void CountData(CString code);
	void FindEnd();
	DATA FindPut(CString szMonth, CString szPrice);
	DATA FindCall(CString szMonth, CString szPrice);
	BOOL FindMonth(CString szMon);
	void SearchMonth();
	void AddList(PCODE& pcode);

	void SearchPrice();
	void Standard(CString szCode);
	void ButtonText(CString szName, CString szCode);
	BOOL FindPrice(CString szCompare);
	
protected:

	CCheckBox*	m_pCheck;
	CString		m_szName;
	CString		m_szCode;
	CString		m_szBName;
	CString		m_szBCode;
	int             m_Spread;
	int		m_Kind;		// select kind...
	int		m_Type;
	CTool		m_Tool;
	CPoint		m_Point;
	int		m_nAtm;
	int		m_nCountMonth;
	int		m_nStep;
	BOOL		m_bKostar;
	BOOL		m_bSort;
	HBRUSH		m_hBrush;

	int		m_nSFSet[7];

	
	CArray<PCODE, PCODE>		m_arrayput;
	CArray<PCODE, PCODE>		m_arraycall;
	
	CList<PCODE, PCODE>		m_ListPcode;
	CQArray<CString, CString>	m_ArrayPrice;
	CList<CString, CString>		m_ListMonth;
	CArray<CString, CString>	m_ArrayOMonth;
	HBRUSH				m_br;

	// Generated message map functions
	//{{AFX_MSG(CFuturesDlg)
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonKospi200();
	afx_msg void OnButtonMonth12();
	afx_msg void OnButtonMonth9();
	afx_msg void OnButtonMonth6();
	afx_msg void OnButtonMonth3();
	afx_msg void OnButtonSPMonth9();
	afx_msg void OnButtonSPMonth6();
	afx_msg void OnButtonSPMonth3();
	afx_msg void OnButtonName();
	afx_msg void OnDoubleclickedButtonMonth9();
	afx_msg void OnDoubleclickedButtonMonth6();
	afx_msg void OnDoubleclickedButtonMonth3();
	afx_msg void OnDoubleclickedButtonMonth12();
	afx_msg void OnDoubleclickedButtonSPMonth9();
	afx_msg void OnDoubleclickedButtonSPMonth6();
	afx_msg void OnDoubleclickedButtonSPMonth3();
	afx_msg void OnPaint();
	afx_msg void OnDblclkPcodeList();
	afx_msg void OnSelchangePcodeList();
	afx_msg void OnPOPTIONRdo();
	afx_msg void OnClickListCode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonKostar();
	afx_msg void OnButtonJfuture();
	afx_msg	void OnRadioFuture();
	afx_msg	void OnRadioSpread();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUTURESDLG_H__8B1BD4B4_0720_405F_9C0F_9B7932AFD202__INCLUDED_)
