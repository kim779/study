// ScaleBaseData.cpp: implementation of the CScaleBaseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleBaseData.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MHWND()
#include "../Include_Chart/DataMath.h"					// for CMath
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleCompart  -> scale 구분 type 저장

CScaleCompart& CScaleCompart::operator=(const CScaleCompart& data)
{
	if(this == &data)
		return *this;

	m_strDateCompart = data.GetDateCompart();
	m_strTimeCompart = data.GetTimeCompart();
	m_eTextType = data.GetScaleTextType();
	return *this;
}

CScaleCompart::CScaleCompart():
	m_strDateCompart("."),
	m_strTimeCompart(":"),
	m_eTextType(CScaleBaseData::HORZ_TEXT_COMPART)
{
}

CScaleCompart::CScaleCompart(const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextType):
	m_strDateCompart(dateCompart),
	m_strTimeCompart(timeCompart),
	m_eTextType(eTextType)
{
}

CString CScaleCompart::GetDateCompart() const
{
	return m_strDateCompart;
}

CString CScaleCompart::GetTimeCompart() const
{
	return m_strTimeCompart;
}

CScaleBaseData::TEXTCOMPARTTYPE CScaleCompart::GetScaleTextType() const
{
	return m_eTextType;
}

void CScaleCompart::SetCompart( char *p_szCompart)
{
	if( !p_szCompart) return;
	if( !*p_szCompart) return;

	char *szTimeCompart = strchr( p_szCompart, ';');
	if( szTimeCompart) *szTimeCompart = '\0';
	m_strDateCompart = p_szCompart;
	if( szTimeCompart) *szTimeCompart++ = ';';
	else return;

	char *szTextType = strchr( szTimeCompart, ';');
	if( szTextType) *szTextType = '\0';
	m_strTimeCompart = szTimeCompart;
	if( szTextType) *szTextType++ = ';';
	else return;

	if( !*szTextType) return;

	char *szEnd = strchr( szTextType, ';');
	if( szEnd) *szEnd = '\0';
	m_eTextType = ( CScaleBaseData::TEXTCOMPARTTYPE)atoi( szTextType);;
	if( szEnd) *szEnd++ = ';';
	else return;
}

void CScaleCompart::SetDateCompart(const CString& dateCompart)
{
	m_strDateCompart = dateCompart;
}

void CScaleCompart::SetTimeCompart(const CString& timeCompart)
{
	m_strTimeCompart = timeCompart;
}

void CScaleCompart::SetScaleTextType(const CScaleBaseData::TEXTCOMPARTTYPE eTextType)
{
	m_eTextType = eTextType;
}


//////////////////////////////////////////////////////////////////////
// class CScale

CScale& CScale::operator=(const CScale& data)
{
	if(this == &data)
		return *this;

	m_strScaleData = data.GetScaleData();
	m_eScaleGrid = data.GetGridType();
	return *this;
}

CScale::CScale():
	m_eScaleGrid(CScaleBaseData::SLNONE)
{
}

void CScale::SetData(const CString& scaleData)
{
	m_strScaleData = scaleData;
}

void CScale::SetScaleData( char *p_szScaleData, HWND p_hOcxWnd)
{
	m_strScaleData = p_szScaleData;
}

void CScale::SetGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType)
{
	m_eScaleGrid = eGridType;
}

CString CScale::GetScaleData() const
{
	return m_strScaleData;
}

CScaleBaseData::SCALEGRIDTYPE CScale::GetGridType() const
{
	return m_eScaleGrid;
}

// protected ===============================================================================
void CScale::SetGridType( char *p_szGridType)
{
	if( !p_szGridType) return;
	if( !*p_szGridType) return;

	m_eScaleGrid = ( CScaleBaseData::SCALEGRIDTYPE)atoi( p_szGridType);
}


////////////////////////////////////////////////////////////////////////////////////////////
// class CHorizontalScale

CHorizontalScale& CHorizontalScale::operator=(const CHorizontalScale& data)
{
	if(this == &data)
		return *this;

	CScale::operator = ((CScale) data);
	m_scaleCompart = data.GetScaleCompart();
	m_ePosition = data.GetHorzScalePosition();
	// XScale 높이 지정 - ojtaso (20080429)
	m_nScaleHeight = data.GetScaleHeight();

	// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
	m_nGridTypeDay		= data.GetPeriodGridType(CScaleBaseData::MINTICK_DAY_GUBUN);
	m_nGridTypeWeek		= data.GetPeriodGridType(CScaleBaseData::DAYCHART_WEEK_GUBUN);
	m_nGridTypeMon		= data.GetPeriodGridType(CScaleBaseData::DAYCHART_MON_GUBUN);
	m_nGridTypeYear		= data.GetPeriodGridType(CScaleBaseData::DAYCHART_YEAR_GUBUN);

	//차트 시간구분 
	m_bOn_MINTICK_DAY   = data.GetCheckTime(CScaleBaseData::MINTICK_DAY_GUBUN);
	m_bOn_DAYCHART_WEEK = data.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);
	m_bOn_DAYCHART_MON  = data.GetCheckTime(CScaleBaseData::DAYCHART_MON_GUBUN);
	m_bOn_DAYCHART_YEAR = data.GetCheckTime(CScaleBaseData::DAYCHART_YEAR_GUBUN);
	m_bOn_WEEKCHART_MON = data.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);;
	m_bOn_WEEKCHART_YEAR = data.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);;
	m_bOn_MONCHART_YEAR = data.GetCheckTime(CScaleBaseData::DAYCHART_WEEK_GUBUN);;

	m_nGridWidth_Day	= data.GetGridWidth_Time(CScaleBaseData::MINTICK_DAY_GUBUN);
	m_nGridWidth_Week	= data.GetGridWidth_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN);
	m_nGridWidth_Mon	= data.GetGridWidth_Time(CScaleBaseData::DAYCHART_MON_GUBUN);
	m_nGridWidth_Year	= data.GetGridWidth_Time(CScaleBaseData::DAYCHART_YEAR_GUBUN);

	m_clrDailyDivGridColor	= data.GetGridWidth_Time(CScaleBaseData::MINTICK_DAY_GUBUN);
	m_clrWeekDivGridColor	= data.GetGridWidth_Time(CScaleBaseData::DAYCHART_WEEK_GUBUN);
	m_clrMonDivGridColor	= data.GetGridWidth_Time(CScaleBaseData::DAYCHART_MON_GUBUN);
	m_clrYearDivGridColor	= data.GetGridWidth_Time(CScaleBaseData::DAYCHART_YEAR_GUBUN);

	return *this;
}

