#if !defined(AFX_STOPWARN_H__EA0A629D_F102_4B65_A851_3DAB33DE5414__INCLUDED_)
#define AFX_STOPWARN_H__EA0A629D_F102_4B65_A851_3DAB33DE5414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StopWarn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStopWarn dialog

class CStopWarn : public CDialog
{
// Construction
public:
	CStopWarn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStopWarn)
	enum { IDD = IDD_STOPLOSSWARNNING };
	CRichEditCtrl	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStopWarn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadMsg();
	// Generated message map functions
	//{{AFX_MSG(CStopWarn)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPWARN_H__EA0A629D_F102_4B65_A851_3DAB33DE5414__INCLUDED_)
