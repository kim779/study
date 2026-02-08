// DllSTLoader.cpp: implementation of the CDllSTLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllSTLoader.h"

#include "ProgIDInfo.h"		// for CProgIDInfo
//#include "SiteDefine.h"		// for g_szChartProgID
#include "../../inc/IGateMng.h"
#include "../../inc/IMainInfoMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef int( *PFnIsDebugVersion)( void);	
CDllSTLoader::CDllSTLoader()
{
	m_hSTDll= NULL;
	m_fnGetInputs = NULL;
	m_fnLoadST = NULL;
	m_fnUnloadST = NULL;
	m_fnExecuteST = NULL;
	m_fnStopST = NULL;
	m_fnPauseST = NULL;
	m_fnUpdateST = NULL;
	m_fnCommandST = NULL;
	m_lSTActor = NULL;

	// (2006/3/5 - Seung-Won, Bae) Manage Debug or Release Version and Path of Chart OCX
	if( m_strChartOcxFolder.IsEmpty())
	{
		int nID = 21;
		IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
		if(pGateMng == NULL)
			return;
		IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);
		CString strDataPath = pManager->GetDataDir();
		if(strDataPath.GetLength()==0)
		{
			AfxMessageBox("DataPath를 얻을 수 없습니다.");
		}

		char szProgID[MAX_PATH] = {0,};
		DWORD dwError = GetPrivateProfileString( "Chart Version", "ProgID", "Prog ID Error", szProgID, 255, strDataPath + "\\" + "ChartMetaTable.dat");
		CString strChartOcxPath;
		long lResult = CProgIDInfo::ComServerFromProgID( szProgID, strChartOcxPath);
		if( !lResult)
		{
			m_strChartOcxFolder = strChartOcxPath.Left( strChartOcxPath.ReverseFind( '\\'));
			// 2. Change Current Directory for Load OCX
			char szCurrentDirectory[ 256];
			if( GetCurrentDirectory( 256, szCurrentDirectory))
			{
				if( SetCurrentDirectory( m_strChartOcxFolder))
				{
					HMODULE hChartOCX = LoadLibrary( strChartOcxPath);
					SetCurrentDirectory( szCurrentDirectory);

					if( hChartOCX)
					{
						PFnIsDebugVersion pfnIsDebugVersion = ( PFnIsDebugVersion)::GetProcAddress( hChartOCX, "IsDebugVersion");
						if( pfnIsDebugVersion) m_bIsDebugVersion = ( *pfnIsDebugVersion)();
						FreeLibrary( hChartOCX);
					}
				}
			}
		}
#ifdef _DEBUG
		if( m_strChartOcxFolder.IsEmpty())	TRACE( "[SD Loader] Chart OCX Path is not found! - Error Code %d\r\n", lResult);
		else								TRACE( "[SD Loader] Chart OCX Path is found as [%s]!\r\n", m_strChartOcxFolder);
#endif
	}
}

CDllSTLoader::~CDllSTLoader()
{
	Unload();
}

// (2006/3/5 - Seung-Won, Bae) Manage Debug or Release Version and Path of Chart OCX
BOOL	CDllSTLoader::m_bIsDebugVersion = FALSE;
CString	CDllSTLoader::m_strChartOcxFolder;

//,HWND hSocket/*, long pDrMLib_CDBMgr*/ :제거
long CDllSTLoader::Load(LPCTSTR lpSDDllNamePath, HWND hParent, long lKey, LPCTSTR lpSTName,int nSTPos,LPCTSTR lpAdditionalOption)
{
	if(!lKey)	return -1;		
	if(m_hSTDll) Unload();

	m_strSTDll = lpSDDllNamePath;
	// (2006/3/5 - Seung-Won, Bae) Load suitable DLL Version.
//	if( m_bIsDebugVersion)
//	{
//		m_strSTDll.Replace( "\\SD\\",	"\\SDD\\");
//	}
//	else
//	{
//		m_strSTDll.Replace( "\\SDD\\",	"\\SD\\");
//	}

	// (2006/3/5 - Seung-Won, Bae) Change Current Directory for Load Strategy DLL
	m_hSTDll = NULL;
	m_lSTActor = -1;
	if( !m_strChartOcxFolder.IsEmpty())
	{
		char szCurrentDirectory[ 256];
		if( GetCurrentDirectory( 256, szCurrentDirectory))
		{
			if( SetCurrentDirectory( m_strChartOcxFolder))
			{
				//SD파일 Load ! : KHD 
				if( LoadDLL()) m_lSTActor = m_fnLoadST( hParent,NULL,NULL, lKey, lpSTName,nSTPos,lpAdditionalOption);

				SetCurrentDirectory( szCurrentDirectory);
			}
		}
	}

#ifdef _DEBUG
	if( -1 != m_lSTActor)	TRACE( "Loading [%s] is success!\r\n", m_strSTDll);
	else					TRACE( "Loading [%s] is failed!\r\n", m_strSTDll);
#endif

	return m_lSTActor;
}

