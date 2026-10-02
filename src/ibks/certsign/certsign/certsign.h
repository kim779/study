#pragma once

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


#ifdef __cplusplus
}
#endif
