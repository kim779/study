// ShMemory.cpp : implementation file
//

#include "stdafx.h"
#include "ShMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define	remainSHMEMSIZE	sizeof(HWND) * 50
//#define	remainSHMEMNAME	"_remainCtrl_"

/////////////////////////////////////////////////////////////////////////////
// CShMemory

CShMemory::CShMemory()
{
	m_hKeyFile = NULL;
	m_pKeyView = NULL;
	m_bMaster = false;
}

CShMemory::~CShMemory()
{
}

/////////////////////////////////////////////////////////////////////////////
// CShMemory message handlers

BOOL CShMemory::InitSharedMemory(HWND hWnd)
{
	// Share memory
	m_bMaster = false;

	m_hKeyFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, remainSHMEMNAME);
	if (m_hKeyFile == NULL)
	{
		m_hKeyFile = CreateFileMapping(
			(HANDLE) 0xffffffff,	// use paging file
			NULL,			// no security
			PAGE_READWRITE,		// access mode
			0,			// size/high
			remainSHMEMSIZE,	// size/low
			remainSHMEMNAME);	// name of map object
	
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_hKeyFile = NULL;
			return FALSE;			
		}
		m_bMaster = true;
	}
	
	if (!m_hKeyFile)
	{
		ASSERT(m_hKeyFile);
		m_pKeyView = NULL;
		return FALSE;
	}
	
	m_pKeyView = (char *)::MapViewOfFile(m_hKeyFile, FILE_MAP_ALL_ACCESS, 0, 0, remainSHMEMSIZE);
	if (!m_pKeyView)
	{
		::CloseHandle(m_hKeyFile);
		return FALSE;
	}
	
	if (m_bMaster)
		ZeroMemory(m_pKeyView, remainSHMEMSIZE);

	return TRUE;	
}

long CShMemory::GetHandleCount()
{
	return (long)*m_pKeyView;
}

void CShMemory::AddHandle(HWND hWnd)
{
//TRACE("%s%d\n", "Control hwnd : ", (int)hWnd);
	if (hWnd == NULL) return;
	
	int nCnt = GetHandleCount();
	for (int ii = 0; ii < nCnt; ii++)
		if (GetHandle(ii) == hWnd) return;
		
	*(HWND*)((int)m_pKeyView + sizeof(long) + sizeof(HWND) * nCnt) = hWnd;
	*(long*)m_pKeyView = (long)(nCnt + 1);
}

void CShMemory::RemoveHandle(HWND hWnd)
{
	for (int ii = 0; ii < GetHandleCount(); ii++)
	{
		if (GetHandle(ii) == hWnd)
		{
//TRACE("%d\n", GetHandle(0));
			if (ii == GetHandleCount() -1)
			{
				*(long*)m_pKeyView = (long)(ii);
			}
			else
			{
				void* pCur  = (void *)((int)m_pKeyView + sizeof(long) + sizeof(HWND) * ii);
				void* pNext = (void *)((int)pCur + sizeof(HWND));
				CopyMemory(pCur, pNext, sizeof(HWND) * (GetHandleCount() - ii -1));
				*(long*)m_pKeyView = GetHandleCount()-1;
			}
			*(HWND*)((int)m_pKeyView + sizeof(long) + sizeof(HWND) * GetHandleCount()) = NULL;
//TRACE("%d\n", GetHandle(0));
			return;
		}
	}
}

HWND CShMemory::GetHandle(int Index)
{
	if (Index >= GetHandleCount())
		return NULL;
	
	return *(HWND*)((int)m_pKeyView + sizeof(long) + sizeof(HWND) * Index);
}

void CShMemory::SendTrigger(CString Trigger)
{
	for (int ii = 0; ii < GetHandleCount(); ii++)
	{
		HWND hWnd = GetHandle(ii);
		::SendMessage(hWnd, WM_USER, MAKEWPARAM(MAKEWORD(0x04, 0), 1), (LPARAM)Trigger.GetString());
	}
}

void CShMemory::Close()
{
	if (m_pKeyView)
		UnmapViewOfFile(m_pKeyView);
	if (m_hKeyFile)
	{
		const	BOOL	bRet = CloseHandle(m_hKeyFile);
//		TRACE("Close Handle : %d\n", (int)bRet);
	}
}
