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
	void	SetPallete();


private:
	CMapWnd* m_pMapWnd;
	std::unique_ptr<CPen> m_pLinePen;
	int m_nFontIDNormal=0;
	int m_nFontIDBold=0;
};


