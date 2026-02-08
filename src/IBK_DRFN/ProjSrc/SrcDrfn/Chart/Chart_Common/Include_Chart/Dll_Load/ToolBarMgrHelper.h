// ToolBarMgrHelper.h: interface for the CToolBarMgrHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMGRHELPER_H__593DE569_D965_459A_9129_8EB8DD4C1DB0__INCLUDED_)
#define AFX_TOOLBARMGRHELPER_H__593DE569_D965_459A_9129_8EB8DD4C1DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"	// for CLoadDllLib

interface IToolBarManager;
typedef IToolBarManager *( WINAPI *fpCreateToolBarManager)( CCommonInterface *p_pCommonInterface);

class CToolBarMgrHelper : public CLoadDllLib  
{
public:
	fpCreateToolBarManager	m_pfpCreateToolBarManager; 

public:
	CToolBarMgrHelper()
	{
		m_szDllName = _T("ChartToolBar.dll");
		m_hLib = NULL;
		MakeNull(m_pfpCreateToolBarManager);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}
	}

public:
	IToolBarManager *CreateToolBarManager( CCommonInterface *p_pCommonInterface)
	{
		if( !m_hLib) return NULL;//CS_LOADERROR;

		m_pfpCreateToolBarManager	= ( fpCreateToolBarManager)::GetProcAddress(m_hLib, "CreateToolBarManager");
		if(m_pfpCreateToolBarManager==NULL)
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}
		IToolBarManager *pIToolBarManager = m_pfpCreateToolBarManager( p_pCommonInterface);
		MAKESTATE(LDS_OK, LDR_READY);
		return pIToolBarManager;
	}
};

#endif // !defined(AFX_TOOLBARMGRHELPER_H__593DE569_D965_459A_9129_8EB8DD4C1DB0__INCLUDED_)
