// MainAutoTrendTool.h: interface for the CMainAutoTrendTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINAUTOTRENDTOOL_H__C4216A56_E5E8_42A0_94BF_8F8F470C0030__INCLUDED_)
#define AFX_MAINAUTOTRENDTOOL_H__C4216A56_E5E8_42A0_94BF_8F8F470C0030__INCLUDED_

#include "BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainAutoTrendImplementation;
class CMainBlock;
class CPacketList;

class AFX_EXT_CLASS CMainAutoTrendTool  
{
public:
	CMainAutoTrendTool();
	CMainAutoTrendTool(CWnd* pParent);
	virtual ~CMainAutoTrendTool();

	void SetProcessStarting(const bool bStarting);
	void Set(CWnd* pParent, CMainBlock* pMainBlock, CPacketList* pPacketList, const COLORREF& penColor, const int nPenThickness);
	void SetParent(CWnd* pParent);
	void SetMainBlock(CMainBlock* pMainBlock);
	void SetPacketList(CPacketList* pPacketList);
	void SetPenColor(const COLORREF& color);
	void SetPenThickness(const int nThickness);

	void Calculate(const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);

	bool DoesProcessStarting() const;

	// mouse event
	bool OnRButtonUp(CDC* pDC, const CPoint& point);
	bool OnMouseMove(CDC* pDC, const CPoint& point);

	// ±×¸®±â
	void OnDraw(CDC* pDC);

private:
	CMainAutoTrendImplementation* m_pMainAutoTrendImpl;
};

#endif // !defined(AFX_MAINAUTOTRENDTOOL_H__C4216A56_E5E8_42A0_94BF_8F8F470C0030__INCLUDED_)
