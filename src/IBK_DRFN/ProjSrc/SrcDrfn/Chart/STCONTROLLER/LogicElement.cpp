// LogicElement.cpp: implementation of the CLogicElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicElement::CLogicElement()
{
	m_pIDataItem = NULL;
	m_lIndex = 0;
}

CLogicElement::~CLogicElement()
{

}

double CLogicElement::GetValue()
{
	//<<20100330_JS.Kim 
	//return m_pIDataItem->GetAt(m_lIndex);
	if( m_pIDataItem )
		return m_pIDataItem->GetAt(m_lIndex);
	return 0.0;
	//>>
}


