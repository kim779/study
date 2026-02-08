/*
// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IControlWnd wrapper class

class IControlWnd : public COleDispatchDriver
{
public:
	IControlWnd() {}		// Calls COleDispatchDriver default constructor
	IControlWnd(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IControlWnd(const IControlWnd& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetSetAccount();
	void SetSetAccount(LPCTSTR);
	CString GetDataList();
	void SetDataList(LPCTSTR);
	CString GetFlag();
	void SetFlag(LPCTSTR);

// Operations
public:
	CString GetProperties();
	void SetProperties(LPCTSTR sProperties);
	void Send(LPCTSTR sAccn, LPCTSTR sPswd);
};
*/

/////////////////////////////////////////////////////////////////////////////
// CNotify window
#pragma once
#include "ControlWnd.h"

class CNotify : public CWnd
{
// Construction
public:
	CNotify(CWnd* sendW, int sendK);

// Attributes
public:
	CWnd*	m_sendW{};
	int		m_sendK{};
	int		m_saveK{};

	CWnd*		m_parent{};
	CString		m_root;

	HINSTANCE	m_hInst{};
	CWnd*		m_Wcontrol{};
//	IControlWnd*	m_Icontrol;

	class CControlWnd* m_pControlWnd{};

	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) {};

// Operations
public:
	BOOL	CreateNotify(CWnd* parent, CRect rect, CString root);

	void	SendJango(CString acc, CString pswd);
	CString	Parser(CString &srcstr, CString substr);
	void	SetCtrlWnd(class CControlWnd *pCtrl) { m_pControlWnd = pCtrl; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotify)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNotify();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNotify)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnSend(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};