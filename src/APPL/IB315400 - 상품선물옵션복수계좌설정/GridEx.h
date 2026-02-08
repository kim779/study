// GridEx.h: interface for the CGridEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDEX_H__03F25A27_6120_464D_93F0_3919F3236C25__INCLUDED_)
#define AFX_GRIDEX_H__03F25A27_6120_464D_93F0_3919F3236C25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../control/fx_GridEx/UGCtrl.h"
#include "../../control/fx_GridEx/UGCell.h"

class CMapWnd;

class CGridEx : public CUGCtrl  
{
public:
	CGridEx(CMapWnd* pMapWnd = NULL);
	virtual ~CGridEx();

	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnRowChange(long oldrow, long newrow);
	virtual void OnDClicked(int col, long row, RECT *rect, POINT *point, BOOL processed);

private:
	CMapWnd* m_pMapWnd{};
	int m_nFontIDNormal{};
	int m_nFontIDBold{};

};

#endif // !defined(AFX_GRIDEX_H__03F25A27_6120_464D_93F0_3919F3236C25__INCLUDED_)
