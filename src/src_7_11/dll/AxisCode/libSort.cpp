#include "stdafx.h"
#include "axiscode.h"
#include "libSort.h"

int _sortCode(const void *arg1, const void *arg2)
{
	struct _sortStr* pLVal= (struct _sortStr*)arg1;
	struct _sortStr* pRVal= (struct _sortStr*)arg2;
	return pLVal->code.Compare(pRVal->code);
}

int _sortName(const void *arg1, const void *arg2)
{
	struct _sortStr* pLVal= (struct _sortStr*)arg1;
	struct _sortStr* pRVal= (struct _sortStr*)arg2;
   	return pLVal->name.Compare(pRVal->name);
}
