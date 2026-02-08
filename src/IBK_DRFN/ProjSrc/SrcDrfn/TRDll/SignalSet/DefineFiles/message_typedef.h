#ifndef MESSAGE_TYPEDEFINE
#define MESSAGE_TYPEDEFINE



#define WM_NOTIFY_TREE				WM_USER + 1000
#define NM_ITEM_CLK								  1
#define NM_ITEM_DBCLK							  2


#define WM_NOTIFY_GRID				WM_USER + 2000
#define NM_ITEM_CLK								  1
#define NM_ITEM_DBCLK							  2

#define WM_NOTIFY_SIGNALGRID	WM_USER + 3000
/*
#define NM_ITEM_CLK								  1
#define NM_ITEM_DBCLK							  2
*/
#define NM_ITEM_CELLTYPE						3
#define NM_TH_CLK                   4

#define MF_PFCONTORL_MOVE				WM_USER + 241
#define MF_PFCONTORL_EVENT				WM_USER + 242



const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
const UINT RMSG_GET_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_GET_SKINCOLOR"));
const UINT RMSG_KSMOVECTRL	=	::RegisterWindowMessage(_T("RMSG_KSMOVECTRL"));
const UINT RMSG_PF_CONTROL_EVENT= ::RegisterWindowMessage(_T("RMSG_PF_CONTROL_EVENT"));



#endif // MESSAGE_TYPEDEFINE