// HogaData.cpp: implementation of the CHogaData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HogaData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHogaData::CHogaData()
{
	m_nRow = -1;	
	m_nCol = -1;	
	m_nMask = 0;

	m_crBkClr = RGB(255, 255, 255);
	m_crFgClr = RGB(0, 0, 0);

	m_strText = _T("");
}

CHogaData::~CHogaData()
{

}
