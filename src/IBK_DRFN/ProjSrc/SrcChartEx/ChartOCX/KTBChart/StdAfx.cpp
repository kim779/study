// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CDRDebugerLoader*	g_pDRDebuger = NULL;

CDRDebugerLoader* gfnGetDRDebuger()
{
	if(g_pDRDebuger)
		return g_pDRDebuger;

	g_pDRDebuger = new CDRDebugerLoader();
	if(g_pDRDebuger->IsLoaded()==FALSE)
	{
		delete g_pDRDebuger;
		g_pDRDebuger = NULL;
	}
	return g_pDRDebuger;
}