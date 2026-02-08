// IndicatorConditionBaseData.h: interface for the CIndicatorConditionBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORCONDITIONBASEDATA_H__EFF7E705_4CDD_4658_BFE9_5850B47B4E48__INCLUDED_)
#define AFX_INDICATORCONDITIONBASEDATA_H__EFF7E705_4CDD_4658_BFE9_5850B47B4E48__INCLUDED_

#include "../Include_Chart/DLL_Load/IndicatorConditionDefineData.h"
#include "../Include_Chart/DLL_Load/IndicatorList.h"					// for EIndicatorListType
#include "GraphData.h"
#include "IndicatorInfoMap.h"											// for CIndicatorInfoMap

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiBaseData
{
public:
	enum TITLETYPE
	{
		BASE_TITLE = 0,
		COND_TITLE, 
		BASE_COND_TITLE
	};
};

// CIndicatorInfo 에 저장되는 data 중 subGraph 관련 부분
class CSubGraphPartData
{
public:
	CSubGraphPartData& operator=(const CSubGraphPartData& data);

public:
	CSubGraphPartData();
	CSubGraphPartData(const CSubGraphPartData& data);
	CSubGraphPartData(const CSubGraphData& subGraphData);
	virtual ~CSubGraphPartData();

	const CDoubleList& GetConditionList() const;
	CDoubleList& GetConditionList();
	const CSubGraphData& GetSubGraphData() const;
	CSubGraphData& GetSubGraphData();
	CSubGraphData* GetSubGraphData_PointeType();

	bool GetConditionData(const int nIndex, double& dData) const;
	bool GetConditionData(const int nIndex, CString &p_szDataString) const;
	CString	GetCalPacketNames( void) const;
	CString	GetSubGraphName( void) const;
	void GetSubGraphStringData( CString &p_strSubGraphData) const;
	void GetSubGraphStringData( CString &p_strSubGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle);
//	void GetSubGraphStringData( const char *p_szTitle, CString &p_strSubGraphData) const;
//	void GetSubGraphStringData(const CGraphColor& graphColor, CString &p_strSubGraphData) const;
//	CString	GetSubGraphStringData(const int nCondIndexForTitle, const bool bIncludeOrgTitle, const CGraphColor& graphColor) const;

	bool CopyDataWithVerificationFrom(const CSubGraphPartData* pSourceData);
	bool CopyDataWithVerificationFrom(const CSubGraphData& sourceData);
	bool SetCalPacketNames( const char *p_szCalPacketNames);
	bool SetCalPacketNames( const char *p_szCalPacketNames, const bool bIsAutoCheck);
	void SetConditionList( const char *p_szData, const CString& strCompart = ",");
	bool SetCalConditionData( const int nCondIndex, const double& dData);

	// subGraph 에 있는 계산 조건값을 title로 바꾼다.
	bool ChangeCalConditionDataFromSubGraphTitle();

private:
	void CopyConditionList(const CDoubleList& sourceData);

	void CopyDataWithVerificationFrom(const CDoubleList& sourceData);

	void GetSubGraphStringData( CString &p_SubGraphData, CSubGraphData &subGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const;

private:
	CString m_strCalPacketNames;
	CDoubleList m_calConditionList;	
	CSubGraphData m_subGraphData;
};


// CIndicatorInfo 에 저장되는 data 중 graph 관련 부분
class CGraphPartData
{
private:
	CString m_strGroupName;		// 지표를 분류하는 group name
	CGraphData m_graphData;
	CSubGraphPartDataList m_subGraphPartDataList;

private:
	bool MakeSubGraphPartDataList(const CSubGraphDataList* pSubGraphDataList);
	void CopySubGraphPartDataList(const CSubGraphPartDataList* pSourceData);

	void SetGraphDataInVerifiable(const CGraphData& sourceData);
	void SetSubGraphPartDataListInVerifiable(const CSubGraphPartDataList* pSourceData);
	bool SetAllSubGraphDataList(const CSubGraphDataList* pSubGraphDataList, CSubGraphPartDataList& targetData);

	void GetSubGraphTitle( CSubGraphPartData* pSubGraphPartData, const bool bIsTitle, const bool bIsCond, CString &p_strDataString) const;

public:
	CGraphPartData& operator=(const CGraphPartData& data);

public:
	CGraphPartData();
	CGraphPartData(const CGraphPartData& graphPartData);
	virtual ~CGraphPartData();

