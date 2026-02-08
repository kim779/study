// SystemTradingView.h : interface of the CSystemTradingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMTRADINGVIEW_H__345185DF_2697_4BF4_A583_F56B1EE0EF4D__INCLUDED_)
#define AFX_SYSTEMTRADINGVIEW_H__345185DF_2697_4BF4_A583_F56B1EE0EF4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CrystalEditView.h"

class CSystemTradingDoc;
class CSystemTradingView : public CCrystalEditView
{
protected: // create from serialization only
	CSystemTradingView();
	DECLARE_DYNCREATE(CSystemTradingView)

// Attributes
public:

// Operations
public:
	virtual CCrystalTextBuffer *LocateTextBuffer();
	CSystemTradingDoc* GetDocument();

	void	GetDefualt();
	void	GetDefualt_YES();
	void	AppendLine(LPTSTR szText);
	void	SetLineSelect(UINT nLine);
	void	SetLineSelect(UINT nLine, UINT nColumn);
	void	ClearAllExecution();
	void	ChangeScriptType(UINT nScriptType, BOOL bRedraw);
	void	SetSystemTradingColor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSystemTradingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual DWORD ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);
	virtual DWORD ParseLineCPP(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);
	virtual DWORD ParseLineSTS(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);
	virtual DWORD ParseLineYES(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);

private:
	BOOL	m_bRButtonDown;
	BOOL	m_bExecutionExist;

// Generated message map functions
protected:
	//{{AFX_MSG(CSystemTradingView)
	afx_msg void OnUpdateEditFind(CCmdUI* pCmd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	void OnEditFind(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SystemTradingView.cpp
inline CSystemTradingDoc* CSystemTradingView::GetDocument()
   { return (CSystemTradingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGVIEW_H__345185DF_2697_4BF4_A583_F56B1EE0EF4D__INCLUDED_)
