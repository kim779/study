#pragma once

#define CODE_LEN   13
#define PRICE_LEN  16
#define DIFF_LEN   16
#define VOL_LEN    24
#define RATE_LEN   16

typedef struct TickSnapshot
{
	std::atomic<int> seq{ 0 };   // seqlock
	DWORD ts_ms{ 0 };


    char  RTStype[2];
    char  code[CODE_LEN];
    char  price[PRICE_LEN];
    char  diff[DIFF_LEN];
    char  volume[VOL_LEN];
    char  rate[RATE_LEN];
}TickSnapshot;;

typedef BOOL(__stdcall* PFN_GET_TICK)(const char* code, TickSnapshot* out);
typedef void(* PFN_REGISTER_GET_TICK)(PFN_GET_TICK fn);

constexpr int MAX_CODES_PER_REQ = 200;
constexpr int CODE_STR_LEN = 16;

struct RTS_REGISTER_REQ
{
    HWND  hWnd;                       // 요청 화면 핸들
    int   codeCount;                  // 몇 개 요청
    char  codes[MAX_CODES_PER_REQ][CODE_STR_LEN]; // 종목코드 목록
};
