#ifndef __SysTrading_PARSER__
#define __SysTrading_PARSER__

#include "stdafx.h"
#include "DeclareDefine.h"
#include "../../Include_ST/StructDefine.h"

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <vector>
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO


#define SYSVECTOR	vector<LPSTR>

int SysTradingparse();

int LEX_Init(char *psBuf, int piLen);
int LEX_Close();
void LEX_DeleteCurBuffer();
int LEX_GetLineNumber();
void LEX_InitLineNumber();

class CFindString
{
public:
	CFindString(LPSTR p_szString)
	{
		m_szString = ::strdup(p_szString);
		::strupr(m_szString);
	}

	void DeleteString()
	{
		::free(m_szString);
	}

	bool operator()(LPSTR rhs) const
	{
		char* szString = ::strdup(rhs);
		::strupr(szString);

		int nCmp = ::lstrcmp(m_szString, szString);

		::free(szString);

		if(nCmp == 0)
			return true;
		else
			return false;
	}

private:
	char* m_szString;
};

class SysTradingParser
{
public:
	SysTradingParser();
	SysTradingParser(LPCSTR p_FileNameIn, LPCSTR p_FileNameOut);
	SysTradingParser(UINT p_nKindParse, LPCSTR p_FileNameOut, VT_INNERFUNCINFO* p_pVtFunction, VT_SCRIPTINFO* pVtUserFuncInfo, SYSVECTOR* p_pVtFunctionUser, SYSVECTOR* p_pVtFunctionMember);
	SysTradingParser(UINT p_nKindParse, VT_INNERFUNCINFO* p_pVtFunction, VT_SCRIPTINFO* pVtUserFuncInfo, SYSVECTOR* p_pVtFunctionUser, SYSVECTOR* p_pVtFunctionMember);

	void	CloseFiles(BOOL p_bOnlyCloseFile = FALSE);
	void	ChangeOutputFile(LPCSTR p_FileNameOut);


	int		Parse(LPCSTR p_szString, CErrorInfomation* pErrorInfo = NULL);
	void	DeleteCurBuffer();

	void	AddStack(int p_nType);
	UINT	GetStack();
	void	DelStack();
	void	ChangeStack(UINT p_nType);
	void	ChangeBlockStack();

	int		GetParseErrorIndex(UINT nErrorIndex, LPSTR p_lpszName = NULL);

	void	AddFuncStack(int p_nType);
	UINT	GetFuncStack();
	void	DelFuncStack();
	
	int		AddLocalVariable(LPSTR p_lpszVariable, EXPR* pExpr);
	int		AddInputVariable(LPSTR p_lpszVariable, EXPR* pExpr = NULL);
	void	AddUserFunction(LPSTR p_lpszFunctionNameUser);
	void	AddArrayVariable(LPSTR p_lpszVariable, UINT nArraySize, EXPR* pExpr);
	void	ParseDefFunctionIndex(CString& p_strString, LPCTSTR p_lpszFind);
	void	ParseFunctionIndex(CString& p_strString, LPCTSTR p_lpszFind);
	BOOL	IsDefaultFuncion(LPSTR p_lpszFunctionName);
	BOOL	IsUserFuncion(LPSTR p_szFunctionName);
	BOOL	IsMemberFunction(LPSTR p_lpszFunctionName);
	int		IsLocalVariable(LPSTR p_lpszVariable);
	int		IsInputVariable(LPSTR p_lpszVariable);
	int		IsArrayVariable(LPSTR p_lpszVariable);
	void	DeleteLocalVariable();
	void	DeleteInputVariable();
	void	DeleteArrayVariable();

	void	AddUsedInnerFunction(LPSTR p_lpszFunction);
	void	AddUsedUserFunction(LPSTR p_lpszFunction);
	int		IsUsedInnerFunction(LPSTR p_lpszFunction);
	int		IsUsedUserFunction(LPSTR p_lpszFunction);
	void	DeleteUsedInnerFunction();
	void	DeleteUsedUserFunction();
	void	WriteUsedFunction(CStdioFile* p_pFile, BOOL& bIndexStrategy);

