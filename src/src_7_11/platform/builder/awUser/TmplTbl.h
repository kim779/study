#if !defined(AFX_TMPLTBL_H__FEEAE45D_456B_4203_A54E_F90DCB0CFDFD__INCLUDED_)
#define AFX_TMPLTBL_H__FEEAE45D_456B_4203_A54E_F90DCB0CFDFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TmplTbl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTmplTbl window

class AFX_EXT_CLASS CTmplTbl : public CWnd
{
// Construction
public:
	CTmplTbl();

public:
	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;
	class	CwSock*	m_wsock;
	int		m_state;
	bool		m_enable, m_request;
	CFile		m_file;
	CString		m_status, m_name;
	CStringArray	m_groupArr, m_nameArr;
	CComboBox	*m_pCombo;
	CPoint		m_MousePoint;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTmplTbl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setFileName(int idx, CString name);
	void loadTmpl(CPoint pnt);
	BOOL addItem(CListCtrl *LC, int item, int subItem, CString &str);
	void fillList(CListCtrl *LC, int idx);
	void fillCombo(CComboBox *CBO);
	CString Parser(CString &srcstr, CString substr);
	void openList(CComboBox *CBO);
	void loadFile();
	void finish();
	bool sendData(bool req = true, CString name = "");
	void goState(int state, CString msg = "");
	LONG OnState(WPARAM wParam, LPARAM lParam);
	LONG OnReadData(WPARAM wParam, LPARAM lParam);
	virtual ~CTmplTbl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTmplTbl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMPLTBL_H__FEEAE45D_456B_4203_A54E_F90DCB0CFDFD__INCLUDED_)