	void Initialize();
	void Initialize_GraphData();

	void AddTail(CSubGraphPartData* pSubGraphPartData);
	bool DeleteAllSubGraph();

	CString GetGroupName() const;
	CGraphData& GetGraphData();
	CSubGraphPartDataList* GetSubGraphPartDataList();
	const CGraphData& GetGraphData() const;
	const CSubGraphPartDataList* GetSubGraphPartDataList() const;
	int GetSubGraphPartDataCount() const;
	int GetSubGraphPartDataIndex(const CString& strSubGraphName) const;
	bool GetAllSubGraphDataList(CSubGraphDataList& subGraphDataList) const;
	CSubGraphData* GetSubGraphData(const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const int nSubGraphIndex) const;
	CDoubleList* GetConditionList(const CString& strSubGraphName) const;
	CDoubleList* GetConditionList(const int nSubGraphIndex) const;
	CString GetPacketNames(const int nSubGraphIndex) const;
	CString GetSubGraphTitle(const int nSubGraphIndex) const;

	void GetGraphDataForMaking( CString &p_strGraphData) const;
	void GetGraphDataForMaking( CString &p_strGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const;
	void GetAllSubGraphDataForMaking( CString &p_strAllSubGraphData) const;
	void GetAllSubGraphDataForMaking( CString &p_strAllSubGraphData, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const;
//	CString GetAllSubGraphDataForMaking(const CGraphColorPointerList& graphColorList) const;
//	CString GetAllSubGraphDataForMaking(const CGraphColorPointerList& graphColorList, const int nCondIndexForTitle, const bool bIncludeOrgTitle) const;
	bool GetSubGraphNameList(CStringDataList& SubGraphNameList) const;
	BOOL	GetSubGraphNames( CString &p_strSubGraphNames) const;	// (2006/5/31 - Seung-Won, Bae) To compare with Main Block Graph Objects
	bool GetSubGraphTitleList(CStringDataList& subGraphTitleList) const;
	bool GetSubGraphColorList(CGraphColorPointerList& subGraphColorList) const;

	CIndiBaseData::TITLETYPE GetConditionTitle() const;

	void SetGroupName(const CString& strGroupName);
	void CopyDataWithVerificationFrom(const CGraphPartData& sourceData);
	bool SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck);
	bool SetAllSubGraphPartDataList(const CSubGraphPartDataList* pSubGraphPartDataList);
	bool SetAllSubGraphDataList(const CSubGraphDataList* pSubGraphDataList);
	bool SetAllPacketNames_SubGraphPart(const CString& strPacketNames, const bool bIsAutoCheck);
	bool SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames);
	bool SetAllCalConditionData(const int nCondIndex, const double& dData);
	bool SetSubGraphData(const CSubGraphData* pSourceData);
	bool SetSubGraphTitle(const int nIndex, const CString& strTitle);
	bool SetSubGraphColor(const int nIndex, const CGraphColor& graphColor);
	bool SetSubGraphPenThickness(const int nIndex, const CPenThickness& penThickness);

