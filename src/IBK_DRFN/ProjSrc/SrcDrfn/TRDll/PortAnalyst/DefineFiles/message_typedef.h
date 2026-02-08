#ifndef MESSAGE_TYPEDEFINE
#define MESSAGE_TYPEDEFINE


#define WM_NOTIFY_GRID					WM_USER + 240
#define NM_GRID_CLK								  1
#define NM_GRID_DBCLK								2
#define NM_GRID_CELLDATA						3
#define NM_GRID_HEADERCLK						4
#define NM_GRID_RCLK							5


#define WM_NOTIFY_TREE					WM_USER + 300
#define NM_TREE_CLK								  1
#define NM_TREE_ITEMTITLE           2

#define MF_PFCONTORL_MOVE				WM_USER + 241
#define MF_PFCONTORL_EVENT				WM_USER + 242

#define WMU_SEND_TO_MAIN_NAVIGATOR		WM_USER + 147

const UINT RMSG_MOVECTRL = ::RegisterWindowMessage(_T("RMSG_MOVECTRL"));
const UINT RMSG_PF_CONTROL_EVENT= ::RegisterWindowMessage(_T("RMSG_PF_CONTROL_EVENT"));


#endif // MESSAGE_TYPEDEFINE