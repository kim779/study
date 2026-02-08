#pragma once
// ControlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CStatic
{
// Construction
public:
	CControlWnd();

// Attributes
public:
	int	m_iControlType{}, m_iLineType{};
	int	m_iLineColorIdx{};
	long	m_iLineColor{};
	

// Operations
public:
// Operations
public:
	void			SetParam(struct	_param* param, CWnd *parent);

protected:
	CWnd* m_pMainWindow{};
	CString			m_strImagePath;
// Operations
protected:

	COLORREF getAxColor(int color);
	HCURSOR getAxCursor(int ikind);
	CFont* getAxFont(CString fName, int point, bool bBold, bool bItalic);
	CFont* getAxFont(struct _fontR* font);

	HBITMAP getBitmap(CString path, int idir);
	CPen* getAxPen(COLORREF clr, int width, int style);
	CString GetFindOptionString(CString sData, CString sKey, CString sEndChar);

	CString Variant(int comm, CString data);
	LRESULT SendAxisMessage(WPARAM wParam, LPARAM lPAram);

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
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	LONG OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH

	

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	void ShowLine(SHORT bShow);

	enum
	{
		dispidShowLine = 1L
	};
};