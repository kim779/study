#pragma once
// MainWnd.h : header file
//

typedef  struct  mid {
	char	func[1];	// 'Q' : 조회 'I' 입력
	char	mapn[8];	// 화면번호
	char	kind[1];	// 로그종류
				// 화면별 세부구분
	char	data[512];	// 로그 데이타(MAX 1024)
} pidologm_mid;

class CCodeMap
{
public :
	CString code;
	int	loss=0;
	int	earn=0;
	int	remain=0;
	int	dndiff=0;
	int	maxprice=0;
	int	danga=0;
	int	tick=0;
};

class CParam
{
public:
	int	key=0;		// screen key
	CString	name;		// control CX_Symbol name
	CRect	rect;		// control rect
	CString	fonts;		// font name
	int	point=0;	// font point
	int	style=0;	// font style
	DWORD	tRGB=0;		// text color index
	DWORD	pRGB=0;		// paint color index
	CString	options;	// properties
};

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* parent,_param *param);

// Attributes
public:
	CWnd*		m_pParent;
	CParam		m_Param;
//	CMapStringToPtr	m_mapCode;
	CMap < CString, LPCTSTR, std::shared_ptr<CCodeMap>, std::shared_ptr<CCodeMap> > m_mapCode;
	BOOL		m_bStart;
	CString		m_sAcnt;
	CString		m_sPass;

// Operations
public:
	void ParseAlert(struct _alertR* alert);
	BOOL CheckRealData(CString code,CString sCurr);

private:
	CString		m_sMapHandle;
	CString		m_szName;
	CStringArray	m_arLog;
	BOOL		m_bNewUnit;	// 2023 호가단위개선

private:
	void	SendTR(CString name, CString data, BYTE type, int key);
	void	SetParam(_param *pParam);
	CString Variant(int comm, CString data);
	void	SplitString(CString &strData, CString strToken,CStringArray &ar);
	CString GetJumunCrtCode(CString sCrtCode);
	void	SendLog(CString sData);
	void	SendLogOnce();
	BOOL	RemoveCondition(CString keys);
	int	checkDelta(int spVal, int icodetype);
	int	checkDelta(int spVal, int iCodeType, bool bPlus);
	int	getTickPrice(int price, int tick, int codetype, int iplus);
	int	getTickValue(int price, int tick, int codetype);
	int	getPrice(int price, int codetype, int iroundup = 0/*올림*/);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg BSTR GetProperties();
	afx_msg void Start(BOOL bStop);
	afx_msg BOOL SetCondition(LPCTSTR index,LPCTSTR date,LPCTSTR code, LPCTSTR curr);
	afx_msg long GetPriceSetupValue(LPCTSTR code,long icostval,long iunitType, long istopType, long iconfigval);
	afx_msg void DelCondition(LPCTSTR index,LPCTSTR date,LPCTSTR code);
	afx_msg void Run();
	afx_msg long GetTickPrice(long price, short tick, LPCTSTR code, short iplus);
	afx_msg void SetMapHandle(LPCTSTR sHandle);
	afx_msg BSTR GetMapHandle();
	afx_msg void Reset();
	afx_msg void WriteLog(LPCTSTR sType, LPCTSTR sLog, LPCTSTR sCode, LPCTSTR sName);
	afx_msg void SendTestData(LPCTSTR code, LPCTSTR curr);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};