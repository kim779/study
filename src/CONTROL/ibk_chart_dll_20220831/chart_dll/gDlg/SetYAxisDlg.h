#if !defined(AFX_SETYAXISDLG_H__9DC3AD34_032B_4B69_858E_1A2A726A1FF8__INCLUDED_)
#define AFX_SETYAXISDLG_H__9DC3AD34_032B_4B69_858E_1A2A726A1FF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetYAxisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetYAxisDlg dialog

class CSetYAxisDlg : public CDialog
{
// Construction
public:
	CSetYAxisDlg(char* pcInfo, char* pcTotalInfo, char* pcEnvInfo, CWnd* pwndView, CWnd* pParent = NULL);   // standard constructor

public:

private:
	CWnd*	m_pwndView;

	struct _graph*	m_pgraph;
	struct _envInfo*	m_pEnv;
	CMap<int, int, struct _graph*, struct _graph*>	m_mapInfo;

public:

private:
	BOOL	Init();

public:

// Dialog Data
	//{{AFX_DATA(CSetYAxisDlg)
	enum { IDD = IDD_DIALOG_SET_YAXIS };
	CColorCtrl	m_colorEdge;
	CColorCtrl	m_colorBG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetYAxisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetYAxisDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioAuto();
	afx_msg void OnRadioFit();
	afx_msg void OnRadioFix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETYAXISDLG_H__9DC3AD34_032B_4B69_858E_1A2A726A1FF8__INCLUDED_)
