// SpecialDraw.h: interface for the CSpecialDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECIALDRAW1_H__DD05F85F_6C7F_4D59_9C99_3AD54B5C182D__INCLUDED_)
#define AFX_SPECIALDRAW1_H__DD05F85F_6C7F_4D59_9C99_3AD54B5C182D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class CPacketList;
class CIndicatorInfo;

class CSpecialDraw : public CDraw  
{
public:
	CSpecialDraw();
	virtual ~CSpecialDraw();

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData);

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager);

private:
	// modified by rich 02/09/17
	// InverseLine, ThreeLineBreak 의 penThickness 추가
	bool DrawInverseLine(CDC* pDC, const CRect& graphRegion, 
		CIndicatorInfo* pIndicatorInfo, const CEtcGraphColor& etcGraphColor, const CGraphColor& graphColor, 
		const double& dViewMax, const double& dViewMin, const double& dViewXMax, const double& dViewXMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, const bool bIsInvert,
		HWND p_hOcxWnd);
	bool DrawThreeLineBreak(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, const double& dViewMax, const double& dViewMin);
	bool DrawThreeLineBreak(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness,
		const CString& strRQ, CXScaleManager* pxScaleManager, const double& dViewMax, const double& dViewMin, int p_nColumn);
	bool	DrawPF( CDC* pDC, const CRect& graphRegion, CIndicatorInfo* pIndicatorInfo, const CGraphColor& graphColor, const double& dViewMin,
				const double& dViewMax, const CList< CPacket *, CPacket *> &packetList, HWND p_hOcxWnd);
	bool DrawPAndF(CDC* pDC, const CList<double, double>* pHighList, const CList<double, double>* pLowList, 
		const CRect& graphRegion, const CGraphColor& graphColor, 
		const double& dViewMin, const double& dViewMax, const double& dUnitValue);
	bool	DrawPF( CDC* pDC, const CRect& graphRegion, CIndicatorInfo* pIndicatorInfo, const CGraphColor& graphColor, const double& dViewMin,
				const double& dViewMax, const CList< CPacket *, CPacket *> &packetList,
				const CString& strRQ, CXScaleManager* pxScaleManager, HWND p_hOcxWnd, int p_nColumn);
	bool DrawPAndF(CDC* pDC, const CList<double, double>* pHighList, const CList<double, double>* pLowList, 
		const CRect& graphRegion, const CGraphColor& graphColor, 
		const double& dViewMin, const double& dViewMax, const double& dUnitValue,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawKagi(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const double& dViewMax, const double& dViewMin);
	bool DrawKagi(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList,
		const CString& strRQ, CXScaleManager* pxScaleManager, const double& dViewMax, const double& dViewMin, int p_nColumn);
// (2006.11.09 안승환) 분산차트 추가
	bool DrawDisperSion(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const double& dViewMax, const double& dViewMin);
// (2006.11.09 안승환) 분산차트 추가

};

class CPAndFDraw : public CDraw
{
public:
	bool	GetPacketData( const CList< CPacket *, CPacket *> &packetList, CPacket*& pDatePacket, CList<double, double>*& pHighList,
				CList<double, double>*& pLowList) const;
	double	GetUintHeight( const CRect& region, const double& dViewMin, const double& dViewMax, const double& dUnitValue) const;
	double	GetUnitValue( CPacketList* pPacketList, CIndicatorInfo* pIndicatorInfo) const;

	void DrawOAndX(CDC* pDC, const CRect& graphRegion, 
		const double& dMin, const double& dMax, const int nIndex, const double& dData, 
		const int nUnitCount, const double& dUnitHeight, const double& dUnitValue, const bool bIsX,const int nDataCount);

