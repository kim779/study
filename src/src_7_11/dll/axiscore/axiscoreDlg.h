// axiscoreDlg.h : header file
//

#if !defined(AFX_AXISCOREDLG_H__02F24153_208D_4D6D_A974_EA41567D358F__INCLUDED_)
#define AFX_AXISCOREDLG_H__02F24153_208D_4D6D_A974_EA41567D358F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreDlg dialog

class CAxiscoreDlg : public CDialog
{
// Construction
public:
	CAxiscoreDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAxiscoreDlg)
	enum { IDD = IDD_AXISCORE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxiscoreDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAxiscoreDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISCOREDLG_H__02F24153_208D_4D6D_A974_EA41567D358F__INCLUDED_)
