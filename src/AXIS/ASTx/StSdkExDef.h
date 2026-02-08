#ifndef __STSDKEXDEF_H_
#define __STSDKEXDEF_H_

#include "StSdkExErr.h"
#include "StSdkExCom.h"
#include "stsdkmkd25.h"

#ifdef STSDKEX_EXPORTS
#define STSDKEX_API __declspec(dllexport)
#else
#define STSDKEX_API __declspec(dllimport)
#endif


#if defined(_UNICODE) || defined(UNICODE)
#define IAstxMkd8 IAstxMkd8W
#define STSDK_GETMKD8OBJECT_NAME	"StSdk_GetMkd8ObjectW"
#else
#define IAstxMkd8 IAstxMkd8A
#define STSDK_GETMKD8OBJECT_NAME	"StSdk_GetMkd8ObjectA"
#endif

#if defined(_UNICODE) || defined(UNICODE)
#define IAstxAk IAstxAkW
#define STSDK_GETAKOBJECT_NAME		"StSdk_GetAkObjectW"
#else
#define IAstxAk IAstxAkA
#define STSDK_GETAKOBJECT_NAME		"StSdk_GetAkObjectA"
#endif


#define STSDKEX_Initialize						StSdkEx_1
#define STSDKEX_Unintialize						StSdkEx_2
#define STSDKEX_UnintializeEx					StSdkEx_19
#define STSDKEX_SetEnvOptionA					StSdkEx_16_A
#define STSDKEX_SetEnvOptionW					StSdkEx_16_W
#define STSDKEX_IsInstalled						StSdkEx_3
#define STSDKEX_InstallA						StSdkEx_4_A
#define STSDKEX_InstallW						StSdkEx_4_W
#define STSDKEX_InstallExA						StSdkEx_20_A
#define STSDKEX_InstallExW						StSdkEx_20_W
#define STSDKEX_StopUpdate						StSdkEx_21
#define STSDKEX_IsRunningFuncA					StSdkEx_5_A
#define STSDKEX_IsRunningFuncW					StSdkEx_5_W
#define STSDKEX_StartFuncA						StSdkEx_6_A
#define STSDKEX_StartFuncW						StSdkEx_6_W
#define STSDKEX_StopFuncA						StSdkEx_7_A
#define STSDKEX_StopFuncW						StSdkEx_7_W
#define STSDKEX_PBSetActivateSubFunc			StSdkEx_8
#define STSDKEX_SetEventCallback				StSdkEx_9
#define STSDKEX_SetEventCallbackEx				StSdkEx_10
#define STSDKEX_PBSetExOptionA					StSdkEx_11_A	
#define STSDKEX_PBSetExOptionW					StSdkEx_11_W	
#define STSDKEX_VerifyTrustSelfA				StSdkEx_12_A
#define STSDKEX_VerifyTrustSelfW				StSdkEx_12_W
#define STSDKEX_VerifyTrustSDK					StSdkEx_13
#define STSDKEX_GetPcLogA						StSdkEx_14_A	
#define STSDKEX_GetPcLogW						StSdkEx_14_W
#define STSDKEX_GetMkd8ObjectA					StSdkEx_17_A
#define STSDKEX_GetMkd8ObjectW					StSdkEx_17_W
#define STSDKEX_GetAkObjectA					StSdkEx_18_A
#define STSDKEX_GetAkObjectW					StSdkEx_18_W

#define STSDKEX_MkdInitializeA					StSdkEx_A_1
#define STSDKEX_MkdUninitializeA				StSdkEx_A_2
#define STSDKEX_MkdProtectEditControlA			StSdkEx_A_3
#define STSDKEX_MkdProtectEditControl2A			StSdkEx_A_4
#define STSDKEX_MkdGetTextA						StSdkEx_A_5
#define STSDKEX_MkdSetTextA						StSdkEx_A_6
#define STSDKEX_MkdGetEncTextA					StSdkEx_A_7
#define STSDKEX_MkdInitializeW					StSdkEx_A_8
#define STSDKEX_MkdUninitializeW				StSdkEx_A_9
#define STSDKEX_MkdProtectEditControlW			StSdkEx_A_10
#define STSDKEX_MkdProtectEditControl2W			StSdkEx_A_11
#define STSDKEX_MkdGetTextW						StSdkEx_A_12
#define STSDKEX_MkdSetTextW						StSdkEx_A_13
#define STSDKEX_MkdGetEncTextW					StSdkEx_A_14
#define STSDKEX_AkInitializeA					StSdkEx_A_15
#define STSDKEX_AkUninitializeA					StSdkEx_A_16
#define STSDKEX_AkGetEncTextA					StSdkEx_A_17
#define STSDKEX_AkInitializeW					StSdkEx_A_18
#define STSDKEX_AkUninitializeW					StSdkEx_A_19
#define STSDKEX_AkGetEncTextW					StSdkEx_A_20



