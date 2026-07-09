#if !defined(AFX_LOADTHREAD_H__D8C3FA9D_3A40_45AE_BAA8_F85ECAFA9798__INCLUDED_)
#define AFX_LOADTHREAD_H__D8C3FA9D_3A40_45AE_BAA8_F85ECAFA9798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadThread.h : header file
//

#include "mapvar.h"
#include "awWcc/libwcc.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadThread thread

class CLoadThread : public CWinThread
{
	DECLARE_DYNCREATE(CLoadThread)
public:
	CLoadThread();           // protected constructor used by dynamic creation
	virtual ~CLoadThread();

// Attributes
public:
	CString		m_root, m_mapName, m_path, m_srcN, m_objN;
	CWnd*		m_pWnd;
	bool		m_running;
protected:
	CMapToken	m_token[MAX_TOKEN_N];
	CCriticalSection	m_section;
// Operations
public:
	void setParent(CWnd* pWnd);
	bool compile();
	bool open(CString fileN);
	void setMapName(CString mapName);
	void setRoot(CString sRoot);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADTHREAD_H__D8C3FA9D_3A40_45AE_BAA8_F85ECAFA9798__INCLUDED_)
