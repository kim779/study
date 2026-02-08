#include "stdafx.h"

#include "SysTradingParser.h"
#include "SysTrading.cpp.h"

extern FILE *SysTradingin;
extern FILE *SysTradingout;

void SysTradingParser::WriteGeneralSentence_VBS(LPSTR p_lpszString)
{
	::strcat(p_lpszString, "\n");
	EndStatment(p_lpszString);

	::fwrite(p_lpszString, 1, ::strlen(p_lpszString), SysTradingout);
}

void SysTradingParser::WriteAssignSentence_VBS(LPSTR p_lpszString)
{
	EndStatment(p_lpszString);
	::fwrite(p_lpszString, 1, ::strlen(p_lpszString), SysTradingout);
}

void SysTradingParser::WriteDeclareSentence_VBS(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
/*
	if(p_cType & DTTYPE_INT)
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, %s\n"), m_niptCount++, p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_DOUBLE)
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, %s\n"), m_niptCount++, p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_STRING)
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, %s\n"), m_niptCount++, p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_BOOL)
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, %s\n"), m_niptCount++, p_lpszName, TRUEFALSE, p_lpszData);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	int nIndex = m_vtInputVariable.size();

	::wsprintf(p_lpszResult, _T("DM.SetInputHolder %d, \"%s\"\nDim %s\n%s = DM.InputHolderData(%d, 0)\n"), 
		nIndex, p_lpszName,
		p_lpszName,
		p_lpszName, nIndex);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);
*/

//	AddInputVariable(p_lpszName);
}

void SysTradingParser::WriteVariableSentence_VBS(UCHAR p_cType, LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
/*
	if(p_cType & DTTYPE_INT)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_DOUBLE)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_STRING)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_BOOL)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, TRUEFALSE, p_lpszData);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	int nIndex = m_vtLocalVariable.size();

	::wsprintf(p_lpszResult, _T("DM.SetVarHolder %d, \"%s\"\nDim %s\n%s = %s\n"), 
		nIndex, p_lpszName,
		p_lpszName,
		p_lpszName, p_lpszData);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);
*/

//	AddLocalVariable(p_lpszName);
}

void SysTradingParser::WriteArraySentence_VBS(UCHAR p_cType, LPSTR p_lpszName, LPSTR p_lpszArraySize, EXPR* pExpr, LPSTR p_lpszResult)
{
/*
	if(p_cType & DTTYPE_INT)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_DOUBLE)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_STRING)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, NumericSeries, p_lpszData);
	else if(p_cType & DTTYPE_BOOL)
		::wsprintf(p_lpszResult, _T("DM.SetVariables \"%s\", %d, %s\n"), p_lpszName, TRUEFALSE, p_lpszData);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	int nIndex = m_vtLocalVariable.size();

	::wsprintf(p_lpszResult, _T("DM.SetVarHolder %d, \"%s\"\nDim %s\n%s = %s\n"), 
		nIndex, p_lpszName,
		p_lpszName,
		p_lpszName, p_lpszData);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

	AddLocalVariable(p_lpszName);
*/
}

void SysTradingParser::WriteDefaultArgumentListSentence_VBS(UCHAR p_cArgument, LPSTR p_lpszName, LPSTR p_lpszResult)
{
	if(IS_STRING(p_cArgument))
	{
	}
	else if(IS_INT(p_cArgument))	/* int 도 double로함. */
	{
	}
	else if(IS_SIMPLE(p_cArgument))
	{
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, 0\n"), m_niptCount++, p_lpszName, NumericSeries);
	}
	else if(IS_DOUBLE(p_cArgument))
	{
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, 0\n"), m_niptCount++, p_lpszName, NumericSeries);
	}
	else if(IS_BOOL(p_cArgument))
	{
		::wsprintf(p_lpszResult, _T("DM.SetInputs %d, \"%s\", %d, 0\n"), m_niptCount++, p_lpszName, TRUEFALSE);
	}

	if(IS_ARRAY(p_cArgument))
	{
//		::lstrcat(p_lpszResult, "*");
	}

	::fwrite(p_lpszResult, 1, ::strlen(p_lpszResult), SysTradingout);

	int nIndex = m_vtInputVariable.size();

	::wsprintf(p_lpszResult, _T("DM.SetInputHolder %d, \"%s\"\nDim %s\n%s = DM.InputHolderData(%d, 0)\n"), 
		nIndex, p_lpszName,
		p_lpszName,
		p_lpszName, nIndex);

	::fwrite(p_lpszResult, 1, ::lstrlen(p_lpszResult), SysTradingout);

//	AddInputVariable(p_lpszName);
}

