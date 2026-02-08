// ChartStyleDataController.cpp: implementation of the CChartStyleDataController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartStyleDataController.h"

#include <afxtempl.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "MainBlock.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "Color.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// Interface
///////////////////////////////////////////////////////////////////////////////
CChartStyleDataController::CChartStyleDataController( CMainBlock* pMainBlock, CIndicatorList* pIndicatorList)
{
	m_pMainBlock = pMainBlock;
	m_pIndicatorList = pIndicatorList;
}

// 지표에 관련된 color 를 가져오기
void CChartStyleDataController::GetStyleData_Indicator( CStyleData& styleData) const
{
	if( !m_pMainBlock || !m_pIndicatorList) return;

	GetPriceGraphColor(		styleData);
	GetPriceMAGraphColor(	styleData);
	GetVolumeGraphColor(	styleData);
	GetVolumeMAGraphColor(	styleData);
}

// 지표에 관련된 color 를 적용하기
void CChartStyleDataController::SetStyleData_Indicator( const CStyleData& styleData)
{
	if( !m_pMainBlock || !m_pIndicatorList) return;

	SetPriceGraphColor(		styleData);
	SetPriceMAGraphColor(	styleData);
	SetVolumeGraphColor(	styleData);
	SetVolumeMAGraphColor(	styleData);
}

///////////////////////////////////////////////////////////////////////////////
// Inner Routines
///////////////////////////////////////////////////////////////////////////////

// Price Chart
void CChartStyleDataController::GetPriceGraphColor( CStyleData& styleData) const
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART));
	if( !pIndicatorInfo) return;
	CGraphColor *pGraphColor = pIndicatorInfo->GetSubGraphColor_PointerType( 0);

	styleData.SetPriceUpColor(		pGraphColor->GetColor1());
	styleData.SetPriceDownColor(	pGraphColor->GetColor3());
	styleData.SetPriceEqualColor(	pGraphColor->GetColor5());
	styleData.SetPriceUpFill(		pGraphColor->GetUpGraphFill());
	styleData.SetPriceDownFill(		pGraphColor->GetDownGraphFill());
}
void CChartStyleDataController::SetPriceGraphColor( const CStyleData& styleData)
{
	POSITION psnIndicator = NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART), &psnIndicator);
	while( pIndicatorInfo)
	{
		CGraphColor *pGraphColor = pIndicatorInfo->GetSubGraphColor_PointerType( 0);
		if( !pGraphColor) continue;

		pGraphColor->SetColor1(			styleData.GetPriceUpColor());
		pGraphColor->SetColor3(			styleData.GetPriceDownColor());
		pGraphColor->SetColor5(			styleData.GetPriceEqualColor());
		pGraphColor->SetUpGraphFill(	styleData.GetPriceUpFill());
		pGraphColor->SetDownGraphFill(	styleData.GetPriceDownFill());
		m_pMainBlock->ChangeGraphDataFromIndicatorInfo( pIndicatorInfo->GetGraphName(), false);

		m_pIndicatorList->GetNextIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_CHART), &psnIndicator, pIndicatorInfo);
	}
}

// PriceMA
void CChartStyleDataController::GetPriceMAGraphColor( CStyleData& styleData) const
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_MA));
	if( !pIndicatorInfo) return;
	CList<CGraphColor*, CGraphColor*> subGraphColorList;
	if( !pIndicatorInfo->GetSubGraphColorList(subGraphColorList)) return;

	int nIndex = 0;
	POSITION pos = subGraphColorList.GetHeadPosition();
	while(pos != NULL) styleData.SetPriceMAColor( nIndex++, subGraphColorList.GetNext(pos)->GetColor1());
}
void CChartStyleDataController::SetPriceMAGraphColor( const CStyleData& styleData)
{
	POSITION psnIndicator = NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_MA), &psnIndicator);
	int nMACount = styleData.GetPriceMACount();
	while( pIndicatorInfo)
	{
		for( int i = 0; i < nMACount; i++)
		{
			CGraphColor *pGraphColor = pIndicatorInfo->GetSubGraphColor_PointerType( i);
			if( !pGraphColor) continue;
			pGraphColor->SetColor1( styleData.GetPriceMAColor( i));
		}
		m_pMainBlock->ChangeGraphDataFromIndicatorInfo( pIndicatorInfo->GetGraphName(), false);

		m_pIndicatorList->GetNextIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_MA), &psnIndicator, pIndicatorInfo);
	}
}

