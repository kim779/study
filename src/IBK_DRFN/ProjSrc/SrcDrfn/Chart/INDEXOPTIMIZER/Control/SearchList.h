#if !defined(AFX_SEARCHLIST_H__208BD714_D844_4881_8901_DD967E2A2553__INCLUDED_)
#define AFX_SEARCHLIST_H__208BD714_D844_4881_8901_DD967E2A2553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchList window
//#include "MylistCtrl.h"
#include "SearchListCtrl.h"
#include "SortListCtrl.h"
class CSearchList : public CWnd
{
// Construction
public:
	CSearchList();

// Attributes
public:
//	CSearchListCtrl	m_ListCtrl;
	CSortListCtrl m_ListCtrl;
	CWnd*		m_pParent;
	CRect		m_parentRect;
	CRect		m_crClientRect;
// Operations
public:
	BOOL		Create(CRect rc, CWnd* pParentWnd, 	
				LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:	
	void FP_vEventSeachList(CString csData);
	CString FP_csEditEnter();
	void Selection(int index);
	void SetSelection(CString str, int Column);
	BOOL SortTextItems(int nCol, BOOL bAscending,int low = 0, int high = -1);
	void FP_vEventUpDown(UINT nKey);
//	void FP_vAttachList(CMyListCtrl* list);
	void FP_vAttachList(CListCtrl* list);
	void FP_vShowList(BOOL bShow);
	virtual ~CSearchList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSearchList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHLIST_H__208BD714_D844_4881_8901_DD967E2A2553__INCLUDED_)
