// ExResizeRect.cpp: implementation of the CExResizeRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExResizeRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExResizeRect::CExResizeRect()
{

}

CExResizeRect::CExResizeRect(EX_RESIZE l, EX_RESIZE t, EX_RESIZE r, EX_RESIZE b)
{
	left   = l;
	top    = t;
	right  = r;
	bottom = b;
}

CExResizeRect& CExResizeRect::operator = (const RECT& rc) 
{ 
	left   = (EX_RESIZE) rc.left; 
	top    = (EX_RESIZE) rc.top; 
	right  = (EX_RESIZE) rc.right; 
	bottom = (EX_RESIZE) rc.bottom; 

	return *this;
}

CExResizeRect& CExResizeRect::operator = (const EX_RESIZERECT& rrc) 
{ 
	left   = rrc.left; 
	top    = rrc.top; 
	right  = rrc.right; 
	bottom = rrc.bottom; 

	return *this;
}

CExResizeRect& CExResizeRect::operator += (const EX_RESIZERECT& rrc) 
{ 
	left   += rrc.left; 
	top    += rrc.top; 
	right  += rrc.right; 
	bottom += rrc.bottom; 

	return *this;
}

CExResizeRect CExResizeRect::operator & (const EX_RESIZERECT& rrc)
{ 
	ASSERT(IsNormalized() && ((CExResizeRect&) rrc).IsNormalized());

	CExResizeRect rrcI(
        __max(left,   rrc.left),
        __max(top,    rrc.top), 
		__min(right,  rrc.right),
        __min(bottom, rrc.bottom)); 
	
	// only intersection if resulting rect is normalized
	return (rrcI.IsNormalized()) ? rrcI : CExResizeRect(0,0,0,0);
}
