#if !defined(AFX_SETTICKINFO_H__B6C5B875_2A47_404B_9C18_310393E6F73F__INCLUDED_)
#define AFX_SETTICKINFO_H__B6C5B875_2A47_404B_9C18_310393E6F73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTickInfo.h : header file
//
#include "SetupDialog.h"
#include "ReportCtrl.h"

class _tickitem
{
public:
	int	kind{};
	int	aux{};
	DWORD	selectN{};
	CStringArray	sAry;
};

/////////////////////////////////////////////////////////////////////////////
// CSetTickInfo dialog

class AFX_EXT_CLASS CSetTickInfo : public CSetupDialog
{
// Construction
public:
	CSetTickInfo(int id = 0, CWnd* pParent = NULL);   // standard constructor
	~CSetTickInfo();

	BOOL	GetViewTick(int index);

	virtual void	ApplySetup();
	virtual void	DefaultSetup();
	void SearchGroupList();
// Dialog Data
	//{{AFX_DATA(CSetTickInfo)
	enum { IDD = IDD_SETTICKINFO };
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

private:

public:
	CBitmap* m_bmpCombo;
	CBitmap* m_bmpCombo_N;
	void draw_groupBox(CPaintDC *pdc, int ids);
	CFont m_drawfont;
	int		m_active;
	int		m_fontsize;
	int		m_bold;
	int		m_period;
	
	CString			m_fontname;
	CFont			m_font;
	CBrush			m_brush;
	//CObArray		m_arItemInfo;
	std::vector <std::shared_ptr < _tickitem>> m_arItemInfo;
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
	afx_msg void OnChangePanecount1();
	afx_msg void OnChangePanecount2();
	afx_msg void OnSelchangeFont();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnChangeLIST(WPARAM wParam, LPARAM lParam);  //vc2019
	afx_msg LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTICKINFO_H__B6C5B875_2A47_404B_9C18_310393E6F73F__INCLUDED_)
