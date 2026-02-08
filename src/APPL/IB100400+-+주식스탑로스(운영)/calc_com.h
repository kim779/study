#pragma once

	int	checkDelta(int spVal, int icodetype);
	constexpr int	checkDelta(int spVal, int iCodeType, bool bPlus);
	int	getTickPrice(int price, int tick, int codetype, int iplus);
	int	getTickValue(int price, int tick, int codetype);
	int	getPrice(int price, int codetype, int iroundup=0/*¿Ã¸²*/);
	int	getAmountType(int icodetype, CString spreCurr);

	static bool g_bNewHoga = false;

