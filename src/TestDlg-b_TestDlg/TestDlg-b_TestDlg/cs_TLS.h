#pragma once

struct ThreadData {
	DWORD threadID;
	CString strdata;
};

class cs_TLS
{
public:
	cs_TLS();
	cs_TLS(DWORD tdID);
	virtual ~cs_TLS();

	DWORD m_tdID;
	DWORD tlsIndex;
	void SetThreadLocalData(ThreadData* pdata);
	ThreadData* GetThreadLocalData();
};

