#pragma once
// SFCodeDlg.h : header file
//
#include "../../h/jmcode.h"
#include <math.h>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSFCodeDlg dialog

class CSFCodeDlg : public CDialog
{
// Construction
public:
	CSFCodeDlg(int kind, CPoint pt = CPoint(-1, -1), int type = 0, CWnd* pParent = nullptr);   // standard constructor
	CString GetCode(){ return m_szCode; }
	CString GetName(){ return m_szName; }
	CString GetMgubn(){ return m_szMgubn; }
	void LoadMaster();
	BOOL FindCode(int kind,CString szCode);
	int m_iTYPE{};  //0 - ¿œπ›  , 1 - small 
private:
	vector<struct sfcode> m_fjcode;
	CMapStringToString	m_arName;
	CMapStringToString  m_arHNam;
	CMapStringToString  m_arHCod;
	CMapStringToString  m_mapHCod;
	CPoint	m_point;
	CString m_sPreSearch;

private:
	
protected:
	CString		m_szName;
	CString		m_szCode;
	CString		m_szMgubn;

// Dialog Data
	//{{AFX_DATA(CSFCodeDlg)
	enum { IDD = IDD_DIALOG_NEW_SFCODE };
	CListBox	m_NameList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSFCodeDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSFCodeDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeNameList();
	afx_msg void OnChangeEditSearch();
	afx_msg void OnButtonMth1();
	afx_msg void OnButtonMth2();
	afx_msg void OnButtonMth3();
	afx_msg void OnButtonMth4();
	afx_msg void OnButtonMth5();
	afx_msg void OnButtonMth6();
	afx_msg void OnButtonMth7();
	afx_msg void OnButtonMth8();
	afx_msg void OnButtonMth9();
	afx_msg void OnButtonSpr1();
	afx_msg void OnButtonSpr2();
	afx_msg void OnButtonSpr3();
	afx_msg void OnButtonSpr4();
	afx_msg void OnButtonSpr5();
	afx_msg void OnButtonSpr6();
	afx_msg void OnButtonSpr7();
	afx_msg void OnButtonSpr8();
	afx_msg void OnButtonSearchNext();
	afx_msg void OnButtonSearchPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSFCodeDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnDblclkNameList();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

