#if !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_



#ifndef __CHARTMSG_DEF_H
#define __CHARTMSG_DEF_H
#endif 
/////////////////////////////////////////////////////////////////////////////
// 리스트 관련 
//

//#define	CODE_LENGTH		6
//#define	ACCT_LENGTH		13
//#define	UPJONG_LENGTH	3    // 한화에서는 길이가 3
//#define	FUTOPT_LENGTH	8

#define	UDM_DBCLKJANGOLIST			WM_USER+1100		// 잔고 리스트에서 Double Clik
#define	UDM_DBCLKCOUNTLIST			WM_USER+1200		// 미체결 리스트에서 Double Clik

// GridCtrl.h에서 선언
//#define	UDM_DROPORDERCANCEL			WM_USER+1300		// 취소 주문위에 데이터 Drop
//#define	UDM_DROPORDERCHANGE			WM_USER+1400		// 정정 주문위에 데이터 Drop
//#define	UDM_TRACEMOUSEMOVE			WM_USER+1600		// 그리드에서 Mouse Move

// PopupOrder.h에서 선언
//#define	UDM_DROPGRIDFROMPOPUP		WM_USER+1500		// 팝업에서 그리드로 Drop
																										

#define	UDM_DESTROYJANGODLG			WM_USER+1700
#define	UDM_DESTROYCOUNTDLG			WM_USER+1800
#define	UDM_DESTROYSETUPNUMBERDLG	WM_USER+1900
#define	UDM_DESTROYMAXJUMUNDLG		WM_USER+1910		// JSJ_Add_040421
#define	UDM_DESTROYSETUPPRICEDLG	WM_USER+1920		// JSJ_Add_040421
#define	UDM_DESTROYMAEDOMAESUDLG	WM_USER+1930		// JSJ_Add_040503
#define	UDM_CODESHAREFROMEDITBOX	WM_USER+2000

#define UDM_SPIN_CHANGE				WM_USER+2100		// UpDown Message	
#define	UDM_BUTTON_CLICK			WM_USER+2200		// 기타 호가 Message

#define	UDM_PASS_ENTER				WM_USER+2400		// Pass에서 Enter

#define	UDM_FROM_ACCT				WM_USER+2500		// AcctEdit에서 LengthFull, Enter

const UINT RMSG_FROMCODELIST = ::RegisterWindowMessage(_T("RMSG_FROMCODELIST"));
const UINT RMSG_SETSHAREDATA = ::RegisterWindowMessage(_T("RMSG_SETSHAREDATA"));
//const UINT RMSG_OPENMAPOTHER = ::RegisterWindowMessage("RMSG_OPENMAPOTHER");
const UINT	RMSG_OPENMAP2		= ::RegisterWindowMessage(_T("RMSG_OPENMAP2"));
const UINT RMSG_FROMHOGAJUMUN_DESTROY = ::RegisterWindowMessage(_T("RMSG_FROMHOGAJUMUN_DESTROY"));
const UINT RMSG_HOGACHANGEDATA_CODE = ::RegisterWindowMessage(_T("RMSG_HOGACHANGEDATA_CODE"));
const UINT RMSG_HOGACHANGEDATA_ACCT = ::RegisterWindowMessage(_T("RMSG_HOGACHANGEDATA_ACCT"));
const UINT RMSG_SHARED_PIN = ::RegisterWindowMessage("RMSG_SHARED_PIN");
const UINT RMSG_RESIZEFROMGWAN = ::RegisterWindowMessage("RMSG_RESIZEFROMGWAN");
const UINT RMSG_MESSAGESHOW = ::RegisterWindowMessage("RMSG_MESSAGESHOW");
const UINT RMSG_GETSKINVALUE	=	::RegisterWindowMessage("RMSG_GETSKINVALUE");

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)

