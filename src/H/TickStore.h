#pragma once

#ifdef AXIS_MAIN
#define AXIS_API extern "C" __declspec(dllexport)
#else
#define AXIS_API extern "C" __declspec(dllimport)
#endif

#include <shared_mutex>
#include <bitset>
#define CODE_LEN   13

#define MAX_SYMBOLS 1000
#define MAX_RTS_INDEX 1000
#define FIELD_STR_LEN 32
#define SYMBOL_STR_LEN 32


#pragma message("--------------------------------------------------TickStore.h included")




//----------------------------------------------------------
//mainFrame  update_ticker  실시간 데이터 수신시 저장 구조체
typedef struct TickSnapshot
{
    std::atomic<int> seq{ 0 };   // seqlock
    DWORD ts_ms{ 0 };

    char code[CODE_LEN];

    // symbol value storage
    char values[MAX_SYMBOLS][SYMBOL_STR_LEN];
    std::bitset<MAX_SYMBOLS> valid;   // 값 존재 여부
} TickSnapshot;

constexpr int MAX_SLOT = 4096;
#ifdef AXIS_MAIN
extern std::unordered_map<std::string, int> g_codeToIndex;
extern TickSnapshot g_tickSlots[MAX_SLOT];
extern int g_nextIndex;
extern std::shared_mutex  g_codeMapLock;
#endif

AXIS_API int  Axis_EnsureSlotIndex(const char* code);
AXIS_API const TickSnapshot* Axis_GetTickSlots();

//typedef BOOL(__stdcall* PFN_GET_TICK)(const char* code, TickSnapshot* out);
//typedef void(* PFN_REGISTER_GET_TICK)(PFN_GET_TICK fn);

//----------------------------------------------------------
//관심종목에서 poop 조회시 mainframe 에 핸들과 코드, 심볼리스트를 저장할때

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


//----------------------------------------------------------

struct RtsSubscription
{
    // 코드 목록 (정렬해두면 비교/갱신이 쉬움)
    std::vector<std::string> codes;

    // 심볼 목록: 조회가 빠르게 bitset + 원본 인덱스 list 둘 다 들고가면 좋음
    std::bitset<1000> symbolMask;
    std::vector<int> symbols; // (0~999)
};


//----------------------------------------------------------

struct ST_SEND_TR
{
    CString trname;
    char* datB;
    int datL;
    BYTE stat;
    int key;
    HWND hSender;
};

//----------------------------------------------------------


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

//----------------------------------------------------------
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