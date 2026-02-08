// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

BOOL CClipboardManager::IsDeskClipboardFormatAvailable(UINT uFormat)
{
//	if(!::IsClipboardFormatAvailable(uFormat))
//		return FALSE;
//
//	char	szTmp[10]={0,};
//	int		nSize = 9;
//	if(!GetData(uFormat, szTmp, nSize))
//		return FALSE;
//
//	if( strcmp(szTmp, NCB_BLANKDATA)==0) {
//		return FALSE;
//	}

	return TRUE;
}

BOOL CClipboardManager::GetData(UINT uFormat, LPVOID lpszBuffer ,int nBufSize ,HWND &hWnd)
{
	HGLOBAL hGlobal;		// Global memory handle
	LPVOID lpszData;		// Pointer to clipboard data
	unsigned long nSize;	// Size of clipboard data

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	OpenClipboard(hWnd);

	// Request a pointer to the Data on the clipboard.
	hGlobal = GetClipboardData(uFormat);

	// If there was no Data on the clipboard, we have
	// been returned a NULL handle.	
	if (hGlobal == NULL) return FALSE;

	// Now we have a global memory handle to the Data
	// stored on the clipboard. We have to lock this global
	// handle so that we have access to it.
	lpszData = GlobalLock(hGlobal);
	// Now get the size of the Data on the clipboard.
	nSize = GlobalSize(hGlobal);

	// Make sure the Data on the clipboard is not longer
	// that the buffer that was allocated for it. If it was
	// snip the Data on the clipboard so that it fits.
	if(nSize >= (UINT)nBufSize) nSize = nBufSize - 1;

	// Now, copy the Data into the return buffer. At the
	// end, we need to add a NULL string terminator.
	memcpy(lpszBuffer, lpszData, nSize);
//	for (UINT i = 0; i < nSize; ++i)
//		*(lpszBuffer + i) = *(lpszData + i);
//	*(lpszBuffer + i) = 0;

	// Now, simply unlock the global memory pointer
	// and close the clipboard.
	GlobalUnlock(hGlobal);
	CloseClipboard();

	return TRUE;
}

BOOL CClipboardManager::GetDataLength(UINT uFormat, unsigned long *pnSize, HWND &hWnd)
{
	HGLOBAL hGlobal;		// Global memory handle
	unsigned long nSize;	// Size of clipboard data
	LPVOID lpszData;		// Pointer to clipboard data

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed.
	OpenClipboard(hWnd);

	// Request a pointer to the Data on the clipboard.
	hGlobal = GetClipboardData(uFormat);

	// If there was no Data on the clipboard, we have
	// been returned a NULL handle.	
	if (hGlobal == NULL) return FALSE;

	// Now we have a global memory handle to the Data
	// stored on the clipboard. We have to lock this global
	// handle so that we have access to it.
	lpszData = GlobalLock(hGlobal);
	// Now get the size of the Data on the clipboard.
	nSize = GlobalSize(hGlobal);

	// Now, simply unlock the global memory pointer
	// and close the clipboard.
	GlobalUnlock(hGlobal);
	CloseClipboard();

	// Finally, save the length of the string we found
	// into the pnSize pointer and return.
	*pnSize = nSize;
	return TRUE;
}
	
BOOL CClipboardManager::SetData(UINT uFormat, LPVOID lpszBuffer, unsigned long nSize, HWND &hWnd)
{
	HGLOBAL hGlobal;		// Global memory handle
	LPVOID lpszData;		// Pointer to clipboard data
//	unsigned long nSize;	// Size of clipboard data

	// First, open the clipboard. OpenClipboard() takes one
	// parameter, the handle of the window that will temporarily
	// be it's owner. If NULL is passed, the current process
	// is assumed. After opening, empty the clipboard so we
	// can put our Data on it.
	OpenClipboard(hWnd);
//	Empty(uFormat, hWnd);

	// Get the size of the string in the buffer that was
	// passed into the function, so we know how much global
	// memory to allocate for the string.
//	nSize = lstrlen(lpszBuffer);

	// Allocate the memory for the string.
	hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);
	
	// If we got any error during the memory allocation,
	// we have been returned a NULL handle.
	if (hGlobal == NULL) return FALSE;

	// Now we have a global memory handle to the Data
	// stored on the clipboard. We have to lock this global
	// handle so that we have access to it.
	lpszData = GlobalLock(hGlobal);

	// Now, copy the Data from the buffer into the allocated
	// global memory pointer.
//	for (UINT i = 0; i < nSize + 1; ++i)
//		*(lpszData + i) = *(lpszBuffer + i);
	memcpy(lpszData, lpszBuffer, nSize);

	// Now, simply unlock the global memory pointer,
	// set the clipboard data type and pointer,
	// and close the clipboard.
	GlobalUnlock(hGlobal);
	SetClipboardData(uFormat, hGlobal);
	CloseClipboard();

	return TRUE;
}

BOOL CClipboardManager::Empty(UINT uFormat, HWND &hWnd)
{
	if(OpenClipboard(hWnd)) {
		HGLOBAL hGlobal = GetClipboardData(uFormat);
		LPCSTR szBlank = NCB_BLANKDATA;
		LPVOID lpszData;		// Pointer to clipboard data

		int nSize = strlen(szBlank);
		hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);
		if (hGlobal == NULL) return FALSE;
	
		lpszData = GlobalLock(hGlobal);
		memcpy(lpszData, szBlank, nSize);
		
		// Now, simply unlock the global memory pointer,
		// set the clipboard data type and pointer,
		// and close the clipboard.
		GlobalUnlock(hGlobal);
		BOOL bRet = (SetClipboardData(uFormat, hGlobal) != NULL);
		CloseClipboard();
		// return EmptyClipboard();
		return TRUE;
	}
	return FALSE;
}
