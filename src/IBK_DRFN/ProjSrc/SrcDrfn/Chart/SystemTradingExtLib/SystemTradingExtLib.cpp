/ SystemTradingExtLib.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SystemTradingExtLib.h"


#include "../SystemTrading/LoadHelper.h"
#include "../Common_ST/STConfigDef.h"

#include "../Common_ST/STSiteDef.h"	//'@', "@" 정의

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
// CSystemTradingExtLibApp

BEGIN_MESSAGE_MAP(CSystemTradingExtLibApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTradingExtLibApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingExtLibApp construction

CSystemTradingExtLibApp::CSystemTradingExtLibApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_bInitLib = FALSE;
	::memset(m_szRootPath, 0x00, sizeof(m_szRootPath));

	m_listScriptInfo.RemoveAll();

	m_pLoadPaser = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTradingExtLibApp object

CSystemTradingExtLibApp theApp;

void CallNotifyFunction(FpNotifyList pFunction)
{
	pFunction((CPtrList*)&theApp.m_listScriptInfo, 0);
}

void CSystemTradingExtLibApp::ClearAllScriptInfo()
{
	POSITION pos = m_listScriptInfo.GetHeadPosition();
	while(pos)
	{
		ST_SCRIPTINFO* pScriptInfo = m_listScriptInfo.GetNext(pos);
		delete [] pScriptInfo->m_lpszName;

		delete pScriptInfo;
	}

	m_listScriptInfo.RemoveAll();

	std::for_each(m_vtUserFuncInfo.begin(), m_vtUserFuncInfo.end(), DeleteScriptInfo);
	m_vtUserFuncInfo.clear();

	std::for_each(m_vtInnerFuncInfo.begin(), m_vtInnerFuncInfo.end(), DeleteInnerFuncInfo);
	m_vtInnerFuncInfo.clear();
}

void CSystemTradingExtLibApp::ListupScriptFile()
{
	ClearAllScriptInfo();

	char szPath[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szPath, _T("%s%s*.sts"), m_szRootPath, PATH_SCRIPTFILES);
	::wsprintf(szPath, _T("%s%s*.sts"), m_szUserPath, PATH_SCRIPTFILES2);

	CFileFind ff;
	BOOL bFind = ff.FindFile(szPath);

	CString strScriptName;
	char szConfigPath[MAX_PATH];
	UINT nCompiled = IMG_STS;
	char szType[3] = {NULL, };
	while(bFind)
	{
		bFind = ff.FindNextFile();

		strScriptName = ff.GetFileTitle();
		::GetSTFile(szConfigPath, strScriptName, m_szRootPath);
		
		//@Solomon LPCSTR szAt = ::strrchr(szConfigPath, _T('@')) + 1;
		LPCSTR szAt = ::strrchr(szConfigPath, gSTGubunKey) + 1;
		LPCSTR szDot = ::strrchr(szConfigPath, _T('.'));

		::strset(szType, 0x00);
		::strncpy(szType, szAt, (szDot - szAt));
		if(atoi(szType) != STRATEGY_FIND)
			continue;

//		::wsprintf(szConfigPath, _T("%s%s%s.sfg"), m_szRootPath, PATH_DLLFILES, strScriptName);
		nCompiled = (BOOL)::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), nCompiled, szConfigPath);

		if(nCompiled != IMG_STS && nCompiled != IMG_STS_LOCK)
		{
			ST_SCRIPTINFO* pScriptInfo = new ST_SCRIPTINFO;
			pScriptInfo->m_nLanguageType = ::GetPrivateProfileInt(_T("Basic"), _T("LangType"), 0, szConfigPath);

			pScriptInfo->m_lpszName = new char[strScriptName.GetLength() + 1];
			::lstrcpy(pScriptInfo->m_lpszName, strScriptName);

			m_listScriptInfo.AddTail(pScriptInfo);
		}
	}

	InitUserFuntionList();

#ifdef _DEBUG
	::wsprintf(szPath, _T("%s\\BinD\\STBasicalFunc.dll"), m_szRootPath);
#else
	::wsprintf(szPath, _T("%s\\Bin\\STBasicalFunc.dll"), m_szRootPath);
#endif
	InitInnerFunctionList(szPath);

#ifdef _DEBUG
	::wsprintf(szPath, _T("%s\\BinD\\TechnicalIndexFunc.dll"), m_szRootPath);
#else
	::wsprintf(szPath, _T("%s\\Bin\\TechnicalIndexFunc.dll"), m_szRootPath);
#endif
	InitInnerFunctionList(szPath);
}

