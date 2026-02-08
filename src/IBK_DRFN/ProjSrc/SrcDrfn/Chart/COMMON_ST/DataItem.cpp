// DataItem.cpp: implementation of the CSTDataItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTDataItem::CSTDataItem()
{
	m_pDataItemConnected = NULL;
	m_dInitValue = 0;
	m_lFirstIndex = 0;
	m_bSetValue = FALSE;
}

CSTDataItem::~CSTDataItem()
{

}

BOOL CSTDataItem::Init(LPCTSTR lpName,int nType,double dInitValue,long lCntOfAllData, CIDataItem *pDataItemForConnection /*=NULL*/)
{
	m_strName = lpName;
	m_nType = nType;	
	m_dInitValue = dInitValue;
	if(pDataItemForConnection)
	{
		m_pDataItemConnected = pDataItemForConnection;
		m_arrayData.SetSize(0);
	}
	else
	{
		m_pDataItemConnected = NULL;
		m_arrayData.RemoveAll();
		m_arrayData.SetSize(lCntOfAllData);
		m_bSetValue = FALSE;
		m_lFirstIndex = lCntOfAllData - 1;
	}	
	return TRUE;
}



CString CSTDataItem::GetDataItemName()
{ 
	if(m_pDataItemConnected)
	{
		return m_pDataItemConnected->GetDataItemName();
	}
	return m_strName;
}


long CSTDataItem::GetSize() 
{ 
	if(m_pDataItemConnected)
	{
		return m_pDataItemConnected->GetSize();
	}
	return m_arrayData.GetSize(); 
}

long CSTDataItem::GetFirstDataIndex() 
{ 
	if(m_pDataItemConnected)
	{
		return m_pDataItemConnected->GetFirstDataIndex();
	}
	return (m_lFirstIndex + 1);
}

void CSTDataItem::SetComment(LPCTSTR lpTitle,LPCTSTR lpComment)
{
	m_mapComment.SetAt(lpTitle,lpComment);
}

CString CSTDataItem::GetComment(LPCTSTR lpTitle)
{
	CString strComment;
	m_mapComment.Lookup(lpTitle,strComment);
	return strComment;
}
