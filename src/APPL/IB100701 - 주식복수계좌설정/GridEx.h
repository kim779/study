// GridEx.h: interface for the CGridEx class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

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

private:
	CMapWnd* m_pMapWnd{};
	int m_nFontIDNormal{};
	int m_nFontIDBold{};
};

