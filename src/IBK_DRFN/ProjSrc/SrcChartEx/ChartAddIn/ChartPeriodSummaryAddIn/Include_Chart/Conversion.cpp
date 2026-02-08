// Conversion.cpp: implementation of the CDataConversion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Conversion.h"

#include <assert.h>
#include <FLOAT.H>

#include "Dll_Load/IMetaTable.h"		// for _LTEXT6()
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDataConversion

// public ===============================================================================
// conversion (string & 수 & color) -------------------------------------------------------------

//RGBColor -> String
CString CDataConversion::RGB2Str(const COLORREF clrValue)
{
	int nR = GetRValue(clrValue) ;
	int nG = GetGValue(clrValue) ;
	int nB = GetBValue(clrValue) ;

	CString strValue ;
	strValue.Format("%d,%d,%d", nR, nG, nB) ;
	return strValue ;
}

COLORREF CDataConversion::Str2RGB(const CString& strColor)
{
	CString strClr = strColor ;
	strClr.TrimLeft() ;
	strClr.TrimRight() ;

	COLORREF clrResult = 0;
	if( strClr.IsEmpty())	return clrResult;

	// (2007/1/10 - Seung-Won, Bae) Support No Comma.
	char cSeperator = ',';
	if( strClr.Find( ',') < 0) cSeperator = '.';

	CString strColorData;
	int nColorDataLen = strClr.GetLength();
	char *szColorData = strColorData.GetBufferSetLength( nColorDataLen);
	strcpy( szColorData, strClr);

	char *szItem = szColorData;
	szColorData = strchr( szColorData, cSeperator);
	if( szColorData) *szColorData++ = '\0';
	clrResult += atoi( szItem);
	if( !szColorData) return clrResult;
	if( !*szColorData) return clrResult;

	szItem = szColorData;
	szColorData = strchr( szColorData, cSeperator);
	if( szColorData) *szColorData++ = '\0';
	clrResult += atoi( szItem) * 256;
	if( !szColorData) return clrResult;
	if( !*szColorData) return clrResult;

	szItem = szColorData;
	clrResult += atoi( szItem) * 65536;
	return clrResult;
};

// string -> bool
bool CDataConversion::atob(const char* string)
{
	if(atoi(string) == 1)
		return true;
	return false;
}

bool CDataConversion::atob(const int nData)
{
	return (nData == 0 ? false: true);
}

//	BOOL --> string
CString CDataConversion::Bool2Str(const BOOL bValue)
{
	int nValue = 0 ;
	if(bValue == FALSE)
		nValue = 0;
	else
		nValue = 1;

	return IntToString(nValue) ;
}

// float --> double
double CDataConversion::atod(const float x)
{
	char ch[10];
	sprintf(ch, "%f", x);
	return atof(ch);
}

// int -> CString
CString CDataConversion::IntToString(const int Num)
{
	CString strData;
	strData.Format("%d", Num);
	return strData;
}

// Num : 1234  length : -1 ==> "12340"
// Num : 1234  length : 0 ==> "1234"
// Num : 1234  length : 3 ==> "123"
// Num : 1234  length : 6 ==> "001234"
// 2011.01.25 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//CString CDataConversion::IntToString(const int Num, const int length)
CString CDataConversion::IntToString(const __int64 Num, const int length)
{
	CString strData;
	strData.Format( "%I64d", Num);
// 2011.01.25 by SYS <<

	if(length > 0){
		if(strData.GetLength() > length){
			return strData.Left(length);
		}
		else if(strData.GetLength() < length){
			CString temp;
			for(int i = 0; i < length - strData.GetLength(); i++)
				temp += "0";
			return temp + strData;
		}
	}
	else if(length < 0){
		CString temp;
		for(int i = 0; i < length; i++)
			temp += "0";
		return strData + temp;
	}

	return strData;
}

// double -> CString
CString CDataConversion::DoubleToString(const double& Num)
{
	//assert(Num != DBL_MAX);
	//assert(Num != -DBL_MAX);

	CString strData;
	strData.Format("%f", Num);
	strData.TrimRight("0");
	if(strData.Right(1) == '.') strData.TrimRight(".");

	return strData;
}

/*CString CDataConversion::DoubleToString(const double& Num)
{ 
	// Num = 340.000 인 경우를 생각해서 정수부분과 실수부분을 나누어서 계산한다.

	int nNum = (int)floor(Num);
	if(nNum < 0.0)
		nNum = (int)floor(Num + 0.5);
	double dNum = Num - (double)nNum;

	CString temp, str;
	temp.Format("%f", dNum); // 소수부분
	if(temp.Left(1) == '1')
		nNum += 1;
	str.Format("%d", nNum);  // 정수부분

	int count = 0;
	// Num의 소수부분인(dNum) 0.xxx 부분 비교는 0. 을 제외하고 한다.
	for(int i = 2; i < temp.GetLength(); i++){
		if(temp[i] != '0')
			count = i+1;
	}

	if(count != 0)
		str += '.';
		
	for(i = 2; i < count; i++)
		str += temp[i];

	return str;
}*/

// 수숫점 몇자리(decimalDownCount - 소수부분)까지만 바꾼다.
// ex) Num = 1234.5678   decimalDownCount = 2 -> return : "1234.57" (반올림)
//                       decimalDownCount = 6 -> return : "1234.567800"
//                       decimalDownCount = -2 -> return : "12" (반올림)
CString CDataConversion::DoubleToString(const double& Num, const int decimalDownCount)
{
	if(decimalDownCount == 0){
		return DoubleToString(CMath::Round_Double(Num));
	}
	if(decimalDownCount > 0){
		double dRoundNum = CMath::Round(Num, decimalDownCount +1);
		CString strData = DoubleToString(dRoundNum);
		int nStart = 0;
		int nFindIndex = strData.Find('.');
		if(nFindIndex >= 0)
			nStart = strData.GetLength() - (nFindIndex +1);
		else
			strData += '.';

		if(nStart > decimalDownCount){
			strData = strData.Left(nFindIndex + decimalDownCount + 1);
		}

		for(int nIndex = nStart; nIndex < decimalDownCount; nIndex++)
			strData += '0';

		return strData;
	}

	double dData = CMath::Round(Num, abs(decimalDownCount));
	CString strData;
	strData.Format("%.0f", dData);
	strData = strData.Left(strData.GetLength() + decimalDownCount); // "xxxxxx" -> "xxxx" (negativeCount = 2 경우)
	return strData;
}

// CString -> double
// update 11.27.2001
// atof() 로 구현 가능 (sy 2003.04.23.)
// -> % 와 같은 이물질 제거를 위함이였으나 불필요하게 이함수가 남용 되는걸 막기 위해 없앰.
/*double CDataConversion::StringToDouble(const CString& strData)
{
	CString strDecimal = strData;
	bool isPlus = IsPluse(strDecimal);
	// 03.16.2002	%등의 이물질을 제거한다
	if(strDecimal.Right(1) != "0" && atoi(strDecimal.Right(1)) == 0)
		strDecimal = strDecimal.Left(strDecimal.GetLength() - 1);

	CString strInt = GetStringData(strDecimal, ".");
	if(strData.Find(".") < 0)	{
		strDecimal = "";
		strInt = strData;
	}
	
	double IntData = 0.0, DecimalData = 0.0;
	if(!strInt.IsEmpty() && strInt != "0"){ //정수 부분
		IntData = atof(strInt);
	}
	if(!strDecimal.IsEmpty() && strDecimal != "0"){  // 소수점 부분
		DecimalData = atof(strDecimal)*pow(0.1, (double)strDecimal.GetLength());
	}

	if(isPlus)
		return (IntData + DecimalData);
	return (IntData - DecimalData);
}*/

// 숫자형 문자열로 변환한다.
// 단 '.'은 숫자형으로 본다.
CString CDataConversion::NormalNumberString(const CString& strData)
{
	if(strData.IsEmpty())
		return "";

	CString strRealData;
	for(int nIndex = 0; nIndex < strData.GetLength(); nIndex++)
	{
		char cData = strData[nIndex];

		// (2008/3/27 - Seung-Won, Bae) Do not use isdigit(). (It did not work in some system)
		if(cData == '.' || ( '0' <= cData && cData <= '9'))
			strRealData += cData;
	}
	return strRealData;
}

// 가장 문자열이 긴 data.
double CDataConversion::GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType)
{
	int nDecimalDownCount = GetDecimalDownCount(dataType);
	CString strMin = DoubleToString(dData1, nDecimalDownCount);
	CString strMax = DoubleToString(dData2, nDecimalDownCount);

	if(strMin.GetLength() > strMax.GetLength())
		return dData1;

	return dData2;
}

// ----------------------------------------------------------------------------
//sy 2004.08.18
// int -> time format int
/*int CDataConversion::CalcTimeFormat(const CString& strValue, const CString& strCharge, const CString& strFormat, const bool bIsFuture)
{
	CString szFormat = strFormat, szDD, szSS, szMM, szHH, szDD1, szSS1, szMM1, szHH1, szTime = strValue, szCharge = strCharge;
	szFormat.MakeUpper();	

	if(szFormat.Find("SS") >= 0)	{
		szSS = szTime.Mid(szFormat.Find("SS"), 2);
		szSS1 = szCharge.Mid(szFormat.Find("SS"), 2);
	}
	if(szFormat.Find("MM") >= 0)	{
		szMM = szTime.Mid(szFormat.Find("MM", 2), 2);
		szMM1 = szCharge.Mid(szFormat.Find("MM", 2), 2);
	}
	if(szFormat.Find("HH") >= 0)	{
		szHH = szTime.Mid(szFormat.Find("HH"), 2);
		szHH1 = szCharge.Mid(szFormat.Find("HH"), 2);
	}
	if(szFormat.Find("MMDD") >= 0)	{
		szDD = szTime.Mid(szFormat.Find("MMDD"), 4);
		szDD1 = szCharge.Mid(szFormat.Find("MMDD"), 4);
	}
	else if(szFormat.Find("DD") >= 0)	{
		szDD = szTime.Mid(szFormat.Find("DD"), 2);
		szDD1 = szCharge.Mid(szFormat.Find("DD"), 2);
	}

	int nSS = atoi(szSS), nMM = atoi(szMM), nHH = atoi(szHH), nDD = atoi(szDD),
		nSS1 = atoi(szSS1), nMM1 = atoi(szMM1), nHH1 = atoi(szHH1), nDD1 = atoi(szDD1);

	if(!bIsFuture)
	{
		if(nSS < nSS1)
		{
			nMM --;
			nSS = nSS + 60 - nSS1;
			if(nSS < 0)
				nSS = abs(nSS);
		}
		else 
			nSS -= nSS1;

		if(nMM < nMM1)
		{
			nHH --;
			nMM = nMM + 60 - nMM1;
		}
		else 
			nMM -= nMM1;

		nHH -= nHH1;
		if(nHH < 9)
		{
			if(nDD > 0)
				nDD -- ;
			nHH = 7 + nHH;
		}
		nDD -= nDD1;
	}
	else
	{
		nSS += nSS1;
		nMM += nMM1;
		nHH += nHH1;
		nDD += nDD1;
		if(nSS >= 60)
		{
			if(nSS >= 120)
			{
				nMM += 2;
				nSS -= 120;
			}
			else
			{
				nMM ++;
				nSS -= 60;
			}
		}

		if(nMM >= 60)
		{
			if(nMM >= 120)
			{
				nHH += 2;
				nMM -= 120;
			}
			else
			{
				nHH ++;
				nMM -= 60;
			}
		}

		if(nDD > 0)
		{
			//DDHHMMSS 날짜 type이 있음.
			//?? 16시가 넘은 경우 장외거래 되는 경우가 있다.
			//if(nHH >= 16) {
			//	nDD ++;
			//	nHH -= 7;
			//}

			// MMDD로 시작되는 경우 -> 월과 일부분을 따로 구분하여 처리
			nDD = GetRealDailyInTimeFormat(nDD, strFormat);
			//sy end
		}
		//날짜 type이 없으면 다음날짜를 표현할수 없다.
		else
		{ 
			//마지막 시간을 넘을때는 마지막 시간으로 세팅한다.
			if(nHH > 24)
			{	
				nHH = 24;
				nMM = 59;
				nSS = 59;
			}
		}
	}
	szSS = IntToString(nSS, 2);
	szMM = IntToString(nMM, 2);
	szHH = IntToString(nHH, 2);
	if(szFormat.Find("MMDD") >= 0)
		szDD = IntToString(nDD, 4);
	else
		szDD = IntToString(nDD, 2);

	CString strRet = "";
	// 날짜가 없으면 값에 넣지 않는다
	if(szFormat.Find("DD") >= 0 && szDD != "00")
		strRet += szDD;
	if(szFormat.Find("HH") >= 0)
		strRet += szHH;
	if(szFormat.Find("MM") >= 0)
		strRet += szMM;
	if(szFormat.Find("SS") >= 0)
		strRet += szSS;

	return atoi(strRet);
}*/

int CDataConversion::CalcTimeFormat(const CString& strBaseValue, const CString& strAddValue, 
		const CString& strFormat, const bool bIsFuture, const bool bIsTransactionTime)
{
	if(bIsFuture)
		return CalcTimeFormat_Future(strBaseValue, strAddValue, strFormat, bIsTransactionTime);
	
	return CalcTimeFormat_Past(strBaseValue, strAddValue, strFormat, bIsTransactionTime);
}

