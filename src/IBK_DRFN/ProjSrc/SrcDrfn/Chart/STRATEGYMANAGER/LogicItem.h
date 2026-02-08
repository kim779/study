// LogicItem.h: interface for the CLogicItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICITEM_H__C4A5E091_1CA4_4ED5_A52C_3F770009D295__INCLUDED_)
#define AFX_LOGICITEM_H__C4A5E091_1CA4_4ED5_A52C_3F770009D295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/IDataItem.h"
#include "../Common_ST/ISDataItem.h"
#include "../Common_ST/DealData.h"

enum {
	DISPIID_ADD = 0,
	DISPIID_SUB,
	DISPIID_MUL,
	DISPIID_DIV,
	DISPIID_AND,
	DISPIID_OR,
	DISPIID_NEXT
};


class CLogicOperation;
class CLogicST;
class CLogicItem  
{
public:
	virtual ~CLogicItem();
	virtual long Run(LPCTSTR lpDataCode, long lDataCnt, LPCTSTR lpEndDate,LPCTSTR lpOption) { return TRUE;}
	virtual BOOL Stop() {return TRUE;}
	virtual BOOL Pause() {return TRUE;}
	virtual long Update(long lPos, BOOL bIsUpdateData) { return TRUE; }	////@060922전략Real관련수정
	virtual BOOL IsFilled() {return TRUE;}
	virtual void InitFilledSet() {}
	virtual CISDataItem *GetDateItem() { return NULL;}
	virtual CPtrArray* GetPArrayHistoryData() { return NULL;}	
	virtual CDealData* GetValue(LPCTSTR lpTime, char chType,LPCTSTR lpCurrentTime,int nPos, LPCTSTR lpCurrentTimeType) {return NULL;}
	virtual long GetCount() { return 0;}
	virtual CLogicST *GetLogicST(long lIndex) { return NULL;}
	virtual CString GetTime() { return "";}	
	virtual long GetTimeTypeNum() { return 0;}

	CLogicOperation* GetParent() {return m_pLogicItemParent;}

	virtual	void SetPosition(POSITION pos){;}

protected:
	friend class CLogicOperation;
	void SetParent(CLogicOperation* pParent) { m_pLogicItemParent = pParent;}
	CLogicItem();
private:
	CLogicOperation* m_pLogicItemParent;
	
	int m_nType;	// 1:CLogicST, 2:CLogicOperation

public:
	int		GetType() { return m_nType;	}
	void	SetType(int nType) { m_nType = nType;	}
};

#endif // !defined(AFX_LOGICITEM_H__C4A5E091_1CA4_4ED5_A52C_3F770009D295__INCLUDED_)
