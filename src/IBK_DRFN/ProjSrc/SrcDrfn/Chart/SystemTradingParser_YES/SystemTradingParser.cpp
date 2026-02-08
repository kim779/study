// SystemTradingParser.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SystemTradingParser.h"
#include "SysTrading_YES/SysTradingParser.h"
#include "../include_ST/DeclareDefine.h"
#include "../Common_ST/STConfigDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		STR_INSERT_POS		_T("INSERT_PARSED_HERE")
#define		STR_INSERT_DECLARE	_T("FUNCTION_DECLARE")
#define		STR_INSERT_MAIN		_T("MAIN_FUNCTION_IMPLEMENT")
#define		STR_INSERT_SOURCE	_T("SOURCE_SCRIPT")
#define		STR_INSERT_INPUT	_T("SOURCE_INPUT")
#define		STR_INSERT_USERINFO	_T("USER_INFOMATION")
#define		STR_INSERT_FUNCTION	_T("USER_FUNCTION_IMPLEMENT")
#define		STR_INSERT_FUNCINP	_T("FUNCTION_INPUT")
#define		STR_BEGIN_BLOCK		_T("SEARCH_USERFUNCTION_DONOT_ERASE_THIS_LINE")
#define		STR_USED_USERFUNC	_T("USED_USERFUNCTION_DONOT_ERASE_THIS_LINE")

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

static void DeleteString(LPSTR p_szVariable)
{
	delete [] p_szVariable;
}

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingParserApp

BEGIN_MESSAGE_MAP(CSystemTradingParserApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTradingParserApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingParserApp construction

CSystemTradingParserApp::CSystemTradingParserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_szRootPath = NULL;
}

int CSystemTradingParserApp::ParseFileToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo)
{
//	ListupFunction();
	
	CStdioFile file(p_szFile, CFile::modeRead);
	UINT nFileSize = (UINT)file.GetLength();

	if(!nFileSize)
		return CM_EMPTYFILE_STRATEGY;

	char szOutput[MAX_PATH] = {NULL, };
	::wsprintf(szOutput, _T("%s%sOutput\\MainSmt.out"), m_szRootPath, PATH_STDATA);

	SysTradingParser SysTradingParser(PARSE_CPLUSPLUS, szOutput, m_pVtInnerFuncInfo, m_pVtUserFuncInfo, &m_vtUserFunction, &m_vtMemberVariable);

	LPSTR szFile = new char[nFileSize + 1];
	::memset(szFile, 0x00, nFileSize + 1);

	file.Read(szFile, nFileSize);
	int nError = SysTradingParser.Parse(szFile, pErrorInfo);

	delete [] szFile;

	if(nError)
	{
		file.Close();

		SysTradingParser.DeleteLocalVariable();
		SysTradingParser.DeleteArrayVariable();
		SysTradingParser.DeleteInputVariable();
		SysTradingParser.DeleteUsedInnerFunction();
		SysTradingParser.DeleteUsedUserFunction();

		SysTradingParser.CloseFiles();

		pErrorInfo->SetErrorFileName(p_szFile);

		return nError;
	}

	CString strLine;
	CStringArray strArrLine;

	file.SeekToBegin();
	while(file.ReadString(strLine))
		strArrLine.Add(strLine);

	WriteEndStatement(&SysTradingParser);

	file.Close();
	SysTradingParser.CloseFiles(TRUE);

	bIndexFunction = InsertToMainFileCPP(&SysTradingParser, &strArrLine, pInfo, pErrorInfo);
	
	SysTradingParser.DeleteLocalVariable();
	SysTradingParser.DeleteArrayVariable();
	SysTradingParser.DeleteInputVariable();
	SysTradingParser.DeleteUsedInnerFunction();
	SysTradingParser.DeleteUsedUserFunction();
	strArrLine.RemoveAll();


//	if(!m_vtUserFunction.empty())
//	{
//		::wsprintf(szOutput, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
//		SysTradingParser.ChangeOutputFile(szOutput);
//	}

	char szUserFunctionFile[MAX_PATH] = {NULL, };
	vector<LPSTR>::iterator itUserFunction;

	char szPath[MAX_PATH] = {NULL, };
	
	for(itUserFunction = m_vtUserFunction.begin(); itUserFunction != m_vtUserFunction.end(); itUserFunction++)
	{
//@유진삭제::wsprintf(szUserFunctionFile, _T("%s%s%s.uft"), m_szRootPath, PATH_SCRIPTFILES, (*itUserFunction));
		::wsprintf(szUserFunctionFile, _T("%s%s%s.uft"), m_szUserPath, PATH_SCRIPTFILES2, (*itUserFunction));

		if(!file.Open(szUserFunctionFile, CFile::modeRead))
			continue;

		nFileSize = (UINT)file.GetLength();
		if(!nFileSize)
			return CM_EMPTYFILE_FUNCTION;

		LPSTR szUserFunctionName = (*itUserFunction);

		::wsprintf(szOutput, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
		SysTradingParser.ChangeOutputFile(szOutput);

		::GetSTFile(szPath, szUserFunctionName, m_szRootPath);
//		::wsprintf(szPath, _T("%s%s%s.sfg"), m_szRootPath, PATH_DLLFILES, szUserFunctionName);
		int nValue = ::GetPrivateProfileInt(BASIC, LANGTYPE, 0, szPath);

//		UINT nID = 0;
//		CScriptFind findScript(szUserFunctionName);
//		VT_SCRIPTINFO::iterator it = std::find_if(m_pVtUserFuncInfo->begin(), m_pVtUserFuncInfo->end(), findScript);

//		if(it != m_pVtUserFuncInfo->end())
//			nID = (*it)->GetParemeterCount();

		SysTradingParser.BeginUserFunction(szUserFunctionName);

		szFile = new char[nFileSize + 1];
		::memset(szFile, 0x00, nFileSize + 1);

		int nCnt = m_vtUserFunction.size();

		file.Read(szFile, nFileSize);
		pErrorInfo->SetFileType(FT_USERFUNCTION);
		pErrorInfo->SetScriptType(nValue);
		nError = SysTradingParser.Parse(szFile, pErrorInfo);

		delete [] szFile;

		if(nError)
		{
			file.Close();

			SysTradingParser.DeleteLocalVariable();
			SysTradingParser.DeleteArrayVariable();
			SysTradingParser.DeleteInputVariable();
			SysTradingParser.DeleteUsedInnerFunction();
			SysTradingParser.DeleteUsedUserFunction();

			SysTradingParser.CloseFiles();

//@유진삭제	::wsprintf(szOutput, _T("%s%s%s.uft"), m_szRootPath, PATH_SCRIPTFILES, szUserFunctionName);
			::wsprintf(szOutput, _T("%s%s%s.uft"), m_szUserPath, PATH_SCRIPTFILES2, szUserFunctionName);
			pErrorInfo->SetErrorFileName(szOutput);

			return nError;
		}
	
		file.SeekToBegin();
		while(file.ReadString(strLine))
			strArrLine.Add(strLine);

		WriteEndStatement(&SysTradingParser);

		if(m_vtUserFunction.size() - nCnt)
		{
			itUserFunction = m_vtUserFunction.begin();
			itUserFunction += nCnt - 1;
		}

		SysTradingParser.EndUserFunction(szUserFunctionName);

		file.Close();
		SysTradingParser.CloseFiles(TRUE);

		InsertToUserFunctionFileCPP(&SysTradingParser, szUserFunctionName, &strArrLine, pInfo, pErrorInfo);

		SysTradingParser.DeleteLocalVariable();
		SysTradingParser.DeleteArrayVariable();
		SysTradingParser.DeleteInputVariable();
		SysTradingParser.DeleteUsedInnerFunction();
		SysTradingParser.DeleteUsedUserFunction();
		strArrLine.RemoveAll();		
	}


	SysTradingParser.CloseFiles();
//	InsertToTemplateFileCPP();

	return 0;
}

int CSystemTradingParserApp::ParseMainToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo)
{
//	ListupFunction();

	CStdioFile file(p_szFile, CFile::modeRead);
	UINT nFileSize = (UINT)file.GetLength();
	if(!nFileSize)
		return CM_EMPTYFILE_STRATEGY;

	char szOutput[MAX_PATH] = {NULL, };
	::wsprintf(szOutput, _T("%s%sOutput\\MainSmt.out"), m_szRootPath, PATH_STDATA);

	SysTradingParser SysTradingParser(PARSE_CPLUSPLUS, szOutput, m_pVtInnerFuncInfo, m_pVtUserFuncInfo, &m_vtUserFunction, &m_vtMemberVariable);

	LPSTR szFile = new char[nFileSize + 1];
	::memset(szFile, 0x00, nFileSize + 1);

	file.Read(szFile, nFileSize);
	int nError = SysTradingParser.Parse(szFile, pErrorInfo);

	delete [] szFile;

	if(nError)
	{
		file.Close();

		SysTradingParser.DeleteLocalVariable();
		SysTradingParser.DeleteArrayVariable();
		SysTradingParser.DeleteInputVariable();
		SysTradingParser.DeleteUsedInnerFunction();
		SysTradingParser.DeleteUsedUserFunction();

		SysTradingParser.CloseFiles();

		return nError;
	}

	CString strLine;
	CStringArray strArrLine;

	file.SeekToBegin();
	while(file.ReadString(strLine))
		strArrLine.Add(strLine);

	WriteEndStatement(&SysTradingParser);

	file.Close();
	SysTradingParser.CloseFiles(TRUE);

	bIndexFunction = InsertToMainFileCPP(&SysTradingParser, &strArrLine, pInfo, pErrorInfo);
	
	SysTradingParser.DeleteLocalVariable();
	SysTradingParser.DeleteArrayVariable();
	SysTradingParser.DeleteInputVariable();
	SysTradingParser.DeleteUsedInnerFunction();
	SysTradingParser.DeleteUsedUserFunction();
	strArrLine.RemoveAll();

	SysTradingParser.CloseFiles();

	return 0;
}

