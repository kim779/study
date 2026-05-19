#pragma once


// CUpload

class CUpload : public CWnd
{
public:
	CUpload(bool bExport = false);
	virtual ~CUpload();

	void	Upload(CString usid, CString path, int trkind, int kind, bool enable, struct _mapH *mapH);
	void	FormUploadSetting(CString strFormInfo);
	int	GetGubn(){ return m_gubn; }


protected:
	class	CwSock*	m_wsock;
	bool	m_export;

	struct	_mapH	*m_mapH;
	CString	m_path;
	CString m_usid;
	int		m_trkind;
	bool		m_done;
	enum	rKIND { W_SRC, W_BIN, W_NICK } m_kind;
	int		m_gubn;
	CString		m_ver1;
	CString		m_ver2;
	CString		m_desc;

	bool	initialize();
	bool	sendData();
	int	_sendData();
	void	finish();
	CString	setMapPath(CString mapN);

	afx_msg LRESULT OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenedSock(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


