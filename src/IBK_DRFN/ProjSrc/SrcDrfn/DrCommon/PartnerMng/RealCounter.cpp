#include "stdafx.h"
#include "RealCounter.h"

CRealCounterMng::CRealCounterMng()
{
}

CRealCounterMng::~CRealCounterMng()
{
	RemoveAll();
}

POSITION CRealCounterMng::FindItem(LPCSTR szCode)
{
	CString szCompCode(szCode);
	if(szCompCode[0]=='A'||szCompCode[0]=='J')
		szCompCode = szCompCode.Mid(1);
	POSITION pos, tPos;
	for(pos=m_itemList.GetHeadPosition(); pos;)
	{
		tPos = pos;
		CRealCounterItem item=m_itemList.GetNext(pos);
		if(item.m_szCode.Compare(szCompCode)==0)
			return tPos;
	}
	return NULL;
}

int CRealCounterMng::AddItem(LPCSTR szCode)
{
	POSITION pos = FindItem(szCode);
	if(pos)
	{
		CRealCounterItem item=m_itemList.GetAt(pos);
		item.m_nCount ++;
		m_itemList.SetAt(pos, item);

// 		{
// 			CString szDebug;
// 			szDebug.Format("RealCount Add [%s --> %d\n", szCode, item.m_nCount);
// 			OutputDebugString(szDebug);
// 		}
		return item.m_nCount;
	}
	
	CRealCounterItem item;
	item.m_szCode = szCode;
	item.m_nCount ++;
	m_itemList.AddTail(item);

// 	{
// 		CString szDebug;
// 		szDebug.Format("RealCount Add [%s --> %d\n", szCode, item.m_nCount);
// 		OutputDebugString(szDebug);
// 	}
	return item.m_nCount;
}

int CRealCounterMng::RemoveItem(LPCSTR szCode)
{
	POSITION pos = FindItem(szCode);
	if(pos)
	{
		CRealCounterItem item=m_itemList.GetAt(pos);
		item.m_nCount --;

// 		{
// 			CString szDebug;
// 			szDebug.Format("RealCount Del [%s --> %d\n", szCode, item.m_nCount);
// 			OutputDebugString(szDebug);
// 		}

		if(item.m_nCount==0)
		{
			m_itemList.RemoveAt(pos);			
			return 0;
		}
		return item.m_nCount;
	}

// 	{
// 		CString szDebug;
// 		szDebug.Format("RealCount Del [%s --> 없는종목\n", szCode);
// 		OutputDebugString(szDebug);
// 	}
	return 0;
}

void CRealCounterMng::RemoveAll()
{
	m_itemList.RemoveAll();
}
