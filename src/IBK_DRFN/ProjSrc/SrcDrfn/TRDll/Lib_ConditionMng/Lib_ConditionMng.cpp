// Lib_ConditionMng.cpp : Defines the initialization routines for the DLL.
//

/********************************************************************/
/*																	                                */
/*										조건 검색 기능 지원 Lib												*/
/*																	                                */
/*										2005. 12.  		  Doori		                      */
/*                    작성자 : 우동우																*/
/********************************************************************/


#include "stdafx.h"
#include "Lib_ConditionMng.h"
#include "LogicCalculate.h"

#include "../../ForSite/FS_Env.h"

#include "Winsock2.h"
#include <math.h>

//KHD 
#include "../../inc/IGateMng.h"
#include "../../inc/IMainInfoMng.h"
//END 

#define SZ_CODE										8
#define SZ_NAME										21
#define SZ_CONDILIST_NAME					100
#define SZ_CONDILIST_PATH					256

//# 고정 필드 : 현재가 / 전일대비 / 등락율 / 거래량
long		FIX_FIELD_ID[] = { 9001, 9002, 9003, 9008 };
CString	FIX_FIELD_NAME[] = { "현재가", "전일대비", "등락률", "거래량" };
long		FIX_FIELD_UNIT[] = { 1, 100, 100, 1};

#define TYPE_CONDITIONDATA_SYS		0
#define TYPE_CONDITIONDATA_USER		1
#define TYPE_CONDITIONDATA_OTHER	2
#define TYPE_CONDITIONDATA_TEMP		3
#define TYPE_CONDITIONDATA_THEME	4
#define TYPE_CONDITIONDATA_REAL_OTHER	5		// 2009.10.26 -hidden- 조건검색 실시간용

#define PROGRAM_DEBUG_TITLE			"\n# [Lib_ConditionMng] - "

#define TAB_TITLE_USERCONDI       "사용자검색"

#define WMU_GET_MODULE_MANINWND		WM_USER + 148

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
// CLib_ConditionMngApp

BEGIN_MESSAGE_MAP(CLib_ConditionMngApp, CWinApp)
	//{{AFX_MSG_MAP(CLib_ConditionMngApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLib_ConditionMngApp construction

CLib_ConditionMngApp::CLib_ConditionMngApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLib_ConditionMngApp object

CLib_ConditionMngApp theApp;

//extern CString KSLink_GetFileHeader();	-hidden-
BOOL CLib_ConditionMngApp::InitInstance() 
{
	m_pLogicCalculate = NULL;

	m_strLoadFilePath = _T("");

	/*
	m_bMoveSysLoadedXMLFile = VARIANT_FALSE;
	m_bFixSysLoadedXMLFile = VARIANT_FALSE;
	*/

	m_bLoadFile_User = FALSE;
	m_bLoadFile_Sys = FALSE;
	SetDeleteFolderList(&m_listUserInfoFolder);
	SetDeleteFolderList(&m_listSysInfoFolder);

	m_listConditionGroup.RemoveAll();
	m_listOneElement.RemoveAll();
	m_listGroupElement.RemoveAll();

	CString strUserPath, strAppPath;
	strAppPath = GetAppFileRoot();

	CFileFind finder;
	if(0 == finder.FindFile(strAppPath + "\\*.*"))
		CreateDirectory(strAppPath, 0);

	
	/*strUserPath = CDirMgr::GetUserDir();*/
	
//KHD : 빠른종목 검색 로딩  
	IGateManager* pGateMng = NULL;
	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(21);
		m_strUserFolder = pManager->GetEncUserDir();

		::SetEnvironmentVariable(KEY_USERDIRECTORY, m_strUserFolder);
		
		GetLoad_ConditionInfo();
		GetLoad_FieldInfo();
	}
//END 	

	return CWinApp::InitInstance();
}

int CLib_ConditionMngApp::ExitInstance() 
{
	SetInitVal();

	if (m_listUserInfoFolder.GetCount() > 0)
		SetDeleteFolderList(&m_listUserInfoFolder);

	if (m_listSysInfoFolder.GetCount() > 0)
		SetDeleteFolderList(&m_listSysInfoFolder);

	SetClear_FieldInfo();


	CoUninitialize();

	TRACE("\n# CLib_ConditionMngApp::ExitInstance");

	return CWinApp::ExitInstance();
}

BOOL _DLL_Open(CStringArray *psaMSTCode) 
{
	BOOL bResult = FALSE;

	if (theApp.m_pLogicCalculate == NULL)
	{
		if (psaMSTCode == NULL)
			return bResult;

		if (theApp.m_pLogicCalculate == NULL)
			theApp.m_pLogicCalculate = new CLogicCalculate;

		if (theApp.m_pLogicCalculate->SetInit(psaMSTCode) == FALSE)
		{
			delete theApp.m_pLogicCalculate;
			theApp.m_pLogicCalculate = NULL;

			return bResult;
		}
		else
		{
			if (psaMSTCode == NULL)
				return bResult;

			theApp.m_saMSTCode.Copy(*psaMSTCode);
			bResult = TRUE;
		}
	}

	return bResult;
}

void _DLL_Close() 
{
	theApp.m_saMSTCode.RemoveAll();
	if (theApp.m_pLogicCalculate != NULL)
	{
		delete theApp.m_pLogicCalculate;
		theApp.m_pLogicCalculate = NULL;
	}

	theApp.m_saPortItem.RemoveAll();
	TRACE((CString)(CString)PROGRAM_DEBUG_TITLE + "Close Lib");
}

void* _DLL_GetPortItem() 
{
	return (void*)&theApp.m_saPortItem;
}

BOOL _DLL_Open2() 
{
	BOOL bResult = FALSE;

	if (theApp.m_pLogicCalculate == NULL)
	{
		if (theApp.m_pLogicCalculate == NULL)
			theApp.m_pLogicCalculate = new CLogicCalculate;
		bResult = TRUE;
	}	
	return bResult;
}

CString CLib_ConditionMngApp::GetAppFileRoot(void)
{
	CString strPath;
	GetDir_DrfnRoot(strPath);
	return  strPath;
}

int _DLL_GetFind_ConditionPath(int nType, BOOL bGetAlert, LPSTR &lpName, LPSTR &lpPath)
{
	int nResult = -1;

	CStringArray saName, saPath, saAlert;
	CString strName, strPath;
	if (theApp.GetFind_ConditionPath(nType, bGetAlert, saName, saPath, saAlert) == FALSE)
		return nResult;

	int nDataCnt;
	nDataCnt = saName.GetSize();
	if (nDataCnt > 0)
	{
		lpName = new char[SZ_CONDILIST_NAME * nDataCnt];
		ZeroMemory((char*)lpName, SZ_CONDILIST_NAME * nDataCnt);

		lpPath = new char[SZ_CONDILIST_PATH * nDataCnt];
		ZeroMemory((char*)lpPath, SZ_CONDILIST_PATH * nDataCnt);

		int nOffsetName=0, nOffsetPath=0;
		for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
		{
			strName = saName.GetAt(nIndex);
			strPath = saPath.GetAt(nIndex);
			memcpy((char*)lpName + nOffsetName, strName.GetBuffer(0), min(SZ_CONDILIST_NAME, strName.GetLength()));
			memcpy((char*)lpPath + nOffsetPath, strPath.GetBuffer(0), min(SZ_CONDILIST_PATH, strPath.GetLength()));

			nOffsetName += SZ_CONDILIST_NAME;
			nOffsetPath += SZ_CONDILIST_PATH;
		}

		nResult = nDataCnt;
	}

	return nResult;
}

BOOL CLib_ConditionMngApp::GetFind_ConditionPath(int nType, BOOL bGetAlert, 
												 CStringArray &saName, CStringArray &saPath, CStringArray &saAlert)
{
	//# nType => 0:추천, 1:사용자, 2:Language
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder;

	if (nType == 0)
		plistInfoFolder = &m_listSysInfoFolder;
	else if (nType == 1)
		plistInfoFolder = &m_listUserInfoFolder;
	else if (nType == 2)
		return FALSE;

	if (plistInfoFolder->GetCount() == 0)
	{
		TRACE((CString)PROGRAM_DEBUG_TITLE + "구성할 Folder가 존재하지 않습니다.");
		return FALSE;
	}

	BOOL bAddFlag;
	CString strName, strPath;

	POSITION posFolder, posFile;
	XMLTreeInfoFolder		stXMLTreeInfoFolder;
	XMLTreeInfoFile			stXMLTreeInfoFile;

	int nIndex_Folder, nIndex_File, nFileCnt;
	for (nIndex_Folder = 0; nIndex_Folder <	plistInfoFolder->GetCount(); nIndex_Folder++)
	{
		posFolder = plistInfoFolder->FindIndex(nIndex_Folder);
		stXMLTreeInfoFolder = plistInfoFolder->GetAt(posFolder);

		if (stXMLTreeInfoFolder.plistXMLTreeInfoFile == NULL)
			continue;

		nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		for (nIndex_File = 0; nIndex_File <	stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nIndex_File++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nIndex_File);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

			bAddFlag = TRUE;
			if (bGetAlert == TRUE)
			{
				if (stXMLTreeInfoFile.strAlertReg == "TRUE")
					bAddFlag = TRUE;
				else if (stXMLTreeInfoFile.strAlertReg == "FALSE")
					bAddFlag = FALSE;
				else if (stXMLTreeInfoFile.strAlertReg.IsEmpty() == FALSE)
					bAddFlag = TRUE;
			}

			if (bAddFlag == TRUE)
			{
				saName.Add(stXMLTreeInfoFile.strFileName);

				strPath = (nType == 0) ? (CString)FOLDER_SYSFILE : (CString)FOLDER_USERFILE;
				strPath += "\\" + stXMLTreeInfoFile.strFolderPath + "\\" + stXMLTreeInfoFile.strFilePath;
				saPath.Add(strPath);

				saAlert.Add(stXMLTreeInfoFile.strAlertReg);
			}
		}
	}

	return TRUE;
}

BOOL _DLL_GetFind_ConditionPath_Array(int nType, BOOL bGetAlert,
									  CStringArray &saName, CStringArray &saPath)
{
	saName.RemoveAll();
	saPath.RemoveAll();

	CStringArray saAlert;

	return theApp.GetFind_ConditionPath(nType, bGetAlert, saName, saPath, saAlert);
}

BOOL _DLL_GetFind_CondiPath_Array(int nType, CStringArray &saName, CStringArray &saPath, CStringArray &saAlert)
{
	saName.RemoveAll();
	saPath.RemoveAll();
	saAlert.RemoveAll();

	return theApp.GetFind_ConditionPath(nType, FALSE, saName, saPath, saAlert);
}

BOOL _DLL_GetFind_GroupConditionPath(int nType, BOOL bGetAlert,
									 CStringArray &saGroupName, CStringArray &saPath)
{
	saGroupName.RemoveAll();
	saPath.RemoveAll();

	return theApp.GetFind_GroupConditionPath(nType, bGetAlert, saGroupName, saPath);
}

BOOL CLib_ConditionMngApp::GetFind_GroupConditionPath(int nType, BOOL bGetAlert,
													  CStringArray &saGroupName, CStringArray &saPath)
{
	//# nType => 0:추천, 1:사용자, 2:Language
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder;

	CString strFilePath, strErrorMsg;
	if (nType == 0)
		plistInfoFolder = &m_listSysInfoFolder;
	else if (nType == 1)
	{
		strFilePath = GetFilePath("USER");
		SetDeleteFolderList(&m_listUserInfoFolder);
		if (GetXMLToList(strFilePath, "USER", &m_listUserInfoFolder) == FALSE)
		{
			strErrorMsg.Format("[%s] Tree List File이 존재하지 않습니다.", strFilePath);
			SetSendMainMsg(strErrorMsg);
		}

		plistInfoFolder = &m_listUserInfoFolder;
	}
	else if (nType == 2)
	{
		strFilePath = GetFilePath("SYS");
		return FALSE;
	}

	if (plistInfoFolder->GetCount() == 0)
	{
		TRACE((CString)PROGRAM_DEBUG_TITLE + "구성할 Folder가 존재하지 않습니다.");
		return FALSE;
	}

	BOOL bAddFlag;
	CString strName, strPath;

	POSITION posFolder, posFile;
	XMLTreeInfoFolder		stXMLTreeInfoFolder;
	XMLTreeInfoFile			stXMLTreeInfoFile;

	int nIndex_Folder, nIndex_File, nFileCnt;
	for (nIndex_Folder = 0; nIndex_Folder <	plistInfoFolder->GetCount(); nIndex_Folder++)
	{
		posFolder = plistInfoFolder->FindIndex(nIndex_Folder);
		stXMLTreeInfoFolder = plistInfoFolder->GetAt(posFolder);

		if (stXMLTreeInfoFolder.plistXMLTreeInfoFile == NULL)
			continue;

		nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();

		if (nFileCnt > 0)
			saGroupName.Add(stXMLTreeInfoFolder.strName);

		for (nIndex_File = 0; nIndex_File <	stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nIndex_File++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nIndex_File);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

			bAddFlag = TRUE;
			if (bGetAlert == TRUE)
			{
				if (stXMLTreeInfoFile.strAlertReg == "TRUE")
					bAddFlag = TRUE;
				else if (stXMLTreeInfoFile.strAlertReg == "FALSE")
					bAddFlag = FALSE;
				else if (stXMLTreeInfoFile.strAlertReg.IsEmpty() == FALSE)
					bAddFlag = TRUE;
			}

			if (bAddFlag == TRUE)
			{
				strPath.Format("%s|%s|%s\\%s\\%s", stXMLTreeInfoFolder.strName, 
					stXMLTreeInfoFile.strFileName,
					(nType == 0) ? (CString)FOLDER_SYSFILE : (CString) FOLDER_USERFILE, 
					stXMLTreeInfoFile.strFolderPath, 
					stXMLTreeInfoFile.strFilePath);
				saPath.Add(strPath);
			}
		}
	}

	return TRUE;
}

BOOL _DLL_GetOther_ConditionPath(CString strFilePath, CString strWorkFolder, BOOL bGetAlert,
								 CStringArray &saGroupName, CStringArray &saPath)
{
	saGroupName.RemoveAll();
	saPath.RemoveAll();
	CStringArray saAlert;

	return theApp.GetOther_ConditionPath(strFilePath, strWorkFolder, bGetAlert, saGroupName, saPath, saAlert);
}

BOOL _DLL_GetOther_CondiPath(CString strFilePath, CString strWorkFolder, 
							 CStringArray &saGroupName, CStringArray &saPath, CStringArray &saAlert)
{
	saGroupName.RemoveAll();
	saPath.RemoveAll();
	saAlert.RemoveAll();

	return theApp.GetOther_ConditionPath(strFilePath, strWorkFolder, FALSE, saGroupName, saPath, saAlert);
}

BOOL CLib_ConditionMngApp::GetOther_ConditionPath(CString strFilePath, CString strWorkFolder, BOOL bGetAlert,
												  CStringArray &saGroupName, CStringArray &saPath, CStringArray &saAlert)
{
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> listInfoFolder;
	SetDeleteFolderList(&listInfoFolder);

	CString strErrorMsg;
	strFilePath = GetAppFileRoot() + FOLDER_OTHERFILE + "\\" + strFilePath;
	if (GetXMLToList(strFilePath, "OTHER", &listInfoFolder) == FALSE)
	{
		strErrorMsg.Format("%s File Load 실패!", strFilePath);
		SetSendMainMsg(strErrorMsg);
		return FALSE;
	}

	if (listInfoFolder.GetCount() == 0)
	{
		SetSendMainMsg((CString)PROGRAM_DEBUG_TITLE + "구성할 Folder가 존재하지 않습니다.");
		return FALSE;
	}

	BOOL bAddFlag;
	CString strName, strPath;

	POSITION posFolder, posFile;
	XMLTreeInfoFolder		stXMLTreeInfoFolder;
	XMLTreeInfoFile			stXMLTreeInfoFile;

	int nIndex_Folder, nIndex_File, nIndex_Item, nFileCnt;
	for (nIndex_Folder = 0; nIndex_Folder <	listInfoFolder.GetCount(); nIndex_Folder++)
	{
		posFolder = listInfoFolder.FindIndex(nIndex_Folder);
		stXMLTreeInfoFolder = listInfoFolder.GetAt(posFolder);

		if (stXMLTreeInfoFolder.plistXMLTreeInfoFile == NULL)
		{
			saGroupName.Add(stXMLTreeInfoFolder.strName);		//# Add Folder Name!
			continue;
		}

		nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		for (nIndex_File = 0; nIndex_File <	nFileCnt; nIndex_File++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nIndex_File);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

			bAddFlag = TRUE;
			if (bGetAlert == TRUE)
			{
				if (stXMLTreeInfoFile.strAlertReg == "TRUE")
					bAddFlag = TRUE;
				else if (stXMLTreeInfoFile.strAlertReg == "FALSE")
					bAddFlag = FALSE;
				else if (stXMLTreeInfoFile.strAlertReg.IsEmpty() == FALSE)
					bAddFlag = TRUE;
			}

			if (bAddFlag == TRUE)
			{
				if (strWorkFolder.IsEmpty() == TRUE)
					strPath.Format("%s|%s|%s\\%s\\%s|", 
					stXMLTreeInfoFolder.strName, 
					stXMLTreeInfoFile.strFileName,
					(CString)FOLDER_OTHERFILE, 
					stXMLTreeInfoFile.strFolderPath, 
					stXMLTreeInfoFile.strFilePath);
				else
					strPath.Format("%s|%s|%s\\%s\\%s\\%s|", 
					stXMLTreeInfoFolder.strName, 
					stXMLTreeInfoFile.strFileName,
					(CString)FOLDER_OTHERFILE, 
					strWorkFolder,
					stXMLTreeInfoFile.strFolderPath, 
					stXMLTreeInfoFile.strFilePath);
				saPath.Add(strPath);

				saAlert.Add(stXMLTreeInfoFile.strAlertReg);

				bAddFlag = FALSE;
				for (nIndex_Item = 0; nIndex_Item < saGroupName.GetSize(); nIndex_Item++)
				{
					if (stXMLTreeInfoFolder.strName == saGroupName.GetAt(nIndex_Item))
					{
						bAddFlag = TRUE;
						break;
					}
				}

				if (bAddFlag == FALSE)
					saGroupName.Add(stXMLTreeInfoFolder.strName);
			}
		}
	}

	SetDeleteFolderList(&listInfoFolder);

	return TRUE;
}

