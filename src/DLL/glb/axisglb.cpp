// axisglb.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "axisglb.h"
#include "glbDlg.h"

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
// CAxisglbApp

BEGIN_MESSAGE_MAP(CAxisglbApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisglbApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisglbApp construction

CAxisglbApp::CAxisglbApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisglbApp object

CAxisglbApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisglbApp initialization

BOOL CAxisglbApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}


__declspec(dllexport) long APIENTRY axGetGLB(char* path, char* sndB, int sndL, char* rcvB, int maxL, bool staff)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CGlbDlg* glb = new CGlbDlg();
	glb->Init(path, sndB, sndL, staff);
	if (glb->DoModal() == IDOK)
	{
		int	size = min(maxL, glb->m_returns.GetLength());
		CopyMemory(rcvB, glb->m_returns, size);
		delete glb;
		return size;
	}
	delete glb;
	return 0;
}

__declspec(dllexport) long APIENTRY axGetGLB_Index(char* path, char* sndB, int sndL, char* rcvB, int maxL, int iIndex, bool staff)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CGlbDlg* glb = new CGlbDlg();
	glb->Init(path, sndB, sndL, staff);
	glb->SetIpIndex(iIndex);
	if (glb->DoModal() == IDOK)
	{
		int	size = min(maxL, glb->m_returns.GetLength());
		CopyMemory(rcvB, glb->m_returns, size);
		delete glb;
		return size;
	}
	delete glb;
	return 0;
}
