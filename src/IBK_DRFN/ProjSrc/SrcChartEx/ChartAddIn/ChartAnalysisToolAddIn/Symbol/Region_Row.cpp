// Region_Row.cpp: implementation of the CRegion_Row class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "Region_Row.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegion_Row::CRegion_Row(const int nRowIndex)
{
	m_nRow = nRowIndex;
}

CRegion_Row::~CRegion_Row()
{

}

CRect CRegion_Row::GetBlockRegion() const
{
	return m_BlockRegion;
}

void CRegion_Row::SetBlockRegion(const CRect &blockregion)
{
	m_BlockRegion = blockregion;
}

int CRegion_Row::GetRowIndex()
{
	return m_nRow;
}


void CRegion_Row::SetRowIndex(const int nRow)
{
	m_nRow = nRow;
}
