#if !defined(AFX_AXTABVIEW_H__CC8851BC_F45C_4E2D_8678_9F1B89EA960F__INCLUDED_)
#define AFX_AXTABVIEW_H__CC8851BC_F45C_4E2D_8678_9F1B89EA960F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxTabView.h : header file
//
#define	TABH	20
/////////////////////////////////////////////////////////////////////////////
// CAxTabView view

class CAxTabView : public CView
{
protected:
	CAxTabView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAxTabView)

// Attributes
public:
	int		m_key;
	class _axisRes*	m_axisres;
	class CNTab*	m_tab;
// Operations
public:
	void	SizeWindow(CSize size);
	void	ChangeTab(CString aMapN);
	void	JustWindow(CString aMapN, bool resize = false);
	void	JustWindow(CString home, CString key, class CTMenu* menu, 
			CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM));
	void	ChangeSkin();
	void	SetInfo(int key, class _axisRes* axisres);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxTabView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAxTabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxTabView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXTABVIEW_H__CC8851BC_F45C_4E2D_8678_9F1B89EA960F__INCLUDED_)
