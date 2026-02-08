// LogicMaker2.h: interface for the CLogicMaker2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICMAKER2_H__31A2619A_049C_4F64_B998_0982C1B1E79F__INCLUDED_)
#define AFX_LOGICMAKER2_H__31A2619A_049C_4F64_B998_0982C1B1E79F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogicItem.h"
#include "Token.h"
#include "Vector.h"
class CDBMgr;
class CLogicMaker2  
{
public:
	CLogicMaker2();
	virtual ~CLogicMaker2();
//	void AddDataItemToUse(LPCTSTR lpKey,CIDataItem* pDataItem);

// 20060705 함수인자 제거 : ,HWND hSocket/*,long pDBMgr*/
//	CLogicItem *Evaluates(LPCTSTR lpLogic,LPCTSTR strSTPath,HWND hParent,HWND hSocket/*,long pDBMgr*/);
	CLogicItem *Evaluates(LPCTSTR lpLogic,LPCTSTR strSTPath,HWND hParent, long pChartItem=NULL);

	void SetArrayPath(CStringArray *pStArray) { m_pArrayPath = pStArray;}
private:	
	void clearVector();
	void trimLeft(const char*& expr);
	void postfixNotation(const char* expr);
	inline int  priority(int op);

	CToken::ttype tokenizeOperators(const char*& expr, char *token, int& dispiid);
	CToken::ttype tokenizeOperands(const char*& expr, char *token, int& dispiid);
	CVector<CToken*> _postfix;
//	CMap<CString, LPCSTR, CIDataItem*, CIDataItem*> m_mapDataItem;
	CStringArray *m_pArrayPath;//KHD:전략Path를 가지고 있다.
};

#endif // !defined(AFX_LOGICMAKER2_H__31A2619A_049C_4F64_B998_0982C1B1E79F__INCLUDED_)
