#pragma once

#include "CScrollWindow.h"
// CControlWnd

class CControlWnd : public CWnd
{
	DECLARE_DYNAMIC(CControlWnd)

public:
	std::thread drawingThread;
	std::mutex threadMutex;
	bool drawingThreadRunning = false;

	void SetDrawingThreadRunning(bool isRunning) {
		std::lock_guard<std::mutex> lock(threadMutex);
		drawingThreadRunning = isRunning;
	}

	void CloseDrawingThread() {
		SetDrawingThreadRunning(false);
	//	drawingThread.join(); // Wait for the thread to finish
	}


public:
	CControlWnd();
	virtual ~CControlWnd();
	CString m_slog;
	CRect m_rect;
	BOOL LoadImage();
	void DrawInfo(CDC* pDC);
	std::unique_ptr<CBitmap> m_pBitmap;

	void CheckWnd(CString strdata);
	CScrollBar m_vScrollBar;
	int m_nScrollPos;
	int m_Scroll{};
	int m_lHeadHeight = 18;
	int m_lDataHeight = 18;
	int m_wDisableBar = SB_VERT;
	CPoint m_StartPnt{};
	long	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	long	GetScrollMax32(int nBar);
	void adjustScroll(int wBar);




	BYTE* m_byte{};


	BITMAPINFO inputBitmapInfo;		//비트맵
	unsigned char* inputRGBbuffer;	//RGB 이미지 버퍼
	int inputStageTop, inputStageBottom, inputStageRight, inputStageLeft;
	int inputStageWidth, inputStageHeight;
	int inputWidthPerRegion, inputHalfWidthPerRegion;
	double inputStageXoffset, inputStageYoffset, inputStageZoom;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	void TestDraw(CDC* pDC);
};


class CMeMDC : public CDC
{
public:

	// constructor sets up the memory DC
	CMeMDC(CDC* pDC) : CDC()
	{
		ASSERT(pDC != nullptr);

		m_pDC = pDC;
		m_oldBitmap = nullptr;
		m_bMemDC = !pDC->IsPrinting();

		if (m_bMemDC)    // Create a Memory DC
		{
			pDC->GetClipBox(&m_rect);
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());
			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else        // Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}

	// Destructor copies the contents of the mem DC to the original DC
	~CMeMDC()
	{
		if (m_bMemDC)
		{
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
			SelectObject(m_oldBitmap);

			m_bitmap.DeleteObject();
		}
		else
		{
			m_hDC = m_hAttribDC = nullptr;
		}
	}

	// Allow usage as a pointer
	CMeMDC* operator->() { return this; }

	// Allow usage as a pointer
	operator CMeMDC* () { return this; }

private:
	CBitmap  m_bitmap;
	CBitmap* m_oldBitmap;
	CDC* m_pDC;
	CRect    m_rect;
	BOOL     m_bMemDC;
};