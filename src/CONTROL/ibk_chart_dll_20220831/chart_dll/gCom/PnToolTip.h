#if !defined(AFX_PNTOOLTIP_H__D3F36409_6D6A_4F58_AD50_D8ECFAEB2AF2__INCLUDED_)
#define AFX_PNTOOLTIP_H__D3F36409_6D6A_4F58_AD50_D8ECFAEB2AF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnToolTip window

class CPnToolTip : public CWnd
{
// Construction
public:
	CPnToolTip(CWnd* pView, CWnd* pParent, int event);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnToolTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnToolTip();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnToolTip)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CAxisGComApp	*m_pApp;
	CWnd		*m_pView;
	CWnd		*m_pParent;
	CFont*		m_pFont;
	int		m_event;

	CWnd		*m_pWnd;
	int		m_showDelay;

	COLORREF	m_clrBkColor;
	COLORREF	m_clrTextColor;

	CSize		m_fSize;

	int		m_cntWidth;
	int		m_cntHeight;

	CPoint		m_savePt;

public:	
	CString		m_dispStr;
	
public:
	BOOL	Create(CFont* pFont);
	void	RelayEvent(UINT message);
	void	hideToolTip();

private:
	void	showToolTip();
	bool	checkViewFocus();
	void	getTextInfo(CString str, int& cntWidth, int& cntHeight);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNTOOLTIP_H__D3F36409_6D6A_4F58_AD50_D8ECFAEB2AF2__INCLUDED_)
