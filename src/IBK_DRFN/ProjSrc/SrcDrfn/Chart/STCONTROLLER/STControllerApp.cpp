// STController.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "STControllerApp.h"
#include "STController.h"
#include "STControllerCfg.h"
#include "ApQurier.h"
#include "../../inc/IMainInfoMng.h"		// For IMainInfoManager Interface 

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
// CSTControllerApp

BEGIN_MESSAGE_MAP(CSTControllerApp, CWinApp)
	//{{AFX_MSG_MAP(CSTControllerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSTControllerApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSTControllerApp construction

CSTControllerApp::CSTControllerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_IDIncreaser = 0;
}

long CSTControllerApp::GetSTController()
{
	 CISTController* pStController = new CSTController(this);
	 return (long)pStController;
}

long CSTControllerApp::GetSTControllerCfg()
{
//	 CISTController* pStController = new CSTControllerCfg(this);
//	 return (long)pStController;

	return NULL;
}

int CSTControllerApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strName;
	CInnerFunction *pInnerFunction = NULL;
	POSITION pos = m_mapInnerFunction0.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapInnerFunction0.GetNextAssoc(pos, strName, pInnerFunction);
		delete pInnerFunction;
	}
	m_mapInnerFunction0.RemoveAll();	

	pos = m_mapInnerFunction1.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapInnerFunction1.GetNextAssoc(pos, strName, pInnerFunction);
		delete pInnerFunction;
	}
	m_mapInnerFunction1.RemoveAll();	
	
	return CWinApp::ExitInstance();
}

BOOL CSTControllerApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class	
	CInnerFunction *pInnerFunctoin = NULL;
	long lIndex = 0;
	CString strFuncName;

//	m_innerFuncLoader.LoadInnerFuncDLL("STBasicalFunc.dll");
	// 차트ocx가 등록된 곳에서 같이 로딩될 수 있도록 준비작업.
	CString szDll("STBasicalFunc.dll");
	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if(pMng)
	{
		szDll = pMng->GetDataString(29);		// 29 : Chart Ocx 경로 구하기
		szDll += "STBasicalFunc.dll";
	}
	m_innerFuncLoader.LoadInnerFuncDLL(szDll);

	while(TRUE)
	{
		pInnerFunctoin = (CInnerFunction *)m_innerFuncLoader.LoadInnerFunction(lIndex);
		if(!pInnerFunctoin) break;
		strFuncName = pInnerFunctoin->GetName(0);
		if(strFuncName.GetLength())
		{
			m_mapInnerFunction0.SetAt(strFuncName,pInnerFunctoin);
		}
		else
		{
			delete pInnerFunctoin;
		}
		lIndex++;
	}
	lIndex = 0;
	while(TRUE)
	{
		pInnerFunctoin = (CInnerFunction *)m_innerFuncLoader.LoadInnerFunction(lIndex);
		if(!pInnerFunctoin) break;
		strFuncName = pInnerFunctoin->GetName(1);
		if(strFuncName.GetLength())
		{
			m_mapInnerFunction1.SetAt(strFuncName,pInnerFunctoin);
		}		
		else
		{
			delete pInnerFunctoin;
		}
		lIndex++;
	}

	m_innerFuncLoader2.LoadInnerFuncDLL("TechnicalIndexFunc.dll");
	lIndex = 0;
	while(TRUE)
	{
		pInnerFunctoin = (CInnerFunction *)m_innerFuncLoader2.LoadInnerFunction(lIndex);
		if(!pInnerFunctoin) break;
		strFuncName = pInnerFunctoin->GetName(0);
		if(strFuncName.GetLength())
		{
			m_mapInnerFunction0.SetAt(strFuncName,pInnerFunctoin);
		}
		else
		{
			delete pInnerFunctoin;
		}
		
		lIndex++;
	}

	lIndex = 0;
	while(TRUE)
	{
		pInnerFunctoin = (CInnerFunction *)m_innerFuncLoader2.LoadInnerFunction(lIndex);
		if(!pInnerFunctoin) break;
		strFuncName = pInnerFunctoin->GetName(1);
		if(strFuncName.GetLength())
		{
			m_mapInnerFunction1.SetAt(strFuncName,pInnerFunctoin);
		}
		else
		{
			delete pInnerFunctoin;
		}		
		lIndex++;
	}
	return CWinApp::InitInstance();
}

