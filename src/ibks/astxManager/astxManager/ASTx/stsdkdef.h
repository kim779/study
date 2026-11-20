#ifndef __STSDKDEF_H_
#define __STSDKDEF_H_

#include "stsdkmkd25.h"
#include "stsdkpb.h"
#include "stsdkfw.h"
#include "stsdkverify.h"
#include "stsdkpclog.h"
#include "stsdkpcscan.h"
#include "IStSdkEnv.h"


#ifdef STSDK_EXPORTS
#define STSDK_API __declspec(dllexport)
#else
#define STSDK_API __declspec(dllimport)
#endif


#if defined(_UNICODE) || defined(UNICODE)
	#define IAstxMkd8 IAstxMkd8W
	#define STSDK_GETMKD8OBJECT_NAME	"StSdk_GetMkd8ObjectW"
#else
	#define IAstxMkd8 IAstxMkd8A
	#define STSDK_GETMKD8OBJECT_NAME	"StSdk_GetMkd8ObjectA"
#endif

#define STSDK_GETPBOBJECT_NAME		"StSdk_GetPbObject"

#define STSDK_GETFWOBJECT_NAME		"StSdk_GetFwObject"

#define STSDK_GETVERIFYOBJECT_NAME	"StSdk_GetVerifyObject"

#define STSDK_GETPCLOGOBJECT_NAME	"StSdk_GetPcLogObject"

#define STSDK_GETENVOBJECT_NAME		"StSdk_GetEnvObject"

// --------------------------------------------------------------------------------------
// StandAlone

#if defined(_UNICODE) || defined(UNICODE)
#define STSDKSA_GETMKD8OBJECT_NAME	"StSdkSa_GetMkd8ObjectW"
#else
#define STSDKSA_GETMKD8OBJECT_NAME	"StSdkSa_GetMkd8ObjectA"
#endif

#define STSDKSA_GETPBOBJECT_NAME		"StSdkSa_GetPbObject"

#define STSDKSA_GETFWOBJECT_NAME		"StSdkSa_GetFwObject"

#define STSDKSA_GETVERIFYOBJECT_NAME	"StSdkSa_GetVerifyObject"

#define STSDKSA_GETPCLOGOBJECT_NAME		"StSdkSa_GetPcLogObject"

#define STSDKSA_GETENVOBJECT_NAME		"StSdkSa_GetEnvObject"

#define STSDKSA_GETENV2OBJECT_NAME		"StSdkSa_GetEnv2Object"

#ifdef __cplusplus
extern "C"
{
#endif


// --------------------------------------------------------------------------------------
// Interface

// Anti-keylogger (MKD25)
STSDK_API IAstxMkd8A* StSdk_GetMkd8ObjectA(DWORD* pdwErrorCode);
STSDK_API IAstxMkd8W* StSdk_GetMkd8ObjectW(DWORD* pdwErrorCode);

// Anti-keylogger (AK)
STSDK_API IAstxAkA* StSdk_GetAkObjectA(DWORD* pdwErrorCode);
STSDK_API IAstxAkW* StSdk_GetAkObjectW(DWORD* pdwErrorCode);

// Protect Browser
STSDK_API IAstxPb* StSdk_GetPbObject();

// Firewall
STSDK_API IAstxFw* StSdk_GetFwObject();

// Verify
STSDK_API IAstxVerify* StSdk_GetVerifyObject();

// PcLog
STSDK_API IAstxPcLog* StSdk_GetPcLogObject();

// Environment
STSDK_API IStSdkEnv* StSdk_GetEnvObject();


// --------------------------------------------------------------------------------------
// StandAlone Interface

// Anti-keylogger (MKD25)
STSDK_API IAstxMkd8A* StSdkSa_GetMkd8ObjectA(DWORD* pdwErrorCode);
STSDK_API IAstxMkd8W* StSdkSa_GetMkd8ObjectW(DWORD* pdwErrorCode);

// Anti-keylogger (AK)
STSDK_API IAstxAkA* StSdkSa_GetAkObjectA(DWORD* pdwErrorCode);
STSDK_API IAstxAkW* StSdkSa_GetAkObjectW(DWORD* pdwErrorCode);

// Protect Browser
STSDK_API IAstxPb* StSdkSa_GetPbObject();

// Firewall
STSDK_API IAstxFw* StSdkSa_GetFwObject();

// Verify
STSDK_API IAstxVerify* StSdkSa_GetVerifyObject();

// PcLog
STSDK_API IAstxPcLog* StSdkSa_GetPcLogObject();

// Env
STSDK_API IStSdkEnv*  StSdkSa_GetEnvObject();
STSDK_API IStSdkEnv2* StSdkSa_GetEnv2Object();

// PCScan
STSDK_API IAstxPCScan*  StSdkSa_GetPCScanObject();


#ifdef __cplusplus
}
#endif

#endif // __STSDKDEF_H_
