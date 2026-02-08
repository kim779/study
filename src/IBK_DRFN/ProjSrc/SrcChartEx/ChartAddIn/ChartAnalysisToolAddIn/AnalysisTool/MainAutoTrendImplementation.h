// MainAutoTrendImplementation.h: interface for the CMainAutoTrendImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINAUTOTRENDIMPLEMENTATION_H__3FBAAB99_D40B_43D2_81CF_B7F6C197B717__INCLUDED_)
#define AFX_MAINAUTOTRENDIMPLEMENTATION_H__3FBAAB99_D40B_43D2_81CF_B7F6C197B717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoTrendData.h"
#include "AutoTrendDrawer.h"

interface IChartManager;
class CAutoTrendDrawingCalculatorData;
class CCoordinate;

class CMainAutoTrendImplementation  
{
public:
	CMainAutoTrendImplementation( IChartManager *p_pIChartManager);
	CMainAutoTrendImplementation( IChartManager *p_pIChartManager, CWnd* pParent);
	virtual ~CMainAutoTrendImplementation();

	void SetProcessStarting( IChartManager *p_pIChartManager, const bool bStarting);
	void Set(CWnd* pParent, IChartManager* pMainBlock, IPacketManager* pPacketList, const COLORREF& penColor, const int nPenThickness);
	void SetParent(CWnd* pParent);
	void SetMainBlock(IChartManager* pMainBlock);
	void SetPacketList(IPacketManager* pPacketList);
	void SetPenColor(const COLORREF& color);
	void SetPenThickness(const int nThickness);

	void Calculate( IChartManager *p_pIChartManager, const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	
	bool DoesProcessStarting() const;

	// mouse event
	bool OnRButtonUp(CDC* pDC, const CPoint& point);
	bool OnMouseMove(CDC* pDC, const CPoint& point);

	// 그리기
	void OnDraw(CDC* pDC);

private:
	CWnd* m_pParent;
	IChartManager* m_pIChartManager;
	IPacketManager* m_pPacketList;
	bool m_bProcessStarting;
	bool m_bPrevPointUsed;
	CPoint m_prevPoint;
	CAutoTrendDrawingCalculatorData m_calData;
	CAutoTrendDrawer m_drawer;

	// data 초기화
	void Initialize( IChartManager *p_pIChartManager, const bool bIsAllData);

	bool DoesStarting() const;
	bool GetAutoTrendDrawingCalData(const CPoint& point, CAutoTrendDrawingCalculatorData& calData) const;

	// 그리기
	void EraseLastToolDrawing(CDC* pDC, const CRect& noFrameGraphRegion, const CPoint& point);
	void DrawNewToolDrawing(CDC* pDC, const CAutoTrendDrawingCalculatorData& calData, const CPoint& point);

	// 커서 바꾸기
	void SendMessageToParent(const bool bIsProcessStarting);
};

#endif // !defined(AFX_MAINAUTOTRENDIMPLEMENTATION_H__3FBAAB99_D40B_43D2_81CF_B7F6C197B717__INCLUDED_)
