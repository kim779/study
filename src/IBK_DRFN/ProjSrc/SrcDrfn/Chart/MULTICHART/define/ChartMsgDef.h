#ifndef __CHART_MSG_DEF__BYGM0604
#define __CHART_MSG_DEF__BYGM0604
#include <afxtempl.h>

#include "../../commontr/TRDefine.h"

#define UMSG_INPUTCODETOINPUTBAR		WM_USER+1	// 종목코드를 보낸다.
#define UMSG_SEND2SERVER				WM_USER+2	// 서버로 조회를 날린다.
#define UMSG_RECEIVEFROMSERVER			WM_USER+3	// 서버로부터 조회결과를 받는다.
#define UMSG_CHANGEMODE					WM_USER+4	// 종목입력모드를 바꾼다.
#define UMSG_SHOWINDICATORCONFIGDLG		WM_USER+5	// 지표정보창을 보인다.
#define UMSG_SETTYPE					WM_USER+6	// 차트의 데이터타입을 지정한다.
#define UMSG_SETCNTS					WM_USER+7	// 차트의 데이터수를 지정한다.
#define UMSG_SETZOOMVALUE				WM_USER+8	// 차트의 보여지는 데이터수를 지정한다.
#define UMSG_SETINITIALCHARTDATA		WM_USER+9	// 차트의 기본초기값을 보낸다.
#define UMSG_SHOWREGFAVORITEDLG			WM_USER+10	// 즐겨찾기창을 보인다.
#define UMSG_SETMATRIX					WM_USER+11	// 멀티차트의 배열을 바꾼다.
#define UMSG_REQUESTMORE				WM_USER+12	// 서버로 추가 데이터를 요구한다.
#define UMSG_RESETCHART					WM_USER+13	// 차트를 초기화한다.
#define UMSG_APPLYFAVORITE				WM_USER+14	// 즐겨찾기를 적용한다.
#define UMSG_GETNAMEINDICATOR			WM_USER+15	// 지표값들을 가져온다.
#define UMSG_HASGRAPH					WM_USER+16	// 해당그래프가 있는지 체크한다.
#define UMSG_ADDCHARTBLOCK				WM_USER+17	// 해당그래프를 추가한다.
#define UMSG_RESETOCX					WM_USER+18	// OCX를 초기화한다.
#define UMSG_SETJONGMOKLINK				WM_USER+19	// 종목연동세팅을 한다.
#define UMSG_SETTYPELINK				WM_USER+20	// 설정연동세팅을 한다.
#define UMSG_LOADCHART					WM_USER+21	// 차트를 로드한다.
#define UMSG_SAVECHART					WM_USER+22	// 차트를 저장한다.
#define UMSG_ADDUPJONGDATA				WM_USER+23	// 업종데이터를 추가한다.
#define UMSG_FIXDATE					WM_USER+24	// 일자를 고정시킨다.
#define UMSG_INPUTCODEFROMINPUTBAR		WM_USER+25	// InputBar로부터 종목코드를 받아 반영한다.
#define UMSG_SHOWSCALE					WM_USER+26	// X,Y축의 Scale 보기를 설정한다.
#define UMSG_SETVIEWENDTIME				WM_USER+27	// 차트의 뷰상의 마지막날짜를 세팅한다.
#define UMSG_UPDATEINDEX				WM_USER+28	// 차트및 지표를 전부 반영한다.
#define UMSG_GETENDDATEFROMINPUTBAR		WM_USER+29	// InputBar에서 기간고정이 되어있다면 해당 마지막 날짜
#define UMSG_SENDSHAREDCODE				WM_USER+30	// Main공유메모리영역에 코드를 보낸다.
#define UMSG_SETONECHART				WM_USER+31	// 선택된한개의 차트를 제외한 나머지는 삭제한다.
#define UMSG_SENDTOMSGBOX				WM_USER+32	// 메세지박스에 메세지를 보낸다.
#define UMSG_ADDMARKETDATA				WM_USER+33	// 시장데이터를 추가한다.
#define UMSG_GETTYPEFROMINPUTBAR		WM_USER+34	// 데이터타입을 받는다.
#define UMSG_GETTYPENUMFROMINPUTBAR		WM_USER+35	// 데이터타입의간격을 받는다.
#define UMSG_UPDATEREVISED				WM_USER+36	// 수정주가 사용여부를 보낸다.
#define UMSG_GETREVISED					WM_USER+37	// 수정주가 사용여부를 받는다.
#define UMSG_CLEARLINK					WM_USER+38	// 종목,시간연동을제거한다.
#define UMSG_MULTICODES					WM_USER+39	// 다수종목을 조회한다.
#define UMSG_SENDSYNCCODE				WM_USER+40	// 코드를 동기화한다.
#define UMSG_INPUTINSHAREDCODELIST		WM_USER+41	// 공유코드리스트에종목을 입력한다.
#define UMSG_INPUTUPJONGCODE			WM_USER+42	// 업종코드를 입력한다.
#define UMSG_GETZOOMVALUE				WM_USER+43	// 차트의 보여지는 데이터수를 가져온다.
#define UMSG_SHOWENVIRDLG				WM_USER+44	// 환경설정DLG를 보여준다.
#define UMSG_GETINC						WM_USER+45	// 추가, 삭제 구분을 받는다.
#define UMSG_EDITCODE_INIT				WM_USER+46  // 한글종목찾기 초기화...
#define	UMSG_SENDSHARETIME				WM_USER+47  // 시간공유...
#define	UMSG_GETSAVED					WM_USER+48  // 분할저장정보를 받는다...
#define	UMSG_GETTIMESHARSYSEAGE			WM_USER+49	// 시간공유의 사용여부를 받는다...
#define	UMSG_SETTIMESHARSYSEAGE			WM_USER+50	// 시간공유의 사용여부를 보낸다...
#define	UMSG_SETUPPERLOWPRICEMARKUSEAGE	WM_USER+51	// 상하한가표시 사용여부를 보낸다...
#define	UMSG_GETUPPERLOWPRICEMARKUSEAGE	WM_USER+52	// 상하한가표시 사용여부를 받는다...
#define	UMSG_RATEVALUE					WM_USER+53	// 비율/Dollar/원화 값으로 변경하며 가격차트 반영한다.
#define	UMSG_SETSHOWLOCK				WM_USER+54	// 락 정보 표시 설정한다.
#define	UMSG_GETSHOWLOCK				WM_USER+55	// 락 정보 표시 설정한다.
#define	UMSG_SENDJCODECHECKED			WM_USER+56	// Check하는 종목코드를 보낸다.
#define UMSG_GETJCODECHECKEDFROMCHART	WM_USER+57	// Check하는 종목코드를 가져온다.
#define UMSG_GETJCODECHECKEDFROMLEFTBAR	WM_USER+58	// Check하는 종목코드를 가져온다.
#define	UMSG_SENDJCODSYNHIGHLIGHTED		WM_USER+59	// 선택표시된 종목을 해제한다.
#define	UMSG_GETJONGMOKCODE				WM_USER+60	// 선택된차트의종목을보낸다.
#define	UMSG_UPDATECODE					WM_USER+61	// WmdMapView의종목Update를받는다.
#define	UMSG_SETCHARTHOGA				WM_USER+62	// 차트-호가 의 값을 WndMapView에 보낸다.
#define	UMSG_GETFAVORITETHINGSMNG		WM_USER+63	// CFavoriteThingsMng Pointer를 가져온다.
#define UMSG_UPDATEMATRIX				WM_USER+64	// 화면배열을 재조정한다.
#define UMSG_INPUTCODETOCHARTMNG		WM_USER+65	// 종목코드를 보낸다.
#define UMSG_GETCNTOFCHART				WM_USER+66	// 차트의 수를 가져온다.
#define UMSG_GETSTATUSENABLE			WM_USER+67	// 조회의 상태를 가져온다.
#define UMSG_CHANGEMODEBYINPUTBAR		WM_USER+68	// 종목입력모드를 바꾼다. 0: 삽입, 1:추가
#define UMSG_SETACCOUNTINFO				WM_USER+69	// 계좌정보를 보낸다.
#define UMSG_SETHIGHLIGHTINFO			WM_USER+70	// Highlight 정보를 보낸다.
#define UMSG_GETJCODECHECKEDFROMCODESRC	WM_USER+71	// Check List를 가져온다.
#define UMSG_SETNORMALIZEDMINUTETIMERULE	WM_USER+72	// 허수봉표시를 설정한다.
#define UMSG_GETNORMALIZEDMINUTETIMERULE	WM_USER+73	// 허수봉표시를 가져온다.
#define UMSG_ANALYSISPATTERN				WM_USER+74	// ChartPatternAddin.dll
#define UMSG_CHARTGUIDELINE					WM_USER+75	// ChartGuideLine.dll
#define UMSG_INPUTFUTURECODE				WM_USER+76	// 선물코드를 입력한다.
#define UMSG_GETDRAGMODETYPE				WM_USER+77	// 기간분석사용유무를 가져온다.
#define UMSG_SETDRAGMODETYPE				WM_USER+78	// 기간분석사용유무를 보낸다.
#define UMSG_SETORDEREDINFO					WM_USER+79	// 체결내역정보를 본다.
#define UMSG_LOADSYSTEMTRADING				WM_USER+80

//2005. 04. 25 ----------------- Start Add New User message ---------------------------------------
// add by sy, nam
#define USMG_SHOW_DATAWND					WM_USER+81	//chart data list를 보일지 결정한다.
#define USMG_NOTIFY_REVISED					WM_USER+82  //수정주가 사용여부를 컨트롤에 반영하도록 한다.
#define UMSG_GET_TICKMINDAY_VALUE			WM_USER+83  //현재 선택된 차트의 Tick/Min/Day의 단위를 가져온다.
#define UMSG_USE_LOG						WM_USER+84	//로그 사용여부를 chart ocx에 보낸다.
#define UMSG_GETLOG							WM_USER+85	//로그 사용여부를 InputBar로부터 받는다.
#define UMSG_CHANGE_CHARTVALUE				WM_USER+86	//추가될 차트의 내용이 변경될 경우 InputBar의 차트 콤보와 CTabChartList쪽의 선택된 항목을 변경한다.
#define UMSG_GET_TIME_FROM_DATETIMEPICKER	WM_USER+87	//상단툴바에서 하단 툴바의 달력컨트롤의 마지막 조회 종료일을 얻어온다.
#define UMSG_ADDITEM_BTNCLICK				WM_USER+88	//비교차트의 종목추가 버튼 클릭
#define UMSG_DELITEM_BTNCLICK				WM_USER+89	//비교차트의 종목삭제 버튼 클릭
#define UMSG_INITIAL_BTNCLICK				WM_USER+90	//비교차트의 초기화   버튼 클릭
#define UMSG_ADDITEM						WM_USER+91	//비교차트의 선택된 Tab에서 현재 선택된 종목을 CLeftbar로 보낸다.
#define UMSG_GET_COMPARECHART_DATA			WM_USER+92	//비교차트의 추가되는 종목에대한 TR조회를 툴바에게 요청한다.

