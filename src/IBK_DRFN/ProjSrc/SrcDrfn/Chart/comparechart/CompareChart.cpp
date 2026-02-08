// CompareChart.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CompareChart.h"
#include "StdDialog.h"

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
// CCompareChartApp

BEGIN_MESSAGE_MAP(CCompareChartApp, CWinApp)
	//{{AFX_MSG_MAP(CCompareChartApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompareChartApp construction

CCompareChartApp::CCompareChartApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCompareChartApp object

CCompareChartApp theApp;
HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CCompareChartApp initialization

BOOL CCompareChartApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();
	g_hInstance = theApp.m_hInstance;
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

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

const UINT RMSG_GETUNIQUEID = ::RegisterWindowMessage(_T("RMSG_GETUNIQUEID"));
const UINT RMSG_GETMAPTR	= ::RegisterWindowMessage(_T("RMSG_GETMAPTR"));				// 화면번호 요청

HRESULT WINAPI fnInit(STODINIT_IN* pIn, STODINIT_OUT* pOut)
{
	CString strValue;
	CString strParam1, strParam2, strParam3;

	CStdDialog* pDialog =  new CStdDialog;

	if(!strParam1.IsEmpty())
		pDialog->SetParameters1(strParam1);
	if(!strParam2.IsEmpty())
		pDialog->SetParameters2(strParam2);
	if(!strParam3.IsEmpty())
		pDialog->SetParameters3(strParam3);

	pOut->m_pDllKey = pDialog;
	pOut->m_pPlatformObj = &pDialog->m_xPlatformObj;

	return S_OK;
}

HRESULT WINAPI fnLoad(STODCREATEINFO_IN* pIn, STODCREATEINFO_OUT* pOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStdDialog* pDialog = (CStdDialog*)pIn->m_pInitOut->m_pDllKey;
	if(pDialog)
	{
		pDialog->m_hPlatform = theApp.m_hPlatform;
		pDialog->m_hViewInMain = pIn->m_hParent;
		pDialog->m_strScreenNo.Format("%04d", pIn->m_nID);
//		pDialog->m_strMapFileName.Format("%s", pIn->szMapFileName);

		pDialog->m_nScreenType = CStdDialog::Screen_fluctuation;

		if(pDialog->m_strScreenNo == "7123")
			pDialog->m_nScreenType = CStdDialog::Screen_Compchart;//복합비교 
//		else if(pDialog->m_strScreenNo == "0520")
//			pDialog->m_nScreenType = CStdDialog::Screen_ELWChart;
		else if(pDialog->m_strScreenNo == "7124")
			pDialog->m_nScreenType = CStdDialog::Screen_ForeignChart; //상대비교
		else if(pIn->m_nID==nScreenID_0424)
		//else if(pDialog->m_strScreenNo == "0424")
			pDialog->m_nScreenType = CStdDialog::Screen_fluctuation;

		pDialog->m_pShareInputData->m_nScreenType = pDialog->m_nScreenType;

		pDialog->Create(IDD_STD_DIALOG, CWnd::FromHandle(pIn->m_hParent));		
//		pDialog->SetWindowPos(&CWnd::wndBottom, 0, 0, pDialog->m_nWidthOfDlg, pDialog->m_nHeightOfDlg,SWP_SHOWWINDOW|SWP_NOZORDER);
//		pDialog->Resize();
		pDialog->ShowWindow(SW_SHOW);

		pOut->m_pWndObj = &(pDialog->m_xWndObj);
	}

	return S_OK;
}

HRESULT WINAPI fnUnLoad(void* dwDllKey)
{
	CStdDialog* pDialog = (CStdDialog*)dwDllKey;
	if(pDialog)
	{
		pDialog->m_xGSharedCli.RemoveSharedNameToServer();
		pDialog->DestroyWindow();
		delete pDialog;
		pDialog=NULL;
	}
	return S_OK;
}

HRESULT WINAPI fnFuncCall(void* dwDllKey, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result)
{
	CStdDialog* pDlgChart = (CStdDialog*)dwDllKey;
	if(pDlgChart)
	{
		CString strFuncName = szFuncName;
		CString strParam = szParam;
		if(strFuncName == _T("SetFrameInfo"))
		{			
			CString strValue;
			CString strValueX;
			CString strValueY;
			long lX = 0;
			long lY = 0;
			long lXOld = 0;
			long lYOld = 0;
			long lPos = -1;
			if(strParam.IsEmpty()) return S_OK;
			for(long lIndex=0;lIndex<6;lIndex++)
			{
				lPos = strParam.Find(',');
				if(lPos==-1)	
				{
					lPos = strParam.GetLength();
				}
				strValue = strParam.Left(lPos);
				strParam.Delete(0,lPos+1);

				if(lIndex==0)
				{
					lXOld = strValue[0] - 48;
					lYOld = strValue[1] - 48;					
				}				
				else if(lIndex==3)
				{
					lX = strValue[0] - 48;
					lY = strValue[1] - 48;					
				}
				else if(lIndex==4)
				{
					strValueX = strValue;
				}
				else if(lIndex==5)
				{
					strValueY = strValue;
				}
			}
			if(lXOld>lX)	// smaller
			{
				pDlgChart->SetSmallView();
			}
			if(lXOld<lX)	// bigger
			{
				pDlgChart->SetSmallView(FALSE);
			}
			pDlgChart->SetWindowPos(&CWnd::wndBottom,0,0,atol(strValueX),atol(strValueY),SWP_SHOWWINDOW|SWP_NOZORDER);
			pDlgChart->Resize();
		}
		else if(strFuncName == _T("SetCodeInfo"))
		{
			if(strParam.IsEmpty()) return S_OK;
			pDlgChart->InputCodeFromOutSide(strParam);
		}
	}	
	return S_OK;
}

long WINAPI fnFormResize(void* pDialog, int width, int height)
{
	CStdDialog* pDlgChart = (CStdDialog*)pDialog;
	pDlgChart->SetWindowPos(&CWnd::wndBottom,0,0,width,height,SWP_SHOWWINDOW|SWP_NOZORDER);
	return 1;
}

extern "C" __declspec(dllexport) CWnd* CREATE_CHARTSCREEN(int iTradingID, CWnd* pParent, HWND hPlatform)
{
	theApp.m_hPlatform = hPlatform;

	STODINIT_IN  InitIn;
	STODINIT_OUT InitOut;

	INIT_STODINIT_IN((&InitIn));
	INIT_STODINIT_OUT2(InitOut);

	HRESULT hr = fnInit(&InitIn, &InitOut);

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

extern "C" __declspec(dllexport) void DESTROY_CHARTSCREEN(CWnd *pWnd)
{
	CStdDialog* pDlgChart = (CStdDialog*)pWnd;
	if(pDlgChart)
	{
		pDlgChart->DestroyWindow();
		delete pDlgChart;
	}
}

extern "C" __declspec(dllexport) LRESULT SETKEYCODE_TRADING(CWnd *pWnd, WPARAM wp, LPARAM lp)
{
	CStdDialog* pDialog = (CStdDialog*)pWnd;
	if(pDialog)
	{
		pDialog->SendMessage(UMSG_UPDATECODE, wp, lp);
		return 1L;
	}
	return 0L;
}

extern "C" __declspec(dllexport) LRESULT DROPKEY_TRADING(CWnd *pWnd, WPARAM wp, LPARAM lp)
{
	CStdDialog* pDialog = (CStdDialog*)pWnd;
	if(pDialog)
	{
		pDialog->SendMessage(UMSG_DROPCODE, wp, lp);
		return 1L;
	}
	return 0L;
}