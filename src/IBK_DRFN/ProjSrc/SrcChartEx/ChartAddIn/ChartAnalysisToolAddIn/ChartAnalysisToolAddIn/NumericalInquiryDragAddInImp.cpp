// NumericalInquiryDragAddInImp.cpp: implementation of the CNumericalInquiryDragAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NumericalInquiryDragAddInImp.h"
#include "InquiryDataWindowDlg.h"							// for CInquiryDataWindowDlg
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumericalInquiryDragAddInImp::CNumericalInquiryDragAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);

	// save point for inflate when mouse drag & drop
	// 09.21.2001
	m_ptStartDragDrop = CPoint(-1, -1);

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed
	m_bOnToolExecuted		= FALSE;
	// (2006/6/28 - Seung-Won, Bae) Manage Notify from Over-NIDLG to Drag-NIDLG for avoiding the Multiple NIDLG Active.
	m_bOverActivated		= FALSE;
	
	// (2006/6/27 - Seung-Won, Bae) Manage Numerical Inquiry Dialog with DLL Resource
	ML_SET_LANGUAGE_RES();
	m_pNumericalInquiryDlg = new CInquiryDataWindowDlg( p_pIChartOCX, m_pChartCWnd);
	
	// (2006/7/3 - Seung-Won, Bae) Manage the Flag of Sub Tool Executed for Restarting in Over Closing.
	m_bOnSubToolExecuted = FALSE;	
}

CNumericalInquiryDragAddInImp::~CNumericalInquiryDragAddInImp()
{
	// (2006/6/27 - Seung-Won, Bae) Manage Numerical Inquiry Dialog with DLL Resource
	ML_SET_LANGUAGE_RES();
	if( m_pNumericalInquiryDlg) delete m_pNumericalInquiryDlg;	
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CNumericalInquiryDragAddInImp::m_strAddInItemName = "NIDLG_DRAG";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CNumericalInquiryDragAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnResetMainBlock)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CNumericalInquiryDragAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
	if( 1030 == p_nCommandType && m_pNumericalInquiryDlg)
	{
		m_pNumericalInquiryDlg->SetExportMode( ( HWND)p_lData);
		return TRUE;
	}
	return FALSE;
}

// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CNumericalInquiryDragAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	// 종찬(05/08/18) 수지조회창의 레이아웃 안그려지는 버그 수정용.
	if( m_pNumericalInquiryDlg && !m_bOverActivated) if( m_pNumericalInquiryDlg->GetSafeHwnd())
		m_pNumericalInquiryDlg->RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);		
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CNumericalInquiryDragAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	if( m_pNumericalInquiryDlg && !m_bOverActivated) if( m_pNumericalInquiryDlg->GetSafeHwnd()) m_pNumericalInquiryDlg->Refresh();	
}
// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CNumericalInquiryDragAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( m_pNumericalInquiryDlg && !m_bOverActivated) if( m_pNumericalInquiryDlg->GetSafeHwnd()) m_pNumericalInquiryDlg->Refresh();	
}

void CNumericalInquiryDragAddInImp::OnResetChart( void)	
{
	if(m_bOnSubToolExecuted == FALSE) return;

	m_bOnSubToolExecuted = FALSE;	
	m_bOverActivated = FALSE;
	if( m_pNumericalInquiryDlg && !m_bOverActivated) 
		m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( m_bOnSubToolExecuted);	
} 
//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

void CNumericalInquiryDragAddInImp::OnResetMainBlock( void)
{
	if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->SetNumericalInquiryDlgBrush( m_pIChartOCX->GetStyleValueNIDLGColor());
}

BOOL CNumericalInquiryDragAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID)
{
	if( EAI_ALL_APPLY_STYLE == p_eCommandID) if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->SetNumericalInquiryDlgBrush( m_pIChartOCX->GetStyleValueNIDLGColor());
	return FALSE;
}

BOOL CNumericalInquiryDragAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( EAI_NIDLG_DRAG_ONMOUSEMOVE != p_eCommandID) return FALSE;
//////////////////////////////////////////////////////////////////////
// (2006/6/28 - Seung-Won, Bae) NIDLG Move with Other Control
//////////////////////////////////////////////////////////////////////
	if( m_pNumericalInquiryDlg && !m_bOverActivated) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( CDC::FromHandle( p_hDC), point);	
	return TRUE;
}

