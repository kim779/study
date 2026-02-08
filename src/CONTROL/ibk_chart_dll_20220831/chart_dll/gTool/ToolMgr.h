// ToolMgr.h: interface for the CToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLMGR_H__ABA6BDE9_B9BD_4C0E_B4D8_D225390C1B4A__INCLUDED_)
#define AFX_TOOLMGR_H__ABA6BDE9_B9BD_4C0E_B4D8_D225390C1B4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBase.h"

class AFX_EXT_CLASS CToolMgr  
{
public:
	CToolMgr();
	virtual ~CToolMgr();

	static CToolBase*	CreateTool(CWnd* pwndView, CWnd* pwndParent, CRect rectTool, CRect rectDraw, COLORREF rgbBG, CIndcBase* pIndcBase, int iToolKind);
	static CToolBase*	CopyTool(CToolBase* pToolBase);
};

#endif // !defined(AFX_TOOLMGR_H__ABA6BDE9_B9BD_4C0E_B4D8_D225390C1B4A__INCLUDED_)
