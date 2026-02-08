/******************************************************************************
 * NAME	: DX_OpSensCal.cpp							 
 * DESC	: 옵션 관련 지표 산출 (이항모델 & Black-Sholes)
 * REV. : VER	DATE	DESCRIPTION
 * 	  ----	------	-------------------------------------------------------
 *	  1.00	2004.05.02	Initial version.
 *****************************************************************************/
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CDX_OpSensCal.h"

#include <fcntl.h>
#include <math.h>
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDX_OpSensCal::CDX_OpSensCal()
{

}

CDX_OpSensCal::~CDX_OpSensCal()
{

}

// Helper Functions..

int CDX_OpSensCal::CallPut(LPSTR code)
{
	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
	//                              '2', 'B' : Call option
	//			        '3', 'C' : Put option
	//			        '4', 'D' : 스프레드
	switch (code[0])
	{
	case '2':
	case 'B': return CALL;
	case '3':
	case 'C': return PUT;
	default:  break;
	}

	return 0;
}

double CDX_OpSensCal::GetND(double value)
{
	return (1/sqrt(2 * PI) * exp(-1*pow(value, 2)/2));
}

double CDX_OpSensCal::GetFactorial(int value)
{
	double	ret = 1;
	for (int ii = 1; ii <= value; ii++)
	{
		ret = ret * ii;
	}
	return ret;
}

double CDX_OpSensCal::CNormal(double value)
{
	double dval = 0, phai = 0, ret = 0;

	if (_isnan(value) || !_finite(value))//_isnan : 숫자가 아니다._finite:유효숫자 범위가 아니다.
		value = 0;
	
	dval = 1 / (1 + GAMMA * value);
	phai = 3.1415924814;
	
	if (value >= 0) 
	{
		ret = 1 - (((1 / (pow((2 * phai), 0.5))) * exp(-1 * pow(value, 2) / 2)) * 
		(CNORMAL_1 * dval + CNORMAL_2 * pow(dval, 2) + CNORMAL_3 * pow(dval, 3) + CNORMAL_4 * pow(dval, 4) + CNORMAL_5 * pow(dval, 5)));
	}
	else
	{
		ret = 1 - CNormal(value * -1);
	}

	return ret;
}

double CDX_OpSensCal::GetDouble(double value)
{
	char	buffer[64]{};

	sprintf(buffer, "%.6f", value);

	return atof(buffer);
}

double CDX_OpSensCal::GetOptionTheoryAmt(POPTION in)
{
	if (in->strike == 0.0 || in->volatility == 0.0 || in->remainDays == 0.0)
		return 0.0;

	double d1 = 0, d2 = 0, ret = 0;

	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * pow(in->remainDays, 0.5));
	d2 = d1 - (in->volatility * pow(in->remainDays, 0.5));

	ret = (in->underlying/* - in->baedangJisu*/) * CNormal(d1) - in->strike * exp(-1 * in->cd * in->remainDays) * CNormal(d2);

	if (CallPut(in->code) == PUT)
		ret = ret - (in->underlying/* - in->baedangJisu*/) + in->strike * exp(-1 * in->cd * in->remainDays);

	return ret;
}

// 옵션 내재변동성
double CDX_OpSensCal::GetOptionInsideVolatility(POPTION in)
{
	if (in->underlying == 0.0)
		return 0.0;

	double	high = 1, low = 0, ret = 0;
	double	thoery = 0;
	const	double	curr = in->currAmt / in->underlying;

	for (;;)
	{
		if (high - low <= 0.0001)
			break;

		in->volatility = (high + low) / 2;
		
		thoery = GetOptionTheoryAmt(in);

		if (thoery / in->underlying > curr)
			high = (high + low) / 2;
		else
			low = (high + low) / 2;
	}

	ret = ((high + low) / 2);
	return ret;
}

double CDX_OpSensCal::GetOptionDelta(POPTION in)
{
	if (in->strike == 0.0 || in->volatility == 0.0 || in->remainDays == 0.0/* || in->underlying < in->baedangJisu*/)
		return 0.0;

	double d1 = 0, ret = 0;
	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * sqrt(in->remainDays));

	ret = CNormal(d1);

	if (CallPut(in->code) == PUT)
		ret = ret - 1;
	return ret;
}

double CDX_OpSensCal::GetOptionGamma(POPTION in)
{
	if (in->strike == 0.0 || in->volatility == 0.0 || in->remainDays == 0.0)
		return 0.0;

	double d1 = 0, ret = 0;
	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * sqrt(in->remainDays)/*pow(in->remainDays, 0.5)*/);

	ret = GetND(d1) / ((in->underlying/* - in->baedangJisu*/) * in->volatility * sqrt(in->remainDays)/*pow(in->remainDays, 0.5)*/);

	return ret;
}

