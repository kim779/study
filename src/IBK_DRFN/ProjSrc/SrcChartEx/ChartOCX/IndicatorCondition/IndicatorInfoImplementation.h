// IndicatorInfoImplementation.h: interface for the CIndicatorInfoImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORINFOIMPLEMENTATION_H__FD0ED1E0_4C20_4017_95D1_0CF063686D07__INCLUDED_)
#define AFX_INDICATORINFOIMPLEMENTATION_H__FD0ED1E0_4C20_4017_95D1_0CF063686D07__INCLUDED_

#include "IndicatorConditionBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDefaultIndicatorInfoBuilder;
// CGraph : 복수종목 - ojtaso (20070308)
class CGraph;
class CIndicatorInfoImplementation  
{
public:
	CIndicatorInfoImplementation( const EIndicatorInfoType p_eIndicatorInfoType);
	CIndicatorInfoImplementation( const CIndicatorInfoImplementation* pDefaultIndiInfoImpl);
	virtual ~CIndicatorInfoImplementation();

// (2006/12/9 - Seung-Won, Bae) Support Indicator Info Type.
protected:
	EIndicatorInfoType	m_eIndicatorInfoType;

public:
	EIndicatorInfoType	GetIndicatorInfoType( void)		{	return m_eIndicatorInfoType;	}

private:
	CGraphPartData m_graphPartData;
	CDoubleList m_calConditionList;		// 계산을 위해 필요한 조건값 list
	CDoubleList m_drawConditionList;	// 그리기위해 필요한 조건값 list
	CDoubleList m_baseLineList;			// 기준선 list
	// CGraph : 복수종목 - ojtaso (20070308)
	CGraph*		m_pGraph;

private:
	void CopyAllData(const CIndicatorInfoImplementation* pSourceData, BOOL p_bSameGraph = TRUE);
	void CopyDoubleList(const CDoubleList& sourceData, CDoubleList& targetData);
	void SetDoubleListInVerifiable(const CDoubleList& sourceData, CDoubleList& targetData);

	CString GetStringDoubleList(const CDoubleList& doubleList, const CString& strCompart) const;
	CIndiBaseData::TITLETYPE GetConditionTitle() const;
	CIndiBaseData::TITLETYPE GetConditionTitle(const CString& strIndicatorName) const;

	int SetDoubleListData(const int nIndex, const double& dData, CDoubleList& targetList);

	bool ChangeCalConditionDataFromSubGraphTitle();
	bool GetCalConditionDataFromSubGraphTitle(const CString& strOrgTitle, double& dCalData);

public:
	bool CopyDataWithVerificationFrom(const CIndicatorInfoImplementation& sourceData);

	void Initialize( void);
	bool ClearCommonCalConditionList();
	bool ClearDrawConditionList();
	bool ClearBaseLineList();

	CString GetGroupName() const;
	CString GetIndicatorName() const;
	CString	GetGraphName() const;
	const CGraphPartData& GetGraphPartData() const;
	CGraphPartData* GetGraphPartData();
	int GetCommonCalConditionCount() const;
	int GetDrawConditionCount() const;
	int GetBaseLineCount() const;
	int GetCalConditionCount(const int nSubGraphIndex) const;
	int GetCalConditionCount(const CString& strSubGraphName) const;
	const CDoubleList& GetCommonCalConditionList() const;
	CDoubleList* GetCommonCalConditionList();
	const CDoubleList& GetDrawConditionList() const;
	CDoubleList* GetDrawConditionList();
	const CDoubleList& GetBaseLineList() const;
	CDoubleList* GetBaseLineList();
	CDoubleList* GetCalConditionList(const int nSubGraphIndex) const;
	CDoubleList* GetCalConditionList(const int nSubGraphIndex);
	CDoubleList* GetCalConditionList(const CString& strSubGraphName) const;
	CDoubleList* GetCalConditionList(const CString& strSubGraphName);

