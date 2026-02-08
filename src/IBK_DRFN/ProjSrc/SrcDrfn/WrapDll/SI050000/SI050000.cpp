// SI050000.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SI050000.h"

//#include "../Common/MainWnd.h"
#include "ViewWrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[2] = __FILE__;
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
// CSI050000App

BEGIN_MESSAGE_MAP(CSI050000App, CWinApp)
	//{{AFX_MSG_MAP(CSI050000App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSI050000App construction

CSI050000App::CSI050000App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_pWinixParam = NULL;
	m_pPcTrHelper = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSI050000App object

CSI050000App theApp;

// __declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
// {
// 	AFX_MANAGE_STATE(AfxGetStaticModuleState());
// 	
// 	CMainWnd *pMainWnd = new CMainWnd(parent);
// 	
// 	if (!pMainWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
// 		CRect(0, 0, 1000, 600), parent, 100, NULL))
// 	{
// 		delete pMainWnd;
// 		return NULL;
// 	}	
// 	
// 	return pMainWnd;
// }

void gGetCaptionTitle(HWND hWnd, LPCSTR szScreenNo)
{
	//@100204-alzioyes. winix에서 처리한다고 해서 DRFN에서는 처리안하는 것으로 수정함.
	return ;
// 	if(!IsWindow(hWnd)) FALSE;
// 	
// 	char* pHomeCC = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
// 	CString szCfg;
// 	szCfg.Format("%s\\%s", pHomeCC, "\\Hanaro\\PctrMng.cfg");
// 	char aTitle[MAX_PATH] = {0,};
// 
// 	if(::GetPrivateProfileString("Title", szScreenNo, "", aTitle, MAX_PATH, szCfg)>0)
// 	{
// 		::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, titleCC), (LPARAM)aTitle);
// 	}
}

//#include <winhand_.h>
//100530:동일화면 다르게 저장/읽기 <--

// Before -->
//KHD : 현재 지원이 안되서 이함수로 대체 
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	{
		CString szDllName;
		szDllName.Format("%4.4s", &theApp.m_pszExeName[2]);
		theApp.m_szDllName = szDllName;

		HWND hWnd = pParent->GetSafeHwnd();
		gGetCaptionTitle(hWnd, szDllName);
	}

	CViewWrap* pWrapWnd = new CViewWrap();
	CRect rect;
	pParent->GetWindowRect( &rect );
	pParent->ScreenToClient( &rect );

	if( !pWrapWnd->Create( NULL, NULL, WS_CHILD|WS_VISIBLE, rect, pParent, 999 ) )
		return NULL;

	return pWrapWnd;
}

// Before <--

// axCreate에서 파라미터만 더 추가된 것임.
// After -->
//KHD : 현재 IBK 이 함수 지원되지 않음 ( 복수저장 부분)  
__declspec(dllexport) CWnd* WINAPI axCreateX(CWnd *pParent, void* pData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
	{
		CString szDllName;
		szDllName.Format("%4.4s", &theApp.m_pszExeName[2]);
		theApp.m_szDllName = szDllName;
		
		HWND hWnd = pParent->GetSafeHwnd();
	
		gGetCaptionTitle(hWnd, szDllName);
	}
	
	CViewWrap* pWrapWnd = new CViewWrap();
	
	CRect rect;
	pParent->GetWindowRect( &rect );
	pParent->ScreenToClient( &rect );
	
	pWrapWnd->SetStartInfo(pData);
	if( !pWrapWnd->Create( NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, pParent, 999 ) )
		return NULL;
	
	return pWrapWnd;
}

// After <--
//100530:동일화면 다르게 저장/읽기 <--

BOOL CSI050000App::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
//	m_pWinixParam = (void*)new _param;

	m_pPcTrHelper = new CPCTrMngHelper;

	DOChartOCXRegister();

	return CWinApp::InitInstance();
}

int CSI050000App::ExitInstance() 
{
	if(m_pPcTrHelper) delete m_pPcTrHelper; m_pPcTrHelper=NULL;

	if(m_pWinixParam) 
	{
// 		_param* pAppParamInfo = (_param*)m_pWinixParam;
// 		delete pAppParamInfo;
		m_pWinixParam = NULL;
	}
	
	return CWinApp::ExitInstance();
}

#include "../../inc/ISystemUtilMng.h"
#include "../../ForSite/FS_Env.h"
BOOL CSI050000App::DOChartOCXRegister()
{
	//자동등록을 원하지 않으면 셋팅해요.
	BOOL bReg = (BOOL)AfxGetApp()->GetProfileInt("STBasic", "ChartRegist", 1);
	if(bReg==0) return TRUE;
	
	CString szDir, szCFG;
	GetDir_DrfnData(szDir);
	szCFG = szDir + "\\chartmetatable.dat";
	char aProgID[100]={0,}, aOcxName[100]={0,};
	if(::GetPrivateProfileString("Chart Version", "ProgID", "", aProgID, 100, szCFG)==0)
		return FALSE;
	
	if(::GetPrivateProfileString("Chart Version", "ProgName", "", aOcxName, 100, szCFG)==0)
		return FALSE;
	
	CLSID clsid;
	ISystemUtilManager2* pMng = (ISystemUtilManager2*)AfxGetPctrInterface(UUID_ISystemUtilManager);
	if(pMng)
	{
		GetDir_DrfnBin(szDir);		
		CString szOcxName;
		szOcxName = szDir + "\\" + aOcxName;
		
		if(pMng->IsRegisteredEx(aProgID, clsid, aOcxName, szOcxName)==FALSE)
		{			
			pMng->DLLRegister(szOcxName);
		}
		return pMng->IsRegistered(aProgID, clsid);
	}
	return TRUE;
}
