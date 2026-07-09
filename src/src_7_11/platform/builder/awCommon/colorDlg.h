#if !defined(AFX_COLORDLG_H__F635FFC6_6584_4FCE_869F_A0F9677FB61D__INCLUDED_)
#define AFX_COLORDLG_H__F635FFC6_6584_4FCE_869F_A0F9677FB61D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorDlg.h : header file
//

#include "resource.h"
#include "ColorBox.h"

#define	userCOLOR	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class AFX_EXT_CLASS CColorDlg : public CDialog
{
// Construction
public:
	CColorDlg(CWnd* pParent, DWORD color, bool user = true, CRect voidRect = CRect(0));   // standard constructor
	~CColorDlg();
// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_COLOR };
	CTabCtrl	m_tabCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	DWORD	m_color;

protected:
	CColorBox	*m_pUserColor, *m_pSysColor;
	bool	m_user, m_capture, m_bDrag;
	CRect	m_voidRect, m_rc;
	CPoint	m_firstPnt;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTabctrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__F635FFC6_6584_4FCE_869F_A0F9677FB61D__INCLUDED_)