#define UMSG_LOADSAVECHART					WM_USER+93	// 차트를 저장하고 로드한다.

#define UMSG_INPUTFOREIGNCODE				WM_USER+100 // 해외지수 코드를 입력한다.


#define USMG_PLEASE_RESIZE					WM_USER+150 //Floating 호가창 Resize
#define USMG_CHANGE_ZORDER					WM_USER+151 //Floating 호가창 Change Z-Order
#define UMSG_FORCE_SHOW_HOGAWND				WM_USER+152 //Floating 호가창 강제로 보이도록 한다.
#define UMSG_SETONEPAGECNT					WM_USER+153 //05.06. 30 비교차트에서 별도로 추가되는 차트 화면 봉수 설정.
#define UMSG_GET_POINTER_OF_DBMGR			WM_USER+154 //05.06. 30 멀티차트 MainDlg에 CDBMgr* 를 요청.
#define UMSG_SET_POINTER_OF_DBMGR			WM_USER+155 //05.06. 30 멀티차트 에서 CDBMgr* 를 요청한 윈도우에게 전송.

#define UMSG_CHANGE_COMPARECHART_TYPE		WM_USER+156 //05.07. 14 비교차트 에서 차트의 형태를 바꾸도록 ChartMng와 기타 관련된 Window에서 전송한다.
#define UMSG_INIT_COMPARECHART				WM_USER+157 //05.07. 14 비교차트 관련 모든 Window초기화 
#define UMSG_SENDTR_HOGA_DATA				WM_USER+158 //05.07. 15 메인 다이알로그에 호가를 요청한다.
#define UMSG_DISCONNECT_HOGA_REAL			WM_USER+159 //05.07. 15 메인 다이알로그에 호가리얼 Disconnect를 요청한다.
#define UMSG_SETTYPE_NO_REQUEST				WM_USER+160 //05.07. 19 조회 간격을 셋한다.

#define UMSG_RE_REQUEST_TRDATA				WM_USER+161 //05.07. 20 비교차트의 TR을 다시 요청하도록 리스트에 메세지를 보낸다.
#define UMSG_COMPARE_TR_JONMOK_CNT			WM_USER+162 //05.07. 21 비교차트의 TR요청 종목 갯수를 셋한다.
#define UMSG_MOVE_CHART_VIEWRANGE			WM_USER+163 //05.07. 28 현 차트의 보이는 범위를 셋한다.
#define UMSG_REPLAYCHART_READY				WM_USER+164 //05.08. 01 복기차트를 위한 준비를 하도록 한다.
#define UMSG_RECEIVE_TR09501				WM_USER+165 //05.07. 29 코스피/코스닥 종목/업종/테마 종목등을 받는다.

#define UMSG_RECEIVE_FIND0301				WM_USER+166 //05.07. 30 조건검색 종목등을 받는다.
#define UMSG_DISCONNECT_ALLREAL				WM_USER+167 //05.08. 02 차트의 모든 Real 해제한다.
#define UMSG_DESTROY_FLOATINGHOGA			WM_USER+168 //05.08. 02 호가 데이터 창을 Destroy
#define UMSG_GET_FUTOPTGUBUN				WM_USER+169 //05.08. 05 선물옵션 구분값을 Return
#define UMSG_SET_ENDREQUESTDATE				WM_USER+170 //05.08. 09 차트 조회 요청 날짜를 변경한다.

#define UMSG_GET_CHARTMODE					WM_USER+171 //05.08. 11 TR요청 전에 현재 차트 모드가 무엇인지 알기 위해 요청
#define UMSG_GET_REPLAY_STARTTIME			WM_USER+172 //05.08. 11 TR요청 전에 현재 복기 모드일경우 복기 시작시간 요청 
#define UMSG_SETLOG							WM_USER+173 //05.08.23  로그를 사용여부를 InputBar에 체크하도록 한다.
#define UMSG_STOP_JIPYOCYCLE				WM_USER+174 //05.08.24  지표 돌리기 Stop
#define UMS_SETCHART_DATEINFO				WM_USER+175 //05.08.25  선택된 차트에 툴바(CExtInputBarDlg)에서 변경된 날짜 정보를 넘긴다.

#define UMSG_SHOW_HOGADLG					WM_USER+176 //05.08.25  차트 리사이징 후 Floating Hoga Dialog를 갱신시켜 준다.
#define UMSG_ENABLE_SENDHOGATR				WM_USER+177 //05.08.29  호가 조회가 가능한지 여부를 보낸다.
#define UMSG_CHECK_BE_COMPLETE_QUERYDATA	WM_USER+178 //05.09.05  TR이 조회가 되서 결과가 왔는지 여부.
#define UMSG_SETTEXT_CODE_INPUTBAR			WM_USER+179 //05.09.05  종목 코드만 Set한다.
#define UMSG_GETJONGMOKLINK					WM_USER+180 //05.09.06	종목 동기화 상태인지 체크한다.

#define UMSG_END_REQUEST_CHARTDATA			WM_USER+181 //05.09.08	차트 조회가 끝났음을 InputBar에 알린다.
#define UMSG_SIGNAL_CLICK					WM_USER+182 //05.09.09  신호등 Click
#define UMSG_STOP_APPLY_EXPTOOOL			WM_USER+183 //05.09.10  외부툴바에서 뭔가 다른게 선택될때 타 차트들에게 연속그리기 해제시킨다.
#define UMSG_FIREGSHAREDATA					WM_USER+184 //05.09.09  차트SharedData
#define UMSG_GETENVSINK						WM_USER+185 //05.09.23  환경 동기화 체크 여부.

#define UMSG_GET_REPLAY_DAYBEFORE			WM_USER+186 //05.10.14  복기 시작 n일전 의 n값을  가져온다.
#define UMSG_ENABLE_PLAYBTN					WM_USER+187 //05.10.14  복기 상태에서 받아야 할 데이터를 다 받았을 경우 Play버튼 활성화 한다.
#define UMSG_CHARTODRTRACE					WM_USER+188 //OrderTrace.dll
#define UMSG_CHART_INIT_FROMOCX				WM_USER+189 //OCX로부터 초기화 버튼이 눌렸을 때 ChartMng가 그 통지를 날려준다.
#define UMSG_ADDSYSTEMTRADINGINDEX			WM_USER+190 //05.10.27 시스템트레이딩지표를 추가한다. gm0604

#define UMSG_EXTINPUT_CHANGEDATE			WM_USER+191 //05.10.31  ExtInputBarDlg에서 종료일 선택상태에서 날짜가 바뀔경우 TR조회를 날린다.
#define UMSG_CLEAR_MAREKT_TR				WM_USER+192 //05.11.01  차트 데이터 조회가 안 날아가야 할 경우 시장지표 TR준비 상태를 Clear시킨다.
#define UMSG_RECV_MARKET_TR					WM_USER+193 //05.11.08  시장 지표 TR을 받는다.
#define UMSG_EDIT_FOCUS						WM_USER+194 //05.11.08  종목코드에 포커스
#define UMSG_GET_CANDLE_CNT					WM_USER+195 //05.11.17  8주/12주/24주 차트의 각 설정에 따른 기본 봉갯수를 Get

#define UMSG_EXT_TOOL_RESTORE				WM_USER+196 //05.11.24  외부툴바에서 초기화가 눌렸을 경우 해당 차트에 종목이 셋 되어있는지 확인후 없으면 넘기지 않기 위해 조사하는 메세지 
#define UMSG_DEL_CHART_BLOCK				WM_USER+197 //05.11.30  차트 블럭 삭제.
#define UMSG_APPLY_ANALYZE_PERIOD_ADDIN		WM_USER+198 //AnalyzePeriodAddin.dll Call
#define UMSG_CHART_PRINT					WM_USER+199 //06.01.23  프린트
#define UMSG_CHART_EXCEL					WM_USER+200 //06.01.23  엑셀로 보내기

#define UMSG_SHOWINNERTOOLBAR				WM_USER+201	// X,Y축의 Scale 보기를 설정한다.
#define UMSG_SAVEJIPYOONEFILE				WM_USER+202	// 06.05.02 일/주/월/분/틱 구분 혹은 구분 없이 한파일에 저장
#define UMSG_GETSAVEJIPYOONEFILE			WM_USER+203	// 06.05.02 일/주/월/분/틱 구분 혹은 구분 없이 한파일에 저장
#define UMSG_ONECLICK_HIDE					WM_USER+204 //06.10.02  원클릭 숨기기
#define UMSG_DWMM_CHART						WM_USER+205 //06.10.02  일/주/월/분 차트
#define UMSG_PERIOD_SUMMARY					WM_USER+206 //06.11.08  기간요약
//2007.01.17 add by LYH  툴바버튼 동기화
#define UMSG_SYNC_TOOLBAR					WM_USER+208 //07.01.17  툴바버튼 동기화
#define UMSG_DROPCODE						WM_USER+209 //07.01.17  툴바버튼 동기화
#define UMSG_GETLOADSAVECODE				WM_USER+210	//07.02.08  저장할때 코드로 읽어오기 GET
#define UMSG_SETLOADSAVECODE				WM_USER+211	//07.02.08  저장할때 코드로 읽어오기 SET

#define UMSG_REQUEST_FORQUERYOPT			WM_USER+212 //08.04.29	당일조회로 인한 조회
#define UMSG_GETQUERYOPT					WM_USER+213 //08.04.29	당일조회 여부
const UINT RMSG_SETQUERYOPT = ::RegisterWindowMessage(_T("RMSG_SETQUERYOPT"));

#define UMSG_RUN_AUTOTIMER					WM_USER+220 //08.04.29	돌려보기 처리
#define UMSG_IS_PLAYSTATE					WM_USER+221 //08.04.29	돌려보기 상태확인

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
//20110502 이문수 >>
#define	UMSG_GET_COMMON_DATA				WM_USER+224
#define	UMSG_SET_COMMON_DATA				WM_USER+225
#define	UMSG_IS_PARENT_STDDIALOG			WM_USER+226
//20110502 이문수 <<
#define UMSG_STANDBY_TR_CALL				WM_USER+227	// 대기열
#define UMSG_RELAY_POSSSIBLE_CHECK			WM_USER+228 // 돌려보기 가능여부 확인
//------------------------------ End Add New User message   ---------------------------------------

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
//20110502 이문수 >>
#define	COMDATA_SAVE_JIPYO_PER_CODE_PERIOD	0
#define	COMDATA_SAVE_JIPYO_PER_CODE_PERIOD_MENU_ID	1
//20110502 이문수 <<