	void GetGraphDataForMaking( CString &p_strGraphData) const;
	void GetAllSubGraphDataForMaking( CString &p_strGraphData) const;
//	CString GetAllSubGraphDataForMaking(const CGraphColorPointerList& graphColorList) const;
	CString GetStringCommonCalConditionList(const CString& strCompart = ',') const;
	CString GetStringDrawConditionList(const CString& strCompart = ',') const;
	CString GetStringBaseLineList(const CString& strCompart = ',') const;
	CGraphData& GetGraphData();
	CGraphData* GetGraphData_PointerType();
	void GetGraphDataDrawingData(bool& bCalcRiseFallRate_LowHigh, bool& bCalcRiseFallRate_LowCur, 
		bool& bCalcRiseFallRate_HighCur, bool& bCalcRate_PrevCur, int& nCalcRiseFallRate_CurData, BOOL &p_bShowOHLC) const;
	CString GetCommonPacketNames() const;
	CString GetPacketNames(const int nSubGraphIndex) const;
	CSignalData GetSignalData() const;
	CSignalData& GetSignalData();
	CSignalData* GetSignalData_PointerType();
	bool IsSignalUse() const;
	bool IsSignalShow() const;
	int GetSubGraphCount() const;
	bool GetAllSubGraphDataList(CSubGraphDataList& subGraphDataList) const;
	CSubGraphData* GetSubGraphData(const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const int nSubGraphIndex) const;
	int GetSubGraphIndex(const CString& strSubGraphName) const;
	CString GetSubGraphName(const int nIndex) const;
	CString GetSubGraphTitle(const int nIndex) const;
	bool GetSubGraphNameList(CStringDataList& SubGraphNameList) const;
	BOOL	GetSubGraphNames( CString &p_strSubGraphNames) const;	// (2006/5/31 - Seung-Won, Bae) To compare with Main Block Graph Objects
	CGraphColor GetSubGraphColor(const int nIndex) const;
	CGraphColor* GetSubGraphColor_PointerType(const int nIndex = 0) const;
	bool GetSubGraphColorList(CGraphColorPointerList& subGraphColorList) const;

	bool GetCopyDataFrom(const CIndicatorInfoImplementation* pIndiInfoImpl, BOOL p_bSameGraph = TRUE);
	int SetCommonCalConditionData(const int nIndex, const double& dData);
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetDrawConditionData(const int nIndex, const double& dData);
	int SetBaseLineData(const int nIndex, const double& dData);
	bool SetAllCalConditionData_SubGraphPart(const int nCondIndex, const double& dData);
	int SetCalConditionData(const int nSubGraphIndex, const int nCondIndex, const double& dData);

	bool SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck = true);
	bool SetCommonPacketNames(const CString& strPacketNames);
	bool SetAllPacketNames_SubGraphPart(const CString& strPacketNames, const bool bIsAutoCheck);
	bool SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames);
	void SetSignalData(const CSignalData& signalData);

	bool SetAllSubGraphDataList(const CSubGraphDataList* pSubGraphDataList, const bool bAutoCalConditionData = true);
	bool SetSubGraphData(const CSubGraphData* pSourceData);
	bool SetSubGraphTitle(const int nIndex, const CString& strTitle);
	bool SetSubGraphColor(const int nIndex, const CGraphColor& graphColor);
	bool SetSubGraphColor(const int nIndex, const COLORREF& lineColor);
	bool SetSubGraphPenThickness(const int nIndex, const CPenThickness& penThickness);
	bool SetSubGraphPenThickness(const int nIndex, const int nPenThickness);

	bool ChangeAllSubGraphTitleFromCalConditionData();

// (2006/5/28 - Seung-Won, Bae) for Old User Indicator Info. Add New SubGraph.
public:
	BOOL						AddSubGraphPartDataWithCopy( const CSubGraphPartData* pSubGraphPartData);
	CSubGraphPartData *			GetSubGraphPartData( int p_nIndex);

// (2006/5/29 - Seung-Won, Bae) Update CGraphDataDrawingData ('graphdatadrawing')
public:
	void						SetGraphDataDrawingDataWithCopy( const CGraphDataDrawingData * p_pGraphDataDrawingData);
	CGraphDataDrawingData *		GetGraphDataDrawingData( void);

// (2007/2/28 - Seung-Won, Bae) Retrieve Sub Graph Hiding
public:
	BOOL	GetSubGraphHiding( const char *p_szSubGraphName);
	BOOL	GetSubGraphHiding( const int p_nSubGraphIndex);

	// CGraph : 복수종목 - ojtaso (20070308)
	void	SetGraph(CGraph* pGraph);
	CGraph*	GetGraph();

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void	SetLoadedRQ(LPCTSTR lpszRQ);
	LPSTR	m_lpszLoadedRQ;
};

#endif // !defined(AFX_INDICATORINFOIMPLEMENTATION_H__FD0ED1E0_4C20_4017_95D1_0CF063686D07__INCLUDED_)
