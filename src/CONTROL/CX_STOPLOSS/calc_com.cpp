#include "stdafx.h"
#include "calc_com.h"
#include "math.h"

int checkDelta(int spVal, int icodetype)
{
	int	nRet = 0;

#if 1
	if (icodetype == kospiType)
	{
		if (spVal < 1000)
			nRet = 1;
		else if (spVal >= 1000 && spVal < 5000)
			nRet = 5;
		else if (spVal >= 5000 && spVal < 10000)
			nRet = 10;
		else if (spVal >= 10000 && spVal < 50000)
			nRet = 50;
		else if (spVal >= 50000 && spVal < 100000)
			nRet = 100;
		else if (spVal >= 100000 && spVal < 500000)
			nRet = 500;
		else
			nRet = 1000;
	}
	else if (icodetype == kosdaqType)
	{
		if (spVal < 1000)
			nRet = 1;
		else if (spVal >= 1000 && spVal < 5000)
			nRet = 5;
		else if (spVal >= 5000 && spVal < 10000)
			nRet = 10;
		else if (spVal >= 10000 && spVal < 50000)
			nRet = 50;
		else
			nRet = 100;
	}
	else if (icodetype == thirdType)
	{
		if (spVal < 1000)
			nRet = 1;
		else if (spVal >= 1000 && spVal < 5000)
			nRet = 5;
		else if (spVal >= 5000 && spVal < 10000)
			nRet = 10;
		else if (spVal >= 10000 && spVal < 50000)
			nRet = 50;
		else if (spVal >= 50000 && spVal < 100000)
			nRet = 100;
		else if (spVal >= 100000 && spVal < 500000)
			nRet = 500;
		else
			nRet = 1000;
	}
	else if (icodetype == etfType || icodetype == etnType)
	{
		if (spVal < 2000)
			nRet = 1;
		else
			nRet = 5;
	}
	else if (icodetype == elwType)
	{
		nRet = 5;
	}
	else
	{
		if (spVal < 1000)
			nRet = 1;
		else if (spVal >= 1000 && spVal < 5000)
			nRet = 5;
		else if (spVal >= 5000 && spVal < 10000)
			nRet = 10;
		else if (spVal >= 10000 && spVal < 50000)
			nRet = 50;
		else if (spVal >= 50000 && spVal < 100000)
			nRet = 100;
		else if (spVal >= 100000 && spVal < 500000)
			nRet = 500;
		else
			nRet = 1000;
	}
#else	// 2023 주식 호가가격단위 개선
	if (icodetype == thirdType)
	{
		if (spVal < 1000)
			nRet = 1;
		else if (spVal >= 1000 && spVal < 5000)
			nRet = 5;
		else if (spVal >= 5000 && spVal < 10000)
			nRet = 10;
		else if (spVal >= 10000 && spVal < 50000)
			nRet = 50;
		else if (spVal >= 50000 && spVal < 100000)
			nRet = 100;
		else if (spVal >= 100000 && spVal < 500000)
			nRet = 500;
		else
			nRet = 1000;
	}
	else if (icodetype == etfType || icodetype == elwType)
	{
		nRet = 5;
	}
	else
	{
		if (spVal < 2000)
			nRet = 1;
		else if (spVal < 5000)
			nRet = 5;
		else if (spVal < 20000)
			nRet = 10;
		else if (spVal < 50000)
			nRet = 50;
		else if (spVal < 200000)
			nRet = 100;
		else if (spVal < 500000)
			nRet = 500;
		else
			nRet = 1000;
	}
#endif
	return nRet;
}

