#include "stdafx.h"
#include "calc_com.h"
#include "math.h"


int checkDelta(int spVal, int icodetype)
{
	int nRet=0;
	if (icodetype & kospiType)
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
	else if (icodetype & kosdaqType)
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
	else if(icodetype & thirdType)
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
	else if (icodetype & etfType || icodetype & elwType)
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
	return nRet;
}

int checkDelta( int spVal, int iCodeType, bool bPlus )
{
	int nRet=0;
	if (iCodeType & kospiType)
	{
		if (bPlus)
		{
			if      (spVal <    1000)	nRet = 1;
			else if (spVal <    5000)	nRet = 5;
			else if (spVal <   10000)	nRet = 10;
			else if (spVal <   50000)	nRet = 50;
			else if (spVal <  100000)	nRet = 100;
			else if (spVal <  500000)	nRet = 500;
			else if (spVal >= 500000)	nRet = 1000;
		}
		else
		{
			if      (spVal <=   1000)	nRet = 1;
			else if (spVal <=   5000)	nRet = 5;
			else if (spVal <=  10000)	nRet = 10;
			else if (spVal <=  50000)	nRet = 50;
			else if (spVal <= 100000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else if (spVal  > 500000)	nRet = 1000;
		}
	}
	else if (iCodeType & kosdaqType)
	{
		if (bPlus)
		{
			if      (spVal <   1000)	nRet = 1;
			else if (spVal <   5000)	nRet = 5;
			else if (spVal <  10000)	nRet = 10;
			else if (spVal <  50000)	nRet = 50;
			else if (spVal >= 50000)	nRet = 100;
		}
		else
		{
			if      (spVal <=  1000)	nRet = 1;
			else if (spVal <=  5000)	nRet = 5;
			else if (spVal <= 10000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else if (spVal  > 50000)	nRet = 100;
		}
	}
	else if(iCodeType & thirdType)
	{
		if (bPlus)
		{
			if      (spVal <    1000)	nRet = 1;
			else if (spVal <    5000)	nRet = 5;
			else if (spVal <   10000)	nRet = 10;
			else if (spVal <   50000)	nRet = 50;
			else if (spVal <  100000)	nRet = 100;
			else if (spVal <  500000)	nRet = 500;
			else if (spVal >= 500000)	nRet = 1000;
		}
		else
		{
			if      (spVal <=   1000)	nRet = 1;
			else if (spVal <=   5000)	nRet = 5;
			else if (spVal <=  10000)	nRet = 10;
			else if (spVal <=  50000)	nRet = 50;
			else if (spVal <= 100000)	nRet = 100;
			else if (spVal <= 500000)	nRet = 500;
			else if (spVal  > 500000)	nRet = 1000;
		}
	}
	else if (iCodeType & etfType || iCodeType & elwType)
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
			else if (spVal >= 50000)	nRet = 100;
		}
		else
		{
			if      (spVal <=  1000)	nRet = 1;
			else if (spVal <=  5000)	nRet = 5;
			else if (spVal <= 10000)	nRet = 10;
			else if (spVal <= 50000)	nRet = 50;
			else if (spVal  > 50000)	nRet = 100;
		}
	}
	return nRet;
}


double	getTickPrice(double price, int tick, int codetype, int iplus)
{
	CString ss;
#if 0
	// price에 따른 호가 단위 계산하여 tick만큼 loop
	int nPrice = price*100;
 	ss.Format("GETTICKPRICE : [%f] [%d]\n",price,nPrice);	
 	OutputDebugString(ss);
	int profit1 = 5

	for(int i = 0 ; i < tick ; i++ )
	{
		if (iplus>0)	nPrice += profit1 ;
		else			nPrice -= profit1;
	}
	//nPrice -= nPrice%profit1;
	return nPrice/100;
#else
	price = price*100;
// 	ss.Format("IPLUS TYPE : [%d]\n",iplus);
// 	OutputDebugString(ss);
	CString tmp;
	tmp.Format("%f",price);
	int nPrice = atoi(tmp);
	//OutputDebugString("TTTTTTTTTTTTTTTTT\n");
	ss.Format("GETTICKPRICE : [%f] [%d]\n",price,nPrice);	
 	OutputDebugString(ss);

	int delta = 5;
	
//	if(price < 300)
	if(price < 1000)   //week
		delta = 1;

	int tickval = 0;
	if (iplus>0)
	{
		for(int n=0; n<tick; ++n)
		{
			tickval += delta;
			nPrice += delta;
		}

		if(delta == 5)
		{
			if(nPrice%10 >5)
			{
				nPrice -= (nPrice % 5);
				nPrice += delta;
			}
			else
			{
				nPrice -= (nPrice % 5);
			}
		}
	}
	else
	{
		for(int n=0; n<tick; ++n)
		{
// 			int delta = 5;
// 
// 			if(price < 300)
// 				delta = 1;

			tickval -= delta;
			nPrice -= delta;
		}

		if(delta == 5)
		{
			if(nPrice%10 > 0 && nPrice%10 < 5)
			{
				nPrice -= (nPrice % 5);
				//nPrice -= 5;
			}
			else
			{
				nPrice -= (nPrice % 5);
			}
		}
	}
	
	//nPrice -= (nPrice % 5);
	ss.Format("GETTICKPRICE : [%f] [%f]\n",price,(float)nPrice/100);	
	OutputDebugString(ss);
	return (double)nPrice/100;
#endif
}


double	getTickValue(double price, int tick, int codetype)
{
#if 0
	int profit1 = 5;
	return profit1 * tick;
#else
	double tickval = 0;
	if (tick>0)
	{
		for(int n=0; n<tick; ++n)
		{
			double delta = 0.05;

			if(price < 3)
				delta = 0.01;

			tickval += delta;
			price += delta;
		}
	}
	else
	{
		for(int n=tick; n<0; ++n)
		{
			double delta = 0.05;

			if(price < 3)
				delta = 0.01;

			tickval -= delta;
			price -= delta;
		}
	}
	return tickval;
#endif
}

double	getPrice(double price, int codetype, int iroundup/*올림*/)
{
	//float iprofit = checkDelta(price, codetype);
	int nPrice = (int)(price * 100);
	CString s;
// 	s.Format("NPrice : [%f] [%d]\n",price,nPrice);
// 	OutputDebugString(s);

	int iprofit = 5;

	if(nPrice < 3)
	{
		iprofit = 1;
	}

	int idiv = nPrice % iprofit;

	if( idiv > 0 )
	{
		nPrice -= idiv;
		if(iroundup>0)
			nPrice += iprofit;

	}

	//nPrice = nPrice / 100;
	
	double fres = (double)((double)nPrice / 100);

	return fres;
}


int getAmountType(int icodetype, CString spreCurr)
{
	int iprecurr = atoi(spreCurr);

	if(icodetype == kospiType)
	{
	//	if(iprecurr < 50000)
			return 10;
	}

	return 1;
}