BOOL _DLL_GetConditionData(int nType, BOOL bIndustry, int nJustLockGroup, CStringArray &saGroupName, CStringArray &saCondiData)
{
	BOOL bJustAlert = FALSE;

	return theApp.GetConditionData(nType, bIndustry, bJustAlert, nJustLockGroup, saGroupName, saCondiData);
}

BOOL CLib_ConditionMngApp::GetConditionData(int nType, BOOL bIndustry, BOOL bJustAlert, int nJustLockGroup, 
											CStringArray &saGroupName, CStringArray &saCondiData)
{
	BOOL bResult = TRUE;

	saGroupName.RemoveAll();
	saCondiData.RemoveAll();

	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> listInfoFolder;
	if (GetFind_LoadListFile(nType, bIndustry, &listInfoFolder) == TRUE)
	{
		if (listInfoFolder.GetCount() == 0)
		{
			SetSendMainMsg((CString)PROGRAM_DEBUG_TITLE + "구성할 Folder가 존재하지 않습니다.");
			return FALSE;
		}

		CString strWorkFolder;
		if (nType == (int)TYPE_CONDITIONDATA_SYS)   //# SYS (추천)
			strWorkFolder = (CString)FOLDER_SYSFILE;
		else if (nType == (int)TYPE_CONDITIONDATA_USER)   //# USER (사용자)
		{
			CString strUserPath;
			strUserPath = m_strUserFolder;
			strWorkFolder = strUserPath + (CString)FOLDER_USERFILE;
		}
		else if (nType == (int)TYPE_CONDITIONDATA_OTHER)   //# OTHER (원클릭)
			strWorkFolder = (CString)FOLDER_DATA + (CString)FOLDER_OTHERFILE + "\\" + (CString)CONDITIONLIST_FOLDER;
		else if (nType == (int)TYPE_CONDITIONDATA_TEMP)		// \\sys\\temp.xml
			strWorkFolder = (CString)FOLDER_SYSFILE;
		else if(nType == (int)TYPE_CONDITIONDATA_THEME)
			strWorkFolder = (CString)FOLDER_DATA + (CString)FOLDER_OTHERFILE + "\\" + (CString)CONDITIONLIST_FOLDER;
		else if(nType == (int)TYPE_CONDITIONDATA_REAL_OTHER)	// 2009.10.26 -hidden- 조건 검색 실시간용
			strWorkFolder = (CString)FOLDER_SYSFILE;

		if ((bIndustry == TRUE) && (nType != (int)TYPE_CONDITIONDATA_OTHER))
			strWorkFolder += "_Industry";

		BOOL bAddFlag;
		CString strCondiData;

		POSITION posFolder, posFile;
		XMLTreeInfoFolder		stXMLTreeInfoFolder;
		XMLTreeInfoFile			stXMLTreeInfoFile;

		int nIndex_Folder, nIndex_File, nIndex_Item, nFileCnt;
		for (nIndex_Folder = 0; nIndex_Folder <	listInfoFolder.GetCount(); nIndex_Folder++)
		{
			posFolder = listInfoFolder.FindIndex(nIndex_Folder);
			stXMLTreeInfoFolder = listInfoFolder.GetAt(posFolder);

			if (nType == (int)TYPE_CONDITIONDATA_OTHER) //# CString nType => 0 : 추천, 1 : 사용자, 2 : 빠른조건검색
			{
				if (nJustLockGroup > -1)
				{
					if (stXMLTreeInfoFolder.bLock != (BOOL)nJustLockGroup)
						continue;
				}
			}

			if (stXMLTreeInfoFolder.plistXMLTreeInfoFile == NULL)
			{
				saGroupName.Add(stXMLTreeInfoFolder.strName);		//# Add Folder Name!
				continue;
			}

			nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
			for (nIndex_File = 0; nIndex_File <	nFileCnt; nIndex_File++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nIndex_File);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

				bAddFlag = TRUE;
				if (bJustAlert == TRUE)
				{
					if (stXMLTreeInfoFile.strAlertReg == "TRUE")
						bAddFlag = TRUE;
					else if (stXMLTreeInfoFile.strAlertReg == "FALSE")
						bAddFlag = FALSE;
					else if (stXMLTreeInfoFile.strAlertReg.IsEmpty() == FALSE)
						bAddFlag = TRUE;
				}

				if (bAddFlag == TRUE)
				{
					if (nType == (int)TYPE_CONDITIONDATA_TEMP)
					{
						strCondiData.Format("%s|%s|%s\\%s|%s|%s|", 
							stXMLTreeInfoFolder.strName, 
							stXMLTreeInfoFile.strFileName,
							strWorkFolder,
							stXMLTreeInfoFile.strFilePath,
							stXMLTreeInfoFile.strAlertReg,
							stXMLTreeInfoFile.strExplain);
					}
					else
					{
						strCondiData.Format("%s|%s|%s\\%s\\%s|%s|%s|", 
							stXMLTreeInfoFolder.strName, 
							stXMLTreeInfoFile.strFileName,
							strWorkFolder,
							stXMLTreeInfoFile.strFolderPath, 
							stXMLTreeInfoFile.strFilePath,
							stXMLTreeInfoFile.strAlertReg,
							stXMLTreeInfoFile.strExplain);
					}
					saCondiData.Add(strCondiData);

					bAddFlag = FALSE;
					for (nIndex_Item = 0; nIndex_Item < saGroupName.GetSize(); nIndex_Item++)
					{
						if (stXMLTreeInfoFolder.strName == saGroupName.GetAt(nIndex_Item))
						{
							bAddFlag = TRUE;
							break;
						}
					}

					if (bAddFlag == FALSE)
						saGroupName.Add(stXMLTreeInfoFolder.strName);
				}
			}
		}

		SetDeleteFolderList(&listInfoFolder);
	}
	else
		bResult = FALSE;

	return bResult;
}

BOOL CLib_ConditionMngApp::GetFind_LoadListFile(int nType, BOOL bIndustry, CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder)
{
	SetDeleteFolderList(plistInfoFolder);

	BOOL bRead_Folder = FALSE;

	CString strType, strFilePath;
	if (nType == (int)TYPE_CONDITIONDATA_SYS)   //# SYS (추천)
		strType = "SYS";
	else if (nType == (int)TYPE_CONDITIONDATA_USER)   //# USER (사용자)
	{
		strType = "USER";
		bRead_Folder = TRUE;
	}
	else if (nType == (int)TYPE_CONDITIONDATA_OTHER)   //# OTHER (원클릭)
		strType = "OTHER";
	else if( nType == (int)TYPE_CONDITIONDATA_TEMP)	
	{
		strType = "TEMP";
		bRead_Folder = TRUE;
	}
	else if(nType == (int)TYPE_CONDITIONDATA_THEME)		// 업종/테마 급변
	{
		strType = "SUDDEN";
	}
	else if(nType == (int)TYPE_CONDITIONDATA_REAL_OTHER)	// 2009.10.26 -hidden- 조건검색 실시간용	
		strType = "REAL_OTHER";

	strFilePath = GetFilePath(strType, bIndustry);

	if (bRead_Folder == TRUE)
	{
		CString strFolderPath;
		strFolderPath = GetFolderPath(strType, bIndustry);

		if(strType == "TEMP")
			GetTempCondition(strFolderPath, plistInfoFolder);
		else
			GetPathToList(strFolderPath, plistInfoFolder);
	}
	else
	{
		CString strErrorMsg;
		if (GetXMLToList(strFilePath, strType, plistInfoFolder) == FALSE)
		{
			strErrorMsg.Format("%s File Load 실패!", strFilePath);
			SetSendMainMsg(strErrorMsg);
			return FALSE;
		}
	}

	return TRUE;
}

int _DLL_GetFind_Packet(CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField, 
						BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt)
{  
	int nResult = -1;

	if (theApp.GetFind_Packet(strPath, nTrIndex, bDefultResultField) == TRUE)
	{
		if(strPath.Find("temp.xml") > -1)			// 실시간 검색은 항상 Re-load
			theApp.m_strLoadFilePath = "";			
		else
			theApp.m_strLoadFilePath = strPath;
		theApp.m_stUser_Finder.TrIndex = nTrIndex;
		if (bGetIndustry == TRUE)
			theApp.m_stUser_Finder.nTargetType = 10;
		else
			theApp.m_stUser_Finder.nTargetType = 0;

		theApp.m_stUser_Finder.Sort_flag = '0';		//# 정렬 방향(0:오름, 1:내림)
		theApp.m_stUser_Finder.Sort_Num = 100;			//# 종목코드

		//# Byte Ordering!
		User_Finder	stUser_Finder;
		memset(&stUser_Finder, 0x00, SZ_User_Finder);
		memcpy(&stUser_Finder, &theApp.m_stUser_Finder, SZ_User_Finder);

		if (bOrdering_Apply == TRUE)
			_DLL_GetFind_Packet_ByteOrdering(&stUser_Finder);

		lpPacket = new char[SZ_User_Finder];
		memcpy(lpPacket, &stUser_Finder, SZ_User_Finder);

		nOrderCnt = theApp.m_nOrderCnt;

		nResult = (int)SZ_User_Finder;
	}

	return nResult;
}

int _DLL_GetFind_GroupPacket(CStringArray *psaPath, LPSTR &lpPacket)
{
	CString strPath;
	BOOL bDefultResultField = TRUE;
	int nTrIndex = 0;
	int nIndex;
	for (nIndex = 0; nIndex < _ttoi(psaPath->GetAt(nIndex)); nIndex++)
	{
		strPath = psaPath->GetAt(nIndex);

		if (theApp.GetFind_Packet(strPath, nTrIndex, bDefultResultField) == TRUE)
		{
			//theApp.m_strLoadFilePath = strPath;
			theApp.m_stUser_Finder.TrIndex = nTrIndex;
			theApp.m_stUser_Finder.Sort_flag = '0';		//# 정렬 방향(0:오름, 1:내림)
			theApp.m_stUser_Finder.Sort_Num = 100;			//# 종목코드

			//# Byte Ordering!
			User_Finder	stUser_Finder;
			memset(&stUser_Finder, 0x00, SZ_User_Finder);
			memcpy(&stUser_Finder, &theApp.m_stUser_Finder, SZ_User_Finder);
			_DLL_GetFind_Packet_ByteOrdering(&stUser_Finder);

			if (nIndex == 0)
				lpPacket = new char[SZ_User_Finder];
			else
			{
				size_t size;
				size = _msize(lpPacket);

				lpPacket = (char*)realloc((void*)lpPacket, size + SZ_User_Finder);
			}
			//lpPacket = new char[SZ_User_Finder];
			memcpy(lpPacket, &stUser_Finder, SZ_User_Finder);
		}
	}

	return psaPath->GetSize() * (int)SZ_User_Finder;
}

BOOL CLib_ConditionMngApp::GetFind_Packet(CString strPath, int nTrIndex, BOOL bDefultResultField)
{
	BOOL bLoadData = FALSE;

	if (strPath == m_strLoadFilePath)
		bLoadData = TRUE;
	else
	{
		CString strFullFilePath;

		if(strPath[0] == '\\')
		{
			strFullFilePath = GetAppFileRoot() + strPath;
		}
		else
		{
			strFullFilePath = strPath;
		}


		bLoadData = GetLoad_ConditionData(strFullFilePath);
		if (bLoadData == TRUE)
		{
			if (SetMakePacket(bDefultResultField, m_stUser_Finder, m_nOrderCnt) == FALSE)
			{
				m_strLoadFilePath = "";      
				return FALSE;
			}
		}
		else
		{
			CString strTemp;
			strTemp.Format("%s Loading Error!", strPath);
			TRACE((CString)PROGRAM_DEBUG_TITLE + strTemp);

			m_strLoadFilePath = "";
			return FALSE;
		}
	}

	//-- log -------------------------------------------------------------------
	/*	long	TrIndex;
	long	nTargetType;					
	long    TargetIdxCnt;                	
	long    TargetResultViewCnt;           
	char    TargetExp[EXPLEN];             

	long    ComplexIdxCnt;                 
	long    ComplexResultViewCnt;          
	char    ComplexExp[EXPLEN];            

	long    FieldEditerCnt;                
	long    FieldList[REAL_IDXCNT];		

	long    ResultIdxCnt;					
	char    ResultExp[EXPLEN];				

	char    TargetBitChk;                   
	char	TargetBit[RESULT_MAX];			

	char    Sort_flag;                     
	long    Sort_Num;                      

	long    BackDay;					
	long    BackTime;					

	User_Define    stUser_Define[MAX_IDXCNT];
	*/
	/*
	CStringArray arrSrc;
	CString strTemp;
	strTemp.Format("%d, %d, %d, %d", 
	m_stUser_Finder.TrIndex, m_stUser_Finder.nTargetType, m_stUser_Finder.TargetIdxCnt, m_stUser_Finder.TargetResultViewCnt);
	arrSrc.Add(strTemp);

	strTemp.Format("%s, %d, %d, %s, %d",
	m_stUser_Finder.TargetExp, m_stUser_Finder.ComplexIdxCnt, m_stUser_Finder.ComplexResultViewCnt, 
	m_stUser_Finder.ComplexExp, m_stUser_Finder.FieldEditerCnt);
	arrSrc.Add(strTemp);

	CString strTemp1;
	int i = 0;
	for(i = 0; i < REAL_IDXCNT; ++i)
	{
	strTemp1.Format("%d", m_stUser_Finder.FieldList[i]);
	strTemp += strTemp1;
	}
	strTemp1.Format("%d", m_stUser_Finder.ResultIdxCnt);
	strTemp += strTemp1;
	arrSrc.Add(strTemp);

	strTemp.Format("%s", m_stUser_Finder.ResultExp);
	arrSrc.Add(strTemp);

	strTemp.Format("%c, %s", m_stUser_Finder.TargetBitChk, m_stUser_Finder.TargetBit);
	arrSrc.Add(strTemp);

	strTemp.Format("%c, %d, %d, %d", m_stUser_Finder.Sort_flag, m_stUser_Finder.Sort_Num, m_stUser_Finder.BackDay, m_stUser_Finder.BackTime);
	arrSrc.Add(strTemp);
	*/
	/*	long    IndexNo; 
	long    GroupNo; 
	long    GroupIdxCnt;
	long    TargetBase;	
	long    Type; 		
	long    Type1; 		
	long    Type2; 		
	long    STerm;		
	long    ETerm;		
	long    Rank;		
	long    Trend;		
	long	SignalCnt1;	
	long	SignalCnt2;	
	long    ResultViewNum;
	char    IndexType;	
	char    SearchType;	
	char    Operator;	
	char    Reverse;	
	char    IdxContiGubun;
	char    Element;	
	char	Code[6];
	double  Param[MAX_PARAM];
	double  Condition[MAX_CONDI];

	for(i = 0; i < MAX_IDXCNT; ++i)
	{
	strTemp.Format("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %c, %c, %c, %c, %c, %c, %s, %f, %f, %f, %f, %f, %f, %f, %f, %f",
	m_stUser_Finder.stUser_Define[i].IndexNo,
	m_stUser_Finder.stUser_Define[i].GroupNo,
	m_stUser_Finder.stUser_Define[i].GroupIdxCnt,
	m_stUser_Finder.stUser_Define[i].TargetBase,
	m_stUser_Finder.stUser_Define[i].Type,
	m_stUser_Finder.stUser_Define[i].Type1,
	m_stUser_Finder.stUser_Define[i].Type2,
	m_stUser_Finder.stUser_Define[i].STerm,
	m_stUser_Finder.stUser_Define[i].ETerm,
	m_stUser_Finder.stUser_Define[i].Rank,
	m_stUser_Finder.stUser_Define[i].Trend,
	m_stUser_Finder.stUser_Define[i].SignalCnt1,
	m_stUser_Finder.stUser_Define[i].SignalCnt2,
	m_stUser_Finder.stUser_Define[i].ResultViewNum,
	m_stUser_Finder.stUser_Define[i].IndexType,
	m_stUser_Finder.stUser_Define[i].SearchType,
	m_stUser_Finder.stUser_Define[i].Operator,
	m_stUser_Finder.stUser_Define[i].Reverse,
	m_stUser_Finder.stUser_Define[i].IdxContiGubun,
	m_stUser_Finder.stUser_Define[i].Element,
	m_stUser_Finder.stUser_Define[i].Code,
	m_stUser_Finder.stUser_Define[i].Param[0],
	m_stUser_Finder.stUser_Define[i].Param[1],
	m_stUser_Finder.stUser_Define[i].Param[2],
	m_stUser_Finder.stUser_Define[i].Condition[0],
	m_stUser_Finder.stUser_Define[i].Condition[1],
	m_stUser_Finder.stUser_Define[i].Condition[2],
	m_stUser_Finder.stUser_Define[i].Condition[3],
	m_stUser_Finder.stUser_Define[i].Condition[4],
	m_stUser_Finder.stUser_Define[i].Condition[5]);

	arrSrc.Add(strTemp);
	}

	CStdioFile destFile("d:\\SetMakePacket.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeText);

	for(i = 0; i < arrSrc.GetSize(); ++i)
	{
	destFile.WriteString(arrSrc.GetAt(i) + "\r\n");
	}
	destFile.Close();
	*/

	//------------------------------------------------------------------
	return bLoadData;
}

