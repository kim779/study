#pragma once
// WarningWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWarningWnd window

class CWarningWnd : public CWnd
{
// Construction
public:
	CWarningWnd(CWnd* pWizard);
	inline int GetMapKey() { return m_nMapKey; };
	void Init();
	void SetPallete();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarningWnd)
	//}}AFX_VIRTUAL

// Implementation
private:
	CWnd* m_pWizard;
	int m_nMapKey;
public:
	virtual ~CWarningWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWarningWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

