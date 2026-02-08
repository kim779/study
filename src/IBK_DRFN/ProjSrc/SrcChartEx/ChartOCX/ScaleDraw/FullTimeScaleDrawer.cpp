// FullTimeScaleDrawer.cpp: implementation of the CFullTimeScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FullTimeScaleDrawer.h"

#include "../Include_Chart/DLL_Load/XScaleManager.h"		// for CXScaleManager
#include "../Include_Chart/DLL_Load/PacketListManager.h"	// for CPacketListManager
#include "../Include_Chart/DLL_Load/PacketList.h"			// for CPacketList
#include "../Include_Chart/DLL_Load/IMetaTable.h"			// for _LTEXT6()
#include "../Include_Chart/BaseData.h"						// for CDrawingHorzScaleData
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFullTimeScaleDrawer::CFullTimeScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CHorzScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

CFullTimeScaleDrawer::~CFullTimeScaleDrawer()
{

}

#define _STT_YEAR		0x0040
#define _STT_MONTH		0x0020
#define _STT_DAY		0x0010
#define _STT_HOUR		0x0008
#define _STT_MINUTE		0x0004
#define _STT_SECOND		0x0002
#define _STT_BOLD_GRID	0x0001
int CFullTimeScaleDrawer::GetScaleTextType( int p_nScaleType, CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const tm &p_tmPrev, const tm &p_tmDRI, int p_nSkipFactor)
{
	switch( p_nScaleType)
	{
		case 0:	switch( p_eScaleDrawType)
				{
					default:				return _STT_YEAR;
					case CScaleBaseData::HORZ_TICK:
											return _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_SECOND | _STT_BOLD_GRID;
				}
				break;
		case 1:	switch( p_eScaleDrawType)
				{
					default:	if( p_tmPrev.tm_year != p_tmDRI.tm_year)
											return _STT_YEAR | _STT_MONTH | _STT_BOLD_GRID;
								//	else month only like below !
					case CScaleBaseData::HORZ_TIME_ONE:	
					case CScaleBaseData::HORZ_TIME_TWO:
											return _STT_MONTH;
				}
				break;
		case 2:	switch( p_eScaleDrawType)
				{
					default:	if( 28 <= p_nSkipFactor)
								{
									if( p_tmPrev.tm_year != p_tmDRI.tm_year)
											return _STT_YEAR | _STT_MONTH | _STT_BOLD_GRID;
									else	return _STT_MONTH;
								}
								else if( 14 == p_nSkipFactor)
								{
									if( p_tmPrev.tm_year != p_tmDRI.tm_year)
											return _STT_YEAR | _STT_MONTH | _STT_DAY | _STT_BOLD_GRID;
									else	return _STT_MONTH | _STT_DAY;
								}
								else
								{
									if( p_tmPrev.tm_year != p_tmDRI.tm_year)
											return _STT_YEAR | _STT_MONTH | _STT_DAY | _STT_BOLD_GRID;
									else if( p_tmPrev.tm_mon != p_tmDRI.tm_mon)
											return _STT_MONTH | _STT_DAY | _STT_BOLD_GRID;
									else	return _STT_DAY;
								}
					case CScaleBaseData::HORZ_TIME_ONE:	
					case CScaleBaseData::HORZ_TIME_TWO:
								if( p_tmPrev.tm_year != p_tmDRI.tm_year
									|| p_tmPrev.tm_mon != p_tmDRI.tm_mon)
											return _STT_MONTH | _STT_DAY | _STT_BOLD_GRID;
								else		return _STT_DAY;
					case CScaleBaseData::HORZ_TIME_SECOND:
					case CScaleBaseData::HORZ_TICK:
											return _STT_DAY;
				}
				break;
		case 3: if( ( CChartInfo::HTS == m_eChartMode && 7 <= p_nSkipFactor) || ( CChartInfo::HTS != m_eChartMode && 24 <= p_nSkipFactor))
				{
					if( p_tmPrev.tm_year != p_tmDRI.tm_year
						|| p_tmPrev.tm_mon != p_tmDRI.tm_mon)
											return _STT_MONTH | _STT_DAY | _STT_BOLD_GRID;
					else					return _STT_DAY;
				}
		case 4:	switch( p_eScaleDrawType)
				{
					default:	if( p_tmPrev.tm_year != p_tmDRI.tm_year
									|| p_tmPrev.tm_mon != p_tmDRI.tm_mon
									|| p_tmPrev.tm_mday != p_tmDRI.tm_mday)
											return _STT_MONTH | _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_BOLD_GRID;
								else if( p_tmPrev.tm_hour != p_tmDRI.tm_hour && 4 == p_nScaleType && p_nSkipFactor < 60)
											return _STT_HOUR | _STT_MINUTE | _STT_BOLD_GRID;
								else		return _STT_HOUR | _STT_MINUTE;
					case CScaleBaseData::HORZ_TIME_SECOND:
					case CScaleBaseData::HORZ_TICK:
								if( p_tmPrev.tm_year != p_tmDRI.tm_year
									|| p_tmPrev.tm_mon != p_tmDRI.tm_mon
									|| p_tmPrev.tm_mday != p_tmDRI.tm_mday)
											return _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_BOLD_GRID;
								else		return _STT_HOUR | _STT_MINUTE;
				}
				break;
		case 5:	if( p_tmPrev.tm_year != p_tmDRI.tm_year
					|| p_tmPrev.tm_mon != p_tmDRI.tm_mon
					|| p_tmPrev.tm_mday != p_tmDRI.tm_mday
					|| p_tmPrev.tm_hour != p_tmDRI.tm_hour
					|| p_tmPrev.tm_min != p_tmDRI.tm_min)
											return _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_SECOND | _STT_BOLD_GRID;
				else						return _STT_HOUR | _STT_MINUTE | _STT_SECOND;
	}
	return 0;
}

