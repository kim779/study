// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chartdatalistvieweraddin.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData()
{
	m_pistrDataList = NULL;
	m_pidDataList = NULL;
}

//CData::CData(const CData& ci)
//{
//	m_RowDataList.RemoveAll();
//	POSITION pos = ci.m_RowDataList.GetHeadPosition();
//	while(pos)
//	{
//		m_RowDataList.AddTail(ci.m_RowDataList.GetNext(pos));
//	}
//}

CData::~CData()
{
	// (2009/9/24 - Seung-Won, Bae) Release it.
	if( m_pistrDataList) m_pistrDataList->Release();
	if( m_pidDataList) m_pidDataList->Release();
}

//const CData& CData::operator=(const CData &ci)
//{
//	m_RowDataList.RemoveAll();
//	POSITION pos = ci.m_RowDataList.GetHeadPosition();
//	while(pos)
//	{
//		m_RowDataList.AddTail(ci.m_RowDataList.GetNext(pos));
//	}
//
//	return *this;
//}

