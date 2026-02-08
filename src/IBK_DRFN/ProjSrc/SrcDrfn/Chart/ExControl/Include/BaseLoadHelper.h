#if !defined(AFX_BASEANY2TRADERMAINLOADHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
#define AFX_BASEANY2TRADERMAINLOADHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_

class CExLoadLib
{
public:
	CExLoadLib() 
	{
		m_hLib = NULL;
	}

	~CExLoadLib()
	{
		UnLoadLib();
	}
	
	virtual BOOL LoadProcAddress() 
	{ 
		ASSERT(FALSE); 
		return FALSE;
	}

	HMODULE LoadLib()
	{
		HMODULE	hLib = NULL;
		hLib = ::LoadLibrary(m_szDllName);

		DWORD err = GetLastError();

		if(!hLib) 
		{
			char szMsg[MAX_PATH] = {0, };
			::sprintf(szMsg, "DLL[%s]을 Load할 수 없습니다. [%d]", m_szDllName, err);
			AfxMessageBox(szMsg);

			return NULL;
		}

		m_hLib = hLib;

		if(!LoadProcAddress()) 
		{
			UnLoadLib();
			
			return NULL;
		}

		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(!m_hLib)
			return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		
		m_hLib = NULL;

		return bResult;
	}

	void GetCurrentWorkDirectory(char* szPath, int nSize)
	{
		::GetModuleFileName(AfxGetInstanceHandle(), szPath, nSize);
                int i = 0;
		for( i = ::strlen(szPath) - 1; i >= 0 && szPath[i] != '\\'; i--);

		szPath[i] = 0x00;
	}

// Attibutes
public:
	HMODULE		m_hLib;
	char		m_szDllName[MAX_PATH];
};

#endif // !defined(AFX_BASEANY2TRADERMAINLOADHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
