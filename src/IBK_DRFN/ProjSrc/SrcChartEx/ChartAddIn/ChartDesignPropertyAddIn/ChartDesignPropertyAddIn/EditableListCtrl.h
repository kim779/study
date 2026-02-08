#if !defined(AFX_EDITABLELISTCTRL_H__CAFE41FA_3D51_47C3_8E14_3027688AF62A__INCLUDED_)
#define AFX_EDITABLELISTCTRL_H__CAFE41FA_3D51_47C3_8E14_3027688AF62A__INCLUDED_

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Editing이 가능한 List Control Class이다.
//
//	참고	:	1. 이 List Control은 반드시 InsertColumn시에 자신의 Memeber를 사용하여야,
//					Column별 Edit여부가 설정된다.
///////////////////////////////////////////////////////////////////////////////

#include <afxcmn.h>	// for CListCtrl


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.31
//
//	목적	:	Parent Window에게 편집완료됨을 알리는 Notification Message와 Structure를 정의한다.
//
//	수정	:	(2003.02.02) Notification에 수정 이전의 Data도 전달하도록 한다.
///////////////////////////////////////////////////////////////////////////////
const UINT RMSG_ELC_CELLEDITED = ::RegisterWindowMessage(_T("RMSG_EDITABLELISTCTRL_CELLEDITED"));
typedef struct tagNMEDITABLELISTCTRL
{
	int				m_nItemIndex;
	int				m_nColumnIndex;
	const char *	m_szNewData;
	const char *	m_szOldData;
} NMEDITABLELISTCTRL, FAR *LPNMEDITABLELISTCTRL;


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditableListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl window

class CLVEdit;
class CEditableListCtrl : public CListCtrl
{
// Construction
public:
	CEditableListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditableListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditableListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditableListCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	특정 Point에 설정된 Item을 편집하는 Routine을 별도로 제공한다.
//
//	내용	:	1. BeginLabelEdit()와 LDoubleClick()에서 둘다 Sub Item을 편집할 수 있도록 한다.
//				2. 또한 다른 Routine에서도 Index로 호출할 수 있도록 Index의 Interface도 제공한다.
//
//	참고	:	편집 Control 생성 Routine Override할 수 있도록 Virtual로 생성한다.
///////////////////////////////////////////////////////////////////////////////
public:
	void			EditSubItem( CPoint point);
	virtual BOOL	EditSubItem( int p_nItem, int p_nSubItem);

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	편집 가능한 Column을 Array로 명시한다.
//
//	수정	:	(2003.01.29) Byte Array를 BOOL Array로 활용하였으나, 다음과 같이 Flag으로 활용하여 
//					ReadOnly, CopyOnly, Editable로 분류하여 처리토록한다.
//					0	: ReadOnly
//					1	: CopyOnly
//					2	: Editable
///////////////////////////////////////////////////////////////////////////////
public:
	CByteArray	m_bEditableColumn;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	편집 가능한 Column은 Column 추가시마다 등록시킨다.
//
//	수정	:	(2003.01.08) 기본적으로 편집가능하도록 조정한다.
//				(2003.01.29) Byte Array를 BOOL Array로 활용하였으나, 다음과 같이 Flag으로 활용하여 
//					ReadOnly, CopyOnly, Editable로 분류하여 처리토록한다.
///////////////////////////////////////////////////////////////////////////////
public:
	int		InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1)
			{	m_bEditableColumn.Add( 2);	return CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.07
//
//	목적	:	Key Input으로 편집을 시작하는 Routine를 추가한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	void	StartEditSubItem( void);

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.01.08
//
//	목적	:	Label 편집시의 Edit Control을 관리한다.
//
//	참고	:	1. Library의 Header로써 추가 Header File이 필요 없도록 Pointer로 정의한다.
///////////////////////////////////////////////////////////////////////////////
protected:
	CLVEdit *	m_pLabelEdit;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.02.21
//
//	목적	:	Enter를 이용하여야만 Update를 지원할지 여부를 설정하는 Interface를 관리한다.
//
//	내용	:	1. 기본적으로 Flase이며, Enter외에도 단순히 Kill Focus 시점에도 Update된다.
//				2. 그렇다 하여도, Esc Key를 이용한 Kill Focus의 시점에는 Update되지 않는다.
///////////////////////////////////////////////////////////////////////////////
public:
	void		SetUpdateOnlyEnter( BOOL p_bUpdateOnlyEnter);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITABLELISTCTRL_H__CAFE41FA_3D51_47C3_8E14_3027688AF62A__INCLUDED_)
