#if !defined(AFX_ILBUTTON_H__3605EB7B_DC70_4339_A3B0_75C0934C6DBF__INCLUDED_)
#define AFX_ILBUTTON_H__3605EB7B_DC70_4339_A3B0_75C0934C6DBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ILButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CILButton window

class CILButton : public CButton
{
// Construction
public:
	CILButton();

// 변수
public:
	BOOL					m_bUseDrawTitle;
	BOOL					m_bUseToolTip;
	CString					m_szTitle;
	CString					m_szDrawTitle;
	BOOL					m_bButtonDown;
	
protected:
	CImageList*				m_pImageList;				// 버튼을 그리는 이미지를 담고 있는 리스트
	int						m_nNormal;					// Normal 상태의 이미지 인덱스
	int						m_nDown;					// Down 상태의 이미지 인덱스
	int						m_nDisable;					// Disable 상태의 이미지 인덱스
	int						m_nOver;					// Over 상태의 이미지 인덱스
	BOOL					m_bPushLike;				// PushLike 버튼사용여부
	BOOL					m_bCheck;					// ChekcState
	BOOL					m_bLbuttonDown;				// 마우스버튼 다운 체트
	CToolTipCtrl			m_Tooltip;
	COLORREF				m_clrBack;

// 함수
public:
	void					FP_vSetImage(	CImageList* pImageList,	
											COLORREF	clrBack,
											int			nNormal = -1,
											int			nDown = -1,
											BOOL		bDrawTitle = FALSE,
											CString		strTitle =_T(""),
											BOOL		bPushLike = FALSE,
											int			nDisable = -1,
											int			nOver = -1
											);
	void					SetCheck(BOOL bCheck);
	BOOL					GetCheck();
	void					FP_vSetToolTipText(CString s);
	void					RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CILButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBtnTitle(CString szTitle, CString szDrawTitle);
	virtual ~CILButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CILButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ILBUTTON_H__3605EB7B_DC70_4339_A3B0_75C0934C6DBF__INCLUDED_)
