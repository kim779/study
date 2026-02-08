// SymbolAddInImp.cpp: implementation of the CSymbolAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartAnalysisToolAddIn.h"
#include "SymbolAddInImp.h"

#include "../Include_Analysis/DLL_Load/SymbolHelper.h"				// for CSymbolHelper
#include "../Include_Chart/EnvironmentData.h"						// for CStyleData
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"	// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_ICmdUI.h"				// for ICmdUI

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR *const G_ANALYSIS_PROPERTY_INFO	= "AnalysisPropInfo.ans";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSymbolAddInImp::CSymbolAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	m_eTimeType = CScaleBaseData::HORZ_NOTYPE;

	// (2006/2/2 - Seung-Won, Bae) Initialize Symbol AddIn Tool
	CClientDC dc( m_pChartCWnd);
	// (2003.12.23, 배승원) Resource Manager 적용을 개선한다.
	HFONT hOldFont = ( HFONT)::SelectObject( dc.GetSafeHdc(), m_pIChartOCX->GetChartFont());
	// (2003.12.23, 배승원) DLL을 SK Common Folder로 옮긴다.
	ILPCSTR szHtsPath = m_pIChartOCX->GetOCXPathString();
	m_pSymbolHelper = new CSymbolHelper( szHtsPath);
	m_pSymbolHelper->SetMainBlock( m_pIChartManager,p_pIChartOCX);
	m_pSymbolHelper->Initialize( &dc, m_pChartCWnd);

	// (2009/9/2 - Seung-Won, Bae) for Image Path
	ILPCSTR szImagePath = m_pIChartOCX->GetImagePathString();
	CString strImagePath( szImagePath);
	m_pSymbolHelper->SetDeskFilePath( strImagePath);

	m_pSymbolHelper->SetColor( m_pIChartOCX->GetStyleValueAnalysisToolColor());
	::SelectObject( dc.GetSafeHdc(), hOldFont);

	m_ptOldMouse = CPoint(-1,-1);
	// (2006/3/4 - Seung-Won, Bae) Init Default Symbol Type
	m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_SELECTION);

	// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
	m_bOnAllMouseExclusive = FALSE;

	ILPCSTR szUserPath = p_pIChartOCX->GetUserFilePathString();
	m_strUserPath = szUserPath;
	m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;

	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);
}

