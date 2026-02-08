#pragma once
// MapWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window
#define 	IDC_MAPVIEW1	5001
#define		IDC_MAPVIEW2	5002
#define		IDC_MAPVIEW3	5003
#define		IDC_MAPVIEW4	5004
#define		IDC_MAPVIEW5	5005
#define		IDC_MAPVIEW6	5006
#define		IDC_MAPVIEW7	5007
#define		IDC_MAPVIEW8	5008

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* pWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
	void closeMap();	
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWnd*	m_pViewWnd;
	int	m_mapKey;
	void loadMap(CString mapname, CString option = _T(""));	
public:
	void ChangeMap(CString mapname, CString option = _T(""));
	CString Variant(int comm, CString data = _T(""));
	void SendTrigger(CString code);

};

