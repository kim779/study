// ChartDataListViewerAddInImp.cpp: implementation of the CChartDataListViewerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "ChartDataListViewerAddInImp.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Addin_134221/I134221/_ICmdUI.h"	// for ICmdUI
#include "DataListViewMngWnd.h"							// for CDataListViewMngWnd 20081025 이문수 >>
#include "DlgDataListViewer.h"							// for CDlgDataListViewer

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartDataListViewerAddInImp::CChartDataListViewerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2006/6/22 - Seung-Won, Bae) Manage DataListViwer Dialog

	//20081025 이문수 >>
	//m_pDataListViewer = NULL; //20081025 이문수 주석처리
	m_pDataListViewMngWnd=NULL;
	//20081025 이문수 <<
}

CChartDataListViewerAddInImp::~CChartDataListViewerAddInImp()
{
	// (2006/6/22 - Seung-Won, Bae) Manage DataListViwer Dialog

	//20081025 이문수 >>
	
	if( m_pDataListViewMngWnd)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if( IsWindow( m_pDataListViewMngWnd->GetSafeHwnd())) 
			m_pDataListViewMngWnd->DestroyWindow();
		delete m_pDataListViewMngWnd;
		m_pDataListViewMngWnd = NULL;
	}
//	if( m_pDataListViewer)
//	{
//		AFX_MANAGE_STATE(AfxGetStaticModuleState());
//		//AfxSetResourceHandle(ML_LANGUAGE_RES);
//
//		 if( IsWindow( m_pDataListViewer->GetSafeHwnd())) m_pDataListViewer->DestroyWindow();
//		 delete m_pDataListViewer;
//		m_pDataListViewer = NULL;
//	}

	//20081025 이문수 <<
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartDataListViewerAddInImp::m_strAddInItemName = "DATA_VIEWER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartDataListViewerAddInImp)
	ONADDINEVENT( OnRecalculationComplete)
	ONADDINEVENT( OnGraphAdded)
	ONADDINEVENT( OnGraphDeleted)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)// 20081007 JS.Kim	사용자시간대 선택
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartDataListViewerAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( nID != ID_DLV_SHOW_DATA_LIST_VIEWER) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( !p_pICmdUI) return FALSE;
		p_pICmdUI->Enable( TRUE);
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		OnToolCommand( CToolOptionInfo::T_DATALIST_REPORT, 1);
		return TRUE;
	}

	return TRUE;
}

void CChartDataListViewerAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( CToolOptionInfo::T_DATALIST_REPORT != p_nToolType) return;

	// (2006/12/29 - Seung-Won, Bae) Support modaless
	if( 0 == p_nCmdOption && (!m_pDataListViewMngWnd || !(m_pDataListViewMngWnd->m_pDataListViewer))) return;

	// (2008/2/22 - Seung-Won, Bae) Do not skip to use m_pChartCWnd
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//20081025 이문수 >>
	if( 0 == p_nCmdOption) 
	{
		if(m_pDataListViewMngWnd)
		{
			if(IsWindow(m_pDataListViewMngWnd->GetSafeHwnd())) 
				m_pDataListViewMngWnd->DestroyWindow();
			delete m_pDataListViewMngWnd;
			m_pDataListViewMngWnd = NULL;
		}
		return;
	}
	
	if(!m_pDataListViewMngWnd)
	{
		m_pDataListViewMngWnd = new CDataListViewMngWnd;
		if(!m_pDataListViewMngWnd->Create(m_pChartCWnd, 2000))
		{
			delete m_pDataListViewMngWnd;
			m_pDataListViewMngWnd = NULL;
		}

		m_pDataListViewMngWnd->SetAddInImp(this);
	}

	if(m_pDataListViewMngWnd && IsWindow(m_pDataListViewMngWnd->GetSafeHwnd()))
	{
		m_pDataListViewMngWnd->CreateDataListView();
	}
	return;
}

