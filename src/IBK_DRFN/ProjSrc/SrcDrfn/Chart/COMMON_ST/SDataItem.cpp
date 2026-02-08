// SDataItem.cpp: implementation of the CSDataItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SDataItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSDataItem::CSDataItem()
{
	m_pSDataItemConnected = NULL;
	m_chType = NULL;
}

CSDataItem::~CSDataItem()
{

}

BOOL CSDataItem::Init(LPCTSTR lpName,LPCTSTR lpData,long lCntOfAllData,CISDataItem *pDataItemForConnection /*=NULL*/)
{
	m_strName = lpName;
	if(m_pSDataItemConnected)
	{
		m_pSDataItemConnected = pDataItemForConnection;
	}
	else
	{
		m_arraySData.SetSize(lCntOfAllData);
		if(lCntOfAllData&&lpData)	SetAt(0, lpData);
	}	
	return TRUE;

}

void CSDataItem::SetSize(long lSize)
{
	m_arraySData.SetSize(lSize);
}