int CSystemTradingParserApp::ParseUserFunctionToCPP(LPSTR p_szFile, LPSTR lpszUserFunctionName, CErrorInfomation* pErrorInfo, CScriptSecurity* pInfo)
{
	CStdioFile file(p_szFile, CFile::modeRead);
	UINT nFileSize = (UINT)file.GetLength();
	if(!nFileSize)
		return CM_EMPTYFILE_FUNCTION;

	char szOutput[MAX_PATH] = {NULL, };
	::wsprintf(szOutput, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
	SysTradingParser SysTradingParser(PARSE_CPLUSPLUS, szOutput, m_pVtInnerFuncInfo, m_pVtUserFuncInfo, &m_vtUserFunction, &m_vtMemberVariable);

	SysTradingParser.BeginUserFunction(lpszUserFunctionName);

	LPSTR szFile = new char[nFileSize + 1];
	::memset(szFile, 0x00, nFileSize + 1);

	file.Read(szFile, nFileSize);
	int nError = SysTradingParser.Parse(szFile, pErrorInfo);

	delete [] szFile;

	if(nError)
	{
		file.Close();

		SysTradingParser.DeleteLocalVariable();
		SysTradingParser.DeleteArrayVariable();
		SysTradingParser.DeleteInputVariable();
		SysTradingParser.DeleteUsedInnerFunction();
		SysTradingParser.DeleteUsedUserFunction();

		SysTradingParser.CloseFiles();

		return nError;
	}

	CString strLine;
	CStringArray strArrLine;

	file.SeekToBegin();
	while(file.ReadString(strLine))
		strArrLine.Add(strLine);

	WriteEndStatement(&SysTradingParser);

	SysTradingParser.EndUserFunction(lpszUserFunctionName);

	file.Close();
	SysTradingParser.CloseFiles(TRUE);

	InsertToUserFunctionFileCPP(&SysTradingParser, lpszUserFunctionName, &strArrLine, pInfo, pErrorInfo);

	SysTradingParser.DeleteLocalVariable();
	SysTradingParser.DeleteArrayVariable();
	SysTradingParser.DeleteInputVariable();
	SysTradingParser.DeleteUsedInnerFunction();
	SysTradingParser.DeleteUsedUserFunction();
	strArrLine.RemoveAll();

	SysTradingParser.CloseFiles();

	return 0;
}

BOOL CSystemTradingParserApp::ParseFileToVBS(LPSTR p_szFile)
{
//	ListupFunction();
	
	char szOutput[MAX_PATH] = {NULL, };
	::wsprintf(szOutput, _T("%s%sOutput\\MainSmt.out"), m_szRootPath, PATH_STDATA);

	SysTradingParser SysTradingParser(PARSE_VISUALBASIC, szOutput, m_pVtInnerFuncInfo, m_pVtUserFuncInfo, &m_vtUserFunction, &m_vtMemberVariable);

	CStdioFile file(p_szFile, CFile::modeRead);

	CString strLine;
	while(file.ReadString(strLine))
		SysTradingParser.Parse(strLine);

	SysTradingParser.DeleteLocalVariable();
	SysTradingParser.DeleteArrayVariable();
	SysTradingParser.DeleteInputVariable();

	file.Close();

	if(!m_vtUserFunction.empty())
	{
		::wsprintf(szOutput, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
		SysTradingParser.ChangeOutputFile(szOutput);
	}

	WriteEndStatement(&SysTradingParser);

	char szUserFunctionFile[MAX_PATH] = {NULL, };
	vector<LPSTR>::iterator itUserFunction;

	for(itUserFunction = m_vtUserFunction.begin(); itUserFunction != m_vtUserFunction.end(); itUserFunction++)
	{
//@유진삭제::wsprintf(szUserFunctionFile, _T("%s%s%s.uft"), m_szRootPath, PATH_SCRIPTFILES, (*itUserFunction));
		::wsprintf(szUserFunctionFile, _T("%s%s%s.uft"), m_szUserPath, PATH_SCRIPTFILES2, (*itUserFunction));
		
		if(!file.Open(szUserFunctionFile, CFile::modeRead))
			continue;

		LPSTR szUserFunctionName = (*itUserFunction);

//		UINT nID = 0;
		CScriptFind findScript(szUserFunctionName);
		VT_SCRIPTINFO::iterator it = std::find_if(m_pVtUserFuncInfo->begin(), m_pVtUserFuncInfo->end(), findScript);

//		if(it != m_pVtUserFuncInfo->end())
//			nID = (*it)->GetParemeterCount();

		SysTradingParser.BeginUserFunction(szUserFunctionName);

		int nCnt = m_vtUserFunction.size();
		while(file.ReadString(strLine))
			SysTradingParser.Parse(strLine);

		if(m_vtUserFunction.size() - nCnt)
		{
			itUserFunction = m_vtUserFunction.begin();
			itUserFunction += nCnt - 1;
		}

		SysTradingParser.EndUserFunction(szUserFunctionName);

		file.Close();
	}

	SysTradingParser.CloseFiles();

	InsertToTemplateFileVBS(SysTradingParser.GetUsedMemberVariable());

	return TRUE;
}

void CSystemTradingParserApp::ListupFunction()
{
	char szFile[MAX_PATH] = {NULL, };
	::wsprintf(szFile, _T("%s%sParseInfo.dat"), m_szRootPath, PATH_STDATA);
	
	char szModule[1000];
//	::GetPrivateProfileString(_T("FUNCTION"), _T("DEFAULT"), _T(""), szModule, sizeof(szModule), szFile);

//	if(!::lstrlen(szModule))
//		return;

	char szPreLoading[50];
/*
	while(TRUE) 
	{
		int nPos = ::strcspn(szModule, _T(";"));
		::strncpy(szPreLoading, szModule, nPos);
		szPreLoading[nPos] = 0x00;

		LPSTR szFunction = new char[::lstrlen(szPreLoading) + 1];
		::lstrcpy(szFunction, szPreLoading);

		m_vtDefFunction.push_back(szFunction);
		
		if(!szModule[nPos])
			break;

		::lstrcpy(szModule, szModule + nPos + 1);
		::strset(szPreLoading, 0x00);
	}
*/

	::GetPrivateProfileString(_T("FUNCTION"), _T("MEMBER"), _T(""), szModule, sizeof(szModule), szFile);

	if(!::lstrlen(szModule))
		return;

	while(TRUE) 
	{
		int nPos = ::strcspn(szModule, _T(";"));
		::strncpy(szPreLoading, szModule, nPos);
		szPreLoading[nPos] = 0x00;

		LPSTR szFunction = new char[::lstrlen(szPreLoading) + 1];
		::lstrcpy(szFunction, szPreLoading);

		m_vtMemberVariable.push_back(szFunction);
		
		if(!szModule[nPos])
			break;

		::lstrcpy(szModule, szModule + nPos + 1);
		::strset(szPreLoading, 0x00);
	}
}

void CSystemTradingParserApp::DeleteAllVector()
{
//	for_each(m_vtDefFunction.begin(), m_vtDefFunction.end(), DeleteString);
//	m_vtDefFunction.clear();

	for_each(m_vtUserFunction.begin(), m_vtUserFunction.end(), DeleteString);
	m_vtUserFunction.clear();

	for_each(m_vtMemberVariable.begin(), m_vtMemberVariable.end(), DeleteString);
	m_vtMemberVariable.clear();
}

BOOL CSystemTradingParserApp::InsertToMainFileCPP(SysTradingParser* p_pSysTradingParser, CStringArray* p_pArrLine, CScriptSecurity* pInfo, CErrorInfomation* pErrorInfo)
{
	BOOL bReturn = FALSE;

	// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김-주석처리 >>
	//char szTempFile[MAX_PATH] = {NULL, };
	//::wsprintf(szTempFile, _T("%s%sMain.cpp"), m_szRootPath, PATH_STDATA);
	// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김-주석처리 <<

	char szCPPFile[MAX_PATH] = {NULL, };
	::wsprintf(szCPPFile, _T("%s%sOutput\\STSample.cpp"), m_szRootPath, PATH_STDATA);
	
	// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 >>
	//CStdioFile fileInput(szTempFile, CFile::modeRead|CFile::typeBinary);
	int nIndex = 0;
	CStringArray strMainSrcLineArray;
	::ExtractLine(g_szMainCPPSrc, strMainSrcLineArray);
	// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 <<
	
	CStdioFile fileOutput(szCPPFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	
	CStringArray strArrInput;

	CString strLine(_T(""));
	char szNewLine[2] = {0x0a, 0x0d};

	// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 >>
	//while(fileInput.ReadString(strLine))
	//{
	for( nIndex = 0 ; nIndex < strMainSrcLineArray.GetSize(); nIndex++ )
	{
		strLine = strMainSrcLineArray.GetAt(nIndex);
	// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 <<

		if(strLine.Find(STR_INSERT_POS, 0) < 0)
		{
			fileOutput.WriteString(strLine);
			fileOutput.Write(szNewLine, 1);
			continue;
		}
		else if(strLine.Find(STR_INSERT_SOURCE, 0) > 0)
		{
			char szLine[300];
			CString strSource, strTemp;

			BOOL bContunueInput = FALSE;

			for(int i = 0; i < p_pArrLine->GetSize(); i++)
			{
				strSource = p_pArrLine->GetAt(i);
				strSource.Replace(_T("\""), _T("\\\""));
				::wsprintf(szLine, _T("\"%s\","), strSource);

				fileOutput.WriteString(szLine);
				fileOutput.Write(szNewLine, 1);

				strTemp = strSource;
				strTemp.MakeUpper();
				if(strTemp.Find(_T("INPUT")) >= 0 || bContunueInput)
				{
					if(strSource.Find(_T(';'), 0) >= 0)
					{
						strSource.Replace(_T(";"), _T(""));
						bContunueInput = FALSE;
					}
					else
						bContunueInput = TRUE;

					strSource.Replace(_T(" "), _T(""));
					strSource.Replace(_T("\t"), _T(""));

					strSource.Delete(0, strSource.Find(_T(":"), 0) + 1);
					int nIndex = 0;

					CString strInput;
					while(TRUE)
					{
						if(!AfxExtractSubString(strInput, strSource, nIndex++, ','))
							break;
						
						if(!strInput.IsEmpty())
							strArrInput.Add(strInput);
					}
				}
			}
		}
		else if(strLine.Find(STR_INSERT_INPUT, 0) > 0)
		{
			char szLine[300] = {NULL, };
			for(int i = 0; i < strArrInput.GetSize(); i++)
			{
				::wsprintf(szLine, _T("\"%s\","), strArrInput.GetAt(i));
				fileOutput.WriteString(szLine);
				fileOutput.Write(szNewLine, 1);
			}

			strArrInput.RemoveAll();
		}
		else if(strLine.Find(STR_INSERT_USERINFO, 0) > 0)
		{
			char szLine[300] = {NULL, };

			::wsprintf(szLine, _T("char* g_szUserID = \"%s\";"), pInfo->GetUserID());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("char* g_szPassword = \"%s\";"), pInfo->GetPassword());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("long g_lExpiredDate = %d;"), atoi(pInfo->GetExpires()));
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::lstrcpy(szLine, _T("long g_lSourceType = 1;	// 0: Pascal, 1:VisualBasic"));
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("BOOL g_bUseUserID = %d;"), pInfo->GetUseUserID());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("BOOL g_bUsePassword = %d;"), pInfo->GetUsePassword());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("BOOL g_bUseExpiredDate = %d;"), pInfo->GetUseExpires());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);
		}
		else if(strLine.Find(STR_INSERT_MAIN, 0) > 0)
		{
			char szInputFile[MAX_PATH] = {NULL, };
			::wsprintf(szInputFile, _T("%s%sOutput\\MainSmt.out"), m_szRootPath, PATH_STDATA);
			
			CFile fileMainSmt;
			if(!fileMainSmt.Open(szInputFile, CFile::modeRead))
				continue;
			
			p_pSysTradingParser->WriteUsedFunction(&fileOutput, bReturn);

			UINT nFileSize = (UINT)fileMainSmt.GetLength();
			LPSTR szFileText = new char[nFileSize];
			
			UINT nReaded = fileMainSmt.Read(szFileText, nFileSize);
			while(nReaded < nFileSize)
				nReaded += fileMainSmt.Read(szFileText + nReaded, nFileSize - nReaded);
			
			fileOutput.Write(szFileText, nFileSize);
			
			fileMainSmt.Close();
			delete [] szFileText;
		}
	}
	
	//fileInput.Close();// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김-주석처 >>
	fileOutput.Close();

	return bReturn;
}