CHorizontalScale::CHorizontalScale():
	CScale(),
	m_nScaleHeight(-1),	// XScale 높이 지정 - ojtaso (20080429)
	m_ePosition(CScaleBaseData::HORZ_NONE),
	m_nGridTypeDay(CScaleBaseData::SLNONE),	// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
	m_nGridTypeWeek(CScaleBaseData::SLNONE),
	m_nGridTypeMon(CScaleBaseData::SLNONE),
	m_nGridTypeYear (CScaleBaseData::SLNONE),
	m_bOn_MINTICK_DAY(TRUE),
	m_bOn_DAYCHART_WEEK(FALSE),
	m_bOn_DAYCHART_MON(FALSE),
	m_bOn_DAYCHART_YEAR(FALSE),
	m_bOn_WEEKCHART_MON(FALSE),
	m_bOn_WEEKCHART_YEAR(FALSE),
	m_bOn_MONCHART_YEAR(FALSE),
	m_nGridWidth_Day(1),
	m_nGridWidth_Week(1),
	m_nGridWidth_Mon(1),
	m_nGridWidth_Year(1),
	m_clrDailyDivGridColor(_DPV_CLRDAILYDIVGRIDCOLOR),
	m_clrWeekDivGridColor(_DPV_CLRWEEKDIVGRIDCOLOR),
	m_clrMonDivGridColor(_DPV_CLRMONDIVGRIDCOLOR),
	m_clrYearDivGridColor(_DPV_CLRYEARDIVGRIDCOLOR)

{
}

// -----------------------------------------------------------------------------------------
void CHorizontalScale::SetScaleData( char *p_szScaleData, HWND p_hOcxWnd)
{
	if( !p_szScaleData) return;
	if( !*p_szScaleData) return;

	// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;
	char *szPosition = strchr( p_szScaleData, ';');
	if( szPosition) *szPosition = '\0';
	_MHWND( p_hOcxWnd);
	CScale::SetScaleData( ( char *)_STOM0( p_szScaleData), p_hOcxWnd);
	if( szPosition) *szPosition++ = ';';
	else return;

	char *szGridType = strchr( szPosition, ';');
	if( szGridType) *szGridType = '\0';
	SetHorzScalePosition( szPosition);
	if( szGridType) *szGridType++ = ';';
	else return;

	char *szCompart = strchr( szGridType, ';');
	if( szCompart) *szCompart = '\0';

	{
		char szType[2] = {NULL, };
		::memcpy(szType, szGridType, 1);
		CScale::SetGridType( szType);
		//KHD:  그리드 속성 세팅 :4자리  분/틱차트에서의 일자구분선 개별옵션 - onlyojt
		int nlen = ::lstrlen(szGridType);
		if(nlen <=4)//초기화 
		{
			m_nGridTypeDay = CScaleBaseData::SHORTDOTTEDLINE;
			m_nGridTypeWeek = CScaleBaseData::SHORTDOTTEDLINE;
			m_nGridTypeMon = CScaleBaseData::SHORTDOTTEDLINE;
			m_nGridTypeYear = CScaleBaseData::SHORTDOTTEDLINE;
		}
		else
		{
			::memcpy( szType, ++szGridType, 1 );
			SetPeriodGridType( CScaleBaseData::MINTICK_DAY_GUBUN, szType );

			::memcpy( szType, ++szGridType, 1 );
			SetPeriodGridType( CScaleBaseData::DAYCHART_WEEK_GUBUN, szType );

			::memcpy( szType, ++szGridType, 1 );
			SetPeriodGridType( CScaleBaseData::DAYCHART_MON_GUBUN, szType );

			::memcpy( szType, ++szGridType, 1 );
			SetPeriodGridType( CScaleBaseData::DAYCHART_YEAR_GUBUN, szType );
			//Che유무 세팅 : 7자리

			if( ::lstrlen( szGridType ) >= 7 )
			{
				for( int i = 0; i < 7; i++ )
				{
					::memcpy( szType, ++szGridType, 1 );
					SetCheckTime( (CScaleBaseData::SCALEGRIDTIMEGUBUN)i, atoi( szType ) );
				}
			}
			//시간별 Width : 4자리 
			int Width = 1;
			if( ::lstrlen( szGridType ) >= 4 )
			{
				::memcpy( szType, ++szGridType, 1 );	Width = atoi( szType );
				if( Width == 0 )						Width = 1;
				SetGridWidth_Time( CScaleBaseData::MINTICK_DAY_GUBUN, Width );

				::memcpy( szType, ++szGridType, 1 );	Width = atoi( szType );
				if( Width == 0 )						Width = 1;
				SetGridWidth_Time( CScaleBaseData::DAYCHART_WEEK_GUBUN, Width );

				::memcpy( szType, ++szGridType, 1 );	Width = atoi( szType );
				if( Width == 0 )						Width = 1;
				SetGridWidth_Time( CScaleBaseData::DAYCHART_MON_GUBUN, Width );

				::memcpy( szType, ++szGridType, 1 );	Width = atoi( szType );
				if( Width == 0 )						Width = 1;
				SetGridWidth_Time( CScaleBaseData::DAYCHART_YEAR_GUBUN, Width );
			}
			int iStart2 = 0;
			CString strColorInfo;
			strColorInfo.Format(_T("%s"), szGridType);
			SetGridColor_Time( CScaleBaseData::MINTICK_DAY_GUBUN, (COLORREF)(COLORREF) atol(TokenizeEx(strColorInfo, ",", iStart2)) );
			SetGridColor_Time( CScaleBaseData::DAYCHART_WEEK_GUBUN, (COLORREF)(COLORREF) atol(TokenizeEx(strColorInfo, ",", iStart2)) );
			SetGridColor_Time( CScaleBaseData::DAYCHART_MON_GUBUN, (COLORREF)(COLORREF) atol(TokenizeEx(strColorInfo, ",", iStart2)) );
			SetGridColor_Time( CScaleBaseData::DAYCHART_YEAR_GUBUN, (COLORREF)(COLORREF) atol(TokenizeEx(strColorInfo, ",", iStart2)) );

			//Scale Width 
/*			if( ::lstrlen( szGridType ) >= 2 )// KHD 
			{
				::memcpy( szType, ++szGridType, 1 );
				SetGridWidth( atoi( szType ) );//가로 
				::memcpy( szType, ++szGridType, 1 );
				SetGridVert_Width( atoi( szType ) );//세로
			}
*/
			//END
		}
	}

	if( szCompart) *szCompart++ = ';';
	else return;

	m_scaleCompart.SetCompart( szCompart);
}

CString CHorizontalScale::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	if( iStart < 0) return "";

	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if( iStart == -1) return "";

	iStart++;
	return strSrc.Mid(iCur, iStart - iCur - 1);
}

