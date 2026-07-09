// tlSplit.h: interface for the CtlSplit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLSPLIT_H__53C489A2_46A0_4563_BC34_8DD162B414CA__INCLUDED_)
#define AFX_TLSPLIT_H__53C489A2_46A0_4563_BC34_8DD162B414CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlSplit : public CtlRect  
{
public:
	CtlSplit(CChildView* view);
	virtual ~CtlSplit();

	void	Draw(CDC* dc, int fN);

protected:
	void	drawBegin(CPoint point);
	void	drawTo(CPoint point);
	void	drawEnd();
};

#endif // !defined(AFX_TLSPLIT_H__53C489A2_46A0_4563_BC34_8DD162B414CA__INCLUDED_)
