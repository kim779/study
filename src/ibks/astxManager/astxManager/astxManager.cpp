#include "pch.h"
#include "astxManager.h"
#include "ASTx/StSdkExCfg.h"
#include "ASTx/StSdkExCom.h"
#include "ASTx/StSdkExDef.h"
#include "ASTx/StSdkExErr.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>

#pragma comment(lib, "ASTx/StSdkEx.lib")

// Same value as MainFrm.h STSDKEX_CUSTOM_POLICY_URL
static const char* AM_POLICY_URL = "http://webclinic.ahnlab.com/astx/policy/customer_stsdk_default.html";

static const DWORD kFuncs[] = { AM_FUNC_PB, AM_FUNC_FW, AM_FUNC_PCSCAN };

static BOOL        s_init = FALSE;
static std::string s_customer;
static IAstxMkd8A* s_ak = NULL;
static AM_AK_CB    s_akCb = NULL;

static void amLog(const char* fmt, ...)
{
	char buf[512] = { 0 };
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buf, fmt, ap);
	va_end(ap);
	OutputDebugStringA(buf);
}

AM_API BOOL AM_IsInstalled(void)
{
	return STSDKEX_IsInstalled() != STSDKEX_ERROR_PRODUCT_NOT_INSTALLED;
}

AM_API DWORD AM_Install(HWND parent)
{
	if (AM_IsInstalled())
		return AM_OK;

	// empty URL: SDK uses its own default master URL
	const DWORD r = STSDKEX_InstallA(STSDKEX_UI_MODE_SHOW, parent, "");
	amLog("[AM][DLL] install result=0x%08lx", r);
	return r;
}

AM_API DWORD AM_Init(const char* customerId, AM_EVENT_CB cb)
{
	if (s_init)
		return AM_OK;
	if (customerId == NULL || customerId[0] == '\0')
		return STSDKEX_ERROR_NULL_CUSTOMER_ID;
	if (!AM_IsInstalled())
		return AM_ERR_NOT_INSTALLED;

	DWORD r;
	if ((r = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_ASYNC, "on")) != AM_OK ||
		(r = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_FUNCTIMEOUT, "180000")) != AM_OK ||
		(r = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_STANDALONE, "on")) != AM_OK)
	{
		amLog("[AM][DLL] init env option FAIL result=0x%08lx", r);
		return r;
	}

	if (cb)
		STSDKEX_SetEventCallback(cb);

	r = STSDKEX_Initialize();
	if (r != AM_OK)
	{
		amLog("[AM][DLL] init FAIL result=0x%08lx", r);
		return r;
	}

	s_customer = customerId;
	s_init = TRUE;
	amLog("[AM][DLL] init Ok");
	return AM_OK;
}

AM_API void AM_Uninit(void)
{
	if (!s_init)
		return;

	AM_AkStop();
	AM_Stop(AM_FUNC_ALL);
	STSDKEX_Unintialize();  

	s_init = FALSE;
	amLog("[AM][DLL] uninit");
}

AM_API BOOL AM_IsInitialized(void)
{
	return s_init;
}

// ---------------- PB / FW / PCSCAN ----------------
// Fix: original startPB/startFW returned TRUE even on failure. Returns first failure code.
AM_API DWORD AM_Start(DWORD funcMask)
{
	if (!s_init)
		return AM_ERR_NOT_INITIALIZED;

	DWORD first = AM_OK;
	for (int i = 0; i < _countof(kFuncs); i++)
	{
		const DWORD f = kFuncs[i];
		if ((funcMask & f) == 0)
			continue;

		if (f == AM_FUNC_PB)
			STSDKEX_PBSetActivateSubFunc(STSDKEX_PB_ACTIVATE_PROTECT_MEMORY | STSDKEX_PB_ACTIVATE_BLOCK_REMOTE);
		
		DWORD r = STSDKEX_StartFuncA(s_customer.c_str(), f, AM_POLICY_URL);
		if (r == STSDKEX_ERROR_FUNCTION_ALREADY_STARTED || r == STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE)
			r = AM_OK;

		amLog("[AM][DLL] start func=0x%08lx result=0x%08lx", f, r);
		if (r != AM_OK && first == AM_OK)
			first = r;
	}
	return first;
}

AM_API DWORD AM_Stop(DWORD funcMask)
{
	if (!s_init)
		return AM_ERR_NOT_INITIALIZED;

	DWORD first = AM_OK;
	for (int i = 0; i < _countof(kFuncs); i++)
	{
		const DWORD f = kFuncs[i];
		if ((funcMask & f) == 0)
			continue;

		DWORD r = STSDKEX_StopFuncA(s_customer.c_str(), f);
		if (r == STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED)
			r = AM_OK;

		amLog("[AM][DLL] stop func=0x%08lx result=0x%08lx", f, r);
		if (r != AM_OK && first == AM_OK)
			first = r;
	}
	return first;
}

AM_API DWORD AM_IsRunning(DWORD funcCode)
{
	if (s_customer.empty())
		return AM_ERR_NOT_INITIALIZED;
	return STSDKEX_IsRunningFuncA(s_customer.c_str(), funcCode);
}

// --- keyboard protect (AK)
//FIX original AKSDK_Callback had a missing break (BAD_PARAMETER fell through)
static int __stdcall amAkCallback(long code, LPARAM param)
{
	amLog("[AM][DLL] ak event code=0x%08lx param=0x%08lx", (DWORD)code, (DWORD)param);
	return s_akCb ? s_akCb(code, param) : 0;
}

