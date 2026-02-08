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
#include "./Dll_Load/GraphDrawer.h"
#include "./CalcDataForDraw.h"
class CIndicatorInfo;

// (2006/9/5 - Seung-Won, Bae) for Import in Extension DLL
#ifndef AFX_EXT_CLASS_INDICOND
	#ifdef _INDICOND_EXPORT									// in IndicatorCondition
		#define AFX_EXT_CLASS_INDICOND	AFX_CLASS_EXPORT
//	#elif defined _DD_NOT_IMPORT							// in Lib or Regular used in IndicatorCondition.
//		#define AFX_EXT_CLASS_DD			
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_INDICOND	AFX_CLASS_IMPORT
	#endif
#endif

// Pen Thickness
class AFX_EXT_CLASS_INDICOND CPenThickness
{
public:
	CPenThickness& operator=(const CPenThickness& data);
	bool operator==(const CPenThickness& data) const;

public:
	CPenThickness();
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	CPenThickness(const int nThickness1, const int nThickness2, const int nThickness3, const int nBaseLineThickness, const bool bIsHightlight = false);
	CPenThickness(const CPenThickness& penThickness);

	void SetData( const char *p_szPenThickness);
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

	void	GetStringData( CString &p_strPenThickness) const;
	
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	void SetBaseLineThickness(const int nThickness);
	int GetBaseLineThickness() const;

private:
	bool m_bIsHightlight;	// 선 강조 유무 (true = 강조)
	int m_nThickness1;
	int m_nThickness2;
	int m_nThickness3;

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	int m_nBaseLineThickness;

	int GetDrawingThickness(const int orgPenThickness) const;
};

class AFX_EXT_CLASS_INDICOND CGraphDataDrawingData
{
public:
	CGraphDataDrawingData& operator=(const CGraphDataDrawingData& data);
	bool operator==(const CGraphDataDrawingData& data);

public:
	CGraphDataDrawingData();
	CGraphDataDrawingData( const char *p_szGraphDataDrawingData);
	CGraphDataDrawingData(const bool bCalcRiseFallRate_LowHigh, const bool bCalcRiseFallRate_LowCur, 
		const bool bCalcRiseFallRate_HighCur, const bool bCalcRate_PrevCur, const int nCalcRiseFallRate_CurData, const BOOL p_bShowOHLC);
	CGraphDataDrawingData(const CGraphDataDrawingData& data);

	void	SetDoesCalcRiseFallRate_LowHigh(const bool bCalcData);
	void	SetDoesCalcRiseFallRate_LowCur(const bool bCalcData);
	void	SetDoesCalcRiseFallRate_HighCur(const bool bCalcData);
	void	SetDoesCalcRate_PrevCur(const bool bCalcData);
	void	SetCalcRiseFallRate_CurData(const int nCalcData);
	void	SetShowOHLC( const BOOL p_bShowOHLC);	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data

	bool	DoesAllDefaultData() const;
	bool	DoesCalcRiseFallRate_LowHigh() const;
	bool	DoesCalcRiseFallRate_LowCur() const;
	bool	DoesCalcRiseFallRate_HighCur() const;
	bool	DoesCalcRate_PrevCur() const;
	int		GetCalcRiseFallRate_CurData() const;
	BOOL	GetShowOHLC( void) const;				// (2009/9/12 - Seung-Won, Bae) for OHLC graph data

	// viewdata 가져오기
	void GetData( CString &p_strGraphDataDrawingData) const;

private:
	bool	m_bCalcRiseFallRate_LowHigh;	// 최저가 대비 최고가 등락률
	bool	m_bCalcRiseFallRate_LowCur;	// 최저가 대비 현재가 등락률
	bool	m_bCalcRiseFallRate_HighCur;	// 최고가 대비 현재가 등락률
	bool	m_bCalcRate_PrevCur;			// 전일비
	int		m_nCalcRiseFallRate_CurData;	// 전일가 대비 현재가 등락률 (현재가)
	BOOL	m_bShowOHLC;	// (2009/9/12 - Seung-Won, Bae) for OHLC graph data

	void SetData( const char *p_szGraphDataDrawingData);

// (2006/5/9 - Seung-Won, Bae) Clear Data
public:
	void	ClearData( void);
};

// ============================================================================
// signal에 대한 data
class AFX_EXT_CLASS_INDICOND CSignalData
{
public:
	CSignalData& operator=(const CSignalData& data);
	bool operator==(const CSignalData& data);

public:
	CSignalData(const bool bIsUse = false, const bool bIsShow = false);
	CSignalData( const char *p_szSignalData);
	CSignalData(const CSignalData& data);
	~CSignalData();

	void SetUse(const bool bIsUse);
	void SetShow(const bool bIsShow);

