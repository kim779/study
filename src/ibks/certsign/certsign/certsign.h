#pragma once
#include <stdio.h>
#include <stdarg.h>

#define cslog(fmt, ...) cslogImpl(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)

static void cslogImpl(const char* func, int line, const char* fmt, ...)
{
    char msg [1024];
    va_list args;
    va_start(args, fmt);
    _vsnprintf_s(msg, sizeof(msg), _TRUNCATE, fmt, args);
    va_end(args);

    char out[1200];
    _snprintf_s(out, sizeof(out), _TRUNCATE, "[CERTSIGN][%s:%d] %s\n", func, line, msg);
    OutputDebugStringA(out);
}

#ifdef CERTSIGN_EXPORTS
#define CS_API __declspec(dllexport)
#else
#define CS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HCS;

// Opens a signing session (initializes vendor APP_CONTEXT). Returns NULL on failure.
CS_API HCS  CS_Open(void);

// Selects a certificate by DN + password directly, without showing the vendor's
// own certificate-picker dialog. Returns 0 on success, or the vendor error code
// (sk_if_GetLastErrorCode-style, e.g. 2417=wrong password, 2500=cert not found) on failure.
CS_API int  CS_Select(HCS h, const char* dn, const char* password,
    unsigned char storage, const char* certSetName);

// Signs data with the certificate selected by CS_Select.
// sigOutLen: in = capacity of sigOut buffer, out = actual signature length written.
// Returns 0 on success, vendor error code on failure.
CS_API int  CS_Sign(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen);

// Closes the session and releases the vendor context.
CS_API void CS_Close(HCS h);

// Returns the last vendor error message (ANSI string from the vendor SDK).
CS_API const char* CS_GetLastError(void);

CS_API int CS_SelectInteractive(HCS h, HWND parentWnd,
    char* dnOut, int dnOutCap,
    unsigned char* storageOut,
    char* certSetNameOut, int certSetNameOutCap);

//Sign data for th LOGIN step - produces a full PKCS#7 SignedData bundle
//with the signer's certificate embedded (server identifyes the account from 
//the embedded cert). Nedds CS_Select/CS_SelectInteractive to have succeeded
//Unlike CS_Sign (lean, no-cert, for repeat TR-signing), this is required
//for login because the server has no prior session to identify you by.
CS_API int CS_SignFull(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen);

#ifdef __cplusplus
}
#endif
