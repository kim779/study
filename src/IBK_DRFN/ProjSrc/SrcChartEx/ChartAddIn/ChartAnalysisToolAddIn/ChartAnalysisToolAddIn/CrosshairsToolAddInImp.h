// CrosshairsToolAddInImp.h: interface for the CCrosshairsToolAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CROSSHAIRSTOOLADDINIMP_H__6DE63D3A_5E23_4F61_8B80_CC527EF77694__INCLUDED_)
#define AFX_CROSSHAIRSTOOLADDINIMP_H__6DE63D3A_5E23_4F61_8B80_CC527EF77694__INCLUDED_

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
// Author		: Seung-Won, Bae	Date :2006/7/3
// Comments		: Crosshairs Tool for Numerical Inquiery
// Using way	: It dose not have OnLButtonDown(). ( OnLButtonDown make CrossLine Object)
// See			: 
//-----------------------------------------------------------------------------

class CMainAnalysisTool;
class CCrosshairsToolAddInImp : public CChartAddInBase
{
public:
	CCrosshairsToolAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CCrosshairsToolAddInImp();

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

	BOOL	m_bCrossBtnExecuted;
	BOOL	m_bCrossCfgExecuted;
	BOOL	m_bWithNIDlgOnToolExec;

// (2007/01/02 - jwp) MouseCrossCursor ++
private:
	CRect m_rctBlockRegion;					// View의 전체 Graph 영역이다. (Block의 View 영역이다.)
	CRect m_rctGraphDrawingRegion;			// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)

	BOOL m_bLButtonDown;
	BOOL m_bOnSetCaprture;
	CPoint m_ptOldMouse;
	CRect m_rctTextLeft;	
	CRect m_rctTextRight;	
	CRect m_rctTextTop;	
	CRect m_rctTextBottom;	
	CString m_strTextXDate;	
	CString m_strTextYValue;
	
// (2007/01/02 - jwp) MouseCrossCursor --
	void DrawNewCross( HDC p_hDC);
	BOOL GetGraphTotalRegion(CPoint point);
	BOOL GetCurrentPosData( CPoint ptPoint);
	void SetCaptureWithFlag();
	void ReleaseCaptureWithFlag();

protected:
	HWND	m_hOcxWnd;
	CPen	m_penLine;

protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB);
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

#endif // !defined(AFX_CROSSHAIRSTOOLADDINIMP_H__6DE63D3A_5E23_4F61_8B80_CC527EF77694__INCLUDED_)
