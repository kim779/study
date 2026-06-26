#ifndef EXAMPLE_H   // 매크로가 정의되어 있지 않은 경우
#define EXAMPLE_H  2  // 매크로를 정의합니다.

#pragma once
 struct	_ledgerH {
	char	size[6];	// 0	전문길이		I	S	X	전문전체(Size-6)
	char	type[1];	// 6	전문구분		I	S		B:계정계, I:정보계, F:파일송수신, N:Notice 자동갱신(R:정보계, D:계정계)
	char	encf[2];	// 7	압축구분		I/O	S	X	전문압축/암호화구분(00:압축,암호화 안함)
	char	dptr[3];	// 9	데이터 옵셋		I/O	S		입/출력 데이타가 시작되는 위치(공통헤더를 포함)
	char	svcd[8];	// 12	서비스코드		I	Cf	●	서비스코드               ♥ TxHead(3) of Map
	char	cono[3];	// 20	회사번호		I	S		회사번호 "991"
	char	usid[16];	// 23	사용자ID		I	C		트랜잭션을 발생시킨 사용자 고유정보 (예:사용자 접속ID)
	char	cogb[2];	// 39	접속분류코드		I	S		접속 접속매체 (HT : 자체HTS)
	char	fep1[12];	// 41	접속채널번호		I		X	접속 순차번호(3)+일련번호(9)
	char	pbid[16];	// 53	트랜잭션추적ID		I	Cf	X	전문 추적ID ('@'로시작)  ♥ 로그인 성공시 받은값 유지
	char	puip[12];	// 69	공인IP			I	S		공인IP (168.126.63.1의 경우 "168126063001" 형태)
	char	pcip[12];	// 81	사설IP			I	C		사설IP (168.126.63.1의 경우 "168126063001" 형태)
	char	dept[3];	// 93	처리지점 번호		I			처리PC가 위치한 지점
	char	brno[3];	// 96	지점번호		I			계좌가 속한 지점
	char	term[8];	// 99	단말번호		I	Cm	●	매체코드 (직원 : 001 고객 : 002)
	char	lang[1];	// 107	언어구분		I	S		K:한글, E:영문, C:중국, J:일본
	char	time[9];	// 108	AP처리시각		O			요청 처리시각 (HHMMSSmmm)
	char	ecod[4];	// 117	메시지코드		O			오류코드 :(정상코드: 0000 ~ 0999, 비정상코드 : 1000 ~ 9999, 요청시 Space로 채움)
	char	eknd[1];	// 121	출력메시지구분		O			
	char	fep2[1];	// 122	압축요청구분		I		X	
	char	fkey[4];	// 123	기능키목록		I/O	Cm	●	입력: 기능키(1바이트) ♥ TxHead(2) of Map (5:확인 7:연속 C:전송)
	char	rcnt[4];	// 127	요청 레코드 개수	I	Cm	●	일반적으로 "0000"을 입력
	char	fil1[6];	// 131	Filler			-		X	
	char	next[1];	// 137	연속여부		I/O	C		연속 데이터 유무 (Y:연속있음, 이외의 값:연속없음)
	char	nkey[18];	// 138	연속키			I/O	C	●	연속처리를 위한 키값
	char	usiz[2];	// 156	가변시스템정보길이	I		X	IBK 고정	
	char	hsiz[2];	// 158	가변헤더정보길이	I/O		X	사용 : epwd와 함께 사용
	char	msiz[2];	// 160	가변메시지길이		O		X	성공 및 오류메시지 (요청시 "00"입력)
	char	fep3[10];	// 162	Reserved for  FEP		S	X	
	char	mkty[1];	// 172	시장구분			Cfm	●	1:현물 2:선옵 3:기타    ♥TxHead(0) of MAP
	char	odrf[1];	// 173	주문구분			Cfm	●	1:주문 2:조회 3:이체    ♥TxHead(1) of MAP
	char	evid[4];	// 174	Reserved for  I/F			X	
	char	svrm[12];	// 178	서버IP				S		서버IP (168.126.63.1의 경우 "168126063001" 형태)
	char	skip[1];	// 190	Flag for I/F
	char	fil2[49];	// 191	Filler					X	
	char	eact[1];	// 240	에러구분					에러구분(0:상태바 1:메세지박스)
	char	emsg[99];	// 241	오류메세지					코드(4)+메시지(94)(9999 오류입니다 !)
	//char	epwd[44];	// 340	sha256암호화 된 계좌비밀번호
};

