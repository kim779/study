#if !defined(AFX_EXDRAWBUTTON_H__42FFF992_01A3_4F06_BEFC_D26E6C64F1E4__INCLUDED_)
#define AFX_EXDRAWBUTTON_H__42FFF992_01A3_4F06_BEFC_D26E6C64F1E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExDrawButton.h : header file
//
#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExDrawButton window
class CLoaderDrawButton;
class CExDrawButton : public CExButton
{
// Construction
public:
	CExDrawButton(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderDrawButton*		m_pLoader;
	POSITION				m_posChangedSkin;

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
	long		ActionControl(LPVOID lpAction);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 색상지정 세팅 여부
	// Paramter
	//		UINT nButton : 버튼 종류 (기본버튼 SKINBTN_DEFAULT)
	//		SKINBTN_ACCENT : 강조버튼, SKINBTN_TAB : 탭버튼, SKINBTN_BUY : 매수버튼, SKINBTN_SELL : 매도버튼, SKINBTN_CCLMDY : 취소정정버튼
	// Comments		: 기본버튼인 경우 이 함수를 호출하지 않아도 됩니다.
	//				  기본버튼은 컬러테이블에 기본버튼 색상을 이용합니다. (따로 지정할 필요없음)
	//				  메인에서 스킨이 변경되었을때 자동으로 적용됩니다.
	//-----------------------------------------------------------------------------
	BOOL		SetButtonColor(UINT nButton = 0);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 아이콘 세팅 여부
	// Paramter
	//		CSize size : 아이콘 크기
	//		HICON hIcon : Up 아이콘 핸들
	//		HICON hIconPushed : Down 아이콘 핸들
	//		UINT nIconAlign : 아이콘 정렬방식 (BTN_ICON_NORMAL:왼쪽, BTN_ICON_RIGHT:오른쪽, BTN_ICON_TWOROWS:위쪽)
	// Comments		: 아이콘을 지정합니다.
	//				  비트맵 지정과 차이는 아이콘의 배경이 TransParent이고 버튼 크기변경에도 아이콘은 고정됩니다.
	//-----------------------------------------------------------------------------
	BOOL		SetIconImage(CSize size, HICON hIcon, HICON hIconPushed = NULL, UINT nIconAlign = BTN_ICON_NORMAL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		LPCTSTR lpszText : 변경할 버튼 텍스트
	// Comments		: 버튼의 텍스트를 변경합니다.
	//				  SetWindowText함수로 텍스트를 변경할때에는 Invalidate함수 다시 그려야 합니다. 
	//-----------------------------------------------------------------------------
	void		SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

	virtual void	UseToggleMode(BOOL bUse = TRUE);
	virtual BOOL	GetToggleState();
	virtual void	SetToggleState(BOOL bPush);
	virtual void	ReleasePushed();
	
	// 마지막 인자는 항상 NULL
	void AddToggleGroup(CExDrawButton* pButton, ...);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExDrawButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExDrawButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExDrawButton)
	afx_msg void OnDestroy();
	afx_msg BOOL OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXDRAWBUTTON_H__42FFF992_01A3_4F06_BEFC_D26E6C64F1E4__INCLUDED_)
