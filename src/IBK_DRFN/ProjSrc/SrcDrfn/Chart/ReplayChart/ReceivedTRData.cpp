// ReceivedTRData.cpp: implementation of the CReceivedTRData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReceivedTRData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// CReceivedEachTR class
//-----------------------------------------------------------------------------

CReceivedEachTR::CReceivedEachTR()
{
	m_nNDayBefore = 0;
	m_szDateInfo = "";
	m_nTickIdx = 0;
	m_strBojoMsg = "";
	memset(m_szKey, 0x00, sizeof(m_szKey));
}


CReceivedEachTR::~CReceivedEachTR()
{
	ClearTRData();
}

void CReceivedEachTR::ClearTRData()
{
	m_arrChart.RemoveAll();
}


//-----------------------------------------------------------------------------
// CReceivedTRData class
//-----------------------------------------------------------------------------
CReceivedTRData::CReceivedTRData()
{
	m_szStockCode = "";
	
	if( m_lstTRData.GetCount() > 0 )
	{
		POSITION pos = m_lstTRData.GetHeadPosition();
		while( pos )
			delete m_lstTRData.GetNext(pos);

		m_lstTRData.RemoveAll();
	}
}


CReceivedTRData::~CReceivedTRData()
{
	if( m_lstTRData.GetCount() > 0 )
	{
		POSITION pos = m_lstTRData.GetHeadPosition();
		while( pos )
			delete m_lstTRData.GetNext(pos);

		m_lstTRData.RemoveAll();
	}
}
