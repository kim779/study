// STControllerLoader.cpp: implementation of the CSTControllerLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STControllerLoader.h"

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
CSTControllerLoader::CSTControllerLoader()
{
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
		if( !CProgIDInfo::ComServerFromProgID( szProgID, strChartOcxPath))
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
	}

	m_hSTController= NULL;
	m_fnGetSTController = NULL;
	m_strSTController = "STController.dll";
	LoadSTController();
}

CSTControllerLoader::~CSTControllerLoader()
{
	UnloadSTController();
}

// (2006/3/5 - Seung-Won, Bae) Manage Debug or Release Version and Path of Chart OCX
BOOL	CSTControllerLoader::m_bIsDebugVersion = FALSE;
CString	CSTControllerLoader::m_strChartOcxFolder;

BOOL CSTControllerLoader::LoadSTController()
{
	CString strMessage;	
	if(m_hSTController) return TRUE;

	// (2006/3/5 - Seung-Won, Bae) Change Current Directory for Load Strategy DLL
	m_hSTController = NULL;
	char szCurrentDirectory[ 256];
	if( GetCurrentDirectory( 256, szCurrentDirectory))
	{
		if( SetCurrentDirectory( m_strChartOcxFolder))
		{
			m_hSTController = ( HINSTANCE)::LoadLibrary( m_strSTController);	
			SetCurrentDirectory( szCurrentDirectory);
		}
	}
	if( !m_hSTController) m_hSTController = (HINSTANCE) ::LoadLibrary(m_strSTController);	// in Error, Use Current Directory.

	if( m_hSTController == NULL)
	{
		strMessage.Format("%s을 Load할 수 없습니다.",m_strSTController);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	m_fnGetSTController = (_DLL_GetSTController)::GetProcAddress(m_hSTController,"fnGetSTController");
	if(m_fnGetSTController == NULL)
	{
		strMessage.Format("%s에서 fnGetSTController 찾을 수 없습니다.",m_strSTController);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	m_fnGetSTControllerCfg = (_DLL_GetSTControllerCfg)::GetProcAddress(m_hSTController,"fnGetSTControllerCfg");
	if(m_fnGetSTControllerCfg == NULL)
	{
		strMessage.Format("%s에서 fnGetSTControllerCfg 찾을 수 없습니다.",m_strSTController);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	return TRUE;
}

BOOL CSTControllerLoader::UnloadSTController()
{
	if(!m_hSTController) return TRUE;
	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hSTController);
	if(bResult)
	{
		m_hSTController = NULL;
		m_fnGetSTController = NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CISTController *CSTControllerLoader::GetSTController()
{
	if(!m_fnGetSTController) return NULL;
	return (CISTController *)m_fnGetSTController();
}

CISTController *CSTControllerLoader::GetSTControllerCfg()
{
	if(!m_fnGetSTControllerCfg) return NULL;
	return (CISTController *)m_fnGetSTControllerCfg();
}