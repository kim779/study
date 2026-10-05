#include "pch.h"
#include "certsign.h"

#include "include/InterfaceTypes.h"
#include "include/InterfaceDLL.h"

#pragma comment (lib, "lib/SKComdIF.lib")
#pragma comment (lib, "lib/SKCommIF.lib")

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
	ZeroMemory(s->encpass, sizeof(s->encpass));
	sk_if_GetEncryptedPassword(plain, s->encpass);
	SecureZeroMemory(plain, plainCap);
}

CS_API HCS CS_Open(void)
{
	//cslog("CS_SelectInteractive rc=%d storage=%d", rc, (int)s->sd.bOldStorage)
	cslog("[cersign]CS_Open");
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

	CS_SESSION* s = (CS_SESSION*)h;

	cslog("[cersign]CS_Select dn=[%s] certSetName=[%s] ", dn, certSetName);

	ZeroMemory(&s->sd, sizeof(SD_API_CONTEXT));
	s->sd.bOldStorage = storage;                                   // 0x00 -> 실제 값(3)
	strncpy_s(s->sd.szUserId, sizeof(s->sd.szUserId), dn, _TRUNCATE);
	strncpy_s(s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd), password, _TRUNCATE);
	if (certSetName)
		strncpy_s(s->sd.szCertSetName, sizeof(s->sd.szCertSetName), certSetName, _TRUNCATE);  // 추가

	sk_if_SetWrongPasswordLimit(1);
	sk_if_SetKeySaferMode(1);

	if (!sk_if_CertSetSelect(&s->sd))
		return sk_if_GetLastErrorCode();

	if (sk_if_cert_preset_context(&s->appCtx, &s->sd) < 0)
		return sk_if_GetLastErrorCode();

	savePasswd(s, s->sd.szOldPasswd, sizeof(s->sd.szOldPasswd));
	s->selected = true;
	return 0;
}

CS_API  int CS_Sign(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen)
{
	if (!h || !sigOut || !sigOutLen)
		return -1;

	cslog("CS_Sign len=[%d] data=[%s] ", dataLen, data);

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

	CS_SESSION* s = (CS_SESSION*)h;

	cslog("[cersign]CS_SelectInteractive  data=[%s] ", dnOut);

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