CString CFullTimeScaleDrawer::GetScaleText( int &p_nTextType, tm &p_tmDRI, CStringArray &p_astrTimeUnit, int p_nTimeDiff, int p_nScaleType, CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const tm &p_tmPrev, int p_nSkipFactor, int p_nScaleTextTypeMask, CXScaleManager *p_pxScaleManager, const CString &p_strBlockRQ)
{
#ifdef _DEBUG
//	p_nTextType = _STT_YEAR | _STT_MONTH | _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_SECOND;
#endif

	p_nTextType = GetScaleTextType( p_nScaleType, p_eScaleDrawType, p_tmPrev, p_tmDRI, p_nSkipFactor);

	// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
	// (2009/5/28 - Seung-Won, Bae) Do not bold on first grid.
	p_nTextType = p_nTextType & p_nScaleTextTypeMask;

	CString data;
	long lSpecialTimeCode = 0;
	if( p_pxScaleManager->IsSpecialTime( p_strBlockRQ, mktime( &p_tmDRI), lSpecialTimeCode))
			switch( lSpecialTimeCode)
	{
		case 3100:
		case 310000:	data = _LTEXT6( C6_END_OF_TRADING);
						break;
		case 4100:
		case 410000:	data = _LTEXT6( C6_END_OF_OVERTIME_TRADE);
						break;
		case 7100:
		case 710000:	data = _LTEXT6( C6_AFTER_CLOSING);
						break;
		case 8100:
		case 810000:	data = _LTEXT6( C6_END_OF_SINGLE_PRICE);
						break;
		default:		data = "";
						break;
	}
	else	switch( p_nTextType & ~( _STT_BOLD_GRID))
	{
		case _STT_YEAR:
				data.Format( "%04d%s", p_tmDRI.tm_year + 1900, p_astrTimeUnit[ 6]);
				break;
		case _STT_YEAR | _STT_MONTH:
				data.Format( "%04d%s%02d%s", p_tmDRI.tm_year + 1900, p_astrTimeUnit[ 0], p_tmDRI.tm_mon + 1, p_astrTimeUnit[ 7]);
				break;
		case _STT_YEAR | _STT_MONTH | _STT_DAY:
				data.Format( "%04d%s%02d%s%02d%s", p_tmDRI.tm_year + 1900, p_astrTimeUnit[ 0], p_tmDRI.tm_mon + 1, p_astrTimeUnit[ 1], p_tmDRI.tm_mday, p_astrTimeUnit[ 8]);
				break;
		case _STT_YEAR | _STT_MONTH | _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_SECOND:
				data.Format( "%04d%s%02d%s%02d%s,%02d%s%02d%s", p_tmDRI.tm_year + 1900, p_astrTimeUnit[ 0], p_tmDRI.tm_mon + 1, p_astrTimeUnit[ 1], p_tmDRI.tm_mday, p_astrTimeUnit[ 8], p_tmDRI.tm_hour, p_astrTimeUnit[ 3], p_tmDRI.tm_min, p_astrTimeUnit[ 4], p_tmDRI.tm_sec, p_astrTimeUnit[ 11]);
				break;
		case _STT_MONTH:
				data.Format( "%02d%s", p_tmDRI.tm_mon + 1, p_astrTimeUnit[ 7]);
				break;
		case _STT_MONTH | _STT_DAY:
				data.Format( "%02d%s%02d%s", p_tmDRI.tm_mon + 1, p_astrTimeUnit[ 1], p_tmDRI.tm_mday, p_astrTimeUnit[ 8]);
				break;
		case _STT_MONTH | _STT_DAY | _STT_HOUR | _STT_MINUTE:
				data.Format( "%02d%s%02d%s,%02d%s%02d%s", p_tmDRI.tm_mon + 1, p_astrTimeUnit[ 1], p_tmDRI.tm_mday, p_astrTimeUnit[ 8], p_tmDRI.tm_hour, p_astrTimeUnit[ 3], p_tmDRI.tm_min, p_astrTimeUnit[ 10]);
				break;
		case _STT_DAY:
				data.Format( "%02d%s", p_tmDRI.tm_mday, p_astrTimeUnit[ 8]);
				break;
		case _STT_DAY | _STT_HOUR | _STT_MINUTE:
				data.Format( "%02d%s,%02d%s%02d%s", p_tmDRI.tm_mday, p_astrTimeUnit[ 8], p_tmDRI.tm_hour, p_astrTimeUnit[ 3], p_tmDRI.tm_min, p_astrTimeUnit[ 10]);
				break;
		case _STT_DAY | _STT_HOUR | _STT_MINUTE | _STT_SECOND:
				data.Format( "%02d%s,%02d%s%02d%s%02d%s", p_tmDRI.tm_mday, p_astrTimeUnit[ 8], p_tmDRI.tm_hour, p_astrTimeUnit[ 3], p_tmDRI.tm_min, p_astrTimeUnit[ 4], p_tmDRI.tm_sec, p_astrTimeUnit[ 11]);
				break;
		case _STT_HOUR | _STT_MINUTE:
				data.Format( "%02d%s%02d%s", p_tmDRI.tm_hour, p_astrTimeUnit[ 3], p_tmDRI.tm_min, p_astrTimeUnit[ 10]);
				break;
		case _STT_HOUR | _STT_MINUTE | _STT_SECOND:
				data.Format( "%02d%s%02d%s%02d%s", p_tmDRI.tm_hour, p_astrTimeUnit[ 3], p_tmDRI.tm_min, p_astrTimeUnit[ 4], p_tmDRI.tm_sec, p_astrTimeUnit[ 11]);
				break;
	}
	return data;
}

