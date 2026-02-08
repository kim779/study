// ExitShort.cpp: implementation of the CExitShort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "ExitShort.h"
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

CExitShort::CExitShort()
{
	m_strName = "EXITSHORT";
	m_strInputInfo.Format("%s,%s",SIGNAL,USEEXITSHORT);
	m_dPosValue = VALEXITSHORT;
}

CExitShort::~CExitShort()
{

}
