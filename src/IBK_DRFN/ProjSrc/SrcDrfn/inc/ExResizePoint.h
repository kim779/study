// ExResizePoint.h: interface for the CExResizePoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXRESIZEPOINT_H__7D39BC43_45C6_40EC_B271_DE8A13468CC0__INCLUDED_)
#define AFX_EXRESIZEPOINT_H__7D39BC43_45C6_40EC_B271_DE8A13468CC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExResizeRect.h"

struct EX_RESIZEPOINT
{
	EX_RESIZE x; // Specifies the x-coordinate of a point
	EX_RESIZE y; // Specifies the y-coordinate of a point
};

class CExResizePoint : public EX_RESIZEPOINT
{
public:
	CExResizePoint();
	CExResizePoint(EX_RESIZE cx, EX_RESIZE cy);

};

#define SZ_TOP_LEFT      CExResizePoint( 0, 0)  // Specifies a top left position.
#define SZ_TOP_CENTER    CExResizePoint(.5, 0)  // Specifies a top center position.
#define SZ_TOP_RIGHT     CExResizePoint( 1, 0)  // Specifies a top right position.
#define SZ_MIDDLE_LEFT   CExResizePoint( 0,.5)  // Specifies a middle left position.
#define SZ_MIDDLE_CENTER CExResizePoint(.5,.5)  // Specifies a middle center position.
#define SZ_MIDDLE_RIGHT  CExResizePoint( 1,.5)  // Specifies a middle right position.
#define SZ_BOTTOM_LEFT   CExResizePoint( 0, 1)  // Specifies a bottom left position.
#define SZ_BOTTOM_CENTER CExResizePoint(.5, 1)  // Specifies a bottom center position.
#define SZ_BOTTOM_RIGHT  CExResizePoint( 1, 1)  // Specifies a bottom right position.

#endif // !defined(AFX_EXRESIZEPOINT_H__7D39BC43_45C6_40EC_B271_DE8A13468CC0__INCLUDED_)
