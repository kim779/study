// Buy.cpp: implementation of the CBuy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Buy.h"
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

CBuy::CBuy()
{
	m_strName = "BUY";
	m_strInputInfo.Format("%s,%s",SIGNAL,USEBUY);
	m_dPosValue = VALENTRYBUY;
}

CBuy::~CBuy()
{

}

