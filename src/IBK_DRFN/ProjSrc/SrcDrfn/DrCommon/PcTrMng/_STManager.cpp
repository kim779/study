// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

CSTManager::~CSTManager()
{
	RemoveAll();
}

void CSTManager::RemoveAll()
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
		delete m_List.GetNext(pos);

	m_List.RemoveAll();
}

//CSTManager_Info* CSTManager::FindItem(LPCSTR szSTPath)
//{
//	for(POSITION pos=m_List.GetHeadPosition(); pos;)
//	{
//		CSTManager_Info* pItem = m_List.GetNext(pos);
//		if(pItem->m_szSTPath.CompareNoCase(szSTPath)==0) return pItem;
//	}
//
//	return NULL;
//}
POSITION CSTManager::FindItem(LPCSTR szSTPath, CSTManager_Info*& pInfo)
{
	POSITION posInfo = NULL;
	pInfo = NULL;
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
	{
		posInfo = pos;
		pInfo = m_List.GetNext(pos);
		if(pInfo->m_szSTPath.CompareNoCase(szSTPath)==0) 
			return posInfo;
	}

	return NULL;
}

int CSTManager::AddSTRunINfo(LPCSTR szSTPath, BOOL bAdd)
{
	CSTManager_Info* pItem = NULL;
	POSITION pos = FindItem(szSTPath, pItem);

	int nItemCount = 0;
	if(pos)
	{
		if(bAdd)
		{
			nItemCount = ++pItem->m_nCount;
		}
		else
		{
			if((nItemCount = --pItem->m_nCount) > 0)
			{
				return nItemCount;
			}
			else
			{
				m_List.RemoveAt(pos);
				delete pItem;
				pItem = NULL;
			}
		}
	}
	else
	{
		if(bAdd)
		{
			pItem = new CSTManager_Info;
			nItemCount = ++pItem->m_nCount;
			pItem->m_szSTPath = szSTPath;
			m_List.AddTail(pItem);
		}
	}

	if(m_List.GetCount() == 0)
		m_List.RemoveAll();

	return nItemCount;
}

int CSTManager::GetSTRunINfo(LPCSTR szSTPath)
{
	CSTManager_Info* pItem = NULL;
	POSITION pos = FindItem(szSTPath, pItem);
	if(!pos) return 0;		// Count = 0;

	return pItem->m_nCount;
}

