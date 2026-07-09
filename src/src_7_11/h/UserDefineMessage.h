/*-----------------------------------------------------------
//
//	사용자정의 Message
//	file	: UserDefineMessage.h
//	written	: scbang
//	date	: 2003. 12. 24
//	ver	: 1.0
-------------------------------------------------------------*/

#ifndef _USER_DEFINE_MESSAGE
#define _USER_DEFINE_MESSAGE


#define	UDM_MESSAGE		WM_USER + 888


// 환경
#define	UDM_GET_COLOR		1000		// Color		// GetColor(lParam);
#define	UDM_GET_ROOT		1001		// Root Dir		// GetRoot();
#define	UDM_GET_PARENT_WND	1002		// Parent Wnd		// m_pwndParent

// 종목정보
#define	UDM_GET_JCOD_NAME	1100		// 종목명
#define	UDM_GET_JCOD		1101		// 종목코드

// Type
#define	UDM_GET_TYPE		1200		// 선물, 옵션 여부

// Mouse Action
#define	UDM_LBUTTON_DOWN	1300		// LButton Down
#define	UDM_LBUTTON_UP		1301		// LButton Up
#define	UDM_RBUTTON_DOWN	1310		// RButton Down
#define	UDM_RBUTTON_UP		1311		// RButton Up


// 계좌정보
#define	UDM_GET_ACCOUNT		2000		// 계좌번호		// m_strAccount.operator LPCTSTR();
#define	UDM_GET_ACCOUNT_NAME	2001		// 계좌명
#define	UDM_GET_PASSWORD	2002		// 패스워드

// 잔고정보
#define	UDM_GET_BALANCE		2100		// 잔고 개수		// m_ptw->GetBalance();

// 주문 수량정보
#define	UDM_GET_ORDER_COUNT	2200		// 계약수		// m_ptcw->GetCount();

// 주문환경 정보
#define	UDM_GET_CONFIRM_ORDER	2300		// 주문확인여부
#define	UDM_GET_ENABLE_ORDER	2301		// 주문가능여부


// 주문정보
#define	UDM_SEND_NEW_ORDER	2400		// 신규주문
#define	UDM_SEND_CANCEL_ORDER	2401		// 취소주문
#define	UDM_SEND_FIX_ORDER	2402		// 정정주문
#define	UDM_SEND_OPPOSITE_ORDER	2403		// 반대주문
#define	UDM_SEND_ENABLE_COUNT_ORDER	2410	// 주문가능 수량만큼 주문


// 주문 가능수량
#define	UDM_SEND_ENABLE_ORDER_COUNT	2600		// 주문가능수량 조회



// Guide Message
#define	UDM_GUIDE_MESSAGE	4000		// Guide Message
#define	UDM_POST_GUIDE_MESSAGE	4001		// Post Guide Message




// Popup
#define	UDM_POPUP_SELL_ENABLE	5000		// 매도가능		// PopupSellEnable((DWORD)lParam);
#define	UDM_POPUP_BUY_ENABLE	5001		// 매수가능		// PopupBuyEnable((DWORD)lParam);




// 다른윈도우의 pointer
#define	UDM_GET_MESSAGE_WND	9000		// MessageWnd Pointer Up
#define	UDM_GET_TICKCOUNT_WND	9001		// TickCountWnd Pointer Up


#endif