// Printer.cpp: implementation of the CPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Wizard.h"
#include "Printer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinter::CPrinter(CWinApp* app)
{
	m_app            = app;
	m_hDevMode       = NULL;
	m_hDevNames      = NULL;
	m_hSaveDevMode   = NULL;
	m_hSaveDevNames  = NULL;
	m_strdirname     = PRINTERSETTINGS_DIRECTORYNAME;
	m_strPrinterName = PRINTERNAME_UNDEFINED;
}

CPrinter::CPrinter(const CString name)
{
	CPrinter((CWinApp*)AfxGetApp());

	m_strdirname = name;
}

CPrinter::~CPrinter()
{
	if (m_hDevMode)	 GlobalFree(m_hDevMode);
	if (m_hDevNames) GlobalFree(m_hDevNames);

	RestorePrinter();				// if possible MFC-Printer will go back to m_hSaveDevMode
}

void CPrinter::operator=(const CPrinter* src)
{
	VERIFY(src != NULL);

	if (src != (CPrinter *) 0)
	{
		m_hDevMode       = CopyHandle(src->m_hDevMode);
		m_hDevNames      = CopyHandle(src->m_hDevNames);
		m_strPrinterName = src->m_strPrinterName;
		m_strdirname     = src->m_strdirname;
		m_strfilename    = src->m_strfilename;
	}
}

void CPrinter::operator=(const CPrinter& src)
{
	*this = &src;
}

//
// check to see if our printer is still available (m_strprintername)
//
BOOL CPrinter::IsPrinterAvailable(LPCTSTR pszDeviceName)
{
	HANDLE	hPrinter;

	if (OpenPrinter((char *)pszDeviceName, &hPrinter, NULL) == FALSE)
		return FALSE;
	ClosePrinter(hPrinter);
	return TRUE;
}


//
// change settings for global printer
// pszDeviceName is the name of your printer
//
BOOL CPrinter::SetPrinterDevice(LPCTSTR pszDeviceName) const
{
	HANDLE	hPrinter;
	PRINTER_DEFAULTS pd;
	int	ret = 0;
	
	ZeroMemory(&pd, sizeof(pd)); 
	pd.DesiredAccess = PRINTER_ALL_ACCESS;			// because of SetPrinter we need all access

	if (OpenPrinter((char *)pszDeviceName, &hPrinter, &pd) == FALSE)
		return FALSE;

	DWORD	dwBytesReturned, dwBytesNeeded;

	GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);
	PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,dwBytesNeeded);
	if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded,&dwBytesReturned) == 0)
	{
		GlobalFree(p2);
		ClosePrinter(hPrinter);
		return FALSE;
	}

	// Lock handle for DEVMODE and copy DEVMODE data to PRINTER_INFO_2.
	// (Instead you can change DEVMODE-fields at p2->pDevMode directly)
	DEVMODE* pDevMode = (DEVMODE*) GlobalLock(m_hDevMode);
	int	dwsize = sizeof(*p2->pDevMode);

	CopyDevmode(p2->pDevMode,pDevMode);
	DEVNAMES* lpDevNames   = (LPDEVNAMES)GlobalLock(m_hDevNames);
	LPTSTR	lpszDeviceName = (LPTSTR )lpDevNames + lpDevNames->wDeviceOffset;

	// check to see if our settings are valid
	ret = DocumentProperties(NULL,hPrinter,lpszDeviceName,NULL,pDevMode,DM_IN_BUFFER);
	p2->pSecurityDescriptor = NULL;
//	p2->Attributes = 0;
	p2->Priority = 0;
	if (IDOK == ret)					// change settings
		ret = SetPrinter(hPrinter,2,(LPBYTE)p2,0);

	GlobalFree(p2);						// free PRINTER_INFO_2.
	GlobalUnlock(m_hDevMode);	
	GlobalUnlock(m_hDevNames);
	ClosePrinter(hPrinter);

	SendMessage(HWND_BROADCAST, WM_DEVMODECHANGE, 0L, (LPARAM)pszDeviceName);
								// Notification: settings changed
	return ret;
}


