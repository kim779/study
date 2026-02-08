#pragma once
// MapWnd.h : header file
//

#include "NotifyCtrl.h"
#include "AxisExt.h"

#define	DELWND(w)	{ if(w) {w->DestroyWindow(); delete w; w=NULL;} }

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd, public CAxisExt
{
// Construction
public:
	CMapWnd(CWnd *pParent);

// Attributes
public:
	CWnd * m_pParent;

private:
	std::unique_ptr<class CNotifyCtrl>		m_pSNotify;		//주식용
	std::unique_ptr<class CNotifyCtrl>		m_pCNotify;		//선물옵션용
	HWND m_MainWnd;
	bool	m_bType;		//옵션인지 주식인지


// Operations
public:
	BOOL	CreateMap(CWnd *pParent);
	void	JangoSend(LPCTSTR acno, LPCTSTR pswd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnAccount(WPARAM wParam, LPARAM lParam);
	afx_msg long OnAppSignal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

