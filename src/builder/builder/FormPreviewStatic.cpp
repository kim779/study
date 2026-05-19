#include "StdAfx.h"
#include "FormPreviewStatic.h"
#include "h/mainvar.h"

BEGIN_MESSAGE_MAP(CFormPreviewStatic, CStatic)
	ON_MESSAGE(WM_OBJMSG, &CFormPreviewStatic::OnObjMsg)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CFormPreviewStatic::CFormPreviewStatic()
{
}


CFormPreviewStatic::~CFormPreviewStatic()
{
}

LRESULT CFormPreviewStatic::OnObjMsg(WPARAM wp, LPARAM lp) // 오브젝트 이벤트 후킹
{
	CPoint pt;
	switch (LOWORD(wp))
	{
	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnLButtonDown(0, pt);
		break;
	}

	return 0;
}

void CFormPreviewStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	this->ClientToScreen(&point);
	GetParent()->ScreenToClient(&point);
	GetParent()->SendMessage(WM_OBJMSG, MAKEWPARAM(WM_LBUTTONDOWN, 0), MAKELPARAM(point.x, point.y));
	CStatic::OnLButtonDown(nFlags, point);
}
