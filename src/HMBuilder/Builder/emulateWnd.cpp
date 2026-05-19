// emulateWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "emulateWnd.h"


// CEmulateWnd

IMPLEMENT_DYNCREATE(CEmulateWnd, CView)

CEmulateWnd::CEmulateWnd()
{

}

CEmulateWnd::~CEmulateWnd()
{
}

BEGIN_MESSAGE_MAP(CEmulateWnd, CView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CEmulateWnd 그리기입니다.

void CEmulateWnd::OnDraw(CDC* pDC)
{
}


// CEmulateWnd 진단입니다.

#ifdef _DEBUG
void CEmulateWnd::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CEmulateWnd::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEmulateWnd 메시지 처리기입니다.


void CEmulateWnd::OnDestroy()
{
	m_pDropTarget = NULL;
	CView::OnDestroy();
}