//-> 미래
//   bIsTransactionTime = true : 9:00 ~ 15:00 의 시간대로 구성한다.
int CDataConversion::CalcTimeFormat_Future(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime)
{
	int nBaseDay = 0, nBaseHour = 0, nBaseMin = 0, nBaseSec = 0;
	int nAddDay = 0, nAddHour = 0, nAddMin = 0, nAddSec = 0;
	if(!GetTimeData(strBaseValue, strAddValue, strFormat, 
		nBaseDay, nBaseHour, nBaseMin, nBaseSec, nAddDay, nAddHour, nAddMin, nAddSec))
		return 0;

	int nDay = nBaseDay + nAddDay;
	int nHour = nBaseHour + nAddHour;
	int nMin = nBaseMin + nAddMin;
	int nSec = nBaseSec + nAddSec;

	//일자가 없고 마지막 시간을 넘을때는 마지막 시간으로 세팅한다.
	if(strFormat.Find("DD") < 0)
	{
		// "00시 59분 59초"
		if(nHour > 24)
			return GetTimeInFormat(strFormat, 24, 0, 59, 59);
	}

	//초
	GetDataInLimit(60, nSec, nMin);
	//분
	GetDataInLimit(60, nMin, nHour);
	//시 -> 임의 거래시간으로 계산할지의 여부에 따라 틀려짐.
	GetHourTime_Future(nBaseHour, nAddHour, bIsTransactionTime, nDay, nHour);
	//일
	nDay = GetRealDailyInTimeFormat(nDay, strFormat);

	return GetTimeInFormat(strFormat, nDay, nHour, nMin, nSec);
}

//-> 미래(시간)
void CDataConversion::GetHourTime_Future(const int nBaseHour, const int nAddHour, 
		const bool bIsTransactionTime, int& nDay, int& nHour)
{
	// 거래시간을 사용하지 않은 경우. 0 ~ 24시
	if(!bIsTransactionTime)
	{
		GetDataInLimit(24, nHour, nDay);
		return;
	}

	// (2006/7/11 - Seung-Won, Bae) Check Minute Sum over 60.
	int nAddHour2 = nAddHour + ( nHour - nAddHour - nBaseHour);

	// 거래시간을 사용하는 경우. 9 ~ 15시
	nDay += (nAddHour2 / 6);
	nHour = nBaseHour + (nAddHour2 % 6);
	if(nHour > 15)
	{
		nHour -= 15;
		if(nHour < 9)
			nHour += 8;

		nDay++;
	}
}

//-> 과거
int CDataConversion::CalcTimeFormat_Past(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, const bool bIsTransactionTime)
{
	int nBaseSec = 0, nBaseMin = 0, nBaseHour = 0, nBaseDay = 0;
	int nAddSec = 0, nAddMin = 0, nAddHour = 0, nAddDay = 0;
	if(!GetTimeData(strBaseValue, strAddValue, strFormat, 
		nBaseDay, nBaseHour, nBaseMin, nBaseSec, nAddDay, nAddHour, nAddMin, nAddSec))
		return 0;

	//이부분은 기존코딩을 그대로 둠. 디버깅하기에 힘들어서..
	if(nBaseSec < nAddSec)
	{
		nBaseMin --;
		nBaseSec = nBaseSec + 60 - nAddSec;
		if(nBaseSec < 0)
			nBaseSec = abs(nBaseSec);
	}
	else 
		nBaseSec -= nAddSec;

	if(nBaseMin < nAddMin)
	{
		nBaseHour --;
		nBaseMin = nBaseMin + 60 - nAddMin;
	}
	else 
		nBaseMin -= nAddMin;

	nBaseHour -= nAddHour;
	if(nBaseHour < 0)
	{
		if(nBaseDay > 0)
			nBaseDay -- ;
		nBaseHour = 24 + nBaseHour;
	}
	nBaseDay -= nAddDay;

	return GetTimeInFormat(strFormat, nBaseDay, nBaseHour, nBaseMin, nBaseSec);
}

int CDataConversion::GetTimeInFormat(const CString& strFormat, const int nDay, const int nHour, const int nMin, const int nSec)
{
	if(strFormat.IsEmpty())
		return 0;

	CString strAllData;
	//일
	if(strFormat.Find("MMDD") >= 0)
		strAllData = IntToString(nDay, 4);
	else if(strFormat.Find("DD") >= 0)
		strAllData = IntToString(nDay, 2);

	//시
	if(strFormat.Find("HH") >= 0)
		strAllData += IntToString(nHour, 2);
	//분
	if(strFormat.Find("MM", 2) >= 0)
		strAllData += IntToString(nMin, 2);
	//초
	if(strFormat.Find("SS") >= 0)
		strAllData += IntToString(nSec, 2);

	return atoi(strAllData);
}

bool CDataConversion::GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		int& nBaseDay, int& nBaseHour, int& nBaseMin, int& nBaseSec, 
		int& nAddDay, int& nAddHour, int& nAddMin, int& nAddSec)
{
	if(strBaseValue.IsEmpty() || strAddValue.IsEmpty() || strFormat.IsEmpty())
		return false;

	//초
	GetTimeData(strBaseValue, strAddValue, strFormat, "SS", 0, nBaseSec, nAddSec);
	//시
	GetTimeData(strBaseValue, strAddValue, strFormat, "HH", 0, nBaseHour, nAddHour);
	//분
	GetTimeData(strBaseValue, strAddValue, strFormat, "MM", 2, nBaseMin, nAddMin);

	//일
	if(strFormat.Find("MMDD") >= 0)
		GetTimeData(strBaseValue, strAddValue, strFormat, "MMDD", 0, nBaseDay, nAddDay);
	else if(strFormat.Find("DD") >= 0)
		GetTimeData(strBaseValue, strAddValue, strFormat, "DD", 0, nBaseDay, nAddDay);

	return true;
}

bool CDataConversion::GetTimeData(const CString& strBaseValue, const CString& strAddValue, const CString& strFormat, 
		const CString& strFindType, const int nFindStart, int& nBaseData, int& nAddData)
{
	int nFindIndex = strFormat.Find(strFindType, nFindStart);
	int nLength = strFindType.GetLength();
	if(nFindIndex < 0 || nLength <= 0)
		return false;

	CString strTemp = strBaseValue.Mid(nFindIndex, nLength);
	nBaseData = atoi(strTemp);
	strTemp = strAddValue.Mid(nFindIndex, nLength);
	nAddData = atoi(strTemp);
	return true;
}
//sy end

//sy 2004.08.18
// MMDD로 시작되는 경우 -> 월과 일부분을 따로 구분하여 처리
int CDataConversion::GetRealDailyInTimeFormat(const int nDD, const CString& strFormat)
{
	int nYear = 0, nMonth = 0, nDay = 0;
	if(!GetRealDailyInTimeFormat(nDD, nYear, nMonth, nDay))
		return nDD;

	// 년, 월을 올바르게 맞춘다.
	ChagneUprightData_YearAndMonth(nYear, nMonth);
	// 일을 올바르게 맞춘다.
	ChagneUprightData_Day(nYear, nMonth, nDay);

	if(strFormat.Find("MMDD") >= 0)
	{
		return (nMonth * 100 + nDay);
	}
	else if(strFormat.Find("DD") >= 0)
	{
		return nDay;
	}

	return nDD;
}

//-> 실질적인 월, 일 값을 구한다.
//-> MMDD 가 붙어져 있는걸 년, 월, 일로 나누어 분류한다.
bool CDataConversion::GetRealDailyInTimeFormat(const int nOrgData, int& nYear, int& nMonth, int& nDay)
{
	if(nOrgData <= 0)
		return false;

	nDay = nMonth = nYear = 0;
	if(nOrgData >= 10000)
	{
		nYear = nOrgData / 10000;
		nMonth = (nOrgData % 10000) / 100;
		nDay = (nOrgData % 10000) % 100;
	}
	else
	{
		nMonth = nOrgData / 100;
		nDay = nOrgData % 100;
	}

	return true;
}

//-> 년, 월을 올바르게 맞춘다.
void CDataConversion::ChagneUprightData_YearAndMonth(int& nYear, int& nMonth)
{
	// 년, 월이 없을 경우에는 현재 PC의 년,월로 셋팅한다.
	if(nYear <= 0 || nMonth <= 0)
	{
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime);

		if(nMonth <= 0)
			nMonth = systemTime.wMonth;

		if(nYear <= 0)
			nYear = systemTime.wYear;
	}

	while(nMonth > 12)
	{
		nMonth -= 12;
		nYear++;
	}
}

//-> 일을 올바르게 맞춘다.
/*void CDataConversion::ChagneUprightData_Day(int& nYear, int& nMonth, int& nDay)
{
	while(IsCalUprightData(nYear, nMonth, nDay))
	{
		if(nMonth == 2)
		{
			if(nYear > 0 && ((nYear % 4) == 0))
			{
				if(nDay <= 29)
					break;
				nDay -= 29;
				nMonth++;
			}
			else
			{
				if(nDay <= 28)
					break;
				nDay -= 28;
				nMonth++;
			}
		}
		else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
		{
			if(nDay <= 30)
				break;

			nDay -= 30;
			nMonth++;
		}
		else
		{
			if(nDay <= 31)
				break;

			nDay -= 31;
			nMonth++;
		}

		if(nMonth > 12)
		{
			nMonth -= 12;
			nYear++;
		}
	}
}*/

void CDataConversion::ChagneUprightData_Day(int& nYear, int& nMonth, int& nDay)
{
	// 12달중 가장 한달중 날짜가 작은 날로 한다.
	while(nDay > 28)
	{
		int nMonthFullDay = GetMonthFullDay(nYear, nMonth, nDay);
		if(nMonthFullDay <= 0)
			return;

		if(nDay <= nMonthFullDay)
			return;

		nDay -= nMonthFullDay;
		nMonth++;

		if(nMonth > 12)
		{
			nMonth -= 12;
			nYear++;
		}
	}
}

int CDataConversion::GetMonthFullDay(const int nYear, const int nMonth, const int nDay)
{
	// 2월 : 28 or 29일
	if(nMonth == 2)
	{
		if(nYear > 0 && ((nYear % 4) == 0))
			return 29;
		return 28;
	}
	// 4, 6, 9, 11달 : 30일
	else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
	{
		return 30;
	}

	return 31;
}

// 올바른 month 와 day 를 구할지의 여부
bool CDataConversion::IsCalUprightData(const int nYear, const int nMonth, const int nDay)
{
	if(nDay <= 28)
		return false;

	// -> 2월: 28일로 계산.
	// -> 4, 6, 9, 11달 : 30일
	// -> 나머지 달 : 31일
	if(nMonth > 12 || nDay > 31)
	{
		return true;
	}
	else if(nMonth <= 0)
	{
		return (nDay > 31);
	}
	else if(nMonth == 2)
	{
		if(nYear > 0 && ((nYear % 4) == 0))
			return (nDay > 29);
		return (nDay > 28);
	}
	else if(nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11)
	{
		return (nDay > 30);
	}
	else
	{
		return (nDay > 31);
	}

	return false;
}
//sy end

// int -> time format int
int CDataConversion::CalcTimeFormat(const int nValue, const int nCharge, const CString& strFormat, const bool bIsTransactionTime)
{
	if(strFormat.IsEmpty()) return 0;
	
	CString szTime = IntToString(nValue, strFormat.GetLength());	
	CString szCharge = IntToString(abs(nCharge), strFormat.GetLength());
	
	return CalcTimeFormat(szTime, szCharge, strFormat, (nCharge >= 0), bIsTransactionTime);
}

int CDataConversion::CalcTimeFormat_Past(const int nValue, const int nCharge, const CString& strFormat)
{
	if(strFormat.IsEmpty() )
		return 0;
	
	CString szTime = IntToString(nValue, strFormat.GetLength());	
	CString szCharge = IntToString(abs(nCharge), strFormat.GetLength());

	CString strBaseValue = szTime;
	CString strAddValue = szCharge;

	int nBaseDay = 0, nBaseHour = 0, nBaseMin = 0, nBaseSec = 0;
	int nAddDay = 0, nAddHour = 0, nAddMin = 0, nAddSec = 0;
	if(!GetTimeData(strBaseValue, strAddValue, strFormat, 
		nBaseDay, nBaseHour, nBaseMin, nBaseSec, nAddDay, nAddHour, nAddMin, nAddSec))
		return 0;

	int nDay = nBaseDay - nAddDay;
	int nHour = nBaseHour - nAddHour;
	int nMin = nBaseMin - nAddMin;
	int nSec = nBaseSec - nAddSec;

	//일자가 없고 마지막 시간을 넘을때는 처음 시간으로 세팅한다.
	if(strFormat.Find("DD") < 0)
	{
		// "00시 00분 00초"
		if(nHour < 0 )
			return GetTimeInFormat(strFormat, 24, 0, 0, 0);
	}

	//초
	while( nSec < 0 )
	{
		nSec += 60;
		nMin--;
	}
	while( nMin < 0 )
	{
		nMin += 60;
		nHour--;
	}
	while( nHour < 0 )
	{
		nHour += 24;
//		nDay--;
	}

	//일
	nDay = GetRealDailyInTimeFormat(nDay, strFormat);

	return GetTimeInFormat(strFormat, nDay, nHour, nMin, nSec);
}

//sy 2002.7.30. -> 미래 시간 구하기(기존소스에서 수정)
/*int CDataConversion::CalcTimeFormatforCharge(const int nValue, const int nCharge, const CString& strFormat, const int nInterval)
{
	if(nValue <= 0 || nCharge == 0)
		return nValue;	

	int mon = 0, day = 0, hour = 0, min = 0, sec = 0;
	if(!OnPickoutTime(abs(nCharge), strFormat, nInterval, mon, day, hour, min, sec))
		return nValue;

	CString szChargeTime = "";
	if(strFormat.Left(2) == "MM")
		szChargeTime += IntToString(mon, 2);
	if(strFormat.Find("DD") >= 0)
		szChargeTime += IntToString(day, 2);
	if(strFormat.Find("HH") >= 0)
		szChargeTime += IntToString(hour, 2);
	if(strFormat.Find("MM") >= 0)
		szChargeTime += IntToString(min, 2);
	if(strFormat.Find("SS") >= 0)
		szChargeTime += IntToString(sec, 2);
	
	CString szTime = IntToString(nValue, strFormat.GetLength());
	
	return CalcTimeFormat(szTime, szChargeTime, nCharge, strFormat);
}*/
//sy end

