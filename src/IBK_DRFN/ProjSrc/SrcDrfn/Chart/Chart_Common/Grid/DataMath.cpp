// DataMath.cpp: implementation of the CDataMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMath

// public ===============================================================================
// 반올림 함수들 ------------------------------------------------------------------------
// double 을 반올림해서 int형으로 변환 : round(123.567) -> 124
int CMath::Round(const double x)
{
	if(x < 0)
		return (int)floor(x - 0.5);
	return (int)floor(x + 0.5);
}

// 실수 반올림 : 소숫점 아래의 roundPosition자리에서 반올림 : round(123.567, 2) -> 123.6
double CMath::Round(const double x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	double temp = x * pow(10.0, (double)(roundPosition-1));
	double value = floor(temp +0.5) * pow(0.1, (double)(roundPosition-1));

	return value;
}

// 정수 반올림 : roundPosition자리에서 반올림 : round(123567, 2) -> 123600
int CMath::Round(const int x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	double value = (double)x * pow(0.1, (double)roundPosition);
	value = floor(value+0.5) * pow(10.0, (double)roundPosition);

	return (int)value;
}

// 버림 함수들 --------------------------------------------------------------------------
// 실수 버림 : 소숫점 아래의 roundPosition자리에서 버림 : Floor(123.567, 2) -> 123.5
double CMath::Floor(const double x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	double temp = x * pow(10.0, (double)(roundPosition-1));
	double value = floor(temp) * pow(0.1, (double)(roundPosition-1));

	return value;
}

// 정수 버림 : roundPosition자리에서 버림 : Floor(123567, 2) -> 123500
int CMath::Floor(const int x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	double value = (double)x * pow(0.1, (double)roundPosition);
	value = floor(value) * pow(10.0, (double)roundPosition);

	return (int)value;
}

// 제곱 함수 ----------------------------------------------------------------------------
int CMath::Power(const int data, const int count)
{
	return (int)floor(pow((double)data, (double)count));
}

// 두점간의 거리 --> sqrt(x*x+y*y)
double CMath::Distance(const CPoint& pt1, const CPoint& pt2)
{
	return sqrt(pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2));
}

// Log 값 -------------------------------------------------------------------------------
double CMath::Log(const double& data)
{
	return (log(data) * 10000.0);
}

// --------------------------------------------------------------------------------------
// 자릿수를 구한다.
int CMath::NumberCount(const int x)
{
	CString data;
	data.Format("%d", x);

	return data.GetLength();
}
