// EnvironmentConfigurationHelper.h: interface for the CChartConfigurationHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENVIRONMENTCONFIGURATIONHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_)
#define AFX_ENVIRONMENTCONFIGURATIONHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"
#include "EnvironmentData.h"
//////////////////////////////////////////////////////////////////////////////
//Dll에서 구현되어 있는 함수 형태의 포인터 형 선언..
typedef void (WINAPI *fpLoadEnvironmentConfiguration)(const CEnvironmentData& );
typedef bool (WINAPI* fpGetChartSkinData)(const CString&, const CString&, const int, CStyleData& ); //sy 2005.04.26.

class CEnvironmentConfigurationHelper : public CLoadDllLib
{
public:
	BOOL m_delAuto;
	fpLoadEnvironmentConfiguration  m_pfpLoadEnvironmentConfiguration;
	fpGetChartSkinData  m_pfpGetChartSkinData;//sy 2005.04.26.

	CEnvironmentConfigurationHelper(LPCSTR _szDll)
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"EnvironmentConfiguration.dll");
		else
			m_szDllName = _T("EnvironmentConfiguration.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadEnvironmentConfiguration);
		MakeNull(m_pfpGetChartSkinData);
		LoadLib();
	}

	CEnvironmentConfigurationHelper(BOOL deleteAuto)
	{
		m_delAuto = TRUE;
		m_szDllName = _T("EnvironmentConfiguration.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadEnvironmentConfiguration);
		MakeNull(m_pfpGetChartSkinData);
		LoadLib();

		m_delAuto = deleteAuto;
	}

	CEnvironmentConfigurationHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("EnvironmentConfiguration.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadEnvironmentConfiguration);
		MakeNull(m_pfpGetChartSkinData);
		LoadLib();
	}

	~CEnvironmentConfigurationHelper()
	{
	}

	int LoadEnvironmentConfiguration(const CEnvironmentData& environmentdata)
	{
		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;

		m_pfpLoadEnvironmentConfiguration	= (fpLoadEnvironmentConfiguration)::GetProcAddress(m_hLib, "LoadEnvironmentConfiguration");
		if(m_pfpLoadEnvironmentConfiguration==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		m_pfpLoadEnvironmentConfiguration(environmentdata);
		MAKESTATE(LDS_OK, LDR_READY);
		return GetState();//nRet;
	}

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

#endif // !defined(AFX_ENVIRONMENTCONFIGURATIONHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_)