	bool ChangeCalConditionDataFromSubGraphTitle();

// (2006/5/28 - Seung-Won, Bae) for Old User Indicator Info. Add New SubGraph.
public:
	BOOL				AddSubGraphPartDataWithCopy( const CSubGraphPartData* pSubGraphPartData);
	CSubGraphPartData *	GetSubGraphPartData(const CString& strSubGraphName) const;
	CSubGraphPartData *	GetSubGraphPartData(const int nSubGraphIndex) const;
	CSubGraphPartData * GetSubGraphPartData(const CSubGraphPartDataList* pSubGraphPartDataList, const CString& strSubGraphName) const;

// (2006/5/29 - Seung-Won, Bae) Update CGraphDataDrawingData ('graphdatadrawing')
public:
	void						SetGraphDataDrawingDataWithCopy( const CGraphDataDrawingData * p_pGraphDataDrawingData);
	CGraphDataDrawingData *		GetGraphDataDrawingData( void);

public:
	BOOL SetCalPacketNames( int p_nSubGraphIndex, const CString& strCalPacketNames, const bool bIsAutoCheck);

// (2007/2/28 - Seung-Won, Bae) Retrieve Sub Graph Hiding
public:
	BOOL	GetSubGraphHiding( const char *p_szSubGraphName);
	BOOL	GetSubGraphHiding( const int p_nSubGraphIndex);
};


// ----------------------------------------------------------------------------
// CIndicatorInfo 들이 저장 되어 있는 list class

class CIndicatorList;
class CGraphTypeAndStyleData;
class CIndicatorGroupDataList;
class CBaseIndicatorList
{
public:
	enum INDICATORTYPE
	{
		ALL = 0,
		GENERAL,
		SPECIAL,
		NONE
	};

public:
	// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
	CBaseIndicatorList( EIndicatorListType p_eIndicatorListType);
	virtual ~CBaseIndicatorList();

private:
	double m_dVersion;
	CIndicatorInfoMap *m_pGeneralMap; // 일반지표(default)
	CIndicatorInfoMap *m_pSpecialMap; // 특수지표(default)
	CIndicatorInfoMap *m_pNoneIndiMap; // none지표(default)

// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
protected:
	EIndicatorListType	m_eIndicatorListType;
public:
	EIndicatorListType	GetIndicatorListType( void) const	{	return m_eIndicatorListType;	}

// Version Info
private:
	double GetVersion( char *&p_szData) const;
public:
	double GetVersion() const;
	bool SetVersion( char *&p_szData);
	void SetVersion(const double& dVersion);

// Info Map
protected:
	friend class CDefaultIndicatorListBuilder;
	friend class CDefaultUserTypeIndiListBuilder;
	CIndicatorInfoMap *			GetGeneralIndiMap();
	CIndicatorInfoMap *			GetSpecialIndiMap();
	CIndicatorInfoMap *			GetNoneIndiMap();
	const CIndicatorInfoMap *	GetGeneralIndiMap() const;
	const CIndicatorInfoMap *	GetSpecialIndiMap() const;
	const CIndicatorInfoMap *	GetNoneIndiMap() const;

// Add and Delete
public:
	bool				AddIndicatorInfo( CIndicatorInfo* pIndicatorInfo);
	bool				Delete( CIndicatorInfo* pIndicatorInfo);
	bool				DeleteAll( const CBaseIndicatorList::INDICATORTYPE eDeleteType);
	bool				RemoveAllIndicator( const CBaseIndicatorList::INDICATORTYPE eDeleteType, const char *p_szIndicatorName);

// List Info
public:
	bool				IsEmpty() const;
	bool				GetIndicatorNameList( CBaseIndicatorList::INDICATORTYPE eType, CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const;
	bool				GetGraphNameList( CBaseIndicatorList::INDICATORTYPE eType, CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const;
	bool				GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList& indiGroupDataList, CChartInfo::CHART_MODE p_eChartMode) const;				// Indicator의 Group에 관련된 내용

// Get Indicator Info
public:
	CIndicatorInfo *	GetIndicatorInfo( const char *p_szIndicatorKeyName, CChartInfo::CHART_MODE p_eChartMode);
	CIndicatorInfo *	GetFirstIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator, CChartInfo::CHART_MODE p_eChartMode) const;
	void				GetNextIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator, CIndicatorInfo *&p_pPrevIndicatorInfo) const;
};

//-> Default(IndiCond.cfg) 관련 지표들 저장
class CDefaultIndicatorList : public CBaseIndicatorList
{
public:
	// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
	CDefaultIndicatorList( EIndicatorListType p_eIndicatorListType);
	virtual ~CDefaultIndicatorList();

	bool AddIndiInfoFromCfgString( CIndicatorList* pIndicatorList, CGraphTypeAndStyleData* pGraphTypeAndStyleData, char *&p_szDataString, HWND p_hOcxWnd);
};


//-> 사용자 정의된 지표 관련 저장
class CUserIndicatorList : public CBaseIndicatorList
{
public:
	// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
	CUserIndicatorList( CDefaultIndicatorList* pDefaultIndiList, EIndicatorListType p_eIndicatorListType, HWND p_hOcxWnd);
protected:
	CDefaultIndicatorList* m_pDefaultIndiList;

public:
	bool AddIndiInfoFromCfgString(CIndicatorList* pIndicatorList, char *&p_szDataString, const bool dDoesDeleteOldData);
protected:
	double GetDefaultVersion() const;

public:
	void GetWritingFileData( CStringList &p_slDataString) const;
protected:
	double GetWritingFileVersion() const;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

#endif // !defined(AFX_INDICATORCONDITIONBASEDATA_H__EFF7E705_4CDD_4658_BFE9_5850B47B4E48__INCLUDED_)
