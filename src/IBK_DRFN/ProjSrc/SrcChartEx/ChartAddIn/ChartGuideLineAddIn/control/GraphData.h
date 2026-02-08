// GraphData.h: interface for the CGraphData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_)
#define AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "Color.h"

class CIndicatorInfo;

// Pen Thickness
class CPenThickness
{
public:
	CPenThickness& operator=(const CPenThickness& data);
	bool operator==(const CPenThickness& data) const;

public:
	CPenThickness();
	CPenThickness(const CString& strOnlyThicknessData);
	CPenThickness(const int nThickness1, const int nThickness2, const int nThickness3, const bool bIsHightlight = false);
	CPenThickness(const CPenThickness& penThickness);

	void SetData(const CString& strData);
	void SetHightlight(const bool bIsHightlight);
	void SetThickness(const int nThickness1, const int nThickness2, const int nThickness3);
	void SetThickness1(const int nThickness);
	void SetThickness2(const int nThickness);
	void SetThickness3(const int nThickness);
	void ReversedHightlight();

	bool IsHightlight() const;
	int GetThickness1() const;
	int GetThickness2() const;
	int GetThickness3() const;
	int GetDrawingThickness1() const;
	int GetDrawingThickness2() const;
	int GetDrawingThickness3() const;

	CString GetStringData() const;

private:
	bool m_bIsHightlight;	// 선 강조 유무 (true = 강조)
	int m_nThickness1;
	int m_nThickness2;
	int m_nThickness3;

	void SetIntData(const CString& strData, int& nData);
	int GetDrawingThickness(const int orgPenThickness) const;
};


// graph data를 보이는 여부에 대한 data
class CGraphDataDrawingData
{
public:
	CGraphDataDrawingData& operator=(const CGraphDataDrawingData& data);

public:
	CGraphDataDrawingData();
	CGraphDataDrawingData(const CString& strData);
	CGraphDataDrawingData(const bool bCalcRiseFallRate_LowHigh, const bool bCalcRiseFallRate_LowCur, 
		const bool bCalcRiseFallRate_HighCur, const bool bCalcRiseFallRate_PrevCur, const bool bCalcRate_PrevCur);
	CGraphDataDrawingData(const CGraphDataDrawingData& data);

	void SetDoesCalcRiseFallRate_LowHigh(const bool bCalcData);
	void SetDoesCalcRiseFallRate_LowCur(const bool bCalcData);
	void SetDoesCalcRiseFallRate_HighCur(const bool bCalcData);
	void SetDoesCalcRiseFallRate_PrevCur(const bool bCalcData);
	void SetDoesCalcRate_PrevCur(const bool bCalcData);

	bool DoesAllFalseData() const;
	bool DoesCalcRiseFallRate_LowHigh() const;
	bool DoesCalcRiseFallRate_LowCur() const;
	bool DoesCalcRiseFallRate_HighCur() const;
	bool DoesCalcRiseFallRate_PrevCur() const;
	bool DoesCalcRate_PrevCur() const;

	// viewdata 가져오기
	CString GetData() const;

private:
	bool m_bCalcRiseFallRate_LowHigh;	// 최저가 대비 최고가 등락률
	bool m_bCalcRiseFallRate_LowCur;	// 최저가 대비 현재가 등락률
	bool m_bCalcRiseFallRate_HighCur;	// 최고가 대비 현재가 등락률
	bool m_bCalcRiseFallRate_PrevCur;	// 전일가 대비 현재가 등락률 (현재가)
	bool m_bCalcRate_PrevCur;			// 전일비

	void SetData(const CString& strData);
};


// ============================================================================
// graph 에 대한 data
class CGraphData
{
public:
	CGraphData& operator=(const CGraphData& data);

public:
	CGraphData();
	CGraphData(const CString& strData);
	CGraphData(const int nFunctionIndex, const CString& functionName, const CString& packetNames, 
		const bool bSignal, const CGraphDataDrawingData& drawingData);
	CGraphData(const CGraphData& data);

	void SetFunctionIndex(const int nFunctionIndex);
	void SetFunctionName(const CString& functionName);
	void SetPacketNames(const CString& packetNames);
	void SetSignal(const bool bSignal);
	void SetGraphDataDrawingData(const CGraphDataDrawingData& drawingData);

	int GetFunctionIndex() const;
	CString GetFunctionName() const;
	CString GetPacketNames() const;
	bool IsSignal() const;
	CGraphDataDrawingData GetGraphDataDrawingData() const;

	// graph data 가져오기
	CString GetData() const;

private:
	int m_nFunctionIndex;		// 같은 function 의 index
	CString m_strFunctionName;	// 함수 name
	CString m_strPacketName;	// 해당 graph에 맞는 packet
	bool m_bSignal;				// Graph에 signal 표시
	CGraphDataDrawingData m_drawingData;	// Graph에 data 표시

	void SetData(const CString& strData);
};


// subGraph에 대한 data
class CSubGraphData  
{
public:
	CSubGraphData& operator=(const CSubGraphData& data);

public:
	CSubGraphData();
	CSubGraphData(const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness);
	CSubGraphData(const CSubGraphData& data);
	CSubGraphData(const CString& strData);

	void SetTitleEndPosition(const int nTitleEndPosition);
	void SetData(const CString& strData);
	void SetType(const CGraphBaseData::GRAPHTYPE type);
	void SetStyle(const CGraphStyle& style);
	void SetDrawStyle(const CGraphDrawStyle& drawStyle);
	void SetName(const CString& name);
	void SetTitle(const CString& title);
	void SetHiding(const bool hiding);
	void SetColor(const CGraphColor& color);
	void SetPenThickness(const CPenThickness& penThickness);

	int GetTitleEndPosition() const;
	CGraphBaseData::GRAPHTYPE GetType() const;
	CGraphStyle GetStyle() const;
	CGraphDrawStyle GetDrawStyle() const;
	CString GetName() const;
	CString GetTitle() const;
	bool GetHiding() const;
	CGraphColor GetColor() const;
	CGraphColor* GetColor_PointerType();
	CGraphColor& GetColor();
	CPenThickness GetPenThickness() const;
	CPenThickness& GetPenThickness();

	CString GetStringData() const;
	CString GetStringData(const CString& strTitle) const;

private:
	int m_nTitleEndPosition;
	CGraphBaseData::GRAPHTYPE m_TYPEType;	// graph type
	CGraphStyle m_nStyle;					// graph style
	CGraphDrawStyle m_nDrawStyle;			// graph draw style
	CString m_strName;		// graph name
	CString m_strTitle;		// graph title
	bool m_bHiding;			// 그릴것인지 여부
	CGraphColor m_clrColor;	// graph의 색들
	CPenThickness m_penThickness;	// graph pen 굵기

	CGraphBaseData::GRAPHTYPE GetType(const CString& strType) const;
};

#endif // !defined(AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_)
