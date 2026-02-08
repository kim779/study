// ExResizePoint.cpp: implementation of the CExResizePoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExResizePoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExResizePoint::CExResizePoint()
{

}

CExResizePoint::CExResizePoint(EX_RESIZE cx, EX_RESIZE cy)
{
    x = cx;
    y = cy;
}