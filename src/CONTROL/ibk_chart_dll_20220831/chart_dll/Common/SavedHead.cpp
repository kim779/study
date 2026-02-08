// SavedHead.cpp: implementation of the CSavedHead class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SavedHead.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSavedHead::CSavedHead()
{
	m_pcBuf	= NULL;
	ZeroMemory(&m_sh, SZ_SAVEDHEAD);
}

CSavedHead::~CSavedHead()
{
	if (m_pcBuf)
	{
		delete m_pcBuf;
		m_pcBuf = NULL;
	}
}

char* CSavedHead::Extract(char* pcBuf)
{
	CopyMemory(&m_sh, pcBuf, SZ_SAVEDHEAD);

	return (char*)pcBuf + SZ_SAVEDHEAD;
}

char* CSavedHead::ReadFile(CString strFullPathFileName, DWORD& dwReadLen)
{
	DWORD dwAccess		= GENERIC_READ;
	DWORD dwShareMode	= FILE_SHARE_READ;
	DWORD dwDistribution	= OPEN_EXISTING;

	HANDLE hFile = CreateFile(strFullPathFileName.operator LPCTSTR(), dwAccess, dwShareMode, NULL, dwDistribution, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		//Debug("CreateFile() Error [%d]\n", GetLastError());
		return NULL;
	}

	DWORD dwSize = GetFileSize (hFile, NULL) ; 
	if (dwSize == INVALID_FILE_SIZE) 
	{
		//Debug("GetFileSize() Error [%d]\n", GetLastError());
		return NULL;
	}

	m_pcBuf = new char[dwSize];
	BOOL bResult = ::ReadFile(hFile, m_pcBuf, dwSize, &dwReadLen, NULL);
	CloseHandle(hFile);

	if (dwReadLen != dwSize || !bResult)
	{
		delete m_pcBuf;
		m_pcBuf = NULL;
	}
	else
	{
		CopyMemory(&m_sh, m_pcBuf, SZ_SAVEDHEAD);
	}

	return m_pcBuf;
}

bool CSavedHead::WriteFile(CString strFullPathFileName, void* pvBuf, DWORD dwWriteLen)
{
	DWORD dwAccess = GENERIC_WRITE;
	DWORD dwShareMode = FILE_SHARE_WRITE;
	DWORD dwDistribution = CREATE_ALWAYS;
	CString strTempFileName = strFullPathFileName + '_';

	HANDLE hFile = CreateFile(strTempFileName.operator LPCTSTR(), dwAccess, dwShareMode, NULL, dwDistribution, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("CreateFile() Error [%d]\n", GetLastError());
		return false;
	}

	DWORD dwWroteLen;
	BOOL bResult = ::WriteFile(hFile, pvBuf, dwWriteLen, &dwWroteLen, NULL);
	if (!bResult)
	{
		TRACE("WriteFile() Error [%d]\n", GetLastError());
		return false;
	}

	if (dwWriteLen != dwWroteLen)
	{
		TRACE("Write Length Not Match\n");
		return false;
	}

	bResult = FlushFileBuffers(hFile);
	if (!bResult)
	{
		TRACE("FlushFileBuffers() Error [%d]\n", GetLastError());
		return false;
	}

	bResult = CloseHandle(hFile);
	if (!bResult)
	{
		TRACE("CloseHandle() Error [%d]\n", GetLastError());
		return false;
	}

	bResult = CopyFile(strTempFileName, strFullPathFileName, FALSE);
	if (!bResult)
	{
		TRACE("CopyFile() Error [%d]\n", GetLastError());
		return false;
	}

	DeleteFile(strTempFileName);

	return true;
}

/*
bool CSavedHead::Write(CString strFullPathFileName, DWORD dwVerstion, DWORD dwDataCount, DWORD dwOneDataSize, char* pcBuf)
{
	Make(dwVersion, dwDataCount, dwOndDataSize, pcBuf);

	return WriteFile(strFullPathFileName, pcBuf, m_sh.dwTotalLength);

}
*/
bool CSavedHead::Write(CString strFullPathFileName)
{
	return WriteFile(strFullPathFileName, m_pcBuf, m_sh.dwTotalLength);
}


char* CSavedHead::Read(CString strFullPathFileName, DWORD& dwReadLen)
{
	m_pcBuf = ReadFile(strFullPathFileName, dwReadLen);
	if (!m_pcBuf || dwReadLen < 1)
		return NULL;

	return m_pcBuf + SZ_SAVEDHEAD;
}


char* CSavedHead::Make(DWORD dwVerstion, DWORD dwDataCount, DWORD dwOneDataSize)
{
	CTime time = CTime::GetCurrentTime();
	CString strDate;
	strDate.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CString strTime;
	strTime.Format("%02d%02d%02d", time.GetHour(), time.GetMinute(), time.GetSecond());

	m_sh.dwVersion = dwVerstion;
	m_sh.dwDataCnt = dwDataCount;
	m_sh.dwOneDataSize = dwOneDataSize;
	m_sh.dwDataLength = dwDataCount * dwOneDataSize;
	m_sh.dwTotalLength = SZ_SAVEDHEAD + m_sh.dwDataLength;
	
	CopyMemory(m_sh.acDate, strDate.operator LPCTSTR(), sizeof(m_sh.acDate));
	CopyMemory(m_sh.acTime, strTime.operator LPCTSTR(), sizeof(m_sh.acTime));

	m_pcBuf = new char[m_sh.dwTotalLength];
	ZeroMemory(m_pcBuf, m_sh.dwTotalLength);
	CopyMemory(m_pcBuf, &m_sh, SZ_SAVEDHEAD);

	return m_pcBuf + SZ_SAVEDHEAD;
}