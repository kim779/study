// ChartPatternAddin.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ChartPatternAddin.h"

#include "./Include_Addin/_IAddInManager.h"			// for IAddInManager
#include "./Include_Addin/I423611/_IChartAddIn.h"		// for IChartAddIn
#include "SiteCustomAddin.h"										// for CSiteCustomAddin


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
// CChartPatternAddinApp

BEGIN_MESSAGE_MAP(CChartPatternAddinApp, CWinApp)
	//{{AFX_MSG_MAP(CChartPatternAddinApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartPatternAddinApp construction

CChartPatternAddinApp::CChartPatternAddinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChartPatternAddinApp object

CChartPatternAddinApp theApp;
HINSTANCE g_hInstance;


// (2004.10.08, 배승원) Chart AddIn DLL의 Interface Version을 지정하는 Macro를 정의한다.
#define _CHART_ADDIN_IVERSION( nOVersion)										\
	int		g_nChartAddInVersion = 1##nOVersion - 1000000;						\
	char	g_szChartAddInIVersion[ 50] = "ChartAddIn Version : " #nOVersion;
// (2004.10.08, 배승원) Chart AddIn DLL의 Version을 Static String으로 저장하여 DLL Module File에 명시되도록 한다.
//		또한 int로도 관리하여 GetAddInIVersion에서 이용할 수 있도록 한다.
_CHART_ADDIN_IVERSION( 423611)

// [04/10/07] Chart AddIn DLL의 Version을 알려주는 Interface를 제공한다.
ADDIN_API int GetAddInIVersion(void)
{
	return g_nChartAddInVersion;
}

// [04/10/07] Chart AddIn DLL의 Version을 알려주는 Interface를 제공한다.
ADDIN_API IChartAddIn *InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
{
	// 0. NULL Pointer를 확인한다.
	if( !p_pIAddInManager) return NULL;

	// 1. Chart Ocx의 Interface를 구한다.
	IChartOCX *pIChartOCX = p_pIAddInManager->GetIChartOCX423611();
	if( !pIChartOCX) return NULL;

	// 2. Chart Ocx의 Interface를 이용하여 AddIn Object를 생성한다.
	IChartAddIn *pIChartAddIn = new CSiteCustomAddin( pIChartOCX, p_pIAddInDllBase, p_bOverExclusive);
	pIChartAddIn->AddRef();
	// 3. Chart Ocx의 Interface를 Release한다.
	pIChartOCX->Release();

	// (2004.10.13, 배승원) 생성한 AddIn Object에 기본 Chart Manager들이 제대로 등록되지 않으면,
	//		AddIn 생성을 취소시킨다.
	if( !pIChartAddIn->HasFullManager())
	{
		pIChartAddIn->Release();
		pIChartAddIn = NULL;
	}

	// 4. 생성한 AddIn Object의 Interface를 Return한다.
	return pIChartAddIn;
}

BOOL CChartPatternAddinApp::InitInstance() 
{
	g_hInstance = theApp.m_hInstance;	
	
	return CWinApp::InitInstance();
}

