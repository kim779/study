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
	char		password[64];
	bool		selected;
};

CS_API HCS CS_Open(void)
{
	CS_SESSION* s = new CS_SESSION();
	ZeroMemory(&s->appCtx, sizeof(APP_CONTEXT));
	ZeroMemory(&s->sd, sizeof(SD_API_CONTEXT));
	s->password[0] = 0;
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

	strncpy_s(s->password, sizeof(s->password), password, _TRUNCATE);
	s->selected = true;
	return 0;
}

CS_API  int CS_Sign(HCS h, const unsigned char* data, int dataLen, unsigned char* sigOut, int* sigOutLen)
{
	if (!h || !sigOut || !sigOutLen)
		return -1;

	CS_SESSION* s = (CS_SESSION*)h;
	if (!s->selected)
		return -1;

	UString in = { dataLen, (unsigned char*)data };
	UString out = { 0, nullptr };
	if (sk_if_cert_SignData(&s->appCtx, s->password, &in, &out))
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

	SD_API_CONTEXT_NEW ctxNew;
	ZeroMemory(&ctxNew, sizeof(SD_API_CONTEXT_NEW));

	sk_if_SetWrongPasswordLimit(1);
	sk_if_SetKeySaferMode(1);

	if (parentWnd && IsWindow(parentWnd))
		sk_if_DialogModalMode(parentWnd);

	if (!sk_if_CertSetSelectExt(&ctxNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA))
		return sk_if_GetLastErrorCode();

	if (sk_if_cert_preset_context(&s->appCtx, &ctxNew.sd) < 0)
		return sk_if_GetLastErrorCode();

	if (dnOut)
		strncpy_s(dnOut, dnOutCap, ctxNew.sd.szDN, _TRUNCATE);
	if (storageOut)
		*storageOut = ctxNew.sd.bOldStorage;
	if (certSetNameOut)
		strncpy_s(certSetNameOut, certSetNameOutCap, ctxNew.sd.szCertSetName, _TRUNCATE);

	CopyMemory(&s->sd, &ctxNew.sd, sizeof(SD_API_CONTEXT));
	s->selected = true;

	return 0;
}