CSymbolAddInImp::~CSymbolAddInImp()
{
	// SymbolHelper
	if( m_pSymbolHelper)
	{
		delete m_pSymbolHelper;
		m_pSymbolHelper = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CSymbolAddInImp::m_strAddInItemName = "SYMBOL_TOOL";
CString CSymbolAddInImp::m_strToolAddInItemName = "ANALYSIS_TOOL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSymbolAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnBlockChanged)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnCmdMsg)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnRButtonDown)
	ONADDINEVENT( OnRButtonUp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CSymbolAddInImp::OnDrawAfterGraph( HDC p_hDC)
{
	if( m_pSymbolHelper) m_pSymbolHelper->OnDraw( CDC::FromHandle( p_hDC));
}

RECT CSymbolAddInImp::DrawCursor(HDC p_hDC)
{
	RECT rct = { 0, 0, 0, 0};	

	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
	{
		CRect rect = m_pSymbolHelper->GetGraphTotalRegion(m_ptOldMouse);
		m_pSymbolHelper->DrawTool( CDC::FromHandle( p_hDC));
		rct = rect;
	}
	return rct;
}

void CSymbolAddInImp::OnResetChart(void)
{
	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
	{
		m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);

		// 툴바도 해제한다
		//m_pIChartOCX->ReleaseOtherToolUI( ( CToolOptionInfo::TOOLOPTION)-1);

		if(nType == CSymbol::SYMBOL_BULET)
			m_pSymbolHelper->DestroySymbolDlg();

		// 연속그리기 해제
		m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_SELECTION);					

		// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}

	m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;
}
//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CSymbolAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( !m_pSymbolHelper) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		switch( nID)
		{
			case ID_CSA_CONFIG:
			case ID_CSA_COPY:
			case ID_CSA_DELETE:
			case ID_CSA_SQUARE:
			case ID_CSA_CIRCLE:
			case ID_CSA_TRIANGLE:
			case ID_CSA_RELEASE_SERIESDRAW:
			{
				if( p_pICmdUI) p_pICmdUI->Enable( TRUE);
				break;
			}
		
			default: return FALSE;
		}

		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		ML_SET_LANGUAGE_RES();				// to Load Menu in this Module
		switch( nID)
		{
			case ID_CSA_CONFIG:	m_pSymbolHelper->OnMenu( CMenuType::MENU_SET, TRUE);
								break;
			case ID_CSA_DELETE:	m_pSymbolHelper->OnMenu( CMenuType::MENU_DELETE, TRUE);
								break;
			case ID_CSA_COPY:	m_pSymbolHelper->SetSymbolCopy();
								break;
			case ID_CSA_SQUARE:	m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_SQUARE);
								break;
			case ID_CSA_CIRCLE:	m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_CIRCLE);
								break;
			case ID_CSA_TRIANGLE:	m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_TRIANGLE);
									break;
			case ID_CSA_RELEASE_SERIESDRAW:
			{
				CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
				if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
				{
					m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);

					// 툴바도 해제한다
					//m_pIChartOCX->ReleaseOtherToolUI( ( CToolOptionInfo::TOOLOPTION)-1);

					if(nType == CSymbol::SYMBOL_BULET)
						m_pSymbolHelper->DestroySymbolDlg();

					// 연속그리기 해제
					m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_SELECTION);					

					// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
					m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
				}

				m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;
				break;
			}

			default:	return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CSymbolAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	if( !m_pSymbolHelper) 
		return FALSE;

	switch( p_eCommandID)
	{
		case EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR:
		{
			COLORREF clrAnalysisTool = m_pIChartOCX->GetStyleValueAnalysisToolColor();
			COLORREF clrAnalysisText = m_pIChartOCX->GetStyleValueAnalysisToolColor();//TextColor
			m_pSymbolHelper->SetShapeDrawingToolType(clrAnalysisTool,PS_SOLID,-1,true);
			m_pSymbolHelper->ChangeAllElementsColor( clrAnalysisTool, clrAnalysisText);
			return FALSE;		// EAI_ALL_...
		}
		case EAI_ALL_APPLY_STYLE:
		{
			m_pSymbolHelper->SetColor( m_pIChartOCX->GetStyleValueAnalysisToolColor());
			return FALSE;
		}
		default:	
			return FALSE;
	}
	return TRUE;
}

void CSymbolAddInImp::OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
{
	if( !m_pSymbolHelper) return;

	switch( p_eChangeType)
	{
		case CBlockBaseData::BLOCK_INSERT:	m_pSymbolHelper->InsertElementTree( CBlockIndex( p_nSourceRow, p_nSourceColumn));
											break;
		case CBlockBaseData::BLOCK_MOVE:	m_pSymbolHelper->MoveElementTree( CBlockIndex( p_nSourceRow, p_nSourceColumn), CBlockIndex( p_nTargetRow, p_nTargetColumn));
											break;
		case CBlockBaseData::BLOCK_DELETE:	m_pSymbolHelper->DeleteAllElementInBlock( CBlockIndex( p_nSourceRow, p_nSourceColumn));
											break;
	}
}

void CSymbolAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( !m_pSymbolHelper) return;

	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_RESET_STATE)
	{				
		if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
		{
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);

			// 툴바도 해제한다
			//m_pIChartOCX->ReleaseOtherToolUI( ( CToolOptionInfo::TOOLOPTION)-1);

			if(nType == CSymbol::SYMBOL_BULET)
				m_pSymbolHelper->DestroySymbolDlg();

			// 연속그리기 해제
			m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_SELECTION);					

			// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
			m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
		}

		m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;
		return;
	}
	
	if(p_nToolType == CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY)
	{
		//if( m_eToolOption != CToolOptionInfo::T_TOOL_SELECTION  &&
		//	m_eToolOption != CToolOptionInfo::T_DELETE_ALL		&&
		//	m_eToolOption != CToolOptionInfo::T_TOOL_TRACEING_TT && 
		//	m_eToolOption != CToolOptionInfo::T_NIDLG_OVER		&&
		//	m_eToolOption != CToolOptionInfo::T_DELETE_INORDER	&&
		//	m_eToolOption != CToolOptionInfo::T_REMOVE_OBJ)
		if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,TRUE);
		return;
	}
	
	if( p_nToolType == CToolOptionInfo::T_TOOL_TRACEING_TT || 
		p_nToolType == CToolOptionInfo::T_NIDLG_OVER)
		return;

	if( p_nToolType == CToolOptionInfo::T_WHOLEVIEW)
		return;

	if( p_nToolType == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT ||
		p_nToolType == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT ||
		p_nToolType == CToolOptionInfo::T_ADDIN_AUTOSCROLL_STOP )
		return;

	//내 스스로가 선택되었다는 것을 알린다 - 나는 REturn
	int nElementType = (int)p_nCmdOption;
	if( p_nToolType == CToolOptionInfo::T_TOOL_SELECTION &&
		m_pSymbolHelper->GetSelectToolType() == (nElementType-20000))
		return;
				
	if( p_nToolType == CToolOptionInfo::T_DELETE_INORDER || 
		p_nToolType == CToolOptionInfo::T_REMOVE_OBJ)
	{
		if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,TRUE);
		return;
	}

	if(p_nToolType == CToolOptionInfo::T_DELETE_ALL)
	{
		if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)
			m_pIChartOCX->OnToolStateChanged(m_eToolOption,TRUE);

		// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
		//		Delete with checking current state.
		//		See CKoscomChartCtrl::SetPacketData()
		if( 820 != p_nCmdOption)
		{
			m_pSymbolHelper->DeleteAllSymbol();
			return;
		}
		// 1. Check the ItemCode and time period.
		CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType = m_pIDefaultPacketManager->GetBaseTimeDataType();
		LPCTSTR szItemCode = NULL, szItemName = NULL;
		m_pIPacketListManager->GetItemCode( "DEFAULT", szItemCode, szItemName);
		CString strItemCode( szItemCode);
		if( m_eTimeType == eTimeType && m_strItemCode == strItemCode) return;
		m_pSymbolHelper->DeleteAllSymbol();
		m_eTimeType = eTimeType;
		m_strItemCode = strItemCode;
		return;
	}

	CSymbol::SYMBOLTOOL nSymbol;	
	m_eToolOption = ( CToolOptionInfo::TOOLOPTION)p_nToolType;
	switch( m_eToolOption)
	{
		case CToolOptionInfo::T_SYMBOL_BULET:	nSymbol = CSymbol::SYMBOL_BULET;
												LoadData(nSymbol);
												break;

		case CToolOptionInfo::T_SYMBOL_SQUARE:	nSymbol = CSymbol::SYMBOL_SQUARE;
												LoadData(nSymbol);
												break;

		case CToolOptionInfo::T_SYMBOL_CIRCLE:	nSymbol = CSymbol::SYMBOL_CIRCLE;
												LoadData(nSymbol);
												break;

		case CToolOptionInfo::T_SYMBOL_TRIANGLE:nSymbol = CSymbol::SYMBOL_TRIANGLE;
												LoadData(nSymbol);
												break;

		case CToolOptionInfo::T_SYMBOL_TEXT:	nSymbol = CSymbol::SYMBOL_TEXT;
												LoadData(nSymbol);
												break;

		//case CToolOptionInfo::T_DELETE_INORDER:break;
		//case CToolOptionInfo::T_DELETE_ALL:	//모두지우기
		//									nSymbol = CSymbol::SYMBOL_SELECTION;
		//									m_pSymbolHelper->DeleteAllSymbol();
		//									m_pIChartOCX->InvalidateControl();
		//									break;

		//case CToolOptionInfo::T_REMOVE_OBJ:	nSymbol = CSymbol::SYMBOL_SELECTION;
		//									m_pSymbolHelper->DeleteCurrentSymbol();
		//									m_pIChartOCX->InvalidateControl();
		//									break;

		case CToolOptionInfo::T_TOOL_SELECTION:	// (2006/7/3 - Seung-Won, Bae) Release Tool in other Tool Type.
		default:							nSymbol = CSymbol::SYMBOL_SELECTION;
											break;
	}

	// 업데이트 필요
	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart with New Tool.
	if( p_nCmdOption == 0 || m_pIChartManager->IsEmpty()) 
		nSymbol = CSymbol::SYMBOL_SELECTION;

	m_pSymbolHelper->SetCurrentSymbolType( nSymbol);			
	if( nSymbol == CSymbol::SYMBOL_BULET) 
	{
		m_pSymbolHelper->LoadSymbolDlg();
	}
	else
	{
		m_pSymbolHelper->DestroySymbolDlg();
		m_pSymbolHelper->SetCurrentSymbolType( nSymbol);
	}

	// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
	if( CSymbol::SYMBOL_SELECTION != nSymbol && CSymbol::SYMBOL_NONE != nSymbol)
		m_pIAddInToolMgr->GetAllMouseExclusive( m_pIAddInDllBase);
	else
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
}

