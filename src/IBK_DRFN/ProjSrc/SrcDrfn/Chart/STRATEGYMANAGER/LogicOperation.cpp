// LogicOperation.cpp: implementation of the CLogicOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicOperation.h"
#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicOperation::CLogicOperation()
{
	m_nOperation = -1;
	m_pLogicItemLeft = NULL;
	m_pLogicItemRight = NULL;
	m_DealDataForNext.Clear();
}

CLogicOperation::~CLogicOperation()
{
	fnClearItem(1);
	fnClearItem(2);
}

void CLogicOperation::fnClearItem(int nType)
{
//	m_CriticalSection.Lock();
	if(nType==1)
	{
		if(m_pLogicItemLeft)
		{
			delete m_pLogicItemLeft;
			m_pLogicItemLeft = NULL;
		}
	}
	else if(nType==2)
	{
		if(m_pLogicItemRight)
		{
			delete m_pLogicItemRight;
			m_pLogicItemRight = NULL;
		}
	}
		
//	m_CriticalSection.Unlock();
}

void CLogicOperation::SetOperation(int nOperation)
{
	m_nOperation = nOperation;
}

void CLogicOperation::SetItemLeft(CLogicItem* pLogicItemLeft, BOOL bNoError /*= FALSE*/)
{
	if(m_pLogicItemLeft&&!bNoError)
	{
		::AfxMessageBox("SetItemLeft Error!");
	}
	pLogicItemLeft->SetParent(this);
	m_pLogicItemLeft = pLogicItemLeft;
}

void CLogicOperation::SetItemRight(CLogicItem* pLogicItemRight, BOOL bNoError /*= FALSE*/)
{
	if(m_pLogicItemRight&&!bNoError)
	{
		::AfxMessageBox("SetItemRight Error!");
	}
	pLogicItemRight->SetParent(this);
	m_pLogicItemRight = pLogicItemRight;
}

long CLogicOperation::Run(LPCTSTR lpDataCode, long lDataCnt, LPCTSTR lpEndDate,LPCTSTR lpOption)
{
	long lRetValue1 = 0;
	long lRetValue2 = 0;
	if(m_pLogicItemLeft)
	{
		lRetValue1 = m_pLogicItemLeft->Run(lpDataCode,lDataCnt,lpEndDate,lpOption);
	}
	if(m_pLogicItemRight)
	{
		lRetValue2 = m_pLogicItemRight->Run(lpDataCode,lDataCnt,lpEndDate,lpOption);
	}
	if(lRetValue1==0||lRetValue2==0)
	{
		return 0;
	}
	else
	{
		return lRetValue1>lRetValue2? lRetValue1: lRetValue2;
	}
}

BOOL CLogicOperation::Stop()
{
	if(m_pLogicItemLeft)
	{
		m_pLogicItemLeft->Stop();
	}
	if(m_pLogicItemRight)
	{
		m_pLogicItemRight->Stop();
	}

	return TRUE;
}

//@060922전략Real관련수정
long CLogicOperation::Update(long lPos, BOOL bIsUpdateData)
{
	if(m_pLogicItemLeft)
	{
		//return m_pLogicItemLeft->Update(lPos, bIsUpdateData);
		m_pLogicItemLeft->Update(lPos, bIsUpdateData);
	}
	if(m_pLogicItemRight)
	{
		//return m_pLogicItemRight->Update(lPos, bIsUpdateData);
		m_pLogicItemRight->Update(lPos, bIsUpdateData);
	}
	return 0;
}

BOOL CLogicOperation::Pause()
{
	if(m_pLogicItemLeft)
	{
		m_pLogicItemLeft->Pause();
	}
	if(m_pLogicItemRight)
	{
		m_pLogicItemRight->Pause();
	}	
	return TRUE;
}
BOOL CLogicOperation::IsFilled()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		if(m_pLogicItemLeft->IsFilled()&&m_pLogicItemRight->IsFilled())
		{
			return TRUE;
		}
	}
	return FALSE;
}


void CLogicOperation::InitFilledSet()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
//		m_CriticalSection.Lock();
		m_pLogicItemLeft->InitFilledSet();
		m_pLogicItemRight->InitFilledSet();
//		m_CriticalSection.Unlock();
	}
}

long CLogicOperation::GetTimeTypeNum()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		int nLeft = m_pLogicItemLeft->GetTimeTypeNum();
		int nRight = m_pLogicItemRight->GetTimeTypeNum();
		if(nLeft>nRight)
		{
			return nLeft;
		}
		else
		{
			return nRight;
		}
	}
	return 0;
}

