#ifndef __ALZIO__TRCOMM__HEADER__
#define	__ALZIO__TRCOMM__HEADER__

#include <afxmt.h>
#include "../../COMMON_BASE/TrComm/Packet.h"
#include "../../../New/DataMgr/DBMgr.h"


// ****************************************************************************
// ICallbackStatus
// 
// 0 : Ready State
// 1 : 소켓으로 Send하기 바로 전
// 2 : 소켓으로 Send한 바로 후
// 3 : 소켓으로부터 대기상태
// 4 : 소켓에서 데이터 받음
// ****************************************************************************
#define		ICBS_READY				0
#define		ICBS_SSENDBEFORE		1
#define		ICBS_SSENDAFTER			2
#define		ICBS_SREADY				3
#define		ICBS_SRECEIVE			4
#define		ICBS_TIMEOUT			-10
#define		ICBS_CREATEERR			-1
#define		ICBS_PARAMERR			-2

#define		WRET_READY				0
#define		WRET_RECEIVE			1
#define		WRET_TIMEOUT			2
#define		WRET_SENDERR			3
#define		WRET_QUIT				4



#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;




DECLARE_INTERFACE_(ICallbackStatus, IUnknown)
{
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;
	
    // *** IRichEditOleCallback methods ***
	STDMETHOD(ReceiveData) (HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen) PURE;
    STDMETHOD(StatusChange) (int nState) PURE;
};
typedef         ICallbackStatus FAR * LPCALLBACKSTATTUS;


// ****************************************************************************
// CTRComm
// ****************************************************************************
class CTRComm : public CWnd
{
public:
	CTRComm()
	{
		m_nTRTimer=-1;
		m_hSocket = NULL;
		m_pCallback = NULL;
		m_nTRTimeout = 90;
		m_bEnc = FALSE;
		m_dwTextStyle = DT_EXPANDTABS|DT_EXTERNALLEADING|DT_NOPREFIX|DT_WORDBREAK;
		m_nWaitResult = WRET_READY;
		m_bCert = FALSE;
	}
	~CTRComm()
	{
		StopTRTimer();
		DestroyWindow();
	}

	// Data Send
public:
	// TR ID관리과 HanWha TR 
	BOOL	Send2HanWha(void* pRecData, int nLen);
	BOOL    Send2IntData(void* pRecData, int nLen, CWnd* pSendWnd, int nType = 0);
	BOOL	Send2Server(LPCSTR szData, int nLen, BOOL bWait=FALSE, LPCTSTR szHeader="");
	BOOL	Send2Server_Buf(LPCSTR szData, int nLen, BOOL bWait=FALSE, LPCTSTR szHeader="");
	BOOL	Send2Gateway(LPCSTR szData, int nLen, CDBMgr* pDBMgr, BOOL bWait=FALSE, BOOL bNext=FALSE);
	BOOL	Send2Direct(LPCSTR szData, LPCSTR szHeader, int nLen, BOOL bWait/*=TRUE*/);
	BOOL	RealSend(HWND hWnd, LPVOID pData, int nLen, DWORD dwType=TYPE400, BOOL bBPTrData=FALSE);
	void	SetTR(LPCSTR szTR, BOOL bCert = FALSE) { m_szTR = szTR;  m_bCert = bCert;}
	void	SetSocket(HWND hSocket)	{ m_hSocket = hSocket;	}
	void	SetTimeout(int nValue) { m_nTRTimeout = nValue;	}
	void	SetEnc(BOOL bValue) { m_bEnc = bValue;	}
	void	ChangeCallback(void*	pCallback) { m_pCallback=(ICallbackStatus*)pCallback;	}

protected:
	int		m_nWaitResult;		// 데이터 처리상태
	ICallbackStatus*	m_pCallback;	// 콜백처리용 인터페이스
	HWND	m_hSocket;		// 소켓핸들
	int		m_nErrorCode;	// 에러코드
	CString	m_szTR;			// TR번호
	int		m_nTRTimer;		// TimerID
	int		m_nTRTimeout;	// TimeOut설정
	BOOL	m_bEnc;			// Data Encrytion
	BOOL	m_bCert;		// 공인인증 여부
	
	CWnd	*m_pParent;
	DWORD	m_dwTextStyle;		// Extended Style
	BYTE	m_ucPrevNext;
	BYTE	m_ucErrCode;

	void	StartTRTimer();
	void	StopTRTimer();

	BOOL	NotifyParent(LPCSTR szTRName, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen );
	int		OnCopyData_400(LPVOID lpData, DWORD cbData);		
	int		WaitResult();
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTRComm)
	public:
	virtual BOOL Create(CWnd* pParentWnd=NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	BYTE GetPrevNextByte(){return m_ucPrevNext;}
	BYTE GetErrCodeByte(){return m_ucErrCode;}
	// Generated message map functions
protected:
	//{{AFX_MSG(CTRComm)
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // __ALZIO__TRCOMM__HEADER__
