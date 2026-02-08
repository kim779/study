// ConditionSearchVersionManager.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "ConditionSearchVersionManager.h"
#include "../../ForSite/FS_Env.h"
#include "../DR_Include/unzip.h"
#include "AxFileManager.h"//20120112 이문수 동부압축툴 적용 >>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FOLDER_FIND				"\\icss\\finddata"
#define FILE_DATAVERSION_LOCAL "LocalVersionDataFile.cfg"
#define FILE_DATAVERSION		"SvrVersionDataFile.cfg"
#define FILE_FINDDATA			"finddata"
#define FILE_LANGGUAGEDATA		"data"
#define FILE_EXP				"exp"
#define FILE_MAP				"map"
#define FILE_OTHER				"other"
#define FILE_PATTERNDATA		"patterndata"
#define FILE_SYS				"sys"
#define FILE_EXTENSION			".zip"

#define WMU_START_UNZIP			WM_USER + 3000
#define WMU_END_UNZIP			WM_USER + 3001

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
// CConditionSearchVersionManagerApp

BEGIN_MESSAGE_MAP(CConditionSearchVersionManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CConditionSearchVersionManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionSearchVersionManagerApp construction

CConditionSearchVersionManagerApp::CConditionSearchVersionManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CConditionSearchVersionManagerApp object

CConditionSearchVersionManagerApp theApp;

void _DLL_Open(CString strFormCode, CWnd* pTargetWnd, CFont* pFontDef)
{
	theApp.LibOpen(strFormCode, pTargetWnd, pFontDef);
}

void _DLL_Close(CWnd* pTargetWnd)
{
	theApp.LibClose(pTargetWnd);
}

HINSTANCE g_hInstance;
BOOL CConditionSearchVersionManagerApp::InitInstance()
{
	g_hInstance = theApp.m_hInstance;
	m_bUnZipLanguageData = FALSE;
	m_bUnZipExp = FALSE;
	m_bUnZipMap = FALSE; 
	m_bUnZipOther = FALSE;
	m_bUnZipPatternData = FALSE;
	m_bUnZipSys = FALSE;
	
	m_bLoadLocalVerDataFile = LoadLocalVersionDataFile();
	m_bLoadSvrVerDataFile = LoadSvrVersionDataFile();
	
	m_pCommMsg = NULL;

	m_nVerMngMode = 0;

	return CWinApp::InitInstance();
}

void CConditionSearchVersionManagerApp::LibOpen(CString strFormCode, CWnd* pTargetWnd, CFont* pFontDef)
{
	m_listLoadWnd.AddTail(pTargetWnd);
	m_strLoadFormCode = strFormCode;

	CheckDataFileVersion(pTargetWnd, pFontDef);
}

void CConditionSearchVersionManagerApp::LibClose(CWnd* pTargetWnd)
{
	POSITION pos;
	for(int nIndex = 0; nIndex < m_listLoadWnd.GetCount(); nIndex++)
	{
		pos = m_listLoadWnd.FindIndex(nIndex);
		CWnd* pWnd = (CWnd*)m_listLoadWnd.GetAt(pos);

		if(pWnd == pTargetWnd)
		{
			m_listLoadWnd.RemoveAt(pos);
			break;
		}
			
	}
}

int CConditionSearchVersionManagerApp::ExitInstance()
{
	if(m_listLoadWnd.GetCount() > 0)
		return FALSE;
	
	return CWinApp::ExitInstance();
}

BOOL CConditionSearchVersionManagerApp::LoadSvrVersionDataFile()
{
	CString strSvrVerDataFile;
	GetDir_DrfnRoot(strSvrVerDataFile);
	strSvrVerDataFile.Format("%s%s\\%s", strSvrVerDataFile, FOLDER_FIND, FILE_DATAVERSION);
	
	CFileFind ff;
	if(!ff.FindFile(strSvrVerDataFile))
	{
		CString strMsg;
		strMsg.Format("%s 파일이 없습니다. 고객센터로 문의하십시요.", FILE_DATAVERSION);
		/*SetMessageBox(strMsg);*/
		return FALSE;
	}
	
	char szData[32];
	GetPrivateProfileString("DATA_FILE_VER", "LANGUAGEDATA", "0", szData, sizeof(szData), strSvrVerDataFile);
	m_strSvrLanguageDataVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "EXP", "0", szData, sizeof(szData), strSvrVerDataFile);
	m_strSvrExpVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "MAP", "0", szData, sizeof(szData), strSvrVerDataFile);
	m_strSvrMapVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "OTHER", "0", szData, sizeof(szData), strSvrVerDataFile);
	m_strSvrOtherVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "PATTERNDATA", "0", szData, sizeof(szData), strSvrVerDataFile);
	m_strSvrPatternDataVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "SYS", "0", szData, sizeof(szData), strSvrVerDataFile);
	m_strSvrSysVer.Format("%s", szData);
	
	return TRUE;
}

