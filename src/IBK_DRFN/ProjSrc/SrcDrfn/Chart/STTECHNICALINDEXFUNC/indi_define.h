#ifndef _INDI_DEFINE_H_
#define _INDI_DEFINE_H_
#include "StdAfx.h"
#include "../Common_ST/IDataItem.h"
#include "../Common_ST/ITempMemMng.h"

// standard header
#include <math.h>
#define UNKNOWN_DATA_TYPE	CIDataItem*
#define TEMP_DATA_TYPE		CITempMemMng*
#define GET_AT(x,y)			(x)->GetAt(y)
#define SET_AT(x,y,z)		(x)->SetAt(y,z)
#define GET_TEMP_DBL(x)		(x)->GetDblArray(MAX_TEMP_ARRAY_SIZE)
#define GET_SUB_TEMP(x,y)	(x)->GetSubTemp(y)
#define NONAVAILABLE		HUGE_VAL
// standard header



#endif
