#pragma once
// Sheet1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheet1

class CSheet1 : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet1)

// Construction
public:
	CSheet1(UINT nIDCaption, CWnd* pParent = NULL, UINT iSelectPage = 0, CWnd* pView = NULL);
	CSheet1(LPCTSTR pszCaption, CWnd* pParent = NULL, UINT iSelectPage = 0, CWnd* pView = NULL);

	void SetBkColor(CDC* pDc, int nSelTab, COLORREF clrBK);
// Attributes
public:
	//CWnd	*m_pParent;
	//CWnd	*m_pView;
	std::unique_ptr<class CStgPage>	  m_pStgPage;
	std::unique_ptr<class CMyStgPage> m_pMyStgPage;
	std::unique_ptr<class CSonikPage> m_pSonikPage;
	std::unique_ptr<class CHedgePage> m_pHedgePage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheet1)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheet1();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheet1)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

