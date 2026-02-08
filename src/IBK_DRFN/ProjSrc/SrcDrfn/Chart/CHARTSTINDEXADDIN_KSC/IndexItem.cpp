// IndexItem.cpp: implementation of the CIndexItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndexItem.h"

#include "./Include_AddIn/I139092/_IPacket.h"		// for IPacket

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexItem::CIndexItem()
{
	// (2006/1/19 - Seung-Won, Bae) Interface of Remove All IPacket Reference
	RemoveAllPacket();

	m_strInputValue = "";
	m_indexType = AssistIndex;
}

CIndexItem::~CIndexItem()
{
	// (2006/1/19 - Seung-Won, Bae) Interface of Remove All IPacket Reference
	RemoveAllPacket();
}

CString CIndexItem::GetInputValue()
{
	if(m_strInputValue.GetLength()>0)
	{
		return (',' + m_strInputValue);
	}
	return "";
}

// (2006/1/19 - Seung-Won, Bae) Interface of Remove All IPacket Reference
void CIndexItem::RemoveAllPacket()
{
	IPacket *pIPacket = NULL;
	while( !m_ListOutputIPacket.IsEmpty())
	{
		pIPacket = m_ListOutputIPacket.RemoveHead();
		pIPacket->Release();
	}
	m_ListOutputIPacket.RemoveAll();
}
