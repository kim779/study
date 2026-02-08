// wDrChartDgnDlg.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "wDrChartDgnDlg.h"

#include "ChartPropDlg.h"

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
// CWDrChartDgnDlgApp

BEGIN_MESSAGE_MAP(CWDrChartDgnDlgApp, CWinApp)
	//{{AFX_MSG_MAP(CWDrChartDgnDlgApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWDrChartDgnDlgApp construction

CWDrChartDgnDlgApp::CWDrChartDgnDlgApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWDrChartDgnDlgApp object

CWDrChartDgnDlgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWDrChartDgnDlgApp initialization

BOOL CWDrChartDgnDlgApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	DOChartOCXRegister();

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

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}

#include "MapNameDlg.h"
//	디자인 모드일때 설청창 띄우는 함수.
__declspec(dllexport) char* WINAPI axModalPropDlg(CWnd* pParent, const char* pszOpt, const char* pszFullPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	AfxEnableControlContainer();

	CString strRoot, strFile;
	strRoot.Format(_T("%s"), pszFullPath);
	strFile.Format(_T("%s"), pszOpt);
	
	CString strMapPath;//KHD: 풀패스가 달라서 수정 
	CString stCtrlName, stMapName; //= (LPCTSTR)pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0));OutputDebugString("KHD:  Desige MapName = " + mapName);
	CMapNameDlg Mapdlg;
	int nPos = strRoot.ReverseFind(_T('\\'));
	if (nPos > 0)
	{
		int nLen = strRoot.GetLength();
		if (nPos >= nLen-1)
			strRoot = strRoot.Left(nPos);
		nPos = strRoot.ReverseFind(_T('\\'));
		strRoot = strRoot.Left(nPos);
		strMapPath = strRoot + _T("\\gex\\");
 	}

// 	if(Mapdlg.DoModal()==IDOK)
// 	{
// 		stMapName = Mapdlg.m_strMapName;
// 		stCtrlName = Mapdlg.m_stCtrlCode;
// 		strFile.Format("%s_%s.map", stMapName,stCtrlName);
// 	}else 
// 		return "";
	
	CString strFullPath = strMapPath + strFile;	
	//strRoot = "C:\\Project\\WinRel\\gex\\TT000000.map";
	//"C:\\Program Files\\IBKS(dev)\\HTS\\gex\\TT000000.map";//"C:\\Project\\IBKS\\ProjSrc\\WinRel\\gex\\TT000000.map";//Test : Full경로(파일명_컨트롤명)가 와야 한다. : 플래폼에 요청 
	CChartPropDlg	dlg; 
	if (dlg.SetAllPath(strFullPath) == FALSE)
	{
		AfxMessageBox(_T("설정파일 경로를 확인해 주십시요."));
		return _T("");
	}

	CString strPath = _T("");
	if (dlg.DoModal())
		strPath = dlg.GetFileName();

	return (LPTSTR)(LPCTSTR)strPath;
}

// PropertyChange.
__declspec(dllexport) int WINAPI drPropertySetDlg(LPCSTR pszFullPath, int nMode, long _pDataList)
{
	if(!_pDataList) return -3;	//-3:입력에러.

	CWinixConverter converter;
	//Step 1. Open
	{
		CFile file;
		if (!file.Open(pszFullPath, CFile::typeBinary|CFile::modeRead))
		{
			//OutputDebugString();
			return -1;	//-1 : File Open Error
		}
		CArchive ar(&file, CArchive::load);	
		converter.Serialize(ar);
	}

	int nRet = 0;
	//Step 2. PropertyChange.
	{
		CStringList* pDataList = (CStringList*)_pDataList;
		if(nMode==0)	//Modify
		{
			CString strKey, strData;
			POSITION pos = pDataList->GetHeadPosition();
			int nPos;
			for(; pos;)
			{
				strKey = pDataList->GetNext(pos);
				nPos = strKey.Find('\t');
				if(nPos<1) continue;
				
				strData = strKey.Mid(nPos+1);
				strKey  = strKey.Left(nPos);
				converter.m_propMap.SetAt(strKey, strData);
				nRet++;
			}		
		}
	}

	//Step 3. Save
	{
		CFile file;
		if (!file.Open(pszFullPath, CFile::typeBinary|CFile::modeWrite))
		{
			//OutputDebugString();
			return -2;	//-2 : File WriteOpen Error
		}
		CArchive ar(&file, CArchive::store);	
		converter.Serialize(ar);
	}

	return nRet;	// nRet: Success Count
}

#include "../../inc/ISystemUtilMng.h"
#include "../../ForSite/FS_Env.h"
BOOL CWDrChartDgnDlgApp::DOChartOCXRegister()
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
