#pragma once
// GridWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridWnd window
#define BOX_NONE	0
#define BOX_LEFT	1
#define BOX_RIGHT	2
#define BOX_TOP		4
#define BOX_BOTTOM	8
#define BOX_ALL		(BOX_LEFT | BOX_RIGHT | BOX_TOP | BOX_BOTTOM)

class CGridWnd : public CWnd
{
// Construction
public:
	CGridWnd(CWnd* pView, CWnd* parent);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CIB410100App* m_pApp = nullptr;
	CWnd*	m_pParent = nullptr;
	CWnd*	m_pView = nullptr;

	struct _infogrid {
		CRect		rc;
		CString		text;
		COLORREF	bcolor{}, fcolor{};
		UINT		format{};
	};
	_infogrid m_infoGrid[20][2]{};

	COLORREF		m_cFgColor{}, m_cBgColor{}, m_hFgColor{}, m_hBgColor{}, m_gBgColor{}, m_bkColor{};
	CPen			*m_pPen=nullptr;
	CFont*			m_pFont{};
	int				m_nRow{};
	int				m_fcolWidth{};

public:
	void	SetColor();
	void	InitGrid(CRect gridRC);
	void	SetGridItemText(int row, int col, CString text);
	void	InitFormat();
	void	DrawGrid(CDC *pDC);
	void	DrawBox(CDC* pDC, COLORREF nOutLine, COLORREF nBkColor, CRect rect = CRect(-1, -1, -1, -1), UINT nLine = BOX_ALL, BOOL bFill = TRUE);
	void	SetText(CDC* pDC, CRect rect, CString strInput, COLORREF txColor, UINT fmt = DT_CENTER);

};