BOOL CSymbolAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( !m_pSymbolHelper) return FALSE;

	switch( p_eCommandID)
	{
		case EAI_BAS_OCXELEMENTMENU:	return m_pSymbolHelper->OnMenu( ( CMenuType::ELEMENTMENU)p_lParam1, p_lParam2);
	}
	return FALSE;	// for Crosshairs and NIDLG Tool Release
}

BOOL CSymbolAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	// 20081007 JS.Kim	사용자시간대 선택
	if( !m_pSymbolHelper ) 
		return FALSE;
 
	switch( p_eCommandID)
	{
		case EAI_TOOLORDERMGR_DELETE_THIS_TOOL:	
			if( strcmp(p_szParam,m_strToolAddInItemName) == 0 )
				return m_pSymbolHelper->DeleteElementPos(p_lParam);
		case EAI_UNDO_REDO_MANAGER_UNDO_THIS_WORK:
			if( strcmp(p_szParam,m_strToolAddInItemName) == 0 )
				return m_pSymbolHelper->UndoElementPos(p_lParam);
		case EAI_UNDO_REDO_MANAGER_REDO_THIS_WORK:
			if( strcmp(p_szParam,m_strToolAddInItemName) == 0 )
				return m_pSymbolHelper->RedoElementPos(p_lParam);
		// 20081007 JS.Kim	사용자시간대 선택
		case EAI_USER_CHANGE_TIMEDIFF:	
			m_pSymbolHelper->RecalcElementXPoint(p_szParam, (int)p_lParam);
			return FALSE;		// 다른 AddIn에서도 처리할 수 있도록 FALSE를 return한다.
	}
	
	return FALSE;	
}

BOOL CSymbolAddInImp::LoadData(int nTool)
{
	CString strFileName;
	strFileName.Format("%s%s",m_strUserPath,G_ANALYSIS_PROPERTY_INFO);
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	finder.Close();
	
	COLORREF crDefault = m_pIChartOCX->GetStyleValueAnalysisToolColor();
	if(bFind == FALSE)
	{
		m_pSymbolHelper->SetShapeDrawingToolType(crDefault,PS_SOLID,1,true);
		return FALSE;
	}
		
	CString strSection,strKey;	
	if(nTool == CSymbol::SYMBOL_SQUARE)
		nTool = 101;	
	else if(nTool == CSymbol::SYMBOL_CIRCLE)
		nTool = 102;	
	else if(nTool == CSymbol::SYMBOL_TRIANGLE)
		nTool = 103;	
	
	strSection.Format("%d",nTool);
	strKey = _T("LineColor");
	COLORREF crLineColor = (COLORREF)GetPrivateProfileInt(strSection,strKey,crDefault,strFileName);

	strKey = _T("LineStyle");
	int nLineStyle = GetPrivateProfileInt(strSection,strKey,PS_SOLID,strFileName);

	strKey = _T("LineWeight");
	int nLineWeight = GetPrivateProfileInt(strSection,strKey,0,strFileName);

	strKey = _T("FillRegion");
	int nFillRegion = GetPrivateProfileInt(strSection,strKey,1,strFileName);
	
	m_pSymbolHelper->SetShapeDrawingToolType(crLineColor,nLineStyle,nLineWeight,nFillRegion == 1);
	return TRUE;
}

