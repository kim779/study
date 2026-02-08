// ExStatic.h: interface for the CExStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXSTATIC_H__751A3F09_37B9_468F_AF59_79B0C19C6A5B__INCLUDED_)
#define AFX_EXSTATIC_H__751A3F09_37B9_468F_AF59_79B0C19C6A5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderStatic;
class CExStatic : public CStatic  
{
	DECLARE_DYNCREATE(CExStatic);

// Construction
public:
	CExStatic(BOOL bUseDefSkin = FALSE);

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
	// Return		: 스태틱 색상 지정 여부
	// Paramter
	//		COLORREF clrOutLine : 바깥쪽 라인 색상
	//		COLORREF clrBackground : 배경 색상
	//		COLORREF clrText : 텍스트 색상
	// Comments		: 컬러테이블을 이용하지 않고 스태틱을 그림
	//-----------------------------------------------------------------------------
	BOOL	SetStaticColor(COLORREF clrOutLine, COLORREF clrBackground, COLORREF clrText);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 스태틱 색상 지정 여부
	// Paramter
	//		UINT nOutLine : 바깥쪽 라인 색상 인덱스 (컬러테이블에 정의되어있는 인덱스)
	//		UINT nBackground : 배경 색상 인덱스 (컬러테이블에 정의되어있는 인덱스)
	//		UINT nText : 텍스트 색상 인덱스 (컬러테이블에 정의되어있는 인덱스)
	// Comments		: 컬러테이블 인덱스를 이용하여 스태틱을 그림
	//-----------------------------------------------------------------------------
	BOOL	SetSkinIndex(UINT nOutLine, UINT nBackground, UINT nText);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 스태틱 색상 지정 여부
	// Paramter
	//		LPCTSTR lpszOutLine : 바깥쪽 라인 색상 이름 (컬러테이블에 정의되어있는 이름)
	//		LPCTSTR lpszBackground : 배경 색상 이름 (컬러테이블에 정의되어있는 이름)
	//		LPCTSTR lpszText : 텍스트 색상 이름 (컬러테이블에 정의되어있는 이름)
	// Comments		: 컬러테이블 이름을 이용하여 스태틱을 그림
	//-----------------------------------------------------------------------------
	BOOL	SetSkinString(LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 아이콘 지정 여부
	//		CSize size : 아이콘 크기
	//		HICON hIcon : Up 아이콘 핸들
	//		HICON hIconPushed : Down 아이콘 핸들
	//		UINT nIconAlign : 아이콘 정렬방식 (STC_ICON_NORMAL:왼쪽, STC_ICON_RIGHT:오른쪽, STC_ICON_TWOROWS:위쪽)
	// Comments		: 아이콘을 지정합니다.
	//-----------------------------------------------------------------------------
	BOOL	SetIconImage(CSize size, HICON hIcon, UINT nIconAlign = STC_ICON_NORMAL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: void
	// Paramter
	//		LPCTSTR lpszText : 변경할 스태틱 텍스트
	// Comments		: 스태틱의 텍스트를 변경합니다.
	//				  SetWindowText함수로 텍스트를 변경할때에는 Invalidate함수 다시 그려야 합니다. 
	//-----------------------------------------------------------------------------
	void	SetText(LPCTSTR lpszText);

	void	GetWindowText(CString& rString);

// Attributes
protected:
	CLoaderStatic*		m_pLoader;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExStatic)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExStatic)
	afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXSTATIC_H__751A3F09_37B9_468F_AF59_79B0C19C6A5B__INCLUDED_)
