#ifndef _CALC_COM_H
#define _CALC_COM_H

int	checkDelta(int spVal, int icodetype);
int	checkDelta(int spVal, int iCodeType, bool bPlus);
int	getTickPrice(int price, int tick, int codetype, int iplus);
int	getTickValue(int price, int tick, int codetype);
int	getPrice(int price, int codetype, int iroundup=0/*¿Ã¸²*/);

#endif