//2005. 07. 27 add by sy, nam =======================================================
//외부 툴바와 연동하기 위해 
//Main에 현재 활성화된 Dll의 핸들을 wParam에 실어 넘긴다.
// 1. Main <-> DLL
const UINT RMSG_CHARTMAIN		= ::RegisterWindowMessage(_T("RMSG_CHARTMAIN"));

// 2. DLL->Chart OCX
const UINT RMS_ANALYSIS_FUNC	= ::RegisterWindowMessage(_T("RMS_ANALYSIS_FUNC"));
//===================================================================================

//@유진추가
const UINT RMSG_GET_USERID = ::RegisterWindowMessage(_T("RMSG_GET_USERID"));	// User ID 얻기
//@유진추가

//2005. 08. 24 add by sy, nam =======================================================
//지표 돌려보기를 멈추게 하기 위해 
//메세지를 보낸다.
//const UINT RMS_STOP_JIPYO_CYCLE	= ::RegisterWindowMessage(_T("RMS_STOP_JIPYO_CYCLE"));
//====================================================================================

// 스킨 처리
const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
const UINT RMSG_GET_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_GET_SKINCOLOR"));

// 세로스케일 변경가능한 Method 추가 - ojtaso (20081013)
const UINT RMSG_CHANGEVERTSCALETYPE = ::RegisterWindowMessage(_T("RMSG_CHANGEVERTSCALETYPE"));

const UINT RMSG_CHANGE_CROSSLINE = ::RegisterWindowMessage(_T("RMSG_CHANGE_CROSSLINE"));

const UINT RMSG_ADD_PLATFORM_MENU = ::RegisterWindowMessage(_T("RMSG_ADD_PLATFORM_MENU"));

const UINT RMSG_SETINITIALCHARTDATA = ::RegisterWindowMessage(_T("RMSG_SETINITIALCHARTDATA"));

const UINT RMSG_GETINFO = ::RegisterWindowMessage(_T("RMSG_GETINFO"));
const UINT RMSG_SETINFO = ::RegisterWindowMessage(_T("RMSG_SETINFO"));

// 05.05.30 add by sy, nam =========================================
#define FROM_TABCHART						1	//UMSG_CHANGE_CHARTVALUE를 받을 때메세지를 전송시킨 Wnd 구분자 
#define FROM_INPUTBAR						2	//UMSG_CHANGE_CHARTVALUE를 받을 때메세지를 전송시킨 Wnd 구분자 
#define FROM_TABJIPYO_MARKET				3	//UMSG_CHANGE_CHARTVALUE를 받을 때메세지를 전송시킨 Wnd 구분자 
												//시장지표를 보낼때 설정됨.
#define FROM_EXTINPUTBAR_MARKET				4	//UMSG_CHANGE_CHARTVALUE를 받을 때메세지를 전송시킨 Wnd 구분자 
												//시장지표를 보낼때 설정됨.
//===================================================================================

// message definition From Invoke DLL 
#define UMSG_SEND2SERVERFROMINVOKE			WM_USER+500	
#define UMSG_GETCODEFROMINVOKE				WM_USER+501
#define UMSG_GETMAINHANDLEFROMINVOKE		WM_USER+502

// message definition To Invoke DLL 
#define INVOKEMSG_SETACCOUNTNQUOTECODE		1
#define INVOKEMSG_GETAVGINFO				2
#define INVOKEMSG_INIT						100
#define INVOKEMSG_RECEIVEDATA				101
#define INVOKEMSG_CHANGECODE				102
#define INVOKEMSG_SHOWORDEREDINFO			103
#define INVOKEMSG_RECEIVEREALDATA			105
// 이평 기준선 실시간 수신 표시 - ojtaso (20090210)
#define INVOKEMSG_RECALCGUIDELINE			107



#define DLLTITLENAME			"종합차트"

//#define ENVINFOFILE				"\\MultiChartInfo.dat"
#define ENVINFOORDERCHART		"\\MultiOrderChartInfo.dat"


//#define STOCK_ENVINFOFILE		"\\1570_MultiChartInfo.dat"
//#define FUTOPT_ENVINFOFILE		"\\2800_MultiChartInfo.dat"
//#define UPJONG_ENVINFOFILE		"\\1580_MultiChartInfo.dat"
//#define FOREIGN_ENVINFOFILE		"\\6500_MultiChartInfo.dat"

#define STOCK_ENVINFOFILE		"Stock_ChartInfo.dat"
#define UPJONG_ENVINFOFILE		"Upjong_ChartInfo.dat"
#define FUTOPT_ENVINFOFILE		"FutOpt_ChartInfo.dat"
#define FOREIGN_ENVINFOFILE		"Foreign_ChartInfo.dat"
#define ENVINFOFILE		"ChartInfo.dat"

//===========================================================================
// 2005. 10. 12
//===========================================================================
#define JIPYOSYNC_FILE			"JipyoSync.dat"
// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
#define	JIPYORQ_FILE			"JipyoRQ.dat"		//20110502 이문수 >>

#define TICK_JIPYOSYNC			"Tick_"
#define SEC_JIPYOSYNC			"Sec_"
#define MIN_JIPYOSYNC			"Min_"
#define DAY_JIPYOSYNC			"Day_"
#define WEEK_JIPYOSYNC			"Week_"
#define MONTH_JIPYOSYNC			"Month_"
#define YEAR_JIPYOSYNC			"Year_"

//===========================================================================


//#define STOCK_ENVINFOORDERCHART			"\\1570_MultiOrderChartInfo.dat"
//#define FUTOPT_ENVINFOORDERCHART		"\\2800_MultiOrderChartInfo.dat"
//#define UPJONG_ENVINFOORDERCHART		"\\1580_MultiOrderChartInfo.dat"
//#define FOREIGN_ENVINFOORDERCHART		"\\6500_MultiOrderChartInfo.dat"

#define STOCK_ENVINFOORDERCHART			"\\0301_MultiOrderChartInfo.dat"
#define FUTOPT_ENVINFOORDERCHART		"\\0303_MultiOrderChartInfo.dat"
#define UPJONG_ENVINFOORDERCHART		"\\0302_MultiOrderChartInfo.dat"
#define FOREIGN_ENVINFOORDERCHART		"\\0304_MultiOrderChartInfo.dat"

#define	NID_CODEINPUT				4578	//05.06.21 주식/선물 종목 코드 Ctrl Resource ID
#define	NID_ACCTINPUT				4579	//05.06.21 계좌 종목 코드 Ctrl Resource ID
#define	NID_UPJONGINPUT				4580	//05.06.21 업종 종목 코드 Ctrl Resource ID
#define	NID_FOREIGNINPUT			4581	//05.06.21 해외지수 종목 코드 Ctrl Resource ID
#define	NID_FUTOPTINPUT				4582	//05.06.22 선물옵션  종목 코드 Ctrl Resource ID


#define TAB_CHART		"도구"
#define TAB_ASSISTINDEX "지표"
#define TAB_FAVORITEGRP "관심"
#define TAB_CODESEARCH	"종목"
#define TAB_ORDER		"주문"
#define TAB_MYFAVORITE	"사용자"
#define TAB_SYSTEM		"전략"		// 2005. 05. 16 add by sy, nam
//#define TAB_SYSTEMTRADING	"시스템트레이딩" // 2005.10.25 gm0604
#define TAB_SYSTEMTRADING	"전략" // 2005.10.25 gm0604

//#define TABNO_CHART				(0)
//#define TABNO_ASSISTINDEX		(1)
//#define TABNO_FAVORITEGRP		(2)
//#define TABNO_CODESEARCH		(3)
//#define TABNO_MYFAVORITE		(4)
//#define TABNO_ORDER				(5)
//#define TABNO_SYSTEMTRADING		(6)		// 2005. 10. 26 add by gm0604
//
//#define TABNO_SYSTEM			(7)		// 2005. 05. 16 add by sy, nam
//
//#ifdef _DEBUG
//#define STOCK_TABTOTALCNT		(7)		// 2005. 11. 1 gm0604
//#else
//#define STOCK_TABTOTALCNT		(7)		// 2005. 05. 16 add by sy, nam 일단 시스템탭은 제외.
////#define STOCK_TABTOTALCNT		(6)		// 2005. 05. 16 add by sy, nam 일단 시스템탭은 제외.
//#endif

#define TABNO_CODESEARCH		(0)
#define TABNO_ASSISTINDEX		(1)
#define TABNO_SYSTEMTRADING		(2)		// 2005. 10. 26 add by gm0604
#define TABNO_CHART				(3)		// 2006. 09. 19 remove by LYH
#define TABNO_FAVORITEGRP		(4)
#define TABNO_CODESEARCHUP		(5)
#define TABNO_CODESEARCHFO		(6)

#define TABNO_CODESEARCHFR		(8)		// 해외
#define TABNO_CODESEARCHELW		(9)		// ELW

#define TABNO_CODESEARCHCMM		(10)	// 상품선물옵션차트 - ojtaso (20071001)

#define TABNO_SYSTEM			(4)		// 2005. 05. 16 add by sy, nam
#define TABNO_MYFAVORITE		(4)

#ifdef _DEBUG
#define STOCK_TABTOTALCNT		(2)		// 2005. 11. 1 gm0604
#else
#define STOCK_TABTOTALCNT		(2)		// 2005. 05. 16 add by sy, nam 일단 시스템탭은 제외.
#endif

#define UPJONG_TABTOTALCNT		(3)		// 2005. 05. 16 add by sy, nam
#define FUTOPT_TABTOTALCNT		(3)		// 2005. 05. 16 add by sy, nam
//#define UPJONG_TABTOTALCNT		(4)		// 2005. 05. 16 add by sy, nam // 2005. 11. 14 gm0604
//#define FUTOPT_TABTOTALCNT		(5)		// 2005. 05. 16 add by sy, nam // 2005. 11. 14 gm0604

//#define DOWN_LIMIT				'3'		//하한 2005. 05. 17 add by sy, nam
//#define DOWN					'4'		//하락 2005. 05. 17 add by sy, nam
//#define EQUAL					'5'		//보합 2005. 05. 17 add by sy, nam
//#define UP						'6'		//상승 2005. 05. 17 add by sy, nam
//#define UP_LIMIT				'7'		//상한 2005. 05. 17 add by sy, nam

#define DOWN_LIMIT				'4'		//하한 2005. 05. 17 add by sy, nam
#define DOWN					'5'		//하락 2005. 05. 17 add by sy, nam
#define EQUAL					'3'		//보합 2005. 05. 17 add by sy, nam
#define UP_LIMIT				'1'		//상한 2005. 05. 17 add by sy, nam
#define UP						'2'		//상승 2005. 05. 17 add by sy, nam

