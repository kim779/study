#pragma once


// CWC_FileSync
#include <queue>
#include <afxmt.h>

class CMQue : public CObject
{
public:
	CMQue();
	virtual ~CMQue();

	CWnd* m_pParentWnd{};
	CString m_strSec{}, m_strItem{}, m_strVal{}, m_strFile{};
};

class CWC_FileSync : public CWnd
{
	DECLARE_DYNAMIC(CWC_FileSync)

public:
	CWC_FileSync();
	virtual ~CWC_FileSync();

	CString _slog{};
	CString m_strSec{}, m_strItem{}, m_strVal{}, m_strFile{};

	//파일 writting 동기처리
	CRITICAL_SECTION csMapHandle{};
	std::queue<void*> m_thQueue;
	CObArray	m_que{};
	BOOL m_bUseing{};
	CCriticalSection m_cs;

	BOOL CheckThreadUsing();
	void StartNextThread();
	void synWritePrivateProfileString(CString sSec, CString sItem, CString sVal, CString sPath);


	void cs_Lock();
	void cs_Unlock();


	DWORD convert_ansi_to_unicode(CStringW& swUnicode, const char* ansi, const int isize);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