int checkDelta(int spVal, int iCodeType, bool bPlus)
{
	int	nRet=0;

#if 1
	if (iCodeType == kospiType)
	{
		if (bPlus)
		{
			if      (spVal <    1000)	nRet = 1;
			else if (spVal <    5000)	nRet = 5;
			else if (spVal <   10000)	nRet = 10;
			else if (spVal <   50000)	nRet = 50;
			else if (spVal <  100000)	nRet = 100;
			else if (spVal <  500000)	nRet = 500;
			else				nRet = 1000;
		}
		else
		{
			if      (spVal <=   1000)	nRet = 1;
			else if (spVal <=   5000)	nRet = 5;
			else if (spVal <=  10000)	nRet = 10;
			else if (spVal <=  50000)	nRet = 50;
			else if (spVal <= 100000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else				nRet = 1000;
		}
	}
	else if (iCodeType == kosdaqType)
	{
		if (bPlus)
		{
			if      (spVal <   1000)	nRet = 1;
			else if (spVal <   5000)	nRet = 5;
			else if (spVal <  10000)	nRet = 10;
			else if (spVal <  50000)	nRet = 50;
			else				nRet = 100;
		}
		else
		{
			if      (spVal <=  1000)	nRet = 1;
			else if (spVal <=  5000)	nRet = 5;
			else if (spVal <= 10000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else				nRet = 100;
		}
	}
	else if (iCodeType == thirdType)
	{
		if (bPlus)
		{
			if      (spVal <    1000)	nRet = 1;
			else if (spVal <    5000)	nRet = 5;
			else if (spVal <   10000)	nRet = 10;
			else if (spVal <   50000)	nRet = 50;
			else if (spVal <  100000)	nRet = 100;
			else if (spVal <  500000)	nRet = 500;
			else				nRet = 1000;
		}
		else
		{
			if      (spVal <=   1000)	nRet = 1;
			else if (spVal <=   5000)	nRet = 5;
			else if (spVal <=  10000)	nRet = 10;
			else if (spVal <=  50000)	nRet = 50;
			else if (spVal <= 100000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else				nRet = 1000;
		}
	}
	else if (iCodeType == etfType || iCodeType == etnType)
	{
		if (bPlus)
		{
			if (spVal < 2000)
				nRet = 1;
			else
				nRet = 5;
		}
		else
		{
			if (spVal <= 2000)
				nRet = 1;
			else
				nRet = 5;

		}
	}
	else if (iCodeType == elwType)
	{
		nRet = 5;
	}
	else
	{
		if (bPlus)
		{
			if      (spVal <   1000)	nRet = 1;
			else if (spVal <   5000)	nRet = 5;
			else if (spVal <  10000)	nRet = 10;
			else if (spVal <  50000)	nRet = 50;
			else				nRet = 100;
		}
		else
		{
			if      (spVal <=  1000)	nRet = 1;
			else if (spVal <=  5000)	nRet = 5;
			else if (spVal <= 10000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else				nRet = 100;
		}
	}
#else	// 2023 주식 호가가격단위 개선
	if (iCodeType == thirdType)
	{
		if (bPlus)
		{
			if (spVal < 1000)	nRet = 1;
			else if (spVal < 5000)	nRet = 5;
			else if (spVal < 10000)	nRet = 10;
			else if (spVal < 50000)	nRet = 50;
			else if (spVal < 100000)	nRet = 100;
			else if (spVal < 500000)	nRet = 500;
			else				nRet = 1000;
		}
		else
		{
			if (spVal <= 1000)	nRet = 1;
			else if (spVal <= 5000)	nRet = 5;
			else if (spVal <= 10000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else if (spVal <= 100000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else				nRet = 1000;
		}
	}
	else if (iCodeType == etfType || iCodeType == elwType)
	{
		nRet = 5;
	}
	else
	{
		if (bPlus)
		{
			if (spVal < 2000)		nRet = 1;
			else if (spVal < 5000)		nRet = 5;
			else if (spVal < 20000)		nRet = 10;
			else if (spVal < 50000)		nRet = 50;
			else if (spVal < 200000)	nRet = 100;
			else if (spVal < 500000)	nRet = 500;
			else				nRet = 1000;
		}
		else
		{
			if (spVal <= 1)			nRet = 0;
			else if (spVal <= 2000)		nRet = 1;
			else if (spVal <= 5000)		nRet = 5;
			else if (spVal <= 20000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else if (spVal <= 200000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else				nRet = 1000;
		}
	}
///////////
// 	if (iCodeType == kospiType)
	{
		if (bPlus)
		{
			if (spVal < 1000)	nRet = 1;
			else if (spVal < 5000)	nRet = 5;
			else if (spVal < 10000)	nRet = 10;
			else if (spVal < 50000)	nRet = 50;
			else if (spVal < 100000)	nRet = 100;
			else if (spVal < 500000)	nRet = 500;
			else				nRet = 1000;
		}
		else
		{
			if (spVal <= 1000)	nRet = 1;
			else if (spVal <= 5000)	nRet = 5;
			else if (spVal <= 10000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else if (spVal <= 100000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else				nRet = 1000;
		}
	}

//////////////
#endif
	return nRet;
}

int	getTickPrice(int price, int tick, int codetype, int iplus)
{
#if 0
	// price에 따른 호가 단위 계산하여 tick만큼 loop
	int nPrice = price;
	int profit1 = checkDelta(nPrice, codetype);

	for (int i = 0 ; i < tick ; i++)
	{
		if (iplus>0)	nPrice += profit1 ;
		else			nPrice -= profit1;
	}
	nPrice -= nPrice%profit1;
	return nPrice;
#else
	int	delta = 0, tickval = 0;

	if (iplus > 0)
	{
		for (int n = 0; n < tick; n++)
		{
			delta = checkDelta(price, codetype, true);
			tickval += delta;
			price   += delta;
		}
	}
	else
	{
		for (int n = 0; n < tick; n++)
		{
			delta = checkDelta(price, codetype, false);
			tickval -= delta;
			price   -= delta;
		}
	}
	price -= (price % checkDelta(price, codetype, (iplus>0) ? true : false));
	return price;
#endif
}


int	getTickValue(int price, int tick, int codetype)
{
#if 0
	int profit1 = checkDelta(price, codetype);
	return profit1 * tick;
#else
	int	delta = 0, tickval = 0;
	if (tick > 0)
	{
		for (int n = 0; n < tick; n++)
		{
			delta = checkDelta(price, codetype, true);
			tickval += delta;
			price   += delta;
		}
	}
	else
	{
		for (int n = tick; n < 0; n++)
		{
			delta = checkDelta(price, codetype, false);
			tickval -= delta;
			price   -= delta;
		}
	}
	return tickval;
#endif
}

int	getPrice(int price, int codetype, int iroundup/*올림*/)
{
	const	int	iprofit = checkDelta(price, codetype);
	const	int	idiv = price % iprofit;

	if (idiv > 0)
	{
		price -= idiv;
		if (iroundup>0)
			price += iprofit;

	}
	return price;
}