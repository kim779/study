// DataMath.h: interface for the CDataMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_)
#define AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/* 반올림 yiy 2006.12.21
	실수연산을 통해서 화면에 그 값을 사상하기 위해서는 정수로 변환하는 작업이 필요하게 됩니다.
	이때 반올림 연산을 사용하게 되는데요 이 반올림 연산의 결과인 화면에 보여지는 점은 양수일때 에만
	큰 의미를 같습니다. 화면의 좌표는 (0,0) 부터 시작하기 때문이지요...
	그렇다고 양수일때 의미있는 값을 만들수 있는 방법3을 쓰게 되면 보이지않는 영역에서 보이는 영역으로 선을 그을 경우에
	보이지 않는 영역(음수 영역) 이 1px 오차가 발생할수 있고 그것이 원하지 않는 결과일수 있습니다.(선택의 문제)
	해서 아주 큰수를 더해서 정수로 변환하고 다시 더한 큰수를 빼는 방법을 사용할수 있습니다.
	이렇게 할경우 아주 멀리 떨어진 음수 영역의 좌표가 1px 달라진다고 해도 보여지는 영역의 정보에 미치는 영향이 거의 미비(거의 없음)
	하기 때문에 방법2를 쓸수 있습니다.
	
	방법1은 양수와 음수에 대해서 정확한 반올림 연산을 할수 있도록 하는 것으로 이경우 조건문이 들어가게 됩니다.
	하지만 이 방법이 기존의 방법보다 유리할수 있는 것은 대부분의 반올림 연산의 파라메터로 들어오는 값은 양수가 될것이기 때문에
	CPU의 분기 예측에 의해서 어느정도 커버가 될수 있는 것입니다.

	일반적인 시스템에서 예상하기에 (상황에 따라서 다를수 있습니다.)
	성능상으로 보면 [방법3 > 방법2 > 방법1]으 순으로 결과가 나올것입니다.
	정확도로 보면    [방법 1> 방법2 > 방법3]으 순으로 결과가 나옵니다.

	현재는 가장 느릴수 있지만 방법1을 사용하고 있습니다.
	더 높은 성능을 끌어내고 싶다면 방법2를 써도 될것으로 생각됩니다.
*/

#define ROUND(x) int((x>0)?(x+0.5):(x-0.5))		// 방법1
//#define ROUND(x) (int(x+5000000.5) - 5000000)		// 방법2
//#define ROUND(x) int(x+0.5)				// 방법3

class CMath
{
public:
	// 반올림
	static int Round(const double x);
	static double Round_Double(const double& x);
	static double Round(const double& x, const int roundPosition);
	static int Round(const int x, const int roundPosition);

	// 버림
	static double Floor(const double& x, const int floorPosition);
	static int Floor(const int x, const int floorPosition);

	// 거듭제곱
	static double Divide10(const double& dData, const int count);
	static int Power(const int data, const int count);
	// 두점간의 거리
	static double Distance(const CPoint& pt1, const CPoint& pt2);
	// 두점의 중간 위치
	static int Center(const int pt1, const int pt2);

	// log에 관한 내용
	static double Log(const double& data);
	static double Exp(const double& data);

	// 나머지 구하기 (%)
	static double Modulus(const double& data1, const double& data2);

	// 각도
	static int TanI(const int nAngle);
	static double TanD(const double& dAngle);
	static double Angle(const double& dTan);

	// 자릿수를 구한다.
	static int NumberCount(const int x);
};

#endif // !defined(AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_)
