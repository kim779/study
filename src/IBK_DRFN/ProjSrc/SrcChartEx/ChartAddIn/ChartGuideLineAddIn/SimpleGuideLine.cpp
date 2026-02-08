// SimpleGuideLine.cpp: implementation of the CSimpleGuideLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleGuideLine.h"

#include <math.h>										// for HUGE_VAL
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for ML_LANGUAGE_RES


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleGuideLine::CSimpleGuideLine( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2009/7/26 - Seung-Won, Bae) Manage property
	m_nBlockRow		= -1;
	m_nBlockColumn	= -1;
	m_nVScaleIndex	= -1;
	m_dYValue		= DBL_MIN;
	m_nLineStyle	= 0;
	m_nThickness	= 0;
	m_clrColor		= RGB( 0, 0, 0);

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();
}

CSimpleGuideLine::~CSimpleGuideLine()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CSimpleGuideLine::m_strAddInItemName = "SIMPLE_GUIDE_LINE";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSimpleGuideLine)
	ONADDINEVENT(OnDrawBeforeGraph)
END_ADDIN_EVENT_MAP()


// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CSimpleGuideLine::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
//	Chart1.GetProperty("LoadAddIn(ChartGuideLineAddIn.dll:SIMPLE_GUIDE_LINE)")
//	#                                 BlockPosRow.Column:VScaleIndex:YValue:Thickness:LineStyle:ColorR.G.B
//	Chart1.GetProperty("InvokeAddInStr(ChartGuideLineAddIn.dll:SIMPLE_GUIDE_LINE,,0.0:0:0:3:0:255.0.0)")

	CString strData( p_szData);		// BlockPosRow.Column:VScaleIndex:YValue:Thickness:LineStyle:ColorR.G.B

	// 0. Get the Block Position
	int nIndex = strData.Find( ':');
	if( nIndex < 0) return FALSE;
	if( 0 != nIndex)
	{
		CString strBlockPosition = strData.Left( nIndex);
		strData = strData.Mid( nIndex + 1);

		// 0.1 Get the Row Position
		nIndex = strBlockPosition.Find( '.');
		if( nIndex <= 0) return FALSE;
		m_nBlockRow = atol( strBlockPosition.Left( nIndex));
		strBlockPosition = strBlockPosition.Mid( nIndex + 1);
		// 0.2 Get the Column Position
		if( strBlockPosition.IsEmpty()) return FALSE;
		m_nBlockColumn = atol( strBlockPosition);
	}

	// 0.5 Get the Vertical Scale Index
	nIndex = strData.Find( ':');
	if( nIndex < 0) return FALSE;
	if( 0 != nIndex) m_nVScaleIndex = atoi( strData.Left( nIndex));
	strData = strData.Mid( nIndex + 1);

	// 1. Get the Y Value
	nIndex = strData.Find( ':');
	if( nIndex < 0) return FALSE;
	if( 0 != nIndex) m_dYValue = atof( strData.Left( nIndex));
	strData = strData.Mid( nIndex + 1);

	// 2. Get the Thickness
	nIndex = strData.Find( ':');
	if( nIndex < 0) return FALSE;
	if( 0 != nIndex) m_nThickness = atoi( strData.Left( nIndex));
	strData = strData.Mid( nIndex + 1);

	// 2.5 Get the Line Style
	nIndex = strData.Find( ':');
	if( nIndex < 0) return FALSE;
	if( 0 != nIndex) m_nLineStyle = atoi( strData.Left( nIndex));
	strData = strData.Mid( nIndex + 1);

	// 3. Get the Color
	if( strData.IsEmpty()) return TRUE;
	// 3.1 Get the Red
	nIndex = strData.Find( '.');
	if( nIndex <= 0) return FALSE;
	int nRed = atol( strData.Left( nIndex));
	strData = strData.Mid( nIndex + 1);
	// 3.2 Get the Green
	nIndex = strData.Find( '.');
	if( nIndex <= 0) return FALSE;
	int nGreen = atol( strData.Left( nIndex));
	strData = strData.Mid( nIndex + 1);
	// 3.3 Get the Blue
	if( strData.IsEmpty()) return TRUE;
	int nBlue = atol( strData);
	m_clrColor = RGB( nRed, nGreen, nBlue);

	return TRUE;
}


void CSimpleGuideLine::OnDrawBeforeGraph(HDC p_hDC)
{
	ML_SET_LANGUAGE_RES();

	CDC * pDC = CDC::FromHandle(p_hDC);
	if( !pDC) return;

	// 0. Retrieve the Drawing ENV.
	IBlock *pIBlock = m_pIChartManager->GetBlock( m_nBlockRow, m_nBlockColumn);
	if( !pIBlock) return;

	double dViewMax = 0.0, dViewMin = 0.0;
	pIBlock->GetVScaleGroupMinMax( m_nVScaleIndex, dViewMin, dViewMax);
	
	BOOL bLog = pIBlock->IsLogScale();
	BOOL bReverse = pIBlock->IsVScaleInvert( m_nVScaleIndex);
	CRect rctGraphDrawingRegion = pIBlock->GetGraphDrwingRegion();
	
	CRect rctGraphRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctGraphRegion, rctNoFrameAndMargin);

	pIBlock->Release();

	// (2007/1/15 - Seung-Won, Bae) Check No Data in Block
	if( dViewMin == dViewMax && dViewMax == -HUGE_VAL) return;

	// 1. Line Position를 산출한다. (View의 Data Min/Max와 Graph Region을 이용한다.)
	int nLineYPos = m_pIChartOCX->ConvertDataToYPosition( m_dYValue, dViewMax, dViewMin,
						rctGraphDrawingRegion.top, rctGraphDrawingRegion.bottom, bLog, bReverse);

	CPen pnGuideLine( m_nLineStyle, m_nThickness, m_clrColor);
	CPen *ppnOld = pDC->SelectObject( &pnGuideLine);
	pDC->MoveTo( rctGraphRegion.left,	nLineYPos);
	pDC->LineTo( rctGraphRegion.right,	nLineYPos);
	pDC->SelectObject( ppnOld);
}
