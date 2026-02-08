// Conversion.h: interface for the CConversion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERSION_H__4EDC5FDA_0443_4CE1_888C_8E01B8820C70__INCLUDED_)
#define AFX_CONVERSION_H__4EDC5FDA_0443_4CE1_888C_8E01B8820C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataConversion
{
public:	
	// color --> string
	static CString RGB2Str(const COLORREF clrValue);
	// string --> color
	static COLORREF Str2RGB(const CString& strColor);
	// string -> bool
	static bool atob(const char* string);
	// int -> bool
	static bool atob(const int nData);
	// bool --> string
	static CString Bool2Str(const BOOL bValue) ;
	// float --> double
	static double atod(const float x);
	// int -> CString
	static CString IntToString(const int Num);
	static CString IntToString(const int Num, const int length);
	// double -> CString
	static CString DoubleToString(const double& Num);
	static CString DoubleToString(const double& Num, const int decimalDownCount);
	// 숫자형 문자열로 변환한다.
	static CString NormalNumberString(const CString& strData);

	static int CalcTimeFormat(const int nValue, const int nCharge, const CString& strFormat, const bool bIsTransactionTime);
	static int CalcTimeFormatforCharge(const int nValue, const int nIndex, const CString& strFormat, const int nInterval, const bool bIsTransactionTime);
	static int CalcDateFormat(const int nValue, const int nCharge, const CString& strFormat, const int nCalcType, const int nInterval);
	static bool GetSpecialTime(const CString& strFormat, const CString& strData, CString& strSpecialTime);

	// string을 원하는 string로 trim.
	static CString GetStringData(CString& data, const CString& trimData, const bool bIsInside = true);
	static CString GetStringData(CString& data, const CString& trimStart, const CString& trimEnd, const bool bIsInside);
	// trimStr가 nIndex 번째까지의 문자까지를 자른다.
	static CString GetStringData(CString& data, const CString& trimStr, const int nIndex);
	// trimStr가 nIndex 번째까지의 string.
	static CString GetLeftToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex = 0, const bool bIsInside = true);
	static CString GetMidToTrimDataStartIndex(const CString& data, const CString& trimStr, const int nStartIndex, const bool bIsInside_start, const bool bIsInside_end);
	static CString GetMidToTrimDataEndIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside_start, const bool bIsInside_end);
	static CString GetMidToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex = 0, const bool bIsInside = true);
	static CString GetMidToTrimDataIndex(const CString& data, const CString& trimStart, const CString& trimEnd, const int nStartIndex = 0, const bool bIsInside = true);
	// string에 원하는 string의 갯수
	static int GetStringDataCount(const CString& orgStr, const CString& comparisonStr);
	// 해당 index를 찾는다.
	static int FindIndex(const CString& orgData, const CString& findData, const int nFindDataCountIndex = 0);
	// 소수점 이하 몇 자릿수
	static int GetDecimalDownCount(const CString& strDecimalType, const bool bIsCheckDigit = true);
	// data type을 double 형태로 변환
	static double GetDataType(CString strDataType);
	// "x10000" -> "x만" 형태로 변환 (10000 이상만 한글로 바꾼다.)
	static CString GetChangeFormat_NumCharUnit(const CString& strOrgFormat);
	// 원하는 포맷으로 스트링 변경
	static CString GetStringFormatTrans(const CString& strData, const CString& strFormat/*, const bool bIsRound = true*/);
	// 원하는 포맷으로 스트링 변경 (천단위마다 , 를 표시)
	static CString GetStringThousandFormat(const double& dData, const double& dFormat);
	static CString GetStringThousandFormat(const CString& strData);
	// 원하는 포캣으로 스트링 변경 (날짜, 시간 표시)
	static CString GetStringFormatTrans(const CString& strData, const CString& strFormat, const CString& strCompartDate, const CString& strCompartTime = "");
	static CString GetStringFormatbyCompart(const CString& strData, CString& strFormat, const CString& strCompartDate, const CString& strCompartTime, CString& strResult);
	static CString GetCompartType(const CString& strFormat, const CString& strCompartDate, const CString& strCompartTime);	
	static int GetStringFormatTransCutoff(const double& dData, const CString& strFormat, CString& strData, CString& strFormatTrans);
	static int GetStringFormatTransCutoff(const double& dData, const double& dFormat, CString& strData, CString& strFormatTrans);
	// 원하는 문자를 원하는 갯수만큼 특정 스트링에 삽입 또는 추가
	static CString InsertString(const CString& strData, const CString& strInsert, const int nInsertLength, const int nInsertType);

	// 그릴 영역 정한다.
	static void SelectClipRegion(CDC* pDC, const CRect& rect);
	// 그릴 영역을 푼다.
	static void SelectClipRegion(CDC* pDC);

	// 값에 의해 화면의 point로 변환된 값을 가져온다.
	static CPoint GetVerticalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter = true);
	static CPoint GetVerticalPoint(const CRect& blockRegion, const double& yMin, const double& yMax, const double& xMin, const double& xMax, const double& nowYData, const double& nowXData);
	static CPoint GetVerticalPointBox(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter = true);
	static CPoint GetHorizontalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min = 1.0, const double& max = 1.0, const double& nowData = 1.0);
	static int GetHorizontalPoint_X(const CRect& blockRegion, const double& min, const double& max, const double& nowData);
	// 절대좌표 -> 상대좌표
	static int AbsoluteToRelative_X(const CRect& region, const int absolute_x, const int pageDataCount);
	static int AbsoluteToRelative_X(const CRect& region, const int absolute_x, const double& dataViewMin, const double& dataViewMax);
	static double AbsoluteToRelative_Y(const CRect& region, const int absolute_y, const double& dataViewMin, const double& dataViewMax);
	// 상대좌표 -> 절대좌표. (특정 값을 화면 Y축 값으로 바꿔줌)
	static int RelativeToAbsolute_Y(const CRect& region, const double& relative_y, const double& dataViewMin, const double& dataViewMax);
	static CPoint RelativeToAbsolute(const CRect& region, const int startIndex, const int endIndex, 
			const double& dataViewMin, const double& dataViewMax, 
			const int currentIndex, const double& currentPrice);
	static int RelativeToAbsolute_X(const CRect& region, const int startIndex, const int endIndex, const int currentIndex);

	// 해당 point가 있는 data의 index --> 자료일자인 경우
	static int GetPacketDataIndex_Date(const CRect& drawingRegion, const CPoint& point, const int dataStart, const int dataEnd);
	// 날짜 하나의 x간격
	static double GetRealWidth(const double& dWidth, const int nIndex);

	//invert graph을 위해서 dataPt를 invert 한다.
	static int InvertGraphDataPt(const CRect& region, const int dataPt_y);