//SD파일을 Load하고 함수 포인터를 반환 한다. 
BOOL CDllSTLoader::LoadDLL( void)
{
	m_hSTDll = ( HINSTANCE)::LoadLibrary( m_strSTDll);	

	if( m_hSTDll == NULL)
	{
		m_strError.Format("%s을 Load할 수 없습니다.", m_strSTDll);
		return FALSE;
	}

	m_fnGetInputs = (_DLL_GetInputs)::GetProcAddress(m_hSTDll,"fnGetInputs");
	if(m_fnGetInputs == NULL)
	{
		m_strError.Format("%s에서 fnGetInputs함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}
	m_fnLoadST = (_DLL_LoadST)::GetProcAddress(m_hSTDll,"fnLoadST");
	if(m_fnLoadST == NULL)
	{
		m_strError.Format("%s에서 fnLoadST함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}
	m_fnUnloadST = (_DLL_UnloadST)::GetProcAddress(m_hSTDll,"fnUnloadST");
	if(m_fnUnloadST == NULL)
	{
		m_strError.Format("%s에서 fnUnloadST함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}
	m_fnExecuteST = (_DLL_ExecuteST)::GetProcAddress(m_hSTDll,"fnExecuteST");
	if(m_fnExecuteST == NULL)
	{
		m_strError.Format("%s에서 fnExecuteST함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}	
	m_fnStopST = (_DLL_StopST)::GetProcAddress(m_hSTDll,"fnStopST");
	if(m_fnStopST == NULL)
	{
		m_strError.Format("%s에서 fnStopST함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}	
	m_fnPauseST = (_DLL_PauseST)::GetProcAddress(m_hSTDll,"fnPauseST");
	if(m_fnPauseST == NULL)
	{
		m_strError.Format("%s에서 fnPauseST 함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}	
	m_fnUpdateST = (_DLL_UpdateST)::GetProcAddress(m_hSTDll,"fnUpdateST");
	if(m_fnUpdateST == NULL)
	{
		m_strError.Format("%s에서 fnUpdateST 함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}	
	m_fnCommandST = (_DLL_CommandST)::GetProcAddress(m_hSTDll,"fnCommandST");
	if(m_fnCommandST == NULL)
	{
		m_strError.Format("%s에서 fnCommandST 함수를 찾을 수 없습니다.", m_strSTDll);
		return FALSE;
	}	
	return TRUE;
}

BOOL CDllSTLoader::UnloadDLL()
{
	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hSTDll);
	if(bResult)
	{
		m_hSTDll = NULL;
		m_fnGetInputs = NULL;
		m_fnLoadST = NULL;
		m_fnUnloadST = NULL;
		m_fnExecuteST = NULL;
		m_fnStopST = NULL;
		m_fnPauseST = NULL;
		m_fnUpdateST = NULL;
		m_fnCommandST = NULL;
	}
	else
	{
		return 0;
	}
	return 1;
}

long CDllSTLoader::Unload()
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return -1;
	m_fnUnloadST(m_lSTActor);
	UnloadDLL();
	return 1;
}

long CDllSTLoader::Execute(LPCTSTR lpInputData)
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnExecuteST(m_lSTActor, lpInputData);
}

long CDllSTLoader::Stop()
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnStopST(m_lSTActor);
}

long CDllSTLoader::Pause()
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnPauseST(m_lSTActor);
}

long CDllSTLoader::Update(long lPos, BOOL bIsUpdateData)
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnUpdateST(m_lSTActor,lPos,bIsUpdateData);
}

long CDllSTLoader::Command(LPCTSTR lpCommand, LPCTSTR lpOption)
{
	if(!m_lSTActor)	return 0;
	if(!m_hSTDll) return 0;
	return m_fnCommandST(m_lSTActor, lpCommand,lpOption);
}

CString CDllSTLoader::GetInput(long lIndex)
{
	if(!m_lSTActor)	return "#";
	if(!m_hSTDll) return "#";
	return m_fnGetInputs(lIndex);
}