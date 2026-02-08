// IndicatorInfo.h: interface for the CIndicatorInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORINFO_H__EE00E300_03E4_4B7C_88DE_CFB7991E43C4__INCLUDED_)
#define AFX_INDICATORINFO_H__EE00E300_03E4_4B7C_88DE_CFB7991E43C4__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndicatorInfoImplementation;
class CGraphTypeAndStyleData;
class CGraphPartData;
class CGraphColor;
class CGraphData;
class CSubGraphData;
class CPenThickness;

class AFX_EXT_CLASS CIndicatorInfo
{
public:
	static CIndicatorInfo* Make();
	static CIndicatorInfo* Make(const CIndicatorInfo* pDefaultIndicatorInfo);
	static void Delete(CIndicatorInfo* pIndicatorInfo);

public:
	CIndicatorInfo(const CIndicatorInfo* pDefaultIndicatorInfo);
	CIndicatorInfo(const CString& strUserFileInfoData);
	virtual ~CIndicatorInfo();

	bool FileDataBuild_Default(CGraphTypeAndStyleData* pGraphTypeAndStyleData, const CString& strIndiInfoData);
	bool FileDataBuild_User(const CString& strIndiInfoData);
	bool SetDataInVerifiable(const CIndicatorInfo& sourceData);
	bool SetDataInVerifiable_Ver1Dot2(const CString& strIndiInfoData);

	CIndicatorInfoImplementation* GetIndicatorInfoImpl() const;
	CString GetGroupName() const;
	CString GetIndicatorName() const;
	CGraphPartData* GetGraphPartData() const;
	CGraphPartData* GetGraphPartData();
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

	bool DoesEqualCommonCalConditionList(const CString& strData, const CString& strCompart = ',') const;
	bool DoesEqualCalConditionList(const int nSubGraphIndex, const CString& strData, const CString& strCompart = ',') const;
	bool DoesEqualCommonPakcetNames(const CString& strData) const;
	bool DoesEqualPacketNames_SubGraphPart(const int nSubGraphIndex, const CString& strData) const;

	CString GetGraphDataForMaking() const;
	CString GetAllSubGraphDataForMaking() const;
	CString GetAllSubGraphDataForMaking(const CList<CGraphColor*, CGraphColor*>& graphColorList) const;
	CString GetStringCommonCalConditionList(const CString& strCompart = ',') const;
	CString GetStringDrawConditionList(const CString& strCompart = ',') const;
	CString GetStringBaseLineList(const CString& strCompart = ',') const;
	CGraphData& GetGraphData();
	CGraphData* GetGraphData_PointerType();
	CString GetCommonPacketNames() const;
	CString GetPacketNames(const int nSubGraphIndex) const;
	bool IsSignal() const;
	int GetSubGraphCount() const;
	bool GetAllSubGraphDataList(CList<CSubGraphData*, CSubGraphData*>& subGraphDataList) const;
	CSubGraphData* GetSubGraphData(const CString& strSubGraphName) const;
	CSubGraphData* GetSubGraphData(const int nSubGraphIndex) const;
	int GetSubGraphIndex(const CString& strSubGraphName) const;
	CString GetSubGraphName(const int nIndex = 0) const;
	CString GetSubGraphTitle(const int nIndex = 0) const;
	bool GetSubGraphNameList(CList<CString, CString>& subGraphNameList) const;
	CGraphColor GetSubGraphColor(const int nIndex = 0) const;
	CGraphColor* GetSubGraphColor_PointerType(const int nIndex = 0) const;
	bool GetSubGraphColorList(CList<CGraphColor*, CGraphColor*>& subGraphColorList) const;

	bool SetIndicatorInfo(const CIndicatorInfo* pIndicatorInfo);
	void SetCommonCalConditionData(const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetCommonCalConditionData(const int nIndex, const double& dData);
	void SetDrawConditionData(const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetDrawConditionData(const int nIndex, const double& dData);
	void SetBaseLineData(const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetBaseLineData(const int nIndex, const double& dData);
	bool SetAllCalConditionData_SubGraphPart(const int nCondIndex, const double& dData);
	bool SetCalConditionData(const int nSubGraphIndex, const CString& strData, const CString& strCompart = ',');
	// return : -1(해당 nIndex 없음) 0(해당 nIndex있으나 값이 같음) 1(변경함)
	int SetCalConditionData(const int nSubGraphIndex, const int nCondIndex, const double& dData);

	bool SetGraphData(const CGraphData& graphData, const bool bIsAutoCheck = true);
	bool SetCommonPacketNames(const CString& strPacketNames);
	bool SetAllPacketNames_SubGraphPart(const CString& strPacketNames);
	bool SetPacketNames(const int nSubGraphIndex, const CString& strPacketNames);
	void SetSignal(const bool bIsSignal);

	bool SetAllSubGraphDataList(const CList<CSubGraphData*, CSubGraphData*>* pSubGraphDataList, const bool bAutoCalConditionData = true);
	bool SetSubGraphData(const CSubGraphData* pSourceData);
	bool SetSubGraphTitle(const int nIndex, const CString& strTitle);
	bool SetSubGraphColor(const int nIndex, const CGraphColor& graphColor);
	bool SetSubGraphColor(const int nIndex, const COLORREF& lineColor);
	bool SetSubGraphPenThickness(const int nIndex, const CPenThickness& penThickness);
	bool SetSubGraphPenThickness(const int nIndex, const int nPenThickness);

private:
	CIndicatorInfoImplementation* m_pIndiInfoImpl;
};

#endif // !defined(AFX_INDICATORINFO_H__EE00E300_03E4_4B7C_88DE_CFB7991E43C4__INCLUDED_)