BOOL CNumericalInquiryDragAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart
	if( m_pIChartManager->IsEmpty()) return FALSE;

	switch( p_eCommandID)
	{
		case EAI_ALL_ON_CHANGE_CURSOR_MODE:
			// (2006/2/13 - Seung-Won, Bae) Manage the Flag of Environment Setting
			m_bOnToolExecuted = ( p_lParam & NUMERICALINQUIRYDLG) && !( p_lParam & CROSSLINEVIEW);
			if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->OnNumericalInquiryOptionChanged( p_lParam);
			//		Return False for Next Cursor Mode Handler
			return FALSE;

		// (2006/6/28 - Seung-Won, Bae) Notify from Over-NIDLG to Drag-NIDLG for avoiding the Multiple NIDLG Active.
		case EAI_NIDLG_DRAG_OVER_ACTIVATED:	m_bOverActivated		= p_lParam;
											
											if( m_pNumericalInquiryDlg) 
												m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( !m_bOverActivated && ( m_bOnToolExecuted || m_bOnSubToolExecuted));
											return FALSE;		// for NIDLG_TOOL
			
		case EAI_NIDLG_DRAG_SHOWHIDE:	// (2006/7/3 - Seung-Won, Bae) Manage the Flag of Sub Tool Executed for Restarting in Over Closing.
										m_bOnSubToolExecuted = ( 1 == p_lParam);
											
										//////////////////////////////////////////////////////////////////////
										// (2006/6/28 - Seung-Won, Bae) NIDLG Show/Hide with Other Control
										//////////////////////////////////////////////////////////////////////
										if( m_pNumericalInquiryDlg && !m_bOverActivated) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( m_bOnSubToolExecuted);			
										break;

		// (2006/6/24 - Seung-Won, Bae) Manage the Numerical Inquiry Data Queryer with HorizontalLine Option.
		case EAI_NIDLG_ALL_ENABLE_HORIZONTAL_LINE:	if( m_pNumericalInquiryDlg) 
														m_pNumericalInquiryDlg->EnableHorizontalLineInNIDLG( p_lParam);
													return FALSE;

		default:	return FALSE;
	}

	return TRUE;
}

BOOL CNumericalInquiryDragAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{ 
	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		// (2007/2/14 - Seung-Won, Bae) In Win2000, if m_ptStartDragDrop == point, DragZoom takes start.
		//	So, m_ptStartDragDrop == point Checking makes loosing the chance to GeneralStart.
		//	if( m_ptStartDragDrop == point)	return FALSE;			// It is not moved.

		if( p_dwMouseActionID != m_dwMouseActionID) return FALSE;	// It is not one action.

		if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
//////////////////////////////////////////////////////////////////////
// (2006/6/28 - Seung-Won, Bae) NIDLG Show with Mouse Action.
//////////////////////////////////////////////////////////////////////
		if( m_pNumericalInquiryDlg && !m_bOverActivated) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( TRUE);		
	}

//////////////////////////////////////////////////////////////////////
// (2006/6/28 - Seung-Won, Bae) NIDLG Move with Mouse Action.
//////////////////////////////////////////////////////////////////////
	if( m_pNumericalInquiryDlg && !m_bOverActivated) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( CDC::FromHandle( p_hDC), point);	
	return TRUE;
}

BOOL CNumericalInquiryDragAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_bOnToolExecuted || m_pIChartManager->IsEmpty() || m_pIChartOCX->IsOnSpecialChart()
		|| CRect(0, 0, 0, 0) == m_pIChartManager->GetGraphRegionSumInColumn( point)) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;
	m_ptStartDragDrop = point;

	return FALSE;	// return FALSE. OnLButtonDown is not start time of this tool.
}

BOOL CNumericalInquiryDragAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;

	m_ptStartDragDrop = CPoint(-1, -1);

//////////////////////////////////////////////////////////////////////
// (2006/6/28 - Seung-Won, Bae) NIDLG Hide with Mouse Action.
//////////////////////////////////////////////////////////////////////
	if( m_pNumericalInquiryDlg && !m_bOverActivated) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( FALSE);	
	return TRUE;
}
