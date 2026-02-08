// OrderGrid.h: interface for the COrderGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"
#include "../../control/fx_GridEx/UGCell.h"
#include "../../control/fx_GridEx/UGCBType.h"
#include "../../control/fx_GridEx/UGCTSpin.h"
#include "../../control/fx_GridEx/UGCTelps.h"

class COrderGrid : public CUGCtrl  
{
public:
	COrderGrid();
	virtual ~COrderGrid();

private:
	int OnDropList(long ID,int col,long row,long msg,long param);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	int OnSpinButton(long ID,int col,long row,long msg,long param);
	int OnEllipsisButton(long ID,int col,long row,long msg,long param);
	int OnEditFinish(int col, long row,CWnd* edit,LPCTSTR string,BOOL cancelFlag);

public:
	CString Parser(CString &srcstr, CString substr);
	void SetPallete();
	BOOL IsCodeComplete(long nRow);
	void GetCheckCode(CStringArray* pArr);
	void GetSendData(CStringArray* pArr);
	void SetOrderResult(CStringArray* pArr);
	void SetFOCData(CStringArray* pArr, BOOL bHoga);
	void SetTriggerData(CString strCode, CString strCurPrice);
	void SetOrderType(CString strVal);
	void SetQuantity(CString strVal);
	void SetOrderPrice(CString strVal);
	void SetOrderState(CString strVal);
	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual int OnCanSizeCol(int col);

	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void OnCharDown(UINT *vcKey,BOOL processed);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);

private:
	BOOL IsChecked(long nRow);
	void CodeComplete(int nRow, BOOL bSet);
	void InitHistoryCode(CStringList* pList);
	void SetCheckState(int nRow, BOOL bState);
	void UpdateCheckedRowData(int nCol, CString strVal);

	BOOL m_bInit;
	CPen* m_pLinePen;
	int m_nFontIDNormal;
	int m_nFontIDBold;
	int m_nSpinButtonIndex;
	int m_nCodeComboIndex;
	int	m_nEllipsisIndex;

	CUGCheckBoxType m_CheckBox;
	CUGSpinButtonType m_SpinButton;
	CUGEllipsisType	m_Ellipsis;
};

