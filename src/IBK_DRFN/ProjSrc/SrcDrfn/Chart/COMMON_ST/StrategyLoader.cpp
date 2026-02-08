// StrategyLoader.cpp: implementation of the CStrategyLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StrategyLoader.h"

#include "../INCLUDE_ST/ProgIDInfo.h"		// for CProgIDInfo
//#include "../INCLUDE_ST/SiteDefine.h"		// for g_szChartProgID
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
CStrategyLoader::CStrategyLoader()
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
	
	m_hStrategyManager = NULL;
	m_fnGetStrategyItem = NULL;
	m_strStrategyManager = "StrategyManager.dll";
	LoadStrategyManager();
}

CStrategyLoader::~CStrategyLoader()
{
	UnloadStrategyManager();
}

// (2006/3/5 - Seung-Won, Bae) Manage Debug or Release Version and Path of Chart OCX
BOOL	CStrategyLoader::m_bIsDebugVersion = FALSE;
CString	CStrategyLoader::m_strChartOcxFolder;

BOOL CStrategyLoader::LoadStrategyManager()
{
	CString strMessage;	
	if(m_hStrategyManager) return TRUE;

	char szCurrentDirectory[ 256];
	if( GetCurrentDirectory( 256, szCurrentDirectory))
	{
		if( SetCurrentDirectory( m_strChartOcxFolder))
		{
			m_hStrategyManager  = (HINSTANCE) ::LoadLibrary(m_strStrategyManager);

			SetCurrentDirectory( szCurrentDirectory);
		}
	}
	
	if( m_hStrategyManager == NULL)
	{
		strMessage.Format("%s을 Load할 수 없습니다.",m_strStrategyManager);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	m_fnGetStrategyItem = (_DLL_GetStrategyItem)::GetProcAddress(m_hStrategyManager,"fnGetStrategyItem");
	if(m_fnGetStrategyItem == NULL)
	{
		strMessage.Format("%s에서 fnGetStrategyItem 찾을 수 없습니다.",m_strStrategyManager);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	return TRUE;

}

BOOL CStrategyLoader::UnloadStrategyManager()
{
	if(!m_hStrategyManager) return TRUE;
	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hStrategyManager);
	if(bResult)
	{
		m_hStrategyManager = NULL;
		m_fnGetStrategyItem = NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CIStrategyItem *CStrategyLoader::GetStrategyItem()
{
	if(!m_fnGetStrategyItem) return NULL;
	return (CIStrategyItem *)m_fnGetStrategyItem();
}