void CSystemTradingExtLibApp::InitUserFuntionList()
{
	char szPath[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szPath, _T("%s%s*.uft"), m_szRootPath, PATH_SCRIPTFILES);
	::wsprintf(szPath, _T("%s%s*.uft"), m_szUserPath, PATH_SCRIPTFILES2);
	
	UINT nID = TREEINDEX_DEFUALTITEM;

	CFileFind ff;
	BOOL bFind = ff.FindFile(szPath);

	CString strScriptName;
	char szConfigPath[MAX_PATH];
	UINT nCompiled = IMG_FUNC;
	while(bFind)
	{
		bFind = ff.FindNextFile();

		strScriptName = ff.GetFileTitle();
		::GetSTFile(szConfigPath, strScriptName, m_szRootPath);
//		::wsprintf(szConfigPath, _T("%s%s%s.sfg"), m_szRootPath, PATH_DLLFILES, strScriptName);
		nCompiled = (BOOL)::GetPrivateProfileInt(_T("Basic"), _T("Compiled"), nCompiled, szConfigPath);

		CScriptInfomation* pUserFunc = new CScriptInfomation(strScriptName, nID++, FT_USERFUNCTION, nCompiled);
		m_vtUserFuncInfo.push_back(pUserFunc);
	}
}

typedef BOOL (WINAPI* FpGetInputs)(long, long, CString&, CString&);
void CSystemTradingExtLibApp::InitInnerFunctionList(LPSTR lpszFileName)
{
	HINSTANCE hInnerFunction = ::LoadLibrary(lpszFileName);

	if(!hInnerFunction)
		return;

	FpGetInputs	pGetInputs;
	pGetInputs = (FpGetInputs)::GetProcAddress(hInnerFunction, _T("fnGetInputs"));
	
	if(!pGetInputs)
	{
		::FreeLibrary(hInnerFunction);
		return;
	}

	int i = 0;
	CString strFunctioName(_T(""));
	CString strInputs(_T(""));

	CString strProperty(_T(""));
	CString strSection(_T(""));
	CString strKey(_T(""));
	CString strRefIndex(_T(""));

	int nLength = 0;

	while(pGetInputs(0, i, strFunctioName, strInputs))
//	while(pGetInputs(i, strFunctioName, strInputs))
	{
		strFunctioName.Replace(_T(" "), _T(""));
		strInputs.Replace(_T(" "), _T(""));

		if(!strFunctioName.GetLength())
		{
			i++;
			continue;
		}

		if(strInputs.GetLength() && strInputs[0] == '[')
		{
			strInputs.Delete(0, 1);
			if(AfxExtractSubString(strProperty, strInputs, 0, ']'))
			{
				nLength = strProperty.GetLength();
				AfxExtractSubString(strSection, strProperty, 0, ',');
				AfxExtractSubString(strKey, strProperty, 1, ',');
				AfxExtractSubString(strRefIndex, strProperty, 2, ',');
			}

			strInputs.Delete(0, nLength + 1);
		}
		
		CInnerFunctionInfo* pInnerFunction = new CInnerFunctionInfo(strFunctioName, strInputs, i, strSection, strKey, atoi(strRefIndex));
		m_vtInnerFuncInfo.push_back(pInnerFunction);

		i++;
		nLength = 0;

		strProperty.Empty();
		strSection.Empty();
		strRefIndex.Empty();
	}

	::FreeLibrary(hInnerFunction);
}

void CSystemTradingExtLibApp::LoadSystemTradingParser()
{
//@유진삭제
//	m_pLoadPaser = new CLoadSystemTradingParserHelper;
//	m_pLoadPaser->InitSystemTradingParser(m_szRootPath, &m_vtUserFuncInfo, &m_vtInnerFuncInfo);
//@유진삭제
// 유진의 경우 디렉토리의 체계때문에 조건에서는 DLL을 로딩하지 못하므로 경로를 지정해줘야함.
	CString szDllName;
	szDllName.Format("%s\\%s\\%s", theApp.m_szRootPath, "dev", "SystemTradingParser.dll");
	m_pLoadPaser = new CLoadSystemTradingParserHelper((LPSTR)(LPCSTR)szDllName);
	m_pLoadPaser->InitSystemTradingParser(m_szRootPath, &m_vtUserFuncInfo, &m_vtInnerFuncInfo);
}

void CSystemTradingExtLibApp::DeleteSystemTradingParser()
{
	if(m_pLoadPaser)
	{
		delete m_pLoadPaser;
		m_pLoadPaser = NULL;
	}
}

