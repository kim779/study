#include "stdafx.h"

#include "SysTradingParser.h"
#include "SysTrading.cpp.h"
#include "../../include_ST/DeclareDefine.h"

extern FILE *SysTradingin;
extern FILE *SysTradingout;

BOOL g_bDelEndif = FALSE;
int g_nEnd = 0;


int SysTradingParser::WriteGeneralSentence_CPP(LPSTR p_lpszString)
{
	IsEndBlock_CPP(TRUE);
	::strcat(p_lpszString, _T(";\n"));

	UINT nType = GetStack();
	if(nType == STACK_STATEMENT)
	{
		DelStack();
		nType = GetStack();
		if(nType == STACK_IF)
		{
			DelStack();
			if(g_nEnd)
			{
				if(!m_bDontCert)
				{
					char szLine[100 + NAME_SZ] = {NULL, };
					::lstrcpy(szLine, _T("IFEND_\n"));
					
					for(int i = 0; i < g_nEnd; i++)
					{
						::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
					}
				}
				
				g_nEnd = 0;
			}
		}
		else if(nType == STACK_ELSE_BLOCK)
		{
			DelStack();
			if(!m_bDontCert)
			{
				char szLine[100 + NAME_SZ] = {NULL, };
				::lstrcpy(szLine, _T("IFEND_\n"));
				
				::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
			}
		}
		else if(nType == STACK_IF_END)
		{
			if(g_nEnd > 0)
				g_nEnd--;
		}
	}
	else if(nType == STACK_ELSE_BLOCK)
		DelStack();
	
	g_bDelEndif = (nType == STACK_IF);

	if(!m_bDontCert)
		::fwrite(p_lpszString, 1, ::strlen(p_lpszString), SysTradingout);

	EndStatment(p_lpszString);
//	WriteOneStatementEnd_CPP(p_lpszString);
	AddStack(STACK_STATEMENT);

	return 0;
}

int SysTradingParser::WriteAssignSentence_CPP(LPSTR p_lpszString)
{
	IsEndBlock_CPP(TRUE);

	UINT nType = GetStack();
	if(nType == STACK_STATEMENT)
	{
		DelStack();
		nType = GetStack();
		if(nType == STACK_IF)
		{
			DelStack();
			if(g_nEnd)
			{
				if(!m_bDontCert)
				{
					char szLine[100 + NAME_SZ] = {NULL, };
					::lstrcpy(szLine, _T("IFEND_\n"));
					
					for(int i = 0; i < g_nEnd; i++)
					{
						::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
//						AddStack(STACK_IF_END);
					}
				}
				
				g_nEnd = 0;
			}
		}
		else if(nType == STACK_ELSE_BLOCK)
		{
			DelStack();
			if(!m_bDontCert)
			{
				char szLine[100 + NAME_SZ] = {NULL, };
				::lstrcpy(szLine, _T("IFEND_\n"));
				
				::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
			}
		}
		else if(nType == STACK_IF_END)
		{
			if(g_nEnd > 0)
				g_nEnd--;
		}
	}
	else if(nType == STACK_ELSE_BLOCK)
		DelStack();
	
	g_bDelEndif = (nType == STACK_IF);
	
	if(!m_bDontCert)
		::fwrite(p_lpszString, 1, ::strlen(p_lpszString), SysTradingout);

	EndStatment(p_lpszString);
//	WriteOneStatementEnd_CPP(p_lpszString);
	AddStack(STACK_STATEMENT);;

	return 0;
}

int SysTradingParser::WriteDeclareSentence_CPP(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
	if(AddInputVariable(p_lpszName, pExpr) < 0 && m_pErrorInfo)
	{
		int nError = 99999;
		nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_DUPLINPUT_STRATEGY : CM_ERROR_DUPLINPUT_FUNCTION;
		m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
		return nError;
	}

	// 오근문 대리 요청
	// input의 초기값을 무조건 0으로 세팅
	if(p_cType & DTTYPE_INT)
		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, 0);\n"), m_niptCount++, p_lpszName);
//		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, %s);\n"), m_niptCount++, p_lpszName, pExpr->sStr);
	else if(p_cType & DTTYPE_DOUBLE)
		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, 0);\n"), m_niptCount++, p_lpszName);
//		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, %s);\n"), m_niptCount++, p_lpszName, pExpr->sStr);
	else if(p_cType & DTTYPE_STRING)
		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, 0);\n"), m_niptCount++, p_lpszName);
//		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, %s);\n"), m_niptCount++, p_lpszName, pExpr->sStr);
	else if(p_cType & DTTYPE_BOOL)
		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, TRUEFALSE, 0);\n"), m_niptCount++, p_lpszName);
//		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, TRUEFALSE, %s);\n"), m_niptCount++, p_lpszName, pExpr->sStr);

	if(!m_bDontCert)
		::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	return 0;
}

int SysTradingParser::WriteVariableSentence_CPP(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
	if(AddLocalVariable(p_lpszName, pExpr) < 0 && m_pErrorInfo)
	{
		int nError = 99999;
		nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_DUPLVAR_STRATEGY : CM_ERROR_DUPLVAR_FUNCTION;
		m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
		return nError;
	}

	if(p_cType & DTTYPE_INT)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, %s);\n"), m_nvarCount++, p_lpszName, pExpr->sStr);
	else if(p_cType & DTTYPE_DOUBLE)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, %s);\n"), m_nvarCount++, p_lpszName, pExpr->sStr);
	else if(p_cType & DTTYPE_STRING)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, %s);\n"), m_nvarCount++, p_lpszName, pExpr->sStr);
	else if(p_cType & DTTYPE_BOOL)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, TRUEFALSE, %s);\n"), m_nvarCount++, p_lpszName, pExpr->sStr);

	if(!m_bDontCert)
		::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

//	::wsprintf(p_lpszResult, _T("SETVARHOLDER(%d, %s);\n"), m_vtLocalVariable.size(), p_lpszName);
//	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	return 0;
}

int SysTradingParser::WriteNameVariableSentence_CPP(LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
	int nIndex = -1;
	if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, GETVARHOLDERDATA(%d, %s));\n"), m_nvarCount++, p_lpszName, nIndex, pExpr->sArg);
	else if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, GETINPUTHOLDERDATA(%d, %s));\n"), m_nvarCount++, p_lpszName, nIndex, pExpr->sArg);
	else
	{
		if(m_pErrorInfo)
		{
			int nError = 99999;
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_UNDECLARE_MEMBER_STRATEGY : CM_UNDECLARE_MEMBER_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(pExpr->sStr, LEX_GetLineNumber(), nError);
			return nError;
		}
	}

	if(!m_bDontCert)
		::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	AddLocalVariable(p_lpszName, pExpr);
	return 0;
}

