#if !defined(AFX_PORTPOLIODELDLG_H__11086608_BCB9_4149_84CC_EFC4A656E444__INCLUDED_)
#define AFX_PORTPOLIODELDLG_H__11086608_BCB9_4149_84CC_EFC4A656E444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortPolioDelDlg.h : header file
//

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "./AddControl/PortEditCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CPortPolioDelDlg dialog
class CPortPolioDelDlg : public CDialog
{

private:
	CFont m_fontDefault;

public:
	CPortPolioDelDlg(CWnd* pParent = NULL);   // standard constructor
	
	CWnd* m_pMainWnd;
	
	CList <int, int&> m_listCheckInfo;
	CPortEditCtrl     *m_peditPortfolio;
	CRect             m_rePortDel;
		
	CCheckListBox   m_chklistPortDel;
	CfxImgButton	m_btnDelete;
	CfxImgButton	m_btnCancel;

	
	
	// Dialog Data
	//{{AFX_DATA(CPortPolioDelDlg)
	enum { IDD = IDD_PORTDEL_DLG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortPolioDelDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CPortPolioDelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnDeleteBtnClicked();
	afx_msg void OnCancelBtnClicked();
	//afx_msg void OnAllDeleteBtnClicked();
	afx_msg void OnPortDelCtrlSelChange();
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTPOLIODELDLG_H__11086608_BCB9_4149_84CC_EFC4A656E444__INCLUDED_)
