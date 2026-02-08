// ConfirmGrid.h: interface for the CConfirmGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"
#include "../../control/fx_GridEx/UGCell.h"

class CMapWnd;

class CConfirmGrid : public CUGCtrl  
{
public:
	CConfirmGrid(CMapWnd* pMapWnd = NULL);
	virtual ~CConfirmGrid();

	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual int OnCanSizeCol(int col);

private:
	CMapWnd* m_pMapWnd;
	CPen* m_pLinePen;
	int m_nFontIDNormal;
	int m_nFontIDBold;
};

