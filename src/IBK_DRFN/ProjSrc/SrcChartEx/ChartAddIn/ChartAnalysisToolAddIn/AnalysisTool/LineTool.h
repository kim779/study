// LineTool.h: interface for the CLineTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINETOOL_H__D27C27DE_2A4B_4747_9E07_9D4059B5A9FA__INCLUDED_)
#define AFX_LINETOOL_H__D27C27DE_2A4B_4747_9E07_9D4059B5A9FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointTool.h"
#include "LineDrawer.h"

class CAnalysisData;
class CLineTool : public CTwoPointDownTool
{
public:
	CLineTool(CAnalysisData* pAnalysisData, const CAnalysis::TOOLTYPE p_eToolType, 
			const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight, const bool bRightRate=TRUE);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CLineDrawer m_lineDrawer;
protected:
	CAnalysis::TOOLTYPE				m_eToolType;
protected:
	virtual CAnalysis::TOOLTYPE		GetToolType() const;
};

// ----------------------------------------------------------------------------
// 수직선
class CVertLineTool : public COnePointMoveTool
{
public:
	CVertLineTool(CAnalysisData* pAnalysisData = NULL,const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextVertPosition& position);
	void SetTextPosition(const bool bTop, const bool bBottom);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

private:
	CTextVertPosition m_textPosition;
	CVertLineDrawer m_vertLineDrawer;
};

// ----------------------------------------------------------------------------
// 수평선
class CHorzLineTool : public COnePointMoveTool
{
public:
	CHorzLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

	// 그리는 영역들에 속한 point 인지의 여부.
	//virtual bool PointInDrawingRegion(const CPoint& point) const;

private:
	CTextHorzPosition m_textPosition;
	CHorzLineDrawer m_horzLineDrawer;
};

// ----------------------------------------------------------------------------
// 십자선
class CCrossLineTool
{
public:
	CCrossLineTool();

	void SetTextPosition(const CTextHorzVertPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight, const bool bTop, const bool bBottom);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	const CTextPosition* GetTextPosition() const;
	CTextPosition* GetTextPosition();
	COnePointDrawer& GetOnePointDrawer();

private:
	CTextHorzVertPosition m_textPosition;
	CCrossLineDrawer m_crossLineDrawer;
};

class CCrossLineMoveTool : public COnePointMoveTool, public CCrossLineTool
{
public:
	CCrossLineMoveTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();
};

class CCrossLineDownTool : public COnePointDownTool, public CCrossLineTool
{
public:
	CCrossLineDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();
};

// ----------------------------------------------------------------------------
// 삼등분선
class CTrisectionLineTool : public CTwoPointDownTool
{
public:
	CTrisectionLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CTrisectionLineDrawer m_trisectionLineDrawer;
};

// ----------------------------------------------------------------------------
// 사등분선
class CQuadrisectLineTool : public CTwoPointDownTool
{
public:
	CQuadrisectLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	
	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CQuadrisectLineDrawer m_quadrisectLineDrawer;
};

// ----------------------------------------------------------------------------
// 엔드류 피치포크
class CAndrewsPitchforkTool : public CThreePointDownTool
{
public:
	CAndrewsPitchforkTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CThreePointDrawer& GetThreePointDrawer();

private:
	CAndrewsPitchforkDrawer m_andrewsDrawer;
};

// ----------------------------------------------------------------------------
// 사이클 구간
class CCycleLinesTool : public CTwoPointDownTool //CTwoPointMoveTool
{
public:
	CCycleLinesTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	//virtual CPoints GetTwoPoints() const;
private:
	CCycleLinesDrawer m_cycleLinesDrawer;
};

// ----------------------------------------------------------------------------
// 스피드 라인
class CSpeedLineTool : public CTwoPointDownTool
{
public:
	CSpeedLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CSpeedLineDrawer m_speedLineDrawer;
};

// ----------------------------------------------------------------------------
// 스피드 Fan
class CSpeedFanTool : public CTwoPointDownTool
{
public:
	CSpeedFanTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CSpeedFanDrawer m_speedFanDrawer;
};

// ----------------------------------------------------------------------------
// 스피드 Arc
class CSpeedArcTool : public CTwoPointDownTool
{
public:
	CSpeedArcTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CSpeedArcDrawer m_speedArcDrawer;
};

// ----------------------------------------------------------------------------
// 선형회귀선 
class CLinearRegressionTool : public CTwoPointDownTool
{
public:
	CLinearRegressionTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();
private:
	CLinearRegressionDrawer m_linearRegressionDrawer;
};

// ----------------------------------------------------------------------------
// 선형회귀채널 
class CRaffRegressionTool : public CTwoPointDownTool
{
public:
	CRaffRegressionTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();
private:
	CRaffRegressionDrawer m_raffRegressionDrawer;
};

// ----------------------------------------------------------------------------
// 각도추세선 
class CAngleLineTool : public CTwoPointDownTool
{
public:
	CAngleLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextShowPosition& position);
	void SetTextPosition(const bool bShow);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);
	
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CAnalysis::TOOLTYPE	GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();
private:
	CTextShowPosition m_textPosition;
	CAngleLineDrawer m_angleLineDrawer;
};

// ----------------------------------------------------------------------------
// Candle중심선 
class CCandleLineTool : public COnePointMoveTool
{
public:
	CCandleLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextShowPosition& position);
	void SetTextPosition(const bool bShow);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();
private:
	CTextShowPosition m_textPosition;
	CCandleLineDrawer m_candleLineDrawer;
};

// ----------------------------------------------------------------------------
// 허수봉
class CCandleBongTool : public CTwoPointDownTool
{
public:
	CCandleBongTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CCandleBongDrawer m_candleBongDrawer;
};

// ----------------------------------------------------------------------------
// 연필추세선
class CPencilLineTool : public CMultiPointDownTool
{
public:
	CPencilLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CMultiPointDrawer& GetMultiPointDrawer();

private:
	CPencilLineDrawer m_pencilLineDrawer;
};

// ----------------------------------------------------------------------------
// 기간요약
class CPeriodSumLineTool : public CTwoPointDownTool
{
public:
	CPeriodSumLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	
	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CPeriodSumLineDrawer m_periodSumLineDrawer;
};

// ----------------------------------------------------------------------------
// ElliotWaveLine
class CElliotLineTool : public CTwoPointDownTool
{
public:
	CElliotLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	
protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CElliotLineDrawer m_elliotLineDrawer;
};

// ----------------------------------------------------------------------------
// 조정 백분율 
class CAdjustRatioTool : public CThreePointDownTool
{
public:
	CAdjustRatioTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CThreePointDrawer& GetThreePointDrawer();

private:
	CAdjustRatioDrawer m_adjustRatioDrawer;
};
#endif // !defined(AFX_LINETOOL_H__D27C27DE_2A4B_4747_9E07_9D4059B5A9FA__INCLUDED_)