int SysTradingParser::WriteArraySentence_CPP(UCHAR p_cType, LPSTR p_lpszName, LPSTR p_lpszArraySize, EXPR* pExpr, LPSTR p_lpszResult)
{
	if(p_cType & DTTYPE_INT)
		::wsprintf(p_lpszResult, _T("ARRAY(%d, %s, NumericSeries, %s, %s);\n"), m_narrCount++, p_lpszName, pExpr->sStr, p_lpszArraySize);
	else if(p_cType & DTTYPE_DOUBLE)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, %s, %s);\n"), m_narrCount++, p_lpszName, pExpr->sStr, p_lpszArraySize);
	else if(p_cType & DTTYPE_STRING)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, NumericSeries, %s, %s);\n"), m_narrCount++, p_lpszName, pExpr->sStr, p_lpszArraySize);
	else if(p_cType & DTTYPE_BOOL)
		::wsprintf(p_lpszResult, _T("VARIABLES(%d, %s, TRUEFALSE, %s, %s);\n"), m_narrCount++, p_lpszName, pExpr->sStr, p_lpszArraySize);

	if(!m_bDontCert)
		::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

//	::wsprintf(p_lpszResult, _T("SETVARHOLDER(%d, %s);\n"), m_vtLocalVariable.size(), p_lpszName);
//	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	AddArrayVariable(p_lpszName, atoi(p_lpszArraySize), pExpr);

	return 0;
}

int SysTradingParser::WriteDefaultArgumentListSentence_CPP(UCHAR p_cArgument, LPSTR p_lpszName, LPSTR p_lpszResult)
{
	if(IS_STRING(p_cArgument))
	{
	}
	else if(IS_INT(p_cArgument))	/* int 도 double로함. */
	{
	}
	else if(IS_SIMPLE(p_cArgument))
	{
		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSimple, 0);\n"), m_niptCount++, p_lpszName);
	}
	else if(IS_DOUBLE(p_cArgument))
	{
		::wsprintf(p_lpszResult, _T("INPUTS(%d, %s, NumericSeries, 0);\n"), m_niptCount++, p_lpszName);
	}
	else if(IS_BOOL(p_cArgument))
	{
	}

	if(IS_ARRAY(p_cArgument))
	{
//		::lstrcat(p_lpszResult, "*");
	}

	if(!m_bDontCert)
	{
		::fwrite(p_lpszResult, 1, ::strlen(p_lpszResult), SysTradingout);

//		::wsprintf(p_lpszResult, _T("SETINPUTHOLDER(%d, %s);\n"), m_vtInputVariable.size(), p_lpszName);
//		::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);
	}

	AddInputVariable(p_lpszName, NULL);

	return 0;
}

int SysTradingParser::WriteForStartSentence_CPP(EXPR* pExpr, LPSTR p_lpszBeginArg, LPSTR p_lpszEndArg, LPSTR p_lpszResult)
{
	int nIndex = -1;

	if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("FORBEGIN_(SETVARHOLDERDATA(%d, %s, %s), (GETVARHOLDERDATA(%d, %s) <= %s), SETVARHOLDERDATA(%d, %s, GETVARHOLDERDATA(%d, %s) + 1))\n"),
			nIndex, pExpr->sArg, p_lpszBeginArg, nIndex, pExpr->sArg, p_lpszEndArg, nIndex, pExpr->sArg, nIndex, pExpr->sArg);
	else if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("FORBEGIN_(SETINPUTHOLDERDATA(%d, %s, %s), (GETINPUTHOLDERDATA(%d, %s) <= %s), SETINPUTHOLDERDATA(%d, %s, GETINPUTHOLDERDATA(%d, %s) + 1))\n"),
			nIndex, pExpr->sArg, p_lpszBeginArg, nIndex, pExpr->sArg, p_lpszEndArg, pExpr->sArg, nIndex, nIndex, pExpr->sArg);
	else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("FORBEGIN_(SETARRAYHOLDERDATA(%d, %s, %s), (GETARRAYHOLDERDATA(%d, %s) <= %s), SETARRAYHOLDERDATA(%d, %s, GETARRAYHOLDERDATA(%d, %s) + 1))\n"),
			nIndex, pExpr->sArg, p_lpszBeginArg, nIndex, pExpr->sArg, p_lpszEndArg, nIndex, pExpr->sArg, nIndex, pExpr->sArg);

	if(!m_bDontCert)
		::fwrite(p_lpszResult, 1, ::strlen(p_lpszResult), SysTradingout);

	return 0;
}

int SysTradingParser::WriteIfStartSentence_CPP(UINT p_nIF, LPSTR p_lpszCondition, LPSTR p_lpszResult)
{
	if(m_bDontCert)
		return 0;

	if(p_nIF == 1)
	{
		::wsprintf(p_lpszResult, _T("IFBEGIN_(%s)\n"), p_lpszCondition);
		g_nEnd++;
/*
		int nStack = GetStack();
		if(nStack == STACK_STATEMENT)
		{
			DelStack();
			int nStack2 = GetStack();
			if(nStack2 != STACK_IF_END)
				g_nEnd++;
		}
		AddStack(nStack);
*/
	}
	else if(p_nIF == 2)
	{
		int nStack = GetStack();
		if(nStack == STACK_STATEMENT)
		{
			DelStack();

			int nStack2 = GetStack();
			if(nStack2 == STACK_IF)
			{
				::fseek(SysTradingout, -8, SEEK_CUR);
				g_nEnd++;

				DelStack();
			}
			else if(nStack2 == STACK_IF_END)
			{
				DelStack();
				int nStack3 = GetStack();
				if(nStack3 == STACK_IF)
					g_nEnd++;
				else if(nStack3 == STACK_ELSE)
				{
					if(g_nEnd)
					{
						if(!m_bDontCert)
						{
							char szLine[100 + NAME_SZ] = {NULL, };
							::lstrcpy(szLine, _T("IFEND_\n"));
							
							for(int i = 0; i < g_nEnd; i++)
								::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
						}
						
						g_nEnd = 0;
					}
				}
				::fseek(SysTradingout, -8, SEEK_CUR);

				DelStack();
				AddStack(nStack2);
				AddStack(nStack3);
			}
			else if(nStack2 == STACK_ELSE)
			{
				if(g_nEnd)
				{
					if(!m_bDontCert)
					{
						char szLine[100 + NAME_SZ] = {NULL, };
						::lstrcpy(szLine, _T("IFEND_\n"));
						
						for(int i = 0; i < g_nEnd; i++)
							::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
					}
					
					g_nEnd = 0;
				}

				::fseek(SysTradingout, -8, SEEK_CUR);
//				g_nEnd++;
			}
			else if(nStack == 0 || nStack == STACK_IF_BLOCK || nStack == STACK_ELSE || nStack == STACK_ELSE_BLOCK)
			{
				if(m_pErrorInfo)
				{
					int nError = 99999;
					nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_ELSESTMTERR_STRATEGY : CM_ERROR_ELSESTMTERR_FUNCTION;
					m_pErrorInfo->SetErrorLineInfomation(_T(""), LEX_GetLineNumber(), nError);
					return nError;
				}
			}

			AddStack(nStack);
		}
		else if(nStack == STACK_IF_END)
			g_nEnd++;
		else if(nStack == 0 || nStack == STACK_IF_BLOCK || nStack == STACK_ELSE || nStack == STACK_ELSE_BLOCK)
		{
			if(m_pErrorInfo)
			{
				int nError = 99999;
				nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_ELSESTMTERR_STRATEGY : CM_ERROR_ELSESTMTERR_FUNCTION;
				m_pErrorInfo->SetErrorLineInfomation(_T(""), LEX_GetLineNumber(), nError);
				return nError;
			}
		}

		::lstrcpy(p_lpszResult, _T("ELSE_\n"));
	}
	
	::fwrite(p_lpszResult, 1, ::strlen(p_lpszResult), SysTradingout);

	return 0;
}

