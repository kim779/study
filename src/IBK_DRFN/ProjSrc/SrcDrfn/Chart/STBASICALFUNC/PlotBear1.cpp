// PlotBear1.cpp: implementation of the CPlotBear1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "PlotBear1.h"
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

CPlotBear1::CPlotBear1()
{
	m_strName = "PLOTBEAR1";
	m_strInputInfo.Format("%s,%s",INDEX,USEBEAR);
	m_dPosValue = -1;
}

CPlotBear1::~CPlotBear1()
{

}
