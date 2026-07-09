// tlText.h: interface for the CtlText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLTEXT_H__A1DADD81_4EA6_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLTEXT_H__A1DADD81_4EA6_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlText : public CtlRect  
{
public:
	CtlText(CChildView* view, UINT id);
	virtual ~CtlText();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ txNONE=0, txSYS, txLABEL }	m_type;
	void	drawEnd();
};

#endif // !defined(AFX_TLTEXT_H__A1DADD81_4EA6_11D4_A024_00001CD7F9BE__INCLUDED_)