AM_API DWORD AM_AkStart(AM_AK_CB cb)
{
	if (!s_init)
		return AM_ERR_NOT_INITIALIZED;
	if (s_ak)
		return AM_OK;

	DWORD err = AM_OK;
	IAstxMkd8A* ak = STSDKEX_GetMkd8ObjectA(s_customer.c_str(), &err);
	if (ak == NULL)
	{
		amLog("[AM][DLL] ak get object FAIL result=0x%08lx", err);
		return (err != AM_OK) ? err : STSDKEX_ERROR_FUNCTION_FAILED;
	}

	s_akCb = cb;
	err = ak->Initialize(AstxmodeProtectInAstx, amAkCallback);
	if (err != AM_OK)
	{
		s_akCb = NULL;
		amLog("[AM][DLL] ak init FAIL result=0x%08lx", err);
		return err;
	}

	s_ak = ak;
	amLog("[AM][DLL] ak start OK");
	return AM_OK;
}

AM_API void AM_AkStop(void)
{
	if (s_ak == NULL)
		return;

	const DWORD r = s_ak->Uninitialize();
	amLog("[AM][DLL] ak stop result=0x%08lx", r);
	s_ak = NULL;      // drop even on failure so restart is possible
	s_akCb = NULL;
}

AM_API DWORD AM_AkProtectEdit(HWND edit)
{
	if (s_ak == NULL)
		return AM_ERR_NOT_INITIALIZED;
	const DWORD r = s_ak->ProtectEditControl(edit);
	amLog("[AM][DLL] ak protect hwnd=0x%p result=0x%08lx", edit, r);
	return r;
}

AM_API DWORD AM_AkGetText(HWND edit, char* out, DWORD* size)
{
	if (s_ak == NULL)
		return AM_ERR_NOT_INITIALIZED;
	return s_ak->GetText(edit, out, size);       // do not log out
}

AM_API DWORD AM_AkGetEncText(DWORD customCode, HWND edit, char* out, DWORD* size, BYTE random[])
{
	if (s_ak == NULL)
		return AM_ERR_NOT_INITIALIZED;
	return s_ak->GetEncText(customCode, edit, out, size, random);
}

// ---------------- util ----------------

AM_API const char* AM_ErrStr(DWORD code)
{
	switch (code)
	{
	case STSDKEX_ERROR_SUCCESS:                        return "성공";
	case STSDKEX_ERROR_UNINITIALIZE_FAILED:            return "SDK 종료에 실패했습니다.";
	case STSDKEX_ERROR_LOADLIBRARY_FAILED:             return "SDK 모듈을 불러오지 못했습니다.";
	case STSDKEX_ERROR_GETPROCADDRESS_FAILED:          return "SDK 함수를 찾지 못했습니다.";
	case STSDKEX_ERROR_FUNCTION_START_FAILED:          return "보안 기능 시작에 실패했습니다.";
	case STSDKEX_ERROR_FUNCTION_STOP_FAILED:           return "보안 기능 종료에 실패했습니다.";
	case STSDKEX_ERROR_NOT_INITIALIZED:                return "SDK가 초기화되지 않았습니다.";
	case STSDKEX_ERROR_BAD_PARAMETER:                  return "잘못된 인자입니다.";
	case STSDKEX_ERROR_INITIALIZE_FAILED:              return "SDK 초기화에 실패했습니다.";
	case STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED:       return "보안 기능이 이미 종료되어 있습니다.";
	case STSDKEX_ERROR_INVALID_CUSTOMER_ID:            return "고객사 ID가 올바르지 않습니다.";
	case STSDKEX_ERROR_INVALID_FUNCTION_CODE:          return "기능 코드가 올바르지 않습니다.";
	case STSDKEX_ERROR_PRODUCT_NOT_INSTALLED:          return "ASTx가 설치되어 있지 않습니다.";
	case STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED:       return "ASTx 키보드보안이 호환되지 않습니다. ASTx를 사용하는 프로그램을 모두 종료 후 다시 실행해 주십시오.";
	case STSDKEX_ERROR_NOT_PROTECTED_HWND:             return "보호되지 않은 입력창입니다.";
	case STSDKEX_ERROR_SIGN_NOT_VERIFYED:              return "ASTx 모듈 서명 확인에 실패했습니다.";
	case STSDKEX_ERROR_FUNCTION_FAILED:                return "보안 기능 호출에 실패했습니다.";
	case STSDKEX_ERROR_FUNCTION_NOT_RUNNING:           return "보안 기능이 동작 중이 아닙니다.";
	case STSDKEX_ERROR_FUNCTION_ALREADY_STARTED:       return "보안 기능이 이미 동작 중입니다.";
	case STSDKEX_ERROR_CUSTOMER_POLICY_DOWNLOAD_FAILED: return "고객사 정책을 내려받지 못했습니다.";
	case STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE: return "다른 프로그램의 ASTx 설정이 우선 적용 중입니다.";
	case STSDKEX_ERROR_CONNECT_FAILED:                 return "ASTx 서비스 연결에 실패했습니다.";
	case STSDKEX_ERROR_ASDSVC_NOT_RUNNING:             return "ASTx 서비스가 실행 중이 아닙니다.";
	case STSDKEX_ERROR_ASTX_UPDATING:                  return "ASTx 업데이트 중입니다.";
	case STSDKEX_ERROR_NULL_CUSTOMER_ID:               return "고객사 ID가 없습니다.";
	case STSDKEX_ERROR_UPDATE_TIMEOUT:                 return "ASTx 업데이트 시간이 초과되었습니다.";
	default:                                           return "알 수 없는 오류입니다.";
	}
}