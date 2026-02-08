#pragma once
// ControlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window


class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

	void	SetParam(struct _param* param);
	void	RsDataAddString(CString strWriteData, COLORREF crTextColor, int sSize);

// Attributes
public:
	CWnd*	m_pParent;
	struct	_param	m_param;
	enum { MODE_INOUT = 0, MODE_JUSTSHOW = 1 };
	int m_mode{};//0: 입출력모드  1:출력모드
	CFont* getAxFont(CString fName, int point, int style);
protected:
	CRichEditCtrl* m_Edit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void SetData(LPCTSTR strData);
	//}}AFX_DISPATCH
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};