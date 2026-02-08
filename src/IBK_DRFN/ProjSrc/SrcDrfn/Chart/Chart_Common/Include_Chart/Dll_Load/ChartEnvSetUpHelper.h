 // ChartEnvSetUpHelper.h: interface for the CChartEnvSetUpHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTENVSETUPHELPER_H__DF431C3C_E172_4279_88B0_26869D664DF6__INCLUDED_)
#define AFX_CHARTENVSETUPHELPER_H__DF431C3C_E172_4279_88B0_26869D664DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LoadDll.h"
#include "EnvironmentData.h"
#include "IndicatorConfigurationData.h"


class CCommonInterface;


// Define the Function Pointer
// 차트 환경 설정 Dialog Load
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadChartEnvSetUpDlgAll)(CCommonInterface *, const int);


// Define the Function Pointer
// 지표 설정 Dialog Load
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadChartIndicatorSetUpDlg)(CCommonInterface *);


// Define the Function Pointer
// 환경 설정 Dialog만 Load
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadChartEnvSetUpDlg)(CCommonInterface *, const int);


// 전체 지표 List을 띄워 지표 등록여부만을 결정하는 것으로 OCX에서만 이용되고 있으나,
// 그것도 현재 Popup Menu와 내부/외부 ToolBar에 등록되어 있지 않아 실제 이용되지 않는 기능이다.
// Return Value는 CIndicatorConfigurationData::INDICONFIG_RETURN를 이용하지 않고, 
// IDOK 값으로 고정시된다. (생성되는 Dialog에 '확인/취소' Button이 없다.)
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadIndicatorList)(CCommonInterface* );

// 선택된 Graph별 지표 설정을 받는도록 하는 기능으로 ChartObject DLL의 CGraphTool에서 이용되고 있다.
// Return Value는 CIndicatorConfigurationData::INDICONFIG_RETURN로 반환된다.
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fnpLoadIndicatorConfiguration)(const CIndicatorConfigurationData&, CIndicatorConfigurationChangedData&);

// 선택된 Graph별 지표 설정을 받는도록 하는 기능에서,
// 별도의 Check Button으로 Default (User Default) Indicator Info도 변경하도록 지원할 것인지를
// 설정받아 처리토록 한다.
typedef void (WINAPI *fnpSetWithDefaultIndicator)( BOOL p_bWithDefaultInfo);

//sy 2005.12.20. -> Skin 사용
typedef bool (WINAPI* fpGetChartSkinData)(const CString&, const CString&, const int, CStyleData& );

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
// Comments		: 
// Using way	: 
// See			: class CLoadDllLib
//-----------------------------------------------------------------------------
class CChartEnvSetUpHelper : public CLoadDllLib
{
public:
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Modifier		: 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CChartEnvSetUpHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("ChartEnvSetUp.dll");
		m_hLib = NULL;

		MakeNull(m_pfnpLoadChartEnvSetUpDlgAll);
		MakeNull(m_pfnpLoadChartIndicatorSetUpDlg);
		MakeNull(m_pfnpLoadChartEnvSetUpDlg);
		MakeNull(m_pfpLoadIndicatorConfiguration);
		MakeNull(m_pfpLoadIndicatorList);
		MakeNull(m_pfpSetWithDefaultIndicator);
		MakeNull(m_pfpGetChartSkinData);

