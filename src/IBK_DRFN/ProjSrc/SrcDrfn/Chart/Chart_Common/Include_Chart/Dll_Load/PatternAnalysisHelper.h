// PatternAnalysisHelper.h: interface for the CPatternAnalysisHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERNANALYSISHELPER_H__12856503_12F8_4834_8324_87AC01F7B38D__INCLUDED_)
#define AFX_PATTERNANALYSISHELPER_H__12856503_12F8_4834_8324_87AC01F7B38D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"

class CPacketList;
class CMainBlock;

typedef CWnd*	(WINAPI *fpLoadPatternAnalysisWnd)(CPacketList*, CMainBlock*, CString, CWnd*);
typedef void	(WINAPI *fpShowPattern)(CWnd*, CPoint);
typedef int		(WINAPI *fpDeletePatternAnalysisDlg)(CWnd*);
typedef void	(WINAPI *fpReCalcPatternData)(CWnd*);

class CPatternAnalysisHelper : public CLoadDllLib  
{
public:
	fpLoadPatternAnalysisWnd		m_pfpLoadPatternAnalysisWnd; 
	fpDeletePatternAnalysisDlg		m_pfpDeletePatternAnalysisDlg;
	fpShowPattern					m_pfpShowPattern;
	fpReCalcPatternData				m_pfpReCalcPatternData;

	BOOL							m_delAuto;

	CPatternAnalysisHelper(LPCSTR _szDll)
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"PatternAnalysis.dll");
		else
			m_szDllName = _T("PatternAnalysis.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadPatternAnalysisWnd);
		MakeNull(m_pfpDeletePatternAnalysisDlg);
		MakeNull(m_pfpShowPattern);
		MakeNull(m_pfpReCalcPatternData);
		LoadLib();
	}

	CPatternAnalysisHelper(BOOL deleteAuto)
	{
		m_delAuto = TRUE;
		m_szDllName = _T("PatternAnalysis.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadPatternAnalysisWnd);
		MakeNull(m_pfpDeletePatternAnalysisDlg);
		MakeNull(m_pfpShowPattern);
		MakeNull(m_pfpReCalcPatternData);
		LoadLib();

		m_delAuto = deleteAuto;
	}

	CPatternAnalysisHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("PatternAnalysis.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadPatternAnalysisWnd);
		MakeNull(m_pfpDeletePatternAnalysisDlg);
		MakeNull(m_pfpShowPattern);
		MakeNull(m_pfpReCalcPatternData);
		LoadLib();
	}

	~CPatternAnalysisHelper()
	{
	}

	CWnd* fnLoadPatternAnalysisWnd(CPacketList* pPacketList, CMainBlock* pMainBlock, CString strFilePath, CWnd* pParent)
	{
		if(!m_hLib) 
			//return m_bState;//CS_LOADERROR;
			return NULL;

		m_pfpLoadPatternAnalysisWnd	= (fpLoadPatternAnalysisWnd)::GetProcAddress(m_hLib, "fnLoadPatternAnalysisWnd");
		if(m_pfpLoadPatternAnalysisWnd == NULL)
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			//return GetState();
			return NULL;
		}
		CWnd *wnd = m_pfpLoadPatternAnalysisWnd(pPacketList, pMainBlock, strFilePath, pParent);
		MAKESTATE(LDS_OK, LDR_READY);

		//return GetState();
		return wnd;
	}

	int fnDeletePatternAnalysisDlg(CWnd* pParent)
	{
		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;

		m_pfpDeletePatternAnalysisDlg	= (fpDeletePatternAnalysisDlg)::GetProcAddress(m_hLib, "fnDeletePatternAnalysisDlg");
		if(m_pfpDeletePatternAnalysisDlg == NULL)
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();
		}
		m_pfpDeletePatternAnalysisDlg(pParent);
		MAKESTATE(LDS_OK, LDR_READY);

		return GetState();
	}

	void WINAPI fnShowPattern(CWnd* pParent, CPoint point)
	{
		if(!m_hLib) 
			return;

		m_pfpShowPattern	= (fpShowPattern)::GetProcAddress(m_hLib, "fnShowPattern");
		if(m_pfpShowPattern == NULL)
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return;
		}
//		m_pfpShowPattern(nIndex);
		m_pfpShowPattern(pParent, point);
		MAKESTATE(LDS_OK, LDR_READY);

		return;
	}

	void WINAPI fnReCalcPatternData(CWnd* pParent)
	{
		if(!m_hLib) 
			return;

		m_pfpReCalcPatternData = (fpReCalcPatternData)::GetProcAddress(m_hLib, "fnReCalcPatternData");
		if(m_pfpReCalcPatternData == NULL)
		{
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return;
		}
		m_pfpReCalcPatternData(pParent);
		MAKESTATE(LDS_OK, LDR_READY);

		return;
		
	}
};

#endif // !defined(AFX_PATTERNANALYSISHELPER_H__12856503_12F8_4834_8324_87AC01F7B38D__INCLUDED_)
