#if !defined(AFX_RUSHMAN_H__7C84EEA9_DDD3_4605_90EF_2EA061C8EAFA__INCLUDED_)
#define AFX_RUSHMAN_H__7C84EEA9_DDD3_4605_90EF_2EA061C8EAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RushMan.h : header file
//

#include "Connector.h"

#define	WM_RUSHMAN_MSG	100

/////////////////////////////////////////////////////////////////////////////
// CRushMan window

class CRushMan : public CWnd
{
// Construction
public:
	CRushMan();

// Attributes
public:
	int			m_iCount;	
	int			m_iQueryGubn;
private:
	CString		m_sId;
	CString		m_sPwd;
	CString		m_suuid;
	CString		m_sOpt;	// 立加可记
	

	CString		m_sIp;
	long		m_nPort;
	long		m_nConnectCount;
	long		m_nConnectInterval;
	long		m_nQueryInterval;
	long		m_nResponseWaitTime;
	
	int		m_nAbsConnectorIdx;
	CPtrArray	m_arConnector;
	CPtrArray	m_arQueryData;

// Operations
public:
	void	SetUser(CString Id, CString Pwd, CString uuid);
	void	InitQuery(int QueryInterval, int ResponseWaitTime);
	void	StartConnect(CString Ip, int Port, int ConnectCount, int ConInterval, CString Opt);

	int	NewConnect(CString Opt);
	void	DisConnect(int idx);
	void	DisConnectAll();

	int	GetCount();
	int	GetLoginCount();
	int	GetAlertCount();
	int	GetAliveCount();
	CString Parser(CString &srcstr, CString substr);

	int	AbsToIdx(int abs);
	void	ClearQueryData();
	int	AddQuery(CString QueryFile);
	void Send(int icnt, int igubn, int iQueryInterval);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRushMan)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRushMan();

	// Generated message map functions
protected:
	LRESULT OnConnectorMsg(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CRushMan)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUSHMAN_H__7C84EEA9_DDD3_4605_90EF_2EA061C8EAFA__INCLUDED_)