#define	L_ledgerH	sizeof(struct _ledgerH)		// 384 bytes

 struct _intersx
{
	char	gubn = '0';		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수	
	CString code;			// 종목코드
	CString name;			// 종목명
	CString	xprc;			// 보유단가
	CString	xnum;			// 보유수량
	CString	xupnum;			// 상장 주식수
	char	bookmark = '0';		// 북마크
	char	futureGubn{};		// 신용매매 구분
	double  creditPrc{};		// 신용금액
	double	maeipPrc{};		// 매입금액

	void copy(void* item);

	 void operator=(struct _intersx& ref)
	{
		code = ref.code;
		name = ref.name;
		gubn = ref.gubn;
		xprc = ref.xprc;
		xnum = ref.xnum;
		xupnum = ref.xupnum;
		bookmark = ref.bookmark;
		futureGubn = ref.futureGubn;
		creditPrc = ref.creditPrc;
		maeipPrc = ref.maeipPrc;
	}

	 void empty()
	{
		code.Empty();
		name.Empty();
		gubn = '0';
		xprc.Empty();
		xnum.Empty();
		xupnum.Empty();
		bookmark = '0';
		futureGubn = '0';
		creditPrc = 0;
		maeipPrc = 0;
	}
};

interface  IFC_test
{
public:
	void test() { AfxMessageBox("IFC_test"); }
	  void vir_test();
};

class cl_test : public IFC_test
{
public:
	void test() { AfxMessageBox("cl_test"); }
};

enum _xKIND {
	xALLCODE = 1,	// 전종목
	xKOSPI,		// 코스피
	xKOSDAQ,	// 코스닥
	xFUTURE,	// 선물
	xCALLCODE,	// 콜옵션
	xPUTCODE, 	// 풋옵션	
	xKOPSI200,	// 코스피 200
	xKOSDAQ50,	// 코스닥 50
	xKRX100,	// KRX100
	xFREECODE,	// 프리보드
	xETFCODE,	// ETF		
	xELWCODE,	// ELW
	xELWBASE,	// ELW기초자산
	xSINJUCODE,	// 신주인수권
	xKOSPIJISU,	// 코스피지수
	xKOSDAQJISU,	// 코스닥지수
	xTHEMACODE,	// 테마별
	xGROUPCODE,	// 그룹사별
	xINTEREST,	// 관심종목
	xPCALL,		// 주식옵션(콜)
	xPPUT,		// 주식옵션(풋)
	xKOSPIUP,	// 코스피업종
	xKOSDAQUP,	// 코스닥업종
	xKOSPIex,	// 코스피 추가
	xKOSDAQex,	// 코스닥 추가
	xREMAIN,	// 계좌별 보유잔고
	xISSUE,		// 이슈종목
	xELWISSUE,	// 발행회사별
	xELWASSET,	// 기초자산별
	xRECOMMAND,  // IBK추천종목
	xETNCODE	//2014.11.06 KSJ ETN 추가
};

struct _treeID
{
	UINT	kind : 7;		// 대분류 _xKIND 값
	UINT	skind : 7;	// 중분류 지정값 또는 인덱스
	UINT	degree : 2;	// value값이 %02d(두자리) %03d(세자리) 여부
	UINT	value : 10;	// 아이템 지정값 또는 인덱스 1 ~ 1024까지의 값
	UINT	depth : 3;	// 대분류/중분류/아이템
	UINT	dummy : 3;
};

#define CAST_TREEID(x)	((struct _treeID*)&x)

static UINT MAKE_TREEID(UINT kind, UINT skind = 0, UINT degree = 0, UINT value = 0, UINT depth = 0, UINT dummy = 0)
{
	const UINT	ret = kind;
	struct _treeID* treeID = (struct _treeID*)&ret;
	treeID->kind = kind;
	treeID->skind = skind;
	treeID->degree = degree;
	treeID->value = value;
	treeID->depth = depth;
	treeID->dummy = dummy;

	return ret;
}








struct	_param {
	int		key{};			// LOWORD(key) : screen key, HIWORD(key) : input size
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point{};			// font point
	int		style{};			// font style
	DWORD		tRGB{};			// text color index
	DWORD		pRGB{};			// paint color index
	CString		options;		// properties
};


#define PS_CREATE_WND 1   //화면 윈도우 만드는 프로세스



