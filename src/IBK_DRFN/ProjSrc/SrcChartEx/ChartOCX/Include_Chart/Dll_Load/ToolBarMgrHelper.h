// ToolBarMgrHelper.h: interface for the CToolBarMgrHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMGRHELPER_H__593DE569_D965_459A_9129_8EB8DD4C1DB0__INCLUDED_)
#define AFX_TOOLBARMGRHELPER_H__593DE569_D965_459A_9129_8EB8DD4C1DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"	// for CLoadDllLib

interface IChartCtrl;
interface IToolBarManager;
typedef IToolBarManager *( WINAPI *fpCreateToolBarManager)( IChartCtrl *p_pIChartCtrl);

class CToolBarMgrHelper : public CLoadDllLib  
{
public:
	fpCreateToolBarManager	m_pfpCreateToolBarManager; 

public:
	CToolBarMgrHelper()
	{
		m_hLib = NULL;
		MakeNull(m_pfpCreateToolBarManager);

		// (2006/2/20 - Seung-Won, Bae) Load OCX's Folders DLL
		m_szDllName = g_strOcxPath + _T("ChartToolBar.dll");
		LoadLib();
		// (2006/3/4 - Seung-Won, Bae) If not found, Load in Bin Folder.
		if(m_hLib == NULL)
		{
			m_szDllName = _T("ChartToolBar.dll");
			LoadLib();
		}
	}

public:
	IToolBarManager *CreateToolBarManager( IChartCtrl *p_pIChartCtrl)
	{
		if( !m_hLib) return NULL;//CS_LOADERROR;

		m_pfpCreateToolBarManager	= ( fpCreateToolBarManager)::GetProcAddress(m_hLib, "CreateToolBarManager");
		if(m_pfpCreateToolBarManager==NULL)
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}
		IToolBarManager *pIToolBarManager = m_pfpCreateToolBarManager( p_pIChartCtrl);
		MAKESTATE(LDS_OK, LDR_READY);
		return pIToolBarManager;
	}
};

#endif // !defined(AFX_TOOLBARMGRHELPER_H__593DE569_D965_459A_9129_8EB8DD4C1DB0__INCLUDED_)
