#if !defined(AFX_FILTERSETDLG_H__4713C34D_05F1_4C4A_BB71_8F0EA3C2FD39__INCLUDED_)
#define AFX_FILTERSETDLG_H__4713C34D_05F1_4C4A_BB71_8F0EA3C2FD39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinterSetDlg.h : header file
//

#include "resource.h"

//# Control
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
/////////////////////////////////////////////////////////////////////////////
// CFilterSetDlg dialog

class CFilterSetWnd;
class CFilterSetDlg : public CDialog
{
public:
	CFont m_fontDefault;
	CFilterSetDlg(CWnd* pParent);   // standard constructor
	
	CFilterSetWnd *m_pwndFilterSet;
	
	CfxImgButton	m_btnBase;
	CfxImgButton	m_btnCancel;
	CfxImgButton	m_btnApply;
	
private:
	int m_nIndustry;
	CWnd*	m_pParent;

public:
	void SetIndustry(int nIndustry)	{ m_nIndustry = nIndustry; }
	int  GetIndustry()				{ return m_nIndustry; }
	
private:
	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }
	
	// Dialog Data
	//{{AFX_DATA(CFilterSetDlg)
	enum { IDD = IDD_DLG_FILTER };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSetDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CFilterSetDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	afx_msg void OnBtnBase();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnApply();
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSETDLG_H__4713C34D_05F1_4C4A_BB71_8F0EA3C2FD39__INCLUDED_)
