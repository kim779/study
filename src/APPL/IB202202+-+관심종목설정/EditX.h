#pragma once
// EditX.h on header file
//

#define	WM_EDITX	(WM_USER + 100)

#define	wpBLANK		1
#define	wpENTER		2
#define	wpSEARCH1	3
#define	wpSEARCH2	4
#define wpDown		10
#define	wpBTNREDRAW		7  //0422
#define	wpBTNCLEAR	8  //0422

/////////////////////////////////////////////////////////////////////////////
// CEditX window

class CEditX : public CEdit
{
// Construction
public:
	CEditX();
	bool	m_bKeyDown;
	bool m_bTracking{};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditX)
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseLeave();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


class CXEditPrompt : public CEdit
{
	DECLARE_DYNCREATE(CXEditPrompt)
	// Construction
public:
	CXEditPrompt();
	virtual ~CXEditPrompt();


	bool	m_bKeyDown{};
	bool	m_bTracking{};
	// Operations
public:
	void Reset();

	// Attributes
public:
	COLORREF GetPromptColor() { return m_crPromptColor; }
	CString GetPromptText() { return m_strPromptText; }
	int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
	void GetWindowText(CString &rString) const;
	void SetPromptColor(COLORREF crText);
	void SetPromptText(LPCTSTR lpszPrompt);
	void SetWindowText(LPCTSTR lpszString);

	COLORREF GetBKColor() const { return m_crBkColor; }
	void SetBKColor(COLORREF crBK);
	int GetWindowTextLength() const;
	DWORD GetCueBannerAlign() { return m_dwCueBannerAlign; }
	void SetCueBannerAlign(DWORD val);

	BOOL IsShowingCueBanner() const { return m_bShowCueBanner; }
	void setInputData(CString sInput){ _preinput = sInput; }
	CString getInputData() const { return _preinput; }
protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitEdit();

protected:
	inline void UpdateStyleForCueBanner()
	{
		m_dwOldAlignStyle = GetStyle() & (ES_LEFT | ES_CENTER | ES_RIGHT);
		ModifyStyle(0, m_dwCueBannerAlign);
	}
	inline void UpdateStyleForNormalText()
	{
		ModifyStyle(m_dwCueBannerAlign, m_dwOldAlignStyle);
	}
	inline void EnableCueBanner(BOOL bEnable = TRUE)
	{
		m_bShowCueBanner = bEnable;
		bEnable ? UpdateStyleForCueBanner() : UpdateStyleForNormalText();
	}

private:
	BOOL m_bShowCueBanner;	  // TRUE = user has not yet selected this control
	CString m_strPromptText;  // prompt text to display initially
	COLORREF m_crPromptColor; // color to use for prompt text
	CBrush m_brush;		  // background brush for WM_CTLCOLOR

	COLORREF m_crBkColor;	  // color of background
	CBrush m_robrush;	  // Read-only background brush for WM_CTLCOLOR
	DWORD m_dwCueBannerAlign; // ES_LEFT
	DWORD m_dwOldAlignStyle{};
	CString _preinput;



	// Generated message map functions
protected:
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd *pOldWnd);
	afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
};