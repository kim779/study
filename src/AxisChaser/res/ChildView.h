// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__83C30872_8276_4049_BC46_029C0660CB67__INCLUDED_)
#define AFX_CHILDVIEW_H__83C30872_8276_4049_BC46_029C0660CB67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define K_NORMAL	0
#define K_SNDRCV	1
#define K_RTMS		2
#define K_SYSTRACE	3
/////////////////////////////////////////////////////////////////////////////
// CChildView window
#include "RichLib/RichEditCtrlEx.h"
class CChildView : public CWnd
{
// Construction
public:
	CChildView();
	virtual ~CChildView();

// Attributes
protected:
	CFont		m_font;
	CRichEditCtrlEx	m_trace;

	BOOL	m_bBINARY;
	BOOL	m_bSNDRCV;
	BOOL	m_bRTM;
	BOOL	m_bSYSTRACE;
	BOOL	m_bNOSCROLL;
	BOOL	m_bSTAYONTOP;

	BOOL	m_bBold[4];
	BOOL	m_bItalic[4];
	BOOL	m_bUline[4];
	BOOL	m_bStrike[4];

	BOOL	m_bSameColor;
	BOOL	m_bSameFont;

	COLORREF	m_bColor;
	COLORREF	m_tColor[4];
	CString		m_fName[4];
	int		m_fSize[4];
	int		m_rtsL;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Test();

	void	Init();
	void	CopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) ;

	void	addTrace(CString dat, int kind = K_NORMAL);
	CString	parse(CString &dat, CString separate);
	void	setTrace(bool setting = true);
	void	OnRCVData(WPARAM wParam, LPARAM lParam);
	void	WriteFile(char* pBytes, int nBytes);
	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnModeBinary();
	afx_msg void OnUpdateModeBinary(CCmdUI* pCmdUI);
	afx_msg void OnModeText();
	afx_msg void OnUpdateModeText(CCmdUI* pCmdUI);
	afx_msg void OnTrBase();
	afx_msg void OnUpdateTrBase(CCmdUI* pCmdUI);
	afx_msg void OnRtm();
	afx_msg void OnUpdateRtm(CCmdUI* pCmdUI);
	afx_msg void OnSystemtrace();
	afx_msg void OnUpdateSystemtrace(CCmdUI* pCmdUI);
	afx_msg void OnSetbkcolor();
	afx_msg void OnSettextcolor();
	afx_msg void OnSetfont();
	afx_msg void OnClear();
	afx_msg void OnDestroy();
	afx_msg void OnNoscroll();
	afx_msg void OnUpdateNoscroll(CCmdUI* pCmdUI);
	afx_msg void OnStayontop();
	afx_msg void OnUpdateStayontop(CCmdUI* pCmdUI);
	//}}AFX_MSG
//	afx_msg void OnProcessCmd(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnUpdateProcessCmd(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__83C30872_8276_4049_BC46_029C0660CB67__INCLUDED_)
