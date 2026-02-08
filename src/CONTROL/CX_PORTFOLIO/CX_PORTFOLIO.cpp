// CX_PORTFOLIO.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_PORTFOLIO.h"
#include "Portfolio.h"
#include "CInterWnd.h"

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
// CCX_PORTFOLIOApp

BEGIN_MESSAGE_MAP(CCX_PORTFOLIOApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_PORTFOLIOApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_PORTFOLIOApp construction

CCX_PORTFOLIOApp::CCX_PORTFOLIOApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_PORTFOLIOApp object

CCX_PORTFOLIOApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_PORTFOLIOApp initialization

BOOL CCX_PORTFOLIOApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}


__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPortfolio* pCtrl = new CPortfolio(parent, (struct _param*)pParam);
	if (!pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), parent, 100))
	{
		TRACE("CPortfolio Create Failed\n");
		delete pCtrl;
		return 0;
	}

	return pCtrl;
}

UINT AFXAPI HashData(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

#include "../../H/interMSG.h"
#include "CDlgInter.h"
#include "CInterWnd.h"

__declspec(dllexport) CString WINAPI axReadGroup(long wnd, char* sUser)
{
	CString str;
	/*CInterWnd* pwnd = new CInterWnd(sRoot, sUser);
	if (!pwnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), nullptr, 100))
	{
		TRACE("CPortfolio Create Failed\n");
		delete pwnd;
		return "";
	}

	CString strRoot, strUser;
	strRoot.Format("%s", sRoot);
	char buff[MAX_PATH]{};
	sprintf(buff, "%08u", HashData((LPCSTR)strRoot));

	char chFileName[500];
	GetModuleFileName(NULL, chFileName, MAX_PATH);
	strRoot.Format("%s", chFileName);
	strRoot.TrimRight();
	int ifind = strRoot.ReverseFind('\\');
	strRoot = strRoot.Left(ifind);
	strRoot.Replace("exe", "user");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s.ini", strRoot, buff, buff);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return "";

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
	CWnd* m_pMainFrame = CWnd::FromHandle(hWnd);

	CString strHwnd;
	strHwnd.Format("%d", nullptr);
	::SendMessage(hWnd, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_INTERGROUP, 0), (LPARAM)strHwnd.operator LPCSTR());*/
	CDlgInter dlg((CWnd*)wnd, sUser);
	if (dlg.DoModal() == IDOK)
	{
		TRACE("!231");
	}
	/*CDlgInter dlg(nullptr, sRoot);
	
	if (dlg.DoModal() == IDOK)
	{
		TRACE("!231");
	}
	int ierror = GetLastError();
	TRACE("!231");*/
	//CopyMemory(pdata, dlg.m_pdata, szBUFF);
	//CInterWnd* pInterWnd = new CInterWnd( sRoot,  sUser);


	/*CString strRoot, strUser;
	strRoot.Format("%s", sRoot);
	char buff[MAX_PATH]{};
	sprintf(buff, "%08u", HashKey((LPCSTR)strRoot));

	char chFileName[500];
	GetModuleFileName(NULL, chFileName, MAX_PATH);
	strRoot.Format("%s", chFileName);
	strRoot.TrimRight();
	int ifind = strRoot.ReverseFind('\\');
	strRoot = strRoot.Left(ifind);
	strRoot.Replace("exe", "user");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s.ini", strRoot, buff,buff);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return "";

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
	CWnd* m_pMainFrame = CWnd::FromHandle(hWnd);*/
	

	return "";
}