//sy 2004.08.18
int CDataConversion::CalcTimeFormatforCharge(const int nValue, const int nIndex, const CString& strFormat, const int nInterval, const bool bIsTransactionTime)
{
	if(!IsCalcTimeFormatforCharge(nValue, nIndex, strFormat, nInterval))
		return nValue;	

	CString strAdditionTime = GetAdditionDataInFormat(nValue, nIndex, strFormat, nInterval);
	if(strAdditionTime.IsEmpty())
		return nValue;

	CString strCurTime = IntToString(nValue, strFormat.GetLength());
	return CalcTimeFormat(strCurTime, strAdditionTime, strFormat, (nIndex >= 0), bIsTransactionTime);
}

bool CDataConversion::IsCalcTimeFormatforCharge(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval)
{
	if(nBaseTime <= 0 || nAddIndex == 0 || strFormat.IsEmpty() || nInterval <= 0)
		return false;	

	CString strRight = strFormat.Right(1);
	if(strRight == "Y" || strRight == "D")
		return false;

	return (strFormat != "YYYYMM");
}

// nAddIndex = 100 처럼 index 값을 DDHHMMSS 형태인 00006000 식으로 수정한다.
CString CDataConversion::GetAdditionDataInFormat(const int nBaseTime, const int nAddIndex, const CString& strFormat, const int nInterval)
{
	int nMon = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;
	if(!GetAllTime(abs(nAddIndex), nInterval, strFormat, nMon, nDay, nHour, nMin, nSec))
		return "";

/*	nMon = nDay = nHour = nMin = nSec = 0;
	if(!OnPickoutTime(nBaseTime, abs(nAddIndex), strFormat, nInterval, nMon, nDay, nHour, nMin, nSec))
		return "";*/

	CString strAdditionTime;
	if(strFormat.Left(2) == "MM")
		strAdditionTime += IntToString(nMon, 2);
	if(strFormat.Find("DD") >= 0)
		strAdditionTime += IntToString(nDay, 2);
	if(strFormat.Find("HH") >= 0)
		strAdditionTime += IntToString(nHour, 2);
	if(strFormat.Find("MM") >= 0)
		strAdditionTime += IntToString(nMin, 2);
	if(strFormat.Find("SS") >= 0)
		strAdditionTime += IntToString(nSec, 2);
	
	return strAdditionTime;
}

// format 과 interval 를 이용해 해당 값으로 변환한다.
//->ex) "MMDDHHMMSS", interval = 1000 : 10분 간격이라는 의미. 즉 nMin의 data만 존재.
bool CDataConversion::GetAllTime(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay, int& nHour, int& nMin, int& nSec)
{
	// 시간 부분 - 시, 분, 초
	if(!GetAllTime_TimePart(nAddIndex, nInterval, strFormat, nDay, nHour, nMin, nSec))
		return false;

	// 날짜 부분 - 월, 일
	GetAllTime_DatePart(nAddIndex, nInterval, strFormat, nMon, nDay);
	return true;
}

//-> 시간 부분
bool CDataConversion::GetAllTime_TimePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nDay, int& nHour, int& nMin, int& nSec)
{
	bool bIsIntervalEnd = false;
	// 초
	GetAllTime_Second(nAddIndex, nInterval, strFormat, nSec, nMin, bIsIntervalEnd);
	// 분
	if(!bIsIntervalEnd)
	{
		GetAllTime_Minute(nAddIndex, nInterval, strFormat, nMin, nHour, bIsIntervalEnd);
	}
	// 시
	if(!bIsIntervalEnd)
	{
		GetAllTime_Hour(nAddIndex, nInterval, strFormat, nHour, nDay, bIsIntervalEnd);
	}

	GetDataInLimit(60, nMin, nHour);
	GetDataInLimit(24, nHour, nDay);
	return (nSec > 0 || nMin > 0 || nHour > 0 || nDay > 0);
}

//-> 날짜 부분
bool CDataConversion::GetAllTime_DatePart(const int nAddIndex, const int nInterval, const CString& strFormat, 
		int& nMon, int& nDay)
{
	// 일
	if(strFormat.Find("DD") >= 0)
	{
	}
	// 월
	if(strFormat.Find("MMDD") >= 0)
	{
	}

	return (nDay > 0 || nMon > 0);
}

//-> "초" 관련 부분. 
//   bIsIntervalEnd : interval 값이 다 적용되었는지의 여부.
bool CDataConversion::GetAllTime_Second(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nSec, int& nMin, bool& bIsIntervalEnd)
{
	if(nInterval <= 0 || strFormat.Find("SS") < 0)
		return false;

	return GetAllTime(nAddIndex, nInterval, 100, 1, 60, nSec, nMin, bIsIntervalEnd);
}

//-> "분" 관련 부분.
bool CDataConversion::GetAllTime_Minute(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nMin, int& nHour, bool& bIsIntervalEnd)
{
	if(strFormat.Right(4) == "MMSS")
	{
		return GetAllTime(nAddIndex, nInterval, 10000, 100, 60, nMin, nHour, bIsIntervalEnd);
	}
	else if(strFormat.Right(4) == "HHMM")
	{
		return GetAllTime(nAddIndex, nInterval, 100, 1, 60, nMin, nHour, bIsIntervalEnd);
	}

	return false;
}

//-> "시" 관련 부분.
bool CDataConversion::GetAllTime_Hour(const int nAddIndex, const int nInterval, 
		const CString& strFormat, int& nHour, int& nDay, bool& bIsIntervalEnd)
{
	//ing
	int nFindIndex = strFormat.Find('H');
	if(nFindIndex < 0)
		return false;

	int nMaxGap = CMath::Power(10, (strFormat.GetLength() - nFindIndex));
	int nMinGap = nMaxGap / 100;

	return GetAllTime(nAddIndex, nInterval, nMaxGap, nMinGap, 24, nHour, nDay, bIsIntervalEnd);
}


bool CDataConversion::GetAllTime(const int nAddIndex, const int nInterval, const int nMaxGap, const int nMinGap, 
		const int nLimitData1, int& nData1, int& nData2, bool& bIsIntervalEnd)
{
	if(nLimitData1 <= 0 || nMaxGap <= 0 || nMinGap <= 0)
		return false;

	bIsIntervalEnd = (nInterval < nMaxGap);
	int nAddData = nAddIndex * ((nInterval % nMaxGap) / nMinGap);
	nData1 += nAddData;

	GetDataInLimit(nLimitData1, nData1, nData2);
	return (nAddData > 0);
}

void CDataConversion::GetDataInLimit(const int nLimitData1, int& nData1, int& nData2)
{
	while(nData1 >= nLimitData1)
	{
		nData1 -= nLimitData1;
		nData2++;
	}
}
//sy end

int CDataConversion::CalcDateFormat(const int nValue, const int nCharge, const CString& strFormat, const int nCalcType, const int nInterval)
{
	if(strFormat.IsEmpty())
		return 0;
	
	int year, mon, day = 0, len, year_c, mon_c, day_c = 0;
	if(OnPickoutDate(abs(nCharge), strFormat, nCalcType, year_c, mon_c, day_c) == false)
		return 0;
	
	//	int ndate = nValue + nCharge;
	CString strDate = IntToString(nValue, strFormat.GetLength());
	if((len = GetStringDataCount(strFormat, "Y")) > 0)
		year = atoi(strDate.Mid(strFormat.Find("Y"), len));
	if((len = GetStringDataCount(strFormat, "M")) > 0)
		mon = atoi(strDate.Mid(strFormat.Find("M"), len));
	if((len = GetStringDataCount(strFormat, "D")) > 0)
		day = atoi(strDate.Mid(strFormat.Find("D"), len));
	
	if(nCharge > 0)	{
		if((day += day_c) > 30)	{
			int nmon = day / 30;
			day -= 30 * nmon;
			mon += nmon;
		}
		if((mon += mon_c) > 12)	{
			int nyear = mon / 12;
			mon -= 12 * nyear;
			year += nyear;
		}
		year += year_c;
	}
	else if(nCharge < 0)	{
		if((day -= day_c) <= 0)	{
			int nmon = day / 30 - 1;
			day -= 30 * nmon;
			mon += nmon;
		}
		if((mon -= mon_c) <= 0)	{
			int nyear = mon / 12 - 1;
			mon -= 12 * nyear;
			year += nyear;
		}
		year -= year_c;
	}
	else
		return nValue;

	CString retStr = IntToString(year, GetStringDataCount(strFormat, "Y")) + IntToString(mon, 2);
	if(strFormat.Find("DD") >= 0)
		retStr += IntToString(day, 2);

	return atoi(retStr) ;
}

// 20080805 김진순 24시간 시세  <<
CString CDataConversion::GetRealDateInTimeFormat(const int nDD, const CString& strFormat)
{
	int nYear = 0, nMonth = 0, nDay = 0;
	if(!GetRealDailyInTimeFormat(nDD, nYear, nMonth, nDay))
	{
		CString strTemp;
		strTemp.Format("%d", nDD);
		return strTemp;//2022.7.27.tour2k
		//return nDD;
	}

	// 년, 월을 올바르게 맞춘다.
	ChagneUprightData_YearAndMonth(nYear, nMonth);
	// 일을 올바르게 맞춘다.
	ChagneUprightData_Day(nYear, nMonth, nDay);

	bool bIsYear(false), bIsMonth(false), bIsDay(false);

	// "YYYY..." : "YYYYMM", "YYYYMMDD"
	if(strFormat.Find("YYYY") >= 0)
		bIsYear = true;

	// "MMDD...", "YYYYMM..." : "YYYYMM", "YYYYMMDD", "MMDD", "MMDDHHSS"
	if(strFormat.Find("MMDD") >= 0 || strFormat.Find("YYYYMM") >= 0)
		bIsMonth = true;
	
	// "...DD..." : "YYYYMMDD", "MMDD", "MMDDHHMMSS", "DDHHMMSS"
	if(strFormat.Find("DD") >= 0)
		bIsDay = true;


	CString strDate;
	//strDate.Format("%04d%02d%02d", (bIsYear ? nYear : ""), (bIsMonth ? nMonth: ""), (bIsDay ? nDay: ""));
	if( bIsYear && bIsMonth && bIsDay )
		strDate.Format("%04d%02d%02d", nYear, nMonth, nDay);
	else if( bIsMonth && bIsDay )
		strDate.Format("%02d%02d", nMonth, nDay);
	else if( bIsDay )
		strDate.Format("%02d", nDay);

	return strDate;
}
//>>

// data 추출 (string 형) ----------------------------------------------------------------
// data에서 처음으로 trimStr가 있는 문자까지를 자른다.
// - ex) GetStringData("It is trim test", "t")
// bIsInside = true  -> return : "I", data : " is trim test"
// bIsInside = false -> return : "It", data : " is trim test"
CString CDataConversion::GetStringData(CString& data, const CString& trimData, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	int count = data.Find(trimData);
	if(count < 0)
		return "";

	int length = data.GetLength();
	CString retData;
	if(count > 0)
		retData = data.Left(count);
	data = data.Mid(count+trimData.GetLength(), length - count);

	if(bIsInside)
		return retData;
	return (retData + trimData);
}

CString CDataConversion::GetStringData(CString& data, const CString& trimStart, const CString& trimEnd, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	GetStringData(data, trimStart);
	CString strInsideData = GetStringData(data, trimEnd);

	if(bIsInside)
		return strInsideData;
	return (trimStart + strInsideData + trimEnd);
}

// trimStr가 nIndex 번째까지의 문자까지를 자른다.
// - ex) GetStringData("It is trim test", " ", 3) -> return : "It is trim", data : "test"
CString CDataConversion::GetStringData(CString& data, const CString& trimStr, const int nIndex)
{
	if(data.IsEmpty())
		return "";

	CString tempData = data;
	CString retData;
	int totalCount = 0;
	for(int i = 0; i < nIndex; i++){
		int count = data.Find(trimStr);
		if(count == -1)
			break;
		data = data.Mid(count+trimStr.GetLength(), tempData.GetLength() - count);
		totalCount += count;
		totalCount += trimStr.GetLength();
	}
	retData = tempData.Left(totalCount);
	return retData;
}

// trimStr가 nIndex 번째까지의 string. -> nIndex > 0
// - ex) GetLeftToTrimDataIndex("It is trim test", " ", 2, true) -> return : "It is trim"
//												            false -> return : "It is trim "
CString CDataConversion::GetLeftToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside)
{
	int nFindIndex = FindIndex(data, trimStr, nEndIndex);
	if(nFindIndex < 0)
		return "";

	if(!bIsInside)
		nFindIndex += trimStr.GetLength();

	return data.Left(nFindIndex);
}

// - ex) GetMid..("시가,고가,저가,", ",", 0, true, true) -> return : "고가"
//										     false, false -> return : ",고가,"
// nIndex : trimStart Index
CString CDataConversion::GetMidToTrimDataStartIndex(const CString& data, const CString& trimStr, const int nStartIndex, const bool bIsInside_start, const bool bIsInside_end)
{
	if(nStartIndex < 0 || data.Find(trimStr) < 0)
		return "";

	// get할 시작 위치.
	int nFirstIndex = CDataConversion::FindIndex(data, trimStr, nStartIndex);
	if(nFirstIndex < 0)
		return "";

	// 앞의 trimdata 포함안할 경우
	if(bIsInside_start)
		nFirstIndex += trimStr.GetLength();

	// get할 끝 위치.
	int nEndIndex = CDataConversion::FindIndex(data, trimStr, nStartIndex +1);
	if(nEndIndex < 0){
		nEndIndex = data.GetLength();
	}

	int nCount = nEndIndex - nFirstIndex;
	if(!bIsInside_end){ // trim data의 뒷부분 포함하는 경우
		if(nEndIndex < data.GetLength())
			nCount += trimStr.GetLength();
	}

	return data.Mid(nFirstIndex, nCount);
}

