#ifndef MESSAGE_TYPEDEFINE
#define MESSAGE_TYPEDEFINE

#define WM_NOTIFY_GRIDEVENT		WM_USER + 340
#define NM_CLICK_L				1
#define NM_CLICK_R				2
#define NM_CLICK_CELLTYPE		3
#define NM_DBCLICK_L			4

#define WM_NOTIFY_GRID			WM_USER + 240
#define NM_GRID_CLK             1
#define NM_GRID_DBCLK           2
#define NM_GRID_SHARED          3

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

#define UMSG_REG_GWAN				WM_USER + 101
#define UMSG_OPEN_SCREEN			WM_USER + 102
#define UMSG_DISP_MSG				WM_USER + 103
#define UMSG_OPEN_GWANDLG			WM_USER + 104
#define UMSG_DROP_CODE				WM_USER + 105
#define UMSG_LINK_CODE				WM_USER + 106
#define WMU_SEND_TO_MAIN_NAVIGATOR	WM_USER + 147

#define UMSG_CHART_SYMBOLDATA				WM_USER + 100
#define UMSG_CHART_GETDATETIME				WM_USER + 110
#define UMSG_CHART_SETWINDOWDATA			WM_USER + 120
#define UMSG_CHART_SETZOOMVALUE				WM_USER + 130

const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
const UINT RMSG_GET_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_GET_SKINCOLOR"));
const UINT RMSG_KSMOVECTRL	=	::RegisterWindowMessage(_T("RMSG_KSMOVECTRL"));
const UINT RMSG_SHARE_CODE	=	::RegisterWindowMessage(_T("RMSG_SHARE_CODE"));
const UINT RMSG_WAIT_CORSOR	=	::RegisterWindowMessage(_T("RMSG_WAIT_CORSOR"));
const UINT RMSG_PF_CONTROL_EVENT= ::RegisterWindowMessage(_T("RMSG_PF_CONTROL_EVENT"));
const UINT RMSG_GETPARTNER_MNG = ::RegisterWindowMessage(_T("RMSG_GETPARTNER_MNG"));
const UINT RMSG_RECEIVETRANDATA = ::RegisterWindowMessage(_T("RMSG_RECEIVETRANDATA"));
const UINT RMSG_MOVECTRL = ::RegisterWindowMessage(_T("RMSG_MOVECTRL"));
const UINT RMSG_RECEIVEDIRECTFID = ::RegisterWindowMessage(_T("RMSG_RECEIVEDIRECTFID"));
//const UINT RMSG_SHARE_CODE	=	::RegisterWindowMessage(_T("RMSG_SHARE_CODE"));
 
const UINT RMSG_SET_JCODECTRL =	::RegisterWindowMessage(_T("RMSG_SET_JCODECTRL"));

const LPCTSTR		MATCHVAL_OPT_LIST[] = { ">", ">=", "=", "<=", "<" };
const LPCTSTR		MATCHVAL_OPT_DATA = ">\n>=\n=\n<=\n<\n";
typedef struct tagMatchInfo
{
	int			nRow;
	BOOL		bUser;
	int			nOptIndex;
	CString		strField;
	CString		strElement;
	int 		nFieldIndex;

	int			nGridSourceRow;			//# Temp
} MatchInfo;

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;


#endif // MESSAGE_TYPEDEFINE