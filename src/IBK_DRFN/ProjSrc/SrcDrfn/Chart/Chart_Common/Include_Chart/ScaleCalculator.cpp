// ScaleCalculator.cpp: implementation of the CScaleCalculator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleCalculator.h"

#include "ScaleBaseData.h"
#include "Conversion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleCalculator -> scale text 변경

// public =================================================================================
// 날짜 / 시간이 모두 존재한 경우
CString CScaleCalculator::GetCompartScaleData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	switch(scaleDrawerType){
	case CScaleBaseData::HORZ_DAILY:	//일간
	case CScaleBaseData::HORZ_WEEKLEY:	//주간
	case CScaleBaseData::HORZ_MONTHLY:	//월간
		return GetCompartScaleData_Date(packetData, dataType, scaleCompart.GetDateCompart());
	case CScaleBaseData::HORZ_TIME_ONE:	//시간
	case CScaleBaseData::HORZ_TIME_TWO:	//시간
	case CScaleBaseData::HORZ_TICK:		//틱
	case CScaleBaseData::HORZ_TIME_SECOND:
		return GetCompartScaleData_Time(scaleDrawerType, packetData, dataType, scaleCompart);
	}

	return "";
}

CString CScaleCalculator::GetCompartScaleData_Date(const CString& packetData, const CString& dataType, const CString& compartType)
{
	return GetCompartScaleDate(GetRealPacketData(packetData, dataType.GetLength()), dataType, compartType);
}

CString CScaleCalculator::GetCompartScaleData_Time(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	CString realPacketData = GetRealPacketData(packetData, dataType.GetLength());

	// "장마감" 과 "시간외" 를 체크한다.
	if(GetPacketDataInTimeOut(dataType, realPacketData))
		return realPacketData;

	switch(scaleDrawerType){
	case CScaleBaseData::HORZ_TIME_ONE:	//시간
	case CScaleBaseData::HORZ_TICK:		//틱
	case CScaleBaseData::HORZ_TIME_SECOND:
		return GetCompartScaleTime_One(realPacketData, dataType, scaleCompart);
	case CScaleBaseData::HORZ_TIME_TWO:	//시간
		return GetCompartScaleTime_Two(realPacketData, dataType, scaleCompart);
	}

	return "";
}

// ----------------------------------------------------------------------------------------
// 다중 string 일경우 대비한 size 구하기
CSize CScaleCalculator::GetTextSize(CDC* pDC, const CString& data)
{
	CString strData = data;
	CSize textSize(0, 0);
	for(int i = 0; i < CDataConversion::GetStringDataCount(data, "\r\n"); i++){
		CString temp = CDataConversion::GetStringData(strData, "\r\n");
		CSize tempSize = pDC->GetTextExtent(temp);
		if(tempSize.cx > textSize.cx)
			textSize.cx = tempSize.cx;
		if(i == 0)
			textSize.cy += tempSize.cy;
		else
			textSize.cy += tempSize.cy;// + MARGIN;
	}
	if(strData.IsEmpty() == FALSE){
		CSize tempSize = pDC->GetTextExtent(strData);
		if(tempSize.cx > textSize.cx)
			textSize.cx = tempSize.cx;
		textSize.cy += tempSize.cy;
	}

	return textSize; 
}

// 갯수에 맞춰 size를 가져온다.
CSize CScaleCalculator::GetTextSize(const CScaleBaseData::TEXTCOMPARTTYPE textCompart, const CSize& baseSize, const int xCount, const int yCount)
{
	if(xCount <= 0 || yCount <= 0) // 값이 존재하지 않을 경우
		return CSize(0, 0);

	// text 구분 type가 세로형일 경우
	if(textCompart == CScaleBaseData::VERT_TEXT)
		return CSize(baseSize.cx *yCount, baseSize.cy *xCount);

	return CSize(baseSize.cx *xCount, baseSize.cy *yCount);
}

CSize CScaleCalculator::GetTextSize(CDC* pDC, const CString& data, const int xCount, const int yCount)
{
	CSize textSize = pDC->GetTextExtent(data);
	textSize.cx = textSize.cx * xCount;
	textSize.cy = textSize.cy * yCount;

	return textSize;
}