// - ex) GetMid..("시가,고가,저가,", ",", 1, true, true) -> return : "고가"
//											 false, false -> return : ",고가,"
// nIndex : trimEnd Index
CString CDataConversion::GetMidToTrimDataEndIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside_start, const bool bIsInside_end)
{
	int nFindIndex = FindIndex(data, trimStr, nEndIndex);
	if(nFindIndex < 0)
		return "";

	// get할 시작 위치를 구하기.
	int nFirst = 0;
	if(nEndIndex > 0)
		nFirst = FindIndex(data, trimStr, nEndIndex -1);

	// trim data의 앞부분 포함안하는 경우
	if(bIsInside_start){
		// - 처음에 trimStr 이 없는 경우 생각하기
		if(nFirst > 0 || data.Left(trimStr.GetLength()) == trimStr){
			nFirst += trimStr.GetLength();
		}
	}

	int nCount = nFindIndex - nFirst;
	if(!bIsInside_end) // trim data의 뒷부분 포함하는 경우
		nCount += trimStr.GetLength();

	return data.Mid(nFirst, nCount);
}

// - ex) GetMid..("It is trim test", " ", 2, true) -> return : "trim"
//									         false -> return : "trim "
// - ex) GetMid..("시가,고가,저가,", ",", 1, true) -> return : "고가"
//									         false -> return : "고가,"
CString CDataConversion::GetMidToTrimDataIndex(const CString& data, const CString& trimStr, const int nEndIndex, const bool bIsInside)
{
	return GetMidToTrimDataEndIndex(data, trimStr, nEndIndex, true, bIsInside);
}

// - ex) GetMid..("It is trim test", " ", " ", 0, true) -> return : "is"
//											      false -> return : " is "
//										 	   1, true) -> return : "trim"
CString CDataConversion::GetMidToTrimDataIndex(const CString& data, const CString& trimStart, const CString& trimEnd, const int nStartIndex, const bool bIsInside)
{
	if(data.IsEmpty())
		return "";

	// trim 문자의 위치를 찾는다.
	// 만약 trim 문자가 null 일 경우 각자 처음과 끝을 의미한다.
	int nFindIndex_trimStart = FindIndex(data, trimStart, nStartIndex);
	int nFindIndex_trimEnd = -1;
	if(trimEnd.IsEmpty())
		nFindIndex_trimEnd = data.GetLength();
	else
		nFindIndex_trimEnd = data.Find(trimEnd, nFindIndex_trimStart +trimStart.GetLength());

	if(nFindIndex_trimStart < 0 || nFindIndex_trimEnd < 0)
		return "";

	// 추출할 string의 위치와 갯수를 구한다.
	int nFirst = nFindIndex_trimStart;
	if(bIsInside)
		nFirst += trimStart.GetLength();

	int nCount = nFindIndex_trimEnd - nFirst;
	if(!bIsInside)
		nCount += trimEnd.GetLength();

	return data.Mid(nFirst, nCount);
}

// 해당 문자가 총 몇개 가지고 있는지를 구한다.
int CDataConversion::GetStringDataCount(const CString& orgStr, const CString& comparisonStr)
{
	if(orgStr.IsEmpty() != 0 || comparisonStr.IsEmpty() != 0)
		return -1;

	// (2005/12/8 - Seung-Won, Bae) for Performance. Do without CString
	int count = 0;
	const char *szFound = orgStr;
	while( ( szFound = strstr( szFound, comparisonStr)) != NULL)
	{
		count++;
		szFound += comparisonStr.GetLength();
	}

	return count;
}

// 해당 index를 찾는다.
// orgData 에서 nFindDataCountIndex 번째 있는 findData의 index.
// ex) FindIndex("It is testing", "t", 2) -> return : 9
//     FindIndex("It is testing", "t", 3) -> return : -1
int CDataConversion::FindIndex(const CString& orgData, const CString& findData, const int nFindDataCountIndex)
{
	if(orgData.IsEmpty() || findData.IsEmpty() || nFindDataCountIndex < 0)
		return -1;

	int nFindStartIndex = -1;
	for(int nCount = 0; nCount <= nFindDataCountIndex; nCount++){
		int nIndex = orgData.Find(findData, nFindStartIndex +1);
		if(nIndex < 0)
			return -1;

		nFindStartIndex = nIndex;
	}

	return nFindStartIndex;
}

// 소수점 이하 몇자리인지를 가져온다.  0.001 -> return : 3
// bIsCheckDigit = true -> x0.01% -> return : 2
// bIsCheckDigit = false -> x0.01% -> return : 3
int CDataConversion::GetDecimalDownCount(const CString& strDecimalType, const bool bIsCheckDigit) 
{
	// 20080910 JS.Kim		8진법/32진법 처리, 소숫점 이하 자리수 최대자리로 Return <<
	if( IsBaseFormat(strDecimalType) )
	{
		int nBase, nDecLen1, nDecLen2;
		if( GetBaseFormatInfo(strDecimalType, nBase, nDecLen1, nDecLen2) )
			return nDecLen1 + nDecLen2;
		return 0;
	}
	//>>

	if(strDecimalType.IsEmpty() || strDecimalType.Find('.') < 0)
		return 0;

	CString strType = strDecimalType;
	if(bIsCheckDigit)
		strType = NormalNumberString(strDecimalType);

	int nIndex = strType.Find(".");
	if(strType.IsEmpty() || nIndex < 0)
		return 0;

	return (strType.GetLength() - (nIndex +1));
}

// data type을 double 형태로 변환
// "x0.01%", "x0.01", "X0.01%", "X0.01", "×0.01%", "×0.01", "1,000" 등
// 이런식으로 숫자와 소숫점을 제외한 타입일 경우만 사용한다.
double CDataConversion::GetDataType(CString strDataType)
{
	strDataType.Remove('%');
	strDataType.Remove('x');
	strDataType.Remove('X');
	strDataType.Remove(',');
	strDataType.Replace("×", "");

	return atof(strDataType);
}

// "x10000" -> "x만" 형태로 변환 (10000 이상만 한글로 바꾼다.)
CString CDataConversion::GetChangeFormat_NumCharUnit(const CString& strOrgFormat)
{
	CString strNewFormat = strOrgFormat;

	if(strOrgFormat.Find("1000000000000") != -1){	// 조
		strNewFormat.Replace("1000000000000", _LTEXT6( C6_TRILLION));
	}
	else if(strOrgFormat.Find("100000000000") != -1){// 천억
		strNewFormat.Replace("100000000000", _LTEXT6( C6_HUNDRED_BILLION));
	}
	else if(strOrgFormat.Find("10000000000") != -1){// 백억
		strNewFormat.Replace("10000000000", _LTEXT6( C6_TEN_BILLION));
	}
	else if(strOrgFormat.Find("1000000000") != -1){	// 십억
		strNewFormat.Replace("1000000000", _LTEXT6( C6_BILLION));
	}
	else if(strOrgFormat.Find("100000000") != -1){	// 억
		strNewFormat.Replace("100000000", _LTEXT6( C6_HUNDRED_MILLION));
	}
	else if(strOrgFormat.Find("10000000") != -1){	// 천만
		strNewFormat.Replace("10000000", _LTEXT6( C6_TEN_MILLION));
	}
	else if(strOrgFormat.Find("1000000") != -1){	// 백만
		strNewFormat.Replace("1000000", _LTEXT6( C6_MILLION));
	}
	else if(strOrgFormat.Find("100000") != -1){		// 십만
		strNewFormat.Replace("100000", _LTEXT6( C6_HUNDRED_THOUSAND));
	}
	else if(strOrgFormat.Find("10000") != -1){		// 만
		strNewFormat.Replace("10000", _LTEXT6( C6_TEN_THOUSAND));
	}

	return strNewFormat;
}

// 기본 문자열에 원하는 문자열을 원하는 수만큼 반복해서 insert 또는 append 한다
// strData : 기본문자열, strInsert : 삽입할 문자열, nInsertLength : 삽입할 문자열의 갯수
// insert type : 0 -> insert, 1 -> append
CString CDataConversion::InsertString(const CString& strData, const CString& strInsert, const int nInsertLength, const int nInsertType)
{
	if(nInsertLength <= 0)
		return strData;
	CString szData = strData, szInsert;

	for(int i = 0 ; i < nInsertLength ; i++)	{
		szInsert += strInsert;
	}
	if(nInsertType == 0)	{
		szData = szInsert + szData;
	}
	else if(nInsertType == 1)	{
		szData += szInsert;
	}

	return szData;
}

// 원하는 스트링을 절사해서 포맷과 스트링, 변환된 스트링 사이즈까지 반환한다
// 데이타가 백만단위보다 큰 경우에만 절사
// 그 외의 경우에는 GetStringFormatTrans() 사용한 결과와 같은 strdata가 생성된다
// strData : 변환한 데이타, strFormatTrans : 변환한 format
// return : 변환한 데이타의 길이
int CDataConversion::GetStringFormatTransCutoff(const double& dData, const CString& strFormat, CString& strData, CString& strFormatTrans, BOOL p_bWithAutoScale)
{
	// 20080910 JS.Kim		진법 표현인 경우	<<
	if( IsBaseFormat(strFormat) )
	{
		strFormatTrans = strFormat;
		CString szData = DoubleToString(dData);
		strData = GetStringFormatTrans(szData, strFormat);
		return strData.GetLength();	
	}
	//>>

	CString szFormatType, szFormat = strFormat;
	//strFormatTrans = szFormat;

	if(!strFormat.IsEmpty() && strFormat.Right(1) != "0" && strFormat.Right(1) != "1")	{
		szFormatType = strFormat.Right(1);
		szFormat = strFormat.Left(strFormat.GetLength() -1);
	}

	strFormatTrans = szFormat;

	CString szData = DoubleToString(dData);
	// 08.04.2002  - data는 double인데 타입이 정수인 경우 소수점 이하 절사
	if(strFormat.Right(1) == "1" && szData.Find(".") > 0){
		int nDecimalDownCount = 0;
		int nFormatIndex = strFormat.Find(".");
		if(nFormatIndex >= 0 && nFormatIndex <= (strFormat.GetLength() -1))
			nDecimalDownCount = strFormat.GetLength() - (nFormatIndex +1);
		szData = DoubleToString(dData, nDecimalDownCount);
	}
	
	if( szFormat.Find(".") < 0)
	{
		//sy 2002.10.28.
		if( szData.Find(".") != -1)  szData = GetStringData(szData, ".");

		int nStartIndex = szFormat.Find( "1");
		if( 0 <= nStartIndex)
		{
			int nTruncCount = szFormat.Mid( nStartIndex).Replace( '0', '_');
			if( 0 < nTruncCount) szData = szData.Left( szData.GetLength() - nTruncCount);
		}
	}

	//sy 2002.9.11. -> 1140000 : 절사 -114000 : 절사 안함 (음수는 - 빼고 생각)
	int nNumLength = szData.GetLength();
	if( p_bWithAutoScale && ( ( szData.Left(1) == '-' && nNumLength > 7) || (szData.Left(1) != '-' && nNumLength > 6)))
	{
		if(szFormat.Find(".") < 0)
		{
			strData = szData.Left(szData.GetLength() - (szData.GetLength() - 6));
			strFormatTrans = InsertString(szFormat, "0", szData.GetLength() - 6, 1);//strFormat + "0";
		}
		else if(szFormat.Find(".") >= 0)
		{
			int npoint = GetStringDataCount(szFormat, "0"); //소수점 자리가 몇인지 안다
			strData = szData.Left(szData.GetLength() - (szData.GetLength() - 6));
			int npow = szData.GetLength() - 6;
			if(szData.Find(".") > 0)
				npow -= szData.GetLength() - szData.Find(".");
			if(npow > 0)	{
				strFormatTrans.Delete(szFormat.Find(".") - 1, npoint + 1);
				strFormatTrans = InsertString(strFormatTrans, "0", npow, 1);
			}
		}
	}
	else
	{
		strData = szData;
		strFormatTrans = szFormat;
	}

	strFormatTrans += szFormatType;
	strFormatTrans.Replace(" ", "");

	strData = GetStringFormatTrans(strData, strFormatTrans);
	return strData.GetLength();	
}

int CDataConversion::GetStringFormatTransCutoff(const double& dData, const double& dFormat, CString& strData, CString& strFormatTrans, BOOL p_bWithAutoScale)
{
	return GetStringFormatTransCutoff(dData, DoubleToString(dFormat), strData, strFormatTrans, p_bWithAutoScale);
}

// 원하는 포맷으로 스트링 변경
//sy 2003.8.8. -> packet type 에 따라 무조건 버림을 하는 버그 수정.
CString CDataConversion::GetStringFormatTrans(const CString& strData, const CString& strFormat/*, const bool bIsRound*/)
{
	if(strData.IsEmpty())
		return strData;

	if(strFormat.IsEmpty())
		return GetStringThousandFormat(strData);

	int nFindIndex_1 = strFormat.Find("1");
	int nFormatLength = strFormat.GetLength();
	// 03.19.2002  format이 날짜 형인 경우에는 날짜 포맷 변경 함수를 따르도록 한다
	if(nFindIndex_1 < 0 && nFormatLength > 1)
		return GetStringFormatTrans(strData, strFormat, "");

	CString strUnit; // 단위 (%)

	// 20080910 JS.Kim				<<
	//	미국채채권 표시를 위해 8진법/32진법으로의 변환
	int nBase, nDecLen1, nDecLen2;
	if( GetBaseFormatInfo(strFormat, nBase, nDecLen1, nDecLen2) )
	{
		// 단위구하는 방식 변경. '0'이 아닌 경우로 체크하지 않고 숫자가 아니면 단위로 처리
		CString strUnit; // 단위 (%)
		char chData;
		while( nFormatLength > 0 )
		{
			chData = strFormat.GetAt(--nFormatLength);
			if(chData < '0' || chData > '9')
				strUnit.Insert(0, chData);
			else
				break;
		}
		
		return GetStringThousandFormat(Gl_Format32String(atof(strData), nBase, nDecLen1+nDecLen2, nDecLen2)) + strUnit;
	}
	//>>

	// 숫자인 경우
	if(strFormat.Right(1) != "0" && nFindIndex_1 != nFormatLength -1){
		strUnit = strFormat.Right(nFormatLength - (nFindIndex_1 + 1));
	}

	double dData = atof(strData);
	int nDecimalDownCount = GetDecimalDownCount(strFormat, true);
	if(nDecimalDownCount < 0)
		nDecimalDownCount = 0;

	// data type 숫자인 경우 (x1, x10, x0.1, x0.01)
	CString strRealData = DoubleToString(dData, nDecimalDownCount);
	return GetStringThousandFormat(strRealData) + strUnit;
}

