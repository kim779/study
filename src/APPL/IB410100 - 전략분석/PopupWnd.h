#pragma once
// PopupWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopupWnd window

class CPopupWnd : public CWnd
{
// Construction
public:
	CPopupWnd();

// Attributes
public:
	CIB410100App	*m_pApp;
	std::unique_ptr<CBitmap> m_pBitmap;
	CRect			m_poprt;
	
// Operations
public:
	void		DrawPopup(int chknum, CString names, CString data);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPopupWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopupWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

