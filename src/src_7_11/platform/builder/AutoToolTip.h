#if !defined(AFX_AUTOTOOLTIP_H__FB20581D_BD30_477C_9419_26BFB713B65A__INCLUDED_)
#define AFX_AUTOTOOLTIP_H__FB20581D_BD30_477C_9419_26BFB713B65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoToolTip dialog

class CAutoToolTip : public CDialog
{
// Construction
public:
	int	getWidth();
	CAutoToolTip(CWnd* pParent = NULL);   // standard constructor
	
	CWnd *m_pParent;
	long	m_nStart;
// Dialog Data
	//{{AFX_DATA(CAutoToolTip)
	enum { IDD = IDD_AUTOTIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoToolTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoToolTip)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOTOOLTIP_H__FB20581D_BD30_477C_9419_26BFB713B65A__INCLUDED_)