double CDX_OpSensCal::GetOptionVegga(POPTION in)
{
	if (in->strike == 0.0 || in->volatility == 0.0 || in->remainDays == 0.0)
		return 0.0;

	double d1 = 0, ret = 0;

	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * sqrt(in->remainDays));

	ret = (in->underlying/* - in->baedangJisu*/) * sqrt(in->remainDays) * GetND(d1) / 100;

	return ret;
}

double CDX_OpSensCal::GetOptionTheta(POPTION in)
{
	if (in->strike == 0.0 || in->volatility == 0.0 || in->remainDays == 0.0)
		return 0.0;

	double d1 = 0, d2 = 0, ret = 0;

	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * sqrt(in->remainDays));

	d2 = d1 - in->volatility * sqrt(in->remainDays);

	if (CallPut(in->code) == CALL) 
		ret = ((-1 * (in->underlying/* - in->baedangJisu*/) * GetND(d1) * in->volatility / (2 * sqrt(in->remainDays))) -
			in->cd * in->strike * exp(-1 * in->cd * in->remainDays) * CNormal(d2)) / 100;
	else
		ret = ((-1 * (in->underlying/* - in->baedangJisu*/) * GetND(d1) * in->volatility / (2 * sqrt(in->remainDays))) +
			in->cd * in->strike * exp(-1 * in->cd * in->remainDays) * CNormal(-1 * d2)) / 100;

	return (ret / 3.65);
}

double CDX_OpSensCal::GetOptionRho(POPTION in)
{
	if (in->strike == 0.0 || in->volatility == 0.0 || in->remainDays == 0.0)
		return 0.0;

	double	d1 = 0, d2 = 0, ret = 0;
	const	int	flag = CallPut(in->code) == CALL ? 1: -1;

	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * sqrt(in->remainDays));
	d2 = d1 - in->volatility * sqrt(in->remainDays);

	ret = flag * in->strike * in->remainDays * exp(-1 * in->cd * in->remainDays) * CNormal(flag * d2) / 100;

	return ret;
}


void CDX_OpSensCal::Calc(long calcItem, POPTION in, double &iv, double &delta, double &gamma, double &theta, double &vegga, double &rho)
{
	iv = 0.0;
	delta = 0.0;
	gamma = 0.0;
	theta = 0.0;
	vegga = 0.0;
	rho = 0.0;

	if (in->strike == 0.0 || in->remainDays == 0.0 || in->underlying == 0.0)
		return;
		
	double	high = 1, low = 0, ret = 0;
	double	d1 = 0, d2 = 0, thoery = 0;
	const	double	curr = in->currAmt / in->underlying;

	// IV 계산
	for (int n = 0;n < 1000;n++)
	{
		if (high - low <= 0.003)
			break;

		in->volatility = (high + low) / 2;
		thoery = GetOptionTheoryAmt(in);
		if (thoery / in->underlying > curr)
			high = (high + low) / 2;
		else
			low = (high + low) / 2;
	}
	iv = ((high + low) / 2);
	in->volatility = iv;
	if (calcItem == CAL_IV) return;

	// 공통변수
	d1 = (log((in->underlying/* - in->baedangJisu*/) / in->strike) + (in->cd + pow(in->volatility, 2) / 2) * in->remainDays) /
			(in->volatility * sqrt(in->remainDays));
	d2 = d1 - in->volatility * sqrt(in->remainDays);

	// Delta 
	if (calcItem & CAL_DELTA)
	{
		delta = CNormal(d1);
		if (CallPut(in->code) == PUT)
			delta = delta - 1;
	}

	// Gamma
	if (calcItem & CAL_GAMMA)
	{
		gamma = GetND(d1) / ((in->underlying/* - in->baedangJisu*/) * in->volatility * sqrt(in->remainDays)/*pow(in->remainDays, 0.5)*/);
	}

	// Theta
	if (calcItem & CAL_THETA)
	{
		if (CallPut(in->code) == CALL) 
			ret = ((-1 * (in->underlying/* - in->baedangJisu*/) * GetND(d1) * in->volatility / (2 * sqrt(in->remainDays))) -
				in->cd * in->strike * exp(-1 * in->cd * in->remainDays) * CNormal(d2)) / 100;
		else
			ret = ((-1 * (in->underlying/* - in->baedangJisu*/) * GetND(d1) * in->volatility / (2 * sqrt(in->remainDays))) +
				in->cd * in->strike * exp(-1 * in->cd * in->remainDays) * CNormal(-1 * d2)) / 100;
		theta = (ret / 3.65);
	}

	// vegga
	if (calcItem & CAL_VEGGA)
	{
		vegga = (in->underlying/* - in->baedangJisu*/) * sqrt(in->remainDays) * GetND(d1) / 100;
	}

	// rho
	if (calcItem & CAL_RHO)
	{
		int flag = CallPut(in->code) == CALL ? 1: -1;
		rho = flag * in->strike * in->remainDays * exp(-1 * in->cd * in->remainDays) * CNormal(flag * d2) / 100;
	}
}
