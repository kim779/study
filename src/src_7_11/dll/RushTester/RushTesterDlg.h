// RushTesterDlg.h : header file
//

#if !defined(AFX_RUSHTESTERDLG_H__81BCCC33_E997_4940_B37D_894B1CAB732A__INCLUDED_)
#define AFX_RUSHTESTERDLG_H__81BCCC33_E997_4940_B37D_894B1CAB732A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RushMan.h"

/////////////////////////////////////////////////////////////////////////////
// CRushTesterDlg dialog

class CRushTesterDlg : public CDialog
{
// Construction
public:
	CRushTesterDlg(CWnd* pParent = NULL);	// standard constructor

public:
	void	LoadSet();
	void	SaveSet();
// Dialog Data
	//{{AFX_DATA(CRushTesterDlg)
	enum { IDD = IDD_RUSHTESTER_DIALOG };
	CListBox	m_lstError;
	CListCtrl	m_report;
	CListBox	m_lstQryFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRushTesterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON	m_hIcon;

	bool	m_alive;
	int	m_period;
	int	m_second;
	int	m_qryCnt;
	int	m_rcvCnt;
	float	m_totalBytes;
	int	m_subBytes;
	int	m_maxBytes;
	int	m_minBytes;
	int	m_alertCnt;
	int	m_noticeCnt;

	void	registerControl();
	void	clear();
	BOOL	addColumn(int col, int width);
	BOOL	addItem(CString items, int item, int subi);

	// Generated message map functions
	LRESULT OnRushManMsg(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CRushTesterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnconnect();
	afx_msg void OnBtnDisconnect();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnQryfile();
	afx_msg void OnBtnClearfile();
	virtual void OnOK();
	afx_msg void OnBtnFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CRushMan m_rushMan;
	CComboBox m_cblist;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnSend();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUSHTESTERDLG_H__81BCCC33_E997_4940_B37D_894B1CAB732A__INCLUDED_)
