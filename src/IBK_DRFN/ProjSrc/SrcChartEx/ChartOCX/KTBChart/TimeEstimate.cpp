// TimeEstimate.cpp: implementation of the CTimeEstimate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeEstimate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeEstimate::CTimeEstimate()
{
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&m_frq))
		m_bTimeEstimate =  TRUE;
	else
		m_bTimeEstimate =  FALSE;

	m_start = 0;
}

CTimeEstimate::~CTimeEstimate()
{

}

void CTimeEstimate::StartEstimate()
{
	if(!m_bTimeEstimate) return;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
}

DWORD CTimeEstimate::EndEstimate()
{
	if(!m_bTimeEstimate) return 0;

	__int64 end, diff;
	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	diff = end - m_start;
	diff = (diff * 1000) / m_frq;
	DWORD MicroSec = (DWORD)(diff & 0xffffffff);	
	return MicroSec;
}