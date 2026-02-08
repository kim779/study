#if !defined(AFX_AXTABVIEW_H__CC8851BC_F45C_4E2D_8678_9F1B89EA960F__INCLUDED_)
#define AFX_AXTABVIEW_H__CC8851BC_F45C_4E2D_8678_9F1B89EA960F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxTabView.h : header file
//
#define	TABH	29 //28  
/////////////////////////////////////////////////////////////////////////////
// CAxTabView view
#include "useMaindefine.h"
class CAxTabView : public CView
{
protected:
	CAxTabView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAxTabView)

// Attributes
public:
	int		m_key{};
#ifdef DF_USE_CPLUS17
	std::unique_ptr<class CNTab> m_tab;
#else
	class CNTab*	m_tab;
#endif
// Operations
public:
	void	SizeWindow(CSize size);
	void	ChangeTab(CString aMapN);
	void	JustWindow(CString aMapN, bool resize = false);
	void	JustWindow(CString home, CString key, class CTMenu* menu, 
			CString aMapN, bool (*axiscall)(int, WPARAM, LPARAM), bool resize = false);
	void	ChangeSkin();
	void	SetInfo(int key);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxTabView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CResourceHelper* m_resourceHelper;
	CString m_mapN;
public:
	BOOL IsControlKey(UINT key);
	void JustView(bool resize = false);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXTABVIEW_H__CC8851BC_F45C_4E2D_8678_9F1B89EA960F__INCLUDED_)