bool CFullTimeScaleDrawer::DrawTextOrGrid( CDC* pDC, const CDrawingHorzScaleData &drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	if( !pxScaleManager) return false;

	// (2009/1/22 - Seung-Won, Bae) Use selected RQ's scale draw type.
	CPacketListManager *pPacketListManager = pxScaleManager->GetPacketListManager();
	if( !pPacketListManager) return false;
	CString strMainRQ = pPacketListManager->GetIChartCtrl()->GetCurrentRQ();
	CPacketList *pPacketList = pPacketListManager->GetPacketList( strMainRQ);
	if( !pPacketList) return false;
	CScaleBaseData::HORZSCALEDRAWERTYPE eScaleDrawType = pPacketList->GetDateUnitType();

	CDataBlockList< int> *pblScaleDRI = NULL;
	CDataBlockList< tm> *pblScaleTime = NULL;
	int nSkipFactor = 1;
	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	CString strPacketType = pxScaleManager->GetPacketType( "");

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//int nScaleType = pxScaleManager->GetScaleDRIandTime( drawingHorzScaleData.GetColumnIndex(), pblScaleDRI, pblScaleTime, scaleRegion.Width(), eScaleDrawType, pPacketList->GetTimeInterval(), p_szRQ, nSkipFactor, strPacketType);
	int nColumnIndex = drawingHorzScaleData.GetColumnIndex();
	int nScaleWidth = scaleRegion.Width();
	int nTimeInterval = pPacketList->GetTimeInterval();

	CString strFormat = strPacketType;	// 디폴트포맷
	if (strPacketType == "YYYYMMDDHHMMSS")
	{
		// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
		switch(eScaleDrawType)
		{
		case CScaleBaseData::HORZ_DAILY:		// 일
		case CScaleBaseData::HORZ_WEEKLEY:		// 주 
			strFormat = _T("YYYYMMDD");		
			break;
		case CScaleBaseData::HORZ_MONTHLY:		// 월 
			strFormat = _T("YYYYMM");		
			break;
		case CScaleBaseData::HORZ_TIME_ONE:		// 분
		case CScaleBaseData::HORZ_TIME_TWO:
			strFormat = _T("MMDDHHMM");
			break;
		case CScaleBaseData::HORZ_TICK:			// 틱 
		case CScaleBaseData::HORZ_TIME_SECOND:
			strFormat = _T("DDHHMMSS");
			break;
		default:
			strFormat = _T("YYYYMMDD");		
			break;
		}
	}
	int nScaleType = pxScaleManager->GetScaleDRIandTime( nColumnIndex, pblScaleDRI, pblScaleTime, nScaleWidth, eScaleDrawType, nTimeInterval, p_szRQ, nSkipFactor, strFormat);
	// 2011.01.27 by SYS <<
	if( nScaleType < 0) return false;

	int nDRICount = pblScaleDRI->GetItemCount();
	BOOL bReverse = pxScaleManager->GetReverse();
	int nDRI_Start = -1;
	const int *pIReturn = pblScaleDRI->GetAt( 0);
	if( pIReturn) nDRI_Start = *pIReturn;

	// 그리기 위한 기본 data를 가져온다.
	double onePtWidth = 0.0;
	int posPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment(scaleRegion, drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posPt_y);

	int pre_textEndPos = 0;
	if(bReverse == FALSE)	pre_textEndPos = 0;
	else					pre_textEndPos = scaleRegion.right;

	// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//int nScaleTextTypeMask = GetScaleTextTypeMask( strPacketType);
	int nScaleTextTypeMask = GetScaleTextTypeMask( strFormat);
	// 2011.01.27 by SYS <<

	// (2009/6/4 - Seung-Won, Bae) for one time from loop.
	CScaleCompart &scaleCompart = horzScale.GetScaleCompart();
	CStringArray astrTimeUnit;
	astrTimeUnit.SetSize( 12);
	astrTimeUnit.SetAt( 0, scaleCompart.GetDateCompart());
	astrTimeUnit.SetAt( 1, scaleCompart.GetDateCompart());
	astrTimeUnit.SetAt( 2, scaleCompart.GetDateCompart());
	astrTimeUnit.SetAt( 3, scaleCompart.GetTimeCompart());
	astrTimeUnit.SetAt( 4, scaleCompart.GetTimeCompart());
	astrTimeUnit.SetAt( 5, scaleCompart.GetTimeCompart());
	if( 1 < scaleCompart.GetDateCompart().GetLength())
	{
		astrTimeUnit.SetAt( 0, _LTEXT6( C6_YEAR));
		astrTimeUnit.SetAt( 6, _LTEXT6( C6_YEAR));
		astrTimeUnit.SetAt( 1, _LTEXT6( C6_MONTH));
		astrTimeUnit.SetAt( 7, _LTEXT6( C6_MONTH));
		astrTimeUnit.SetAt( 2, _LTEXT6( C6_DAY));
		astrTimeUnit.SetAt( 8, _LTEXT6( C6_DAY));
	}
	if( 1 < scaleCompart.GetTimeCompart().GetLength())
	{
		astrTimeUnit.SetAt( 3,	_LTEXT6( C6_HOUR));
		astrTimeUnit.SetAt( 9,	_LTEXT6( C6_HOUR));
		astrTimeUnit.SetAt( 4,	_LTEXT6( C6_MINUTE));
		astrTimeUnit.SetAt( 10,	_LTEXT6( C6_MINUTE));
		astrTimeUnit.SetAt( 5,	_LTEXT6( C6_SECOND));
		astrTimeUnit.SetAt( 11,	_LTEXT6( C6_SECOND));
	}

	// (2009/10/1 - Seung-Won, Bae) Caclulate TimeDiff of BlockRQ from MainRQ.
	int nTimeDiff = 0;
	if( p_szRQ) if( strMainRQ != p_szRQ)
	{
		CPacketList *pPacketListBlock = pPacketListManager->GetPacketList( p_szRQ);
		if( pPacketListBlock)
			nTimeDiff = pPacketListBlock->GetBaseTimeDifference() - pPacketList->GetBaseTimeDifference();
	}

	CString data;
	int posPt_x = 0;
	int nScaleTextType = 0, nPrevScaleTextType = 0;
	int nDRI = -1;
	tm tmDRI, tmPrev;
	memset( &tmDRI, 0, sizeof( tm));
	//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
	tmDRI.tm_isdst = -1;
	tmPrev.tm_isdst = -1;

	pblScaleDRI->SetIndex( 0);
	pblScaleTime->SetIndex( 0);
	CXScaleManager::PAREA parea;
	const tm *pMReturn = NULL;
	CStringArray	saScaleText;
	CUIntArray		saScalePos;
	int i = 0;
	for( i = 0; i < nDRICount; i++)
	{
		pIReturn = pblScaleDRI->GetNext();
		if( pIReturn) nDRI = *pIReturn;

		tmPrev = tmDRI;
		pMReturn = pblScaleTime->GetNext();
		if( pMReturn) tmDRI = *pMReturn;

		pxScaleManager->GetAreaFromDRI( drawingHorzScaleData.GetColumnIndex(), nDRI, &parea);
		if( !parea) continue;

//		posPt_x = parea->center;
		posPt_x = bReverse ? parea->right : parea->left;

		data = GetScaleText( nScaleTextType, tmDRI, astrTimeUnit, nTimeDiff, nScaleType, eScaleDrawType, tmPrev, nSkipFactor, ( nScaleTextTypeMask | ( 0 < i ? _STT_BOLD_GRID : 0)), pxScaleManager, p_szRQ);

		// 해당된 경우에만 scale그린다.
		// 마지막에서 text를 표현 - ojtaso (20071227)
		// pre text와 겹칠 경우엔 그리지 않는다.
		if( nPrevScaleTextType < nScaleTextType
			|| !bReverse && pre_textEndPos < posPt_x
			|| bReverse && posPt_x < pre_textEndPos)
		{
			nPrevScaleTextType = nScaleTextType;

			saScaleText.Add( data);
			saScalePos.Add( posPt_x);

			CSize textSize = pDC->GetTextExtent(data);
			if(bReverse == FALSE)	pre_textEndPos = posPt_x + textSize.cx + MARGIN;
			else					pre_textEndPos = posPt_x - textSize.cx - MARGIN;
		}

		CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), posPt_x, nScaleTextType & _STT_BOLD_GRID ? 3 : 1);
	}

	if(bReverse == FALSE)	pre_textEndPos = scaleRegion.right;
	else					pre_textEndPos = 0;
	for( i = saScaleText.GetSize() - 1; 0 <= i; i--)
	{
		posPt_x = saScalePos.GetAt( i);
		data	= saScaleText.GetAt( i);

		// scale text 적을 공간 비교
		CSize textSize = pDC->GetTextExtent(data);

		// 해당된 경우에만 scale그린다.
		// 마지막에서 text를 표현 - ojtaso (20071227)
		// pre text와 겹칠 경우엔 그리지 않는다.
		if( bReverse && pre_textEndPos <= posPt_x - textSize.cx - MARGIN
			|| !bReverse && posPt_x + textSize.cx + MARGIN <= pre_textEndPos)
		{
			// text구분선 그리기
			CPoint nowPos( posPt_x, posPt_y);

			CHorzScaleDrawer::DrawTextCompartLine(pDC, scaleRegion, nowPos, (horzScale.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP ? true: false), drawingHorzScaleData.GetCompartLineColor());

			if(bReverse == FALSE)
			{
				// text 그리기
				CPoint textPosPt(nowPos.x +MARGIN, nowPos.y);
				if(scaleRegion.Height() > textSize.cy)
					CDrawer::DrawTextOut(pDC, textPosPt, data, drawingHorzScaleData.GetScaleColor().GetScaleTextColor());
			}
			else
			{
				// text 그리기
				CPoint textPosPt(nowPos.x - MARGIN, nowPos.y);
				textPosPt.x -= textSize.cx;
				if(scaleRegion.Height() > textSize.cy)
					CDrawer::DrawTextOut(pDC, textPosPt, data, drawingHorzScaleData.GetScaleColor().GetScaleTextColor());
			}
			pre_textEndPos = posPt_x;
		}
	}

	//KHD : 시간구분 그리기  - 전 스케일 데이터를 가져온다. 
	pxScaleManager->GetScaleDRIandTimeAllData( 2, pblScaleDRI, pblScaleTime, eScaleDrawType, p_szRQ, nSkipFactor);
	nDRICount = pblScaleDRI->GetItemCount();
	pblScaleDRI->SetIndex( 0);
	pblScaleTime->SetIndex( 0);

	BOOL bDivGrid_Min_Day = FALSE;
	BOOL bDivGrid_Day_Year = FALSE;
	BOOL bDivGrid_Day_Week = FALSE;
	BOOL bDivGrid_Day_Mon  = FALSE;

	if (eScaleDrawType == CScaleBaseData::HORZ_TIME_ONE || eScaleDrawType == CScaleBaseData::HORZ_TIME_TWO ||
		eScaleDrawType == CScaleBaseData::HORZ_TICK || eScaleDrawType == CScaleBaseData::HORZ_TIME_SECOND)	//분틱차트일때..
	{
		bDivGrid_Min_Day = horzScale.GetCheckTime(CScaleBaseData::MINTICK_DAY_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_DAILY)					//일차트일때..
	{
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_YEAR_GUBUN);
		bDivGrid_Day_Week = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);
		bDivGrid_Day_Mon  = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_MON_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_WEEKLEY)					//주차트일때..
	{
		bDivGrid_Day_Mon  = horzScale.GetCheckTime(CScaleBaseData::WEEKCHART_MON_GUBUN);
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::WEEKCHART_YEAR_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_MONTHLY)					//월차트일때..
	{
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::MONCHART_YEAR_GUBUN);
	}

	for(i = 0; i < nDRICount; i++)
	{
		pIReturn = pblScaleDRI->GetNext();
		if( pIReturn) nDRI = *pIReturn;

		tmPrev = tmDRI;
		pMReturn = pblScaleTime->GetNext();
		if( pMReturn) tmDRI = *pMReturn;

		pxScaleManager->GetAreaFromDRI( drawingHorzScaleData.GetColumnIndex(), nDRI, &parea);
		if( !parea)
			continue;

		// X좌표 조회구간의 시작일자와 종료일자 표기 - onlyojt
		int posPt_x = bReverse ? parea->right : parea->left;

		//KHD :  분/틱차트에서의 일자구분선, 년별 옵션 개별옵션 - 
		if( bDivGrid_Min_Day && i != 0) 
		{
			if(tmPrev.tm_mday != tmDRI.tm_mday)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::MINTICK_DAY_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::MINTICK_DAY_GUBUN), posPt_x, horzScale.GetGridWidth_Time(CScaleBaseData::MINTICK_DAY_GUBUN));
		}

		// 20150603 최성필 0012355: [0520] 일주기 차트에서 '주 구분선' 적용 시, 월요일이 휴장일일 경우, 제대로 나타나지 않는 오류 수정 >>
		// 주 구분선을 매주 월요일날로 지정한 것이 아닌, 이전 요일이(저번 주 금요일) 현재 요일보다 값이 크면, 한 주가 지난 것으로 간주하여 처리
		//if((bDivGrid_Day_Week ) && (tmDRI.tm_wday == 1 )&&i!=0)
		if( (bDivGrid_Day_Week ) && 
			( tmPrev.tm_wday > tmDRI.tm_wday || 
			(tmPrev.tm_wday <= tmDRI.tm_wday && ((tmDRI.tm_yday - tmPrev.tm_yday) >= 7)) )
	      )
		// 20150603 최성필 <<
		{
			CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_WEEK_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN));
		}

		if(bDivGrid_Day_Mon && i!=0)
		{
			if(tmPrev.tm_mon != tmDRI.tm_mon)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_MON_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_MON_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::WEEKCHART_MON_GUBUN));
		}

		if(bDivGrid_Day_Year)
		{	
			if((tmPrev.tm_year != tmDRI.tm_year) && i != 0)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_YEAR_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_YEAR_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::WEEKCHART_YEAR_GUBUN));
		}
	}
	return true;
}

