#if !defined(AFX_ONTIMER_H__554E913E_BED2_4D57_BAE9_62E0475A4A40__INCLUDED_)
#define AFX_ONTIMER_H__554E913E_BED2_4D57_BAE9_62E0475A4A40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnTimer.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// COnTimer thread

class COnTimer : public CWinThread
{
	DECLARE_DYNCREATE(COnTimer)

public:
	COnTimer();           // protected constructor used by dynamic creation
	virtual ~COnTimer();

// Attributes
private:
	bool	m_alive;
	CEvent		m_event;
	CDWordArray	m_que;

	CCriticalSection m_section;
	CMap	< int, int, int, int& > m_keys;
	CWnd*	m_view;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnTimer)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
public:
	void	Dispatch(int key);
	void	Startup(CWnd* view);
	void	Cleanup(bool exit = false);
	void	SetEvent()			{ m_event.SetEvent(); }

protected:
	bool	DoParse(int key);

	// Generated message map functions
	//{{AFX_MSG(COnTimer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONTIMER_H__554E913E_BED2_4D57_BAE9_62E0475A4A40__INCLUDED_)
