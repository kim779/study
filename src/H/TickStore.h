#pragma once
#include <bitset>
#define CODE_LEN   13
//#define PRICE_LEN  16
//#define DIFF_LEN   16
//#define VOL_LEN    24
//#define RATE_LEN   16
//
//typedef struct TickSnapshot
//{
//	std::atomic<int> seq{ 0 };   // seqlock
//	DWORD ts_ms{ 0 };
//
//
//    char  RTStype[2];
//    char  code[CODE_LEN];
//    char  price[PRICE_LEN];
//    char  diff[DIFF_LEN];
//    char  volume[VOL_LEN];
//    char  rate[RATE_LEN];
//}TickSnapshot;;

#define MAX_SYMBOLS 1000
#define MAX_RTS_INDEX 1000
#define FIELD_STR_LEN 32
#define SYMBOL_STR_LEN 32

typedef struct TickSnapshot
{
    std::atomic<int> seq{ 0 };   // seqlock
    DWORD ts_ms{ 0 };

    char code[CODE_LEN];

    // symbol value storage
    char values[MAX_SYMBOLS][SYMBOL_STR_LEN];
    std::bitset<MAX_SYMBOLS> valid;   // 값 존재 여부
} TickSnapshot;





typedef BOOL(__stdcall* PFN_GET_TICK)(const char* code, TickSnapshot* out);
typedef void(* PFN_REGISTER_GET_TICK)(PFN_GET_TICK fn);

constexpr int MAX_CODES_PER_REQ = 200;
constexpr int CODE_STR_LEN = 16;

#define MAX_SYMBOLS_PER_REQ  1000   // 필요시 줄여도 됨

struct RTS_REGISTER_REQ
{
    HWND  hWnd;
    int   codeCount;
    char  codes[MAX_CODES_PER_REQ][CODE_STR_LEN];

    int   symbolCount;
    int   symbols[MAX_SYMBOLS_PER_REQ];   // 심볼 인덱스 목록 (0~999)
};






struct ST_SEND_TR
{
    CString trname;
    char* datB;
    int datL;
    BYTE stat;
    int key;
    HWND hSender;
};




#define MAX_REQ_SYMBOLS 128

struct RTS_READ_REQ
{
    char code[CODE_LEN];      // target code

    int symbolCount;          // number of symbols
    int symbols[MAX_REQ_SYMBOLS];

    // output
    char values[MAX_REQ_SYMBOLS][SYMBOL_STR_LEN];
    bool valid[MAX_REQ_SYMBOLS];

    DWORD ts_ms;
};


#define WM_RTS_MAIN_PUSH  (WM_USER + 0x5201)

struct RTS_PUSH_ITEM
{
    char code[CODE_LEN];
    int  symbol;
    char value[SYMBOL_STR_LEN];
    DWORD ts_ms;
};


struct DispatchJob
{
    std::string code;
    int symbol;
    std::vector<HWND> targets;
};