BOOL CSymbolAddInImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( VK_DELETE == nChar && m_pSymbolHelper) m_pSymbolHelper->DeleteCurrentSymbol();
	return FALSE;
}

BOOL CSymbolAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)
{
	m_ptOldMouse = point;
	if( !m_pSymbolHelper) return FALSE;

	// (2006/2/15 - Seung-Won, Bae) Do not return FALSE to deny to select other object in Bulet Tool.
	if( CSymbol::SYMBOL_BULET == m_pSymbolHelper->GetCurrentToolType())	return TRUE;

	return m_pSymbolHelper->OnMouseMove( CDC::FromHandle( p_hDC), nFlags, point);
}

BOOL CSymbolAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	m_bLButtonDBClk = FALSE;
	if( !m_pSymbolHelper) return FALSE;

	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	if(nType == CSymbol::SYMBOL_BULET)
	{
		if(m_pSymbolHelper->IsExistSymbolDlg() == FALSE)
		{
			if(m_pIAddInToolMgr->GetAllMouseExclusive(m_pIAddInDllBase) == TRUE)
				m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);	
			
			m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_SYMBOL_BULET,FALSE);
		}
		else
		{
			m_pIChartOCX->OnToolStateChanged(CToolOptionInfo::T_SYMBOL_BULET,TRUE);
		}
	}

	if( !m_pSymbolHelper->OnLButtonDown( CDC::FromHandle( p_hDC), point)) return FALSE;

	// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
	m_bOnAllMouseExclusive = m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase);
	if( !m_bOnAllMouseExclusive) m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase);
	
	// (2006/2/1 - Seung-Won, Bae) Release Other Tools Selection
	m_pIAddInToolMgr->OnAddInToolCommand( EAI_ALL_RELEASE_SELECTION_EXCEPT_ONE, m_pIAddInDllBase);

	return TRUE;
}
	
BOOL CSymbolAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pSymbolHelper) return FALSE;

	// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
	if( !m_bOnAllMouseExclusive) m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase);

	CSymbol::SYMBOLTOOL nCurr = m_pSymbolHelper->GetCurrentToolType();
	BOOL bResult = FALSE;
	if(m_bLButtonDBClk == FALSE)
		bResult = m_pSymbolHelper->OnLButtonUp( CDC::FromHandle( p_hDC), point);
	if( nCurr != m_pSymbolHelper->GetCurrentToolType()) 
	{
		m_pIChartOCX->ReleaseOtherToolUI( ( CToolOptionInfo::TOOLOPTION)-1);

		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
	}
	// 20080916 김진순  분석도구 그리고 나면 무조건 연속그리기해제
	else if( nCurr != CSymbol::SYMBOL_TEXT && nCurr != CSymbol::SYMBOL_NONE && nCurr != CSymbol::SYMBOL_SELECTION && nCurr != CSymbol::SYMBOL_BULET)
	{
		m_pIChartOCX->OnToolStateChanged(m_eToolOption,FALSE);

//		if(nCurr == CSymbol::SYMBOL_BULET)
//			m_pSymbolHelper->DestroySymbolDlg();

		// 연속그리기 해제
		m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_SELECTION);					

		// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);

		m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;
	}

	return bResult;
}

BOOL CSymbolAddInImp::OnRButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pSymbolHelper) return FALSE;

	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	if(nType == CSymbol::SYMBOL_BULET && m_pSymbolHelper->IsExistSymbolDlg() == FALSE)
	{
		// 툴바도 해제한다
		m_pIChartOCX->ReleaseOtherToolUI( ( CToolOptionInfo::TOOLOPTION)-1);

		m_pSymbolHelper->DestroySymbolDlg();

		// 연속그리기 해제
		m_pSymbolHelper->SetCurrentSymbolType( CSymbol::SYMBOL_NONE);					

		// (2006/2/1 - Seung-Won, Bae) Make Mouse Exclusive
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);		
	}

	//m_eToolOption = CToolOptionInfo::T_TOOL_SELECTION;	
	return TRUE;
}