#define CODE_STOCK_LENGTH		6
#define CODE_STOCK_LENGTH2		6
#define	ACCT_LENGTH				13
//#define UPJONG_LENGTH			3		// 한화에서는 길이가 3
//#define UPJONG_LENGTH			4		// 동부에서는 길이가 4
//#define UPJONG_LENGTH			5		// KOSCOM에서는 길이가 4
#define UPJONG_LENGTH			3		// Sygene에서는 길이가 3
#define FUTOPT_LENGTH			8
//#define	FOREIGN_LENGTH			4
#define	KQFUT_LENGTH			5		//코스닥 선물 종목 길이 
#define	KQOPT_LENGTH			10		//코스닥 옵션 종목 길이 
#define	KQOPT_LENGTH12			12
#define ELW_LENGTH			7		// ELW 삼성: 길이가 7
#define COMMODITY_LENGTH			8	// 상품선물옵션차트 - ojtaso (20071001)


// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
#define CHANGE_CHART			0		//전환모드		2005. 05. 19 add by sy, nam
#define ADD_NEW_CHART			1		//추가모드		2005. 05. 19 add by sy, nam
#define	ADD_NEW_ITEM			2		//종목추가
#define REPLAY_CHART			2		//복기모드		2005. 08. 01 add by sy, nam
#define OVERLAPP_CHART			3		//중첩모드		2005. 05. 19 add by sy, nam

#define TICK_DATA_CHART			'0'		// Tick	차트	2005. 05. 19 add by sy, nam
#define MIN_DATA_CHART			'1'		// 분	차트	2005. 05. 19 add by sy, nam
#define DAY_DATA_CHART			'2'		// 일	차트	2005. 05. 19 add by sy, nam
#define WEEK_DATA_CHART			'3'		// 주	차트	2005. 05. 19 add by sy, nam
#define MONTH_DATA_CHART		'4'		// 월	차트	2005. 05. 19 add by sy, nam
#define YEAR_DATA_CHART			'5'		// 년	차트	2005. 05. 19 add by sy, nam
//#define SEC10_DATA_CHART		'9'		// 삼성사용하지않음	// 10초 차트	2005. 09. 28 add by sy, nam
//#define SEC30_DATA_CHART		'6'		// 30초 차트	2005. 09. 28 add by sy, nam
#define SEC_DATA_CHART			'6'		// 초   차트	2005. 09. 28 add by sy, nam
#define HOUR_DATA_CHART			'7'		// 시	차트	2006. 09. 27 삼성차트

//#define DAY_DATA_CHART			'1'		// 일	차트	2005. 05. 19 add by sy, nam
//#define WEEK_DATA_CHART			'2'		// 주	차트	2005. 05. 19 add by sy, nam
//#define MONTH_DATA_CHART		'3'		// 월	차트	2005. 05. 19 add by sy, nam
//#define YEAR_DATA_CHART			'5'		// 년	차트	2005. 05. 19 add by sy, nam
//#define TICK_DATA_CHART			'6'		// Tick	차트	2005. 05. 19 add by sy, nam
//#define MIN_DATA_CHART			'7'		// 분	차트	2005. 05. 19 add by sy, nam
//#define SEC10_DATA_CHART		'8'		// 10초 차트	2005. 09. 28 add by sy, nam
//#define SEC30_DATA_CHART		'9'		// 30초 차트	2005. 09. 28 add by sy, nam

//==============================================================================
//2005. 06. 24 add by sy, nam
//리얼 받을때..SetReal Price 함수로 분기시 구분자 값ㅇ
#define STOCK_REAL				0
#define UPJONG_REAL				1
#define FUTURE_REAL				2
#define OPTION_REAL				3
#define ELW_REAL				5
#define COMMODITY_REAL			6		// 상품선물옵션차트 - ojtaso (20071001)
#define FO_REAL					7		// 선물옵션지표 - alzioyes(20110124)
#define SFUTURE_REAL			8		// 주식선물실시간 - 이상신(20130618)
//==============================================================================



//1.선물.2.콜 3.풋 4.스프레드 K. 코스닥 선물 
#define FUTURE_CODE				'1'		// 선물코드		2005. 06. 02 add by sy, nam
#define CALL_OPT_CODE			'2'		// 콜 옵션		2005. 06. 02 add by sy, nam
#define PUT_OPT_CODE			'3'		// 풋 옵션		2005. 06. 02 add by sy, nam
#define SPREAD_CODE				'4'		// 스프레드		2005. 06. 02 add by sy, nam
#define QFUTURE_CODE			'K'		// 코스닥 선물  2005. 09. 22 add by sy, nam


// 05. 06. 22 add by sy, nam
// 원, %, $ 구분 정의 
#define WON_TYPE				0
#define PERCENT_TYPE			1
#define DOLLAR_TYPE				2
// --------------------------------------------------------------------------
//  Chart\COMMONTR\TRDefine.h에서 정의하도록 정리함.
// --------------------------------------------------------------------------
//#define QUERY_STRMARKET_10452	"HWS10452" //2005.10.20  투자자 순매수 만
//#define QUERY_STR_FSTAR			"HWS10416" //2005.11.01	 스타 선물
// #define QUERY_STRJONGMOK		"d1010"
// #define QUERY_STRUPJONG			"d1011"
// #define QUERY_STRFUTOPT			"d1012"
// #define QUERY_STRSTOCKFUTOPT	"d1013"
// #define QUERY_STRFOREIGN		"d1014"
// #define QUERY_STRMARKET			"d1015"
// #define QUERY_STRDOLLAR			"d1018"
// #define QUERY_STOCK_HOGA		"c1002"
// #define QUERY_FUTOPT_HOGA		"f0201"
// #define QUERY_STOCKFUT_HOGA		"s3301"
// #define QUERY_STOCKOPT_HOGA		"f3201"
// #define QUERY_ACCOUNTCODE		"a4017"
// #define QUERY_ACCOUNTCONC		"a4018"
//#define QUERY_STRKQFUTOPT		"HWS10420" //2005.06.24  코스닥선물옵션 차트데이타 조회
//#define QUERY_STRSTOCKOPT		"HWS10421" //2005.06.24  주식옵션 차트데이타 조회
//#define QUERY_STRTHEME_GROUP	"HWS10410" //2005.06.23
//#define QUERY_STRTHEME_ITEM		"HWS10411" //2005.06.23
//#define QUERY_STRSTOCK_COMP		"HWS10430" //2005.07.12  주식 비교차트 종목데이터 조회
#define QUERY_STR_AVGMP			"HWS10418" //05.12.01	사용자 지정 이동평균 값 조회 

#define LINK_FUTURE_CODE		"101AA"		//07.01.22 연결선물 코드(삼성)

#define MEMCPYBYTARSIZE(_Tar,_Src) memcpy(_Tar,_Src,sizeof(_Tar));
#define MEMCPYBYTARSIZE2(_Tar,_Src,_lOffset) {memcpy(&_Tar,&_Src[_lOffset],sizeof(_Tar));_lOffset+=sizeof(_Tar);}
#define MEMCPYBYSRCSIZE(_Tar,_Src,_lOffset) {memcpy(&_Tar[_lOffset],(void*)&_Src,sizeof(_Src)); _lOffset+=sizeof(_Src);}
#define MEMCPYWITHNULL(szTarget,szSrc) {memcpy(szTarget,szSrc,sizeof(szSrc)); szTarget[sizeof(szSrc)] = NULL;}
#define SHIFTCONTROL(Wnd,nWidth,rect)	{Wnd.GetWindowRect(rect);ScreenToClient(rect);Wnd.SetWindowPos(&CWnd::wndTop,rect.left+nWidth,0,0,0,SWP_NOSIZE);Wnd.Invalidate();}

#define GAP_TABDLG			(4)
#define HEIGHTOFJONGSTATUS	(20)
#define WIDTHOFBUTTON		(40)
#define HEIGHTOFCOMBO		(20)
#define HEIGHTOFTOOLBARBUTTON	(19)
#define WIDTHOFTOOLBARBUTTON	(20)

//#define	CRLEFTBARBACK	RGB(221,224,233)
//#define	CRBACK			RGB(221,224,233)
#define	CRLEFTBARBACK	RGB(225,226,231)
//#define	CRBACK			RGB(239,242,247)
#define	CRBACK			RGB(222,228,234)
//#define CRGRID			RGB(239,243,246)
#define CRGRID			RGB(255,255,255)
#define CRSELECTED		RGB(180,200,240)
#define DEFAULT_BKGRND_COLOR		RGB(213,213,213)

//@유진추가
//#define	CLR_SIDEBAR_BKGRND			RGB(177,199,234)
//#define	CLR_SPLIT_BKGRND			RGB(177,199,234)
//#define	CLR_LEFTBAR_BKGRND			RGB(177,199,234)
//#define	CLR_INPUTBAR_BKGRND			RGB(177,199,234)
//#define	CLR_EXTINPUTBAR_BKGRND		RGB(177,199,234)
//#define	CLR_TOOLBAR_BKGRND			RGB(177,199,234)
#define		CLR_FONT_DEFAULT			RGB(0,0,1)
//@유진추가


#define DEFAULTCNTATONCE (400)
#define MINICNTATONCE	(120)
#define GRDMX			(3000)
#define CHTMX			(5200)
#define CHTMXREQUEST	(5000)
#define BOJO_MSG_LENGTH	(300)


#define GWANSIM_GRPNO_LEN	(2)		//2005. 07. 25
#define GWANSIM_MAX			(100)	//2005. 07. 25


#define ERRORMSG1	"해당종목을 더이상 추가할수없습니다."
#define ERRORMSG2	"업종지수지표는 [주]/[일]/[월]간에만 적용이 가능합니다."
#define ERRORMSG3	"등록할 종목을 선택하십시오."
#define ERRORMSG4	"시장지표는 [일]간에만 적용이 가능합니다."
#define ERRORMSG5	"조회가 지연되고 있습니다. 다시 조회하시기 바랍니다."
#define ERRORMSG6	"조회중 에러가 발생되었습니다. 다시 조회하시기 바랍니다."
#define ERRORMSG7	"저장한 차트화일이 없습니다."

#define IMGLIST_15X15		0x0001
#define IMGLIST_20X20		0x0002
#define IMGLIST_20X20TOOL	0x0004
#define IMGLIST_46X20		0x0008
#define IMGLIST_50X20		0x0010
#define IMGLIST_28X22		0x0020
#define IMGLIST_17X20		0x0040
#define IMGLIST_43X20		0x0080
#define IMGLIST_58X20		0x0100
#define IMGLIST_46X20GWN	0x0200 
#define IMGLIST_8X25		0x0400 
#define IMGLIST_151X21		0x0800 
#define IMGLIST_72X20		0x1000 