BOOL CConditionSearchVersionManagerApp::LoadLocalVersionDataFile()
{
	CString strLocalVersionDataFile;
	GetDir_DrfnRoot(strLocalVersionDataFile);
	strLocalVersionDataFile.Format("%s%s\\%s", strLocalVersionDataFile, FOLDER_FIND, FILE_DATAVERSION_LOCAL);
	
	m_stLocalDataFilePath = strLocalVersionDataFile;
	
	CFileFind ff;
	if(!ff.FindFile(strLocalVersionDataFile))
		return FALSE;
	
	char szData[32];
	
	GetPrivateProfileString("DATA_FILE_VER", "LANGUAGEDATA", "0", szData, sizeof(szData), strLocalVersionDataFile);
	m_strLocalLanguageDataVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "EXP", "0", szData, sizeof(szData), strLocalVersionDataFile);
	m_strLocalExpVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "MAP", "0", szData, sizeof(szData), strLocalVersionDataFile);
	m_strLocalMapVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "OTHER", "0", szData, sizeof(szData), strLocalVersionDataFile);
	m_strLocalOtherVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "PATTERNDATA", "0", szData, sizeof(szData), strLocalVersionDataFile);
	m_strLocalPatternDataVer.Format("%s", szData);
	
	GetPrivateProfileString("DATA_FILE_VER", "SYS", "0", szData, sizeof(szData), strLocalVersionDataFile);
	m_strLocalSysVer.Format("%s", szData);
	
	return TRUE;
}

