// SDItemInfo.cpp: implementation of the CSDItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDItemInfo.h"
#include "StrategyConfigureWizardDef.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STConfigDef.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSDItemInfo::CSDItemInfo()
{
	m_pISTControllerD = NULL;
	m_nType = 0;
	m_strTimeInfo = TIMEDEFAULT;
	m_nIndexInST = -1;
}

CSDItemInfo::~CSDItemInfo()
{
}

BOOL CSDItemInfo::SetNewSDItemInfo(HWND hParentWnd, HWND hSocket, CDrMLib_CDBMgr* pDBMgr, LPCTSTR lpName, int nType, LPCTSTR lpPath, LPCTSTR lpSTName,int nNewSTPos)
{
	m_strName = lpName;
	m_nType = nType;
	m_strPath = lpPath;
	m_strPath += "\\";
	m_strPath += lpName;
	m_nIndexInST = nNewSTPos;
	if(nType==SYSTEM_ST)
	{
		m_strTitle += "(S)";
	}
	else if(nType==USER_ST)
	{
		m_strTitle += "(U)";
	}
	m_strTitle += lpName;

	CString strDllPath = m_strPath + ".SD";
	CFileStatus status;
	BOOL bDLLIsExist = CFile::GetStatus(strDllPath,status);
	if(!bDLLIsExist)
	{
		return FALSE;
	}
	CString strOption;
	strOption.Format("%s=%s",LOAD_MODE,MODE_TEST);
	long lRetValue = m_loaderOfSTLoader.Load(strDllPath, hParentWnd, (long)this, lpSTName, m_nIndexInST,strOption);
	if(lRetValue==-1) return FALSE;
	return UpdateData();
}

BOOL CSDItemInfo::UpdateData()
{
	return m_loaderOfSTLoader.Execute(""); //then receive m_pISTControllerD..
}