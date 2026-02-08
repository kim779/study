#include "StdAfx.h"
#include "draw_clip_helper.h"

void draw_clip_wrapper(CWnd* pWnd, CDC* pDC, std::function<void(CDC*, const CRect*)> drawFunc, const CRect* pClip)
{
    CRect clientRect;
    pWnd->GetClientRect(&clientRect);

    // 실제 버퍼링할 영역
    CRect bufferRect = pClip ? *pClip : clientRect;
    if (bufferRect.IsRectEmpty())
        return;

    int w = bufferRect.Width();
    int h = bufferRect.Height();

    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, w, h);
    CBitmap* old = memDC.SelectObject(&bmp);

    // memDC 좌표를 0,0 기준으로 맞추기
    memDC.SetViewportOrg(-bufferRect.left, -bufferRect.top);

    // memDC clip 영역은 0~w, 0~h 이 되어야 함
    memDC.IntersectClipRect(0, 0, w, h);

    // drawFunc 에 넘길 clipRect 는 memDC 좌표계로 변환해야 함
    CRect localClip = bufferRect;
    localClip.OffsetRect(-bufferRect.left, -bufferRect.top);

    // 실제 그리기 호출
    drawFunc(&memDC, &localClip);

    // BitBlt source 는 memDC (0,0)
    pDC->BitBlt(
        bufferRect.left,
        bufferRect.top,
        w, h,
        &memDC,
        0, 0,
        SRCCOPY
    );

    memDC.SelectObject(old);
}