// private ================================================================================
// dataType에 맞게 packetData를 수정한다.
CString CScaleCalculator::GetRealPacketData(const CString& packetData, const int nLength)
{
	if(packetData.IsEmpty() || packetData.GetLength() == nLength)
		return packetData;

	if(packetData.GetLength() > nLength)
		return packetData.Left(nLength);

	CString temp;
	for(int i = 0; i < (nLength - packetData.GetLength()); i++){
		temp += "0";
	}

	return (temp + packetData);
}

// ----------------------------------------------------------------------------------------
//sy 2004.04.16.
// "장전종료(6666)", "장마감종료(7777)", "장마감(8888)", "시간외(9999)" 를 체크한다.
bool CScaleCalculator::GetPacketDataInTimeOut(const CString& dataType, CString& realPacketData)
{
	CString strSpecialTime;
	if(CDataConversion::GetSpecialTime(dataType, realPacketData, strSpecialTime))
	{
		realPacketData = strSpecialTime;
		return true;
	}
	return false;

	/*if(dataType.Right(4) != "HHMM")
		return false;

	if(realPacketData.Right(4) == "6666"){ // 장전종료
		realPacketData = "장전종료";
		return true;
	}
	if(realPacketData.Right(4) == "7777"){ // 장마감종료
		realPacketData = "장마감종료";
		return true;
	}
	if(realPacketData.Right(4) == "8888"){ // 장마감
		realPacketData = "장마감";
		return true;
	}
	if(realPacketData.Right(4) == "9999"){ // 시간외
		realPacketData = "시간외";
		return true;
	}
	return false;*/
}

CString CScaleCalculator::GetCompartScaleTime_One(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	CString strData_date, strData_time;
	GetCompartScaleDateTime(packetData, dataType, scaleCompart, strData_date, strData_time);

	if(!strData_date.IsEmpty() && !strData_time.IsEmpty()){
		if(scaleCompart.GetDateCompart().GetLength() > 1)
			return strData_date + strData_time;
		return strData_date + scaleCompart.GetDateCompart() + strData_time;
	}
	if(!strData_date.IsEmpty())
		return strData_date;
	return strData_time;
}

CString CScaleCalculator::GetCompartScaleTime_Two(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart)
{
	CString strData_date, strData_time;
	GetCompartScaleDateTime(packetData, dataType, scaleCompart, strData_date, strData_time);

	if(!strData_date.IsEmpty() && !strData_time.IsEmpty())
		return strData_time + "\r\n" + strData_date;
	if(!strData_date.IsEmpty())
		return strData_date;
	return strData_time;
}

void CScaleCalculator::GetCompartScaleDateTime(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart, CString& strData_date, CString& strData_time)
{
	strData_date = strData_time = "";
	int datePos = dataType.ReverseFind('D');
	if(datePos < 0){
		strData_time = GetCompartScaleTime(packetData, dataType, scaleCompart.GetTimeCompart());
		return;
	}

	strData_date = GetCompartScaleData_Date(packetData.Left(datePos +1), dataType.Left(datePos +1), scaleCompart.GetDateCompart());

	CString packetData_time = packetData.Right(packetData.GetLength() - (datePos +1));
	CString dataType_time = dataType.Right(dataType.GetLength() - (datePos +1));
	strData_time = GetCompartScaleTime(packetData_time, dataType_time, scaleCompart.GetTimeCompart());
}

