// CContentWnd.cpp: 구현 파일
//

#include "stdAfx.h"
#include "IB771000.h"
#include "CContentWnd.h"


// CContentWnd

IMPLEMENT_DYNAMIC(CContentWnd, CWnd)

CContentWnd::CContentWnd()
{

}

CContentWnd::~CContentWnd()
{
}


BEGIN_MESSAGE_MAP(CContentWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CContentWnd 메시지 처리기




void CContentWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rec;
	GetClientRect(rec);

	dc.FillSolidRect(rec, RGB(255, 255, 255));
}
