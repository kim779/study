#if !defined(AFX_INPUTCONTROL_H__DEE41104_5449_11D2_865F_0000C061C6E2__INCLUDED_)
#define AFX_INPUTCONTROL_H__DEE41104_5449_11D2_865F_0000C061C6E2__INCLUDED_

#include "SearchList.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//

/////////////////////////////////////////////////////////////////////////////
// CInputControl window

class CInputControl : public CEdit
{
// Construction
public:
	CInputControl();
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputControl)
	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	public:
//		virtual BOOL PreTranslateMessage(MSG *pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	//
	void		GetFile(LPCSTR szKey, CString &szPath);
	void		SetPath(CString strPath);
	void		ReadMasterFile();
	void		FillCode_JMaster();
	void		FillCode_KMaster();
	void		SetType(int nType); // 0: Quote, 1:Index, 2:Future
	CString		m_strPath;
	CListCtrl	m_ctrlCodeList;
	
	
	// 한글종목명 검색을 위한 루틴
	BOOL		mp_bHangulMode;
	void		FT_vHangulSearch();
	BOOL		FP_bCheckNumeric(CString csData);
	BOOL		FP_bCheckCodeByList(CString szCode);
	
	void			FP_vShowComboList(BOOL bShow);				// 한글종목명 검색을 Show한다.
	void			FP_vEventSeachList(CString csData);			// 한글종목명 검색을 위한 함수
	BOOL			FP_bEnableHangulSearch();					// 한글종목명이 있는 코드를 체크하기위한 루틴

	BOOL		FP_bCheckCodeByList();
//	void		ShowSearchList(BOOL bShow) { m_bShowSearchList = bShow;}

	CSearchList		m_SearchList;						// 한글종목명 검색을 위한 윈도우
	

	virtual ~CInputControl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputControl)
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bESC;	
	int	m_nType; // 0: Quote, 1:Index, 2:Future
	//BOOL m_bShowSearchList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTCONTROL_H__DEE41104_5449_11D2_865F_0000C061C6E2__INCLUDED_)
