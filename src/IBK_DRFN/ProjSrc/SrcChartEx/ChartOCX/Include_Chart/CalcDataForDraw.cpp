// CalcDataForDraw.cpp: implementation of the CCalcDataForDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalcDataForDraw.h"

#define MAKENULL(pVal) pVal = NULL;
#define SAFE_DELETE(pVal) if(pVal) delete pVal; pVal = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCalcDataForDraw::CCalcDataForDraw()
{	
	// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
	//		Reset all value in ResetValue().
	//		But Do not reset Memory Pointer and Size Value in ResetValue().
	ResetValue();

	// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
	//		Memory Pointer
	MAKENULL(pptLine);
	MAKENULL(dCurOpen);
	MAKENULL(dCurClose);
	MAKENULL(dCurVolume);
	MAKENULL(rtGraph);
	MAKENULL(lpPoint_High);
	MAKENULL(lpPoint_Low);
	MAKENULL(lpPoint_Bohap);
	MAKENULL(lpPolyCount_High);
	MAKENULL(lpPolyCount_Low);
	MAKENULL(lpPolyCount_Bohap);
	MAKENULL(lpdwcntPoly_High);
	MAKENULL(lpdwcntPoly_Low);	
	MAKENULL(lpdwcntPoly_Bohap);
	MAKENULL( lpPointBuffer);

	// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
	//		Size Value
	nSize_pptLine = 0;
	nSize_pptLine = 0;
	nSize_dCurOpen = 0;
	nSize_dCurClose = 0;
	nSize_dCurVolume = 0;
	nSize_rtGraph = 0;
	nSize_lpPoint_High = 0;
	nSize_lpPolyCount_High = 0;
	nSize_lpdwcntPoly_High = 0;
	nSize_lpPoint_Low = 0;
	nSize_lpPolyCount_Low = 0;
	nSize_lpdwcntPoly_Low = 0;
	nSize_lpPoint_Bohap = 0;
	nSize_lpPolyCount_Bohap = 0;
	nSize_lpdwcntPoly_Bohap = 0;
	nSize_lpPointBuffer		= 0;
}

CCalcDataForDraw::~CCalcDataForDraw()
{
	// (2009/2/27 - Seung-Won, Bae) Temporary Pointer (no memory)
	MAKENULL( m_pTemp);

	SAFE_DELETE(pptLine);
	SAFE_DELETE(dCurOpen);
	SAFE_DELETE(dCurClose);
	SAFE_DELETE(dCurVolume);
	SAFE_DELETE(rtGraph);
	SAFE_DELETE(lpPoint_High);
	SAFE_DELETE(lpPoint_Low);
	SAFE_DELETE(lpPoint_Bohap);
	SAFE_DELETE(lpPolyCount_High);
	SAFE_DELETE(lpPolyCount_Low);
	SAFE_DELETE(lpPolyCount_Bohap);
	SAFE_DELETE(lpdwcntPoly_High);
	SAFE_DELETE(lpdwcntPoly_Low);	
	SAFE_DELETE(lpdwcntPoly_Bohap);	
	SAFE_DELETE( lpPointBuffer);
}

bool CCalcDataForDraw::CheckYValue(double dMaxValue, double dMinValue, bool bLog, bool bInvert, const CRect& graphRegion, int nCntPoint)
{
	bool bResult = TRUE;

	if(m_dMax	 != dMaxValue)		bResult = FALSE;
	if(m_dMin	 != dMinValue)		bResult = FALSE;
	if(m_bLog  	 != bLog)			bResult = FALSE;
	if(m_bInvert != bInvert)		bResult = FALSE;
	if(m_graphRegion != graphRegion)bResult = FALSE;
	if(m_nPoints     != nCntPoint)  bResult = FALSE;
	return bResult;
}