// 20081007 JS.Kim	사용자시간대 선택 >>
BOOL CChartDataListViewerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( EAI_USER_CHANGE_TIMEDIFF == p_eCommandID)
	{
		if(m_pDataListViewMngWnd && IsWindow(m_pDataListViewMngWnd->GetSafeHwnd()))
		{
			if(m_pDataListViewMngWnd->m_pDataListViewer && IsWindow(m_pDataListViewMngWnd->m_pDataListViewer->GetSafeHwnd()))
			{
				if(m_pDataListViewMngWnd->IsUpdateReady())
					m_pDataListViewMngWnd->Update();
			}
		}
		return FALSE;
	}
	
	return FALSE;	
}
// 20081007 JS.Kim	사용자시간대 선택 <<

//20081023 이문수 >>
CString CChartDataListViewerAddInImp::GetPacketNameList()
{
	return "";
}
//20081023 이문수 <<


//20081025 이문수 >>
IChartManager *	CChartDataListViewerAddInImp::GetIChartManager()
{
	return m_pIChartManager;
}

IChartOCX *		CChartDataListViewerAddInImp::GetIChartOCX()
{
	return m_pIChartOCX;
}

IPacketManager *CChartDataListViewerAddInImp::GetIPacketManager()
{
	return m_pIDefaultPacketManager;
}

void CChartDataListViewerAddInImp::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if(m_pDataListViewMngWnd && IsWindow(m_pDataListViewMngWnd->GetSafeHwnd()))
	{
		if(m_pDataListViewMngWnd->m_pDataListViewer && IsWindow(m_pDataListViewMngWnd->m_pDataListViewer->GetSafeHwnd()))
		{
			if(m_pDataListViewMngWnd->IsUpdateReady())
				m_pDataListViewMngWnd->Update();
		}
	}
}

// On Graph Deleted.
void CChartDataListViewerAddInImp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if(m_pDataListViewMngWnd && IsWindow(m_pDataListViewMngWnd->GetSafeHwnd()))
	{
		if(m_pDataListViewMngWnd->m_pDataListViewer && IsWindow(m_pDataListViewMngWnd->m_pDataListViewer->GetSafeHwnd()))
		{
			if(m_pDataListViewMngWnd->IsUpdateReady())
				m_pDataListViewMngWnd->Update();
		}
	}
}
//20081025 이문수 <<

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CChartDataListViewerAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	//20081025 이문수 >>
	if( !m_pDataListViewMngWnd) return;
	// (2007/1/2 - Seung-Won, Bae) Check Closed Data List Viewer.
	HWND hWnd = m_pDataListViewMngWnd->GetSafeHwnd();
	if( !hWnd || !IsWindow( hWnd)) return;
	if( !m_pDataListViewMngWnd->m_pDataListViewer) return;
	hWnd = m_pDataListViewMngWnd->m_pDataListViewer->GetSafeHwnd();
	if(!hWnd || !IsWindow(hWnd)) return;

	// (2007/1/18 - Seung-Won, Bae) Check Calculation waiting.
	if( m_pIChartOCX->IsEnableIndicatorCalculation()) OnToolCommand( CToolOptionInfo::T_DATALIST_REPORT, 1);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CChartDataListViewerAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	//20081025 이문수 >>
	if( !m_pDataListViewMngWnd) return;
	// (2007/1/2 - Seung-Won, Bae) Check Closed Data List Viewer.
	HWND hWnd = m_pDataListViewMngWnd->GetSafeHwnd();
	if( !hWnd || !IsWindow( hWnd)) return;
	if( !m_pDataListViewMngWnd->m_pDataListViewer) return;
	hWnd = m_pDataListViewMngWnd->m_pDataListViewer->GetSafeHwnd();
	if(!hWnd || !IsWindow(hWnd)) return;

	m_pDataListViewMngWnd->RealUpdate();		
}

// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
void CChartDataListViewerAddInImp::OnRecalculationComplete( void)
{
	OnPacketDataMultiItem( NULL, NULL, TRUE);
}
