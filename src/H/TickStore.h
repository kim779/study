#pragma once

#include <windows.h>
#include <atomic>

constexpr int CODE_LEN = 13; // 12자리 + null
constexpr int PRICE_LEN = 16;
constexpr int DIFF_LEN = 16;
constexpr int VOL_LEN = 24;
constexpr int RATE_LEN = 16;

struct TickSnapshot 
{ 
	std::atomic<int> seq; 
	DWORD ts_ms; 
	char RTStype[1]; 
	char code[CODE_LEN]; 
	char price[PRICE_LEN]; 
	char diff[DIFF_LEN]; 
	char volume[VOL_LEN]; 
	char rate[RATE_LEN]; 
};


//관심 화면에서 메인 자신핸들과 종목코드 등록
constexpr int MAX_CODES_PER_REQ = 200;
constexpr int CODE_STR_LEN = 16;

struct RTS_REGISTER_REQ
{
	HWND  hWnd;                       // 요청 화면 핸들
	int   codeCount;                  // 몇 개 요청
	char  codes[MAX_CODES_PER_REQ][CODE_STR_LEN]; // 종목코드 목록
};