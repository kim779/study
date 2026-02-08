// CrosshairsOverAddInImp.h: interface for the CCrosshairsOverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSHAIRSOVERADDINIMP_H__6E98B557_AB31_4F1A_A72B_1234C296088C__INCLUDED_)
#define AFX_CROSSHAIRSOVERADDINIMP_H__6E98B557_AB31_4F1A_A72B_1234C296088C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/28
// Comments
//
//	1. Tool Type with Crosshairs and Numerical Inquiry DLG (NIDLG) with Z-Order
//			.......... (Look header of Drag NIDLG AddIn Tool Object Class)
//	2. AddIn Object and Co-Operations.
//			.......... (Look header of Drag NIDLG AddIn Tool Object Class)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/14
// Comments		: Crosshairs Cursor for Numerical Inquiery
// Using way	: It dose not have OnLButtonDown(). ( OnLButtonDown make CrossLine Object)
// See			: 
//-----------------------------------------------------------------------------

class CMainAnalysisTool;
class CCrosshairsOverAddInImp : public CChartAddInBase
{
public:
	CCrosshairsOverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CCrosshairsOverAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/1/24 - Seung-Won, Bae) Manage Analysis Tool Interface.
protected:
	CMainAnalysisTool* m_pAnalysisTool;

// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
protected:
	BOOL	m_bCrossLineFollowClose;

// (2006/6/28 - Seung-Won, Bae) Manage Notify from CrossLine Object Tool to Over-Cross for avoiding the Multiple Cross Active.
protected:
	BOOL	m_bObjectActivated;

// (2006/7/3 - Seung-Won, Bae) Manage Notify from Crosshairs Tool.
protected:
	BOOL	m_bToolActivated;

protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
};

#endif // !defined(AFX_CROSSHAIRSOVERADDINIMP_H__6E98B557_AB31_4F1A_A72B_1234C296088C__INCLUDED_)
