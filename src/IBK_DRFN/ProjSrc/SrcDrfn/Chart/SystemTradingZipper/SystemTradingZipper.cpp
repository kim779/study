// SystemTradingZipper.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SystemTradingZipper.h"
#include "../Include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingZipperApp

BEGIN_MESSAGE_MAP(CSystemTradingZipperApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTradingZipperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingZipperApp construction

CSystemTradingZipperApp::CSystemTradingZipperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_zip.SetSpanCallback(NULL, this);
	m_zip.SetAdvanced(1500000);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTradingZipperApp object

CSystemTradingZipperApp theApp;

extern "C" __declspec(dllexport) BOOL EX_CompressFile(LPSTR lpszZip, LPSTR lpszFile)
{
	theApp.m_zip.Open(lpszZip, CZipArchive::create, 1457500);
	BOOL bReturn = theApp.m_zip.AddNewFile(lpszFile);
	theApp.m_zip.Close();

	return bReturn;
}

extern "C" __declspec(dllexport) BOOL EX_CompressFiles(LPSTR lpszZip, CStringArray& arFiles)
{
	theApp.m_zip.Open(lpszZip, CZipArchive::create, 1457500);

	int nSize = arFiles.GetSize();
	for(int i = 0; i < nSize; i++)
		theApp.m_zip.AddNewFile(arFiles.GetAt(i));

	theApp.m_zip.Close();

	return TRUE;
}

extern "C" __declspec(dllexport) BOOL EX_ExtractFile(LPCSTR lpszZip, LPSTR lpszFolder)
{
	theApp.m_zip.Open(lpszZip, CZipArchive::open, 0);
	for(int i = 0; i < theApp.m_zip.GetNoEntries(); i++)
		theApp.m_zip.ExtractFile(i, lpszFolder);

	return TRUE;
}

extern "C" __declspec(dllexport) void EX_ExtractFileNotify(LPCSTR lpszZip, LPSTR lpszFolder, FpNotifyExtract pNotify)
{
	CFileHeader header;
	theApp.m_zip.Open(lpszZip, CZipArchive::open, 0);

	for(int i = 0; i < theApp.m_zip.GetNoEntries(); i++)
	{
		if(theApp.m_zip.ExtractFile(i, lpszFolder))
		{
			theApp.m_zip.GetFileInfo(header, i);

			if(theApp.m_zip.GetFileExt(header.m_szFileName).CompareNoCase(_T(".sd")) == 0)
				pNotify(TRUE, theApp.m_zip.GetFileTitle(header.m_szFileName));
			else
				pNotify(FALSE, theApp.m_zip.GetFileTitle(header.m_szFileName));
		}
	}
}