#define FNAME_STSDKEX_Initialize				"StSdkEx_1"
#define FNAME_STSDKEX_Unintialize				"StSdkEx_2"
#define FNAME_STSDKEX_IsInstalled				"StSdkEx_3"
#define FNAME_STSDKEX_InstallA					"StSdkEx_4_A"
#define FNAME_STSDKEX_InstallW					"StSdkEx_4_W"
#define FNAME_STSDKEX_IsRunningFuncA			"StSdkEx_5_A"
#define FNAME_STSDKEX_IsRunningFuncW			"StSdkEx_5_W"
#define FNAME_STSDKEX_StartFuncA				"StSdkEx_6_A"
#define FNAME_STSDKEX_StartFuncW				"StSdkEx_6_W"
#define FNAME_STSDKEX_StopFuncA					"StSdkEx_7_A"
#define FNAME_STSDKEX_StopFuncW					"StSdkEx_7_W"
#define FNAME_STSDKEX_PBSetActivateSubFunc		"StSdkEx_8"
#define FNAME_STSDKEX_SetEventCallback			"StSdkEx_9"
#define FNAME_STSDKEX_SetEventCallbackEx		"StSdkEx_10"
#define FNAME_STSDKEX_PBSetExOptionA			"StSdkEx_11_A"
#define FNAME_STSDKEX_PBSetExOptionW			"StSdkEx_11_W"	
#define FNAME_STSDKEX_VerifyTrustSelfA			"StSdkEx_12_A"
#define FNAME_STSDKEX_VerifyTrustSelfW			"StSdkEx_12_W"
#define FNAME_STSDKEX_VerifyTrustSDK			"StSdkEx_13"
#define FNAME_STSDKEX_GetPcLogA					"StSdkEx_14_A"	
#define FNAME_STSDKEX_GetPcLogW					"StSdkEx_14_W"
#define FNAME_STSDKEX_Inner15					"StSdkEx_15"
#define FNAME_STSDKEX_SetEnvOptionA				"StSdkEx_16_A"
#define FNAME_STSDKEX_SetEnvOptionW				"StSdkEx_16_W"
#define FNAME_STSDKEX_GetMkd8ObjectA			"StSdkEx_17_A"
#define FNAME_STSDKEX_GetMkd8ObjectW			"StSdkEx_17_W"
#define FNAME_STSDKEX_GetAkObjectA				"StSdkEx_18_A"
#define FNAME_STSDKEX_GetAkObjectW				"StSdkEx_18_W"
#define FNAME_STSDKEX_UnintializeEx				"StSdkEx_19"
#define FNAME_STSDKEX_InstallExA				"StSdkEx_20_A"
#define FNAME_STSDKEX_InstallExW				"StSdkEx_20_W"
#define FNAME_STSDKEX_StopUpdate				"StSdkEx_21"

#define FNAME_STSDKEX_MkdInitializeA			"StSdkEx_A_1"
#define FNAME_STSDKEX_MkdUninitializeA			"StSdkEx_A_2"
#define FNAME_STSDKEX_MkdProtectEditControlA	"StSdkEx_A_3"
#define FNAME_STSDKEX_MkdProtectEditControl2A	"StSdkEx_A_4"
#define FNAME_STSDKEX_MkdGetTextA				"StSdkEx_A_5"
#define FNAME_STSDKEX_MkdSetTextA				"StSdkEx_A_6"
#define FNAME_STSDKEX_MkdGetEncTextA			"StSdkEx_A_7"
#define FNAME_STSDKEX_MkdInitializeW			"StSdkEx_A_8"
#define FNAME_STSDKEX_MkdUninitializeW			"StSdkEx_A_9"
#define FNAME_STSDKEX_MkdProtectEditControlW	"StSdkEx_A_10"
#define FNAME_STSDKEX_MkdProtectEditControl2W	"StSdkEx_A_11"
#define FNAME_STSDKEX_MkdGetTextW				"StSdkEx_A_12"
#define FNAME_STSDKEX_MkdSetTextW				"StSdkEx_A_13"
#define FNAME_STSDKEX_MkdGetEncTextW			"StSdkEx_A_14"
#define FNAME_STSDKEX_AkInitializeA				"StSdkEx_A_15"
#define FNAME_STSDKEX_AkUninitializeA			"StSdkEx_A_16"
#define FNAME_STSDKEX_AkGetEncTextA				"StSdkEx_A_17"
#define FNAME_STSDKEX_AkInitializeW				"StSdkEx_A_18"
#define FNAME_STSDKEX_AkUninitializeW			"StSdkEx_A_19"
#define FNAME_STSDKEX_AkGetEncTextW				"StSdkEx_A_20"



