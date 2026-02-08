// STConfigureDlg.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "STConfigureDlg.h"
#include "../Include_ST/Commonlib.h"
#include "DlgRunMain.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSTConfigureDlgApp

BEGIN_MESSAGE_MAP(CSTConfigureDlgApp, CWinApp)
	//{{AFX_MSG_MAP(CSTConfigureDlgApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CSTConfigureDlgApp construction

CSTConfigureDlgApp::CSTConfigureDlgApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// nMode 1:Write, 0:Delete 
BOOL CSTConfigureDlgApp::WriteConfigure(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, int nMode)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_dlgSTOption.GetSafeHwnd())
	{
//		m_dlgSTOption.DestroyWindow();
		m_dlgSTOption.ShowWindow(SW_HIDE);
		m_dlgSTOption.SetISTControllerD(NULL);
	}
	return TRUE;

	//if(pISTController)
	//{
	//	m_dlgSTOption.SetISTControllerD(pISTController);
	//}
	//m_dlgSTOption.SetParentModified(hParent);

	//HINSTANCE hOld = AfxGetResourceHandle();
	//AfxSetResourceHandle(g_hInstance);
	//m_dlgSTOption.Create(CDlgSTOption::IDD,CWnd::FromHandle(hParent));
	//AfxSetResourceHandle(hOld);
	//m_dlgSTOption.ShowWindow(SW_HIDE);
	//if(nMode==1)
	//{
	//	m_dlgSTOption.SaveConfig();
	//}
	//else
	//{
	//	m_dlgSTOption.DeleteConfig();
	//}
	//
	//if(m_dlgSTOption.GetSafeHwnd())
	//{
	//	m_dlgSTOption.DestroyWindow();
	//}

	//return TRUE;
}

BOOL CSTConfigureDlgApp::ShowConfigureDlg(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_dlgSTOption.GetSafeHwnd())
	{
		m_dlgSTOption.DestroyWindow();
	}
	if(pISTController)
	{
		m_dlgSTOption.SetISTControllerD(pISTController);
	}
	else
	{
		m_dlgSTOption.SetSfgPath(lpDLLName, nType);
	}

	m_dlgSTOption.SetOpenType(0);

	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	m_dlgSTOption.Create(CDlgSTOption::IDD,CWnd::FromHandle(hParent));
	AfxSetResourceHandle(hOld);

	m_dlgSTOption.ShowWindow(SW_SHOW);
	m_dlgSTOption.SetParentModified(hParent);

	return TRUE;
}

BOOL CSTConfigureDlgApp::ShowconfigureDlgOnWizard(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, CRect* pRect)
{
	if(m_dlgSTOption.GetSafeHwnd())
	{
		m_dlgSTOption.ShowWindow(SW_HIDE);
		m_dlgSTOption.SetISTControllerD(pISTController);
		m_dlgSTOption.ChangeConfigureInfo();
		m_dlgSTOption.ShowWindow(SW_SHOW);
		return TRUE;
	}

	if(pISTController)
	{
		m_dlgSTOption.SetISTControllerD(pISTController);
	}
	else
	{
		m_dlgSTOption.SetSfgPath(lpDLLName, nType);
	}

	m_dlgSTOption.SetOpenType(1);

	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	m_dlgSTOption.Create(CDlgSTOption::IDD,CWnd::FromHandle(hParent));
	AfxSetResourceHandle(hOld);

	m_dlgSTOption.MoveWindow(pRect);
	m_dlgSTOption.ShowWindow(SW_SHOW);
	m_dlgSTOption.SetParentModified(hParent);

	return TRUE;
}

void CSTConfigureDlgApp::SetPassword(LPCTSTR lpPassword)
{
	m_dlgSTOption.SetPassword(lpPassword);
}

void CSTConfigureDlgApp::SetUserID(LPCTSTR lpUserID)
{
	m_dlgSTOption.SetUserID(lpUserID);
}

void CSTConfigureDlgApp::SetExpiredDate(LPCTSTR lpExpiredDate)
{
	m_dlgSTOption.SetExpiredDate(lpExpiredDate);
}

CString CSTConfigureDlgApp::GetPassword()
{
	return m_dlgSTOption.GetPassword();
}