/* packet type 에 의한 반올림이 아닌 버림 형태로 바꿔진다.
CString CDataConversion::GetStringFormatTrans(const CString& strData, const CString& strFormat)
{	
	if(strData.IsEmpty() == TRUE)
		return strData;

	if(strFormat.IsEmpty() == TRUE)
		return GetStringThousandFormat(strData);

	// 03.19.2002  format이 날짜 형인 경우에는 날짜 포맷 변경 함수를 따르도록 한다
	if(strFormat.Find("1") < 0 && strFormat.GetLength() > 1)
		return GetStringFormatTrans(strData, strFormat, "");

	CString szFormat = strFormat, szUnit = "", szMinus = "", szData = strData;

	if(strFormat.Right(1) != "0" && atoi(strFormat.Right(1)) == 0)	{
		szFormat = strFormat.Left(strFormat.GetLength() - 1);
		szUnit = strFormat.Right(1);
	}	

	int nDecimalType = GetDecimalDownCount(szFormat);

	if(strData.Left(1) == "-")	{
		szMinus = strData.Left(1);
		szData = strData.Right(strData.GetLength() - szMinus.GetLength());
	}
	// 08.04.2002  - data는 double인데 타입이 정수인 경우 소수점 이하 절사
//	if(szFormat.Find(".") < 0 && szFormat.Right(1) == "1" && strData.Find(".") > 0)
//		szData = strData.Left(strData.Find("."));
	if(szFormat.Find(".") < 0 && szFormat.Right(1) == "1" && szData.Find(".") > 0)
		szData = szData.Left(szData.Find("."));

	if(nDecimalType > 0){
		// 08.05.2002 데이타에 소수점에 있는 경우 원래 소수점 데이타 취하기
		if(szData.Find(".") > 0)	{
			if(nDecimalType > szData.GetLength() - szData.Find(".") - 1)	{
				//sy 2002.8.28.
				int nZeroCount = nDecimalType - (szData.GetLength() - szData.Find(".") - 1);
				for(int i = 0 ; i < nZeroCount; i++)
					szData += "0";
				//end
			}
			else
				szData = szData.Left(szData.Find(".")+1+nDecimalType);
		}
		// 데이타에 소수점에 없고 데이타 타입에 소수점이 있는 경우 소수점 더하기
		else	{			
			CString szDecimal('0', nDecimalType);
			szData = szData + "." + szDecimal;
		}

		int numberCount = szData.GetLength() - nDecimalType - 1;
		if(numberCount > 0)			
			return szMinus + GetStringThousandFormat(szData.Left(numberCount)) + szData.Right(nDecimalType + 1) + szUnit;
		else{
			CString temp;
			for(int i = numberCount; i < 0; i++)
				temp += "0";
			return szMinus + "0." + temp + szData + szUnit;
		}
	}

	return szMinus + GetStringThousandFormat(szData) + szUnit;
}*/

CString CDataConversion::GetStringThousandFormat(const double& dData, const double& dFormat)
{
	int decimalDownCount = 0;
	// 0.1, 0.01 형태
	if(dFormat < 1.0){
		double temp = dFormat;
		while(temp < 1.0){
			temp *= 10;
			decimalDownCount++;
		}
	}
	// 10, 100, 1000 형태
	else if(dFormat > 1.0){
		double temp = dFormat;
		while(temp > 1.0){
			temp /= 10;
			decimalDownCount--;
		}
	}

	CString strData = DoubleToString(dData, decimalDownCount);
	CString strNumberData = strData, strDecimalData;
	if(strData.Find('.') >= 0){
		strNumberData = GetStringData(strData, '.');
		strDecimalData = strData;
	}

	if(strNumberData.IsEmpty())
		return "";

	strNumberData = GetStringThousandFormat(strNumberData);
	if(!strDecimalData.IsEmpty() && strDecimalData.Left(1) != '.')
		strDecimalData = '.' + strDecimalData;

	return strNumberData + strDecimalData;
}

// 20080910 JS.Kim		미국채채권 표시를 위해 8진법/32진법으로의 변환을 위한 함수 추가<<
CString CDataConversion::GetStringThousandFormat(const double& dData, const CString& strFormat)
{
	CString strData = DoubleToString(dData, strFormat);
	CString strNumberData = strData, strDecimalData;
	if(strData.Find('\'') >= 0)
	{
		strNumberData = GetStringData(strData, '\'');
		strDecimalData = strData;
		if(!strDecimalData.IsEmpty() && strDecimalData.Left(1) != '\'')
			strDecimalData = '\'' + strDecimalData;
	}
	else if(strData.Find('.') >= 0)
	{
		strNumberData = GetStringData(strData, '.');
		strDecimalData = strData;
		if(!strDecimalData.IsEmpty() && strDecimalData.Left(1) != '.')
			strDecimalData = '.' + strDecimalData;

	}

	if(strNumberData.IsEmpty())
		return "";

	strNumberData = GetStringThousandFormat(strNumberData);

	return strNumberData + strDecimalData;
}
//>>

// 원하는 포맷으로 스트링 변경 (천단위마다 , 를 표시)
CString CDataConversion::GetStringThousandFormat(const CString& strData)
{
	CString temp = strData;
	bool puls = true;
	if(temp.Find('-') == 0){
		puls = false;
		temp.TrimLeft('-');
	}
	if(temp.GetLength() <= 3)
		return strData ;

	CString strDecimal, strT = temp;
	//sy -> 소숫점 이하 부분
	// 20080910 JS.Kim		미국채채권 표시를 위해 8진법/32진법으로의 변환을 위한 수정<<
	if(strT.Find('\'') != -1){
		strDecimal = strT.Right(strT.GetLength() - strT.Find('\''));
		strT.Replace(strDecimal, "");
	}
	//>>
	else if(strT.Find('.') != -1){
		strDecimal = strT.Right(strT.GetLength() - strT.Find('.'));
		strT.Replace(strDecimal, "");
	}
	//end
	
	CString str;
	while(!strT.IsEmpty())	{
		if(strT.GetLength() > 3)	{
			str = "," + strT.Right(3) + str ;
			strT.Delete(strT.GetLength() - 3, 3) ;
		}
		else	{
			str = strT + str ;
			strT.Empty();
		}
	}
	if(puls)
		return str + strDecimal;
	return '-' + str + strDecimal;
}

CString CDataConversion::GetStringFormatTrans(const CString& strData, const CString& strFormat, 
											  const CString& strCompartDate, const CString& strCompartTime)
{
	CString szFormat = strFormat, szData = "", szCompartDate = strCompartDate, szCompartTime = strCompartTime ;

	if(strCompartDate.IsEmpty())
		szCompartDate = "/" ;
	if(strCompartTime.IsEmpty())
		szCompartTime = ":" ;

	szFormat.TrimLeft();
	szFormat.TrimRight();

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//CString strValue = IntToString(atoi(strData), szFormat.GetLength());
	CString strValue = IntToString(_atoi64(strData), szFormat.GetLength());
	// 2011.01.06 by SYS <<

	//sy 2004.04.16
	CString strSpecialTime;
	if(GetSpecialTime(strFormat, strData, strSpecialTime))
		return strSpecialTime;
	//end

	GetStringFormatbyCompart(strValue, szFormat, szCompartDate, szCompartTime, szData);

	return szData;
}

CString CDataConversion::GetStringFormatbyCompart(const CString& strData, CString& strFormat, 
												  const CString& strCompartDate, const CString& strCompartTime, 
												  CString& strResult)
{
	if(strData.IsEmpty())
		return "" ;
	CString szType = strFormat.Left(1);

	int nFcount = 0;
	int length = szType.GetLength();
	int nMax = strFormat.GetLength() - length;
	for(int i = 0; i <= nMax; i++){
		if(strFormat.Mid(i, length) == szType)
			nFcount++;
		else
			break;
	}

	if(strFormat.GetLength() == nFcount)	{
		strResult = strResult + strData ;
		return "" ;
	}

	//sy 2004.04.16
	CString strSpecialTime;
	if(GetSpecialTime(strFormat, strData, strSpecialTime))
	{
		strResult = strSpecialTime;
		return strResult;
	}
	//end

	CString strRemainder = strData.Right(strData.GetLength() - nFcount);
	strResult = strResult + strData.Left(nFcount) + GetCompartType(strFormat, strCompartDate, strCompartTime) ;	

	strFormat = strFormat.Right(strFormat.GetLength() - nFcount);
	
	return GetStringFormatbyCompart(strRemainder, strFormat, strCompartDate, strCompartTime, strResult);
}

CString CDataConversion::GetCompartType(const CString &strFormat, const CString &strCompartDate, const CString &strCompartTime)
{
	CString szcurType = strFormat.Left(1);
	CString sznextType = "";
	if(strFormat.GetLength() > 3)
		sznextType = strFormat.Mid(3, 1);

	if((szcurType == "Y" || szcurType == "M" || szcurType == "D") && !(sznextType == "S"))
		return strCompartDate;
	else
		return strCompartTime;
}

// Draw ---------------------------------------------------------------------------------
// 그릴 영역 정한다.
void CDataConversion::SelectClipRegion(CDC* pDC, const CRect& rect)
{
	CRgn rgn;
	rgn.CreateRectRgn((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom+1);
	pDC->SelectClipRgn(&rgn);
}

// 그리는 영역기본값으로 돌린다.
void CDataConversion::SelectClipRegion(CDC* pDC)
{
	pDC->SelectClipRgn(NULL);
}

// data 추출 (point 형) -----------------------------------------------------------------
// 상대좌표 --> 절대좌표
// 값에 의해 화면의 point로 변환된 값을 가져온다.
CPoint CDataConversion::GetVerticalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter)
{
	if( max == -HUGE_VAL || min == -HUGE_VAL) return blockRegion.TopLeft();

	int x = GetChangingPosition(nIndex, blockRegion.left, realWidth, bCenter);
	if(x < 0)
		x = blockRegion.left;

	int y = GetChangingPosition(blockRegion.bottom, blockRegion.Height(), min, max-min, nowData);
	//if(y < 0)
	//	y = blockRegion.top;
	
	return CPoint(x, y);
}

CPoint CDataConversion::GetVerticalPoint(const CRect& blockRegion, const double& yMin, const double& yMax, const double& xMin, const double& xMax, const double& nowYData, const double& nowXData)
{
	int x = GetChangingPosition(blockRegion.right, blockRegion.Width(), xMin, xMax-xMin, nowXData);
	if(x < 0)
		x = blockRegion.left;
	else
		x = blockRegion.right - x + blockRegion.left; 

	int y = GetChangingPosition(blockRegion.bottom, blockRegion.Height(), yMin, yMax-yMin, nowYData);
	//if(y < 0)
	//	y = blockRegion.top;

	return CPoint(x, y);
}

CPoint CDataConversion::GetVerticalPointBox(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData, const bool bCenter)
{
	int x = GetChangingPosition(nIndex, blockRegion.left, realWidth, bCenter);
	int y = GetChangingPosition(blockRegion.bottom, blockRegion.Height(), min, max-min, nowData);
	//if(y < 0)
	//	y = blockRegion.top;
	
	return CPoint(x, y);
}

// 값에 의해 화면의 point로 변환된 값을 가져온다.
CPoint CDataConversion::GetHorizontalPoint(const CRect& blockRegion, const int nIndex, const double& realWidth, const double& min, const double& max, const double& nowData)
{
	int x = GetChangingPosition(blockRegion.right, blockRegion.Width(), min, max-min, nowData);
	if(x < 0){
		if(max < 0 && min < 0)
			x = blockRegion.right;
		else
			x = blockRegion.left;
	}
	else
		x = blockRegion.right - x + blockRegion.left; 

	int y = GetChangingPosition(nIndex, blockRegion.top, realWidth, true);
	if(y < 0)
		y = blockRegion.top;

	return CPoint(x, y);
}

int CDataConversion::GetHorizontalPoint_X(const CRect& blockRegion, const double& min, const double& max, const double& nowData)
{
	int x = GetChangingPosition(blockRegion.right, blockRegion.Width(), min, max-min, nowData);
	if(x < 0){
		if(max < 0 && min < 0)
			x = blockRegion.right;
		else
			x = blockRegion.left;
	}
	else
		x = blockRegion.right - x + blockRegion.left; 

	return x;
}

// 절대좌표 --> 상대좌표
int CDataConversion::AbsoluteToRelative_X(const CRect& region, const int absolute_x, const int pageDataCount)
{
	if(pageDataCount <= 0 || absolute_x < region.left ||  absolute_x > region.right)
		return 0;

	return int((double)((absolute_x - region.left)*pageDataCount) / (double)region.Width());
}

int CDataConversion::AbsoluteToRelative_X(const CRect& region, const int absolute_x, const double& dataViewMin, const double& dataViewMax)
{
	if(absolute_x < region.left ||  absolute_x > region.right)
		return 0;
    
	double dData = ((double)region.right - absolute_x) * (dataViewMax - dataViewMin);
	dData = dData / (double)region.Width();
	return CMath::Round(dataViewMax - dData);	
}

