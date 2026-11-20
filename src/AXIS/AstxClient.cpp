#include "stdafx.h"
#include "AstxClient.h"
#include "ASTx/StSdkExCfg.h"
#include "ASTx/StSdkExCom.h"
#include "ASTx/StSdkExDef.h"
#include "ASTx/StSdkExErr.h"
#include <stdio.h>

#pragma comment(lib, "ASTx/StSdkEx.lib")

static_assert(AstxClient::FUNC_PB == STSDKEX_FUNC_CODE_PB, "ASTx code mismatch");
static_assert(AstxClient::FUNC_FW == STSDKEX_FUNC_CODE_FW, "ASTx code mismatch");
static_assert(AstxClient::FUNC_PCSCAN == STSDKEX_FUNC_CODE_PCSCAN, "ASTx code mismatch");
static_assert(AstxClient::RESULT_OK == STSDKEX_ERROR_SUCCESS, "ASTx code mismatch");
static_assert(AstxClient::RESULT_NOT_INSTALLED == STSDKEX_ERROR_PRODUCT_NOT_INSTALLED, "ASTx code mismatch");
// Same values as astxManager.dll and MainFrm.h STSDKEX_CUSTOM_POLICY_URL
static const char* kPolicyUrl = "http://webclinic.ahnlab.com/astx/policy/customer_stsdk_default.html";
static const DWORD kFuncs[] = { STSDKEX_FUNC_CODE_PB, STSDKEX_FUNC_CODE_FW, STSDKEX_FUNC_CODE_PCSCAN };

// ---------------- SDK event callbacks ----------------
// Callbacks arrive on the thread that called Init (UI thread, checked 2026-11-20).
// Notices are shown on a worker thread: the SDK callback returns at once and the host UI keeps running.
// One box per kind at a time; repeated events while it is shown are only logged.
enum { kNoticeMemory, kNoticeRemoteDetect, kNoticeRemoteBlock, kNoticeCount };
static volatile LONG s_noticeBusy[kNoticeCount] = { 0 };

struct NoticeArg { int kind; char text[256]; };

static DWORD WINAPI NoticeThread(LPVOID p)
{
	NoticeArg* a = (NoticeArg*)p;
	::MessageBoxA(NULL, a->text, "HTS", MB_OK | MB_ICONWARNING | MB_TOPMOST | MB_SETFOREGROUND);
	::InterlockedExchange(&s_noticeBusy[a->kind], 0);
	delete a;
	return 0;
}

static void ShowNotice(int kind, const char* text)
{
	if (::InterlockedCompareExchange(&s_noticeBusy[kind], 1, 0) != 0)
	{
		::OutputDebugStringA("[ASTx][CB] notice already showm - skip\n");
		return;
	}

	NoticeArg* a = new NoticeArg;
	a->kind = kind;
	lstrcpynA(a->text, text, sizeof(a->text));
	HANDLE h = ::CreateThread(NULL, 0, NoticeThread, a, 0, NULL);
	if (h)
		::CloseHandle(h);
	else
	{
		::InterlockedExchange(&s_noticeBusy[kind], 0);
		delete a;
	}
}

static int __stdcall OnSdkEvent(long code, void* param, long paramSize)
{
	UNREFERENCED_PARAMETER(paramSize);
	char log[300];

	switch (code)
	{
	case STSDKEX_PB_CALLBACK_ABNORMAL_MEMORY_ACCESS:
		if (param)      // valid only inside this callback
		{
			const PST_STSDKEX_PB_PROCESS_PATH p = (PST_STSDKEX_PB_PROCESS_PATH)param;
			sprintf_s(log, "[ASTx][CB] abnormal memory access pid=%lu name=%.200s\n", p->ulPID, p->szProcessName);
			::OutputDebugStringA(log);
		}
		ShowNotice(kNoticeMemory, "메모리에 허가되지 않은 접근을 시도했습니다.");
		break;

	case STSDKEX_PB_CALLBACK_REMOTE_DETECT:
		::OutputDebugStringA("[ASTx][CB] remote detect\n");
		ShowNotice(kNoticeRemoteDetect, "원격 접근을 감지하였습니다.");
		break;

	case STSDKEX_PB_CALLBACK_REMOTE_BLOCKD:
		::OutputDebugStringA("[ASTx][CB] remote blocked\n");
		ShowNotice(kNoticeRemoteBlock, "원격 접근을 차단하였습니다.");
		break;

	default:    // START / STOP / process list changes etc.: param is an error code, log only
		sprintf_s(log, "[ASTx][CB] event code=0x%08lx param=0x%08lx\n", (DWORD)code, (DWORD)(ULONG_PTR)param);
		::OutputDebugStringA(log);
		break;
	}
	return 0;
}

