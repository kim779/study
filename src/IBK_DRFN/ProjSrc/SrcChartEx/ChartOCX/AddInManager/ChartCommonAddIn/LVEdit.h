#if !defined(AFX_LVEDIT_H__18636D59_169E_4D7F_BD1C_EA2C450B6CA2__INCLUDED_)
#define AFX_LVEDIT_H__18636D59_169E_4D7F_BD1C_EA2C450B6CA2__INCLUDED_

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	List Control의 Sub Item을 편집하는 Edit Control이다.
///////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LVEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLVEdit window

class CLVEdit : public CEdit
{
// Construction
public:
	CLVEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLVEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLVEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLVEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	편집된 내용을 반영할 List Control의 Item과 SubItem Index를 관리한다.
//
//	내용	:	Parent에서 Edit Control을 생성할 때 설정한다. (Friend Class)
///////////////////////////////////////////////////////////////////////////////
protected:
	int m_nItem;
	int m_nSubItem;
friend class CEditableListCtrl;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Ctrl+Arrow Key를 통하여 편집 Sub Item의 조정을 시도하는 Routine를 제공한다.
//
//	수정	:	(2003.01.08) 이동 성공 여부를 확인하기 위하여 BOOL을 Return한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	BOOL	TryEditSubItem( int p_nItem, int p_nSubItem);

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.08
//
//	목적	:	Ctrl Key의 눌린 상태를 나타내는 Flag을 관리한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	BOOL	m_bCtrl;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	Enter를 이용하여야만 Update를 지원할지 여부를 나타내는 Flag을 관리한다.
//
//	내용	:	1. 기본적으로 Flase이며, Enter외에도 단순히 Kill Focus 시점에도 Update된다.
//				2. 그렇다 하여도, Esc Key를 이용한 Kill Focus의 시점에는 Update되지 않는다.
///////////////////////////////////////////////////////////////////////////////
protected:
	BOOL		m_bUpdateOnlyEnter;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	UpdateOnlyEnter Option을 제공하기 위하여, Update Routine을 별도로 제공한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	void		UpdateToListCtrl( void);

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	Parent인 List Control의 Pointer를 관리한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	CEditableListCtrl *	m_pEditableListCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LVEDIT_H__18636D59_169E_4D7F_BD1C_EA2C450B6CA2__INCLUDED_)
