#include "stdafx.h"

#include "SysTradingParser.h"
#include "SysTrading.cpp.h"

#include "../../include_ST/DeclareDefine.h"

extern FILE *SysTradingin;
extern FILE *SysTradingout;

static void DeleteString(LPSTR p_szVariable)
{
	delete [] p_szVariable;
}

SysTradingParser::SysTradingParser()
{
	SysTradingin = NULL;
	SysTradingout = NULL;

	m_nKindofFile = PARSE_CPLUSPLUS;
	m_lStateFlag = 0;

	m_niptCount = 0;
	m_nvarCount = 0;
	m_narrCount = 0;

	m_nInnerFunc = 0;
	m_nUserFunc = 0;
	
	m_pVtInnerFuncInfo = NULL;
	m_pVtUserFuncInfo = NULL;
	m_pVtFunctionUser = NULL;
	m_pVtFunctionMember = NULL;

	m_lpszFunctionName = NULL;
	m_bUsedUserFunction = FALSE;
	m_nUsedMemberVariable = 0;

	m_bDontCert = FALSE;

	LEX_SetSysTradingParser(this);
	YACC_SetSysTradingParser(this);

	m_nParenthesis = m_nBracket = 0;
}

SysTradingParser::SysTradingParser(LPCSTR p_FileNameIn, LPCSTR p_FileNameOut)
{
	SysTradingin = ::fopen(p_FileNameIn, "r");
	SysTradingout = ::fopen(p_FileNameOut, "w");

	m_nKindofFile = PARSE_CPLUSPLUS;
	m_lStateFlag = 0;

	m_niptCount = 0;
	m_nvarCount = 0;
	m_narrCount = 0;

	m_nInnerFunc = 0;
	m_nUserFunc = 0;

	m_pVtInnerFuncInfo = NULL;
	m_pVtUserFuncInfo = NULL;
	m_pVtFunctionUser = NULL;
	m_pVtFunctionMember = NULL;

	m_lpszFunctionName = NULL;
	m_bUsedUserFunction = FALSE;
	m_nUsedMemberVariable = 0;

	m_bDontCert = FALSE;

	LEX_Init(NULL, 0);

	LEX_SetSysTradingParser(this);
	YACC_SetSysTradingParser(this);

	m_nParenthesis = m_nBracket = 0;
}

SysTradingParser::SysTradingParser(UINT p_nKindParse, LPCSTR p_FileNameOut, VT_INNERFUNCINFO* p_pVtFunction, VT_SCRIPTINFO* pVtUserFuncInfo, SYSVECTOR* p_pVtFunctionUser, SYSVECTOR* p_pVtFunctionMember)
{
	SysTradingout = ::fopen(p_FileNameOut, "w");

	m_nKindofFile = p_nKindParse;
	m_lStateFlag = 0;

	m_niptCount = 0;
	m_nvarCount = 0;
	m_narrCount = 0;

	m_nInnerFunc = 0;
	m_nUserFunc = 0;

	m_pVtInnerFuncInfo = p_pVtFunction;
	m_pVtUserFuncInfo = pVtUserFuncInfo;
	m_pVtFunctionUser = p_pVtFunctionUser;
	m_pVtFunctionMember = p_pVtFunctionMember;

	m_lpszFunctionName = NULL;
	m_bUsedUserFunction = FALSE;
	m_nUsedMemberVariable = 0;

	m_bDontCert = FALSE;

	LEX_Init(NULL, 0);
	LEX_SetSysTradingParser(this);
	YACC_SetSysTradingParser(this);

	m_nParenthesis = m_nBracket = 0;
}

SysTradingParser::SysTradingParser(UINT p_nKindParse, VT_INNERFUNCINFO* p_pVtFunction, VT_SCRIPTINFO* pVtUserFuncInfo, SYSVECTOR* p_pVtFunctionUser, SYSVECTOR* p_pVtFunctionMember)
{
	SysTradingout = NULL;

	m_nKindofFile = p_nKindParse;
	m_lStateFlag = 0;

	m_niptCount = 0;
	m_nvarCount = 0;
	m_narrCount = 0;

	m_nInnerFunc = 0;
	m_nUserFunc = 0;

	m_pVtInnerFuncInfo = p_pVtFunction;
	m_pVtUserFuncInfo = pVtUserFuncInfo;
	m_pVtFunctionUser = p_pVtFunctionUser;
	m_pVtFunctionMember = p_pVtFunctionMember;

	m_lpszFunctionName = NULL;
	m_bUsedUserFunction = FALSE;
	m_nUsedMemberVariable = 0;

	m_bDontCert = TRUE;

	LEX_Init(NULL, 0);
	LEX_SetSysTradingParser(this);
	YACC_SetSysTradingParser(this);

	m_nParenthesis = m_nBracket = 0;
}

