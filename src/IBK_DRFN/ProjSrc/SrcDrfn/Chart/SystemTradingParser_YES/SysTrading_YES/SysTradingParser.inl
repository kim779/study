#ifndef __SysTradingParser_INL_INCLUDED
#define __SysTradingParser_INL_INCLUDED

#include "SysTradingParser.h"

inline int SysTradingParser::WriteGeneralSentence(LPSTR p_lpszString)
{
	int nReturn = 0;
	
	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteGeneralSentence_CPP(p_lpszString);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteGeneralSentence_VBS(p_lpszString);

	return nReturn;
}

inline int SysTradingParser::WriteAssignSentence(LPSTR p_lpszString)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteAssignSentence_CPP(p_lpszString);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteAssignSentence_VBS(p_lpszString);

	return nReturn;
}

inline int SysTradingParser::WriteDeclareSentence(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteDeclareSentence_CPP(p_cType, p_lpszName, pExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteDeclareSentence_VBS(p_cType, p_lpszName, pExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::WriteVariableSentence(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteVariableSentence_CPP(p_cType, p_lpszName, pExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteVariableSentence_VBS(p_cType, p_lpszName, pExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::WriteArraySentence(UCHAR p_cType, LPSTR p_lpszName, LPSTR p_lpszArraySize, EXPR* pExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteArraySentence_CPP(p_cType, p_lpszName, p_lpszArraySize, pExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteArraySentence_VBS(p_cType, p_lpszName, p_lpszArraySize, pExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::WriteDefaultArgumentListSentence(UCHAR p_cArgument, LPSTR p_lpszName, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteDefaultArgumentListSentence_CPP(p_cArgument, p_lpszName, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteDefaultArgumentListSentence_VBS(p_cArgument, p_lpszName, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::WriteForStartSentence(EXPR* pExpr, LPSTR p_lpszBeginArg, LPSTR p_lpszEndArg, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteForStartSentence_CPP(pExpr, p_lpszBeginArg, p_lpszEndArg, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteForStartSentence_VBS(pExpr, p_lpszBeginArg, p_lpszEndArg, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::WriteIfStartSentence(UINT p_nIF, LPSTR p_lpszCondition, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteIfStartSentence_CPP(p_nIF, p_lpszCondition, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteIfStartSentence_VBS(p_nIF, p_lpszCondition, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseConditionSentence(LPSTR p_lpszCondition1, LPSTR p_lpszCondition2, LPSTR p_lpszResult, UINT nCondition)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseConditionSentence_CPP(p_lpszCondition1, p_lpszCondition2, p_lpszResult, nCondition);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseConditionSentence_VBS(p_lpszCondition1, p_lpszCondition2, p_lpszResult, nCondition);

	return nReturn;
}

inline int SysTradingParser::ParseOperatorSentence(LPSTR p_lpszOperator1, LPSTR p_lpszOperator2, LPSTR p_lpszResult, UINT nOperator)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseOperatorSentence_CPP(p_lpszOperator1, p_lpszOperator2, p_lpszResult, nOperator);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseOperatorSentence_VBS(p_lpszOperator1, p_lpszOperator2, p_lpszResult, nOperator);

	return nReturn;
}

inline int SysTradingParser::ParseNameSentence(EXPR* pExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseNameSentence_CPP(pExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseNameSentence_VBS(pExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseArraySentence(LPSTR p_lpszName, LPSTR p_lpszArray, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseArraySentence_CPP(p_lpszName, p_lpszArray, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseArraySentence_VBS(p_lpszName, p_lpszArray, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseReservedSentence(UINT p_nReserved, LPSTR p_lpszValue, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseReservedSentence_CPP(p_nReserved, p_lpszValue, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseReservedSentence_VBS(p_nReserved, p_lpszValue, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseReservedSentence2(UINT p_nReserved, LPSTR p_lpszValue, EXPR* pExpr)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseReservedSentence2_CPP(p_nReserved, p_lpszValue, pExpr);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseReservedSentence2_VBS(p_nReserved, p_lpszValue, pExpr);

	return nReturn;
}

inline int SysTradingParser::ParseAssignSentence(UINT p_nAssign, EXPR* pExpr, LPSTR p_lpszRight, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseAssignSentence_CPP(p_nAssign, pExpr, p_lpszRight, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseAssignSentence_VBS(p_nAssign, pExpr, p_lpszRight, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseFunctionSentence(EXPR* pExpr, LPSTR p_lpszArgument, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseFunctionSentence_CPP(pExpr, p_lpszArgument, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseFunctionSentence_VBS(pExpr, p_lpszArgument, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseFunctionNameSentence(LPSTR p_lpszName, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseFunctionNameSentence_CPP(p_lpszName, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseFunctionNameSentence_VBS(p_lpszName, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseFunctionArgumentSentence(UINT p_nArgument, LPSTR p_lpszList, EXPR* pExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseFunctionArgumentSentence_CPP(p_nArgument, p_lpszList, pExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseFunctionArgumentSentence_VBS(p_nArgument, p_lpszList, pExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseDefaultArgumentListSentence(LPSTR p_lpszList1, LPSTR p_lpszList2, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseDefaultArgumentListSentence_CPP(p_lpszList1, p_lpszList2, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseDefaultArgumentListSentence_VBS(p_lpszList1, p_lpszList2, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::ParseMemberFunctionSentence(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseMemberFunctionSentence_CPP(p_nMember, pExpr, p_lpszExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseMemberFunctionSentence_VBS(p_nMember, pExpr, p_lpszExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::EndBlock(LPSTR p_lpszStatment)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = EndBlock_CPP(p_lpszStatment);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		EndBlock_VBS(p_lpszStatment);

	return nReturn;
}

inline int SysTradingParser::EndStatment(LPSTR p_lpszStatment)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = EndStatment_CPP(p_lpszStatment);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		EndStatment_VBS(p_lpszStatment);

	return nReturn;
}

inline int SysTradingParser::BeginUserFunction(LPSTR p_szFunctionName)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = BeginUserFunction_CPP(p_szFunctionName);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		BeginUserFunction_VBS(p_szFunctionName);

	m_lpszFunctionName = p_szFunctionName;

	return nReturn;
}

inline int SysTradingParser::EndUserFunction(LPSTR p_szFunctionName)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = EndUserFunction_CPP(p_szFunctionName);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		EndUserFunction_VBS(p_szFunctionName);

	m_lpszFunctionName = NULL;
	m_bUsedUserFunction = FALSE;

	return nReturn;
}

inline int SysTradingParser::ParseInnerFunctionSentence(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ParseInnerFunctionSentence_CPP(p_nMember, pExpr, p_lpszExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		ParseInnerFunctionSentence_VBS(p_nMember, pExpr, p_lpszExpr, p_lpszResult);

	return nReturn;
}

inline int SysTradingParser::WriteOneStatementEnd(LPSTR p_lpszString)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteOneStatementEnd_CPP(p_lpszString);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteOneStatementEnd_VBS(p_lpszString);

	return nReturn;
}

inline int SysTradingParser::IsEndBlock(BOOL bStatment)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = IsEndBlock_CPP(bStatment);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		IsEndBlock_VBS();

	return nReturn;
}

inline int SysTradingParser::IsEndOneBlock()
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = IsEndOneBlock_CPP();
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		IsEndOneBlock_VBS();

	return nReturn;
}

inline int SysTradingParser::DelEndBlock()
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = DelEndBlock_CPP();
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		DelEndBlock_VBS();

	return nReturn;
}

inline int SysTradingParser::WriteNameVariableSentence(LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
	int nReturn = 0;

	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = WriteNameVariableSentence_CPP(p_lpszName, pExpr, p_lpszResult);
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteNameVariableSentence_VBS(p_lpszName, pExpr, p_lpszResult);

	return nReturn;
}

inline void SysTradingParser::WriteEndStatement()
{
	if(m_nKindofFile == PARSE_CPLUSPLUS)
		WriteEndStatement_CPP();
	else if(m_nKindofFile == PARSE_VISUALBASIC)
		WriteEndStatement_VBS();
}

inline int SysTradingParser::ProcessEndStatment()
{
	int nReturn = 0;
	if(m_nKindofFile == PARSE_CPLUSPLUS)
		nReturn = ProcessEndStatment_CPP();

	return nReturn;
}

inline void SysTradingParser::SetStrategyType(LPSTR p_lpszName)
{
	if(m_nKindofFile == PARSE_CPLUSPLUS)
		SetStrategyType_CPP(p_lpszName);
}

#endif // __SysTradingParser_INL_INCLUDED