typedef struct {
	CString m_szKey;		// 공유명
	CString m_szValue;		// 공유값
}STSHAREDDATA;

typedef struct {
	CString m_strCode;
	CString m_strTime;
//@유진추가
	UINT	m_nDefault;
	UINT	m_nMode;
//@유진추가
}STINPUTCODEDATA;

typedef struct {
	CString m_strCode;
	BOOL	m_bDontPutInShared;
	BOOL	m_bRequestWithoutHesitation;
}STINPUTCODEDATA2;

typedef struct {
	BYTE	m_bType;	//주식:0,업종:1.선물:2,옵션:3
	CString m_strCode;
}ST_CODEDATA;

typedef CList<ST_CODEDATA, ST_CODEDATA>		LIST_CODEDATA;
//bDontPutInShared				는 코드공유에 넣어야 하는지 여부이고, 
//bRequestWithoutHesitation     현재 조회코드하고 동일한 종목코드가 들어가면 AP조회가 안되도록 
//								되어있는데  을 TRUE로 하면 종목동일여부와 상관없이 AP조회하라는 얘기

// Favorite Searcher Info Structure
typedef struct  {
	char m_chTitle[64];
	long m_lTabNum;
	long m_lSubData1;
	long m_lSubData2;
	long m_lSubData3;
	long m_lSubData4;
	long m_lSubData5;
	char m_chData1[64];
	char m_chData2[64];
	char m_chData3[64];
}stFvrData;

// Chart Basical Info Structure
typedef struct  {
	long m_lTotalCnt;
	long m_lZoomValue;
	long m_lStartDate;		//2005. 07. 25 add by sy, nam
	long m_lEndDate;
	char m_chType;			// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	CString m_strTick;		// 2Byte Tick Number
	CString m_strSec;		// 2Byte Sec Number
	CString m_strMin;		// 2Byte Min Number
	CString m_strDay;		// 2Byte Day Number
	CString m_strHour;		// 2Byte Day Number
	CString m_strCode;
	int  m_nChartNumericValue;
	BOOL m_bFackedCandle;
	int  m_nSelCol;			//2005. 05. 24 add by sy, nam  InputBar등 기타 현재 선택된 차트의 Index로 차트 영역의 값을 Read
	int  m_nSelRow;			//2005. 05. 24 add by sy, nam  InputBar등 기타 현재 선택된 차트의 Index로 차트 영역의 값을 Read

	BOOL m_bRevised;		//05. 08. 22 - 수정주가
	BOOL m_bLog;			//05. 08. 22 - 로그 
	CString m_strChartLineType ;// 05. 08. 23 - 차트 모양 문자열
	BOOL m_bStartDateState;	//05. 08. 25 - 시작일이 선택되어 있는지 여부.
	
}stChartInitData;

// Server Input Structure
typedef struct  {
	CString		m_strTRCode;
	void*		m_pBuffer;
	long		m_lLength;
	CWnd*		m_pWnd;
	int			m_nType;		// 0: Normal 1:Invoke
	void*		m_pRqstChartItem;	//TR을 요청한 ChartItem
}stSend2Server;




// Server Output Structure
typedef struct  {
	CString		m_strTRCode;
	void *		m_pBuffer;
	DWORD		m_dwLength;
}stReceiveFromServer;

typedef struct  {
	HWND		m_hChartMng;
	CWnd*		m_pChartItem;
	void *		m_pBuffer;	
	DWORD		m_dwLength;
}stMSGToInvoke;


// Chart Equity History Info Structure(In Server)
//typedef struct _S_33100_S
//{
//	char    date[8];			/* 날짜     		*/
//	char    open[10];			/* 시가     		*/
//	char    high[10];			/* 고가     		*/
//	char    low[10];			/* 저가     		*/
//	char    close[10];			/* 종가   			*/
//	char    volume[10];     	/* 거래량   		*/
//	char    value[12];     	/* 거래대금    20351005 구본희추가   		*/
//	char	rights[1];			/* 대당락, 권리락..	*/
//} S33100_S;


// Chart Equity History Info Structure (In Client)
//typedef struct _S33100_CHART
//{
//	char    tmp[5];         			/* Chart Count      */
//	char    bojomsg[BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
//	S33100_S  chart[CHTMX];
//} S33100_CHART;

// Chart Equity  Info Structure (In Server's Output )
//typedef struct _S33100_O 
//{
//	char	name[20];			/* 종목명			*/
//	char	jang[10];			/* 장구분 : 'KOSPI', 'KOSDAQ', 'KOSPI200' */
//	char    price[10]; 		    /* 현재가   		*/
//	char    sign[1];        	/* 등락부호 		*/
//	char    change[10];     	/* 대비     		*/
//	char    chrate[6];      	/* 대비율   		*/
//	char    volume[10];     	/* 거래량   		*/
//	char    offer[10];      	/* 매도호가 		*/
//	char    bid[10];        	/* 매수호가 		*/
//	char    preprice[10];   	/* 기준가   		*/
//	char    nkey[14];           /* 이전키           */
//	char    Cchtsize[ 6];		/* Chart Size       */
//	//------------- PosForChart
//	char    tmp[5];         	/* Chart Count      */
//	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256으로 바꿈 05.05.06*/
//	S33100_S  chart[CHTMX];
//} S33100_O;

//@유진삭제
typedef struct _S_33100_S
{
	char    date[8];			/* 날짜     		*/
	char    open[10];			/* 시가     		*/
	char    high[10];			/* 고가     		*/
	char    low[10];			/* 저가     		*/
	char    close[10];			/* 종가   			*/
	char    volume[10];     	/* 거래량   		*/
	//char    value[12];     	/* 거래대금    20351005 구본희추가   		*/
	char	rights[3];			/* 대당락, 권리락..	*/
} S33100_S;

typedef struct _S33100_CHART
{
	char    tmp[5];         			/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
	S33100_S  chart[CHTMX];
} S33100_CHART;

typedef struct _S33100_O 
{
	char	name[20];			/* 종목명			*/
	char    price[10]; 		    /* 현재가   		*/
//	char    open[10];			/* 시가             */
//	char    high[10];			/* 고가             */
//	char    low[10];			/* 저가             */
	char    sign[1];        	/* 등락부호 		*/
	char    change[10];     	/* 대비     		*/
	char    chrate[6];      	/* 대비율   		*/
//	char    volume[10];     	/* 거래량   		*/
//	char    uplmtprice[10];		/* 상한가           */
//	char    dnlmtprice[10];		/* 하한가           */
	char    preprice[10];   	/* 기준가   		*/
	char    nkey[14];           /* 이전키           */
	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256으로 바꿈 05.05.06*/
	S33100_S  chart[CHTMX];
} S33100_O;

//// Chart Equity History Request
//typedef struct _S33100_I
//{
//	char    shcode		[7];		/* 종목코드								*/
//	char	date		[8];		/* 검색 시작일							*/
//	char	num			[4];		/* 검색할 데이터 갯수					*/
//	char	unit		[3];		/* 틱/분/일 조회간격단위				*///<========2005.08.11
//	char    button		[1];		/* 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)	*/
//	char    modGb		[1];		/* 수정주가 구분 1:수정주가반영			*/	
//	char	nflag		[1];		/* 다음(1) / 조회(0) 구분자 			*/
//	char	nkey		[14];		/* 다음 / 이전 Fetch Key				*/
//	char	nDataGubun	[1];		/* '0': 가격, '1':비율					*/
//	char    ImgRgb		[1];        /* '0':허수봉 '1':실봉					*/
//	char    Irecordgb	[1];		/* 복기 구분값(1)						*///<========2005.08.11
//	char    nDayBefore	[2];		/* 복기 시작 n일전 						*///<========2005.10.14
//	char    Itime		[6];		/* 복기시간 (hhmmss)					*///<========2005.08.11 	
//	/// don't sent to server..
//	char	chNull;
//} S33100_I;
//@유진삭제
//@유진추가
// 주식조회

