#ifndef __LOAD_FOR_DRDEBUGER__BY_JUNOK_LEE__HEADER__
#define __LOAD_FOR_DRDEBUGER__BY_JUNOK_LEE__HEADER__

#define		def_DRDebuger_GetProc(pInterface, type_pInterface, dllInterface) if(!m_hLib) return NULL;	\
		if(!pInterface)	{																		\
			pInterface = (type_pInterface)::GetProcAddress(m_hLib, dllInterface);				\
		if(!pInterface)		return NULL;														\
	}

#define		def_DRDebuger_CallProc(pInterface, type_pInterface, dllInterface) if(!m_hLib) return ;	\
		if(!pInterface)	{																		\
			pInterface = (type_pInterface)::GetProcAddress(m_hLib, dllInterface);				\
		if(!pInterface)		return ;															\
	}


#define		g_dCDRDebugerLoader_Version		(1.00)
#define		g_DRDebugerLoader_DLL			"DRDebuger.dll"
#define		g_DRDebugerLoader_TryCount		(10)
class CDRDebugerLoader
{
protected:

	typedef  long (WINAPI* FpDRLog_GetInferface)(int, long);
	typedef  long (WINAPI* FpDRLog_SetInferface)(int, long);
	typedef  void (WINAPI* FpDRLog_Message)(LPCSTR p_szMessage);
	typedef  void (WINAPI* FpDRLog_MessageEx)(LPCSTR p_szMessage, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_RequestData)(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_ResponseData)(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_RealAdvise)(LPCSTR p_szRealKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_RealUnAdvise)(LPCSTR p_szRealKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_RealData)(LPCSTR p_szRealKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_DebugData)(LPCSTR p_szDataKey, LPBYTE p_Data, long p_DataLen);	//DataKey 한개의 데이터키
	typedef  void (WINAPI* FpDRLog_DebugDataEx)(LPCSTR p_szDataKey, LPBYTE p_Data, long p_DataLen);	//DataKey조합
	typedef  void (WINAPI* FpDRLog_LogData)(LPCSTR p_szType, LPCSTR p_szFlag, LPCSTR p_Data, long p_DataLen);
	typedef  void (WINAPI* FpDRLog_OutputDebugString)(LPCSTR p_szMessage);
	typedef  void (WINAPI* FpDRLog_RequestSR)(BYTE cAU, BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey);
	typedef  void (WINAPI* FpDRLog_RequestFileData)(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_ResponseFileData)(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_RequestStringsData)(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_ResponseStringsData)(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData);
	typedef  void (WINAPI* FpDRLog_FileTranWithData1)(LONG hSender, BYTE cIO, LPCSTR pTranName, CStringList *pDataList);
	typedef  void (WINAPI* FpDRLog_FileTranWithData2)(LONG hSender, BYTE cIO, LPCSTR pTranName, LPVOID pFileData, DWORD dwCount);
	typedef  void (WINAPI* FpDRLog_FileTranWithDirect)(LONG hSender, BYTE cIO, LPCSTR pTranName, LPCSTR pszFileName);