// Volume Chart
void CChartStyleDataController::GetVolumeGraphColor( CStyleData& styleData) const
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_VOLUME_CHART));
	if( !pIndicatorInfo) return;
	CGraphColor *pGraphColor = pIndicatorInfo->GetSubGraphColor_PointerType( 0);

	//sy 2005.04.19.
	styleData.SetTradeUpColor(		pGraphColor->GetColor1());
	styleData.SetTradeDownColor(	pGraphColor->GetColor3());
	styleData.SetTradeEqualColor(	pGraphColor->GetColor5());
	styleData.SetTradeUpFill(		pGraphColor->GetUpGraphFill());
	styleData.SetTradeDownFill(		pGraphColor->GetDownGraphFill());
}
void CChartStyleDataController::SetVolumeGraphColor( const CStyleData& styleData)
{
	POSITION psnIndicator = NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_VOLUME_CHART), &psnIndicator);
	while( pIndicatorInfo)
	{
		CGraphColor *pGraphColor = pIndicatorInfo->GetSubGraphColor_PointerType( 0);
		if( !pGraphColor) continue;

		pGraphColor->SetColor1(			styleData.GetTradeUpColor());
		pGraphColor->SetColor3(			styleData.GetTradeDownColor());
		pGraphColor->SetColor5(			styleData.GetTradeEqualColor());
		pGraphColor->SetUpGraphFill(	styleData.GetTradeUpFill());
		pGraphColor->SetDownGraphFill(	styleData.GetTradeDownFill());
		m_pMainBlock->ChangeGraphDataFromIndicatorInfo( pIndicatorInfo->GetGraphName(), false);

		m_pIndicatorList->GetNextIndicatorInfo( EILT_USER, _MTEXT( C2_VOLUME_CHART), &psnIndicator, pIndicatorInfo);
	}
}

// VolumeMA
void CChartStyleDataController::GetVolumeMAGraphColor( CStyleData& styleData) const
{
	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_VOLUME_MA));
	if( !pIndicatorInfo) return;
	CList<CGraphColor*, CGraphColor*> subGraphColorList;
	if( !pIndicatorInfo->GetSubGraphColorList(subGraphColorList)) return;

	int nIndex = 0;
	POSITION pos = subGraphColorList.GetHeadPosition();
	while(pos != NULL) styleData.SetPriceMAColor( nIndex++, subGraphColorList.GetNext(pos)->GetColor1());
}
void CChartStyleDataController::SetVolumeMAGraphColor( const CStyleData& styleData)
{
	POSITION psnIndicator = NULL;
	CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetFirstIndicatorInfo( EILT_USER, _MTEXT( C2_VOLUME_MA), &psnIndicator);
	int nMACount = styleData.GetPriceMACount();
	while( pIndicatorInfo)
	{
		for( int i = 0; i < nMACount; i++)
		{
			CGraphColor *pGraphColor = pIndicatorInfo->GetSubGraphColor_PointerType( i);
			if( !pGraphColor) continue;
			pGraphColor->SetColor1( styleData.GetPriceMAColor( i));
		}
		m_pMainBlock->ChangeGraphDataFromIndicatorInfo( pIndicatorInfo->GetGraphName(), false);

		m_pIndicatorList->GetNextIndicatorInfo( EILT_USER, _MTEXT( C2_PRICE_MA), &psnIndicator, pIndicatorInfo);
	}
}
