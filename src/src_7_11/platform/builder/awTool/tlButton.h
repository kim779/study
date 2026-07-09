// tlButton.h: interface for the CtlButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLBUTTON_H__707B6701_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLBUTTON_H__707B6701_50C2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlButton : public CtlRect  
{
public:
	CtlButton(CChildView* view, UINT id);
	virtual ~CtlButton();

	void	Draw(CDC* dc, int fN);

protected:
	class	Cpicture*	m_pic;
	enum	{ bnNONE=0, bnRADIO, bnCHECK, bnPUSH }	m_type;
	void	drawEnd();
};

#endif // !defined(AFX_TLBUTTON_H__707B6701_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
