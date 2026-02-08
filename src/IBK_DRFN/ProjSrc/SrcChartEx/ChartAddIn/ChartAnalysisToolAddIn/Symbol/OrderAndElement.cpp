// OrderAndElement.cpp: implementation of the COrderAndElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "OrderAndElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrderAndElement::COrderAndElement(const int nIndex, CDrawingSymbol *pDrawingSymbol)
{
	m_nIndex = nIndex;
	m_pDrawingSymbol = pDrawingSymbol;
}

CDrawingSymbol* COrderAndElement::GetElement()
{
	return m_pDrawingSymbol;
}

int COrderAndElement::GetIndex()
{
	return m_nIndex;
}
