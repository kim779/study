#pragma once
// DllCtrl.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDllCtrl window

class CDllCtrl : public CWnd
{
// Construction
public:
	CDllCtrl(class CMapWnd *pParent);

// Attributes
public:

protected:
	class CMapWnd *m_pMapWnd;

	int m_nkey;				//load«— ∏ ¿« key
	CWnd *m_pScreen;

	int m_nHeight;
	CFont *m_pFont;

	BOOL		m_bCreated;

	CString m_strName, m_strSendMsg;

// Operations
public:
	void LoadMap(CString strMapN, CString strName);
	void SetScreenParam(CString strParam, CString strData);
	int GetMapHeight();

protected:
	void CloseMap();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDllCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDllCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

