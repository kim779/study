#include "pch.h"
#include "cs_TLS.h"

cs_TLS::cs_TLS()
{
	tlsIndex = TlsAlloc();
}

cs_TLS::cs_TLS(DWORD tdID)
{
	m_tdID = tdID;
	tlsIndex = TlsAlloc();
}

cs_TLS::~cs_TLS()
{
	TlsFree(tlsIndex);
}

void cs_TLS::SetThreadLocalData(ThreadData* pdata)
{
	TlsSetValue(tlsIndex, pdata);
	ThreadData* pData = reinterpret_cast<ThreadData*>(TlsGetValue(tlsIndex));
}

ThreadData* cs_TLS::GetThreadLocalData() {
	// 현재 스레드의 TLS에서 데이터 가져오기
	ThreadData* pData = reinterpret_cast<ThreadData*>(TlsGetValue(tlsIndex));
	return pData;
}