void CSystemTradingExtLibApp::AddNotifyFunction(FpNotifyList pFunction)
{
	if(!pFunction)
		return;
	
	vector<FpNotifyList>::iterator it = std::find(m_vtNotifyFunction.begin(), m_vtNotifyFunction.end(), pFunction);
	if(it == m_vtNotifyFunction.end())
		m_vtNotifyFunction.push_back(pFunction);
}

void CSystemTradingExtLibApp::DeleteNotifyFunction(FpNotifyList pFunction)
{
	vector<FpNotifyList>::iterator it = std::find(m_vtNotifyFunction.begin(), m_vtNotifyFunction.end(), pFunction);
	if(it != m_vtNotifyFunction.end())
		m_vtNotifyFunction.erase(it);
}

void CSystemTradingExtLibApp::NotifyFunction()
{
	std::for_each(m_vtNotifyFunction.begin(), m_vtNotifyFunction.end(), CallNotifyFunction);
}

BOOL CSystemTradingExtLibApp::DecodingFile(LPCSTR lpszSource, LPCSTR lpszDest, LPSTR lpszLine1/* = NULL*/, LPSTR lpszLine2/* = NULL*/, LPSTR lpszLine3/* = NULL*/)
{
//@유진삭제
//	CLoadSystemTradingEncoder loadEncoder;
//	return loadEncoder.DecryptFile(lpszSource, lpszDest, lpszLine1, lpszLine2, lpszLine3);
//@유진삭제
	// 유진의 경우 디렉토리의 체계때문에 조건에서는 DLL을 로딩하지 못하므로 경로를 지정해줘야함.
	CString szDllName;
	//@Solomon szDllName.Format("%s\\%s\\%s", theApp.m_szRootPath, "chart", "SystemTradingEncoder.dll");
	szDllName.Format("%s%s\\%s", theApp.m_szRootPath, "\\dev", "SystemTradingEncoder.dll");
	CLoadSystemTradingEncoder loadEncoder((LPSTR)(LPCSTR)szDllName);
	return loadEncoder.DecryptFile(lpszSource, lpszDest, lpszLine1, lpszLine2, lpszLine3);
}


extern "C" __declspec(dllexport) BOOL EX_InitExtLib(FpNotifyList pFunction, LPSTR lpszRootPath)
{
	theApp.AddNotifyFunction(pFunction);

	if(!theApp.m_bInitLib)
	{
		::lstrcpy(theApp.m_szRootPath, lpszRootPath);
		KEY_GETENVVARIABLE();
		if(xRead>0)
		{
			sprintf(theApp.m_szUserPath, xUserEncDir);
		}
		else
		{
			sprintf(theApp.m_szUserPath, "%s%s", lpszRootPath, "\\Usr");
		}
		theApp.ListupScriptFile();
	}

	theApp.m_bInitLib = TRUE;

	return TRUE;
}

extern "C" __declspec(dllexport) void EX_ReloadScriptInfo(LPSTR lpszRootPath)
{
	if(!theApp.m_bInitLib)
		return;

	::lstrcpy(theApp.m_szRootPath, lpszRootPath);
	KEY_GETENVVARIABLE();
	if(xRead>0)
	{
		sprintf(theApp.m_szUserPath, xUserEncDir);
	}
	else
	{
		sprintf(theApp.m_szUserPath, "%s%s", lpszRootPath, "\\Usr");
	}
	theApp.ListupScriptFile();

	theApp.NotifyFunction();
}

extern "C" __declspec(dllexport) CPtrList* EX_GetScriptList()
{
	if(!theApp.m_bInitLib)
		return NULL;

	return (CPtrList*)&theApp.m_listScriptInfo;
}

extern "C" __declspec(dllexport) void EX_ExitExtLib(FpNotifyList pFunction)
{
	if(pFunction)
	{
		theApp.DeleteNotifyFunction(pFunction);
		theApp.ClearAllScriptInfo();

		theApp.m_bInitLib = FALSE;
	}
	else
	{
		if(!theApp.m_bInitLib)
			theApp.ClearAllScriptInfo();
	}

	theApp.DeleteSystemTradingParser();
}

extern "C" __declspec(dllexport) BOOL EX_GetScriptInfo(LPSTR lpszScripName, CString& strInfo)
{
	VT_DECLDATA vtUsedUserFunction;
	vtUsedUserFunction.clear();
	
	char szSaveFile[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.m_szRootPath, PATH_SCRIPTFILES, lpszScripName);
	::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.m_szUserPath, PATH_SCRIPTFILES2, lpszScripName);

	char szBackupFile[MAX_PATH] = {NULL, };
	::GetSTFile(szBackupFile, lpszScripName, theApp.m_szRootPath);
