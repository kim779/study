// LogicOperation.h: interface for the CLogicOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICOPERATION_H__05A677F4_E297_41F6_AB47_ABE32DAF80AC__INCLUDED_)
#define AFX_LOGICOPERATION_H__05A677F4_E297_41F6_AB47_ABE32DAF80AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LogicItem.h"

class CLogicOperation  : public CLogicItem
{
public:
	CLogicOperation();
	virtual ~CLogicOperation();
	virtual double GetValue();
	virtual void SetIndex(long lIndex);
	
	void SetOperation(int nOperation);
	void SetItemLeft(CLogicItem* pLogicItemLeft, BOOL bNoError = FALSE);
	void SetItemRight(CLogicItem* pLogicItemRIght, BOOL bNoError = FALSE);
	CLogicItem* GetItemLeft() { return m_pLogicItemLeft;}
	CLogicItem* GetItemRight() { return m_pLogicItemRight;}
private:
	CLogicItem* m_pLogicItemLeft;
	CLogicItem* m_pLogicItemRight;
	int m_nOperation;
};

#endif // !defined(AFX_LOGICOPERATION_H__05A677F4_E297_41F6_AB47_ABE32DAF80AC__INCLUDED_)
