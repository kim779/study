#ifndef _NEOPLATFORM__INCLUDE_LOADDLL__HEADER__
#define _NEOPLATFORM__INCLUDE_LOADDLL__HEADER__

#define		MakeNull(xxVal)			xxVal=NULL
#define		SafeDelete(xxVal)		if(xxVal) delete xxVal; xxVal=NULL

// LoadDll State => LDS_
#define		LDS_OK				TRUE
#define		LDS_SUCCESS			TRUE
#define		LDS_ERROR			FALSE
#define		LDS_FAIL			FALSE

// 0이하일때는 실패,에러의 상태값
// 0이상일때 실패,에러가 아닌 상태값
// LoadDLL Result => LDR_
#define		LDR_READY				0		// 준비상태
#define		LDR_LOADERROR			-1		// DLL Load Error
#define		LDR_UNLOADERROR			-2		// DLL Unload Error
#define		LDR_GETPROCADDRESS		-3		// GetProcAddress Error
#define		LDR_INPUTERROR			-4		// 입력(파라미터)상의 에러
#define		LDR_OUTPUTERROR			-5		// 출력상의 에러

#define		MAKESTATE(xxState, xxResult)		m_bState=xxState; m_nResult=xxResult;
class CLoadDllLib
{
public:
	CLoadDllLib() {
		MAKESTATE(LDS_OK, LDR_READY);
		m_hLib = NULL;		
	}
	~CLoadDllLib()
	{
		UnLoadLib();
	}

	HMODULE LoadLib()
	{
		CString szDllName = m_szDllName;
		HMODULE		hLib = NULL;
		hLib = AfxLoadLibrary(szDllName);
		if(hLib==NULL) {
			MAKESTATE(LDS_FAIL, LDR_LOADERROR);

			CString msg;
			msg.Format("DLL[%s]을 Load할 수 없습니다.", szDllName);
			TRACE("%s\n", msg);
			// AfxMessageBox(msg);
			return NULL;
		}

		MAKESTATE(LDS_OK, LDR_READY);
		TRACE("Loaded %s\n", m_szDllName);

		m_hLib = hLib;
		return hLib;
	}
	BOOL UnLoadLib()
	{
		if(!m_hLib) {
			MAKESTATE(LDS_FAIL, LDR_UNLOADERROR);
			return TRUE;
		}

		BOOL bResult = AfxFreeLibrary(m_hLib);
		m_hLib = NULL;

		MAKESTATE(LDS_OK, LDR_READY);
		TRACE("UnLoaded %s\n", m_szDllName);

		return bResult;
	}
	BOOL GetState()   { return m_bState; }
	BOOL GetResult()  { return m_nResult; }

// Attibutes
public:
	HMODULE		m_hLib;
	CString		m_szDllName;
	int			m_bState,		//  상태값 : 에러.실패(0), 성공(1)
				m_nResult;		//	에러, 성공 실패등을 나타내는 값

};

class CResourceHelp
{
public:
	HINSTANCE m_hOld;
	CResourceHelp(HINSTANCE hOld, HINSTANCE hNew)
	{
		m_hOld = hOld;
		AfxSetResourceHandle(hNew);
	}

	~CResourceHelp()
	{
		AfxSetResourceHandle(m_hOld);
	}
};


#endif //_NEOPLATFORM__INCLUDE_LOADDLL__HEADER__
