#pragma once


#include "pch.h"
// CWsh
//class CMQue : public CObject
//{
//public:
//	CMQue();
//	virtual ~CMQue();
//
//public:
//	int m_ssm;
//	int m_nBytes;
//	char* m_pBytes;
//};

class Cupdate
{
public:
	CString m_name;
	int m_size;
	CString m_info;
};

#define	WFM_EVENT		WM_USER
#define	WFM_PUSH		(WM_USER+1)
#define	WFM_AXIS		(WM_USER+2)
#define	WFM_STAT		(WM_USER+3)
#define	WFM_WRITE		(WM_USER+4)
#define	WFM_VERS		(WM_USER+5)

class CWsh : public CWinThread
{
	DECLARE_DYNCREATE(CWsh)

protected:
	CWsh();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CWsh();

private:
	bool m_alive;
	CEvent m_event;
	CObArray m_que;

	CCriticalSection m_section;
	CWnd* m_pWnd;
	CString		m_home;

	int		m_current;
	int		m_order;
	CArray < Cupdate*, Cupdate*> m_list;
	enum class stFlag { stNONE = 0, stAXIS, stRSC, stDONE, stERR } m_state;

	CStdioFile	m_file;
	class	CZip* m_zip;

	char* m_datb;
	int		m_bytes;

	bool		m_axisOnly;
	bool		m_runDown;	// runtime download

public:
	bool m_running;
	CWinThread* m_thread;
	CEvent		m_eventRTM;
	CObArray	m_queRTM;
	CCriticalSection m_sectionRTM;
	CString m_slog;
	int m_tick;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();

public:
	void Dispatch(int ssm, char* pBytes, int nBytes);
	void	Startup(CWnd* pWnd, char* home) { m_pWnd = pWnd; m_home = home; }
	void	Cleanup() { m_alive = false; m_event.SetEvent(); }
	bool	GetUpdateList();
	BOOL	DoRequest(bool info = false);
	int	GetQueuingCount();
	void	SetDownloadMode(bool axisOnly, int runtime = -1);

	void	BeginRTM();
	void	TerminateRTM();
	void	SendRTM(class CMQue* pMQ);

public:
	void DoParse(class CMQue* pMQ);
	void ParseRTM(class CMQue* pMQ);
	bool MakeDir(struct _rsmH* rsmH, CString& path, bool temporary = false);
	BOOL IsDir(CString path);

	void Update(char* pBytes, int nBytes, BOOL update = FALSE);
	bool MakeUpdateList();
	void ClearUpdateList();
	int LoadFile(CString path, CStringArray& arr);
	void LoadVersion(CString path);

	void DoClose();





};


