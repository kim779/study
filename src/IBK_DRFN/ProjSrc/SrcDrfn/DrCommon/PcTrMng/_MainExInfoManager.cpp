#include "stdafx.h"
#include "PcTrMng.h"
#include "../../inc/DefineCode.h"

// ----------------------------------------------------------------------
// CMainExInfoManager
CMainExInfoManager::CMainExInfoManager()
{
	m_MapByteData.SetSize(MAX_IMainExInfoManager);
	int i = 0;
	for( i=0; i<MAX_IMainExInfoManager; i++)
	{
		m_MapByteData.SetAt(i, NULL);
	}
	
	m_MapDisp.SetSize(MAX_MAINEXDISP);
	for(i=0; i<MAX_MAINEXDISP; i++)
	{
		m_MapDisp.SetAt(i, NULL);
	}
}

CMainExInfoManager::~CMainExInfoManager()
{
	fnClearAll();
}

void CMainExInfoManager::fnClearAll()
{
	m_MainExCritical.Lock();
	CheckAutoDelete();
	for(int i=0; i<MAX_IMainExInfoManager && i<m_MapByteData.GetSize(); i++)
	{
		COleVariant* pVar = (COleVariant*)m_MapByteData.GetAt(i);
		if(pVar) delete pVar;
			
	}
	m_MapStringData.RemoveAll();
	m_MapByteData.SetSize(0);
	m_MapDisp.SetSize(0);

	POSITION tPos=NULL;
	for(POSITION pos=m_UserData.GetHeadPosition(); pos;)
	{
		tPos = pos;
		ST_FOR_UserData* pSTData = m_UserData.GetNext(pos);
		if(pSTData)
		{
			if(pSTData->pData && pSTData->nDataLen>0)
			{
				free(pSTData->pData);
				delete pSTData;
				m_UserData.RemoveAt(tPos);
			}
		}
	}
	m_UserData.RemoveAll();

	m_MainExCritical.Unlock();
}

COleVariant* CMainExInfoManager::GetData(LPCSTR szKey)
{
	return (COleVariant*)m_MapStringData[szKey];
}


BOOL CMainExInfoManager::SetData(LPCSTR szKey, const COleVariant* pvData)
{
	COleVariant* pVar = new COleVariant(pvData);
	m_MapStringData[szKey] = (void*)pVar;

	return TRUE;
}


COleVariant* CMainExInfoManager::GetData(int nIndex)
{
	if(nIndex<MAX_IMainExInfoManager && nIndex<m_MapByteData.GetSize())
		return (COleVariant*)m_MapByteData.GetAt(nIndex);

	return NULL;
}

//	0	소켓핸들	long	O	O	메인
//	1	CodeOcx윈도우 포인터	long	O	O	메인
//	2	Signal.dll Lib핸들(HMODULE)	long	O	O	DRDS
//	3	메인프레임 핸들	long	O	O	메인
//	4	메인 AFX_MODULE_STATE*	long	O	O	메인
//	5		long	O	O	메인
//	6		long	O	O	메인
//	7					메인예약
//	8					메인예약
//	9					메인예약
//	10					메인예약
//	11					메인예약
//	12					메인예약
//	13					메인예약
//	14					메인예약
//	15					메인예약
//  16		long		관심예약
//  17		long		임시그룹예약
BOOL CMainExInfoManager::SetData(int nIndex, const COleVariant* pvData)
{
	if(nIndex>=MAX_IMainExInfoManager)
		return FALSE;

	if(nIndex==1) { // CodeOcx일경우 셋팅.
// 	//		theApp.m_MasterMng.m_pCodeOcx = (CCodeOcx*)pvData->lVal;
// 			CWnd* pCodeWnd = (CWnd*)pvData->lVal;
// 
// 			IDispatch* pDisp = NULL;
// 			LPUNKNOWN pUnKnown = pCodeWnd->GetControlUnknown();
// 			HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
// 			if(pDisp)
// 			{
// 				theApp.m_MasterMng.m_CodeOcx.AttachDispatch(pDisp);
// 				theApp.m_MasterMng.m_CodeOcx.InitInterfaceName(pDisp);
// 			}
	}
	if(nIndex==2) { // SignalDLL Lib핸들
//		theApp.m_SigMng.SetLibHandle((HMODULE)pvData->lVal);
	}
	if(nIndex==3) { // MainFrame핸들
		theApp.SetHanaroEnvSetting((HWND)pvData->lVal, HANARO_CLIPKEY_MAINFRAME);
		theApp.m_hMainHwnd = (HWND)pvData->lVal;
		theApp.LoadTestDLL();
	}

	// nIndex == 4 메인 AFX_MODULE_STATE* 사용 (ojtaso 20060104)
	// nIndex 5 ~ 15메인용...

//	if(nIndex==IMainExInfoManager_Gwansim)
//	{
//		m_MapByteData.SetAt(nIndex, (void*)pvData); return TRUE;
//	}
	if(nIndex==IMainExInfoManager_ImsiGroup)
	{
		CheckAutoDelete();
		DeleteData(nIndex);

		LIST_TR_CODEINFO* pList = (LIST_TR_CODEINFO*)SaveImgiGroup(pvData->lVal);
		COleVariant* pVar = new COleVariant((long)pList);
		pVar->vt = VT_I4;
		m_MapByteData.SetAt(nIndex, (void*)pVar);

		return TRUE;
	}

	DeleteData(nIndex);

	COleVariant* pVar = new COleVariant(pvData);
	m_MapByteData.SetAt(nIndex, (void*)pVar);

	return TRUE;
}

