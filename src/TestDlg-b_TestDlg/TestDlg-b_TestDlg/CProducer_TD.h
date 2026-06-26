#pragma once



// CProducer_TD
#include "pch.h"
#pragma once
//#ifndef DF_MQUE
//#define DF_MQUE
//class CMQue : public CObject
//{
//public:
//	CMQue();
//	virtual ~CMQue();
//
//public:
//
//	int	m_ssm;
//
//	int	m_nBytes;
//	char* m_pBytes;
//};
//#endif

class CProducer_TD : public CWinThread
{
	DECLARE_DYNCREATE(CProducer_TD)

public:
	CProducer_TD();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CProducer_TD();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	int m_id;
	CWnd* m_pWnd{};
	CString m_home;
	bool		m_alive;
	CEvent m_event, m_Event_end;
	CCriticalSection m_section, m_2section;
	CObArray	m_que;
	void	Startup(CWnd* pWnd, char* home) { m_pWnd = pWnd; m_home = home; }
	void	Dispatch(int ssm, char* pBytes, int nBytes);
	void endThread();

	void test_stopcode();
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


