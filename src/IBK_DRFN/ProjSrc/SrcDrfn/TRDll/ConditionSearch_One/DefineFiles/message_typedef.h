#ifndef MESSAGE_TYPEDEFINE
#define MESSAGE_TYPEDEFINE

#define WM_XMLTREE_CTRL					WM_USER + 230
#define NM_SELECT_ITEM								1
#define NM_SELECT_ITEM_FOLDER					3
#define NM_ADD_ITEM										4
#define NM_MODIFY_ITEM								5
#define NM_SELECT_FILE								6
#define NM_SELECT_FILE_FOLDER					7
#define NM_LOAD_FILE									8
#define NM_SELECT_PORT								9
#define NM_ADD_PORT										10
#define NM_SELECT_ACCOUNTHAVECODE			11
#define NM_ADD_ACCOUNTHAVECODE				12
#define NM_SELECT_LANGUAGE						13
#define NM_ADD_LANGUAGE								14
#define NM_SELECT_USERPATTEN					15
#define NM_ADD_USERPATTEN							16
#define NM_ADD_MARK										17
#define NM_DEL_MARK										18
#define NM_GET_PORT										19
#define UM_SM_SIGNAL_MSG			(WM_USER + 550) //신호처리기에서 받은 데이타를 등록된 콘트롤에 전달한다


#define WM_NOTIFY_GRID					WM_USER + 240
#define NM_GRID_LIST_CLK				1
#define NM_GRID_GROUP_CLK				2
#define NM_GRID_RESULT_CLK              3
#define NM_GRID_RESULT_DBCLK            4
#define NM_GRID_RESULT_SHARED           5
#define NM_ITEM_CLK						6

#define WMU_SEND_TO_MAIN_NAVIGATOR		WM_USER + 147

#define WMU_GET_MODULE_MANINWND			WM_USER + 148


const UINT RMSG_CHANGE_SKINCOLOR= ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
const UINT RMSG_GET_SKINCOLOR	= ::RegisterWindowMessage(_T("RMSG_GET_SKINCOLOR"));
const UINT RMSG_KSMOVECTRL		= ::RegisterWindowMessage(_T("RMSG_KSMOVECTRL"));
const UINT RMSG_KSENABLECTRL	= ::RegisterWindowMessage(_T("RMSG_KSENABLECTRL"));
const UINT RMSG_KSGETFRAMEWND	= ::RegisterWindowMessage(_T("RMSG_KSGETFRAMEWND"));
const UINT RMSG_SETENABLECTRL	= ::RegisterWindowMessage(_T("RMSG_SETENABLECTRL"));
const UINT RMSG_WAIT_CORSOR		= ::RegisterWindowMessage(_T("RMSG_WAIT_CORSOR"));
#endif // MESSAGE_TYPEDEFINE