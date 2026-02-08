#ifndef __LOAD_FOR_PARTNERMANAGER__BY_JUNOK_LEE__HEADER__
#define __LOAD_FOR_PARTNERMANAGER__BY_JUNOK_LEE__HEADER__

// #include "../../_Include/LoadTreeListMng.h"

typedef  long (WINAPI* FpPTM_GetInferface)(int, long);
typedef  long (WINAPI* FpPTM_SetInferface)(int, long);
typedef  void (WINAPI* FpPTM_SetPcTrInterface)(int, long);

#include "../ForSite/FS_Env.h"

class CPartnerMngLoader
{
public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	FpPTM_GetInferface	m_fpGetInferface;

//	Constructor..Destructor..
public:
	CPartnerMngLoader()
	{
		m_hLib = NULL;

		GetDir_DrfnBin(m_szLibName);
		m_szLibName += "\\PartnerMng.dll";

		m_fpGetInferface = NULL;

		LoadLib();
	}

	~CPartnerMngLoader()
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

		m_fpGetInferface = (FpPTM_GetInferface)::GetProcAddress(m_hLib, "GetInferface");
	}


	long PTMGetInferface(int nID, long pKey)
	{
		if(!m_hLib ||!m_fpGetInferface)	 return NULL;

		return m_fpGetInferface(nID, pKey);
	}

	long PTMSetInferface(int nID, long pKey)
	{
		if(!m_hLib)	 return NULL;

		FpPTM_SetInferface m_fpFn
			= (FpPTM_SetInferface)::GetProcAddress(m_hLib, "SetInferface");
		
		if(m_fpFn)
			return m_fpFn(nID, pKey);

		return NULL;
	}

	void PTMSetPcTrInterface(int nID, long pKey)
	{
		if(!m_hLib)	 return ;
		
		FpPTM_SetPcTrInterface m_fpFn
			= (FpPTM_SetPcTrInterface)::GetProcAddress(m_hLib, "SetPcTrInterface");

		if(m_fpFn)	m_fpFn(nID, pKey);
	}
};


#endif //__LOAD_FOR_PARTNERMANAGER__BY_JUNOK_LEE__HEADER__
