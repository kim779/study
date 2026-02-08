// ChartDesignPropertyAddInImp.cpp: implementation of the CChartDesignPropertyAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ChartDesignPropertyAddInImp.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../Include_Chart/xUtil_Chart.h"				// for CHART_CONVERT_BEGIN
#include "../Include_Chart/PropertyMap.h"				// for CPropertyMap
#include "../Include_Chart/PropertiesData.h"			// for g_anPropertyShortKey[]

#include "InitializeControlDlg.h"						// for CInitializeControlDlg
#include "ChartPropertySheet.h"							// for CChartPropertySheet
#include "DFontPropPage.h"								// for CDFontPropPage
#include "DlgLocalLanguageTable.h"						// for CDlgLocalLanguageTable

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgGlobalLanguageTable CChartDesignPropertyAddInImp::m_dlgGLT;

CChartDesignPropertyAddInImp::CChartDesignPropertyAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();

	// (2006/7/4 - Seung-Won, Bae) Manage Menu Using Order Flag.
	m_bExeMenuInit = FALSE;
	const char *szPacketInfo = m_pIChartOCX->GetPacketInfoStr();
	if( !szPacketInfo) if( !*szPacketInfo) m_bExeMenuInit = TRUE;
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartDesignPropertyAddInImp::m_strAddInItemName = "DESIGN_PROPERTY";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartDesignPropertyAddInImp)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_CWnd_CPoint)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartDesignPropertyAddInImp::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( nID < ID_DPP_LAYOUT_AND_COLOR || ID_DPP_GML_TABLE < nID ) return FALSE;
	
	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		CCmdUI *pCmdUI = ( CCmdUI *)pExtra;
		if( pCmdUI) switch( nID)
		{
			case ID_DPP_LAYOUT_AND_COLOR:	pCmdUI->Enable( !m_bExeMenuInit);
											break;
			case ID_DPP_PROPERTY_PAGE:
			case ID_DPP_MENU_RESTART:		pCmdUI->Enable( m_bExeMenuInit);
											break;
			case ID_DPP_PROPERTY_FONT:		pCmdUI->Enable( TRUE);
											break;
			case ID_DPP_LML_TABLE:			pCmdUI->Enable( TRUE);			// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
											break;
			case ID_DPP_GML_TABLE:			pCmdUI->Enable( TRUE);			// (2008/2/24 - Seung-Won, Bae) Show Global Meta Language Table
											break;
			default:	return FALSE;
		}
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		switch( nID)
		{
			case ID_DPP_LAYOUT_AND_COLOR:
			{
				CPropertyMap mapProperty;
				m_pIChartOCX->GetAllProperties3( &mapProperty, TRUE);

				//블럭 & 칼라 속성 설정 페이지 띄움
				ML_SET_LANGUAGE_RES();
				CInitializeControlDlg Initdlg( m_pChartCWnd->GetSafeHwnd());
				Initdlg.SetOcxWnd( m_pChartCWnd);
				Initdlg.SetProperty( &mapProperty);
				Initdlg.DoModal();
				if( Initdlg.GetReturnValue() == 1)
				{
					m_pIChartOCX->SetAllProperties3( &mapProperty);
					m_bExeMenuInit = TRUE;

// (2006/8/31 - Seung-Won, Bae) Support Samsung for notifing to MapGen.
// 해당 site관련 Addin Define 삭제 - ojtaso (20090907)
//#ifdef _ADDIN_SS
						HWND hWnd = GetParent( m_pIChartOCX->GetOcxHwnd());
						if( hWnd) ::SendMessage( hWnd, WM_COMMAND, ID_DPP_DESIGN_APPLIED, 0);
//#endif
				}
				break;
			}

			case ID_DPP_PROPERTY_PAGE:
			{
				CPropertyMap mapProperty;
				m_pIChartOCX->GetAllProperties3( &mapProperty, TRUE);

				int nRow = 1, nColumn = 1;
				IPropertyMap *pIPropertyMap = &mapProperty;
				/*const*/ char* szData;
				pIPropertyMap->Lookup( g_anPropertyShortKey[ NBLOCKROWCOUNT], szData);
				if( szData) nRow = atoi( szData);
				pIPropertyMap->Lookup( g_anPropertyShortKey[ NBLOCKCOLUMNCOUNT], szData);
				if( szData) nColumn = atoi( szData);

				ML_SET_LANGUAGE_RES();
				CChartPropertySheet dlgProp( "Property Page", m_pChartCWnd->GetSafeHwnd());
				dlgProp.SetIGraphInfoManager( m_pIGraphInfoManager);
				dlgProp.SetBlockProperty( nRow, nColumn);
				dlgProp.SetProperty( &mapProperty);
				if( dlgProp.DoModal() == IDOK)
				{
					// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
					CString strLocalLanguageTable;
					if( m_pChartCWnd) if( m_pChartCWnd->GetSafeHwnd())
						strLocalLanguageTable = g_iMetaTable.GetLocalLanguageTable( m_pChartCWnd->GetSafeHwnd());
					CHART_ADDTOSTRLIST_START( TRUE)
						CHART_ADDTOSTRLIST_STRING( ( ( IPropertyMap *)&mapProperty), g_anPropertyShortKey[ LOCALLANGUAGETABLE], strLocalLanguageTable, _DPV_LOCALLANGUAGETABLE);
					CHART_ADDTOSTRLIST_END

					m_pIChartOCX->SetAllProperties3( &mapProperty);

// (2006/8/31 - Seung-Won, Bae) Support Samsung for notifing to MapGen.
// 해당 site관련 Addin Define 삭제 - ojtaso (20090907)
//#ifdef _ADDIN_SS
						HWND hWnd = GetParent( m_pIChartOCX->GetOcxHwnd());
						if( hWnd) ::SendMessage( hWnd, WM_COMMAND, ID_DPP_DESIGN_APPLIED, 0);
//#endif
				}
				break;
			}

			case ID_DPP_MENU_RESTART:	m_bExeMenuInit	= FALSE;
										break;

			case ID_DPP_PROPERTY_FONT:
			{
				CPropertyMap mapProperty;
				m_pIChartOCX->GetAllProperties3( &mapProperty, TRUE);

				ML_SET_LANGUAGE_RES();
				CDFontPropPage fontdlg( m_pChartCWnd);
				fontdlg.SetProperty( &mapProperty);
				if( fontdlg.DoModal() == IDOK)
				{
					// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
					CString strLocalLanguageTable;
					if( m_pChartCWnd) if( m_pChartCWnd->GetSafeHwnd())
						strLocalLanguageTable = g_iMetaTable.GetLocalLanguageTable( m_pChartCWnd->GetSafeHwnd());
					CHART_ADDTOSTRLIST_START( TRUE)
						CHART_ADDTOSTRLIST_STRING( ( ( IPropertyMap *)&mapProperty), g_anPropertyShortKey[ LOCALLANGUAGETABLE], strLocalLanguageTable, _DPV_LOCALLANGUAGETABLE);
					CHART_ADDTOSTRLIST_END

					m_pIChartOCX->SetAllProperties3( &mapProperty);

// (2006/8/31 - Seung-Won, Bae) Support Samsung for notifing to MapGen.
// 해당 site관련 Addin Define 삭제 - ojtaso (20090907)
//#ifdef _ADDIN_SS
					HWND hWnd = GetParent( m_pIChartOCX->GetOcxHwnd());
					if( hWnd) ::SendMessage( hWnd, WM_COMMAND, ID_DPP_DESIGN_APPLIED, 0);
//#endif
				}
				break;
			}
			// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
			case ID_DPP_LML_TABLE:
			{
				CPropertyMap mapProperty;
				m_pIChartOCX->GetAllProperties3( &mapProperty, TRUE);

				ML_SET_LANGUAGE_RES();
				CDlgLocalLanguageTable dlgLLT( m_pChartCWnd);
				dlgLLT.SetProperty( &mapProperty);
				if( dlgLLT.DoModal() == IDOK)
				{
					m_pIChartOCX->SetAllProperties3( &mapProperty);
// (2006/8/31 - Seung-Won, Bae) Support Samsung for notifing to MapGen.
// 해당 site관련 Addin Define 삭제 - ojtaso (20090907)
//#ifdef _ADDIN_SS
					HWND hWnd = GetParent( m_pIChartOCX->GetOcxHwnd());
					if( hWnd) ::SendMessage( hWnd, WM_COMMAND, ID_DPP_DESIGN_APPLIED, 0);
//#endif
				}
				break;
			}
			// (2008/2/24 - Seung-Won, Bae) Show Global Meta Language Table
			case ID_DPP_GML_TABLE:
			{
				if( !IsWindow( m_dlgGLT.GetSafeHwnd()))
				{
					ML_SET_DEFAULT_RES();
					m_dlgGLT.Create( IDD_GLOBAL_LANGUAGE_TABLE);
				}
				m_dlgGLT.ShowWindow( SW_SHOW);
				break;
			}

			default:	return FALSE;
		}

		// (2006/7/7 - Seung-Won, Bae) Update OCX UI
		m_pIChartOCX->InvalidateControl();
		return TRUE;
	}

	return FALSE;
}

BOOL CChartDesignPropertyAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, CWnd *p_pWnd, CPoint &point)
{
	if( EAI_ALL_CONTEXT_MENU != p_eCommandID) return FALSE;

	ML_SET_LANGUAGE_RES();

	CMenu menu;
	if( !menu.LoadMenu( IDR_CONTEXT)) return FALSE;

	CMenu * pPopupMenu = ( CMenu*)menu.GetSubMenu(0);
	if( pPopupMenu != NULL)
	{
		CRect rc;
		m_pChartCWnd->GetWindowRect( &rc);
		pPopupMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, m_pChartCWnd);
	}

	return TRUE;
}
