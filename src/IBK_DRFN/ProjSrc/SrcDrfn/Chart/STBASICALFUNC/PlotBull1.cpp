// PlotBull1.cpp: implementation of the CPlotBull1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "PlotBull1.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STConfigdef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlotBull1::CPlotBull1()
{
	m_strName = "PLOTBULL1";
	m_strInputInfo.Format("%s,%s",INDEX,USEBULL);
	m_dPosValue = 1;
}

CPlotBull1::~CPlotBull1()
{

}
