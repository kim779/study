#if !defined(AFX_COLORDLG_H__F635FFC6_6584_4FCE_869F_A0F9677FB61D__INCLUDED_)
#define AFX_COLORDLG_H__F635FFC6_6584_4FCE_869F_A0F9677FB61D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// colorDlg.h : header file
//

#include "resource.h"

#define	userCOLOR	0x02000000

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class AFX_EXT_CLASS CColorDlg : public CDialog
{
// Construction
public:
	CColorDlg(CWnd* pParent, DWORD color, bool user = true);   // standard constructor
	~CColorDlg();
// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_COLOR };
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
	class	CAxisDraw*	m_draw;
	class	CAxisPalette*	m_palette;

protected:
	CString	m_path;

// Implementation
protected:
	bool	m_user;
	CRect	makeRect(int row = -1, int col = -1);
	void	onUser();
	void	loadUserColor(COLORREF* ccolor);
	void	saveUserColor(COLORREF* ccolor);

	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__F635FFC6_6584_4FCE_869F_A0F9677FB61D__INCLUDED_)
