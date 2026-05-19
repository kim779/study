#pragma once

#include "h/axisrsm.h"
#define	DN_FILE	"resource.ini"
#define	TR_FILE	"trlayout.ini"

// CDownload

class CDownload : public CWnd
{
DECLARE_DYNCREATE(CDownload)
public:
	CDownload();
	virtual ~CDownload();
	bool	Download(CString usid, CString formname, CString svrPath, CString localPath, int trkind, int kind = 0, int id = 0, bool enable = true);

protected:
	class	CwSock*	m_sock;

	CString	m_svrPath;
	CString	m_localPath;
	CString m_formname;
	CString m_usid;
	CString m_sMsg;
	int	m_trkind;
	bool	m_done;
	bool	m_alive;
	int	m_kind;
	int	m_idno;
	int	m_percentage;

	bool	initialize();
	void	finish(bool close = false);
	void	sendData();
	void	setProgress(int percentage, int type = 1);

	afx_msg LRESULT OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenedSock(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


