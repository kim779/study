#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

// Ledger filler
#define ORDER_INFO_IDX          (0x10)

#define WM_ACCOUNT			(WM_USER+10000)
#define	WM_JANGO_CHANGE		(WM_USER+10001)

// define Window Messeage
#define WM_APP_SIG			(WM_USER+0xF0)
#define WP_ACCN_CHANGE			(0x00000001)
#define WP_F_JNGO_CHANGE		(0x00000002)
#define WP_S_JNGO_CHANGE		(0x00000003)

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// 매도 back
#define	COLOR_MSB			170		// 매수 back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)

// for speed parsing
class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
};
class op_notice_spliter {
public:
	bool operator() (char ch)
	{
		return (ch=='\r')||(ch=='\n')||(ch=='\t');
	}
};

typedef map<int, StringProxy> RealData;
typedef enum { CT_NONE=0, CT_FUTURE, CT_OPTION, CT_SPREAD } CODE_TYPE;
class SiseData {
public:
	SiseData() { code=""; type=CT_NONE; }
	CString code;
	CODE_TYPE type;
	char udgb;
	int gjga, siga, koga, jega, curr, pcurr;
	int rate, gvol, mgjv, shga, hhga, diff;
	int t_dvol, t_svol, t_dcnt, t_scnt, t_sdch;
	vector<int> fhoga;
	vector<int> frate;
	struct  {
		int mdga;
		int msga;
		int dvol;
		int svol;
		int dcnt;
		int scnt;
	} hoga[MAX_FHOGA];
};

class Order {
public:
	Order() { ZeroMemory(this, sizeof(Order)); }
	char acno[12];
	char code[12];
	int ojno;	// 원주문번호
	int juno;	// 주문번호
	int odgb;	// 주문구분
	int oprc;	// 주문가격
	int oqty;	// 주문수량
	int dprc;	// 체결가격
	int dqty;	// 체결수량
	int wqty;	// 미체결수량
	bool stop;  // stop주문여부
};

class OrderResult {
public:
	CString acno;
	CString juno;
	CString ojno;
	CString emsg;
};

class Micheg {
public:
	Micheg() { ZeroMemory(this, sizeof(Micheg)); }
	char acno[12];		// 계좌번호
	char code[12];		// 종목코드
	int oprc;			// 주문가격
	int d_cnt;			// 매도 미체결건수
	int s_cnt;			// 매수 미체결건수
	int d_wqty;			// 매도 미체결수량
	int s_wqty;			// 매수 미체결수량
};

class Jango {
public:
	Jango() { ZeroMemory(this, sizeof(Jango)); }
	void operator=(const Jango &rhs) { CopyMemory(this, &rhs, sizeof(Jango)); }
	char flag;		// for Window Message
	char acno[12];	// 계좌번호
	char code[12];	// 종목코드
	char name[40];	// 종목명
	char gubn[10];	// 구분
	int  jqty;		// 보유수량
	int  xqty;		// 청산가능수량
	int  pprc;		// 평균가
	double camt;	// 평가금액
	double tamt;	// 평가손익
	double srat;	// 수익률
	double mamt;	// 매입금액
};

class ReserveData
{
public:
	ReserveData() { ZeroMemory(this, sizeof(ReserveData)); }
	int odgb;	// 주문구분 1:매도, 2:매수
	int cprc;	// 체크가격
	int oprc;	// 주문가격
	int ptic;	// 체크/주문 가격 Tick
	int oqty;	// 주문수량
	int row;	// 그리드Row
	int col;	// 그리드Col
};

class ModifyData
{
public:
	ModifyData() { ZeroMemory(this, sizeof(ModifyData)); }
	int odgb;	// 주문구분 1:매도, 2:매수
	int fprc;	// 정정할 주문가격
	int tprc;	// 주문할 주문가격
	int rtic;   // 이동 Tick for Rsvd
};

class CancelData
{
public:
	CancelData() { ZeroMemory(this, sizeof(CancelData)); }
	int odgb;	// 주문구분 1:매도, 2:매수
	int cprc;	// 취소주문가격 
};

typedef vector<ReserveData> RsvdDataArray;			// 예약주문내역목록
typedef map<string, Jango> CodeJangoMap;			// 종목별 잔고 현황
typedef map<int, Order> OrderMap;					// 주문번호별 주문정보 맵
typedef map<string, OrderMap> CodeOrderMap;			// 종목별 주문번호별 주문정보 맵
typedef map<int, Micheg> PriceMichegMap;			// 주문가격별 미체결 수량정보
typedef map<string, PriceMichegMap> CodeMichegMap;	// 종목별 주문가격별 미체결 수량정보

class Account {				// 계좌종합정보
public:
	Account() : IsValid(FALSE) {}
	BOOL IsValid{};				// 검증된 계좌인지
	CString acno, pswd, acnm;	// 계좌번호, 비밀번호
	int mult{};					// 주문 배수
	CodeOrderMap order;			// 종목별 > 주문번호별 > 미체결 주문정보
	CodeJangoMap jango;			// 종목별 잔고내역
	CodeMichegMap micheg;		// 종목별 주문가격별 미체결 수량정보
};

typedef map<string, Account> AccountMap;	// 계좌번호별 계좌종합정보 맵



#define MY_DEBUG

#ifdef MY_DEBUG
	#define DECLARE_MYDEBUG()				extern FILE *_fpdbg;
	#define INIT_MYDEBUG(path)				_fpdbg = fopen(path, "wb");
	#define UNINIT_MYDEBUG()				fclose(_fpdbg);
	#define FPRT(y)							if(_fpdbg) { fprintf(_fpdbg, y"\r\n"); fflush(_fpdbg); }
	#define FPRT1(y, a1)					if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1); fflush(_fpdbg); }
	#define FPRT2(y, a1, a2)				if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2);  fflush(_fpdbg); }
	#define FPRT3(y, a1, a2, a3)			if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3);  fflush(_fpdbg); }
	#define FPRT4(y, a1, a2, a3, a4)		if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3, a4);  fflush(_fpdbg); }
	#define FPRT5(y, a1, a2, a3, a4, a5)	if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3, a4, a5);  fflush(_fpdbg); }
#else
	#define DECLARE_MYDEBUG()
	#define INIT_MYDEBUG(path)
	#define UNINIT_MYDEBUG()
	#define FPRT(y)						
	#define FPRT1(y, a1)					
	#define FPRT2(y, a1, a2)				
	#define FPRT3(y, a1, a2, a3)			
	#define FPRT4(y, a1, a2, a3, a4)		
	#define FPRT5(y, a1, a2, a3, a4, a5)	
#endif