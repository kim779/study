// ExListCtrl.h: interface for the CExListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXLISTCTRL_H__8BCCC1F8_8DA9_46E1_9E6E_42E9D4C60074__INCLUDED_)
#define AFX_EXLISTCTRL_H__8BCCC1F8_8DA9_46E1_9E6E_42E9D4C60074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderListCtrl;
class CExListCtrl : public CListCtrl  
{
// Construction
public:
	CExListCtrl(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderListCtrl*		m_pLoader;

// Operations
public:
 	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 메모리 포인터나 long형 값
	// Paramter
	//		LPVOID lpAction : 메모리 포인터 (구조체, 클래스등등..)
	// Comments		: 정의된 함수이외에 작업을 해야 하는 경우 ActionControl 함수를
	//				  사용해 클래스 수정없이 프로젝트 진행 가능.
	//				  파라메터와 리턴값은 각 화면에서의 요구사항데로 정의 가능함
	//				  ActionControl을 사용해 추가된 사항은 추후 클래스 수정으로 함수가 추가되지만
	//				  원활한 프로젝트 진행을 위해 임시로 이 함수를 사용합니다.
	//-----------------------------------------------------------------------------	
	long	ActionControl(LPVOID lpAction);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		BOOL bDisable : 컬럼 리사이징 가능 여부
	// Comments		: 리스트의 컬럼 너비 조정을 비활성화 할 것인지 세팅
	//-----------------------------------------------------------------------------
	void	DisableColumnSizing(BOOL bDisable);

//	void	SetHeaderHeight(int nHeight);
	void	SetRowHeight(int nHeight);
	void	SetAutoColSize(BOOL bAutoSize);
	void	SetTwotoneRowColor(int nRow = 1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExListCtrl();

	// Generated message map functions
protected:

	//{{AFX_MSG(CExListCtrl)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXLISTCTRL_H__8BCCC1F8_8DA9_46E1_9E6E_42E9D4C60074__INCLUDED_)
