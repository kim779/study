// StrategyLoader.h: interface for the CStrategyLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRATEGYLOADER_H__55A42034_2552_402D_B0F7_677858197A31__INCLUDED_)
#define AFX_STRATEGYLOADER_H__55A42034_2552_402D_B0F7_677858197A31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef  long (WINAPI* _DLL_GetStrategyItem)();

#include "../Common_ST/IStrategyItem.h"

class CStrategyLoader  
{
public:
	CStrategyLoader();
	CIStrategyItem *GetStrategyItem();
	virtual ~CStrategyLoader();
private:
	BOOL LoadStrategyManager();
	BOOL UnloadStrategyManager();
	_DLL_GetStrategyItem m_fnGetStrategyItem;
	HINSTANCE m_hStrategyManager;
	CString m_strStrategyManager;

// (2006/3/5 - Seung-Won, Bae) Manage Debug Version Flag and Path of Chart OCX
protected:
	static int		m_bIsDebugVersion;
	static CString	m_strChartOcxFolder;
};

#endif // !defined(AFX_STRATEGYLOADER_H__55A42034_2552_402D_B0F7_677858197A31__INCLUDED_)
