#pragma once
// EditX.h : header file
//

#define	WM_EDITX	(WM_USER + 100)

#define	wpBLANK		1
#define	wpENTER		2
#define	wpSEARCH1	3
#define	wpSEARCH2	4
#define	wpUP		5
#define	wpDOWN		6

/////////////////////////////////////////////////////////////////////////////
// CEditX window

class CEditX : public CEdit
{
// Construction
public:
	CEditX();
	bool	m_bKeyDown;
	/*CRect m_rectClear{};
	CBitmap* m_pClearBmp{};
	CString m_path{};
	void DrawBitmap(CDC* pDC, CRect rect);
	void Init();
	CString m_strimagepath;
	CWnd* m_pParent{};*/
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditX)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditX)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

