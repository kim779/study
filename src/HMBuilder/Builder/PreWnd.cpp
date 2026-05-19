// PreWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PreWnd.h"


// CPreWnd

IMPLEMENT_DYNAMIC(CPreWnd, CWnd)

CPreWnd::CPreWnd()
{

}

CPreWnd::~CPreWnd()
{
}


BEGIN_MESSAGE_MAP(CPreWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPreWnd 메시지 처리기입니다.




void CPreWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
}
