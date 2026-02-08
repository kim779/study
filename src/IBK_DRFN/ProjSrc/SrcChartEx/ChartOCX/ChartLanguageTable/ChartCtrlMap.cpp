// ChartCtrlMap.cpp: implementation of the CChartCtrlMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLanguageTable.h"
#include "ChartCtrlMap.h"

#include "ChartCtrl.h"					// for CChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartCtrlMap::CChartCtrlMap()
{

}

CChartCtrlMap::~CChartCtrlMap()
{
	HWND hOcxWnd = NULL;
	CChartCtrl *pChartCtrl = NULL;
	POSITION psnChartCtrl = GetStartPosition();
	while( psnChartCtrl)
	{
		GetNextAssoc( psnChartCtrl, hOcxWnd, pChartCtrl);
		if( !pChartCtrl) continue;
		delete pChartCtrl;
	}
	RemoveAll();
}
