// ChartCheckerModuleHelper.h: interface for the CChartCheckerModuleHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTCHECKERMODULEHELPER_H__E47D3C0F_AD5B_42AA_9584_4E8331D1A354__INCLUDED_)
#define AFX_CHARTCHECKERMODULEHELPER_H__E47D3C0F_AD5B_42AA_9584_4E8331D1A354__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"

class CMainBlock;

typedef bool (WINAPI* fpLoadIndicatorDataChecket)(CWnd* , CMainBlock* , const CString& );


class CChartCheckerModuleHelper : public CLoadDllLib
{
public:
	CChartCheckerModuleHelper(LPCSTR _szDll)
	{
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"ChartCheckerModule.dll");
		else
			m_szDllName = _T("ChartCheckerModule.dll");

		InitializeAllData();
		LoadLib();
	}

	virtual ~CChartCheckerModuleHelper()
	{
	}


	// indicator data checker load
	bool LoadIndicatorDataChecket(CWnd* pParent, CMainBlock* pMainBlock, const CString& strPath)
	{
		if(!m_hLib)
			return false;

		m_pfpLoadIndicatorDataChecket = (fpLoadIndicatorDataChecket)::GetProcAddress(m_hLib, "LoadIndicatorDataChecket");
		if(m_pfpLoadIndicatorDataChecket == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;
		}
		return m_pfpLoadIndicatorDataChecket(pParent, pMainBlock, strPath);
	}

private:
	fpLoadIndicatorDataChecket m_pfpLoadIndicatorDataChecket;

	void InitializeAllData()
	{
		MakeNull(m_pfpLoadIndicatorDataChecket);
		m_pfpLoadIndicatorDataChecket = NULL;
	}
};

#endif // !defined(AFX_CHARTCHECKERMODULEHELPER_H__E47D3C0F_AD5B_42AA_9584_4E8331D1A354__INCLUDED_)