double CDataConversion::AbsoluteToRelative_Y(const CRect& region, const int absolute_y, const double& dataViewMin, const double& dataViewMax)
{
	double unitHeight = (dataViewMax - dataViewMin) / (double)region.Height();
	if(absolute_y < region.top)
		return dataViewMax + ((double)(region.top - absolute_y) * unitHeight);
	else if(absolute_y > region.bottom)
		return dataViewMin - ((double)(absolute_y - region.bottom) * unitHeight);

	double dData = (double)(region.bottom - absolute_y) * (dataViewMax - dataViewMin) / (double)region.Height();
	return (dataViewMin + dData);
}

// 상대좌표 --> 절대좌표
int CDataConversion::RelativeToAbsolute_Y(const CRect& region, const double& relative_y, const double& dataViewMin, const double& dataViewMax)
{
	if( dataViewMax == dataViewMin) return region.bottom;
	double dData = (relative_y - dataViewMin) * (double)region.Height() / (dataViewMax - dataViewMin);
	return (region.bottom - CMath::Round(dData));
}

// ----------------------------------------------------------------------------
CPoint CDataConversion::RelativeToAbsolute(const CRect& region, const int startIndex, const int endIndex, 
			const double& dataViewMin, const double& dataViewMax, 
			const int currentIndex, const double& currentPrice)
{
	double realWidth = GetRealWidth((double)region.Width(), (endIndex - startIndex +1));

	// 바꿀 상대좌표가 그리는 start/end 에 속할 경우
	if(currentIndex >= startIndex  &&  currentIndex <= endIndex)
		return RelativeToAbsoluteInDisplayIndex(region, realWidth, dataViewMin, dataViewMax, (currentIndex - startIndex), currentPrice);

	// 바꿀 상대좌표가 그리는 범위(start/end)에 벗어날 경우
	double realHeight = (double)region.Height()/(dataViewMax - dataViewMin);
	return RelativeToAbsoluteOutDisplayIndex(region, realWidth, realHeight, 
		dataViewMin, dataViewMax, startIndex, endIndex, currentIndex, currentPrice);
}

// x 좌표
int CDataConversion::RelativeToAbsolute_X(const CRect& region, const int startIndex, const int endIndex, const int currentIndex, const bool bCenter)
{
	double realWidth = GetRealWidth((double)region.Width(), (endIndex - startIndex +1));

	// 바꿀 상대좌표가 그리는 start/end 에 속할 경우
	if(currentIndex >= startIndex && currentIndex <= endIndex)
		return RelativeToAbsoluteInDisplayIndex_X(region, realWidth, (currentIndex - startIndex), bCenter);

	// 바꿀 상대좌표가 그리는 범위(start/end)에 벗어날 경우
	return RelativeToAbsoluteOutDisplayIndex_X(region, realWidth, startIndex, endIndex, currentIndex);
}

// --------------------------------------------------------------------------------------
// 해당 point가 있는 data의 index --> 자료일자인 경우
int CDataConversion::GetPacketDataIndex_Date(const CRect& region, const CPoint& point, const int dataStart, const int dataEnd)
{
	// (2004.10.11, 배승원) Region Width가 0인 경우를 Devide Zero로 예외처리한다.
	if(dataEnd < dataStart || dataStart < 0 || region.Width() == 0)
		return -1;

	int index = int((double)((point.x - region.left)*(dataEnd - dataStart +1)) / (double)region.Width());
	return dataStart + index;
}

// 날짜 하나의 x간격
double CDataConversion::GetRealWidth(const double& dWidth, const int nIndex)
{
	if(nIndex <= 0)
		return 0.0;

	return (dWidth / (double)nIndex);
}

// --------------------------------------------------------------------------------------
//invert graph을 위해서 dataPt를 invert 한다.
int CDataConversion::InvertGraphDataPt(const CRect& region, const int dataPt_y)
{
	int halfHeight  = region.Height() / 2;

	if(dataPt_y > region.top + halfHeight)
		return region.top + (region.bottom - dataPt_y);
	else if(dataPt_y < region.top + halfHeight)
		return region.bottom - (dataPt_y - region.top);
	else 
		return dataPt_y;

	return dataPt_y;
}

// private ==============================================================================
// pluse 인지를 체크한다.
bool CDataConversion::IsPluse(const CString& data)
{
	return (data.Left(1) != '-');
}

// --------------------------------------------------------------------------------------
// GetVerticalPoint(), GetHonrizontalPoint() 의 내부함수들
int CDataConversion::GetChangingPosition(const int startPos, const int length, const double& originStartData, const double& originLength, const double& nowData)
{
	if(length <= 0 || originLength == 0)
		return -1;

	int ChangingLength = CMath::Round(length * ((nowData - originStartData) / originLength));
	return (startPos - ChangingLength);
}

int CDataConversion::GetChangingPosition(const int nowIndex, const int startPos, const double& width, const bool bCenter)
{
	if(nowIndex < 0)
		return -1;

	int nNowPos = CMath::Round(width * nowIndex);
	if(bCenter){
		int nNextPos = CMath::Round(width * (nowIndex +1));
		// line 은 오른쪽으로 그리기 때문에 "버림". gap이 3, 5 같은 경우 대비.
		int nHalf = (nNextPos - nNowPos) / 2;
		nNowPos += nHalf;
	}

	return (startPos + nNowPos);
}

// --------------------------------------------------------------------------------------
// 상대좌표 -> 절대좌표 : 상대좌표가 해당 범위에 있을 경우
CPoint CDataConversion::RelativeToAbsoluteInDisplayIndex(const CRect& region, const double& realWidth, 
		const double& dataViewMin, const double& dataViewMax, const int currentIndex, const double& currentPrice)
{
	int	x = RelativeToAbsoluteInDisplayIndex_X(region, realWidth, currentIndex);
	int y = RelativeToAbsoluteInDisplayIndex_Y(region, dataViewMin, dataViewMax, currentPrice);
	
	return CPoint(x, y);
}

// 상대좌표 -> 절대좌표 : 상대좌표가 해당 범위에 밖에 있을 경우
CPoint CDataConversion::RelativeToAbsoluteOutDisplayIndex(const CRect& region, 
		const double& realWidth, const double& realHeight, const double& dataViewMin, const double& dataViewMax, 
		const int startIndex, const int endIndex, const int currentIndex, const double& currentPrice)
{
	int x = RelativeToAbsoluteOutDisplayIndex_X(region, realWidth, startIndex, endIndex, currentIndex);
	int y = RelativeToAbsoluteOutDisplayIndex_Y(region, realHeight, dataViewMin, dataViewMax, currentPrice);

	return CPoint(x, y);
}

// --------------------------------------------------------------------------------------
// 상대좌표 -> 절대좌표 : 상대좌표가 해당 범위에 있을 경우
int CDataConversion::RelativeToAbsoluteInDisplayIndex_X(const CRect& region, const double& realWidth, const int currentIndex, const bool bCenter)
{
	int	x = GetChangingPosition(currentIndex, region.left, realWidth, bCenter);
	if(x == -1)
		return region.left;

	return x;
}

// 상대좌표 -> 절대좌표 : 상대좌표가 해당 범위에 밖에 있을 경우
int CDataConversion::RelativeToAbsoluteOutDisplayIndex_X(const CRect& region, const double& realWidth, 
		const int startIndex, const int endIndex, const int currentIndex)
{
	if(currentIndex < startIndex)
		return region.left - CMath::Round(double(startIndex - currentIndex)*realWidth);
	else if(currentIndex > endIndex)
		return region.right + CMath::Round(double(currentIndex - endIndex)*realWidth);

	return 0;
}

// --------------------------------------------------------------------------------------
int CDataConversion::RelativeToAbsoluteInDisplayIndex_Y(const CRect& region, const double& dataViewMin, const double& dataViewMax, const double& currentPrice)
{
	return GetChangingPosition(region.bottom, region.Height(), dataViewMin, dataViewMax - dataViewMin, currentPrice);
	//if(y < 0)
	//	return region.top;
}

int CDataConversion::RelativeToAbsoluteOutDisplayIndex_Y(const CRect& region, 
		const double& realHeight, const double& dataViewMin, const double& dataViewMax, const double& currentPrice)
{
	if(dataViewMax < currentPrice)
		return region.top - CMath::Round((currentPrice - dataViewMax)*realHeight);
	else if(dataViewMax > currentPrice)
		return region.bottom + CMath::Round((dataViewMin - currentPrice)*realHeight);
	
	return GetChangingPosition(region.bottom, region.Height(), dataViewMin, dataViewMax - dataViewMin, currentPrice);
}

// --------------------------------------------------------------------------------------
//sy 2002.7.30. -> 미래 시간 구하기
//sy 2004.08.18
/*bool CDataConversion::OnPickoutTime(const int nCharge, const CString& strFormat, const int nInterval, int& mon, int& day, int& hour, int& min, int& sec)
{
	mon = day = hour = min = sec = 0;

	if(nCharge <= 0)
		return false;
	
	// 일자 -> 년, 일, 월 만 존재.
	if(strFormat.Right(1) == 'D' || strFormat.Find('Y') >= 0)
		return false;

	// 시간
	if(strFormat.Right(1) == "S") {// 초
		int nMin = 0;
		//분데이타
		if(nInterval >= 100) {
			nMin = (nInterval / 100);
			sec = 60 * nCharge * nMin;
		}
		else
			sec = nInterval * nCharge;
	}
	else if(strFormat.Right(1) == "M") {// 분
		int nhour = 0;
		//시간데이타
		if(nInterval >= 100) {
			nhour = (nInterval / 100);
			min = 60 * nCharge * nhour;
		}
		else
			min = nCharge * nInterval;
	}
	else if(strFormat.Right(1) == "H") // 시
		hour = nCharge * nInterval;
	
	while(sec >= 60){
		min++;
		sec -= 60;
	}
	while(min >= 60){
		hour++;
		min -= 60;
	}
	while(hour >= 7){
		day++;
		hour -= 7;
	}
	while(day >= 30){
		mon++;
		day -= 30;
	}

	return (sec != 0 || min != 0 || hour != 0 || day != 0 || mon != 0);
}*/

bool CDataConversion::OnPickoutTime(const int nCurData, const int nCharge, const CString& strFormat, const int nInterval, int& mon, int& day, int& hour, int& min, int& sec)
{
	mon = day = hour = min = sec = 0;

	if(nCharge <= 0)
		return false;
	
	// 일자 -> 년, 일, 월 만 존재.
	if(strFormat.Right(1) == 'D' || strFormat.Find('Y') >= 0)
		return false;

	// 시간
	if(strFormat.Right(1) == "S") {// 초
		int nMin = 0;
		//분데이타
		if(nInterval >= 100) {
			nMin = (nInterval / 100);
			sec = 60 * nCharge * nMin;
		}
		else
			sec = nInterval * nCharge;
	}
	else if(strFormat.Right(1) == "M") {// 분
		int nhour = 0;
		//시간데이타
		if(nInterval >= 100) {
			nhour = (nInterval / 100);
			min = 60 * nCharge * nhour;
		}
		else
			min = nCharge * nInterval;
	}
	else if(strFormat.Right(1) == "H") // 시
		hour = nCharge * nInterval;
	
	while(sec >= 60){
		min++;
		sec -= 60;
	}
	while(min >= 60){
		hour++;
		min -= 60;
	}
	while(hour >= 7){
		day++;
		hour -= 7;
	}
	while(day >= 30){
		mon++;
		day -= 30;
	}

	return (sec != 0 || min != 0 || hour != 0 || day != 0 || mon != 0);
}

bool CDataConversion::OnPickoutDate(const int nValue, const CString& strFormat, const int nType, int& year, int& mon, int& day)
{
	switch(nType){
	case 1:	//일간
		return OnPickoutDaily(nValue, strFormat, year, mon, day);
	case 2:	//주간
		return OnPickoutWeekley(nValue, strFormat, year, mon, day);
	case 3:	//월간
		return OnPickoutMonthly(nValue, strFormat, year, mon, day);
	}

	return false;
}

bool CDataConversion::OnPickoutDaily(const int nValue, const CString& strFormat, int& year, int& mon, int& day)
{
	CString strValue = IntToString(nValue, strFormat.GetLength());
	if(strFormat.Right(1) == 'D')
		day = nValue % 30;
	else
		day = 0;

	if(strFormat.Find('M') >= 0)
		mon = nValue / 30;
	else
		mon = 0;

	if(strFormat.Find('Y') >= 0){
		year = mon / 12;
		mon = mon % 12;
	}
	else
		year = 0;

	return (day != 0 || mon != 0 || year != 0);
}

bool CDataConversion::OnPickoutWeekley(const int nValue, const CString& strFormat, int& year, int& mon, int& day)
{
	if(strFormat.Right(1) == 'D')
		day = (nValue * 7) % 30;
	else
		day = 0;

	if(strFormat.Find('M') >= 0)
		mon = (nValue * 7) / 30;
	else
		mon = 0;

	if(strFormat.Find('Y') >= 0){
		year = mon / 12;
		mon = mon % 12;
	}
	else
		year = 0;

	return (day != 0 || mon != 0 || year != 0);
}

bool CDataConversion::OnPickoutMonthly(const int nValue, const CString& strFormat, int& year, int& mon, int& day)
{
	day = 0;

	if(strFormat.Find('M') >= 0)
		mon = nValue % 12;
	else
		mon = 0;

	if(strFormat.Find('Y') >= 0)
		year = nValue / 12;
	else
		year = 0;

	return (mon != 0 || year != 0);
}
//end sy

