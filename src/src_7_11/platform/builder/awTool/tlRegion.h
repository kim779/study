// tlRegion.h: interface for the CtlRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLREGION_H__63FB2C60_50F2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLREGION_H__63FB2C60_50F2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlRegion : public CtlRect  
{
public:
	CtlRegion(CChildView* view, UINT id);
	virtual ~CtlRegion();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ rnNONE=0, rnGRAPH, rnOBJECT, rnUSEROB, rnBROWSER, rnCONTROL }	m_type;
	void	drawEnd();
};

#endif // !defined(AFX_TLREGION_H__63FB2C60_50F2_11D4_A024_00001CD7F9BE__INCLUDED_)
