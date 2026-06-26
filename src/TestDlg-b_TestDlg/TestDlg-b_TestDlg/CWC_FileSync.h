#pragma once


// CWC_FileSync
#include <queue>
class CWC_FileSync : public CWnd
{
	DECLARE_DYNAMIC(CWC_FileSync)

public:
	CWC_FileSync();
	virtual ~CWC_FileSync();

	CString _slog{};


	//파일 writting 동기처리
	CRITICAL_SECTION csMapHandle{};
	std::queue<void*> m_thQueue;
	CObArray	m_que{};
	BOOL m_bUseing{};
	CCriticalSection m_cs;
	CEvent		m_event;

	BOOL CheckThreadUsing();
	void StartNextThread();
	void synWritePrivateProfileString(CString sSec, CString sItem, CString sVal, CString sPath);


	void cs_Lock();
	void cs_Unlock();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


