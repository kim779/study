// LogicOperation.h: interface for the CLogicOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICOPERATION_H__05A677F4_E297_41F6_AB47_ABE32DAF80AC__INCLUDED_)
#define AFX_LOGICOPERATION_H__05A677F4_E297_41F6_AB47_ABE32DAF80AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <AFXMT.H>
#include "LogicItem.h"

class CLogicOperation  : public CLogicItem
{
public:
	CLogicOperation();
	virtual ~CLogicOperation();	
	
	virtual long Run(LPCTSTR lpDataCode, long lDataCnt, LPCTSTR lpEndDate,LPCTSTR lpOption);
	virtual BOOL Stop();
	virtual BOOL Pause();
	virtual long Update(long lPos, BOOL bIsUpdateData);	//@060922전략Real관련수정
	virtual BOOL IsFilled();
	virtual void InitFilledSet();
	virtual CISDataItem *GetDateItem();
	virtual CPtrArray* GetPArrayHistoryData();
	virtual CDealData* GetValue(LPCTSTR lpTime, char chType,LPCTSTR lpCurrentTime,int nPos, LPCTSTR lpCurrentTimeType);
	virtual long GetCount();
	virtual CLogicST *GetLogicST(long lIndex);
	virtual CString GetTime();
	virtual long GetTimeTypeNum();

	void SetOperation(int nOperation);
	void SetItemLeft(CLogicItem* pLogicItemLeft, BOOL bNoError = FALSE);
	void SetItemRight(CLogicItem* pLogicItemRIght, BOOL bNoError = FALSE);
	CLogicItem* GetItemLeft() { return m_pLogicItemLeft;}
	CLogicItem* GetItemRight() { return m_pLogicItemRight;}
	CDealData* GetValue(CDealData* pDealValueLeft, CDealData* pDealValueRight, LPCTSTR lpTime, char chType,LPCTSTR lpCurrentTime);

private:
	CLogicItem* m_pLogicItemLeft;
	CLogicItem* m_pLogicItemRight;
	int m_nOperation;
	CDealData m_DealDataForNext;
	CDealData m_DealData;

//	CCriticalSection m_CriticalSection;
	void fnClearItem(int nType);
};

#endif // !defined(AFX_LOGICOPERATION_H__05A677F4_E297_41F6_AB47_ABE32DAF80AC__INCLUDED_)