void CCalcDataForDraw::SetYValue(double dMaxValue, double dMinValue, bool bLog, bool bInvert, const CRect& graphRegion, int nCntPoint)
{
	m_dMax			= dMaxValue;
	m_dMin			= dMinValue;
	m_bLog			= bLog;
	m_bInvert		= bInvert;
	m_graphRegion	= graphRegion;
	m_nPoints	    = nCntPoint;
}

// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
void CCalcDataForDraw::ResetValue( void)
{
	m_dMax = 0.0;
	m_dMin = 0.0;
	m_bLog = false;
	m_bInvert = false;
	m_graphRegion = CRect(0,0,0,0);
	m_nPoints	  = 0;
	m_Prept = CPoint(0,0);

	m_nS_High = 0;
	m_nH_High = 0;
	m_nL_High = 0;
	m_nE_High = 0;
	m_nS_Low  = 0;
	m_nH_Low  = 0;
	m_nL_Low  = 0;
	m_nE_Low  = 0;

	m_nCurXHigh = 0;
	m_nCurXLow  = 0;
	m_nHighLow  = 0;

	nFlag		= 0;
	nDTI_Start	= 0;
	nDTI_End	= 0;
	nMaxTop		= INT_MAX;
	nPreLeft    = 0;
	nDTI_Start_LastPixel = 0;
	nIndex_LastPixel	 = 0;
	nArrowIndex			 = 0;
	dPreClose			 = 0;
	dPreVolume			 = 0;
	nCount_High			 = 0;
	nCount_Low			 = 0;
	nCount_Bohap	  	 = 0;
	nCount_High2		 = 0;
	nCount_Low2			 = 0;
	nCount_Bohap2	  	 = 0;
	nPtCnt				 = 0;
	nLastPixel_high		 = 0;
	nLastPixel_Low		 = 0;
	nLastPixel_Bohap	 = 0;
	nLastPolyCnt_high    = 0;
	nLastPolyCnt_Low	 = 0;
	nLastPolyCnt_Bohap	 = 0;
	nPolyCount_High			= 0;
	nPointCount_High		= 0;
	nLastPolyCount_High		= 0;
	nLastPointCount_High	= 0;
	nPolyCount_Low			= 0;
	nPointCount_Low			= 0;
	nLastPolyCount_Low		= 0;
	nLastPointCount_Low		= 0;

	// (2009/2/27 - Seung-Won, Bae) Temporary Pointer (no memory)
	MAKENULL( m_pTemp);
}
#define _CDFD_DEFINE( Type, Member)							\
	void CCalcDataForDraw::Alloc_##Member( int p_nSize)		\
	{														\
		if( p_nSize <= nSize_##Member) return;				\
		if( Member) delete [] Member;						\
		nSize_##Member = p_nSize;							\
		Member = new Type[ nSize_##Member];					\
		Allocated( nSize_##Member);							\
	}
_CDFD_DEFINE( POINT, pptLine);
_CDFD_DEFINE( double, dCurOpen);
_CDFD_DEFINE( double, dCurClose);
_CDFD_DEFINE( double, dCurVolume);
_CDFD_DEFINE( CRect, rtGraph);
_CDFD_DEFINE( POINT, lpPoint_High);
_CDFD_DEFINE( INT, lpPolyCount_High);
_CDFD_DEFINE( DWORD, lpdwcntPoly_High);
_CDFD_DEFINE( POINT, lpPoint_Low);
_CDFD_DEFINE( INT, lpPolyCount_Low);
_CDFD_DEFINE( DWORD, lpdwcntPoly_Low);
_CDFD_DEFINE( POINT, lpPoint_Bohap);
_CDFD_DEFINE( INT, lpPolyCount_Bohap);
_CDFD_DEFINE( DWORD, lpdwcntPoly_Bohap);
_CDFD_DEFINE( POINT,	lpPointBuffer);

void CCalcDataForDraw::Allocated( int p_nSize)
{
	ASSERT( TRUE);
}
