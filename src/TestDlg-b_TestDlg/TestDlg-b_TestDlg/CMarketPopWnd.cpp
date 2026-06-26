// CMarketPopWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CMarketPopWnd.h"

#define TM_CHECKMOVE  9898
// CMarketPopWnd

IMPLEMENT_DYNAMIC(CMarketPopWnd, CWnd)

CMarketPopWnd::CMarketPopWnd()
{

}

CMarketPopWnd::~CMarketPopWnd()
{
}

BEGIN_MESSAGE_MAP(CMarketPopWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CMarketPopWnd 메시지 처리기

void CMarketPopWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rc;
	GetClientRect(rc);

	dc.FillSolidRect(rc, RGB(255, 255, 255));
}


int CMarketPopWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	//SetTimer(TM_CHECKMOVE, 10, nullptr);
	return 0;
}


BOOL CMarketPopWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}


void CMarketPopWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_CHECKMOVE:
		{
			if (IsWindowVisible())
			{
				CRect rec, tmprec;
				tmprec.SetRect(0, 0, 200, 200);
				MoveWindow(tmprec, true);
				/*m_pParent->GetWindowRect(&rec);
				if (rec != m_parentRect)
				{
					GetWindowRect(tmprec);
					tmprec.OffsetRect(rec.left - m_parentRect.left, rec.top - m_parentRect.top);
					MoveWindow(tmprec, true);
					m_parentRect = rec;
				}*/
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


void CMarketPopWnd::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	PostMessage(WM_CLOSE, 0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMarketPopWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	PostMessage(WM_CLOSE, 0, 0);
	CWnd::OnLButtonDown(nFlags, point);
}


void CMarketPopWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMarketPopWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMarketPopWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMarketPopWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
