// HWStrategyConfigureWizard.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DlgStrategyConfigure.h"
#include "../Common_ST/Commonlib.h"
#include "../Common_ST/_STLoadManager.h"
#include "../Common_ST/STGfxOutBarCtrl.h"
#include "DlgSysVarOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE HWStrategyConfigureWizardDLL = { NULL, NULL };


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("HWSTRATEGYCONFIGUREWIZARD.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(HWStrategyConfigureWizardDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(HWStrategyConfigureWizardDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("HWSTRATEGYCONFIGUREWIZARD.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(HWStrategyConfigureWizardDLL);
	}
	return 1;   // ok
}


///////////////////////////////////////////////////////////////////////////////
// 만든이             : 
// Email              : 
// 만든 날짜          : 
// 함수 이름          : WINAPI
// 리턴 타입          : HRESULT
// 리턴 타입 설명     : 
// 파라미터
//		STODINIT_IN* pIn  : 
//		STODINIT_OUT* pOut   : 
// 함수 설명          : 생성되기전에 필요한 정보를 넘긴다.
///////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI fnInit(STODINIT_IN* pIn, STODINIT_OUT* pOut, void* pInterface)
{
	CDlgStrategyConfigure* pDlgConfigure = new CDlgStrategyConfigure;
	pOut->m_pDllKey = pDlgConfigure;
	pDlgConfigure->m_pSTRunMng = (ISTRunManager*)pInterface;
	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// 만든이             : 
// Email              : 
// 만든 날짜          : 
// 함수 이름          : WINAPI
// 리턴 타입          : HRESULT
// 리턴 타입 설명     : 
// 파라미터
//		STODCREATEINFO_IN* pIn  : 
//		STODCREATEINFO_OUT* pOut   : 
// 함수 설명          : 생성에 필요한 정보를 다 넘긴 상태에서 Create에 해당하는 루틴처리
/////////////////////////////////////////////////////////////////////////////////extern "C" _declspec(dllexport) HRESULT FPLoad(SZSTODCREATEINFO_IN* pIn, SZSTODCREATEINFO_OUT* pOut)
HRESULT WINAPI fnLoad(STODCREATEINFO_IN* pIn, STODCREATEINFO_OUT* pOut)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(HWStrategyConfigureWizardDLL.hResource);
	AfxInitRichEdit();
	CDlgStrategyConfigure* pDlgConfigure = (CDlgStrategyConfigure*)pIn->m_pInitOut->m_pDllKey;
	// >-------------------------------------------------------
	// 이 루틴에서 무엇인가 필요한 것을 생성하는 처리를 한다.
	// <-------------------------------------------------------
	if(pDlgConfigure)
	{
		// UserID
		// 되도록이면 ID도 멤버로 가지고 있지말고 실제 필요할 때 조회할 것을 권함.
		pDlgConfigure->m_strUserID = (LPCSTR)AfxGetPctrData(0);	//참고문서[정리_PC데이터매니저.xls]

		// Root Path
		pDlgConfigure->m_strRootPath = (LPCSTR)AfxGetPctrData(10);	//참고문서[정리_PC데이터매니저.xls]
		
		// Data Path
		pDlgConfigure->m_strDataDir = (LPCSTR)AfxGetPctrData(12);	//참고문서[정리_PC데이터매니저.xls]

		// Image Path
//		pDlgConfigure->m_strImageDir = (LPCSTR)AfxGetPctrData(14);
		pDlgConfigure->m_strImageDir = pDlgConfigure->m_strRootPath + "\\Image";

//@유진삭제
//		// User Directory 경로 
		IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);

		//@Solomon pDlgConfigure->m_strUserDir = pMng->GetEncUserDir();
		// User디렉토리중 Chart폴더를 기본폴더로 한다.
		pDlgConfigure->m_strUserDir.Format("%s", pMng->GetEncUserDir("chart"));

//		pDlgConfigure->m_strUserDir = (LPCSTR)AfxGetPctrData(17);	//참고문서[정리_PC데이터매니저.xls]
//@유진삭제

		// Dialog 생성
		pDlgConfigure->Create(CDlgStrategyConfigure::IDD, CWnd::FromHandle(pIn->m_hParent));
		pDlgConfigure->FormInit();					// socket, drds 관련 초기화
		pDlgConfigure->FormLoad();					// 초기화 후에 행해질 작업 처리
		pDlgConfigure->ShowWindow(SW_SHOW);
	}

	AfxSetResourceHandle(hInst);
	return S_OK;
}