//	::wsprintf(szBackupFile, _T("%s%s%s.sfg"), theApp.m_szRootPath, PATH_DLLFILES, lpszScripName);

	int nCompiled = ::GetPrivateProfileInt(BASIC, _T("Compiled"), 1, szBackupFile);
	if(nCompiled == 1 || nCompiled == 2)
	{
		strInfo.Format(_T("%s 전략 스크립트는 검증되지 않았습니다."), lpszScripName);
		return FALSE;
	}

	int nScriptType = ::GetPrivateProfileInt(BASIC, LANGTYPE, 0, szBackupFile);

//@유진삭제	CLoadSystemTradingEncoder* pLoadEncoder = new CLoadSystemTradingEncoder;
	// 유진의 경우 디렉토리의 체계때문에 조건에서는 DLL을 로딩하지 못하므로 경로를 지정해줘야함.
	CString szDllName;
	//@Solomon szDllName.Format("%s\\%s\\%s", theApp.m_szRootPath, "chart", "SystemTradingEncoder.dll");
	szDllName.Format("%s%s\\%s", theApp.m_szRootPath, "\\dev", "SystemTradingEncoder.dll");
	CLoadSystemTradingEncoder* pLoadEncoder = new CLoadSystemTradingEncoder((LPSTR)(LPCSTR)szDllName);
	
	::wsprintf(szBackupFile, _T("%s.bak"), szSaveFile);

	CFileFind ff;
	BOOL bFind = ff.FindFile(szBackupFile);
	if(!bFind)
		pLoadEncoder->DecryptFile(szSaveFile, szBackupFile);

	if(!theApp.m_pLoadPaser)
		theApp.LoadSystemTradingParser();
	theApp.m_pLoadPaser->InitSystemTradingParser(theApp.m_szRootPath, &theApp.m_vtUserFuncInfo, &theApp.m_vtInnerFuncInfo);
	theApp.m_pLoadPaser->GetUsedUserFunction(&vtUsedUserFunction, bFind ? szSaveFile : szBackupFile);
	theApp.DeleteSystemTradingParser();

	if(!bFind)
		::DeleteFile(szBackupFile);

	VT_DECLDATA::iterator it = vtUsedUserFunction.begin();
	while(it != vtUsedUserFunction.end())
	{
		VT_DECLDATA vtUsedFunction;

//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.m_szRootPath, PATH_SCRIPTFILES, (*it)->GetDeclareOriginString());
		::wsprintf(szSaveFile, _T("%s%s%s.uft"), theApp.m_szUserPath, PATH_SCRIPTFILES2, (*it)->GetDeclareOriginString());
		::GetSTFile(szBackupFile, (*it)->GetDeclareOriginString(), theApp.m_szRootPath);
//		::wsprintf(szBackupFile, _T("%s%s%s.sfg"), theApp.m_szRootPath, PATH_DLLFILES, (*it)->GetDeclareOriginString());

		int nCompiled = ::GetPrivateProfileInt(BASIC, _T("Compiled"), 7, szBackupFile);
		if(nCompiled == 7 || nCompiled == 8)
		{
			strInfo.Format(_T("%s 전략 스크립트가 사용하는 %s 함수는 검증되지 않았습니다."), lpszScripName, (*it)->GetDeclareOriginString());
			delete pLoadEncoder;
			return FALSE;
		}

		::wsprintf(szBackupFile, _T("%s.bak"), szSaveFile);
		bFind = ff.FindFile(szBackupFile);
		if(!bFind)
			pLoadEncoder->DecryptFile(szSaveFile, szBackupFile);

		if(!theApp.m_pLoadPaser)
			theApp.LoadSystemTradingParser();
		theApp.m_pLoadPaser->InitSystemTradingParser(theApp.m_szRootPath, &theApp.m_vtUserFuncInfo, &theApp.m_vtInnerFuncInfo);
		theApp.m_pLoadPaser->GetUsedUserFunction(&vtUsedFunction, bFind ? szSaveFile : szBackupFile);
		theApp.DeleteSystemTradingParser();

		if(!bFind)
			::DeleteFile(szBackupFile);
		
		VT_DECLDATA::iterator itFunc = vtUsedFunction.begin();
		bFind = FALSE;
		while(itFunc != vtUsedFunction.end())
		{
			CFindVariable findFunc((*itFunc)->GetDeclareOriginString());
			VT_DECLDATA::iterator itFind = std::find_if(vtUsedUserFunction.begin(), vtUsedUserFunction.end(), findFunc);
			if(itFind == vtUsedUserFunction.end())
			{
				bFind = TRUE;
				CDeclareData* pFunction = new CDeclareData(vtUsedUserFunction.size(), (*itFunc)->GetDeclareOriginString(), _T(""));
				vtUsedUserFunction.push_back(pFunction);
			}

			itFunc++;
		}

		for_each(vtUsedFunction.begin(), vtUsedFunction.end(), DeleteVariable);
		
		if(bFind)
		{
			it = vtUsedUserFunction.begin();
			continue;
		}

		it++;
	}
	
	if(nScriptType == 1)	// Yes Language
		strInfo += _T("MAIN_SCOPE : 100;\n");

	CString strSource;
	CString strLine;

	CStdioFile fileScript;
	char szFunction[MAX_PATH] = {NULL, };
	
	it = vtUsedUserFunction.begin();
	while(it != vtUsedUserFunction.end())
	{
		CDeclareData* pData = (*it);

//@유진삭제::wsprintf(szFunction, _T("%s%s%s.uft"), theApp.m_szRootPath, PATH_SCRIPTFILES, pData->GetDeclareOriginString());
		::wsprintf(szFunction, _T("%s%s%s.uft"), theApp.m_szUserPath, PATH_SCRIPTFILES2, pData->GetDeclareOriginString());

		::wsprintf(szBackupFile, _T("%s.bak"), szSaveFile);
		bFind = ff.FindFile(szBackupFile);
		if(!bFind)
			pLoadEncoder->DecryptFile(szFunction, szBackupFile);

		if(fileScript.Open(bFind ? szSaveFile : szBackupFile, CFile::modeRead))
		{			
			BOOL bContinue = TRUE;
			CString strTemp(_T(""));

			while(fileScript.ReadString(strLine) && bContinue)
			{
				strTemp = strLine;
				strTemp.Replace(_T(" "), _T(""));
				if(strTemp.GetLength())
				{
					if(strTemp.Left(5).CompareNoCase(_T("input")) == 0)
					{
						AfxExtractSubString(strTemp, strLine, 0, ':');
						strLine.Delete(0, strTemp.GetLength() + 1);
						strLine.TrimLeft();

						strLine.TrimRight();
						if(strLine.Right(1) == _T(";"))
							strLine.Delete(strLine.GetLength() - 1, 1);

						strTemp.Format(_T("Function NumericSeries %s(%s)"), pData->GetDeclareOriginString(), strLine);
						strInfo += strTemp;
					}
					else
					{
						strTemp.Format(_T("Function NumericSeries %s()"), pData->GetDeclareOriginString());
						strInfo += strTemp;
					}

					strInfo += _T(";\n");
					bContinue = FALSE;
				} // if(strTemp.GetLength())
				
			} // while(fileScript.ReadString(strLine))

			// 본문읽는다.
			if(strTemp.GetLength())
			{
				fileScript.SeekToBegin();
				strSource += strTemp;
				strSource += _T("\nBegin\n");

				UINT nSize = fileScript.GetLength();

				LPSTR lpszScript = new char[nSize + 1];
				::memset(lpszScript, 0x00, nSize + 1);
				fileScript.Read(lpszScript, nSize);

				strSource += lpszScript;
				strSource += _T("\nEnd\n");

				delete [] lpszScript;
			}

			fileScript.Close();
		} // if(fileScript.Open(szBackupFile, CFile::modeRead))

		if(!bFind)
			::DeleteFile(szBackupFile);

		it++;
	}

//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.m_szRootPath, PATH_SCRIPTFILES, lpszScripName);
	::wsprintf(szSaveFile, _T("%s%s%s.sts"), theApp.m_szUserPath, PATH_SCRIPTFILES2, lpszScripName);
	::wsprintf(szBackupFile, _T("%s.bak"), szSaveFile);

	bFind = ff.FindFile(szBackupFile);
	if(!bFind)
		pLoadEncoder->DecryptFile(szSaveFile, szBackupFile);

	if(fileScript.Open(bFind ? szSaveFile : szBackupFile, CFile::modeRead))
	{
		UINT nSize = fileScript.GetLength();

		LPSTR lpszScript = new char[nSize + 1];
		::memset(lpszScript, 0x00, nSize + 1);
		fileScript.Read(lpszScript, nSize);

		strSource += lpszScript;

		delete [] lpszScript;

		fileScript.Close();
	}

	if(!bFind)
		::DeleteFile(szBackupFile);

	delete pLoadEncoder;

	strInfo += strSource;

	for_each(vtUsedUserFunction.begin(), vtUsedUserFunction.end(), DeleteVariable);

	return TRUE;
}