//
// Show the printer common dialog
// Accept and store all settings
//
// This code is predominantly from a microsoft sample:
// HOWTO: Implementing an Application-Defined Default Printer 
// Article ID: Q193103 
//
CString CPrinter::PrinterSetup(CWnd* pWnd)
{
	ASSERT(pWnd);						// Ask the user which printer to use.
	
	CPrintDialog pd(FALSE, PD_ALLPAGES|PD_USEDEVMODECOPIES|PD_NOPAGENUMS|PD_HIDEPRINTTOFILE|PD_NOSELECTION, pWnd);

	// Make sure we don't accidentally create a device context
	pd.m_pd.Flags &= ~PD_RETURNDC;				// Reset flag set by constructor.

	// Force the CPrintDialog to use our device mode & name.
	if(m_hDevMode)
		pd.m_pd.hDevMode  = CopyHandle(m_hDevMode);
	if(m_hDevNames)
		pd.m_pd.hDevNames = CopyHandle(m_hDevNames);

	// Display the dialog box and let the user make their selection.

	if (pd.DoModal() == IDOK)
	{
		// The user clicked OK
		// (and POSSIBLY changed printers).
		// In any case, the CPrintDialog logic made a copy of the original
		// DEVMODE/DEVNAMES that we passed it and applied the user's
		// changes to that copy and discarded the original copy we passed
		// it. (NOTE: If the user had clicked CANCEL instead, the original
		// values we passed would have been returned unchanged).
		if(m_hDevMode)
			GlobalFree(m_hDevMode);                 // Free old copies.
		if(m_hDevNames)
			GlobalFree(m_hDevNames);                // Free old copies.
		if(pd.m_pd.hDevMode)				// Save new copies.
			m_hDevMode  = CopyHandle(pd.m_pd.hDevMode);
		if(pd.m_pd.hDevNames)				// Save new copies.
			m_hDevNames = CopyHandle(pd.m_pd.hDevNames);
	}

	// Regardless of whether the user clicked OK or CANCEL,
	// we need to ALWAYS do a GlobalFree of CPrintDialog's
	// m_pd.hDevMode and m_pd.hDevNames upon return from
	// DoModal in order to prevent a resource leak.
	GlobalFree(pd.m_pd.hDevMode);				// Because DoModal was called,
	GlobalFree(pd.m_pd.hDevNames);				// we need to free these.
	return DevmodePrinterName();
}

//
// This code is from a microsoft sample:
// HOWTO: Implementing an Application-Defined Default Printer 
// Article ID: Q193103 
//
HANDLE CPrinter::CopyHandle(HANDLE h)
{
	// Return a handle to a copy of the data that the passed handle was for.
	if (!h)
		return NULL;

	DWORD	dwLen = GlobalSize(h);
	HANDLE	hCopy = GlobalAlloc(GHND, dwLen);

	if (hCopy)
	{
		BYTE* lpCopy = (BYTE*)GlobalLock(hCopy);
		BYTE* lp     = (BYTE*)GlobalLock(h);
		if (lpCopy && lp)
			CopyMemory(lpCopy,lp,dwLen);
		GlobalUnlock(hCopy);
		GlobalUnlock(h);
	}
	return hCopy;
}

//
// retrieve our printername
//
CString CPrinter::DevmodePrinterName()
{
	if (!m_hDevMode)
		m_strPrinterName = PRINTERNAME_UNDEFINED;
	else 
	{
		DEVMODE* pDevMode = (DEVMODE*)GlobalLock(m_hDevMode);
		m_strPrinterName = (LPCTSTR) (pDevMode->dmDeviceName);
		GlobalUnlock(m_hDevMode);
	}
	return m_strPrinterName;
}


void CPrinter::CopyDefaultMfcPrinter()
{
	PRINTDLG pd;

	// Get MFC's printer
	if (m_app->GetPrinterDeviceDefaults(&pd))
	{
		// default setting page Orientation horizontally
		/*
		LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode);
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;  
		::GlobalUnlock(pd.hDevMode); 
		*/

		// Make a copy
		m_hDevNames = CopyHandle(pd.hDevNames);
		m_hDevMode = CopyHandle(pd.hDevMode);
		DevmodePrinterName();
	}
}

void CPrinter::SetThisPrinter()
{
	PRINTDLG pd;

	// Save MFC's printer and select ours instead.
	m_app->GetPrinterDeviceDefaults(&pd);
	m_hSaveDevNames = pd.hDevNames;
	m_hSaveDevMode  = pd.hDevMode;
	m_app->SelectPrinter(m_hDevNames, m_hDevMode, FALSE);
}

void CPrinter::RestorePrinter()
{
	// Restore previous MFC printer if possible
	if (m_hSaveDevNames && m_hSaveDevMode)
		m_app->SelectPrinter(m_hSaveDevNames, m_hSaveDevMode, FALSE);
	m_hSaveDevNames = NULL;
	m_hSaveDevMode  = NULL;
}

