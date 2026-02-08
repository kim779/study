#if !defined(AFX_TOOLTIP_H__83E89C62_8C30_4F65_98F2_68CF7AA5D8A0__INCLUDED_)
#define AFX_TOOLTIP_H__83E89C62_8C30_4F65_98F2_68CF7AA5D8A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTip.h : header file
//

#define	MAIN_TOOLTIP_CLASS	"Main ToolTip"
/////////////////////////////////////////////////////////////////////////////
// CToolTip window

typedef struct {
	CString datetime;
	CString alias;
	HBITMAP bmp;
	CString description;
} _PATTERN;

class CToolTip : public CWnd
{
// Construction
public:
	CToolTip(CWnd *pView, class CObjEvent *pObjEvent);

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

private:	
	CWnd*			m_pwndView;
	class CObjEvent*	m_pObjEvent;
	int			m_iCellHeight;
	int			m_iMaxWidthTitle;
//	CRect			m_rcBase;
	CRect			m_rcDraw;
	COLORREF		m_crBack;
	CFont*			m_pfont;	
	CStringArray		m_arTitle;
	CStringArray		m_arContent;

	CString			m_text;
	_PATTERN		m_patternInfo;
	enum	{ MODE_DATA = 0, MODE_TEXT, MODE_PATTERN } m_mode;
public:
	int	Create();
	CRect	SetData(CString sData);
	CSize	SetText(CString sText);
	CSize	SetPattern(LPCTSTR datetime, LPCTSTR alias, LPCTSTR imageName, LPCTSTR description);
	void	WindowMove(int x, int y, int cx, int cy);

private:
	CSize	CalWndSize(CString strExam);
	CRect	MakeSizeWnd();
	void	DrawTip(CDC *pDC);
	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTip)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIP_H__83E89C62_8C30_4F65_98F2_68CF7AA5D8A0__INCLUDED_)
