// DataMath.h: interface for the CDataMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_)
#define AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "math.h"

class CMath
{
public:
	// 반올림
	static int Round(const double x);
	static double Round(const double x, const int roundPosition);
	static int Round(const int x, const int roundPosition);

	// 버림
	static double Floor(const double x, const int floorPosition);
	static int Floor(const int x, const int floorPosition);

	// 거듭제곱
	static int Power(const int data, const int count);
	// 두점간의 거리
	static double Distance(const CPoint& pt1, const CPoint& pt2);

	// log
	static double Log(const double& data);

	// 자릿수를 구한다.
	static int NumberCount(const int x);
};

#endif // !defined(AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_)
