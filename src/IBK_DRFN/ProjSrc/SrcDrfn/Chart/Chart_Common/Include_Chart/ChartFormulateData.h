// ChartFormulateData.h: interface for the CChartFormulateData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTFORMULATEDATA_H__E1DA3540_AD73_4CA2_AEA6_453BCD701392__INCLUDED_)
#define AFX_CHARTFORMULATEDATA_H__E1DA3540_AD73_4CA2_AEA6_453BCD701392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CPacketList;
class CIndicatorList;
class CIndicatorInfo;
class CGraphCalculateData;

class CChartFormulateData  
{
public:
	CChartFormulateData& operator=(const CChartFormulateData& data);

public:
	CChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	CChartFormulateData(CPacketList* pPacketList, CIndicatorList* pIndicatorList);
	CChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, const CString& strFunctionName, 
		const CString& strPacketNames, const CString& strSubGraphNames, const CString& strViewableSubGraphNames, 
		const int nDisplayDataStartIndex, const int nDisplayDataEndIndex, CPacketList* pPacketList, CIndicatorList* pIndicatorList);

	void SetCalculateType(const CCalculateBaseData::CALCULATETYPE eCalType);
	void SetFunctionName(const CString& strFunctionName);
	void SetPacketNames(const CString& strPacketNames);
	void SetSubGraphNames(const CString& strSubGraphNames);
	void SetViewableSubGraphNames(const CString& strViewableSubGraphNames);
	void SetDisplayDataStartIndex(const int nDisplayDataStartIndex);
	void SetDisplayDataEndIndex(const int nDisplayDataEndIndex);
	void SetPacketList(CPacketList* pPacketList);
	void SetIndicatorList(CIndicatorList* pIndicatorList);

	CCalculateBaseData::CALCULATETYPE GetCalculateType() const;
	CString GetFunctionName() const;
	CString GetPacketNames() const;
	CString GetSubGraphNames() const;
	CString GetViewableSubGraphNames() const;
	int GetDisplayDataStartIndex() const;
	int GetDisplayDataEndIndex() const;
	CPacketList* GetPacketList() const;
	CIndicatorList* GetIndicatorList() const;

	bool DoesProcessStarting() const;

private:
	CCalculateBaseData::CALCULATETYPE m_eCalType;
	CString m_strFunctionName;
	CString m_strPacketNames;
	CString m_strSubGraphNames;
	CString m_strViewableSubGraphNames;
	int m_nDiaplayDataStartIndex;	// 현재 화면에 보이는 처음 data index
	int m_nDiaplayDataEndIndex;		// 현재 화면에 보이는 마지막 data index
	CPacketList* m_pPacketList;
	CIndicatorList* m_pIndicatorList;

	void SetDataInEndComma(CString& strOrgData, const CString& strNewData);
};

#endif // !defined(AFX_CHARTFORMULATEDATA_H__E1DA3540_AD73_4CA2_AEA6_453BCD701392__INCLUDED_)
