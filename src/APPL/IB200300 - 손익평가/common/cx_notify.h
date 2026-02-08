#pragma once
class IControlWnd : public COleDispatchDriver
{
public:
	IControlWnd() {}		// Calls COleDispatchDriver default constructor
	IControlWnd(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IControlWnd(const IControlWnd& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

public:
	CString	GetDataList();
	void	SetDataList(LPCTSTR);
	CString	GetFlag();
	void	SetFlag(LPCTSTR);
	BOOL	GetAllOption();
	void	SetAllOption(BOOL);
	void	Clear();
	void	SendX(LPCTSTR sAccn, LPCTSTR sPswd, LPCTSTR sPosition);
	void	SendGroup(LPCTSTR userS, LPCTSTR gIDS, LPCTSTR logPassS, LPCTSTR codeS);
};

/////////////////////////////////////////////////////////////////////////////
class CNotify : public CWnd
{
public:
	CNotify(CWnd* sendW, int sendK);
	virtual ~CNotify();
public:

	CWnd*		m_sendW;
	int		m_sendK;
	int		m_saveK;

	CWnd*		m_parent;
	CString		m_root;

	HINSTANCE	m_hInst;
	CWnd*		m_Wcontrol;
	IControlWnd*	m_Icontrol;

	CWnd*		(APIENTRY* axCreate)(CWnd*, void*);
	bool		m_bAll;

public:
	BOOL	CreateNotify(CWnd* parent, CRect rect, CString root, bool bAll);

	void	SendJango(CString acc, CString pswd, CString sPosition = "0");	// "1":碍技阂府,"2":距技阂府
	void	SendJango2(CString userS, CString gIDS, CString logPassS, CString codeS);
	void	Clear();
	void	SubSendMessage(WPARAM wParam, LPARAM lParam);
	//{{AFX_VIRTUAL(CNotify)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CNotify)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnSend(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