//typedef struct
//{
//	char	shcode		[15];	/* 종목코드			*/
//	char	date		[8];	/* 검색시작일		*/
//	char	num			[4];	/* 조회건수			*/
//	char	unit		[3];	/* 조회단위(틱/분/일)								*/	
//	char	button		[1];	/* 조회구분(틱(0)/분(1)/일(2)/주(3)/월(4)/년(5))	*/
//	char	modGb		[1];	/* 수정주가(1:수정주가반영)							*/
//	char	nflag		[1];	/* 조회모드(다음(1) / 조회(0) 구분자)				*/
//	char	nkey		[21];	/* 검색키(다음 / 이전 Fetch Key)					*/
//	char	nDataGubun	[1];	/* 가격구분('0': 가격, '1':비율)					*/
//	char	ImgRgb		[1];	/* 봉구분('0':허수봉 '1':실봉)						*/
//	char	Irecordgb	[1];	/* 복기구분값(1)	*/
//	char	nDayBefore	[2];	/* 복기시작n일전	*/
//	char	Itime		[6];	/* 복기시간(hhmmss) */
//	char	nOpt		[1];	/* 당일조회여부 */
//	// don't sent to server..
//	char	chNull;
//}p0602InBlock;
//
//typedef struct
//{
//	char	name		[20];	/* 종목명			*/
//	char	jang		[10];	/* 장구분			*/
//	char	price		[10];	/* 현재가			*/
//	char	sign		[1];	/* 등락부호			*/
//	char	change		[10];	/* 대비				*/
//	char	chrate		[6];	/* 대비율			*/
//	char	volume		[10];	/* 거래량			*/
//	char	offer		[10];	/* 매도호가			*/
//	char	bid			[10];	/* 매수호가			*/
//	char	preprice	[10];	/* 기준가			*/
//	char	nkey		[14];	/* 이전키			*/
//	char	cntsz		[6];	/* 차트사이즈		*/
//	char	fcnt		[5];	/* 차트카운트		*/
//	char	bojomsg		[256];	/* 보조메시지		*/
//}p0602OutBlock1;
//
//typedef struct
//{
//	char	date		[8];	/* 날짜				*/
//	char	open		[10];	/* 시가				*/
//	char	high		[10];	/* 고가				*/
//	char	low			[10];	/* 저가				*/
//	char	close		[10];	/* 종가				*/
//	char	volume		[10];	/* 거래량			*/
//	char	value		[12];	/* 거래대금			*/
//	char	rights		[1];	/* 권리락			*/
//}p0602OutBlock2;
//
//// 업종조회
//typedef struct
//{
//	char		Sjanggb[1];		/*  [장구분]장구분     */
//	char		Supcode[3];		/*  [업종코드]업종코드     */
//	char		Sdate[8];		/*  [검색시작일]검색시작일     */
//	char		Smaxbong[4];	/*  [검색할데이터갯수]검색할데이터갯수     */
//	char		Sunit[3];		/*  [N봉(틱분시일)]N봉(틱분시일)     */
//	char		Sbtngb[1];		/*  [조회구분]조회구분     */
//	char		Snextgb[1];		/*  [다음/조회]다음/조회     */
//	char		Snextkey[21];	/*  [페치키]페치키     */
//	char		nOpt[1];		/* 당일조회여부 */
//	// don't sent to server..
//	char	chNull;
//}p0603InBlock;
//
//typedef struct
//{
//	char		name[20];		/*  [종목명]종목명     */
//	char		jang[10];		/*  [장구분]장구분     */
//	char		price[10];		/*  [현재가]현재가     */
//	char		sign[1];		/*  [등락부호]등락부호     */
//	char		change[10];		/*  [대비]대비     */
//	char		chrate[6];		/*  [대비율]대비율     */
//	char		volume[10];		/*  [거래량]거래량     */
//	char		offer[10];		/*  [매도호가]매도호가     */
//	char		bid[10];		/*  [매수호가]매수호가     */
//	char		preprice[10];	/*  [기준가]기준가     */
//	char		nkey[14];		/*  [이전키]이전키     */
//	char		cntsz[6];		/*  [차트사이즈]차트사이즈     */
//	char		fcnt[5];		/*  [차트카운트]차트카운트     */
//	char		bojomsg[256];	/*  [보조메시지]보조메시지     */
//}p0603OutBlock1;
//
//typedef struct
//{
//	char		date[8];		/*  [날짜]날짜     */
//	char		open[10];		/*  [시가]시가     */
//	char		high[10];		/*  [고가]고가     */
//	char		low[10];		/*  [저가]저가     */
//	char		close[10];		/*  [종가]종가     */
//	char		volume[10];		/*  [거래량]거래량     */
//	char		value[12];		/*  [거래대금]거래대금     */
//	char		rights[1];		/*  [권리락]권리락     */
//}p0603OutBlock2;
//
//// 선물조회
//typedef struct
//{
//	char		Scode[15];		/*  [종목코드]종목코드     */
//	char		Sdate[8];		/*  [검색시작일]검색시작일     */
//	char		Smaxbong[4];		/*  [검색할데이터갯수]검색할데이터갯수     */
//	char		Sunit[3];		/*  [N봉(틱분시일)]N봉(틱분시일)     */
//	char		Sbtngb[1];		/*  [구분(틱분일주월)]구분(틱분일주월)     */
//	char		Snextgb[1];		/*  [조회구분(다음조회)]조회구분(다음조회)     */
//	char		Snextkey[21];		/*  [페치키]페치키     */
//	char		Simgrgb[1];		/*  [봉구분(허수실봉)]봉구분(허수실봉)     */
//	char		Srecordgb[1];		/*  [복기구분값]복기구분값     */
//	char		Sredate[2];		/*  [복기시작n일전]복기시작n일전     */
//	char		Stime[6];		/*  [복기시간(hhmmss)]복기시간(hhmmss)     */
//	char		nOpt[1];		/* 당일조회여부 */
//	// don't sent to server..
//	char	chNull;
//}p0604InBlock;
//
//typedef struct
//{
//	char		name[20];		/*  [종목명]종목명     */
//	char		jang[10];		/*  [장구분]장구분     */
//	char		price[10];		/*  [현재가]현재가     */
//	char		sign[1];		/*  [등락부호]등락부호     */
//	char		change[10];		/*  [대비]대비     */
//	char		chrate[6];		/*  [대비율]대비율     */
//	char		volume[10];		/*  [거래량]거래량     */
//	char		offer[10];		/*  [매도호가]매도호가     */
//	char		bid[10];		/*  [매수호가]매수호가     */
//	char		preprice[10];	/*  [기준가]기준가     */
//	char		nkey[14];		/*  [이전키]이전키     */
//	char		cntsz[6];		/*  [차트사이즈]차트사이즈     */
//	char		fcnt[5];		/*  [차트카운트]차트카운트     */
//	char		bojomsg[256];	/*  [보조메시지]보조메시지     */
//}p0604OutBlock1;
//
//typedef struct
//{
//	char		date[8];		/*  [날짜]날짜     */
//	char		open[10];		/*  [시가]시가     */
//	char		high[10];		/*  [고가]고가     */
//	char		low[10];		/*  [저가]저가     */
//	char		close[10];		/*  [종가]종가     */
//	char		volume[10];		/*  [거래량]거래량     */
//	char		value[12];		/*  [거래대금]거래대금     */
//	char		rights[1];		/*  [권리락]권리락     */
//}p0604OutBlock2;
//
//// ELW조회
//typedef struct
//{
//	char		Scode[15];		/*  [종목코드]종목코드     */
//	char		Sdate[8];		/*  [검색시작일]검색시작일     */
//	char		Smaxbong[4];		/*  [조회건수]조회건수     */
//	char		Sunit[3];		/*  [N봉(틱분시일)]N봉(틱분시일)     */
//	char		Sbtngb[1];		/*  [구분(틱분일주월)]구분(틱분일주월)     */
//	char		Snextgb[1];		/*  [조회구분(다음조회)]조회구분(다음조회)     */
//	char		Snextkey[21];		/*  [페치키]페치키     */
//	char		Simggb[1];		/*  [봉구분(허수실수)]봉구분(허수실수)     */
//	char		nOpt[1];		/* 당일조회여부 */
//	// don't sent to server..
//	char	chNull;
//}p0605InBlock;
//
//typedef struct
//{
//	char		name[20];		/*  [종목명]종목명     */
//	char		jang[10];		/*  [장구분]장구분     */
//	char		price[10];		/*  [현재가]현재가     */
//	char		sign[1];		/*  [등락부호]등락부호     */
//	char		change[10];		/*  [대비]대비     */
//	char		chrate[6];		/*  [대비율]대비율     */
//	char		volume[10];		/*  [거래량]거래량     */
//	char		offer[10];		/*  [매도호가]매도호가     */
//	char		bid[10];		/*  [매수호가]매수호가     */
//	char		preprice[10];	/*  [기준가]기준가     */
//	char		nkey[14];		/*  [이전키]이전키     */
//	char		cntsz[6];		/*  [차트사이즈]차트사이즈     */
//	char		fcnt[5];		/*  [차트카운트]차트카운트     */
//	char		bojomsg[256];	/*  [보조메시지]보조메시지     */
//}p0605OutBlock1;
//
//typedef struct
//{
//	char		date[8];		/*  [날짜]날짜     */
//	char		open[10];		/*  [시가]시가     */
//	char		high[10];		/*  [고가]고가     */
//	char		low[10];		/*  [저가]저가     */
//	char		close[10];		/*  [종가]종가     */
//	char		volume[10];		/*  [거래량]거래량     */
//	char		value[12];		/*  [거래대금]거래대금     */
//	char		rights[1];		/*  [권리락]권리락     */
//}p0605OutBlock2;

//@유진추가

// Chart Futures History Request
typedef struct _S36100_I
{
	char    jmcode		[15];			/* 종목코드							*/
	char	date		[8];			/* 검색 시작일						*/
	char	num			[4];			/* 검색할 데이터 갯수				*/
	char    unit		[3];			/* 기간  Number						*///<========2005.08.11
	char    button		[1];			/* 틱(0)/30초(1)/분(2)/일(3)/주(4)	*/
	char	nflag		[1];			/* 다음(1) / 조회(0) 구분자 		*/
	char	nkey		[14];			/* 다음 / 이전 Fetch Key			*/
	char    ImgRgb		[1];			/* '0':허수봉 '1':실봉              */
	char    Irecordgb	[1];			/* 복기 구분값(1)					*///<========2005.08.11
	char    nDayBefore	[2];			/* 복기 시작 n일전 					*///<========2005.10.14
	char    Itime		[6];			/* 복기시간 (hhmmss)				*///<========2005.08.11  	

	/// don't sent to server..
	char	chNull;
} S36100_I;

// Chart Futures Info Structure (In Server's Output )
typedef struct _S_36100_S
{
	char    date[8];			/* 날짜     		*/
	char    open[10];			/* 시가     		*/
	char    high[10];			/* 고가     		*/
	char    low[10];			/* 저가     		*/
	char    close[10];			/* 종가   			*/
	char    volume[10];     	/* 거래량   		*/
	//char    value[12];     		/* 거래대금   		*/
} S36100_S;

// Chart Future History Info Structure (In Client)
typedef struct _S36100_CHART
{
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */
	S36100_S  chart[CHTMX];
} S36100_CHART;

// Chart Futures Info Structure (In Server's Output )
typedef struct _S36100_O 
{
	char	name[20];			/* 종목명			*/
	char    price[10]; 		    /* 현재가   		*/
//	char    open[10];			/* 시가             */
//	char    high[10];			/* 고가             */
//	char    low[10];			/* 저가             */    
	char    sign[1];        	/* 등락부호 		*/
	char    change[10];     	/* 대비     		*/
	char    chrate[6];      	/* 대비율   		*/
//	char    volume[10];     	/* 거래량   		*/
//	char    uplmtprice[10];		/* 상한가           */
//	char    dnlmtprice[10];		/* 하한가           */
	char    preprice[10];   	/* 기준가   		*/
	//char	medoho[10];			/* 매도호가 		*/
	//char	mesuho[10];			/* 매수호가 		*/

   char    nkey    [ 14];   /* 이전키           */
	//char    Cchtsize[ 6];		/* Chart Size       */

	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */

	S36100_S  chart[CHTMX];

} S36100_O;

typedef struct _S35100_I
{
	char    radio		[1];			// 장구분 '1'-KOSPI, '2'-KOSDAQ, '3'-KOSPI200
	//char    upcode		[3];			// 종목코드             
	char    upcode		[4];			// 종목코드             //KOSCOM 4자리
	char    date		[8];			// 검색 시작일          
	char    num			[4];			// 검색할 데이터 갯수   
	char    unit		[3];			/* 기간  Number			*///<============2005.08.11
	char    button		[1];			// 30초(0)/분(1)/일(2)/주(3)/월(4)
	char    nflag		[1];			// 다음(1) / 조회(0) 구분자     
	char    nkey		[14];           // 다음 / 이전 Fetch Key        
	//char	nDataGubun	[1];			/* 0: 가격, 1:비율 */

	/// don't sent to server..
	char	chNull;
} S35100_I;

typedef struct _S_35100_S
{
	char    date[8];            // 날짜             
	char    open[10];           // 시가             
	char    high[10];           // 고가             
	char    low[10];            // 저가             
	char    close[10];          // 종가             
	char    volume[10];         // 거래량           
	//char    value[12];			// 거래대금    20351005 구본희추가          
} S35100_S;

typedef struct _S35100_CHART
{
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */
	S35100_S  chart[CHTMX];
} S35100_CHART;

