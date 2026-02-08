#ifndef __LOAD_FOR_ORDERMANAGER__BY_JUNOK_LEE__HEADER__
#define __LOAD_FOR_ORDERMANAGER__BY_JUNOK_LEE__HEADER__

// #include "../../_Include/LoadOrderMng.h"

typedef  LONG (WINAPI* FpOrderMng_GetInferface)(int);

class COrderMngLoader
{
//	Constructor..Destructor..
public:
	COrderMngLoader()
	{
		m_hLib = NULL;
		SetLibName(NULL);
		// m_hLib = LoadLib();
	}

//	COrderMngLoader(LPCSTR _szLibName=NULL)
//	{
//		m_hLib = NULL;
//		SetLibName(_szLibName);
//		m_hLib = LoadLib();
//	}

	~COrderMngLoader()
	{
		UnLoadLib();
	}

public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	FpOrderMng_GetInferface	m_fpOrderMng_GetInferface;

	void SetLibName(LPCSTR _szLibName)
	{
		if(_szLibName==NULL)
			m_szLibName = "OrderMng.dll";
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

	LONG OrderMng_GetInferface(int nType)
	{
		if(!m_hLib)	
		{
			m_hLib = LoadLib();
			if(!m_hLib)
				return NULL;
		}

		if(m_fpOrderMng_GetInferface==NULL)
			m_fpOrderMng_GetInferface = (FpOrderMng_GetInferface)::GetProcAddress(m_hLib,"GetInferface");

		if(m_fpOrderMng_GetInferface == NULL)
		{
			AfxMessageBox("OrderMng.dll에서 GetInferface함수를 찾을 수 없습니다.");
			return NULL;
		}

		return m_fpOrderMng_GetInferface(nType);
	}
};


#endif //__LOAD_FOR_ORDERMANAGER__BY_JUNOK_LEE__HEADER__
