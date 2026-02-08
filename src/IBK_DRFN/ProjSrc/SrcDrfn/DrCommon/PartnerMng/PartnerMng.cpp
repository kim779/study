// PartnerMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PartnerMng.h"

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
// CPartnerMngApp

BEGIN_MESSAGE_MAP(CPartnerMngApp, CWinApp)
	//{{AFX_MSG_MAP(CPartnerMngApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartnerMngApp construction

CPartnerMngApp::CPartnerMngApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	for(int i=0; i<MAX_Partner_INTERFACE; i++) m_uaInterface[i].pInterface=NULL;
	m_pPctrGateMng = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPartnerMngApp object

BOOL CPartnerMngApp::InitInstance() 
{
	g_pDRDebuger = new CDRDebugerLoader();
	// TODO: Add your specialized code here and/or call the base class
	m_uaInterface[LUID_IPartnerManager].pInterface = (LONG)&m_PtSvrMng;
	m_uaInterface[LUID_IChartBozoMsgManager].pInterface = (LONG)&m_ChartBozoMng;
	m_uaInterface[LUID_IPartnerUtilManager].pInterface = (LONG)&m_PtUtilMng;

	//m_pPctrGateMng = (IGateManager*)AfxGetPctrInterface(UUID_IGateManager);

	return CWinApp::InitInstance();
}

int CPartnerMngApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(g_pDRDebuger) delete g_pDRDebuger; g_pDRDebuger=NULL;
	return CWinApp::ExitInstance();
}

typedef struct
{
	CString szUpLimit, szDnLimit;
}stLocalDataA;

void CPartnerMngApp::DoMapStrObj_UpDnInfo(LPCSTR szKey, LPCSTR sUpLimit, LPCSTR sDnLimit)
{
	CString strValue;
	strValue.Format(_T("%s:%s"), sUpLimit, sDnLimit);
	m_mapHighLow.SetAt(szKey, strValue);
}

void CPartnerMngApp::DoGetMapStrObj_UpDnInfo(LPCSTR szKey, CString &szUpLimit, CString &szDnLimit)
{
	CString strValue;
	if (m_mapHighLow.Lookup(szKey, strValue) == TRUE)
	{
		int nFind = strValue.Find(_T(":"));
		szUpLimit = strValue.Left(nFind);
		szDnLimit = strValue.Mid(nFind+1);
	}
}

LONG CPartnerMngApp::DoGetPctrInterface(int nID)
{
	if(nID==0)
	{
		if(m_pPctrGateMng!=NULL) return (LONG)m_pPctrGateMng;

		m_pPctrGateMng = (IGateManager*)AfxGetPctrInterface(nID);
		return (LONG)m_pPctrGateMng;
	}

	return (LONG)AfxGetPctrInterface(nID);
}

CPartnerMngApp theApp;

/////////////////////////////////////////////////////////////////////////////
//

LONG WINAPI GetInferface(int nID, long dwData)
{
	return theApp.m_GateMng.GetInterface(nID, dwData);
}

LONG WINAPI SetInferface(int nID, long dwData)
{
//	int nID = (int)wp;
//	LONG dwInterface = dwData

	return NULL;
}

void WINAPI SetPcTrInterface(int nID, long dwData)
{
	switch(nID)
	{
		case 0:
			theApp.m_pPctrGateMng = (IGateManager*)dwData;
			break;
		default:
			break;
	}
}
