#pragma once
// AxisWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxisWnd window
class CControlWnd;

class CAxisWnd : public CWnd
{
// Construction
public:
	CAxisWnd();

// Attributes
public:
	CString		m_strCode;
	std::unique_ptr<CControlWnd>	m_pControl;
	struct	_param	m_param {};
	CWnd*		m_pAxisWnd;

// Operations
public:
	void	SetParam(const struct _param* param, CWnd *parent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxisWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxisWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LONG OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CAxisWnd)
	afx_msg void Send(LPCTSTR sCode);
	afx_msg void Clear();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};