static int __stdcall OnAkEvent(long code, LPARAM param)
{
	char log[128];
	sprintf_s(log, "[ASTx][CB] ak code=0x%08lx param=0x%08lx\n", (DWORD)code, (DWORD)param);
	::OutputDebugStringA(log);
	return 0;
}

bool AstxClient::Load(char* log, int logCap)
{
	if (m_tried)
		return m_h != NULL;
	m_tried = true;

	HMODULE h = ::LoadLibraryA("astxManager.dll");
	if (h == NULL)
	{
		if (log && logCap > 0)
			sprintf_s(log, logCap, "[ASTx][AM] astxManager.dll not loaded err=%lu - use built-in", ::GetLastError());
		return false;
	}

#define AM_GET(name) ((*(FARPROC*)&m_##name = ::GetProcAddress(h, "AM_" #name)) != NULL)
	const bool ok = AM_GET(IsInstalled) && AM_GET(Install) && AM_GET(Init) && AM_GET(Uninit) &&
		AM_GET(IsInitialized) && AM_GET(Start) && AM_GET(Stop) && AM_GET(IsRunning) &&
		AM_GET(AkStart) && AM_GET(AkStop) && AM_GET(AkProtectEdit) && AM_GET(ErrStr);
#undef AM_GET

	if (!ok)
	{
		::FreeLibrary(h);     // pointers are never used while m_h is NULL
		if (log && logCap > 0)
			sprintf_s(log, logCap, "[ASTx][AM] astxManager.dll export missing - use built-in");
		return false;
	}

	m_h = h;
	if (log && logCap > 0)
		sprintf_s(log, logCap, "[ASTx][AM] astxManager.dll loaded - use astxManager");
	return true;
}

// ---------------- install / init ----------------

BOOL AstxClient::IsInstalled()
{
	if (m_h)
		return m_IsInstalled();
	return STSDKEX_IsInstalled() != STSDKEX_ERROR_PRODUCT_NOT_INSTALLED;
}

DWORD AstxClient::InstalledCode()
{
	if (m_h)
		return m_IsInstalled() ? STSDKEX_ERROR_SUCCESS : STSDKEX_ERROR_PRODUCT_NOT_INSTALLED;
	return STSDKEX_IsInstalled();
}

DWORD AstxClient::Install(HWND parent)
{
	if (m_h)
		return m_Install(parent);
	if (IsInstalled())
		return STSDKEX_ERROR_SUCCESS;
	return STSDKEX_InstallA(STSDKEX_UI_MODE_SHOW, parent, "");   // empty URL: SDK default master URL
}

DWORD AstxClient::Init(const char* customerId)
{
	if (m_h)
	{
		const DWORD r = m_Init(customerId, OnSdkEvent);
		if (r == STSDKEX_ERROR_SUCCESS)
			m_customer = customerId;
		return r;
	}

	if (m_init)
		return STSDKEX_ERROR_SUCCESS;

	DWORD r;
	if ((r = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_ASYNC, "on")) != STSDKEX_ERROR_SUCCESS ||
		(r = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_FUNCTIMEOUT, "180000")) != STSDKEX_ERROR_SUCCESS ||   // 3 min
		(r = STSDKEX_SetEnvOptionA(STSDKEX_ENV_OPTION_STANDALONE, "on")) != STSDKEX_ERROR_SUCCESS)
		return r;

	STSDKEX_SetEventCallback(OnSdkEvent);

	r = STSDKEX_Initialize();
	if (r != STSDKEX_ERROR_SUCCESS)
		return r;

	m_customer = customerId;
	m_init = true;
	return STSDKEX_ERROR_SUCCESS;
}

