#pragma once

	int			checkDelta(int spVal, int icodetype);
	int			checkDelta(int spVal, int iCodeType, bool bPlus);
	double			getTickPrice(double price, int tick, int codetype, int iplus);
	double			getTickValue(double price, int tick, int codetype);
	double			getPrice(double price, int codetype, int iroundup=0/*¿Ã¸²*/);
	int			getAmountType(int icodetype, CString spreCurr);



