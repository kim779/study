#if !defined(AFX_NEXPAND_H__9BFB0919_C407_4BAD_9457_D29C2B0F3FAD__INCLUDED_)
#define AFX_NEXPAND_H__9BFB0919_C407_4BAD_9457_D29C2B0F3FAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NExpand.h : header file
//
#define WM_EXPANDEVENT	WM_USER+333
#define STAT_CNT	3
#define STAT_ENABLE	0
#define STAT_DOWN	1
#define STAT_DISABLE	2

#define DIR_NONE	0
#define DIR_LEFT	1
#define DIR_RIGHT	2
/////////////////////////////////////////////////////////////////////////////
// CNExpand window

class CNExpand : public CWnd
{
// Construction
public:
	CNExpand(CWnd* parent, CString home);

// Attributes
protected:
	CWnd*		m_parent;
	CString		m_home;
	CBitmap		m_img;
	UINT		m_status[2];
	UINT		m_selectFlag;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNExpand)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNExpand();
	void	SetStatus(int lStatus, int rStatus);
	UINT	SelectButton(CPoint pt);
	void	drawImg(int dir, bool down = FALSE, CDC* pDC = NULL);

	// Generated message map functions
protected:
	//{{AFX_MSG(CNExpand)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEXPAND_H__9BFB0919_C407_4BAD_9457_D29C2B0F3FAD__INCLUDED_)
