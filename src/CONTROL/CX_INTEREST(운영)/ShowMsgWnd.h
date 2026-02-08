#pragma once
// ShowMsgWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFBShowMsgWnd window

class CShowMsgWnd : public CWnd
{
// Operations
public:
	static CWnd* ShowMessage(CWnd* pParentWnd, LPCTSTR cszMessage, UINT nDelay = 1000, BOOL bProgress = FALSE);
	static CWnd* ShowMessage(CWnd* pParentWnd, CWnd *pRectWnd, int nWidth, int nHeight, LPCTSTR cszMessage, UINT nDelay = 1000, BOOL bProgress = FALSE);

// Construction
protected:
	CShowMsgWnd();
	CString		m_strMessage;
	CWnd*		m_pParentWnd;
	COLORREF	m_crBkgrnd;
	BOOL		m_bProgress;
	int			m_nMax;
	int			m_nPos;
	int			m_nOffset;

	static LOGFONT	m_lfFont;
	static int		m_nInstanceCnt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowMsgWnd)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShowMsgWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShowMsgWnd)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	static CSize GetTextExtent(CWnd* pWnd, LPCTSTR cszText);

public:
	void   SetMex(int nMax) { m_nMax = nMax; }
	void   SetPos(int nPos, BOOL bRedraw = TRUE);
	void   AddPos(int nPos, BOOL bRedraw = TRUE);

};