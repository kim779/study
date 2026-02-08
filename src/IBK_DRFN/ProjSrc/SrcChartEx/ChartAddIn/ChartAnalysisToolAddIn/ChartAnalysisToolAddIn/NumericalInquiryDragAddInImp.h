// NumericalInquiryDragAddInImp.h: interface for the CNumericalInquiryDragAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMERICALINQUIRYDRAGADDINIMP_H__3B7175EC_1113_47C9_89E4_0039BAEF2FCB__INCLUDED_)
#define AFX_NUMERICALINQUIRYDRAGADDINIMP_H__3B7175EC_1113_47C9_89E4_0039BAEF2FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"		// for Ixxxxxx.h

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/28
// Comments
//
//	1. Tool Type with Crosshairs and Numerical Inquiry DLG (NIDLG) with Z-Order
//			NIDLG_DRAG				->
//			CROSS_DRAG				->	}-	It can be setted with Environment Setting.
//												and Only by Environment Setting for Env-Variable UI Update.
//												So, Tool Setting process Environment Setting.
//											These are not analysis tool.
//												So. Thease are not released with T_TOOL_SELECTION.
//
//			CrossLine Object Tool	->	}-	This is analysis tool.
//												So. Thase are released with T_TOOL_SELECTION or Other Tool ID or R-button Menu.
//			NIDLG_TOOL				->
//			CROSS_TOOL				->	}-	These are not analysis tool.
//												But these are used like analysis tool.
//												So. these are released with T_TOOL_SELECTION or Other Tool ID or R-button Menu.
//
//			NIDLG_OVER				->
//			CROSS_OVER				->	}-	These are not analysis tool.
//												So. Thease are not released with T_TOOL_SELECTION.
//
//	2. AddIn Object and Co-Operations.
//			AddIns
//				ANALYSIS_TOOL	: CrossLine and etc Analysis Object Tool (CAnalysisAddInImp)
//				NIDLG_DRAG		: CNumericalInquiryDragAddInImp
//				NIDLG_TOOL		: CNumericalInquiryToolAddInImp
//				NIDLG_OVER		: CNumericalInquiryOverAddInImp
//				CROSS_DRAG		: CCrosshairsDragAddInImp
//				CROSS_TOOL		: CCrosshairsToolAddInImp
//				CORSS_OVER		: CCrosshairsOverAddInImp
//			Co-Operations
//				EAI_NIDLG_DRAG_SHOWHIDE	/ EAI_NIDLG_DRAG_ONMOUSEMOVE
//					- From		: CROSS_DRAG, CROSS_TOOL, ANALYSIS_TOOL
//					- To		: NIDLG_DRAG
//					- Object	: for the Option 'with NIDLG'
//									CROSS_DRAG		~ Drag Crosshairs with NIDLG
//									CROSS_TOOL		~ Crosshairs Tool with NIDLG
//									ANALYSIS_TOOL	~ CrossLine Object Tool with NIDLG
//														Caution! for Using 'CrossLine Object Tool with NIDLG' and Over NIDLG in same time,
//															CrossLine Object Tool does not use the NIDLG_TOOL but use the NIDLG_DRAG.
//															Using NIDLG_DRAG make it possible to prevent the NIDLG_DRAG in NIDLG_TOOL Activated.
//				EAI_NIDLG_DRAG_OVER_ACTIVATED
//					- From		: NIDLG_OVER
//					- To		: NIDLG_DRAG, NIDLG_TOOL
//					- Object	: for preventing the NIDLG_DRAG and the NIDLG_TOOL in NIDLG_OVER Activated.
//				EAI_CROSSHAIRS_DRAG_OVER_ACTIVATED
//					- From		: CROSS_OVER
//					- To		: CROSS_DRAG
//					- Object	: for preventing the CROSS_DRAG in CROSS_OVER Activated.
//				EAI_CROSSHAIRS_OVER_CROSSLINE_ACTIVATED
//					- From		: ANALYSIS_TOOL
//					- To		: CROSS_OVER
//					- Object	: for preventing the CROSS_OVER in ANALYSIS_TOOL's CrossLine Object Mode Activated.
//				EAI_CROSSHAIRS_OVER_CROSSHAIRS_TOOL_ACTIVATED
//					- From		: CROSS_TOOL
//					- To		: CROSS_OVER
//					- Object	: for preventing the CROSS_OVER in CROSS_TOOL Activated.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/7
// Comments		: Numerical Inquiry Cursor AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CInquiryDataWindowDlg;
class CNumericalInquiryDragAddInImp : public CChartAddInBase
{
public:
	CNumericalInquiryDragAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CNumericalInquiryDragAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// save point for inflate when mouse drag & drop
protected:
	CPoint	m_ptStartDragDrop;
// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
protected:
	DWORD	m_dwMouseActionID;

// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed
protected:
	BOOL	m_bOnToolExecuted;
// (2006/6/28 - Seung-Won, Bae) Manage Notify from Over-NIDLG to Drag-NIDLG for avoiding the Multiple NIDLG Active.
protected:
	BOOL	m_bOverActivated;	
// (2006/7/3 - Seung-Won, Bae) Manage the Flag of Sub Tool Executed for Restarting in Over Closing.
protected:
	BOOL	m_bOnSubToolExecuted;
	
// (2006/6/27 - Seung-Won, Bae) Manage Numerical Inquiry Dialog
protected:
	CInquiryDataWindowDlg *	m_pNumericalInquiryDlg;	// 수치조회창 다이얼로그.
	CInquiryDataWindowDlg *	m_pTracingToolTipDlg;	// TracingToolTip 다이얼로그.
	HWND	m_hOcxWnd;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
	virtual void	OnResetMainBlock( void);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_NUMERICALINQUIRYDRAGADDINIMP_H__3B7175EC_1113_47C9_89E4_0039BAEF2FCB__INCLUDED_)
