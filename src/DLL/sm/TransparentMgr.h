// TransparentMgr.h: interface for the CTransparentMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSPARENTMGR_H__CB85ECF4_09FF_4D3C_B27A_739820BC4257__INCLUDED_)
#define AFX_TRANSPARENTMGR_H__CB85ECF4_09FF_4D3C_B27A_739820BC4257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CTPInfo
{
public:
	int alpha;
	BOOL bPlus;
	BOOL bDestroy;

	CTPInfo()
	{
		alpha = 0;
		bPlus = TRUE;
		bDestroy = FALSE;
	}
	~CTPInfo()
	{
	}
	CTPInfo & operator = (const CTPInfo src)
	{
		alpha = src.alpha;
		bPlus = src.bPlus;
		bDestroy = src.bDestroy;
		return (CTPInfo&)*this;
	}
};

class AFX_EXT_CLASS CTransparentMgr  
{
public:
	BOOL	m_bAutoDelete;
public:
	static void CALLBACK OnTPTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
	void SetTPWnd(HWND hWnd, int nTime = 10, BOOL bPlus = TRUE, BOOL bDestroy = FALSE);
	CTransparentMgr();
	HINSTANCE m_hUser32{};
	virtual ~CTransparentMgr();
};

#endif // !defined(AFX_TRANSPARENTMGR_H__CB85ECF4_09FF_4D3C_B27A_739820BC4257__INCLUDED_)
