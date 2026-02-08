#include "stdafx.h"
#include "PcTrMng.h"

/////////////////////////////////////////////////////////////////////////////
// CDMMngInfo
CDMMngInfo::CDMMngInfo()
{
	m_pThisPoint = NULL;
	m_dwKey = 0;
	m_pCallFunc = 0;
}

CDMMngInfo::CDMMngInfo(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc)
{
	m_szKey = szKey;
	m_pThisPoint = pThisPoint;
	m_dwKey = dwKey;
	m_pCallFunc = pCallFunc;
}

/////////////////////////////////////////////////////////////////////////////
// CAUDataMonitorManager
CAUDataMonitorManager::CAUDataMonitorManager()
{
}


CAUDataMonitorManager::~CAUDataMonitorManager()
{
	ClearListAll();
}

void CAUDataMonitorManager::ClearListAll()
{
	m_List.RemoveAll();
}

long CAUDataMonitorManager::Advise(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc)
{
	CDMMngInfo newInfo(szKey, pThisPoint, dwKey, pCallFunc);
	POSITION pos = m_List.AddTail(newInfo);
	return (long)pos;
}

void CAUDataMonitorManager::UnAdvise(long dwRet)
{
	POSITION pos = (POSITION)dwRet;
	if(pos)	m_List.RemoveAt(pos);
}

void CAUDataMonitorManager::NotifyData(LPCSTR szKey, long dwSubKey, long dwNotifyData)
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
	{
		CDMMngInfo item = m_List.GetNext(pos);
		if(item.m_szKey.Compare(szKey)==0 && item.m_pCallFunc!=NULL)
		{
			item.m_pCallFunc(item.m_szKey, item.m_pThisPoint, item.m_dwKey, dwSubKey, dwNotifyData);
		}
	}
}
