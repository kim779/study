// DealDataItem.h: interface for the CDealDataItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALDATAITEM_H__AD4CCFEB_779B_4B33_927B_E2D8F056B50C__INCLUDED_)
#define AFX_DEALDATAITEM_H__AD4CCFEB_779B_4B33_927B_E2D8F056B50C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "DealData.h"


class CDealDataItem  
{
public:
	CDealDataItem();
	virtual ~CDealDataItem();
	CDealData* SetData(int nIndex, long lDealValue, long lType = 0, LPCTSTR lpDealName = NULL, double dPriceByCmd = 0.
		, double dContract = 0., CStringList* pStListEntryNameForExit = NULL);
	void SetComment(LPCTSTR lpTitle,LPCTSTR lpComment);
	POSITION CDealDataItem::GetHeadPosition( ) const;
	POSITION CDealDataItem::GetTailPosition( ) const;
	CDealData* GetNext(POSITION& pos);
	CDealData* GetPrev(POSITION& pos);
	CDealData* GetTail();
	CDealData* GetHead();
	CDealData* GetDealDataByIndex(int nIndex);
	CString GetComment(LPCTSTR lpTitle);
	int GetCount() { return m_listData.GetCount();}
	void EnableContinuty(BOOL bEnable, long lMaxContinuty = 0);
	void EnableMinimumChange(BOOL bEnable, double dMinumumRate = 0);
	void Clear();
private:
	void RemoveTail();

	CList<CDealData*,CDealData*> m_listData;
	CMapStringToString m_mapComment;
	int m_nLastIndex;
	long m_lLastDealValue;
	BOOL m_EnableContinuty;
	long m_lMaxContinuty;
	long m_lCountOfContinuty;
	double m_dMinimumChangeRate;
	double m_dLastCurrentPrice;
};

#endif // !defined(AFX_DEALDATAITEM_H__AD4CCFEB_779B_4B33_927B_E2D8F056B50C__INCLUDED_)