void CSystemTradingParserApp::InsertToUserFunctionFileCPP(SysTradingParser* p_pSysTradingParser, LPSTR p_lpszFunction, CStringArray* p_pArrLine, CScriptSecurity* pInfo, CErrorInfomation* pErrorInfo)
{
// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김-주석처리 >>
//	char szTempFile[MAX_PATH] = {NULL, };
//	::wsprintf(szTempFile, _T("%s%sUserFunction.cpp"), m_szRootPath, PATH_STDATA);
// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김-주석처리 <<

	char szCPPFile[MAX_PATH] = {NULL, };
	::wsprintf(szCPPFile, _T("%s%sOutput\\%s.STSample.cpp"), m_szRootPath, PATH_STDATA, p_lpszFunction);
// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 >>	
//	CStdioFile fileInput(szTempFile, CFile::modeRead|CFile::typeBinary);
	int nIndex;
	CStringArray strUserSrcLineArray;
	::ExtractLine(g_szUserFuncSrc, strUserSrcLineArray);
// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 <<

	CStdioFile fileOutput(szCPPFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	
	CString strLine(_T(""));
	char szNewLine[2] = {0x0a, 0x0d};

	CStringArray strArrInput;

// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 >>
	//while(fileInput.ReadString(strLine))
	//{
	for(nIndex = 0 ; nIndex < strUserSrcLineArray.GetSize(); nIndex++ )
	{
		strLine = strUserSrcLineArray.GetAt(nIndex);
// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김 <<

		if(strLine.Find(STR_INSERT_POS, 0) < 0)
		{
			fileOutput.WriteString(strLine);
			fileOutput.Write(szNewLine, 1);
			continue;
		}
		
		if(strLine.Find(STR_INSERT_DECLARE, 0) > 0)
		{
			char szParameter[150] = {NULL, };
			::wsprintf(szParameter, _T("DECLAREFUNCTION_(%s)"), p_lpszFunction);
			
			fileOutput.WriteString(szParameter);
			fileOutput.Write(szNewLine, 1);
		}
		else if(strLine.Find(STR_INSERT_SOURCE, 0) > 0)
		{
			char szLine[300];
			CString strSource;
			
			for(int i = 0; i < p_pArrLine->GetSize(); i++)
			{
				strSource = p_pArrLine->GetAt(i);
				::wsprintf(szLine, _T("\"%s\","), strSource);

				fileOutput.WriteString(szLine);
				fileOutput.Write(szNewLine, 1);

				if(strSource.Find(_T("Input")) >= 0)
				{
					strSource.Replace(_T(" "), _T(""));
					strSource.Replace(_T(";"), _T(""));
					strSource.Delete(0, strSource.Find(_T(":"), 0) + 1);
					int nIndex = 0;

					CString strInput;
					while(TRUE)
					{
						if(!AfxExtractSubString(strInput, strSource, nIndex++, ','))
							break;

						strArrInput.Add(strInput);
					}
				}
			}
		}
		else if(strLine.Find(STR_INSERT_INPUT, 0) > 0)
		{
			char szLine[300];
			for(int i = 0; i < strArrInput.GetSize(); i++)
			{
				::wsprintf(szLine, _T("\"%s\","), strArrInput.GetAt(i));
				fileOutput.WriteString(szLine);
				fileOutput.Write(szNewLine, 1);
			}

			strArrInput.RemoveAll();
		}
		else if(strLine.Find(STR_INSERT_USERINFO, 0) > 0)
		{
			char szLine[300] = {NULL, };

			::wsprintf(szLine, _T("char* g_szUserID = \"%s\";"), pInfo->GetUserID());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("char* g_szPassword = \"%s\";"), pInfo->GetPassword());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("long g_lExpiredDate = %d;"), atoi(pInfo->GetExpires()));
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::lstrcpy(szLine, _T("long g_lSourceType = 1;	// 0: Pascal, 1:VisualBasic"));
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("BOOL g_bUseUserID = %d;"), pInfo->GetUseUserID());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("BOOL g_bUsePassword = %d;"), pInfo->GetUsePassword());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);

			::wsprintf(szLine, _T("BOOL g_bUseExpiredDate = %d;"), pInfo->GetUseExpires());
			fileOutput.WriteString(szLine);
			fileOutput.Write(szNewLine, 1);
		}
		else if(strLine.Find(STR_INSERT_FUNCTION, 0) > 0)
		{
			char szParameter[150] = {NULL, };
			::wsprintf(szParameter, _T("FUNCTIONBEGIN_(%s)"), p_lpszFunction);
			fileOutput.WriteString(szParameter);
			fileOutput.Write(szNewLine, 1);
			
			BOOL bIndexFunction = FALSE;
			p_pSysTradingParser->WriteUsedFunction(&fileOutput, bIndexFunction);

			char szInputFile[MAX_PATH] = {NULL, };
			::wsprintf(szInputFile, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
			
			CFile fileFunctionSmt;
			if(!fileFunctionSmt.Open(szInputFile, CFile::modeRead))
				continue;
			
			UINT nFileSize = (UINT)fileFunctionSmt.GetLength();
			LPSTR szFileText = new char[nFileSize];
			
			UINT nReaded = fileFunctionSmt.Read(szFileText, nFileSize);
			while(nReaded < nFileSize)
				nReaded += fileFunctionSmt.Read(szFileText + nReaded, nFileSize - nReaded);
			
			fileOutput.Write(szFileText, nFileSize);
			
			::wsprintf(szParameter, "FUNCTIONEND_(%s)", p_lpszFunction);
			fileOutput.WriteString(szParameter);
			fileOutput.Write(szNewLine, 1);

			fileFunctionSmt.Close();
			delete [] szFileText;
		}
		else if(strLine.Find(STR_INSERT_FUNCINP, 0) > 0)
		{
			char szParameter[250] = {NULL, };
			::wsprintf(szParameter, "return %s((CSTActor*)lSTActor, lIndex, bTemp0, bTemp1, bTemp2 , bTemp3, bTemp4 ,bTemp5 ,bTemp6, bTemp7, bTemp8, bTemp9);", p_lpszFunction);
			fileOutput.WriteString(szParameter);
			fileOutput.Write(szNewLine, 1);
		}
	}
	
	//fileInput.Close();// 언어편집기 main.cpp, userfunction.cpp 보안점검에서 삭제요청받아 소스 내부로 내용을 옮김-주석처리 >>
	fileOutput.Close();
}

