// tlSheet.h: interface for the CtlSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLSHEET_H__DAE20BC0_A94B_44C8_8A27_583A503F2B3C__INCLUDED_)
#define AFX_TLSHEET_H__DAE20BC0_A94B_44C8_8A27_583A503F2B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlSheet : public CtlRect  
{
public:
	CtlSheet(CChildView* view);
	virtual ~CtlSheet();

	void	Draw(CDC* dc, int fN);

protected:
	void	drawEnd();
};

#endif // !defined(AFX_TLSHEET_H__DAE20BC0_A94B_44C8_8A27_583A503F2B3C__INCLUDED_)