void CHorizontalScale::SetHorzScalePosition( char *p_szScalePosition)
{
	if( !p_szScalePosition) return;
	if( !*p_szScalePosition) return;

	m_ePosition = ( CScaleBaseData::HORZSCALEPOSITION)atoi( p_szScalePosition);
}

void CHorizontalScale::SetHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	m_ePosition = ePosition;
}

void CHorizontalScale::SetScaleCompart(const CScaleCompart& scaleCompart)
{
	m_scaleCompart = scaleCompart;
}

// XScale 높이 지정 - ojtaso (20080429)
void CHorizontalScale::SetScaleHeight(const int nHeight)
{
	m_nScaleHeight = nHeight;
}

// -----------------------------------------------------------------------------------------
CScaleCompart CHorizontalScale::GetScaleCompart() const
{
	return m_scaleCompart;
}

CScaleCompart& CHorizontalScale::GetScaleCompart()
{
	return m_scaleCompart;
}

CScaleBaseData::HORZSCALEPOSITION CHorizontalScale::GetHorzScalePosition() const
{
	return m_ePosition;
}

// XScale 높이 지정 - ojtaso (20080429)
int CHorizontalScale::GetScaleHeight() const
{
	return m_nScaleHeight;
}

// scale의 모든 data
void CHorizontalScale::GetScaleAllData( CString &p_strHorzScaleData) const
{
	// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;log;차트뒤집기;

	CString strScaleData = CScale::GetScaleData();

	char szPosition[ 10];
	itoa( m_ePosition, szPosition, 10);
	int nPosition = strlen( szPosition);

	char szGridType[ 10];
	itoa( CScale::GetGridType(), szGridType, 10);
	int nGridType = strlen( szGridType);

	// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
	char szDAY_GUBUN[ 10], szWEEK_GUBUN[ 10],szMON_GUBUN[ 10],szYEAR_GUBUN[ 10];
	char szDayCheck[10],  szDayWeekCheck[10], szDayMonCheck[10], szDayYearCheck[10];
	char szWeekMonCheck[10],  szWeekYearCheck[10],  szMonYearCheck[10];
	char szDayWidth[10],szWeekWidth[10],szMonWidth[10],szYearWidth[10];
	CString strDayColor, strWeekColor, strMonColor, strYearColor;
	//KHD : 스케일 가로/ 스케일 가로 저장 
//	char szGaro_Width[10], szSero_Width[10];

	// 시간구분선 종류 저장
	itoa(m_nGridTypeDay , szDAY_GUBUN, 10);
	itoa(m_nGridTypeWeek , szWEEK_GUBUN, 10);
	itoa(m_nGridTypeMon, szMON_GUBUN, 10);
	itoa(m_nGridTypeYear, szYEAR_GUBUN, 10);

	// 시간구분선 사용여부 저장 
	itoa(m_bOn_MINTICK_DAY , szDayCheck, 10);
	itoa(m_bOn_DAYCHART_WEEK , szDayWeekCheck, 10);
	itoa(m_bOn_DAYCHART_MON , szDayMonCheck, 10);
	itoa(m_bOn_DAYCHART_YEAR , szDayYearCheck, 10);
	itoa(m_bOn_WEEKCHART_MON , szWeekMonCheck, 10);
	itoa(m_bOn_WEEKCHART_YEAR , szWeekYearCheck, 10);
	itoa(m_bOn_MONCHART_YEAR , szMonYearCheck, 10);

	// 시간구분선 굵기 저장 
	itoa(m_nGridWidth_Day , szDayWidth, 10);
	itoa(m_nGridWidth_Week , szWeekWidth, 10);
	itoa(m_nGridWidth_Mon , szMonWidth, 10);
	itoa(m_nGridWidth_Year , szYearWidth, 10);

	strDayColor.Format(_T("%ld"), m_clrDailyDivGridColor);
	strWeekColor.Format(_T("%ld"), m_clrWeekDivGridColor);
	strMonColor.Format(_T("%ld"), m_clrMonDivGridColor);
	strYearColor.Format(_T("%ld"), m_clrYearDivGridColor);

	//Scale 가로/세로
//	itoa(m_nGrid_Horz_Width , szGaro_Width, 10);//가로 
//	itoa(m_nGrid_Vert_Width , szSero_Width, 10);//세로
	//END

	CString strDateCompart = m_scaleCompart.GetDateCompart();
	CString strTimeCompart = m_scaleCompart.GetTimeCompart();

	char szScaleTextType[ 10];
	itoa( m_scaleCompart.GetScaleTextType(), szScaleTextType, 10);
	int nScaleTextType = strlen( szScaleTextType);

	int nHorzScaleDataLen = 0;
	nHorzScaleDataLen += strScaleData.GetLength();
	nHorzScaleDataLen++;								// ';'
	nHorzScaleDataLen += nPosition;
	nHorzScaleDataLen++;								// ';'
	nHorzScaleDataLen += nGridType;
	{
		// 분/틱차트에서의 일자구분선 개별옵션
		nHorzScaleDataLen += strlen( szDAY_GUBUN);			// 시간구분선 종류 저장
		nHorzScaleDataLen += strlen( szWEEK_GUBUN);
		nHorzScaleDataLen += strlen( szMON_GUBUN);
		nHorzScaleDataLen += strlen( szYEAR_GUBUN);
		nHorzScaleDataLen += strlen( szDayCheck);			// 시간구분선 사용여부 저장
		nHorzScaleDataLen += strlen( szDayWeekCheck);
		nHorzScaleDataLen += strlen( szDayMonCheck);
		nHorzScaleDataLen += strlen( szDayYearCheck);
		nHorzScaleDataLen += strlen( szWeekMonCheck);
		nHorzScaleDataLen += strlen( szWeekYearCheck);
		nHorzScaleDataLen += strlen( szMonYearCheck);
		nHorzScaleDataLen += strlen( szDayWidth);			// 시간구분선 굵기 저장
		nHorzScaleDataLen += strlen( szWeekWidth);
		nHorzScaleDataLen += strlen( szMonWidth);
		nHorzScaleDataLen += strlen( szYearWidth);
		nHorzScaleDataLen += strDayColor.GetLength();		nHorzScaleDataLen++;		// ','
		nHorzScaleDataLen += strWeekColor.GetLength();		nHorzScaleDataLen++;		// ','
		nHorzScaleDataLen += strMonColor.GetLength();		nHorzScaleDataLen++;		// ','
		nHorzScaleDataLen += strYearColor.GetLength();		nHorzScaleDataLen++;		// ','
	//	nHorzScaleDataLen += nGrid_Horz_Width;	// 가로/세로
	//	nHorzScaleDataLen += nGrid_Vert_Width;
		// 
	}
	nHorzScaleDataLen++;								// ';'
	nHorzScaleDataLen += strDateCompart.GetLength();
	nHorzScaleDataLen++;								// ';'
	nHorzScaleDataLen += strTimeCompart.GetLength();
	nHorzScaleDataLen++;								// ';'
	nHorzScaleDataLen += nScaleTextType;
	nHorzScaleDataLen++;								// ';'

	char *szHorzScaleData = p_strHorzScaleData.GetBufferSetLength( nHorzScaleDataLen);

	strcpy( szHorzScaleData, strScaleData);
	szHorzScaleData += strScaleData.GetLength();
	*szHorzScaleData++ = ';';

	strcpy( szHorzScaleData, szPosition);
	szHorzScaleData += nPosition;
	*szHorzScaleData++ = ';';

	strcpy( szHorzScaleData, szGridType);
	szHorzScaleData += nGridType;
	{
		//KHD : 분/틱차트에서의 일자구분선 개별옵션 -: 그리드타입4자리 
		strcpy( szHorzScaleData, szDAY_GUBUN);			szHorzScaleData += strlen( szDAY_GUBUN);
		strcpy( szHorzScaleData, szWEEK_GUBUN);			szHorzScaleData += strlen( szWEEK_GUBUN);
		strcpy( szHorzScaleData, szMON_GUBUN);			szHorzScaleData += strlen( szMON_GUBUN);
		strcpy( szHorzScaleData, szYEAR_GUBUN);			szHorzScaleData += strlen( szYEAR_GUBUN);

		//시간체크  :7자리
		strcpy( szHorzScaleData, szDayCheck);			szHorzScaleData += strlen( szDayCheck) ;
		strcpy( szHorzScaleData, szDayWeekCheck);		szHorzScaleData += strlen( szDayWeekCheck);
		strcpy( szHorzScaleData, szDayMonCheck);		szHorzScaleData += strlen( szDayMonCheck);
		strcpy( szHorzScaleData, szDayYearCheck);		szHorzScaleData += strlen( szDayYearCheck);
		strcpy( szHorzScaleData, szWeekMonCheck);		szHorzScaleData += strlen( szWeekMonCheck);
		strcpy( szHorzScaleData, szWeekYearCheck);		szHorzScaleData += strlen( szWeekYearCheck);
		strcpy( szHorzScaleData, szMonYearCheck);		szHorzScaleData += strlen( szMonYearCheck);

		//그리드 Width : 4자리 
		strcpy( szHorzScaleData, szDayWidth);			szHorzScaleData += strlen( szDayWidth);
		strcpy( szHorzScaleData, szWeekWidth);			szHorzScaleData += strlen( szWeekWidth);
		strcpy( szHorzScaleData, szMonWidth);			szHorzScaleData += strlen( szMonWidth);
		strcpy( szHorzScaleData, szYearWidth);			szHorzScaleData += strlen( szYearWidth);

		//그리드 Width : 4자리 
		strcpy( szHorzScaleData, strDayColor);			szHorzScaleData += strDayColor.GetLength();		*szHorzScaleData++ = ',';
		strcpy( szHorzScaleData, strWeekColor);			szHorzScaleData += strWeekColor.GetLength();	*szHorzScaleData++ = ',';
		strcpy( szHorzScaleData, strMonColor);			szHorzScaleData += strMonColor.GetLength();		*szHorzScaleData++ = ',';
		strcpy( szHorzScaleData, strYearColor);			szHorzScaleData += strYearColor.GetLength();	*szHorzScaleData++ = ',';

		//Scale Widht : 2자리 
//		strcpy( szHorzScaleData, szGaro_Width);			szHorzScaleData += nGrid_Horz_Width ;//가로 
//		strcpy( szHorzScaleData, szSero_Width);			szHorzScaleData += nGrid_Vert_Width ;//세로
	}
	*szHorzScaleData++ = ';';

	strcpy( szHorzScaleData, strDateCompart);
	szHorzScaleData += strDateCompart.GetLength();
	*szHorzScaleData++ = ';';

	strcpy( szHorzScaleData, strTimeCompart);
	szHorzScaleData += strTimeCompart.GetLength();
	*szHorzScaleData++ = ';';

	strcpy( szHorzScaleData, szScaleTextType);
	szHorzScaleData += nScaleTextType;
	*szHorzScaleData++ = ';';
}


