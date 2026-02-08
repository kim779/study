// LogicMaker.h: interface for the CLogicMaker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICMAKER_H__316419B6_BF27_4F61_A3CD_8E3B7A02C6EC__INCLUDED_)
#define AFX_LOGICMAKER_H__316419B6_BF27_4F61_A3CD_8E3B7A02C6EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "Token.h"
#include "Vector.h"
#include "../Common_ST/IDataItem.h"
class CLogicItem;
class CLogicMaker  
{
public:
	CLogicMaker();
	void AddDataItemToUse(LPCTSTR lpKey,CIDataItem* pDataItem);
	CLogicItem *EvaluatesByExp(LPCTSTR lpLogic);
	CLogicItem *EvaluatesByVal(double dValue);
	LPCTSTR GetErrorMessage();
	virtual ~CLogicMaker();
private:	
	void clearVector();
	void trimLeft(const char*& expr);
	void postfixNotation(const char* expr);
	inline int  priority(int op);

	CToken::ttype tokenizeOperators(const char*& expr, char *token, int& dispiid);
	CToken::ttype tokenizeOperands(const char*& expr, char *token, int& dispiid);
	CVector<CToken*> _postfix;
	CMap<CString, LPCSTR, CIDataItem*, CIDataItem*> m_mapDataItem;
	CString m_strError;
	BOOL m_bHasError;
};

#endif // !defined(AFX_LOGICMAKER_H__316419B6_BF27_4F61_A3CD_8E3B7A02C6EC__INCLUDED_)
