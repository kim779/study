// Sell.cpp: implementation of the CSell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "Sell.h"
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
CSell::CSell()
{
	m_strName = "SELL";
	m_strInputInfo.Format("%s,%s",SIGNAL,USESELL);
	m_dPosValue = VALENTRYSELL;
}

CSell::~CSell()
{

}
