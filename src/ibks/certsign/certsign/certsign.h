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

//Sign data for th LOGIN step - produces a full PKCS#7 SignedData bundle
//with the signer's certificate embedded (server identifyes the account from 
//the embedded cert). Nedds CS_Select/CS_SelectInteractive to have succeeded
//Unlike CS_Sign (lean, no-cert, for repeat TR-signing), this is required
//for login because the server has no prior session to identify you by.
CS_API int CS_SignFull(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen);

// TR signing re-confirm: shows the vendor's small password dialog (no certificate picker)
// and, on success, replaces the session's encrypted password. Same as CertifyCtrl::checkPasswd().
// Returns 0 on success, vendor error code on failure (2417=wrong password, 2501=cancel).
CS_API int CS_VerifyPassword(HCS h, HWND parentWnd);

// After login (CertifyCtrl::CertifyFull caRUN): clears the stored password so the vendor shows its
// small password dialog, signs data as PKCS#7, and on success replaces the encrypted password.
// Returns 0 on success, vendor error code on failure (1000=cancel, 2417=wrong password).
CS_API int CS_SignFullPrompt(HCS h, HWND parentWnd, const unsigned char* data, int dataLen,
    unsigned char* sigOut, int* sigOutLen);

//clude 인증서 선택  - 로컬과 달리 항상 벤터 팝업(폰 pin/푸시확인) 필요
CS_API int CS_CloudSelectInteractive(HCS h, HWND parentWnd, char* dnOut, int dnOutCap, char* certSetNameOut, int certSetNameOutCap);

//cloude 서명 
CS_API int CS_CloudSignFull(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen);
#ifdef __cplusplus
}
#endif
