#include "pch.h"
#include "certsign.h"
#include <stdio.h>
#include <stdarg.h>

#include "include/InterfaceTypes.h"
#include "include/InterfaceDLL.h"

#pragma comment (lib, "lib/SKComdIF.lib")
#pragma comment (lib, "lib/SKCommIF.lib")

#define cslog(fmt, ...) cslogImpl(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)

static void cslogImpl(const char* func, int line, const char* fmt, ...)
{
	char msg[1024];
	va_list args;
	va_start(args, fmt);
	_vsnprintf_s(msg, sizeof(msg), _TRUNCATE, fmt, args);
	va_end(args);

	char out[1200];
	_snprintf_s(out, sizeof(out), _TRUNCATE, "[CERTSIGN][%s:%d] %s\n", func, line, msg);
	OutputDebugStringA(out);
}

static unsigned long csFnv(const unsigned char* p, int n)
{
	unsigned long h = 2166136261u;
	for (int i = 0; i < n; i++) { h ^= p[i]; h *= 16777619u; }
	return h;
}

struct CS_SESSION
{
	APP_CONTEXT appCtx;
	SD_API_CONTEXT sd;
	char		encpass[32 + 1];	// sk_if_GetEncryptedPassword 결과 (CertifyCtrl::m_encpass와 동일)
	bool		selected;
};

// 평문 비밀번호를 33바이트 암호화 값으로 보관하고 평문은 지운다 (CertifyCtrl::savePasswd 패턴)
static void savePasswd(CS_SESSION* s, char* plain, int plainCap)
{
	//cslog("[certsign.dll] savePasswd");
	cslog("[certsign.dll] savePasswd [%s]", plain);
	ZeroMemory(s->encpass, sizeof(s->encpass));
	sk_if_GetEncryptedPassword(plain, s->encpass);
	SecureZeroMemory(plain, plainCap);
}

CS_API HCS CS_Open(void)
{
	//cslog("CS_SelectInteractive rc=%d storage=%d", rc, (int)s->sd.bOldStorage)
	cslog("[certsign.dll] CS_Open");
	CS_SESSION* s = new CS_SESSION();
	ZeroMemory(&s->appCtx, sizeof(APP_CONTEXT));
	ZeroMemory(&s->sd, sizeof(SD_API_CONTEXT));
	ZeroMemory(s->encpass, sizeof(s->encpass));
	s->selected = false;

	if (sk_if_cert_InitContextApp(&s->appCtx, nullptr, 0) == -1)
	{
		delete s;
		return nullptr;
	}
	return (HCS)s;
}

CS_API int CS_Select(HCS h, const char* dn, const char* password,
	unsigned char storage, const char* certSetName)
{
	if (!h || !dn || !password)
		return -1;

	cslog("[certsign.dll] CS_Select");
	CS_SESSION* s = (CS_SESSION*)h;

	ZeroMemory(&s->sd, sizeof(SD_API_CONTEXT));
	s->sd.bOldStorage = storage;                 // queryDn: 0x00 (전체 매체)
	s->sd.bForceAllCaSearchMode = 1;             // queryDn과 동일: 모든 CA 검색
	strncpy_s(s->sd.szUserId, sizeof(s->sd.szUserId), dn, _TRUNCATE);
	if (certSetName && *certSetName)
		strncpy_s(s->sd.szCertSetName, sizeof(s->sd.szCertSetName), certSetName, _TRUNCATE);

	// CertifyId: SetPasswordEncMode(1) + GetEncryptedPassword → queryDn: 암호화 값을 szOldPasswd에
	ZeroMemory(s->encpass, sizeof(s->encpass));
	sk_if_SetPasswordEncMode(1);
	sk_if_GetEncryptedPassword(password, s->encpass);
	CopyMemory(s->sd.szOldPasswd, s->encpass, min(sizeof(s->encpass), sizeof(s->sd.szOldPasswd)));

	sk_if_SetWrongPasswordLimit(1);
	sk_if_SetKeySaferMode(1);

	int rc = 0;
	if (!sk_if_CertSetSelect(&s->sd))
		rc = sk_if_GetLastErrorCode();               // 2417=비번오류, 2500=인증서 없음, 2001=만료
	else if (sk_if_cert_preset_context(&s->appCtx, &s->sd) < 0)
		rc = sk_if_GetLastErrorCode();

	cslog("CS_Select rc=%d storage=%d", rc, (int)storage);   // DN·비번은 찍지 않음
	if (rc != 0)
	{
		SecureZeroMemory(s->encpass, sizeof(s->encpass));
		return rc;
	}
	s->selected = true;                              // 서명은 CS_Sign → SignData_notEncode(encpass)
	return 0;

}

