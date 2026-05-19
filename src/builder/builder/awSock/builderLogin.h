#pragma once

#include "../../h/axisrsm.h"

#define	DN_FILE		"resource.ini"

// CDownload

class AFX_EXT_CLASS CbuilderLogin : public CWnd
{
	DECLARE_DYNAMIC(CbuilderLogin)

public:
	CbuilderLogin(CString UserID, CString pass, CString domain);
	virtual ~CbuilderLogin();

	bool	Create();
//	bool	Download(CString svrPath, CString localPath, int kind = 0, bool enable = true);
//
protected:
	class	CwSock*	m_sock;
	struct	_rsmH	m_sndH;
	struct	_rsmH*	m_rcvH;
	CString m_UserID;
	CString m_pass;
	CString m_domain;
//
//	CString	m_svrPath;
//	CString	m_localPath;
	bool	m_done;
	bool	m_alive;
//	int	m_kind;
//	int	m_percentage;
//
	bool	initialize();
	BOOL	LoginStart();
	void	finish(bool close = false);
//	void	sendData();
//	void	setProgress(int percentage, int type = 1);
//
	afx_msg LRESULT OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenedSock(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


