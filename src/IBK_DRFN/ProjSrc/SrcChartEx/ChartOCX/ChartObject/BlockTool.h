// BlockTool.h: interface for the CBlockTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKTOOL_H__209534FC_5C91_43C1_9167_63CEC0ED9A9B__INCLUDED_)
#define AFX_BLOCKTOOL_H__209534FC_5C91_43C1_9167_63CEC0ED9A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"
// 블럭 최대화 추가 - ojtaso (20080519)
#include <afxtempl.h>

class CMainBlock;
class CGraphRegionColor;

// 현재 block을 찾는 Tool
class CBlockTool : public CBaseTool
{
public:
	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);
	void DrawSelectionButton(CDC* pDC, CMainBlock* pMainBlock);

	bool Find(CMainBlock* pMainBlock, const CPoint& point);

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex) = 0;
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor) = 0;
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex) = 0;

	void DrawSelectionBlock(CDC* pDC, CMainBlock* pMainBlock, const CBlockIndex& blockIndex);
	bool Executing(CMainBlock* pMainBlock, const CPoint& point);
	CBlockIndex GetSourceBlockIndex() const;

private:
	CBlockIndex m_sourceBlockIndex; // block 번호

	bool IsDrawSelectionButton(CMainBlock* pMainBlock) const;
	void GetColor(const CGraphRegionColor& graphRegionColor, COLORREF& frameColor, COLORREF& backgroundColor) const;
};


// block move tool
class CBlockMoveTool : public CBlockTool
{
public:
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex);
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);
};


// block delete tool
class CBlockDeleteTool : public CBlockTool
{
public:
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex);
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);

private:
	bool DeleteBlock(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);
};

// 블럭 최대화 추가 - ojtaso (20080519)
class CBlockSizeTool : public CBlockTool
{
public:
	CBlockSizeTool();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

protected:
	virtual bool FindBlock(CMainBlock* pMainBlock, const CPoint& point, CBlockIndex& blockIndex);
	virtual void DrawSelectionBlock(CDC* pDC, const CRect& region, const COLORREF& penColor, const COLORREF& brushColor);
	virtual bool Executing(CMainBlock* pMainBlock, const CBlockIndex& targetBlockIndex);
};

// 구분선에 대한 Tool
class CCompartLineTool : public CBaseTool
{
public:
	// Tool의 모든 내용을 초기화
	virtual void InitializeAllData();
	virtual CBlockBaseData::TOOLTYPE GetToolType() const;

	// mouse
	virtual bool OnLButtonDown(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);
	virtual bool OnLButtonUp(CDC* pDC, CMainBlock* pMainBlock, const CPoint& point);
	virtual void OnMouseMove(CDC* pDC, CMainBlock* pMainBlock, const UINT nFlags, const CPoint& point);

	virtual void OnDraw(CDC* pDC, CMainBlock* pMainBlock);

	// 해당 구분선 찾는다.
	bool Find(CMainBlock* pMainBlock, const CPoint& point);

private:
	bool m_bPreMMovePointUsed;		// m_preMMovePt 사용 여부
	CPoint m_lBtDownPt;				// 드래그를 위해 처음 위치를 저장한다.
	CPoint m_preMMovePt;			// mouse move point
	CBlockIndex m_compartLineIndex; // 구분선의 번호

	bool IsPermissionChangingCompartLine(const CRect& blockRegion, const CPoint& point) const;
	CPoint GetTwoPointGap(const CPoint& pt1, const CPoint& pt2) const;
	CBaseToolEnumData::CURSORTYPE GetBlockCursorType(const CBlockIndex& compartLineIndex) const;

	void DrawCompartLine_Move(CDC* pDC, const CRect& region, const CPoint& point);
	void DrawCompartLine(CDC* pDC, const CRect& region, const CPoint& point);
	void DrawLine(CDC* pDC, const CPoint& pt1, const CPoint& pt2);
};

#endif // !defined(AFX_BLOCKTOOL_H__209534FC_5C91_43C1_9167_63CEC0ED9A9B__INCLUDED_)
