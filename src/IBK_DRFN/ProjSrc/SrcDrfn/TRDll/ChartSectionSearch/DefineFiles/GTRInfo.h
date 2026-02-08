// GTRInfo.h: interface for the CGTRInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTRINFO_H__B7391ED9_905E_490D_B9C9_96E28F7CE9DF__INCLUDED_)
#define AFX_GTRINFO_H__B7391ED9_905E_490D_B9C9_96E28F7CE9DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../include/WMUtil.h"
class CGTRInfo  
{
public:
	void SetTrInfo(HWND hWnd, CString sMsg, LPVOID pVoid,long len, int nCount);
	CGTRInfo(CString sTr, CString sData, BOOL bNext = TRUE);
	CGTRInfo(CString sTr, long lData);
	virtual ~CGTRInfo();

	static WORD		m_wTRID;

public:
	HWND		m_hWnd;
	void*		m_pSendWnd;
	CString		m_sTR;
	CString		m_sMsg;
	CString		m_sSendData;
	long		m_nLen;
	int			m_nCount;
	BOOL		m_bNext;

	void*       m_pJonMokData;
	long		m_lSendID;

	CPtrArray   m_arrRealInfo;
	BOOL		m_bOnlyData;
	long        m_lData;
	int			m_nSel;
};

#endif // !defined(AFX_GTRINFO_H__B7391ED9_905E_490D_B9C9_96E28F7CE9DF__INCLUDED_)
