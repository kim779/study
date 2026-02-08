#include "stdafx.h"
#include "PcTrMng.h"

// ----------------------------------------------------------------------
// CCodeListManager
CCodeListManager::CCodeListManager()
{
}

BOOL CCodeListManager::LoadFile(CCLM_LOADIN *pInData)
{
	CTreeCtrl* pTree = pInData->m_pTreeCtrl;
	LPCSTR szFile = pInData->m_szFile;
	LPCSTR szSection = pInData->m_szSection;
	CString szSubKey("COUNT");
	int nCount = ::GetPrivateProfileInt(szSection, szSubKey,0, szFile);

	HTREEITEM hTreeParent=TVI_ROOT, hTree = NULL;
	char szTitle[50+1]={0,};

	for(int i=0; i<nCount; i++)
	{
		szSubKey.Format("%02d", i++);
		::GetPrivateProfileString(szSection, szSubKey, "", szTitle, 50, szFile);
		if(strlen(szTitle)>0)
		{
			hTree = pTree->InsertItem(szTitle, hTreeParent);
		}
	}
	return FALSE;
}
