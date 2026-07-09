// tlImage.h: interface for the CtlImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLIMAGE_H__707B6704_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLIMAGE_H__707B6704_50C2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlImage : public CtlRect  
{
public:
	CtlImage(CChildView* view, UINT id);
	virtual ~CtlImage();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ imNONE=0, imAVI }	m_type;
	void	drawEnd();
};

#endif // !defined(AFX_TLIMAGE_H__707B6704_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
