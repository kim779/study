#if !defined(AFX_CONNECTOR_H__00BCFC87_E289_47CD_A945_155578451305__INCLUDED_)
#define AFX_CONNECTOR_H__00BCFC87_E289_47CD_A945_155578451305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Connector.h : header file
//

#define	WM_CONNECTOR_MSG	WM_USER + 100

#define	WL_TRYCONNECT		WM_USER + 200
#define	WL_RUN			WM_USER + 201
#define	WL_QUERY		WM_USER + 202
#define	WL_RECEIVE		WM_USER + 203
#define	WL_DISCONNECT		WM_USER + 204
#define	WL_ALERT		WM_USER + 205
#define	WL_NOTICE		WM_USER + 206
#define	WL_ERROR		WM_USER + 207
#define	WL_CBDATA		WM_USER + 208

#define	WL_ENABLEFALSE		WM_USER + 209
#define	WL_ENABLETRUE		WM_USER + 210

struct stConnectInfo
{
	CTime	m_timeTryConnect;
	CTime	m_timeConnected;
	CTime	m_timeLastQuery;
	CTime	m_timeLastOub;
	CTime	m_timeLastAlert;
};

class CQueryData
{
public:
	BOOL	m_bOop;
	CString	m_trCode;
	char*	m_pData;
};

// 대투 Login Data
const int dtUser = 0, dtName = 1, dtTerm = 2, dtPostCode = 3, dtPostName = 4, 
	dtAuth = 5, dtRegNo = 6, dtOther = 7, dtGroup = 8, dtMgrApprIp = 9, 
	dtMgrApprTermId = 10, dtDate = 11, dtDeptCd = 12, dtChannel = 13, dtNone = 14;

// 하나 Login Data
const int  hnUser = 0, hnName = 1, hnPostCode = 2, hnPostName = 3, hnAuth = 4, 
	hnOther = 5, hnGroup = 6, hnBzPostCode = 7, hnBzPostName = 8, hnDate = 9,
	hnTerm = 10, hnTestYn = 11, hnDptKnd = 12, hnSecNo = 13, hnQryKnd = 14,
	hnMapN = 15, hnConnState = 16, hnDeptSecNo = 17, hnNone = 18;

#define SIGNON_CNT (((dtNone) > (hnNone)) ? (dtNone) : (hnNone))

/////////////////////////////////////////////////////////////////////////////
// CConnector window

class CConnector : public CWnd
{
// Construction
public:
	CConnector(CWnd* pParent, int idx);

	
// Attributes
public:
	CWnd*		m_parent;
	int		m_nAbsIndex;
	int		m_iCount;
	int     m_iQueryGubn;


	CString		m_sId;
	CString		m_sPwd;
	CString		m_suuid;
	CString		m_sOpt;
	CString		m_sCaPwd;
	CString		m_sIp;
	int		m_nPort;

	CPtrArray*	m_parQueryData;
	int		m_nQueryInterval;
	int		m_nResponseWaitTime;

	CString		m_page;
	CString		m_save;
	bool		m_gridNextMode;
	bool		m_gridAppendMode;

	struct stConnectInfo m_connectInfo;

	CWnd*		m_sock;
	CWnd*		m_xecure;
	CWnd*		m_certify;

	enum		{ lkUser,	// 일반유저
			  lkSiseOnly }	// 정규접속 없이 기본시세 제공
			m_loginKind;
	enum		{ mtNO, mtCON, mtSIGN, mtCA, mtXEC, mtRUN } m_mode;

	CString		m_hts;		// 접속 HTS ("D" 대투, "H" 하나)
	BOOL		m_autoCaPwd;	// 공인인증 비밀번호 자동접속
	BOOL		m_ca;		// 공인인증 or 시세전용접속
	int		m_port;

	BOOL		m_order;
	unsigned char	m_term;
	CString		m_AppUser, m_AppTerm;
	CString		m_signOn[SIGNON_CNT];
	CStringArray	m_accounts;
	char*		m_pLedger;
	CString		m_infoX;
	CString		m_mapN;
	int			igubn;
// Operations
public:
	void	SetUser(CString Id, CString Pwd, CString uuid =_T(""));
	char*	GetTrData(int igubn);
	void	InitQuery(CPtrArray* QueryData, int QueryInterval, int ResponseWaitTime);
	BOOL	DoQuery();
	BOOL	DoQuery(int id);
	void	SetQueryData(int icnt, int igugn);

	void	DoConnect(CString Ip, int Port, CString Opt);
	void	DoCloseConnect();
	void	SendMsgToParent(UINT msg, LPARAM lParam = 0);

	BOOL	CreateWnd(int nID);
	BOOL	Open(char* ip, int port, char* id, char* pwd, char* capwd, char* opt);
	void	Close();
	BOOL	SendTr(struct _axisH* axH, char* datB, int datL, int igubn = 0);

	void	OnAlert(int type, CString text);
	void	OnPush(char* pBytes, int nBytes);
	void	OnRead(char* pBytes, int nBytes);
	BOOL	OnAxis(int type, char* pBytes, int variant);
	void	OnSign(int signK, char *pBytes, int nBytes);
	void	OnXecure(int encK, char *pBytes, int nBytes);
	void	OnAxisOubs(struct _axisH* axisH, char *pBytes, int nBytes, int totalL);
	void	OnNotice(char* pBytes, int nBytes);
	void	OnCertify(char* pBytes, int nBytes);

	void	GetledgerH(struct _ledgerH* pLedgerH, int igubn);
private:

private:
	BOOL	SignOn();
	BOOL	SetCertify(BOOL Active);
	void	RunXecure();
	int	DoXecure(char* pBytes, int nBytes);
	int	DoCertify(char* pBytes, int nBytes);
	BOOL	Write(int msgK, CString trxC, char* datB, int datL, int key = 0);
	void	Run();

	// Call axSock ActiveX control Function
	BOOL	OpenSock(LPCTSTR home, LPCTSTR ip, long port);
	void	CloseSock();
	BOOL	StartAxis(char* key, long mode);
	BOOL	WriteStream(char* pBytes, long nBytes);

	// Call Xecure ActiveX control Function
	BOOL	Xecure(int helper, char* pBytes, int& nBytes);
	BOOL	Certify(char* pBytes, int& nBytes, char* maps);
	BOOL	CertifyErr(char* pBytes, int nBytes);

	void	sendErr(int ErrCode, CString ErrText);
	void	sendDialogPAN(int winK, CString DialogMsg);
	void	clearLoginData();
	void	readSignR(char* datB);
	void	addRegistry(char* datB, int datL, CString& dns);
	CString getLocalIP();
	CString getMacAddress();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnector)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConnector();

	// Generated message map functions
protected:
	afx_msg void OnFireStream(int type_, int pBytes, int nBytes);

	afx_msg void OnRunMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDisconnectMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOubMsg(WPARAM wParam, LPARAM lParam, int nBytes);
	afx_msg void OnAlertMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNoticeMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnErrorMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDialogPanMsg(WPARAM wParam, LPARAM lParam);

	//{{AFX_MSG(CConnector)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()  
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTOR_H__00BCFC87_E289_47CD_A945_155578451305__INCLUDED_)
