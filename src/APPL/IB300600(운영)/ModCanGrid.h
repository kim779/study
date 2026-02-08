// ModCanGrid.h: interface for the CModCanGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"
#include "../../control/fx_GridEx/UGCell.h"
#include "../../control/fx_GridEx/UGCBType.h"
#include "../../control/fx_GridEx/UGCTSpin.h"
#include "../../control/fx_GridEx/UGCTelps.h"

class CModCanGrid : public CUGCtrl  
{
public:
	CModCanGrid();
	virtual ~CModCanGrid();

private:
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	int OnSpinButton(long ID,int col,long row,long msg,long param);
	int OnDropList(long ID,int col,long row,long msg,long param);

public:
	void SetPallete();
	void SetOrderResult(CStringArray *pArr);
	void SetModCanResult(CStringArray* pArr);
	void GetSendData(CStringArray* pArr);
	void SetFOCData(CStringArray* pArr, BOOL bHoga);
	void GetCheckCode(CStringArray* pArr);
	void SetTriggerData(CString strCode, CString strCurPrice);
	BOOL IsCodeComplete(long nRow);
	void SetModCanType(CString strVal);
	void SetQuantity(CString strVal);
	void SetModCanPrice(CString strVal);
	void SetModCanState(CString strVal);
	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual int OnCanSizeCol(int col);

	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void OnCharDown(UINT *vcKey,BOOL processed);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);

private:
	void SetCheckState(int nRow, BOOL bState);
	void UpdateCheckedRowData(int nCol, CString strVal);
	BOOL IsChecked(long nRow);

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

