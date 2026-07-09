#if !defined(AFX_SETTICKINFO_H__B6C5B875_2A47_404B_9C18_310393E6F73F__INCLUDED_)
#define AFX_SETTICKINFO_H__B6C5B875_2A47_404B_9C18_310393E6F73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTickInfo.h : header file
//
#include "resource.h"
#include "ReportCtrl.h"
#include "../ctrl/colorCombo.h"

class _tickitem
{
public:
	int	kind;
	int	aux;
	DWORD	selectN;
	CStringArray	sAry;
};

/////////////////////////////////////////////////////////////////////////////
// CSetTickInfo dialog

class AFX_EXT_CLASS CSetTickInfo : public CDialog
{
// Construction
public:
	CSetTickInfo(CString home, CString userN, int id = 0, CWnd* pParent = NULL);   // standard constructor
	~CSetTickInfo();

	BOOL	GetViewTick(int index);

	void	ApplySetup();
	void	DefaultSetup();

// Dialog Data
	//{{AFX_DATA(CSetTickInfo)
	enum { IDD = IDD_SETTICKINFO };
	CcolorCombo	m_cbColor;
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	CComboBox	m_interCom;
	CComboBox	m_updnCom;
	CReportCtrl	m_indexList;
	CReportCtrl	m_newsList;
	UINT	m_bar1cnt;
	UINT	m_bar2cnt;
	BOOL	m_view1;
	BOOL	m_view2;
	BOOL	m_screenbar;
	//}}AFX_DATA

public:
	int		m_active;
	int		m_fontsize;
	int		m_bold;
	int		m_period;
	
	CString		m_home;
	CString		m_userName;
	CString		m_fontname;

	COLORREF	m_color;
	CFont		m_font;

	CBrush		m_brush;

	CObArray	m_arItemInfo;
	CStringArray	m_ary;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTickInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	Init();
	void	loadInfo();
	void	saveInfo();
	void	SetSplit(int cnt = 6, int idx = 0);
	void	SelectTicker();
	CWnd*	GetWnd(UINT nID) { return GetDlgItem(nID); }
	void	UpdateInfoText(bool all = false);
	void	GetSplitItemArray(int barIndex, CObArray& ary);
	void	GetKindAry(CObArray& ary);
	void	SetItemInfo();
	CString	GetPaneString(_tickitem* item);
	void	SetStatus(int kind);
	void	ProcessLbutton(CPoint pt);
	CString	GetIndexString(int idx);
	void	UpdateItemInfo(int kind);
	void	loadInter();
	void	writeInterCode(CString section, int groupN);
	int	matchItemN(int key);
	void	setFont();
	COLORREF GetColor(int index);
	// Generated message map functions
	//{{AFX_MSG(CSetTickInfo)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangePanecount1();
	afx_msg void OnChangePanecount2();
	afx_msg void OnSelchangeFont();
	//}}AFX_MSG
	afx_msg LRESULT OnChangeLIST(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTICKINFO_H__B6C5B875_2A47_404B_9C18_310393E6F73F__INCLUDED_)
