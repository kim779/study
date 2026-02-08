// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

CNewsSignalManager::CNewsSignalManager()
{
	m_pNewsMoniterWnd = NULL;
	m_nAddRef = 0;
}

CNewsSignalManager::~CNewsSignalManager()
{
}

//BOOL CNewsSignalManager::IsNewsSignalLoaded()
//{
//	if(m_pNewsMoniterWnd) return TRUE;
//	return FALSE;
//}

// Create 성공 후 리턴 1; 실패 0, 생성된 것 리턴하면 2
// cType : E -> 유진 소리마치 메인Type;
LONG CNewsSignalManager::AdviseNewsSignal(char cType, LONG dwTypeValue)
{
	if (m_pNewsMoniterWnd == NULL)
	{
		m_pNewsMoniterWnd = new CNewsMoniterWnd();
		m_pNewsMoniterWnd->m_szRootPath = theApp.m_MainInfoMng.GetRootDir();
		m_pNewsMoniterWnd->m_szRootPath += "\\DEV";
		//DWORD	dwStyle = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
		//BOOL bRet = m_pNewsMoniterWnd->Create(NULL,_T("뉴스 감시모니터"), dwStyle, CRect(0, 0, 0, 0), this, 1111, NULL);
		BOOL bRet = m_pNewsMoniterWnd->OpenWindow();
		if(!bRet)
		{
			delete m_pNewsMoniterWnd;
			m_pNewsMoniterWnd = NULL;
			m_nAddRef = 0;
			return NULL;
		}
	}

	ST_NewsSignalMng* pData = new ST_NewsSignalMng;
	pData->cType = cType;
	pData->dwTypeValue = dwTypeValue;
	POSITION pos = m_List.AddTail(pData);
	m_nAddRef++;
	return (LONG)pos;
}

// Avise할 때 받은 키값.
BOOL CNewsSignalManager::UnAdviseNewsSignal(LONG dwAdviseKey)
{
	if(dwAdviseKey)
	{
		POSITION pos = (POSITION)dwAdviseKey;
		ST_NewsSignalMng* pData = m_List.GetAt(pos);
		if(pData) delete pData;
		m_List.RemoveAt(pos);
	}

	m_nAddRef--;
	if(m_nAddRef==0)
	{
		m_pNewsMoniterWnd->DestroyWindow();
		delete m_pNewsMoniterWnd;
		m_pNewsMoniterWnd = NULL;
	}
	if(m_nAddRef<0) 
	{
		TRACE("CNewsSignalManager::ExitNewsSignal() 비정상적으로 불림\n");
		m_nAddRef = 0;
	}
	return m_nAddRef;
}

int CNewsSignalManager::GetAdviseCount()
{
	return m_nAddRef;
}

int CNewsSignalManager::SetNewsSignal(BOOL bAdvise, CWnd* pScreenWnd, CString strType, CString strKey)
{
	int nRet=0;
	if (m_pNewsMoniterWnd != NULL)
	{
		nRet = m_pNewsMoniterWnd->SetSingSiganl( bAdvise, pScreenWnd, strType , strKey );
	}

	return nRet;
}

void CNewsSignalManager::GetSignalData(CString strType, CMapStringToPtr &mapSignalSet)
{
	int nRet=0;
	if (m_pNewsMoniterWnd != NULL)
	{
		m_pNewsMoniterWnd->GetSignalData( strType, mapSignalSet);
	}
}

void CNewsSignalManager::GetFilterData(long &stFilterInfo)
{
	int nRet=0;
	if (m_pNewsMoniterWnd != NULL)
	{
		m_pNewsMoniterWnd->GetFilterData( stFilterInfo );
	}
}

void CNewsSignalManager::SetSignalData(CString strType, CMapStringToPtr *pmapSignalSet)
{
	if (m_pNewsMoniterWnd != NULL)
	{
		m_pNewsMoniterWnd->SetSignalData( strType, pmapSignalSet );
	}
}

void CNewsSignalManager::SetFilterData(long *pstFilterInfo)
{
	if (m_pNewsMoniterWnd != NULL)
	{
		m_pNewsMoniterWnd->SetFilterData( pstFilterInfo );
	}
}

int CNewsSignalManager::GetSiganlTable(CString strType, CStringArray &saSignalTable )
{
	int nRet=0;
	if (m_pNewsMoniterWnd != NULL)
	{
		m_pNewsMoniterWnd->GetSiganlTable( strType, saSignalTable );
	}
	return nRet;
}

HWND CNewsSignalManager::GetDummyHwnd()
{
	if (m_pNewsMoniterWnd != NULL)
	{
		return m_pNewsMoniterWnd->GetDummyHwnd();
	}

	return NULL;
}
