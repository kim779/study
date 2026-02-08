#ifndef __LOAD_MCHARTUTIL_LIB__HEADER__
#define __LOAD_MCHARTUTIL_LIB__HEADER__

//#include "../../inc/LoadMChartUtil.h"

// 차트화면 프린트지원
typedef  LONG  (WINAPI* LIBUtilScreenPrint)(CWnd* pTarWnd);

// 성공하면 생성된 key값, 실패NULL
typedef  LONG  (WINAPI* LIBUtilFDlgAdvise)();

// 성공하면 dwKey리턴. 실패NULL
typedef  void (WINAPI* LIBUtilFDlgUnAdvise)(LONG dwKey);

class CMChartUtilLoadLib
{
//	Constructor..Destructor..
public:
	LIBUtilScreenPrint			m_fpUtilScreenPrint;
	LIBUtilFDlgAdvise			m_fpUtilFDlgAdvise;
	LIBUtilFDlgUnAdvise			m_fpUtilFDlgUnAdvise;

public:
	CMChartUtilLoadLib()
	{
		//@Solomon의 Chart경로는 dev.
		m_szLibName = "dev\\MChartUtil.dll";
		m_hLib = NULL;

		m_fpUtilScreenPrint		= NULL;
		m_fpUtilFDlgAdvise		= NULL;
		m_fpUtilFDlgUnAdvise	= NULL;

		LoadLib();
	}
	
	~CMChartUtilLoadLib()
	{
		UnLoadLib();
	}

public:
	HINSTANCE m_hLib;
	CString m_szLibName;

	HINSTANCE LoadLib()
	{
		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			TRACE("%s DLL을 Load할 수 없습니다.\n", m_szLibName);
			// AfxMessageBox("Drds DLL을 Load할 수 없습니다.");
			return NULL;
		}

		m_hLib = hLib;
		LoadProcess();
		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}

	void LoadProcess()
	{
		if(!m_hLib)	return ;

		m_fpUtilScreenPrint = (LIBUtilScreenPrint)::GetProcAddress(m_hLib, "UtilScreenPrint");
		
		m_fpUtilFDlgAdvise		= (LIBUtilFDlgAdvise)::GetProcAddress(m_hLib, "UtilFDlgAdvise");
		m_fpUtilFDlgUnAdvise	= (LIBUtilFDlgUnAdvise)::GetProcAddress(m_hLib, "UtilFDlgUnAdvise");
	}

	LONG UtilScreenPrint(CWnd* pWnd)
	{
		if(!m_hLib || !m_fpUtilScreenPrint) return NULL;
		return m_fpUtilScreenPrint(pWnd);
	}

	LONG  UtilFDlgAdvise()
	{
		if(!m_hLib || !m_fpUtilFDlgAdvise) return NULL;
		return m_fpUtilFDlgAdvise();
	}

	void UtilFDlgUnAdvise(LONG dwKey)
	{
		if(!m_hLib || !m_fpUtilFDlgUnAdvise) return;
		m_fpUtilFDlgUnAdvise(dwKey);
	}
};


#endif //__LOAD_MCHARTUTIL_LIB__HEADER__
