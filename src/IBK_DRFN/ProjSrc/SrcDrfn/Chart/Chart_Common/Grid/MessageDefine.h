#if !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_


// 계좌관련
#define		CODE_LENGTH		7
#define		ACCT_LENGTH		11 // "-" 제거된 상태 13

// 로컬공유
#define		NPRSN_LOCAL_RESIZE		"MAP_RESIZE"			// 내부공유 문자(공유명 앞 9자리는 항상 "!NP_LOCAL"이어야 함)


// 메시지 
#define	UDM_DBCLKJANGOLIST			WM_USER+1100			// 잔고 리스트에서 Double Clik

// GridCtrl.h에서 선언
//#define	UDM_DROPORDER_CANCEL		WM_USER+1300		// 취소 주문위에 데이터 Drop
//#define	UDM_DROPORDER_CHANGE		WM_USER+1400		// 정정 주문위에 데이터 Drop
//#define	UDM_TRACE_MOUSEMOVE			WM_USER+1600		// 그리드에서 Mouse Move

// PopupOrder.h에서 선언
//#define	UDM_DROPGRID_FROM_POPUP		WM_USER+1500		// 팝업에서 그리드로 Drop

#define	UDM_HIDE_CANCEL_DLG			WM_USER+1800
#define	UDM_DESTROY_SETUPNUMBER_DLG	WM_USER+1900
#define	UDM_DESTROY_MAXJUMUN_DLG	WM_USER+1910		// JSJ_Add_040421
#define	UDM_DESTROY_SETUPPRICE_DLG	WM_USER+1920		// JSJ_Add_040421
#define	UDM_DBCLK_MICHAELIST		WM_USER+1930		// JSJ_Add_040503
#define UDM_CODECHANGE_YES			WM_USER+1940		// JSJ_Add_040908
#define UDM_CODECHANGE_NO			WM_USER+1950		// JSJ_Add_040908
#define	UDM_DESTROY_VOLUMESET_DLG	WM_USER+1960		// JSJ_Add_040503

#define	UDM_CODESHAREFROMEDITBOX	WM_USER+2000
#define UDM_SPIN_CHANGE				WM_USER+2100		// UpDown Message	
#define	UDM_BUTTON_CLICK			WM_USER+2200		// 기타 호가 Message

#define	UDM_PASS_ENTER				WM_USER+2400		// Pass에서 Enter
#define	UDM_NUMBER_ENTER			WM_USER+2410		// 수량, 금액에서 Enter

#define	UDM_FROM_ACCT				WM_USER+2500		// AcctEdit에서 LengthFull, Enter



const UINT RMSG_FROMCODELIST			= ::RegisterWindowMessage(_T("RMSG_FROMCODELIST"));			// 종목코드 변경
const UINT RMSG_SETSHAREDATA			= ::RegisterWindowMessage(_T("RMSG_SETSHAREDATA"));			// 공유 메세지 
const UINT RMSG_RESIZEDLL				= ::RegisterWindowMessage(_T("RMSG_RESIZEDLL"));			// 화면 리사이징
const UINT RMSG_DISPMESSAGE				= ::RegisterWindowMessage(_T("RMSG_DISPMESSAGE"));			// 메시지 전송
const UINT RMSG_OPENMAPWITHDATA			= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHDATA"));		// 맵을 열고 데이터를 넘김
const UINT RMSG_OPENMAPWITHSHARE		= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHSHARE"));		// 맵을 열고 종목코드를 넘김
const UINT RMSG_REAUESTMAINFUNC			= ::RegisterWindowMessage(_T("RMSG_REAUESTMAINFUNC"));		// 비밀번호 저장
const UINT RMSG_GETSKINVALUE			= ::RegisterWindowMessage(_T("RMSG_GETSKINVALUE"));			// 색상
const UINT RMSG_GETMAPTR				= ::RegisterWindowMessage(_T("RMSG_GETMAPTR"));				// 화면번호 요청
const UINT RMSG_GETACCTPASSENCKEY		= ::RegisterWindowMessage(_T("RMSG_GETACCTPASSENCKEY"));	// 계좌비밀번호 요청
const UINT RMSG_VERIFYACCTPASS			= ::RegisterWindowMessage(_T("RMSG_VERIFYACCTPASS"));		// 계좌번호 검증(내부, 외부 구분없음)
const UINT RMSG_REFRESHCOMPONENT		= ::RegisterWindowMessage(_T("RMSG_REFRESHCOMPONENT"));		// 비밀번호 변경(계좌재조회)
const UINT RMSG_SETFOCUSTOCODE			= ::RegisterWindowMessage(_T("RMSG_SETFOCUSTOCODE"));		// 종목코드 포커스

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MESSAGEDEFINE_H__67840956_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)

