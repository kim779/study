#if !defined(AFX_SKINDLG_H__02F15016_C696_4DB2_9329_43DAF5D12A8C__INCLUDED_)
#define AFX_SKINDLG_H__02F15016_C696_4DB2_9329_43DAF5D12A8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg dialog
#include "resource.h"
class CSkinDlg : public CDialog
{
// Construction
public:
	CSkinDlg(bool (*axiscall)(int, WPARAM, LPARAM), CString home, CWnd* pParent = NULL);   // standard constructor
	~CSkinDlg();

	void	ShowSkin(CPoint point, bool down = false);
	void	ChangeInfo();
// Dialog Data
	//{{AFX_DATA(CSkinDlg)
	enum { IDD = IDD_SELECTSKIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_skinN;
	bool		m_down;
	CString		m_home;
	CBitmap		m_bitmap[2];

	CStringArray	m_ary;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	void	loadPalette();
	void	draw_ActiveSkin(CDC* pDC);
	void	change_Palette(CString skinN);
	// Generated message map functions
	//{{AFX_MSG(CSkinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINDLG_H__02F15016_C696_4DB2_9329_43DAF5D12A8C__INCLUDED_)
