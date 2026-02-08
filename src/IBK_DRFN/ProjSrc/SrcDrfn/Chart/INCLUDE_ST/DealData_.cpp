// DealData.cpp: implementation of the CDealData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealData.h"
#include "STControllerDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealData::CDealData()
{
	Clear();
}

CDealData::~CDealData()
{

}

void CDealData::Clear()
{
	m_lDealValue = 0;
	m_lType = 0;
	m_dPrice = 0.;
	m_dContract = 0.;
	m_nIndex = -1;
	m_strDealName.Empty();
	m_strListEntryNameForExit.RemoveAll();
}

CDealData::CDealData(const CDealData &rhs) 
{
	m_lDealValue = rhs.m_lDealValue;
	m_lType = rhs.m_lType;
	m_dPrice = rhs.m_dPrice;
	m_dContract = rhs.m_dContract;
	m_nIndex = rhs.m_nIndex;
	m_strDealName = rhs.m_strDealName;
	m_strListEntryNameForExit.RemoveAll();
	m_strListEntryNameForExit.AddTail((CStringList*)&rhs.m_strListEntryNameForExit);
}

CDealData  CDealData::operator =(const CDealData& rhs )
{
	m_lDealValue = rhs.m_lDealValue;
	m_lType = rhs.m_lType;
	m_dPrice = rhs.m_dPrice;
	m_dContract = rhs.m_dContract;
	m_nIndex = rhs.m_nIndex;
	m_strDealName = rhs.m_strDealName;
	m_strListEntryNameForExit.RemoveAll();
	m_strListEntryNameForExit.AddTail((CStringList*)&rhs.m_strListEntryNameForExit);
	return *this;
}

CDealData  CDealData::operator |(const CDealData& rhs )
{
	CDealData dealData;
	if(m_lDealValue==0||rhs.m_lDealValue==0)	// BUY|NULL, SELL|NULL
	{
		dealData.m_lDealValue = m_lDealValue|rhs.m_lDealValue;
	}
	else if(m_lDealValue==rhs.m_lDealValue)	// BUY|BUY, SELL|SELL
	{
		dealData.m_lDealValue = m_lDealValue;
	}
	else if(							
		(m_lDealValue&VALEXITSHORT&&rhs.m_lDealValue&VALENTRYBUY)
		||(m_lDealValue&VALENTRYBUY&&rhs.m_lDealValue&VALEXITSHORT) )
	{
		dealData.m_lDealValue = VALBUY;
	}
	else if(
		(m_lDealValue&VALEXITLONG&&rhs.m_lDealValue&VALENTRYSELL)
		||(m_lDealValue&VALENTRYSELL&&rhs.m_lDealValue&VALEXITLONG) )
	{
		dealData.m_lDealValue = VALSELL;
	}
	if(m_dContract>rhs.m_dContract)
	{
		dealData.m_dContract = m_dContract;
	}
	else
	{
		dealData.m_dContract = rhs.m_dContract;
	}

	if(m_dPrice>rhs.m_dPrice)
	{
		dealData.m_dPrice = m_dPrice;
		dealData.m_lType = m_lType;
	}
	else
	{
		dealData.m_dPrice = rhs.m_dPrice;
		dealData.m_lType = rhs.m_lType;
	}	
	

	long lLengthLeft = m_strDealName.GetLength();
	long lLengthRight = rhs.m_strDealName.GetLength();
	if(lLengthLeft&&lLengthRight)
	{
		dealData.m_strDealName = m_strDealName + "|" + rhs.m_strDealName;
	}
	else if(lLengthLeft)
	{
		dealData.m_strDealName = m_strDealName;
	}		
	else 
	{
		dealData.m_strDealName = rhs.m_strDealName;
	}

	dealData.m_strListEntryNameForExit.RemoveAll();
	dealData.m_strListEntryNameForExit.AddTail(&m_strListEntryNameForExit);
	POSITION posRight = rhs.m_strListEntryNameForExit.GetHeadPosition();
	POSITION posLeft;
	CString strEntryNameForExit;
	while(posRight)
	{
		strEntryNameForExit = rhs.m_strListEntryNameForExit.GetNext(posRight);
		posLeft = m_strListEntryNameForExit.Find(strEntryNameForExit);
		if(posLeft==NULL)
		{
			dealData.m_strListEntryNameForExit.AddTail(strEntryNameForExit);			
		}
	}	
	if(m_dContract>rhs.m_dContract)
	{
		dealData.m_dContract = m_dContract;
	}
	else 
	{
		dealData.m_dContract = rhs.m_dContract;
	}
	
	return dealData;
}

CDealData  CDealData::operator &(const CDealData& rhs )
{
	CDealData dealData;
	dealData.m_lDealValue = m_lDealValue&rhs.m_lDealValue;
	if(dealData.m_lDealValue)
	{
		if(m_dContract>rhs.m_dContract)
		{
			dealData.m_dContract = rhs.m_dContract;
		}
		else
		{
			dealData.m_dContract = m_dContract;
			
		}
		if(m_dPrice>rhs.m_dPrice)
		{
			dealData.m_dPrice = rhs.m_dPrice;	
			dealData.m_lType = rhs.m_lType;
		}
		else
		{
			dealData.m_dPrice = m_dPrice;		
			dealData.m_lType = m_lType;
		}

		long lLengthLeft = m_strDealName.GetLength();
		long lLengthRight = rhs.m_strDealName.GetLength();
		if(lLengthLeft&&lLengthRight)
		{
			dealData.m_strDealName = m_strDealName + "&" + rhs.m_strDealName;
		}
		else if(lLengthLeft)
		{
			dealData.m_strDealName = m_strDealName;
		}		
		else 
		{
			dealData.m_strDealName = rhs.m_strDealName;
		}

		POSITION posLeftHead = m_strListEntryNameForExit.GetHeadPosition();
		POSITION posRightHead = rhs.m_strListEntryNameForExit.GetHeadPosition();
		dealData.m_strListEntryNameForExit.RemoveAll();
		if(posLeftHead!=NULL&&posRightHead!=NULL)
		{
			POSITION posLeft = posLeftHead;
			POSITION posRight;
			CString strLeft;
			CString strRight;
			while(posLeft)
			{
				strLeft = m_strListEntryNameForExit.GetNext(posLeft);
				posRight = rhs.m_strListEntryNameForExit.Find(strLeft);
				if(posRight)
				{
					dealData.m_strListEntryNameForExit.AddTail(strLeft);
				}
			}
		}
		if(m_dContract>rhs.m_dContract)
		{
			dealData.m_dContract = rhs.m_dContract;
		}
		else 
		{
			dealData.m_dContract = m_dContract;
		}

	}
	return dealData;
}

