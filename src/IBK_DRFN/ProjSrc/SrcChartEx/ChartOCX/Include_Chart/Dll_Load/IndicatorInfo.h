// IndicatorInfo.h: interface for the CIndicatorInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORINFO_H__EE00E300_03E4_4B7C_88DE_CFB7991E43C4__INCLUDED_)
#define AFX_INDICATORINFO_H__EE00E300_03E4_4B7C_88DE_CFB7991E43C4__INCLUDED_

#include <afxtempl.h>

#include "IndicatorList.h"					// for EIndicatorListType

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndicatorInfoImplementation;
class CGraphTypeAndStyleData;
class CGraphPartData;
class CGraphColor;
class CSignalData;
class CGraphData;
class CSubGraphData;
class CPenThickness;
class CSubGraphPartData;
class CGraphDataDrawingData;
class CViewMinMaxDrawingData;
// CGraph : 복수종목 - ojtaso (20070308)
class CGraph;

class AFX_EXT_CLASS CIndicatorInfo
{
private:
	CIndicatorInfoImplementation* m_pIndiInfoImpl;

public:
	static CIndicatorInfo *	Make(const CIndicatorInfo* pDefaultIndicatorInfo);
public:
	CIndicatorInfo( const EIndicatorInfoType p_eIndicatorInfoType);
	virtual ~CIndicatorInfo();

protected:	// for Deny NULL Parameter.
	CIndicatorInfo( const CIndicatorInfo* pDefaultIndicatorInfo);

public:
	EIndicatorInfoType	GetIndicatorInfoType( void) const;

// (2006/5/28 - Seung-Won, Bae) Item Copy with Verification.
//		groupName			:	Not Supported.
//		graphName			:	?
//		packet				:	Not Supported. (Some error can occur in calculation.) 
//		cal_cnd				:	Supported in same count.
//		draw_cnd			:	Supported in same count.
//		baseline			:	Supported.
//		signal				:	Supported in signal used.
//		graphdatadrawing	:	Supported.
//		minmaxdatadrawing	:	Supported.
//		<SubGraph>			:	Supported in same Name of Default CFG.
//		type				:	Supported in same type.					(Site Defult CFG)
//		type_int			:	Supported in same type.					(User or User Default CFG)
//		sub_packet			:	Supported in Not-Null String.
//		sub_cal_cnd			:	Supported in same count.
//		title				:	Supported.
//		color				:	Supported.
//		hide				:	Supported.
//		linethickness		:	Supported.
public:
	bool CopyDataWithVerificationFrom(const CIndicatorInfo& sourceData);

public:
	CString GetGroupName() const;
	CString GetIndicatorName() const;
	CString	GetGraphName( void) const;
	CGraphPartData* GetGraphPartData() const;
	CGraphPartData* GetGraphPartData();
	int GetCommonCalConditionCount() const;
	int GetDrawConditionCount() const;
	int GetBaseLineCount() const;
	int GetCalConditionCount(const int nSubGraphIndex) const;
	int GetCalConditionCount(const CString& strSubGraphName) const;
	CList<double, double>* GetCommonCalConditionList() const;
	CList<double, double>* GetCommonCalConditionList();
	CList<double, double>* GetDrawConditionList() const;
	CList<double, double>* GetDrawConditionList();
	CList<double, double>* GetBaseLineList() const;
	CList<double, double>* GetBaseLineList();
	CList<double, double>* GetCalConditionList(const int nSubGraphIndex) const;
	CList<double, double>* GetCalConditionList(const int nSubGraphIndex);
	CList<double, double>* GetCalConditionList(const CString& strSubGraphName) const;
	CList<double, double>* GetCalConditionList(const CString& strSubGraphName);

	bool DoesEqualPacketNames_SubGraphPart(const int nSubGraphIndex, const CString& strData) const;

	void GetGraphDataForMaking( CString &p_strGraphData) const;
	void GetAllSubGraphDataForMaking( CString &p_strAllSubGraphData) const;
//	CString GetAllSubGraphDataForMaking(const CList<CGraphColor*, CGraphColor*>& graphColorList) const;
	CString GetStringCommonCalConditionList(const CString& strCompart = ',') const;
	CString GetStringDrawConditionList(const CString& strCompart = ',') const;
	CString GetStringBaseLineList(const CString& strCompart = ',') const;
	CGraphData& GetGraphData();
	CGraphData* GetGraphData_PointerType();
	void GetGraphDataDrawingData( bool& bCalcRiseFallRate_LowHigh, bool& bCalcRiseFallRate_LowCur, 
		bool& bCalcRiseFallRate_HighCur, bool& bCalcRate_PrevCur, int& nCalcRiseFallRate_CurData, BOOL &p_bShowOHLC) const;
	CString GetCommonPacketNames() const;
	CString GetPacketNames(const int nSubGraphIndex) const;
	CSignalData GetSignalData() const;
	CSignalData& GetSignalData();
	CSignalData* GetSignalData_PointerType();
	bool IsSignalUse() const;
	bool IsSignalShow() const;
	int GetSubGraphCount() const;
	bool GetAllSubGraphDataList(CList<CSubGraphData*, CSubGraphData*>& subGraphDataList) const;
	CSubGraphData* GetSubGraphData(const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const int nSubGraphIndex) const;
	int GetSubGraphIndex(const CString& strSubGraphName) const;
	CString GetSubGraphName(const int nIndex = 0) const;
	CString GetSubGraphTitle(const int nIndex = 0) const;
	bool GetSubGraphNameList(CList<CString, CString>& SubGraphNameList) const;
	BOOL	GetSubGraphNames( CString &p_strSubGraphNames) const;	// (2006/5/31 - Seung-Won, Bae) To compare with Main Block Graph Objects
	CGraphColor GetSubGraphColor(const int nIndex = 0) const;
	CGraphColor* GetSubGraphColor_PointerType(const int nIndex = 0) const;
	bool GetSubGraphColorList(CList<CGraphColor*, CGraphColor*>& subGraphColorList) const;

	bool GetCopyDataFrom(const CIndicatorInfo* pIndicatorInfo, BOOL p_bSameGraph = TRUE);
//	void SetCommonCalConditionData(const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetCommonCalConditionData(const int nIndex, const double& dData);
//	void SetDrawConditionData(const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetDrawConditionData(const int nIndex, const double& dData);
	int SetBaseLineData(const int nIndex, const double& dData);
	bool SetAllCalConditionData_SubGraphPart(const int nCondIndex, const double& dData);
//	bool SetCalConditionData(const int nSubGraphIndex, const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetCalConditionData(const int nSubGraphIndex, const int nCondIndex, const double& dData);

	bool SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck = true);
	bool SetCommonPacketNames(const CString& strPacketNames);
	bool SetAllPacketNames_SubGraphPart(const CString& strPacketNames, const bool bIsAutoCheck);
	bool SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames);
	void SetSignalData(const CSignalData& signalData);

	bool SetAllSubGraphDataList(const CList<CSubGraphData*, CSubGraphData*>* pSubGraphDataList, const bool bAutoCalConditionData = true);
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
	CGraph*	GetGraph() const;
	CGraph*	GetGraph();

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void	SetLoadedRQ(LPCTSTR lpszRQ);
};

#endif // !defined(AFX_INDICATORINFO_H__EE00E300_03E4_4B7C_88DE_CFB7991E43C4__INCLUDED_)
