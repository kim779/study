#if !defined(AFX_MINITICKBARCFG_H__D42EA226_80D1_4E0B_9091_08E49CEF368F__INCLUDED_)
#define AFX_MINITICKBARCFG_H__D42EA226_80D1_4E0B_9091_08E49CEF368F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniTickBarCFG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiniTickBarCFG dialog
class CSetTickButton;
class CMiniTickBarCFG : public CDialog
{
// Construction
public:
	CMiniTickBarCFG(CSetTickButton* pTickButton, CWnd* pParent = NULL);   // standard constructor

	void SetTickButtonConfig();

// Dialog Data
	//{{AFX_DATA(CMiniTickBarCFG)
	enum { IDD = IDD_MINITICKBAR_CFG };
	//}}AFX_DATA
	CString	m_strBtn[9];
	int		m_nMarket[9];
	int		m_nDate[9];
	UINT	m_nTick[9];
	int		m_bAllDay[9];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniTickBarCFG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMiniTickBarCFG)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelchangeCmbMarket(UINT nID);
	afx_msg void OnSelchangeCmbDate(UINT nID);
	afx_msg void OnEditTick(UINT nID);
	afx_msg void OnSelchangeCmbAllday(UINT nID);
	afx_msg void OnDeltaposSpinTick(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	CSetTickButton*		m_pTickButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINITICKBARCFG_H__D42EA226_80D1_4E0B_9091_08E49CEF368F__INCLUDED_)