void _DLL_GetFind_Packet_ByteOrdering(User_Finder *pstUserFinder)
{
	//@유진에서는 ByteOrdering 사용하지 않음.	
	return;

	int nIndex;

	pstUserFinder->TrIndex = ntohl(pstUserFinder->TrIndex);
	pstUserFinder->nTargetType = ntohl(pstUserFinder->nTargetType);
	pstUserFinder->TargetIdxCnt = ntohl(pstUserFinder->TargetIdxCnt);
	pstUserFinder->TargetResultViewCnt = ntohl(pstUserFinder->TargetResultViewCnt);

	pstUserFinder->ComplexIdxCnt = ntohl(pstUserFinder->ComplexIdxCnt);
	pstUserFinder->ComplexResultViewCnt = ntohl(pstUserFinder->ComplexResultViewCnt);

	pstUserFinder->FieldEditerCnt = ntohl(pstUserFinder->FieldEditerCnt);
	for (nIndex = 0; nIndex < (int)MAX_IDXCNT; nIndex++)
		pstUserFinder->FieldList[nIndex] = ntohl(pstUserFinder->FieldList[nIndex]);

	pstUserFinder->ResultIdxCnt = ntohl(pstUserFinder->ResultIdxCnt);

	pstUserFinder->Sort_Num = ntohl(pstUserFinder->Sort_Num);

	pstUserFinder->BackDay = ntohl(pstUserFinder->BackDay);
	pstUserFinder->BackTime = ntohl(pstUserFinder->BackTime);

	for (nIndex = 0; nIndex < (int)MAX_IDXCNT; nIndex++)
	{
		pstUserFinder->stUser_Define[nIndex].IndexNo = ntohl(pstUserFinder->stUser_Define[nIndex].IndexNo);
		pstUserFinder->stUser_Define[nIndex].GroupNo = ntohl(pstUserFinder->stUser_Define[nIndex].GroupNo);
		pstUserFinder->stUser_Define[nIndex].GroupIdxCnt = ntohl(pstUserFinder->stUser_Define[nIndex].GroupIdxCnt);
		pstUserFinder->stUser_Define[nIndex].TargetBase = ntohl(pstUserFinder->stUser_Define[nIndex].TargetBase);
		pstUserFinder->stUser_Define[nIndex].Type = ntohl(pstUserFinder->stUser_Define[nIndex].Type);
		pstUserFinder->stUser_Define[nIndex].Type1 = ntohl(pstUserFinder->stUser_Define[nIndex].Type1);
		pstUserFinder->stUser_Define[nIndex].Type2 = ntohl(pstUserFinder->stUser_Define[nIndex].Type2);
		pstUserFinder->stUser_Define[nIndex].STerm = ntohl(pstUserFinder->stUser_Define[nIndex].STerm);
		pstUserFinder->stUser_Define[nIndex].ETerm = ntohl(pstUserFinder->stUser_Define[nIndex].ETerm);
		pstUserFinder->stUser_Define[nIndex].Rank = ntohl(pstUserFinder->stUser_Define[nIndex].Rank);
		pstUserFinder->stUser_Define[nIndex].Trend = ntohl(pstUserFinder->stUser_Define[nIndex].Trend);
		pstUserFinder->stUser_Define[nIndex].SignalCnt1 = ntohl(pstUserFinder->stUser_Define[nIndex].SignalCnt1);
		pstUserFinder->stUser_Define[nIndex].SignalCnt2 = ntohl(pstUserFinder->stUser_Define[nIndex].SignalCnt2);
		pstUserFinder->stUser_Define[nIndex].ResultViewNum = ntohl(pstUserFinder->stUser_Define[nIndex].ResultViewNum);

		for (int nIndex_Param = 0; nIndex_Param < (int)MAX_PARAM; nIndex_Param++)
			theApp.SetByteOrderRevDouble((char far *)&(pstUserFinder->stUser_Define[nIndex].Param[nIndex_Param]));

		for (int nIndex_Condi = 0; nIndex_Condi < (int)MAX_CONDI; nIndex_Condi++)
			theApp.SetByteOrderRevDouble((char far *)&(pstUserFinder->stUser_Define[nIndex].Condition[nIndex_Condi]));
	}

	//pstUserFinder->ScriptLength = ntohl(pstUserFinder->ScriptLength);
}

void CLib_ConditionMngApp::SetByteOrderRevDouble(LPSTR pDouble)
{
	int nCount, nLength = 4;

	for (nCount = 0; nCount < nLength; nCount++)
	{
		unsigned char chTemp;
		chTemp = *(pDouble + nCount);
		*(pDouble + nCount) = *(pDouble + sizeof(double) - 1 - nCount);
		*(pDouble + sizeof(double) - 1 - nCount) = chTemp;
	}
}

void CLib_ConditionMngApp::SetInitVal()
{
	m_listOneElement.RemoveAll();
	m_listGroupElement.RemoveAll();
	m_listConditionGroup.RemoveAll();

	m_saPortGroup.RemoveAll();
	m_bPortChk = FALSE;
}

BOOL CLib_ConditionMngApp::GetLoad_ConditionData(CString strFullFilePath)
{
	SetInitVal();

	BOOL bResult = TRUE;

	MSXML::IXMLDOMNodePtr			    pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;

	MSXML::IXMLDOMNodeListPtr		  pNSFixHead;
	MSXML::IXMLDOMNodePtr			    pNFixHead;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFixHead;

	MSXML::IXMLDOMNodeListPtr		  pNSCondition;
	MSXML::IXMLDOMNodePtr			    pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;

	MSXML::IXMLDOMNodeListPtr		  pNSFormat;
	MSXML::IXMLDOMNodePtr			    pNFormat;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFormat;

	CString strTemp;
	HRESULT hr;
	hr = CoInitialize(NULL);

	try {
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		pDOMConditionIndex->async = false;
		VARIANT_BOOL bLoadedXMLFile;
		bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strFullFilePath));

		if(bLoadedXMLFile == VARIANT_FALSE) 
		{
			bResult = FALSE;
			return bResult;
		}

		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		pNAConditions = pNConditions->attributes;
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;

		/*
		//## PortPolio Element Set ##
		pNAttributeTemp = pNAConditions->getNamedItem("PORTFOLIO_DATA");
		strTemp = CString((PCHAR)pNAttributeTemp->text);

		CString strPortfolio; 
		int nFindPos, nFindPos2;
		int nPortLength = strTemp.GetLength();
		int nCount = 0, nMaxGroupNum = 1;
		do
		{
		nFindPos = strTemp.Find('/');
		if (nFindPos == -1)
		break;
		strPortfolio = strTemp.Mid(0, nFindPos);

		PortPolio  strucePort;

		nFindPos2 = strPortfolio.Find("_");
		strucePort.strPortNumber = strPortfolio.Mid(0, nFindPos2); 
		strucePort.strPortName = strPortfolio.Mid(nFindPos2 + 1, strPortfolio.GetLength() - nFindPos2); 
		//m_ctrlPortEdit.AddPortData(strucePort);

		m_saPortGroup.Add(strucePort.strPortNumber);

		strTemp = strTemp.Mid(nFindPos + 1, nPortLength - nFindPos);
		nCount++;
		}while(1);

		//## PortPolio Operation Set ##
		pNAttributeTemp = pNAConditions->getNamedItem("PORTFOLIO_OPERATOR");
		m_bPortOpt = (CString((PCHAR)pNAttributeTemp->text) == "and") ? TRUE : FALSE;

		pNAttributeTemp = pNAConditions->getNamedItem("PORTFOLIO_CHK");
		m_bPortChk = (CString((PCHAR)pNAttributeTemp->text) == "1") ? TRUE : FALSE;
		*/

		pNAttributeTemp = pNAConditions->getNamedItem("TARGETBASE_TYPE");
		int nTargetBaseIndex = atoi((PCHAR)pNAttributeTemp->text);

		//## User_Define Struct Data Insert ##
		pNSCondition = pNConditions->selectNodes("CONDITION");
		int nConditionCnt = pNSCondition->length;
		for(int nConditionIndex = 0; nConditionIndex < nConditionCnt; nConditionIndex++)
		{ 
			OneConditionInfo	stOneConditionInfo;

			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNCondition = pNSCondition->item[nConditionIndex];				
			pNACondition = pNCondition->attributes;

			pNAttributeTemp = pNACondition->getNamedItem("CONDITION_FLAG");
			stOneConditionInfo.bFalg = (CString((PCHAR)pNAttributeTemp->text) == "1") ? TRUE : FALSE;

			pNAttributeTemp = pNACondition->getNamedItem("NAME");
			stOneConditionInfo.strName = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("TAGNAME");
			stOneConditionInfo.strTag = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("FILE_NAME");
			stOneConditionInfo.strMapFile = CString((PCHAR)pNAttributeTemp->text);

			//## Grid Field View CheckBox View State ##
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_VAL");
			stOneConditionInfo.bFieldVal = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;

			//## View CheckBox State Flag ##
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_STATE");
			stOneConditionInfo.bFieldState = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;

			pNAttributeTemp = pNACondition->getNamedItem("FIELD_ENABLE");
			stOneConditionInfo.bFieldEnable = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;

			//### User_define Data Set ###
			pNAttributeTemp = pNACondition->getNamedItem("INDEX_NO");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.IndexNo = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("GROUP_NO");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.GroupNo = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("TARGETBASE_TYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.nTargetBaseType = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("TARGETBASE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.TargetBase = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("TYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Type = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("TYPE1");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Type1 = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("TYPE2");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Type2 = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("STERM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.STerm = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("ETERM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.ETerm = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("RANK");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Rank = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("TREND");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.Trend = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("SIGNALCNT1");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.SignalCnt1 = atoi(strTemp);

			pNAttributeTemp = pNACondition->getNamedItem("SIGNALCNT2");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.SignalCnt2 = atoi(strTemp);

			//# RESULTVIEW => 값보기(Values)
			pNAttributeTemp = pNACondition->getNamedItem("RESULTVIEWNUM");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			stOneConditionInfo.stCondition.ResultViewNum = atoi(strTemp);

			//# FIELD ID
			CString strFieldID;
			pNAttributeTemp = pNACondition->getNamedItem("FIELD_ID");
			strFieldID = CString((PCHAR)pNAttributeTemp->text);

			//# Add Move Filed Info!
			GetMoveFieldIDToInfo(strFieldID, stOneConditionInfo.stHeadMoveInfo);

			//# INDEX TYPE
			pNAttributeTemp = pNACondition->getNamedItem("INDEXTYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.IndexType,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.IndexType));

			//# SEARCH TYPE
			pNAttributeTemp = pNACondition->getNamedItem("SEARCHTYPE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.SearchType,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.SearchType));

			//# OPERATOR
			pNAttributeTemp = pNACondition->getNamedItem("OPERATOR");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.Operator,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.Operator));

			//# REVERSE
			pNAttributeTemp = pNACondition->getNamedItem("REVERSE");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.Reverse,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.Reverse));

			pNAttributeTemp = pNACondition->getNamedItem("GUBUN");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			memcpy(&stOneConditionInfo.stCondition.IdxContiGubun,
				strTemp.operator const char*(),
				sizeof(stOneConditionInfo.stCondition.IdxContiGubun));

			//# PARAM
			for(int i = 0; i < MAX_PARAM; i++)
			{
				strTemp.Format("P%d", i+1);
				pNAttributeTemp = pNACondition->getNamedItem(_bstr_t(strTemp));
				if(pNAttributeTemp == NULL)
				{
					stOneConditionInfo.stCondition.Param[i] = 0.0;
					continue;
				}
				stOneConditionInfo.stCondition.Param[i] = atof(CString((PCHAR)pNAttributeTemp->text));
			}

			//# CONDITION
			for (int j = 0; j < MAX_CONDI; j++)
			{
				strTemp.Format("C%d", j+1);
				pNAttributeTemp = pNACondition->getNamedItem(_bstr_t(strTemp));
				if(pNAttributeTemp == NULL)
				{
					stOneConditionInfo.stCondition.Condition[j] = 0.0;
					continue;
				}
				stOneConditionInfo.stCondition.Condition[j] = atof(CString((PCHAR)pNAttributeTemp->text));
			}

			//## Title Name ##
			pNFormat = pNCondition->selectSingleNode("FORMAT");
			pNAFormat = pNFormat->attributes;
			stOneConditionInfo.strText = CString((PCHAR)pNFormat->text);

			//## Element Info ##
			OneElementInfo	stOneElement;

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_VIEW");
			stOneElement.bView = stOneConditionInfo.bFalg;

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_ELEMENT");
			stOneElement.strElement = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_NOT");
			stOneElement.strNot = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_OPERATION");
			stOneElement.strOperation = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_BRACKET");
			stOneElement.strBracket = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_BRACKETCNT");
			stOneElement.lBracketCount = atol((PCHAR)pNAttributeTemp->text);

			//if (stOneConditionInfo.bFalg == FALSE)
			//	continue;

			ConditionGroupInfo		stConditionGroupInfo;

			stConditionGroupInfo.strElement = stOneElement.strElement;
			stConditionGroupInfo.nGroup = stOneConditionInfo.stCondition.GroupNo;
			stConditionGroupInfo.stOneConditionInfo = stOneConditionInfo;
			m_listConditionGroup.AddTail(stConditionGroupInfo);

			if (stConditionGroupInfo.nGroup == 0)
				m_listOneElement.AddTail(stOneElement);
			else
			{
				GroupElementInfo  stGroupElement;

				stGroupElement.nGroup = stConditionGroupInfo.nGroup;
				stGroupElement.stOneElementInfo = stOneElement;
				m_listGroupElement.AddTail(stGroupElement);
			}
		}  
	} 
	catch(_com_error &e)
	{
		e = e;
		bResult = FALSE;
	}

	return bResult;
}

