#if !defined(AFX_MESSAGEDEFINE_H__17563481_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)
#define AFX_MESSAGEDEFINE_H__17563481_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// 사용자 정의 메세지
//
#define		UDM_SELCHANGE	100
#define		UDM_FONT_DATA	200


/////////////////////////////////////////////////////////////////////////////
// 소켓 관련 
//
#define	SOCKET_UNKNOWNERR		-100		// Unknown Error
#define	SOCKET_SOCKCREATEERR		-5		// NeoSock생성에러
#define	SOCKET_CONNECTERRROR		-4		// 연결시 에러 발생
#define	SOCKET_CLOSE			-3		// 서버에서 CLOSE 메시지 받음
#define SOCKET_RECEIVEQUIT		-2		// 종료메세지 받음
#define SOCKET_TIMEOUT			-1		// 소켓 연결시 시간초과	
#define SOCKET_READY			0		// 준비중
#define SOCKET_OK			1		// 정상완료


#define UDM_RECV_SOCKET_STATE	    WM_USER+801 //소켓 연결 상태 
#define UDM_RECV_SOCKET_DATA        WM_USER+802 //소켓에서 오는 데이타 
#define UDM_GENERAL_DATA		    WM_USER+112 //일반 데이타를 보냄 
#define	UDM_SEND_USERDATA			WM_USER+120 //유저데이터 SEND 
#define	UDM_SOCK_CREATE				WM_USER+113 //Login(dll)에서 메인에게 소켓연결요청 
#define UDM_SOCK_DISCONNECT			WM_USER+114 //Login(dll)에서 메인에게 소켓종료요청 
#define UDM_GET_IPADDRESS			WM_USER+115	//Login(dll)에서 메인에게 소켓IP요청 
#define IDW_WEBCONNECT              117

/////////////////////////////////////////////////////////////////////////////
// 사용자 정의 툴바 
//
#define	UDM_TRCODE_ENTER			WM_USER+200 //TR CODE ENTER

#define UDM_TICKERDATA_RCV			WM_USER+201	//TICKER DATA

#define UDM_URGENCY_REV				WM_USER+202 //Urgency Data

#define	UDM_POLLINGDATA_RECV		WM_USER+203	// SocketDlg->MainFrame (Polling 데이타)

#define UDM_CLOSEWITHOUTASK			WM_USER+204	// 무조건 종료

#define UDM_SOCKETCLOSE				WM_USER+205	// 소켓 클로즈 발생

#define UDM_UPTOOLBARFUNC			WM_USER+210	// 상단 단축 툴바 버튼 클릭 이벤트 WPARAM : Function 번호

/////////////////////////////////////////////////////////////////////////////
// 프레임 정보(View -> ChildFrame) 
//
#define UDM_FRAME_INFO				WM_USER+300 // View -> Frame (사이즈, 타이틀) 
#define UDM_RETURN_MESSAGE			WM_USER+310 // View -> Frame (맵 메세지)
#define	UDM_GETFRAME_SIZE			WM_USER+342	// 현재 화면의 사이즈를 알고자 할 때 
#define	UDM_SETFRAME_SIZE			WM_USER+343	// Frame Size 변경 : WPARAM->Flag, LPARAM->size 정보

/////////////////////////////////////////////////////////////////////////////
// 계좌 비밀번호 저장시 
//
#define	UDM_ACCT_MSG				WM_USER+400 
#define UDM_ACCT_DRAGDROP			WM_USER+401
#define	UDM_LIST_CHANGEITEM			WM_USER+420
#define	UDM_LIST_CHECKCOLUMN		WM_USER+425
#define	ACCT1_CLEARALL				1
#define ACCT1_CANCEL				2
#define	ACCT2_CANCEL				3

#define UDM_INIT_FOCUS				WM_USER+430
#define UDM_EDIT_RETURN				WM_USER+440

// {{ bds 계좌관련 추가
#define	UDM_SETSHAREDATA			WM_USER+701	// 공유데이타변경
												// wParam : bNotify, lParam : STSHAREDDATA
#define	UDM_ACCTINFO				WM_USER+702	// 계좌관련 
// }}

/////////////////////////////////////////////////////////////////////////////
// 화면잠금 기능  
#define DEF_SCR_ROCK				WM_USER+100