BOOL CSymbolAddInImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pSymbolHelper) return FALSE;

	ML_SET_LANGUAGE_RES();	// to Load Menu in this Module

	// 1. Check Menu
	BOOL bMenuLoaded = FALSE;
	CMenu Context;
	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	/*if(m_pSymbolHelper->IsExistSymbolDlg() == TRUE && nType == CSymbol::SYMBOL_BULET)
	{
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
		m_pSymbolHelper->SetCurrentSymbolType(CSymbol::SYMBOL_NONE);
	}*/

	if( nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)	bMenuLoaded = Context.LoadMenu( IDR_SYMBOL);
	if( !bMenuLoaded) if( m_pSymbolHelper->OnRButtonUp( point))					bMenuLoaded = Context.LoadMenu( IDR_SYMBOL_CONFIG);
	if( !bMenuLoaded) 
	{	
		m_pIAddInToolMgr->ReleaseAllMouseExclusive( m_pIAddInDllBase);
		return FALSE;
	}

	// 2. Show Menu
	CMenu * pPopup = Context.GetSubMenu( 0);
	if( pPopup)
	{
		CPoint pt( point);
		m_pChartCWnd->ClientToScreen( &pt);
		pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, m_pChartCWnd, NULL);
	}
	Context.DestroyMenu();
	return TRUE;
}

BOOL CSymbolAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pSymbolHelper) return FALSE;

	m_bLButtonDBClk = TRUE;
	CSymbol::SYMBOLTOOL nType = m_pSymbolHelper->GetCurrentToolType();
	if(nType != CSymbol::SYMBOL_NONE && nType != CSymbol::SYMBOL_SELECTION)	
		return FALSE;

	ML_SET_LANGUAGE_RES();	// to Load Menu in this Module
	
	if( !m_pSymbolHelper->OnLButtonDbClk( CDC::FromHandle( p_hDC), point)) return FALSE;
	
	return FALSE;
}

void CSymbolAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	if(p_pIAddInEnvMap->GetCount() <= 0)
		return;

	const char *chToolLoading;//2022.07.27.tour2k
	CString strToolLoading;	
	p_pIAddInEnvMap->Lookup(m_strAddInItemName,chToolLoading);
	strToolLoading.Format("%s",chToolLoading);
	if(strToolLoading.IsEmpty() == TRUE)
		return;

	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	if( strToolLoading.Left( 5) == "CODE:")
	{
		int nItemInfoLen = strToolLoading.Find( "\r\n");
		CString strItemInfo = strToolLoading.Mid( 5, nItemInfoLen - 5);
		strToolLoading = strToolLoading.Mid( nItemInfoLen + 2);

		nItemInfoLen = strItemInfo.Find( " TIME:");
		m_strItemCode = strItemInfo.Left( nItemInfoLen);
		CString strTimeType = strItemInfo.Mid( nItemInfoLen + 6);
		m_eTimeType = ( CScaleBaseData::HORZSCALEDRAWERTYPE)atoi( strTimeType);

		// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		m_pSymbolHelper->SetXScaleType(m_eTimeType);
		// 2011.02.07 by SYS <<
	}

	m_pSymbolHelper->BuildElementsFromString(strToolLoading);
}

void CSymbolAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	
{
	CString strSaveToolList = m_pSymbolHelper->GetSavedElementsData();

	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	if( !strSaveToolList.IsEmpty())
	{
		CString strItemInfo;
		strItemInfo.Format( "CODE:%s TIME:%d\r\n", m_strItemCode, m_eTimeType);
		strSaveToolList = strItemInfo + strSaveToolList;
	}

	p_pIAddInEnvMap->SetAt(m_strAddInItemName,strSaveToolList);
}