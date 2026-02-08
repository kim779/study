#if !defined(AFX_EMPPASSCHANGEDLG2_H__9435B69D_DFCE_45B7_8C64_3E7F4F86BF82__INCLUDED_)
#define AFX_EMPPASSCHANGEDLG2_H__9435B69D_DFCE_45B7_8C64_3E7F4F86BF82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmpPassChangeDlg2.h : header file
//

#include "ImgBtn.h"
#include "MainFrm.h"

#define IDC_BTN_SEND	(1000)
#define IDC_BTN_CLOSE	(1001)

struct sbpgt104_mid
{
	char In[5];
	char LoginPwd[32];
	char ChgLoginPwd[32];
};

struct sbpgt104_mod
{
	struct sbpgt104_mid mid;
	char Out[5];
	char MenuAuthKey[256];
};

class CEmpPassChangeDlg : public CDialog
{
// Construction
public:
	CEmpPassChangeDlg(CWnd* pParent, BOOL bCloseEnable);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEmpPassChangeDlg2)
	enum { IDD = IDD_EMPPASSCHANGE };
	CString	m_newpswd2;
	CString	m_newpswd;
	CString	m_oldpswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpPassChangeDlg2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bCloseEnable;
	CBrush m_brush;
	HBITMAP m_hBack;
	CImgBtn m_btnSend, m_btnClose;
	CMainFrame *m_pMain;

public:
	void processOUB(LPCSTR dat, int len);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmpPassChangeDlg2)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSend();
	afx_msg void OnBtnClose();
	afx_msg void OnMaxText();
	void OnOk();
	void OnCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPPASSCHANGEDLG2_H__9435B69D_DFCE_45B7_8C64_3E7F4F86BF82__INCLUDED_)
