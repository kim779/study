// LogicMaker.cpp: implementation of the CLogicMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicMaker.h"
#include "LogicItem.h"
#include "LogicValue.h"
#include "LogicElement.h"
#include "LogicOperation.h"
#include "Stack.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#ifdef _MSC_VER
#define STACK_DUMP(_stack_) \
{ \
	CStack<CToken*>::iterator it = _stack_.begin(); \
 \
	afxDump << "Dump stack = {"; \
	while (it != _stack_.end()) \
	{ \
		afxDump << ((const char*)(*(*it))); \
		afxDump << " "; \
		it++; \
	} \
	afxDump << "}\n"; \
}
#else /* !_MSC_VER */
#define STACK_DUMP(_stack_) \
{ \
	CStack<CToken*>::iterator it = _stack_.begin(); \
 \
	printf("Dump stack = {"); \
	while (it != _stack_.end()) \
	{ \
		printf(((const char*)(*(*it)))); \
		printf(" "); \
		it++; \
	} \
	printf("}\n"); \
}
#endif /* !_MSC_VER */
#else /* !_DEBUG */
#define STACK_DUMP(_stack_)
#endif /* !_DEBUG */



#define ISSIGN(_c_) (_c_ == '-' || _c_ == '+')
#define ISOPERATOR(_c_) (_c_ == '-' || _c_ == '+' || _c_ == '*' || _c_ == '/' || _c_ == '%')
#define ISNUMBER(_c_) (ISSIGN(_c_) || iswdigit(_c_) || _c_ == '.')

#define ISWHITESPACE(c)         isspace((c) & 0x0ff)
#define CASEINSENSITIVE(c1, c2) (tolower((int) c1) - tolower((int) c2))


CLogicMaker::CLogicMaker()
{
	m_bHasError = FALSE;
}

CLogicMaker::~CLogicMaker()
{
	// (2006/2/28 - Seung-Won, Bae) Clear Item.
	clearVector();
}

void CLogicMaker::AddDataItemToUse(LPCTSTR lpKey,CIDataItem* pDataItem)
{
	m_mapDataItem.SetAt(lpKey,pDataItem);
}

CLogicItem *CLogicMaker::EvaluatesByExp(LPCTSTR lpLogic)
{
	m_bHasError = FALSE;
	CString strLogic = lpLogic;
//	strLogic.MakeUpper();
	postfixNotation(strLogic);	

	CVector<CToken*>::iterator it = _postfix.begin();
	CLogicItem* pCurrentLogicItem = NULL;
	CStack<CLogicItem*> stackLogicItem;
	while (it != _postfix.end())
	{
		CToken* token = (*it);
		switch (token->_type)
		{
			case CToken::typeOperand:
				{
					if(ISNUMBER((*token)[0]))
					{
						CLogicValue *pLogicValue = new CLogicValue;
						pLogicValue->SetValue((atof(*token)));
						pCurrentLogicItem = pLogicValue;
					}
					else
					{						
						CIDataItem *pDataItem = NULL;
						CLogicElement *pLogicElement = new CLogicElement;
						BOOL bRetValue = m_mapDataItem.Lookup(*token,pDataItem);
						if(!bRetValue)
						{
							m_bHasError = TRUE;
							m_strError.Format("%s 데이터가 존재하지 않습니다.",(LPCTSTR)*token);
							ASSERT(FALSE);
						}
						pLogicElement->SetItem(pDataItem);
						pCurrentLogicItem = pLogicElement;
					}
					stackLogicItem.push(pCurrentLogicItem);
				}
				break;			
			case CToken::typeOperator:
				{
					CLogicOperation *pLogicOperation = new CLogicOperation;
					CLogicItem* var_b = stackLogicItem.pop();
					CLogicItem* var_a = stackLogicItem.pop();
					pLogicOperation->SetItemLeft(var_a);
					pLogicOperation->SetItemRight(var_b);
					pLogicOperation->SetOperation(token->_dispiid);

					pCurrentLogicItem = pLogicOperation;
					stackLogicItem.push(pCurrentLogicItem);
				}
				break;
			case CToken::typeLParenthesis:
			case CToken::typeRParenthesis:
				{
				}
				break;
		}
		it++;
	}
	if(pCurrentLogicItem->GetParent())
	{
		AfxMessageBox("Error EvaluatesByExp2!!");
	}
	return pCurrentLogicItem;
}

CLogicItem *CLogicMaker::EvaluatesByVal(double dValue)
{
	CLogicValue *pItem = new CLogicValue;
	pItem->SetValue(dValue);	
	return pItem;
}

