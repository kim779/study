#pragma once

#include <afxpriv.h>
// CScrollWindow

class CScrollWindow : public CWnd
{
	//DECLARE_DYNAMIC(CScrollWindow)

public:
	CScrollWindow();
	virtual ~CScrollWindow();

public:
	//sets the sizes relating to the scrolling view port in device 
	//coordinates

	void SetScrollSizes(SIZE sizeTotal,
		const SIZE& sizePage = CSize(0, 0),
		const SIZE& sizeLine = CSize(0, 0));

	//Returns the current location fo the upper corner of the scrolling view
	//스크롤뷰의 상단 모서리 의 현재 위치를 반환한다
	//port in device coordinates
	CPoint GetScrollPosition() const;


	//Returns the total size in device coordinates of the scrolling region.
	//스크롤뷰의 장치 좌표에서 전체 크기를 반환 한다.
	CSize GetTotalSize() const;

	//Returns the some dimensions of the scrolling region. All the values are
	//스크롤영역의 일부값을 반환한다?(전체 스크롤영역에서 현재 화면에 보여지는 일부를 반환?)
	void GetScrollSizes(SIZE& sizeTotal,
		SIZE& sizePage, SIZE& sizeLine) const;

	//Moves the scrolling view port to a position in device coordinates.
	//스크롤 뷰포트(스크롤 위치)를 장치좌표의 위치로(POINT pt) 이동시킨다.
	void ScrollToPosition(POINT pt);

	//Fills the outside of the scrolling region with the specified brush.
	//지정한 브러쉬로 스크롤 영역(화면에 보이는) 외부를 칠한다?? (안보이는데 굳이?)
	void FillOutsideRect(CDC* pDC, CBrush* pBrush);

	//Resizes the parent window so that no scroll bar is needed.
	//스크롤 바가 필요하지 않도록 부모 윈도우 크기를 조절한다...
	void ResizeParentToFit(BOOL bShrinkOnly = TRUE);

	//scrolling implementation support for OLE
	//OLE 에 대한 스크롤링 구현 지원
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);  //check

	//Scrolling implementation support for OLE.
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);  //check

protected:
	enum
	{
		CX_BORDER = 1,
		CY_BORDER = 1
	};

	static BOOL m_bGotMouseScrollLines;

	CSize m_sizeTotal;           // Total size in device units   
	CSize m_sizePage;            // Per page scroll size in device units
	CSize m_sizeLine;            // Per line scroll size in device units

	BOOL m_bInsideUpdate;  // Internal state for OnSize callback

	int m_cxVScroll;
	int m_cyHScroll;

protected:
	static UINT GetMouseScrollLines();

	//Adjust scrollbars and more.
	void UpdateBars();

	// A helper method which is invoked by OnMouseWheel.
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

	//Gets the client size not taking into account the scroll bars.
	//return TRUE if enough room to add scrollbars if needed.
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	void GetScrollBarSizes(CSize& sizeSb);

	//Helper to return the state of the scrollbars without actually changing
	//실제 변화없이 스크롤바의 상태를 반환해주는 헬퍼
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
		CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

	//Called by the framework before the OnDraw member function is called for 
	// screen display and before the OnPrint member function is called for each
	// page during printing or print preview.
	//화면 표시를 위해 OnDraw 멤버 함수가 호출되기 전과 인쇄 또는 인쇄 미리 보기 동안
	//각 페이지에 대해 OnPrint 멤버 함수가 호출되기 전에 프레임워크에서 호출됩니다
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);  //check
	virtual void OnDraw(CDC* pDC);


	//Called by the framework to render an image of the document.

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnPaint();
};


