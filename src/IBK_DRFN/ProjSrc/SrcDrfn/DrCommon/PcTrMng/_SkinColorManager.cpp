// #include "../../inc/ISkinColorMng.h"
#include "stdafx.h"
#include "PcTrMng.h"

/////////////////////////////////////////////////////////////////////////////
// CSkinColorManager


CSkinColorManager::CSkinColorManager()
{
	m_bLoadedCfgInfo = FALSE;
}

CSkinColorManager::~CSkinColorManager()
{
	m_ArrColorTable.RemoveAll();
//@유진삭제	m_ArrColorTableDefault.RemoveAll();
	m_ListSkinSite.RemoveAll();
	// ClearSkinInfo();
	m_ArrSkinInfo.RemoveAll();
	
	m_ArrColorTable.SetSize(0);
//@유진삭제	m_ArrColorTableDefault.SetSize(0);
	m_ArrSkinInfo.SetSize(0);
}

//void CSkinColorManager::ClearSkinInfo()
//{
//	int nSize = m_ArrSkinInfo.GetSize();
//
//	for(int i=0; i<nSize; i++)
//	{
//		ST_SKININFO* pTmp = m_ArrColorTable.GetAt(i);
//		if(pTmp) delete pTmp;
//	}
//}

COLORREF CSkinColorManager::GetColor(LPCSTR szKeyName)
{
	for(int i=0; i<m_ArrColorTable.GetSize(); i++)
	{
		STUSERCOLORTABLE info = m_ArrColorTable.GetAt(i);
		if(info.szName.Compare(szKeyName)==0)
		{
			return info.rgb;
		}
	}

	return RGB(0,0,0);
}

// 기본 스킨 테이블 ojtaso
COLORREF CSkinColorManager::GetDefaultColor(LPCSTR szKeyName)
{
//@유진삭제	
/*
	if(!m_ArrColorTableDefault.GetSize())
	{
		char szTableFile[MAX_PATH]  = {NULL, };
		::sprintf(szTableFile, _T("%s\\%s"), theApp.m_szRootDir, ISkinColor_default_File);

		LoadDefaultColorTable(szTableFile);
	}

	for(int i=0; i<m_ArrColorTableDefault.GetSize(); i++)
	{
		STUSERCOLORTABLE info = m_ArrColorTableDefault.GetAt(i);
		if(info.szName.Compare(szKeyName)==0)
		{
			return info.rgb;
		}
	}
*/
	return RGB(0,0,0);
}

int CSkinColorManager::GetIndex(LPCSTR szKeyName)
{
	for(int i=0; i<m_ArrColorTable.GetSize(); i++)
	{
		STUSERCOLORTABLE info = m_ArrColorTable.GetAt(i);
		if(info.szName.Compare(szKeyName)==0)
		{
			return i;
		}
	}
	
	return -1;
}

// 기본 스킨 테이블 ojtaso
int CSkinColorManager::GetDefaultIndex(LPCSTR szKeyName)
{
//@유진삭제	
/*
	if(!m_ArrColorTableDefault.GetSize())
	{
		char szTableFile[MAX_PATH]  = {NULL, };
		::sprintf(szTableFile, _T("%s\\%s"), theApp.m_szRootDir, ISkinColor_default_File);

		LoadDefaultColorTable(szTableFile);
	}

	for(int i=0; i<m_ArrColorTableDefault.GetSize(); i++)
	{
		STUSERCOLORTABLE info = m_ArrColorTableDefault.GetAt(i);
		if(info.szName.Compare(szKeyName)==0)
		{
			return i;
		}
	}
*/	
	return -1;
}

// 
COLORREF CSkinColorManager::GetColor(int nIndex)
{
	if(nIndex<0 || m_ArrColorTable.GetSize()<nIndex)
		return RGB(0,0,0);
	
	STUSERCOLORTABLE info = m_ArrColorTable.GetAt(nIndex);
	return info.rgb;
}

