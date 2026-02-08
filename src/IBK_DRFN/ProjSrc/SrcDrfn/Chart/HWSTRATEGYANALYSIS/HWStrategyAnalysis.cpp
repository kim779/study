// HWStrategyAnalysis.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "DlgAnalysis.h"
#include "../../inc/ISTManager.h" // 시스템트레이딩 관련된 컨텐츠 인터페이스
#include "../Common_ST/DataItem.h"
#include "../Common_ST/SDataItem.h"
#include "../../inc/ISysTabExChartItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE HWStrategyAnalysisDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("HWSTRATEGYANALYSIS.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(HWStrategyAnalysisDLL, hInstance))
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

		new CDynLinkLibrary(HWStrategyAnalysisDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("HWSTRATEGYANALYSIS.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(HWStrategyAnalysisDLL);
	}
	return 1;   // ok
}


///////////////////////////////////////////////////////////////////////////////
// 만든이             : 이준옥(Jun-Ok, Lee)
// Email              : alzioyes@hotmail.com
// 만든 날짜          : 2003/2/19
// 함수 이름          : WINAPI
// 리턴 타입          : HRESULT
// 리턴 타입 설명     : 
// 파라미터
//		STODINIT_IN* pIn  : 
//		STODINIT_OUT* pOut   : 
// 함수 설명          : 생성되기전에 필요한 정보를 넘긴다.
// 060721 void* pInterface 추가.
///////////////////////////////////////////////////////////////////////////////
HRESULT WINAPI fnInit(STODINIT_IN* pIn, STODINIT_OUT* pOut, void* pInterface)
{
// (2005/9/23 - Seung-Won, Bae) Load this Library to Protect Unload of Application Ending. (for Memory Leak Report)
#ifdef _DEBUG
	LoadLibrary( "HWStrategyAnalysis.dll");
#endif

	CDlgAnalysis* pDlgAnalysis = new CDlgAnalysis;
	pOut->m_pDllKey = pDlgAnalysis;

	// 인터페이스를 이용한 예제
//#ifdef _DEBUG
	if(pInterface)
	{
		ISTInterface* pSTInterface = (ISTInterface*)pInterface;
		pDlgAnalysis->m_pChartInterface = (long)pInterface;// KHD : Interface 저장
		ISTAnalysisInput* pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
		//입력 정보 저장 : KHD 2006.07.27
		pDlgAnalysis->m_pCtlList		=(CXListCtrl*)pMng->GetControl(1);//리스트 컨트롤 포인터 얻어오기.
		pMng->SetUserData(FALSE, NULL);
		pDlgAnalysis->m_bGraphDivide = FALSE;//분할 그래프만 남길지.. 성과분석을 띄울지..
		pDlgAnalysis->m_nCodeType		=pMng->GetCodeType();
		pDlgAnalysis->m_stCode			=pMng->GetCode();
		pDlgAnalysis->m_stCodeName		=pMng->GetCodeName();
		pDlgAnalysis->m_nCount			=pMng->GetCount();
		pDlgAnalysis->m_nDivideCnt		=pMng->GetDivideCnt();
		pDlgAnalysis->m_DivideState		=pMng->GetDivideState();
		pMng->GetUnitPos();
		pDlgAnalysis->m_dfDivideRate	=pMng->GetDeivideRate();
		pDlgAnalysis->m_pChartItem      = (ISysTabExChartItem*)pMng->GetControl(0);//Chart컨트롤을 가져온다.
		pDlgAnalysis->m_hTabChartEx		= pMng->GetControl(2);//ExTab을 가져온다. : KHD
		pDlgAnalysis->m_nStartType = 0;//성과분석실행
		STMNG_ITEMINFO Info;
		pMng->GetSTInfo(&Info);
		pDlgAnalysis->m_stSTPath		=Info.szSTPath +"\\"+ Info.szSTName + ".st";//전략ST파일 경로
		pDlgAnalysis->m_stSTName		= Info.szSTName + ".ST";
		pDlgAnalysis->m_stEndDate	    =pMng->GetEndDay();

		if(pDlgAnalysis->m_stSTName ==""||pDlgAnalysis->m_stCodeName=="")
		{
			AfxMessageBox("전략을 선택하지 않았습니다. 따라서 성과분석을 할 수 없습니다.");
			pDlgAnalysis->DestroyWindow();
			delete pDlgAnalysis;
			pDlgAnalysis = NULL;
			pOut->m_pDllKey = NULL;
			return S_FALSE;
		}
		//END	

	}
//#endif
	// ~인터페이스를 이용한 예제
	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////
// 만든이             : 이준옥(Jun-Ok, Lee)
// Email              : alzioyes@hotmail.com
// 만든 날짜          : 2003/2/19
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
	AfxSetResourceHandle(HWStrategyAnalysisDLL.hResource);
	AfxInitRichEdit();
	CDlgAnalysis* pDlgAnalysis = (CDlgAnalysis*)pIn->m_pInitOut->m_pDllKey;
	if(pDlgAnalysis==NULL) return S_FALSE;
	
	// >-------------------------------------------------------
	// 이 루틴에서 무엇인가 필요한 것을 생성하는 처리를 한다.
	// <-------------------------------------------------------
	if(pDlgAnalysis) 
	{
//@유진추가
		// Root Path
		pDlgAnalysis->m_strRootPath = (LPCSTR)AfxGetPctrData(10);	//참고문서[정리_PC데이터매니저.xls]
		
		// Data Path
		pDlgAnalysis->m_strDataDir = (LPCSTR)AfxGetPctrData(12);	//참고문서[정리_PC데이터매니저.xls]
//@유진추가
//@유진삭제
//		// Root Path
//		GET_ROOT_DIR(pDlgAnalysis->m_strRootPath);
//		if(pDlgAnalysis->m_strRootPath.GetLength()==0)
//		{
//			AfxMessageBox("RootPath를 얻을 수 없습니다.");
//		}
//		// Data Path
//		
//		GET_DATA_DIR(pDlgAnalysis->m_strDataDir);
//		if(pDlgAnalysis->m_strDataDir.GetLength()==0)
//		{
//			AfxMessageBox("DataPath를 얻을 수 없습니다.");
//		}
//
//		// User Directory 경로 : KHD : 경로 찾아 오는 부분 ..
//		pDlgAnalysis->m_strUserDir = pDlgAnalysis->m_strRootPath + "\\User\\" + pDlgAnalysis->m_strUserID;
//@유진삭제

		// User Directory 경로 
		IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
		pDlgAnalysis->m_strUserDir = pMng->GetEncUserDir();

		// 다이아로그 생성
		pDlgAnalysis->Create(CDlgAnalysis::IDD, CWnd::FromHandle(pIn->m_hParent));
		pDlgAnalysis->ShowWindow(SW_SHOW);
		

	}
AfxSetResourceHandle(hInst);
	return S_OK;
}