CISDataItem *CLogicOperation::GetDateItem()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		int nLeft = m_pLogicItemLeft->GetTimeTypeNum();
		int nRight = m_pLogicItemRight->GetTimeTypeNum();
		if(nLeft>nRight)
		{
			return m_pLogicItemLeft->GetDateItem();
		}
		else
		{
			return m_pLogicItemRight->GetDateItem();
		}
	}
	return NULL;
}

CDealData* CLogicOperation::GetValue(CDealData* pDealValueLeft, CDealData* pDealValueRight, LPCTSTR lpTime, char chType, LPCTSTR lpCurrentTime)
{
	m_DealData.Clear();
	switch(m_nOperation)
	{
	case DISPIID_OR:
		m_DealData = *pDealValueLeft|*pDealValueRight;
		break;
	case DISPIID_NEXT:
		if(pDealValueLeft->m_lDealValue)
		{
// 2008.05.29 by LYH >> next 조건인 경우 신호 표시 안 되는 오류 수정
			m_DealDataForNext.m_lDealValue = pDealValueLeft->m_lDealValue;
// 2008.05.29 by LYH <<
			if(	// BUY NEXT SELL, SELL NEXT BUY 
				  m_DealDataForNext.m_lDealValue&VALBUY&&pDealValueLeft->m_lDealValue&VALSELL
				||m_DealDataForNext.m_lDealValue&VALSELL&&pDealValueLeft->m_lDealValue&VALBUY
				)
			{
				m_DealDataForNext.Clear();
			}
			else if(	// BUY NEXT BUY, SELL NEXT SELL 
				  m_DealDataForNext.m_lDealValue&VALBUY&&pDealValueLeft->m_lDealValue&VALBUY
				||m_DealDataForNext.m_lDealValue&VALSELL&&pDealValueLeft->m_lDealValue&VALSELL
				)
			{
				m_DealDataForNext.m_lDealValue &= pDealValueLeft->m_lDealValue;
			}			
		}		
		if(pDealValueRight->m_lDealValue&&m_DealDataForNext.m_lDealValue&pDealValueRight->m_lDealValue)
		{
			long lValue = m_DealDataForNext.m_lDealValue;
			m_DealDataForNext.Clear();
			m_DealData.m_lDealValue = lValue&pDealValueRight->m_lDealValue;
		}
		break;		
	default://case DISPIID_AND:
		{
			m_DealData.m_lDealValue = (long)pDealValueLeft->m_lDealValue&(long)pDealValueRight->m_lDealValue;
		}		
		break;
	}
	return &m_DealData;
}

CDealData* CLogicOperation::GetValue(LPCTSTR lpTime, char chType,LPCTSTR lpCurrentTime,int nPos, LPCTSTR lpCurrentTimeType)
{
	CDealData* pDealValueLeft = m_pLogicItemLeft->GetValue(lpTime,chType,lpCurrentTime,nPos,lpCurrentTimeType);
	CDealData* pDealValueRight = m_pLogicItemRight->GetValue(lpTime,chType,lpCurrentTime,nPos,lpCurrentTimeType);
	return GetValue(pDealValueLeft,pDealValueRight,lpTime,chType,lpCurrentTime);
}

long CLogicOperation::GetCount()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		return (m_pLogicItemLeft->GetCount()+m_pLogicItemRight->GetCount());
	}
	return 0;
}


CLogicST *CLogicOperation::GetLogicST(long lIndex)
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		int nCountLeft = m_pLogicItemLeft->GetCount();
		int nCountRight = m_pLogicItemRight->GetCount();

		if(lIndex<nCountLeft)
		{
			return m_pLogicItemLeft->GetLogicST(lIndex);
		}
		else if(lIndex<(nCountLeft+nCountRight))
		{
			return m_pLogicItemRight->GetLogicST(lIndex-nCountLeft);
		}
	}
	return NULL;
}


CString CLogicOperation::GetTime()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		int nLeft = m_pLogicItemLeft->GetTimeTypeNum();
		int nRight = m_pLogicItemRight->GetTimeTypeNum();
		if(nLeft>nRight)
		{
			return m_pLogicItemLeft->GetTime();
		}
		else
		{
			return m_pLogicItemRight->GetTime();
		}
	}
	return "";
}


CPtrArray* CLogicOperation::GetPArrayHistoryData()
{
	if(m_pLogicItemLeft&&m_pLogicItemRight)
	{
		int nLeft = m_pLogicItemLeft->GetTimeTypeNum();
		int nRight = m_pLogicItemRight->GetTimeTypeNum();
		if(nLeft>nRight)
		{
			return m_pLogicItemLeft->GetPArrayHistoryData();
		}
		else
		{
			return m_pLogicItemRight->GetPArrayHistoryData();
		}
	}
	return NULL;
}