//sy 2004.04.16.
bool CDataConversion::GetSpecialTime(const CString& strFormat, const CString& strData, CString& strSpecialTime)
{
	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	if( CChartInfo::WORLD_ON == m_eChartMode) return false;

	int nCodeIndex = strFormat.Find( "HHMM");
	if(strFormat.GetLength() < 4 || nCodeIndex == -1 || strData.GetLength() < nCodeIndex + 4) return false;

	CString strCode = strData.Mid( nCodeIndex, 4);
	
	if( strCode == "6666")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_OPENING_TRADE);
		return true;
	}
	
	if( strCode == "7777")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_CLOSING_TRADE);
		return true;
	}

	// (2006/11/19 - Seung-Won, Bae) Support samsung code. 장마감 009991, 시간외마감 009999
	if( strCode == "9999")
	{
		strSpecialTime = _LTEXT6( C6_OVERTIME_TRADE);
		return true;
	}

	// (2006/12/29 - Seung-Won, Bae) Support koscom code. 장마감 310000
	// (2008/11/19 - Seung-Won, Bae) Do not support "3100" and "4100" for Woori.
	if( strCode == "8888" || strCode == "0099")	//  || strCode == "3100")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_TRADING);
		return true;
	}

	// 시간외마감 추가 - ojtaso (20071130)
	if( strCode == "4100")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_OVERTIME_TRADE);
		return true;
	}

	// (2009/3/19 - Seung-Won, Bae) for After Closing.
	if( strCode == "7100")
	{
		strSpecialTime = _LTEXT6( C6_AFTER_CLOSING);
		return true;
	}
	if( strCode == "8100")
	{
		strSpecialTime = _LTEXT6( C6_END_OF_SINGLE_PRICE);
		return true;
	}

	return false;
}
//sy end

// (2004.10.22, 배승원) 주어진 색상의 반대 명암(명암 50%차)인 보색을 구한다.
//	[기본색상 원리]
//		1. 색은 원색상에서 채도변화후 다시 명도변화를 거쳐 나타난다.
//			따라서, 역으로 주어진 색의 명도변화이전의 색상을 구하고,
//			다시 채도변화 이전의 색상을 구하여 원색상을 구한다.
//		2. HSI 표기
//			원색과 그 가공 방식에 대한 Factor 값이다.
//			Hue(색상), Saturation(채도), Intensity(명암)
//			각 값은 RGB 값을 선택하기 위한 비중치들로 Maximum 값을 기준으로하는 비율로 이해하여야 한다.
//			여기서는 Windows의 색상 Dialog과 같이하여, 이해를 쉽게하기 위하여 240을 최대로 하여 설명한다.
//			(Windows 색상 Dialog의 경우, 정밀한 RGB값을 선택하기 위하여 240을 선택하였다.
//				그것은 RGB 값이 최대 255로 유사한 정밀도이면서
//				색상과 채도 기준인 6분할과 2분할이 가능한 6의 배수로 선정된 것이다.)
//		3. 원색
//			H 값(Hue, 색상)에 의해 결정되며, 그값은 다음과 같이 구분지을 수 있다. (6분할)
//				0		R(255,0,0)
//				20			(255,127,0)		R Max, G 값 변화구간, B Min
//				40		RG(255,255,0)
//				60			(127,255,0)		R 값 변화구간, G Max, B Min
//				80		G(0,255,0)
//				100			(0,255,127)		R Min, G Max, B 값 변화구간
//				120		GB(0,255,255)
//				140			(0,127,255)		R Min, G 값 변화구간. B Max
//				160		B(0,0,255)
//				180			(127,0,255)		R 값 변화구간, G Min, B Max
//				200		RB(255,0,255)
//				220			(255,0,127)		R Max, G Min, B 값 변화구간
//				240		R(255,0,0)
//			원색의 특징으로는 항상 RGB중 하나는 0, 하나는 255인 색갈이다.
//				Max(RGB) = 255, Min(RGB) = 0
//		4. 채도변화
//			S 값(Saturation, 채도)에 의해 결정되며, 채도에 의해 원색은 다음과 같이 변형된다.
//				RGB = ( RGB - 127.5) * H / 240 + 127.5
//			즉, 각 색상은 H 비중만큼 127.5 (RGB 범위의 중심값)으로 몰리게된다.
//			특히 240 (Max)이면 원색을 유지하고, 0 (Min)이면 완전 무채색(회색)으로 변형되게 된다.
//			채도변화된 색상의 특징은 RGB 값중 0이나 255의 유무를 보장할 수는 없으나,
//				RGB 값중 가장 큰 값과 작은 값의 평균이 127.5 (무채색)가 된다.
//				그것은 원색의 특징인 가장 큰 값이 255, 작은 값은 0이며,
//				같은 비중(채도)으로 127.5에 몰리게 되기 때문에 이 둘의 평균값이 127.5로 유지된다.
//				이 값을 채도중심이라고 하자. (변화복원 설명에 인용)
//					( Max(RGB) + Min(RGB)) / 2 = 127.5
//		5. 명암변화
//			I 값 (Instensity, 명암)에 의해 결정되며, 120을 기준으로 240(+120) 밝게 (흰색)
//				0(-120) 어둡게(검은색)으로 비중변화를 주는 것이다.
//			따라서, 밝게 변화시킬 경우 (명암, I값 120초과 설정시)
//				RGB = 255 - ( 255 - RGB) * ( 240 - I) / 120
//			어둡게 변화시킬 경우 (명암, I값 120미만 설정시)
//				RGB = RGB * I / 120
//			이렇게 변환되어 나타나는 색은 특별한 제한 없이 모든 색상의 RGB 값을 도출하게 된다.
//		6. 원색찾기
//			임의의 색을 명암에 변화를 주거나, 채도, 또는 색상변화(일명 RGB Rotate)을 주는 처리를 위해서는
//				임의의 색의 원색을 다시 산출하여 계산하게 된다.
//			역시 원색을 산출하는 것은 색상변형의 반대 방법으로, 먼저 명암변화복원, 채도변화복원을 통해 이뤄진다.
//		7. 명암변화복원
//			명암변화복원의 기본원리는 복원후 채도중심이 127.5가 나와야 한다는 것이다.
//			또한 명암변화를 통해서도 채도중심은 변화된 색상 RGB값중 가장 큰 값과 작은 값의 평균위치로
//				변화된다는 것이다. 이를 통하여 명암 변화값(즉 I)을 산출할 수 있다.
//			먼저 명암 변화 설정값 I는 다음과 같이 구하여진다.
//				I = ( Max(RGB) + Min(RGB)) / 2 * 240 / 255
//			이렇게 구해진 I값이 120보다 크면 밝게 변환된 것이고, 작으면 어둠게 변환된 것이다.
//				각각의 경우 변환 공식이 틀리기 때문에, 역시 변화 복원도 구분하여 처리한다.
//			I = 120 인 경우
//				RGB = RGB
//			I < 120 인 경우
//				RGB = RGB / I * 120
//			I > 120 인 경우 
//				RGB = 255 - ( 255 - RGB) * 120 / ( 240 - I)
//		8. 채도변화복원
//			채도 변화의 복원의 기본 원리는 변화 복원후 Max(RGB)는 255가 Min(RGB)는 0이 나와야 한다는 것이다.
//			따라서, 명암복원된 색상의 Max(RGB)값이 255가 되게 해주는 H를 구하면,
//				Min(RGB)값도 0으로 되게 해주는 H가 되어, 채도 변화 설정값을 확인하게 된다.
//			즉, 채도변화 설정값 H는 다음과 같이 구하여진다.
//				H = ( Max(RGB) - 127.5) * 240 / ( 255 - 127.5)
//			이렇게 구해진 H값을 이용하여 채도변화이전의 원색을 구한다.
//				RGB = ( RGB - 127.5) * 240 / H + 127.5
//		9. 보색
//			보색은 원색의 색상을 나타내는 Hue 값의 반대 색상(120 차)을 말한다.
//				따라서 보색은 원색의 Hue를 120차, 즉 (H+120)%240의 Hue를 구하여 산출한다.
//				그러나, Hue의 120차의 색상은 색상의 배열 관계상, RGB 색상일 경우 (255-R,255-G,255-B)가 그 보색에 해당한다.
//			이 경우, 채도가 0인 무채색의 경우 그 보색 역시 무채색으로, 만약 명도차가 업다면 같은 색이 된다.
//				즉, (255-R,255-G,255-B)은 원색의 명도를 명도중심 120을 중심으로 대칭인 명도를 갖게되지만,
//				이 색상의 명도가 120이고, 무채색의 경우 원색과 보색 모두 같은 회색(127,127,127)를 갖게된다.
//		10. 명도보색
//			보색 관계가 서로 눈에 띄는 관계가 원색인 경우를 전제한 것을 감안하여,
//				원색이 아닌 경우에도 서로 명도차를 두어 색이 눈에 띄게 보정한 대응 색상을 산출하자.
//			보색이면서 명도가 120차가 나게 선택하면되는데,
//				이를 위해서는 주어진 색상의 기준 명도 색상을 구한뒤,
//				기준 명도 색상의 보색을 구하고, 다시 이 색의 반대 명도 색상을 구하게 된다.
COLORREF CDataConversion::GetComplementColor( COLORREF p_crOrg)
{
	// 1. 기본 색상을 분리한다.
	BYTE btR = GetRValue( p_crOrg);
	BYTE btG = GetGValue( p_crOrg);
	BYTE btB = GetBValue( p_crOrg);
	// 2. 명암 설정값을 산출한다.
	// 2.1 RGB의 Min/Max값을 구한다.
	BYTE btMin = ( btR < btG) ? ( btB < btR ? btB : btR) : ( btB < btG ? btB : btG);
	BYTE btMax = ( btR < btG) ? ( btB < btG ? btG : btB) : ( btB < btR ? btR : btB);
	// 2.2 명암 설정값 I를 산출한다.
	double dI = ( btMax + btMin) / 2. * 240. / 255.;
	// 3. 명암 복원색을 찾는다.
	if( dI < 120)
	{
		btR = BYTE( btR / dI * 120.);
		btG = BYTE( btG / dI * 120.);
		btB = BYTE( btB / dI * 120.);
	}
	else
	{
		btR = BYTE( 255. - ( 255. - btR) * 120. / (240. - dI));
		btG = BYTE( 255. - ( 255. - btG) * 120. / (240. - dI));
		btB = BYTE( 255. - ( 255. - btB) * 120. / (240. - dI));
	}
	// 4. 보색을 구한다.
	btR = BYTE( 255 - btR);
	btR = BYTE( 255 - btG);
	btR = BYTE( 255 - btB);
	// 5. 반대 명암 설정값을 구한다. (명암 50%차)
	if( dI <= 120) dI += 120;
	else dI -= 120;
	// 6. 명암복원한 색을 반대명암설정으로 다시 명암적용한다.
	if( dI < 120)
	{
		btR = BYTE( btR * dI / 120.);
		btG = BYTE( btG * dI / 120.);
		btB = BYTE( btB * dI / 120.);
	}
	else
	{
		btR = BYTE( 255. - ( 255. - btR) * (240. - dI) / 120.);
		btG = BYTE( 255. - ( 255. - btG) * (240. - dI) / 120.);
		btB = BYTE( 255. - ( 255. - btB) * (240. - dI) / 120.);
	}
	// 7. 구해진 색상을 반환한다.
	return RGB( btR, btG, btB);
}


// 20080910 JS.Kim		
//	미국채채권 표시를 위해 8진법/32진법으로의 변환 <<

// Type 이 진법처리인지 여부를 구한다
bool CDataConversion::IsBaseFormat(const CString& dataType)
{
	if( dataType.Find("1/") >= 0 )
		return true;
	return false;
}

// 진법 변환 정보를 얻는다
// return : 진법변환이면 true, 아니면 false
// nBase : 진수
// nDecLen1 : 소숫점 앞 자리수
// nDecLen2 : 수숫점 뒤 자리수
bool CDataConversion::GetBaseFormatInfo(const CString& dataType, int &nBase, int &nDecLen1, int &nDecLen2)
{
	if( !IsBaseFormat( dataType ) )
		return false;

	CString format(dataType);
	char *szBuf = format.GetBuffer(dataType.GetLength()+1);	
	char* p;

	szBuf = strstr(szBuf, "1/");
	// 진법 Base 구하기
	p = strchr(szBuf, '\'');
	if( p )		*p = 0x0;
	nBase = atoi(szBuf+2);
	szBuf = p + 1;

	p = strchr(szBuf, '.');
	if( p )		*p = 0x0;
	nDecLen1 = atoi(szBuf);
	szBuf = p + 1;

	nDecLen2 = atoi(szBuf);

	nDecLen1 -= nDecLen2;

	format.ReleaseBuffer();

	return true;
}
// 수숫점 몇자리(decimalDownCount - 소수부분)까지만 바꾼다.
// ex) Num = 1234.5678   decimalDownCount = 2 -> return : "1234.57" (반올림)
//                       decimalDownCount = 6 -> return : "1234.567800"
//                       decimalDownCount = -2 -> return : "12" (반올림)
CString CDataConversion::DoubleToString(const double& Num, const CString& dataType)
{
	int nBase, nDecLen1, nDecLen2;
	if( GetBaseFormatInfo(dataType, nBase, nDecLen1, nDecLen2) )
	{
		return Gl_Format32String(Num, nBase, nDecLen1+nDecLen2, nDecLen2);
	}
	int nDecimal = GetDecimalDownCount(dataType, true);
	return CDataConversion::DoubleToString(Num, nDecimal);
}

//	우리선물에서 사용하는 소리마치 로직 가져옴 
//*******************************************************************/
/*! Function Name : Format32String
/*! Function      : 진법정보를 이용하여 포맷을 변경해 Return해준다. 맨 마지막의 마스킹은 
/*!					진법데이터의 크기가 Long값을 넘기때문에 마스킹으로 처리하는 로직을 따로 추가했음
/*! Param         : dllexport) CString Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask
/*! Return        : 
/*! Create        : 강지원 , 2006/11/16
/*!	Revision      : 이상신 , 2010/03/17		코드 정리.
/*! Comment       : 
//******************************************************************/
CString CDataConversion::Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask)
{
	int nMask = uMask;
	if (nMask == 0)
		nMask = 1;

	double dTBValue = (double)nTBValue/(double)nMask;

	return Gl_Format32String(dTBValue, scale, outdec, indec);
}