CScaleBaseData::SCALEGRIDTYPE CHorizontalScale::GetPeriodGridType(CScaleBaseData::SCALEGRIDTIMEGUBUN nType) const
{
	CScaleBaseData::SCALEGRIDTYPE eType;
	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		eType = m_nGridTypeDay;		break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	eType = m_nGridTypeWeek;	break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	eType = m_nGridTypeMon;		break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	eType = m_nGridTypeYear;	break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	eType = m_nGridTypeMon;		break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	eType = m_nGridTypeYear;	break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	eType = m_nGridTypeYear;	break;
	}

	return eType;
}

void CHorizontalScale::SetPeriodGridType(int nType, char *p_szGridType)
{
	if( !p_szGridType) return;
	if( !*p_szGridType) return;

	CScaleBaseData::SCALEGRIDTYPE eGridType = ( CScaleBaseData::SCALEGRIDTYPE)atoi( p_szGridType);

	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		m_nGridTypeDay = eGridType;			break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	m_nGridTypeWeek = eGridType;		break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	m_nGridTypeMon = eGridType;			break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	m_nGridTypeYear = eGridType;		break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	m_nGridTypeMon = eGridType;			break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	m_nGridTypeYear = eGridType;		break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	m_nGridTypeYear = eGridType;		break;
	}
}

void CHorizontalScale::SetPeriodGridType(const CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const CScaleBaseData::SCALEGRIDTYPE eGridType, BOOL bCheck)
{
	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		m_nGridTypeDay = eGridType;		m_bOn_MINTICK_DAY = bCheck;		break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	m_nGridTypeWeek = eGridType;	m_bOn_DAYCHART_WEEK = bCheck;	break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	m_nGridTypeMon = eGridType;		m_bOn_DAYCHART_MON = bCheck;	break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	m_nGridTypeYear = eGridType;	m_bOn_DAYCHART_YEAR = bCheck;	break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	m_nGridTypeMon = eGridType;		m_bOn_WEEKCHART_MON = bCheck;	break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	m_nGridTypeYear = eGridType;	m_bOn_WEEKCHART_YEAR = bCheck;	break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	m_nGridTypeYear = eGridType;	m_bOn_MONCHART_YEAR = bCheck;	break;
	}
}

