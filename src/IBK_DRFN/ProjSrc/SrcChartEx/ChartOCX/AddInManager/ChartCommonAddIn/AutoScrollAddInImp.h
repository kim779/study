// AutoScrollAddInImp.h: interface for the CAutoScrollAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOSCROLLADDINIMP_H__58B81CB0_00D5_43E8_9516_87A51C452737__INCLUDED_)
#define AFX_AUTOSCROLLADDINIMP_H__58B81CB0_00D5_43E8_9516_87A51C452737__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 20
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CAutoScrollAddInImp : public CChartAddInBase  
{
public:
	CAutoScrollAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CAutoScrollAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

protected:
	int m_nCurrentMode;
	UINT m_uTimerID;
	

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces for Inquiry Tooltip
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual void OnTimer( UINT p_uTimerID);
	virtual void OnToolCommand( const int p_nToolType, const int p_nCmdOption );
};

#endif // !defined(AFX_AUTOSCROLLADDINIMP_H__58B81CB0_00D5_43E8_9516_87A51C452737__INCLUDED_)