//*******************************************************************/
/*!	Revision      : 이상신 , 2010/03/17		코드 정리.				*/
//*******************************************************************/
CString CDataConversion::Gl_Format32String(double dTBValue, int scale, int outdec, int indec)
{
	CString strValue = "", strResult = "";

	if (scale < 0 || outdec < 0 || indec < 0)
	{
		strValue.Format("%d", (long)dTBValue);
		return strValue;
	}

	BOOL bFlag = FALSE;
	if (dTBValue < 0)
	{
		dTBValue *= -1.0;
		bFlag = TRUE;
	}

	if (scale > 1)
	{
		int nInt = (int)floor(dTBValue);
		double dValue = dTBValue - (double)(nInt);
		dValue = dValue * scale;
		if( indec > 0 )
			strValue.Format("%d'%0*.*f", nInt, outdec+1, indec, dValue);
		else
			strValue.Format("%d'%0*.0f", nInt, outdec, dValue);
	}
	else
		return "0";

	if (bFlag)
		strResult.Format("-%s", strValue);
	else
		strResult.Format("%s", strValue);

	return strResult;
}

//20081010 이문수 8, 32진법표기 => 10진법으로 변환>>
BOOL CDataConversion::GetDataToDec(CString strInput, CString strPacketType, CString & strOutput )
{
	strInput.Remove(',');	
	strInput.TrimLeft(); 
	strInput.TrimRight();
	strInput.TrimLeft('+');

	CString strTemp, strInt;

	//현재 타입이 진법입력이어야 하는지 여부 판단
	if (!IsBaseFormat(strPacketType)) 
	{
		strOutput = strInput;//10진법으로 판단하고 입력값 그대로 출력
		return TRUE;
	}

	int nBase, nDecLen1, nDecLen2;
	GetBaseFormatInfo(strPacketType, nBase, nDecLen1, nDecLen2);	
	if (nBase <= 0 || nDecLen1 < 0 || nDecLen2 < 0) 
		return FALSE;

	int nPos2, nPos = strInput.Find('\'');
	if (nPos < 0)
	{
		if ((nPos = strInput.Find('.')) >= 0)	//' 없고 .만 있는 경우
			return FALSE;
		else									//' 없고 .도 없는 경우
		{
			strTemp.Format("%d", atoi(strInput));

			if (strTemp == strInput)			// 숫자가 아닌 다른 값이 들어있는지 여부 판단.
			{
				strOutput = strInput;
				return TRUE;
			}
			else
				return FALSE;
		}
	}

	///////////////////////////////////
	//' 와 . 역전입력 찾기 EX) 20.10'10 => 오류
	nPos2 = strInput.Find('.');
	if (nPos2 >= 0 && nPos > nPos2)
		return FALSE;

	///////////////////////////////////
	//정수부, 중간부, 소수부로 분리
	strInt = strInput.Left(nPos);
	strInput = strInput.Mid(nPos + 1);

	double dInt = atof(strInt);
	double dFloat = atof(strInput)/(double)nBase;

	BOOL bSign = FALSE;
	if (dInt < 0)
		bSign = TRUE;

	dInt = fabs(dInt);
	long double dOutput =  (bSign) ? - (dInt + dFloat) : dInt + dFloat;

	strOutput.Format("%f", dOutput);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//8진법,32진법 입력값이 유효한지 체크만 하는 함수
//주의:
//8진법,32진법=>10진법으로 변환할 때는 GetDataToDec함수를 사용할 것.
//(GetDataToDec 함수 내부에서 IsValid8_32함수의 하는 일을 모두 하고 있으므로 IsValid8_32를 호출하여 검사를 따로 해줄 필요없음)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDataConversion::IsValid8_32(CString strInput)
{
	strInput.Remove(',');	
	strInput.TrimLeft(); 
	strInput.TrimRight();
	strInput.TrimLeft('+');
	
	CString strTemp;
	
	int nPos1, nPos2;
	CString strInt, strMid, strPoint;	
	
	//////////////////////////////////////
	//'가 없는 경우 처리..
	int nPos = strInput.Find('\'');
	if(nPos < 0) 
	{
		//' 없고 .만 있는 경우 
		if((nPos = strInput.Find('.')) >= 0)
			return FALSE;
		else
		{	//'없고 .도 없는 경우 
			strTemp.Format("%d", atoi(strInput));
			
			if(strTemp == strInput)		//숫자가 아닌 다른 값이 들어있는지 여부 판단.
				return TRUE;
			else
				return FALSE;
		}
	}	
	
	///////////////////////////////////
	//' 와 . 역전입력 찾기 EX) 20.10'10 => 오류
	nPos1		=   nPos;
	nPos2		=	strInput.Find('.');
	if(nPos2 >= 0 && nPos1 > nPos2)
		return FALSE;
	
	
	///////////////////////////////////
	//정수부, 중간부, 소수부로 분리하여 오류체크.
	strInt		=	strInput.Left(nPos);
	strInput	=	strInput.Mid(nPos + 1);
	if((nPos = strInput.Find('.')) < 0) 
	{
		strMid	= strInput;
		strPoint= "";
	}
	else
	{		
		strMid	= strInput.Left(nPos);
		strPoint= strInput.Mid(nPos + 1);
		
		strMid.TrimLeft(); strMid.TrimRight();
		if(strMid.GetLength() <= 0) // EX) 20'.10 => 오류
			return FALSE;
	}
	
	/////////////////////////////////////////////////
	//각 부분별로 숫자가 아닌 다른 값이 들어있는지 여부 판단.
	BOOL bSign = FALSE;
	if(strInt.GetLength() > 0 && strInt[0] == '-') bSign = TRUE;
	
	if(strInt.GetLength() > 0)
	{
		CString strInteger = strInt;
		
		if(bSign && strInteger.GetLength() == 1) //정수부에 "-" 만 입력된 경우.
			strInteger = "0";//-부호를 제거한 후 처리.
		else
		{
			if(bSign) //-부호를 제거한 후 처리.
				strInteger = strInteger.Mid(1);

			//-부호를 제거하고 맨 앞에 있는 0값들을 제거.
			strInteger.TrimLeft('0');
			if(strInteger.GetLength() <= 0) //최소한 0값 하나는 남겨두어야 한다.
				strInteger = "0";
		}

		strTemp.Format("%d", atoi(strInteger));
		if(strTemp != strInteger)
			return FALSE;		
	}

	if(strMid.GetLength() > 0)
	{	
		strTemp.Format("%0*d", strMid.GetLength(), atoi(strMid));
		if(strTemp != strMid) return FALSE;
	}

	if(strPoint.GetLength() > 0)
	{	
		strTemp.Format("%0*d", strPoint.GetLength(), atoi(strPoint));
		if(strTemp != strPoint) return FALSE;
	}
	/////////////////////////////////////////////////
	
	return TRUE;
}
//20081010 이문수 <<




// 20081007 JS.Kim	사용자시간대 선택
double CDataConversion::CalcTimeDiff(double dData, int nTimeDiff, int nCurrTimeDiff, CString strType)
{
	if( nTimeDiff == 0 )		return dData;

	int nTime = 0, nPosition;
	if( (nPosition = strType.Find("HH")) < 0 )	// Packet에 시간 데이타가 없으므로 시차 계산할 필요 없다.
		return dData;

	int nBaseTimeDiff = nTimeDiff;

	time_t timeResult;
	tm tmResult;
	/*
	time_t timeCur;
	tm* ptmCur;
	time(&timeCur);
	ptmCur = localtime(&timeCur);
	*/
	//Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다<<
	tm* ptmCur = CalcCurrentDateTimToPacketTime(nCurrTimeDiff);

	double newTime = dData;
	__int64 lTime = (__int64)dData;

	if(strType == "MMDDHHMM")
	{
		if(lTime)
		{
			int nMon = (int)((lTime / 1000000)- 1);
			if(nMon > ptmCur->tm_mon)	ptmCur->tm_year--;
			else if( ptmCur->tm_mon >= 12 && nMon == 1 )	ptmCur->tm_mon++;

			ptmCur->tm_mon = nMon;
			ptmCur->tm_mday = (int)((lTime % 1000000) / 10000);
			ptmCur->tm_hour = (int)((lTime % 10000) / 100 + nBaseTimeDiff);
			ptmCur->tm_min	= (int)(lTime % 100);
		}
		ptmCur->tm_sec = 0;

		tmResult = *ptmCur;
		timeResult = mktime(&tmResult);

		newTime = (double)((tmResult.tm_mon+1)*1000000 + tmResult.tm_mday*10000 + tmResult.tm_hour*100 + tmResult.tm_min);
	}
	else if(strType == "HHMMSS")
	{
		if(lTime)
		{
			int nHour = (int)(lTime / 10000);
			if(nHour > ptmCur->tm_hour)	ptmCur->tm_mday--;

			ptmCur->tm_hour = nHour + nBaseTimeDiff;
			ptmCur->tm_min	= (int)(lTime % 10000) / 100;
			ptmCur->tm_sec	= (int)(lTime % 100);
		}

		tmResult = *ptmCur;
		timeResult = mktime(&tmResult);
		newTime = (double)(tmResult.tm_hour*10000 + tmResult.tm_min*100 + tmResult.tm_sec);
	} 
	else if (strType == "DDHHMMSS")
	{
		if(lTime)
		{
			int nDay = (int)(lTime / 1000000);
			if(nDay > ptmCur->tm_mday)						ptmCur->tm_mon--;
			else if( ptmCur->tm_mday >= 28 && nDay == 1 )	ptmCur->tm_mon++;

			ptmCur->tm_mday = nDay;
			ptmCur->tm_hour	= (int)(lTime % 1000000) / 10000 + nBaseTimeDiff;
			ptmCur->tm_min	= (int)(lTime % 10000) / 100;
			ptmCur->tm_sec	= (int)(lTime % 100);
		}

		tmResult = *ptmCur;
		timeResult = mktime(&tmResult);

		newTime = (double)(tmResult.tm_mday*1000000 + tmResult.tm_hour*10000 + tmResult.tm_min*100 + tmResult.tm_sec);
	} 

	return newTime;
}

#define		LOCAL_TIMEDIFF		9		// 한국 GMT +9

// Local PC의 현재시간을 데이타의 실제 시간대의 시간(거래소시간)으로 변경한다
// 년월일 데이타가 서버에서 내려오지 않는 경우 년월일 데이타를 PC의 시간으로 생성하기 위해 필요
tm *CDataConversion::CalcCurrentDateTimToPacketTime(int nTimeDiff)
{
	time_t timeCur;
	tm* ptmCur;
	time(&timeCur);
	ptmCur = localtime(&timeCur);		// Local Time

	ptmCur->tm_hour += (nTimeDiff - LOCAL_TIMEDIFF);
	mktime(ptmCur);
	return ptmCur;
}

// (2008/11/15 - Seung-Won, Bae) Conversion Time and Double
BOOL CDataConversion::ConvertTMtoDouble( tm &p_tmTime, double &p_dTime)
{
	p_dTime = p_tmTime.tm_year + 1900;
	p_dTime = p_dTime * 100 + p_tmTime.tm_mon + 1;
	p_dTime = p_dTime * 100 + p_tmTime.tm_mday;
	p_dTime = p_dTime * 100 + p_tmTime.tm_hour;
	p_dTime = p_dTime * 100 + p_tmTime.tm_min;
	p_dTime = p_dTime * 100 + p_tmTime.tm_sec;
	return TRUE;
}
BOOL CDataConversion::ConvertDoubletoTM( double &p_dTime, tm &p_tmTime)
{
/*
	__int64 lClock = (__int64)p_dTime;

	p_tmTime.tm_year = (lClock / 10000) - 1900;
	p_tmTime.tm_mon  = (lClock % 10000) / 100 - 1;
	p_tmTime.tm_mday = (lClock % 100);
	p_tmTime.tm_hour = (lClock % 100);
	p_tmTime.tm_min = (lClock % 100);
	p_tmTime.tm_sec = (lClock % 100);
*/

	double dClock = ( p_dTime + 0.1) / 100000000000000;
	double dValue = 0.0;
	dClock = modf( dClock * 10000, &dValue);
	p_tmTime.tm_year	= int( dValue - 1900);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_mon	= int( dValue - 1);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_mday	= int( dValue);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_hour	= int( dValue);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_min	= int( dValue);
	dClock = modf( dClock * 100, &dValue);
	p_tmTime.tm_sec	= int( dValue);

	return TRUE;
}

__int64 CDataConversion::GetPacketTypedTime( CString strPacketType, double p_dTime)
{
	CString strLeft2 = strPacketType.Left( 2);
	if( strLeft2 != "YY")
	{
		p_dTime -= int( p_dTime / 10000000000.) * 10000000000.;
		if( strLeft2 != "MM")
		{
			p_dTime -= int( p_dTime / 100000000.) * 100000000.;
			if( strLeft2 != "DD")
			{
				p_dTime -= int( p_dTime / 1000000.) * 1000000.;
				if( strLeft2 != "HH")
				{
					ASSERT( FALSE);
				}
			}
		}
	}
	CString strRight2 = strPacketType.Right( 2);
	if( strRight2 != "SS")
	{
		p_dTime /= 100;
		double dFractional = modf( p_dTime, &p_dTime);
		if( 0.009 < dFractional) p_dTime++;
		if( strRight2 != "MM")
		{
			p_dTime /= 100;
			dFractional = modf( p_dTime, &p_dTime);
			if( 0.009 < dFractional) p_dTime++;
			if( strRight2 != "HH")
			{
				p_dTime /= 100;
				dFractional = modf( p_dTime, &p_dTime);
				if( 0.009 < dFractional) p_dTime++;
				if( strRight2 != "DD")
				{
					ASSERT( FALSE);
				}
			}
		}
	}
	
	return __int64(p_dTime);
}