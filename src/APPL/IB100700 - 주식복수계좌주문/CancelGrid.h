// CancelGrid.h: interface for the CCancelGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"

class CMapWnd;
class CCancelGrid : public CUGCtrl  
{
public:

	virtual ~CCancelGrid();

	void UpdateCheckedRowData(int nCol, CString strVal);
	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual int OnCanSizeCol(int col);
	int GetSendData(CStringArray *pArr);
	BOOL IsChecked(long nRow);
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void SetCheckState(int nRow, BOOL bState);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	void ChangeFont(CFont* pFont, double* lrate);
	void SetPallete(CMapWnd* pMapWnd);


private:
	int OnDropList(long ID,int col,long row,long msg,long param);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	BOOL IsAccComplete(long nRow);

private:
	int m_nFontIDNormal{};
	int m_nFontIDBold{};

	std::unique_ptr<CPen> m_pLinePen;
	int	  m_iRowHeight{};
};


