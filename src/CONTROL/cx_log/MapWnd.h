#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window


#include "../../h/axisgenv.h"
#include "../../h/axisfire.h"

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(_param* pParam, CWnd* pWizard );

	CWnd*	m_pWizard;
	int	m_sParamKey;
	CString m_sParamName;

	CString m_strHome{}, m_strConfig{}, m_strInstall{}, m_strInstallName{};
	CString m_slog{};
	CString m_strMemo{};

	HANDLE m_hKeyShared{};
// Attributes
public:
	UINT AFXAPI HashDataAXIS(LPCSTR key);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMapWnd)
	afx_msg void axlog(LPCTSTR stag, LPCTSTR slog);
	afx_msg BSTR IDtoSha(LPCTSTR sId);
	afx_msg BSTR getroot();
	afx_msg void openPDF(LPCTSTR strPDF);
	afx_msg void Writelog(LPCTSTR spath, LPCTSTR slog);
	afx_msg BSTR HTSEncode(LPCTSTR sID, LPCTSTR sKey);
	afx_msg void ExcuteFile(LPCTSTR fullpath);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	void OpenChrome(BSTR strUrl);

	enum
	{
		dispidsVal = 26,
		dispidDoSomething = 25L,
		dispidGetMonitorY_inch = 24L,
		dispidGetMonitorX_inch = 23L,
		dispidGetDPIY = 22L,
		dispidGetDPIX = 21L,
		dispidGetScreenHeight = 20L,
		dispidGetScreenWidth = 19L,
		dispidSendSharedMsg = 18L,
		dispidInitShared = 17L,
		dispidDecFileFromDlg = 16L,
		dispidDecHFile = 15L,
		dispidWriteHlog = 14L,
		dispidMemo_Delete = 13L,
		dispidsMemo = 12,
		dispidMemo_Upload = 11L,
		dispidMemo_Search = 10L,
		dispidEDGE_lib_Installed = 9L,
		dispidOpenChrome = 8L
	};
	void EDGE_lib_Installed();
	BOOL  CheckEdgelibInstalled();
	void checkMemo(CString sKey);
	void Memo_Search(BSTR sCode);
	void SendMemoTR_Control(CString name, char* datb, int datl, char stat, int trkey, CString sKey);
	void Memo_Upload(BSTR sCode, BSTR sMemo);
	void OnsMemoChanged();
	CString m_sMemo;
	void Memo_Delete(BSTR sCode);
	void WriteHlog(BSTR sFile, BSTR sSec, BSTR sKey,  BSTR sVal);
	void DecHFile(BSTR sFile, BSTR sFolder);
	void DecFileFromDlg();
	void InitShared();
	void SendSharedMsg(BSTR strVal);
	BSTR GetScreenWidth();
	BSTR GetScreenHeight();
	BSTR GetDPIX();
	BSTR GetDPIY();
	BSTR GetMonitorX_inch();
	BSTR GetMonitorY_inch();
	void DoSomething(BSTR sDo, BSTR sgubn);
	void OnsValChanged();
	CString m_sVal;

	CString Parser(CString& srcstr, CString substr);
	CWnd* m_pMainWnd{};  //axis.exe ÀÇ mainFrame
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