bool CFullTimeScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	if( !pxScaleManager) return false;

	// (2009/1/22 - Seung-Won, Bae) Use selected RQ's scale draw type.
	CPacketListManager *pPacketListManager = pxScaleManager->GetPacketListManager();
	if( !pPacketListManager) return false;
	CString strMainRQ = pPacketListManager->GetIChartCtrl()->GetCurrentRQ();
	CPacketList *pPacketList = pPacketListManager->GetPacketList( strMainRQ);
	CScaleBaseData::HORZSCALEDRAWERTYPE eScaleDrawType = pPacketList->GetDateUnitType();
	CDataBlockList< int> *pblScaleDRI = NULL;
	CDataBlockList< tm> *pblScaleTime = NULL;
	int nSkipFactor = 1;
	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	CString strPacketType = pxScaleManager->GetPacketType( "");


	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//int nScaleType = pxScaleManager->GetScaleDRIandTime( drawingHorzScaleData.GetColumnIndex(), pblScaleDRI, pblScaleTime, horzScaleRegion.GetBottomScaleRegion().Width(), eScaleDrawType, pPacketList->GetTimeInterval(), p_szRQ, nSkipFactor, strPacketType);
	int nColumnIndex = drawingHorzScaleData.GetColumnIndex();
	int nScaleWidth = horzScaleRegion.GetBottomScaleRegion().Width();
	int nTimeInterval = pPacketList->GetTimeInterval();
	
	CString strFormat = strPacketType;	// 디폴트포맷
	if (strPacketType == "YYYYMMDDHHMMSS")
	{
		// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
		switch(eScaleDrawType)
		{
		case CScaleBaseData::HORZ_DAILY:		// 일
		case CScaleBaseData::HORZ_WEEKLEY:		// 주 
			strFormat = _T("YYYYMMDD");		
			break;
		case CScaleBaseData::HORZ_MONTHLY:		// 월 
			strFormat = _T("YYYYMM");		
			break;
		case CScaleBaseData::HORZ_TIME_ONE:		// 분
		case CScaleBaseData::HORZ_TIME_TWO:
			strFormat = _T("MMDDHHMM");
			break;
		case CScaleBaseData::HORZ_TICK:			// 틱 
		case CScaleBaseData::HORZ_TIME_SECOND:
			strFormat = _T("DDHHMMSS");
			break;
		default:
			strFormat = _T("YYYYMMDD");		
			break;
		}
	}
	int nScaleType = pxScaleManager->GetScaleDRIandTime( nColumnIndex, pblScaleDRI, pblScaleTime, nScaleWidth, eScaleDrawType, nTimeInterval, p_szRQ, nSkipFactor, strFormat);
	// 2011.01.27 by SYS <<
	if( nScaleType < 0) return false;

	int nDRICount = pblScaleDRI->GetItemCount();
	BOOL bReverse = pxScaleManager->GetReverse();
	int nDRI_Start = -1;
	const int *pIReturn = pblScaleDRI->GetAt( 0);
	if( pIReturn) nDRI_Start = *pIReturn;

	int pre_textEndPos = 0;
	if(bReverse == FALSE)	pre_textEndPos = 0;
	else					pre_textEndPos = horzScaleRegion.GetBottomScaleRegion().right;

	// 그리기 위한 기본 data를 가져온다.
	double onePtWidth = 0.0;
	int posTopPt_y = 0, posBottomPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment( horzScaleRegion.GetTopScaleRegion(), drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posTopPt_y);
	CHorzScaleDrawer::GetDrawingEnvironment( horzScaleRegion.GetBottomScaleRegion(), drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posBottomPt_y);

	// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//int nScaleTextTypeMask = GetScaleTextTypeMask( strPacketType);
	int nScaleTextTypeMask = GetScaleTextTypeMask( strFormat);
	// 2011.01.27 by SYS <<

	// (2009/6/4 - Seung-Won, Bae) for one time from loop.
	CScaleCompart &scaleCompart = horzScale.GetScaleCompart();
	CStringArray astrTimeUnit;
	astrTimeUnit.SetSize( 12);
	astrTimeUnit.SetAt( 0, scaleCompart.GetDateCompart());
	astrTimeUnit.SetAt( 1, scaleCompart.GetDateCompart());
	astrTimeUnit.SetAt( 2, scaleCompart.GetDateCompart());
	astrTimeUnit.SetAt( 3, scaleCompart.GetTimeCompart());
	astrTimeUnit.SetAt( 4, scaleCompart.GetTimeCompart());
	astrTimeUnit.SetAt( 5, scaleCompart.GetTimeCompart());
	if( 1 < scaleCompart.GetDateCompart().GetLength())
	{
		astrTimeUnit.SetAt( 0, _LTEXT6( C6_YEAR));
		astrTimeUnit.SetAt( 6, _LTEXT6( C6_YEAR));
		astrTimeUnit.SetAt( 1, _LTEXT6( C6_MONTH));
		astrTimeUnit.SetAt( 7, _LTEXT6( C6_MONTH));
		astrTimeUnit.SetAt( 2, _LTEXT6( C6_DAY));
		astrTimeUnit.SetAt( 8, _LTEXT6( C6_DAY));
	}
	if( 1 < scaleCompart.GetTimeCompart().GetLength())
	{
		astrTimeUnit.SetAt( 3,	_LTEXT6( C6_HOUR));
		astrTimeUnit.SetAt( 9,	_LTEXT6( C6_HOUR));
		astrTimeUnit.SetAt( 4,	_LTEXT6( C6_MINUTE));
		astrTimeUnit.SetAt( 10,	_LTEXT6( C6_MINUTE));
		astrTimeUnit.SetAt( 5,	_LTEXT6( C6_SECOND));
		astrTimeUnit.SetAt( 11,	_LTEXT6( C6_SECOND));
	}

	// (2009/10/1 - Seung-Won, Bae) Caclulate TimeDiff of BlockRQ from MainRQ.
	int nTimeDiff = 0;
	if( p_szRQ) if( strMainRQ != p_szRQ)
	{
		CPacketList *pPacketListBlock = pPacketListManager->GetPacketList( p_szRQ);
		if( pPacketListBlock)
			nTimeDiff = pPacketListBlock->GetBaseTimeDifference() - pPacketList->GetBaseTimeDifference();
	}

	CString data;
	int posPt_x = 0;
	CStringArray	saScaleText;
	CUIntArray		saScalePos;
	int nScaleTextType = 0, nPrevScaleTextType = 0;
	int nDRI = -1;
	tm tmDRI, tmPrev;
	memset( &tmDRI, 0, sizeof( tm));
	//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
	tmDRI.tm_isdst = -1;
	tmPrev.tm_isdst = -1;

	pblScaleDRI->SetIndex( 0);
	pblScaleTime->SetIndex( 0);
	CXScaleManager::PAREA parea;
	const tm *pMReturn = NULL;
	int i = 0;
	for( i = 0; i < nDRICount; i++)
	{
		pIReturn = pblScaleDRI->GetNext();
		if( pIReturn) nDRI = *pIReturn;

		tmPrev = tmDRI;
		pMReturn = pblScaleTime->GetNext();
		if( pMReturn) tmDRI = *pMReturn;

		pxScaleManager->GetAreaFromDRI( drawingHorzScaleData.GetColumnIndex(), nDRI, &parea);
		if( !parea) continue;

//		posPt_x = parea->center;
		posPt_x = bReverse ? parea->right : parea->left;

		data = GetScaleText( nScaleTextType, tmDRI, astrTimeUnit, nTimeDiff, nScaleType, eScaleDrawType, tmPrev, nSkipFactor, ( nScaleTextTypeMask | ( 0 < i ? _STT_BOLD_GRID : 0)), pxScaleManager, p_szRQ);

		// 해당된 경우에만 scale그린다.
		// 마지막에서 text를 표현 - ojtaso (20071227)
		// pre text와 겹칠 경우엔 그리지 않는다.
		if( nPrevScaleTextType < nScaleTextType
			|| !bReverse && pre_textEndPos < posPt_x
			|| bReverse && posPt_x < pre_textEndPos)
		{
			nPrevScaleTextType = nScaleTextType;

			saScaleText.Add( data);
			saScalePos.Add( posPt_x);

			CSize textSize = pDC->GetTextExtent(data);
			if(bReverse == FALSE)	pre_textEndPos = posPt_x + textSize.cx + MARGIN;
			else					pre_textEndPos = posPt_x - textSize.cx - MARGIN;
		}

		CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), posPt_x, nScaleTextType & _STT_BOLD_GRID ? 3 : 1);
	}

	if(bReverse == FALSE)	pre_textEndPos = horzScaleRegion.GetBottomScaleRegion().right;
	else					pre_textEndPos = 0;
	for( i = saScaleText.GetSize() - 1; 0 <= i; i--)
	{
		posPt_x = saScalePos.GetAt( i);
		data	= saScaleText.GetAt( i);

		// scale text 적을 공간 비교
		CSize textSize = pDC->GetTextExtent(data);

		// 해당된 경우에만 scale그린다.
		// 마지막에서 text를 표현 - ojtaso (20071227)
		// pre text와 겹칠 경우엔 그리지 않는다.
		if( bReverse && pre_textEndPos <= posPt_x - textSize.cx - MARGIN
			|| !bReverse && posPt_x + textSize.cx + MARGIN <= pre_textEndPos)
		{
			// text구분선 그리기
			CPoint nowTopPos( posPt_x, posTopPt_y);
			CPoint nowBottomPos( posPt_x, posBottomPt_y);

			CHorzScaleDrawer::DrawTextCompartLine(pDC, horzScaleRegion.GetTopScaleRegion(), nowTopPos, true, drawingHorzScaleData.GetCompartLineColor());
			CHorzScaleDrawer::DrawTextCompartLine(pDC, horzScaleRegion.GetBottomScaleRegion(), nowBottomPos, false, drawingHorzScaleData.GetCompartLineColor());

			if(bReverse == FALSE)
			{
				// text 그리기
				// scale text 적을 공간 비교
				CPoint textPosPt(nowTopPos.x +MARGIN, nowTopPos.y);
				if( horzScaleRegion.GetTopScaleRegion().Height() > textSize.cy)
					CDrawer::DrawTextOut(pDC, textPosPt, data, drawingHorzScaleData.GetScaleColor().GetScaleTextColor());
				textPosPt.y = nowBottomPos.y;
				if( horzScaleRegion.GetBottomScaleRegion().Height() > textSize.cy)
					CDrawer::DrawTextOut(pDC, textPosPt, data, drawingHorzScaleData.GetScaleColor().GetScaleTextColor());
			}
			else
			{
				// text 그리기
				CPoint textPosPt(nowTopPos.x - MARGIN, nowTopPos.y);
				// scale text 적을 공간 비교
				textPosPt.x -= textSize.cx;
				if( horzScaleRegion.GetTopScaleRegion().Height() > textSize.cy)
					CDrawer::DrawTextOut(pDC, textPosPt, data, drawingHorzScaleData.GetScaleColor().GetScaleTextColor());
				textPosPt.y = nowBottomPos.y;
				if( horzScaleRegion.GetTopScaleRegion().Height() > textSize.cy)
					CDrawer::DrawTextOut(pDC, textPosPt, data, drawingHorzScaleData.GetScaleColor().GetScaleTextColor());
			}
			pre_textEndPos = posPt_x;
		}
	}
	
	//KHD : 시간구분 그리기  - 전 스케일 데이터를 가져온다. 
	pxScaleManager->GetScaleDRIandTimeAllData( 2, pblScaleDRI, pblScaleTime, eScaleDrawType, p_szRQ, nSkipFactor);
	nDRICount = pblScaleDRI->GetItemCount();
	pblScaleDRI->SetIndex( 0);
	pblScaleTime->SetIndex( 0);

	BOOL bDivGrid_Min_Day = FALSE;
	BOOL bDivGrid_Day_Year = FALSE;
	BOOL bDivGrid_Day_Week = FALSE;
	BOOL bDivGrid_Day_Mon  = FALSE;

	if (eScaleDrawType == CScaleBaseData::HORZ_TIME_ONE || eScaleDrawType == CScaleBaseData::HORZ_TIME_TWO ||
		eScaleDrawType == CScaleBaseData::HORZ_TICK || eScaleDrawType == CScaleBaseData::HORZ_TIME_SECOND)	//분틱차트일때..
	{
		bDivGrid_Min_Day = horzScale.GetCheckTime(CScaleBaseData::MINTICK_DAY_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_DAILY)					//일차트일때..
	{
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_YEAR_GUBUN);
		bDivGrid_Day_Week = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);
		bDivGrid_Day_Mon  = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_MON_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_WEEKLEY)					//주차트일때..
	{
		bDivGrid_Day_Mon  = horzScale.GetCheckTime(CScaleBaseData::WEEKCHART_MON_GUBUN);
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::WEEKCHART_YEAR_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_MONTHLY)					//월차트일때..
	{
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::MONCHART_YEAR_GUBUN);
	}

	for(i = 0; i < nDRICount; i++)
	{
		pIReturn = pblScaleDRI->GetNext();
		if( pIReturn) nDRI = *pIReturn;

		tmPrev = tmDRI;
		pMReturn = pblScaleTime->GetNext();
		if( pMReturn) tmDRI = *pMReturn;

		pxScaleManager->GetAreaFromDRI( drawingHorzScaleData.GetColumnIndex(), nDRI, &parea);
		if( !parea)
			continue;

		// X좌표 조회구간의 시작일자와 종료일자 표기 - onlyojt
		int posPt_x = bReverse ? parea->right : parea->left;

		//KHD :  분/틱차트에서의 일자구분선, 년별 옵션 개별옵션 - 
		if( bDivGrid_Min_Day && i != 0) 
		{
			if(tmPrev.tm_mday != tmDRI.tm_mday)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::MINTICK_DAY_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::MINTICK_DAY_GUBUN), posPt_x, horzScale.GetGridWidth_Time(CScaleBaseData::MINTICK_DAY_GUBUN));
		}

		// 20150603 최성필 0012355: [0520] 일주기 차트에서 '주 구분선' 적용 시, 월요일이 휴장일일 경우, 제대로 나타나지 않는 오류 수정 >>
		// 주 구분선을 매주 월요일날로 지정한 것이 아닌, 이전 요일이(저번 주 금요일) 현재 요일보다 값이 크면, 한 주가 지난 것으로 간주하여 처리
		//if((bDivGrid_Day_Week ) && (tmDRI.tm_wday == 1 )&&i!=0)
		if( (bDivGrid_Day_Week ) && 
			( tmPrev.tm_wday > tmDRI.tm_wday || 
			(tmPrev.tm_wday <= tmDRI.tm_wday && ((tmDRI.tm_yday - tmPrev.tm_yday) >= 7)) )
	      )
		// 20150603 최성필 <<
		{
			CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_WEEK_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN));
		}

		if(bDivGrid_Day_Mon && i!=0)
		{
			if(tmPrev.tm_mon != tmDRI.tm_mon)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_MON_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_MON_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::WEEKCHART_MON_GUBUN));
		}

		if(bDivGrid_Day_Year)
		{	
			if((tmPrev.tm_year != tmDRI.tm_year) && i != 0)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_YEAR_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_YEAR_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::WEEKCHART_YEAR_GUBUN));
		}
	}
	return true;
}

