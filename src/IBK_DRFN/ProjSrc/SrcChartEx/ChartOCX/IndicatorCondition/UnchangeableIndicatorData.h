// UnchangeableIndicatorData.h: interface for the CUnchangeableIndicatorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNCHANGEABLEINDICATORDATA_H__CED370A3_7EFC_4BA8_BD31_0C21D57D215F__INCLUDED_)
#define AFX_UNCHANGEABLEINDICATORDATA_H__CED370A3_7EFC_4BA8_BD31_0C21D57D215F__INCLUDED_

#include "IndicatorConditionBaseData.h"

#include "../Include_Chart/DLL_Load/GraphTypeData.h"
#include "../Include_Chart/Dll_Load/IndicatorGroupData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUnchangeableData
{
public:
	CUnchangeableData();

	// 저장관련 내용 Site, User Default File
	bool FileSUDefaultDataBuild( CIndicatorList* pIndicatorList, const CString& strMainDefaultFilePathAndName, const CString& strUserDefaultFilePathAndName, HWND p_hOcxWnd);
	void GetWritingFileData( CStringList &p_slDataString) const;

	double GetDefaultVersion() const;
	const CDefaultIndicatorList& GetMainDefaultIndicatorList() const;
	CDefaultIndicatorList& GetMainDefaultIndicatorList();
	CDefaultIndicatorList* GetMainDefaultIndicatorList_Pointer();
	// Indicator의 Group에 관련된 내용
	BOOL GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList &p_dlIndicatorGroup, CChartInfo::CHART_MODE p_eChartMode);
	// Indicator List
	CUserIndicatorList* GetUserDefaultIndiList() const;

	// IndicatorInfo 에 관련된 내용
	bool GetMainDefaultIndicatorNameList_General( CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const;
	bool GetMainDefaultIndicatorNameList_Special( CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const;
	bool GetMainDefaultIndicatorNameList_None(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const;
	const CGraphTypeAndStyleData *GetGraphTypeAndStyleData( void)	{	return &m_graphTypeAndStyleData;	}

private:
	bool m_bDefaultInit;
	CGraphTypeAndStyleData m_graphTypeAndStyleData;

	CDefaultIndicatorList m_defaultIndiList;
	CUserIndicatorList m_userDefaultIndiList; // 주의 - "기본으로 저장"을 할경우만 userDefault는 생성된다!!
};

#endif // !defined(AFX_UNCHANGEABLEINDICATORDATA_H__CED370A3_7EFC_4BA8_BD31_0C21D57D215F__INCLUDED_)
