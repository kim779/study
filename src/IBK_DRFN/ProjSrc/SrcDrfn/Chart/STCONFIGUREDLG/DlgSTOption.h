#if !defined(AFX_DLGSTOPTION_H__B6818782_5085_462B_8ABF_119264C822DF__INCLUDED_)
#define AFX_DLGSTOPTION_H__B6818782_5085_462B_8ABF_119264C822DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSTOption.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
#include "control/XTabCtrl.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STConfigdef.h"
#include "DlgSTConfig.h"
#include "Control/xSkinButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSTOption dialog
class CTabBasic;
class CDlgSTOption : public CRscDlg
{
// Construction
public:
	CDlgSTOption(CWnd* pParent = NULL);   // standard constructor
	void SetISTControllerD(CISTControllerD *pSTControllerD) { m_pSTControllerD = pSTControllerD;m_nType=0;}
	void SetSfgPath(LPCTSTR lpSfgName, int nType) { m_strSfgName = lpSfgName;m_nType=nType;}
	void ChangeSfgPath(LPCTSTR lpSfgName);
	CString GetSdPath();
	void SetPassword(LPCTSTR lpPassword) {  m_strPassword = lpPassword;}
	void SetUserID(LPCTSTR lpUserID) {  m_strUserID = lpUserID;}
	void SetExpiredDate(LPCTSTR lpExpiredDate) {  m_strExpiredDate = lpExpiredDate;}
	CString GetPassword();
	CString GetExpiredDate();
	CString GetUserID();
	
	void	GetSTFile(LPCTSTR lpszScriptFile);

	void SetUsePassword(BOOL bUse) { m_bUsePassword = bUse;}
	void SetUseUserID(BOOL bUse) { m_bUseUserID = bUse;}
	void SetUseExpiredDate(BOOL bUse) { m_bUseExpiredDate = bUse;}
	BOOL	GetUsePassword();
	BOOL	GetUseUserID();
	BOOL	GetUseExpiredDate();
	int GetType() { return m_nType;}
	void SetErrorMessage(LPCTSTR lpErrorMessage) { m_strErrorMessage = lpErrorMessage;}
	CISTControllerD* GetISTControllerD() { return m_pSTControllerD;}
	void SetParentModified(HWND hParent) { m_hParentUsr = hParent;};
	DWORD GetPrivateProfileStringInST(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, int nUseType);  // 1:OnlySfg, 2: ST,if possible
	BOOL WritePrivateProfileStringInST(LPCTSTR lpAppName,LPCTSTR lpKeyName,LPCTSTR lpString, int nUseType );	// 1:OnlySfg, 2:ST,if possible 3:Both
	CString GetSfgPath();
	void SaveConfig();
	void DeleteConfig();
	HWND GetHwndUser() { return m_hParentUsr;}

	void SetOpenType(int nOpenType) {m_nOpenType = nOpenType;}
	void ChangeConfigureInfo();
	void ApplyConfig();

// Dialog Data
	//{{AFX_DATA(CDlgSTOption)
	enum { IDD = IDD_DLG_ST_OPTION };
	CxSkinButton	m_btOK;
	CxSkinButton	m_btCancel;
	CXTabCtrl	m_tabCtrl;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSTOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CArray<CRscDlg*,CRscDlg*> m_arrayTab;
	CISTControllerD *m_pSTControllerD;
	CTabBasic * m_pTabBasic;
	CString m_strSfgName;
	int		m_nType;
	CString m_strPassword;
	CString m_strUserID;
	CString m_strExpiredDate;
	BOOL	m_bUsePassword;
	BOOL	m_bUseUserID;
	BOOL	m_bUseExpiredDate;
	CString m_strErrorMessage;
	HWND	m_hParentUsr;
	COLORREF m_crBk;
	CBrush	m_brBkColor;

	CDlgSTConfig* m_pSTConfig;
	int		m_nOpenType;
	CString	m_szSTPath;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSTOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTOPTION_H__B6818782_5085_462B_8ABF_119264C822DF__INCLUDED_)
