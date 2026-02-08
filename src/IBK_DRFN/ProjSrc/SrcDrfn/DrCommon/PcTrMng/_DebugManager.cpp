#include "stdafx.h"
#include "PcTrMng.h"
#include "../../inc/EnvDir.h"
//#include "../../inc/IDebugMng.h"

#define DRMONITOR_CLASSNAME		_T("DRMonitorClass")	//실행디버그의 클래스명.
#define	gDebugWindow			"DRMonitor"				//exe뺀 실행파일명. 
UINT m_uRMSG_HandleToDebug = ::RegisterWindowMessage("RMSG_HandleToDebug");

#define DRDATACOMP_CLASSNAME		_T("DRDataCompWindow")

CDebugManager::CDebugManager() : m_pDRDebuger(NULL)
{	
}

CDebugManager::~CDebugManager()
{
	if(m_pDRDebuger)
	{
		delete m_pDRDebuger;
		m_pDRDebuger = NULL;
	}
}

LONG CDebugManager::GetDRDebuger()
{
	if(m_pDRDebuger)
		return (LONG)m_pDRDebuger;

	m_pDRDebuger = new CDRDebugerLoader();
	if(m_pDRDebuger)
	{
		if(m_pDRDebuger->IsLoaded()==FALSE)
		{
			delete m_pDRDebuger;
			m_pDRDebuger = NULL;
		}
	}
	return (LONG)m_pDRDebuger;
}

void CDebugManager::Trace(LPCTSTR lpszFormat, ...)
{
	if(!m_pDRDebuger) GetDRDebuger();
	if(!m_pDRDebuger) return;

	ASSERT(AfxIsValidString(lpszFormat));

	va_list args;
	va_start(args, lpszFormat);
	
	int nBuf;
	TCHAR szBuffer[4096+1]={0,};
	
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	if(nBuf<=0) return;
	ASSERT(nBuf >= 0);
	
	va_end(args);

	//DoMessageToDebugWnd(IDM2_TAG_UM, '*', "*", nBuf, (LPBYTE)szBuffer);
	m_pDRDebuger->DRLog_Message(szBuffer);
}

void CDebugManager::Trace(int nMessagKey, LPCTSTR lpszFormat, ...){	}
void CDebugManager::DoMessageToDebugWnd(BYTE nType, BYTE cIO, LPCSTR pDataName, DWORD dwSize, LPBYTE pData){}

void CDebugManager::WriteLogFile(void* sData, int nDataLen, LPCSTR szFileName, BOOL bInsertCR, LPCSTR szLogDir)
{
	CString szFile;
	szFile.Format("%s\\%s", szLogDir, szFileName);

	FILE* fp = fopen(szFile, "aw");
	if(fp)
	{
		fwrite(sData, nDataLen, 1, fp);
		if(bInsertCR) fwrite("\r\n", 2, 1, fp);
		fclose(fp);
	}
}

void CDebugManager::TraceEx(BYTE cMessageTag, BYTE cIO, LPCSTR pDataName, DWORD lDataSize, LPBYTE pData) {}
void CDebugManager::TraceTran(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData)
{
	if(!m_pDRDebuger) GetDRDebuger();
	if(!m_pDRDebuger)	return;

	switch(cIO)
	{
		case 'I':
			m_pDRDebuger->DRLog_RequestData(pTranName, pData, dwSize);
			break;
		case 'O':
			m_pDRDebuger->DRLog_ResponseData(pTranName, pData, dwSize);
			break;
	}
}

void CDebugManager::TraceReal(LPCSTR pDataName, DWORD dwSize, LPBYTE pData)
{
	if(!m_pDRDebuger) GetDRDebuger();
	if(!m_pDRDebuger)	return;

	m_pDRDebuger->DRLog_RealData(pDataName, pData, dwSize);
}

void CDebugManager::TraceSR(BYTE cAU, BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey){}
BOOL CDebugManager::TraceCompData(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData) { return FALSE; }
BOOL CDebugManager::DoMessageToCompDataWnd(BYTE nType, BYTE cIO, LPCSTR pDataName, DWORD dwSize, LPBYTE pData){	return TRUE;}

void CDebugManager::ShowOutputDebugString(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));
	
	va_list args;
	va_start(args, lpszFormat);
	
	int nBuf;
	TCHAR szBuffer[4096+1]={0,};
	
	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	if(nBuf<=0) return;
	ASSERT(nBuf >= 0);
	
	va_end(args);

	OutputDebugString(szBuffer);
}