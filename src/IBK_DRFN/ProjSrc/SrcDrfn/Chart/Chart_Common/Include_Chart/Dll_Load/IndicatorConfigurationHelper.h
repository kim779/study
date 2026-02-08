// IndicatorConfigurationHelper.h : main header file for the INDICATORCONFIGURATIONHELPER application
//

#if !defined(AFX_INDICATORCONFIGURATIONHELPER_H__AC973E75_E86C_4ABF_981E_83EC5441B56B__INCLUDED_)
#define AFX_INDICATORCONFIGURATIONHELPER_H__AC973E75_E86C_4ABF_981E_83EC5441B56B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"
#include "IndicatorConfigurationData.h"

/////////////////////////////////////////////////////////////////////////////
class CCommonInterface;

// (2004.09.03, 배승원, 분석) 전체 지표 설정창을 띄우는 것으로 OCX에서만 이용되고 있다.
//		특히 CIndicatorConfigurationData::INDICONFIG_RETURN를 이용하지 않고, IDOK, IDCANCEL만으로 Return되고,
//		현재 OCX에서만 이용되면서, Return 값은 이용되지 않고 있다.
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fpLoadAllIndicatorConfiguration)(CCommonInterface* , CMainBlock*, CIndicatorList*, CPacketList*);

// (2004.09.03, 배승원, 분석) 전체 지표 List을 띄워 지표 등록여부만을 결정하는 것으로 OCX에서만 이용되고 있으나,
//			그것도 현재 Popup Menu와 내부/외부 ToolBar에 등록되어 있지 않아 실제 이용되지 않는 기능이다.
//		Return Value는 CIndicatorConfigurationData::INDICONFIG_RETURN를 이용하지 않고, 
//			IDOK 값으로 고정시된다. (생성되는 Dialog에 '확인/취소' Button이 없다.)
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fpLoadIndicatorList)(CCommonInterface* , CMainBlock*, CIndicatorList*, CPacketList*);

// (2004.09.03, 배승원, 분석) 선택된 Graph별 지표 설정을 받는도록 하는 기능으로 ChartObject DLL의 CGraphTool에서 이용되고 있다.
//		설정된 지표의 값을 추출하는 GetChangedData()과 함께 이용된다.
// (2004.09.03, 배승원) Return Value는 CIndicatorConfigurationData::INDICONFIG_RETURN로 반환된다.
typedef CIndicatorConfigurationData::INDICONFIG_RETURN (WINAPI *fpLoadIndicatorConfiguration)(const CIndicatorConfigurationData&);

// (2004.09.06, 배승원) 선택된 Graph별 지표 설정을 받는도록 하는 기능에서,
//		별도의 Check Button으로 Default (User Default) Indicator Info도 변경하도록 지원할 것인지를
//		설정받아 처리토록 한다.
typedef void (WINAPI *fpSetWithDefaultIndicator)( BOOL p_bWithDefaultInfo);

// (2004.09.03, 배승원, 분석) 선택된 Graph별 지표 설정 값을 추출하는 기능으로 ChartObject DLL의 CGraphTool에서 이용되고 있다.
//		Graph별 지표 설정을 받는 LoadIndicatorConfiguration()과 함께 이용된다.
typedef bool (WINAPI *fpGetChangedData)(CIndicatorConfigurationChangedData&);

class CIndicatorConfigurationHelper : public CLoadDllLib
{
public:
	fpLoadIndicatorConfiguration	m_pfpLoadIndicatorConfiguration; 
	fpLoadAllIndicatorConfiguration	m_pfpLoadAllIndicatorConfiguration;
	fpLoadIndicatorList				m_pfpLoadIndicatorList;
	fpGetChangedData				m_pfpGetChangedData;
	fpSetWithDefaultIndicator		m_pfpSetWithDefaultIndicator;

	BOOL m_delAuto;

