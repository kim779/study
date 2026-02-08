// TempMemMng.cpp: implementation of the CTempMemMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TempMemMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTempMemMng::CTempMemMng()
{
	m_pDbArray = NULL;
	memset(m_pTempMemMng,0x00,sizeof(m_pTempMemMng));
}

CTempMemMng::~CTempMemMng()
{
	// (2006/4/3 - Seung-Won, Bae) Check All Index for Random Allocation in Pool (0 ~ 9)
	for(int nPos=0;nPos<MAX_INDEX_TEMPMEM;nPos++) if( m_pTempMemMng[nPos])
	{
		delete m_pTempMemMng[nPos];
		m_pTempMemMng[nPos] = NULL;
	}
	if(m_pDbArray)
	{
		delete [] m_pDbArray;
		m_pDbArray = NULL;		
	}
}

double *CTempMemMng::GetDblArray(int nSize)
{
	if(!m_pDbArray)
	{
		m_pDbArray = new double[nSize];
		::ZeroMemory(m_pDbArray,sizeof(double)*nSize);
	}
	return m_pDbArray;
}

CITempMemMng* CTempMemMng::GetSubTemp(int nIndex)
{
	if(!m_pTempMemMng[nIndex])// nIndex's MAX_INDEX_TEMPMEM is 10
	{
		m_pTempMemMng[nIndex] = new CTempMemMng;
	}
	return m_pTempMemMng[nIndex];
}