BOOL CConditionSearchVersionManagerApp::CheckDataFileVersion(CWnd* pTargetWnd, CFont* pFontDef)
{
	if(m_nVerMngMode == 1 && !m_bLoadSvrVerDataFile)
		return FALSE;
	
	CString strFindDataPath;
	GetDir_DrfnRoot(strFindDataPath);
	strFindDataPath.Format("%s%s", strFindDataPath, FOLDER_FIND);
	
	CString strDataFolderPath;
	CString strFileName;
	CString strDownFilePath;
	CString strMoveFilePath;
	CFileFind ff;
	
	m_CriticalSection.Lock();

	CStringArray saUnZipTargetDataFolderPath;
	CStringArray saUnZipTargetFileName;
	
	// find//data
	if(!m_bUnZipLanguageData 
		&& (m_strLoadFormCode == "HD840000" || m_strLoadFormCode == "HD840100" ||
		m_strLoadFormCode == "HD840300" || m_strLoadFormCode == "HD840400" || 
		m_strLoadFormCode == "HD840200" || m_strLoadFormCode == "HD841500" || m_strLoadFormCode == "HD840600"))
	{
		strDataFolderPath.Format("%s\\%s", strFindDataPath, FILE_LANGGUAGEDATA);
		strFileName.Format("%s%s", FILE_LANGGUAGEDATA, FILE_EXTENSION);

		m_bUnZipLanguageData = CheckVersion(strFindDataPath, strDataFolderPath, strFileName, 
											&saUnZipTargetDataFolderPath, &saUnZipTargetFileName, 
											m_strSvrLanguageDataVer, m_strLocalLanguageDataVer);
	}
	
	// find//exp
	if(!m_bUnZipExp
		&& (m_strLoadFormCode == "HD840000" || m_strLoadFormCode == "HD840100" ||
		m_strLoadFormCode == "HD840300" || m_strLoadFormCode == "HD8404" || 
		m_strLoadFormCode == "HD841500" || m_strLoadFormCode == "HD840700"))
	{
		strDataFolderPath.Format("%s\\%s", strFindDataPath, FILE_EXP);
		strFileName.Format("%s%s", FILE_EXP, FILE_EXTENSION);
		
		m_bUnZipExp = CheckVersion(strFindDataPath, strDataFolderPath, strFileName, 
								   &saUnZipTargetDataFolderPath, &saUnZipTargetFileName, 
								   m_strSvrExpVer, m_strLocalExpVer);
	}
	
	// find//map
	if(!m_bUnZipMap 
		&& (m_strLoadFormCode == "HD840000" || m_strLoadFormCode == "HD841600" || 
		m_strLoadFormCode == "HD840500" || m_strLoadFormCode == "HD840600"))
	{
		strDataFolderPath.Format("%s\\%s", strFindDataPath, FILE_MAP);
		strFileName.Format("%s%s", FILE_MAP, FILE_EXTENSION);

		m_bUnZipMap = CheckVersion(strFindDataPath, strDataFolderPath, strFileName, 
								   &saUnZipTargetDataFolderPath, &saUnZipTargetFileName, 
								   m_strSvrMapVer, m_strLocalMapVer);
	}
	
	// find//other
	if(!m_bUnZipOther
		&& (m_strLoadFormCode == "HD840100" || m_strLoadFormCode == "HD840300" 
		|| m_strLoadFormCode == "HD8404" || m_strLoadFormCode == "HD840200" || 
		m_strLoadFormCode == "HD841600" || m_strLoadFormCode == "HD841700" || m_strLoadFormCode == "HD840500"))
	{
		strDataFolderPath.Format("%s\\%s", strFindDataPath, FILE_OTHER);
		strFileName.Format("%s%s", FILE_OTHER, FILE_EXTENSION);
		
		m_bUnZipOther = CheckVersion(strFindDataPath, strDataFolderPath, strFileName, 
									 &saUnZipTargetDataFolderPath, &saUnZipTargetFileName, 
									 m_strSvrOtherVer, m_strLocalOtherVer);
	}
	
	// find//patterndata
	if(!m_bUnZipPatternData 
		&& (m_strLoadFormCode == "HD841000" || m_strLoadFormCode == "HD841100" || m_strLoadFormCode == "HD841200"))
	{
		strDataFolderPath.Format("%s\\%s", strFindDataPath, FILE_PATTERNDATA);
		strFileName.Format("%s%s", FILE_PATTERNDATA, FILE_EXTENSION);
		
		m_bUnZipPatternData = CheckVersion(strFindDataPath, strDataFolderPath, strFileName, 
							       	       &saUnZipTargetDataFolderPath, &saUnZipTargetFileName, 
									       m_strSvrPatternDataVer, m_strLocalPatternDataVer);
	}
	
	// find//sys
	if(!m_bUnZipSys 
		&& (m_strLoadFormCode == "HD840000" || m_strLoadFormCode == "HD840100" ||
		m_strLoadFormCode == "HD840300" || m_strLoadFormCode == "HD8404" 
		|| m_strLoadFormCode == "HD840200" || m_strLoadFormCode == "HD841700"))
	{
		strDataFolderPath.Format("%s\\%s", strFindDataPath, FILE_SYS);
		strFileName.Format("%s%s", FILE_SYS, FILE_EXTENSION);
			
		m_bUnZipSys = CheckVersion(strFindDataPath, strDataFolderPath, strFileName, 
								   &saUnZipTargetDataFolderPath, &saUnZipTargetFileName, 
								   m_strSvrSysVer, m_strLocalSysVer);
	}
	
	if(saUnZipTargetDataFolderPath.GetSize() != saUnZipTargetFileName.GetSize())	
		return FALSE;
	
	if(saUnZipTargetDataFolderPath.GetSize() > 0)
	{
		
// 		HINSTANCE hInstnsce = theApp.m_hInstance;
// 		
// 		HINSTANCE hInstSave = AfxGetResourceHandle();
// 		AfxSetResourceHandle(hInstnsce);
		
		m_pCommMsg = new CRequestMsg;								// 통신조회 메세지바 생성
		m_pCommMsg->Create(CRequestMsg::IDD,NULL);
	
		CString	strTemp = "조건검색 버전처리중입니다. 잠시만 기다려 주십시오.";
		m_pCommMsg->ShowMessage((LPCTSTR)strTemp, NULL, pFontDef);

		for(int nIndex = 0; nIndex < saUnZipTargetDataFolderPath.GetSize(); nIndex++)
		{
			UnZipDataFile(saUnZipTargetDataFolderPath.GetAt(nIndex), saUnZipTargetFileName.GetAt(nIndex));
			
			//20120112 이문수 동부압축툴 적용 주석처리 >>
			//CString strFilePath;
			//strFilePath.Format("%s\\%s", saUnZipTargetDataFolderPath.GetAt(nIndex), saUnZipTargetFileName.GetAt(nIndex));			
			//DeleteFile(strFilePath);
			//20120112 이문수 동부압축툴 적용 <<
		}
		m_pCommMsg->HideMessage();

		if (m_pCommMsg)
		{
			m_pCommMsg->DestroyWindow();
			delete m_pCommMsg;
			m_pCommMsg = NULL;
		}

		/*AfxSetResourceHandle(hInstSave);*/
	}
	
	// test
// 	m_bUnZipLanguageData = FALSE;
// 	m_bUnZipExp = FALSE;
// 	m_bUnZipMap = FALSE;
// 	m_bUnZipOther = FALSE;
// 	m_bUnZipPatternData = FALSE;
// 	m_bUnZipSys = FALSE;

	SaveLocalDataFileVersion();

	m_CriticalSection.Unlock();

	return TRUE;//20120112 이문수 동부압축툴 적용 >>
}