CS_API  int CS_Sign(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen)
{
	if (!h || !sigOut || !sigOutLen)
		return -1;

	cslog("[certsign.dll] CS_Sign");
	//cslog("CS_Sign len=[%d] data=[%s] ", dataLen, data);
	cslog("CS_Sign len=%d fnv=%08X", dataLen, csFnv(data, dataLen));

	CS_SESSION* s = (CS_SESSION*)h;
	if (!s->selected)
		return -1;

	UString in = { dataLen, (unsigned char*)data };
	UString out = { 0, nullptr };
	// 암호화 비밀번호로 서명 (CertifyCtrl::Certify의 자동서명 경로와 동일, 팝업 없음)
	if (sk_if_cert_SignData_notEncode(&s->appCtx, s->encpass, &in, &out, NULL))
		return sk_if_GetLastErrorCode();

	if (out.length > *sigOutLen)
	{
		sk_if_cert_MemFree(out.value);
		return -2;
	}

	CopyMemory(sigOut, out.value, out.length);
	*sigOutLen = out.length;
	sk_if_cert_MemFree(out.value);
	return 0;
}

CS_API void CS_Close(HCS h)
{
	if (!h)
		return;

	cslog("[certsign.dll] CS_Close");
	CS_SESSION* s = (CS_SESSION*)h;
	sk_if_cert_static_context_release();
	SecureZeroMemory(s, sizeof(CS_SESSION));
	delete s;
}

CS_API const char* CS_GetLastError(void)
{
	return sk_if_GetLastErrorMsg();
}

CS_API int CS_SelectInteractive(HCS h, HWND parentWnd,
	char* dnOut, int dnOutCap,
	unsigned char* storageOut,
	char* certSetNameOut, int certSetNameOutCap)
{
	if (!h)
		return -1;

	cslog("[certsign.dll] CS_SelectInteractive");
	CS_SESSION* s = (CS_SESSION*)h;

	//cslog("[cersign]CS_SelectInteractive  data=[%s] ", dnOut);
	cslog("[cersign]CS_SelectInteractive  storage=%d",  (int)s->sd.bOldStorage);


	SD_API_CONTEXT_NEW ctxNew;
	ZeroMemory(&ctxNew, sizeof(SD_API_CONTEXT_NEW));

	sk_if_SetWrongPasswordLimit(1);
	sk_if_SetKeySaferMode(1);

	if (parentWnd && IsWindow(parentWnd))
		sk_if_DialogModalMode(parentWnd);

	if (!sk_if_CertSetSelectExt(&ctxNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA))
		return sk_if_GetLastErrorCode();

	// preset_context는 넘긴 SD_API_CONTEXT의 "주소"를 appCtx.pInterfaceContext에 보관하고
	// 서명 시 거기서 비밀번호(szOldPasswd)를 읽는다. 지역변수(ctxNew.sd)를 넘기면 함수 반환 후
	// 댕글링 포인터가 되어 다음 CS_Sign이 2417(비밀번호 오류)로 실패한다 → 세션 구조체에 먼저 복사.
	CopyMemory(&s->sd, &ctxNew.sd, sizeof(SD_API_CONTEXT));
	SecureZeroMemory(ctxNew.sd.szOldPasswd, sizeof(ctxNew.sd.szOldPasswd));

	if (sk_if_cert_preset_context(&s->appCtx, &s->sd) < 0)
		return sk_if_GetLastErrorCode();

	savePasswd(s, s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));

	if (dnOut)
		strncpy_s(dnOut, dnOutCap, s->sd.szDN, _TRUNCATE);
	if (storageOut)
		*storageOut = s->sd.bOldStorage;
	if (certSetNameOut)
		strncpy_s(certSetNameOut, certSetNameOutCap, s->sd.szCertSetName, _TRUNCATE);

	s->selected = true;

	return 0;
}

