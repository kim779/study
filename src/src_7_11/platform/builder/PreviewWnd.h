#if !defined(AFX_PREVIEWWND_H__7269F17C_C167_44AE_9EEB_F69B653F8A93__INCLUDED_)
#define AFX_PREVIEWWND_H__7269F17C_C167_44AE_9EEB_F69B653F8A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd view

class CPreviewWnd : public CView
{
public:
	CPreviewWnd();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPreviewWnd)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewWnd)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreviewWnd();
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWWND_H__7269F17C_C167_44AE_9EEB_F69B653F8A93__INCLUDED_)
