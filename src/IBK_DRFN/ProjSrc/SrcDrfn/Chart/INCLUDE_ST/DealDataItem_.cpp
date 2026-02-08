// DealDataItem.cpp: implementation of the CDealDataItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealDataItem::CDealDataItem()
{
	m_nLastIndex = -1;
	m_lLastDealValue = 0;
	m_EnableContinuty = FALSE;
	m_lMaxContinuty = 0;
	m_lCountOfContinuty = 0;
	m_dMinimumChangeRate = 0.;
	m_dLastCurrentPrice = 0.;
}

CDealDataItem::~CDealDataItem()
{
	Clear();
}

void CDealDataItem::Clear()
{
	CDealData *pDealData = NULL;
	POSITION pos = m_listData.GetHeadPosition();
	while(pos)
	{
		pDealData = m_listData.GetNext(pos);
		delete pDealData;
	}
	m_listData.RemoveAll();
	m_nLastIndex = -1;	
	m_lLastDealValue = 0;
	m_mapComment.RemoveAll();
}

POSITION CDealDataItem::GetHeadPosition( ) const
{
	return m_listData.GetHeadPosition();
}

CDealData* CDealDataItem::GetNext(POSITION& pos)
{
	return m_listData.GetNext(pos);
}

CDealData* CDealDataItem::GetPrev(POSITION& pos)
{
	return m_listData.GetPrev(pos);
}

POSITION CDealDataItem::GetTailPosition( ) const
{
	return m_listData.GetTailPosition();
}

CDealData* CDealDataItem::GetTail()
{
	return m_listData.GetTail();
}

CDealData* CDealDataItem::GetHead()
{
	return m_listData.GetHead();
}

void CDealDataItem::EnableContinuty(BOOL bEnable, long lMaxContinuty /*=0*/) 
{
	m_EnableContinuty = bEnable;
	m_lMaxContinuty = lMaxContinuty;
}

void CDealDataItem::RemoveTail()
{
	int nCount = m_listData.GetCount();
	if(nCount==0) return;
	CDealData *pDealData = m_listData.RemoveTail();
	delete pDealData;
	if(nCount==1)
	{
		Clear();
	}
	else
	{
		pDealData = m_listData.GetTail();
		m_nLastIndex = pDealData->m_nIndex;
		m_lLastDealValue = pDealData->m_lDealValue;
		m_lCountOfContinuty = 0;
		POSITION pos = m_listData.GetTailPosition();
		while(pos)
		{
			pDealData = m_listData.GetPrev(pos);
			if(m_lLastDealValue==pDealData->m_lDealValue)
			{
				m_lCountOfContinuty++;
			}
		}
	}
}

CDealData* CDealDataItem::GetDealDataByIndex(int nIndex)
{
	CDealData *pDealData = NULL;
	if(nIndex<0) return NULL;	
	if(m_nLastIndex==nIndex)
	{
		pDealData = m_listData.GetTail();
	}
	else if(m_nLastIndex<nIndex)
	{
		pDealData = new CDealData;
		pDealData->m_nIndex = nIndex;
		m_listData.AddTail(pDealData);
		m_nLastIndex = nIndex;
	}
	else // it would be slow...
	{
		POSITION pos = m_listData.GetTailPosition();
		while(pos)
		{
			pDealData = m_listData.GetPrev(pos);
			if(pDealData->m_nIndex==nIndex)
			{
				break;
			}
			else if(pDealData->m_nIndex<nIndex)
			{
				return NULL;
			}
		}		
	}
	return pDealData;	
}


CDealData* CDealDataItem::SetData(int nIndex, long lDealValue, long lType /*=0*/, LPCTSTR lpDealName /*= NULL*/, double dPriceByCmd/* = 0.*/, double dContract/*= 0.*/, CStringList* pStListEntryNameForExit/*=NULL*/)
{
	if(!lDealValue) 
	{
		if(nIndex==m_nLastIndex)// Index가 마지막 입력Index와 같고 lDealValue가 0이면 마지막 입력데이터를 삭제한다.
		{
			RemoveTail();
		}
		return NULL;
	}
	
	CDealData *pDealData = GetDealDataByIndex(nIndex);
	if(pDealData)
	{
		pDealData->m_lDealValue |= lDealValue;
		pDealData->m_dPrice = dPriceByCmd;
		pDealData->m_dContract = dContract;	
		pDealData->m_lType = lType;
		if(lpDealName&&pDealData->m_strDealName.IsEmpty())
		{
			pDealData->m_strDealName = lpDealName;
		}	
		if(pStListEntryNameForExit)
		{
			pDealData->m_strListEntryNameForExit.AddTail(pStListEntryNameForExit);
		}
		m_lLastDealValue = pDealData->m_lDealValue;
		m_nLastIndex = nIndex;
	}
	return pDealData;
}

void CDealDataItem::SetComment(LPCTSTR lpTitle,LPCTSTR lpComment)
{
	m_mapComment.SetAt(lpTitle,lpComment);
}

CString CDealDataItem::GetComment(LPCTSTR lpTitle)
{
	CString strComment;
	m_mapComment.Lookup(lpTitle,strComment);
	return strComment;
}