CS_API int CS_SignFull(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen)
{
	if (!h || !sigOut || !sigOutLen)
		return -1;

	cslog("[certsign.dll] CS_SignFull len=[%d]  data=[%s]", dataLen, data);

	CS_SESSION* s = (CS_SESSION*)h;
	if (!s->selected)
		return -1;

	UString in = { dataLen, (unsigned char*)data };
	UString out = { 0 , nullptr };
	//로그인용 - notEncode 아님. 벤터 CertifyFull/sign() 과 동일한 함수
	if (sk_if_cert_SignData(&s->appCtx, s->encpass, &in, &out))
		return sk_if_GetLastErrorCode();

	if (out.length > *sigOutLen)
	{
		sk_if_cert_MemFree(out.value);
		return  -2;
	}

	CopyMemory(sigOut, out.value, out.length);
	*sigOutLen = out.length;
	sk_if_cert_MemFree(out.value);

	return 0;
}

//certfyctrl::checkPasswd() 동일 : 컨텍스트 비번을 비우고 빈 원문으로 서명 
//벤더가 작은 비밀번호 창을 띄워 입력값을 sd.szOldPasswd에 넣음 -> 암호화해서 encpass 교체
CS_API int CS_VerifyPassword(HCS h, HWND parentWnd)
{
	if (!h)
		return -1;

	cslog("[certsign.dll] CS_VerifyPassword");
	CS_SESSION* s = (CS_SESSION*)h;
	if (!s->selected)
		return -1;
	SecureZeroMemory(s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));   // s->sd == appCtx.pInterfaceContext
	sk_if_SetKeySaferMode(1);
	if (parentWnd)
		sk_if_DialogModalMode(parentWnd);

	char empty[32 + 1] = { 0 };
	UString in = { 0, nullptr };
	UString out = { 0, nullptr };
	int rc = 0;
	if (sk_if_cert_SignData_notEncode(&s->appCtx, empty, &in, &out, NULL))
		rc = sk_if_GetLastErrorCode();
	else
	{
		sk_if_cert_MemFree(out.value);
		savePasswd(s, s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));   // encpass 교체 + 평문 삭제
	}
	SecureZeroMemory(s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));
	cslog("CS_VerifyPassword rc=%d", rc);
	return rc;
}