// 기본 스킨 테이블 ojtaso
COLORREF CSkinColorManager::GetDefaultColor(int nIndex)
{
//@유진삭제	
/*	if(!m_ArrColorTableDefault.GetSize())
	{
		char szTableFile[MAX_PATH]  = {NULL, };
		::sprintf(szTableFile, _T("%s\\%s"), theApp.m_szRootDir, ISkinColor_default_File);

		LoadDefaultColorTable(szTableFile);
	}

	if(nIndex<0 || m_ArrColorTableDefault.GetSize()<nIndex)
		return RGB(0,0,0);
	
	STUSERCOLORTABLE info = m_ArrColorTableDefault.GetAt(nIndex);
	return info.rgb;
*/
	return RGB(255, 255, 255);
}

// 칼라테이블 배열 리턴
long CSkinColorManager::GetColorTableArray()
{
	return (long)&m_ArrColorTable;
}

// 기본 스킨 테이블 ojtaso
// 칼라테이블 배열 리턴
long CSkinColorManager::GetDefaultColorTableArray()
{
//@유진삭제	
return NULL;
/*
	if(!m_ArrColorTableDefault.GetSize())
	{
		char szTableFile[MAX_PATH]  = {NULL, };
		::sprintf(szTableFile, _T("%s\\%s"), theApp.m_szRootDir, ISkinColor_default_File);

		LoadDefaultColorTable(szTableFile);
	}
	return (long)&m_ArrColorTableDefault;
*/
}

// ISkinColorSite 등록
long CSkinColorManager::AdviseSite(ISkinColorSite* pSite)
{
	POSITION pos = m_ListSkinSite.AddTail(pSite);
	return (long)pos;
}

// ISkinColorSite 해지.
BOOL CSkinColorManager::UnadviseSite(long dwKey)
{
	POSITION pos = (POSITION)dwKey;
	if(pos) {
		m_ListSkinSite.RemoveAt(pos);
		return TRUE;
	}
	return FALSE;
}

// 칼라테이블을 로딩한다.
// 기존 윈넷에 있는 루틴을 그대로 가져옴.
BOOL CSkinColorManager::LoadColorTable(LPCSTR szTableFile, BOOL bNotify)
{
	if(m_ArrColorTable.GetSize()>0) return TRUE;
	
	m_ArrColorTable.RemoveAll();

	int nCnt = ::GetPrivateProfileInt("컬러개수", "개수", 0, szTableFile);
	if(nCnt==0)
	{
		CString szMsg;
		szMsg.Format("파일[%s]에서 칼라테이블 정보를 읽을 수 없습니다.", szTableFile);
		TRACE("%s\n", szMsg);
		//AfxMessageBox(szMsg);
		return FALSE;
	}

	m_ArrColorTable.SetSize(nCnt);

	char tmp[4+1]={0,}, key1[120+1]={0,}, sColorName[50+1]={0,};
	int bRGB[3];

	for(int i=0; i < nCnt; i++)
	{
		sprintf(tmp, "%d", i);
		::GetPrivateProfileString("컬러인덱스", tmp, "", key1, 120, szTableFile);
		sscanf(key1, "%[^,], %d, %d, %d", sColorName, &bRGB[0], &bRGB[1], &bRGB[2]);
		
		STUSERCOLORTABLE info;
		info.szName = sColorName;
		info.rgb = RGB(bRGB[0], bRGB[1], bRGB[2]);
		m_ArrColorTable.SetAt(i, info);
	}

	return TRUE;
}

// 등록된 ISkinColorSite 에 ColorTableChanged() 콜한다.
void CSkinColorManager::NotifyToSite()
{
	for(POSITION pos=m_ListSkinSite.GetHeadPosition(); pos; )
	{
		ISkinColorSite *pSite = m_ListSkinSite.GetNext(pos);
		if(pSite)
		{
			pSite->ColorTableChanged();
		}
	}
}

