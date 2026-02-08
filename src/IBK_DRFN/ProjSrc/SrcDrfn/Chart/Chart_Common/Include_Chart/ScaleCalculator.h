// ScaleCalculator.h: interface for the CScaleCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALECALCULATOR_H__E30175FE_B31E_49B4_B8D8_F030A7034924__INCLUDED_)
#define AFX_SCALECALCULATOR_H__E30175FE_B31E_49B4_B8D8_F030A7034924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CScaleCompart;

// scale 계산에 관련된 class
class CScaleCalculator
{
public:
	// packetData와 scaleCompart 에 맞춰 두개를 합한 string 추출
	static CString GetCompartScaleData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);
	static CString GetCompartScaleData_Date(const CString& packetData, const CString& dataType, const CString& compartType);
	static CString GetCompartScaleData_Time(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);

	// 다중 string 일경우 대비한 size 구하기
	static CSize GetTextSize(CDC* pDC, const CString& data);
	// 문자의 길이를 입력하여 size을 가져온다.
	static CSize GetTextSize(const CScaleBaseData::TEXTCOMPARTTYPE textCompart, const CSize& baseSize, const int xCount = 1, const int yCount = 1);
	static CSize GetTextSize(CDC* pDC, const CString& data, const int xCount, const int yCount);

private:
	static CString GetRealPacketData(const CString& packetData, const int nLength);

	static bool GetPacketDataInTimeOut(const CString& dataType, CString& realPacketData);
	static CString GetCompartScaleTime_One(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);
	static CString GetCompartScaleTime_Two(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart);
	static void GetCompartScaleDateTime(const CString& packetData, const CString& dataType, const CScaleCompart& scaleCompart, CString& strData_date, CString& strData_time);

	static CString GetCompartScaleDate(const CString& packetData, const CString& dataType, const CString& compartType);
	static CString GetCompartScaleTime(const CString& packetData, const CString& dataType, const CString& compartType);

	static bool ChangeData(CString& baseData, const CString& insertData, const int position);
	static CString GetTrimData(const CString& packetData, const CString& dataType, const TCHAR& strTrim);
	static CString GetMonthEnglish(const CString& month);
};

#endif // !defined(AFX_SCALECALCULATOR_H__E30175FE_B31E_49B4_B8D8_F030A7034924__INCLUDED_)
