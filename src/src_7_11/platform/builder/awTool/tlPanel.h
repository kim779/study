// tlPanel.h: interface for the CtlPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLPANEL_H__63FB2C63_50F2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLPANEL_H__63FB2C63_50F2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlPanel : public CtlRect  
{
public:
	CtlPanel(CChildView* view, UINT id);
	virtual ~CtlPanel();

	void	Draw(CDC* dc, int fN);

protected:
	class	Cpicture*	m_pic;
	enum	{ pnNONE=0, pnPANEL, pnGROUP }	m_type;
	void	drawEnd();
};

#endif // !defined(AFX_TLPANEL_H__63FB2C63_50F2_11D4_A024_00001CD7F9BE__INCLUDED_)
