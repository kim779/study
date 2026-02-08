#if !defined(AFX_CONDITIONVIEW_H__A5C3C4EF_E20A_4C4B_A5EC_C3D31DC1A1AA__INCLUDED_)
#define AFX_CONDITIONVIEW_H__A5C3C4EF_E20A_4C4B_A5EC_C3D31DC1A1AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConditionView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConditionView view

#include "ConditionMakeWnd.h"
#include "ConditionResultWnd.h"
class CConditionView : public CView
{
protected:
	CConditionView();           // protected constructor used by dynamic creation
  virtual ~CConditionView();
	DECLARE_DYNCREATE(CConditionView)

public:
	COLORREF	m_clrMainSkin;
  CConditionMakeWnd		*m_pwndConditionMake;
	CConditionResultWnd	*m_pwndConditionResult;

// Operations
public:
  void SetInit();
  void SetTopExpend(int nState);
//  void ChangeSkinColor(COLORREF clrSkin);
  void ChangeSkinColor(COLORREF clrSkin, ST_TABCOLOR stTabColor);
  void OnExtendLigcEdit(BOOL bExtendCommonLogicEdit, int nTapSelect);

private : 
  BOOL m_bInitFlag;
  BOOL m_bTopExp;
  int  m_nTopExp;

  BOOL m_bExtendCommonLogicEdit;
  int  m_nTapSelect;

  void OnSize();
  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONVIEW_H__A5C3C4EF_E20A_4C4B_A5EC_C3D31DC1A1AA__INCLUDED_)