BOOL CLib_ConditionMngApp::SetMakePacket(BOOL bDefultResultField, User_Finder &stUser_Finder, int &nOrderCnt)
{
	m_saPortItem.RemoveAll();

	BOOL bResult = FALSE;

	BOOL bHave_Port = FALSE;

	int nUserDefineCnt = 0;
	POSITION posFirstOrder = NULL;
	User_Define		stUser_Define[MAX_IDXCNT];
	CList<User_Define, User_Define> listUser_Define;

	memset((LPVOID)&stUser_Finder, 0x00, SZ_User_Finder);
	memset((LPVOID)stUser_Define, 0x00, SZ_User_Define * MAX_IDXCNT);

	stUser_Finder.TargetIdxCnt = 0;
	stUser_Finder.TargetResultViewCnt = 0;
	stUser_Finder.ComplexIdxCnt = 0;
	stUser_Finder.ComplexResultViewCnt = 0;
	stUser_Finder.TargetIdxCnt = 0;
	stUser_Finder.ComplexIdxCnt = 0;

	int nCntCandle = 0;
	CString strElement;
	OneConditionInfo stOneConditionInfo;
	stOneConditionInfo.stCondition.IndexNo = 0;

	int nMaxGroup = GetMaxGroup();
	if (GetMaxGroup() < 0)
		return bResult;

	int nMoveFieldCnt = 0;
	for (int nGroupIndex = 0; nGroupIndex <= GetMaxGroup(); nGroupIndex++)
	{ 
		int nGroupItemCnt = GetGroupToConditionCnt(nGroupIndex, FALSE);

		if (nGroupIndex == 0)
			stUser_Finder.TargetIdxCnt = nGroupItemCnt;
		else
			stUser_Finder.ComplexIdxCnt += nGroupItemCnt;

		for (int nConditionIndex = 0; nConditionIndex < GetGroupToConditionCnt(nGroupIndex); nConditionIndex++)
		{
			strElement = GetElementInfo(nGroupIndex, nConditionIndex);

			if (strElement.IsEmpty() == TRUE)
				continue;

			GetElementToOneConditionInfo(strElement, stOneConditionInfo);	

			if (stOneConditionInfo.bFalg == TRUE)
			{
				stOneConditionInfo.stCondition.IndexNo++;

				stOneConditionInfo.stCondition.GroupNo = nGroupIndex;
				stOneConditionInfo.stCondition.GroupIdxCnt = GetGroupToConditionCnt(nGroupIndex);

				strcpy(&stOneConditionInfo.stCondition.Element, strElement.GetBuffer(0));

				stOneConditionInfo.stCondition.STerm += nCntCandle;
				stOneConditionInfo.stCondition.ETerm += nCntCandle;


				if (stOneConditionInfo.stCondition.IdxContiGubun == '0')
				{
					//# 값 보기 필드 정보 재구성!
					if (bDefultResultField == TRUE)
						stOneConditionInfo.stCondition.ResultViewNum = 0;
					else
					{
						if (stOneConditionInfo.bFieldVal == TRUE)
							nMoveFieldCnt += stOneConditionInfo.stCondition.ResultViewNum;
					}

					/*
					if (stOneConditionInfo.bFieldVal == TRUE)
					{	
					//m_ResultGrid.HeadMoveInsert(stOneConditionInfo.stHeadMoveInfo);

					stOneConditionInfo.stCondition.ResultViewNum = stOneConditionInfo.stHeadMoveInfo.nColCnt;
					}
					else
					stOneConditionInfo.stCondition.ResultViewNum = 0;
					*/

					if (posFirstOrder == NULL)
						listUser_Define.AddTail(stOneConditionInfo.stCondition);
					else
						listUser_Define.InsertBefore(posFirstOrder, stOneConditionInfo.stCondition);
				}
				else
				{
					//# 값보기 삭제	<= 고정 필드로 구성
					if (bDefultResultField == TRUE)
						stOneConditionInfo.stCondition.ResultViewNum = 0;
					else
					{
						if (stOneConditionInfo.bFieldVal == TRUE)
							nMoveFieldCnt += stOneConditionInfo.stCondition.ResultViewNum;
					}

					if (posFirstOrder == NULL)
					{
						listUser_Define.AddTail(stOneConditionInfo.stCondition);
						posFirstOrder = listUser_Define.GetTailPosition();
					}
					else
						listUser_Define.AddTail(stOneConditionInfo.stCondition);
				}
			}
		}

		nOrderCnt = 0;
		if (listUser_Define.GetCount() > 0)
		{
			POSITION pos;
			for (int nIndex = 0; nIndex < listUser_Define.GetCount(); nIndex++)
			{
				pos = listUser_Define.FindIndex(nIndex);
				stUser_Define[nUserDefineCnt] = listUser_Define.GetAt(pos);

				if (stUser_Define[nUserDefineCnt].IndexType == '1')   //# 순위지표 개수! (2006.02.13)
					nOrderCnt++;

				if ((stUser_Define[nUserDefineCnt].Type == 9999) && (bHave_Port == FALSE))
					bHave_Port = TRUE;


				/*
				//# News Condition => C1 Val Change 
				if (stUser_Define[nUserDefineCnt].Type == NEWS_TYPE)
				{
				stUser_Define[nUserDefineCnt].Condition[0] = GetNewsCodeToIndex((int)stUser_Define[nUserDefineCnt].Condition[0]);

				stUser_Define[nUserDefineCnt].SignalCnt1 = 1;
				stUser_Define[nUserDefineCnt].SignalCnt2 = 1;
				}
				*/

				nUserDefineCnt++;
			}
		}

		listUser_Define.RemoveAll();
		posFirstOrder = NULL;
	}

	if (nUserDefineCnt == 0)
	{
		TRACE((CString)PROGRAM_DEBUG_TITLE + "SetMakePacket => 검색할 조건이 없습니다.");
		return FALSE;
	}
	else
		memcpy(&stUser_Finder.stUser_Define, &stUser_Define, (nUserDefineCnt * sizeof(User_Define)));

	/*
	m_stUser_Finder.FieldEditerCnt = m_ResultGrid.m_nNumFixField;
	if (m_ResultGrid.HeadFixJongCodeFlag() == TRUE)
	m_stUser_Finder.FieldEditerCnt--;

	POSITION	posFix;
	HeadInfoType TempHeadInfo;
	int nFixFieldIndex = 0;
	for (nIndex = 0; nIndex < m_ResultGrid.m_nNumFixField; nIndex++)
	{
	posFix = m_ResultGrid.m_HeadInfo.FindIndex(nIndex);
	TempHeadInfo = m_ResultGrid.m_HeadInfo.GetAt(posFix);

	if (TempHeadInfo.lHeadID != JONGCODE_ID)
	{
	m_stUser_Finder.FieldList[nFixFieldIndex] = TempHeadInfo.lHeadID;
	nFixFieldIndex++;
	}
	}
	*/


	CString strCommonExpression = "";
	GetString_LogicOne(strCommonExpression);
	if(strCommonExpression != "")
	{
		strCommonExpression = strCommonExpression + "!";
		memset(stUser_Finder.TargetExp, ' ', EXPLEN);
		memcpy(stUser_Finder.TargetExp, strCommonExpression, strCommonExpression.GetLength());
	}

	CString strOrderExpression = "";
	GetString_LogicGroup(strOrderExpression);
	if(strOrderExpression != "")
	{
		strOrderExpression = strOrderExpression + "!";
		memset(stUser_Finder.ComplexExp, ' ', EXPLEN);
		memcpy(stUser_Finder.ComplexExp, strOrderExpression, strOrderExpression.GetLength());
	}

	if ((m_pLogicCalculate != NULL) && (bHave_Port == TRUE))
	{
		strCommonExpression.Replace("!", "");
		strOrderExpression.Replace("!", "");
		CString strLogic;

		if (strCommonExpression.IsEmpty() == TRUE) 
			strLogic = strOrderExpression;
		else if (strOrderExpression.IsEmpty() == TRUE)
			strLogic = strCommonExpression;
		else
			strLogic = strCommonExpression + "&" + strOrderExpression;

		if (strLogic.IsEmpty() == FALSE)
		{
			memset(stUser_Finder.TargetBit, 0x30, (int)RESULT_MAX);

			char sTargetBit[RESULT_MAX - RESULT_MAX_OFFSET];
			stUser_Finder.TargetBitChk = (SetTargetBit(strLogic, sTargetBit) == TRUE) ?'1':'0';
			if (stUser_Finder.TargetBitChk == '1')
				memcpy(stUser_Finder.TargetBit + 1, sTargetBit, (int)RESULT_MAX - (int)RESULT_MAX_OFFSET);
		}
		else
		{
			stUser_Finder.TargetBitChk = '0';
			memset(&stUser_Finder.TargetBit, 0x30, (int)RESULT_MAX);
		}
	}
	else
	{
		stUser_Finder.TargetBitChk = '0';
		memset(&stUser_Finder.TargetBit, 0x30, (int)RESULT_MAX);
	}

	//# Back Test 제외!
	stUser_Finder.BackDay = -1;
	stUser_Finder.BackTime = -1;

	//# Set Fix Field!
	if (bDefultResultField == TRUE)
		SetMakePacket_Field(-1, m_stUser_Finder);
	else
		SetMakePacket_Field(nMoveFieldCnt, m_stUser_Finder);

	return TRUE;
}

//extern void Dll_GetRegKwansimMasterGroupItem(CString strName, CStringArray &arrItem);	-hidden-
BOOL CLib_ConditionMngApp::SetTargetBit(CString strLogic, char *psResultData)
{
	//# Logic Calculate!   Add : 2006.10
	typedef struct tagCodeList
	{
		char sElement;
		int nCnt;
		char *psData;
	} CodeList;


	char *psLogic;
	psLogic = (char *)malloc(strLogic.GetLength() + 1);
	memset(psLogic, 0, strLogic.GetLength() + 1);
	memcpy(psLogic, strLogic.GetBuffer(0), strLogic.GetLength());

	CPtrArray *paData = new CPtrArray;

	CString strCode;
	int nIndex, nItem;
	POSITION pos;
	ConditionGroupInfo  stConditionGroupInfo;
	for (nIndex = 0; nIndex < m_listConditionGroup.GetCount(); nIndex++)
	{
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);

		CodeList *pstPortInfo = (CodeList *)malloc(sizeof(CodeList));
		memset(pstPortInfo, 0x00, sizeof(CodeList));
		memcpy(&pstPortInfo->sElement, stConditionGroupInfo.strElement.GetBuffer(0), sizeof(pstPortInfo->sElement));
		if (stConditionGroupInfo.stOneConditionInfo.strTag == (CString)TAG_TYPE_PORTPOLIO)
		{
			CStringArray saItem, saName;
			//Dll_GetRegKwansimMasterGroupItem(stConditionGroupInfo.stOneConditionInfo.strName, saItem); //-hidden-
			CString strKey = GetGwanKeyFromName(stConditionGroupInfo.stOneConditionInfo.strName);
			GetGwansimItem(strKey, saItem, saName);
			pstPortInfo->nCnt = saItem.GetSize();

			int nSize = 7 * pstPortInfo->nCnt;
			pstPortInfo->psData = (char *)malloc(nSize);
			memset(pstPortInfo->psData, 0x30, nSize); 
			for (nItem = 0; nItem < saItem.GetSize(); nItem++)
			{
				strCode = saItem.GetAt(nItem);
				strCode = strCode.Left((int)CODE_LENGTH);
				m_saPortItem.Add(strCode);
				memcpy(pstPortInfo->psData + 7 * nItem, strCode.GetBuffer(0), 7);
			}
		}

		paData->Add(pstPortInfo);
	}

	BOOL bState = m_pLogicCalculate->GetLogicCalculate(psLogic, paData, &psResultData);
	if (bState == TRUE)
		SetSendMainMsg("LogicCalculate 성공!");
	else
		SetSendMainMsg("LogicCalculate 실패!");

	m_pLogicCalculate->SetFree_PtrArray(paData);
	//Lib_SetFree_PtrArray(paData);
	delete paData;
	free(psLogic);

	int nTrueCnt = 0, nFalseCnt = 0;
	for (nIndex = RESULT_MAX_OFFSET; nIndex < (int)RESULT_MAX; nIndex++)
	{
		if (psResultData[nIndex] == '1')
			nTrueCnt++;
		else if (psResultData[nIndex] == '0')
			nFalseCnt++;
	}

	BOOL bResult;
	if ((nTrueCnt == m_saMSTCode.GetSize()) || (nTrueCnt == 0))
		bResult = FALSE;
	else
		bResult = TRUE;
	return bResult;
}

CString CLib_ConditionMngApp::GetElementInfo(int nGroup, int nConditionIndex)
{
	CString strElement = "";

	int nConditionIndex_Target = 0;

	POSITION pos;
	ConditionGroupInfo  stConditionGroupInfo;
	for (int nIndex = 0; nIndex < m_listConditionGroup.GetCount(); nIndex++)
	{
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);

		if (stConditionGroupInfo.nGroup == nGroup)
		{
			if (nConditionIndex == nConditionIndex_Target)
			{
				strElement = stConditionGroupInfo.strElement;
				break;
			}
			else
				nConditionIndex_Target++;
		}
	}

	return strElement;
}

void CLib_ConditionMngApp::SetMakePacket_Field(int nMoveFieldCnt, User_Finder	&stUser_Finder)
{
	//# 기본 필드 : 종목명 / 종목코드

	int nIndex;
	if (nMoveFieldCnt == -1)		//# 고정 필드 구성!
	{
		stUser_Finder.FieldEditerCnt = _countof(FIX_FIELD_ID);
		for (nIndex = 0; nIndex < _countof(FIX_FIELD_ID); nIndex++)
			stUser_Finder.FieldList[nIndex] = FIX_FIELD_ID[nIndex];
	}
	else		//# 값보기 필드 구성!
	{
		stUser_Finder.FieldEditerCnt = nMoveFieldCnt;
		for (nIndex = 0; nIndex < nMoveFieldCnt; nIndex++)
			stUser_Finder.FieldList[nIndex] = 0;
	}
}

void _DLL_GetFind_FixFieldInfo(CStringArray &saColName, CUIntArray &iaColUnit)
{
	saColName.RemoveAll();
	iaColUnit.RemoveAll();

	saColName.Add("대비부호");
	iaColUnit.Add(0);

	saColName.Add("종목코드");
	iaColUnit.Add(0);

	saColName.Add("종목명");
	iaColUnit.Add(0);

	for (int nIndex = 0; nIndex < _countof(FIX_FIELD_ID); nIndex++)
	{
		saColName.Add((CString)FIX_FIELD_NAME[nIndex]);
		iaColUnit.Add(FIX_FIELD_UNIT[nIndex]);
	}
}

int CLib_ConditionMngApp::GetMaxGroup()
{
	int nResult = -1;

	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	ConditionGroupInfo stConditionGroupInfo;

	for (int i = 0; i < nCount; i++)
	{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);

		if (nResult < stConditionGroupInfo.nGroup)
			nResult = stConditionGroupInfo.nGroup;
	}

	return nResult;
}

int CLib_ConditionMngApp::GetGroupToConditionCnt(int nGroup, BOOL bAll)
{
	int nResult = 0;

	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	ConditionGroupInfo stConditionGroupInfo;

	for (int i = 0; i < nCount; i++)
	{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);

		if (stConditionGroupInfo.nGroup == nGroup)
		{
			if (bAll == TRUE)
				nResult++;
			else
			{
				if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
					nResult++;
			}
		}
	}

	return nResult;
}

BOOL CLib_ConditionMngApp::GetElementToOneConditionInfo(CString strElement, OneConditionInfo  &stOneConditionInfo)
{
	BOOL bResult = FALSE;

	ConditionGroupInfo  stConditionGroupInfo;

	strElement.TrimLeft();
	strElement.TrimRight();

	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);

		stConditionGroupInfo.strElement.TrimLeft();
		stConditionGroupInfo.strElement.TrimRight();
		if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
		{
			stOneConditionInfo = stConditionGroupInfo.stOneConditionInfo;
			bResult = TRUE; 
			break;
		}
	}

	return bResult;
}

void CLib_ConditionMngApp::GetString_LogicOne(CString &strLogic)
{
	strLogic = "";


	OneElementInfo  stOneElement;
	int nCount = m_listOneElement.GetCount();

	POSITION pos;
	int nElementIndex = 0;
	int nViewCount = 0;
	for(int i = 0; i < nCount; i++)
	{
		pos = m_listOneElement.FindIndex(i);

		//## Element 정리 ##
		stOneElement = m_listOneElement.GetAt(pos);

		if (stOneElement.bView == FALSE)
			continue;
		else
			nElementIndex++;

		CString strOperation;
		if (stOneElement.bView == TRUE)
		{
			//if (nStepCnt >= nViewCount)  //## nStepIndex
			//{
			if (stOneElement.strOperation == "and")
			{
				//strarrayOperater.Add("1");
				strOperation = "&";
			}
			else if (stOneElement.strOperation == "or")
			{
				//strarrayOperater.Add("0");
				strOperation = "|";
			}
			else if (stOneElement.strOperation == "next")  //## 순차검색
			{
				//strarrayOperater.Add("1");
				strOperation = "~";
			}

			if(nElementIndex == 1)
				strLogic += stOneElement.strBracket + stOneElement.strElement;
			else
			{
				if(stOneElement.strBracket.IsEmpty() == TRUE)
					strLogic += strOperation + stOneElement.strElement;
				else if( stOneElement.strBracket[0] == '(' )
					strLogic += strOperation + stOneElement.strBracket + stOneElement.strElement;
				else if( stOneElement.strBracket[0] == ')' )
					strLogic += strOperation + stOneElement.strElement + stOneElement.strBracket;
			}

			nViewCount++;
			//}
		}
	}
}


void CLib_ConditionMngApp::GetString_LogicGroup(CString &strGroupLogic)
{
	strGroupLogic = "";


	int nElementIndex = 0;
	int nAfterGroup, nMaxGroup;
	nMaxGroup = GetMaxGroup();
	int nCount = m_listGroupElement.GetCount();
	if (nCount == 0)
		return;

	POSITION pos;
	GroupElementInfo  stGroupElement;
	for (int nNowGroup = 1; nNowGroup <= nMaxGroup; nNowGroup++)
	{
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pos = m_listGroupElement.FindIndex(nIndex);
			stGroupElement = m_listGroupElement.GetAt(pos);

			//## Group 구분 ##
			if (nNowGroup == stGroupElement.nGroup)
			{
				if (stGroupElement.stOneElementInfo.bView == TRUE)
					nElementIndex++;
				else
					continue;

				if (stGroupElement.stOneElementInfo.bView == TRUE)
				{
					if (nElementIndex == 1)
						strGroupLogic += stGroupElement.stOneElementInfo.strBracket 
						//+ stGroupElement.stOneElementInfo.strNot
						+ stGroupElement.stOneElementInfo.strElement;
					else
					{
						CString strOpt = "";
						if (stGroupElement.nGroup != nAfterGroup)
							strOpt = "~";
						else
						{
							if (stGroupElement.stOneElementInfo.strOperation == "and")
								strOpt = "&";
							else if (stGroupElement.stOneElementInfo.strOperation == "or")
								strOpt = "|";
						}

						if(stGroupElement.stOneElementInfo.strBracket.IsEmpty() == TRUE)
						{
							if (strOpt.IsEmpty() == FALSE)
								strGroupLogic += " " + strOpt + " " 
								//+ stOneElement.strNot +
								+ stGroupElement.stOneElementInfo.strElement;
							else
								strGroupLogic += " " + strOpt 
								+ " " //+ stOneElement.strNot
								+ stGroupElement.stOneElementInfo.strElement;
						}
						else if (stGroupElement.stOneElementInfo.strBracket[0] == '(')
						{
							if (strOpt.IsEmpty() == FALSE)
							{
								strGroupLogic += " " + strOpt + " " 
									+ stGroupElement.stOneElementInfo.strBracket 
									//+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement;
							}
							else
							{
								strGroupLogic += " " + strOpt
									+ " " + stGroupElement.stOneElementInfo.strBracket 
									//+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement;
							}
						}
						else if (stGroupElement.stOneElementInfo.strBracket[0] == ')')
						{
							if (strOpt.IsEmpty() == FALSE)
							{
								strGroupLogic += " " + strOpt + " " 
									//+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement 
									+ stGroupElement.stOneElementInfo.strBracket;
							}
							else
							{
								strGroupLogic += " " + strOpt 
									+ " " //+ stOneElement.strNot
									+ stGroupElement.stOneElementInfo.strElement 
									+ stGroupElement.stOneElementInfo.strBracket;
							}
						}
					}

					nAfterGroup = nNowGroup;
				}
			}
		}
	}

	strGroupLogic.Replace(" ", "");
}