	HINSTANCE m_hLib;
	CString m_szLibName;
	FpDRLog_GetInferface				m_fpGetInferface;
	FpDRLog_SetInferface				m_fpSetInferface;
	FpDRLog_Message						m_fpMessage;
	FpDRLog_MessageEx					m_fpMessageEx;
	FpDRLog_RequestData					m_fpRequestData;
	FpDRLog_ResponseData				m_fpResponseData;
	FpDRLog_RealAdvise					m_fpRealAdvise;
	FpDRLog_RealUnAdvise				m_fpRealUnAdvise;
	FpDRLog_RealData					m_fpRealData;
	FpDRLog_DebugData					m_fpDebugData;
	FpDRLog_DebugDataEx					m_fpDebugDataEx;
	FpDRLog_LogData						m_fpLogData;
	FpDRLog_OutputDebugString			m_fpOutputDebugString;
	FpDRLog_RequestSR					m_fpRequestSR;
	FpDRLog_RequestFileData				m_fpRequestFileData;
	FpDRLog_ResponseFileData			m_fpResponseFileData;
	FpDRLog_RequestStringsData			m_fpRequestStringsData;
	FpDRLog_ResponseStringsData			m_fpResponseStringsData;
	FpDRLog_FileTranWithData1			m_fpFileTranWithData1;
	FpDRLog_FileTranWithData2			m_fpFileTranWithData2;
	FpDRLog_FileTranWithDirect			m_fpFileTranWithDirect;

public:
	CString		m_szMessage;
	double		m_dLoaderVersion;

protected:
	int			m_nLoadTry;

//	Constructor..Destructor..
public:
	CDRDebugerLoader(BOOL bAutoLoad=TRUE)
	{
		m_nLoadTry = 0;
		m_hLib = NULL;
		m_dLoaderVersion = g_dCDRDebugerLoader_Version;

		GetExeDir(m_szLibName);
		m_szLibName += "\\";
		m_szLibName += g_DRDebugerLoader_DLL;	//exe와 같은 위치에 존재.
		//OutputDebugString(m_szLibName);

		m_fpGetInferface = NULL;
		m_fpSetInferface = NULL;
		m_fpMessage = NULL;
		m_fpMessageEx = NULL;
		m_fpRequestData = NULL;
		m_fpResponseData = NULL;
		m_fpRealAdvise = NULL;
		m_fpRealUnAdvise = NULL;
		m_fpRealData = NULL;
		m_fpDebugData = NULL;
		m_fpDebugDataEx = NULL;
		m_fpLogData = NULL;
		m_fpOutputDebugString = NULL;
		m_fpRequestSR = NULL;
		m_fpRequestFileData = NULL;
		m_fpResponseFileData = NULL;
		m_fpRequestStringsData = NULL;
		m_fpResponseStringsData = NULL;
		m_fpFileTranWithData1 = NULL;
		m_fpFileTranWithData2 = NULL;
		m_fpFileTranWithDirect = NULL;

		if(bAutoLoad)	LoadLib();
	}

	~CDRDebugerLoader()
	{
		UnLoadLib();
	}

	BOOL IsLoaded() {
		if(m_hLib) return TRUE;

		if(m_nLoadTry<g_DRDebugerLoader_TryCount) {
			m_nLoadTry++;
			return FALSE;
		}
		else {
			LoadLib();			
			return (BOOL)m_hLib;
		}
	}

	void GetExeDir(CString &strDir)
	{
		//CAZUtil_EnvInfo::GetChartBinDir(strDir);

		char _szBuf_[255] = {0,};
		::GetModuleFileName(NULL, _szBuf_, 255);
			
		CString	_strRoot_ = _szBuf_;
		
		int _loc_ = _strRoot_.ReverseFind('\\');
		if( _loc_ > 0)
			_strRoot_ = _strRoot_.Left(_loc_);

		strDir = _strRoot_;
	}

	HINSTANCE LoadLib()
	{
		m_nLoadTry = 0;
		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			CString szMsg;
			szMsg.Format("[drfn]DLL(%s)을 Load할 수 없습니다.\n", m_szLibName);
			//OutputDebugString(szMsg);
			// AfxMessageBox(szMsg);

			//CString szCLibName;
			//szCLibName.Format("C:\\DRfn\\%s", g_DRDebugerLoader_DLL);
			//hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)szCLibName);

			if( hLib == NULL)
				return NULL;
		}

		m_hLib = hLib;
		m_nLoadTry = g_DRDebugerLoader_TryCount;

		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;
		m_nLoadTry = 0;