//
// Save our settings to file
//
int CPrinter::Save(LPCTSTR strFileName)
{
	SetFileName(strFileName);

	ASSERT(!m_strfilename.IsEmpty());
	ASSERT(!m_strdirname.IsEmpty());
	ASSERT(m_hDevMode);
	ASSERT(m_hDevNames);

	DWORD	dret = NO_ERROR;

	// create special dir for printer settings
	BOOL	ret = CreateDirectory(m_strdirname, NULL);
	if (!ret)
	{
		dret = GetLastError();
		if (dret != ERROR_ALREADY_EXISTS)
		{
			TRACE("error at CreateDirectory: %d\n",dret);
			return dret;
		}
	}
	// ask for the length of current directoryname 
	dret = GetCurrentDirectory(0, NULL);
	if (dret > 0 )
	{
		CString	od;

		LPTSTR	pst = od.GetBuffer(dret);		// just big enough
		dret = GetCurrentDirectory(dret,pst);		// to hold the string
		SetCurrentDirectory(m_strdirname);		// change to our directory

		HANDLE	hDatei = 				// create a new empty file
			CreateFile(
				m_strfilename,
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		if (NULL == hDatei)
		{
			TRACE("error at CreateFile: %d\n",GetLastError());
			return GetLastError();
		}

		DWORD	dwLen = GlobalSize(m_hDevMode);
		BYTE*	lp    = (BYTE*)GlobalLock(m_hDevMode);
		VERIFY(WriteFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
								// leading length
		VERIFY(WriteFile(hDatei,lp,dwLen,&dret,NULL));	// structure data
		if (dret != dwLen)
			TRACE("printersettings: problem writing DevMode %d <-> %d\n",dwLen,dret);
		GlobalUnlock(m_hDevMode);

		dwLen = GlobalSize(m_hDevNames);
		lp    = (BYTE*)GlobalLock(m_hDevNames);
		VERIFY(WriteFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
								// leading length
		VERIFY(WriteFile(hDatei,lp,dwLen,&dret,NULL));
								// structure data
		if (dret != dwLen)
			TRACE("printersettings: problem writing DevNames %d <-> %d\n",dwLen,dret);
		GlobalUnlock(m_hDevNames);

		CloseHandle(hDatei);
		SetCurrentDirectory(pst);
		od.ReleaseBuffer();
	}
	return NO_ERROR;
}

//
// load our settings from file
//
int CPrinter::Load(LPCTSTR strFileName)
{
	SetFileName(strFileName);

	ASSERT(!m_strfilename.IsEmpty());
	ASSERT(!m_strdirname.IsEmpty());

	if (m_hDevMode)		GlobalFree(m_hDevMode);
	if (m_hDevNames)	GlobalFree(m_hDevNames);

	m_hDevMode = m_hDevNames = NULL;
	DWORD dret = NO_ERROR;
	
	// use a special directory for our printer settings
	BOOL ret = CreateDirectory(m_strdirname,NULL);
	if (!ret)
	{
		dret = GetLastError();
		if (dret != ERROR_ALREADY_EXISTS)
		{
			TRACE("error at CreateDirectory: %d\n",dret);
			return dret;
		}
	}

	dret = GetCurrentDirectory(0,NULL);
	if (dret > 0 )
	{
		CString	od;

		od.~od();
		LPTSTR pst = od.GetBuffer(dret + 1);
		dret = GetCurrentDirectory(dret,pst);
		SetCurrentDirectory(m_strdirname);

		HANDLE hDatei = 				// open file, ensure it is present
			CreateFile(
				m_strfilename,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		// if the file could not be found
		// use MFC's fefault printer
		if (INVALID_HANDLE_VALUE == hDatei)
		{
			dret = GetLastError();
			CopyDefaultMfcPrinter();
			SetCurrentDirectory(pst);
			DevmodePrinterName();
			return dret;
		}

		DWORD   dwLen = 0;
		VERIFY(ReadFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
								// read leading length
		ASSERT(dwLen < 0xFFFF);				// if desired do more testing for proper size here
		ASSERT(dwLen > 0);
		m_hDevMode = GlobalAlloc(GHND, dwLen);
		if (NULL == m_hDevMode)
		{
			dret = GetLastError();
			TRACE("error at GlobalAlloc m_hDevMode: %d\n",dret);
			CloseHandle(hDatei);
			return dret;
		}
		BYTE* lpCopy = (BYTE*)GlobalLock(m_hDevMode);
		VERIFY(ReadFile(hDatei,lpCopy,dwLen,&dret,NULL));
								// read the whole caboodle
		GlobalUnlock(m_hDevMode);
		VERIFY(ReadFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
								// read leading length
		ASSERT(dwLen < 0xFFFF);				// is it believable
		ASSERT(dwLen > 0);
		m_hDevNames = GlobalAlloc(GHND, dwLen);
		if (NULL == m_hDevNames)
		{
			dret = GetLastError();
			TRACE("error at GlobalAlloc m_hDevNames: %d\n",dret);
			CloseHandle(hDatei);
			return dret;
		}
		lpCopy = (BYTE*)GlobalLock(m_hDevNames);
		VERIFY(ReadFile(hDatei,lpCopy,dwLen,&dret,NULL));
								// read DevNames structure
		GlobalUnlock(m_hDevNames);
		CloseHandle(hDatei);
		SetCurrentDirectory(pst);
	}
	DevmodePrinterName();
	return NO_ERROR;
}

int CPrinter::CopyDevmode(DEVMODE *pDest,const DEVMODE *pSrc) const
{
	int nBytes = min( pDest->dmSize + pDest->dmDriverExtra, pSrc->dmSize + pSrc->dmDriverExtra);
	CopyMemory(pDest,pSrc,nBytes);
	return nBytes;
}
