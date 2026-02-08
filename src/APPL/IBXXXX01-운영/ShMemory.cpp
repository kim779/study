// ShMemory.cpp : implementation file
//

#include "stdafx.h"
#include "ShMemory.h"
#include "../../h/axisfire.h"

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
	InitializeCriticalSection(&csMapHandle);
	
	m_hKeyFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, remainSHMEMNAME);

	if (m_hKeyFile)
		CloseHandle(m_hKeyFile);

	m_hKeyFile = ::CreateFileMapping(
		(HANDLE) 0xffffffff,	// use paging file
		NULL,			// no security
		PAGE_READWRITE,		// access mode
		0,			// size/high
		remainSHMEMSIZE,	// size/low
		remainSHMEMNAME);	// name of map object
/*	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		m_hKeyFile = NULL;
		return FALSE;			
	}
*/	m_bMaster = true;

	if(!m_hKeyFile)
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
//TRACE("%s%d\n", "Master 핸들 : ", (int)hWnd);
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
			if (ii == GetHandleCount() -1)
			{
				*(long*)m_pKeyView = (long)(ii);
			}
			else
			{
				void* pCur  = (void *)((int)m_pKeyView + sizeof(long) + sizeof(HWND) * ii);
				void* pNext = (void *)((int)pCur + sizeof(HWND));
				CopyMemory(pCur, pNext, sizeof(HWND) * (GetHandleCount() - ii -1));
			}
			*(HWND*)((int)m_pKeyView + sizeof(long) + sizeof(HWND) * (GetHandleCount() -1)) = NULL;
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

void CShMemory::SendTrigger(CString Trigger,HWND _hwnd)
{
	if ( _hwnd > 0)
	{
//		TRACE("\tSendTrigger - %x\n", _hwnd);
		::SendMessage(_hwnd, WM_USER, MAKEWPARAM(MAKEWORD(DLL_TRIGGER, -1), false), (LPARAM)(LPCTSTR)Trigger);
		
		return;
	}

	for (int ii = 1/*0:Master*/; ii < GetHandleCount(); ii++)
	{
		HWND hWnd = GetHandle(ii);
//		TRACE("\tSendTrigger - %x\n", hWnd);
//TRACE("%s%d\n", "Master에서 보내는 Control hwnd : ", (int)hWnd);
		
		//TRACE(Trigger+"]1\n");
		if (Trigger.Find("|")>1)
		{
			Trigger = Trigger.Mid(0,Trigger.Find("|"));
			//Trigger.Delete(Trigger.Find("|"),3);
		}
		//TRACE(Trigger+"]2\n");
		
		::SendMessage(hWnd, WM_USER, MAKEWPARAM(MAKEWORD(DLL_TRIGGER, -1), false), (LPARAM)(LPCTSTR)Trigger);
	}
}

void CShMemory::Close()
{
	if (m_pKeyView) UnmapViewOfFile(m_pKeyView);
	if (m_hKeyFile) 
//		TRACE("Close Handle : %d\n", (int)CloseHandle(m_hKeyFile));
	DeleteCriticalSection(&csMapHandle);
}
