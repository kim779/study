#pragma once
// MainWnd.h : header file
//

#include "MapWnd.h"
#include "Label.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

#define MAP_WIDTH						1920
#define MAP_HEIGHT						1080

#define URL			"http://www.kma.go.kr/wid/queryDFS.jsp?gridx=59&gridy=127"

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	CMainWnd(CWnd *parent);

// Attributes
public:

// Operations
public:
	CWnd* m_pWizard = nullptr;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();


	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	int	m_mapKey{};
	int	m_timer{};
	CString Variant(int nComm, CString strData = _T("") );
	void	init_map();
	void	set_timer(char *timer);
	bool	get_xml();
	CString	m_clock;
	CRect  m_rectchart_1, m_rectchart_2, m_rectchart_3;
public:


private:
	std::unique_ptr<class	CMapWnd> m_pMapWnd1 = nullptr;
	std::unique_ptr<class	CMapWnd> m_pMapWnd2 = nullptr;
	std::unique_ptr<class	CMapWnd> m_pMapWnd3 = nullptr;
	std::unique_ptr<class	CMapWnd> m_pMapWnd4 = nullptr;
	std::unique_ptr<class	CMapWnd> m_pMapWnd5 = nullptr;
	std::unique_ptr<class	CMapWnd> m_pMapWnd6 = nullptr;
	std::unique_ptr<class	CMapWnd> m_pMapWnd7 = nullptr;


	std::unique_ptr<class	CMapWnd> m_pMapWnd8 = nullptr;


	std::unique_ptr<CLabel> m_label = nullptr;
	std::unique_ptr<CLabel> m_label2 = nullptr;
	
};