		return bResult;
	}

	long DRLog_GetInferface(int nID, long pKey, long &result)
	{
		def_DRDebuger_GetProc(m_fpGetInferface, FpDRLog_GetInferface, "DRLog_GetInferface");

		result = m_fpGetInferface(nID, pKey);
		return 1;
	}

	long DRLog_SetInferface(int nID, long pKey, long &result)
	{
		def_DRDebuger_GetProc(m_fpSetInferface, FpDRLog_SetInferface, "DRLog_SetInferface");

		result = m_fpSetInferface(nID, pKey);
		return 1;
	}

	long DRLog_Message(LPCSTR p_szMessage=NULL)
	{
		def_DRDebuger_GetProc(m_fpMessage			, FpDRLog_Message			, "DRLog_Message");
		if(p_szMessage==NULL)
			m_fpMessage(m_szMessage);
		else
			m_fpMessage(p_szMessage);

		return 1;
	}

	long DRLog_MessageEx(LPCSTR p_szMessage=NULL, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpMessageEx			, FpDRLog_MessageEx			, "DRLog_MessageEx");
		if(p_szMessage==NULL)
			m_fpMessageEx(m_szMessage, szExtraData);
		else
			m_fpMessageEx(p_szMessage, szExtraData);

		return 1;
	}
	
	long DRLog_RequestData(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpRequestData		, FpDRLog_RequestData		, "DRLog_RequestData");
		m_fpRequestData(p_szIOKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}
	long DRLog_RequestData(LPCSTR p_szIOKey, LPCSTR p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		return DRLog_RequestData(p_szIOKey, (LPBYTE)p_Data, p_DataLen, szExtraData);
	}
	
	long DRLog_ResponseData(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpResponseData		, FpDRLog_ResponseData		, "DRLog_ResponseData");
		m_fpResponseData(p_szIOKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}

	long DRLog_ResponseData(LPCSTR p_szIOKey, LPCSTR p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		return DRLog_ResponseData(p_szIOKey, (LPBYTE)p_Data, p_DataLen, szExtraData);
	}
	
	long DRLog_RealAdvise(LPCSTR p_szRealKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpRealAdvise		, FpDRLog_RealAdvise		, "DRLog_RealAdvise");
		m_fpRealAdvise(p_szRealKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}
	
	long DRLog_RealUnAdvise(LPCSTR p_szRealKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpRealUnAdvise		, FpDRLog_RealUnAdvise		, "DRLog_RealUnAdvise");
		m_fpRealAdvise(p_szRealKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}
	
	long DRLog_RealData(LPCSTR p_szRealKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpRealData			, FpDRLog_RealData			, "DRLog_RealData");
		m_fpRealData(p_szRealKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}

	//p_szDataKey : 한개의 데이터키
	long DRLog_DebugData(LPCSTR p_szDataKey, LPBYTE p_Data, long p_DataLen)
	{
		def_DRDebuger_GetProc(m_fpDebugData			, FpDRLog_DebugData			, "DRLog_DebugData");
		m_fpDebugData(p_szDataKey, p_Data, p_DataLen);
		return 1;
	}

	//p_szDataKey : "key=value\t" 형태로 구분되는 멀티 데이터키(마지막 \t로 구분)
	// ex) p_szDataKey="a=1\tb=2\t"; 
	long DRLog_DebugDataEx(LPCSTR p_szDataKey, LPBYTE p_Data, long p_DataLen)
	{
		def_DRDebuger_GetProc(m_fpDebugDataEx			, FpDRLog_DebugDataEx			, "DRLog_DebugDataEx");
		m_fpDebugDataEx(p_szDataKey, p_Data, p_DataLen);
		return 1;
	}

	long DRLog_LogData(LPCSTR p_szType, LPCSTR p_szFlag, LPCSTR p_Data, long p_DataLen)
	{
		def_DRDebuger_GetProc(m_fpLogData			, FpDRLog_LogData			, "DRLog_LogData");
		m_fpLogData(p_szType, p_szFlag, p_Data, p_DataLen);
		return 1;
	}

	void DRLog_OutputDebugString(LPCTSTR lpszFormat, ...)
	{
		ASSERT(AfxIsValidString(lpszFormat));

		va_list args;
		va_start(args, lpszFormat);
		
		int nBuf;
		TCHAR szBuffer[2048+1]={0,};

		nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf<=0) return;
		ASSERT(nBuf >= 0);
		
		va_end(args);

		def_DRDebuger_CallProc(m_fpOutputDebugString		, FpDRLog_OutputDebugString		, "DRLog_OutputDebugString");
		m_fpOutputDebugString(szBuffer);
	}

	//CDRDebugerLoader::DRLog_Trace()
	static void DRLog_Trace(LPCTSTR lpszFormat, ...)
	{
		ASSERT(AfxIsValidString(lpszFormat));

		va_list args;
		va_start(args, lpszFormat);
		
		int nBuf;
		TCHAR szBuffer[1024+1]={0,};

		nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
		if(nBuf<=0) return;
		ASSERT(nBuf >= 0);
		
		va_end(args);

		CString szDRDebugerMessage;
		szDRDebugerMessage.Format("[DRCS]%s\n", szBuffer);
		OutputDebugString(szDRDebugerMessage);
	}

	long DRLog_SRAdvise(BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey)
	{
		def_DRDebuger_GetProc(m_fpRequestSR		, FpDRLog_RequestSR		, "DRLog_RequestSR");

		BYTE cAU = 'A';
		m_fpRequestSR(cAU, cIO, pTranName, hReceiver, dwOnCopyDataKey);
		return 1;
	}

	long DRLog_SRUnAdvise(BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey)
	{
		def_DRDebuger_GetProc(m_fpRequestSR		, FpDRLog_RequestSR		, "DRLog_RequestSR");

		BYTE cAU = 'U';
		m_fpRequestSR(cAU, cIO, pTranName, hReceiver, dwOnCopyDataKey);
		return 1;
	}

	long DRLog_RequestFileData(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpRequestFileData		, FpDRLog_RequestFileData		, "DRLog_RequestFileData");
		m_fpRequestFileData(p_szIOKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}
	
	long DRLog_ResponseFileData(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpResponseFileData		, FpDRLog_ResponseFileData		, "DRLog_ResponseFileData");
		m_fpResponseFileData(p_szIOKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}

	long DRLog_RequestStringsData(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpRequestStringsData		, FpDRLog_RequestStringsData		, "DRLog_RequestStringsData");
		m_fpRequestStringsData(p_szIOKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}
	
	long DRLog_ResponseStringsData(LPCSTR p_szIOKey, LPBYTE p_Data, long p_DataLen, LPCSTR szExtraData=NULL)
	{
		def_DRDebuger_GetProc(m_fpResponseStringsData		, FpDRLog_ResponseStringsData		, "DRLog_ResponseStringsData");
		m_fpResponseStringsData(p_szIOKey, p_Data, p_DataLen, szExtraData);
		return 1;
	}

	long DRLog_FileTranWithData1(LONG hSender, BYTE cIO, LPCSTR pTranName, CStringList *pDataList)
	{
		def_DRDebuger_GetProc(m_fpFileTranWithData1		, FpDRLog_FileTranWithData1		, "DRLog_FileTranWithData1");
		m_fpFileTranWithData1(hSender, cIO, pTranName, pDataList);
		return 1;
	}

	long DRLog_FileTranWithData2(LONG hSender, BYTE cIO, LPCSTR pTranName, LPVOID pFileData, DWORD dwCount)
	{
		def_DRDebuger_GetProc(m_fpFileTranWithData2		, FpDRLog_FileTranWithData2		, "DRLog_FileTranWithData2");
		m_fpFileTranWithData2(hSender, cIO, pTranName, pFileData, dwCount);
		return 1;
	}

	long DRLog_FileTranWithDirect(LONG hSender, BYTE cIO, LPCSTR pTranName, LPCSTR pszFileName)
	{
		def_DRDebuger_GetProc(m_fpFileTranWithDirect		, FpDRLog_FileTranWithDirect		, "DRLog_FileTranWithDirect");
		m_fpFileTranWithDirect(hSender, cIO, pTranName, pszFileName);
		return 1;
	}
};


#endif //__LOAD_FOR_DRDEBUGER__BY_JUNOK_LEE__HEADER__
