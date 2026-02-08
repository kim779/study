// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

/////////////////////////////////////////////////////////////////////////////
// CTrCommManager
CTrCommManager::CTrCommManager()
{
	m_hSocket = NULL;
}

CTrCommManager::~CTrCommManager()
{
	for(POSITION pos=m_TrCommList.GetHeadPosition(); pos; )
	{
		CTrCommImpl* pTrComm = m_TrCommList.GetNext(pos);
		if(pTrComm)
		{
			TRACE("CTrCommManager::~CTrCommManager  남은 CTrCommImpl Cnt[%d] [%08X]\n",
						m_TrCommList.GetCount(), pTrComm);
			// delete pTrComm;
			TRACE("CTrCommManager::~CTrCommManager[TrComm을 UnAdvise한 곳이 있습니다.\n");
		}
	}
	m_TrCommList.RemoveAll();
}

ITrComm* CTrCommManager::AdviseTrComm()
{
	CTrCommImpl* pNewTr = new CTrCommImpl(m_hSocket);
	m_TrCommList.AddTail(pNewTr);

	TRACE("CTrCommManager::AdviseTrComm Cnt[%d] [%08X]\n", m_TrCommList.GetCount(), pNewTr);

	return pNewTr;
}

void CTrCommManager::UnAdviseTrComm(ITrComm* pInTr)
{
	POSITION pOld=NULL;
	for(POSITION pos=m_TrCommList.GetHeadPosition(); pos; )
	{
		pOld = pos;
		CTrCommImpl* pTrComm = m_TrCommList.GetNext(pos);
		if(pTrComm==pInTr)
		{
			delete pTrComm;
			m_TrCommList.RemoveAt(pOld);
			TRACE("CTrCommManager::UnAdviseTrComm Cnt[%d] [%08X]\n", m_TrCommList.GetCount(), pInTr);
			return;
		}
	}
}


HWND CTrCommManager::GetSocketHandle(LPCSTR szType)
{
	if(strcmp(szType, "MAIN")==0)
	{
		return m_hSocket;
//		int nID = 0;  // 0 : socketID
//		COleVariant *pVar = theApp.m_MainExinfoMng.GetData(nID);
//		HWND hSocket = HWND(pVar->lVal);
//		return hSocket;
	}

	return NULL;
}


void CTrCommManager::SetSocketHandle(LPCSTR szType, HWND hSocket)
{
	m_hSocket = hSocket;
	if(strcmp(szType, "MAIN")==0)
	{
		int nID = 0;  // 0 : socketID
		COleVariant var((long)hSocket);
		theApp.m_MainExinfoMng.SetData(nID, &var);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTrCommImpl

CTrCommImpl::CTrCommImpl(HWND hSocket)
{
	m_pTrWnd = new CTRComm;
//	SetSocket(hSocket);
	HWND hWnd;
	AfxGetHanaroMainWnd(hWnd);
	CWnd* pMain = CWnd::FromHandle(hWnd);
	m_pTrWnd->Create(pMain);
}

CTrCommImpl::~CTrCommImpl()
{
	if(m_pTrWnd)
	{
		m_pTrWnd->DestroyWindow();
		delete m_pTrWnd; m_pTrWnd=NULL;
	}
}

BOOL CTrCommImpl::Send2Server(void* szData, int nLen, int nKey)
{
// 	CString szDebug;
// 	szDebug.Format("[DRCS]CTrCommImpl::Send2Server");
// 	OutputDebugString(szDebug);


	m_CommCritical.Lock();
	int nRet = m_pTrWnd->Send2Server(szData, nLen, nKey);

	m_CommCritical.Unlock();
	return nRet;
}

void CTrCommImpl::SetTR(LPCSTR szTR, BOOL bCert)
{
	m_pTrWnd->SetTR(szTR, bCert);
}

// 콜백함수를 바꾸려고 할 때 사용
// I-PARAM : 새로운 ITrCommSite*
// O-PARAM : 기존에 등록된 ITrCommSite*.
void CTrCommImpl::ChangeCallback(ITrCommSite* pSite)
{
	m_pTrWnd->ChangeCallback(pSite);
}

void CTrCommImpl::SetBaseData(LPCSTR szKey, LPCSTR szValue)
{
	m_pTrWnd->SetBaseData(szKey, szValue);
}

void CTrCommImpl::GetBaseData(LPCSTR szKey, CString &rValue)
{
	m_pTrWnd->GetBaseData(szKey, rValue);
}

void CTrCommImpl::SetBaseDataLong(LPCSTR szKey, LONG dwValue)
{
	m_pTrWnd->SetBaseDataLong(szKey, dwValue);
}
