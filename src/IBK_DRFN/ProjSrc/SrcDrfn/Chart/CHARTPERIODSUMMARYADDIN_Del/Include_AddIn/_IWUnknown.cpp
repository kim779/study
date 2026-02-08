// IWUnknown.cpp: implementation of the CIWUnknown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "_IWUnknown.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWUnknown::CIWUnknown()
{
	// (2004.10.07, 배승원) AddRef()와 Release()를 제공한다. (QueryInterface()는 NULL로 등록해 놓는다.)
	m_ulReference = 0;
}

CIWUnknown::~CIWUnknown()
{

}

// (2004.10.07, 배승원) AddRef()와 Release()를 제공한다. (QueryInterface()는 NULL로 등록해 놓는다.)
ULONG CIWUnknown::AddRef( void)
{
	m_ulReference++;
	return m_ulReference;
}
ULONG CIWUnknown::Release( void)
{
	m_ulReference--;
	if( m_ulReference <= 0)
	{
		// (2006/6/19 - Seung-Won, Bae) Cuation! Do not update after delete!
		m_ulReference = 0;
		delete this;
		// (2006/6/27 - Seung-Won, Bae) Cuation! Do not access this.
		return 0;
	}

	return m_ulReference;
}
