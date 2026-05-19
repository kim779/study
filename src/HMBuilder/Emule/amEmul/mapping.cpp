// mapping.cpp : implementation file
//

#include "stdafx.h"
#include "mapping.h"
//#include "../h/dbgout.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapping 

#define	L_shm		sizeof(struct _shm) + 1
#define	N_shm		"_udpDataX64_#"

bool CMapping::initMapping()
{
	bool create = false;
	m_handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, N_shm);
	if (m_handle == NULL)
	{
		m_handle = CreateFileMapping(
				INVALID_HANDLE_VALUE,		// use paging file
				NULL,				// no security
				PAGE_READWRITE,			// access mode
				0,				// size/high
				L_shm,				// size/low
				N_shm);				// name of map object
		if (m_handle == NULL)
			return false;
		create = true;
	}

	LPVOID lpvMem = MapViewOfFile(
			m_handle,			// object to map view
			FILE_MAP_ALL_ACCESS,		// access mode
			0,				// offset/high
			0,				// offset/low
			0);				// map file count

	if (lpvMem == NULL)
	{
		CloseHandle(m_handle);
		m_handle = NULL;
		return false;
	}

	m_shm = (struct _shm *) lpvMem;

	if (create)
	{
		m_shm->widx = m_shm->ridx = 0;
		m_shm->hwnd[0] = m_shm->hwnd[1] = (HWND) 0;
		ZeroMemory(m_shm->actF, actBUF+1);
		ZeroMemory(m_shm->actB, maxBUF+1);
		m_shm->attach = 1;
	}
	else
		m_shm->attach++;

	return true;
}

void CMapping::endMapping()
{
	if (m_handle != NULL)
	{
		m_shm->attach--;
		if (!m_shm->attach)
		{
			UnmapViewOfFile((LPVOID) m_shm);
			CloseHandle(m_handle);
			m_handle = NULL;
		}
	}
}

void CMapping::setHwnd(int idx, HWND hwnd)
{
	if (m_shm != NULL)
		m_shm->hwnd[idx] = hwnd;
}

LRESULT CMapping::sendMsg(TOmsg to, WPARAM wp, LPARAM lp)
{
	if (m_shm != NULL && IsWindow(m_shm->hwnd[to]))
		return ::SendMessage(m_shm->hwnd[to], WM_CMDDATA, wp, lp);
	return 0;
}

int CMapping::readCount()
{
	int readC = m_shm->widx - m_shm->ridx;
	if (readC < 0) readC += maxQUE;

	return readC;
}

bool CMapping::writeData(CString code, int symC, WORD* symB, CString datB, bool trade, bool notify)
{
	if (readCount() >= maxQUE)
		return false;

	m_sync.Lock();

	int	len = code.GetLength();
	CopyMemory(m_shm->mque[m_shm->widx].m_code, code.operator LPCTSTR(), len);
	m_shm->mque[m_shm->widx].m_code[len] = '\0';

	m_shm->mque[m_shm->widx].m_nMbols = symC;
	CopyMemory(m_shm->mque[m_shm->widx].m_pMbols, symB, symC*sizeof(WORD));

	m_shm->mque[m_shm->widx].m_nBytes = datB.GetLength();
	CopyMemory(m_shm->mque[m_shm->widx].m_pBytes, datB.operator LPCTSTR(), m_shm->mque[m_shm->widx].m_nBytes);
	m_shm->mque[m_shm->widx].m_pBytes[m_shm->mque[m_shm->widx].m_nBytes] = '\0';
	m_shm->mque[m_shm->widx].m_trade = trade;

//	if (notify && m_shm->hwnd[0] != (HWND) 0)
//		::PostMessage(m_shm->hwnd[0], WM_MAPDATA, 0, 0);

	m_shm->widx++;
	if (m_shm->widx >= maxQUE)
		m_shm->widx = 0;

	m_sync.Unlock();
	return true;
}

bool CMapping::readData(CString& code, CWordArray* rtmk, char*& datB, bool& trade)
{
	if (readCount() <= 0)
		return false;

	code = CString(m_shm->mque[m_shm->ridx].m_code, (int)strlen(m_shm->mque[m_shm->ridx].m_code));

	rtmk->RemoveAll();
	for (int ii = 0; ii < m_shm->mque[m_shm->ridx].m_nMbols; ii++)
		rtmk->Add(m_shm->mque[m_shm->ridx].m_pMbols[ii]);

//	datB = CString(m_shm->mque[m_shm->ridx].m_pBytes, m_shm->mque[m_shm->ridx].m_nBytes);
	datB = m_shm->mque[m_shm->ridx].m_pBytes;
	trade = m_shm->mque[m_shm->ridx].m_trade;

	m_shm->ridx++;
	if (m_shm->ridx >= maxQUE) m_shm->ridx = 0;

	return true;
}

int CMapping::runProcess(CString home, CString pnam)
{
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	BOOL	fBool;
	CString	aps, cmd;

	aps.Format("%s/%s/%s", home, "exe", pnam);
	cmd.Format(" /x %s /p %s", "exeCast", home);

	fBool = CreateProcess(
		aps,					// application name
		(char*)cmd.operator LPCTSTR(),		// command line
		NULL,					// process attribute
		NULL,					// thread attribute
		FALSE,					// is inherit handle
		NORMAL_PRIORITY_CLASS,			// creation flags
		NULL,					// environment
		NULL,					// current directory
		&si,					// STARTUPINFO
		&pi);					// PROCESS_INFORMATION

	if (!fBool || !pi.hProcess)
		return GetLastError();

	return 0;
}

void CMapping::killProcess()
{
	if (m_shm != NULL && IsWindow(m_shm->hwnd[toCast]))
	{
		::SendMessage(m_shm->hwnd[toCast], WM_DESTROY, 0, 0);
		WaitForSingleObject(m_shm->hwnd[toCast], 3000); Sleep(500);
		if (::IsWindow(m_shm->hwnd[toCast]))
			::SendMessage(m_shm->hwnd[toCast], WM_CLOSE, 0, 0);
	}
}

void CMapping::sendMsg()
{
	if (m_shm->hwnd[0] != (HWND) 0)
		::PostMessage(m_shm->hwnd[0], WM_MAPDATA, 0, 0);
}
