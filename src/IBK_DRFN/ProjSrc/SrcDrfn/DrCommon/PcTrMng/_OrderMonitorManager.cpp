// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"
#include "MonitorWnd.h"

const UINT RMSG_SINGLEMAPOPEN		= ::RegisterWindowMessage("RMSG_SINGLEMAPOPEN");

COrderMonitorManager::COrderMonitorManager()
{
	m_hATHwnd = NULL;
}

HWND COrderMonitorManager::GetHandle()
{
	if(m_hATHwnd==NULL)	CheckOpenMoniterServer();
	// m_hATHwnd = (HWND)theApp.m_StateMng.m_dwInterface[RunModule_AutoTrade];
	return m_hATHwnd;
}


HWND COrderMonitorManager::CheckOpenMoniterServer()
{
	m_hATHwnd = (HWND)theApp.m_StateMng.m_dwInterface[RunModule_AutoTrade];
	if(m_hATHwnd) return m_hATHwnd;

	{ // 메인에 화면열기 요청
		//CString szMapID="11490";
		char aMapID[6]={0,};
		CString szCFG; szCFG.Format("%s%s", theApp.m_szRootDir, "/Data/HidenMng.ini");
		::GetPrivateProfileString("HIDENMNG", "AUTOTMONTOR", "1149", aMapID, 5, szCFG);
		CString szMapID(aMapID); szMapID+="0";
		HWND hMain = theApp.m_hMainHwnd;
		m_szMapID = szMapID;
		// ::SendMessage(hMain, RMSG_SINGLEMAPOPEN, 0, (LPARAM)(LPCTSTR)szMapID);
		::PostMessage(hMain, RMSG_SINGLEMAPOPEN, 0, (LPARAM)(LPCTSTR)m_szMapID);
	}

	// 화면을 연다음엔 모니터링 핸들값 체크.
	CMonitorWnd mWnd;
	if(mWnd.OpenWindow())
	{
		if(mWnd.WaitResult()>MONITER_WRET_READY)
		{
			m_hATHwnd = mWnd.m_hATHwnd;
			return m_hATHwnd;
		}
	}
	return NULL;
}


BOOL COrderMonitorManager::SetState(DSBM_RUNSTATE* pData, long dwLong)
{
	if(!GetHandle()) return FALSE;

	COPYDATASTRUCT copyData;
	copyData.dwData = SBMID_RUNSTATE;
	copyData.cbData = DSBM_RUNSTATE_SIZE;
	copyData.lpData = (void*)pData;

	::SendMessage(m_hATHwnd, WM_COPYDATA, 0, (LPARAM)&copyData);
	return TRUE;
}

// 조건의 동작상태를 나타낸다.
BOOL COrderMonitorManager::SetCondition(DSBM_RUN* pData, long dwLong)
{
	if(!GetHandle()) return FALSE;

	COPYDATASTRUCT copyData;
	copyData.dwData = SBMID_CONDITION;
	copyData.cbData = DSBM_RUN_SIZE;
	copyData.lpData = (void*)pData;

	::SendMessage(m_hATHwnd, WM_COPYDATA, 0, (LPARAM)&copyData);
	return TRUE;
}

// 주문로그를 남긴다.
BOOL COrderMonitorManager::SetTradeLog(DSBM_TRADELOG* pData, long dwLong)
{	
	if(!GetHandle()) return FALSE;

	COPYDATASTRUCT copyData;
	copyData.dwData = SBMID_SBLOG;
	copyData.cbData = DSBM_SBLOG_SIZE;
	copyData.lpData = (void*)pData;

	::SendMessage(m_hATHwnd, WM_COPYDATA, 0, (LPARAM)&copyData);
	return TRUE;
}