BOOL CHorizontalScale::GetCheckTime(CScaleBaseData::SCALEGRIDTIMEGUBUN nType) const
{
	BOOL bType;

	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		bType = m_bOn_MINTICK_DAY;		break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	bType = m_bOn_DAYCHART_WEEK;	break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	bType = m_bOn_DAYCHART_MON;		break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	bType = m_bOn_DAYCHART_YEAR;	break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	bType = m_bOn_WEEKCHART_MON;	break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	bType = m_bOn_WEEKCHART_YEAR;	break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	bType = m_bOn_MONCHART_YEAR;	break;
	}

	return bType;
}

void CHorizontalScale::SetCheckTime(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, BOOL bCheck)
{
	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		m_bOn_MINTICK_DAY  = bCheck;	break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	m_bOn_DAYCHART_WEEK  = bCheck;	break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	m_bOn_DAYCHART_MON  = bCheck;	break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	m_bOn_DAYCHART_YEAR  = bCheck;	break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	m_bOn_WEEKCHART_MON  = bCheck;	break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	m_bOn_WEEKCHART_YEAR  = bCheck;	break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	m_bOn_MONCHART_YEAR  = bCheck;	break;
	}
}

unsigned short CHorizontalScale::GetGridWidth_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType,BOOL bBold/* = FALSE*/) const
{
	unsigned short nGridWidth = 1;

	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		nGridWidth = m_nGridWidth_Day;		break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	nGridWidth = m_nGridWidth_Week;		break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	nGridWidth = m_nGridWidth_Mon;		break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	nGridWidth = m_nGridWidth_Year;		break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	nGridWidth = m_nGridWidth_Mon;		break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	nGridWidth = m_nGridWidth_Year;		break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	nGridWidth = m_nGridWidth_Year;		break;
	}

	return nGridWidth;
}

void CHorizontalScale::SetGridWidth_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, unsigned short nGridWidth)
{
	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		m_nGridWidth_Day = nGridWidth;	break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	m_nGridWidth_Week = nGridWidth;	break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	m_nGridWidth_Mon = nGridWidth;	break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	m_nGridWidth_Year = nGridWidth;	break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	m_nGridWidth_Mon = nGridWidth;	break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	m_nGridWidth_Year = nGridWidth;	break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	m_nGridWidth_Year = nGridWidth;	break;
	}
}

unsigned long CHorizontalScale::GetGridColor_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType) const
{
	unsigned long nGridColor = 1;

	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		nGridColor = m_clrDailyDivGridColor;	break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	nGridColor = m_clrWeekDivGridColor;		break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	nGridColor = m_clrMonDivGridColor;		break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	nGridColor = m_clrYearDivGridColor;		break;
		case CScaleBaseData::WEEKCHART_MON_GUBUN:	nGridColor = m_clrMonDivGridColor;		break;
		case CScaleBaseData::WEEKCHART_YEAR_GUBUN:	nGridColor = m_clrYearDivGridColor;		break;
		case CScaleBaseData::MONCHART_YEAR_GUBUN:	nGridColor = m_clrYearDivGridColor;		break;
	}

	return nGridColor;
}