typedef struct _ST_COPY_DATA
{
	int iNum;
	HWND hwnd;
	struct	_param pParam;
	TCHAR atcData[MAX_PATH];
}ST_COPY_DATA;





#pragma once
//  프로세스 찾기

DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess);
BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam);
HWND GetHwndFromProcessHandle(HANDLE hProcess);
HWND GetHwndFromProcessID(DWORD dwProcessID);


DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess)
{
	// http://www.codeproject.com/Articles/21926/Getting-Process-ID-from-Process-Handle

	// [in]  process handle
	// [out] process ID, or 0xffffffff in case of failure

	if (hProcess == NULL)    return 0xffffffff;
	PTHREAD_START_ROUTINE lpStartAddress = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "GetCurrentProcessId");
	if (lpStartAddress == NULL) return 0xffffffff;
	// We do not know, whether process handle already has required access rights;

	// thus we have to duplicate it
	HANDLE hProcessAccAdj;
	BOOL bRes = DuplicateHandle(GetCurrentProcess(),
		hProcess, GetCurrentProcess(), &hProcessAccAdj,
		PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
		FALSE, 0);
	if (!bRes || hProcessAccAdj == NULL)
	{
		UINT unError = GetLastError();
		return 0xffffffff;
	}
	// Create a remote thread; as its starting address 

	// we specify GetCurrentProcessId() address,
	// which is the same for all processes. Note that GetCurrentProcessId() has no input
	// parameters, and we don't care about our thread stack cleanup,
	// as it will be destroyed right after this call

	DWORD dwThreadID;
	HANDLE hRemoteThread = CreateRemoteThread(hProcessAccAdj, NULL,
		0, lpStartAddress, 0, 0, &dwThreadID);
	CloseHandle(hProcessAccAdj);
	if (hRemoteThread == NULL) return 0xffffffff;
	// Wait until process ID is obtained

	// (replace INFINITE value below to a smaller value to avoid deadlocks);
	// then get the thread exit code, which is a value returned by GetCurrentProcessId()
	// in the context of the remote process
	WaitForSingleObject(hRemoteThread, INFINITE);
	DWORD dwExitCode;
	if (GetExitCodeThread(hRemoteThread, &dwExitCode) == 0)    dwExitCode = 0xffffffff;
	CloseHandle(hRemoteThread);
	return dwExitCode;
}

BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam)
{
	HANDLE hTarget = NULL;
	DWORD dwID = 0, dwSrcID = (DWORD)(*(int*)lParam);

	GetWindowThreadProcessId(hWnd, &dwID);

	if (dwID == dwSrcID)
	{
		*((int*)lParam) = (int)hWnd;
		*(((int*)lParam) + 1) = 1;
		return FALSE;
	}

	return TRUE;
}

HWND GetHwndFromProcessHandle(HANDLE hProcess)
{
	if (hProcess == NULL)
		return NULL;

	__int64 lltmp = 0; // 64bit 를 상위 하위 나누어 쓴다
	lltmp = (int)GetProcessIDbyProcessHandle(hProcess); // GetProcessId()

	EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out

	if (((int)(lltmp >> 32)))
		return (HWND)(int)lltmp;

	return NULL;
}

HWND GetHwndFromProcessID(DWORD dwProcessID)
{
	if (dwProcessID == 0 || dwProcessID == -1)
		return NULL;

	__int64 lltmp = 0; // 64bit 를 상위 하위 나누어 쓴다
	lltmp = (int)dwProcessID;

	EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out

	if (((int)(lltmp >> 32)))
		return (HWND)(int)lltmp;

	return NULL;
}

//#include <SDKDDKVer.h>
//
//#include <Lm.h>
//#pragma comment(lib, "netapi32.lib")
//BOOL GetWindowsVersion(DWORD& dwMajor, DWORD& dwMinor)
//{
//	static DWORD dwMajorCache = 0, dwMinorCache = 0;
//	if (0 != dwMajorCache)
//	{
//		dwMajor = dwMajorCache;
//		dwMinor = dwMinorCache;
//		return TRUE;
//	}
//
//	LPWKSTA_INFO_100 pBuf = NULL;
//	if (NERR_Success != NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf))
//		return FALSE;
//
//	dwMajor = dwMajorCache = pBuf->wki100_ver_major;
//	dwMinor = dwMinorCache = pBuf->wki100_ver_minor;
//	NetApiBufferFree(pBuf);
//
//	return TRUE;
//}

#endif // EXAMPLE_H   // 매크로를 닫습니다.