BOOL CConditionSearchVersionManagerApp::SaveLocalDataFileVersion()
{
	if(m_nVerMngMode == 1 && !m_bLoadSvrVerDataFile)
		return FALSE;
	
	BOOL bResult = FALSE;
	
	TRY 
	{
		if(m_bUnZipLanguageData)
			WritePrivateProfileString("DATA_FILE_VER", "LANGUAGEDATA", m_strSvrLanguageDataVer, m_stLocalDataFilePath);
		if(m_bUnZipExp)
			WritePrivateProfileString("DATA_FILE_VER", "EXP", m_strSvrExpVer, m_stLocalDataFilePath);
		if(m_bUnZipMap)
			WritePrivateProfileString("DATA_FILE_VER", "MAP", m_strSvrMapVer, m_stLocalDataFilePath);
		if(m_bUnZipOther)
			WritePrivateProfileString("DATA_FILE_VER", "OTHER", m_strSvrOtherVer, m_stLocalDataFilePath);
		if(m_bUnZipPatternData)
			WritePrivateProfileString("DATA_FILE_VER", "PATTERNDATA", m_strSvrPatternDataVer, m_stLocalDataFilePath);
		if(m_bUnZipSys)
			WritePrivateProfileString("DATA_FILE_VER", "SYS", m_strSvrSysVer, m_stLocalDataFilePath);
		
		bResult = TRUE;
	}
	CATCH (CFileException, pEx)
	{
		bResult = FALSE;		
	}
	END_CATCH
		
		return bResult;
}

BOOL CConditionSearchVersionManagerApp::UnZipDataFile(CString strFilePath, CString strFileName)
{
	//20120112 이문수 동부압축툴 적용 >>
	CString strTargetPath	= strFilePath + "\\";
	CString strSrcPath		= strFilePath + "\\" + strFileName;

	CAxFileManager axManager;
	if(!axManager.FileCheck(strSrcPath) )
	{
		return FALSE;
	}

	int nResult = axManager.zipCopy((LPSTR)(LPCTSTR)strSrcPath, (LPSTR)(LPCTSTR)strTargetPath);
	::fflush(NULL);

	if(nResult == 0)	return TRUE;
	else				return FALSE;	

//	BOOL bResult = TRUE;
//	
//	char szBuf[256];
//	memset(szBuf, 0x00, sizeof(szBuf));
//	
//	GetCurrentDirectory(sizeof(szBuf), szBuf);
//	SetCurrentDirectory((LPCTSTR)strFilePath);
//	
//	strFilePath.Format("%s\\%s", strFilePath, strFileName);
//	
//	CFileFind ff;
//	if(ff.FindFile(strFilePath))
//	{
//		HZIP hz = OpenZip(strFilePath, 0);
//		
//		if(hz == NULL)
//			return FALSE;
//		
//		ZIPENTRY ze;
//		GetZipItem(hz, -1, &ze);
//		
//		int nNumItems = ze.index;
//		
//		for(int zi = 0; zi < nNumItems; zi++)
//		{
//			GetZipItem(hz, zi, &ze);
//			if(UnzipItem(hz, zi, ze.name) != ZR_OK)
//			{
//				bResult = FALSE;
//			}
//		}
//		
//		CloseZip(hz);
//	}
//	else
//	{
//		CString strMsg;
//		strMsg.Format("%s 파일이 없습니다. 고객센터로 문의하십시오.", strFileName);
//		/*SetMessageBox(strMsg);*/
//	}
//	
//	SetCurrentDirectory(szBuf);
//	
//	return bResult;
	//20120112 이문수 동부압축툴 적용 <<
}

BOOL CConditionSearchVersionManagerApp::CheckVersion(CString strZipFileDir, CString strMoveTargetDir, CString strZipFileName,
													 CStringArray* psaPath, CStringArray* psaFileName,
													 CString strSvrVer, CString strLocalVer)
{
	CFileFind ff;

	CString strZipFilePath;
	strZipFilePath.Format("%s\\%s", strZipFileDir, strZipFileName);
	
	CString strMoveTargetPath;
	strMoveTargetPath.Format("%s\\%s", strMoveTargetDir, strZipFileName);
	
	if(m_nVerMngMode == 0)
	{
		if(!ff.FindFile(strZipFilePath))
			return FALSE;

		if(!ff.FindFile(strMoveTargetDir))
			CreateDirectory(strMoveTargetDir, NULL);

		// test
		MoveFile(strZipFilePath, strMoveTargetPath);
		/*CopyFile(strZipFilePath, strMoveTargetPath, NULL);*/
	}
	else if(m_nVerMngMode == 1)
	{
		if(strSvrVer == strLocalVer)
			return FALSE;

		if(!ff.FindFile(strMoveTargetDir))
			CreateDirectory(strMoveTargetDir, NULL);

		CopyFile(strZipFilePath, strMoveTargetPath, NULL);
	}

	psaPath->Add(strMoveTargetDir);
	psaFileName->Add(strZipFileName);

	return TRUE;
}