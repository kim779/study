#if !defined(AFX_SETUPENVDLG_H__57612F8B_2FDF_44C0_823A_73A5FE997A3A__INCLUDED_)
#define AFX_SETUPENVDLG_H__57612F8B_2FDF_44C0_823A_73A5FE997A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSetupEnvDlg : public CDialog
{
public:
	CSetupEnvDlg(CWnd* pMain, char* info, CWnd* pParent = NULL);
	void SetScreen();

	//{{AFX_DATA(CSetupEnvDlg)
	enum { IDD = IDD_SETUP_ENV };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSetupEnvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSetupEnvDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	struct _envInfo*	m_pEnv;
	CWnd*	m_pMain;
	CColorCtrl	m_color[9];
	CKindCtrl	m_kind[5];
	CWidthCtrl	m_width[5];
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_SETUPENVDLG_H__57612F8B_2FDF_44C0_823A_73A5FE997A3A__INCLUDED_)
