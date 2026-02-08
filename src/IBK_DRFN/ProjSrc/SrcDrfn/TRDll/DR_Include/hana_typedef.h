#ifndef HANA_TYPEDEF
#define HANA_TYPEDEF

#define SCREEN_NO		"HD770000"

#define	MYGETAPP	((CHD770000App *)AfxGetApp())

#define WM_MANAGE	(WM_USER + 1000)

/////////////////////////////////////////////////////////////////
//////  WM_Main_NormalWnd	Msg Kind
/////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define MK_GETROOT		1	// ROOT Directory
#define MK_GETUSER		2	// USER명
#define MK_SETPAL		3	// set color
#define MK_SENDTR		4	// sendtr
#define MK_VARIANT		5	// variant
#define MK_GETGROUP		6	// get group count
#define MK_GROUP		7	// HIWORD(wParam) 1 => add 0 => minus
#define MK_GETBITMAP		8	// get bitmap handle
#define MK_TREEVISIBLE		9	// get treesize
#define MK_BASKETVISIBLE	10	// get treesize
#define MK_ISEXPAND		11	// 그룹추가시 확장모드인가 분할모드인가...
#define MK_SIZE			12	// 화면사이즈 조절시 사용
#define MK_OPENSCR		13	// 화면 열기
#define MK_EXPECT		14	// 예상가 관련
#define MK_EXCEL		15	// 엑셀로 보내기
#define MK_SELGROUP		17	// 그룹선택에 관한 메시지
#define MK_MEMOUPDATE		18	// 메모 변경
#define MK_CLICKGROUP		19	// 그룹 클릭시 작업을 위한 옵션
#define MK_CLOSEGRID		20	// 그리드 창을 닫는다
#define MK_SETDATAKIND		21	// 데이터 종류
#define MK_TREEDATA		22	// 트리선택시 그룹으로 보내지는 메시지
#define MK_GETDATATITLE		23	// 현재 데이터의 타이틀을 요구한다.
#define MK_GETWND		24	// 윈도우 포인터 가져오기
#define MK_GETAFIELD		25	// 현재 선택된 필드 인덱스를 가져온다 From ToolWindow
#define MK_TURNCODE		26	// 종목돌려보기
#define MK_HAWKEYE		28	// hawkeye의 사용여부 확인
#define MK_INPUTOVER		29	// 오버되는 종목코드 입력하기
#define MK_SYNC			30	// get critical section
#define MK_RECVDATA		31	// data를 받았을 때 메시지
#define MK_GETMAX		32	// get max data
#define MK_RTSDATA		33	// rtsdata
#define MK_HIDECODE		34	// hide code search combo
#define MK_KILLFOCUS		35	// kill focus
#define MK_ENDDRAG		36	// end drag
#define MK_INSERTROW		37	
#define MK_DELETEROW		38	
#define MK_SETUPOK		39	// 설정 변경
#define MK_SAVE			40	// 관심그룹 저장
#define MK_SELINT		41	// 관심그룹 선택
#define MK_SELFIELD		42	// 필드 선택
#define MK_SEARCHOPT		43	// get search option
#define MK_CHANGEFIELD		44	// Change Field
#define MK_TOOLSIZE		45	// toolbar size
#define MK_CHART		46	// 차트 사용여부
#define MK_GETDATAKIND		47	
#define MK_GETID		48	// user id
#define MK_NEWS			50	// 뉴스
#define MK_MEMO			51	// 메모
#define MK_PROCDLL		52	// 내자신에게 procdll이 들어오는것을 버린다.
#define MK_SISECATCH		53	// 시세감시
#define MK_GROUPHEIGHT		54
#define MK_HSCROLL			55	// ADD PSH 20070911
#define MK_MKPROP			56	// ADD PSH 20070917
#define MK_GETMOVECFG		57	// ADD PSH 20070917
#define MK_SETMOVECFG		58	// ADD PSH 20070917
#define MK_GETMOVESAVE		59	// ADD PSH 20070917
#define MK_SETMOVESAVE		60	// ADD PSH 20070917
#define MK_TOOLBUTTON		61	// ADD PSH 20070917
#define MK_SORTBASE			62	// ADD PSH 20070917
#define MK_AUTOSAVE			63	// ADD PSH 20070917
#define MK_ALERTWAVE		64	// ADD PSH 20070917
#define MK_MARKETCOLOR		65	// ADD PSH 20070917
#define MK_MARKETCODE		66	// ADD PSH 20070917

// parser
#define SYMBOL_GRID		"$90303"
#define CH_DEL			0x7f
#define CH_TAB			char(0x09)
#define CH_LF			0x0A

#define BTN_WIDTH_BASE      12
#define BTN_WIDTH_STEP      13

#define HEIGHT_TAB				24

const UINT RMSG_GRIDSCROLLEND =	::RegisterWindowMessage("RMSG_GRIDSCROLLEND");

enum {
	CODE_JONGMOK = 1,
	CODE_KJONGMOK,
};

// 제공처 위치(0: 왼, 1: 오른, 2: 없음)
enum {
	POS_LEFT,
	POS_RIGHT,
	POS_HIDE,
};

enum {
	MOVE_UP,
	MOVE_DOWN,
};


#endif
