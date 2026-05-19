#pragma once

#include "../../h/axisrsm.h"

#define	DN_FILE		"resource.ini"
#define	TR_FILE		"trlayout.ini"

// CDownload

class AFX_EXT_CLASS CDownload : public CWnd
{
	DECLARE_DYNAMIC(CDownload)

public:
	CDownload();
	virtual ~CDownload();

	bool	Create();
	//bool	Download(CString svrPath, CString localPath, int kind = 0, bool enable = true);
	bool	Download(CString usid, CString formname, CString svrPath, CString localPath, int trkind, int kind = 0, int id = -1, bool enable = true, char fileType = 'S');
	void	setHistoryData(char Type = ' ', char dirt = ' ', char* seqn = NULL);
	void	setSearchData(const CStringArray& arr);

protected:
	class	CwSock*	m_sock;
	struct	_rsmH	m_sndH;
	struct	_rsmH*	m_rcvH;

	CString	m_svrPath;
	CString	m_localPath;
	CString m_formname;
	CString m_usid;
	int	m_trkind;
	bool	m_done;
	bool	m_alive;
	int	m_kind;
	int	m_idno;
	int	m_percentage;
	char	m_Type;
	char	m_dirt;
	char	m_seqn[8];
	char*	m_SearchData;
	int	m_iSize;
	char	m_fileType;

	bool	initialize();
	void	finish(bool close = false);
	void	sendData();
	void	setProgress(int percentage, int type = 1);

	afx_msg LRESULT OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenedSock(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


