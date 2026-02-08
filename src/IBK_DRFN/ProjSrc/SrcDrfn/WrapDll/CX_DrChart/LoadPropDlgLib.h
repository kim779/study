#ifndef __LOAD_FOR_ORDERMANAGER__BY_JUNOK_LEE__HEADER__
#define __LOAD_FOR_ORDERMANAGER__BY_JUNOK_LEE__HEADER__

// #include "../../_Include/LoadOrderMng.h"
#include "../../ForSite/FS_Env.h"
typedef  char* (WINAPI* axModalPropDlg)(CWnd* , const char* , const char* );
typedef  int (WINAPI* drPropertySetDlg)(LPCSTR pszFullPath, int nMode, long _pDataList);

class CPropDlgLib
{
//	Constructor..Destructor..
public:
	CPropDlgLib()
	{
		m_hLib = NULL;
		m_axModalPropDlg = NULL;
		m_drPropertySetDlg = NULL;
		SetLibName(NULL);
	}

	~CPropDlgLib()
	{
		UnLoadLib();
	}

public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	axModalPropDlg	m_axModalPropDlg;
	drPropertySetDlg m_drPropertySetDlg;

	void SetLibName(LPCSTR _szLibName)
	{
		if (_szLibName==NULL)
			m_szLibName = "CX_DrChartDgnDlg.dll";
		else
			m_szLibName = _szLibName;
	}


	HINSTANCE LoadLib()
	{
		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			CString szMsg;
			szMsg.Format("%s을 Load할 수 없습니다.", m_szLibName);
			TRACE("%s\n", szMsg);
			// AfxMessageBox(szMsg);
			return NULL;
		}
		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}

	char* FnAxModalPropDlg(CWnd* pWnd, const char *pszData, const char *pszFullPath)
	{
		if (!m_hLib)
		{
			m_hLib = LoadLib();
			if (!m_hLib)
				return NULL;
		}

		if (m_axModalPropDlg == NULL)
			m_axModalPropDlg = (axModalPropDlg)::GetProcAddress(m_hLib, "axModalPropDlg");

		if (m_axModalPropDlg == NULL)
		{
			//AfxMessageBox("OrderMng.dll에서 GetInferface함수를 찾을 수 없습니다.");
			return NULL;
		}

		return m_axModalPropDlg(pWnd, pszData, pszFullPath);
	}

	int FndrPropertySetDlg(LPCSTR pszFullPath, int nMode, CStringList* pDataList, int &rRet)
	{
		if (!m_hLib)
		{
			m_hLib = LoadLib();
			if (!m_hLib)
				return 0;
		}
		
		if (m_drPropertySetDlg == NULL)
			m_drPropertySetDlg = (drPropertySetDlg)::GetProcAddress(m_hLib, "drPropertySetDlg");
		
		if (m_drPropertySetDlg == NULL)
		{
			//AfxMessageBox("OrderMng.dll에서 GetInferface함수를 찾을 수 없습니다.");
			return 0;
		}
		rRet = m_drPropertySetDlg(pszFullPath, nMode, (long)pDataList);
		return 1;
	}
};

#endif //__LOAD_FOR_ORDERMANAGER__BY_JUNOK_LEE__HEADER__