int SysTradingParser::ParseConditionSentence_CPP(LPSTR p_lpszCondition1, LPSTR p_lpszCondition2, LPSTR p_lpszResult, UINT nCondition)
{
	if(nCondition == 1)			// Semicolon
		::wsprintf(p_lpszResult, _T("%s;"), p_lpszCondition1);
	else if(nCondition == 2)	// Parenthesis
		::wsprintf(p_lpszResult, _T("(%s)"), p_lpszCondition1);
	else if(nCondition == 3)	// Equal
		::wsprintf(p_lpszResult,  _T("%s == %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 4)	// Less Then
		::wsprintf(p_lpszResult,  _T("%s < %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 5)	// Less Equal
		::wsprintf(p_lpszResult,  _T("%s <= %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 6)	// Greater Then
		::wsprintf(p_lpszResult,  _T("%s > %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 7)	// Greater Equal
		::wsprintf(p_lpszResult,  _T("%s >= %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 8)	// Not Equal
		::wsprintf(p_lpszResult,  _T("%s != %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 50)	// Not
		::wsprintf(p_lpszResult,  _T("!%s"), p_lpszCondition1);
	else if(nCondition == 51)	// Or
		::wsprintf(p_lpszResult,  _T("%s || %s"), p_lpszCondition1, p_lpszCondition2);
	else if(nCondition == 52)	// And
		::wsprintf(p_lpszResult,  _T("%s && %s"), p_lpszCondition1, p_lpszCondition2);

	return 0;
}

int SysTradingParser::ParseOperatorSentence_CPP(LPSTR p_lpszOperator1, LPSTR p_lpszOperator2, LPSTR p_lpszResult, UINT nOperator)
{
	if(nOperator == 1)			// Plus
		::wsprintf(p_lpszResult,  _T("%s + %s"), p_lpszOperator1, p_lpszOperator2);
	else if(nOperator == 2)		// Minus
		::wsprintf(p_lpszResult,  _T("%s - %s"), p_lpszOperator1, p_lpszOperator2);
	else if(nOperator == 3)		// Mod
		::wsprintf(p_lpszResult,  _T("%s %% %s"), p_lpszOperator1, p_lpszOperator2);
	else if(nOperator == 4)		// Multiple
		::wsprintf(p_lpszResult,  _T("%s * %s"), p_lpszOperator1, p_lpszOperator2);
	else if(nOperator == 5)		// Divide
		::wsprintf(p_lpszResult,  _T("%s / %s"), p_lpszOperator1, p_lpszOperator2);
	else if(nOperator == 6)		// Minus value
		::wsprintf(p_lpszResult, _T("-%s"), p_lpszOperator1);
	else if(nOperator == 7)		// Parenthesis value
		::wsprintf(p_lpszResult, _T("(%s)"), p_lpszOperator1);

	return 0;
}

int SysTradingParser::ParseNameSentence_CPP(EXPR* pExpr, LPSTR p_lpszResult)
{
	int nIndex = -1;

	if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("GETVARHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg); 
	else if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("GETINPUTHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg);
	else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
		::wsprintf(p_lpszResult, _T("GETARRAYHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg);
	else
	{
		if(IsMemberFunction(pExpr->sStr))
		{
			AddFuncStack(FUNCTION_MEMBER);		// 멤버 함수라고 판단
			::wsprintf(p_lpszResult, _T("pSTActor->m_STDataManager.%s()"), pExpr->sStr); 
		}
		else if(IsDefaultFuncion(pExpr->sStr))
		{
			AddFuncStack(FUNCTION_DEFAULT);		// 내부 함수라고 판단
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, %s)"), IsUsedInnerFunction(pExpr->sStr), pExpr->sArg);

			CompareInnerFunctionArgumentList_CPP(p_lpszResult, pExpr->sStr);

			AddUsedInnerFunction(pExpr->sStr);
		}
		else if(IsUserFuncion(pExpr->sStr))
		{
			AddFuncStack(FUNCTION_USER);		// 사용자 함수라고 판단

			LPSTR szName = ::strdup(pExpr->sStr);
			::strupr(szName);

			if(m_lpszFunctionName && !::lstrcmp(m_lpszFunctionName, szName))
			{
				::wsprintf(p_lpszResult, _T("GETFUNCDATA(%s)"), pExpr->sArg);
			}
			else
			{
				AddUsedUserFunction(pExpr->sStr);
				::wsprintf(p_lpszResult, _T("pSTActor->SubFunc((long)pSTActor, %d, %s)"), IsUsedUserFunction(pExpr->sStr), pExpr->sArg);
			}

			::free(szName);
			AddUserFunction(pExpr->sStr);
		}
		else
		{
			if(m_pErrorInfo)
			{
				int nError = 99999;
				nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_UNDECLARE_MEMBER_STRATEGY : CM_UNDECLARE_MEMBER_FUNCTION;
				m_pErrorInfo->SetErrorLineInfomation(pExpr->sStr, LEX_GetLineNumber(), nError);
				return nError;
			}
		}

		DelFuncStack();
	}

	return 0;
}

int SysTradingParser::ParseArraySentence_CPP(LPSTR p_lpszName, LPSTR p_lpszArray, LPSTR p_lpszResult)
{
	int nIndex = -1;
	if((nIndex = IsLocalVariable(p_lpszName)) >= 0)
		::wsprintf(p_lpszResult, _T("GETVARHOLDERDATA(%d, %s)"), nIndex, p_lpszArray);
	else if((nIndex = IsInputVariable(p_lpszName)) >= 0)
		::wsprintf(p_lpszResult, _T("GETINPUTHOLDERDATA(%d, %s)"), nIndex, p_lpszName);
	else if((nIndex = IsArrayVariable(p_lpszName)) >= 0)
		::wsprintf(p_lpszResult, _T("GETARRAYHOLDERDATA(%d, %s)"), nIndex, p_lpszName);

	return 0;
}

int SysTradingParser::ParseReservedSentence_CPP(UINT p_nReserved, LPSTR p_lpszValue, LPSTR p_lpszResult)
{
	if(p_nReserved == 1)		// OPEN
		::lstrcpy(p_lpszResult, _T("OPENDATA(0)"));
	else if(p_nReserved == 2)	// OPEN, Braket
		::wsprintf(p_lpszResult, _T("OPENDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 3)	// HIGH
		::lstrcpy(p_lpszResult, _T("HIGHDATA(0)"));
	else if(p_nReserved == 4)	// HIGH, Braket
		::wsprintf(p_lpszResult, _T("HIGHDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 5)	// LOW
		::lstrcpy(p_lpszResult, _T("LOWDATA(0)"));
	else if(p_nReserved == 6)	// LOW, Braket
		::wsprintf(p_lpszResult, _T("LOWDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 7)	// CLOSE
		::lstrcpy(p_lpszResult, _T("CLOSEDATA(0)"));
	else if(p_nReserved == 8)	// CLOSE, Braket
		::wsprintf(p_lpszResult, _T("CLOSEDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 9)	// VOLUME
		::lstrcpy(p_lpszResult, _T("VOLUMEDATA(0)"));
	else if(p_nReserved == 10)	// VOLUME, Braket
		::wsprintf(p_lpszResult, _T("VOLUMEDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 11)	// AMOUNT
		::lstrcpy(p_lpszResult, _T("AMOUNTDATA)0)"));
	else if(p_nReserved == 12)	// AMOUNT, Braket
		::wsprintf(p_lpszResult, _T("AMOUNTDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 13)	// DATE
		::lstrcpy(p_lpszResult, _T("pSTActor->m_STDataManager.GetVarData(\"Date\",0)"));
	else if(p_nReserved == 14)	// DATE, Braket
		::wsprintf(p_lpszResult, _T("pSTActor->m_STDataManager.GetVarData(\"Date\",%s)"), p_lpszValue);
	else if(p_nReserved == 15)	// TIME
		::lstrcpy(p_lpszResult, _T("pSTActor->m_STDataManager.GetVarData(\"Time\",0)"));
	else if(p_nReserved == 16)	// TIME, Braket
		::wsprintf(p_lpszResult, _T("pSTActor->m_STDataManager.GetVarData(\"Time\",%s)"), p_lpszValue);
	else if(p_nReserved == 17)	// JMCODE
		::lstrcpy(p_lpszResult, _T("JMCODE")); 
	else if(p_nReserved == 18)	// CURRENTDATE
		::lstrcpy(p_lpszResult, _T("CURRENTDATE")); 
	else if(p_nReserved == 19)	// CURRENTTIME
		::lstrcpy(p_lpszResult, _T("CURRENTTIME")); 	
	else if(p_nReserved == 20)	// CURRENTBAR
		::lstrcpy(p_lpszResult, _T("CURRENTBAR")); 	

	return 0;
}

int SysTradingParser::ParseReservedSentence2_CPP(UINT p_nReserved, LPSTR p_lpszValue, EXPR* pExpr)
{
	if(p_nReserved == 1)		// OPEN
		::lstrcpy(pExpr->sStr, _T("OPENDATA(0)"));
	else if(p_nReserved == 2)	// OPEN, Braket
		::wsprintf(pExpr->sStr, _T("OPENDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 3)	// HIGH
		::lstrcpy(pExpr->sStr, _T("HIGHDATA(0)"));
	else if(p_nReserved == 4)	// HIGH, Braket
		::wsprintf(pExpr->sStr, _T("HIGHDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 5)	// LOW
		::lstrcpy(pExpr->sStr, _T("LOWDATA(0)"));
	else if(p_nReserved == 6)	// LOW, Braket
		::wsprintf(pExpr->sStr, _T("LOWDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 7)	// CLOSE
		::lstrcpy(pExpr->sStr, _T("CLOSEDATA(0)"));
	else if(p_nReserved == 8)	// CLOSE, Braket
		::wsprintf(pExpr->sStr, _T("CLOSEDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 9)	// VOLUME
		::lstrcpy(pExpr->sStr, _T("VOLUMEDATA(0)"));
	else if(p_nReserved == 10)	// VOLUME, Braket
		::wsprintf(pExpr->sStr, _T("GETVOLUMEDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 11)	// AMOUNT
		::lstrcpy(pExpr->sStr, _T("AMOUNTDATA)0)"));
	else if(p_nReserved == 12)	// AMOUNT, Braket
		::wsprintf(pExpr->sStr, _T("AMOUNTDATA(%s)"), p_lpszValue);
	else if(p_nReserved == 13)	// DATE
		::lstrcpy(pExpr->sStr, _T("pSTActor->m_STDataManager.GetVarData(\"Date\",0)"));
	else if(p_nReserved == 14)	// DATE, Braket
		::wsprintf(pExpr->sStr, _T("pSTActor->m_STDataManager.GetVarData(\"Date\",%s)"), p_lpszValue);
	else if(p_nReserved == 15)	// TIME
		::lstrcpy(pExpr->sStr, _T("pSTActor->m_STDataManager.GetVarData(\"Time\",0)"));
	else if(p_nReserved == 16)	// TIME, Braket
		::wsprintf(pExpr->sStr, _T("pSTActor->m_STDataManager.GetVarData(\"Time\",%s)"), p_lpszValue);
	else if(p_nReserved == 17)	// JMCODE
		::lstrcpy(pExpr->sStr, _T("JMCODE")); 
	else if(p_nReserved == 18)	// CURRENTDATE
		::lstrcpy(pExpr->sStr, _T("CURRENTDATE")); 
	else if(p_nReserved == 19)	// CURRENTTIME
		::lstrcpy(pExpr->sStr, _T("CURRENTTIME")); 	
	else if(p_nReserved == 20)	// CURRENTBAR
		::lstrcpy(pExpr->sStr, _T("CURRENTBAR")); 	

	pExpr->cExprKind = DTTYPE_INT;
	
	return 0;
}

int SysTradingParser::ParseAssignSentence_CPP(UINT p_nAssign, EXPR* pExpr, LPSTR p_lpszRight, LPSTR p_lpszResult)
{
	if(p_nAssign == 1)
	{
		LPSTR szName = ::strdup(pExpr->sStr);
		::strupr(szName);

		int nIndex = -1;
		if((nIndex = IsInputVariable(szName)) >= 0)
			::wsprintf(p_lpszResult, _T("SETINPUTHOLDERDATA(%d, %s, %s);\n"), nIndex, pExpr->sArg, p_lpszRight);
		else if((nIndex = IsLocalVariable(szName)) >= 0)
			::wsprintf(p_lpszResult, _T("SETVARHOLDERDATA(%d, %s, %s);\n"), nIndex, pExpr->sArg, p_lpszRight);
		else if((nIndex = IsArrayVariable(szName)) >= 0)
			::wsprintf(p_lpszResult, _T("SETARRAYHOLDERDATA(%d, %s, %s);\n"), nIndex, pExpr->sArg, p_lpszRight);
		else if(m_lpszFunctionName && !::lstrcmp(m_lpszFunctionName, szName))
			::wsprintf(p_lpszResult, _T("SETFUNCDATA(0, %s);\n"), p_lpszRight);
		else if(GetFuncStack() ==  FUNCTION_USER)
			::wsprintf(p_lpszResult, _T("SETFUNCDATA(0, %s);\n"), p_lpszRight);

		::free(szName);
	}

	return 0;
}

int SysTradingParser::ParseFunctionSentence_CPP(EXPR* pExpr, LPSTR p_lpszArgument, LPSTR p_lpszResult)
{
	UINT nFuncType = GetFuncStack();
	if(nFuncType == FUNCTION_DEFAULT)
	{
		AddUsedInnerFunction(pExpr->sStr);

		::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, %s%s)"), IsUsedInnerFunction(pExpr->sStr), pExpr->sArg, p_lpszArgument);
		CompareInnerFunctionArgumentList_CPP(p_lpszResult, pExpr->sStr);
//		::wsprintf(p_lpszResult, _T("pSTActor->InitArg();\n%sINNERFUNC(%s);\n"), p_lpszArgument, p_lpszName);
	}
	else if(nFuncType == FUNCTION_MEMBER)
	{
	}
	else if(nFuncType == FUNCTION_USER)
	{
		LPSTR szName = ::strdup(pExpr->sStr);
		::strupr(szName);

		if(m_lpszFunctionName && !::lstrcmp(m_lpszFunctionName, szName))
		{
			::wsprintf(p_lpszResult, _T("GETFUNCDATA(%s)"), pExpr->sArg);
		}
		else
		{
			AddUsedUserFunction(pExpr->sStr);
			
			if(*p_lpszArgument == ',')
				::wsprintf(p_lpszResult, _T("pSTActor->SubFunc((long)pSTActor, %d, %s%s)"), IsUsedUserFunction(pExpr->sStr), pExpr->sArg, p_lpszArgument);
			else
				::wsprintf(p_lpszResult, _T("pSTActor->SubFunc((long)pSTActor, %d, %s, SUBINPUT(0, %s))"), IsUsedUserFunction(pExpr->sStr), pExpr->sArg, p_lpszArgument);
		}

		::free(szName);
	}
	else
	{
		if(!::strncmp(p_lpszResult, _T("FIND"), 4))
			::strset(p_lpszResult, 0x00);
		else
			::wsprintf(p_lpszResult, _T("GETFUNCDATA(%s)"), p_lpszArgument);
	}

	DelFuncStack();

	CString strResult(p_lpszResult);
	strResult.TrimRight();
	if(strResult.Right(1) == _T(","))
	{
		if(m_pErrorInfo)
		{
			int nError = 99999;
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_FUNCPARM_STRATEGY : CM_ERROR_FUNCPARM_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(pExpr->sStr, LEX_GetLineNumber(), nError);
			return nError;
		}
	}

	return 0;
}

int SysTradingParser::ParseFunctionNameSentence_CPP(LPSTR p_lpszName, LPSTR p_lpszResult)
{
	::lstrcpy(p_lpszResult, p_lpszName);

	if(IsDefaultFuncion(p_lpszName))
	{
		AddFuncStack(FUNCTION_DEFAULT);
		SetStrategyType(p_lpszName);
	}
	else if(IsMemberFunction(p_lpszName))
		AddFuncStack(FUNCTION_MEMBER);
	else if(IsUserFuncion(p_lpszName))
	{
		AddFuncStack(FUNCTION_USER);
		AddUserFunction(p_lpszName);
	}
	else
	{
		LPSTR lpszName = ::strdup(p_lpszName);
		::strupr(lpszName);

		if(!::strncmp(lpszName, _T("FIND"), 4))
		{
			SetStrategyType(p_lpszName);

			::lstrcpy(p_lpszResult, lpszName);
			::free(lpszName);
			return 0;
		}
		
		::free(lpszName);

		if(m_pErrorInfo)
		{
			int nError = 99999;
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_UNDECLARE_MEMBER_STRATEGY : CM_UNDECLARE_MEMBER_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszName, LEX_GetLineNumber(), nError);
			return nError;
		}
	}

	return 0;
}

int SysTradingParser::ParseFunctionArgumentSentence_CPP(UINT p_nArgument, LPSTR p_lpszList, EXPR* pExpr, LPSTR p_lpszResult)
{
	UINT nFuncType = GetFuncStack();

	if(p_nArgument == 1)
	{
		if(nFuncType == FUNCTION_DEFAULT)
		{
			int nIndex = -1;
			if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T(", GETINPUTHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg);
			else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T(", GETVARHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg);
			else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T(", GETARRAYHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg);
			else
				::wsprintf(p_lpszResult, _T(", %s"), pExpr->sStr);
		}
		else if(nFuncType == FUNCTION_USER)
		{
			int nIndex = -1;
			if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T(", SUBINPUT(0, GETINPUTHOLDERDATA(%d, %s)))"), nIndex, pExpr->sArg);
			else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T(", SUBINPUT(0, GETVARHOLDERDATA(%d, %s)))"), nIndex, pExpr->sArg);
			else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T(", SUBINPUT(0, GETARRAYHOLDERDATA(%d, %s)"), nIndex, pExpr->sArg);
			else
				::wsprintf(p_lpszResult, _T(", SUBINPUT(0, %s)"), pExpr->sStr);
		}
	}
	else if(p_nArgument == 2)
	{
		CString strFind(_T(""));
		if(nFuncType == FUNCTION_DEFAULT)
		{
			strFind.Format(_T(", %s, %s"), p_lpszList, pExpr->sStr); 
//			ParseDefFunctionIndex(strFind, _T("pSTActor->m_varArg"));
		}
		else if(nFuncType == FUNCTION_USER)
		{
			strFind.Format(_T(", SUBINPUT(0, %s), SUBINPUT(0, %s)"), p_lpszList, pExpr->sStr); 
			ParseFunctionIndex(strFind, _T("SUBINPUT("));
		}

		::lstrcpy(p_lpszResult, strFind);
	}
	else if(p_nArgument == 3)
	{
//		if(m_lStateFlag & FUNCTION_USER)
//		{
			::lstrcpy(p_lpszResult, pExpr->sStr);
//		}
//		else
//			::wsprintf(p_lpszResult, _T("pSTActor->m_varArg01 = %s\n"), pExpr->sStr); 
	}
	else if(p_nArgument == 4)
	{
//		if(m_lStateFlag & FUNCTION_DEFAULT)
//			::wsprintf(p_lpszResult, _T("%s\npSTActor->m_varArg01 = %s;\n"), p_lpszList, pExpr->sStr); 
//		else if(m_lStateFlag & FUNCTION_USER)
			::wsprintf(p_lpszResult, _T("%s, %s"), p_lpszList, pExpr->sStr); 
	}

	return 0;
}

int SysTradingParser::ParseDefaultArgumentListSentence_CPP(LPSTR p_lpszList1, LPSTR p_lpszList2, LPSTR p_lpszResult)
{
	::wsprintf(p_lpszResult, _T("%s, %s"), p_lpszList1, p_lpszList2);	

	return 0;
}

int SysTradingParser::ParseMemberFunctionSentence_CPP(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult)
{
	if(p_nMember == 1)			// CROSS ABOVE
	{
		int nIndex = -1;
		if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSABOVE(GETINPUTHOLDERDATA(%d, %s), %s)"), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSABOVE(GETVARHOLDERDATA(%d, %s), %s)"), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSABOVE(GETARRAYHOLDERDATA(%d, %s), %s)"), nIndex, pExpr->sArg, p_lpszExpr);
	}
	else if(p_nMember == 2)		// CROSS BELOW
	{
		int nIndex = -1;
		if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSBELOW(GETINPUTHOLDERDATA(%d, %s), %s)"), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSBELOW(GETVARHOLDERDATA(%d, %s), %s)"), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSBELOW(GETARRAYHOLDERDATA(%d, %s), %s)"), nIndex, pExpr->sArg, p_lpszExpr);
	}

	return 0;
}

int SysTradingParser::ParseInnerFunctionSentence_CPP(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult)
{
	if(p_nMember == 1)			// CROSS ABOVE
	{
		LPSTR lpszFunction = _T("CROSSABOVE");
		AddUsedInnerFunction(lpszFunction);

		int nIndex = -1;
		if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, GETINPUTHOLDERDATA(%d, %s), %s)"), IsUsedInnerFunction(lpszFunction), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, GETVARHOLDERDATA(%d, %s), %s)"), IsUsedInnerFunction(lpszFunction), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, GETARRAYHOLDERDATA(%d, %s), %s)"), IsUsedInnerFunction(lpszFunction), nIndex, pExpr->sArg, p_lpszExpr);
		else
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, %s, %s)"), IsUsedInnerFunction(lpszFunction), pExpr->sStr, p_lpszExpr);
	}
	else if(p_nMember == 2)		// CROSS BELOW
	{
		LPSTR lpszFunction = _T("CROSSBELOW");
		AddUsedInnerFunction(lpszFunction);

		int nIndex = -1;
		if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, GETINPUTHOLDERDATA(%d, %s), %s)"), IsUsedInnerFunction(lpszFunction), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, GETVARHOLDERDATA(%d, %s), %s)"), IsUsedInnerFunction(lpszFunction), nIndex, pExpr->sArg, p_lpszExpr);
		else if((nIndex = IsArrayVariable(pExpr->sStr)) >= 0)
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, GETARRAYHOLDERDATA(%d, %s), %s)"), IsUsedInnerFunction(lpszFunction), nIndex, pExpr->sArg, p_lpszExpr);
		else
			::wsprintf(p_lpszResult, _T("pSTActor->InnerFunc(g_lSourceType, %d, 0, %s, %s)"), IsUsedInnerFunction(lpszFunction), pExpr->sStr, p_lpszExpr);
	}

	return 0;
}

int SysTradingParser::EndBlock_CPP(LPSTR p_lpszStatment)
{
	UINT nType = GetStack();
	if(nType == 0)
	{
		if(m_pErrorInfo)
		{
			int nError = 99999;
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_ENDSTMTERR_STRATEGY : CM_ERROR_ENDSTMTERR_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(p_lpszStatment, LEX_GetLineNumber(), nError);
			return nError;
		}

		return 0;
	}

	if(nType == STACK_STATEMENT)
	{
		DelStack();
		nType = GetStack();

		if(g_nEnd > 0 && nType == STACK_IF_BLOCK)
			g_nEnd--;
	}
	
	switch(nType)
	{
	case STACK_FOR_BLOCK:
		ChangeStack(STACK_FOR_BLOCKEND);
		break;
	case STACK_IF_BLOCK:
		ChangeStack(STACK_IF_BLOCKEND);
		break;
	case STACK_ELSEIF_BLOCK:
		ChangeStack(STACK_ELSEIF_BLOCKEND);
		break;
	case STACK_ELSE_BLOCK:
		ChangeStack(STACK_ELSE_BLOCKEND);
		break;
	case STACK_WHILE_BLOCK:
		ChangeStack(STACK_WHILE_BLOCKEND);
		break;
	default:
		DelStack();
		EndBlock_CPP(p_lpszStatment);
	}	

	return 0;
}

int SysTradingParser::EndStatment_CPP(LPSTR p_lpszStatment)
{
	UINT nType = GetStack();
	switch(nType)
	{
	case STACK_FOR:
		if(!m_bDontCert)
		{
			::lstrcat(p_lpszStatment, _T("FOREND_\n"));
			::fwrite(p_lpszStatment, 1, ::strlen(p_lpszStatment), SysTradingout);
		}
		DelStack();
		break;
	case STACK_IF:
		if(!m_bDontCert)
		{
			::lstrcpy(p_lpszStatment, _T("IFEND_\n"));
			::fwrite(p_lpszStatment, 1, ::strlen(p_lpszStatment), SysTradingout);
			AddStack(STACK_IF_END);
		}

		if(g_nEnd > 0)
			g_nEnd--;

//		DelStack();
		break;
	case STACK_ELSEIF:
		break;
	case STACK_ELSE:
		{
			if(!m_bDontCert)
			{
				::lstrcpy(p_lpszStatment, _T("IFEND_\n"));
				::fwrite(p_lpszStatment, 1, ::strlen(p_lpszStatment), SysTradingout);
				AddStack(STACK_IF_END);
			}
			
			if(g_nEnd > 0)
				g_nEnd--;
		}
		break;
	case STACK_WHILE:
		break;
	}	

	return 0;
}

int SysTradingParser::BeginUserFunction_CPP(LPSTR p_szFunctionName)
{
//	char szLine[100 + NAME_SZ] = {NULL, };

//	::wsprintf(szLine, "FUNCTIONBEGIN_(%s)\n", p_szFunctionName);
//	::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);

	return 0;
}

int SysTradingParser::EndUserFunction_CPP(LPSTR p_szFunctionName)
{
//	char szLine[100 + NAME_SZ] = {NULL, };

//	::wsprintf(szLine, "FUNCTIONEND_(%s)\n", p_szFunctionName);
//	::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);

	return 0;
}

int SysTradingParser::WriteOneStatementEnd_CPP(LPSTR p_lpszString)
{
	UINT nType = GetStack();
	if(nType == STACK_STATEMENT)
	{
		DelStack();
		EndStatment(p_lpszString);
	}
	else if(nType == STACK_ELSE)
		EndStatment(p_lpszString);
	else
		AddStack(STACK_STATEMENT);

	return 0;
}

int SysTradingParser::IsEndBlock_CPP(BOOL bStatment)
{
	char szLine[100 + NAME_SZ] = {NULL, };

	UINT nType = GetStack();
	switch(nType)
	{
	case STACK_FOR_BLOCKEND:
		if(!m_bDontCert)
		{
			::lstrcpy(szLine, _T("FOREND_\n"));
			::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
		}

		DelStack();
		break;
	case STACK_IF_BLOCKEND:
		if(!m_bDontCert)
		{
			::lstrcpy(szLine, _T("IFEND_\n"));
			::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);

			if(g_nEnd > 0)
				g_nEnd--;
		}

		DelStack();
//		AddStack(STACK_IF_END);
		break;
	case STACK_ELSEIF_BLOCKEND:
		DelStack();
		break;
	case STACK_ELSE_BLOCKEND:
		if(!m_bDontCert)
		{
			::lstrcpy(szLine, _T("IFEND_\n"));
			::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
		}
		DelStack();
		break;
	case STACK_WHILE_BLOCKEND:
		DelStack();
		break;
//	case STACK_STATEMENT:
//		AddStack(STACK_IF_END);
//		break;
	default:
		{
			if(!bStatment)
			{
				AddStack(STACK_IF_END);
				return 0;
			}
		}

		return -1;
	}


	return 0;
}

int SysTradingParser::IsEndOneBlock_CPP()
{
	UINT nType = GetStack();
	if(nType != STACK_STATEMENT)
		return 0;

	char szLine[100 + NAME_SZ] = {NULL, };

	DelStack();
	nType = GetStack();
	switch(nType)
	{
	case STACK_FOR:
		if(!m_bDontCert)
		{
			::lstrcpy(szLine, _T("FOREND_\n"));
			::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
		}

		DelStack();
		break;
	case STACK_IF:
		if(!m_bDontCert)
		{
			::lstrcpy(szLine, _T("IFEND_\n"));
			::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
		}

		DelStack();
//		AddStack(STACK_IF_END);
		break;
	case STACK_ELSEIF:
		DelStack();
		break;
	case STACK_ELSE:
		DelStack();
		break;
	case STACK_WHILE:
		DelStack();
		break;
	default:
		AddStack(STACK_STATEMENT);
	}

	return 0;
}

int SysTradingParser::DelEndBlock_CPP()
{
	UINT nType = GetStack();
	switch(nType)
	{
	case STACK_FOR_BLOCKEND:
	case STACK_IF_BLOCKEND:
	case STACK_ELSEIF_BLOCKEND:
	case STACK_ELSE_BLOCK:
	case STACK_WHILE_BLOCK:
		DelStack();
		break;
	case STACK_IF_BLOCK:
	case STACK_FOR_BLOCK:
	case 0:
		{
			if(g_bDelEndif)
			{
//				char* szDelChar = _T("IFEND_\n");
				::fseek(SysTradingout, -8, SEEK_CUR);

				g_bDelEndif = FALSE;
			}
		}
		break;
	}

	return 0;
}

// 내부함수 파라메터중에 디폴트 파라메터가 존재하는 경우
// 파싱되는 C++코드에 디폴트 파라메터를 삽입해 준다.
void SysTradingParser::CompareInnerFunctionArgumentList_CPP(LPSTR lpszResult, LPSTR lpszName)
{
	CFindInnerFunction findString(lpszName);

	VT_INNERFUNCINFO::iterator it = std::find_if(m_pVtInnerFuncInfo->begin(), m_pVtInnerFuncInfo->end(), findString);
	findString.DeleteString();

	if(it == m_pVtInnerFuncInfo->end())
		return;
	
	CInnerFunctionInfo* pInnerFunction = (*it);		// 현재 파싱된 내부함수
	UINT nArgSize = pInnerFunction->GetFunctionParameterCount();
	if(!nArgSize)	// 파라메터 개수
		return;

	CString strCompare(lpszResult);
	strCompare.Delete(0, strCompare.Find(_T("("), 0) + 1);		// 사용함수의 괄호 부분만 취한다
	strCompare.Delete(strCompare.GetLength() - 1, 1);

	// 새로 만들어질 파싱문자열(내부함수)
	::memset(lpszResult, 0x00, EXPR_SZ);
	::lstrcpy(lpszResult, _T("pSTActor->InnerFunc("));		// 기본 파싱 문자열

	CString strNewResult(strCompare);
	int nIndex = 0;

	// 처음 두개 파라메터는 내부함수의 파라메터가 아닌
	// 스크립트 종류, 내부함수 인덱스 이므로 따로 파싱할거 없이 그대로 삽입
	AfxExtractSubString(strCompare, strNewResult, nIndex++, ',');
	::lstrcat(lpszResult, strCompare);
	::lstrcat(lpszResult, _T(","));
	AfxExtractSubString(strCompare, strNewResult, nIndex++, ',');
	::lstrcat(lpszResult, strCompare);
	::lstrcat(lpszResult, _T(","));
	AfxExtractSubString(strCompare, strNewResult, nIndex++, ',');
	::lstrcat(lpszResult, strCompare);
	::lstrcat(lpszResult, _T(", "));
	
	// 나머지 파라메터들을 검사하여 사용자가 입력한 파라메터 이외의
	// 디폴트 파라메터가 있으면 삽입하는 루틴
	UINT nParen = 0;
	for(UINT i = 0; i < nArgSize; i++)
	{
		// ','를 기준으로 파라메터를 취한다
		AfxExtractSubString(strCompare, strNewResult, nIndex++, ',');
		if(strCompare.GetLength())
		{
			strCompare.Replace(_T(" "), _T(""));
			
			// 파라메터중에서도 다른 함수를 사용한 파라메터는
			// 괄호'('와 또다른 쉼표','가 있으므로 이것을 파싱하여
			// 실제 내부함수의 파라메터와 충돌을 피한다
			int nCompareLen = strCompare.GetLength();
			for(int j = 0; j < nCompareLen; j++)
			{
				if(strCompare[j] == '(')		// 괄호 증가
					nParen++;
				else if(strCompare[j] == ')')	// 괄호 감소
					nParen--;
			}

			::lstrcat(lpszResult, strCompare);

			// 괄호개수가 있다는 것은 파라메터로 다른 함수를 사용한 것이므로
			// 다음 쉼표',' 로 이동하여 새로운 파라메터를 취한다
			if(nParen)
			{
				::lstrcat(lpszResult, _T(", "));
				i--;
				continue;
			}
		}
		else
		{
			// 디폴트 파라메터를 삽입
			LPCSTR lpszDefault = pInnerFunction->GetDefaultArgument(i);
			if(lpszDefault && ::lstrlen(lpszDefault))
				::lstrcat(lpszResult, lpszDefault);
			else
//				Error
				continue;
		}

		if(i == nArgSize - 1)
			::lstrcat(lpszResult, _T(")"));
		else
			::lstrcat(lpszResult, _T(", "));
	}
}

void SysTradingParser::WriteEndStatement_CPP()
{
	int nStack = GetStack();

	if(g_nEnd)
	{
		if(!m_bDontCert)
		{
			char szLine[100 + NAME_SZ] = {NULL, };
			::lstrcpy(szLine, _T("IFEND_\n"));
			
			for(int i = 0; i < g_nEnd; i++)
			{
				::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
//				AddStack(STACK_IF_END);
			}
		}
		
		g_nEnd = 0;
	}
	else if(nStack == STACK_IF_BLOCKEND)
	{
		if(!m_bDontCert)
		{
			char szLine[100 + NAME_SZ] = {NULL, };
			::lstrcpy(szLine, _T("IFEND_\n"));
			::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
		}
		
		DelStack();
	}

/*
	UINT nType = 0;
	while((nType = GetStack()))
	{
		if(nType == STACK_STATEMENT)
		{
			DelStack();
			continue;
		}

		if(IsEndBlock_CPP() < 0)
		{
			if(nType == STACK_IF || nType == STACK_ELSE)
			{
				if(!m_bDontCert)
				{
					char szLine[100 + NAME_SZ] = {NULL, };
					::lstrcpy(szLine, _T("IFEND_\n"));
					::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
				}
			}

			DelStack();
		}
	}
*/
}

int SysTradingParser::ProcessEndStatment_CPP()
{
	UINT nType = GetStack();
	if(nType == STACK_STATEMENT)
	{
		DelStack();
		int nType2 = GetStack();
		if(nType2 == STACK_IF_END)
		{
			if(!m_bDontCert)
			{
				char szLine[100 + NAME_SZ] = {NULL, };
				::lstrcpy(szLine, _T("IFEND_\n"));
				::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
			}
//			DelStack();
		}

		AddStack(nType);
	}

	if(g_nEnd > 0)
	{
		g_nEnd--;

		if(!IsNextTokenElse() && m_pErrorInfo)
		{
			int nError = 99999;
			nError = m_pErrorInfo->GetFileType() == FT_STRATEGY ? CM_ERROR_ENDSTMTERR2_STRATEGY : CM_ERROR_ENDSTMTERR2_FUNCTION;
			m_pErrorInfo->SetErrorLineInfomation(_T(""), LEX_GetLineNumber(), nError);
			return nError;
		}
	}

	return 0;
}

void SysTradingParser::SetStrategyType_CPP(LPSTR p_lpszName)
{
	if(!m_pErrorInfo)
		return;

	LPSTR lpszName = ::strdup(p_lpszName);
	::strupr(lpszName);

	if(!(m_pErrorInfo->GetStrategyType(STRATEGY_FIND)))	// 조건검색
	{
		if(!::strncmp(lpszName, _T("FIND"), 4))
		{
			m_pErrorInfo->SetStrategyType(STRATEGY_FIND);

			::free(lpszName);
			return;
		}
	}

	if(!(m_pErrorInfo->GetStrategyType(STRATEGY_BULL)))	// 강세/약세
	{
		if(!::strncmp(lpszName, _T("PLOTBULL"), 8) || !::strncmp(lpszName, _T("PLOTBEAR"), 8))
		{
			m_pErrorInfo->SetStrategyType(STRATEGY_BULL);

			::free(lpszName);
			return;
		}
	}

	if(!(m_pErrorInfo->GetStrategyType(STRATEGY_ORDER)))	// 주문신호
	{
		if(!::strncmp(lpszName, _T("BUY"), 3) || !::strncmp(lpszName, _T("SELL"), 4))
		{
			m_pErrorInfo->SetStrategyType(STRATEGY_ORDER);

			::free(lpszName);
			return;
		}
	}

	if(!(m_pErrorInfo->GetStrategyType(STRATEGY_ORDER)))	// 주문신호
	{
		if(!::strncmp(lpszName, _T("EXITSHORT"), 9) || !::strncmp(lpszName, _T("EXITLONG"), 8))
		{
			m_pErrorInfo->SetStrategyType(STRATEGY_ORDER);

			::free(lpszName);
			return;
		}
	}

	if(!(m_pErrorInfo->GetStrategyType(STRATEGY_INDEX)))	// 지표
	{
		if(!::strncmp(lpszName, _T("PLOT"), 4))
		{
			m_pErrorInfo->SetStrategyType(STRATEGY_INDEX);

			::free(lpszName);
			return;
		}
	}

	::free(lpszName);
}

void SysTradingParser::SetEndSentence_CPP(BOOL bEnd)
{
	m_bEndSentence = bEnd;
}

int SysTradingParser::IsEndSentence_CPP(LPSTR p_lpszName)
{
	LPSTR szText = new char[::lstrlen(p_lpszName) + 1];
	::lstrcpy(szText, p_lpszName);

	LPSTR szUText = ::strupr(szText);
	if(!::lstrcmp(szUText, _T("ELSE")) || !::lstrcmp(szUText, _T("ELSEIF")))
		m_bEndSentence = TRUE;

	delete [] szText;

	if(m_pErrorInfo && !m_bEndSentence)
		return GetParseErrorIndex(3, _T(" "));

	return 0;
}