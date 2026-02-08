// MiniUtil.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MiniUtil.h"
#include "DlgMiniUtil.h"

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
// CMiniUtilApp

BEGIN_MESSAGE_MAP(CMiniUtilApp, CWinApp)
	//{{AFX_MSG_MAP(CMiniUtilApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniUtilApp construction

//extern "C" __declspec(dllexport) UINT MiniUtil_DoModal(long lArMiniSetupInfo, CWnd* pParent);
extern "C" __declspec(dllexport) int DoModal_MiniUtil(LPCTSTR szPropertyIn, LPSTR szPropertyOut);

CMiniUtilApp::CMiniUtilApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMiniUtilApp object

CMiniUtilApp theApp;

//UINT MiniUtil_DoModal(long lArMiniSetupInfo, CWnd* pParent)
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	
//
//	CDlgMiniUtil dlg(lArMiniSetupInfo, pParent);
//	int nRet = dlg.DoModal();
//
//	return nRet;
//}

int DoModal_MiniUtil(LPCTSTR szPropertyIn, LPSTR szPropertyOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CDlgMiniUtil dlg(szPropertyIn);
	int nRet = dlg.DoModal();
	if(nRet == IDOK)
		memcpy(szPropertyOut, (LPCTSTR)dlg.m_strPropertyInfoOut, dlg.m_strPropertyInfoOut.GetLength());
	else
		memcpy(szPropertyOut, (LPCTSTR)dlg.m_strPropertyInfo, dlg.m_strPropertyInfo.GetLength());

	return nRet;
}
