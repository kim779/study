// axisView.h : interface of the CAxisView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXISVIEW_H__4B7EBFA1_DA68_41CD_9BC9_9366BC768BCC__INCLUDED_)
#define AFX_AXISVIEW_H__4B7EBFA1_DA68_41CD_9BC9_9366BC768BCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "useMaindefine.h"
class CAxisView : public CView
{
protected: // create from serialization only
	CAxisView();
	DECLARE_DYNCREATE(CAxisView)

// Attributes
public:
	class CAxisDoc* GetDocument();

	BOOL		m_bBkNotice;
	HCURSOR		m_hCursor;
	HCURSOR		m_hCursorHand;
#ifdef DF_USE_CPLUS17
	std::map<CString, std::shared_ptr<CRect>> m_mapCursorMap;
#else
	CMapStringToPtr	m_mapCursorMap;
#endif
	BOOL		m_bTracking;
	
	int	m_key;
// Operations
public:
	void	SetInfo(int key);
	void	DrawEdge(CDC* pDC);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	void RemoveAllMouseMap();
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
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);  //vc2019 UINT->LRESULT
	//}}AFX_MSG
	afx_msg LRESULT OnSetCursorMap(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CResourceHelper* m_resourceHelper;
};

#ifndef _DEBUG  // debug version in axisView.cpp
inline CAxisDoc* CAxisView::GetDocument()
   { return (CAxisDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISVIEW_H__4B7EBFA1_DA68_41CD_9BC9_9366BC768BCC__INCLUDED_)