		LoadLib();
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Return void : 
	// Param  LPCSTR _szDll  ; : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CChartEnvSetUpHelper( LPCSTR _szDll )
	{
		m_delAuto = TRUE;

		if( _szDll )
			m_szDllName.Format( "%s%s", _szDll, "ChartEnvSetUp.dll" );

		else
			m_szDllName = _T("ChartEnvSetUp.dll");
		
		m_hLib = NULL;
		
		MakeNull(m_pfnpLoadChartEnvSetUpDlgAll);
		MakeNull(m_pfnpLoadChartIndicatorSetUpDlg);
		MakeNull(m_pfnpLoadChartEnvSetUpDlg);
		MakeNull(m_pfpLoadIndicatorConfiguration);
		MakeNull(m_pfpLoadIndicatorList);
		MakeNull(m_pfpSetWithDefaultIndicator);
		MakeNull(m_pfpGetChartSkinData);

		LoadLib();
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Return void : 
	// Param  BOOL deleteAuto : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CChartEnvSetUpHelper( BOOL deleteAuto )
	{
		m_delAuto = TRUE;
		m_szDllName = _T("ChartEnvSetUp.dll");
		m_hLib = NULL;
		
		MakeNull(m_pfnpLoadChartEnvSetUpDlgAll);
		MakeNull(m_pfnpLoadChartIndicatorSetUpDlg);
		MakeNull(m_pfnpLoadChartEnvSetUpDlg);
		MakeNull(m_pfpLoadIndicatorConfiguration);
		MakeNull(m_pfpLoadIndicatorList);
		MakeNull(m_pfpSetWithDefaultIndicator);
		MakeNull(m_pfpGetChartSkinData);

		LoadLib();

		m_delAuto = deleteAuto;
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 7
	// Modifier		: 
	// Comments		: 
	//-----------------------------------------------------------------------------
	~CChartEnvSetUpHelper()
	{
	}

public:
	BOOL m_delAuto;
	
	fnpLoadChartEnvSetUpDlgAll	m_pfnpLoadChartEnvSetUpDlgAll;			// 설정 Dialog 전체를 Load하기 위한 Funciton Pointer
	fnpLoadChartIndicatorSetUpDlg m_pfnpLoadChartIndicatorSetUpDlg;		// 지표설정 Dialog만 Load하는 Function pointer
	fnpLoadChartEnvSetUpDlg m_pfnpLoadChartEnvSetUpDlg;					// 환경설정 Dialog만 Load하는 Function pointer

	fnpLoadIndicatorConfiguration	m_pfpLoadIndicatorConfiguration; 
	fnpLoadIndicatorList			m_pfpLoadIndicatorList;
	fnpSetWithDefaultIndicator		m_pfpSetWithDefaultIndicator;
	fpGetChartSkinData  m_pfpGetChartSkinData;//sy 2005.12.20.
	
public:
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho	
	// 지표설정&환경설정 load
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadChartEnvSetUpDlgAll(CCommonInterface *pCommonInterface, const int nActiveTabIndex)
	{
		if( !m_hLib ) 
			return CIndicatorConfigurationData::LOAD_ERROR;			// CS_LOADERROR;

		m_pfnpLoadChartEnvSetUpDlgAll = (fnpLoadChartEnvSetUpDlgAll)::GetProcAddress(m_hLib, "LoadDlgChartEnvSetUpAll");
		
		if( m_pfnpLoadChartEnvSetUpDlgAll == NULL )
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;				// CS_LOADERROR;
		}
		
		else
			return m_pfnpLoadChartEnvSetUpDlgAll(pCommonInterface, nActiveTabIndex);
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho
	// 지표설정 Load.
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadChartIndicatorSetUpDlg(CCommonInterface *pCommonInterface)
	{
		if( !m_hLib ) 
			return CIndicatorConfigurationData::LOAD_ERROR;;			// CS_LOADERROR;

		m_pfnpLoadChartIndicatorSetUpDlg = (fnpLoadChartIndicatorSetUpDlg)::GetProcAddress(m_hLib, "LoadDlgChartIndicatorSetUp");
		
		if( m_pfnpLoadChartIndicatorSetUpDlg == NULL )
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;				// CS_LOADERROR;
		}
		
		else
			return m_pfnpLoadChartIndicatorSetUpDlg(pCommonInterface);
	}

	
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho	
	// 환경설정 Load
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadChartEnvSetUpDlg(CCommonInterface *pCommonInterface, const int nActiveTabIndex)
	{
		if( !m_hLib ) 
			return CIndicatorConfigurationData::LOAD_ERROR;;			// CS_LOADERROR;

		m_pfnpLoadChartEnvSetUpDlg = (fnpLoadChartEnvSetUpDlg)::GetProcAddress(m_hLib, "LoadDlgChartEnvSetUp");
		
		if( m_pfnpLoadChartEnvSetUpDlg == NULL )
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;				// CS_LOADERROR;
		}
		
		else
			return m_pfnpLoadChartEnvSetUpDlg(pCommonInterface, nActiveTabIndex);
	}

	
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
	// 단위 그래프 설정
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorConfiguration(const CIndicatorConfigurationData& indicatorConfigurationData, CIndicatorConfigurationChangedData& changeData)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorConfiguration	= (fnpLoadIndicatorConfiguration)::GetProcAddress(m_hLib, "LoadIndicatorConfiguration");
		if(m_pfpLoadIndicatorConfiguration == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorConfiguration(indicatorConfigurationData, changeData);
	}

	
	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
	// Return CIndicatorConfigurationData::INDICONFIG_RETURN : 
	// Param  CCommonInterface* pCommonInterface : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorList( CCommonInterface* pCommonInterface )
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorList	= (fnpLoadIndicatorList)::GetProcAddress(m_hLib, "LoadIndicatorList");
		if(m_pfpLoadIndicatorList == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorList(pCommonInterface);
	}


	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 4
	// Return void : 
	// Param  BOOL p_bWithDefaultInfo : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	void SetWithDefaultIndicator( BOOL p_bWithDefaultInfo)
	{
		if(!m_hLib) return;

		m_pfpSetWithDefaultIndicator = ( fnpSetWithDefaultIndicator)::GetProcAddress( m_hLib, "SetWithDefaultIndicator");
		if(m_pfpSetWithDefaultIndicator == NULL)
		{
			MAKESTATE( LDS_FAIL, LDR_GETPROCADDRESS);
			return;
		}
		m_pfpSetWithDefaultIndicator( p_bWithDefaultInfo);
	}

	//sy 2005.12.20. -> skin 적용
	bool GetChartSkinData(const CString& strDefaultFilePath, const CString& strUserFilePath, const int nStyleNumber, CStyleData& styleData)
	{
		if(!m_hLib) 
			return (GetState() == TRUE ? true: false);//CS_LOADERROR;

		m_pfpGetChartSkinData	= (fpGetChartSkinData)::GetProcAddress(m_hLib, "GetChartSkinData");
		if(m_pfpGetChartSkinData==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return (GetState() == TRUE ? true: false);//CS_LOADERROR;
		}
		m_pfpGetChartSkinData(strDefaultFilePath, strUserFilePath, nStyleNumber, styleData);
		MAKESTATE(LDS_OK, LDR_READY);
		return (GetState() == TRUE ? true: false);//nRet;
	}
};

#endif // !defined(AFX_CHARTENVSETUPHELPER_H__DF431C3C_E172_4279_88B0_26869D664DF6__INCLUDED_)

//////////////////////////////////////////////////////////////////////////
