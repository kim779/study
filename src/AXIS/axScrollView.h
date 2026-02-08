// axScrollView.h : interface of the CAxScrollView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXSCROLLVIEW_H__6994FC61_11B6_11D6_8AC5_00001CD7F9BE__INCLUDED_)
#define AFX_AXSCROLLVIEW_H__6994FC61_11B6_11D6_8AC5_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axScrollView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxScrollView view

class CAxScrollView : public CScrollView
{
protected:
	CAxScrollView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAxScrollView)
	
// Attributes
public:
	int		m_key{};
protected:
// Operations
public:
	class CAxisDoc* GetDocument();
	void	SetSizeView(int cx, int cy, bool nomove = false);
	void	SetInfo(int key);
	void	DrawEdge(CDC* pDC);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxScrollView)
	protected:
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAxScrollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAxScrollView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CResourceHelper* m_resourceHelper;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXSCROLLVIEW_H__6994FC61_11B6_11D6_8AC5_00001CD7F9BE__INCLUDED_)