void CSystemTradingParserApp::InsertToTemplateFileCPP()
{
	char szTempFile[MAX_PATH] = {NULL, };
	::wsprintf(szTempFile, _T("%s%sSTSample.tmp"), m_szRootPath, PATH_STDATA);
	
	char szCPPFile[MAX_PATH] = {NULL, };
	::wsprintf(szCPPFile, _T("%s%sOutput\\STSample.cpp"), m_szRootPath, PATH_STDATA);
	
	CStdioFile fileInput(szTempFile, CFile::modeRead|CFile::typeBinary);
	CStdioFile fileOutput(szCPPFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	
	CString strLine(_T(""));
	char szNewLine[2] = {0x0a, 0x0d};
	while(fileInput.ReadString(strLine))
	{
		if(strLine.Find(STR_INSERT_POS, 0) < 0)
		{
			fileOutput.WriteString(strLine);
			fileOutput.Write(szNewLine, 1);
			continue;
		}
		
		if(strLine.Find(STR_INSERT_DECLARE, 0) > 0)
		{
			if(m_vtUserFunction.empty())
				continue;
			
			char szFunctionDeclare[200] = {NULL, };

			vector<LPSTR>::iterator it = m_vtUserFunction.begin();
			while(it != m_vtUserFunction.end())
			{
				char szParameter[150] = {NULL, };
/*
				UINT nID = 0;
				CFindFunction findFunc((*it));
				VT_USERFUNCINFO::iterator itFind = std::find_if(m_pVtUserFuncInfo->begin(), m_pVtUserFuncInfo->end(), findFunc);
				
				if(itFind != m_pVtUserFuncInfo->end())
					nID = (*itFind)->GetParemeterCount();

				for(int i = 0; i < nID; i++)
				{
					::wsprintf(szFunctionDeclare, _T(", BOOL bTemp%d"), i + 1);
					::strcat(szParameter, szFunctionDeclare);
				}
*/
				
				::wsprintf(szFunctionDeclare, _T("DECLAREFUNCTION_(%s)\n"), (*it));
				
				fileOutput.WriteString((LPCSTR)szFunctionDeclare);
				it++;
			}
		}
		else if(strLine.Find(STR_INSERT_MAIN, 0) > 0)
		{
			char szInputFile[MAX_PATH] = {NULL, };
			::wsprintf(szInputFile, _T("%s%sOutput\\MainSmt.out"), m_szRootPath, PATH_STDATA);
			
			CFile fileMainSmt;
			if(!fileMainSmt.Open(szInputFile, CFile::modeRead))
				continue;
			
			UINT nFileSize = (UINT)fileMainSmt.GetLength();
			LPSTR szFileText = new char[nFileSize];
			
			UINT nReaded = (UINT)fileMainSmt.Read(szFileText, nFileSize);
			while(nReaded < nFileSize)
				nReaded += fileMainSmt.Read(szFileText + nReaded, nFileSize - nReaded);
			
			fileOutput.Write(szFileText, nFileSize);
			
			fileMainSmt.Close();
			delete [] szFileText;
		}
		else if(strLine.Find(STR_INSERT_FUNCTION, 0) > 0)
		{
			char szInputFile[MAX_PATH] = {NULL, };
			::wsprintf(szInputFile, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
			
			CFile fileFunctionSmt;
			if(!fileFunctionSmt.Open(szInputFile, CFile::modeRead))
				continue;
			
			UINT nFileSize = (UINT)fileFunctionSmt.GetLength();
			LPSTR szFileText = new char[nFileSize];
			
			UINT nReaded = fileFunctionSmt.Read(szFileText, nFileSize);
			while(nReaded < nFileSize)
				nReaded += fileFunctionSmt.Read(szFileText + nReaded, nFileSize - nReaded);
			
			fileOutput.Write(szFileText, nFileSize);
			
			fileFunctionSmt.Close();
			delete [] szFileText;
		}
	}
	
	fileInput.Close();
	fileOutput.Close();
}

void CSystemTradingParserApp::InsertToTemplateFileVBS(UINT p_nUsedMemVariable)
{
	char szTempFile[MAX_PATH] = {NULL, };
	::wsprintf(szTempFile, _T("%s%sAVG.tmp"), m_szRootPath, PATH_STDATA);
	
	char szCPPFile[MAX_PATH] = {NULL, };
	::wsprintf(szCPPFile, _T("%s%sOutput\\AVG.vbs"), m_szRootPath, PATH_STDATA);
	
	CStdioFile fileInput(szTempFile, CFile::modeRead|CFile::typeBinary);
	CStdioFile fileOutput(szCPPFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	
	CString strLine(_T(""));
	char szNewLine[2] = {0x0a, 0x0d};
	while(fileInput.ReadString(strLine))
	{
		if(strLine.Find(STR_INSERT_POS, 0) < 0)
		{
			fileOutput.WriteString(strLine);
			fileOutput.Write(szNewLine, 1);
			continue;
		}
		
		if(strLine.Find(STR_INSERT_DECLARE, 0) > 0)
		{
			if(m_vtUserFunction.empty())
				continue;
			
			char szFunctionDeclare[200] = {NULL, };

			vector<LPSTR>::iterator it = m_vtUserFunction.begin();
			while(it != m_vtUserFunction.end())
			{
				char szParameter[150] = {NULL, };
				::wsprintf(szFunctionDeclare, _T("DECLAREFUNCTION_(%s)\n"), (*it));
				
				fileOutput.WriteString((LPCSTR)szFunctionDeclare);
				it++;
			}
		}
		else if(strLine.Find(STR_INSERT_MAIN, 0) > 0)
		{
			char szInputFile[MAX_PATH] = {NULL, };
			::wsprintf(szInputFile, _T("%s%sOutput\\MainSmt.out"), m_szRootPath, PATH_STDATA);
			
			CFile fileMainSmt;
			if(!fileMainSmt.Open(szInputFile, CFile::modeRead))
				continue;
			
			UINT nFileSize = (UINT)fileMainSmt.GetLength();
			LPSTR szFileText = new char[nFileSize];
			
			if(p_nUsedMemVariable)
				WriteMemberVariable(p_nUsedMemVariable, &fileOutput);

			UINT nReaded = fileMainSmt.Read(szFileText, nFileSize);
			while(nReaded < nFileSize)
				nReaded += fileMainSmt.Read(szFileText + nReaded, nFileSize - nReaded);
			
			ParseOutputScript(szFileText, nFileSize);

			fileOutput.Write(szFileText, nFileSize);
			
			fileMainSmt.Close();
			delete [] szFileText;
		}
		else if(strLine.Find(STR_INSERT_FUNCTION, 0) > 0)
		{
			char szInputFile[MAX_PATH] = {NULL, };
			::wsprintf(szInputFile, _T("%s%sOutput\\FunctionSmt.out"), m_szRootPath, PATH_STDATA);
			
			CFile fileFunctionSmt;
			if(!fileFunctionSmt.Open(szInputFile, CFile::modeRead))
				continue;
			
			UINT nFileSize = (UINT)fileFunctionSmt.GetLength();
			LPSTR szFileText = new char[nFileSize];
			
			UINT nReaded = fileFunctionSmt.Read(szFileText, nFileSize);
			while(nReaded < nFileSize)
				nReaded += fileFunctionSmt.Read(szFileText + nReaded, nFileSize - nReaded);
			
			ParseOutputScript(szFileText, nFileSize);

			fileOutput.Write(szFileText, nFileSize);
			
			fileFunctionSmt.Close();
			delete [] szFileText;
		}
	}
	
	fileInput.Close();
	fileOutput.Close();
}

void CSystemTradingParserApp::ParseOutputScript(LPVOID p_pOutput, UINT& p_nSize)
{
	CStringArray strArray;
	LPSTR szElement = new char[p_nSize];
	::memset(szElement, 0x00, p_nSize);
	UINT i = 0;
	for( i = 0; i < p_nSize; i++)
	{
		if(*((LPSTR)p_pOutput + i) == 0x0d)
		{
			if(::lstrlen(szElement))
				strArray.Add(szElement);
			
			::memset(szElement, 0x00, sizeof(szElement));
		}
		else if(*((LPSTR)p_pOutput + i) == 0x0a)
			continue;
		else
			::strncat(szElement, (LPSTR)p_pOutput + i, 1);
	}

	if(!strArray.GetSize())
	{
		delete [] szElement;
		return;
	}

	BOOL bUsedUserFunction = FALSE;
	CString strElement(_T(""));
	for(i = strArray.GetSize() - 1; i >= 0; i--)
	{
		strElement = strArray.GetAt(i);
		if(strElement.Find(STR_USED_USERFUNC, 0) >= 0)
		{
			bUsedUserFunction = TRUE;
			continue;
		}
		else if(strElement.Find(STR_BEGIN_BLOCK, 0) >= 0)
		{
			if(bUsedUserFunction)
				bUsedUserFunction = FALSE;
			continue;
		}

		if(bUsedUserFunction)
			strElement.Replace(_T("'"), _T(""));
		else
			strElement.Replace(_T("DM.IfCondition"), _T(""));

		strArray.SetAt(i, strElement);		
	}

	::memset(p_pOutput, 0x00, p_nSize);

	int nIncrease = 0;
	char szNewLine[2] = {0x0d, 0x0a};
	for(i = 0; i < strArray.GetSize(); i++)
	{
		strElement = strArray.GetAt(i);
		if(strElement.Find("'", 0) >= 0)
			continue;

		::memcpy((LPSTR)p_pOutput + nIncrease, strElement, strElement.GetLength());
		nIncrease += strElement.GetLength();

		::memcpy((LPSTR)p_pOutput + nIncrease, szNewLine, sizeof(szNewLine));
		nIncrease += sizeof(szNewLine);
	}
	
	::memcpy((LPSTR)p_pOutput + nIncrease, szNewLine, sizeof(szNewLine));
	p_nSize = nIncrease + sizeof(szNewLine);

	delete [] szElement;
}

void CSystemTradingParserApp::WriteMemberVariable(UINT p_nUsedMemVar, CStdioFile* p_pFile)
{
	char szLine[100] = {NULL, };
	char szNewLine[2] = {0x0d, 0x0a};
	
	if(p_nUsedMemVar & MEMVAR_OPEN)
	{
		::lstrcpy(szLine, _T("Open = DM.OpenData(0)"));
		p_pFile->WriteString(szLine);
		p_pFile->Write(szNewLine, sizeof(szNewLine));
	}

	if(p_nUsedMemVar & MEMVAR_HIGH)
	{
		::lstrcpy(szLine, _T("High = DM.HighData(0)"));
		p_pFile->WriteString(szLine);
		p_pFile->Write(szNewLine, sizeof(szNewLine));
	}

	if(p_nUsedMemVar & MEMVAR_LOW)
	{
		::lstrcpy(szLine, _T("Low = DM.LowData(0)"));
		p_pFile->WriteString(szLine);
		p_pFile->Write(szNewLine, sizeof(szNewLine));
	}

	if(p_nUsedMemVar & MEMVAR_CLOSE)
	{
		::lstrcpy(szLine, _T("Close = DM.CloseData(0)"));
		p_pFile->WriteString(szLine);
		p_pFile->Write(szNewLine, sizeof(szNewLine));
	}

	if(p_nUsedMemVar & MEMVAR_VOLUME)
	{
		::lstrcpy(szLine, _T("Volume = DM.VolumeData(0)"));
		p_pFile->WriteString(szLine);
		p_pFile->Write(szNewLine, sizeof(szNewLine));
	}

	if(p_nUsedMemVar & MEMVAR_AMOUNT)
	{
		::lstrcpy(szLine, _T("Amount = DM.AmountData(0)"));
		p_pFile->WriteString(szLine);
		p_pFile->Write(szNewLine, sizeof(szNewLine));
	}
}

void CSystemTradingParserApp::WriteEndStatement(SysTradingParser* pSysParser)
{
	if(pSysParser->GetStack())
	{
		char szStatement[256] = {NULL, };
//		pSysParser->WriteOneStatementEnd(szStatement);
		pSysParser->WriteEndStatement();
	}
}

void CSystemTradingParserApp::CheckScriptStatement(LPSTR p_szFile, LPSTR lpszPath)
{
//	ListupFunction();
	CStdioFile file(p_szFile, CFile::modeRead);
	UINT nFileSize = (UINT)file.GetLength();

	if(!nFileSize)
		return;
	
	SysTradingParser SysTradingParser(PARSE_CPLUSPLUS, m_pVtInnerFuncInfo, m_pVtUserFuncInfo, &m_vtUserFunction, &m_vtMemberVariable);

	LPSTR szFile = new char[nFileSize + 1];
	::memset(szFile, 0x00, nFileSize + 1);

	file.Read(szFile, nFileSize);
	int nError = SysTradingParser.Parse(szFile);

	delete [] szFile;

	if(nError)
	{
		file.Close();
		SysTradingParser.CloseFiles(TRUE);

		SysTradingParser.DeleteLocalVariable();
		SysTradingParser.DeleteArrayVariable();
		SysTradingParser.DeleteInputVariable();
		SysTradingParser.DeleteUsedInnerFunction();
		SysTradingParser.DeleteUsedUserFunction();

		SysTradingParser.CloseFiles();

		return;
	}

	CString strLine;
	CStringArray strArrLine;

	file.SeekToBegin();
	while(file.ReadString(strLine))
		strArrLine.Add(strLine);

	file.Close();
	SysTradingParser.CloseFiles(TRUE);

	int nIndex = 0;
	VT_DECLDATA* pVtInputVariable = SysTradingParser.GetUsedInputVarVector();
	VT_DECLDATA::iterator it = pVtInputVariable->begin();

	while(it != pVtInputVariable->end())
	{
		WriteInputVariable((*it)->GetDeclareOriginString(), (*it)->GetDeclareInitString(), nIndex++, lpszPath);
		it++;
	}

	char szTemp[20] = {NULL, };
	::wsprintf(szTemp, _T("%d"), nIndex);
	::WritePrivateProfileString(_T("Variables"), _T("InputCnt"), szTemp, lpszPath);

	for(int i = nIndex; i < 10; i++)
	{
		::wsprintf(szTemp, _T("InputName%d"), i);
		::WritePrivateProfileString(_T("Variables"), szTemp, NULL, lpszPath);

		::wsprintf(szTemp, _T("Input%d"), i);
		::WritePrivateProfileString(_T("Variables"), szTemp, NULL, lpszPath);
	}
		
	VT_DECLDATA* pVtUsedInnerFunc = SysTradingParser.GetUsedInnerFuncVector();
	it = pVtUsedInnerFunc->begin();

	while(it != pVtUsedInnerFunc->end())
	{
		WriteInnerFuncInfo((*it)->GetDeclareOriginString(), lpszPath, &strArrLine);
		it++;
	}

	SysTradingParser.DeleteLocalVariable();
	SysTradingParser.DeleteArrayVariable();
	SysTradingParser.DeleteInputVariable();
	SysTradingParser.DeleteUsedInnerFunction();
	SysTradingParser.DeleteUsedUserFunction();
	strArrLine.RemoveAll();
	
	SysTradingParser.CloseFiles();
}

void CSystemTradingParserApp::WriteInputVariable(LPCSTR lpszInputName, LPCSTR lpszInputInit, int nIndex, LPSTR lpszPath)
{
	char szInputName[128] = {NULL, };
	char szInputKey[20] = {NULL, };
	::wsprintf(szInputKey, _T("InputName%d"), nIndex);

	::GetPrivateProfileString(_T("Variables"), szInputKey, _T(""), szInputName, sizeof(szInputName), lpszPath);
//	if(::lstrcmp(lpszInputName, szInputName))
	{
		::WritePrivateProfileString(_T("Variables"), szInputKey, lpszInputName, lpszPath);
		::wsprintf(szInputKey, _T("Input%d"), nIndex);

		::WritePrivateProfileString(_T("Variables"), szInputKey, lpszInputInit, lpszPath);
	}
}

void CSystemTradingParserApp::WriteInnerFuncInfo(LPCSTR lpszFuncName, LPSTR lpszPath, CStringArray* pStrLine)
{
	CFindInnerFunction findInnerFunc(const_cast<LPSTR>(lpszFuncName));
	VT_INNERFUNCINFO::iterator it = std::find_if(m_pVtInnerFuncInfo->begin(), m_pVtInnerFuncInfo->end(), findInnerFunc);
	findInnerFunc.DeleteString();

	if(it == m_pVtInnerFuncInfo->end())
		return;

	LPCSTR lpszSection = (*it)->GetSection();
	LPCSTR lpszKey = (*it)->GetKey();
	if(!::lstrlen(lpszSection) || !::lstrlen(lpszKey))
		return;

	UINT nRefIndex = (*it)->GetReferenceIndex();
	if(nRefIndex)
	{
		CString strLine(_T(""));
		CString strName(_T(""));
		int nFind = -1;
		for(int i = 0; i < pStrLine->GetSize(); i++)
		{
			strLine = pStrLine->GetAt(i);
			nFind = strLine.Find(lpszFuncName);
			if(nFind >= 0)
			{
				TCHAR cChar = strLine.GetAt(nFind + ::lstrlen(lpszFuncName));
				if(cChar != _T(' ') && cChar != _T('(') && cChar != _T(';') && cChar != _T('\n') && cChar != _T('\t'))
					continue;
				
				ParseFunctionArguments(strLine);
				strName = ::strchr((LPCSTR)strLine, '"');	// 완벽하진 않지만..
//				AfxExtractSubString(strName, strLine, nRefIndex - 1, ',');

				while(ChangeIndexString(strName, lpszPath));
				AfxExtractSubString(strName, strName, 0, ',');

				::WritePrivateProfileString(lpszSection, lpszKey, (LPCSTR)strName, lpszPath);
			}
		}
	}
	else
		::WritePrivateProfileString(lpszSection, lpszKey, _T("1"), lpszPath);
}

void CSystemTradingParserApp::ParseFunctionArguments(CString& strLine)
{
	int nBegein = strLine.Find("(") + 1;
	int nEnd = 0;
	int nLength = strLine.GetLength();

	BOOL bDblQuotation = FALSE;
	int nBracket = 1;
	TCHAR cIndex = NULL;
	for(int i = nBegein; i < nLength; i++)
	{
		cIndex = strLine.GetAt(i);
		if(cIndex == '"')
		{
			bDblQuotation = !bDblQuotation;
			continue;
		}
		else if(cIndex == '(' && !bDblQuotation)
			nBracket++;
		else if(cIndex == ')' && !bDblQuotation)
		{
			nBracket--;
			if(nBracket == 0)
			{
				nEnd = i;
				break;
			}
		}
	}

	if(nEnd)
	{
		CString strCopy(strLine);
		strLine = strCopy.Mid(nBegein, nEnd - nBegein);
	}
}

BOOL CSystemTradingParserApp::ChangeIndexString(CString& strName, LPSTR lpszPath)
{
	CString strCopy(strName);
	strCopy.Replace(_T("\""), _T(""));
	int nFind = strCopy.Find(_T("%"));
	if(nFind < 0)
		return FALSE;
	strCopy.Delete(0, nFind + 1);

	if(!strCopy.GetLength())
		return FALSE;

	CString strValue(_T(""));
	if(!AfxExtractSubString(strValue, strCopy, 0, '%'))
		return FALSE;

	if(!strValue.GetLength())
		return FALSE;

	int nCount = ::GetPrivateProfileInt(_T("Variables"), _T("InputCnt"), 0, lpszPath);
	if(!nCount)
		return FALSE;

	char szInputName[128] = {NULL, };
	char szInputKey[20] = {NULL, };
	for(int i = 0; i < nCount; i++)
	{
		::wsprintf(szInputKey, _T("InputName%d"), i);
		::GetPrivateProfileString(_T("Variables"), szInputKey, _T(""), szInputName, sizeof(szInputName), lpszPath);

		if(strValue.CompareNoCase(szInputName) == 0)
		{
			::wsprintf(szInputKey, _T("Input%d"), i);
			::GetPrivateProfileString(_T("Variables"), szInputKey, _T(""), szInputName, sizeof(szInputName), lpszPath);

			strCopy.Format(_T("%%%s%%"), strValue);
			strName.Replace(strCopy, szInputName);

			return TRUE;
		}
	}

	return FALSE;
}

void CSystemTradingParserApp::GetUsedUserFunction(LPSTR p_szFile, VT_DECLDATA* pVtUsedUserFunction)
{
	CFile file(p_szFile, CFile::modeRead);

	UINT nFileSize = (UINT)file.GetLength();

	if(!nFileSize)
		return;

	SysTradingParser SysTradingParser(PARSE_CPLUSPLUS, m_pVtInnerFuncInfo, m_pVtUserFuncInfo, &m_vtUserFunction, &m_vtMemberVariable);

	LPSTR szFile = new char[nFileSize + 1];
	::memset(szFile, 0x00, nFileSize + 1);

	file.Read(szFile, nFileSize);
	int nError = SysTradingParser.Parse(szFile);

	delete [] szFile;

	if(nError)
	{
		file.Close();

		SysTradingParser.DeleteLocalVariable();
		SysTradingParser.DeleteArrayVariable();
		SysTradingParser.DeleteInputVariable();
		SysTradingParser.DeleteUsedInnerFunction();
		SysTradingParser.DeleteUsedUserFunction();

		SysTradingParser.CloseFiles();

		return;
	}

	file.Close();
	SysTradingParser.CloseFiles(TRUE);

	VT_DECLDATA::iterator it = SysTradingParser.GetUsedUserFuncVector()->begin();
	while(it != SysTradingParser.GetUsedUserFuncVector()->end())
	{
		CDeclareData* pFunction = new CDeclareData(pVtUsedUserFunction->size(), (*it)->GetDeclareOriginString(), _T(""));
		pVtUsedUserFunction->push_back(pFunction);
		it++;
	}

	SysTradingParser.DeleteLocalVariable();
	SysTradingParser.DeleteArrayVariable();
	SysTradingParser.DeleteInputVariable();
	SysTradingParser.DeleteUsedInnerFunction();
	SysTradingParser.DeleteUsedUserFunction();
	
	SysTradingParser.CloseFiles();
}

void CSystemTradingParserApp::GetUsedUserAllFunction(LPSTR p_szFile, VT_DECLDATA* pVtUsedUserFunction)
{
	GetUsedUserFunction(p_szFile, pVtUsedUserFunction);

	VT_DECLDATA::iterator it = pVtUsedUserFunction->begin();
	while(it != pVtUsedUserFunction->end())
	{
		VT_DECLDATA vtUsedFunction;

		char szSaveFile[MAX_PATH] = {NULL, };
//@유진삭제::wsprintf(szSaveFile, _T("%s%s%s.uft"), m_szRootPath, PATH_SCRIPTFILES, (*it)->GetDeclareOriginString());
		::wsprintf(szSaveFile, _T("%s%s%s.uft"), m_szUserPath, PATH_SCRIPTFILES2, (*it)->GetDeclareOriginString());

		GetUsedUserFunction(szSaveFile, &vtUsedFunction);

		VT_DECLDATA::iterator itFunc = vtUsedFunction.begin();
		BOOL bFind = FALSE;
		while(itFunc != vtUsedFunction.end())
		{
			CFindVariable findFunc((*itFunc)->GetDeclareOriginString());
			VT_DECLDATA::iterator itFind = std::find_if(pVtUsedUserFunction->begin(), pVtUsedUserFunction->end(), findFunc);
			if(itFind != pVtUsedUserFunction->end())
			{
				bFind = TRUE;
				CDeclareData* pFunction = new CDeclareData(pVtUsedUserFunction->size(), (*itFind)->GetDeclareOriginString(), _T(""));
				pVtUsedUserFunction->push_back(pFunction);
			}

			itFunc++;
		}

		if(bFind)
		{
			it = pVtUsedUserFunction->begin();
			continue;
		}

		it++;
	}
}

void CSystemTradingParserApp::DeleteCurBuffer()
{
	SysTradingParser SysTradingParser;
	SysTradingParser.DeleteCurBuffer();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTradingParserApp object

CSystemTradingParserApp theApp;

extern "C" __declspec(dllexport) void EX_InitSystemTradingParser(LPSTR p_szRootPath, VT_SCRIPTINFO* p_pVtUserFuncInfo, VT_INNERFUNCINFO* p_pVtInnerFuncInfo)
{
	theApp.m_pVtUserFuncInfo = p_pVtUserFuncInfo;
	theApp.m_pVtInnerFuncInfo = p_pVtInnerFuncInfo;
	theApp.m_szRootPath = p_szRootPath;

	theApp.m_szUserPath.Format("%s%s", p_szRootPath, "\\Usr");

	KEY_GETENVVARIABLE();
	if(xRead>0) theApp.m_szUserPath = xUserEncDir;
}

extern "C" __declspec(dllexport) int EX_ParseFileToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo)
{
	return theApp.ParseFileToCPP(p_szFile, pErrorInfo, bIndexFunction, pInfo);
}

extern "C" __declspec(dllexport) int EX_ParseMainToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo)
{
	return theApp.ParseMainToCPP(p_szFile, pErrorInfo, bIndexFunction, pInfo);
}

extern "C" __declspec(dllexport) BOOL EX_ParseUserFunctionToCPP(LPSTR p_szFile, LPSTR p_lpszUserFunctionName, CErrorInfomation* pErrorInfo, CScriptSecurity* pInfo)
{
	return theApp.ParseUserFunctionToCPP(p_szFile, p_lpszUserFunctionName, pErrorInfo, pInfo);
}

extern "C" __declspec(dllexport) BOOL EX_ParseFileToVBS(LPSTR p_szFile)
{
	return theApp.ParseFileToVBS(p_szFile);
}

extern "C" __declspec(dllexport) void EX_ExitParser()
{
	theApp.DeleteAllVector();
}

extern "C" __declspec(dllexport) void EX_CheckScriptStatement(LPSTR p_szFile, LPSTR lpszPath)
{
	theApp.CheckScriptStatement(p_szFile, lpszPath);
}

extern "C" __declspec(dllexport) void EX_GetUsedUserFunction(VT_DECLDATA* pVtUsedUserFunction, LPSTR p_szFile)
{
	theApp.GetUsedUserFunction(p_szFile, pVtUsedUserFunction);
}

extern "C" __declspec(dllexport) void EX_GetUsedUserAllFunction(VT_DECLDATA* pVtUsedUserFunction, LPSTR p_szFile)
{
	theApp.GetUsedUserAllFunction(p_szFile, pVtUsedUserFunction);
}

extern "C" __declspec(dllexport) void EX_DeleteCurBuffer()
{
	theApp.DeleteCurBuffer();
}