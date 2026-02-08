#if !defined(AFX_DLGRUNMAIN_H__B6818782_5085_462B_8ABF_119264C822DF__INCLUDED_)
#define AFX_DLGRUNMAIN_H__B6818782_5085_462B_8ABF_119264C822DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRunMain.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
#include "control/XTabCtrl.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STConfigdef.h"
#include "DlgSTConfig.h"
#include "Control/xSkinButton.h"

#include "TabRunSignal.h"		// 신호설정
#include "TabRunJumun.h"		// 주문설정
#include "TabRunClear.h"		// 강제청산
#include "TabRunSetup.h"		// 운용설정

/////////////////////////////////////////////////////////////////////////////
// CDlgRunMain dialog
class CTabBasic;
class CDlgRunMain : public CRscDlg
{
// Construction
public:
	CDlgRunMain(LPCSTR szSTFile, LPCSTR szSTName, CWnd* pParent = NULL);   // standard constructor
	CDlgRunMain(HWND hViewHandle, LPCSTR szSTFile, LPCSTR szSTName, CWnd* pParent = NULL);   // standard constructor

	void SetParentModified(HWND hParent) { m_hParentUsr = hParent;};
	HWND GetHwndUser() { return m_hParentUsr;}
	BOOL GetDefaultFile(CString &szDefaultFile);

	void InitControl();
	BOOL SaveConfig();

// Dialog Data
	//{{AFX_DATA(CDlgRunMain)
	enum { IDD = IDD_DLG_RUNMAIN };
	CExEdit			m_editTitle;
	CExDrawButton	m_btOK;
	CExDrawButton	m_btCancel;
	CExDrawButton	m_btDefault;
	CXTabCtrl		m_tabCtrl;
	//}}AFX_DATA
//	CxSkinButton	m_btOK;
//	CxSkinButton	m_btCancel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRunMain)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CString		m_szSTFile, m_szSTName;
	CArray<CRscDlg*,CRscDlg*> m_arrayTab;
	COLORREF	m_crBk;
	CBrush		m_brBkColor;
	HWND		m_hParentUsr;
	HWND		m_hViewHandle;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRunMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetdefault();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRUNMAIN_H__B6818782_5085_462B_8ABF_119264C822DF__INCLUDED_)
