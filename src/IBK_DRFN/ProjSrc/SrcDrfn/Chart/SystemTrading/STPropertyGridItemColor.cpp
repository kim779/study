// STPropertyGridItemColor.cpp: implementation of the CSTPropertyGridItemColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "systemtrading.h"

#include "STPropertyGridItemColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_DYNAMIC(CSTPropertyGridItemColor, CXTPPropertyGridItem)

CSTPropertyGridItemColor::CSTPropertyGridItemColor(CString strCaption, COLORREF clr/* = 0*/, COLORREF* pBindColor/* = NULL*/)
	: CXTPPropertyGridItemColor(strCaption)
{
	m_pBindColor = pBindColor;
	SetColor(clr);
	m_nFlags = 0;
}

CSTPropertyGridItemColor::~CSTPropertyGridItemColor()
{

}

void CSTPropertyGridItemColor::SetValue(CString strValue)
{	
	m_strValue = strValue;

	if (m_pBindString)
	{
		*m_pBindString = strValue;
	}
#ifdef _XTP_ACTIVEX
	BindDispatch();
#endif

	SetEditText(m_strValue);
	
	if (m_pGrid && m_pGrid->m_hWnd) m_pGrid->Invalidate(FALSE);
}
