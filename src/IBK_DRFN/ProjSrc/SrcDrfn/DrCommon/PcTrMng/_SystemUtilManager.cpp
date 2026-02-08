#include "stdafx.h"
#include "PcTrMng.h"
#include "../../inc/ProgIDInfo.h"

// Manager class


////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {IsRegistered}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {2006/10/03}
// 04. function기능    : {OCX/DLL의 등록된 여부를 리턴한다.}
// 05. 입력인자        : {LPCSTR szProgID, CLSID &clsid}
// 06. 출력인자        : {clsid 성공할 경우 값이 채워진다.}
// 07. return 값       : {BOOL형 TRUE/FALSE}
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////
BOOL CSystemUtilManager::IsRegistered(LPCSTR szProgID, CLSID &clsid)
{
	HRESULT hr = AfxGetClassIDFromString(szProgID, &clsid);
	if (FAILED(hr))		return FALSE;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// 01. function명      : {DLLRegister}
// 02. 작  성  자      : {이준옥}
// 03. 작 성 일 자     : {2006/10/03}
// 04. function기능    : {OCX/DLL의 등록처리를 한다.}
// 05. 입력인자        : {szFileName}
// 06. 출력인자        : {}
// 07. return 값       : {처리결과}
//						 0 : 성공
//						 1 : DLLRegister OleInitialize 실패
//						 2 : LoadLibraryEx 에러
//						 3 : GetProcAddress(DllRegisterServer) 에러
//						 4 : DllRegisterServer 에러
// 08. REMARKS 사항    : {}
////////////////////////////////////////////////////////////////////////////////
// int CSystemUtilManager::DLLRegister(LPCSTR szFileName)
// {
// 	int			iReturn=0;
// 	CString		szErrorMsg;
// 
// 	// Initialize OLE.
// 	if (FAILED(OleInitialize(NULL))) {
// 		// AfxMessageBox("DLLRegister OleInitialize 실패");
// 		return 1;
// 	}
// 
// 	char aCurPath[MAX_PATH]={0,};
// 	::GetCurrentDirectory(MAX_PATH, aCurPath);
// 	{
// 		CString szPath(szFileName);
// 		int nPos = szPath.ReverseFind('\\');
// 		if(nPos<0) nPos = szPath.ReverseFind('/');
// 		if(nPos>0)
// 		{
// 			szPath = szPath.Left(nPos);
// 			::SetCurrentDirectory(szPath);
// 		}
// 	}
// 
// 	// SetErrorMode(SEM_FAILCRITICALERRORS);       // Make sure LoadLib fails.
// 	// Load the library.
// 	HINSTANCE hLib = LoadLibraryEx(szFileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
// 	if (hLib < (HINSTANCE)HINSTANCE_ERROR) {
// 		szErrorMsg.Format("File Name=%s, GetLastError() NO = 0x%08lx\n", szFileName, GetLastError());
// 		// AfxMessageBox(szErrorMsg);
// 		iReturn = 2;
// 		goto CleanupOle;
// 	}
// 
// 	HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);
// 	// Find the entry point.
// 	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");
// 	if (lpDllEntryPoint == NULL) {
// 		TCHAR szExt[_MAX_EXT];
// 		_tsplitpath(szFileName, NULL, NULL, NULL, szExt);
// 		if ((_stricmp(szExt, ".dll") != 0) && (_stricmp(szExt, ".ocx") != 0)) {
// 			szErrorMsg.Format("File Name=%s, GetProcAddress Fail\n", szFileName);
// 			// AfxMessageBox(szErrorMsg);
// 		}
// 		iReturn = 3;
// 		goto CleanupLibrary;
// 	}
// 
// 	// Call the entry point.
// 	if (FAILED((*lpDllEntryPoint)())) {
// 		szErrorMsg.Format("File Name=%s, lpDllEntryPoint Fail\n", szFileName);
// 		// AfxMessageBox(szErrorMsg);
// 		iReturn = 4;
// 		goto CleanupLibrary;
// 	}
// 
// CleanupLibrary:
// 	FreeLibrary(hLib);
// 
// CleanupOle:
// 	OleUninitialize();
// 
// 	::SetCurrentDirectory(aCurPath);
// 
// 	return iReturn;
// }

int CSystemUtilManager::DLLRegister(LPCSTR szFileName)
{
	int			iReturn=0;
	CString		szErrorMsg;
	
	char aCurPath[MAX_PATH]={0,};
	::GetCurrentDirectory(MAX_PATH, aCurPath);
	{
		CString szPath(szFileName);
		int nPos = szPath.ReverseFind('\\');
		if(nPos<0) nPos = szPath.ReverseFind('/');
		if(nPos>0)
		{
			szPath = szPath.Left(nPos);
			::SetCurrentDirectory(szPath);
		}
	}

	char szCommand[MAX_PATH];
	::GetWindowsDirectory(szCommand, MAX_PATH);
	strcat(szCommand, "\\System32\\regsvr32.exe");

	CString szParam;
	szParam.Format(" /s \"%s\"", szFileName);
	strcat(szCommand, szParam);	

	CheckExeRun( szCommand, FALSE );

// 	SHELLEXECUTEINFO shExlf = { 0 };
// 	shExlf.cbSize = sizeof(SHELLEXECUTEINFO);
// 	shExlf.fMask = 0;
// 	shExlf.lpVerb = _T("open");
// 	shExlf.lpFile = (LPSTR)(LPCTSTR)szCommand;
// 	shExlf.nShow = SW_HIDE;
// 	shExlf.lpParameters = (LPSTR)(LPCTSTR)szParam;
// 	
// 	ShellExecuteEx(&shExlf);
// 	::SetCurrentDirectory(aCurPath);
	
	return iReturn;
}

// BOOL CSystemUtilManager::IsRegisteredEx(LPCSTR szProgID, CLSID &clsid, LPCSTR szOcxName)
// {
// 	if(!IsRegistered(szProgID, clsid)) return FALSE;
// 
// 	// 등록은 되어있는데, 파일이 없으면 FALSE를 리턴한다.
// 	CString strOcxFile;
// 	CProgIDInfo::ComServerFromProgID( szProgID, strOcxFile);
// 
// 	CString szRegPath;
// 	szRegPath = strOcxFile.Left( strOcxFile.ReverseFind( '\\') + 1);
// 	szRegPath += szOcxName;
// 
// 	{
// 		CFileFind filefind;
// 		if(!filefind.FindFile(szRegPath)) return FALSE;
// 	}
// 
// 	return TRUE;
// }

BOOL CSystemUtilManager::IsRegisteredEx(LPCSTR szProgID, CLSID &clsid, LPCSTR szOcxName, LPCSTR szFullPath)
{
	if(!IsRegistered(szProgID, clsid)) return FALSE;
	
	// 등록은 되어있는데, 파일이 없으면 FALSE를 리턴한다.
	CString strOcxFile;
	CProgIDInfo::ComServerFromProgID( szProgID, strOcxFile);
	
	CString szRegPath;
	szRegPath = strOcxFile.Left( strOcxFile.ReverseFind( '\\') + 1);
	szRegPath += szOcxName;
	
	if(szFullPath !=NULL)
	{
		if(szRegPath.CompareNoCase(szFullPath)==0)
		{
			CFileFind filefind;
			if(!filefind.FindFile(szRegPath))
				return FALSE;
			
			return TRUE;
		}
	}
	else
	{
		CFileFind filefind;
		if(!filefind.FindFile(szRegPath))
			return FALSE;
		return TRUE;
	}
	
	return FALSE;
}

BOOL CSystemUtilManager::CheckExeRun( const CString &szCmdLine, BOOL _bShow )
{
	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFO startupInfo                = {0};
	startupInfo.cb                         = sizeof(startupInfo);
	if( _bShow == FALSE) startupInfo.wShowWindow = SW_HIDE;
	
	// Create the process
	BOOL result = CreateProcess( NULL, (LPTSTR)(LPCTSTR)szCmdLine, 
		NULL, NULL, FALSE, 
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 
		NULL, NULL, &startupInfo, &processInformation);
	
	if (!result)
	{
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
		
		// Display the error
		CString strError = (LPTSTR) lpMsgBuf;
		TRACE(_T("::executeCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), szCmdLine, strError);
		
		// Free resources created by the system
		LocalFree(lpMsgBuf);
		
		// We failed.
		return FALSE;
	}
	else
	{
		DWORD  exitCode;
		
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject( processInformation.hProcess, INFINITE );
		
		// Get the exit code.
		result = GetExitCodeProcess(processInformation.hProcess, &exitCode);
		
		// Close the handles.
		CloseHandle( processInformation.hProcess );
		CloseHandle( processInformation.hThread );
		
		if (!result)
		{
			// Could not get exit code.
			TRACE(_T("Executed command but couldn't get exit code.\nCommand=%s\n"), szCmdLine);
			return FALSE;
		}
		// We succeeded.
		return TRUE;
	}
}