#pragma once

#include"Crypt.h"

// CControlwnd
class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

struct mid
{
	char filepath[300];
	char filename[300];
	char uuid[300];  
	BYTE data[40000];
	char len[19];
	char cnt[19];
//	char next;
};
/*
struct mod
{
	char type;
	char next;
	char filename[30];
	BYTE data[40000];
};
*/

struct mid_down
{
	char filepath[300];
	char filename[300];
	char uuid[300];
	char cnt[19];
};

struct mod_down
{
	char next;
	char  len[19];
	char  totalLen[19];
	BYTE data[40000];
	
};

class CControlwnd : public CWnd
{
	DECLARE_DYNAMIC(CControlwnd)

public:
	CControlwnd();
	virtual ~CControlwnd();

	virtual void OnFinalRelease();

	BOOL		m_bDown, m_bHover, m_bTracking;
	enum	    _MouseSate {dispidGetPercent = 5L,
		dispidDownFile = 4L,
		dispidUploadFile = 3L,
		dispidSetCmPass = 2L,
		dispidSetType = 1L,
		dispidSetdata = 1L,
		MsNo = 0, MsOver, MsDown} m_MouseState;

	char*		m_pdata;
	BYTE*		m_decB;
	Crypt*       m_pcrypt;   
	CWnd*		m_pWizard;
	CWnd*		m_wnd;
	struct mid_down* m_pmid;

	BYTE*		m_plastdata;	
	CParam		m_Param;
	int			m_type;
	int			m_totalSendCnt;
	CRect		m_btnRect;
	long		m_itotal;
	bool		m_bSending;
	int			m_cnt;

	CBitmap		*m_pBitmap[3];

	CBitmap* GetBitmap(CString path);

	CString		m_strResult;
	CString		m_strdownCnt;
	CString	    m_strDownPath;
	CString     m_strComPath;
	CString     m_strFilename;
	CString		m_fileN;
	CString		m_struuid;
	CString     m_strlog;
	CString     m_stropen;

	CStringArray m_upArr;

	CString		m_strHome;
	CStringArray	m_lenArr;
	CPtrArray	m_pDataArr;
	CPtrArray parr;
	CStringArray lenarr; 

	CPtrArray	m_pReDataArr;
	CStringArray	m_relenArr;

	CString Parser(CString &srcstr, CString substr);
	void	SendTR();
	void	SetParam(struct _param *pParam);
	void	DrawBitmap(CDC *pDC, CRect rc, CBitmap *pBitmap);
	void	*Getparam();
	void	GetledgerH(struct _ledgerH* pLedgerH);
	void	GetledgerHDown(struct _ledgerH* pLedgerH);
	bool	sendTR(CString trC, char* pBytes, int nBytes, bool bup = true);
	//void	FileUpload(CString cmpath, CString strpath);
	void	FileUpload(CString cmpath,CString strpath, CString uuid);
	void	Downloadfile(CString strpath);
	void    DownFile(LPCTSTR path, LPCTSTR filename, LPCTSTR uuid, LPCTSTR bopen);

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	void SetType(LPCTSTR type);
	void SetCmPass(LPCTSTR path);
	//void UploadFile(LPCTSTR path,LPCTSTR file);
	void UploadFile(LPCTSTR path,LPCTSTR file, LPCTSTR struuid);
//	void DownFile(LPCTSTR path, LPCTSTR filename);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	BSTR GetPercent(void);
};


