#pragma once
// ControlWnd.h : header file
//

#include "afxinet.h"
#include "wininet.h"
#pragma comment (lib,"wininet.lib")

class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

	BOOL	Create(CWnd* pParent, void* param);
	void	SetParam(_param *pParam);

// Attributes
public:
	CWnd	*m_pParent;
	CParam	m_Param{};
	CString	m_strBody{};
	std::string m_strFileBody{};
	bool	m_bOverFileSize{};
	CString m_sGetPost{};
	CString m_sUrl{};
	CString m_sExt{};
	CString m_sPort{};
	CString m_sRoot{};
	int	m_nSendModeKind;


// Operations
public:
	int	HttpsSend(CString& sRecv);
	CString MakeHeader();
	int	HttpsDebugRecMsg(CString sData);
	CString	ErrorOut(DWORD dError);
	void	ClearHttps();
	CString	Variant(const CWnd* pWizard, int comm, CString data);

	std::string MakeEmFileData(CString strEmName, CString strEmData);
	CString	MakeEmTextData(CString strEmName, CString strEmData);
	CString MakeEmData(CString strEmName, CString strEmData);
	CString MakeFormData(CString strEmName, CString strEmData);
	CString MakeJsonData(CString strEmName, CString strEmData);
	CString	MadeEmBody();
	CString MakeListFormatData(CString strEmList, CString strEmData);
	std::string ReadFile(CString sPath);
	CString AnsiToUTF8RetCString(CString sData);
	CString CControlWnd::Utf8ToCString(const char* utf8String);
	void	DebugString(CString sFunction, CString sKey1 = _T(""), CString sval1 = _T(""), CString sKey2 = _T(""), CString sval2 = _T(""), CString sKey3 = _T(""), CString sval3 = _T(""), CString sKey4 = _T(""), CString sval4 = _T(""));
	void	WriteFile(CString sKey, const char* pBytes, int nBytes, CString sComp = _T(""));

	std::string ansi_to_utf8(const std::string& ansi_string);
	std::string utf8_to_ansi(const std::string& utf8_string);

	std::string charStarToHex(const char* str);
	std::string toHex(char c);
	std::string base64_encode(const std::string& in);
	std::string base64_decode(const std::string& encoded_string);
	std::string lpctstrToString(LPCTSTR lpctstr);
	CString Parser(CString& srValue, CString sGubn);

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
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void _ConnectServer(LPCTSTR strGetPost, LPCTSTR strUrl, LPCTSTR strExt, LPCTSTR strPort);
	afx_msg BSTR _SendHttps();
	afx_msg long _SetHttpsInfo(LPCTSTR strEmKind, LPCTSTR strEmName, LPCTSTR strEmData);
	afx_msg void _SetHttpsSendMode(LPCTSTR sKind);
	afx_msg BSTR _ConvertUtf8(LPCTSTR strValue);
	afx_msg BSTR _ConvertAnsi(LPCTSTR strValue);
	afx_msg BSTR _Base64Encode(LPCTSTR strValue);
	afx_msg BSTR _Base64Decode(LPCTSTR strValue);
	afx_msg BSTR _ConvertUtf8Len(LPCTSTR strValue, short nLen);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

