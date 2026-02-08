#ifndef __ALZIO__TRCOMM__HEADER__
#define	__ALZIO__TRCOMM__HEADER__

#include <afxmt.h>
#include "../../inc/IAUTrCommMng.h"
#include "../../inc/Packet.h"
#include "../../inc/IAUPartnerMng.h"

// ****************************************************************************
// ITRCommSite
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

#define		RQ_FID					0
#define		RQ_TR					1

class CFIDOutputInfo
{
public:
	CFIDOutputInfo() {}

	CDWordArray	pItemLength;		// 아이템별 길이
	CString		strOutputFID;		// 전체 Output FID : 3으로 나누면 갯수 나옴.
};

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
		m_nQueryCount = 0;
		m_nTRType = RQ_FID;	//0:FID 1:TR
		m_cDestination = ' ';
		m_nUniqueID = 1;
		m_nSendDebugID = 0;
		m_nRceiDebugID = 0;
		m_nMsgDebugID = 0;

		m_pTRData = NULL;
		m_nMaxTrSize = 0;
		m_pPtManager = NULL;
		m_pPartnerSite = NULL;
	}
	~CTRComm()
	{
		if(m_pTRData) free(m_pTRData);
		m_pTRData = NULL;

		DestroyWindow();
	}

	// Data Send
public:
	BOOL	Send2Server(void* szData, int nLen, int nKey);
	void	SetTR(LPCSTR szTR, BOOL bCert = FALSE);
	void	ChangeCallback(void*	pCallback) { m_pCallback=(ITrCommSite*)pCallback;	}

	BOOL	IsWaitTRRequest();

	//@solomon 추가 20090928정용호
	BOOL	Send2ServerWithKey(void* szData, int nLen, int nKey); 

protected:
	int		m_nWaitResult;		// 데이터 처리상태
	ITrCommSite*	m_pCallback;	// 콜백처리용 인터페이스
	HWND	m_hSocket;		// 소켓핸들
	int		m_nErrorCode;	// 에러코드
	CString	m_szTR;			// TR번호
	int		m_nTRTimer;		// TimerID
	int		m_nTRTimeout;	// TimeOut설정
	BOOL	m_bEnc;			// Data Encrytion
	BOOL	m_bCert;		// 공인인증 여부
	
	CWnd	*m_pParent;
	DWORD	m_dwTextStyle;		// Extended Style

	void	StartTRTimer();
	void	StopTRTimer();

	BOOL	NotifyParent(LPCSTR szTRName, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen );
	int		WaitResult();

public:
	// 서버로부터 데이타를 받았을경우에 Memory를 보호하기위해서 Critical Session으로 보호한다.
	CCriticalSection	m_CommCritical;

	CString m_strFidOutList;		// 출력용 FID리스트 구성용
	void	OutPutFid(CString strFidList);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTRComm)
	public:
	virtual BOOL Create(CWnd* pParentWnd=NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CTRComm)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnGetMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
//@Solomon090819	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);

public:

protected:
	int	m_nQueryCount;
	int m_nTRType;
	char	m_cDestination;
	CMapStringToString	m_DataTable;
	CMapStringToString	m_PacketID2TrCode;

public:
	void SetDestination(char cDestination){m_cDestination=cDestination;};
	void SetBaseData(LPCSTR szKey, LPCSTR szValue);
	void GetBaseData(LPCSTR szKey, CString &rValue);
	void SetBaseDataLong(LPCSTR szKey, LONG dwValue);

	long Send_MessageData(LPCSTR szTRCode, LPCSTR szMsgCode, LPCSTR szMsg);
	long Send_PacketData(BYTE* pPackData, int nSize, LPCSTR szTRCode, LPCSTR szMsgCode, LPCSTR szMsg);
	
	int	m_nSendDebugID, m_nRceiDebugID,m_nMsgDebugID, m_nUniqueID;
	int GetPacketUniqueID(BOOL bInCrease=TRUE); 

protected:
	UINT m_nMaxTrSize;
	BYTE* m_pTRData;

	LONG m_pPtManager;
	IPartnerSite	*m_pPartnerSite;

//	CCriticalSection	m_InSec;;

	int _DoPreCheck(LPBYTE szData, long nLen, int nKey, BOOL &bChk);
	int _DoPreCheck_WorkDate(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
	int _DoPreCheck_Dollar(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
	int _DoPreCheck_StockHoga(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
	int _DoPreCheck_MstCode(LPBYTE szData, long nLen, LPCSTR szInputKey, BOOL &bChk);
};

#endif // __ALZIO__TRCOMM__HEADER__
