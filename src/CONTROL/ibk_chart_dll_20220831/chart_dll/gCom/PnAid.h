#if !defined(AFX_PNAID_H__14B4136E_6DB7_4F37_9936_AAF97BB67F82__INCLUDED_)
#define AFX_PNAID_H__14B4136E_6DB7_4F37_9936_AAF97BB67F82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnAid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnAid window

#include <afxtempl.h>

class CPnAid : public CWnd
{
// Construction
public:
	CPnAid(CWnd* pwndView, CWnd* pwndGView, CFont* pFont, int iCtrlKind, char* pcDllName);

// Attributes
public:

private:
	int	m_iFocusIndex;
	int	m_iLBDownIndex;
	BOOL	m_bTracking;

	CWnd*	m_pwndView;
	CWnd*	m_pwndGView;

	CFont*	m_pfont;
	CFont*	m_pfontSmall;

	CString	m_strMapName;
	CString	m_strComment;


	CAxisGComApp*	m_pApp;

//	class CMyTool*		m_pMyTool;
	class CPnToolTip*	m_pToolTip;

	
	CArray<class CPnAidInfo*, class CPnAidInfo*>	m_arPnAidInfo;
//	CMap<int, int, class CPnAidInfo*, class CPnAidInfo*>	m_mapPnAidInfo;

public:

private:
	int	Init();
	void	Draw(CDC* pDC);
	void	RemoveArray();
//	void	RemoveMap();

	void	AidConfig();

	bool	Load();
	bool	Save();
	void	InitAid();
	void	ResetRect();

	void	MouseMove(CPoint point);
	void	LButtonDown(CPoint point);
	void	LButtonUp(CPoint point);

	int	FindIndex(CPoint point);

	CString	GetPosComment(CPoint pt);
	void	mouseLeaveAction(WPARAM wParam, LPARAM lParam);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnAid)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnAid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnAid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnAidEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNAID_H__14B4136E_6DB7_4F37_9936_AAF97BB67F82__INCLUDED_)
