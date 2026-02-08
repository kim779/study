// CodeDataFromInputBar.cpp: implementation of the CCodeDataFromInputBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CodeDataFromInputBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCodeDataFromInputBar::CCodeDataFromInputBar()
{
	m_lEndTime = 0;
	m_chType = NULL;
	m_lTypeNum = 1;	
	m_lDataCnt = 0;
	m_lPtrST = 0;
}

CCodeDataFromInputBar::~CCodeDataFromInputBar()
{

}
