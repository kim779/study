// CandleSearch.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ScreenWnd.h"
#include "CandleSearch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCandleSearchApp, CWinApp)
//{{AFX_MSG_MAP(CChartSectionSearchApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCandleSearchApp::CCandleSearchApp()
{
}

CCandleSearchApp theApp;
HINSTANCE g_hInstance;

#include "../../inc/ISystemUtilMng.h"
BOOL gDOChartOCXRegister()
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

BOOL CCandleSearchApp::InitInstance()
{
	g_hInstance = theApp.m_hInstance;
	gDOChartOCXRegister();
	
	return CWinApp::InitInstance();
}

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

extern "C" __declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent)
{
	HINSTANCE hOld = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);

	//20120112 이문수 리소스체크기능추가 >>
	BOOL bResult	= CUtils::CheckResource();
	if(!bResult)	return FALSE;
	//20120112 이문수 <<

	CScreenWnd* pWnd = new CScreenWnd(parent);
	if(pWnd->Create(NULL, "CANDLESEARCH", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), parent, 100) == FALSE)
		return NULL;
	
	AfxSetResourceHandle(hOld);

	return pWnd;
}