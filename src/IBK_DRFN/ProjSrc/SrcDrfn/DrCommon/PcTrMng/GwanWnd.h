#if !defined(AFX_GWANWND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
#define AFX_GWANWND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GwanWnd.h : header file
//

//#include "../../inc/Define_Gwansim.h"
#include "../../inc/IAUGwanMng.h"
/////////////////////////////////////////////////////////////////////////////
// CGwanWnd window

class CGwanWnd : public CWnd
{
// Construction
public:
	CGwanWnd();

// Attributes
public:
//	void*	m_pGwansimList;
	int m_nAdviseGwan;
	HWND m_this;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGwanWnd)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGwanWnd();
	//BOOL	Create(CWnd* pParentWnd);
	BOOL	OpenWindow();

	void	GwanAdvise();
	void	GwanUnAdvise(BOOL bClearAll=FALSE);

	void	ClearList(int nIndex);
	void	ClearAllList();

//	void	RequestIntAll();
//	void	RemoveAllList(void* pData);
// 	void	SetGwansimToPctr(WPARAM wParam, LPARAM lParam);
// 	void	CopyIntTotal(void* _pFrom, void* _pTo);
// 	void	CopyIntGroupSt(CPtrArray* pGroupFrom, CPtrArray* pGroupTo);
// 	void	CopyIntItemSt(CPtrArray* pFrom, CPtrArray* pTo);


	// Generated message map functions
protected:
	//{{AFX_MSG(CGwanWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT OnEugeneGwansim(WPARAM wParam, LPARAM lParam);

	//	afx_msg void OnTimer(UINT nIDEvent);
	//{{관심
	//	afx_msg	LRESULT	OnConcernHWnd(WPARAM wParam, LPARAM lParam);
	//	afx_msg	LRESULT	OnIntRequest(WPARAM wParam, LPARAM lParam);
	//}}관심
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GWANWND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
