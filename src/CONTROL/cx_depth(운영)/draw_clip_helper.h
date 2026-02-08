
#pragma once
#include <afxwin.h>
#include <functional>

// Helper declarations for clip-aware draw
void draw_clip_wrapper(CWnd* pWnd, CDC* pDC, std::function<void(CDC*, const CRect*)> drawFunc, const CRect* pClip = nullptr);
