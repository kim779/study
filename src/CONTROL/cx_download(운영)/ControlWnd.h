#pragma once
// ControlWnd.h : header file
//
#include "xSkinButton.h"
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window
#define IDC_BTN_DOWNLOAD			9384
#define IDC_CTRL_PROGRESS			9385
#define IDC_STATIC_NAME				9837
#define IDC_STATIC_FILENAME			9838
#define IDC_STATIC_FOLDER			9839
#define IDC_EDIT_FOLDER				9840
#define IDC_STATIC_MSG				9841
#define IDC_BTN_FOLDER				9842
#define IDC_BTN_EXCUTE				9843
#define IDC_BTN_CLOSE				9849

#define ID_DOWNLOAD_TIMER			9386

#define CONTEXT_IBKS			9999
#define WM_STATUS_CALLBACK		WM_USER+9875

class CInternetSession1 : public CInternetSession
{
public:
	CInternetSession1( HWND hWnd,
			LPCTSTR pstrAgent = "IBKS",
			DWORD_PTR dwContext = CONTEXT_IBKS,
			DWORD dwAccessType = INTERNET_OPEN_TYPE_DIRECT,
			LPCTSTR pstrProxyName = NULL,
			LPCTSTR pstrProxyBypass = NULL,
			DWORD dwFlags = INTERNET_FLAG_DONT_CACHE) 
	: CInternetSession(pstrAgent,dwContext,dwAccessType,pstrProxyName,pstrProxyBypass,dwFlags)
	{
		//EnableStatusCallback();
		m_hWnd = hWnd;
	}
	~CInternetSession1() {}

	void OnStatusCallback(DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength) 
	{
		::PostMessage(m_hWnd, WM_STATUS_CALLBACK, (WPARAM)dwInternetStatus, (LPARAM)dwContext);
	}
private:
	HWND m_hWnd;

};

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd(_param* pParam, CWnd* pWizard);

// Attributes
public:

// Operations
public:

protected:
	CString		GetAxVariant(int cmd, CString data=_T(""));
	CFont*		GetAxFont(CString fName, int point, int style);

	BOOL		DoIt();	

	BOOL		CheckDownloadCondition(void);

	void		SetMsg(CString str);
private:

	BOOL		m_bDownload;

	BOOL		DoHttp(CInternetSession1* pSess);
	BOOL		DoFtp(CInternetSession1* pSess);
	
	CWnd*		m_pWizard;

	COLORREF	m_clrBack;
	COLORREF	m_clrText;

	CFont*		m_pFont;

	int			m_sParamKey;
	CString		m_sParamName;

	CString		m_strFilter;
	CString		m_strOption;

	CxSkinButton	m_btnDownload;
	CxSkinButton	m_btnFolder;
	CxSkinButton	m_btnExcute;
	CxSkinButton	m_btnClose;

	CProgressCtrl	m_ctrlProg;

	CStatic		m_ctrlName;
	CStatic		m_ctrlFolder;
	CStatic		m_ctrlFileName;
	CStatic		m_ctrlMsg;
	
	CEdit		m_ctrlEdit;		
	
	CBrush		m_brushBack;

	CString		GetFolderPath(CString strFilePath, CString& strFileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	CString m_address;
	afx_msg void OnAddressChanged();
	CString m_protocol;
	afx_msg void OnProtocolChanged();
	CString m_username;
	afx_msg void OnUsernameChanged();
	CString m_pw;
	afx_msg void OnPwChanged();
	CString m_filename;
	afx_msg void OnFilenameChanged();
	CString m_directory;
	afx_msg void OnDirectoryChanged();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void OnClickDownload();
	afx_msg void OnClickFolder();
	afx_msg void OnClickExcute();
	afx_msg void OnClickClose();
	afx_msg BSTR IsDownload();
	afx_msg void DoDownload();
	afx_msg long CheckURLEnable(LPCTSTR strUrl);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};