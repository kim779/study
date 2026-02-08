// SetItemOfColorListBox.h: interface for the CSetItemOfColorListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETITEMOFCOLORLISTBOX_H__1EC85234_E51F_4B4B_852F_D79BBA906B01__INCLUDED_)
#define AFX_SETITEMOFCOLORLISTBOX_H__1EC85234_E51F_4B4B_852F_D79BBA906B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_Addin.h"		// for IGraphInfo
#include "BaseData.h"

class CListBoxColorPickerST;
class CSetItemOfColorListBox  
{
public:
	void SetItem( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void OnApply(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);

private:

	// 종찬(04/10/04) 선형채움식 수정
	void SetItemInCaseOfStylsIsLine(int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo = NULL);
	void SetItemInCaseOfStylsIsBar(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);

	void OnApplyInCaseOfStylsIsBar(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	// 종찬(04/10/04) 선형채움식 수정
	void OnApplyInCaseOfStylsIsLine(int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo = NULL);

	void InCaseOfTypeIsCandlestickandHighLowCloseBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void InCaseOfTypeIsLineBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);

	void InCaseOfStylsIsBong( IGraphInfo* p_pIGraphInfo, const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	// 종찬(04/10/04) 선형채움식 수정
	void InCaseOfStylsIsLine(const bool bSetUp, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox, IGraphInfo* p_pIGraphInfo = NULL);
	void InCaseOfStylsIsBar(const bool bSetUp, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);

	void SetItembyStyleandType(const bool bSetUp, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);

	void SetItem_INVERSELINE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_MACD( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST *pListBox);
	void SetItem_PIVOT(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_GLANCEBLANCE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_SUB_UPFILL_DOWNFILL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_RAINBOW(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_SUBGRAPHNAME(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_DAEGIMEMUL(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_VOLUME(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_PRICE(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	// 종찬(04/10/04) 선형채움식 수정
	void SetItem_SUB_UPFILL_DOWNFILL2( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_COLOR( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_UPDOWN( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	void SetItem_EQUAL( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST* pListBox);
	
	void OnApply_INVERSELINE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_MACD(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_DAEGIMEMUL(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_GLANCEBLANCE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_SUB_UPFILL_DOWNFILL( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex);
	void OnApply_RAINBOW(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_COLOR( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex);
	void OnApply_SUBGRAPHNAME(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_UPDOWN( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex);
	void OnApply_VOLUME(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	void OnApply_PRICE(CListBoxColorPickerST *pListBox, IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	// 종찬(04/10/04) 선형채움식 수정
	void OnApply_SUB_UPFILL_DOWNFILL2( IGraphInfo* p_pIGraphInfo, CListBoxColorPickerST *pListBox, int p_nSubGraphIndex);

	void OnApply_downcolor(	IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color);
	void OnApply_upcolor(	IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color);
	void OnApply_equalcolor(IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex, const COLORREF& color);

	bool IsLineChart( IGraphInfo* p_pIGraphInfo, int p_nSubGraphIndex);
	int GetVolumeType( IGraphInfo* p_pIGraphInfo);
};

#endif // !defined(AFX_SETITEMOFCOLORLISTBOX_H__1EC85234_E51F_4B4B_852F_D79BBA906B01__INCLUDED_)
