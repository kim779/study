#pragma once



// CTD_test

class CTD_test : public CWinThread
{
	DECLARE_DYNCREATE(CTD_test)

protected:
	CTD_test();           // 동적 만들기에 사용되는 protected 생성자입니다.

public:
	virtual ~CTD_test();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	int m_id;
	int m_data;
	CString m_slog;
	CCriticalSection m_cs;

	// race condition demo
	enum TDRole { TD_WRITER = 0, TD_READER = 1 };
	TDRole m_role;
	CArray<CString, CString>* m_pArr;
	CCriticalSection* m_pLock;
	bool m_bUseLock;
	volatile bool m_bAlive;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


