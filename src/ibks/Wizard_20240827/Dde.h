#if !defined(AFX_DDE_H__F5EFA229_9D8C_4E81_83F2_BA17D1D6C59E__INCLUDED_)
#define AFX_DDE_H__F5EFA229_9D8C_4E81_83F2_BA17D1D6C59E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dde.h : header file
//
#include <ddeml.h>

#define	delDS		0x0001
#define	addDS		0x0002
#define	jobDS		0x0004

class CPost : public CObject
{
public:
	CPost()		{ m_key = m_stat = 0; m_item.Empty(); }

public:
	int		m_key;
	int		m_stat;
	CString		m_item;
};

/////////////////////////////////////////////////////////////////////////////
// CDde window

class CDde : public CWnd
{
// Construction
public:
	CDde(CGuard* guard);

// Attributes
public:
	DWORD		m_inst;
	int		m_conv;
	HSZ		m_service, m_topic;
	enum class flagDDE {readyDDE, startDDE, stopDDE, exitDDE} m_dde;

protected:
	class CGuard*	m_guard;
	CString		m_profile;
	bool		m_posting;
	int		m_minor;

	CCriticalSection	m_sync;
	CStringArray		m_que;
	CMapStringToString	m_item;

	CCriticalSection	m_lock;
	CMapStringToOb		m_post;
	CMapStringToString	m_keys;

// Operations
protected:
	void	OnAlert(CString text);
	void	OnError(int key);
	void	Service();
	bool	MakeKey(int& key);
	BOOL	Write(CString text, int key, bool dde);

public:
	void	OnService(CString text, int key);
	void	OnAlert(CString topic, CString item);
	void	OnAdvise(HSZ hItem, bool advise);
	void	OnExit();
	HDDEDATA OnAdvise(HSZ hItem);
	void	OnDDE();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDde)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDde();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDde)
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
HDDEDATA CALLBACK DdeCallback(UINT uType, UINT uFmt, HCONV hConv, HSZ hTopic, HSZ hItem,
					HDDEDATA hData, DWORD dwData1, DWORD dwData2);

#endif // !defined(AFX_DDE_H__F5EFA229_9D8C_4E81_83F2_BA17D1D6C59E__INCLUDED_)
