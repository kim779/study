#if !defined(AFX_HOGAFILTERDLG_H__89E4D647_9841_48E3_8EA8_58DF2AE83AD7__INCLUDED_)
#define AFX_HOGAFILTERDLG_H__89E4D647_9841_48E3_8EA8_58DF2AE83AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HogaFilterDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHogaFilterDLG dialog

class CHogaFilterDLG : public CDialog
{
// Construction
public:
	CHogaFilterDLG(CWnd* pParent = NULL, CString root = _T(""), CString user = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHogaFilterDLG)
	enum { IDD = IDD_FILTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	CString	m_path;
	CString m_HotPath;
	BOOL	m_bHOGAFilter;
	BOOL	m_bFutureTrade;
	BOOL	m_bFutureDepth;
	BOOL	m_bOptionTrade;
	BOOL	m_bOptionDepth;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaFilterDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHogaFilterDLG)
	afx_msg void OnFutureTrade();
	afx_msg void OnFutureDepth();
	afx_msg void OnOptionTrade();
	afx_msg void OnOptionDepth();
	virtual BOOL OnInitDialog();
	afx_msg void OnHogafilter();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOGAFILTERDLG_H__89E4D647_9841_48E3_8EA8_58DF2AE83AD7__INCLUDED_)
