#if !defined(AFX_EXIMGLISTBUTTON_H__810E2818_9499_44EC_9E49_1FECA4720093__INCLUDED_)
#define AFX_EXIMGLISTBUTTON_H__810E2818_9499_44EC_9E49_1FECA4720093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExImgListButton.h : header file
//
#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExImgListButton window
class CLoaderImgListButton;
class CExImgListButton : public CExButton
{
// Construction
public:
	CExImgListButton();

// Attributes
protected:
	CLoaderImgListButton*		m_pLoader;

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
	// Return		: CImageList* 세팅이 되었는지 여부
	// Paramter
	//		CImageList* pImageList : 버튼상태 이미지를 가지고 있는 이미지 리스트 포인터
	//		int nNormal : Up 상태 리스트 인덱스
	//		int nDown : Down 상태 리스트 인덱스
	//		int nDisable : 비활성화 상태 리스트 인덱스
	//		int nOver : MouseOver 상태 리스트 인덱스
	//		LPCSTR lpszText : 버튼 텍스트
	// Comments		: CImageList의 이미지를 이용하여 버튼을 그립니다.
	//-----------------------------------------------------------------------------
	BOOL	SetImageList(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText = NULL, COLORREF clrText = 0x000000, COLORREF clrTextDis = 0x000000);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		LPCTSTR lpszText : 변경할 버튼 텍스트
	// Comments		: 버튼의 텍스트를 변경합니다.
	//-----------------------------------------------------------------------------
	void		SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExImgListButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExImgListButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExImgListButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXIMGLISTBUTTON_H__810E2818_9499_44EC_9E49_1FECA4720093__INCLUDED_)