/////////////////////////////////////////////////////////////////////////////
// 스탑로스 LED 
#define UDM_STOPLOSS_LED			WM_USER+500  // (스탑로스 -> Ticker)
#define STOPLOSS_LED_ON				1
#define STOPLOSS_LED_OFF			2
#define STOPLOSS_LED_BLINK			3
#define UDM_STOPLOSS_DESTROY		WM_USER+510  // (StoplossDlg -> mainfrm)
#define	UDM_STOPLOSS_CREATE			WM_USER+511	 // (TickerDlg -> mainfrm)

////////////////////////////////////////////////////////////////////////////
// MAP 을 띄운다.
#define MSG_SIWHANG					WM_USER+600  // 시황 
#define UDM_MAP_OPEN				WM_USER+613	 // 일반맵 
#define UDM_MAP_OPEN_SEARCH			WM_USER+611 // 4Byte Code Enter
#define UDM_MAP_OPEN_NOCHECK		WM_USER+612	 // NoCheck 알반맵
#define MSG_SIWHANG_JUMUN			WM_USER+620	 // 시황주문 
#define MSG_DEF_GAGE				WM_USER+630	 // SYSTEM GAGE
#define MSG_SHARED_PIN				WM_USER+640	 // PIN 눌렸을때.

#define MSG_PLUS_BTN				WM_USER+641	 // PLUS 눌렸을때.
#define MSG_MINUS_BTN				WM_USER+642	 // MINUS 눌렸을때.
//* 수정 시작 (2001.9.10 1) Print Buttton 사용을 위한 수정
#define MSG_PRINT_BTN				WM_USER+643	
//* 수정 끝 (2001.9.10 1) 

#define	UDM_LOADMENU_FINDTRDATA		WM_USER+644	 // 사용자 화면을 open할 수 있는지 파악위해(시세전용시 사용)

#define	MSG_FONT_RESIZE				WM_USER+650	 // 사이즈 조정 
#define	MSG_FONT_CURRENT			WM_USER+651	 // 현재 사이즈 알아내기위해 
#define	MSG_FRAME_RESIZE			WM_USER+652	 // 프레임 사이즈 변경
#define	MSG_SHARE_CURRENT			WM_USER+653	 // 현재 공유 정보 알아내기 위해 
#define	UDM_FONT_DEFAULT			WM_USER+654	 // 디폴트 사이즈로 변경 
#define	MSG_FOLD_SCREEN				WM_USER+656	 // 화면접기
#define	MSG_ICON_MENU				WM_USER+657	 // 메뉴
#define	MSG_INDEPT_SCREEN			WM_USER+658	 // 독립실행

////////////////////////////////////////////////////////////////////////////
// 탭 변경시 
#define UDM_TAB_CHANGE				WM_USER+700	 // 탭변경시

////////////////////////////////////////////////////////////////////////////
// 화면 프린트
#define	UDM_PRINT_MAP				WM_USER+800	 // View -> MainFrame

////////////////////////////////////////////////////////////////////////////
// 도움말
#define	UDM_HELP_MAP				WM_USER+801	 // View -> MainFrame

////////////////////////////////////////////////////////////////////////////
// 메인 종료
#define	UDM_CLOSE					WM_USER+900	 // 소켓에서 메인 종료 

/////////////////////////////////////////////////////////////////////////////
// 관심
#define	UDM_GWANSIM_REG				WM_USER+802	 // 관심등록창에서 메인으로..
												 // wParam(1) : 관심등록중
												 // wParma(2) : 관심등록완료
												 // wParma(3) : 관심등록취소

/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// 티커로 보기
#define	UDM_TICKER_JUMUN			WM_USER+1000	// 매도, 매수 주문
#define UDM_TICKER_SIHWANG			WM_USER+1001    // 시황창보기 

#define	UDM_TICKER_ALONE			WM_USER+1002	// 티커 다시그리기 wParam : 100
													// 메인으로 가기   wParam : 101
													// 종료            wParam : 102

////////////////////////////////////////////////////////////////////////////
// DeskForm에서 프레임 정보 얻기 위해 
#define	UDM_FRAME_INFO2				WM_USER+1100	// 공유 정보		wParam : 0
													// 프레임 파일정보	wParam : 1						

