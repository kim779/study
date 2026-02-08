// wDrChart.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "wDrChart.h"

#include "LoadPropDlgLib.h"

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
// CWDrChartApp

BEGIN_MESSAGE_MAP(CWDrChartApp, CWinApp)
	//{{AFX_MSG_MAP(CWDrChartApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWDrChartApp construction

CWDrChartApp::CWDrChartApp()
{
	m_pPcTrHelper = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWDrChartApp object

CWDrChartApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWDrChartApp initialization

BOOL CWDrChartApp::InitInstance()
{
	COleObjectFactory::RegisterAll();

	if (m_pPcTrHelper == NULL)
		m_pPcTrHelper = new CPCTrMngHelper;
	
	DOChartOCXRegister();

	return TRUE;
}

int CWDrChartApp::ExitInstance() 
{
	if (m_pPcTrHelper)
	{
		delete m_pPcTrHelper;
		m_pPcTrHelper = NULL;
	}

	return CWinApp::ExitInstance();
}

CWnd* CWDrChartApp::CreateChart(CWnd* pParent, _param* pInfo)
{
	AfxEnableControlContainer();

	CDrChartCtl *pChart = new CDrChartCtl;
	CRect rect = pInfo->rect;
	pChart->SetCtlInfo(pInfo);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS;
	pChart->Create(NULL, _T(""), dwStyle, rect, pParent, ID_CHART_WRAPWND);

	return (CWnd*)pChart;
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

//	Map에서 호출할때 사용하는 초기 생성함수.
__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return theApp.CreateChart(parent, (_param*)cInfo);
}

//	DLL화면에서 호출할때 사용하는 초기 생성함수.
__declspec(dllexport) CWnd* WINAPI axCreate2(CWnd *parent, CWnd* pWizard, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return theApp.CreateChart(parent, (_param*)cInfo);
}

//	디자인 모드일때 설청창 띄우는 함수.
__declspec(dllexport) char* WINAPI axPropDlg(const char* pszOpt, const char* pszMtblPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	AfxEnableControlContainer();

	int nChartType = 0; //0:HTS, 2: 3:해외선물인지.
	{
		CString szDllName;
		szDllName.Format("%s", &theApp.m_pszExeName);
		if(szDllName.CompareNoCase("CX_DRFNFCHART")==0)
			nChartType = 3;
	}

	char *pszRet = NULL;

	CPropDlgLib	lib;
	lib.LoadLib();
	CWnd *pParent = theApp.GetMainWnd();
	pszRet = lib.FnAxModalPropDlg(pParent, pszOpt, pszMtblPath);

	if (pszRet == NULL)
	{
		lib.UnLoadLib();
		return _T("");
	}

	CString strData;
	strData.Format(_T("%s"), pszRet);

	lib.UnLoadLib();

	return (LPTSTR)(LPCTSTR)strData;
}

#include "../../inc/ISystemUtilMng.h"
#include "../../ForSite/FS_Env.h"
BOOL CWDrChartApp::DOChartOCXRegister()
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
