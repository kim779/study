// BuyGrid.h: interface for the CBuyGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../../control/fx_GridEx/UGCtrl.h"

// [strPrice]
// 지정가 일 경우			: 입력받은 가격
// 시장가 / 최유리 일 경우	: 상한가
//
// [strListedQuantity]
// 주식 / ELW 일 경우		: 상장주식수 (1334 Symbol Data)
// 선물옵션일 경우			: Default
#define ALLOW_ORDER		0
#define ORD_OVER_20		1
#define ORD_OVER_50		2
class CMapWnd;

class CBuyGrid : public CUGCtrl  
{
public:
	//CBuyGrid();
	//virtual ~CBuyGrid();

	CString fListedQty;
	void UpdateCheckedRowData(int nCol, CString strVal);
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual void OnSetup();
	virtual int OnCanSizeCol(int col);
	int GetSendData(CStringArray *pArr);
	BOOL IsChecked(long nRow);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void SetCheckState(int nRow, BOOL bState);
	void ChangeFont(CFont* pFont, double* lrate);
	void SetPallete(CMapWnd* pMapWnd);

	CWnd* m_pWizard{};
private:
	int OnDropList(long ID,int col,long row,long msg,long param);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	BOOL IsAccComplete(long nRow);

private:
	BOOL CheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""));
	CString Parser(CString &srcstr, CString substr);
	int m_nFontIDNormal{};
	int m_nFontIDBold{};

	std::unique_ptr<CPen> m_pLinePen{};
	int m_iRowHeight{};
	BOOL m_bApplyRate{};
};


