// DrawSingleton.cpp: implementation of the CDrawSingleton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawSingleton.h"

#include "BarDraw.h"
#include "BongDraw.h"
#include "LineDraw.h"
#include "PieDraw.h"
#include "SpecialDraw.h"
#include "FundDraw.h"
#include "DataBarDraw.h"
#include "OptionDraw.h"
#include "SingleBarDraw.h"
#include "PairBarDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//CBarDraw CDrawSingleton::m_BarDraw;
CBongDraw CDrawSingleton::m_BongDraw;
CLineDraw CDrawSingleton::m_LineDraw;
CPieDraw CDrawSingleton::m_PieDraw;
CSpecialDraw CDrawSingleton::m_SpecialDraw;
CFundDraw CDrawSingleton::m_FundDraw;
CDataViewDraw CDrawSingleton::m_DataViewDraw;
COptionDraw CDrawSingleton::m_OptionDraw;
CSingleBarDraw CDrawSingleton::m_SingleBarDraw;
CPairBarDraw CDrawSingleton::m_PairBarDraw;

CDraw* CDrawSingleton::Find( CGraphBaseData::GRAPHTYPE graphType, CGraphStyle& graphStyle, const CDrawingGraphData& drawingGraphData)
{
	CDraw *pDraw = NULL;
	switch(graphType)
	{
	case CGraphBaseData::Bong_Type: pDraw = &m_BongDraw;
									break;
	case CGraphBaseData::Bar_Type:
		{
			if(graphStyle.GetGraphBarStyle() == CGraphBaseData::SellByTextHorizontalBar)
					pDraw = &m_PairBarDraw;
			else	pDraw = &m_SingleBarDraw;
			break;
		}
	case CGraphBaseData::Line_Type:	pDraw = &m_LineDraw;
									break;
	case CGraphBaseData::Pie_Type:	pDraw = &m_PieDraw;
									break;
	case CGraphBaseData::Special_Type:	pDraw = &m_SpecialDraw;
										break;
	case CGraphBaseData::Fund_Type:	pDraw = &m_FundDraw;
									break;
	case CGraphBaseData::DataView_Type:	pDraw = &m_DataViewDraw;
										break;
	case CGraphBaseData::FuOp_Type:	pDraw = &m_OptionDraw;
									break;
	}

	if( pDraw)
	{
		pDraw->m_hOcxWnd = drawingGraphData.GetOcxHwnd();
		pDraw->m_eChartMode = drawingGraphData.GetChartMode();
	}

	return pDraw;
}