BOOL _DLL_GetFind_Result(LPVOID	aTRData, long dwTRDateLen, 
						 CStringArray &saColData, CStringArray &saResultData,
						 BOOL bDefultResultField/* = FALSE*/)
{
	if (dwTRDateLen == 0) 
		return FALSE;

	saColData.RemoveAll();
	saResultData.RemoveAll();

	char *pData = (char*)aTRData;
	OutPacket stOutPacket;
	memset(&stOutPacket, 0x00, SZ_OutPacket);
	memcpy(&stOutPacket, aTRData, SZ_OutPacket);

	//# Byte Ordering!
	//@유진사용안함.ntohl	stOutPacket.lTrIndex = ntohl(stOutPacket.lTrIndex);
	//@유진사용안함.ntohl	stOutPacket.lTotalJongCnt = ntohl(stOutPacket.lTotalJongCnt);
	//@유진사용안함.ntohl	stOutPacket.lOutFieldCnt = ntohl(stOutPacket.lOutFieldCnt);
	//@유진사용안함.ntohl	stOutPacket.lOutListPacketSize = ntohl(stOutPacket.lOutListPacketSize);
	//@유진사용안함.ntohl	stOutPacket.lFindTime = ntohl(stOutPacket.lFindTime);

	int nIndex, nColIndex;
	CUIntArray		iaUnitNum;
	iaUnitNum.RemoveAll();
	if (bDefultResultField == TRUE)
	{
		for (nIndex = 0; nIndex < _countof(FIX_FIELD_ID); nIndex++)
		{
			saColData.Add(FIX_FIELD_NAME[nIndex]);
			iaUnitNum.Add(FIX_FIELD_UNIT[nIndex]);
		}
	}
	else
	{
		POSITION pos;
		ConditionGroupInfo		stConditionGroupInfo;
		for (nIndex = 0; nIndex < theApp.m_listConditionGroup.GetCount(); nIndex++)
		{
			pos = theApp.m_listConditionGroup.FindIndex(nIndex);
			stConditionGroupInfo = theApp.m_listConditionGroup.GetAt(pos);

			if ((stConditionGroupInfo.stOneConditionInfo.bFieldVal == TRUE) &&			//# 값보기
				(stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE))			//# 값보기 Check Field Have
			{
				for (nColIndex = 0; nColIndex < stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt; nColIndex++)
				{
					saColData.Add(stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.stHeadType[nColIndex].strHeadName);
					iaUnitNum.Add(stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.stHeadType[nColIndex].nUnitNum);
				}
			}
		}
	}

	LONG lTotCnt = 0L;
	LONG lTime = 0L;
	LONG lFieldCnt = 0L;
	LONG lSize = 0L;

	lTotCnt = stOutPacket.lTotalJongCnt;
	lFieldCnt = stOutPacket.lOutFieldCnt;		//# fix field + move field count
	lSize = stOutPacket.lOutListPacketSize;	//# 한종목에 해당하는 결과 List size
	lTime = stOutPacket.lFindTime;

	pData = &pData[SZ_OutPacket];
	dwTRDateLen = dwTRDateLen - SZ_OutPacket;

	CString strTemp, strResult = "";
	OutListPacket stOutListPacket;
	char *pchData;
	for (nIndex = 0; nIndex < lTotCnt; nIndex++)
	{
		memset(&stOutListPacket, 0x00, SZ_OutListPacket);
		memcpy(&stOutListPacket, pData, lSize);

		//# Byte Ordering!
		theApp.SetByteOrdering_Out(lFieldCnt, &stOutListPacket);

		pchData = new char[sizeof(stOutListPacket.chSign) + 1];
		memset(pchData, 0x00, sizeof(stOutListPacket.chSign) + 1);
		memcpy(pchData, &stOutListPacket.chSign, sizeof(stOutListPacket.chSign));
		strResult = (CString)pchData + "/";
		delete [] pchData;

		//# 2006. 12 : Add Danny!
		pchData = new char[sizeof(stOutListPacket.chMarket) + 1];
		memset(pchData, 0x00, sizeof(stOutListPacket.chMarket) + 1);
		memcpy(pchData, &stOutListPacket.chMarket, sizeof(stOutListPacket.chMarket));
		strResult = (CString)pchData + "/";
		delete [] pchData;

		pchData = new char[sizeof(stOutListPacket.chCode) + 1];
		memset(pchData, 0x00, sizeof(stOutListPacket.chCode) + 1);
		memcpy(pchData, &stOutListPacket.chCode, sizeof(stOutListPacket.chCode));
		strResult += (CString)pchData + "/";
		delete [] pchData;

		pchData = new char[sizeof(stOutListPacket.chName) + 1];
		memset(pchData, 0x00, sizeof(stOutListPacket.chName) + 1);
		memcpy(pchData, &stOutListPacket.chName, sizeof(stOutListPacket.chName));
		strResult += (CString)pchData + "/";
		delete [] pchData;

		if (iaUnitNum.GetSize() > 0)
		{
			for (nColIndex = 0; nColIndex < lFieldCnt; nColIndex++)
			{
				strTemp = theApp.GetFormatStr(fabs(stOutListPacket.dFieldData[nColIndex]), iaUnitNum.GetAt(nColIndex), FALSE, "");
				strResult += strTemp + "/";
			}
		}

		saResultData.Add(strResult);
		strResult = "";

		pData = &pData[lSize];
		dwTRDateLen = dwTRDateLen - lSize;
	}

	return TRUE;
}

void CLib_ConditionMngApp::SetByteOrdering_Out(int nFieldCnt, OutListPacket *pstOutListPacket)
{
	//@유진사용안함.ntohl
	return;

	pstOutListPacket->lOrderRank = ntohl(pstOutListPacket->lOrderRank);

	for (int nIndex = 0; nIndex < nFieldCnt; nIndex++)
		SetByteOrderRevDouble((char far *)&(pstOutListPacket->dFieldData[nIndex]));
}

CString CLib_ConditionMngApp::GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;

	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";

		strData.Format(strDis, dData);
	}
	else
	{
		strDis = "%.0f";
		strData.Format(strDis, dData);
	}

	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;

		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
			strData = "0" + strData;

		int nCaretIndex = 0;
		for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
		{
			if ((CString)strFormat[nIndex] == "X")
			{
				strResult += (CString)strData[nCaretIndex];
				nCaretIndex++;
			}
			else
				strResult += (CString)strFormat[nIndex];
		}
	}
	else
	{
		int nCheckIndex = 0;
		BOOL bDotFlag = FALSE;
		nDotPos = strData.Find(".");

		int nStartIndex;
		nLength = strData.GetLength();
		if (nDotPos > 0)
		{
			nStartIndex = nDotPos;
			bDotFlag = TRUE;
		}
		else
		{
			nStartIndex = nLength;
			nDotPos = 0;
		}

		int nCaretIndex = 0;
		CString strResultInt;

		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
		{
			if (nCheckIndex == 3)
			{
				strResultInt += "," + (CString)strData[nIndex];
				nCheckIndex = 1;
			}
			else
			{
				strResultInt += (CString)strData[nIndex];
				nCheckIndex++;
			}
		}

		nLength = strResultInt.GetLength();
		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
			strResult += (CString)strResultInt[nIndex];

		if (nDotPos > 0)
			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
	}

	return strResult;
}

CString CLib_ConditionMngApp::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;

	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";

		strData.Format(strDis, lData / (float)nUnitNum);
	}
	else
	{
		strDis = "%ld";
		strData.Format(strDis, lData);
	}

	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;

		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
			strData = "0" + strData;

		int nCaretIndex = 0;
		for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
		{
			if ((CString)strFormat[nIndex] == "X")
			{
				strResult += (CString)strData[nCaretIndex];
				nCaretIndex++;
			}
			else
				strResult += (CString)strFormat[nIndex];
		}
	}
	else
	{
		int nCheckIndex = 0;
		BOOL bDotFlag = FALSE;
		nDotPos = strData.Find(".");

		int nStartIndex;
		nLength = strData.GetLength();
		if (nDotPos > 0)
		{
			nStartIndex = nDotPos;
			bDotFlag = TRUE;
		}
		else
		{
			nStartIndex = nLength;
			nDotPos = 0;
		}

		int nCaretIndex = 0;
		CString strResultInt;

		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
		{
			if (nCheckIndex == 3)
			{
				strResultInt += "," + (CString)strData[nIndex];
				nCheckIndex = 1;
			}
			else
			{
				strResultInt += (CString)strData[nIndex];
				nCheckIndex++;
			}
		}

		nLength = strResultInt.GetLength();
		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
			strResult += (CString)strResultInt[nIndex];

		if (nDotPos > 0)
			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
	}

	return strResult;
}

void CLib_ConditionMngApp::SetClear_FieldInfo()
{
	if (m_pmapFieldInfo == NULL)
		return;

	POSITION pos;
	CString strKey;
	HeadInfoType *pstHeadType;
	for (pos = m_pmapFieldInfo->GetStartPosition(); pos != NULL;)
	{
		m_pmapFieldInfo->GetNextAssoc(pos, strKey, (void*&)pstHeadType);

		if (pstHeadType != NULL)
		{
			delete pstHeadType;
			pstHeadType = NULL;
		}
		m_pmapFieldInfo->RemoveKey(strKey);
	}
	m_pmapFieldInfo->RemoveAll();
	delete m_pmapFieldInfo;
	m_pmapFieldInfo = NULL;
}

void CLib_ConditionMngApp::GetLoad_FieldInfo()
{
	if (m_pmapFieldInfo == NULL)
		m_pmapFieldInfo = new CMapStringToPtr;

	CString strFullFilePath;
	strFullFilePath = GetAppFileRoot() + FOLDER_DATA + "\\" + MOVEFIELD_SYS_FILE;
	LoadFiledXML(strFullFilePath);

	strFullFilePath = GetAppFileRoot() + FOLDER_DATA + "\\" + FIXFIELD_SYS_FILE;
	LoadFiledXML(strFullFilePath);
}

void CLib_ConditionMngApp::GetMoveFieldIDToInfo(CString strHeadID, HeadMoveInfo  &stHeadMoveInfo)
{
	int nFindPos, nIndex;
	nIndex = 0;
	nFindPos = 0;
	CString strFieldID;
	HeadInfoType *pstHeadType;
	while (nFindPos > -1)
	{
		nFindPos = strHeadID.Find("/", nIndex * 4 + nIndex);
		strFieldID = strHeadID.Mid(nIndex * 4 + nIndex, 4);

		// (2010/11/22 - Seung-Won, Bae) for Not Found Exception.
		if( !m_pmapFieldInfo->Lookup( strFieldID, ( void*&)pstHeadType)) return;

		stHeadMoveInfo.stHeadType[nIndex].lHeadID = pstHeadType->lHeadID;
		stHeadMoveInfo.stHeadType[nIndex].strHeadName = pstHeadType->strHeadName;
		stHeadMoveInfo.stHeadType[nIndex].nUnitNum = pstHeadType->nUnitNum;
		stHeadMoveInfo.stHeadType[nIndex].strUnitText = pstHeadType->strUnitText;
		stHeadMoveInfo.stHeadType[nIndex].strDataType = pstHeadType->strDataType;
		stHeadMoveInfo.stHeadType[nIndex].nDataLength = pstHeadType->nDataLength;
		stHeadMoveInfo.stHeadType[nIndex].nColWidth = pstHeadType->nColWidth;
		stHeadMoveInfo.stHeadType[nIndex].strAlignment = pstHeadType->strAlignment;

		nIndex++;
	}
}

BOOL CLib_ConditionMngApp::LoadFiledXML(CString strFullFilePath)
{
	BOOL bResult = TRUE;

	if (m_pmapFieldInfo == NULL)
		return FALSE;

	MSXML::IXMLDOMNodePtr		      pNIndex;		//# Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;	//# Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   //# Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;

	HRESULT hr;
	hr = CoInitialize(NULL);

	HeadInfoType *pstHeadType;

	try {
		MSXML::IXMLDOMDocumentPtr pDOMFieldIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		pDOMFieldIndex->async = false;
		pDOMFieldIndex->load(_bstr_t(strFullFilePath));

		pNIndex = pDOMFieldIndex->selectSingleNode("INDEX");
		pNS_FItem  = pNIndex->selectNodes("FITEM");

		int nCount, nLength;
		CString strData;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			pstHeadType = new HeadInfoType;

			pN_FItem = pNS_FItem->item[nCount];	

			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;


			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strData = CString((PCHAR)pNAttributeTemp->text);
			pstHeadType->lHeadID = atol(strData);

			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			pstHeadType->strHeadName = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			strData = CString((PCHAR)pNAttributeTemp->text);
			pstHeadType->nUnitNum = atoi(strData);

			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			pstHeadType->strUnitText = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			pstHeadType->strDataType = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			strData = CString((PCHAR)pNAttributeTemp->text); 
			pstHeadType->nDataLength = atoi(strData);

			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			strData = CString((PCHAR)pNAttributeTemp->text);
			pstHeadType->nColWidth = atoi(strData);

			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			pstHeadType->strAlignment = CString((PCHAR)pNAttributeTemp->text);

			strData.Format("%d", pstHeadType->lHeadID);
			m_pmapFieldInfo->SetAt(strData, (void*&)pstHeadType);
		}
	} 
	catch(_com_error &e)
	{
		e = e;
		bResult = FALSE;
	}

	return bResult;
}

int _DLL_GetFilterToPacket(CString strFullPath, long *plPacket)
{
	int nResult = -1;

	FilterInfo stFilterInfo;

	stFilterInfo.plistDealTimeVol = new CList <SubFilterInfo, SubFilterInfo&>;
	stFilterInfo.plistDealTimeVol->RemoveAll();

	stFilterInfo.plistDealTimePrice = new CList <SubFilterInfo, SubFilterInfo&>;
	stFilterInfo.plistDealTimePrice->RemoveAll();

	if (theApp.GetLoad_FilterData(strFullPath, &stFilterInfo) == TRUE)
	{
		//# Make Packet!
		User_Finder	stUser_Finder;
		theApp.SetConvert_FilterToPacket(&stFilterInfo, stUser_Finder);

		_DLL_GetFind_Packet_ByteOrdering(&stUser_Finder);

		memset((User_Finder*)plPacket, 0x00, SZ_User_Finder);
		memcpy((User_Finder*)plPacket, &stUser_Finder, SZ_User_Finder);

		nResult = (int)SZ_User_Finder;
	}

	stFilterInfo.plistDealTimeVol->RemoveAll();
	delete stFilterInfo.plistDealTimeVol;
	stFilterInfo.plistDealTimeVol = NULL;

	stFilterInfo.plistDealTimePrice->RemoveAll();
	delete stFilterInfo.plistDealTimePrice;
	stFilterInfo.plistDealTimePrice = NULL;

	return nResult;
}