	CIndicatorConfigurationHelper(LPCSTR _szDll)
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"IndicatorConfiguration.dll");
		else
			m_szDllName = _T("IndicatorConfiguration.dll");
		m_hLib = NULL;
		MakeNull( m_pfpLoadIndicatorConfiguration);
		MakeNull( m_pfpLoadAllIndicatorConfiguration);
		MakeNull( m_pfpLoadIndicatorList);
		MakeNull( m_pfpGetChangedData);
		MakeNull( m_pfpSetWithDefaultIndicator);
		LoadLib();
	}

	CIndicatorConfigurationHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("IndicatorConfiguration.dll");
		m_hLib = NULL;
		MakeNull( m_pfpLoadIndicatorConfiguration);
		MakeNull( m_pfpLoadAllIndicatorConfiguration);
		MakeNull( m_pfpLoadIndicatorList);
		MakeNull( m_pfpGetChangedData);
		MakeNull( m_pfpSetWithDefaultIndicator);
		LoadLib();
	}

	~CIndicatorConfigurationHelper()
	{
	}

	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorConfiguration(const CIndicatorConfigurationData& indicatorConfigurationData)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorConfiguration	= (fpLoadIndicatorConfiguration)::GetProcAddress(m_hLib, "LoadIndicatorConfiguration");
		if(m_pfpLoadIndicatorConfiguration == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorConfiguration(indicatorConfigurationData);
	}

	CIndicatorConfigurationData::INDICONFIG_RETURN LoadIndicatorList(CCommonInterface* pCommonInterface, CMainBlock* block, CIndicatorList* pIndicatorList, CPacketList* pPacketList)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadIndicatorList	= (fpLoadIndicatorList)::GetProcAddress(m_hLib, "LoadIndicatorList");
		if(m_pfpLoadIndicatorList == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadIndicatorList(pCommonInterface, block, pIndicatorList, pPacketList);
	}

	CIndicatorConfigurationData::INDICONFIG_RETURN LoadAllIndicatorConfiguration(CCommonInterface* pCommonInterface, CMainBlock* block, CIndicatorList* pIndicatorList, CPacketList* pPacketList)
	{
		if(!m_hLib) 
			return CIndicatorConfigurationData::LOAD_ERROR;

		m_pfpLoadAllIndicatorConfiguration	= (fpLoadAllIndicatorConfiguration)::GetProcAddress(m_hLib, "LoadAllIndicatorConfiguration");
		if(m_pfpLoadAllIndicatorConfiguration == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return CIndicatorConfigurationData::LOAD_ERROR;
		}
		return m_pfpLoadAllIndicatorConfiguration(pCommonInterface, block, pIndicatorList, pPacketList);
	}

	bool GetChangedData(CIndicatorConfigurationChangedData& changedData)
	{
		if(!m_hLib) 
			return false;//CS_LOADERROR;

		m_pfpGetChangedData	= (fpGetChangedData)::GetProcAddress(m_hLib, "GetChangedData");
		if(m_pfpGetChangedData == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;//CS_LOADERROR;
		}
		return m_pfpGetChangedData(changedData);
	}

	// (2004.09.06, 배승원) 선택된 Graph별 지표 설정을 받는도록 하는 기능에서,
	//		별도의 Check Button으로 Default (User Default) Indicator Info도 변경하도록 지원할 것인지를
	//		설정받아 처리토록 한다.
	void SetWithDefaultIndicator( BOOL p_bWithDefaultInfo)
	{
		if(!m_hLib) return;

		m_pfpSetWithDefaultIndicator = ( fpSetWithDefaultIndicator)::GetProcAddress( m_hLib, "SetWithDefaultIndicator");
		if(m_pfpSetWithDefaultIndicator == NULL)
		{
			MAKESTATE( LDS_FAIL, LDR_GETPROCADDRESS);
			return;
		}
		m_pfpSetWithDefaultIndicator( p_bWithDefaultInfo);
	}
};

#endif // !defined(AFX_INDICATORCONFIGURATIONHELPER_H__AC973E75_E86C_4ABF_981E_83EC5441B56B__INCLUDED_)