//벤더 CertifyFull caRUN 분기와 동일 : szOldPasswd 를 비우고 빈 비번으로 SignData->
//벤터가 작은 비밀번호 창을 띄우고 입력값을 sd.szOldPasswd 에 넣음 -> 성공시 savePasswd
CS_API int CS_SignFullPrompt(HCS h, HWND parentWnd, const unsigned char* data, int dataLen,
	unsigned char* sigOut, int* sigOutLen)
{
	if (!h || !sigOut || !sigOutLen)
		return -1;
	CS_SESSION* s = (CS_SESSION*)h;
	if (!s->selected)
		return -1;

	cslog("[certsign.dll] CS_SignFullPrompt");
	SecureZeroMemory(s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));   // s->sd == appCtx.pInterfaceContext
	sk_if_SetKeySaferMode(1);
	if (parentWnd)
		sk_if_DialogModalMode(parentWnd);

	char empty[32 + 1] = { 0 };
	UString in = { dataLen, (unsigned char*)data };
	UString out = { 0, nullptr };
	int rc = 0;
	if (sk_if_cert_SignData(&s->appCtx, empty, &in, &out))
		rc = sk_if_GetLastErrorCode();
	else if (out.length > *sigOutLen)
	{
		sk_if_cert_MemFree(out.value);
		rc = -2;
	}
	else
	{
		CopyMemory(sigOut, out.value, out.length);
		*sigOutLen = out.length;
		sk_if_cert_MemFree(out.value);
		savePasswd(s, s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));   // encpass 교체 + 평문 삭제
	}
	SecureZeroMemory(s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));
	cslog("[certsign.dll] CS_SignFullPrompt rc=%d len=%d sigLen=%d", rc, dataLen, rc == 0 ? *sigOutLen : 0);
	return rc;
}

CS_API int CS_CloudSelectInteractive(HCS h, HWND parentWnd,
	char* dnOut, int dnOutCap,
	char* certSetNameOut, int certSetNameOutCap)
{
	if (!h)
		return -1;

	CS_SESSION* s = (CS_SESSION*)h;

	cslog("[certsign.dll] CS_CloudSelectInteractive");

	SD_API_CONTEXT_NEW ctxNew;
	ZeroMemory(&ctxNew, sizeof(SD_API_CONTEXT_NEW));

	if (parentWnd && IsWindow(parentWnd))
		sk_if_DialogModalMode(parentWnd);

	int rc = sk_if_CloudCertSetSelectExt(&ctxNew, 0);   // 0 = 벤더 선택창(폰 PIN/푸시확인)
	if (rc != 0)
		return sk_if_GetLastErrorCode();

	// CS_SelectInteractive와 같은 이유로, 지역변수(ctxNew.sd)가 아니라 세션 구조체에 먼저 복사
	// (안 그러면 댕글링 포인터로 다음 CS_CloudSignFull이 실패함)
	CopyMemory(&s->sd, &ctxNew.sd, sizeof(SD_API_CONTEXT));

	if (sk_if_cert_preset_context(&s->appCtx, &s->sd) < 0)
		return sk_if_GetLastErrorCode();

	// 클라우드는 로컬비번(encpass) 불필요 - 인증은 선택 단계(벤더 팝업)에서 이미 끝남
	SecureZeroMemory(s->encpass, sizeof(s->encpass));

	if (dnOut)
		strncpy_s(dnOut, dnOutCap, s->sd.szDN, _TRUNCATE);
	if (certSetNameOut)
		strncpy_s(certSetNameOut, certSetNameOutCap, s->sd.szCertSetName, _TRUNCATE);

	s->selected = true;
	return 0;
}

CS_API int CS_CloudSignFull(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen)
{
	if (!h || !sigOut || !sigOutLen)
		return -1;

	cslog("[certsign.dll] CS_CloudSignFull len=%d fnv=%08X", dataLen, csFnv(data, dataLen));

	CS_SESSION* s = (CS_SESSION*)h;
	if (!s->selected)
		return -1;

	UString in = { dataLen, (unsigned char*)data };
	UString out = { 0, nullptr };
	UString r = { 0, nullptr };                 // 벤더도 안 씀 (CertifyCtrl.cpp:1132-1134)
	if (sk_if_cert_SignDataWithR(&s->appCtx, "", &in, &out, &r))
		return sk_if_GetLastErrorCode();

	if (r.value)
		sk_if_cert_MemFree(r.value);            // 벤더는 이거 누락(누수) - 여긴 정리

	if (out.length > *sigOutLen)
	{
		sk_if_cert_MemFree(out.value);
		return -2;
	}

	CopyMemory(sigOut, out.value, out.length);
	*sigOutLen = out.length;
	sk_if_cert_MemFree(out.value);
	return 0;
}