#pragma once

#ifdef  SECURESESSION_EXPORTS
#define SS_API extern "C" __declspec(dllexport)
#else
#define SS_API extern "C" __declspec(dllimport)
#endif

typedef void* HSS;

//세션을 열고 서버로 보낼 최초 핸드셰에크 메시지 (AXISENCX 페이로드)를 만든다.
//password : 벤터 SDK 로컬 프로필 잠금 암호
// confpath : xc_conf.ini 전체경로
//outBuf : [out] 서버로 보낼 첫 핸드셰이크 메시지가 채워짐
//outLen : [in] outBuf 버터크기/ [out] 실제로 채워진 길이
//반환값 : 성공 시 세션 핸들(0이 아님), 실패 시 nullptr
SS_API HSS SS_Open(const char* password, const char* confPath,
    unsigned char* outBuf, int* outLen);

SS_API const char* SS_GetLastError(void);

SS_API int SS_Handshake(HSS h, const unsigned char* inBuf, int inLen, unsigned char* outBuf, int* outLen);

SS_API int SS_Encrypt(HSS h, const unsigned char* in, int inLen, unsigned char* out, int* outLen);

SS_API int SS_Decrypt(HSS h, const unsigned char* in, int inLen, unsigned char* out, int* outLen);

SS_API void SS_Close(HSS h);