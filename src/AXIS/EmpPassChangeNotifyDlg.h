#if !defined(AFX_EMPPASSCHANGEDLG_H__0696EAF3_D6F1_4263_8488_99C54BCE395A__INCLUDED_)
#define AFX_EMPPASSCHANGEDLG_H__0696EAF3_D6F1_4263_8488_99C54BCE395A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgBtn.h"

typedef enum  { PNT_ADVISE, PNT_MUST} PSWD_NOTIFY_TYPE;

class CEmpPassChangeNotifyDlg : public CDialog
{
// Construction
public:
	CEmpPassChangeNotifyDlg(CWnd* pParent, PSWD_NOTIFY_TYPE type);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEmpPassChangeDlg)
	enum { IDD = IDD_EMPPASSCHANAGENOTIFY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmpPassChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CFont m_font;
	HBITMAP m_hBack{};
	CImgBtn m_btnOk, m_btnLater;

protected:
	PSWD_NOTIFY_TYPE m_type;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmpPassChangeDlg)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPPASSCHANGEDLG_H__0696EAF3_D6F1_4263_8488_99C54BCE395A__INCLUDED_)
