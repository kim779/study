#pragma once
// XPButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXPButton window

class CXPButton : public CButton
{
	DECLARE_DYNCREATE(CXPButton)
// Construction
public:
	CXPButton();
	CPen pBoundryPen;
	CPen pInsideBoundryPenLeft;
	CPen pInsideBoundryPenTop;
	CPen pInsideBoundryPenRight;
	CPen pInsideBoundryPenBottom;

	CPen pInsideBoundryPenLeftSel;
	CPen pInsideBoundryPenTopSel;
	CPen pInsideBoundryPenRightSel;
	CPen pInsideBoundryPenBottomSel;

	CPen* pOldPen{};
	CBrush pFillActive{};
	CBrush pFillInactive{};
	CBrush* pOldBrush{};
	BOOL m_bOverControl;
	BOOL m_bTracking;
	BOOL m_bSelected;
	CString strCaption;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXPButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Focus;
	LRESULT afx_msg OnMouseLeave(WPARAM wparam, LPARAM lparam);
	LRESULT afx_msg OnMouseHover(WPARAM wparam, LPARAM lparam);
	void afx_msg OnMouseMove(UINT nFlags, CPoint point);
	void DrawInsideBorder(CDC *pDC,CRect rect);
	void DoGradientFill(CDC *pDC,CRect rect);
	virtual ~CXPButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXPButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

