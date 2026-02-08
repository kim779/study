#if !defined(AFX_EXBITMAPBUTTON_H__564E404A_690E_4C65_973C_447E90B6E41B__INCLUDED_)
#define AFX_EXBITMAPBUTTON_H__564E404A_690E_4C65_973C_447E90B6E41B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExBitmapButton.h : header file
//
#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExBitmapButton window
class CLoaderBitmapButton;
class CExBitmapButton : public CExButton
{
// Construction
public:
	CExBitmapButton();

// Attributes
protected:
	CLoaderBitmapButton*	m_pLoader;

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
	// Return		: 버튼에 비트맵 핸들이 세팅되었는지 여부
	// Paramter
	//		LPCTSTR lpszUp : Up상태의 비트맵 파일 경로
	//		LPCTSTR lpszDown : Down상태의 비트맵 파일 경로
	//		LPCTSTR lpszDis : 비활성화상태의 비트맵 파일 경로
	// Comments		: 버튼 페인트를 비트맵으로 처리할 경우 이 함수를 이용합니다.
	//-----------------------------------------------------------------------------	
	BOOL		SetBitmapFile(LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExBitmapButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExBitmapButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExBitmapButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXBITMAPBUTTON_H__564E404A_690E_4C65_973C_447E90B6E41B__INCLUDED_)
