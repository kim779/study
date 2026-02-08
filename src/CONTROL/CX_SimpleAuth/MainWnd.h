#if !defined(AFX_MAINWND_H__86223C65_68D3_4DAF_B4C7_371EF4AD7EA1__INCLUDED_)
#define AFX_MAINWND_H__86223C65_68D3_4DAF_B4C7_371EF4AD7EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	CWnd* m_pParent;
	_param m_param;
	int m_iType;  //0 금융거래 1:OTP(수동) 2:OTP(자동)
	CString m_root;

	CString m_strClientNum;	//고객번호
	CString m_strPinNumber;	//고객인 입력한 pin번호
	CString m_macaddr;
// Attributes
public:
	LPCTSTR Variant( int cmd, LPCTSTR data );
	void SetParam(_param *pParam);
	void	SearchOTP_MP(CString sUserNum);
	CString GetMacAddr();
	BOOL	SendTR(CString strName, char* pData, int nData, BYTE key);
	void	CertOTP_BP(CString sCustNum, CString spinNum);
// Operations
public:

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	CString m_eRRcode;
	afx_msg void OnERRcodeChanged();
	CString m_oTPsuccYN;
	afx_msg void OnOTPsuccYNChanged();
	CString m_oTPMaxErrCnt;
	afx_msg void OnOTPMaxErrCntChanged();
	CString m_oTPErrCnt;
	afx_msg void OnOTPErrCntChanged();
	short m_oTPState;
	afx_msg void OnOTPStateChanged();
	CString m_oTPSeqNo;
	afx_msg void OnOTPSeqNoChanged();
	CString m_eRRMsg;
	afx_msg void OnERRMsgChanged();
	afx_msg short CertOTP(LPCTSTR sCustNum, LPCTSTR spinNum);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	SHORT SearchOTP(BSTR custnum);

	enum
	{
		dispidSearchOTP = 9L
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__86223C65_68D3_4DAF_B4C7_371EF4AD7EA1__INCLUDED_)