	// ALL
	int		WriteGeneralSentence(LPSTR p_lpszString);
	int		WriteAssignSentence(LPSTR p_lpszString);
	int		WriteDeclareSentence(UCHAR p_cType,  LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	int		WriteVariableSentence(UCHAR p_cType,  LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	int		WriteArraySentence(UCHAR p_cType, LPSTR p_lpszName, LPSTR p_lpszArraySize, EXPR* pExpr, LPSTR p_lpszResult);
	int		WriteDefaultArgumentListSentence(UCHAR p_cArgument, LPSTR p_lpszName, LPSTR p_lpszResult);
	int		WriteForStartSentence(EXPR* pExpr, LPSTR p_lpszBeginArg, LPSTR p_lpszEndArg, LPSTR p_lpszResult);
	int		WriteIfStartSentence(UINT p_nIF, LPSTR p_lpszCondition, LPSTR p_lpszResult);
	int		WriteOneStatementEnd(LPSTR p_lpszString);
	int		WriteNameVariableSentence(LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	void	WriteEndStatement();

	int		ParseConditionSentence(LPSTR p_lpszCondition1, LPSTR p_lpszCondition2, LPSTR p_lpszResult, UINT nCondition);
	int		ParseOperatorSentence(LPSTR p_lpszOperator1, LPSTR p_lpszOperator2, LPSTR p_lpszResult, UINT nOperator);
	int		ParseNameSentence(EXPR* pExpr, LPSTR p_lpszResult);
	int		ParseArraySentence(LPSTR p_lpszName, LPSTR p_lpszArray, LPSTR p_lpszResult);
	int		ParseReservedSentence(UINT p_nReserved, LPSTR p_lpszValue, LPSTR p_lpszResult);
	int		ParseReservedSentence2(UINT p_nReserved, LPSTR p_lpszValue, EXPR* pExpr);
	int		ParseAssignSentence(UINT p_nAssign, EXPR* pExpr, LPSTR p_lpszRight, LPSTR p_lpszResult);
	int		ParseFunctionSentence(EXPR* pExpr, LPSTR p_lpszArgument, LPSTR p_lpszResult);
	int		ParseFunctionNameSentence(LPSTR p_lpszName, LPSTR p_lpszResult);
	int		ParseFunctionArgumentSentence(UINT p_nArgument, LPSTR p_lpszList, EXPR* pExpr, LPSTR p_lpszResult);
	int		ParseDefaultArgumentListSentence(LPSTR p_lpszList1, LPSTR p_lpszList2, LPSTR p_lpszResult);
	int		ParseMemberFunctionSentence(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult);
	int		ParseInnerFunctionSentence(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult);

	int		EndBlock(LPSTR p_lpszStatment);
	int		EndStatment(LPSTR p_lpszStatment);
	int		IsEndBlock(BOOL bStatment);
	int		IsEndOneBlock();
	int		DelEndBlock();
	int		BeginUserFunction(LPSTR p_szFunctionName);
	int		EndUserFunction(LPSTR p_szFunctionName);

	void	IncreaseParenthesis() {m_nParenthesis++;}
	void	DecreaseParenthesis() {m_nParenthesis--;}
	void	FindAnotherError(int& nError);

	int		ProcessEndStatment();
	void	SetStrategyType(LPSTR p_lpszName);

	// C++
	int		WriteGeneralSentence_CPP(LPSTR p_lpszString);
	int		WriteAssignSentence_CPP(LPSTR p_lpszString);
	int		WriteDeclareSentence_CPP(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	int		WriteVariableSentence_CPP(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	int		WriteArraySentence_CPP(UCHAR p_cType, LPSTR p_lpszName, LPSTR p_lpszArraySize, EXPR* pExpr, LPSTR p_lpszResult);
	int		WriteDefaultArgumentListSentence_CPP(UCHAR p_cArgument, LPSTR p_lpszName, LPSTR p_lpszResult);
	int		WriteForStartSentence_CPP(EXPR* pExpr, LPSTR p_lpszBeginArg, LPSTR p_lpszEndArg, LPSTR p_lpszResult);
	int		WriteIfStartSentence_CPP(UINT p_nIF, LPSTR p_lpszCondition, LPSTR p_lpszResult);
	int		WriteOneStatementEnd_CPP(LPSTR p_lpszString);
	int		WriteNameVariableSentence_CPP(LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	void	WriteEndStatement_CPP();
	
	int		ParseConditionSentence_CPP(LPSTR p_lpszCondition1, LPSTR p_lpszCondition2, LPSTR p_lpszResult, UINT nCondition);
	int		ParseOperatorSentence_CPP(LPSTR p_lpszOperator1, LPSTR p_lpszOperator2, LPSTR p_lpszResult, UINT nOperator);
	int		ParseNameSentence_CPP(EXPR* pExpr, LPSTR p_lpszResult);
	int		ParseArraySentence_CPP(LPSTR p_lpszName, LPSTR p_lpszArray, LPSTR p_lpszResult);
	int		ParseReservedSentence_CPP(UINT p_nReserved, LPSTR p_lpszValue, LPSTR p_lpszResult);
	int		ParseReservedSentence2_CPP(UINT p_nReserved, LPSTR p_lpszValue, EXPR* pExpr);
	int		ParseAssignSentence_CPP(UINT p_nAssign, EXPR* pExpr, LPSTR p_lpszRight, LPSTR p_lpszResult);
	int		ParseFunctionSentence_CPP(EXPR* pExpr, LPSTR p_lpszArgument, LPSTR p_lpszResult);
	int		ParseFunctionNameSentence_CPP(LPSTR p_lpszName, LPSTR p_lpszResult);
	int		ParseFunctionArgumentSentence_CPP(UINT p_nArgument, LPSTR p_lpszList, EXPR* pExpr, LPSTR p_lpszResult);
	int		ParseDefaultArgumentListSentence_CPP(LPSTR p_lpszList1, LPSTR p_lpszList2, LPSTR p_lpszResult);
	int		ParseMemberFunctionSentence_CPP(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult);
	int		ParseInnerFunctionSentence_CPP(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult);

	int		EndBlock_CPP(LPSTR p_lpszStatment);
	int		EndStatment_CPP(LPSTR p_lpszStatment);
	int		BeginUserFunction_CPP(LPSTR p_szFunctionName);
	int		EndUserFunction_CPP(LPSTR p_szFunctionName);
	int		IsEndBlock_CPP(BOOL bStatment);
	int		IsEndOneBlock_CPP();
	int		DelEndBlock_CPP();

	void	CompareInnerFunctionArgumentList_CPP(LPSTR lpszResult, LPSTR lpszName);
	int		ProcessEndStatment_CPP();

	void	SetStrategyType_CPP(LPSTR p_lpszName);

	// VBS
	void	WriteGeneralSentence_VBS(LPSTR p_lpszString);
	void	WriteAssignSentence_VBS(LPSTR p_lpszString);
	void	WriteDeclareSentence_VBS(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	void	WriteVariableSentence_VBS(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	void	WriteArraySentence_VBS(UCHAR p_cType, LPSTR p_lpszName, LPSTR p_lpszArraySize, EXPR* pExpr, LPSTR p_lpszResult);
	void	WriteDefaultArgumentListSentence_VBS(UCHAR p_cArgument, LPSTR p_lpszName, LPSTR p_lpszResult);
	void	WriteForStartSentence_VBS(EXPR* pExpr, LPSTR p_lpszBeginArg, LPSTR p_lpszEndArg, LPSTR p_lpszResult);
	void	WriteIfStartSentence_VBS(UINT p_nIF, LPSTR p_lpszCondition, LPSTR p_lpszResult);
	void	WriteOneStatementEnd_VBS(LPSTR p_lpszString);
	void	WriteNameVariableSentence_VBS(LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult);
	void	WriteEndStatement_VBS();
	
	void	ParseConditionSentence_VBS(LPSTR p_lpszCondition1, LPSTR p_lpszCondition2, LPSTR p_lpszResult, UINT nCondition);
	void	ParseOperatorSentence_VBS(LPSTR p_lpszOperator1, LPSTR p_lpszOperator2, LPSTR p_lpszResult, UINT nOperator);
	void	ParseNameSentence_VBS(EXPR* pExpr, LPSTR p_lpszResult);
	void	ParseArraySentence_VBS(LPSTR p_lpszName, LPSTR p_lpszArray, LPSTR p_lpszResult);
	void	ParseReservedSentence_VBS(UINT p_nReserved, LPSTR p_lpszValue, LPSTR p_lpszResult);
	void	ParseReservedSentence2_VBS(UINT p_nReserved, LPSTR p_lpszValue, EXPR* pExpr);
	void	ParseAssignSentence_VBS(UINT p_nAssign, EXPR* pExpr, LPSTR p_lpszRight, LPSTR p_lpszResult);
	void	ParseFunctionSentence_VBS(EXPR* pExpr, LPSTR p_lpszArgument, LPSTR p_lpszResult);
	void	ParseFunctionNameSentence_VBS(LPSTR p_lpszName, LPSTR p_lpszResult);
	void	ParseFunctionArgumentSentence_VBS(UINT p_nArgument, LPSTR p_lpszList, EXPR* pExpr, LPSTR p_lpszResult);
	void	ParseDefaultArgumentListSentence_VBS(LPSTR p_lpszList1, LPSTR p_lpszList2, LPSTR p_lpszResult);
	void	ParseMemberFunctionSentence_VBS(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult);
	void	ParseInnerFunctionSentence_VBS(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult);

	void	EndBlock_VBS(LPSTR p_lpszStatment);
	void	EndStatment_VBS(LPSTR p_lpszStatment);
	void	BeginUserFunction_VBS(LPSTR p_szFunctionName);
	void	EndUserFunction_VBS(LPSTR p_szFunctionName);
	void	IsEndBlock_VBS();
	void	IsEndOneBlock_VBS();
	void	DelEndBlock_VBS();

	UINT	GetUsedMemberVariable() {return m_nUsedMemberVariable;}

	void	TempFunc();

	VT_DECLDATA*	GetUsedInputVarVector() {return &m_vtInputVariable;}
	VT_DECLDATA*	GetUsedInnerFuncVector() {return &m_vtUsedInnerFunction;}
	VT_DECLDATA*	GetUsedUserFuncVector() {return &m_vtUsedUserFunction;}

	LPCSTR myinputptr;
	LPCSTR myinputlim;

private:
	UINT	m_nKindofFile;
	LONG	m_lStateFlag;

	int		m_niptCount;
	int		m_nvarCount;
	int		m_narrCount;

	int		m_nInnerFunc;
	int		m_nUserFunc;

	STACKVECTOR m_vtStackProc;
	STACKVECTOR m_vtStackFunc;
	
	VT_DECLDATA	m_vtLocalVariable;
	VT_DECLDATA	m_vtInputVariable;
	VT_DECLDATA	m_vtArrayVariable;

	VT_DECLDATA	m_vtUsedInnerFunction;
	VT_DECLDATA	m_vtUsedUserFunction;

	VT_INNERFUNCINFO*	m_pVtInnerFuncInfo;
	VT_SCRIPTINFO*		m_pVtUserFuncInfo;
	
	SYSVECTOR*	m_pVtFunction;
	SYSVECTOR*	m_pVtFunctionUser;
	SYSVECTOR*	m_pVtFunctionMember;

	LPSTR		m_lpszFunctionName;
	BOOL		m_bUsedUserFunction;
	UINT		m_nUsedMemberVariable;

	BOOL		m_bDontCert;

	CErrorInfomation* m_pErrorInfo;
	UINT		m_nError;

	int			m_nParenthesis;
	int			m_nBracket;
};

#include "SysTradingParser.inl"

void YACC_SetSysTradingParser(SysTradingParser* p_pParser);
void LEX_SetSysTradingParser(SysTradingParser* p_pParser);
BOOL IsNextTokenElse();

#endif
