#pragma once
// Price.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrice window

class CPrice : public CWnd
{
// Construction
public:
	CPrice();

// Attributes
public:
	CString		m_sRoot, m_rtnStr;
	HINSTANCE	m_hPriceDll{};
	CWnd*		m_pParent=nullptr;
	CWnd*		m_pPrice = nullptr;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrice)
	//}}AFX_VIRTUAL

// Implementation
public:
	void BtnClick();
	void SetValues(CString str);
	void createPrice();

	virtual ~CPrice();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrice)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

