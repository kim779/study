// OrderGrid.h: interface for the COrderGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"
#include "../../control/fx_GridEx/UGCell.h"
#include "../../control/fx_GridEx/UGCBType.h"
#include "../../control/fx_GridEx/UGCTSpin.h"
#include "../../control/fx_GridEx/UGCTelps.h"
#include "../../control/fx_GridEx/UTMaskedEdit.h"


class COrderGrid : public CUGCtrl  
{
public:
	CString Parser(CString &srcstr, CString substr);
	CString CalcQuantityPrice(int nRow, CString strVal);
	COrderGrid();
	virtual ~COrderGrid();
	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual int OnCanSizeCol(int col);
	void SetPallete();
	BOOL GetSendData(CStringArray *pArr);
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	void UpdateCheckedRowData(int nCol, CString strVal, BOOL bCalc = FALSE);
	void GetCheckCode(CStringArray *pArr);
	void SetSCData(CStringArray *pArr, BOOL bHoga, BOOL bStdPrc = FALSE);	//2015.06.12 KSJ 기준가 구하기 추가
	void SetOrderResult(CStringArray *pArr);
	void ChangeFont(CFont* pFont, double* lrate);

private:
	int OnDropList(long ID,int col,long row,long msg,long param);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	int OnEllipsisButton(long ID,int col,long row,long msg,long param);
	int OnEditFinish(int col, long row,CWnd* edit,LPCTSTR string,BOOL cancelFlag);
	int OnEditStart(int col, long row,CWnd **edit);
	void InitHistoryCode(CStringList* pList);
	void CodeComplete(int nRow, BOOL bSet);
	void SetCheckState(int nRow, BOOL bState);
	BOOL IsChecked(long nRow);
	BOOL IsCodeComplete(long nRow);
	void FormatThousand(CString& strVal);

private:
	std::unique_ptr<CPen> m_pLinePen{};
	int m_nFontIDNormal{};
	int m_nFontIDBold{};
	long	m_nEllipsisIndex{};

	CUGEllipsisType  m_Ellipsis{};
	CUGMaskedEdit	 m_cugMaskEdit{};

	BOOL	m_bInit{};
	int		m_iRowHeight{};
};

