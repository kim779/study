// AddIndicatorCmdAddInImp.h: interface for the CAddIndicatorCmdAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDINDICATORCMDADDINIMP_H__5C6F4A52_3507_4E5E_901E_772B437417E7__INCLUDED_)
#define AFX_ADDINDICATORCMDADDINIMP_H__5C6F4A52_3507_4E5E_901E_772B437417E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/11
// Comments		: Indicator Adding Command (WM_COMMAND) Processing AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CAddIndicatorCmdAddInImp : public CChartAddInBase
{
public:
	CAddIndicatorCmdAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CAddIndicatorCmdAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
};

#endif // !defined(AFX_ADDINDICATORCMDADDINIMP_H__5C6F4A52_3507_4E5E_901E_772B437417E7__INCLUDED_)