/////////////////////////////////////////////////////////////////////////////
// 챠트
//
//#define	UDM_CHART_MSG				WM_USER+600	// 메인 챠트 리스트에 챠트 OCX 핸들을 추가한다. 
//#define	UDM_CHARTTOOLBAR_LOAD		WM_USER+601	// 메인의 챠트 툴바를 생성/소멸한다. 
//#define	UDM_CHARTBUTTON_CLICK		WM_USER+602	// 챠트 툴바 버튼 클릭시 메인으로 버튼 정보 보냄.
//#define	UDM_CHARTTOOLBAR_ACTIVE		WM_USER+603	// 차일드 프레임 Active될때 메인으로 보내는 메세지
//#define UDM_GET_CHARTDATA			WM_USER+604 // 차트에서 사용하는 클래스 주소 리턴
//#define UDM_GET_USETOOLBAR			WM_USER+606	// 외부툴바 사용여부
//#define UDM_SET_USETOOLBAR			WM_USER+607 // 메인에서 외부
//#define UDM_ANALYSIS_FUNCTION		WM_USER+532	// 메인에서 차트로 보내는 메세지
//#define	WMU_ADD_INDICATOR			WM_USER+541	// wParam : 1 (add), 0 (delete)	// lParam : ocx 핸들

/////////////////////////////////////////////////////////////////////////////
// 챠트
//
//#define	UDM_CHART_MSG				WM_USER+600	// 메인 챠트 리스트에 챠트 OCX 핸들을 추가한다. 
//#define	UDM_CHARTTOOLBAR_LOAD		WM_USER+601	// 메인의 챠트 툴바를 생성/소멸한다. 
//#define	UDM_CHARTBUTTON_CLICK		WM_USER+602	// 챠트 툴바 버튼 클릭시 메인으로 버튼 정보 보냄.
//#define	UDM_CHARTTOOLBAR_ACTIVE		WM_USER+603	// 차일드 프레임 Active될때 메인으로 보내는 메세지
#define UDM_GET_CHARTDATA			WM_USER+604 // 차트에서 사용하는 클래스 주소 리턴
//#define UDM_GET_USETOOLBAR			WM_USER+606	// 외부툴바 사용여부
#define UDM_SET_USETOOLBAR			WM_USER+607 // 메인에서 외부
//#define UDM_ANALYSIS_FUNCTION		WM_USER+532	// 메인에서 차트로 보내는 메세지
#define	WMU_ADD_INDICATOR			WM_USER+541	// wParam : 1 (add), 0 (delete)	// lParam : ocx 핸들

// <차트수정>
#define	UDM_CHART_RELEASE_ALL		WM_USER+599 // 메인/챠트간에 선택된 툴바 해제
#define	UDM_CHART_ADD				WM_USER+600	// 메인 챠트 리스트에 챠트 OCX 핸들을 추가한다. (0:삭제 1:추가 2:챠트툴바 숨기기)
#define	UDM_CHARTTOOLBAR_LOAD		WM_USER+601	// 메인의 챠트 툴바를 생성/소멸한다. 
#define	UDM_CHARTBUTTON_CLICK		WM_USER+602	// 챠트 툴바 버튼 클릭시 메인으로 버튼 정보 보냄.
#define	UDM_CHARTTOOLBAR_ACTIVE		WM_USER+603	// 차일드 프레임 Active될때 메인으로 보내는 메세지
#define UDM_CHARTSUB_HIDE			WM_USER+609	// Chart Tool Bar Button클릭시 Sub Bar Hide		
#define UDM_ANALYSIS_FUNCTION		WM_USER+532	// 메인에서 차트로 보내는 메세지
#define UDM_GET_USERID				WM_USER+605	// 차트에서 메인으로 사용자 id를 얻기 위해 보내는 메세지			
#define UDM_GET_USETOOLBAR			WM_USER+606	// 차트에서 메인으로 외부툴바 사용여부 얻기 위해 보내는 메세지
#define UDM_TOOL_SELECT				WM_USER+608	// 차트에서 이전에 지정된 챠트형태로 뜰 경우 해당 버튼 토글 요청 (wParam : 해당 지표 enum값 ) 예: 매물대 분석
#define UM_ADD_INDICATOR_SYSTEM		WM_USER+542 // 메인에서 차트로 보내는 메세지(wparam 1, lparam 지표명) => UDM_ANALYSIS_FUNCTION는 분석도구 값 정의된 경우 사용, 그 외는 이 메시지로 지표명으로 전달
#define	UDM_BUTTON_CLICK_FROM_CHART_OCX		WM_USER+610	// 맵의 챠트버튼이 눌렸을때 오는 메시지 정의 추가(030529_변대성)(wParam : 해당 지표 enum값 , lParma : 1->Dowm 2->Up)

