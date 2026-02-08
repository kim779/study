// MemberBitmap.cpp: implementation of the CMemberBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExButton.h"
#include "MemberBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberBitmap::CMemberBitmap(LPVOID lpControl) : CMember(lpControl)
{
	m_pButton = (CButton_Friendly*)lpControl;
	
	m_nWidth = m_nHeight = 0;
	m_hDIB = m_hDIBDown = m_hDIBDis = NULL;
}

CMemberBitmap::~CMemberBitmap()
{
	if(m_hDIB)		
	{
		::GlobalFree(m_hDIB);		
		m_hDIB = NULL;
	}

	if(m_hDIBDown)
	{
		::GlobalFree(m_hDIBDown);	
		m_hDIBDown = NULL;
	}

	if(m_hDIBDis)
	{
		::GlobalFree(m_hDIBDis);	
		m_hDIBDis = NULL;
	}
}
