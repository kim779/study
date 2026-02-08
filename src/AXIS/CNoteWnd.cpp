// CNoteWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "axis.h"
#include "CNoteWnd.h"


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
END_MESSAGE_MAP()



// CNoteWnd 메시지 처리기




void CNoteWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
    CRect rect;
    GetClientRect(rect);

    // 노란색 브러쉬를 생성합니다.
    CBrush yellowBrush(RGB(255, 247, 209));
    dc.FillRect(rect, &yellowBrush);

    // 접힌 부분을 그립니다.
    CPoint foldPoints[3];
    foldPoints[0] = CPoint(rect.right - 20, rect.bottom);
    foldPoints[1] = CPoint(rect.right, rect.bottom - 20);
    foldPoints[2] = CPoint(rect.right, rect.bottom);

    // 어두운 노란색 브러쉬를 생성합니다.
    CBrush foldBrush(RGB(255, 255, 255));
    CBrush* pOldBrush = dc.SelectObject(&foldBrush);

    // 삼각형을 채웁니다.
    dc.Polygon(foldPoints, 3);

    // 접힌 부분과 나머지 부분의 경계를 그립니다.
    dc.MoveTo(rect.right - 20, rect.bottom);
    dc.LineTo(rect.right, rect.bottom - 20);
    dc.LineTo(rect.right, rect.bottom);
    dc.LineTo(rect.right - 20, rect.bottom);

    // 접힌 부분 위의 흰색 삼각형을 그립니다.
    CBrush whiteBrush(RGB(229, 222, 188));
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


void CNoteWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    AfxMessageBox(m_data);
    CWnd::OnLButtonUp(nFlags, point);
}
