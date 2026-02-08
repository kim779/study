#pragma once
// CSearchPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchPage window

class CSearchPage : public CWnd
{
// Construction
public:
	CSearchPage(CWnd* pView = NULL, CWnd* pParent = NULL);
	~CSearchPage();
	// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchPage)
	//}}AFX_VIRTUAL

// Implementation
	
	// Generated message map functions
	void closeMap();	
protected:
	//{{AFX_MSG(CSearchPage)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWnd*		m_pParent;
	CWnd*		m_pView;

	int	m_mapKey;
	void loadMap(CString mapname, CString option = _T(""));	
public:
	void ChangeMap(CString mapname, CString option = _T(""));
	CString Variant(int comm, CString data = _T(""));
	void SendTrigger(CString code);
	void ProcDll(CString strMethod, CString strData);

};

