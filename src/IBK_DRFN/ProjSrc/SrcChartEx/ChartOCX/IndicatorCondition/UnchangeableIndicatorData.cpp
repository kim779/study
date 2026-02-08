// UnchangeableIndicatorData.cpp: implementation of the CUnchangeableIndicatorData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnchangeableIndicatorData.h"
#include "FileBuilder.h"

#include "../Include_Chart/DLL_Load/IndicatorList.h"					// for EIndicatorListType
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CUnchangeableData -> CIndicatorList에서 기본적인(static) data를 가지고 있다.

// (2006/11/30 - Seung-Won, Bae) Support Indicator List Type Manage.
CUnchangeableData::CUnchangeableData() :
		m_defaultIndiList( EILT_SITE_DEFAULT),
		m_userDefaultIndiList( &m_defaultIndiList, EILT_USER_DEFAULT, NULL), 
		m_bDefaultInit(false)
{
}

// public =====================================================================
// default file data 를 객체화.
// user default file data 를 객체화.
bool CUnchangeableData::FileSUDefaultDataBuild( CIndicatorList* pIndicatorList, const CString& strMainDefaultFilePathAndName, const CString& strUserDefaultFilePathAndName, HWND p_hOcxWnd)
{
	if( m_bDefaultInit) return true;

	CString strFileData;
	char *szFileData = CFileBuilder().FileRead( strMainDefaultFilePathAndName, strFileData);
	if( !strFileData.IsEmpty())
	{
		// GraphType 관련 내용 읽기
		if( m_graphTypeAndStyleData.FileDataBuild( szFileData))
		{
			// 지표관련 내용을 읽기
			m_defaultIndiList.AddIndiInfoFromCfgString( pIndicatorList, &m_graphTypeAndStyleData, szFileData, p_hOcxWnd);
		}
	}
	else AfxMessageBox( "Data File Loading Failed!\r\n[" + strMainDefaultFilePathAndName + "]");

	// 순서를 변경해서는 안된다! -> user default/user 는 Site default 값을 참조한다.
	szFileData = CFileBuilder().FileRead( strUserDefaultFilePathAndName, strFileData);
	// Data 폴더에 있는 file을 복사했는지를 비교한다. -> Data 폴더 복사본은 사용하지 않는다. (why?)
	if( !strFileData.IsEmpty() && strFileData.Find( "Graph Type End") < 0)
	{
		// 지표관련 내용을 읽기
		m_userDefaultIndiList.AddIndiInfoFromCfgString(pIndicatorList, szFileData, true);
	}

	m_bDefaultInit = true;
	return true;
}

// file에 저장하기 위한 data.
void CUnchangeableData::GetWritingFileData( CStringList &p_slDataString) const
{
	m_userDefaultIndiList.GetWritingFileData( p_slDataString);
}

// ----------------------------------------------------------------------------
double CUnchangeableData::GetDefaultVersion() const
{
	return m_defaultIndiList.GetVersion();
}

// ----------------------------------------------------------------------------
const CDefaultIndicatorList& CUnchangeableData::GetMainDefaultIndicatorList() const
{
	return m_defaultIndiList;
}

CDefaultIndicatorList& CUnchangeableData::GetMainDefaultIndicatorList()
{
	return m_defaultIndiList;
}

CDefaultIndicatorList* CUnchangeableData::GetMainDefaultIndicatorList_Pointer()
{
	return &m_defaultIndiList;
}

// Indicator의 Group에 관련된 내용
// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
BOOL CUnchangeableData::GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList &p_dlIndicatorGroup, CChartInfo::CHART_MODE p_eChartMode)
{
	return m_defaultIndiList.GetAllIndicatorNameWithGroupName( p_dlIndicatorGroup, p_eChartMode);
}

CUserIndicatorList* CUnchangeableData::GetUserDefaultIndiList() const
{
	return const_cast<CUserIndicatorList*>(&m_userDefaultIndiList);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CUnchangeableData::GetMainDefaultIndicatorNameList_General(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const
{
	return m_defaultIndiList.GetIndicatorNameList(CBaseIndicatorList::GENERAL, IndicatorNameList, p_eChartMode);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CUnchangeableData::GetMainDefaultIndicatorNameList_Special(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const
{
	return m_defaultIndiList.GetIndicatorNameList(CBaseIndicatorList::SPECIAL, IndicatorNameList, p_eChartMode);
}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
bool CUnchangeableData::GetMainDefaultIndicatorNameList_None(CStringDataList& IndicatorNameList, CChartInfo::CHART_MODE p_eChartMode) const
{
	return m_defaultIndiList.GetIndicatorNameList(CBaseIndicatorList::NONE, IndicatorNameList, p_eChartMode);
}
