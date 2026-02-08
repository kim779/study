// TreeListMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TreeListMng.h"

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
// CTreeListMngApp

BEGIN_MESSAGE_MAP(CTreeListMngApp, CWinApp)
	//{{AFX_MSG_MAP(CTreeListMngApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeListMngApp construction

CTreeListMngApp::CTreeListMngApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_dwDataMon = NULL;
	m_pTreeListMng = new CTreeListManager;	
	TRACE("%s\n", "CTreeListMngApp::CTreeListMngApp");
}

BOOL CTreeListMngApp::InitInstance() 
{
	m_pIGateMng; AfxGetIGateManager(m_pIGateMng);

//	IAUGwanManager* pIGwanMng = (IAUGwanManager*)m_pIGateMng->GetInterface(UUID_IAUGwanManager);

//	long dwKey = 100;
//	m_dwDataMon = pIGwanMng->Advise(DataMon_SZGWANSIM, (long)this, dwKey, DataMonitorCallbackProc);

//	TRACE("%s\n", "CTreeListMngApp::InitInstance");
	return CWinApp::InitInstance();
}

int CTreeListMngApp::ExitInstance() 
{
	AppTerminate();
	TRACE("%s\n", "CTreeListMngApp::ExitInstance");
	return CWinApp::ExitInstance();
}

void CTreeListMngApp::AppTerminate()
{
//	if(m_dwDataMon)
//	{
//		IAUGwanManager* pIGwanMng = (IAUGwanManager*)m_pIGateMng->GetInterface(UUID_IAUGwanManager);
//		pIGwanMng->UnAdvise(m_dwDataMon);
//		m_dwDataMon = NULL;
//	}
	if(m_pTreeListMng) delete m_pTreeListMng; m_pTreeListMng=NULL;
}

long CTreeListMngApp::GetGwansimDataAll(long pSGwanInterface)
{
	IWinixGwanManager* pIGwanMng = (IWinixGwanManager*)pSGwanInterface;
	if(pIGwanMng)
	{
		int nType=0;
 		LPCSTR szGroup=NULL;
		pIGwanMng->GetGwansimDataAll(nType, szGroup);
	}
//@Solomon091104-->
// 	IAUGwanManager* pMng = (IAUGwanManager*)m_pIGateMng->GetInterface(UUID_IAUGwanManager);
// 	if(!pMng) return NULL;
// 
// 	int nType=0;
// 	LPCSTR szGroup=NULL;
// 	return pMng->GetGwansimDataAll(nType, szGroup);
//@Solomon091104<--
	return 0;
}

void CTreeListMngApp::GetGwansimGroupName(long pSGwanInterface, CStringArray &arrKey, CStringArray &arrName)
{
	IWinixGwanManager* pIGwanMng = (IWinixGwanManager*)pSGwanInterface;
	if(pIGwanMng)
	{
		//pIGwanMng->GetGwansimGroupName(arrKey, arrName);
		pIGwanMng->RequestGwansimGroup();//tour2k 기존 파일읽기에서 tr전송으로 변경
	}
//@Solomon091104-->
// 	IAUGwanManager* pMng = (IAUGwanManager*)m_pIGateMng->GetInterface(UUID_IAUGwanManager);
// 	if(!pMng) return;
// 
// 	pMng->GetGwansimGroupName(arrKey, arrName);
//@Solomon091104<--
}

void CTreeListMngApp::GetGwansimItem(long pSGwanInterface, CString strKey, CStringArray& arrCode, CStringArray& arrName)
{
	IWinixGwanManager* pIGwanMng = (IWinixGwanManager*)pSGwanInterface;
	if(pIGwanMng)
	{
		pIGwanMng->GetGwansimItem(strKey, arrCode, arrName);
	}

//@Solomon091104-->
// 	IAUGwanManager* pMng = (IAUGwanManager*)m_pIGateMng->GetInterface(UUID_IAUGwanManager);
// 	if(!pMng) return;
// 
// 	pMng->GetGwansimItem(strKey, arrCode, arrName);
//@Solomon091104<--
}

void CTreeListMngApp::NotifyGwamDataChange(long dwKey, long dwSubKey, long dwNotifyData)
{
//	m_pTreeListMng->ReLoadGwanGroup(dwKey, dwSubKey, dwNotifyData);
}

void CTreeListMngApp::DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData)
{
	//if(strcmp(szKey, DataMon_SZGWANSIM)==0)
	//{
	//	CTreeListMngApp* pThis = (CTreeListMngApp*)pThisPoint;
	//	pThis->NotifyGwamDataChange(dwKey, dwSubKey, dwNotifyData);
	//}
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CTreeListMngApp object

CTreeListMngApp theApp;

LONG WINAPI GetInferface(long pIGateMng)
{
	theApp.m_pTreeListMng->SetIGateManager(pIGateMng);
	return (long)theApp.m_pTreeListMng;
}

LONG WINAPI Terminate(long dwKey)
{
	if(theApp.m_pTreeListMng)
		theApp.m_pTreeListMng->ClearAll();
	theApp.AppTerminate();
	return 0L;
}


