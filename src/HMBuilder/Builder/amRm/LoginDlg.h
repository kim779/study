#if !defined(AFX_LOGINDLG_H__INCLUDED___)
#define AFX_LOGINDLG_H__INCLUDED___

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	szBUFF	1024*2

// CLoginDlg 대화 상자입니다.

class CLoginDlg : public CDialog
{
//	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGIN };

protected:
	enum	{ sNONE, sCREATE, sOPEN } m_state;

	class	CwSock*	m_socket;
	char*	m_sendB;
	char*	m_recvB;

	struct	_rsmH*	m_SRH;
	struct	_rsmH*	m_RRH;
	struct _login* m_login;

	char	m_ips[256];
	int	m_port;
	CString m_guide;

	bool	sendData();
	void	close();
	int	toInt(char* pBytes, int plen);
	void	guideMsg(CString string);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeUserid();
	CString m_pswd;
	CString m_userid;
	bool	m_loginOK;

	virtual BOOL OnInitDialog();
	afx_msg void OnClickedLogin();

	afx_msg LRESULT OnSocketSession(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSocketData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_desc;
};

#endif	// AFX_LOGINDLG_H__INCLUDED___