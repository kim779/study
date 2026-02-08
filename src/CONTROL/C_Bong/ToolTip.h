#if !defined(AFX_TOOLTIP_H__83E89C62_8C30_4F65_98F2_68CF7AA5D8A0__INCLUDED_)
#define AFX_TOOLTIP_H__83E89C62_8C30_4F65_98F2_68CF7AA5D8A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTip.h : header file
//

#define	JUGA_TOOLTIP_CLASS	"Bong ToolTip"
/////////////////////////////////////////////////////////////////////////////
// CToolTip window

class CToolTip : public CWnd
{
// Construction
public:
	CToolTip(CWnd *pView, CWnd *pGrpWnd);

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
	CWnd			*m_pView;
	class CGrpWnd		*m_pGrpWnd;
	int			m_nCellHeight;
	CRect			m_rcBase;
	CRect			m_rcDraw;
	COLORREF		m_clrBack;
	CFont			*m_font;	
	CStringArray		m_arTitle;
	CStringArray		m_arContent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIP_H__83E89C62_8C30_4F65_98F2_68CF7AA5D8A0__INCLUDED_)