// 2009.10.26 -hidden-
void _DLL_GetFromatText(CString strFullPath, CString& strText)
{
	MSXML::IXMLDOMNodePtr			pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;

	MSXML::IXMLDOMNodeListPtr		pNSCondition;
	MSXML::IXMLDOMNodePtr			pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;

	MSXML::IXMLDOMNodePtr			pNFormat;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFormat;

	CString strTemp, strFormat;
	HRESULT hr;
	hr = CoInitialize(NULL);

	strFullPath = /*theApp.GetAppFileRoot() + "\\" +*/ strFullPath;
	try 
	{
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		pDOMConditionIndex->async = false;
		pDOMConditionIndex->load(_bstr_t(strFullPath));

		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		pNAConditions = pNConditions->attributes;
		pNSCondition = pNConditions->selectNodes("CONDITION");

		CStringArray arrGroup;
		int nGroupNo = 0;
		int nCurGroupNo = 0;
		CString strGeneral = ""; 
		CString strSequencese= "";
		CString strSequence= "";

		int nConditionCnt = pNSCondition->length;
		for(int nConditionIndex = 0; nConditionIndex < nConditionCnt; nConditionIndex++)
		{ 
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNCondition = pNSCondition->item[nConditionIndex];				
			pNACondition = pNCondition->attributes;

			OneElementInfo	stOneElement;
			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_BRACKETCNT");
			stOneElement.lBracketCount = atol((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_OPERATION");
			stOneElement.strOperation = CString((PCHAR)pNAttributeTemp->text);
			stOneElement.strOperation.MakeUpper();

			pNAttributeTemp = pNACondition->getNamedItem("LOGIC_BRACKET");
			stOneElement.strBracket = CString((PCHAR)pNAttributeTemp->text);


			pNFormat = pNCondition->selectSingleNode("FORMAT");
			pNAFormat = pNFormat->attributes;
			strFormat = CString((PCHAR)pNFormat->text);

			if(1 == stOneElement.lBracketCount)
				strTemp.Format("%s%s", stOneElement.strBracket, strFormat);
			else if(-1 == stOneElement.lBracketCount)
				strTemp.Format("%s%s", strFormat, stOneElement.strBracket);
			else
				strTemp = strFormat;

			pNAttributeTemp = pNACondition->getNamedItem("ORG_GROUP_NO");
			nGroupNo = atoi((PCHAR)pNAttributeTemp->text);
			if(0 == nGroupNo)					// 일반검색
			{
				if(strGeneral.IsEmpty())
				{
					strGeneral += "[일반검색]\r\n";
					strGeneral += strTemp + "\r\n";
				}
				else
					strGeneral += stOneElement.strOperation + " " + strTemp + "\r\n";
			}
			else														// 순차검색
			{
				if(nGroupNo != nCurGroupNo)
				{
					strSequence.Format("[순차검색%d]\r\n", nGroupNo);
					strSequence += strTemp + "\r\n";

					nCurGroupNo = nGroupNo;
				}
				else
					strSequence = stOneElement.strOperation + " " + strTemp + "\r\n";

				strSequencese += strSequence;
			}
		} 
		strText = strGeneral + strSequencese;
	} 
	catch(_com_error &e)
	{
		e = e;
	}
}

void CLib_ConditionMngApp::SetUser_Define_BaseFilter(User_Define &stUser_Define)
{
	::memset(&stUser_Define, 0x00, SZ_User_Define);

	//stUser_Define.IndexNo = nConditionIndex; 				/* 지표 index				*/
	stUser_Define.GroupNo = 0; 				/* 소속 그룹번호			*/
	//stUser_Define.GroupIdxCnt;			/* 그룹내 지표 개수			*/

	stUser_Define.TargetBase = 0;
	//stUser_Define.Type = 3003;
	stUser_Define.Type1 = 99;
	stUser_Define.Type2 = 99;

	stUser_Define.STerm = 0;
	stUser_Define.ETerm = 1;

	stUser_Define.Rank = 100;
	stUser_Define.Trend = 2;
	stUser_Define.SignalCnt1 = 1;
	stUser_Define.SignalCnt2 = 1;				/* 발생회수	(new)					*/
	stUser_Define.ResultViewNum = 0;			/* 결과값보기 필드 갯수(0:사용안함)	*/    

	stUser_Define.IndexType = '0';				/* 검색구분 0:일반 1:순위 2:순차	*/
	stUser_Define.SearchType = '0';				/* 검색형태 0:일반 1:사용자정의	2:뉴스 3:패턴   (new) */

	stUser_Define.Operator = '1';				/* 연산자'0':OR '1':AND '2':NEXT	*/
	stUser_Define.Reverse = '0';				/* 결과제외 '0':A, '1':Not A		*/
	stUser_Define.IdxContiGubun = '0';			/* 지표의 연속발생 구분 '0':단일발생, 1:연속발생지표	*/
	//stUser_Define.Element = nConditionIndex;				/* 조건식 사용문자(A) (new)			*/
	//stUser_Define.Code[6];
}


void CLib_ConditionMngApp::SetConvert_FilterToPacket(FilterInfo *pstFilterInfo, User_Finder	&stUser_Finder)
{
	::memset(&stUser_Finder, 0x00, SZ_User_Finder);

	int nConditionIndex = 0;
	User_Define	stUser_Define;

	SetUser_Define_BaseFilter(stUser_Define);
	stUser_Define.IndexNo = nConditionIndex;
	memcpy(&stUser_Define.Element, CONDITION_INDEX[nConditionIndex], 1);
	if (pstFilterInfo->nMarketType == 0)			//# 0 : 전체, 1 : 코스피, 2 : 코스닥
	{
		stUser_Define.Type = 3003;

		stUser_Define.Condition[0] = 001;
		stUser_Define.Condition[1] = 201;
	}
	else if (pstFilterInfo->nMarketType == 1)			//# 0 : 전체, 1 : 코스피, 2 : 코스닥
	{
		stUser_Define.Type = 3001;
		stUser_Define.Condition[0] = 01;
	}
	else if (pstFilterInfo->nMarketType == 2)			//# 0 : 전체, 1 : 코스피, 2 : 코스닥
	{
		stUser_Define.Type = 3002;
		stUser_Define.Condition[1] = 201;
	}
	memcpy(&stUser_Finder.stUser_Define[nConditionIndex], &stUser_Define, SZ_User_Define);

	if (pstFilterInfo->nNowPriceChk == 1)
	{
		SetUser_Define_BaseFilter(stUser_Define);

		nConditionIndex++;

		stUser_Define.IndexNo = nConditionIndex;
		memcpy(&stUser_Define.Element, CONDITION_INDEX[nConditionIndex], 1);

		stUser_Define.Type = 2101;
		stUser_Define.Type1 = 5;

		stUser_Define.Condition[0] = pstFilterInfo->nNowPriceUp;
		stUser_Define.Condition[1] = pstFilterInfo->nNowPriceDown;

		memcpy(&stUser_Finder.stUser_Define[nConditionIndex], &stUser_Define, SZ_User_Define);
	}

	if (pstFilterInfo->nChangRateChk == 1)
	{
		SetUser_Define_BaseFilter(stUser_Define);

		nConditionIndex++;

		stUser_Define.IndexNo = nConditionIndex; 				/* 지표 index				*/
		memcpy(&stUser_Define.Element, CONDITION_INDEX[nConditionIndex], 1);

		stUser_Define.Type = 2102;
		stUser_Define.Type1 = 5;

		stUser_Define.Condition[0] = pstFilterInfo->nChangRateUp / 100;
		stUser_Define.Condition[1] = pstFilterInfo->nChangRateDown / 100;

		memcpy(&stUser_Finder.stUser_Define[nConditionIndex], &stUser_Define, SZ_User_Define);
	}

	//int nChangValChk[CNT_CHANGEVAL];

	int  nIndex;
	CTime time(CTime::GetCurrentTime());
	CString strTime = time.Format("%H");

	SetUser_Define_BaseFilter(stUser_Define);
	nConditionIndex++;
	stUser_Define.IndexNo = nConditionIndex; 				/* 지표 index				*/
	memcpy(&stUser_Define.Element, CONDITION_INDEX[nConditionIndex], 1);

	stUser_Define.Type = 2201;
	stUser_Define.Type1 = 5;

	BOOL bTimeChk;
	int nMaxVal, nMax_Time, nDealVol;
	nDealVol = pstFilterInfo->nDealVol * 1000;
	if (pstFilterInfo->nDealVolType == 0)		//# 0 : 고정, 1 : Time
	{
		stUser_Define.Condition[1] = nDealVol;
		stUser_Define.Condition[2] = 9000000000;
	}
	else if (pstFilterInfo->nDealVolType == 1)
	{
		bTimeChk = FALSE;
		nMaxVal = 0;
		nMax_Time = 0;

		POSITION	pos;
		SubFilterInfo	stSubFilterInfo;
		for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimeVol->GetCount(); nIndex++)
		{
			pos = pstFilterInfo->plistDealTimeVol->FindIndex(nIndex);
			stSubFilterInfo = pstFilterInfo->plistDealTimeVol->GetAt(pos);

			nDealVol = stSubFilterInfo.nData * 1000;
			if (strTime == stSubFilterInfo.nTime)
			{
				stUser_Define.Condition[1] = nDealVol;
				stUser_Define.Condition[2] = 9000000000;

				bTimeChk = TRUE;
				break;
			}

			if (nMax_Time < stSubFilterInfo.nTime)
			{
				nMax_Time = stSubFilterInfo.nTime;
				nMaxVal = nDealVol;
			}
		}

		if (bTimeChk == FALSE)
		{
			stUser_Define.Condition[1] = nMaxVal;
			stUser_Define.Condition[2] = 9000000000;
		}
	}
	memcpy(&stUser_Finder.stUser_Define[nConditionIndex], &stUser_Define, SZ_User_Define);

	SetUser_Define_BaseFilter(stUser_Define);
	nConditionIndex++;
	stUser_Define.IndexNo = nConditionIndex; 				/* 지표 index				*/
	memcpy(&stUser_Define.Element, CONDITION_INDEX[nConditionIndex], 1);

	stUser_Define.Type = 2401;
	stUser_Define.Type1 = 5;
	int nDealPrice = pstFilterInfo->nDealPrice * 100;
	if (pstFilterInfo->nDealPriceType == 0)		//# 0 : 고정, 1 : Time
	{
		stUser_Define.Condition[1] = nDealPrice;
		stUser_Define.Condition[2] = 9000000000;
	}
	else if (pstFilterInfo->nDealPriceType == 1)
	{
		bTimeChk = FALSE;
		nMaxVal = 0;
		nMax_Time = 0;

		POSITION	pos;
		SubFilterInfo	stSubFilterInfo;
		for (nIndex = 0; nIndex < pstFilterInfo->plistDealTimePrice->GetCount(); nIndex++)
		{
			pos = pstFilterInfo->plistDealTimePrice->FindIndex(nIndex);
			stSubFilterInfo = pstFilterInfo->plistDealTimePrice->GetAt(pos);

			nDealPrice = stSubFilterInfo.nData * 100;
			if (strTime == stSubFilterInfo.nTime)
			{
				stUser_Define.Condition[1] = nDealPrice;
				stUser_Define.Condition[2] = 9000000000;

				bTimeChk = TRUE;
				break;
			}

			if (nMax_Time < stSubFilterInfo.nTime)
			{
				nMax_Time = stSubFilterInfo.nTime;
				nMaxVal = nDealPrice;
			}
		}

		if (bTimeChk == FALSE)
		{
			stUser_Define.Condition[1] = nMaxVal;
			stUser_Define.Condition[2] = 9000000000;
		}
	}
	memcpy(&stUser_Finder.stUser_Define[nConditionIndex], &stUser_Define, SZ_User_Define);

	for (nIndex = 0; nIndex < CNT_DIRECTION; nIndex++)
	{
		if (pstFilterInfo->nDirectionChk[nIndex] == 1)
		{
			SetUser_Define_BaseFilter(stUser_Define);
			nConditionIndex++;
			stUser_Define.IndexNo = nConditionIndex; 				/* 지표 index				*/
			memcpy(&stUser_Define.Element, CONDITION_INDEX[nConditionIndex], 1);

			switch (nIndex)
			{
			case 0 :   //# 관리
				{
					stUser_Define.Type = 3011;
					stUser_Define.Condition[0] = 1;
				}
				break;
			case 1 : //# 이상급등
				{
					stUser_Define.Type = 3010;
					stUser_Define.Condition[0] = 1;
				}
				break;
			case 2 :  //# 거래정지
				{
					stUser_Define.Type = 3008;
					stUser_Define.Condition[0] = 1;
				}
				break;
			case 3 :  //# 정리매매
				{
					stUser_Define.Type = 3016;
					stUser_Define.Condition[0] = 1; 
				}
				break;
			case 4 :  //# 투자유의
				{
					stUser_Define.Type = 3013;
					stUser_Define.Condition[0] = 1;
				}
				break;
			case 5 :  //# 불성실 공시
				{
					stUser_Define.Type = 3014;
					stUser_Define.Condition[0] = 1; 
				}
				break;

			case 6 :  //# 우선주
				{
					stUser_Define.Type = 3004;
					stUser_Define.Condition[0] = 1; 
				}
				break;

			case 7 :  //# 증거금 100%
				{
					stUser_Define.Type = 3020;
					stUser_Define.Condition[0] = 4; 
				}
				break;
			}
			memcpy(&stUser_Finder.stUser_Define[nConditionIndex], &stUser_Define, SZ_User_Define);
		}
	}

	/*
	CStringArray saOutCode;
	*/

	stUser_Finder.TrIndex = 0;
	stUser_Finder.nTargetType = 0;									    
	stUser_Finder.TargetIdxCnt = nConditionIndex + 1;
	stUser_Finder.TargetResultViewCnt = 0;

	CString strElement;
	for (nIndex = 0; nIndex < nConditionIndex + 1; nIndex++)
	{
		if (nIndex == 0)
			strElement = CONDITION_INDEX[nIndex];
		else
		{
			CString strOpt;
			if (stUser_Finder.stUser_Define[nIndex].Operator == '1')
				strOpt = "&";
			else if (stUser_Finder.stUser_Define[nIndex].Operator == '0')
				strOpt = "|";
			strElement += strOpt + CONDITION_INDEX[nIndex];
		}
	}
	strElement += "!";
	memcpy(&stUser_Finder.TargetExp, strElement.GetBuffer(0), EXPLEN);

	stUser_Finder.ComplexIdxCnt = 0;                /* 복합 지표 갯수       */
	stUser_Finder.ComplexResultViewCnt = 0;         /* 복합 값보기필드 개수 */
	//stUser_Finder.ComplexExp[EXPLEN];           /* 복합조건 조합 표현 식*/

	stUser_Finder.FieldEditerCnt = 0;               /* 필드편집 개수   [Fix + Move] */
	//stUser_Finder.FieldList[REAL_IDXCNT];				/* 필드편집 번호리스트 [Just Fix + Move] */

	stUser_Finder.ResultIdxCnt = 0;									/* 결과값비교 개수         [NEW]*/
	//stUser_Finder.ResultExp[EXPLEN];						/* 결과값 비교조합 표현 식 [NEW]*/

	stUser_Finder.TargetBitChk = '0';                 // TargetBit 사용 여부 체크
	//stUser_Finder.TargetBit[RESULT_MAX];  /* 검색 대상 종목		*/

	stUser_Finder.Sort_flag = '0';                    /* 정렬 방향(0:오름, 1:내림)*/
	stUser_Finder.Sort_Num = 100;                     /* 정렬 번호(값보기필드pos) */

	stUser_Finder.BackDay = -1;											/* backtest 일자(미사용:-1) */	
	stUser_Finder.BackTime = -1;											/* backtest 시간(미사용:-1) */
}

BOOL _DLL_GetFilter(CString strFullPath, long *plFilterInfo)
{
	BOOL bResult = FALSE;

	bResult = theApp.GetLoad_FilterData(strFullPath, (FilterInfo *)plFilterInfo);
	if (bResult == FALSE)
	{
		theApp.SetDefult_FilterInfo((FilterInfo *)plFilterInfo);
		bResult = TRUE;
	}

	return bResult;
}

void _DLL_GetFilter_Base(long* plFilterInfo)
{
	theApp.SetDefult_FilterInfo((FilterInfo *)plFilterInfo);
}


BOOL CLib_ConditionMngApp::GetLoad_FilterData(CString strFullPath, FilterInfo *pstFilterInfo)
{
	CFile clsFile;
	if (clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeRead))
	{
		LPSTR lpBuff = NULL;
		DWORD nLen = clsFile.GetLength();
		if(nLen == 0) return FALSE;

		lpBuff = new char[nLen];
		memset(lpBuff, 0, nLen);
		clsFile.Read(lpBuff, nLen);
		clsFile.Close();

		//CMemFile memFile = CMemFile(nLen);//tour2k
		CMemFile memFile ;
		memFile.Attach((unsigned char*)lpBuff, nLen);

		CArchive ar(&memFile, CArchive::load);

		int nIndex, nCount;
		CString strData;

		ar >> pstFilterInfo->nMarketType;			//# 0 : 전체, 1 : 코스피, 2 : 코스닥

		ar >> pstFilterInfo->nNowPriceChk;
		ar >> pstFilterInfo->nNowPriceUp; 
		ar >> pstFilterInfo->nNowPriceDown;

		ar >> pstFilterInfo->nChangRateChk;
		ar >> pstFilterInfo->nChangRateUp; 
		ar >> pstFilterInfo->nChangRateDown;

		for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
			ar >> pstFilterInfo->nChangValChk[nIndex];

		ar >> pstFilterInfo->nDealVolType;		//# 0 : 고정, 1 : Time
		ar >> pstFilterInfo->nDealVol;

		ar >> nCount;
		SubFilterInfo	stSubFilterInfo;
		pstFilterInfo->plistDealTimeVol->RemoveAll(); //// 0001908: [0362] 빠른종목검색 - 필터의 시간대별거래량이 동작하지 않는 현상(초기화)
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			ar >> stSubFilterInfo.nTime;
			ar >> stSubFilterInfo.nData;
			pstFilterInfo->plistDealTimeVol->AddTail(stSubFilterInfo);
		}

		ar >> pstFilterInfo->nDealPriceType;		//# 0 : 고정, 1 : Time
		ar >> pstFilterInfo->nDealPrice;

		ar >> nCount;
		pstFilterInfo->plistDealTimePrice->RemoveAll();  //// 0001908: [0362] 빠른종목검색 - 필터의 시간대별거래량이 동작하지 않는 현상(초기화)
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			ar >> stSubFilterInfo.nTime;
			ar >> stSubFilterInfo.nData;
			pstFilterInfo->plistDealTimePrice->AddTail(stSubFilterInfo);
		}

		for (nIndex = 0; nIndex < (int)CNT_DIRECTION; nIndex++)
			ar >> pstFilterInfo->nDirectionChk[nIndex];

		ar >> nCount;
		pstFilterInfo->saOutCode.RemoveAll();	  //// 0001908: [0362] 빠른종목검색 - 필터의 시간대별거래량이 동작하지 않는 현상(초기화)
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			ar >> strData;
			pstFilterInfo->saOutCode.Add(strData);
		}

		ar.Close();
		if (lpBuff) 
			delete lpBuff;
		return TRUE;
	}

	return FALSE;
}

void CLib_ConditionMngApp::SetDefult_FilterInfo(FilterInfo *pstFilterInfo)
{
	int nIndex;

	pstFilterInfo->nMarketType = 0;			//# 0 : 전체, 1 : 코스피, 2 : 코스닥

	pstFilterInfo->nNowPriceChk = 0;
	pstFilterInfo->nNowPriceUp = 5000; 
	pstFilterInfo->nNowPriceDown = 10000; 

	pstFilterInfo->nChangRateChk = 0; 
	pstFilterInfo->nChangRateUp = 0; 
	pstFilterInfo->nChangRateDown = 500; 

	for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		pstFilterInfo->nChangValChk[nIndex] = 1;

	pstFilterInfo->nDealVolType = 0; ;		//# 0 : 고정, 1 : Time
	pstFilterInfo->nDealVol = 5;
	pstFilterInfo->plistDealTimeVol->RemoveAll();

	pstFilterInfo->nDealPriceType = 0;		//# 0 : 고정, 1 : Time
	pstFilterInfo->nDealPrice = 0;
	pstFilterInfo->plistDealTimePrice->RemoveAll();

	for (nIndex = 0; nIndex < (int)CNT_DIRECTION; nIndex++)
		pstFilterInfo->nDirectionChk[nIndex] = 0;

	pstFilterInfo->saOutCode.RemoveAll();
}