	void DrawOAndX(CDC* pDC, const CRect& graphRegion, 
		const double& dMin, const double& dMax, const int nIndex, const double& dData, 
		const int nUnitCount, const double& dUnitHeight, const double& dUnitValue, const bool bIsX,const int nDataCount, int nLeft, int nRight);
	void DrawDate(CDC* pDC, CPacket* pDatePacket, CPacket* pHighPacket, const CRect& graphRegion, 
		const CGraphColor& graphColor, const double& dUnitValue,const double& dUnitCount, HWND p_hOcxWnd);

private:
	bool GetPacketMinMax(CPacket* pPacket, double& dMin, double& dMax) const;
	bool GetConditionValue(CIndicatorInfo* pIndicatorInfo, const int nIndex, double& dValue) const;
	bool GetListValue(CList<double, double>* pList, const int nIndex, double& dValue) const;
	CRect GetTextRegion(CDC* pDC, const CRect& graphRegion, const CList<CString, CString>& strTextList) const;
	CSize GetTextSize(CDC* pDC, const CList<CString, CString>& strTextList) const;
//	bool GetText(CPacket* pDatePacket, const double& dUnitValue, CList<CString, CString>& strTextList) const;
	bool GetText(CPacket* pDatePacket, CPacket* pHighPacket, const double& dUnitValue, const double& dUnitCount,CList<CString, CString>& strTextList, HWND p_hOcxWnd) const;
			CString GetDateText(CPacket* pDatePacket) const;
//	CString GetUnitCountText(const double& dUnitCount) const;

	void DrawX(CDC* pDC, const CPoint& pt1, const CPoint& pt2);
	void DrawTextOutline(CDC* pDC, const CRect& textRegion, const int nLineGap, const COLORREF& penColor);
};

class CRenkoDraw : public CDraw
{
public:
	bool DrawRenko(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		 const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, 
		 const double& dDataMin, const double& dDataMax, const CDisplayAttributes& displayAttributes, HWND p_hOcxWnd);
	bool DrawRenko(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		 const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, 
		 const double& dDataMin, const double& dDataMax, const CDisplayAttributes& displayAttributes,
		 const CString& strRQ, CXScaleManager* pxScaleManager, HWND p_hOcxWnd, int p_nColumn);

private:
	bool GetDataList(const CList<CPacket*, CPacket*>& packetList, CList<double, double>*& pHighDataSave, 
		CList<double, double>*& pLowDataSave, CList<double, double>*& pTypeDataSave) const;
	void GetEctEnvironment(const CRect& graphRegion, const CDisplayAttributes& displayAttributes, 
		double& dRealWidth, int& nPrevPt_x, int& nStartIndex, int& nEndIndex);
	int GetBlockType(const CList<double, double>* pTypeDataSave, POSITION& pos) const;

	void CreateGdiObject(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor);
	void DestroyGdiObject(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen);
	void SelectGdiObject(CDC* pDC, const int nType, CBrush* pBrush, CPen* pPen);

	void DrawDate(CDC* pDC, CPacket* pDatePacket, CPacket* pHighDataPacket, const CIndicatorInfo* pIndicatorInfo, 
		const CRect& graphRegion, const CGraphColor& graphColor, HWND p_hOcxWnd);
	void DrawTextOutline(CDC* pDC, const CRect& textRegion, const int nLineGap, const COLORREF& penColor);

	bool GetText(const CPacket* pDatePacket, CPacket* pHighDataPacket, const CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& strTextList, HWND p_hOcxWnd) const;
	CString GetDateText(CPacket* pDatePacket) const;
	CString GetRengaText(CPacket* pHighDataPacket, const CIndicatorInfo* pIndicatorInfo, HWND p_hOcxWnd) const;

	CRect GetTextRegion(CDC* pDC, const CRect& graphRegion, const CList<CString, CString>& strTextList) const;
	CSize GetTextSize(CDC* pDC, const CList<CString, CString>& strTextList) const;
	bool IsTextDrawing(const CRect& graphRegion, const CRect& textRegion) const;

private:
	enum ENUM_GDI_TYPE
	{
		UP_TYPE = 0,
		DOWN_TYPE,

		GDI_COUNT
	};
};

#endif // !defined(AFX_SPECIALDRAW1_H__DD05F85F_6C7F_4D59_9C99_3AD54B5C182D__INCLUDED_)
