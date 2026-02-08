
#pragma once
// HogaWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHogaWindow window

class CHogaWindow : public CWnd
{
// Construction
public:
	CHogaWindow(CWnd* pParent = NULL);

// Attributes
public:
	CRect					m_pRect[15][3];

	CString					m_strRealCode;
	CString					m_astrHogaList[15][3];
	CString					m_astrSymbolList[15][3];
	int						m_iDataFormat[15][3]{};
	BOOL					m_bHogaFiags[15][3]{};

	int						m_iWidth{}, m_iHeight{};
	CWnd*					m_pMain;
	CFont					m_pFont;
protected:
	COLORREF				m_txtcolor{}, m_linecolor{}, m_lblcolor{}, m_conditioncolor[3]{}, m_lbltxtcolor{};

// Operations
public:
	int						Create(CRect rect, CWnd* parent, UINT nID) ;
	void					DrawGrid(CDC* pDC);
	void					DrawText(CDC* pDC);
	void					ReDraw();
	void					ReDraw(int irow, int icol);

	void					SetTextColor(COLORREF color){m_txtcolor=color;};
	void					SetLineColor(COLORREF color){m_linecolor=color;};
	void					SetLabelColor(COLORREF color){m_lblcolor=color;};
	void					SetLabelTxtColor(COLORREF color){m_lbltxtcolor=color;};
	void					SetConditionColor(COLORREF upcolor, COLORREF downcolor, COLORREF color){m_conditioncolor[0]=upcolor,m_conditioncolor[1]=downcolor,m_conditioncolor[2]=color;};
	COLORREF				GetTextFormat(CString& sData, int iFormat);



	void					Show(bool bshow, CRect rect=(0));




// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHogaWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHogaWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

