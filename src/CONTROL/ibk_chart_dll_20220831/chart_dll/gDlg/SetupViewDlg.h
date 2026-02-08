#if !defined(AFX_SETUPVIEWDLG_H__E41052C9_F375_4C91_BF05_E8F311A66B97__INCLUDED_)
#define AFX_SETUPVIEWDLG_H__E41052C9_F375_4C91_BF05_E8F311A66B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSetupViewDlg : public CDialog
{
public:
	CSetupViewDlg(CWnd* pMain, char* info, CWnd* pParent = NULL);
	void SetScreen();

	//{{AFX_DATA(CSetupViewDlg)
	enum { IDD = IDD_SETUP_VIEW };
	CSpinCtrl	m_spinRight;
	CSpinCtrl	m_spinLeft;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSetupViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSetupViewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	struct	_envInfo*	m_pEnv;	
	CWnd*	m_pMain;
};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_SETUPVIEWDLG_H__E41052C9_F375_4C91_BF05_E8F311A66B97__INCLUDED_)