void SysTradingParser::ChangeOutputFile(LPCSTR p_FileNameOut)
{
	if(SysTradingout)
		::fclose(SysTradingout);

	SysTradingout = ::fopen(p_FileNameOut, "w");
}

void SysTradingParser::CloseFiles(BOOL p_bOnlyCloseFile/* = FALSE*/)
{
	if(!p_bOnlyCloseFile)
		LEX_Close();

	if(SysTradingin)
	{
		::fclose(SysTradingin);
		SysTradingin = NULL;
	}

	if(SysTradingout)
	{
		::fclose(SysTradingout);
		SysTradingout = NULL;
	}

//	myinputptr = NULL;
//	myinputlim = NULL;
}

int SysTradingParser::Parse(LPCSTR p_szString, CErrorInfomation* pErrorInfo/* = NULL*/)
{
	myinputptr = p_szString;
	myinputlim = p_szString + strlen(p_szString);

	m_pErrorInfo = pErrorInfo;

	m_nParenthesis = m_nBracket = 0;

	LEX_InitLineNumber();
	
	return SysTradingparse();
}

void SysTradingParser::AddFuncStack(int p_nType)
{
	m_vtStackFunc.push_back(p_nType);
}

UINT SysTradingParser::GetFuncStack()
{
	if(m_vtStackFunc.empty())
		return 0;

	STACKVECTOR::reverse_iterator rit = m_vtStackFunc.rbegin();
	return (*rit);
}

void SysTradingParser::DelFuncStack()
{
	if(m_vtStackFunc.empty())
		return;

	m_vtStackFunc.pop_back();
}

// Push the new type to stack
void SysTradingParser::AddStack(int p_nType)
{
	if(!m_vtStackProc.empty())
	{
/*
		STACKVECTOR::reverse_iterator rit = m_vtStackProc.rbegin();
		if((*rit) == STACK_DEL_IF || (*rit) == STACK_DEL_IF_BLOCK)
			DelStack();
*/
/*
		else if((*rit) == STACK_ELSE)
		{
			char szLine[50] = {NULL, };
			EndStatment(szLine);
		}
*/
	}

	m_vtStackProc.push_back(p_nType);
}

// Retrieve the stack head
UINT SysTradingParser::GetStack()
{
	if(m_vtStackProc.empty())
		return 0;

	STACKVECTOR::reverse_iterator rit = m_vtStackProc.rbegin();
	return (*rit);
}

// Delete the stack head
void SysTradingParser::DelStack()
{
	if(m_vtStackProc.empty())
		return;

	m_vtStackProc.pop_back();
}

// Change the stack head to new type
void SysTradingParser::ChangeStack(UINT p_nType)
{
	if(m_vtStackProc.empty())
	{
		AddStack(p_nType);
		return;
	}

	DelStack();
	AddStack(p_nType);
}

// Change the stack head to block type
void SysTradingParser::ChangeBlockStack()
{
	if(m_vtStackProc.empty())
		return;

	STACKVECTOR::reverse_iterator rit = m_vtStackProc.rbegin();
	switch((*rit))
	{
	case STACK_FOR:
	case STACK_IF:
	case STACK_ELSEIF:
	case STACK_ELSE:
	case STACK_WHILE:
		{
			UINT nType = (*rit) + 1;
			DelStack();
			AddStack(nType);
		}
		break;
	}
}

int SysTradingParser::AddLocalVariable(LPSTR p_lpszVariable, EXPR* pExpr)
{
	if(IsLocalVariable(p_lpszVariable) >= 0)
		return -1;

	CDeclareData* pVariable = new CDeclareData(m_vtLocalVariable.size(), p_lpszVariable, pExpr->sStr, pExpr->sStrOrg);
	m_vtLocalVariable.push_back(pVariable);

/*
	LPSTR szVariable = new char[::lstrlen(p_lpszVariable) + 1];
	::lstrcpy(szVariable, p_lpszVariable);

	m_vtLocalVariable.push_back(szVariable);
*/
	return 0;
}