HRESULT WINAPI fnUnLoad(void* dwDllKey)
{
	CDlgStrategyConfigure* pDlgConfigure = (CDlgStrategyConfigure*)dwDllKey;
	if(pDlgConfigure)
	{
		pDlgConfigure->DestroyWindow();
		
		delete pDlgConfigure;
		pDlgConfigure = NULL;
	}
	return S_OK;
}

HRESULT WINAPI fnFuncCall(void* dwDllKey, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result)
{
	return S_OK;
}

extern "C" __declspec(dllexport) CWnd* CREATE_TRADING(int iTradingID, CWnd* pParent)
{
	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut, NULL);

	STODCREATEINFO_IN  InData;
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;
	InData.m_Rect = CRect(0,0,350,400);

	hr = fnLoad(&InData, &OutData);
	CWnd* pWnd = (CWnd*)InData.m_pInitOut->m_pDllKey;
	return pWnd;
}

extern "C" __declspec(dllexport) CWnd* CREATE_TRADING_EX(int iTradingID, CWnd* pParent, void* pInterface)
{
	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut, pInterface);

	STODCREATEINFO_IN  InData;
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;
	InData.m_Rect = CRect(0,0,350,400);

	hr = fnLoad(&InData, &OutData);
	CWnd* pWnd = (CWnd*)InData.m_pInitOut->m_pDllKey;
	return pWnd;
}


extern "C" __declspec(dllexport) void DESTROY_TRADING(CWnd *pWnd)
{
	if ( pWnd )
	{
		fnUnLoad(pWnd);
	}
}

// dwType은 현재 사용하지는 않음. default는 0으로 함.
// 외부에서 전략과 관련된 함수를 호출 할때 사용함.
// FPSCW_GetInterfaceOfStrategy* pFunc = (FPSCW_GetInterfaceOfStrategy*)GetProcAddress(hInst,_T("SCW_GetInterfaceOfStrategy"));
// ISTLoadManager* pSTLoadMng = (ISTLoadManager*)pFunc(0); // AfxGetPctrInterface(111);
// if(pSTLoadMng) {
//		pSTLoadMng->LoadList(....);
//  }
extern "C" __declspec(dllexport) LONG SCW_GetInterfaceOfStrategy(LONG dwType/*=0*/)
{

	LONG dwInterface = NULL;
	
	if(dwType==0)
	{
		CSTLoadManager* pItem = new CSTLoadManager;
		dwInterface = (LONG)pItem;
	}
	return dwInterface;
}

extern "C" __declspec(dllexport) void SCW_DeleteInterfaceOfStrategy(LONG dwKey, LONG dwType)
{
	if(dwType==0)
	{
		CSTLoadManager* pItem = (CSTLoadManager*)dwKey;
		if(pItem) delete pItem; pItem=NULL;
	}
}

// nType 0 : 생성
//		 1 : 삭제
extern "C" __declspec(dllexport) LONG SCW_IGfxOutBarCtrl(int nType, long dwValue, long dwReserved1)
{
	if(nType==0)
	{
		CImpl_IGfxOutBarCtrl* pOBC = new CImpl_IGfxOutBarCtrl;
		return (LONG)pOBC;
	}
	if(nType==1)
	{
		CImpl_IGfxOutBarCtrl* pOBC = (CImpl_IGfxOutBarCtrl*)dwValue;
		if(pOBC) delete pOBC;
		pOBC = NULL;
		return 0L;
	}
	return 0L;
}

extern "C" __declspec(dllexport) int DoModal_DlgSysVarOption(CStringList* p_dwInput, LONG dwOutput)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(HWStrategyConfigureWizardDLL.hResource);

	CDlgSysVarOption dlg;
	dlg.SetParam(p_dwInput, dwOutput);

	int nRet = dlg.DoModal();

	AfxSetResourceHandle(hInst);

	return nRet;
}