//-----------------------------------------------------------------------------
// Author		: {{{ 오정택 }}} ojtaso 	Date : 2005/2/22
// Reason		: Added to chart toolbar dll 
//-----------------------------------------------------------------------------
// BEGIN
const UINT RMSG_REMEMBERCHARTTOOL	= ::RegisterWindowMessage(_T("RMSG_REMEMBERCHARTTOOL"));
// END

// Chart관련 Dialog에서 사용한다. ConfigDll에서 메인에게 보냄
// bds_Chart관련 추가(040114)
#define	WMU_DELCONFIGDLG			WM_USER+1004 // ConfigDll에서 메인에게 보냄


//////////////////////////////////////////////////////////////////////////////////////////////
//bds
#define	UDM_MAP_RELOAD				WM_USER+750 // 화면 열기 메세지(현재창)
#define	UDM_XGP_LOAD				WM_USER+751 // XGP 파일 Load
#define	UDM_CLEAR_MESSAGE			WM_USER+752 // 종합화면 메시지바 Clear
#define	UDM_CHANGE_TAB				WM_USER+753 // 탭 변경
#define	UDM_MINMAX_INFO				WM_USER+754 // 화면 사이즈 제한
#define	UDM_CHANGE_SIZE				WM_USER+755	// 탭변경시 사이즈 변경
#define	UDM_DLG_LOADING				WM_USER+756 // 저장화면 loading하는 dialog에서 버추얼스크린별 화면수 요청메시지  
#define	UDM_GET_TITLE				WM_USER+757 // 맵에서 TITLE을 얻을때 프레임에 보내는 메세지
#define UDM_MAP_RESOTRE				WM_USER+758 // LoadXGPFile 에서 최대화된 윈도우를 Restore시킨다.


/////////////////////////////////////////////////////////////////////////////
// 리스트 관련 
//

// 기타 메세지는 RegisterMessage.h 참조
// #include "../../../Include_base/RegisterMessage.h"
const UINT RMSG_RESIZEDLL0202	= ::RegisterWindowMessage(_T("RMSG_RESIZEDLL0202"));
const UINT RMSG_DISPMESSAGE		= ::RegisterWindowMessage(_T("RMSG_DISPMESSAGE"));
const UINT RMSG_ONMOVE			= ::RegisterWindowMessage(_T("RMSG_ONMOVE"));	// OnMove() 메시지 전달
const UINT RMSG_NOTIFYDLL		= ::RegisterWindowMessage(_T("RMSG_NOTIFYDLL"));	// Main 에서 dll 화면에 보내는 메세지

//계좌비번 가져오기
const UINT RMSG_GETACCTPASS		= ::RegisterWindowMessage(_T("RMSG_GETACCTPASS"));
//계좌비번 저장하기
const UINT RMSG_SETACCTPASS		= ::RegisterWindowMessage(_T("RMSG_SETACCTPASS"));
//강제로 주문창 닫았다가 열기
const UINT RMSG_RESTARTDLL		= ::RegisterWindowMessage(_T("RMSG_RESTARTDLL"));

//공부 부분 리로드 하기
const UINT RMSG_RECALLSHAREDATA = ::RegisterWindowMessage("RMSG_RECALLSHAREDATA");
//const UINT RMSG_REFRESH_ACCT = ::RegisterWindowMessage("RMSG_REFRESH_ACCT");
// 기타 메세지는 RegisterMessage.h 참조

//주문 확인창..
const UINT RMSG_SHOWORDERCONFIRM	= ::RegisterWindowMessage("RMSG_SHOWORDERCONFIRM");	
const UINT RMSG_MAININFO			= ::RegisterWindowMessage(_T("RMSG_MAININFO"));

//JJH 
const UINT RMSG_COLORBYIDX			= ::RegisterWindowMessage(_T("RMSG_COLORBYIDX"));
const UINT RMSG_CALLTOTMENU			= ::RegisterWindowMessage(_T("RMSG_CALLTOTMENU")); // WPARAM : Menu Index LParam : 사용안함
const UINT RMSG_OPENMAPWITHSHARE	= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHSHARE")); // WPARAM : Menu Index LParam : 사용안함
const UINT RMSG_PASSWDCONFERM		= ::RegisterWindowMessage(_T("RMSG_PASSWDCONFERM")); 


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_MESSAGEDEFINE_H__17563481_B1A6_11D4_8D2E_00C026A220A8__INCLUDED_)

