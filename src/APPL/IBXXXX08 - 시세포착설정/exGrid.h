#pragma once

#include "../../control/fx_grid/fxgrid.h"

class CExGrid : public CfxGrid  
{
public:
	CExGrid();
	virtual ~CExGrid();
public:
	void	SetBkColor(COLORREF bkCOL)	{ m_bkCOL = bkCOL; };
	void	OnEditCut();
	void	OnEditPaste();

};

