// NumericalInquiryToolAddInImp.cpp: implementation of the CNumericalInquiryOverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NumericalInquiryOverAddInImp.h"
#include "InquiryDataWindowDlg.h"							// for CInquiryDataWindowDlg

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_ICmdUI.h"		// for ICmdUI

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumericalInquiryOverAddInImp::CNumericalInquiryOverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);

	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed for Capture/Path on MouseMove
	m_bOnToolExecuted = FALSE;

	// (2006/6/27 - Seung-Won, Bae) Manage Numerical Inquiry Dialog with DLL Resource
	ML_SET_LANGUAGE_RES();
	m_pNumericalInquiryDlg = new CInquiryDataWindowDlg( p_pIChartOCX);
}

CNumericalInquiryOverAddInImp::~CNumericalInquiryOverAddInImp()
{
	// (2006/6/27 - Seung-Won, Bae) Manage Numerical Inquiry Dialog with DLL Resource
	ML_SET_LANGUAGE_RES();
	if( m_pNumericalInquiryDlg) delete m_pNumericalInquiryDlg;
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CNumericalInquiryOverAddInImp::m_strAddInItemName = "NIDLG_OVER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CNumericalInquiryOverAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnResetMainBlock)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
END_ADDIN_EVENT_MAP()


// (2006/6/28 - Seung-Won, Bae) Notify to Drag-NIDLG for avoiding the Multiple NIDLG Active.
void CNumericalInquiryOverAddInImp::ShowNumericalInquiryDlg( const BOOL p_bShow)
{
	if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( p_bShow);
	m_pIAddInToolMgr->OnAddInToolCommand( EAI_NIDLG_DRAG_OVER_ACTIVATED, p_bShow);
}

void CNumericalInquiryOverAddInImp::OnResetChart( void)	
{
	// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
	if( m_pNumericalInquiryDlg) if( m_pNumericalInquiryDlg->IsExportMode()) return;

	m_bOnToolExecuted = FALSE;
	m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_NIDLG_OVER,FALSE);
	ShowNumericalInquiryDlg( m_bOnToolExecuted);
} 

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CNumericalInquiryOverAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
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
void CNumericalInquiryOverAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	// 종찬(05/08/18) 수지조회창의 레이아웃 안그려지는 버그 수정용.
	if( m_pNumericalInquiryDlg) if( m_pNumericalInquiryDlg->GetSafeHwnd())
		m_pNumericalInquiryDlg->RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CNumericalInquiryOverAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	if( m_pNumericalInquiryDlg) if( m_pNumericalInquiryDlg->GetSafeHwnd()) m_pNumericalInquiryDlg->Refresh();
}
// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CNumericalInquiryOverAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( m_pNumericalInquiryDlg) if( m_pNumericalInquiryDlg->GetSafeHwnd()) m_pNumericalInquiryDlg->Refresh();
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CNumericalInquiryOverAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( ID_NIA_NIDLG_OVER_EXECUTE != nID) return FALSE;
	
	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( p_pICmdUI)
		{
			p_pICmdUI->Enable( TRUE);
			p_pICmdUI->SetCheck( m_bOnToolExecuted);
		}
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_NIDLG_OVER,!m_bOnToolExecuted);
		OnToolCommand( CToolOptionInfo::T_NIDLG_OVER, !m_bOnToolExecuted);
		return TRUE;	// for Other AddIn with AnalysisTool
	}

	return FALSE;
}

// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
void CNumericalInquiryOverAddInImp::OnResetMainBlock( void)
{
	if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->SetNumericalInquiryDlgBrush( m_pIChartOCX->GetStyleValueNIDLGColor());
}

BOOL CNumericalInquiryOverAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID)
{
	if( EAI_ALL_APPLY_STYLE == p_eCommandID) if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->SetNumericalInquiryDlgBrush( m_pIChartOCX->GetStyleValueNIDLGColor());
	return FALSE;
}

BOOL CNumericalInquiryOverAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	// (2006/6/24 - Seung-Won, Bae) Manage the Numerical Inquiry Data Queryer with HorizontalLine Option.
	if( EAI_NIDLG_ALL_ENABLE_HORIZONTAL_LINE == p_eCommandID && m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->EnableHorizontalLineInNIDLG( p_lParam);
	return FALSE;
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CNumericalInquiryOverAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{
		m_bOnToolExecuted = FALSE;
		m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_NIDLG_OVER,FALSE);
		ShowNumericalInquiryDlg( m_bOnToolExecuted);
		return;
	}
	
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		if( m_bOnToolExecuted == TRUE)
			m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_NIDLG_OVER,TRUE);
		return;
	}
	
	if( CToolOptionInfo::T_NIDLG_OVER != p_nToolType) return;

	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed for Capture/Path on MouseMove
	m_bOnToolExecuted = p_nCmdOption;
	ShowNumericalInquiryDlg( m_bOnToolExecuted);
}

BOOL CNumericalInquiryOverAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart
	// (2006/2/16 - Seung-Won, Bae) Manage the Flag of Tool Executed for Capture/Path on MouseMove
	if( !m_bOnToolExecuted || m_pIChartManager->IsEmpty()) return FALSE;
	if( m_pNumericalInquiryDlg) m_pNumericalInquiryDlg->ShowNumericalInquiryDlg( CDC::FromHandle( p_hDC), point);

	return TRUE;
	//return TRUE; TRUE를 하면 하위 항목이 MouseMove를 타지 않는다 
}