int SysTradingParser::AddInputVariable(LPSTR p_lpszVariable, EXPR* pExpr/* = NULL*/)
{
	if(IsInputVariable(p_lpszVariable) >= 0)
		return -1;

	CDeclareData* pVariable = NULL;
	if(pExpr)
		pVariable = new CDeclareData(m_vtInputVariable.size(), p_lpszVariable, pExpr->sStr, pExpr->sStrOrg);
	else
		pVariable = new CDeclareData(m_vtInputVariable.size(), p_lpszVariable, _T("0"));

	m_vtInputVariable.push_back(pVariable);

/*
	LPSTR szVariable = new char[::lstrlen(p_lpszVariable) + 1];
	::lstrcpy(szVariable, p_lpszVariable);

	m_vtInputVariable.push_back(szVariable);
*/
	return 0;
}

void SysTradingParser::AddArrayVariable(LPSTR p_lpszVariable, UINT nArraySize, EXPR* pExpr)
{
	if(IsArrayVariable(p_lpszVariable) >= 0)
		return;

	CDeclareData* pVariable = new CDeclareData(m_vtArrayVariable.size(), p_lpszVariable, nArraySize, pExpr->sStr, pExpr->sStrOrg);
	m_vtArrayVariable.push_back(pVariable);
}

void SysTradingParser::AddUsedInnerFunction(LPSTR p_lpszFunction)
{
	if(IsUsedInnerFunction(p_lpszFunction) >= 0)
		return;

	CDeclareData* pFunction = new CDeclareData(m_vtUsedInnerFunction.size(), p_lpszFunction, _T(""));
	m_vtUsedInnerFunction.push_back(pFunction);
}

void SysTradingParser::AddUsedUserFunction(LPSTR p_lpszFunction)
{
	if(IsUsedUserFunction(p_lpszFunction) >= 0)
		return;

	CDeclareData* pFunction = new CDeclareData(m_vtUsedUserFunction.size(), p_lpszFunction, _T(""));
	m_vtUsedUserFunction.push_back(pFunction);
}

int SysTradingParser::IsLocalVariable(LPSTR p_lpszVariable)
{
	if(m_vtLocalVariable.empty())
		return -1;

	CFindVariable findVariable(p_lpszVariable);

	VT_DECLDATA::iterator it = std::find_if(m_vtLocalVariable.begin(), m_vtLocalVariable.end(), findVariable);
	if(it != m_vtLocalVariable.end())
		return (*it)->GetDeclareIndex();
	else 
		return -1;
}

int SysTradingParser::IsArrayVariable(LPSTR p_lpszVariable)
{
	if(m_vtArrayVariable.empty())
		return -1;

	CFindVariable findVariable(p_lpszVariable);

	VT_DECLDATA::iterator it = std::find_if(m_vtArrayVariable.begin(), m_vtArrayVariable.end(), findVariable);
	if(it != m_vtArrayVariable.end())
		return (*it)->GetDeclareIndex();
	else 
		return -1;
}

int SysTradingParser::IsInputVariable(LPSTR p_lpszVariable)
{
	if(m_vtInputVariable.empty())
		return -1;

	CFindVariable findVariable(p_lpszVariable);

	VT_DECLDATA::iterator it = std::find_if(m_vtInputVariable.begin(), m_vtInputVariable.end(), findVariable);

	if(it != m_vtInputVariable.end())
		return (*it)->GetDeclareIndex();
	else 
		return -1;
}

int SysTradingParser::IsUsedInnerFunction(LPSTR p_lpszFunction)
{
	if(m_vtUsedInnerFunction.empty())
		return -1;

	CFindVariable findFunction(p_lpszFunction);

	VT_DECLDATA::iterator it = std::find_if(m_vtUsedInnerFunction.begin(), m_vtUsedInnerFunction.end(), findFunction);

	if(it != m_vtUsedInnerFunction.end())
		return (*it)->GetDeclareIndex();
	else 
		return -1;
}

int SysTradingParser::IsUsedUserFunction(LPSTR p_lpszFunction)
{
	if(m_vtUsedUserFunction.empty())
		return -1;

	CFindVariable findFunction(p_lpszFunction);

	VT_DECLDATA::iterator it = std::find_if(m_vtUsedUserFunction.begin(), m_vtUsedUserFunction.end(), findFunction);

	if(it != m_vtUsedUserFunction.end())
		return (*it)->GetDeclareIndex();
	else 
		return -1;
}

