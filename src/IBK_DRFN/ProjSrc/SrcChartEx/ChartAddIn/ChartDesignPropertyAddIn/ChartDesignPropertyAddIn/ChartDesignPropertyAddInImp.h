// ChartDesignPropertyAddInImp.h: interface for the CChartDesignPropertyAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDESIGNPROPERTYADDINIMP_H__1B455AE2_B874_4571_9ABC_9A768C30F5D7__INCLUDED_)
#define AFX_CHARTDESIGNPROPERTYADDINIMP_H__1B455AE2_B874_4571_9ABC_9A768C30F5D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"							// for Ixxxxxx.h
#include "DlgGlobalLanguageTable.h"					// for CDlgGlobalLanguageTable

class CChartDesignPropertyAddInImp : public CChartAddInBase
{
public:
	CChartDesignPropertyAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CWnd *p_pWnd, CPoint &point);

// (2006/7/4 - Seung-Won, Bae) Manage Menu Using Order Flag.
protected:
	BOOL m_bExeMenuInit;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND	m_hOcxWnd;

// (2008/2/24 - Seung-Won, Bae) for Global Meta Table
public
:
	static CDlgGlobalLanguageTable		m_dlgGLT;
};

#endif // !defined(AFX_CHARTDESIGNPROPERTYADDINIMP_H__1B455AE2_B874_4571_9ABC_9A768C30F5D7__INCLUDED_)