void SysTradingParser::WriteForStartSentence_VBS(EXPR* pExpr, LPSTR p_lpszBeginArg, LPSTR p_lpszEndArg, LPSTR p_lpszResult)
{
/*
	if(m_vtStackProc.empty())
	{
		LPCSTR szSearchUserFunction = _T("'SEARCH_USERFUNCTION_DONOT_ERASE_THIS_LINE\n");
		::fwrite(szSearchUserFunction, 1, ::strlen(szSearchUserFunction), SysTradingout);

		m_bUsedUserFunction = FALSE;
	}

	::wsprintf(p_lpszResult, _T("'DM.ForIn\nFor %s = %s To %s\n'DM.BracketIn\n"), 
		p_lpszName, p_lpszBeginArg, p_lpszEndArg);

	::fwrite(p_lpszResult, 1, ::strlen(p_lpszResult), SysTradingout);
*/
}

void SysTradingParser::WriteIfStartSentence_VBS(UINT p_nIF, LPSTR p_lpszCondition, LPSTR p_lpszResult)
{
	if(m_vtStackProc.empty())
	{
		LPCSTR szSearchUserFunction = _T("'SEARCH_USERFUNCTION_DONOT_ERASE_THIS_LINE\n");
		::fwrite(szSearchUserFunction, 1, ::strlen(szSearchUserFunction), SysTradingout);

		m_bUsedUserFunction = FALSE;
	}

	if(p_nIF == 1)
		::wsprintf(p_lpszResult, _T("'DM.IfIn\nIf DM.IfCondition(%s) Then\n'DM.BracketIn\n"), p_lpszCondition);
	else if(p_nIF == 2)
		::lstrcpy(p_lpszResult, _T("Else\n'DM.BracketIn\n"));
	
	::fwrite(p_lpszResult, 1, ::strlen(p_lpszResult), SysTradingout);
}

void SysTradingParser::ParseConditionSentence_VBS(LPSTR p_lpszCondition1, LPSTR p_lpszCondition2, LPSTR p_lpszResult, UINT nCondition)
{
	if(nCondition == 1)			// Semicolon
		::wsprintf(p_lpszResult, _T("%s;"), p_lpszCondition1);
	else if(nCondition == 2)	// Parenthesis
		::wsprintf(p_lpszResult, _T("(%s)"), p_lpszCondition1);
	else if(nCondition == 3)	// Equal
		::wsprintf(p_lpszResult,  _T("%s = %s"), p_lpszCondition1, p_lpszCondition2);
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
}

void SysTradingParser::ParseOperatorSentence_VBS(LPSTR p_lpszOperator1, LPSTR p_lpszOperator2, LPSTR p_lpszResult, UINT nOperator)
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
}

void SysTradingParser::ParseNameSentence_VBS(EXPR* pExpr, LPSTR p_lpszResult)
{
	int nIndex = -1;

	if(IsLocalVariable(pExpr->sStr) >= 0 || IsInputVariable(pExpr->sStr) >= 0)
		::lstrcpy(p_lpszResult, pExpr->sStr);
	else
	{
		if(IsMemberFunction(pExpr->sStr))
		{
			m_lStateFlag |= FUNCTION_MEMBER;	// 멤버 함수라고 판단
			::wsprintf(p_lpszResult, _T("DM.%s()"), pExpr->sStr); 
		}
		else
		{
			m_lStateFlag |= FUNCTION_USER;		// 사용자 함수라고 판단
			::wsprintf(p_lpszResult, _T("%s()"), pExpr->sStr); 
			AddUserFunction(pExpr->sStr);
		}
	}
}

void SysTradingParser::ParseArraySentence_VBS(LPSTR p_lpszName, LPSTR p_lpszArray, LPSTR p_lpszResult)
{
	int nIndex = -1;
	if((nIndex = IsLocalVariable(p_lpszName)) >= 0)
		::wsprintf(p_lpszResult, _T("DM.VarHolderData(%d, %s)"), nIndex, p_lpszArray);
	else if((nIndex = IsInputVariable(p_lpszName)) >= 0)
		::wsprintf(p_lpszResult, _T("DM.InputHolderData(%d, %s)"), nIndex, p_lpszArray);
}