void SysTradingParser::ParseFunctionIndex(CString& p_strString, LPCTSTR p_lpszFind)
{
	int nLength = strlen(p_lpszFind);

	int nFind = p_strString.Find(p_lpszFind, 0);
	int nIndex = 0;
	while(nFind >= 0)
	{
		nIndex++;
		nFind = p_strString.Find(p_lpszFind, nFind + nLength);
	}

	CString strTemp;

	nFind = 0;
	char szIndex[3] = {NULL, };

	for(int i = 0; i < nIndex; i++)
	{
		nFind = p_strString.Find(p_lpszFind, nFind);
		if(nFind >= 0)
		{
			sprintf(szIndex, "%d", i);

			strTemp = p_strString.Mid(nFind, p_strString.GetLength() - nFind);

			int nComma = strTemp.Find(",", nLength);

			strTemp = strTemp.Left(nComma);
			p_strString.Delete(nFind, nComma);

			strTemp.Replace(strTemp.Right(strTemp.GetLength() - nLength),  szIndex);

			p_strString.Insert(nFind, strTemp);
			nFind += nLength;
		}
	}
}

void SysTradingParser::ParseDefFunctionIndex(CString& p_strString, LPCTSTR p_lpszFind)
{
	int nLength = strlen(p_lpszFind);

	int nFind = p_strString.Find(p_lpszFind, 0);
	int nIndex = 0;
	while(nFind >= 0)
	{
		nIndex++;
		nFind = p_strString.Find(p_lpszFind, nFind + nLength);
	}

	CString strTemp;

	nFind = 0;
	char szIndex[3] = {NULL, };

	for(int i = 1; i <= nIndex; i++)
	{
		nFind = p_strString.Find(p_lpszFind, nFind);
		if(nFind >= 0)
		{
			sprintf(szIndex, "%02d", i);

			strTemp = p_strString.Mid(nFind, p_strString.GetLength() - nFind);

			int nComma = strTemp.Find("=", nLength) - 1;

			strTemp = strTemp.Left(nComma);
			p_strString.Delete(nFind, nComma);

			strTemp.Replace(strTemp.Right(strTemp.GetLength() - nLength),  szIndex);

			p_strString.Insert(nFind, strTemp);
			nFind += nLength;
		}
	}
}

BOOL SysTradingParser::IsDefaultFuncion(LPSTR p_szFunctionName)
{
	CFindInnerFunction findString(p_szFunctionName);

	VT_INNERFUNCINFO::iterator it = std::find_if(m_pVtInnerFuncInfo->begin(), m_pVtInnerFuncInfo->end(), findString);
	findString.DeleteString();

	if(it != m_pVtInnerFuncInfo->end())
		return TRUE;
	else
		return FALSE;
}

BOOL SysTradingParser::IsUserFuncion(LPSTR p_szFunctionName)
{
	CScriptFind findString(p_szFunctionName);

	VT_SCRIPTINFO::iterator it = std::find_if(m_pVtUserFuncInfo->begin(), m_pVtUserFuncInfo->end(), findString);

	if(it != m_pVtUserFuncInfo->end())
		return TRUE;
	else
		return FALSE;
}

BOOL SysTradingParser::IsMemberFunction(LPSTR p_lpszFunctionName)
{
	CFindString findString(p_lpszFunctionName);

	SYSVECTOR::iterator it = std::find_if(m_pVtFunctionMember->begin(), m_pVtFunctionMember->end(), findString);
	findString.DeleteString();

	if(it != m_pVtFunctionMember->end())
		return TRUE;
	else
		return FALSE;
}

void SysTradingParser::AddUserFunction(LPSTR p_lpszFunctionNameUser)
{
	if(m_lpszFunctionName && !::lstrcmp(p_lpszFunctionNameUser, m_lpszFunctionName))
		return;
	
	CFindString findString(p_lpszFunctionNameUser);

	SYSVECTOR::iterator it = std::find_if(m_pVtFunctionUser->begin(), m_pVtFunctionUser->end(), findString);
	findString.DeleteString();

	m_bUsedUserFunction = TRUE;

	if(it != m_pVtFunctionUser->end())
		return;

	LPSTR szFunctionNameUser = new char[::lstrlen(p_lpszFunctionNameUser) + 1];
	::lstrcpy(szFunctionNameUser, p_lpszFunctionNameUser);

	m_pVtFunctionUser->push_back(szFunctionNameUser);
}

void SysTradingParser::DeleteLocalVariable()
{
	for_each(m_vtLocalVariable.begin(), m_vtLocalVariable.end(), DeleteVariable);
	m_vtLocalVariable.clear();

	m_nvarCount = 0;
}

void SysTradingParser::DeleteArrayVariable()
{
	for_each(m_vtArrayVariable.begin(), m_vtArrayVariable.end(), DeleteVariable);
	m_vtArrayVariable.clear();

	m_narrCount = 0;
}

