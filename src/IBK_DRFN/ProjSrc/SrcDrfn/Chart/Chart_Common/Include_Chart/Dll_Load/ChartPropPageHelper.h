// ChartPropPageHelper.h: interface for the CChartPropPageHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTPROPPAGEHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_)
#define AFX_CHARTPROPPAGEHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "NeoPlatform/include/LoadDll.h"
//#include "../../include/LoadDll.h"
#include "LoadDll.h"

class CIndicatorList;
//////////////////////////////////////////////////////////////////////////////
//Dll에서 구현되어 있는 함수 형태의 포인터 형 선언..
typedef int (WINAPI *fpInitializeControlDlg)(CWnd*, CString&);
typedef int (WINAPI *fpSetPropertyPage)(CWnd*, CString&, int, int, CIndicatorList*);
typedef int (WINAPI *fpLoadFontPage)(CWnd* pWnd, const CString& );

class CChartPropPageHelper : public CLoadDllLib
{
public:
//GetProcAddress를 이용해서 얻어온 dll함수의 포인터를 대입하기 위한 변수 선언.
	fpInitializeControlDlg  m_pfpInitializeControlDlg;
	fpSetPropertyPage       m_pfpSetPropertyPage;
	fpLoadFontPage          m_pfpLoadFontPage;

	BOOL					m_delAuto;

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.10.16
//
//	목적	:	SkcChart와 NSkcChart가 같은 ChartPropertyPage DLL을 Loading하지 않도록,
//				Module 명을 지정토록한다.
///////////////////////////////////////////////////////////////////////////////
//CLoadDllLib에 구현되어 있는 LoadLib 호출..
	CChartPropPageHelper(LPCSTR _szDll, LPCSTR _szFileName = "ChartPropertyPage.dll")
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll, _szFileName);
		else
			m_szDllName = _szFileName;
		m_hLib = NULL;
		MakeNull(m_pfpInitializeControlDlg);
		MakeNull(m_pfpSetPropertyPage);
		MakeNull(m_pfpLoadFontPage);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}
	}

	CChartPropPageHelper(BOOL deleteAuto)
	{
		m_szDllName = _T("ChartPropertyPage.dll");
		m_hLib = NULL;
		MakeNull(m_pfpInitializeControlDlg);
		MakeNull(m_pfpSetPropertyPage);
		MakeNull(m_pfpLoadFontPage);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}

		m_delAuto = deleteAuto;
	}


	CChartPropPageHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("ChartPropertyPage.dll");
		m_hLib = NULL;
		MakeNull(m_pfpInitializeControlDlg);
		MakeNull(m_pfpSetPropertyPage);
		MakeNull(m_pfpLoadFontPage);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}
	}

	~CChartPropPageHelper()
	{
	}

	//Dll에 구현되어 있는 함수와 같은이름을 가진 함수 선언과 구현,
	int InitializeControlDlg(CWnd* pWnd, CString& strProp)
	{
		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;
			
	//GetProcAddress를 이용해서 Dllg핸들을 이용해서 ShowPropertyPage의 주소를 얻어옴.
		m_pfpInitializeControlDlg	= (fpInitializeControlDlg)::GetProcAddress(m_hLib, "InitializeControlDlg");
		if(m_pfpInitializeControlDlg==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		
		//m_pfpShowPropertyPage에 넘어온 값이 NULL이 아니면 Dll의 함수 호출.
		return m_pfpInitializeControlDlg(pWnd, strProp);
	}

	int SetPropertyPage(CWnd* pWnd, CString& strProp, int nRow, int nColumn, CIndicatorList* pIndicatorList)
	{

		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;
			
		m_pfpSetPropertyPage	= (fpSetPropertyPage)::GetProcAddress(m_hLib, "SetPropertyPage");
		if(m_pfpSetPropertyPage==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		return m_pfpSetPropertyPage(pWnd, strProp, nRow, nColumn, pIndicatorList);
	}

	int LoadFontPage(CWnd* pWnd, const CString& strFontData)
	{

		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;
			
		m_pfpLoadFontPage = (fpLoadFontPage)::GetProcAddress(m_hLib, "LoadFontPage");
		if(m_pfpLoadFontPage==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		return m_pfpLoadFontPage(pWnd, strFontData);
	}

};

#endif // !defined(AFX_CHARTPROPPAGEHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_)
