// BaseObj.cpp: implementation of the CBaseObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chartperiodsummaryaddin.h"
#include "LineBaseObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineBaseObj::CLineBaseObj()
{

}

CLineBaseObj::~CLineBaseObj()
{

}

void CLineBaseObj::Draw(CDC *pDC)
{

}

void CLineBaseObj::SetPos(CPoint pt, int nType)
{

}

CPoint CLineBaseObj::GetPos(int nType)
{
	return CPoint(0,0);
}
