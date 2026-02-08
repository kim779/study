#if !defined(AFX_PROGRESSDLG_H__0D7CC1B6_0C23_4C2C_A6D4_224018D64DD7__INCLUDED_)
#define AFX_PROGRESSDLG_H__0D7CC1B6_0C23_4C2C_A6D4_224018D64DD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog
#include "button.h"
#include "NProgressCtrl.h"
class CProgressDlg : public CDialog
{
// Construction
public:
	CProgressDlg(CWnd* pParent = NULL, BOOL bNew = FALSE);   // standard constructor
	virtual ~CProgressDlg();
// Dialog Data
	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESS, IDD_NEW = IDD_NEW_PROGRESS };
	CNProgressCtrl	m_totalCtrl;
	CNProgressCtrl	m_currCtrl;
	CStatic		m_static;
	CString	m_current;
	//}}AFX_DATA

protected:
	BOOL	m_bNew;
	bool	m_first;
	double	m_szTotal;
	double	m_szCurr;
	int	m_nTotal;
	int	m_nCurr;
	int	m_rate;

	Cbutton	m_bnCancel;
	Cbutton	m_bnTOTSIZE;
	Cbutton	m_bnENDCNT;
	Cbutton	m_bnFILECNT;
	Cbutton	m_bnFILENAME;
	Cbutton	m_errMsg;

	HBITMAP	m_bitmap;
public:
	void	SetTotal(CString total);
	void	SetProgress(CString detail, int rate, BOOL error);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	visibleButton();
	void	initButtons();
	// Generated message map functions
	//{{AFX_MSG(CProgressDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// 20070711 Áß°£¿¡ ¸ØÃß´Â °æ¿ì ´Ý±â ¹öÆ° º¸¿©ÁÜ
	UINT	m_timer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDLG_H__0D7CC1B6_0C23_4C2C_A6D4_224018D64DD7__INCLUDED_)
