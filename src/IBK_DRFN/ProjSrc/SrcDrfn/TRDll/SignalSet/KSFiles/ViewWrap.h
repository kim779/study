#if !defined(AFX_VIEWWRAP_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_)
#define AFX_VIEWWRAP_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewWrap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewWrap window

// View를 감싸는 껍데기
// View를 생성, 삭제, 외부와의 인터페이스 기능
class CViewWrap : public CWnd
{
// Construction
public:
	CViewWrap();

// Attributes
public:
	CView*			m_pView;
	BOOL CreateView( CRuntimeClass * pViewClass );

  void AddViewForSingle();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewWrap)
	//}}AFX_VIRTUAL

//	void OnSetFormLinkEvent();
// Implementation
public:
	virtual ~CViewWrap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewWrap)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWWRAP_H__B6DA075C_AB3C_46E8_9EE4_C495518111C4__INCLUDED_)
