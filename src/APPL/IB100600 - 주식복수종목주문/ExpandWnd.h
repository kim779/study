#pragma once
// ExpandWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpandWnd window

class CExpandWnd : public CWnd
{
// Construction
public:
	CExpandWnd(CWnd* pWizard);

// Attributes
public:

// Operations
public:
	inline int GetMapKey() { return m_nMapKey; };
	void Init();
	void SetPallete();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpandWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExpandWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExpandWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pWizard;
	int m_nMapKey;
};

