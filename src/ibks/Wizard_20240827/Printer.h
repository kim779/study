// Printer.h: interface for the CPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTER_H__F91ACCC5_0274_4C94_9E10_D88D47A0C1A5__INCLUDED_)
#define AFX_PRINTER_H__F91ACCC5_0274_4C94_9E10_D88D47A0C1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winspool.h>						// ClosePrinter, GetPrinter, PRINTER_INFO_2

#define	LOGPIXEL_X		96
#define PRINTERNAME_UNDEFINED		"Printer not Define"
#define PRINTERSETTINGS_DIRECTORYNAME	"Printer Settings"

class CPrinter  
{
public:
	CPrinter(CWinApp* app);
	CPrinter(const CString name);
	virtual ~CPrinter();

public:
	void operator=(const CPrinter* src);
	void operator=(const CPrinter& src);

private:
	CWinApp*	m_app;
	HANDLE		m_hDevMode;
	HANDLE		m_hDevNames;
	HANDLE		m_hSaveDevMode;
	HANDLE		m_hSaveDevNames;
	CString		m_strPrinterName;
	CString		m_strfilename;
	CString		m_strdirname;

public:
	bool	ValidDevMode(void) const
				{ return m_hDevMode != NULL; }
	void	CopyDefaultMfcPrinter();			// get the settings for the default MFC Printer
	void	SetThisPrinter();				// Set the default MFC Printer to this

	BOOL	SetPrinterDevice(LPCTSTR pszDeviceName) const;	// change settings for global Windows Printer
	CString	PrinterSetup(CWnd* pWnd);			// Show the common dialog and get the settings
	void	RestorePrinter();				// restore previous printer settings (if safed)

	CString	GetFileName( void ) const
				{ return m_strfilename; }
	CString	SetDirName( const CString neu) 
				{ CString temp = m_strdirname; m_strdirname = neu; return temp; }
	CString	GetDirName( void ) const
				{ return m_strdirname; }

	int	Save(LPCTSTR strFilename);
	int	Load(LPCTSTR strFilename);
	BOOL	IsPrinterAvailable(LPCTSTR pszDeviceName);	// check to see if our printer is still available

	CString GetPrinterName(void) const			// retrieve the human readable printername :)
				{ return m_strPrinterName; }

private:
	CString	DevmodePrinterName(void);
	HANDLE	CopyHandle(HANDLE h); 
	int	CopyDevmode( DEVMODE* pDest,const DEVMODE * pSrc) const;
								// copy whole DEVMODE ( with dmSize + dmDriverExtra )
	CString	SetFileName( const CString neu) 
				{ CString temp = m_strfilename; m_strfilename = neu; return temp; }
};

#endif // !defined(AFX_PRINTER_H__F91ACCC5_0274_4C94_9E10_D88D47A0C1A5__INCLUDED_)
