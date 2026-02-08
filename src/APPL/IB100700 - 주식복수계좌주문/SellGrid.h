// SellGrid.h: interface for the CSellGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"

class CMapWnd;

class CSellGrid : public CUGCtrl  
{
public:
	CString fListedQty;

	void UpdateCheckedRowData(int nCol, CString strVal);
	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual int OnCanSizeCol(int col);
	int GetSendData(CStringArray *pArr);
	BOOL IsChecked(long nRow);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void SetCheckState(int nRow, BOOL bState);
	void ChangeFont(CFont* pFont, double* lrate);
	void SetPallete(CMapWnd* pWizard);



private:
	int OnDropList(long ID,int col,long row,long msg,long param);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	BOOL IsAccComplete(long nRow);
	int OnEditFinish(int col, long row,CWnd* edit,LPCTSTR string,BOOL cancelFlag);

private:
	BOOL CheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""));

	CString Parser(CString &srcstr, CString substr);
	int m_nFontIDNormal{};
	int m_nFontIDBold{}, m_iRowHeight{};

	std::unique_ptr<CPen> m_pLinePen;
};

