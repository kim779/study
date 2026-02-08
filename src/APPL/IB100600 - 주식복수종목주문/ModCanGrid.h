// ModCanGrid.h: interface for the CModCanGrid class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../../control/fx_GridEx/UGCtrl.h"
#include "../../control/fx_GridEx/UGCell.h"
#include "../../control/fx_GridEx/UGCBType.h"
#include "../../control/fx_GridEx/UGCTSpin.h"
//#include "../../control/fx_GridEx/UGCTelps.h"
#include "../../control/fx_GridEx/UTMaskedEdit.h"

class CModCanGrid : public CUGCtrl  
{
public:
	void ConditionSet(int nCol, CString strVal);

	virtual void OnSetup();
	virtual void OnDrawFocusRect(CDC *dc,RECT *rect);
	virtual void OnSetCell(int col,long row,CUGCell *cell);
	virtual void OnCellChange(int oldcol,int newcol,long oldrow,long newrow);
	virtual int OnCanSizeCol(int col);
	void SetSCData(CStringArray *pArr, BOOL bHoga, BOOL bStdPrc = FALSE);
	void GetCheckCode(CStringArray *pArr);
	void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	void GetSendData(CStringArray *pArr);
	void SetOrderResult(CStringArray *pArr);
	void SetPallete();
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	void ChangeFont(CFont* pFont, double* lrate);


protected:
	void FormatThousand(CString& strVal);
	int OnCheckbox(long ID,int col,long row,long msg,long param);
	BOOL IsChecked(long nRow);
	void SetCheckState(int nRow, BOOL bState);

private:
	std::unique_ptr<CPen> m_pLinePen = nullptr;
	BOOL m_bInit = FALSE;
	int m_nFontIDNormal=0;
	int m_nFontIDBold=0;
	int m_iRowHeight=0;
};

