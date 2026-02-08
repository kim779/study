// FavoriteThingsMng.cpp: implementation of the CFavoriteThingsMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FavoriteThingsMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ITEMSIZE	126

CFavoriteThingsMng::CFavoriteThingsMng()
{
}

CFavoriteThingsMng::~CFavoriteThingsMng()
{

}

void CFavoriteThingsMng::SetPath(CString strPath)
{
	m_strPathFavorite = strPath;
}

BOOL CFavoriteThingsMng::LoadGrpInfo()
{
	if(m_strPathFavorite.IsEmpty()) return FALSE;
	CFileFind ff;
	m_strArrayGrp.RemoveAll();
	CString strFilesFavorite = m_strPathFavorite + "\\*.fvr";
	BOOL bResult = ff.FindFile(strFilesFavorite);
	if(bResult)
	{
		BOOL bFileExist = TRUE;
		while(bFileExist)
		{
			bFileExist = ff.FindNextFile();
			m_strArrayGrp.Add(ff.GetFileTitle());
		}
	}
	return TRUE;
}

BOOL CFavoriteThingsMng::DeleteItemInfo(LPCTSTR lpItemName, LPCTSTR lpGrpName /*= NULL*/)
{
	if(m_strCurrentGrp!=lpGrpName)
	{
		LoadItemInfo(lpGrpName);
	}
	CString strItemName = lpItemName;
	stFvrData _stFvrData;
	memcpy(_stFvrData.m_chData1,strItemName, sizeof(_stFvrData.m_chData1));
	long lPos = FindItem(&_stFvrData);
	if(lPos==-1)	return FALSE;
	m_ArrayFvrItem.RemoveAt(lPos);
	CString strFilesFavorite = m_strPathFavorite + "\\" + lpGrpName + ".fvr";
	CFile fileGroup;
	stFvrData _stFvrItem;
	if(fileGroup.Open(strFilesFavorite,CFile::modeCreate|CFile::modeWrite))
	{
		long lSize = m_ArrayFvrItem.GetSize();
		for(long lPos=0;lPos<lSize;lPos++)
		{
			_stFvrItem = m_ArrayFvrItem.GetAt(lPos);

			fileGroup.Write(&_stFvrItem,sizeof(_stFvrItem));
		}
		fileGroup.Close();
	}	
	return TRUE;
}

BOOL CFavoriteThingsMng::SaveItemInfo(stFvrData  *pFvrData , LPCTSTR lpGrpName /*= NULL*/)
{
	if(m_strCurrentGrp!=lpGrpName)
	{
		LoadItemInfo(lpGrpName);
	}
	if(FindItem(pFvrData)!=-1) return FALSE;
	m_ArrayFvrItem.Add(*pFvrData);
	CString strFilesFavorite = m_strPathFavorite + "\\" + lpGrpName + ".fvr";
	CFile fileGroup;
	stFvrData _stFvrItem;
	if(fileGroup.Open(strFilesFavorite,CFile::modeCreate|CFile::modeWrite))
	{
		long lSize = m_ArrayFvrItem.GetSize();
		for(long lPos=0;lPos<lSize;lPos++)
		{
			_stFvrItem = m_ArrayFvrItem.GetAt(lPos);

			fileGroup.Write(&_stFvrItem,sizeof(_stFvrItem));
		}
		fileGroup.Close();
	}	
	return TRUE;
}

BOOL CFavoriteThingsMng::LoadItemInfo(LPCTSTR lpGrpName)
{
	m_strCurrentGrp = lpGrpName;
	// load item
	CString strFilesFavorite = m_strPathFavorite + "\\" + lpGrpName + ".fvr";
	CFile fileGroup;
	stFvrData _stFvrData;
	if(fileGroup.Open(strFilesFavorite,CFile::modeRead))
	{
		m_ArrayFvrItem.RemoveAll();
		while(fileGroup.Read(&_stFvrData,sizeof(_stFvrData)))
		{
			m_ArrayFvrItem.Add(_stFvrData);
		}
		fileGroup.Close();
	}
	return TRUE;
}

BOOL CFavoriteThingsMng::NewGrp(LPCTSTR lpGrpName)
{
	if(FindGrp(lpGrpName)==-1)
	{
		m_strArrayGrp.Add(lpGrpName);
		m_strCurrentGrp = lpGrpName;
		m_ArrayFvrItem.RemoveAll();
		return TRUE;
	}
	return FALSE;	
}

BOOL CFavoriteThingsMng::DeleteGrp(LPCTSTR lpGrpName)
{
	CString strFilesFavorite = m_strPathFavorite + "\\" + lpGrpName + ".fvr";
	::DeleteFile(strFilesFavorite);
	return TRUE;
}

int CFavoriteThingsMng::FindGrp(LPCTSTR lpGrpName)
{
	CString strGrpName;
	int nSize = m_strArrayGrp.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		strGrpName = m_strArrayGrp.GetAt(nPos);
		if(strGrpName==lpGrpName)
		{
			return nPos;
		}
	}
	return -1;
}

int CFavoriteThingsMng::FindItem(stFvrData  *pFvrData)
{
	stFvrData _stFvrData;
	int nSize = m_ArrayFvrItem.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		_stFvrData = m_ArrayFvrItem.GetAt(nPos);
		//if(!strcmp(_stFvrData.m_chTitle,pFvrData->m_chTitle))
		if(!strcmp(_stFvrData.m_chData1,pFvrData->m_chData1))
		{
			return nPos;
		}
	}
	return -1;
}