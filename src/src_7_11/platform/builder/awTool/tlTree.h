// tlTree.h: interface for the CtlTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLTREE_H__69787E00_8BB6_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLTREE_H__69787E00_8BB6_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"

class AFX_EXT_CLASS CtlTree : public CtlRect  
{
public:
	CtlTree(CChildView* view);
	virtual ~CtlTree();

	void	Draw(CDC* dc, int fN);

protected:
	void	drawEnd();
};

#endif // !defined(AFX_TLTREE_H__69787E00_8BB6_11D4_A024_00001CD7F9BE__INCLUDED_)