	bool IsUse() const;
	bool IsShow() const;
	bool IsDraw() const;

	// signal data 가져오기
	void GetData( CString &p_strSignal) const;

private:
	void SetData( const char *p_szSignalData);

private:
	bool m_bIsUse;	// Graph에 signal(신호) 사용유무
	bool m_bIsShow;	// Graph에 signal(신호) 표시

// (2006/5/9 - Seung-Won, Bae) Clear Data
public:
	void	ClearData( void);

// (2009/10/7 - Seung-Won, Bae) for Signal Arrow Drawing Cache.
public:
	CCalcDataForDraw *	m_pSignalCache;
public:
	void				ClearCalcDataForDraw( void);
};

// (2006/11/22 - Seung-Won, Bae) the Indicator Name can be setted in Only CDefaultIndicatorInfoBuilder
class CGraphDataBase
{
protected:
	friend class	CDefaultIndicatorInfoBuilder;		// for Load User or Map Default Indicator Info
	friend class	CIndicatorListImplementation;		// for Create new Graph Indicator Info.
	friend class	CVertScaleGroup;					// for Create new Graph Name of Old Map with multiple same-indicator.
	friend class	CNotDefaultUserTypeIndiListBuilder;	// for Load User Indicator Info with Old Version Converting.
	friend class	CIndicatorInfoImplementation;		// for Copy of Pair Tick.
	virtual void	SetIndicatorName(	const CString &p_strIndicatorName)	= 0;
	virtual void	SetGraphName(		const CString &p_strGraphName)		= 0;
};

// graph 에 대한 data
class AFX_EXT_CLASS_INDICOND CGraphData : public CGraphDataBase
{
public:
	CGraphData()	{}							// (2006/11/26 - Seung-Won, Bae) Created by CIndicatorInfo Consturctor.
	CGraphData( const char *p_szGraphData, HWND p_hOcxWnd);		// (2006/9/6 - Seung-Won, Bae) Created by AddGraph or AddChartBlock of OCX or BuildGraphsAndScales in Map[CL] Loaded.

// (2006/9/6 - Seung-Won, Bae) Save Map Design Info.
public:
	// 복수종목 저장 : 복수종목 - ojtaso (20070507)
	void					GetData( CString &p_strGraphData, LPCTSTR lpszRQ = NULL) const;

public:
	CGraphData &operator	=( const CGraphData& data);
	void					ClearData( void);				// (2006/5/9 - Seung-Won, Bae) Clear Data

// 해당 graph에 맞는 packet
protected:
	CString	m_strPacketName;
public:
	CString	GetPacketNames( void) const;
	void	SetPacketNames( const CString& packetNames);

// Graph에 signal 사용여부 및 표시
protected:
	CSignalData	m_signalData;
public:
	CSignalData		GetSignalData(	void) const;
	CSignalData &	GetSignalData(	void);
	bool			IsSignalUse(	void) const;
	bool			IsSignalShow(	void) const;
	void			SetSignalData(	const CSignalData& signalData);
	void			SetSignalData(	const bool bIsUse, const bool bIsShow);

// Graph에 data 표시
protected:
	CGraphDataDrawingData	m_graphDataDrawingData;
public:
	CGraphDataDrawingData	GetGraphDataDrawingData( void) const;
	CGraphDataDrawingData &	GetGraphDataDrawingData( void);
	void					SetGraphDataDrawingData( const CGraphDataDrawingData& drawingData);

// (2006/11/26 - Seung-Won, Bae) Support Indicator Name & Graph Name (Support Indicator Name with Graph Name in temporary.)
protected:
	CString			m_strIndicatorName;
	CString			m_strGraphName;
protected:
	virtual void	SetIndicatorName(	const CString &p_strIndicatorName)	{	m_strIndicatorName	= p_strIndicatorName;	}
	virtual void	SetGraphName(		const CString &p_strGraphName)		{	m_strGraphName		= p_strGraphName;		}
protected:
public:
	CString			GetGraphName( void) const								{	return m_strGraphName;						}
	CString			GetIndicatorName( void) const							{	return m_strIndicatorName;					}
public:
	CString			GetGraphName2( void) const								{	return GetGraphName();						}
	CString			GetIndicatorName2( void) const							{	return GetIndicatorName();					}
};

// KTB 체결잔고표시 - ojtaso (20091019)
typedef struct _ST_HORZLINE
{
	UINT m_nIndex;
	CString m_strTitle;
	COLORREF m_clrLine;
	unsigned short m_nLine;
	double m_dPrice;
}ST_HORZLINE;

