// ContractGrid.h: interface for the CContractGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"
class CMapWnd;
class CContractGrid : public CUGCtrl  
{
public:

	//virtual ~CContractGrid();
	void UpdateCheckedRowData(int nCol, CString strVal);
	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual int OnCanSizeCol(int col);
	void	ChangeFont(CFont* pFont, double* lrate);
	void SetPallete(CMapWnd* pMapWnd);


private:
	BOOL IsChecked(long nRow);
	int OnDropList(long ID,int col,long row,long msg,long param);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	void SetCheckState(int nRow, BOOL bState);
	BOOL IsAccComplete(long nRow);

private:
	int m_nFontIDNormal{};
	int m_nFontIDBold{};

	std::unique_ptr<CPen> m_pLinePen{};
	int m_iRowHeight{};
};


