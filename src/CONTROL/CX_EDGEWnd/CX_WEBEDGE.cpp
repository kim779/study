// CX_WEBEDGE.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CX_WEBEDGE.h"

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
// CCX_WEBEDGEApp

BEGIN_MESSAGE_MAP(CCX_WEBEDGEApp, CWinApp)
	//{{AFX_MSG_MAP(CCX_WEBEDGEApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCX_WEBEDGEApp construction

CCX_WEBEDGEApp::CCX_WEBEDGEApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCX_WEBEDGEApp object

CCX_WEBEDGEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCX_WEBEDGEApp initialization

BOOL CCX_WEBEDGEApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

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

#include "MainWnd.h"
#include "ControlWnd.h"

#define DF_EDGE_MAINMODE 1

void CheckEdgeInstalled(BOOL& bInstalled)
{
	CString stmp, sProgramName, strEdgeEnv;
	sProgramName = "Microsoft Edge WebView2 런타임";
	
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		char szSubKey[MAX_PATH];
		DWORD dwIndex = 0;
		DWORD dwSize = MAX_PATH;

		//해당레지스트리의 하위키를 열거 한다. 
		while (RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
		{
			HKEY hSubKey;
			if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{
				stmp.Format("\r\n[Edge] szSubKey=[%s], dwindex=[%d]", szSubKey, dwIndex);
				OutputDebugString(stmp);
				char szDisplayName[MAX_PATH];
				DWORD dwSize = sizeof(szDisplayName);

				//패키지명 버전 확인
				if (RegQueryValueEx(hSubKey, _T("Displayname"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayName), &dwSize) == ERROR_SUCCESS)
				{
					CString strDisplayName(szDisplayName);
					if (strDisplayName.Find(sProgramName) >= 0)
					{
						bInstalled = TRUE;
						char szDisplayVersion[MAX_PATH];
						dwSize = sizeof(szDisplayVersion);
						if (RegQueryValueEx(hSubKey, _T("DisplayVersion"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayVersion), &dwSize) == ERROR_SUCCESS)
						{
							CString strDisplayVersion(szDisplayVersion);
							strEdgeEnv += "DisplayVersion = ";
							strEdgeEnv += strDisplayVersion;
							strEdgeEnv += "|";
						}

						char szInstallDate[MAX_PATH];
						dwSize = sizeof(szInstallDate);
						if (RegQueryValueEx(hSubKey, _T("InstallDate"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szInstallDate), &dwSize) == ERROR_SUCCESS)
						{
							CString szInstallDate(szInstallDate);
							strEdgeEnv += "InstallDate = ";
							strEdgeEnv += szInstallDate;
							strEdgeEnv += "|";
						}

						stmp.Format("\r\n  !!!!!!!!!!!!!!!!!!!EDGE Installed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  \r\n[Edge] strEdgeEnv=[%s], dwindex=[%d]", strEdgeEnv);
						OutputDebugString(stmp);

						break;
					}
				}

			}
			dwSize = MAX_PATH;
			dwIndex++;
		}//while
	}//if
}

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_param* param = (_param*)pParam;
	

	CString stroption, m_slog;
	stroption.Format("%s", param->options);

	BOOL bInstalled{};
	CheckEdgeInstalled(bInstalled);


	/*
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString subKey = "SOFTWARE\\Microsoft\\EdgeUpdate\\ClientState";

	HKEY hResultKey;
	LONG result = RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hResultKey);
	*/

	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);

	CString spath, stmp;
	spath.Format("%s", chfile);
	spath.TrimRight();

	int iFind = spath.Find("exe");
	spath = spath.Left(iFind);
	spath += "\\tab\\BROWSERPOPUP.TXT";

	m_slog.Format("[edge] [%s] spath =[%s]", __FUNCTION__, spath);
	OutputDebugString(m_slog);

	CFileFind find;
	if (find.FindFile(spath))
	{
		bInstalled = false;
	}

#ifdef DF_EDGE_MAINMODE

	if (bInstalled)
	{
		OutputDebugString("[cx_edge]edge start");
		CMainWnd* pControlWnd = new CMainWnd();
		pControlWnd->m_pParent = parent;

		pControlWnd->SetParam((struct _param*)pParam);
		pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

		m_slog.Format("[edge] [%s] axCreate success", __FUNCTION__);
		OutputDebugString(m_slog);

		return pControlWnd;
	}
	else
	{
		ControlWnd* pControlWnd = new ControlWnd();
		pControlWnd->m_pParent = parent;

		pControlWnd->SetParam((struct _param*)pParam);
		pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);
		return pControlWnd;
	}

#else	
	//k1 = edge, k2 = IE , k3 = chrome
	if(stroption.Find("k2") >= 0 )
	{
		ControlWnd* pControlWnd = new ControlWnd();
		pControlWnd->m_pParent = parent;

		pControlWnd->SetParam((struct _param*)pParam);
		pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);
		return pControlWnd;
	}
	else 	if (result == ERROR_SUCCESS)
	{
		OutputDebugString("[cx_edge]edge start");
		CMainWnd* pControlWnd = new CMainWnd();
		pControlWnd->m_pParent = parent;

		pControlWnd->SetParam((struct _param*)pParam);
		pControlWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

		m_slog.Format("[edge] [%s] axCreate success", __FUNCTION__);
		OutputDebugString(m_slog);

		return pControlWnd;
	}
#endif
	      
}