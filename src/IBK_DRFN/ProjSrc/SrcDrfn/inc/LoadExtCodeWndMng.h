#ifndef __LOAD_FOR_EXTCODEWNDMANAGER__BY_JUNOK_LEE__HEADER__
#define __LOAD_FOR_EXTCODEWNDMANAGER__BY_JUNOK_LEE__HEADER__

// #include "../../_Include/LoadTreeListMng.h"

typedef  LONG (WINAPI* FpECW_GetInferface)(long);

class CExtCodeWndMngLoader
{
public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	FpECW_GetInferface	m_fpGetInferface;

//	Constructor..Destructor..
public:
	CExtCodeWndMngLoader()
	{
		m_hLib = NULL;
		m_szLibName = "ExtCodeWnd.dll";
		m_fpGetInferface = NULL;
	}

	~CExtCodeWndMngLoader()
	{
		UnLoadLib();
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

		m_fpGetInferface = (FpECW_GetInferface)::GetProcAddress(m_hLib, "GetInferface");
	}


	LONG ECW_GetInferface(long pIGateMng)
	{
		if(!m_hLib ||!m_fpGetInferface)	 return NULL;

		return m_fpGetInferface(pIGateMng);
	}
};


#endif //__LOAD_FOR_EXTCODEWNDMANAGER__BY_JUNOK_LEE__HEADER__
