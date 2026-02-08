#ifndef __CHARTHELPER_H__
#define __CHARTHELPER_H__

#include "../../h/axisgenv.h"

typedef struct {
	char name[32];		// chart name
	int	kind;			// chart kind index
	int  group;			// menu group
} CHARTKIND, *LPCHARTKIND;

typedef CTypedPtrList<CPtrList, LPCHARTKIND> CChartKindList;

#define JP_TREND			1
#define JP_MOMENTUM			2
#define JP_VOLUME			3
#define JP_MARKET			4
#define JP_CHANNEL			5
//#define JP_INVEST			6
#define JP_ETC				6


#endif // __CHARTHELPER_H__