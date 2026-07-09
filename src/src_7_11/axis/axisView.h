// axisView.h : interface of the CAxisView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXISVIEW_H__4B7EBFA1_DA68_41CD_9BC9_9366BC768BCC__INCLUDED_)
#define AFX_AXISVIEW_H__4B7EBFA1_DA68_41CD_9BC9_9366BC768BCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAxisView : public CView
{
protected: // create from serialization only
	CAxisView();
	DECLARE_DYNCREATE(CAxisView)

// Attributes
public:
	CAxisDoc* GetDocument();

	int	m_key;
	class _axisRes*	m_axisres;
// Operations
public:
	void	SetInfo(int key, class _axisRes* axisres);
	void	DrawEdge(CDC* pDC);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAxisView)
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in axisView.cpp
inline CAxisDoc* CAxisView::GetDocument()
   { return (CAxisDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISVIEW_H__4B7EBFA1_DA68_41CD_9BC9_9366BC768BCC__INCLUDED_)