void SysTradingParser::DeleteInputVariable()
{
	for_each(m_vtInputVariable.begin(), m_vtInputVariable.end(), DeleteVariable);
	m_vtInputVariable.clear();

	m_niptCount = 0;
}

void SysTradingParser::DeleteUsedInnerFunction()
{
	for_each(m_vtUsedInnerFunction.begin(), m_vtUsedInnerFunction.end(), DeleteVariable);
	m_vtUsedInnerFunction.clear();

	m_nInnerFunc = 0;
}

void SysTradingParser::DeleteUsedUserFunction()
{
	for_each(m_vtUsedUserFunction.begin(), m_vtUsedUserFunction.end(), DeleteVariable);
	m_vtUsedUserFunction.clear();

	m_nUserFunc = 0;
}

void SysTradingParser::WriteUsedFunction(CStdioFile* p_pFile, BOOL& bIndexStrategy)
{
	if(!m_vtUsedInnerFunction.empty())
	{
		char szLine[NAME_SZ * 2] = {NULL, };
		char szNewLine[2] = {0x0a, 0x0d};

		VT_DECLDATA::iterator itInner = m_vtUsedInnerFunction.begin();
		while(itInner != m_vtUsedInnerFunction.end())
		{
			::wsprintf(szLine, _T("pSTActor->DeclareInnerFunc(g_lSourceType, %d, \"%s\");"), (*itInner)->GetDeclareIndex(), (*itInner)->GetDeclareString());
			p_pFile->WriteString(szLine);
			p_pFile->Write(szNewLine, 1);
	
			if(!bIndexStrategy)
				bIndexStrategy = (*itInner)->IsIndexInnerFunction();

			itInner++;
		}
	}

	if(!m_vtUsedUserFunction.empty())
	{
		char szLine[NAME_SZ * 2] = {NULL, };
		char szNewLine[2] = {0x0a, 0x0d};

		VT_DECLDATA::iterator itUser = m_vtUsedUserFunction.begin();
		while(itUser != m_vtUsedUserFunction.end())
		{
			::wsprintf(szLine, _T("pSTActor->DeclareSubFunc(%d, \"%s\");"), (*itUser)->GetDeclareIndex(), (*itUser)->GetDeclareString());
			p_pFile->WriteString(szLine);
			p_pFile->Write(szNewLine, 1);

			itUser++;
		}
	}
}

void SysTradingParser::TempFunc()
{
//	AfxMessageBox("aa");
}

void SysTradingParser::DeleteCurBuffer()
{
	LEX_DeleteCurBuffer();
}

int SysTradingParser::GetParseErrorIndex(UINT nErrorIndex, LPSTR p_lpszName/* = NULL*/)
{
	int nError = 99999;
	if(m_pErrorInfo)
	{
		switch(nErrorIndex)
		{
		case 0:
			if(!m_pErrorInfo->GetErrorIndex())
				FindAnotherError(nError);
			else
				nError = m_nError;
			break;
		case 1:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_NOT_INITIALIZED_STRATEGY : CM_NOT_INITIALIZED_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
		case 2:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_NO_SEPARATOR_STRATEGY : CM_NO_SEPARATOR_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
		case 3:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_NO_TERMINATOR_STRATEGY : CM_NO_TERMINATOR_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
		case 4:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_NO_TERM_COMMENT_STRATEGY : CM_NO_TERM_COMMENT_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
		case 5:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_NOT_INIT_ARRAY_STRATEGY : CM_NOT_INIT_ARRAY_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
		case 6:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_ASSIGN_STRATEGY : CM_ERROR_ASSIGN_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
/*
		case 7:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_DUPLINPUT_STRATEGY : CM_ERROR_DUPLINPUT_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
		case 8:
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_DUPLVAR_STRATEGY : CM_ERROR_DUPLVAR_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			break;
*/
		}
	}

	m_nError = nError;
	return nError;
}

void SysTradingParser::FindAnotherError(int& nError)
{
	if(m_nParenthesis != 0)
	{
		nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_PARENTHESIS_STRATEGY : CM_ERROR_PARENTHESIS_FUNCTION;
		m_pErrorInfo->SetLineNumber(LEX_GetLineNumber(), nError);
	}
	else
	{
		nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_UNKNOWN_ERROR_STRATEGY : CM_UNKNOWN_ERROR_FUNCTION;
		m_pErrorInfo->SetLineNumber(LEX_GetLineNumber(), nError);
	}
}

