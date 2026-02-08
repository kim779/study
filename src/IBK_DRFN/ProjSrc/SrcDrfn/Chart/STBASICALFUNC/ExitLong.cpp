// ExitLong.cpp: implementation of the CExitLong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExitLong.h"
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

CExitLong::CExitLong()
{
	m_strName = "EXITLONG";
	m_strInputInfo.Format("%s,%s",SIGNAL,USEEXITLONG);
	m_dPosValue = VALEXITLONG;
}

CExitLong::~CExitLong()
{

}
