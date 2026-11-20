#pragma once
// astxManager.dll - AhnLab ASTx (StSdkEx) wrapper for HTS.
// SDK calls only. UI / policy (when to install, restart, block HTS) stays in the caller.

#include <Windows.h>

#ifdef ASTXMANAGER_EXPORTS
#define AM_API __declspec(dllexport)
#else
#define AM_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

//function codes (same values as STSDKEX_FUNC_CODE_*)  can be OR-ed
#define AM_FUNC_PB		0x00000004   //anti-hacking (memory protect,  block remote)
#define AM_FUNC_FW	0x00000080		//firwall
#define AM_FUNC_PCSCAN	0x00008000  //pc scan
#define AM_FUNC_ALL     (AM_FUNC_PB | AM_FUNC_FW | AM_FUNC_PCSCAN)

//Results : 0 = success, otherwise STSDKEX_ERROR_* (0x32A41xxx) - use AM_Errstr()
#define AM_OK                   0x00000000L
#define AM_ERR_NOT_INITIALIZED  0x32A41007L
#define AM_ERR_NOT_INSTALLED    0x32A4100FL

typedef int(__stdcall* AM_EVENT_CB)(long code, void* param, long paramSize);  // SDK event (PB etc.)
typedef int(__stdcall* AM_AK_CB)(long code, LPARAM param);                    // keyboard protect event

// --- install / init ---
AM_API BOOL  AM_IsInstalled(void);
AM_API DWORD AM_Install(HWND parent);                          // shows vendor installer UI
AM_API DWORD AM_Init(const char* customerId, AM_EVENT_CB cb);  // cb may be NULL
AM_API void  AM_Uninit(void);                                  // stops AK + all funcs, then uninit
AM_API BOOL  AM_IsInitialized(void);

// --- PB / FW / PCSCAN ---
AM_API DWORD AM_Start(DWORD funcMask);  //already-started / priority-rule treated as success
AM_API DWORD AM_Stop(DWORD funcMask);       // already-stopped treated as success
AM_API DWORD AM_IsRunning(DWORD funcCode);  // 0 = running

// --- keyboard protect (AK) ---
AM_API DWORD AM_AkStart(AM_AK_CB cb);       // cb may be NULL
AM_API void  AM_AkStop(void);
AM_API DWORD AM_AkProtectEdit(HWND edit);
AM_API DWORD AM_AkGetText(HWND edit, char* out, DWORD* size);   // size: in = capacity, out = length
AM_API DWORD AM_AkGetEncText(DWORD customCode, HWND edit, char* out, DWORD* size, BYTE random[]);

// --- util ---
AM_API const char* AM_ErrStr(DWORD code);   // Korean message (CP949)




#ifdef __cplusplus
}
#endif
