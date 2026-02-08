#pragma once
// ClearWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClearWnd window

class CClearWnd : public CWnd
{
// Construction
public:
	CClearWnd(CWnd* pWizard);

// Attributes
public:

// Operations
public:
	inline int GetMapKey() { return m_nMapKey; };
	void Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClearWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPallete();
	virtual ~CClearWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClearWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pWizard;
	int m_nMapKey;
};

