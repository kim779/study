// AddInCommand.h: interface for the AddInCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDINCOMMAND_H__116FEE34_B4F9_47BF_9C39_3EAB81B1C864__INCLUDED_)
#define AFX_ADDINCOMMAND_H__116FEE34_B4F9_47BF_9C39_3EAB81B1C864__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/17
// Comments		: AddIn Command Define Header
// Using way	: Do not delete any define. You can do Only adding.
// See			: 
//-----------------------------------------------------------------------------

typedef enum EAICommandType
{
	EAI_ALL_RESET_CURRENT_TOOL,
	// (2006/1/17 - Seung-Won, Bae) Reset Current Tool Mode (Change to select mode!) ~ No Param
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)
	
	EAI_BLOCKTOOL_OCXMENUTYPE,
	// (2006/1/17 - Seung-Won, Bae) Process for Menu of OCX. Look CMenuType in BlockBaseEnum.h
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
	//			p_lResult	: Out, Result, bool
	//			p_lParam1	: CMenuType::MENUTYPE
	//			p_lParam2	: Selected Index

	EAI_BAS_OCXELEMENTMENU,
	// (2006/1/17 - Seung-Won, Bae) Tool Object Command of OCX. Look CMenuType in BlockBaseEnum.h. It used in ChartObject, Analysis, Symbol.
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
	//			p_lResult	: Out, Result, bool
	//			p_lParam1	: CMenuType::ELEMENTMENU
	//			p_lParam2	: Selected Index

	EAI_LOG_SET_IMAGENARY_CANDLE,
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for Imagenary Cancel Type Stting
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
	//			p_szParam	: The Routine Name that sending command.
	//			p_lParam	: Setting Value (TRUE or FALSE)

	EAI_CUSTOM_SAVE_IMAGE,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command for Saving Image of Chart Graph.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)

	_EAI_OBJECT_SET_SIMPLE_MODE_TOOL,
	// (2006/8/8 - Seung-Won, Bae) Not Used any more..
	// (2006/1/23 - Seung-Won, Bae) BlockTool(Object) AddIn Command for Popup Menu Type
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
	//			p_lParam	: Flag for Simple Mode

	EAI_OBJECT_SET_USING_TOOL1,
	// (2006/1/23 - Seung-Won, Bae) BlockTool(Object) AddIn Command for Object Option Setting
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
	//			p_lParam1	: CBlockBaseData::TOOLTYPE
	//			p_lParam2	: bool Value for Tool Option

	EAI_OBJECT_SET_USING_TOOL2,
	// (2006/1/23 - Seung-Won, Bae) BlockTool(Object) AddIn Command for Full Object Option Setting
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
	//			p_lParam1	: m_bControlBlockSize
	//			p_lParam2	: m_bBlockMoveDelete
	//			p_lParam3	: m_bBlockMoveDelete
	//			p_lParam4	: m_bUseScaleRButtonMenu
	//			p_lParam5	: m_bUseScaleRButtonMenu
	//			p_lParam6	: m_bSelectGraph (graph title 선택)
	//			p_lParam7	: m_bSelectGraph (graph 선택)
	//			p_lParam8	: m_bSelectTitle (graph 이동)
	//			p_lParam9	: m_bSelectTitle (graph 삭제)

	_EAI_LOG_ONGETALLPROPERTIES2,
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for GetAllProperties2()
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStringList *p_pParam)
	//			p_pParam	: CStringList of Property Info
	// (2006/4/18 - Seung-Won, Bae) Not used any more. Use InvokeAddInStr( ~, "LOGFILE", ~)

	EAI_CUSTOM_ENABLE_BOUNDMARK,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command for Enabling of BoundMark
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
	//			p_lParam	: BoundMark Enable Flag

	EAI_CUSTOM_GET_BOUNDMARK_FLAG,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command to get the BoundMark Flag
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
	//			p_lResult	: BoundMark Flag

	EAI_CUSTOM_GET_EXISTENCE_BOUNDMARK,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command to get the BoundMark Existence
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
	//			p_lResult	: BoundMark Existence

	EAI_CUSTOM_ENABLE_RECEIVING_TIMESHARE,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command to get the BoundMark Existence
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
	//			p_lResult	: BoundMark Existence

	EAI_ST_CALCULATE_INDICATOR,
	// (2006/1/23 - Seung-Won, Bae) System Trading AddIn Command to process indicators
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam)
	//			p_lResult	: Return Value
	//			p_szParam	: Indicator Name
	//			p_pParam	: Packet List (CPacket List)

	EAI_ST_CALCULATE_ALL,
	// (2006/1/23 - Seung-Won, Bae) System Trading AddIn Command to calculate all indicators
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
	//			p_lResult	: Return Value
	//			p_lParam	: 0(TR) or 1(Real)

	EAI_CUSTOM_CLEAR_TIMEMARK,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command to clear timemark.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)

	EAI_CUSTOM_SET_TIMEMARK,
	// (2006/1/23 - Seung-Won, Bae) Custom AddIn Command to set timemark.
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
	//			p_szParam	: Marking Time
	//			p_lParam	: With-Scroll Flag

	EAI_LOG_REAL_SETTING_CHANGED,
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for Real Setting Change
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam)
	//			p_szParam	: Real Setting String

	_EAI_LOG_ADD_REAL_UPDATE,
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for Real Update Request
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)
	//			p_lParam	: Real Table Record ID
	//			p_szParam	: Real Record Key String
	// (2006/7/12 - Seung-Won, Bae) Not used. (Only Hanwha)


	_EAI_LOG_RESET_REAL_UPDATE,
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for Real Update Cancel
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)
	//			p_lParam	: Real Table Record ID
	//			p_szParam	: Real Record Key String
	// (2006/7/12 - Seung-Won, Bae) Not used. (Only Hanwha)

	EAI_LOG_DRDS_ADVISE, 
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for Real Data Request
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	//			p_lParam	: Request ID
	//			p_szParam1	: Packet Name
	//			p_szParam2	: Field Name
	//			p_szParam3	: Advise Key

	EAI_LOG_DRDS_UNADVISE,
	// (2006/1/23 - Seung-Won, Bae) Log AddIn Command for Real Data Cancel
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	//			p_lParam	: Request ID
	//			p_szParam1	: Packet Name
	//			p_szParam2	: Field Name
	//			p_szParam3	: Advise Key

	EAI_ALL_ANALYSIS_RESET_TEXTPOSITION,
	// (2006/1/24 - Seung-Won, Bae) All Analysis AddIn Command for Reset Text Position of Analysis Tool
	//			It calls the EAI_ANALYSIS_RESET_LINE_TEXT / EAI_ANALYSIS_RESET_CROSS_TEXT / EAI_ANALYSIS_RESET_TRISECTION_TEXT / EAI_ANALYSIS_RESET_QUADRISECTION_TEXT / EAI_ANALYSIS_RESET_FIBO_TEXT
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)

	EAI_ALL_ENVIRONMENT_CHANGED,
	// (2006/6/27 - Seung-Won, Bae) All AddIn Tool Command for Environment Setting Changed.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID);

	EAI_ALL_APPLY_STYLE,
	// (2006/1/26 - Seung-Won, Bae) All AddIn Tool Command for Style Setting Changed Notify
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)

	EAI_ALL_BLOCK_BEHAVIOR,
	// (2006/1/26 - Seung-Won, Bae) All AddIn Tool Command in Block Operation (Insert/Move/Delete)
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam)
	//			p_pParam	: CBlockBehaviorData Object Address

	EAI_ALL_ANALYSIS_RESET_INITIAL_COLOR,
	// (2006/1/24 - Seung-Won, Bae) All Analysis AddIn Command for Initializing Color of Analysis Tool
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)

	EAI_OOA_ON_TOOL_FUNCTION,
	// (2006/1/31 - Seung-Won, Bae) One or All AddIn Tool Command for AddIn Tool Function Operation
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
	//			p_lParam1	: CToolOptionInfo::TOOLOPTION ~ It decide the One or All Process
	//			p_lParam2	: Adding Option Value

	EAI_ALL_RELEASE_SELECTION_EXCEPT_ONE,
	// (2006/1/31 - Seung-Won, Bae) All AddIn Tool Command for Releasing AddIn Object Selection
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam)
	//			p_pParam	: AddIn DLL Interface for Exception.

	EAI_ALL_ON_DELETE_INDICATOR,
	// (2006/1/31 - Seung-Won, Bae) All AddIn Tool Command for Indicator Deleting Event
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);
	//			p_szParam	: Indicator Name

	EAI_ALL_ON_CHANGE_CURSOR_MODE,
	// (2006/1/31 - Seung-Won, Bae) All AddIn Tool Command for Cursor Mode Changed
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, long p_lParam);
	//			p_lParam	: Cursor Mode ~ m_nNumericalInquiryOption, Look BlockBaseEnum.h

	EAI_TOOLTIP_ONLBUTTONDOWN,
	EAI_TOOLTIP_ONLBUTTONUP,
	// (2006/2/15 - Seung-Won, Bae) ToolTip AddIn Tool Command for Manual Action.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	//			pDC					: Mouse Event Parameters
	//			nFlags				: Mouse Event Parameters
	//			point				: Mouse Event Parameters
	//			p_dwMouseActionID	: Mouse Event Parameters

	EAI_ST_REMOVED_DATA_COUNT,
	// (2006/6/5 - Seung-Won, Bae) System Trading AddIn Command for Data Removing Notify
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam			: Removed Data Count

	EAI_NIGLG_BACKROUND_CLOLOR_CHANGED,
	// (2006/2/16 - Seung-Won, Bae) Numerical Inquiry Drag&Over AddIn Tool Command.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: RGB Color

	EAI_NIDLG_DRAG_SHOWHIDE,
	// (2006/2/16 - Seung-Won, Bae) Numerical Inquiry Drag AddIn Tool Command for Other Tool AddIn.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: Show/Hide Flag ~ 0:Hide, 1:Show (without Exclusive)

	EAI_NIDLG_DRAG_ONMOUSEMOVE,
	// (2006/2/16 - Seung-Won, Bae) Numerical Inquiry Drag AddIn Tool Command for Other Tool AddIn.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	//			pDC					: Mouse Event Parameters
	//			nFlags				: Mouse Event Parameters
	//			point				: Mouse Event Parameters
	//			p_dwMouseActionID	: Mouse Event Parameters

	EAI_NIDLG_DRAG_OVER_ACTIVATED,
	// (2006/2/16 - Seung-Won, Bae) Numerical Inquiry Drag AddIn Tool Command for Over NIDLG Activated State.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: Active/Deactive Flag ~ 0:Deactived, 1:Actived

	EAI_CROSSHAIRS_DRAG_OVER_ACTIVATED,
	// (2006/2/16 - Seung-Won, Bae) Crosshairs Drag AddIn Tool Command for Over Crosshairs Activated State.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: Active/Deactive Flag ~ 0:Deactived, 1:Actived

	EAI_CROSSHAIRS_OVER_CROSSLINE_ACTIVATED,
	// (2006/2/16 - Seung-Won, Bae) Crosshairs Over AddIn Tool Command for CrossLine Object Tool Activated State.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: Active/Deactive Flag ~ 0:Deactived, 1:Actived

	EAI_CROSSHAIRS_OVER_CROSSHAIRS_TOOL_ACTIVATED,
	// (2006/2/16 - Seung-Won, Bae) Crosshairs Over AddIn Tool Command for Crosshairs Tool Activated State.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: Active/Deactive Flag ~ 0:Deactived, 1:Actived

	EAI_NIDLG_ALL_ENABLE_HORIZONTAL_LINE,
	// (2006/2/16 - Seung-Won, Bae) NIDLG Drag/Tool/Over AddIn Tool Command for Horizontal Line Option.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	//			p_lParam	: Flag of Visibiltiy.

	EAI_ALL_CONTEXT_MENU,
	// (2006/2/16 - Seung-Won, Bae) All AddIn Tool Command for Context Menu.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, CWnd *p_pWnd, CPoint &point);
	//			p_pWnd	: the Window at R-Button Click.
	//			point	: the Position of R-Button Click

	EAI_ENVDLG_SHOW_SETUP_DLG,
	// (2006/8/8 - Seung-Won, Bae) EnvSetupDlg AddIn Tool Command to show the Single Indicator Setting Dialog
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam);
	//			p_lResult	: Return VAlue
	//			p_pParam	: Indicator Info of CFG

	EAI_STYLE_SET_STYLE,
	// (2006/8/17 - Seung-Won, Bae) Style Manager AddIn Tool Command to set the style for skin.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
	//			p_plParam	: Style NO.

	EAI_DRDS_DATA,
	// (2006/10/5 - Seung-Won, Bae) Drds Real Data Receive Event of DrdsRealReceiverAddIn
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	//			p_lParam1	: Size of DrdsData, Fixed Value (int)
	//			p_lParam2	: Data Structure (DrdsData *)

	EAI_DRDS_PACKET_END,
	// (2006/10/5 - Seung-Won, Bae) Drds Real Data Receive Event of DrdsRealReceiverAddIn
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2);
	//			p_szParam1	: Packet Name
	//			p_szParam2	: Key Code

	EAI_GLANCEBALANCE_CURSOR_GET_STATE,
	// (2006/10/17 - Seung-Won, Bae) GlanceBalance Cursor AddIn Command to get the state of cursor.
	//		OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
	//			p_lResult	: State Flag

	EAI_GLANCEBALANCE_CURSOR_TOGGLE_STATE,
	// (2006/10/17 - Seung-Won, Bae) GlanceBalance Cursor AddIn Command to togle the state of cursor.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID)

	EAI_TOOLORDERMGR_ADD_TOOL_TO_ORDERMANAGER,
	// ( 2006 / 11 / 17 - Sang-Woo, Cho ) Each Tool AddIn Command ToolOrderManager AddIn to be created.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const long p_lParam2)
	//			p_szParam1	: The name of Tool AddIn to be created.
	//			p_lParam2	: The value( ex. pointer of Tool to be created ) which Tool AddIn sends.

	EAI_TOOLORDERMGR_DELETE_TOOL_FROM_ORDERMANAGER,
	// ( 2006 / 11 / 17 - Sang-Woo, Cho ) Each Tool AddIn Command ToolOrderManager AddIn to be delete.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const long p_lParam2)
	//			p_szParam1	: The name of Tool AddIn to be delete.
	//			p_lParam2	: The value which Tool AddIn sent.

	EAI_TOOLORDERMGR_DELETE_THIS_TOOL,
	// ( 2006 / 11 / 17 - Sang-Woo, Cho ) ToolOrderManager AddIn Command Each Tool AddIn to delete.
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const long p_lParam2)
	//			p_szParam1	: The name of Tool AddIn to delete.
	//			p_lParam2	: The value which Tool AddIn sent.

	EAI_UNDO_REDO_MANAGER_ADD_WORK_TO_MANAGER,
	// ( 2006 / 12 / 26 - Sang-Woo, Cho ) 
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
	//			p_szParam	: The name of Tool AddIn to work.
	//			p_lParam	: The value which Tool AddIn sent.

	EAI_UNDO_REDO_MANAGER_DELETE_REDO_WORK_LIST,
	// ( 2006 / 12 / 26 - Sang-Woo, Cho ) 
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
	//			p_szParam	: UNDO_REDO_MANAGER
	//			p_lParam	: 0

	EAI_UNDO_REDO_MANAGER_UNDO_THIS_WORK,
	// ( 2006 / 12 / 26 - Sang-Woo, Cho ) 
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
	//			p_szParam	: The name of Tool AddIn to Undo.
	//			p_lParam	: The value which Tool AddIn sent.

	EAI_UNDO_REDO_MANAGER_REDO_THIS_WORK,
	// ( 2006 / 12 / 26 - Sang-Woo, Cho ) 
	//		OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
	//			p_szParam	: The name of Tool AddIn to Redo.
	//			p_lParam	: The value which Tool AddIn sent.

	EAI_INDICATORINFO_UNDO_REDO_MANAGER_BACKUP_INDICATORINFO,
	// ( 2006 / 12 / 26 - Sang-Woo, Cho ) 
	//		OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
	//			p_lParam	: a count of IndicatorNameList
	//			p_szParam1	: IndicatorName List
	//			p_szParam2	: do not use
	//			p_szParam3	: do not use
	EAI_PACKETLISTMANAGER_CHANGE_RQ,
	// 선택된 RQ : 복수종목 - ojtaso (20070123)

	EAI_PACKETLISTMANAGER_NEW_RQ,
	// 새로운 RQ 추가 : 복수종목 - ojtaso (20070305)

	EAI_PACKETLISTMANAGER_DELETE_RQ
	// RQ 삭제 : 복수종목 - ojtaso (20070305)
};

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
#define DECLARE_ADDIN_EVENT_MAP()																				\
protected:																										\
	static const EAddInEventType	m_eRegisteredEventMap[ EAE_END + 1];										\
protected:																										\
	virtual const EAddInEventType *	GetRegisteredEventMap( void) const	{	return m_eRegisteredEventMap;	}

// (2006/2/12 - Seung-Won, Bae) Define AddIn Event Map
#define BEGIN_ADDIN_EVENT_MAP( CChartAddInImp)												\
const IChartAddIn::EAddInEventType CChartAddInImp::m_eRegisteredEventMap[ EAE_END + 1] =	\
{
#define ADDINEVENT( EventType)		\
	EAE_##EventType
#define ONADDINEVENT( EventType)	\
	ADDINEVENT( EventType),
#define END_ADDIN_EVENT_MAP()	\
	EAE_END,					\
};

#endif // !defined(AFX_ADDINCOMMAND_H__116FEE34_B4F9_47BF_9C39_3EAB81B1C864__INCLUDED_)