BOOL CSkinColorManager::SetSkinConfig(LPCSTR szCfgFileName)
{
	CFileFind file;
	if(file.FindFile(szCfgFileName)==FALSE)
		return FALSE;

	m_szSkinCfgFile = szCfgFileName;
	return TRUE;
}

int CSkinColorManager::GetSkinCount()
{
	if(m_bLoadedCfgInfo==FALSE) LoadSkinInfo();

	int nCount = m_ArrSkinInfo.GetSize();
	return nCount;
}

BOOL CSkinColorManager::GetSkinInfo(int nIndex, ST_SKININFO& skInfo)
{
	if(m_bLoadedCfgInfo==FALSE) LoadSkinInfo();

	if(nIndex<0 || m_ArrSkinInfo.GetSize()<=nIndex)
		return FALSE;

	skInfo = m_ArrSkinInfo.GetAt(nIndex);
	return TRUE;
}

//[SKIN]
//count=6
//01=스킨고전|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default
void CSkinColorManager::LoadSkinInfo()
{
	m_ArrSkinInfo.RemoveAll();

	if(m_szSkinCfgFile.GetLength()==0)
	{
		//ISkinColorSite_SkinInfo_File -> ISkinColorMng.h참고
		m_szSkinCfgFile.Format("%s\\%s", theApp.m_szRootDir, ISkinColorSite_SkinInfo_File);
	}

	LPCSTR szSect="SKIN",
		   szCount = "Count";

	int nCount = ::GetPrivateProfileInt(szSect, szCount, 0, m_szSkinCfgFile);
	if(nCount>0) m_ArrSkinInfo.SetSize(nCount);

	char aTmp[3], aBuffer[150+1]={0,};
	ST_SKININFO stSkinInfo;
	for(int i=0; i<nCount; i++)
	{
		sprintf(aTmp, "%02d", i+1);
		::GetPrivateProfileString(szSect, aTmp,
					ISkinColorSite_Skin_default,	//ISkinColorSite_Skin_default -> ISkinColorMng.h참고
					aBuffer, 150,
					m_szSkinCfgFile);

		CST_SKININFO::Convert(stSkinInfo, aBuffer);
		m_ArrSkinInfo.SetAt(i, stSkinInfo);
	}

	if(m_ArrSkinInfo.GetSize()>0)	m_bLoadedCfgInfo = TRUE;
	else m_bLoadedCfgInfo = FALSE;
}

// 기본 스킨테이블 로드 ojtaso
BOOL CSkinColorManager::LoadDefaultColorTable(LPCSTR szTableFile)
{
//@유진삭제	
/*	
	if(m_ArrColorTableDefault.GetSize())
		return TRUE;

	int nCnt = ::GetPrivateProfileInt("컬러개수", "개수", 0, szTableFile);
	if(nCnt==0)
	{
		CString szMsg;
		szMsg.Format("파일[%s]에서 칼라테이블 정보를 읽을 수 없습니다.", szTableFile);
		TRACE("%s\n", szMsg);
		AfxMessageBox(szMsg);
		return FALSE;
	}

	m_ArrColorTableDefault.SetSize(nCnt);

	char tmp[4+1]={0,}, key1[120+1]={0,}, sColorName[50+1]={0,};
	int bRGB[3];

	for(int i=0; i < nCnt; i++)
	{
		sprintf(tmp, "%d", i);
		::GetPrivateProfileString("컬러인덱스", tmp, "", key1, 120, szTableFile);
		sscanf(key1, "%[^,], %d, %d, %d", sColorName, &bRGB[0], &bRGB[1], &bRGB[2]);
		
		STUSERCOLORTABLE info;
		info.szName = sColorName;
		info.rgb = RGB(bRGB[0], bRGB[1], bRGB[2]);
		m_ArrColorTableDefault.SetAt(i, info);
	}
*/
	return TRUE;
}