bool CFullTimeScaleDrawer::DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
{
	if( !pxScaleManager) return false;

	// (2009/1/22 - Seung-Won, Bae) Use selected RQ's scale draw type.
	CPacketListManager *pPacketListManager = pxScaleManager->GetPacketListManager();
	if( !pPacketListManager) return false;
	CPacketList *pPacketList = pPacketListManager->GetPacketList( pPacketListManager->GetIChartCtrl()->GetCurrentRQ());
	if(!pPacketList) return false;

	CScaleBaseData::HORZSCALEDRAWERTYPE eScaleDrawType = pPacketList->GetDateUnitType();
	CDataBlockList< int> *pblScaleDRI = NULL;
	CDataBlockList< tm> *pblScaleTime = NULL;
	int nSkipFactor = 1;
	// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
	CString strPacketType = pxScaleManager->GetPacketType( "");
	int nScaleType = pxScaleManager->GetScaleDRIandTime( drawingHorzScaleData.GetColumnIndex(), pblScaleDRI, pblScaleTime, horzScaleRegion.GetBottomScaleRegion().Width(), eScaleDrawType, pPacketList->GetTimeInterval(), p_szRQ, nSkipFactor, strPacketType);
	if( nScaleType < 0) return false;

	int nDRICount = pblScaleDRI->GetItemCount();
	BOOL bReverse = pxScaleManager->GetReverse();
	int nDRI_Start = -1;
	const int *pIReturn = pblScaleDRI->GetAt( 0);
	if( pIReturn) nDRI_Start = *pIReturn;

	int pre_textEndPos = 0;
	if(bReverse == FALSE)	pre_textEndPos = 0;
	else					pre_textEndPos = drawingHorzScaleData.GetDrawingGraphRegion().right;

	// 그리기 위한 기본 data를 가져온다.
	double onePtWidth = 0.0;
	int posPt_y = 0;
	CHorzScaleDrawer::GetDrawingEnvironment( drawingHorzScaleData.GetDrawingGraphRegion(), drawingHorzScaleData.GetDisplayAttributes(), pDC->GetTextExtent("0").cy, onePtWidth, posPt_y);

	// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
	int nScaleTextTypeMask = GetScaleTextTypeMask( strPacketType);

	int nScaleTextType = 0;
	int nDRI = -1;
	tm tmDRI, tmPrev;
	memset( &tmDRI, 0, sizeof( tm));
	//[A00000554]alzioyes:20110530 시간셋팅 초기값변경.
	tmDRI.tm_isdst = -1;
	tmPrev.tm_isdst = -1;

	pblScaleDRI->SetIndex( 0);
	pblScaleTime->SetIndex( 0);
	CXScaleManager::PAREA parea;
	const tm *pMReturn = NULL;
	int i = 0;
	for(  i = 0; i < nDRICount; i++)
	{
		pIReturn = pblScaleDRI->GetNext();
		if( pIReturn) nDRI = *pIReturn;

		tmPrev = tmDRI;
		pMReturn = pblScaleTime->GetNext();
		if( pMReturn) tmDRI = *pMReturn;

		pxScaleManager->GetAreaFromDRI( drawingHorzScaleData.GetColumnIndex(), nDRI, &parea);
		if( !parea) continue;

//		int posPt_x = parea->center;
		int	posPt_x = bReverse ? parea->right : parea->left;

		nScaleTextType = GetScaleTextType( nScaleType, eScaleDrawType, tmPrev, tmDRI, nSkipFactor);

		// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
		// (2009/5/28 - Seung-Won, Bae) Do not bold on first grid.
		nScaleTextType = nScaleTextType & ( nScaleTextTypeMask | ( 0 < i ? _STT_BOLD_GRID : 0));

		CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetGridType(), posPt_x, nScaleTextType & _STT_BOLD_GRID ? 3 : 1);
	}

	//KHD : 시간구분 그리기  - 전 스케일 데이터를 가져온다. 
	pxScaleManager->GetScaleDRIandTimeAllData( 2, pblScaleDRI, pblScaleTime, eScaleDrawType, p_szRQ, nSkipFactor);
	nDRICount = pblScaleDRI->GetItemCount();
	pblScaleDRI->SetIndex( 0);
	pblScaleTime->SetIndex( 0);

	BOOL bDivGrid_Min_Day = FALSE;
	BOOL bDivGrid_Day_Year = FALSE;
	BOOL bDivGrid_Day_Week = FALSE;
	BOOL bDivGrid_Day_Mon  = FALSE;

	if (eScaleDrawType == CScaleBaseData::HORZ_TIME_ONE || eScaleDrawType == CScaleBaseData::HORZ_TIME_TWO ||
		eScaleDrawType == CScaleBaseData::HORZ_TICK || eScaleDrawType == CScaleBaseData::HORZ_TIME_SECOND)	//분틱차트일때..
	{
		bDivGrid_Min_Day = horzScale.GetCheckTime(CScaleBaseData::MINTICK_DAY_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_DAILY)					//일차트일때..
	{
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_YEAR_GUBUN);
		bDivGrid_Day_Week = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);
		bDivGrid_Day_Mon  = horzScale.GetCheckTime(CScaleBaseData::DAYCHART_MON_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_WEEKLEY)					//주차트일때..
	{
		bDivGrid_Day_Mon  = horzScale.GetCheckTime(CScaleBaseData::WEEKCHART_MON_GUBUN);
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::WEEKCHART_YEAR_GUBUN);
	}
	else if(eScaleDrawType == CScaleBaseData::HORZ_MONTHLY)					//월차트일때..
	{
		bDivGrid_Day_Year = horzScale.GetCheckTime(CScaleBaseData::MONCHART_YEAR_GUBUN);
	}

	for(i = 0; i < nDRICount; i++)
	{
		pIReturn = pblScaleDRI->GetNext();
		if( pIReturn) nDRI = *pIReturn;

		tmPrev = tmDRI;
		pMReturn = pblScaleTime->GetNext();
		if( pMReturn) tmDRI = *pMReturn;

		pxScaleManager->GetAreaFromDRI( drawingHorzScaleData.GetColumnIndex(), nDRI, &parea);
		if( !parea)
			continue;

		// X좌표 조회구간의 시작일자와 종료일자 표기 - onlyojt
		int posPt_x = bReverse ? parea->right : parea->left;

		//KHD :  분/틱차트에서의 일자구분선, 년별 옵션 개별옵션 - 
		if( bDivGrid_Min_Day && i != 0) 
		{
			if(tmPrev.tm_mday != tmDRI.tm_mday)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::MINTICK_DAY_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::MINTICK_DAY_GUBUN), posPt_x, horzScale.GetGridWidth_Time(CScaleBaseData::MINTICK_DAY_GUBUN));
		}

		// 20150603 최성필 0012355: [0520] 일주기 차트에서 '주 구분선' 적용 시, 월요일이 휴장일일 경우, 제대로 나타나지 않는 오류 수정 >>
		// 주 구분선을 매주 월요일날로 지정한 것이 아닌, 이전 요일이(저번 주 금요일) 현재 요일보다 값이 크면, 한 주가 지난 것으로 간주하여 처리
		//if((bDivGrid_Day_Week ) && (tmDRI.tm_wday == 1 )&&i!=0)
		if( (bDivGrid_Day_Week ) && 
			( tmPrev.tm_wday > tmDRI.tm_wday || 
			(tmPrev.tm_wday <= tmDRI.tm_wday && ((tmDRI.tm_yday - tmPrev.tm_yday) >= 7)) )
	      )
		// 20150603 최성필 <<
		{
			CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_WEEK_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN));
		}

		if(bDivGrid_Day_Mon && i!=0)
		{
			if(tmPrev.tm_mon != tmDRI.tm_mon)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_MON_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_MON_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::WEEKCHART_MON_GUBUN));
		}

		if(bDivGrid_Day_Year)
		{	
			if((tmPrev.tm_year != tmDRI.tm_year) && i != 0)
				CHorzScaleDrawer::DrawGrid(pDC, drawingHorzScaleData, horzScale.GetPeriodGridType(CScaleBaseData::DAYCHART_YEAR_GUBUN), horzScale.GetGridColor_Time(CScaleBaseData::DAYCHART_YEAR_GUBUN), posPt_x,horzScale.GetGridWidth_Time(CScaleBaseData::WEEKCHART_YEAR_GUBUN));
		}
	}

	return true;
}

// (2009/4/30 - Seung-Won, Bae) Check packet type for Auto Scale Type.
int CFullTimeScaleDrawer::GetScaleTextTypeMask( CString p_strPacketType)
{
	int nScaleTextTypeMask = 0;
	if( 0 <= p_strPacketType.Find( 'Y'))		nScaleTextTypeMask |= _STT_YEAR;
	if( 0 <= p_strPacketType.Find( "YM") || 0 <= p_strPacketType.Find( "MD"))
												nScaleTextTypeMask |= _STT_MONTH;
	if( 0 <= p_strPacketType.Find( 'D'))		nScaleTextTypeMask |= _STT_DAY;
	if( 0 <= p_strPacketType.Find( 'H'))		nScaleTextTypeMask |= _STT_HOUR;
	if( 0 <= p_strPacketType.Find( "HM") || 0 <= p_strPacketType.Find( "MS"))
												nScaleTextTypeMask |= _STT_MINUTE;
	if( 0 <= p_strPacketType.Find( 'S'))		nScaleTextTypeMask |= _STT_SECOND;
	return nScaleTextTypeMask;
}