void SysTradingParser::ParseReservedSentence_VBS(UINT p_nReserved, LPSTR p_lpszValue, LPSTR p_lpszResult)
{
	if(p_nReserved == 1)		// OPEN
	{
		::lstrcpy(p_lpszResult, _T("Open"));
		m_nUsedMemberVariable |= MEMVAR_OPEN;
	}
	else if(p_nReserved == 2)	// OPEN, Braket
		::wsprintf(p_lpszResult, _T("DM.OpenData(%s)"), p_lpszValue);
	else if(p_nReserved == 3)	// HIGH
	{
		::lstrcpy(p_lpszResult, _T("High"));
		m_nUsedMemberVariable |= MEMVAR_HIGH;
	}
	else if(p_nReserved == 4)	// HIGH, Braket
		::wsprintf(p_lpszResult, _T("DM.HighData(%s)"), p_lpszValue);
	else if(p_nReserved == 5)	// LOW
	{
		::lstrcpy(p_lpszResult, _T("Low"));
		m_nUsedMemberVariable |= MEMVAR_LOW;
	}
	else if(p_nReserved == 6)	// LOW, Braket
		::wsprintf(p_lpszResult, _T("DM.LowData(%s)"), p_lpszValue);
	else if(p_nReserved == 7)	// CLOSE
	{
		::lstrcpy(p_lpszResult, _T("Close"));
		m_nUsedMemberVariable |= MEMVAR_CLOSE;
	}
	else if(p_nReserved == 8)	// CLOSE, Braket
		::wsprintf(p_lpszResult, _T("DM.CloseData(%s)"), p_lpszValue);
	else if(p_nReserved == 9)	// VOLUME
	{
		::lstrcpy(p_lpszResult, _T("Volume"));
		m_nUsedMemberVariable |= MEMVAR_VOLUME;
	}
	else if(p_nReserved == 10)	// VOLUME, Braket
		::wsprintf(p_lpszResult, _T("DM.VolumeData(%s)"), p_lpszValue);
	else if(p_nReserved == 11)	// AMOUNT
	{
		::lstrcpy(p_lpszResult, _T("Amount"));
		m_nUsedMemberVariable |= MEMVAR_AMOUNT;
	}
	else if(p_nReserved == 12)	// AMOUNT, Braket
		::wsprintf(p_lpszResult, _T("DM.AmountData(%s)"), p_lpszValue);
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
		::lstrcpy(p_lpszResult, _T("giCurrentDate")); 
	else if(p_nReserved == 19)	// CURRENTTIME
		::lstrcpy(p_lpszResult, _T("giCurrentTime")); 	
}

void SysTradingParser::ParseAssignSentence_VBS(UINT p_nAssign, EXPR* pExpr, LPSTR p_lpszRight, LPSTR p_lpszResult)
{
/*
	if(p_nAssign == 1)
	{
		int nIndex = -1;
		if(IsInputVariable(p_lpszLeft) >= 0 || IsLocalVariable(p_lpszLeft) >= 0)
			::wsprintf(p_lpszResult, _T("%s = %s\n"), p_lpszLeft, p_lpszRight);
		else if(m_lpszFunctionName && !::lstrcmp(m_lpszFunctionName, p_lpszLeft))
			::wsprintf(p_lpszResult, _T("%s = %s\n"), m_lpszFunctionName, p_lpszRight);
		else if(m_lStateFlag & FUNCTION_USER)
			::wsprintf(p_lpszResult, _T("%s = %s\n"), p_lpszLeft, p_lpszRight);
	}
*/
}

void SysTradingParser::ParseFunctionSentence_VBS(EXPR* pExpr, LPSTR p_lpszArgument, LPSTR p_lpszResult)
{
	if(m_lStateFlag & FUNCTION_DEFAULT)
	{
		m_lStateFlag &= ~FUNCTION_DEFAULT;

		::wsprintf(p_lpszResult, _T("DM.Func \"%s\", %s\n"), pExpr->sStr, p_lpszArgument);
	}
	else if(m_lStateFlag & FUNCTION_MEMBER)
	{
		m_lStateFlag &= ~FUNCTION_MEMBER;
	}
	else if(m_lStateFlag & FUNCTION_USER)
	{
		m_lStateFlag &= ~FUNCTION_USER;
		
		::wsprintf(p_lpszResult, _T("%s(%s))"), pExpr->sStr, p_lpszArgument);
	}
}

void SysTradingParser::ParseFunctionNameSentence_VBS(LPSTR p_lpszName, LPSTR p_lpszResult)
{
	::lstrcpy(p_lpszResult, p_lpszName);

	if(IsDefaultFuncion(p_lpszName))
		m_lStateFlag |= FUNCTION_DEFAULT;
	else if(IsMemberFunction(p_lpszName))
		m_lStateFlag |= FUNCTION_MEMBER;
	else
	{
		m_lStateFlag |= FUNCTION_USER;
		AddUserFunction(p_lpszName);
	}
}

