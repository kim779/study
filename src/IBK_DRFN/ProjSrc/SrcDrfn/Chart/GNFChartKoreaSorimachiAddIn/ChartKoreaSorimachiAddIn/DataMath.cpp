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
	return (int)floor(x + 0.5);
}

double CMath::Round_Double(const double& x)
{
	return floor(x + 0.5);
}

// 실수 반올림 : 소숫점 아래의 roundPosition자리에서 반올림 : round(123.567, 2) -> 123.6
double CMath::Round(const double& x, const int roundPosition)
{
	if(roundPosition <= 0)
		return Round_Double(x);

	// 소숫점 아래 부분이 "0" 일 경우
	// 아래 부분을 구현 안할 경우 4969221000.0000 -> 4969221000.00001 로 된다.
	if(x == (int)x)
		return x;

	double value = x * pow(10.0, (double)(roundPosition-1));

	// 아래부분을 수정한 이유는 소숫점이 부분이 "0" 이였을 경우 뒤에 "1"이 붙는 경우 발생.
	//return (Round_Double(value) * pow(0.1, (double)(roundPosition-1)));
	return Divide10(Round_Double(value), (roundPosition -1));
}

// 정수 반올림 : roundPosition자리에서 반올림 : round(123567, 2) -> 123600
int CMath::Round(const int x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	//double value = (double)x * pow(0.1, (double)roundPosition);
	double value = Divide10((double)x, roundPosition);
	value = Round(value) * pow(10.0, (double)roundPosition);
	return (int)value;
}

// 버림 함수들 --------------------------------------------------------------------------
// 실수 버림 : 소숫점 아래의 roundPosition자리에서 버림 : Floor(123.567, 2) -> 123.5
double CMath::Floor(const double& x, const int roundPosition)
{
	if(roundPosition <= 0)
		return floor(x);

	double temp = x * pow(10.0, (double)(roundPosition-1));
	//return (floor(temp) * pow(0.1, (double)(roundPosition-1)));
	return Divide10(floor(temp), (roundPosition-1));
}

// 정수 버림 : roundPosition자리에서 버림 : Floor(123567, 2) -> 123500
int CMath::Floor(const int x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	//double value = (double)x * pow(0.1, (double)roundPosition);
	double value = Divide10((double)x, roundPosition);
	value = floor(value) * pow(10.0, (double)roundPosition);
	return (int)value;
}

// --------------------------------------------------------------------------------------
// 제곱 함수 
double CMath::Divide10(const double& dData, const int count)
{
	double dValue = dData;
	for(int nIndex = 0; nIndex < count; nIndex++){
		dValue = dValue / 10.0;
	}
	return dValue;
}

int CMath::Power(const int data, const int count)
{
	return (int)floor(pow((double)data, (double)count));
}

// 두점간의 거리 --> sqrt(x*x+y*y)
double CMath::Distance(const CPoint& pt1, const CPoint& pt2)
{
	return sqrt(pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2));
}

// 두점의 중간 위치
int CMath::Center(const int pt1, const int pt2)
{
	return (pt1 + Round((double)(pt2 - pt1) / 2.0));
}

// log 값
double CMath::Log(const double& data)
{
	//return (log(data) * 10000.0);
	if(data == 0.0)
		return 0.0;

	if(data > 0)
		return log(data);
	return -log(fabs(data));
}

// exp 값
double CMath::Exp(const double& data)
{
	//return exp(data / 10000.0);
	if(data == 0.0)
		return 0.0;

	if(data > 0)
		return exp(data);
	return -exp(fabs(data));
}

// 나머지 구하기(%)
double CMath::Modulus(const double& data1, const double& data2)
{
	if(data2 == 0)
		return 0.0;

	double dTemp = int(data1/data2) * data2;
	return (data1 - dTemp);
}

// --------------------------------------------------------------------------------------
int CMath::TanI(const int nAngle)
{
	if((nAngle%180) == 0)
		return 0;
	else if((nAngle%90) == 0)
		return 0;

	int nRealAngle = nAngle%360;
	double pi = 3.1415926535897931;
	double dAngle = pi * (double)nRealAngle / 180.0;
	return Round(tan(dAngle));
}

double CMath::TanD(const double& dAngle)
{
	if(Modulus(dAngle, 180.0) == 0.0)
		return 0.0;
	else if(Modulus(dAngle, 90.0) == 0.0)
		return 0.0;

	double dRealAngle = Modulus(dAngle, 360.0);

	double pi = 3.1415926535897931;
	dRealAngle = pi * dRealAngle / 180.0;
	return tan(dRealAngle);
}

double CMath::Angle(const double& dTan)
{
	if(dTan == 1.0)
		return 45.0;
	else if(dTan == -1.0)
		return 135.0;
	else if(dTan == 0.0)
		return 180.0;

	double pi = 3.1415926535897931;
	double dAngle = atan(dTan) * 180.0 / pi;
	return dAngle;
}

// --------------------------------------------------------------------------------------
// 자릿수를 구한다.
int CMath::NumberCount(const int x)
{
	CString data;
	data.Format("%d", x);

	return data.GetLength();
}
