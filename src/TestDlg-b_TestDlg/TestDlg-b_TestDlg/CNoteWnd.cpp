// CNoteWnd.cpp: 구현 파일
//

#include "pch.h"
//#include "axis.h"
#include "CNoteWnd.h"
#include "NoteDefine.h"


// CNoteWnd

IMPLEMENT_DYNAMIC(CNoteWnd, CWnd)

CNoteWnd::CNoteWnd()
{

}

CNoteWnd::~CNoteWnd()
{
}


BEGIN_MESSAGE_MAP(CNoteWnd, CWnd)
	ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_MESSAGE(WM_USER, OnMessage)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// CNoteWnd 메시지 처리기


LRESULT CNoteWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
    int len = 0;
    switch (LOBYTE(LOWORD(wParam)))
    {
    case MSG_FUNCWND_LOG:
    {
        CString sdata;
        sdata.Format("%s", lParam);
        TRACE(sdata);
    }
    break;
    }

    return FALSE;
}

void CNoteWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
    if (1)
    {
        double darkenFactor;

        if(m_bMouseTracking)
            darkenFactor = 0.9;
        else
            darkenFactor = 1.0;

        CRect rect;
        GetClientRect(rect);

        CBrush yellowBrush(RGB( static_cast<int>(255 * darkenFactor), static_cast<int>(247 * darkenFactor), static_cast < int>(209 * darkenFactor)));
        dc.FillRect(rect, &yellowBrush);

        // 접힌 부분을 그립니다.
        CPoint foldPoints[3];
        foldPoints[0] = CPoint(rect.right - 20, rect.bottom);
        foldPoints[1] = CPoint(rect.right, rect.bottom - 20);
        foldPoints[2] = CPoint(rect.right, rect.bottom);

        CBrush foldBrush(RGB( 255, 255, 255));
        CBrush* pOldBrush = dc.SelectObject(&foldBrush);

        // 삼각형을 채웁니다.
        dc.Polygon(foldPoints, 3);

        // 접힌 부분과 나머지 부분의 경계를 그립니다.
        dc.MoveTo(rect.right - 20, rect.bottom);
        dc.LineTo(rect.right, rect.bottom - 20);
        dc.LineTo(rect.right, rect.bottom);
        dc.LineTo(rect.right - 20, rect.bottom);

       CBrush whiteBrush(RGB(static_cast<int>(darkenFactor * 229), static_cast<int>(darkenFactor * 222), static_cast<int>(darkenFactor * 188)));
        dc.SelectObject(&whiteBrush);

        CPoint whitePoints[3];
        whitePoints[0] = CPoint(rect.right - 20, rect.bottom);
        whitePoints[1] = CPoint(rect.right, rect.bottom - 20);
        whitePoints[2] = CPoint(rect.right - 20, rect.bottom - 20);

        // 테두리를 그리지 않도록 경로를 사용하여 폴리곤을 그림
        whitePoints[0] = CPoint(rect.right - 19, rect.bottom);
        whitePoints[1] = CPoint(rect.right, rect.bottom - 19);
        whitePoints[2] = CPoint(rect.right - 20, rect.bottom - 20);
        dc.BeginPath();
        dc.Polygon(whitePoints, 3);
        dc.EndPath();
        dc.FillPath();

        // 원래 브러쉬로 복원합니다.
        dc.SelectObject(pOldBrush);
    }
    else
    {
        COLORREF startColor = RGB(202, 224, 77); // yellow
        COLORREF endColor = RGB(205, 224, 135); // yellow

        CRect rect;
        GetClientRect(&rect);

        // 전체 다이얼로그 크기로 라운드 처리된 사각형 생성
        CRgn roundRgn;
        int roundness = 20; // 모서리의 라운드 정도를 설정 (원의 반지름)
        roundRgn.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, roundness, roundness);

        // 라운드 처리된 사각형에 클리핑 영역 설정
        dc.SelectClipRgn(&roundRgn);

        // 그라데이션 그리기
        for (int i = 0; i < rect.Height(); ++i)
        {
            int r = GetRValue(startColor) + (i * (GetRValue(endColor) - GetRValue(startColor)) / rect.Height());
            int g = GetGValue(startColor) + (i * (GetGValue(endColor) - GetGValue(startColor)) / rect.Height());
            int b = GetBValue(startColor) + (i * (GetBValue(endColor) - GetBValue(startColor)) / rect.Height());

            dc.FillSolidRect(0, i, rect.Width(), 1, RGB(r, g, b));
        }

        GetClientRect(rect);
        dc.SetBkMode(TRANSPARENT);
        dc.DrawText(m_data, rect, DT_LEFT);
    }
}


void CNoteWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    AfxMessageBox(m_data);
    CWnd::OnLButtonUp(nFlags, point);
}


void CNoteWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (!m_bMouseTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);

        m_bMouseTracking = true;
        Invalidate();
    }
    CWnd::OnMouseMove(nFlags, point);
}


void CNoteWnd::OnMouseLeave()
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    m_bMouseTracking = false;
    Invalidate();
    CWnd::OnMouseLeave();
}