private:
	static bool IsCalcTimeFormatforCharge(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval);
	static CString GetAdditionDataInFormat(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval);
	static bool GetAllTime(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay, int& nHour, int& nMin, int& nSec);
	static bool GetAllTime_TimePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nDay, int& nHour, int& nMin, int& nSec);
	static bool GetAllTime_DatePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay);
	static bool GetAllTime_Second(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nSec, int& nMin, bool& bIsIntervalEnd);
	static bool GetAllTime_Minute(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nMin, int& nHour, bool& bIsIntervalEnd);
	static bool GetAllTime_Hour(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nHour, int& nDay, bool& bIsIntervalEnd);
	static bool GetAllTime(const int nAddIndex, const int nInterval, const int nMaxGap, const int nMinGap, 
		const int nLimitData1, int& nData1, int& nData2, bool& bIsIntervalEnd);
	static void GetDataInLimit(const int nLimitData1, int& nData1, int& nData2);
	static int CalcTimeFormat(const CString& strBaseValue, const CString& strAddValue, 
		const CString& strFormat, const bool bIsFuture, const bool bIsTransactionTime);
	static int CalcTimeFormat_Future(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime);
	static int CalcTimeFormat_Past(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime);
	static bool GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		int& nBaseDay, int& nBaseHour, int& nBaseMin, int& nBaseSec, 
		int& nAddDay, int& nAddHour, int& nAddMin, int& nAddSec);
	static bool GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		const CString& strFindType, const int nFindStart, int& nBaseData, int& nAddData);
	static int GetTimeInFormat(const CString& strFormat, const int nDay, const int nHour, const int nMin, const int nSec);
	static void GetHourTime_Future(const int nBaseHour, const int nAddHour, 
		const bool bIsTransactionTime, int& nDay, int& nHour);
	// pluse 인지를 체크한다.
	static bool IsPluse(const CString& data);

	// GetVerticalPoint(), GetHonrizontalPoint() 의 내부함수들
	static int GetChangingPosition(const int startPos, const int length, const double& originStartData, const double& originLength, const double& nowData);
	static int GetChangingPosition(const int nowIndex, const int startPos, const double& width, const bool bCenter);

	// 상대좌표 -> 절대좌표 : 상대좌표가 해당 범위에 있을 경우
	static CPoint RelativeToAbsoluteInDisplayIndex(const CRect& region, const double& realWidth, 
			const double& dataViewMin, const double& dataViewMax, const int currentIndex, const double& currentPrice);
	// 상대좌표 -> 절대좌표 : 상대좌표가 해당 범위에 밖에 있을 경우
	static CPoint RelativeToAbsoluteOutDisplayIndex(const CRect& region, 
			const double& realWidth, const double& realHeight, const double& dataViewMin, const double& dataViewMax, 
			const int startIndex, const int endIndex, const int currentIndex, const double& currentPrice);
	static int RelativeToAbsoluteInDisplayIndex_X(const CRect& region, const double& realWidth, const int currentIndex);
	static int RelativeToAbsoluteOutDisplayIndex_X(const CRect& region, 
			const double& realWidth, const int startIndex, const int endIndex, const int currentIndex);
	static int RelativeToAbsoluteInDisplayIndex_Y(const CRect& region, 
			const double& dataViewMin, const double& dataViewMax, const double& currentPrice);
	static int RelativeToAbsoluteOutDisplayIndex_Y(const CRect& region, 
			const double& realHeight, const double& dataViewMin, const double& dataViewMax, const double& currentPrice);

	static bool OnPickoutTime(const int nCurData, const int nCharge, const CString& strFormat, const int nInterval, int& mon, int& day, int& hour, int& min, int& sec);
	static bool OnPickoutDate(const int nValue, const CString& strFormat, const int nType, int& year, int& mon, int& day);	
	static bool OnPickoutDaily(const int nValue, const CString& strFormat, int& year, int& mon, int& day);
	static bool OnPickoutWeekley(const int nValue, const CString& strFormat, int& year, int& mon, int& day);
	static bool OnPickoutMonthly(const int nValue, const CString& strFormat, int& year, int& mon, int& day);

	static int GetRealDailyInTimeFormat(const int nDD, const CString& strFormat);
	static bool GetRealDailyInTimeFormat(const int nOrgData, int& nYear, int& nMonth, int& nDay);
	static void ChagneUprightData_YearAndMonth(int& nYear, int& nMonth);
	static void ChagneUprightData_Day(int& nYear, int& nMonth, int& nDay);
	static int GetMonthFullDay(const int nYear, const int nMonth, const int nDay);
	static bool IsCalUprightData(const int nYear, const int nMonth, const int nDay);
};

#endif // !defined(AFX_CONVERSION_H__4EDC5FDA_0443_4CE1_888C_8E01B8820C70__INCLUDED_)
