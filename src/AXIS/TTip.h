#if !defined(AFX_TTIP_H__9AA8972B_FBD4_4669_97CE_205222F611A5__INCLUDED_)
#define AFX_TTIP_H__9AA8972B_FBD4_4669_97CE_205222F611A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTip window
#define _DUP_SCREEN
class CTTip : public CWnd
{
// Construction
public:
	CTTip(CWnd* mainframe);

// Attributes
public:
	CWnd*	m_mainframe;
	CFont	m_font;
	CRect	m_rect;
	CString	m_tips;
	int	m_nHeight;
#ifdef _DUP_SCREEN
	CStringArray		m_arScreen;
#else
	CMapStringToString	m_mapScreen;
#endif
	CString	m_title;
// Operations
public:
	void	HideTips();
	void	ShowTips(CRect tRc, CRect wRc, CString tips);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTTip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TTIP_H__9AA8972B_FBD4_4669_97CE_205222F611A5__INCLUDED_)