void SysTradingParser::ParseFunctionArgumentSentence_VBS(UINT p_nArgument, LPSTR p_lpszList, EXPR* pExpr, LPSTR p_lpszResult)
{
	if(p_nArgument == 1)
	{
		if(m_lStateFlag & FUNCTION_DEFAULT)
		{
			int nIndex = -1;
			if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T("DM.InputHolderData(%d, 0)\n"), nIndex);
			else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T("DM.VarHolderData(%d, 0)\n"), nIndex);
			else
				::wsprintf(p_lpszResult, _T("%s\n"), pExpr->sStr);
		}
		else if(m_lStateFlag & FUNCTION_USER)
		{
			int nIndex = -1;
			if((nIndex = IsInputVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T("DM.SetSubInputsFrom(1, DM.InputHolderData(%d, 0)\n"), nIndex);
			else if((nIndex = IsLocalVariable(pExpr->sStr)) >= 0)
				::wsprintf(p_lpszResult, _T("DM.SetSubInputsFrom(1, DM.VarHolderData(%d, 0)\n"), nIndex);
		}
	}
	else if(p_nArgument == 2)
	{
		CString strFind(_T(""));
		if(m_lStateFlag & FUNCTION_DEFAULT)
		{
			strFind.Format(_T("%s, %s"), p_lpszList, pExpr->sStr); 
//			ParseDefFunctionIndex(strFind, _T("pSTActor->m_varArg"));
		}
		else if(m_lStateFlag & FUNCTION_USER)
		{
			strFind.Format(_T("DM.SetSubInputsFrom(0, %s), DM.SetSubInputsFrom(0, %s)"), p_lpszList, pExpr->sStr); 
			ParseFunctionIndex(strFind, _T("SetSubInputsFrom("));
		}

		::lstrcpy(p_lpszResult, strFind);
	}
	else if(p_nArgument == 3)
	{
		if(m_lStateFlag & FUNCTION_USER)
		{
			::lstrcpy(p_lpszResult, pExpr->sStr);
		}
		else
			::wsprintf(p_lpszResult, _T("%s"), pExpr->sStr); 
	}
	else if(p_nArgument == 4)
	{
		if(m_lStateFlag & FUNCTION_DEFAULT)
			::wsprintf(p_lpszResult, _T("%s, %s"), p_lpszList, pExpr->sStr); 
		else if(m_lStateFlag & FUNCTION_USER)
			::wsprintf(p_lpszResult, _T("%s, %s"), p_lpszList, pExpr->sStr); 
	}
}

void SysTradingParser::ParseDefaultArgumentListSentence_VBS(LPSTR p_lpszList1, LPSTR p_lpszList2, LPSTR p_lpszResult)
{
	::wsprintf(p_lpszResult, _T("%s, %s"), p_lpszList1, p_lpszList2);	
}

void SysTradingParser::ParseMemberFunctionSentence_VBS(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult)
{
/*
	if(p_nMember == 1)			// CROSS ABOVE
	{
		int nIndex = -1;
		if((nIndex = IsInputVariable(p_lpszName)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSABOVE(DM.InputHolderData(%d, 0), %s)"), nIndex, p_lpszExpr);
		else if((nIndex = IsLocalVariable(p_lpszName)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSABOVE(DM.VarHolderData(%d, 0), %s)"), nIndex, p_lpszExpr);
	}
	else if(p_nMember == 2)		// CROSS BELOW
	{
		int nIndex = -1;
		if((nIndex = IsInputVariable(p_lpszName)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSBELOW(DM.InputHolderData(%d, 0), %s)"), nIndex, p_lpszExpr);
		else if((nIndex = IsLocalVariable(p_lpszName)) >= 0)
			::wsprintf(p_lpszResult, _T("CROSSBELOW(DM.VarHolderData(%d, 0), %s)"), nIndex, p_lpszExpr);
	}
*/
}

