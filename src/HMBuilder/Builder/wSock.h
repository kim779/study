#pragma once
#include "stdafx.h"

#define WM_SOCK WM_USER + 701
#define WM_FIRE WM_USER + 702
// CMSocketCtrl

#define WRK_PORT	17000

class AFX_EXT_CLASS CwSock : public CWnd
{
	DECLARE_DYNAMIC(CwSock)
public:
	CwSock(CWnd* pWnd);
	virtual ~CwSock();

public:
	char*		m_recvB;
	bool		m_alive;
	CString		m_rtnStr;
	CString		m_root;
	CString		m_sIP;
	int			m_Port;

	void	Login(CString sUser, CString sPass);
	bool	Open(CString ips = _T(""), int port = -1);
	BOOL	Write(char* pBytes, int nBytes);
	void	Close();
	void	SetWnd(CWnd *pWnd){ m_wnd = pWnd; }; 

// Dispatch and event IDs
public:
private:
	CWnd* m_wnd;
	CWnd* m_pSock;
protected:
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnFireEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireAlert(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFirePush(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireAxis(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireStat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireWrite(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFireVersion(WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();
};


