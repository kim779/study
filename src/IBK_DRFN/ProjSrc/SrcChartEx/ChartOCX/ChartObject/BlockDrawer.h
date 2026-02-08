// BlockDrawer.h: interface for the CBlockDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKDRAWER_H__B1E57735_9D74_4E10_97A3_2320E463D4EA__INCLUDED_)
#define AFX_BLOCKDRAWER_H__B1E57735_9D74_4E10_97A3_2320E463D4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBlockDrawer  
{
public:
	CBlockDrawer();

	CRect GetBlockMoveRegion(const CPoint& topLeft) const;
	CRect GetBlockDeleteRegion(const CPoint& topRight) const;
	// 블럭 최대화 추가 - ojtaso (20080519)
	CRect GetBlockSizeRegion(const CPoint& topRight) const;

	void DrawFrameRegion(CDC* pDC, const CRect& region, const COLORREF& penColor, const int penThickness);
	void DrawBlockMove2DButton(CDC* pDC, const CPoint& topLeft, const COLORREF& penColor = RGB(0, 0, 0), const COLORREF& brushColor = RGB(255, 255, 255));
	void DrawBlockDelete2DButton(CDC* pDC, const CPoint& topRight, const COLORREF& penColor = RGB(0, 0, 0), const COLORREF& brushColor = RGB(255, 255, 255));
	// 블럭 최대화 추가 - ojtaso (20080519)
	void DrawBlockSize2DButton(CDC* pDC, const CPoint& topRight, const COLORREF& penColor = RGB(0, 0, 0), const COLORREF& brushColor = RGB(255, 255, 255));
	void DrawBlockMove3DButton(CDC* pDC, const CPoint& topLeft, const bool bIsFlat = false, const bool bIsSelection = false);
	void DrawBlockDelete3DButton(CDC* pDC, const CPoint& topRight, const bool bIsFlat = false, const bool bIsSelection = false);

private:
	const int HEIGHT;

	void Draw2DButton(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	void Draw3DButton(CDC* pDC, const CRect& region, const bool bIsFlat, const bool bIsUp);
	void Draw3DButton_Up(CDC* pDC, const CRect& region, const bool bIsFlat);
	void Draw3DButton_Down(CDC* pDC, const CRect& region, const bool bIsFlat);
	COLORREF GetFrameRegionColor(const bool bIsFlat) const;

	void DrawPoints(CDC* pDC, const CRect& region, const COLORREF& penColor, const bool bIsLightPoints);
	bool IsDrawPoint(const CRect& region, const CPoint& point) const;
	CPoint GetDrawPointPosition(const CRect& region, const bool bIsLightPoints) const;

	void DrawX(CDC* pDC, const CRect& region, const COLORREF& penColor);
	// 블럭 최대화 추가 - ojtaso (20080519)
	void DrawMax(CDC* pDC, const CRect& region, const COLORREF& penColor);
	// 블럭 최대화 추가 - ojtaso (20080519)
	void DrawRestore(CDC* pDC, const CRect& region, const COLORREF& penColor);

	void DrawRegion(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	void DrawRegionFrame(CDC* pDC, const CRect& region, const COLORREF& penColor, 
		const bool bIsLeft = true, const bool bIsTop = true, const bool bIsRight = true, const bool bIsBottom = true);
	void DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2);
};

#endif // !defined(AFX_BLOCKDRAWER_H__B1E57735_9D74_4E10_97A3_2320E463D4EA__INCLUDED_)
