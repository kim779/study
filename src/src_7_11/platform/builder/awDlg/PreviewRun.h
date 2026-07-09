#if !defined(AFX_PREVIEWRUN_H__7972A247_B2C8_440C_BCB5_C4734F4DB4FC__INCLUDED_)
#define AFX_PREVIEWRUN_H__7972A247_B2C8_440C_BCB5_C4734F4DB4FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewRun.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPreviewRun dialog

class AFX_EXT_CLASS CPreviewRun : public CDialog
{
// Construction
public:
	CPreviewRun(CWnd* pParent = NULL);   // standard constructor
	void	OnStart();
public:
	CString		m_mapName, m_sCaption;
	CWnd		*m_pWizardCtrl;
	CWnd		*m_pView;
protected:
	int		m_wndType, m_key;
private:

public:
	bool UnRegisterCtrl(CString name);
	bool RegisterCtrl(CString name);
// Dialog Data
	//{{AFX_DATA(CPreviewRun)
	enum { IDD = IDD_PREVIEW_RUN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewRun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreviewRun)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWRUN_H__7972A247_B2C8_440C_BCB5_C4734F4DB4FC__INCLUDED_)