void AstxClient::Uninit()
{
	if (m_h)
	{
		m_Uninit();
		return;
	}
	if (!m_init)
		return;

	AkStop();
	Stop(STSDKEX_FUNC_CODE_PB | STSDKEX_FUNC_CODE_FW | STSDKEX_FUNC_CODE_PCSCAN);
	STSDKEX_Unintialize();     // SDK spelling
	m_init = false;
}

BOOL AstxClient::IsInitialized()
{
	return m_h ? m_IsInitialized() : (m_init ? TRUE : FALSE);
}

// ---------------- PB / FW / PCSCAN ----------------

DWORD AstxClient::Start(DWORD funcMask)
{
	if (m_h)
		return m_Start(funcMask);
	if (!m_init)
		return STSDKEX_ERROR_NOT_INITIALIZED;

	DWORD first = STSDKEX_ERROR_SUCCESS;
	for (int i = 0; i < _countof(kFuncs); i++)
	{
		const DWORD f = kFuncs[i];
		if ((funcMask & f) == 0)
			continue;

		if (f == STSDKEX_FUNC_CODE_PB)
			STSDKEX_PBSetActivateSubFunc(STSDKEX_PB_ACTIVATE_PROTECT_MEMORY | STSDKEX_PB_ACTIVATE_BLOCK_REMOTE);

		DWORD r = STSDKEX_StartFuncA(m_customer.c_str(), f, kPolicyUrl);
		if (r == STSDKEX_ERROR_FUNCTION_ALREADY_STARTED || r == STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE)
			r = STSDKEX_ERROR_SUCCESS;
		if (r != STSDKEX_ERROR_SUCCESS && first == STSDKEX_ERROR_SUCCESS)
			first = r;
	}
	return first;
}

DWORD AstxClient::Stop(DWORD funcMask)
{
	if (m_h)
		return m_Stop(funcMask);
	if (!m_init)
		return STSDKEX_ERROR_NOT_INITIALIZED;

	DWORD first = STSDKEX_ERROR_SUCCESS;
	for (int i = 0; i < _countof(kFuncs); i++)
	{
		const DWORD f = kFuncs[i];
		if ((funcMask & f) == 0)
			continue;

		DWORD r = STSDKEX_StopFuncA(m_customer.c_str(), f);
		if (r == STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED)
			r = STSDKEX_ERROR_SUCCESS;
		if (r != STSDKEX_ERROR_SUCCESS && first == STSDKEX_ERROR_SUCCESS)
			first = r;
	}
	return first;
}

// Force-close gate in the caller: before Init keep the exact direct SDK query.
DWORD AstxClient::IsRunning(DWORD funcCode, const char* customerId)
{
	if (m_h && m_IsInitialized())
		return m_IsRunning(funcCode);
	const char* id = (customerId != NULL) ? customerId : m_customer.c_str();
	return STSDKEX_IsRunningFuncA(id, funcCode);
}

// ---------------- keyboard protect (AK) ----------------

DWORD AstxClient::AkStart()
{
	if (m_h)
		return m_AkStart(OnAkEvent);
	if (!m_init)
		return STSDKEX_ERROR_NOT_INITIALIZED;
	if (m_ak)
		return STSDKEX_ERROR_SUCCESS;

	DWORD err = STSDKEX_ERROR_SUCCESS;
	IAstxMkd8A* ak = STSDKEX_GetMkd8ObjectA(m_customer.c_str(), &err);
	if (ak == NULL)
		return (err != STSDKEX_ERROR_SUCCESS) ? err : STSDKEX_ERROR_FUNCTION_FAILED;

	err = ak->Initialize(AstxmodeProtectInAstx, OnAkEvent);
	if (err != STSDKEX_ERROR_SUCCESS)
		return err;

	m_ak = ak;
	return STSDKEX_ERROR_SUCCESS;
}

