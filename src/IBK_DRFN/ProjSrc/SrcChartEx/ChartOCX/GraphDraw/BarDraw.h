// BarDraw.h: interface for the CBarDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARDRAW_H__4CC99C75_48EA_4760_ABA7_1F752D3FAFA2__INCLUDED_)
#define AFX_BARDRAW_H__4CC99C75_48EA_4760_ABA7_1F752D3FAFA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"
#include "DataMath.h"
#include "Conversion.h"

#include "BaseData.h"
#include "PacketPointerList.h"

#include "PacketList.h"
#include "PacketBase.h"
#include "IndicatorInfo.h"

class CIndicatorInfo;

class CBarDraw : public CDraw 
{
public:
	CBarDraw();
	virtual ~CBarDraw();

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData);
	CRect GetDrawingRegion(CPacket* pPacket, const CRect& graphRegion, 
		const CString& strSubGraphName, const int nStartIndex, const int nEndIndex, 
		const double& dViewMin, const double& dViewMax) const;

	bool CheckBoxBarType(CPacket* pPacket);

protected:
	enum ENUM_BRUSH
	{
		UP_BRUSH,
		DOWN_BRUSH,
		BOHAP_BRUSH,

		BRUSH_COUNT
	};
	
	enum ENUM_PEN
	{
		UP_PEN,
		DOWN_PEN,
		BOHAP_PEN,

		PEN_COUNT
	};

protected:
	void CreatePenAndBrush(const CGraphColor& graphColor, 
		CPen& pen1, CPen& pen2, CPen& pen3, CBrush& brush1, CBrush& brush2, CBrush& brush3);
	void SelectPenAndBrushObject(CDC* pDC, CPen* pPen1, CPen* pPen2, CPen* pPen3, 
		CBrush* pBrush1, CBrush* pBrush2, CBrush* pBrush3, const CGraphColor& graphColor, 
		const int nIndex, const int nCurrentIndex, const int nHighestIndex);
	bool DoesVolumeDrawingType(CIndicatorInfo* pIndicatorInfo) const;
	bool DoesTextDrawing(const CGraphBaseData::BARDRAWSTYLE eDrawStyle) const;
	int GetTextType(const CIndicatorInfo* pIndicatorInfo) const;
	void GetBarWidth(const CRect& graphRegion, const int nDataCount, 
		double& dBarMaxWidth, int& nBarWidth);
	CString GetDrawingText(const int nTextType, const double& dTotalData, const double& dData) const;
	int GetDrawType(CIndicatorInfo* pIndicatorInfo, const bool bIsDrawType) const;
	CPacket* GetPacket(CPacket* pBasePacket, const CString& strPacketName) const;
	double GetTotalData(CList<double, double>* pDataList, const int nEndIndex) const;
	bool GetDatasInPacket(CPacket* pVolumePacket, CPacket* pOpenPacket, CPacket* pClosePacket, 
		const int nDataIndex, int& nDrawingIndex, CList<double, double>*& pVolumeList, POSITION& volumePos, 
		CList<double, double>*& pOpenList, POSITION& openPos, CList<double, double>*& pCloseList, POSITION& closePos) const;
	bool GetDatasInPacket(CPacket* pVolumePacket, CPacket* pClosePacket, const int nDataIndex, int& nDrawingIndex, 
		CList<double, double>*& pVolumeList, POSITION& volumePos, CList<double, double>*& pCloseList, POSITION& closePos) const;
	bool GetDatasInPacket(CPacket* pPacket, const int nDataIndex, int& nDrawingIndex, CList<double, double>*& pDataList, POSITION& pos) const;
	bool GetDatasInPacket(CPacket* pPacket, const int nDataIndex, CList<double, double>*& pDataList, POSITION& pos) const;
	bool GetPrevData(CList<double, double>* pDataList, const POSITION& curPos, const int nIndex, double& dPrevData) const;


	void CreateBrushsAndPens(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor, 
		const double& dWidth, const CPenThickness& penThickness, const bool bIsDrawingThickness);
	void DeleteBrushsAndPens(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen);
	void SelectGdiObject_VolumeChart(CDC* pDC, const double& dCurVolume, const double& dPrevVolume, 
		const CGraphColor& graphColor, CBrush* pBrush, CPen* pPen);
	void SelectGdiObject_PriceChart(CDC* pDC, const int nBohapType, const double& dOpen, const double& dClose, 
		const double& dPreClose, const CGraphColor& graphColor, CBrush* pBrush, CPen* pPen);
	void SelectGdiObject_HighLow(CDC* pDC, const bool bFill, const int nBrushIndex, 
		const int nPenIndex, CBrush* pBrush, CPen* pPen);
	void SelectGdiObject_Bohap(CDC* pDC, const int nBohapType, const double& dClose, 
		const double& dPreClose, const bool bFill, CBrush* pBrush, CPen* pPen);
	void SelectBrushObject(CDC* pDC, const bool bFill, const int nIndex, CBrush* pBrush);

};

#endif // !defined(AFX_BARDRAW_H__4CC99C75_48EA_4760_ABA7_1F752D3FAFA2__INCLUDED_)