BOOL CSTControllerApp::Lookup(long lType, LPCTSTR lpFuncName,CInnerFunction *&pInnerFunction)
{
	switch(lType)
	{
	case 1:		return m_mapInnerFunction1.Lookup(lpFuncName,pInnerFunction); break;
	default:	return m_mapInnerFunction0.Lookup(lpFuncName,pInnerFunction); break;
	}	
}

void CSTControllerApp::DeleteAPQurier(CApQurier *pAPQurier)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nReferenced = 0;	
	BOOL bRetValue = m_mapReferencedCnt.Lookup(pAPQurier,nReferenced);
	if(bRetValue)
	{
		nReferenced--;
		if(nReferenced==0)
		{
			m_mapApQurier.RemoveKey(pAPQurier->GetParent());
			m_mapReferencedCnt.RemoveKey(pAPQurier);			
			pAPQurier->DestroyWindow();
			delete pAPQurier;
		}
		else
		{
			m_mapReferencedCnt.SetAt(pAPQurier,nReferenced);
		}
	}
	else
	{
		bRetValue = m_mapReferencedCntForIndex.Lookup(pAPQurier,nReferenced);
		if(bRetValue)
		{
			nReferenced--;
			if(nReferenced==0)
			{
				try
				{
					m_mapApQurierForIndex.RemoveKey(pAPQurier->GetParent());
					m_mapReferencedCntForIndex.RemoveKey(pAPQurier);			
					pAPQurier->DestroyWindow();
					delete pAPQurier;
				}
				catch (...)
				{
				}
			}
			else
			{
				m_mapReferencedCntForIndex.SetAt(pAPQurier,nReferenced);
			}
		}
	}
	

}

// 20060705 함수인자 제거 : ,HWND hSocket, long pDBMgr
//CApQurier *CSTControllerApp::GetAPQurier(CWnd *pWnd,HWND hSocket, long pDBMgr)
CApQurier *CSTControllerApp::GetAPQurier(CWnd *pWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	pWnd = pWnd->GetParent();
//	CWnd *pWnd1stParent = NULL;
//	while(1)
//	{
//		pWnd1stParent = pWnd->GetParent();
//		if(!pWnd1stParent)
//		{
//			break;
//		}
//		else
//		{
//			pWnd = pWnd1stParent;
//		}
//	}
	int nReferenced = 0;
	CApQurier *pQurier = NULL;
	BOOL bRetValue = FALSE;

// 20060705 소켓 / DBMgr 제거로 인해 불필요한 영역 주석
//	if(!hSocket||!pDBMgr)
//	{
		bRetValue = m_mapApQurierForIndex.Lookup(pWnd,pQurier);
		if(!bRetValue)
		{
			pQurier = new CApQurier;
			pQurier->Create(NULL,"ApQurier",WS_CHILD,CRect(0,0,0,0),pWnd,0x8000+m_IDIncreaser);
			pQurier->ShowWindow(SW_HIDE);

			// 20060705 함수인자 제거
			//pQurier->Init((HWND)hSocket,pDBMgr);
			pQurier->Init();

			m_mapApQurierForIndex.SetAt(pWnd,pQurier);
			m_mapReferencedCntForIndex.SetAt(pQurier,1);
			m_IDIncreaser++;
		}
		else
		{
			m_mapReferencedCntForIndex.Lookup(pQurier,nReferenced);
			nReferenced++;
			m_mapReferencedCntForIndex.SetAt(pQurier,nReferenced);
		}

// 20060705 소켓 / DBMgr 제거로 인해 불필요한 영역 주석
//	}
//	else
//	{
//		bRetValue = m_mapApQurier.Lookup(pWnd,pQurier);
//		if(!bRetValue)
//		{
//			pQurier = new CApQurier;
//			pQurier->Create(NULL,"ApQurier",WS_CHILD,CRect(0,0,0,0),pWnd,0x8000+m_IDIncreaser);
//			pQurier->ShowWindow(SW_HIDE);
//			pQurier->Init((HWND)hSocket,pDBMgr);
//			m_mapApQurier.SetAt(pWnd,pQurier);
//			m_mapReferencedCnt.SetAt(pQurier,1);
//			m_IDIncreaser++;
//		}
//		else
//		{
//			m_mapReferencedCnt.Lookup(pQurier,nReferenced);
//			nReferenced++;
//			m_mapReferencedCnt.SetAt(pQurier,nReferenced);
//		}
//	}
	return pQurier;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CSTControllerApp object



long WINAPI fnGetSTController()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetSTController();
}

long WINAPI fnGetSTControllerCfg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetSTControllerCfg();
}