BOOL _DLL_SetFilter(CString strFullPath, long *plFilterInfo)
{
	BOOL bResult = FALSE;

	bResult = theApp.SetSave_FilterData(strFullPath, (FilterInfo *)plFilterInfo);

	return bResult;
}

BOOL CLib_ConditionMngApp::SetSave_FilterData(CString strFullPath, FilterInfo *pstFilterInfo)
{
	CFile clsFile;
	if(!clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite)) 
		return FALSE;
	CArchive ar(&clsFile, CArchive::store);

	int nIndex, nCount;
	CString strData;

	ar << pstFilterInfo->nMarketType;			//# 0 : 전체, 1 : 코스피, 2 : 코스닥

	ar << pstFilterInfo->nNowPriceChk;
	ar << pstFilterInfo->nNowPriceUp;
	ar << pstFilterInfo->nNowPriceDown;

	ar << pstFilterInfo->nChangRateChk;
	ar << pstFilterInfo->nChangRateUp; 
	ar << pstFilterInfo->nChangRateDown;

	for (nIndex = 0; nIndex < (int)CNT_CHANGEVAL; nIndex++)
		ar << pstFilterInfo->nChangValChk[nIndex];

	ar << pstFilterInfo->nDealVolType;		//# 0 : 고정, 1 : Time
	ar << pstFilterInfo->nDealVol;

	nCount = pstFilterInfo->plistDealTimeVol->GetCount();

	ar << nCount;
	POSITION pos;
	SubFilterInfo	stSubFilterInfo;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pstFilterInfo->plistDealTimeVol->FindIndex(nIndex);
		stSubFilterInfo = pstFilterInfo->plistDealTimeVol->GetAt(pos);

		ar << stSubFilterInfo.nTime;
		ar << stSubFilterInfo.nData;
	}

	ar << pstFilterInfo->nDealPriceType;		//# 0 : 고정, 1 : Time
	ar << pstFilterInfo->nDealPrice;

	nCount = pstFilterInfo->plistDealTimePrice->GetCount();
	ar << nCount;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pstFilterInfo->plistDealTimePrice->FindIndex(nIndex);
		stSubFilterInfo = pstFilterInfo->plistDealTimePrice->GetAt(pos);

		ar << stSubFilterInfo.nTime;
		ar << stSubFilterInfo.nData;
	}

	for (nIndex = 0; nIndex < (int)CNT_DIRECTION; nIndex++)
		ar << pstFilterInfo->nDirectionChk[nIndex];

	nCount = pstFilterInfo->saOutCode.GetSize();
	ar << nCount;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		strData = pstFilterInfo->saOutCode.GetAt(nIndex);
		ar << strData;
	}

	ar.Close();
	clsFile.Close();
	return TRUE;
}

void CLib_ConditionMngApp::GetLoad_ConditionInfo()
{
	SetJobFolder();

	m_bLoadFile_User = SetLoadTreeXml("USER");
	m_bLoadFile_Sys = SetLoadTreeXml("SYS");
}

void CLib_ConditionMngApp::SetDeleteFolderList(CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	if (pFolderList->GetCount() <= 0)
		return;

	POSITION posFolder;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	for (int nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);

		if (stXMLTreeInfoFolder.plistXMLTreeInfoFile != NULL)
		{
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
			delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
			stXMLTreeInfoFolder.plistXMLTreeInfoFile = NULL;
		}
	}
	pFolderList->RemoveAll();
}

void CLib_ConditionMngApp::SetJobFolder()
{
	CString strPath = GetAppFileRoot();

	CFileFind finder;
	CString strUserFolder = GetFolderPath("USER");
	CString strFindFolder;
	for (int nIndex = strUserFolder.GetLength() - 1; nIndex > 0; nIndex--)
	{
		if (strUserFolder[nIndex] == '\\')
		{
			strFindFolder = strUserFolder.Mid(0, nIndex);
			break;
		}
	}      

	BOOL bFoldMake = TRUE;
	BOOL bFoldMake_Temp = TRUE;
	BOOL bWorking = finder.FindFile(strFindFolder + "\\*.*");
	CString strGetPath = _T("");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			strGetPath = finder.GetFilePath();
			if (strGetPath == strUserFolder)
				bFoldMake = FALSE;

			strGetPath = finder.GetFilePath();
			if (strGetPath == strUserFolder + "\\" + (CString)FOLDER_TEMPFILE)
			{
				bFoldMake_Temp = FALSE;
				break;
			}
		}
		continue;
	}  

	if (bFoldMake == TRUE)
	{
		CreateDirectory(strFindFolder, 0);
		CreateDirectory(strUserFolder, 0);
	}

	if (bFoldMake_Temp == TRUE)
		CreateDirectory(strUserFolder + "\\" + (CString)FOLDER_TEMPFILE, 0);
}

BOOL CLib_ConditionMngApp::SetLoadTreeXml(CString strType)
{	
	CString strFilePath;
	strFilePath = GetFilePath(strType);

	char msg[500];
	CString strDebug;
	//	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);


	MSXML::IXMLDOMDocumentPtr pXmlDom = NULL;    
	CoInitialize(NULL); //< COM 연결

	pXmlDom.CreateInstance(__uuidof(MSXML::DOMDocument));     //< 인스턴스 생성 

	CString strErrorMsg;
	pXmlDom->async = false;
	if (!pXmlDom->load (strFilePath.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);

		if (strType == "SYS")
		{
			CString strTemp(BSTR(pParsingErr->reason));	
			strDebug = strTemp + CString(msg) + "\n" + strFilePath;

			strErrorMsg.Format("[%s] Tree List File이 존재하지 않습니다.", strFilePath);
	
			SetSendMainMsg(strErrorMsg);
			return FALSE;
		}
		else
			SetSaveTreeXml("USER", &m_listUserInfoFolder);
	}

	if (strType == "USER")
		SyncTreeListFile(strFilePath);
	else if (strType == "SYS")
	{
		if (GetXMLToList(strFilePath, strType, &m_listSysInfoFolder) == FALSE)
		{
			strErrorMsg.Format("%s Tree List File Load 실패!", strType);
			SetSendMainMsg(strErrorMsg);
		}
	}

	return TRUE;
}

void CLib_ConditionMngApp::SyncTreeListFile(CString strFilePath)
{
	CString strErrorMsg;
	if (GetXMLToList(strFilePath, "USER", &m_listUserInfoFolder) == FALSE)
	{
		strErrorMsg.Format("[%s] Tree List File이 존재하지 않습니다.", strFilePath);
		SetSendMainMsg(strErrorMsg);
	}

	//# Get Real File list #
	CList<XMLTreeInfoFolder, XMLTreeInfoFolder&>    listFolderList;
	CString strFolderPath = GetFolderPath("USER");
	GetPathToList(strFolderPath, &listFolderList);

	ProcessDeleteFolder(&m_listUserInfoFolder, &listFolderList);
	ProcessInsertFolder(&listFolderList, &m_listUserInfoFolder);

	SetDeleteFolderList(&listFolderList);
	listFolderList.RemoveAll();

	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	POSITION posFolder;
	for (int nFolderIndex = 0; nFolderIndex < m_listUserInfoFolder.GetCount(); nFolderIndex++)
	{
		posFolder = m_listUserInfoFolder.FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_listUserInfoFolder.GetAt(posFolder);

		//# File Count ReSet!
		if (stXMLTreeInfoFolder.plistXMLTreeInfoFile == NULL)
			stXMLTreeInfoFolder.nFileCnt = 0;
		else
			stXMLTreeInfoFolder.nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		m_listUserInfoFolder.SetAt(posFolder, stXMLTreeInfoFolder);
	}

	SetSaveTreeXml("USER", &m_listUserInfoFolder);
}

void CLib_ConditionMngApp::GetPathToList(CString strRootFolderPath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	XMLTreeInfoFile		stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;

	CFileFind			ffDirectory;
	CFileFind			ffFile;

	BOOL	bFindFolder, bFindFile;
	CString strFileName, strFileTitle, strTemp;
	CString strFolderPath, strFolderName;
	BOOL bLock;
	int nFileCnt;

	bFindFolder = ffDirectory.FindFile(strRootFolderPath + "\\" + "*.*");
	while (bFindFolder)
	{
		bFindFolder = ffDirectory.FindNextFile();

		strFolderPath = ffDirectory.GetFileName();
		strFolderName = ffDirectory.GetFileTitle();
		if (ffDirectory.IsDirectory() && strFolderPath != "." && strFolderPath != "..")
		{
			bLock = (strFolderPath.CompareNoCase((CString)FOLDER_TEMPFILE) == 0)?TRUE:FALSE;
			nFileCnt = 0;

			stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
			bFindFile = ffFile.FindFile(strRootFolderPath + "\\" + strFolderPath + "\\" + "*.*");
			while (bFindFile)
			{
				bFindFile = ffFile.FindNextFile();
				strFileName = ffFile.GetFileName();

				if (!ffFile.IsDirectory())
				{
					strFileTitle = ffFile.GetFileTitle();
					strTemp = strFileName;
					strTemp.Replace(strFileTitle, "");
					strTemp.MakeUpper();
					if (strTemp.CompareNoCase(".XML") == 0 )
					{
						stXMLTreeInfoFile.strFolderPath = strFolderPath;
						stXMLTreeInfoFile.strFilePath = strFileName;
						stXMLTreeInfoFile.strFileName = strFileTitle;

						GetFileSetState("USER", strFolderPath, strFileName, 
							stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);

						stXMLTreeInfoFile.bQuickReg = FALSE;

						stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);

						nFileCnt++;
					}
				}
			}

			stXMLTreeInfoFolder.bLock = bLock;
			stXMLTreeInfoFolder.strName = strFolderName;
			stXMLTreeInfoFolder.strPath = strFolderPath;
			stXMLTreeInfoFolder.nFileCnt = nFileCnt;

			pFolderList->AddTail(stXMLTreeInfoFolder);
		}
	}
}

void CLib_ConditionMngApp::GetTempCondition(CString strRootFolderPath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	XMLTreeInfoFile		stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	CFileFind			ffFile;
	CString strFileName, strFileTitle, strTemp;
	CString strFolderPath, strFolderName;

	stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
	strFolderPath = strRootFolderPath + "\\temp.xml";
	BOOL bFindFile = ffFile.FindFile(strFolderPath);
	//	BOOL bFindFile = ffFile.FindFile(strRootFolderPath + "\\" + strFolderPath + "\\temp.xml");
	if(bFindFile)
	{
		ffFile.FindNextFile();
		strFileName = ffFile.GetFileName();

		stXMLTreeInfoFile.strFolderPath = strFolderPath;
		stXMLTreeInfoFile.strFilePath = "temp.xml";
		stXMLTreeInfoFile.strFileName = "실시간검색";

		GetFileSetState("TEMP", strFolderPath, "temp.xml", stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);

		stXMLTreeInfoFile.bQuickReg = FALSE;
		stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);

		stXMLTreeInfoFolder.bLock = TRUE;
		stXMLTreeInfoFolder.strName = strFolderName;
		stXMLTreeInfoFolder.strPath = strFolderPath;
		stXMLTreeInfoFolder.nFileCnt = 1;

		pFolderList->AddTail(stXMLTreeInfoFolder);

	}
	ffFile.Close();
}

BOOL CLib_ConditionMngApp::GetXMLToList(CString strFilePath,	CString strType,
										CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;

	MSXML::IXMLDOMNodePtr					pNIndex;

	MSXML::IXMLDOMNodeListPtr			pNSFolder;
	MSXML::IXMLDOMNodePtr					pNFolder;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFolder;

	MSXML::IXMLDOMNodeListPtr			pNSCondition;
	MSXML::IXMLDOMNodePtr					pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;

	BOOL bResult = true;
	VARIANT_BOOL bLoadedXMLFile;

	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strFilePath));

	// XML 파일을 로드하는데 실패하였다면
	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}

	try {
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		pNSFolder = pNIndex->selectNodes("FOLDER");

		CString strFolderName, strFolderPath, strLock;
		int nLength_Folder = pNSFolder->length;
		for (int nFolderIndex = 0; nFolderIndex < nLength_Folder; nFolderIndex++)
		{
			pNFolder = pNSFolder->item[nFolderIndex];
			pNAFolder = pNFolder->attributes;

			MSXML::IXMLDOMNodePtr	pNAttributeTemp;

			pNAttributeTemp = pNAFolder->getNamedItem("NAME");
			stXMLTreeInfoFolder.strName = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNAFolder->getNamedItem("PATH");
			stXMLTreeInfoFolder.strPath = CString((PCHAR)pNAttributeTemp->text);

			pNAttributeTemp = pNAFolder->getNamedItem("LOCK");
			stXMLTreeInfoFolder.bLock = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;

			pNSCondition = pNFolder->selectNodes("CONDITION");
			int nFileCnt = pNSCondition->length;
			stXMLTreeInfoFolder.nFileCnt = nFileCnt;

			if (nFileCnt == 0)
				stXMLTreeInfoFolder.plistXMLTreeInfoFile = NULL;
			else
				stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;

			for (int nFileIndex = 0; nFileIndex < nFileCnt; nFileIndex++)
			{
				pNCondition = pNSCondition->item[nFileIndex];

				// Folder에 아이템 생성하기 - Start
				MSXML::IXMLDOMNodePtr	pNAttributeTemp;
				pNACondition = pNCondition->attributes;

				stXMLTreeInfoFile.strFolderPath = stXMLTreeInfoFolder.strPath;

				pNAttributeTemp = pNACondition->getNamedItem("NAME");
				stXMLTreeInfoFile.strFileName = CString((PCHAR)pNAttributeTemp->text);

				pNAttributeTemp = pNACondition->getNamedItem("PATH");
				stXMLTreeInfoFile.strFilePath = CString((PCHAR)pNAttributeTemp->text);

				pNAttributeTemp = pNACondition->getNamedItem("ALERTREG");
				//stXMLTreeInfoFile.bAlertReg = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;
				stXMLTreeInfoFile.strAlertReg = CString((PCHAR)pNAttributeTemp->text);

				pNAttributeTemp = pNACondition->getNamedItem("UNITCAN");
				stXMLTreeInfoFile.bUnitCan = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;

				pNAttributeTemp = pNACondition->getNamedItem("QUICKREG");
				stXMLTreeInfoFile.bQuickReg = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;

				stXMLTreeInfoFile.strType = strType;
				/*
				if (strType == "SYS")
				{
				pNAttributeTemp = pNACondition->getNamedItem("EXPLANATION");
				stXMLTreeInfoFile.strExplain = CString((PCHAR)pNAttributeTemp->text);
				}
				else
				{
				*/
				pNAttributeTemp = pNACondition->getNamedItem("EXPLANATION");
				if (pNAttributeTemp == NULL)
					stXMLTreeInfoFile.strExplain = "";
				else
					stXMLTreeInfoFile.strExplain = CString((PCHAR)pNAttributeTemp->text);
				//}

				stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
			}

			pFolderList->AddTail(stXMLTreeInfoFolder);
		}
	} 
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

void CLib_ConditionMngApp::ProcessDeleteFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
											   CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	XMLTreeInfoFolder	stSourceInfoFolder, stDestInfoFolder;

	POSITION posFolder;
	int nFolderCnt = pSourceList->GetCount();

	int nFolderIndexPos = 0;
	for (int nFolderIndex = 0; nFolderIndex < nFolderCnt; nFolderIndex++)
	{
		posFolder = pSourceList->FindIndex(nFolderIndexPos);
		stXMLTreeInfoFolder = pSourceList->GetAt(posFolder);

		if (GetFindFolder(stXMLTreeInfoFolder.strPath, pDestList) == FALSE)
		{
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
			delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
			pSourceList->RemoveAt(posFolder);
		}
		else
		{
			posFolder = GetPosFolder(pSourceList, stXMLTreeInfoFolder.strPath);
			stSourceInfoFolder = pSourceList->GetAt(posFolder);

			posFolder = GetPosFolder(pDestList, stXMLTreeInfoFolder.strPath);
			stDestInfoFolder = pDestList->GetAt(posFolder);

			if ((stSourceInfoFolder.plistXMLTreeInfoFile == NULL) ||
				(stDestInfoFolder.plistXMLTreeInfoFile == NULL))
			{

			}
			else
				ProcessDeleteFile(stSourceInfoFolder.plistXMLTreeInfoFile,
				stDestInfoFolder.plistXMLTreeInfoFile);

			nFolderIndexPos++;
		}
	}
}

