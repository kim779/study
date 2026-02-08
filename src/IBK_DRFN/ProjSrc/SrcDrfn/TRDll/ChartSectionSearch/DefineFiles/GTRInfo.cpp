// GTRInfo.cpp: implementation of the CGTRInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "InterestForm.h"
#include "GTRInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CGTRInfo::CGTRInfo(HWND hWnd, CString sTR, CString sMsg, LPVOID pVoid,long len, int nCount)
{
	m_hWnd = hWnd;
	m_sTR  = sTR; 
	m_sMsg = sMsg;
	m_pJonMokData = (ST_INQUIRYDATA*)pVoid;
	m_nLen = len;
	m_nCount = nCount;
}
*/
WORD CGTRInfo::m_wTRID = 0;
CGTRInfo::CGTRInfo(CString sTr, CString sData, BOOL bNext)
{
	m_sTR = sTr;
	m_sSendData = sData;
	m_bNext = bNext;
	m_bOnlyData = FALSE;
	if(m_wTRID>30000) 
		m_wTRID=0;
	m_wTRID++;
	m_nSel = 0;
}



CGTRInfo::~CGTRInfo()
{
}

void CGTRInfo::SetTrInfo(HWND hWnd, CString sMsg, LPVOID pVoid,long len, int nCount)
{
	m_hWnd = hWnd;
	m_sMsg = sMsg;
	m_pJonMokData = (ST_INQUIRYDATA*)pVoid;
	m_nLen = len;
	m_nCount = nCount;
	m_nSel = 0;
}

CGTRInfo::CGTRInfo(CString sTr, long lData)
{
	m_lData = lData;
	m_sTR = sTr;
	m_bOnlyData = FALSE;
	m_bNext = FALSE;
	m_nSel = 0;
	if(m_wTRID>30000) 
		m_wTRID=0;
	m_wTRID++;
}

