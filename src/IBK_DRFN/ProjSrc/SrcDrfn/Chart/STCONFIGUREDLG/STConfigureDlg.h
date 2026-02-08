// STConfigureDlg.h : main header file for the STCONFIGUREDLG DLL
//

#if !defined(AFX_STCONFIGUREDLG_H__DD0CF959_BC31_417C_8B71_B70B1B85F687__INCLUDED_)
#define AFX_STCONFIGUREDLG_H__DD0CF959_BC31_417C_8B71_B70B1B85F687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "DlgSTOption.h"
#include "../Include_ST/ISTControllerD.h"

/////////////////////////////////////////////////////////////////////////////
// CSTConfigureDlgApp
// See STConfigureDlg.cpp for the implementation of this class
//

class CSTConfigureDlgApp : public CWinApp
{
public:
	CSTConfigureDlgApp();
	BOOL ShowConfigureDlg(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType);
	BOOL ShowconfigureDlgOnWizard(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, CRect* pRect);
	BOOL WriteConfigure(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, int nMode);

	void SetPassword(LPCTSTR lpPassword);
	void SetUserID(LPCTSTR lpUserID);
	void SetExpiredDate(LPCTSTR lpExpiredDate);
	CString GetPassword();
	CString GetUserID();
	CString GetExpiredDate();

	void SetUsePassword(BOOL bUse);
	void SetUseUserID(BOOL bUse);
	void SetUseExpiredDate(BOOL bUse); 
	BOOL	GetUsePassword();
	BOOL	GetUseUserID();
	BOOL	GetUseExpiredDate();

private:
	CDlgSTOption m_dlgSTOption;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTConfigureDlgApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTConfigureDlgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STCONFIGUREDLG_H__DD0CF959_BC31_417C_8B71_B70B1B85F687__INCLUDED_)