void CLib_ConditionMngApp::ProcessInsertFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
											   CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList)
{
	POSITION posFolder;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	XMLTreeInfoFolder	stSourceInfoFolder, stDestInfoFolder;

	for (int nFolderIndex = 0; nFolderIndex < pSourceList->GetCount(); nFolderIndex++)
	{
		posFolder = pSourceList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pSourceList->GetAt(posFolder);
		if (GetFindFolder(stXMLTreeInfoFolder.strPath, pDestList) == FALSE)
		{
			XMLTreeInfoFolder		stAddInfoFolder;

			stAddInfoFolder.bLock = stXMLTreeInfoFolder.bLock;
			stAddInfoFolder.strName = stXMLTreeInfoFolder.strName;
			stAddInfoFolder.strPath = stXMLTreeInfoFolder.strPath;

			stAddInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;

			POSITION posFile;
			XMLTreeInfoFile	stXMLTreeInfoFile;
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);

				stAddInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
			}
			stAddInfoFolder.nFileCnt = stAddInfoFolder.plistXMLTreeInfoFile->GetCount();
			pDestList->AddTail(stAddInfoFolder);
		}
		else
		{
			posFolder = GetPosFolder(pSourceList, stXMLTreeInfoFolder.strPath);
			stSourceInfoFolder = pSourceList->GetAt(posFolder);

			posFolder = GetPosFolder(pDestList, stXMLTreeInfoFolder.strPath);
			stDestInfoFolder = pDestList->GetAt(posFolder);

			if ((stSourceInfoFolder.plistXMLTreeInfoFile == NULL) ||
				(stDestInfoFolder.plistXMLTreeInfoFile == NULL))
			{

			}
			else
				ProcessInsertFile(stSourceInfoFolder.plistXMLTreeInfoFile, stDestInfoFolder.plistXMLTreeInfoFile);
		}
	}
}

void CLib_ConditionMngApp::ProcessDeleteFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, 
											 CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList)
{
	if ((pSourceList == NULL) || (pDestList == NULL))
		return;

	POSITION pos;
	XMLTreeInfoFile	stXMLTreeInfoFile;

	int nCount = pSourceList->GetCount();
	int nFileIndexPos = 0;
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pSourceList->FindIndex(nFileIndexPos);
		stXMLTreeInfoFile = pSourceList->GetAt(pos);

		if (GetFindFile(stXMLTreeInfoFile.strFilePath, pDestList) == FALSE)
			pSourceList->RemoveAt(pos);
		else
			nFileIndexPos++;
	}

	/*
	pos = pSourceList->GetHeadPosition();
	while (pos != NULL)
	{
	stXMLTreeInfoFile = pSourceList->GetNext(pos);

	if (GetFindFile(stXMLTreeInfoFile.strFilePath, pDestList) == FALSE)
	{
	//pos = GetPosFile(pSourceList, stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath);
	pSourceList->RemoveAt(pos);	
	}
	}
	*/
}

void CLib_ConditionMngApp::ProcessInsertFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, 
											 CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList)
{
	if ((pSourceList == NULL) || (pDestList == NULL))
		return;

	POSITION pos;
	XMLTreeInfoFile	stXMLTreeInfoFile;

	for (int nIndex = 0; nIndex < pSourceList->GetCount(); nIndex++)
	{
		pos = pSourceList->FindIndex(nIndex);
		stXMLTreeInfoFile = pSourceList->GetAt(pos);

		if (GetFindFile(stXMLTreeInfoFile.strFilePath, pDestList) == FALSE)
		{
			GetFileSetState("USER", stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath, 
				stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);

			stXMLTreeInfoFile.bQuickReg = FALSE;

			pDestList->AddTail(stXMLTreeInfoFile);
		}
	}
}


BOOL CLib_ConditionMngApp::GetFindFile(CString strFilePath,
									   CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList)
{
	BOOL bResult = FALSE;

	POSITION pos;
	XMLTreeInfoFile      stXMLTreeInfoFile;
	for (int nIndex = 0; nIndex < pFileList->GetCount(); nIndex++)
	{
		pos = pFileList->FindIndex(nIndex);
		stXMLTreeInfoFile = pFileList->GetAt(pos);

		if (strFilePath == stXMLTreeInfoFile.strFilePath)
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

BOOL CLib_ConditionMngApp::GetFindFile(CString strFolderPath, CString strFilePath,
									   CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = FALSE;

	if (strFolderPath.IsEmpty() || strFilePath.IsEmpty())
		return bResult;

	POSITION pos;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;
	for (int nIndex = 0; nIndex < pFolderList->GetCount(); nIndex++)
	{
		pos = pFolderList->FindIndex(nIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(pos);

		if (strFolderPath == stXMLTreeInfoFolder.strPath)
			bResult = GetFindFile(strFilePath, stXMLTreeInfoFolder.plistXMLTreeInfoFile);

		if (bResult == TRUE)
			break;
	}

	return bResult;
}

BOOL CLib_ConditionMngApp::GetFindFolder(CString strFolderPath, 
										 CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = FALSE;

	if (strFolderPath.IsEmpty())
		return bResult;

	POSITION pos;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;
	for (int nIndex = 0; nIndex < pFolderList->GetCount(); nIndex++)
	{
		pos = pFolderList->FindIndex(nIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(pos);

		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			bResult = TRUE;
			break;
		}
	}

	/*
	pos = pFolderList->GetHeadPosition();
	while(pos != NULL)
	{
	stXMLTreeInfoFolder = pFolderList->GetNext(pos);
	if (strFolderPath == stXMLTreeInfoFolder.strPath)
	{
	bResult = TRUE;
	break;
	}
	}
	*/

	return bResult;
}

BOOL CLib_ConditionMngApp::SetSaveTreeXml(CString strType,
										  CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = TRUE;

	CString strFilePath;
	strFilePath = GetFilePath(strType);

	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;

	MSXML::IXMLDOMNodePtr						pNFolder;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAFolder;

	MSXML::IXMLDOMNodePtr						pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr		pNACondition;

	try 
	{
		POSITION pos;
		XMLTreeInfoFile stFileInfo;
		XMLTreeInfoFolder stFolderInfo;

		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

		MSXML::IXMLDOMProcessingInstructionPtr		pPI;
		MSXML::IXMLDOMNamedNodeMapPtr						pNAPI;
		MSXML::IXMLDOMNodePtr										pNEncoding;

		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");

		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";

		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		pDOMIndex->appendChild(pPI);

		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;

		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)XML_VERSION_USERFILE;
		pNAIndex->setNamedItem(pNAttributeTemp);

		pDOMIndex->appendChild(pNIndex);
		int nFolderCnt = pFolderList->GetCount();
		for (int nFolderIndex = 0; nFolderIndex < nFolderCnt; nFolderIndex++)
		{
			pos = pFolderList->FindIndex(nFolderIndex);
			stFolderInfo = pFolderList->GetAt(pos);

			MSXML::IXMLDOMNodePtr pNAttributeTemp;			
			pNFolder = pDOMIndex->createElement("FOLDER");
			pNAFolder = pNFolder->attributes;

			pNAttributeTemp = pDOMIndex->createAttribute("NAME");
			pNAttributeTemp->text = (LPCTSTR)stFolderInfo.strName;
			pNAFolder->setNamedItem(pNAttributeTemp);

			pNAttributeTemp = pDOMIndex->createAttribute("PATH");
			pNAttributeTemp->text = (LPCTSTR)stFolderInfo.strPath;
			pNAFolder->setNamedItem(pNAttributeTemp);

			pNAttributeTemp = pDOMIndex->createAttribute("LOCK");
			if (stFolderInfo.strName == (CString)FOLDER_TEMPFILE)
				pNAttributeTemp->text = "TRUE";
			else
				pNAttributeTemp->text = (LPCTSTR)(stFolderInfo.bLock == TRUE)?"TRUE":"FALSE";
			pNAFolder->setNamedItem(pNAttributeTemp);

			pNIndex->appendChild(pNFolder);

			if ((stFolderInfo.plistXMLTreeInfoFile == NULL) || (stFolderInfo.nFileCnt == 0))
				continue;

			int nFileCnt = stFolderInfo.plistXMLTreeInfoFile->GetCount();
			for(int nFileIndex = 0; nFileIndex < nFileCnt; nFileIndex++)
			{
				pos = stFolderInfo.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stFileInfo = stFolderInfo.plistXMLTreeInfoFile->GetAt(pos);

				if (stFolderInfo.strPath == stFileInfo.strFolderPath)
				{
					MSXML::IXMLDOMNodePtr pNAttributeTemp;

					pNCondition = pDOMIndex->createElement("CONDITION");
					pNFolder->appendChild( pNCondition );
					pNACondition = pNCondition->attributes;

					pNAttributeTemp = pDOMIndex->createAttribute("NAME");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strFileName;
					pNACondition->setNamedItem(pNAttributeTemp);

					pNAttributeTemp = pDOMIndex->createAttribute("PATH");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strFilePath;
					pNACondition->setNamedItem(pNAttributeTemp);

					pNAttributeTemp = pDOMIndex->createAttribute("ALERTREG");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strAlertReg;
					pNACondition->setNamedItem(pNAttributeTemp);

					pNAttributeTemp = pDOMIndex->createAttribute("UNITCAN");
					pNAttributeTemp->text = (LPCTSTR)(stFileInfo.bUnitCan == TRUE) ? "TRUE" : "FALSE";
					pNACondition->setNamedItem(pNAttributeTemp);

					pNAttributeTemp = pDOMIndex->createAttribute("QUICKREG");
					pNAttributeTemp->text = (LPCTSTR)(stFileInfo.bQuickReg == TRUE) ? "TRUE" : "FALSE";
					pNACondition->setNamedItem(pNAttributeTemp);

					pNAttributeTemp = pDOMIndex->createAttribute("EXPLANATION");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strExplain;
					pNACondition->setNamedItem(pNAttributeTemp);

					/*
					//# 2005.08.29 User == Sys format!
					if (strType == "SYS")
					{
					pNAttributeTemp = pDOMIndex->createAttribute("EXPLANATION");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strExplain;
					pNACondition->setNamedItem(pNAttributeTemp);
					}
					*/
				}
			}
		}

		pDOMIndex->save(_bstr_t(strFilePath));
	}
	catch(_com_error e) 
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CLib_ConditionMngApp::GetFileSetState(CString strType, CString strFolderPath, CString strFilePath,
										   CString &strAlertReg, BOOL &bUnitCan)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNodePtr			    pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;

	CString strPath;
	strPath = GetFolderPath(strType);
	if(strType != "TEMP")
		strPath = strPath + "\\" + strFolderPath + "\\" + strFilePath;
	else
		strPath = strFolderPath;

	try {
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		pDOMConditionIndex->async = false;
		pDOMConditionIndex->load(_bstr_t(strPath));

		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		pNAConditions = pNConditions->attributes;
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;

		pNAttributeTemp = pNAConditions->getNamedItem("ALERT");
		strAlertReg = (atoi((PCHAR)pNAttributeTemp->text) == 1)?"TRUE":"FALSE";

		pNAttributeTemp = pNAConditions->getNamedItem("GROUP_CNT");
		bUnitCan = (atoi((PCHAR)pNAttributeTemp->text) == 1)?TRUE:FALSE;
	} 
	catch (_com_error e)
	{
		bResult = FALSE;

		strAlertReg = "";
		bUnitCan = FALSE;
	}

	return bResult;
}


POSITION CLib_ConditionMngApp::GetPosFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, 
											CString strFolderPath)
{
	POSITION posResult = NULL;

	XMLTreeInfoFolder	stXMLTreeInfoFolder;

	POSITION posFolder;
	int nFolderIndex;
	for (nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);

		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			posResult = posFolder;
			break;
		}
	}

	return posResult;
}

POSITION CLib_ConditionMngApp::GetPosFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList,
										  CString strFolderPath, CString strFilePath)
{
	POSITION posResult = NULL;

	XMLTreeInfoFile	stXMLTreeInfoFile;
	POSITION posFile;
	for (int nFileIndex = 0; nFileIndex < pFileList->GetCount(); nFileIndex++)
	{
		posFile = pFileList->FindIndex(nFileIndex);
		stXMLTreeInfoFile = pFileList->GetAt(posFile);

		if ((strFolderPath == stXMLTreeInfoFile.strFolderPath) &&
			(strFilePath == stXMLTreeInfoFile.strFilePath))
		{
			posResult = posFile;
			break;
		}
	}

	return posResult;
}


void CLib_ConditionMngApp::SetSendMainMsg(CString strMsg)
{
	TRACE(strMsg);
}

CString CLib_ConditionMngApp::GetFilePath(CString strLoadType, BOOL bIndustry)
{
	CString strFile, strFolder;
	CString strFilePath = "";

	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;

	if (strLoadType == "USER") //# User
	{
		strFile = (CString)TREE_USER_FILE;
		bUserFolder = TRUE;
	}
	else if (strLoadType == "SYS") //# Recommend
		strFile = (CString)TREE_RECOMMEND_FILE;
	else if (strLoadType == "OTHER")
		strFile = (CString)FOLDER_OTHERFILE + "\\" + (CString)CONDITIONLIST_FILE;
	else if(strLoadType == "SUDDEN")
		strFile = (CString)FOLDER_OTHERFILE + "\\" + (CString)CONDITIONLIST_SUDDENFILE;
	else if (strLoadType == "FIX_USER")
		strFile = (CString)FIXFIELD_USER_FILE;
	else if (strLoadType == "FIX_SYS")
		strFile = (CString)FIXFIELD_SYS_FILE;
	else if (strLoadType == "MOVE_USER")
		strFile = (CString)MOVEFIELD_USER_FILE;
	else if (strLoadType == "MOVE_SYS")
		strFile = (CString)MOVEFIELD_SYS_FILE;
	else if (strLoadType == "CNADLE")
	{
		strFile = (CString)CANDLE_INDEX_FILE;
		bSetIndustry = FALSE;
	}
	else if (strLoadType == "FINDERINFO")
		strFile = (CString)FINDERINFO_INIFILE;
	else if (strLoadType == "TEMP")
		strFile = "sys\\" + (CString)TREE_TEMP_FILE;
	else if(strLoadType == "REAL_OTHER")				// 2009.10.26 -hidden-	
		strFile = "sys\\" + (CString)CONDITIONLIST_REAL_FILE;

	if ((bIndustry == TRUE) && (bSetIndustry == TRUE))
		strFile.Replace(".", "_Industry.");

	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath = m_strUserFolder;

		strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFile;
	}
	else
	{
		CString strAppPath;
		strAppPath = GetAppFileRoot();
		strFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
	}

	return strFilePath;
}

CString CLib_ConditionMngApp::GetFolderPath(CString strLoadType, BOOL bIndustry)
{
	CString strFolderPath = "", strFolder;

	BOOL bUserFolder = FALSE;
	if (strLoadType == "SYS")			//# 2 : Recommend
	{
		strFolder = (CString)FOLDER_SYSFILE; 
		if (bIndustry == TRUE)
			strFolder += "_Industry";
	}
	else if (strLoadType == "USER")		//# 3 : user
	{
		strFolder = (CString)FOLDER_USERFILE;
		if (bIndustry == TRUE)
			strFolder += "_Industry";

		bUserFolder = TRUE;
	}
	else if (strLoadType == "LANGUAGE")		//# 4 : Language
		strFolder = (CString)FOLDER_LANGUAGEFILE;
	else if (strLoadType == "TEMP")
		strFolder = (CString)FOLDER_SYSFILE; 

	if (bUserFolder == TRUE && strLoadType != "TEMP")
	{
		CString strUserPath;
		strUserPath = m_strUserFolder;
		
		strFolderPath = (CString)strUserPath + strFolder;
	}
	else
	{
		CString strAppPath;
		strAppPath = GetAppFileRoot();
		strFolderPath = strAppPath /*+ "\\" */+ strFolder;
		//	strFolderPath = strAppPath + "\\" + strFolder;
	}

	return strFolderPath;
}


//# Logic Calculate! #####################################################
BOOL _DLL_SetFree_PtrArray(CPtrArray *paData)
{
	if (theApp.m_pLogicCalculate == NULL)
		return FALSE;

	theApp.m_pLogicCalculate->SetFree_PtrArray(paData);
	return TRUE;
}

BOOL _DLL_GetLogicCalculate(char *psData, CPtrArray *paBitSet, char **psResult)
{
	if (theApp.m_pLogicCalculate == NULL)
		return FALSE;

	return theApp.m_pLogicCalculate->GetLogicCalculate(psData, paBitSet, psResult);
}

void _DLL_SetUserFolderPath(CString strUserFolder)
{
	if(!strUserFolder.IsEmpty())
		theApp.SetUserFolder(strUserFolder);
}

void CLib_ConditionMngApp::GetGwansimItem(CString strKey, CStringArray& arrCode, CStringArray& arrName)
{
	CString strData, strList;

	
	/*strList = m_MainAPI.GetIntrItemList(atoi(strKey));*/
	

	int nFind = strList.Find(_T(","));
	while (nFind > 0)
	{
		strData = strList.Left(nFind);
		strList = strList.Mid(nFind+1);
		nFind = strList.Find(_T(","));

		arrCode.Add(strData);
	}

	if (strList.GetLength() > 0)
		arrCode.Add(strList);
}

CString CLib_ConditionMngApp::GetGwanKeyFromName(LPCSTR szGwanGroupName)
{
	static CString strKey;
	int nIndex = -1;
	
	/*nIndex = m_intrMngr.GetGroupIndex(szGwanGroupName);*/
	

	if (nIndex >= 0)
		strKey.Format("%d", nIndex);
	else
		return "";

	return strKey;

}

void CLib_ConditionMngApp::SetUserFolder(CString strUserFolder)
{
	if(m_strUserFolder == strUserFolder || strUserFolder.IsEmpty())
		return;
	
	m_strUserFolder = strUserFolder;
	::SetEnvironmentVariable(KEY_USERDIRECTORY, m_strUserFolder);
	
	GetLoad_ConditionInfo();
 	GetLoad_FieldInfo();	
}