void CHorizontalScale::SetGridColor_Time(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, COLORREF nGridColor)
{
	switch (nType)
	{
		case CScaleBaseData::MINTICK_DAY_GUBUN:		m_clrDailyDivGridColor = nGridColor;	break;
		case CScaleBaseData::DAYCHART_WEEK_GUBUN:	m_clrWeekDivGridColor = nGridColor;		break;
		case CScaleBaseData::DAYCHART_MON_GUBUN:	m_clrMonDivGridColor = nGridColor;		break;
		case CScaleBaseData::DAYCHART_YEAR_GUBUN:	m_clrYearDivGridColor = nGridColor;		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// class CVerticalScale

CVerticalScale& CVerticalScale::operator=(const CVerticalScale& data)
{
	if(this == &data)
		return *this;

	CScale::operator = ((CScale) data);
	m_ePosition = data.GetVertScalePosition();
	m_eUnitType = data.GetVertScaleUnitType();

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	m_bMinMaxLock	= data.IsVertScaleMinMaxLock();		// Lock
	m_eMinMaxType	= data.GetVertScaleMinMaxType();	// Type
	m_dMin			= data.GetVertScaleMin();			// Min
	m_dMax			= data.GetVertScaleMax();			// Max

	// (2007/1/1 - Seung-Won, Bae) Manage Left/Right Vertical Scale and Grid Line Activate Flag
	m_nVScaleActiveFlag = data.GetActiveFlag();

	//2007.01.22 Scale사용자 설정,호가단위별
	m_nUserHogaType	= data.GetVertUserHogaScaleType();	// Type
	m_dUserNumber	= data.GetVertScaleUserNumber();			// Min
	m_dHogaNumber	= data.GetVertScaleHoGaNumber();			// Max

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	data.GetSpace( m_nUpSpace, m_nDownSpace);

	// 지표 뒤집기 - ojtaso (20071023)
	m_bIsVertScaleInvertType = data.m_bIsVertScaleInvertType;

	return *this;
}

CVerticalScale::CVerticalScale():
	CScale(),
	m_ePosition(CScaleBaseData::VERT_NONE),
	m_eUnitType(CScaleBaseData::VERT_NUM)
{
	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	m_bMinMaxLock	= FALSE;								// Lock
	m_eMinMaxType	= CScaleBaseData::VERT_MINMAX_OF_VIEW;	// Type
	m_dMin			= 0.0;									// Min
	m_dMax			= 0.0;									// Max

	// (2007/1/1 - Seung-Won, Bae) Manage Left/Right Vertical Scale and Grid Line Activate Flag
	m_nVScaleActiveFlag = 0;

	//2007.01.22 Scale사용자 설정,호가단위별
	m_nUserHogaType	= 0;	// Type
	m_dUserNumber	= 0.0;			// Min
	m_dHogaNumber	= 0.0;			// Max

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	m_nUpSpace		= 0;
	m_nDownSpace	= 0;

	m_bIsVertScaleInvertType = false;		// 지표 뒤집기 - ojtaso (20071023)
}

// -----------------------------------------------------------------------------------------
void CVerticalScale::SetScaleData( char *p_szScaleData, HWND p_hOcxWnd)
{
	// 사용할 Data;위치;grid타입;unit타입;
	if( !p_szScaleData) return;
	if( !*p_szScaleData) return;

	char *szPosition = strchr( p_szScaleData, ';');
	if( szPosition) *szPosition = '\0';
	_MHWND( p_hOcxWnd);
	CString strScaleData = _MTOM6_N( p_szScaleData);
	if( strScaleData.IsEmpty()) strScaleData = _STOM0( p_szScaleData);
	CScale::SetScaleData( ( char *)( const char *)strScaleData, p_hOcxWnd);
	if( szPosition) *szPosition++ = ';';
	else return;

	char *szGridType = strchr( szPosition, ';');
	if( szGridType) *szGridType = '\0';
	SetVertScalePosition( szPosition);
	if( szGridType) *szGridType++ = ';';
	else return;

	char *szUnitType = strchr( szGridType, ';');
	if( szUnitType) *szUnitType = '\0';
	CScale::SetGridType( szGridType);
	if( szUnitType) *szUnitType++ = ';';
	else return;

	char *szMinMaxLock = strchr( szUnitType, ';');
	if( szMinMaxLock) *szMinMaxLock = '\0';
	SetVertScaleUnitType(szUnitType);
	if( szMinMaxLock) *szMinMaxLock++ = ';';
	else return;

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	if( !*szMinMaxLock) return;

	char *szMinMaxType = strchr( szMinMaxLock, ';');
	if( szMinMaxType) *szMinMaxType = '\0';
	m_bMinMaxLock	= ( atoi( szMinMaxLock) != 0);									// Lock
	if( szMinMaxType) *szMinMaxType++ = ';';
	else return;

	char *szMin = strchr( szMinMaxType, ';');
	if( szMin) *szMin = '\0';
	m_eMinMaxType	= ( CScaleBaseData::VERTMINMAXTYPE)atoi( szMinMaxType);			// Type
	if( szMin) *szMin++ = ';';
	else return;

	char *szMax = strchr( szMin, ';');
	if( szMax) *szMax = '\0';
	if( strstr( szMin, "INF"))	m_dMin = -HUGE_VAL;									// Min
	else						m_dMin = atof( szMin);
	if( szMax) *szMax++ = ';';
	else return;

	char *szUpSpace = strchr( szMax, ';');
	if( szUpSpace) *szUpSpace = '\0';
	// (2007/2/21 - Seung-Won, Bae) Check Infinite
	if( strstr( szMax, "INF"))	m_dMax	= -HUGE_VAL;								// Max
	else						m_dMax	= atof( szMax);
	if( szUpSpace) *szUpSpace++ = ';';
	else return;

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	if( !*szUpSpace) return;

	char *szDownSpace = strchr( szUpSpace, ';');
	if( szDownSpace) *szDownSpace = '\0';
	m_nUpSpace = atoi( szUpSpace);													// Up Space
	if( szDownSpace) *szDownSpace++ = ';';
	else return;

	char *szUserHogaType = strchr( szDownSpace, ';');
	if( szUserHogaType) *szUserHogaType = '\0';
	m_nDownSpace = atoi( szDownSpace);												// Down Space
	if( szUserHogaType) *szUserHogaType++ = ';';
	else return;

//2007.01.22 Scale사용자 설정,호가단위별
	if( !*szUserHogaType) return;

	char *szUserNumber = strchr( szUserHogaType, ';');
	if( szUserNumber) *szUserNumber = '\0';
	m_nUserHogaType	= atoi( szUserHogaType);										// Type
	if( szUserNumber) *szUserNumber++ = ';';
	else return;
	
	char *szHogaNumber = strchr( szUserNumber, ';');
	if( szHogaNumber) *szHogaNumber = '\0';
	m_dUserNumber	= atoi( szUserNumber);											// User
	if( szHogaNumber) *szHogaNumber++ = ';';
	else return;

	// 지표 뒤집기 - ojtaso (20071023)
	char *szInvertType = strchr( szHogaNumber, ';');
	if( szInvertType) *szInvertType = '\0';
	m_dHogaNumber	= atoi( szHogaNumber);											// Hoga
	if( szInvertType) *szInvertType++ = ';';
	
	// 지표 뒤집기 - ojtaso (20071023)
	char *szEnd = strchr( szInvertType, ';');
	if( szEnd) *szEnd = '\0';
	m_bIsVertScaleInvertType = atoi( szInvertType) ? true : false;					// Invert
	if( szEnd) *szEnd++ = ';';

	else return;
}

// 값 setting
void CVerticalScale::SetVertScalePosition( char *p_szScalePosition)
{
	if( !p_szScalePosition) return;
	if( !*p_szScalePosition) return;
	m_ePosition = ( CScaleBaseData::VERTSCALEPOSITION)atoi( p_szScalePosition);
}

void CVerticalScale::SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	m_ePosition = ePosition;
}

void CVerticalScale::SetVertScaleUnitType( char *p_szScaleUnitType)
{
	if( !p_szScaleUnitType) return;
	if( !*p_szScaleUnitType) return;

	m_eUnitType = ( CScaleBaseData::VERTSCALEUNITTYPE)atoi( p_szScaleUnitType);
}

void CVerticalScale::SetVertScaleUnitType(const CScaleBaseData::VERTSCALEUNITTYPE eUnitType)
{
	m_eUnitType = eUnitType;
}

// -----------------------------------------------------------------------------------------
CScaleBaseData::VERTSCALEPOSITION CVerticalScale::GetVertScalePosition() const
{
	return m_ePosition;
}

CScaleBaseData::VERTSCALEUNITTYPE CVerticalScale::GetVertScaleUnitType() const
{
	return m_eUnitType;
}

// scale의 모든 data
void CVerticalScale::GetScaleAllData( CString &p_strVertScaleData, const char *p_szScaleData) const
{
	CString strScaleData = p_szScaleData ? p_szScaleData : CScale::GetScaleData();

	char szPosition[ 10];
	itoa( m_ePosition, szPosition, 10);
	int nPosition = strlen( szPosition);

	char szGridType[ 10];
	itoa( CScale::GetGridType(), szGridType, 10);
	int nGridType = strlen( szGridType);

	char szUnitType[ 10];
	itoa( m_eUnitType, szUnitType, 10);
	int nUnitType = strlen( szUnitType);

	char szMinMaxType[ 100];
	itoa( m_eMinMaxType, szMinMaxType, 10);
	int nMinMaxType = strlen( szMinMaxType);

	char szMin[ 100];
	int nMin = sprintf( szMin, "%f", m_dMin);

	char szMax[ 100];
	int nMax = sprintf( szMax, "%f", m_dMax);

	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	char szUpSpace[ 20];
	int nUpSpace = sprintf( szUpSpace, "%d", m_nUpSpace);
	char szDownSpace[ 20];
	int nDownSpace = sprintf( szDownSpace, "%d", m_nDownSpace);

//2007.01.22 Scale사용자 설정,호가단위별
	char szUserHogaType[ 10];
	itoa( m_nUserHogaType, szUserHogaType, 10);
	int nUserHogaType = strlen( szUserHogaType);

	char szUserNumber[ 20];
	int nUserNumber = sprintf( szUserNumber, "%f", m_dUserNumber);

	char szHogaNumber[ 20];
	int nHogaNumber = sprintf( szHogaNumber, "%f", m_dHogaNumber);

	// 지표 뒤집기 - ojtaso (20071023)
	char szInvertType[ 20];
	int nInverType = sprintf( szInvertType, "%d", m_bIsVertScaleInvertType ? TRUE : FALSE);

//2007.01.22 Scale사용자 설정,호가단위별

	int nVertScaleData = 0;
	nVertScaleData += strScaleData.GetLength();
	nVertScaleData++;								// ';'
	nVertScaleData += nPosition;
	nVertScaleData++;								// ';'
	nVertScaleData += nGridType;
	nVertScaleData++;								// ';'
	nVertScaleData += nUnitType;
	nVertScaleData++;								// ';'
	nVertScaleData += 2;							// '?;'
	nVertScaleData += nMinMaxType;
	nVertScaleData++;								// ';'
	nVertScaleData += nMin;
	nVertScaleData++;								// ';'
	nVertScaleData += nMax;
	nVertScaleData++;								// ';'
	nVertScaleData += nUpSpace;
	nVertScaleData++;								// ';'
	nVertScaleData += nDownSpace;
	nVertScaleData++;								// ';'
//2007.01.22 Scale사용자 설정,호가단위별
	nVertScaleData += nUserHogaType;
	nVertScaleData++;								// ';'
	nVertScaleData += nUserNumber;
	nVertScaleData++;								// ';'
	nVertScaleData += nHogaNumber;
	nVertScaleData++;								// ';'
	// 지표 뒤집기 - ojtaso (20071023)
	nVertScaleData += nInverType;
	nVertScaleData++;								// ';'
// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	nVertScaleData += 2;							// "\r\n"

	char *szVertScaleData = p_strVertScaleData.GetBufferSetLength( nVertScaleData);

	strcpy( szVertScaleData, strScaleData);
	szVertScaleData += strScaleData.GetLength();
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szPosition);
	szVertScaleData += nPosition;
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szGridType);
	szVertScaleData += nGridType;
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szUnitType);
	szVertScaleData += nUnitType;
	*szVertScaleData++ = ';';
	
	*szVertScaleData++ = ( m_bMinMaxLock ? '1' : '0');
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szMinMaxType);
	szVertScaleData += nMinMaxType;
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szMin);
	szVertScaleData += nMin;
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szMax);
	szVertScaleData += nMax;
	*szVertScaleData++ = ';';
	
	// (2007/7/16 - Seung-Won, Bae) Up/Down Space
	strcpy( szVertScaleData, szUpSpace);
	szVertScaleData += nUpSpace;
	*szVertScaleData++ = ';';
	strcpy( szVertScaleData, szDownSpace);
	szVertScaleData += nDownSpace;
	*szVertScaleData++ = ';';