// 소멸전에 Delete해줘야 하는 것은 여기서 처리한다.
// IMSI그룹정보 ID(17)
void CMainExInfoManager::CheckAutoDelete()
{
	COleVariant* pVar = GetData(IMainExInfoManager_ImsiGroup);
	if(pVar && pVar->lVal)
	{
		DeleteImsiGroup((long)pVar->lVal);
		DeleteData(IMainExInfoManager_ImsiGroup);
	}
}

void CMainExInfoManager::DeleteImsiGroup(long dwData)
{
	LIST_TR_CODEINFO* pList = (LIST_TR_CODEINFO*)dwData;
	if(!pList) return;

//	TR_CODEINFO codeInfo;
//	for(POSITION pos=pList->GetHeadPosition(); pos; )
//	{
//		codeInfo = pList->GetNext(pos);
//	}
	pList->RemoveAll();
	delete pList;
	dwData = NULL;

//	DeleteData(IMainExInfoManager_ImsiGroup);

}

void CMainExInfoManager::DeleteData(int nIndex)
{
	COleVariant* pVar = (COleVariant*)m_MapByteData.GetAt(nIndex);
	if(pVar) delete pVar;
	m_MapByteData[nIndex] = NULL;
}

void CMainExInfoManager::PreUnLoad()
{
	CheckAutoDelete();
}

void* CMainExInfoManager::SaveImgiGroup(long dwData)
{
	LIST_TR_CODEINFO* pList = (LIST_TR_CODEINFO*)dwData;
	LIST_TR_CODEINFO* pNewList = new LIST_TR_CODEINFO;

	if(!pList) return pNewList;

//	TR_CODEINFO codeInfo;
	for(POSITION pos=pList->GetHeadPosition(); pos; )
		pNewList->AddTail(pList->GetNext(pos));

	return pNewList;
}

/* -------------------------------------------- 
// #SiteDependency.cpp에서 구현
// 
	LONG CMainExInfoManager::GetDispid(int nID)
	void CMainExInfoManager::SetDispid(INT nID)
//
-------------------------------------------- */


LONG CMainExInfoManager::GetUserExData(LONG nID)
{
	return (LONG)m_MapPtrToPtr[(void*)nID];
}

BOOL CMainExInfoManager::SetUserExData(LONG nID, LONG dwValue)
{
	m_MapPtrToPtr[(void*)nID] = (void*)dwValue;
	return TRUE;
}

BOOL CMainExInfoManager::DeleteUserExData(LONG nID)
{
	return m_MapPtrToPtr.RemoveKey((void*)nID);
}

BOOL CMainExInfoManager::IsGetUserExData(LONG nID)
{
	void* rValue;
	BOOL bTmp = m_MapPtrToPtr.Lookup((void*)nID, rValue);
	return bTmp;
}

BOOL CMainExInfoManager::GetUserStringData(LPCSTR szKey, CString& rValue)
{
	return m_MapString_I3.Lookup(szKey, rValue);
}

BOOL CMainExInfoManager::SetUserStringData(LPCSTR szKey, LPSTR szData)
{
	m_MapString_I3.SetAt(szKey, szData);
	return TRUE;
}

