// BkUtil.cpp: implementation of the CBkUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BkUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CBkUtil::CBkUtil()
//{
//}

CBkUtil::~CBkUtil()
{
	if (m_brBk.GetSafeHandle())
		m_brBk.DeleteObject();
}

void CBkUtil::SetBrush(COLORREF clr)
{
	m_clrBk = clr;
	
	if (m_brBk.GetSafeHandle())
		m_brBk.DeleteObject();

	m_brBk.CreateSolidBrush(clr);
}

void CBkUtil::DrawBkgrd(CDC* pDC,CRect &rc)
{
	pDC->FillRect(rc, &m_brBk);
}