typedef struct _S35100_O
{
	char    name[20];           // 종목명           
	char    price[10];          // 현재가           
//	char    open[10];			/* 시가             */
//	char    high[10];			/* 고가             */
//	char    low[10];			/* 저가             */    
	char    sign[1];            // 등락부호         
	char    change[10];         // 대비             
	char    chrate[6];          // 대비율           
//	char    volume[10];         // 거래량           
	char    preprice[10];       // 기준가           

	char    nkey[14];           // 이전키 
 	//char    Cchtsize[ 6];		/* Chart Size       */
	
	//==========================================================
	char    tmp[5];             // Chart Count      
	char    bojomsg[BOJO_MSG_LENGTH];        // Chart Bojo Data  

	S35100_S  chart[CHTMX];
} S35100_O;	

//<<<<<<<<<<<<  테마리스트검색 : 10410 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D10410_I
{
    char    dtGb        [ 1];   /* 0:전체 1:검색 2:종목 */
    char    jmcode      [ 6];   /* 종목코드             */
    char    text        [50];   /* 검색어               */
} D10410_I;

typedef struct _D10410_OG
{
	char    grptmcd     [ 3];   /* 테마코드 */
	char    grptmnm     [20];   /* 테마명   */	

    //char    grptmnm     [30];   /* 테마명   */
	//char    grptmcd     [ 4];   /* 테마코드 */
} D10410_OG;

typedef struct _D10410_O
{
    char        grdCnt      [ 4];
   // char        grdBjMsg    [18];
    D10410_OG   grdData     [GRDMX];
} D10410_O;





//<<<<<<<<<<<<  테마종목검색 : 10411 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D10411_I
{
    char    grptmcd      [ 3];  /* 테마코드 */
    char    filtercode  [ 1];   /* 1:전체, 2:보통, 3:우선, 4:관리, 5:감리    default: '1' 로 지정*/
    char    limitvol    [12];   /* 최소 거래량(단위: 천주)                   default: "0           " 으로 지정*/
} D10411_I;

typedef struct _D10411_OG
{
    char    sign        [ 1];
    char    hname       [20];
    char    jmcode      [ 6];
    char    price       [ 9];
    char    change      [ 9];
    char    chgrate     [ 9];
    char    volume      [10];
    char    showgb      [ 1];   /* 당일추가-상승하락한 구분 */
    char    showcnt     [ 3];   /* 당일추가-상승하락한 개수 */
    char    lmtgb       [ 1];   /* 상하한구분   */
    char    lmtcnt      [ 4];   /* 상하한개수   */
    char    updngb      [ 1];   /* 상승하락구분 */
    char    updncnt     [ 4];   /* 상승하락개수 */
    char    statgb      [ 1];   /* 장구분       */
} D10411_OG;

typedef struct _D10411_O
{
    char        grptmnm     [20]; //Han Hwa 30 -> 20

    char        sang        [ 3];
    char        up          [ 3];
    char        bo          [ 3];
    char        dn          [ 3];
    char        ha          [ 3];
    char        tot         [ 5];

    char        uprate      [ 6];
    char        rate        [ 6];
    char        uprate_sign [ 1];
    //char        grdCnt    [ 6]; //Han Hwa
    //char        grdBjMsg   [18];//Han Hwa
    D10411_OG   grdData     [GRDMX]; //서버에서 내려오는 Output은 grdData가 없지만 
	                                 //Client에서 작업하기 편하게 하기 위해 붙인다.  
} D10411_O;

//<<<<  시장동향, 투자자동향, 지표신호 리스트 TR : 20010, 20020, 20030 >>>>

//=============================================== 인풋 ==============
typedef struct _D20010_I
{
   char    dtGb        [ 1];   /* 0:전체 1:검색 2:종목 */
   char    Gb          [ 1];   /* 1:시세 2:투자자 3:지표신호 */
   char    jmcode      [ 6];   /* 종목코드             */
   char    text        [50];   /* 검색어               */
} D20010_I;

//=============================================== 아웃풋반복 =======
typedef struct _D20010_OG
{
    char    grptmnm     [30];   /* 테마명     */
    char    grptmcd     [ 4];   /* 테마인덱스 */
} D20010_OG;

//=============================================== 아웃풋 ============
typedef struct _D20010_O
{
    char    dtGb        [ 1];		/* 0:전체 1:검색 2:종목 */
    char    Gb          [ 1];		/* 1:시세 2:투자자 3:지표신호 4:추천전략 5:투자유의*/

    char        grdCnt      [ 6];           /* 그리드 카운트(4) + 보조메세지 카운트(2) */
    char        grdBjMsg    [18];           /* 보조메세지 */
    D20010_OG   grdData     [GRDMX];        /* 반복데이타 (D20010_OG * 그리드 카운트) */
} D20010_O;

//<<<<  시장동향, 투자자동향, 지표신호 종목 데이타 TR : 20011, 20021, 20031 >>>>
    
//=============================================== 인풋 ==============
typedef struct _D20011_I
{
    char    grptmcd      [ 4];  	/* 테마인덱스 */
    char    filtercode  [ 1];   /* 1:전체, 2:보통, 3:우선, 4:관리, 5:감리   default: '1' 로 지정 */
    char    limitvol    [12];   /* 최소 거래량(단위: 천주)                  default: "0           " 으로 지정 */
} D20011_I;
//=============================================== 아웃풋반복 =======
typedef struct _D20011_OG
{
  //  char    grptmcd	[ 4];		/* <= 추가부분  */
    char    sign        [ 1];  /* 등락부호 */
    char    hname       [20];  /* 종목명   */
    char    jmcode      [ 6];  /* 종목코드 */
    char    price       [ 9];  /* 현재가   */
    char    change      [ 9];  /* 전일대비 */
    char    chgrate     [ 9];  /* 등락률   */
    char    volume      [10];  /* 거래량   */
    char    showgb      [ 1];   /* 당일추가-상승하락한 구분 */
    char    showcnt     [ 3];   /* 당일추가-상승하락한 개수 */
    char    lmtgb       [ 1];   /* 상하한구분   */
    char    lmtcnt      [ 4];   /* 상하한개수   */
    char    updngb      [ 1];   /* 상승하락구분 */
    char    updncnt     [ 4];   /* 상승하락개수 */
    char    statgb      [ 1];   /* 장구분       */
} D20011_OG;
//=============================================== 아웃풋 ============
typedef struct _D20011_O
{
    char        grptmcd      [ 4];		/* <= 추가부분  */

	char        grptmnm     [30];  	/* 테마명 */    
	char        sang        [ 3];  	/* 상한종목수 */
	char        up          [ 3];  	/* 상승종목수 */
	char        bo          [ 3];  	/* 보합종목수 */
	char        dn          [ 3];  	/* 하락종목수 */
	char        ha          [ 3];  	/* 하한종목수 */
	char        tot         [ 5];  	/* 전체종목수 */   
	char        uprate      [ 6];  	/* 상승비중   */
	char        rate        [ 6];  	/* 평균대비율 */ 
	char        uprate_sign [ 1];	/* 상승비중 부호 */

	char        grdCnt      [ 6];   /* 그리드 카운트(4) + 보조메세지 카운트(2) */
	char        grdBjMsg    [18];   /* 보조메세지 */                             
    D20011_OG   grdData     [GRDMX];/* 반복데이타 (D20011_OG * 그리드 카운트) */ 
} D20011_O;





/* 관심 조회 응답 OUTPUT */
typedef struct
{
	char    winid           [ 4];   /* Window ID (Input값을 그대로 돌려줌.  */
	char    cnt             [ 4];
} TR_10310_O;
#define TR_10310_O_SZ   sizeof(TR_10310_O)

typedef struct
{
	char    under           [ 1];   /* 관감리구분 :  0:정상  1:관리  2:감리 */
	char    jg_tp           [ 1];   /* 구분                                 */
	char    jg_code         [15];   /* 종목코드                             */
	char    jg_name         [30];   /* 종목명                               */
	char    price           [ 9];   /* 현재가                               */
	char    change          [ 6];   /* 전일대비                             */
	char    change_rate     [ 5];   /* 대비율                               */
	char    sign            [ 1];   /* 등락부호                             */
	char    volume          [12];   /* 누적거래량                           */
	char    offerho         [ 9];   /* 매도호가                             */
	char    bidho           [ 9];   /* 매수호가                             */
	char    ofremain        [12];   /* 매도잔량                             */
	char    bidremain       [12];   /* 매수잔량                             */
	char    offertot        [12];   /* 총매도잔량                           */
	char    bidtot          [12];   /* 총매수잔량                           */
	char    openam          [ 9];   /* 시가                                 */
	char    high            [ 9];   /* 고가                                 */
	char    low             [ 9];   /* 저가                                 */
	char    value           [14];   /* 거래금액                             */
	char    uplmtprice      [ 9];   /* 상한가                               */
	char    dnlmtprice      [ 9];   /* 하한가                               */
	char    openyak         [ 9];   /* 미결제약정수량                       */
	char    parvalue        [ 9];   /* 액면가                               */
	char    presign         [ 1];   /* 전일등락부호                         */
	char    updnlmtcont_tp  [ 1];   /* 연속구분                             */
	char    updnlmtcont     [ 2];   /* 연속구분                             */
    char    updnlmtcont_tp2 [ 1];   /* 연속상한/하한구분                    신규 */
    char    updnlmtcont2    [ 2];   /* 연속상한/하한구분                    신규 */
	char    baseprice        [ 9];   /* 기준가                               */
	char    cvolume         [ 9];   /* 체결량                               */
	char    open_chrate     [ 5];   /* 시가전일대비율                       */
	char    high_chrate     [ 5];   /* 고가전일대비율                       */
	char    low_chrate      [ 5];   /* 저가전일대비율                       */
	char    hightime        [ 6];   /* 고가시간                     */
	char    lowtime         [ 6];   /* 저가시간                     */
    // 예상체결 추가 - cst 2004/09/10
    char    expsign         [ 1];   /* 예상체결 부호                        */
    char    expprice        [ 9];   /* 예상체결 가                          */
    char    expchang        [ 9];   /* 예상체결 대비                        */
    char    expchgrt        [ 9];   /* 예상체결 대비율                      */
    char    expvol          [12];   /* 예상체결 거래량                      */

} TR_10310_OS;

typedef struct
{
	char    winid           [ 4];   /* Window ID                            */
	char    jgcnt           [ 4];   /* 종목개수                             */
} TR_10310_I;

typedef struct
{
	char    jg_tp           [ 1];   /* 구분                                 */
									/* '1':장내, '2':코스닥,  '3':선물      */
									/* '4':옵션, '5':제3시장, '6':주식옵션  */
	char    jg_code         [15];   /* 종목코드                             */
} TR_10310_IS;