void SysTradingParser::EndBlock_VBS(LPSTR p_lpszStatment)
{
	UINT nType = GetStack();
	switch(nType)
	{
	case STACK_FOR_BLOCK:
		::lstrcpy(p_lpszStatment, _T("'DM.BracketOut\nNext\n'DM.ForOut\n"));
		::fwrite(p_lpszStatment, 1, ::strlen(p_lpszStatment), SysTradingout);
		DelStack();
		break;
	case STACK_IF_BLOCK:
		::lstrcpy(p_lpszStatment, _T("'DM.BracketOut\nEnd If\n'DM.IfOut\n"));
		::fwrite(p_lpszStatment, 1, ::strlen(p_lpszStatment), SysTradingout);
		DelStack();
		break;
	case STACK_ELSEIF_BLOCK:
		break;
	case STACK_ELSE_BLOCK:
		::lstrcpy(p_lpszStatment, _T("'DM.BracketOut\nEnd If\n'DM.IfOut\n"));
		::fwrite(p_lpszStatment, 1, ::strlen(p_lpszStatment), SysTradingout);
		DelStack();
//		DelStack();		// STACK_IF 까지 삭제
		break;
	case STACK_WHILE_BLOCK:
		break;
	}	
}

void SysTradingParser::EndStatment_VBS(LPSTR p_lpszStatment)
{
	UINT nType = GetStack();
	switch(nType)
	{
	case STACK_FOR:
		::lstrcat(p_lpszStatment, _T("'DM.BracketOut\nNext\n'DM.ForOut\n"));
		DelStack();
		break;
	case STACK_IF:
//		::lstrcat(p_lpszStatment, _T("'DM.BracketOut\n"));
		DelStack();
		break;
	case STACK_ELSEIF:
		break;
	case STACK_ELSE:
		::lstrcat(p_lpszStatment, _T("'DM.BracketOut\nEnd If\n'DM.IfOut\n"));
		DelStack();
		EndStatment(p_lpszStatment);
//		DelStack();		// STACK_IF 까지 삭제
		break;
	case STACK_WHILE:
		break;
	}	

	UINT nTemp = GetStack();
	if(nType && !GetStack() && m_bUsedUserFunction)
	{
		m_bUsedUserFunction = FALSE;
		LPCSTR szSearchUserFunction = _T("'USED_USERFUNCTION_DONOT_ERASE_THIS_LINE\n");
		::lstrcat(p_lpszStatment, szSearchUserFunction);
	}
}

void SysTradingParser::BeginUserFunction_VBS(LPSTR p_szFunctionName)
{
/*
	char szLine[100 + NAME_SZ] = {NULL, };
	char szParameter[150] = {NULL, };

	for(int i = 0; i < nParam; i++)
	{
		if(i == 0)
			::wsprintf(szLine, _T("str%d"), i + 1);
		else
			::wsprintf(szLine, _T(", str%d"), i + 1);

		::strcat(szParameter, szLine);
	}
	
	::wsprintf(szLine, "\nFunction %s(%s)\n", p_szFunctionName, szParameter);
	::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);

	::wsprintf(szLine, _T("DM.SubIn\nDM.SetVariables \"%s\", %d, 0\nDM.SetFuncDataHolder \"%s\"\n"), 
		p_szFunctionName, NumericSeries, p_szFunctionName);
	::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
*/
}

void SysTradingParser::EndUserFunction_VBS(LPSTR p_szFunctionName)
{
	char szLine[100 + NAME_SZ] = {NULL, };

	VT_DECLDATA::iterator it = m_vtLocalVariable.begin();
	while(it != m_vtLocalVariable.end())
	{
		::wsprintf(szLine, _T("DM.VarHolderData(%d, 0) = %s\n"), (*it)->GetDeclareIndex(), (*it)->GetDeclareOriginString());
		::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
		it++;
	}

	::wsprintf(szLine, _T("\nDM.FuncData(0) = %s\nDM.SubOut\nEnd Function\n"), p_szFunctionName);
	::fwrite(szLine, 1, ::strlen(szLine), SysTradingout);
}

void SysTradingParser::ParseInnerFunctionSentence_VBS(UINT p_nMember, EXPR* pExpr, LPSTR p_lpszExpr, LPSTR p_lpszResult)
{
}

void SysTradingParser::WriteOneStatementEnd_VBS(LPSTR p_lpszString)
{
}

void SysTradingParser::IsEndBlock_VBS()
{
}

void SysTradingParser::IsEndOneBlock_VBS()
{
}

void SysTradingParser::DelEndBlock_VBS()
{
}

void SysTradingParser::ParseReservedSentence2_VBS(UINT p_nReserved, LPSTR p_lpszValue, EXPR* pExpr)
{
}

void SysTradingParser::WriteNameVariableSentence_VBS(LPSTR p_lpszName, EXPR* pExpr, LPSTR p_lpszResult)
{
}

void SysTradingParser::WriteEndStatement_VBS()
{
}