void AstxClient::AkStop()
{
	if (m_h)
	{
		m_AkStop();
		return;
	}
	if (m_ak == NULL)
		return;
	m_ak->Uninitialize();
	m_ak = NULL;          // drop even on failure so restart is possible
}

DWORD AstxClient::AkProtectEdit(HWND edit)
{
	if (m_h)
		return m_AkProtectEdit(edit);
	return m_ak ? m_ak->ProtectEditControl(edit) : STSDKEX_ERROR_NOT_INITIALIZED;
}

const char* AstxClient::ErrStr(DWORD code)
{
	switch (code)
	{
	case STSDKEX_ERROR_SUCCESS:                               return "성공";
	case STSDKEX_ERROR_UNINITIALIZE_FAILED:                   return "UNINITIALIZE에 실패하였습니다.";
	case STSDKEX_ERROR_LOADLIBRARY_FAILED:                    return "SDK 모듈을 로드하는데 실패하였습니다.";
	case STSDKEX_ERROR_GETPROCADDRESS_FAILED:                 return "SDK 모듈에서 인터페이스를 찾을 수 없습니다.";
	case STSDKEX_ERROR_FUNCTION_START_FAILED:                 return "기능을 시작시키는데 실패하였습니다.";
	case STSDKEX_ERROR_FUNCTION_STOP_FAILED:                  return "기능을 종료시키는데 실패하였습니다.";
	case STSDKEX_ERROR_ALREADY_INSTALLED:                     return "제품이 이미 설치되어 있습니다.";
	case STSDKEX_ERROR_NOT_INITIALIZED:                       return "초기화 되지 않았습니다.";
	case STSDKEX_ERROR_BAD_PARAMETER:                         return "파라미터가 이상합니다.";
	case STSDKEX_ERROR_INITIALIZE_FAILED:                     return "초기화에 실패하였습니다.";
	case STSDKEX_ERROR_NOT_LOADLIBRARY:                       return "SDK 모듈이 로드되어 있지 않습니다.";
	case STSDKEX_ERROR_REMAIN_SHAREDCOUNT:                    return "기능을 다른 곳에서 사용중이므로 종료할 수가 없습니다.";
	case STSDKEX_ERROR_FUNCTION_ALREADY_STOPPED:              return "기능이 이미 종료된 상태입니다.";
	case STSDKEX_ERROR_INVALID_CUSTOMER_ID:                   return "고객사 ID가 잘못 되었습니다.";
	case STSDKEX_ERROR_INVALID_FUNCTION_CODE:                 return "기능 코드가 잘못 되었습니다.";
	case STSDKEX_ERROR_PRODUCT_NOT_INSTALLED:                 return "제품이 설치되지 않았습니다.";
	case STSDKEX_ERROR_DRIVER_RULE_NOT_VERIFYED:              return "드라이버 룰이 유효하지 않습니다.";
	case STSDKEX_ERROR_NOT_PROTECTED_HWND:                    return "보호하지 않는 Edit Control입니다.";
	case STSDKEX_ERROR_NOT_PROPERTY_HWND:                     return "속성이 유효하지 않습니다.";
	case STSDKEX_ERROR_INVALID_PROCESS_ID:                    return "프로세스 ID가 잘못 지정되었습니다.";
	case STSDKEX_ERROR_DOWNLOAD_SETTING_FAILED:               return "다운로드할 파일셋을 구성하는데 실패하였습니다.";
	case STSDKEX_ERROR_DOWNLOAD_UPDATESETUP:                  return "업데이트 프로그램 셋업을 받아올수 없습니다.";
	case STSDKEX_ERROR_CHECK_UPDATEMODULE:                    return "업데이트 프로그램 모듈 검증이 실패하였습니다.";
	case STSDKEX_ERROR_SIGN_NOT_VERIFYED:                     return "서명이 올바르게 되어 있지 않습니다.";
	case STSDKEX_ERROR_FUNCTION_FAILED:                       return "함수를 수행하는데 실패하였습니다.";
	case STSDKEX_ERROR_INVALID_SUB_FUNCTION_CODE:             return "해킹보호 기능의 세부기능 코드가 잘못 되었습니다.";
	case STSDKEX_ERROR_FUNCTION_NOT_RUNNING:                  return "기능이 동작중이지 않습니다.";
	case STSDKEX_ERROR_FUNCTION_ALREADY_STARTED:              return "기능이 이미 동작중인 상태입니다.";
	case STSDKEX_ERROR_CUSTOMER_POLICY_DOWNLOAD_FAILED:       return "고객사 정책파일(기능)을 다운로드 하는데 실패하였습니다.";
	case STSDKEX_ERROR_OPTION_POLICY_DOWNLOAD_FAILED:         return "고객사 정책파일(옵션)을 다운로드 하는데 실패하였습니다.";
	case STSDKEX_ERROR_INVALID_EXCEPTION_PROCESS_LIST:        return "예외프로세스 리스트가 잘못 지정되었습니다.";
	case STSDKEX_ERROR_NOT_ABLE_BECAUSE_PRIORITY_RULE:        return "우선순위(충돌 프로그램 존재, Super룰)에 따라 상위 룰이 존재하여 요청을 수행할 수 없습니다.";
	case STSDKEX_ERROR_CONNECT_FAILED:                        return "기능 연결에 실패하였습니다.";
	case STSDKEX_ERROR_FUNCTION_CALL_FAILED:                  return "시스템 함수 호출에 실패하였습니다.";
	case STSDKEX_ERROR_STSESS_EXEC_FAILED:                    return "STSESS 실행에 실패하였습니다.";
	case STSDKEX_ERROR_STSESS_TIMEOUT_FAILED:                 return "STSESS 실행이 Timeout 되었습니다.";
	case STSDKEX_ERROR_ASDSVC_NOT_RUNNING:                    return "AsdSvc 가 실행중이지 않습니다.";
	case STSDKEX_ERROR_ASTX_UPDATING:                         return "ASTx 업데이트 중입니다.";
	case STSDKEX_ERROR_NULL_CUSTOMER_ID:                      return "CUSTOMER ID가 NULL입니다.";
	case STSDKEX_ERROR_GET_ACTIVE_CONSOLE_SESSION_ID:         return "ActiveConsoleSessionId를 얻지 못했습니다.";
	case STSDKEX_ERROR_CALL_AFTER_INIT:                       return "STSDKEX_Initialize()보다 먼저 호출되어야 합니다.";
	case STSDKEX_ERROR_CALL_NOT_IMPLEMENTED:                  return "지원하지 않는 함수입니다.";
	case STSDKEX_ERROR_PB_NULL_CALLBACK:                      return "콜백함수가 NULL입니다.";
	case STSDKEX_ERROR_PB_NULL_HSBCTL:                        return "HSBCTL이 NULL입니다.";
	case STSDKEX_ERROR_PB_COMPATIBLITY_MODE:                  return "호환성 모드가 설정되어 있습니다.";
	case STSDKEX_ERROR_PB_CREATEOPTIONS_FAILED:               return "CreateOptions이 실패하였습니다.";
	case STSDKEX_ERROR_PB_COPYDRIVER_FAILED:                  return "드라이버 복사에 실패하였습니다.";
	case STSDKEX_ERROR_PB_HSBDRV_INITIALIZE_FAILED:           return "드라이버 초기화에 실패하였습니다.";
	case STSDKEX_ERROR_PB_HSBDRV_START_FAILED:                return "드라이버 구동에 실패하였습니다.";
	case STSDKEX_ERROR_PB_HSBDRV_INSERTEXCEPTIONPROCESS_FAILED: return "드라이버 예외프로세스 설정에 실패하였습니다.";
	case STSDKEX_ERROR_PB_HSBDRV_GETDRIVERVERSION_FAILED:     return "드라이버 버전확인에 실패하였습니다.";
	case STSDKEX_ERROR_PB_HSBDRV_LOADDRIVER_FAILED:           return "드라이버 로드에 실패하였습니다.";
	default:                                                  return "";
	}
}