typedef struct {
	char szBuffer[12]; 
	char shcode[7];//* 종목코드
	char nAvg1[3]; //* 이평1 
	char nAvg2[3]; //* 이평2 
	char nAvg3[3]; //* 이평3 
	char nAvg4[3]; //* 이평4 
	/// don't sent to server..
	char	chNull;
} D10418_I; 


typedef struct _D10418_O { 
	char sBuffer[12]; // Use Only Client 
	char sShcode[7]; // Short Code 
	char sName[20]; // Code Name 
	char sCurPrice[16]; // Current Price 
	char sSum1[16]; // 예를 들어 5일 이라 그러면 오늘을 제외한 이전 4일의 합 
	char sSum2[16]; // 예를 들어 20일 이라 그러면 오늘을 제외한 이전 19일의 합 
	char sSum3[16]; // 예를 들어 60일 이라 그러면 오늘을 제외한 이전 59일의 합 
	char sSum4[16]; // 예를 들어 120일 이라 그러면 오늘을 제외한 이전 119일의 합 
	/// don't sent to server..
	char	chNull;
} D10418_O; 




//-->Input
typedef struct  {
    char    sBuffer[12];	// Use Only Client 
    char    b_n01[11];      //계좌번호
    char    b_n02[ 8];      //일자(매매일자)
    char    b_n03[12];      //종목코드(기본값(전체)-'%')
    char    b_n04[40];      //다음조회(기본값-'       ')
    char    b_n05[ 1];      //조회구분( 0 : 당일  1: 전일 2: 전전일 9:특정일 )
}DK_6102_MQ_IN;


//-->Output 
typedef struct  {

    char    b_n17[12];      //종목번호
    char    b_n18[40];      //종목명
    char    b_n19[10];      //매도수구분
    char    b_n20[ 9];      //수량
    char    b_n21[ 9];      //단가
    char    b_n22[15];      //약정금액
    char    b_n23[15];      //수수료+제세금
    char    b_n24[15];      //정산금액
    char    b_n25[20];      //신용구분명
    char    b_n26[ 8];      //대출일
    char    b_n27[15];      //신용금액
    char    b_n28[ 9];      //이자
    char    b_n29[15];      //연체료
    char    b_n30[ 2];      //시장구분 (01:단주 , 02:OTCBB , 03:코스닥, 04:거래소, 05:코스닥(ECN) , 06:거래소(ECN))
    char    b_n31[40];      //다음조회
}DK_6102_MQ_LOOP;


typedef struct  {
    char    sBuffer[12]; // Use Only Client 
    char    b_n01[15];      //매도(약정금액)합
    char    b_n02[15];      //매도(수수료+제세금)합
    char    b_n03[15];      //매도(정산금액)합
    char    b_n04[15];      //매도(신용,대출금액)합
    char    b_n05[15];      //매도(이자)합
    char    b_n06[15];      //매도(연체료)합
    char    b_n07[15];      //매도(결제금액)합
    char    b_n08[15];      //매수(약정금액)합
    char    b_n09[15];      //매수(수수료+제세금)합
    char    b_n10[15];      //매수(정산금액)합
    char    b_n11[15];      //매수(신용,대출금액)합
    char    b_n12[15];      //매수(이자)합
    char    b_n13[15];      //매수(연체료)합
    char    b_n14[15];      //매수(결제금액)합
    char    b_n15[ 8];      //매매일자
    char    b_n16[ 8];      //수도일자

    DK_6102_MQ_LOOP occur[20];
}DK_6102_MQ_OUT;


//=============================================================================
// 2005. 07. 06 추가 
// 해외 종목 차트 데이터를 조회하기 위한 구조 
//=============================================================================
typedef struct _D03040_I
{
	char    codegb	[ 1];	/* 종목구분							*/
	char    jmcode	[14];	/* 종목코드							*/
	char	date	[ 8];	/* 검색 시작일						*/
	char	num		[ 4];	/* 검색할 데이터 갯수				*/
	char    unit    [ 3];	/* min Number						*///<==========2005. 08.11 
	char    button	[ 1];	/* 분(0)/일(1)/주(2)/월(3)/년(4)	*/
	char	nflag	[ 1];	/* 다음(1) / 조회(0) 구분자			*/
	char	nkey	[14];	/* 다음 / 이전 Fetch Key			*///<==========2005. 08.11 
} D03040_I;


//typedef struct _D03040_OG
//{
//	char    date	[ 8];	/* 날짜     		*/
//	char    open	[10];	/* 시가     		*/
//	char    high	[10];	/* 고가     		*/
//	char    low		[10];	/* 저가     		*/
//	char    close	[10];	/* 종가   		*/
//	char    volume	[10];  	/* 거래량   		*/
//	//char    value	[11];  	/* 거래대금   		*/
//} D03040_OG;
//
//typedef struct _S03040_CHART
//{
//	char    chart_cnt	[5];         /* Chart Count      */
//	char    bojomsg		[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */
//	D03040_OG  chart	[CHTMX];
//} S03040_CHART;
//
//typedef struct _D03040_O
//{
//	char	name	[40];	/* 종목명		*/
//	char    price	[10];   /* 현재가   		*/
//	char    sign	[ 1];  	/* 등락부호 		*/
//	char    change	[10];  	/* 대비     		*/
//	char    chrate	[ 6];  	/* 대비율   		*/
//	char    volume	[10];  	/* 거래량   		*/
//	char	medoho	[10];	/* 매도호가 		*/
//	char	mesuho	[10];	/* 매수호가 		*/
//	char    nkey	[14];   /* 다음키           */
// 	char    Cchtsize[ 6];		/* Chart Size       */
//	//=========================================================================
//	char    chart_cnt[ 5];   /* Chart Count      */
//	char    bojomsg	 [BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
//	D03040_OG chart[CHTMX];
//	
//} D03040_O;

#define D03040_S   S36100_S
#define D03040_O   S36100_O
#define S03040_CHART   S36100_CHART
////=============================================================================

typedef struct 
{
	bool bRealCandle	;	// 실봉 or 실봉 + 허봉
	bool bShowHLBar		;	// 상하한바 표시
	bool bShowRight		;	// 귄리락 / 배당락 표시
	bool bRecvTimeShare	;	// 시간연동 수신
	bool bAutoSave		;	// 차트 종료시 자동저장 유무
	bool bShowLog		;	// 차트 로그표시 여부
	bool bRevisedValue	;	// 수정주가 표시 여부
	bool bStartDate		;	// 차트에 선택되고 있는 날짜가 상단 툴바에서 조회 시작일을 선택했는지 여부.
	//bool bShowCurPrice	;	// 차트 Y 축 눈금위에 현재가 표시 

} CHART_CONFIG_INFO;

//-----------------------------------------------------------------------------
// Author		:				Date :2006.9.20
// Reason		: ELW
//-----------------------------------------------------------------------------
typedef struct _S01007_I 
{
	char    shcode  [15];       /* 종목코드                             */
	char    date	[ 8];       /* 검색 시작일                          */
	char    num		[ 4];       /* 검색할 데이터 갯수                   */
	char    unit    [ 3];       /* Tick, Sec(10s, 30s), Min, Day Number */
	char    button	[ 1];       /* 틱(0)/분(1)/일(2)/주(3)/월(4)        */
	char    nflag	[ 1];       /* 다음(1) / 조회(0) 구분자             */
	char    nkey	[14];       /* 다음 / 이전 Fetch Key                */
} S01007_I;
#define S01007_I_SIZE    sizeof(S01007_I)

typedef struct _S01007_S
{
	char    date[8];			/* 날짜     		*/
	char    open[10];			/* 시가     		*/
	char    high[10];			/* 고가     		*/
	char    low[10];			/* 저가     		*/
	char    close[10];			/* 종가   			*/
	char    volume[10];     	/* 거래량   		*/
} S01007_S;

typedef struct _S01007_CHART
{
	char    tmp[5];         			/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
	S01007_S  chart[CHTMX];
} S01007_CHART;

typedef struct _S01007_O 
{
	char	name[20];			/* 종목명			*/
	char    price[10]; 		    /* 현재가   		*/
	char    open[10];			/* 시가             */
	char    high[10];			/* 고가             */
	char    low[10];			/* 저가             */
	char    sign[1];        	/* 등락부호 		*/
	char    change[10];     	/* 대비     		*/
	char    chrate[6];      	/* 대비율   		*/
	char    volume[10];     	/* 거래량   		*/
	char    uplmtprice[10];		/* 상한가           */
	char    dnlmtprice[10];		/* 하한가           */
	char    preprice[10];   	/* 기준가   		*/
	char    nkey[14];           /* 이전키           */
	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256으로 바꿈 05.05.06*/
	S01007_S  chart[CHTMX];
} S01007_O;




//#define	STOCK_CHART		0
//#define FUTOPT_CHART	1
//#define UPJONG_CHART	2

// 복수종목 차트 선택 : 복수종목 - ojtaso (20070122)
typedef struct _ST_SELCHART {
	//	CString			m_strRQ;				// ChartItem의 RQ (MultiChart는 사용하지 않음)
	CString			m_strChartGubun;		// 시장구분
	CString			m_strItemCode;			// 종목코드
	CString			m_strItemName;			// 종목명
	char			m_cType;				// 주기
	CString			m_strTickMinDayNumber;	// 틱 카운트	
	long			m_lCount;				// 데이터 카운트	
	int				m_nChartNumericValueType;	// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
	CString			m_strKey;
} ST_SELCHART;

// 상품선물옵션 2차 - ojtaso (20071016)
/*
typedef struct _codeinfo{
	int	nInfo[5];

	struct _codeinfo()
	{
		for (int i=0; i < 5; i++)
			nInfo[i] = 0;
	}

}codeinfo;
*/
// 콜 및 풋 옵션 월 리스트 
class COptionItem
{
public: 
	COptionItem(){};
	virtual ~COptionItem(){};

	//DATA
	CString m_strCode;
	int m_nCallPut;	// 2:call, 3:put
	int m_nYear;	// 만기년
	int m_nMonth;	// 만기월 
	int m_nEvtPrice;

	//FUNCTION
	void SetItem(CString strCode, int nCallPut, int nYear, int nMonth)
	{
		m_strCode = strCode;
		m_nCallPut = nCallPut;
		m_nYear = nYear;
		m_nMonth = nMonth;
		m_nEvtPrice = 0;
	};
	void SetItem(CString strCode, int nCallPut, int nYear, int nMonth, int nEvtPrice)
	{
		m_strCode = strCode;
		m_nCallPut = nCallPut;
		m_nYear = nYear;
		m_nMonth = nMonth;
		m_nEvtPrice = nEvtPrice;
	};
};

#endif //__CHART_MSG_DEF__BYGM0604