// ----------------------------------------------------------------------------------------
// <자로일자>타입(YYYYMMDD 등)을 비교하여 packetData를 compart에 맞춰 변경한다.
CString CScaleCalculator::GetCompartScaleDate(const CString& packetData, const CString& dataType, const CString& compartType)
{
	if(compartType.IsEmpty())
		return packetData;

	CString changePacketData = packetData;

	// < . - : / >인 경우
	if(compartType.GetLength() == 1){
		if(dataType.Find('Y') <= 0){	// 년도가 앞에 있는 경우 
			ChangeData(changePacketData, compartType, dataType.ReverseFind('D'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('M'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('Y'));
		}
		else{	// 년도가 뒤에 있는 경우 - 태국
			ChangeData(changePacketData, compartType, dataType.ReverseFind('Y'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('M'));
			ChangeData(changePacketData, compartType, dataType.ReverseFind('D'));
		}
	}
	// < 년월일 >인 경우
	else if(compartType.GetLength() >= 6){
		if(dataType.Find('Y') <= 0)	{	//년도가 앞에 있는 경우 
			ChangeData(changePacketData, "일", dataType.ReverseFind('D'));
			ChangeData(changePacketData, "월", dataType.ReverseFind('M'));
			ChangeData(changePacketData, "년", dataType.ReverseFind('Y'));
		}
		else{	// 년도가 뒤에 있는 경우 - 태국
			ChangeData(changePacketData, "년", dataType.ReverseFind('Y'));
			ChangeData(changePacketData, "월", dataType.ReverseFind('M'));
			ChangeData(changePacketData, "일", dataType.ReverseFind('D'));
		}
	}
	// <영문>인 경우 - Jan
	else{
		CString strYear = GetTrimData(packetData, dataType, 'Y');
		CString strMonth = GetMonthEnglish(GetTrimData(packetData, dataType, 'M'));
		CString strDaily = GetTrimData(packetData, dataType, 'D');

		if(!strMonth.IsEmpty() && !strDaily.IsEmpty())
			changePacketData = strMonth + '/' + strDaily;
		else
			changePacketData = strMonth + strDaily;

		if(dataType.Find('Y') <= 0){	//년도가 앞에 있는 경우 
			if(!strYear.IsEmpty() && !changePacketData.IsEmpty())
				changePacketData = " " + changePacketData;
			changePacketData = strYear + changePacketData;
		}
		else{
			if(!strYear.IsEmpty() && !changePacketData.IsEmpty())
				changePacketData = changePacketData + " ";
			changePacketData = changePacketData + strYear;
		}
	}

	return changePacketData;
}

// <자료시간> packetData와 그 타입(HHMMSSNN등)을 추가하여 길이를 구한다.
CString CScaleCalculator::GetCompartScaleTime(const CString& packetData, const CString& dataType, const CString& compartType)
{
	if(compartType.IsEmpty())
		return packetData;

	CString changePacketData = packetData;

	bool ret1 = false, ret2 = false, ret3 = false;
	// < . - : / >인 경우
	if(compartType.GetLength() == 1){
		ChangeData(changePacketData, compartType, dataType.ReverseFind('S'));
		ChangeData(changePacketData, compartType, dataType.ReverseFind('M'));
		ChangeData(changePacketData, compartType, dataType.ReverseFind('H'));
	}
	// < 시분초 >인 경우
	else{
		ChangeData(changePacketData, "초", dataType.ReverseFind('S'));
		ChangeData(changePacketData, "분", dataType.ReverseFind('M'));
		ChangeData(changePacketData, "시", dataType.ReverseFind('H'));
	}
	
	return changePacketData;
}

// ----------------------------------------------------------------------------------------
// insertData를 baseData의 position위치에 삽입한다.
bool CScaleCalculator::ChangeData(CString& baseData, const CString& insertData, const int position)
{
	if(position < 0)
		return false;

	if(position +2 > baseData.GetLength() && insertData.GetLength() <= 1)
		return false;

	baseData.Insert(position +1, insertData);	
	return true;
}

CString CScaleCalculator::GetTrimData(const CString& packetData, const CString& dataType, const TCHAR& strTrim)
{
	int nStartIndex = dataType.Find(strTrim);
	if(nStartIndex < 0)
		return "";

	int nEndIndex = dataType.ReverseFind(strTrim);
	return packetData.Mid(nStartIndex, nEndIndex - nStartIndex +1);
}

CString CScaleCalculator::GetMonthEnglish(const CString& month)
{
	if(month == "01")
		return "January";
	else if(month == "02")
		return "February";
	else if(month == "03")
		return "March";
	else if(month == "04")
		return "April";
	else if(month == "05")
		return "May";
	else if(month == "06")
		return "June";
	else if(month == "07")
		return "July";
	else if(month == "08")
		return "August";
	else if(month == "09")
		return "September";
	else if(month == "10")
		return "October";
	else if(month == "11")
		return "November";
	else if(month == "12")
		return "December";

	return month;
}
