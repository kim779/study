// LogicMaker2.cpp: implementation of the CLogicMaker22 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicMaker2.h"
#include "LogicItem.h"
#include "LogicST.h"
#include "LogicOperation.h"
#include "Stack.h"
#include "../Common_ST/STConfigDef.h"
#include "../Common_ST/STControllerDef.h"

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
#define ISNUMBER(_c_) (ISSIGN(_c_) || isdigit(_c_) || _c_ == '.')

#define ISWHITESPACE(c)         isspace((c) & 0x0ff)
#define CASEINSENSITIVE(c1, c2) (tolower((int) c1) - tolower((int) c2))

CLogicMaker2::CLogicMaker2()
{
	m_pArrayPath = NULL;
}

CLogicMaker2::~CLogicMaker2()
{
	clearVector();
}

// 20060705 함수인자 제거 : ,HWND hSocket/*,long pDBMgr*/
//CLogicItem *CLogicMaker2::Evaluates(LPCTSTR lpLogic,LPCTSTR strSTPath,HWND hParent,HWND hSocket/*,long pDBMgr*/)
CLogicItem *CLogicMaker2::Evaluates(LPCTSTR lpLogic,LPCTSTR strSTPath,HWND hParent,long pChartItem)
{
	CString strLogic = lpLogic;
	strLogic.MakeUpper();
	postfixNotation(strLogic);	//연산자는 뒤로 빼서.. 두 개의 값을 연산하는 루틴 : 계산기 루틴
	long lIndexOfElement = 0;
	CString strSDName;
	CString strSDPath;
	CString strSDTime;
	BOOL bTrue = TRUE;
	long lPosOfElement = 0;
	long lTypeOfElement = 0;
	CString strKeyName;
	char szBuffer[256];
	int nUseExt = 0;
	int nType = 0;
	int nBongCount = 0;

	CVector<CToken*>::iterator it = _postfix.begin();
	CLogicItem* pCurrentLogicItem = NULL;
	CStack<CLogicItem*> stackLogicItem;
	BOOL bResultOfInit = FALSE;
	while (it != _postfix.end())
	{
		CToken* token = (*it);
		switch (token->_type)
		{
			case CToken::typeOperand:
				{					
					strKeyName.Format("%s%d",STRATEGYPOS,lIndexOfElement);
					::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
					lPosOfElement = atol(szBuffer);

					//SD 파일명을 가져온다.
					strKeyName.Format("%s%d",STRATEGYPOSNAME,lIndexOfElement);
					::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
					strSDName = szBuffer;

					strKeyName.Format("%s%d",STRATEGYPOSTYPE,lIndexOfElement);
					::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
					lTypeOfElement = atol(szBuffer);

					strKeyName.Format("%s%d",STRATEGYPOSTIME,lIndexOfElement);
					::GetPrivateProfileString(STRATEGYLIST,strKeyName,"",szBuffer,sizeof(szBuffer),strSTPath);
					strSDTime = szBuffer;

					strSDPath = m_pArrayPath->GetAt(lTypeOfElement) + "\\" + strSDName + ".sd";
					
					if((*token)[0]=='!')
					{
						bTrue = FALSE;
					}
					else
					{
						bTrue = TRUE;
					}					

					CLogicST* pLogicST = new CLogicST;
					pLogicST->SetType(1);

					strKeyName.Format("%s%d", VARIABLES, lIndexOfElement);
					nUseExt = ::GetPrivateProfileInt(strKeyName, USEEXT, 0, strSTPath);

					strKeyName.Format("%s%d", VARIABLES, lIndexOfElement);
					::GetPrivateProfileString(strKeyName, USEEXTDATA, "", szBuffer, sizeof(szBuffer), strSTPath);
					sscanf(szBuffer, "%d;%d;", &nBongCount, &nType);

					// 
					pLogicST->SetConfig(nUseExt, nType, nBongCount);

					// 20060705 함수인자 제거 : ,hSocket/*,pDBMgr*/
					//bResultOfInit = pLogicST->Init(strSDPath,lPosOfElement,bTrue,strSDTime,strSTPath,hParent,hSocket/*,pDBMgr*/);
					//SD파일을 초기화 하고, SD파일이 제공하는 함수를 Load한다.:KHD 
					bResultOfInit = pLogicST->Init(strSDPath,lPosOfElement,bTrue,strSDTime,strSTPath,hParent,pChartItem);//KHD : ChartItem

					if(!bResultOfInit)// KHD : Test
					{
						return NULL;
						break;
					}

					lIndexOfElement++;

					pCurrentLogicItem = pLogicST;
					stackLogicItem.push(pCurrentLogicItem);//스택에 로드한 SD 파일을 저장한다.
				}
				break;			
			case CToken::typeOperator:
				{
					CLogicOperation *pLogicOperation = new CLogicOperation;
					pLogicOperation->SetType(2);
					CLogicItem* var_b = stackLogicItem.pop();
					CLogicItem* var_a = stackLogicItem.pop();
					pLogicOperation->SetItemLeft(var_a);
					pLogicOperation->SetItemRight(var_b);
					pLogicOperation->SetOperation(token->_dispiid);

					pCurrentLogicItem = pLogicOperation;
					//맨 마지막엔 로드된 SD파일 및 연산자를 가지고 있는 클래스주소를 저장한다.
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
	return pCurrentLogicItem;//연산할 정보를 가지고 있는 Class를 반환한다.
}


void CLogicMaker2::clearVector()
{
	while (!_postfix.empty())
	{
		delete _postfix.remove(0);
	}
	_postfix.clear();
}

void CLogicMaker2::trimLeft(const char*& expr)
{
	while (ISWHITESPACE(*expr))
	{
		expr++;
	}
}

void CLogicMaker2::postfixNotation(const char* expr)
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
					while (!stack.empty() &&priority((*(stack.top()))[0]) >= priority(token[0]))
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


CToken::ttype CLogicMaker2::tokenizeOperands(const char*& expr, char *token, int& dispiid)
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
		else if (c == '&' || c == '|' || c == '>')
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


//#define ANDINLOGIC		0
//#define ORINLOGIC			1
//#define	NEXTINLOGIC		2

CToken::ttype CLogicMaker2::tokenizeOperators(const char*& expr, char *token, int& dispiid)
{
	int len;
	CToken::ttype type;

	trimLeft(expr);

	len = 0;
	type = CToken::typeOperator;

	if (*expr)
	{
		char c = *expr;

		if (c == '&')
		{
			dispiid = DISPIID_AND;
		}
		else if (c == '|')
		{
			dispiid = DISPIID_OR;
		}
		else if (c == '>')
		{
			dispiid = DISPIID_NEXT;
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

int CLogicMaker2::priority(int op)
{ 
	if  (op == '(')
		return  0;
	if  (op == '>')
		return  1;
	if  (op == '|')
		return  2;
	if  (op == '&')
		return  3;
	return  4;
} 