POSITION CMainExInfoManager::GetUserData(LPCSTR szKey, LPBYTE *pData, int &nDataLen)
{
	//if(strcmp(szKey, "MSTCODE")==0 || strcmp(szKey, "FIND_RESULT")==0)
	if(strcmp(szKey, "FIND_RESULT")==0)
	{
		return (POSITION)_DoGetUserData(szKey, pData, nDataLen);
	}

	m_MainExCritical.Lock();
	POSITION tPos = NULL;
	for(POSITION pos=m_UserData.GetHeadPosition(); pos;)
	{
		tPos = pos;

		ST_FOR_UserData* pSTData = m_UserData.GetNext(pos);
		if(pSTData)
		{
			if(pSTData->szDataKey.Compare(szKey)==0)
			{
				nDataLen = pSTData->nDataLen;
				*pData = pSTData->pData;
				return tPos;
			}
		}
	}
	m_MainExCritical.Unlock();
	return FALSE;
}

// 수정 080708화
//				기존의 정보가 있을 경우 업데이트 하는 루틴의 버그 수정. 
//				delete pSTData루틴 삭제.
BOOL CMainExInfoManager::SetUserData(LPCSTR szKey, long pData, int nDataLen)
{
	if(nDataLen<=0) return FALSE;

	//if(strcmp(szKey, "MSTCODE")==0 || strcmp(szKey, "FIND_RESULT")==0)
	if(strcmp(szKey, "FIND_RESULT")==0)
	{
		if(_DoSaveUserData(szKey, (LPBYTE)pData, nDataLen)==true)
		{
			m_UserKeyData.SetAt(szKey, "1");
			return TRUE;
		}
		else
		{
			m_UserKeyData.SetAt(szKey, "0");
			return FALSE;
		}
	}

//	long  pGData = NULL;
	LPBYTE pGData = NULL;
	int   nGDataLen = 0;

	m_MainExCritical.Lock();
	POSITION pos = GetUserData(szKey, &pGData, nGDataLen);
	if(pos)
	{
		ST_FOR_UserData* pSTData = m_UserData.GetAt(pos);
		//if(pGData) free((void*)pGData);
		if(pSTData)
		{
			free (pSTData->pData);
		}
		//delete pSTData;

		pSTData->pData = (BYTE*)malloc(nDataLen+10);
		memcpy(pSTData->pData, (void*)pData, nDataLen+10);
		pSTData->pData[nDataLen] = 0;
		pSTData->nDataLen = nDataLen;

		m_MainExCritical.Unlock();
		return TRUE;
	}

	ST_FOR_UserData* pSTData = new ST_FOR_UserData;
	pSTData->szDataKey = szKey;
	pSTData->pData = (BYTE*)malloc(nDataLen+10);	//여유 바이트 10
	memcpy(pSTData->pData, (BYTE*)pData, nDataLen);
	pSTData->pData[nDataLen] = 0;
	pSTData->nDataLen = nDataLen;

	m_UserData.AddTail(pSTData);

	m_MainExCritical.Unlock();
	return TRUE;
}


bool CMainExInfoManager::_DoSaveUserData(LPCSTR szKey, LPBYTE pData, int nDataLen)
{
	CString szFile;
	szFile.Format("%s\\%s.dat", theApp.m_MainInfoMng.GetEncUserDir(), szKey);

	CFile file;
	if(file.Open(szFile, CFile::modeCreate|CFile::typeBinary))
	{
		file.Write(pData, nDataLen);
		file.Close();

		return true;
	}
	return false;
}

bool CMainExInfoManager::_DoGetUserData(LPCSTR szKey, LPBYTE *pData, int &nDataLen)
{
	CString szFile;
	szFile.Format("%s\\%s.dat", theApp.m_MainInfoMng.GetEncUserDir(), szKey);

	CString rData;
	if(!m_UserKeyData.Lookup(szKey, rData)) return false;
	if(rData.Compare("0")==0) return false;

	{
		CFileFind filefind;
		if(!filefind.FindFile(szFile))
			return false;
	}

	CFile file;
	if(file.Open(szFile, CFile::modeRead|CFile::typeBinary))
	{
		nDataLen = file.GetLength();
		*pData = new BYTE[nDataLen];
		file.Read(pData, nDataLen);
		file.Close();
		return true;
	}
	return false;
}