// * 삭제
HRESULT WINAPI fnUnLoad(void* dwDllKey)
{
	CDlgAnalysis* pDlgAnalysis = (CDlgAnalysis*)dwDllKey;
	if(pDlgAnalysis)
	{
		pDlgAnalysis->DestroyWindow();
		
		delete pDlgAnalysis;
		pDlgAnalysis = NULL;
	}
	return S_OK;
}

HRESULT WINAPI fnFuncCall(void* dwDllKey, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result)
{
	CDlgAnalysis* pDlgAnalysis = (CDlgAnalysis*)dwDllKey;
	if(pDlgAnalysis)
	{
		CString strFuncName;
		strFuncName = szFuncName;

		if(strFuncName == "SetFrameInfo")
		{
		}
	}
	
	return S_OK;

}

//CWnd* WINAPI CreateTradingAup(int iTradingID, CWnd* pParent)
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
	InData.m_Rect = CRect(0,0,300,400);

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
	
	if(InitOut.m_pDllKey==NULL) return NULL;
	STODCREATEINFO_IN  InData;
	STODCREATEINFO_OUT OutData;

	INIT_STODCREATEINFO_IN((&InData));
	InData.m_pInitOut = &InitOut;
	InData.m_hParent = pParent->GetSafeHwnd();
	InData.m_nID = iTradingID;
	InData.m_Rect = CRect(0,0,300,400);

	hr = fnLoad(&InData, &OutData);
	CWnd* pWnd = (CWnd*)InData.m_pInitOut->m_pDllKey;
	return pWnd;
}

//extern "C" __declspec(dllexport) void DestroyTradingAup(CWnd *pWnd)
extern "C" __declspec(dllexport) void DESTROY_TRADING(CWnd *pWnd)
{
	if ( pWnd )
	{
		
		fnUnLoad(pWnd);
	}
}

// ---------------------------------------------------------------------------------
// extern "C" __declspec(dllexport) BOOL SA_ApplyChart(LONG dwChartOcx, CListCtrl* pDivList, LONG dwKey)
// 멀티차트의 시스텝탭에서 "적용" 버튼을 눌렀을 때 호출함.
// 차트에 분할적용 및 리스트에 분할내용을 표시하기 위해서 호출함.
// Start : 06.07.28 alzio
// dwReserved1, dwReserved2 : 추후 사용을 위해서 만들어놓은 변수임.
// ---------------------------------------------------------------------------------
extern "C" __declspec(dllexport) CWnd* SA_ApplyChart(LONG dwChartOcx, CListCtrl* pDivList, LONG dwInfo, LONG pInterface, LONG dwReserved2)
{
	STMNG_ITEMINFO* pInfo = (STMNG_ITEMINFO*)dwInfo;
	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut, (void*)pInterface);
	if(InitOut.m_pDllKey==NULL) return NULL;

	CDlgAnalysis* pDlgAnalysis;
	pDlgAnalysis =  (CDlgAnalysis*)InitOut.m_pDllKey;
	

	if(pDlgAnalysis==NULL) return NULL;

	IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);

	// Root Path
	pDlgAnalysis->m_strRootPath = pMainInfo->GetRootDir();
		
	// Data Path
	pDlgAnalysis->m_strDataDir = pMainInfo->GetDataDir();

	// User Path
	pDlgAnalysis->m_strUserDir = pMainInfo->GetEncUserDir("chart");

	// 다이아로그 생성
	pDlgAnalysis->m_bGraphDivide = TRUE;
	pDlgAnalysis->m_nStartType =1;// 0:성과분석, 1:적용실행
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(HWStrategyAnalysisDLL.hResource);
	pDlgAnalysis->Create(CDlgAnalysis::IDD);
	pDlgAnalysis->ShowWindow(SW_HIDE);
	
	AfxSetResourceHandle(hInst);


	return pDlgAnalysis;
}
