#pragma once


// CUpload

class AFX_EXT_CLASS CUpload : public CWnd
{
	DECLARE_DYNAMIC(CUpload)

public:
	CUpload(bool bExport = false);
	virtual ~CUpload();

	void	Upload(CString path, int kind, bool enable, class mapForm *mapForm);

protected:
	class	CwSock*	m_wsock;
	bool	m_export;

	class	mapForm	*m_mapH;
	CString	m_path;
	bool	m_done;
	int	m_offset;
	enum	rKIND { W_SRC, W_BIN, W_NICK } m_kind;

	bool	initialize();
	bool	sendData();
	int	makeData(char* sndb);
	void	finish();
	CString	setMapPath(CString mapN);

	afx_msg LRESULT OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpenedSock(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


