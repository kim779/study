#ifndef MESSAGE_TYPEDEFINE
#define MESSAGE_TYPEDEFINE

#define WMU_REFRESH_INDEXTREE		WM_USER + 3003	//# Language List Sync!

/********************************************************************/
/*                                                                  */
/*                     EzFinder Message Define                      */
/*                                                                  */
/*                       2004. 11. 2 		Doori                       */
/*                          작성자 : 우동우                         */
/********************************************************************/

#define WM_NOTIFY_CMGRIDEVENT					WM_USER + 100
#define NM_SELECT_GROUP							1		//# Group
#define NM_SELECT_CONDITION						2		//# (0) : Group, (1) : Row
#define NM_CONDITION_DRAGDROP					5		//# (0) : Group, (1) : Old Row, (2) : New Row
#define NM_STARTBONG_VAL						8		//# (0) : Group, (1) : Val
#define NM_ENDBONG_VAL							9		//# (0) : Group, (1) : Val
#define NM_ALIGN_ELEMENT						30		//#=> Last Element Index

#define NM_VALVIEWALL_CHECK						10	//# (0) : Group, (1) : Val
#define NM_DELETEALL							11	//# Group
#define NM_CONDITION_CHECK						20	//# (0) : Group, (1) : Row, (2) : Val
#define NM_TARGET_LIST							21	//# (0) : Group, (1) : Row, (2) : Target
#define NM_VALVIEW_CHECK						22	//# (0) : Group, (1) : Row, (2) : Val
#define NM_MODIFY_BTN							23	//# (0) : Group, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
#define NM_DELETE_BTN							24	//# (0) : Group, (1) : Row, (2) : Element Index

#define NM_ROW_UP_BTN										26
#define NM_ROW_DOWN_BTN										27	
#define NM_ROW_MAXUP_BTN									28
#define NM_ROW_MINDOWN_BTN									29

#define NM_GROUP_ADD							40	//# Group	
#define NM_GROUP_INSERTUP						41	//# Group
#define NM_GROUP_INSERTDOWN						42	//# Group
#define NM_GROUP_DELETE							43	//# Group

#define NM_UPDATE								44

#define NM_SETBTNIMG							(WM_USER + 50)
#define	NM_CHANGE_SUBJECT						51
#define	NM_CHANGE_SUBJECT_INIT					52

#define WM_NOTIFY_GRIDEVENT						WM_USER + 340
#define NM_CLICK_L								1
#define NM_CLICK_R								2
#define NM_CLICK_CELLTYPE						3
#define NM_DBCLICK_L							4

//CQuickTab Class Message
#define WM_NOTIFY_QUICKTAB						WM_USER + 320
#define NM_QUICK_CLICK							1

#define WM_XMLTREE_CTRL							WM_USER + 230
#define NM_SELECT_CLASS     					1
#define NM_SELECT_ITEM							2
#define NM_ADD_ITEM								3
#define NM_MODIFY_ITEM							4
#define NM_SELECT_FILE							5
#define NM_LOAD_FILE							6
#define NM_ADD_MARK								10
#define NM_DEL_MARK								11
#define NM_GET_PORT								20
#define NM_GET_LANGUAGE							30
#define NM_USERCON_DEL							40  


//### Condition Map Message Define ###
#define WM_NOTIFY_MAPLOAD						WM_USER + 240
#define NM_MAPLOAD_UPDATE						1
#define NM_MAPLOAD_OK							2
#define NM_MAPLOAD_CLOSE						3


//## Logic Edit ##
//#   => CConditionMakeWnd
#define WM_LOGIC_EDIT						    WM_USER + 330				
#define NM_EDIT_OPTCHANG						1

#define WM_CONDITIONMAKE_WND					WM_USER + 270
#define NM_INIT									1
#define NM_MAPSHOW								2
#define NM_ADD_DATA								3
#define NM_TAB_CHANGE							4
#define NM_LOAD_FINENAME						5
#define NM_LOAD_FINENAME_TEMP					6
#define NM_SEARCH								8
#define NM_FILE_SYNC							9
#define NM_FILE_SAVE							10
#define NM_CHANGE_CONDITION						11
#define NM_SCREEN_EXP							100

#define WM_CONDITIONRESULT_WND					WM_USER + 310
#define NM_SCREEN_EXP							100
#define NM_LOAD_PORTFOLIO						2
#define NM_UPDATE_FIXFIELD						3
#define NM_ROLL_SEARCH							4
#define NM_SET_WORKDATE							5
#define NM_SET_MSTCODE							6
#define NM_SHOW_LANGUAGE						7
#define NM_SENDDATA_SHARD						8


//## Condition Tab ##
#define WM_NOTIFY_TAB							WM_USER + 260
#define NM_CHANGETAB							1
#define NM_TAGETURL								2
#define NM_ADDFILE								3

#define MAKEWINDOWCLOSE							2
#define MAKEWINDOWOPEN							1	
#define CHANGE_EXP								99

#define WMU_SEND_TO_MAIN_NAVIGATOR				WM_USER + 147
#define UMSG_LANGUAGE_RELOAD					WM_USER + 500

#define RMSG_CHECKMENU							WM_USER + 600

const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
const UINT RMSG_GET_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_GET_SKINCOLOR"));
/*const UINT RMSG_CHECKMENU  = ::RegisterWindowMessage(_T("RMSG_CHECKMENU"));*/
const UINT RMSG_WAIT_CORSOR= ::RegisterWindowMessage(_T("RMSG_WAIT_CORSOR"));
const UINT RMSG_SEARCHRNG = ::RegisterWindowMessage(_T("RMSG_SEARCHRNG"));
const UINT RMSG_SET_ENABLE_BTNINIT = ::RegisterWindowMessage(_T("RMSG_SET_ENABLE_BTNINIT"));
const UINT RMSG_SET_VISIBLE_MAPBUTTON = ::RegisterWindowMessage(_T("RMSG_SET_VISIBLE_MAPBUTTON"));
const UINT RMSG_SET_ENABLE_CTRL = ::RegisterWindowMessage(_T("RMSG_SET_ENABLE_CTRL"));

#endif // MESSAGE_TYPEDEFINE