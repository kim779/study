// axisverDlg.h : header file
//

#if !defined(AFX_AXISVERDLG_H__B4AAD97D_E74B_4810_A51E_434358EDC8DA__INCLUDED_)
#define AFX_AXISVERDLG_H__B4AAD97D_E74B_4810_A51E_434358EDC8DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAxisverDlg dialog

class CAxisverDlg : public CDialog
{
// Construction
public:
	CAxisverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAxisverDlg)
	enum { IDD = IDD_AXISVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAxisverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISVERDLG_H__B4AAD97D_E74B_4810_A51E_434358EDC8DA__INCLUDED_)
