#if !defined(AFX_LOGIN_H__D48B9564_5019_479B_8851_69B505F80976__INCLUDED_)
#define AFX_LOGIN_H__D48B9564_5019_479B_8851_69B505F80976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// login.h : header file
//
#include <AFXTEMPL.H>

class CSortStringArray : public CStringArray { public: 
void Sort(); 

private: 
    BOOL CompareAndSwap(int pos); 
    
}; 

struct	_AdapterInfo{
	char	sMac[13];
	int		iType;
};

typedef CArray <_AdapterInfo, _AdapterInfo&> CArrayAdpInfo;

/////////////////////////////////////////////////////////////////////////////
// CLogin command target

class CLogin : public CCmdTarget
{
	DECLARE_DYNCREATE(CLogin)

	CLogin();           // protected constructor used by dynamic creation
	virtual ~CLogin();

// Attributes
public:
	CString	m_returns;
	CString	m_strRegKey;

	CString	m_user;
	CString	m_name;
	CString	m_term;
	
	CString m_auth;    //권한
	CString m_comGb;	// 화사코드
	CString m_upGb;	// 업종
	CString	m_bExpert;    //전문투자자여부
	CString	m_bPassChange;		 //비밀번호변경
	CString	m_bSameConnect;		// 동시접속여부 
	CString	m_btmpuse;	// 임시사용여부 (사용자고유키 초기화 요청 또는 회사변경 요청시 당일 임시사용 여부)
	CString	m_bUserst;	// 이용제한요청여부 (이용제한요청되었으나 승인이 안되었을 경우)
	CString	m_deptNm;	// 부서명
	CString	m_bBondEx;		//채권전문딜러여부
	CString m_bemailfrm;  //이메일 activate 여부
	CString	m_bnotice;   //공지팝업 존재 유무
	CString	m_buuidrec;  //사용자 고유키 재생성 여부
	CString	m_reserve;   //예비용
	CString	m_errc;      //에러코드
	CString	m_ldate;     //last날짜 
	CString	m_ltime;	// last 시간
	CString m_errmsg;
	CString m_strcompNm;   //회사명

// Operationsㅁ
public:
	CString	SetLogin(char* data);
	CString	GetLogin(int id);
	CString	IpccEtcdata(int nkind, CString sfind, CString svalue);
	void	GetMacAddr(CString& data);
	void	OnOpenUrl(CString strIP);
	void	WriteReg(CString strRegKey, CString sSubKey);
	void	SendDataToMain(char* pdata);
	HWND	GetWinHandle(ULONG pid);
	ULONG	ProcIDFromWnd(HWND hwnd);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	clear();
	void	setDefault(char* data);
	

	// Generated message map functions
	//{{AFX_MSG(CLogin)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLogin)
	afx_msg BOOL _getOrder();
	afx_msg BSTR _getUsid();
	afx_msg BSTR _getUsnm();
	afx_msg BSTR _getbExpert();
	afx_msg BSTR _getTerm();
	afx_msg BSTR _getbBondEx();
	afx_msg BSTR _getbPassChange();
	afx_msg BSTR _getbSameConnect();
	afx_msg BSTR _getbtmpuse();
	afx_msg BSTR _getbUserst();
	afx_msg BSTR _getdeptNm();
	afx_msg BSTR _getbnotice();
	afx_msg BSTR _getbuuidrec();
	afx_msg BSTR _getreserve();
	afx_msg BSTR _getm_errc();
	afx_msg BSTR _getldate();
	afx_msg BSTR _getDef2();
	afx_msg BSTR _GetcomGb();
	afx_msg BSTR _GetupGb();
	afx_msg BSTR _GetChk();
	afx_msg BSTR _GetSvnm();
	afx_msg BSTR _getlComm();
	afx_msg BSTR _getllDate();
	afx_msg BSTR _getltime();
	afx_msg BSTR _getToday();
	afx_msg BSTR _getManage();
	afx_msg BSTR _getAdmin();
	afx_msg BSTR _getSupport();
	afx_msg BSTR _getManagerespon();
	afx_msg BSTR _getAdminrespon();
	afx_msg BSTR _getInquir();
	afx_msg BSTR _WrapWhoi();
	afx_msg BSTR _getBlackUserTime();
	afx_msg BSTR _getBlackUser();
	afx_msg BSTR _getCallLevel();
	afx_msg BSTR _getIPCCInOut();
	afx_msg BSTR _getIPCCFax();
	afx_msg BSTR _getIPCCSms();
	afx_msg BSTR _getIPCCEmail();
	afx_msg BSTR _getItem();
	afx_msg BSTR _getStoreGb();
	afx_msg BSTR _getMacAddr();
	afx_msg BSTR GetConnArea();
	afx_msg BSTR GetDeptTP();
	afx_msg BSTR GetBsnsTP();
	afx_msg BSTR GetDeptTel();
	afx_msg BSTR GetApprYN();
	afx_msg BSTR GetChannel();
	afx_msg BSTR GetCif();
	afx_msg void _ChangeInfo(LPCTSTR strMap, LPCTSTR strKind, LPCTSTR strValue);
	afx_msg void _WriteReg();
	afx_msg BSTR Getauth(void);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	

	enum 
	{
		dispidcompNm = 53,
		dispidauth = 52
	};
	BSTR GetcompNm(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__D48B9564_5019_479B_8851_69B505F80976__INCLUDED_)
