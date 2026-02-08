#pragma once
// ToolTip.h : header file
//

#define	JUGA_TOOLTIP_CLASS	"Bong ToolTip"
/////////////////////////////////////////////////////////////////////////////
// CToolTip window

class CToolTip : public CWnd
{
// Construction
public:
	CToolTip(CWnd *pView, CWnd *pGrpWnd, CFont* pFont);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTip)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTip)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int	Create();
	CRect	SetData(CString sData);

private:
	CSize	CalWndSize(CString strExam);
	CRect	MakeSizeWnd();
	void	DrawTip(CDC *pDC);

private:	
	CWnd*			m_pView{};
	class CStgInfo* m_pWnd{};
	int				m_nCellHeight{};
	CRect			m_rcBase;
	CRect			m_rcDraw;
	COLORREF		m_clrBack{};
	CFont			* m_font{};
	CStringArray		m_arTitle;
	CStringArray		m_arContent;
};