// subGraph에 대한 data
class AFX_EXT_CLASS_INDICOND CSubGraphDataBase
{
public:
	CSubGraphDataBase& operator=(const CSubGraphDataBase& data);

public:
	CSubGraphDataBase();
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	CSubGraphDataBase( const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle);
	CSubGraphDataBase( const CSubGraphDataBase& data);
	CSubGraphDataBase( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName);
	~CSubGraphDataBase();

	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	void SetTitleEndPosition(const int nTitleEndPosition, const int nTitleEndLine = 1);
	void SetData( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName);
	void SetType(const CGraphBaseData::GRAPHTYPE type);
	void SetStyle(const CGraphStyle& style);
	void SetDrawStyle(const CGraphDrawStyle& drawStyle);
	void SetName(const CString& name);
	void SetTitle(const CString& title);
	void SetHiding(const bool hiding);
	void SetColor(const CGraphColor& color);
	void SetPenThickness(const CPenThickness& penThickness);

	void ChangeKeyPart(const CSubGraphDataBase& data);
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	void SetItemName(const CString& strItemName);
	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	void SetSubGraphIndex(const int nSubGraphIndex);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	void SetPaintStyle(const CGraphPaintStyle& nPaintStyle);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	void SetBaseLineStyle(const CGraphBaseLineStyle& nBaseLineStyle);

	int GetTitleEndPosition() const;
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	int GetTitleEndLine() const;
	CGraphBaseData::GRAPHTYPE GetType() const;
	CGraphStyle GetStyle() const;
	CGraphDrawStyle GetDrawStyle() const;
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	CGraphPaintStyle GetPaintStyle() const;
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	CGraphBaseLineStyle GetBaseLineDrawStyle() const;
	CString GetName() const;
	CString GetTitle() const;
	CGraphColor GetColor() const;
	CGraphColor* GetColor_PointerType();
	CGraphColor& GetColor();
	CPenThickness GetPenThickness() const;
	CPenThickness& GetPenThickness();

	void GetStringData( CString &p_strSubGraphData) const;
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	CString GetItemName() const;

	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	int GetSubGraphIndex() const;

	// KTB 체결잔고표시 - ojtaso (20091019)
	long AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void DeleteHorizontalLine(long nIndex);
	void DeleteAllHorizontalLine();

	const CArray<ST_HORZLINE*, ST_HORZLINE*>& GetHorizontalLine() const { return m_arrHorzLine; }

protected:
	int m_nTitleEndPosition;
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	int m_nTitleEndLine;
	CGraphBaseData::GRAPHTYPE m_TYPEType;	// graph type
	CGraphStyle m_nStyle;					// graph style
	CGraphDrawStyle m_nDrawStyle;			// graph draw style
	CGraphPaintStyle m_nPaintStyle;			// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	CGraphBaseLineStyle m_nBaseLineStyle;		// 기준선 두께, 모양 설정 - ojtaso (20071213)
	
	CString m_strName;		// graph name
	CString m_strTitle;		// graph title
	bool m_bHiding;			// 그릴것인지 여부
	CGraphColor m_clrColor;	// graph의 색들
	CPenThickness m_penThickness;	// graph pen 굵기
	// 종목명 표시 : 복수종목 - ojtaso (20070625)
	CString m_strItemName;	// 종목명
	int m_nSubGraphIndex;	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)

	// KTB 체결잔고표시 - ojtaso (20091019)
	CArray<ST_HORZLINE*, ST_HORZLINE*>	m_arrHorzLine;
};
// (2007/2/28 - Seung-Won, Bae) Do not support Sub Graph Hide Info with directly.
//		Just Use the CSubGraph's GetHiding(); or CGraphPartData's GetHiding( p_nIndex);
//		It is for Graph Hiding.
class AFX_EXT_CLASS_INDICOND CSubGraphData : public CSubGraphDataBase
{
public:
	CSubGraphData();
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	CSubGraphData( const CGraphBaseData::GRAPHTYPE type, const CGraphStyle& style, const CGraphDrawStyle& drawStyle, const CString& name, const CString& title, const bool hiding, const CGraphColor& color, const CPenThickness& penThickness, const CGraphPaintStyle& paintStyle, const CGraphBaseLineStyle& baseLineStyle);
	CSubGraphData( const CSubGraphData& data);
	CSubGraphData( const char *p_szSubGraphData, HWND p_hOcxWnd, const char *p_szIndicatorName);
	~CSubGraphData();

	CCalcDataForDraw *	m_pGraphCache; //차트 성능 개선 관련. 계산된 값.
	void	ClearCalcDataForDraw( void);

private:
friend class CSubGraph;
friend class CSubGraphPartData;
friend class CGraphPartData;
friend class CUserTypeIndiInfoBuilder;
	bool GetHiding() const;

public:
	CSubGraphData& operator=(const CSubGraphData& data);
};

#endif // !defined(AFX_GRAPHDATA_H__075DBCF9_ED81_4895_9B71_891B5C1713A7__INCLUDED_)
