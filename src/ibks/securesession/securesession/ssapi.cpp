#include "pch.h"
#include "securesession.h"
#include "Xecure/xc_main.h"

#pragma comment (lib , "Xecure/xcon30.lib")

#define SS_MAXLEN 1024 * 64

enum { SS_STATE_HELLO, SS_STATE_OK, SS_STATE_RUN };   // ← 이 줄이 빠졌었습니다

struct SS_SESSION
{
	XC_CTX ctx;
	int state;
	unsigned char buf[SS_MAXLEN];
	int bufLen;
};

HSS SS_Open(const char* password, const char* confPath, unsigned char* outBuf, int* outLen)
{
	if (XC_INIT((char*)password, (char*)confPath, XC_SMODE_CLIENT) < 0)
		return nullptr;

	SS_SESSION* sess = new SS_SESSION();
	ZeroMemory(&sess->ctx, sizeof(XC_CTX));
	sess->state = SS_STATE_HELLO;

	sess->bufLen = SS_MAXLEN;
	if (XC_ENCODE(&sess->ctx, sess->buf, &sess->bufLen, nullptr, 0, nullptr, 0, XC_TMODE_KEY) < 0)
	{
		delete sess;
		return nullptr;
	}
	sess->state = SS_STATE_OK;

	if (sess->bufLen > *outLen) //호출자 버퍼가 너무 작으면 실패 
	{
		delete sess;
		return nullptr;
	}

	CopyMemory(outBuf, sess->buf, sess->bufLen);
	*outLen = sess->bufLen;

	return (HSS)sess;
}


const char* SS_GetLastError(void)
{
	return XC_GETERR();
}
int SS_Handshake(HSS h, const unsigned char* inBuf, int inLen,
	unsigned char* outBuf, int* outLen)
{
	if (!h)
		return -1;

	SS_SESSION* sess = (SS_SESSION*)h;
	if (sess->state != SS_STATE_OK)
		return -1;

	int decLen = SS_MAXLEN;
	int rc = XC_DECODE(&sess->ctx, sess->buf, &decLen, (unsigned char*)inBuf, inLen, NULL, 0);

	if (rc < 0)
		return -1;   // 진짜 실패, 확실하게 실패 처리

	if (rc == XC_MTYPE_NEWPROFILE)
	{
		sess->bufLen = SS_MAXLEN;
		if (XC_ENCODE(&sess->ctx, sess->buf, &sess->bufLen, NULL, 0, NULL, 0, XC_TMODE_KEY) < 0)
			return -1;

		if (sess->bufLen > *outLen)
			return -1;
		CopyMemory(outBuf, sess->buf, sess->bufLen);
		*outLen = sess->bufLen;
		return 1;   // 미완료, outBuf를 서버로 다시 보내고 SS_Handshake 재호출 필요
	}

	sess->state = SS_STATE_RUN;
	*outLen = 0;
	return 0;   // 완료. 이제 SS_Encrypt/SS_Decrypt 사용 가능
}

int SS_Encrypt(HSS h, const unsigned char* in, int inLen, unsigned char* out, int* outLen)
{
	if (!h) return -1;
	SS_SESSION* sess = (SS_SESSION*)h;
	if (sess->state != SS_STATE_RUN) return -1;
	if (inLen > SS_MAXLEN) return -1;

	sess->bufLen = SS_MAXLEN;
	if (XC_ENCODE(&sess->ctx, sess->buf, &sess->bufLen, (unsigned char*)in, inLen, NULL, 0, XC_TMODE_MSG) < 0)
		return -1;

	if (sess->bufLen > *outLen) return -1;
	CopyMemory(out, sess->buf, sess->bufLen);
	*outLen = sess->bufLen;
	return 0;
}

int SS_Decrypt(HSS h, const unsigned char* in, int inLen, unsigned char* out, int* outLen)
{
	if (!h) return -1;
	SS_SESSION* sess = (SS_SESSION*)h;
	if (sess->state != SS_STATE_RUN) return -1;
	if (inLen > SS_MAXLEN) return -1;

	sess->bufLen = SS_MAXLEN;
	if (XC_DECODE(&sess->ctx, sess->buf, &sess->bufLen, (unsigned char*)in, inLen, NULL, 0) < 0)
		return -1;

	if (sess->bufLen > *outLen) return -1;
	CopyMemory(out, sess->buf, sess->bufLen);
	*outLen = sess->bufLen;
	return 0;
}

void SS_Close(HSS h)
{
	if (h)
		delete (SS_SESSION*)h;
}