CString CSTConfigureDlgApp::GetUserID()
{
	return m_dlgSTOption.GetUserID();
}

CString CSTConfigureDlgApp::GetExpiredDate()
{
	return m_dlgSTOption.GetExpiredDate();
}

void CSTConfigureDlgApp::SetUsePassword(BOOL bUse)
{
	m_dlgSTOption.SetUsePassword(bUse);
}

void CSTConfigureDlgApp::SetUseUserID(BOOL bUse)
{
	m_dlgSTOption.SetUseUserID(bUse);
}

void CSTConfigureDlgApp::SetUseExpiredDate(BOOL bUse)
{
	m_dlgSTOption.SetUseExpiredDate(bUse);
}

BOOL CSTConfigureDlgApp::GetUsePassword()
{
	return m_dlgSTOption.GetUsePassword();
}

BOOL CSTConfigureDlgApp::GetUseUserID()
{
	return m_dlgSTOption.GetUseUserID();
}

BOOL CSTConfigureDlgApp::GetUseExpiredDate()
{
	return m_dlgSTOption.GetUseExpiredDate();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSTConfigureDlgApp object

CSTConfigureDlgApp theApp;


BOOL CSTConfigureDlgApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_hInstance = theApp.m_hInstance;	
	return CWinApp::InitInstance();
}

int CSTConfigureDlgApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_dlgSTOption.DestroyWindow();
	return CWinApp::ExitInstance();
}

// lpFileName ex) "D:\\DY\\동양지존\\STDll\\AVG.SFG"
// Type  1: 전략(new) dll, 2: 사용자 함수(new), 3: 전략, 4: 사용자 함수
BOOL WINAPI fnShowConfigureDlg(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpFileName, int nType)
{	
	return theApp.ShowConfigureDlg(hParent,pISTController, lpFileName,nType);
}

BOOL WINAPI fnShowConfigureDlgOnWizard(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpFileName, int nType, CRect* pRect)
{	
	return theApp.ShowconfigureDlgOnWizard(hParent,pISTController, lpFileName,nType, pRect);
}

BOOL WINAPI fnWriteConfigure(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpFileName, int nType, int nMode)
{	
	return theApp.WriteConfigure(hParent,pISTController, lpFileName,nType,nMode);
}


void WINAPI fnSetPassword(LPCTSTR lpPassword)
{
	theApp.SetPassword(lpPassword);
}

void WINAPI fnSetUserID(LPCTSTR lpUserID)
{
	theApp.SetUserID(lpUserID);
}

void WINAPI fnSetExpiredDate(LPCTSTR lpExpiredDate)
{
	theApp.SetExpiredDate(lpExpiredDate);
}

CString WINAPI fnGetPassword()
{
	return theApp.GetPassword();
}

CString WINAPI fnGetUserID()
{
	return theApp.GetUserID();
}

CString WINAPI fnGetExpiredDate()
{
	return theApp.GetExpiredDate();
}

void WINAPI fnSetUsePassword(BOOL bUse)
{
	theApp.SetUsePassword(bUse);
}

void WINAPI fnSetUseUserID(BOOL bUse)
{
	theApp.SetUseUserID(bUse);
}

void WINAPI fnSetUseExpiredDate(BOOL bUse)
{
	theApp.SetUseExpiredDate(bUse);
}

BOOL WINAPI fnGetUsePassword()
{
	return theApp.GetUsePassword();
}

BOOL WINAPI fnGetUseUserID()
{
	return theApp.GetUseUserID();
}

BOOL WINAPI fnGetUseExpiredDate()
{
	return theApp.GetUseExpiredDate();
}

extern "C" __declspec(dllexport) int fnShowConfigureDlgRunMain(LPCSTR szSTFile, LPCSTR szSTName)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	CDlgRunMain dlg(szSTFile, szSTName);
	int nRet = dlg.DoModal();
	AfxSetResourceHandle(hOld);

	return TRUE;
}

extern "C" __declspec(dllexport) int fnShowConfigureDlgRunMainEx(HWND hViewHandle, LPCSTR szSTFile, LPCSTR szSTName)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	CDlgRunMain dlg(hViewHandle, szSTFile, szSTName);
	int nRet = dlg.DoModal();
	AfxSetResourceHandle(hOld);

	return TRUE;
}