void CLogicMaker::clearVector()
{
	while (!_postfix.empty())
	{
		delete _postfix.remove(0);
	}
	_postfix.clear();
}

void CLogicMaker::trimLeft(const char*& expr)
{
	while (ISWHITESPACE(*expr))
	{
		expr++;
	}
}

void CLogicMaker::postfixNotation(const char* expr)
{
	CStack<CToken*> stack;

	clearVector();

	char token[128];
	CToken::ttype type = CToken::typeUnknown;

	do {
		switch (type)
		{
			case CToken::typeUnknown:
			case CToken::typeOperator:
			case CToken::typeLParenthesis:
			{
				int dispiid = -1;
				type = tokenizeOperands(expr, token, dispiid);
				if (type == CToken::typeEndOfExpr)
					break;
				if (type == CToken::typeOperand)
				{
					_postfix.push_back(new CToken(token, type, dispiid));
					break;
				}
				stack.push(new CToken(token, type, dispiid));
				break;
			}

			case CToken::typeOperand:
			case CToken::typeRParenthesis:
			{
				int dispiid = -1;
				type = tokenizeOperators(expr, token, dispiid);
				if (type == CToken::typeEndOfExpr)
					break;
				if (type == CToken::typeOperator)
				{
					while (!stack.empty() &&
						priority((*(stack.top()))[0]) >= priority(token[0]))
					{
						_postfix.push_back(stack.pop());
					}
				}
				stack.push(new CToken(token, type, dispiid));
				break;
			}
		}
		if (type == CToken::typeRParenthesis)
		{
			while (TRUE)
			{
				if((*(stack.top()))[0] == '(') break;
				_postfix.push_back(stack.pop());
			}
			_postfix.push_back(stack.pop());
		}
	} while (type != CToken::typeEndOfExpr);

	while (!stack.empty())
	{
		_postfix.push_back(stack.pop());
	}
}


CToken::ttype CLogicMaker::tokenizeOperands(const char*& expr, char *token, int& dispiid)
{
	int len;
	int sign = 0;
	CToken::ttype type;

	trimLeft(expr);

	len = 0;
	type = CToken::typeOperand;

	while (*expr)
	{
		char c = *expr;

		if (ISWHITESPACE(c))
		{
			break;
		}
		else if (c == '(' || c == ')')
		{
			if (len == 0 || ISSIGN(token[len-1])) /* see below */
			{
			}
			else
			{
				break;
			}
		}
		else if (c == '+' || c == '-')
		{
			if (len == 0) /* sign */
			{
				sign = 1;
			}
			else
			{
				break;
			}
		}
		else if (c == '*' || c == '/' || c == '%')
		{
			if (len == 0)
			{
				raise_exception(__LINE__, "Operand syntex error - [%s]", expr);
			}
			else
			{
				break;
			}
		}
		else if (isleadbyte(c))
		{
			token[len++] = *expr++;
		}

		token[len++] = *expr++;

		if (c == '(')
		{
			type = CToken::typeLParenthesis;
			break;
		}
		if (c == ')')
		{
			type = CToken::typeRParenthesis;
			break;
		}
	}

	token[len] = '\0';

	return (len == 0) ? CToken::typeEndOfExpr : type;
}

CToken::ttype CLogicMaker::tokenizeOperators(const char*& expr, char *token, int& dispiid)
{
	int len;
	CToken::ttype type;

	trimLeft(expr);

	len = 0;
	type = CToken::typeOperator;

	if (*expr)
	{
		char c = *expr;

		if (c == '+')
		{
			dispiid = DISPIID_ADD;
		}
		else if (c == '-')
		{
			dispiid = DISPIID_SUB;
		}
		else if (c == '*')
		{
			dispiid = DISPIID_MUL;
		}
		else if (c == '/')
		{
			dispiid = DISPIID_DIV;
		}
		else if (c == '(')
		{
			type = CToken::typeLParenthesis;
		}
		else if (c == ')')
		{
			type = CToken::typeRParenthesis;
		}
		else
		{
			raise_exception(__LINE__, "Operator syntex error - [%s]", expr);
		}

		token[len++] = *expr++;
	}

	token[len] = '\0';

	return (len == 0) ? CToken::typeEndOfExpr : type;
}

int CLogicMaker::priority(int op)
{ 
	if  (op == '(')
		return  0;
	if  (op == '+' || op == '-')
		return  1;
	if  (op == '*' || op == '/' || op == '%')
		return  2;
	return  3;
} 

LPCTSTR CLogicMaker::GetErrorMessage()
{
	if(m_bHasError)
	{
		return m_strError;
	}
	return NULL;
}