//2007.01.22 Scale사용자 설정,호가단위별
	strcpy( szVertScaleData, szUserHogaType);
	szVertScaleData += nUserHogaType;
	*szVertScaleData++ = ';';
	
	strcpy( szVertScaleData, szUserNumber);
	szVertScaleData += nUserNumber;
	*szVertScaleData++ = ';';

	strcpy( szVertScaleData, szHogaNumber);
	szVertScaleData += nHogaNumber;
	*szVertScaleData++ = ';';
//2007.01.22 Scale사용자 설정,호가단위별

	// 지표 뒤집기 - ojtaso (20071023)
	strcpy( szVertScaleData, szInvertType);
	szVertScaleData += nInverType;
	*szVertScaleData++ = ';';

	*szVertScaleData++ = '\r';
	*szVertScaleData++ = '\n';
}

// (2007/1/1 - Seung-Won, Bae) Manage Left/Right Vertical Scale and Grid Line Activate Flag
int CVerticalScale::GetActiveFlag( void) const
{
	return m_nVScaleActiveFlag;
}
void CVerticalScale::AddActiveFlag( int p_nActiveFlag)
{
	m_nVScaleActiveFlag |= p_nActiveFlag;
}
void CVerticalScale::RemoveActiveFlag( int p_nActiveFlag)
{
	m_nVScaleActiveFlag &= ~p_nActiveFlag;
	if( m_nVScaleActiveFlag == VSGA_GRIDLINE) m_nVScaleActiveFlag = 0;
}
void CVerticalScale::ResetActiveFlag( int p_nActiveFlag)
{
	m_nVScaleActiveFlag = p_nActiveFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////
// class CHorzScaleRegionHeight  -> 가로 scale 높이 저장

CHorzScaleRegionHeight::CHorzScaleRegionHeight():
	m_nTopHeight(0),
	m_nBottomHeight(0)
{
}

CHorzScaleRegionHeight::CHorzScaleRegionHeight(const int topHeight, const int bottomHeight):
	m_nTopHeight(topHeight),
	m_nBottomHeight(bottomHeight)
{
}

// public =================================================================================
void CHorzScaleRegionHeight::SetTopHorzScaleRegionHeight(const int topHeight)
{
	m_nTopHeight = topHeight;
}

void CHorzScaleRegionHeight::SetBottomHorzScaleRegionHeight(const int bottomHeight)
{
	m_nBottomHeight = bottomHeight;
}

int CHorzScaleRegionHeight::GetTopHorzScaleRegionHeight() const
{
	return m_nTopHeight;
}

int CHorzScaleRegionHeight::GetBottomHorzScaleRegionHeight() const
{
	return m_nBottomHeight;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CVertScaleRegionWidth  -> 세로 scale 폭 저장

CVertScaleRegionWidth::CVertScaleRegionWidth():
	m_nLeftWidth(0),
	m_nRightWidth(0)
{
}

CVertScaleRegionWidth::CVertScaleRegionWidth(const int leftWidth, const int rightWidth):
	m_nLeftWidth(leftWidth),
	m_nRightWidth(rightWidth)
{
}

// public =================================================================================
void CVertScaleRegionWidth::SetLeftVertScaleRegionWidth(const int leftWidth)
{
	m_nLeftWidth = leftWidth;
}

void CVertScaleRegionWidth::SetRightVertScaleRegionWidth(const int rightWidth)
{
	m_nRightWidth = rightWidth;
}

int CVertScaleRegionWidth::GetLeftVertScaleRegionWidth() const
{
	return m_nLeftWidth;
}

int CVertScaleRegionWidth::GetRightVertScaleRegionWidth() const
{
	return m_nRightWidth;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CHorzScaleRegion  -> 가로 scale 영역 저장

CHorzScaleRegion::CHorzScaleRegion():
	m_topScaleRegion(0, 0, 0, 0),
	m_bottomScaleRegion(0, 0, 0, 0)
{
}

// public =================================================================================
void CHorzScaleRegion::SetTopScaleRegion(const CRect& region)
{
	m_topScaleRegion = region;
}

void CHorzScaleRegion::SetBottomScaleRegion(const CRect& region)
{
	m_bottomScaleRegion = region;
}

CRect CHorzScaleRegion::GetTopScaleRegion() const
{
	return m_topScaleRegion;
}

CRect CHorzScaleRegion::GetBottomScaleRegion() const
{
	return m_bottomScaleRegion;
}

bool CHorzScaleRegion::PointInRegion(const CPoint& point) const
{
	return (m_topScaleRegion.PtInRect(point) || m_bottomScaleRegion.PtInRect(point));
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CVertScaleRegion  -> 세로 scale 영역 저장

CVertScaleRegion::CVertScaleRegion():
	m_leftScaleRegion(0, 0, 0, 0),
	m_rightScaleRegion(0, 0, 0, 0)
{
}

// public =================================================================================
void CVertScaleRegion::SetLeftScaleRegion(const CRect& region)
{
	m_leftScaleRegion = region;
}

void CVertScaleRegion::SetRightScaleRegion(const CRect& region)
{
	m_rightScaleRegion = region;
}

void CVertScaleRegion::OffsetRegion(const int nTop, const int nBottom)
{
	m_leftScaleRegion.top += nTop;
	m_leftScaleRegion.bottom += nBottom;
	m_rightScaleRegion.top += nTop;
	m_rightScaleRegion.bottom += nBottom;
}

// ----------------------------------------------------------------------------------------
CRect CVertScaleRegion::GetLeftScaleRegion() const
{
	return m_leftScaleRegion;
}

CRect CVertScaleRegion::GetRightScaleRegion() const
{
	return m_rightScaleRegion;
}

CRect CVertScaleRegion::GetLeftScaleTextRegion() const
{
	if(m_leftScaleRegion.Width() <= 0)
		return CRect(m_leftScaleRegion.left, m_leftScaleRegion.top,
			m_leftScaleRegion.right, m_leftScaleRegion.bottom);

	return CRect(m_leftScaleRegion.left +MARGIN, m_leftScaleRegion.top,
		m_leftScaleRegion.right -MARGIN*2, m_leftScaleRegion.bottom);
}

CRect CVertScaleRegion::GetLeftScaleGridRegion() const
{
	if(m_leftScaleRegion.Width() <= 0)
		return CRect(m_leftScaleRegion.right, m_leftScaleRegion.top,
			m_leftScaleRegion.right, m_leftScaleRegion.bottom);

	return CRect(m_leftScaleRegion.right -MARGIN, m_leftScaleRegion.top,
		m_leftScaleRegion.right, m_leftScaleRegion.bottom);
}

CRect CVertScaleRegion::GetRightScaleTextRegion() const
{
	if(m_rightScaleRegion.Width() <= 0)
		return CRect(m_rightScaleRegion.left, m_rightScaleRegion.top,
			m_rightScaleRegion.right, m_rightScaleRegion.bottom);

	return CRect(m_rightScaleRegion.left +MARGIN*2, m_rightScaleRegion.top,
		m_rightScaleRegion.right -MARGIN, m_rightScaleRegion.bottom);
}

CRect CVertScaleRegion::GetRightScaleGridRegion() const
{
	if(m_rightScaleRegion.Width() <= 0)
		return CRect(m_rightScaleRegion.left, m_rightScaleRegion.top,
			m_rightScaleRegion.left, m_rightScaleRegion.bottom);

	return CRect(m_rightScaleRegion.left, m_rightScaleRegion.top,
		m_rightScaleRegion.left +MARGIN, m_rightScaleRegion.bottom);
}

bool CVertScaleRegion::PointInRegion(const CPoint& point) const
{
	return (m_rightScaleRegion.PtInRect(point) || m_leftScaleRegion.PtInRect(point));
}

///////////////////////////////////////////////////////////////////////////////
// class CDisplayDataMinMax

CDisplayDataMinMax& CDisplayDataMinMax::operator=(const CDisplayDataMinMax& displayDataMinMax)
{
	if(this == &displayDataMinMax)
		return *this;

	m_dMin = displayDataMinMax.GetDisplayMin();
	m_dMax = displayDataMinMax.GetDisplayMax();
	return *this;
}

CDisplayDataMinMax::CDisplayDataMinMax():
	m_dMin( -HUGE_VAL),
	m_dMax( -HUGE_VAL)
{
}

CDisplayDataMinMax::CDisplayDataMinMax(const double& dMin, const double& dMax):
	m_dMin(dMin),
	m_dMax(dMax)
{
}

void CDisplayDataMinMax::SetDisplayData(const double& dMin, const double& dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
}

void CDisplayDataMinMax::Inflate(const CDisplayDataMinMax& displayDataMinMax)
{
	if(displayDataMinMax.GetDisplayMin() < m_dMin) m_dMin = displayDataMinMax.GetDisplayMin();
	if(displayDataMinMax.GetDisplayMax() > m_dMax) m_dMax = displayDataMinMax.GetDisplayMax();
}

void CDisplayDataMinMax::Inflate(const double& dMinGap, const double& dMaxGap)
{
	m_dMin -= dMinGap;
	m_dMax += dMaxGap;
}

// ----------------------------------------------------------------------------
double CDisplayDataMinMax::GetDisplayMin() const
{
	return m_dMin;
}

double CDisplayDataMinMax::GetDisplayMax() const
{
	return m_dMax;
}

double CDisplayDataMinMax::GetCount() const
{
	// (2007/10/9 - Seung-Won, Bae) Check Not Initialized Scale.
	if( m_dMax == -HUGE_VAL || m_dMin == -HUGE_VAL) return 0;

	// 두 차가 0.xxx 일 경우에 그 차가 정확하지 않다.
	// -> 소숫점 부분의 제일 마지막 자릿수(10으로 함)에서 반올림한다.
	double dDifference = m_dMax - m_dMin;
	if( dDifference > 0 && dDifference < 1)
	{
		dDifference = CMath::Round(dDifference, 10);
	}
	return dDifference;
}

double CDisplayDataMinMax::GetMarginDisplayMin() const
{
	return (m_dMin - (GetCount() * 0.06));
}

double CDisplayDataMinMax::GetMarginDisplayMax() const
{
	return (m_dMax + (GetCount() * 0.06));
}

// 현재값이 최대값에 가까운지의 여부
bool CDisplayDataMinMax::IsCloseToMaxValue(const double& dCur, const bool bIsInvert) const
{
	// "뒤집기"인 경우 
	if(bIsInvert)
		return (fabs(m_dMax - dCur) <= fabs(m_dMin - dCur));
	
	return (fabs(m_dMax - dCur) >= fabs(m_dMin - dCur));
}

// (2006/11/19 - Seung-Won, Bae) Support Initializing State.
BOOL CDisplayDataMinMax::IsInitialized( void) const
{
	return m_dMax != -HUGE_VAL && m_dMin != HUGE_VAL;
}
