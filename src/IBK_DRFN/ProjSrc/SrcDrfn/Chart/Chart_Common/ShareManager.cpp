// ShareManager.cpp: implementation of the CShareManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ShareManager.h"


#ifdef _WEEKCHART
	#include "../WeekChart/ChartItem.h"
#endif    // _WEEKCHART

#ifdef _COMPARECHART
	#include "../CompareChart/ChartItem.h"
#endif    // _COMPARECHART

#ifdef _MULTICHART		
	#include "../MultiChart/ChartItem.h"
#endif    // _MULTICHART

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShareManager::CShareManager()
{
	m_pSignalOrderMain = NULL;
}

CShareManager::~CShareManager()
{
}


void CShareManager::InitBaseInfo(void* pDBMgr)
{
	//if(pDBMgr)
	//	m_BaseInfo.InitInfo(pDBMgr);
	m_BaseInfo.InitInfo((void*)((CDrMLib_CDBMgr*)pDBMgr)->GetDBLong());
}

void CShareManager::DestroyBaseInfo()
{
	m_BaseInfo.SetKeyCodeToMain_AllControl();
	m_BaseInfo.CloseInfo();
}

CDrMLib_CBaseInfo* CShareManager::GetBaseInfoPtr()
{
	return &m_BaseInfo;
}

BOOL CShareManager::SetHandleInfoToShareBase()
{
	if(m_pSignalOrderMain)
	{
		m_hCHild = ((CWnd*)m_pSignalOrderMain)->GetSafeHwnd();
		m_hCtrl = (HWND)this;
		return TRUE;
	}
	return FALSE;
}

BOOL CShareManager::AdviseSharedInfo()
{
	BOOL bRet = TRUE;	

	bRet &= m_BaseInfo.AdviseSharedInfo(SHARE_SIG_SAS_ALARM,(CSharedBase*)this,DE_SHARDSET|DE_SHARDGET,DE_AREA_ALL);
	bRet &= m_BaseInfo.AdviseSharedInfo(SHARE_SIG_NKS_ALARM,(CSharedBase*)this,DE_SHARDSET|DE_SHARDGET,DE_AREA_ALL);
	bRet &= m_BaseInfo.AdviseSharedInfo(SHARE_SIG_UAS_ALARM,(CSharedBase*)this,DE_SHARDSET|DE_SHARDGET,DE_AREA_ALL);


	return bRet;
}

BOOL CShareManager::UnAdviseSharedInfo()
{	
	BOOL bRet = TRUE;

	bRet &= m_BaseInfo.UnAdviseSharedInfo(SHARE_SIG_SAS_ALARM,(CSharedBase*)this);
	bRet &= m_BaseInfo.UnAdviseSharedInfo(SHARE_SIG_NKS_ALARM,(CSharedBase*)this);
	bRet &= m_BaseInfo.UnAdviseSharedInfo(SHARE_SIG_UAS_ALARM,(CSharedBase*)this);

	return bRet;
}

BOOL CShareManager::SetSharedData2(	CString sShareName, BYTE* pData, int nLen )
{
	return m_BaseInfo.SetSharedData2(NULL, AREA_ALL, sShareName, pData, nLen, 0, FALSE);
}

long CShareManager::ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock)
{
	int		nOdrState = 0;
	BOOL	bState = TRUE;	
	CString strJongCode;

	if(m_pSignalOrderMain == NULL)
		return 0l;

	if(	(sShareName == SHARE_SIG_SAS_ALARM)	||
		(sShareName == SHARE_SIG_NKS_ALARM) ||
		(sShareName == SHARE_SIG_UAS_ALARM)
	)
	{
		m_pSignalOrderMain->SetSignalData(sShareName,pData,nLen);
	}

	return 0l;
}	


long CShareManager::GetSharedData(CString sShareName, BYTE* pData, int& nLen, int& nStock)
{
	return 0l;
}