#ifdef __cplusplus
extern "C"
{
#endif


//----------------------------------------------------------------------------------
// Common Interface
// Initialize
STSDKEX_API DWORD STSDKEX_Initialize();

// Unintialize
STSDKEX_API DWORD STSDKEX_Unintialize();

// UnintializeEx
STSDKEX_API DWORD STSDKEX_UnintializeEx();

// SetEnvOptionA
STSDKEX_API DWORD STSDKEX_SetEnvOptionA(DWORD dwEnvOption, LPCSTR szValue);

// SetEnvOptionW
STSDKEX_API DWORD STSDKEX_SetEnvOptionW(DWORD dwEnvOption, LPCWSTR szValue);

//----------------------------------------------------------------------------------
// Update Interface
// IsInstalled
STSDKEX_API DWORD STSDKEX_IsInstalled();

// InstallA
STSDKEX_API DWORD STSDKEX_InstallA(DWORD dwUIMode, HWND hWnd, LPCSTR szCustomMasterURL);

// InstallW
STSDKEX_API DWORD STSDKEX_InstallW(DWORD dwUIMode, HWND hWnd, LPCWSTR szCustomMasterURL);

// InstallExA
STSDKEX_API DWORD STSDKEX_InstallExA(DWORD dwIEXMode, PFN_STSDKEX_EventCallback fnCallback, DWORD dwUIMode, HWND hWnd, LPCSTR szCustomMasterURL);

// InstallExW
STSDKEX_API DWORD STSDKEX_InstallExW(DWORD dwIEXMode, PFN_STSDKEX_EventCallback fnCallback, DWORD dwUIMode, HWND hWnd, LPCWSTR szCustomMasterURL);

// FinalizeInstall
STSDKEX_API DWORD STSDKEX_StopUpdate();

// IsRunningFuncA
STSDKEX_API DWORD STSDKEX_IsRunningFuncA(LPCSTR szCustomerID, DWORD dwFuncCode);

// IsRunningFuncW
STSDKEX_API DWORD STSDKEX_IsRunningFuncW(LPCWSTR szCustomerID, DWORD dwFuncCode);

// StartFuncA
STSDKEX_API DWORD STSDKEX_StartFuncA(LPCSTR szCustomerID, DWORD dwFuncCode, LPCSTR szCustomPolicyURL);

// StartFuncW
STSDKEX_API DWORD STSDKEX_StartFuncW(LPCWSTR szCustomerID, DWORD dwFuncCode, LPCWSTR szCustomPolicyURL);

// StopFuncA
STSDKEX_API DWORD STSDKEX_StopFuncA(LPCSTR szCustomerID, DWORD dwFuncCode);

// StopFuncW
STSDKEX_API DWORD STSDKEX_StopFuncW(LPCWSTR szCustomerID, DWORD dwFuncCode);

//----------------------------------------------------------------------------------
// Protect Browser Interface
// PBSetActivateSubFunc
STSDKEX_API DWORD STSDKEX_PBSetActivateSubFunc(DWORD dwSubFunc);

// SetEventCallback
STSDKEX_API DWORD STSDKEX_SetEventCallback(PFN_STSDKEX_EventCallback fnCallback);

// SetEventCallbackEx
STSDKEX_API DWORD STSDKEX_SetEventCallbackEx(HWND hWnd, DWORD dwMsgNum);

// PBSetExOptionA
STSDKEX_API DWORD STSDKEX_PBSetExOptionA(DWORD dwOption, LPCSTR szValue);

// PBSetExOptionW
STSDKEX_API DWORD STSDKEX_PBSetExOptionW(DWORD dwOption, LPCWSTR szValue);

//----------------------------------------------------------------------------------
// Verify Interface
// VerifyTrustSelfA
STSDKEX_API DWORD STSDKEX_VerifyTrustSelfA(LPCSTR szSignedName);

// VerifyTrustSelfW
STSDKEX_API DWORD STSDKEX_VerifyTrustSelfW(LPCWSTR szSignedName);

// VerifyTrustSDK
STSDKEX_API DWORD STSDKEX_VerifyTrustSDK();

//----------------------------------------------------------------------------------
// PcLog Interface
// GetPcLogA
STSDKEX_API DWORD STSDKEX_GetPcLogA(LPCSTR szEnvType, LPCSTR szOutputValue, DWORD* pdwSize, BOOL bReload);

// GetPcLogW
STSDKEX_API DWORD STSDKEX_GetPcLogW(LPCWSTR szEnvType, LPCWSTR szOutputValue, DWORD* pdwSize, BOOL bReload);


//----------------------------------------------------------------------------------
// AK Interface
// GetMkd8ObjectA
STSDKEX_API IAstxMkd8A* STSDKEX_GetMkd8ObjectA(LPCSTR szCustomerID, DWORD* pdwErrorCode);

// GetMkd8ObjectW
STSDKEX_API IAstxMkd8W* STSDKEX_GetMkd8ObjectW(LPCWSTR szCustomerID, DWORD* pdwErrorCode);

// GetAkObjectA
STSDKEX_API IAstxAkA* STSDKEX_GetAkObjectA(LPCSTR szCustomerID, DWORD* pdwErrorCode);

// GetAkObjectW
STSDKEX_API IAstxAkW* STSDKEX_GetAkObjectW(LPCWSTR szCustomerID, DWORD* pdwErrorCode);

//----------------------------------------------------------------------------------
// Inner Interface
STSDKEX_API DWORD StSdkEx_15();

//----------------------------------------------------------------------------------
// 

//----------------------------------------------------------------------------------
// AK Interface

// IAstxMkd8A
STSDKEX_API DWORD STSDKEX_MkdInitializeA(LPCSTR szCustomerID, int nInitMode, PFN_AKSDK_Callback fnCallback);
STSDKEX_API DWORD STSDKEX_MkdUninitializeA();
STSDKEX_API DWORD STSDKEX_MkdProtectEditControlA(HWND hWnd);
STSDKEX_API DWORD STSDKEX_MkdProtectEditControl2A(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength);
STSDKEX_API DWORD STSDKEX_MkdGetTextA(HWND hWnd, CHAR* szOutput, DWORD* pdwSize);
STSDKEX_API DWORD STSDKEX_MkdSetTextA(HWND hWnd, CONST CHAR* lpszText);
STSDKEX_API DWORD STSDKEX_MkdGetEncTextA(DWORD dwCustomCode, HWND hWnd, CHAR* szOutput, DWORD* pdwSize, BYTE bzRandom[]);

// IAstxMkd8W
STSDKEX_API DWORD STSDKEX_MkdInitializeW(LPCWSTR szCustomerID, int nInitMode, PFN_AKSDK_Callback fnCallback);
STSDKEX_API DWORD STSDKEX_MkdUninitializeW();
STSDKEX_API DWORD STSDKEX_MkdProtectEditControlW(HWND hWnd);
STSDKEX_API DWORD STSDKEX_MkdProtectEditControl2W(HWND hWnd, DWORD dwType, DWORD dwMode, DWORD dwMaxLength);
STSDKEX_API DWORD STSDKEX_MkdGetTextW(HWND hWnd, WCHAR* szOutput, DWORD* pdwSize);
STSDKEX_API DWORD STSDKEX_MkdSetTextW(HWND hWnd, CONST WCHAR* lpszText);
STSDKEX_API DWORD STSDKEX_MkdGetEncTextW(DWORD dwCustomCode, HWND hWnd, WCHAR* szOutput, DWORD* pdwSize, BYTE bzRandom[]);

// IAstxAkA
STSDKEX_API DWORD STSDKEX_AkInitializeA(LPCSTR szCustomerID, int nInitMode, PFN_AKSDK_Callback fnCallback);
STSDKEX_API DWORD STSDKEX_AkUninitializeA();
STSDKEX_API DWORD STSDKEX_AkGetEncTextA(DWORD dwCustomCode, IN CHAR* szE2EWindowID, IN CHAR* szE2EFormID, IN CHAR* szE2EInputID, OUT CHAR* szOutput, OUT DWORD* pdwSize, OUT BYTE bzRandom[]);

// IAstxAkW
STSDKEX_API DWORD STSDKEX_AkInitializeW(LPCWSTR szCustomerID, int nInitMode, PFN_AKSDK_Callback fnCallback);
STSDKEX_API DWORD STSDKEX_AkUninitializeW();
STSDKEX_API DWORD STSDKEX_AkGetEncTextW(IN DWORD dwCustomCode, IN WCHAR* szE2EWindowID, IN WCHAR* szE2EFormID, IN WCHAR* szE2EInputID, OUT WCHAR* szOutput, OUT DWORD* pdwSize, OUT BYTE bzRandom[]);




#ifdef __cplusplus
}
#endif

#endif // __STSDKEXDEF_H_
