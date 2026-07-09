// tlBox.h: interface for the CtlBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLBOX_H__707B6703_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLBOX_H__707B6703_50C2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlBox : public CtlRect  
{
public:
	void draw2CBox(CDC *dc);
	CtlBox(CChildView* view);
	virtual ~CtlBox();

	void	Draw(CDC* dc, int fN);

protected:
	void	drawEnd();
	void	draw3DBox(CDC* dc);
};

#endif // !defined(AFX_TLBOX_H__707B6703_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
