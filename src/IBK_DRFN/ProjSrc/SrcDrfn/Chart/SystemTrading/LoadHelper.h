#if !defined(AFX_LOADSYSTEMTRADINGPARSERHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
#define AFX_LOADSYSTEMTRADINGPARSERHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_

#include "../INCLUDE_ST/BaseLoadHelper.h"
#include "../Include_ST/StructDefine.h"
#include "../Include_ST/DeclareDefine.h"

/////////////////////////////////////////////////////////////////////////////
//	CLoadSystemTradingParserHelper
/////////////////////////////////////////////////////////////////////////////
typedef void	(* FpInitSystemTradingParser)(LPSTR, VT_SCRIPTINFO*, VT_INNERFUNCINFO*);
typedef	BOOL	(* FpParseFileToCPP)(LPSTR, CErrorInfomation*, BOOL&, CScriptSecurity*);
typedef	int		(* FpParseMainToCPP)(LPSTR, CErrorInfomation*, BOOL&, CScriptSecurity*);
typedef	int		(* FpParseUserFunctionToCPP)(LPSTR, LPSTR, CErrorInfomation*, CScriptSecurity*);
typedef	BOOL	(* FpParseFileToVBS)(LPSTR);
typedef void	(* FpExitParser)(void);
typedef void	(* FpCheckScriptStatement)(LPSTR, LPSTR);
typedef void	(* FpGetUsedUserFunction)(VT_DECLDATA*, LPSTR);
typedef void	(* FpGetUsedUserAllFunction)(VT_DECLDATA*, LPSTR);
typedef void	(* FpDeleteCurBuffer)(void);
class CLoadSystemTradingParserHelper : public CLoadLib
{
public:
	CLoadSystemTradingParserHelper(char* szModuleName = NULL) 
	{
		if(szModuleName)
			::strcpy(m_szDllName, szModuleName);
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\SystemTradingParser.dll", szPath);
			else
				::strcpy(m_szDllName, "SystemTradingParser.dll");
		}

		pInitSystemTradingParser = NULL;
		pParseFileToCPP = NULL;
		pParseMainToCPP = NULL;
		pParseUserFunctionToCPP = NULL;
		pParseFileToVBS = NULL;
		pExitParser = NULL;
		pCheckScriptStatement = NULL;
		pGetUsedUserFunction = NULL;
		pGetUsedUserAllFunction = NULL;
		pDeleteCurBuffer = NULL;
		
		LoadLib();
	}

	~CLoadSystemTradingParserHelper() 
	{
		pExitParser();
//		pDeleteCurBuffer();
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoadSystemTradingParserHelper의 GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pInitSystemTradingParser		= (FpInitSystemTradingParser)::GetProcAddress(m_hLib, "EX_InitSystemTradingParser");
		pParseFileToCPP		= (FpParseFileToCPP)::GetProcAddress(m_hLib, "EX_ParseFileToCPP");
		pParseMainToCPP		= (FpParseMainToCPP)::GetProcAddress(m_hLib, "EX_ParseMainToCPP");
		pParseUserFunctionToCPP		= (FpParseUserFunctionToCPP)::GetProcAddress(m_hLib, "EX_ParseUserFunctionToCPP");
		pParseFileToVBS		= (FpParseFileToVBS)::GetProcAddress(m_hLib, "EX_ParseFileToVBS");
		pExitParser			= (FpExitParser)::GetProcAddress(m_hLib, "EX_ExitParser");
		pCheckScriptStatement	= (FpCheckScriptStatement)::GetProcAddress(m_hLib, "EX_CheckScriptStatement");
		pGetUsedUserFunction	= (FpGetUsedUserFunction)::GetProcAddress(m_hLib, "EX_GetUsedUserFunction");
		pGetUsedUserAllFunction	= (FpGetUsedUserAllFunction)::GetProcAddress(m_hLib, "EX_GetUsedUserAllFunction");
		pDeleteCurBuffer	= (FpDeleteCurBuffer)::GetProcAddress(m_hLib, "EX_DeleteCurBuffer");

		if(!pParseFileToCPP ||
			!pInitSystemTradingParser ||
			!pParseMainToCPP ||
			!pParseUserFunctionToCPP ||
			!pParseFileToVBS ||
			!pExitParser || 
			!pCheckScriptStatement ||
			!pDeleteCurBuffer ||
			!pGetUsedUserFunction ||
			!pGetUsedUserAllFunction) 
		{
			TRACE0("CLoadSystemTradingParserHelper의 Load에러 발생");

			return FALSE;
		}
		
		return TRUE;
	}

public:
	// Function
	FpInitSystemTradingParser	pInitSystemTradingParser;
	FpParseFileToCPP			pParseFileToCPP;
	FpParseMainToCPP			pParseMainToCPP;
	FpParseUserFunctionToCPP	pParseUserFunctionToCPP;
	FpParseFileToVBS			pParseFileToVBS;
	FpExitParser				pExitParser;
	FpCheckScriptStatement		pCheckScriptStatement;
	FpGetUsedUserFunction		pGetUsedUserFunction;
	FpGetUsedUserAllFunction	pGetUsedUserAllFunction;
	FpDeleteCurBuffer			pDeleteCurBuffer;

	// Implementation
public:
	void InitSystemTradingParser(LPSTR lpszRootPath, VT_SCRIPTINFO* pVtUserFunctionInfo, VT_INNERFUNCINFO* pVtInnerFunctionInfo)
	{
		if(!m_hLib)
			return;

		pInitSystemTradingParser(lpszRootPath, pVtUserFunctionInfo, pVtInnerFunctionInfo);
	}

	int ParseFileToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo)
	{
		if(!m_hLib)
			return FALSE;

		return pParseFileToCPP(p_szFile, pErrorInfo, bIndexFunction, pInfo);
	}

	int ParseMainToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo)
	{
		if(!m_hLib)
			return -1;

		return pParseMainToCPP(p_szFile, pErrorInfo, bIndexFunction, pInfo);
	}

	int ParseUserFunctionToCPP(LPSTR p_szFile, LPSTR p_lpszFunctionName, CErrorInfomation* pErrorInfo, CScriptSecurity* pInfo)
	{
		if(!m_hLib)
			return -1;

		return pParseUserFunctionToCPP(p_szFile, p_lpszFunctionName, pErrorInfo, pInfo);
	}

	BOOL ParseFileToVBS(LPSTR p_szFile)
	{
		if(!m_hLib)
			return FALSE;

		BOOL bReturn = pParseFileToVBS(p_szFile);

		return bReturn;
	}

	void CheckScriptStatement(LPSTR p_szFile, LPSTR lpszPath)
	{
		if(!m_hLib)
			return;

		pCheckScriptStatement(p_szFile, lpszPath);
	}

	void GetUsedUserFunction(VT_DECLDATA* pVtUsedUserFunction, LPSTR lpszPath)
	{
		if(!m_hLib)
			return;

		pGetUsedUserFunction(pVtUsedUserFunction, lpszPath);
	}

	void GetUsedUserAllFunction(VT_DECLDATA* pVtUsedUserFunction, LPSTR lpszPath)
	{
		if(!m_hLib)
			return;

		pGetUsedUserAllFunction(pVtUsedUserFunction, lpszPath);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoadSystemTradingWorkspaceHelper
/////////////////////////////////////////////////////////////////////////////
typedef	BOOL	(* FpCreateWorkspace)(CFrameWnd*, LPSTR, VT_SCRIPTINFO*, VT_SCRIPTINFO*);
typedef CWnd*	(* FpGetStrategyWnd)(void);
typedef CWnd*	(* FpGetFunctionWnd)(void);
typedef CWnd*	(* FpGetFunctionInStrategyWnd)(void);
typedef CWnd*	(* FpGetOutputWnd)(void);
typedef	void	(* FpDeleteWorkspace)(void);
typedef void	(* FpSetOutputText)(int, LPCSTR, CErrorInfomation*);
typedef void	(* FpSetFindText)(LPCSTR, UINT);
typedef void	(* FpSetServerMessage)(LPCSTR, UINT);
typedef void	(* FpAddListStrategy)(CScriptInfomation*);
typedef void	(* FpAddListFunction)(CScriptInfomation*);
typedef void	(* FpAddListFunctionInStrategy)(LPCSTR, VT_DECLDATA*);
typedef void	(* FpAddListFunctionInFunction)(LPCSTR, VT_DECLDATA*);
typedef void	(* FpChangeCompiledOption)(UINT, LPCSTR, UINT);
typedef void	(* FpDeleteFunctionInStrategy)(LPCSTR);
typedef void	(* FpEnsureVisibleFunctionInStrategy)(LPCSTR);
typedef void	(* FpClearAllOutputText)(void);
typedef void	(* FpClearAllFindText)(void);
class CLoadSystemTradingWorkspaceHelper : public CLoadLib
{
public:
	CLoadSystemTradingWorkspaceHelper(char* szModuleName = NULL) 
	{
		if(szModuleName)
			::strcpy(m_szDllName, szModuleName);
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\SystemTradingWorkBar.dll", szPath);
			else
				::strcpy(m_szDllName, "SystemTradingWorkBar.dll");
		}

		pCreateWorkspace = NULL;
		pGetStrategyWnd = NULL;
		pGetFunctionWnd = NULL;
		pGetOutputWnd = NULL;
		pDeleteWorkspace = NULL;
		pSetOutputText = NULL;
		pSetFindText = NULL;
		pAddListStrategy = NULL;
		pAddListFunction = NULL;
		pChangeCompiledOption = NULL;
		pGetFunctionInStrategyWnd = NULL;
		pAddListFunctionInStrategy = NULL;
		pAddListFunctionInFunction = NULL;
		pDeleteFunctionInStrategy = NULL;
		pEnsureVisibleFunctionInStrategy = NULL;
		pClearAllOutputText = NULL;
		pClearAllFindText = NULL;
		
		LoadLib();
	}

	~CLoadSystemTradingWorkspaceHelper() 
	{
		pDeleteWorkspace();
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoadSystemTradingWorkspaceHelper GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pCreateWorkspace		= (FpCreateWorkspace)::GetProcAddress(m_hLib, "EX_CreateWorkspace");
		pGetStrategyWnd			= (FpGetStrategyWnd)::GetProcAddress(m_hLib, "EX_GetStategyWnd");
		pGetFunctionWnd			= (FpGetFunctionWnd)::GetProcAddress(m_hLib, "EX_GetFunctionWnd");
		pGetOutputWnd			= (FpGetOutputWnd)::GetProcAddress(m_hLib, "EX_GetOutputWnd");
		pDeleteWorkspace		= (FpDeleteWorkspace)::GetProcAddress(m_hLib, "EX_DeleteWorkspace");
		pSetOutputText			= (FpSetOutputText)::GetProcAddress(m_hLib, "EX_SetOutputText");
		pSetFindText			= (FpSetFindText)::GetProcAddress(m_hLib, "EX_SetFindText");
		pSetServerMessage		= (FpSetServerMessage)::GetProcAddress(m_hLib, "EX_SetServerMessage");
		pAddListStrategy		= (FpAddListStrategy)::GetProcAddress(m_hLib, "EX_AddListStrategy");
		pAddListFunction		= (FpAddListFunction)::GetProcAddress(m_hLib, "EX_AddListFunction");
		pChangeCompiledOption	= (FpChangeCompiledOption)::GetProcAddress(m_hLib, "EX_ChangeCompiledOption");
		pGetFunctionInStrategyWnd	= (FpGetFunctionInStrategyWnd)::GetProcAddress(m_hLib, "EX_GetFunctionInStrategyWnd");
		pAddListFunctionInStrategy	= (FpAddListFunctionInStrategy)::GetProcAddress(m_hLib, "EX_AddListFunctionInStrategy");
		pAddListFunctionInFunction	= (FpAddListFunctionInFunction)::GetProcAddress(m_hLib, "EX_AddListFunctionInFunction");
		pDeleteFunctionInStrategy	= (FpDeleteFunctionInStrategy)::GetProcAddress(m_hLib, "EX_DeleteListFunctionInStrategy");
		pEnsureVisibleFunctionInStrategy	= (FpEnsureVisibleFunctionInStrategy)::GetProcAddress(m_hLib, "EX_EnsureVisibleFunctionInStrategy");
		pClearAllOutputText		= (FpClearAllOutputText)::GetProcAddress(m_hLib, "EX_ClearAllOutputText");
		pClearAllFindText		= (FpClearAllFindText)::GetProcAddress(m_hLib, "EX_ClearAllFindText");

		if(!pCreateWorkspace ||
			!pGetStrategyWnd ||
			!pGetFunctionWnd ||
			!pGetOutputWnd ||
			!pDeleteWorkspace ||
			!pSetOutputText || 
			!pSetFindText || 
			!pSetServerMessage || 
			!pAddListStrategy || 
			!pAddListFunction || 
			!pGetFunctionInStrategyWnd ||
			!pAddListFunctionInStrategy ||
			!pAddListFunctionInFunction ||
			!pDeleteFunctionInStrategy ||
			!pEnsureVisibleFunctionInStrategy ||
			!pClearAllOutputText ||
			!pClearAllFindText ||
			!pChangeCompiledOption) 
		{
			TRACE0("CLoadSystemTradingWorkspaceHelper Load에러 발생");

			return FALSE;
		}
		
		return TRUE;
	}

public:
	// Function
	FpCreateWorkspace			pCreateWorkspace;
	FpGetStrategyWnd			pGetStrategyWnd;
	FpGetFunctionWnd			pGetFunctionWnd;
	FpGetOutputWnd				pGetOutputWnd;
	FpDeleteWorkspace			pDeleteWorkspace;
	FpSetOutputText				pSetOutputText;
	FpSetFindText				pSetFindText;
	FpSetServerMessage			pSetServerMessage;
	FpAddListStrategy			pAddListStrategy;
	FpAddListFunction			pAddListFunction;
	FpChangeCompiledOption		pChangeCompiledOption;
	FpGetFunctionInStrategyWnd		pGetFunctionInStrategyWnd;
	FpAddListFunctionInStrategy		pAddListFunctionInStrategy;
	FpAddListFunctionInFunction		pAddListFunctionInFunction;
	FpDeleteFunctionInStrategy		pDeleteFunctionInStrategy;
	FpEnsureVisibleFunctionInStrategy		pEnsureVisibleFunctionInStrategy;
	FpClearAllOutputText		pClearAllOutputText;
	FpClearAllFindText		pClearAllFindText;

	// Implementation
public:
	BOOL CreateWorkspace(CFrameWnd* p_pMainWnd, LPSTR p_lpszRootPath, VT_SCRIPTINFO* p_pVtStartegyInfo, VT_SCRIPTINFO* p_pVtUserFuncInfo)
	{
		if(!m_hLib)
			return FALSE;

		return pCreateWorkspace(p_pMainWnd, p_lpszRootPath, p_pVtStartegyInfo, p_pVtUserFuncInfo);
	}

	CWnd* GetStrategyWnd()
	{
		if(!m_hLib)
			return FALSE;

		return pGetStrategyWnd();
	}

	CWnd* GetFunctionWnd()
	{
		if(!m_hLib)
			return FALSE;

		return pGetFunctionWnd();
	}

	CWnd* GetFunctionInStrategyWnd()
	{
		if(!m_hLib)
			return FALSE;

		return pGetFunctionInStrategyWnd();
	}

	CWnd* GetOutputWnd()
	{
		if(!m_hLib)
			return FALSE;

		return pGetOutputWnd();
	}

	void SetOutputText(int nMessage, LPCSTR lpszText, CErrorInfomation* pErrorInfo)
	{
		if(!m_hLib)
			return;

		pSetOutputText(nMessage, lpszText, pErrorInfo);
	}

	void SetFindText(LPCSTR lpszText, UINT nFileType)
	{
		if(!m_hLib)
			return;

		pSetFindText(lpszText, nFileType);
	}

	void SetServerMessage(LPCSTR lpszMessage, UINT nLevel)
	{
		if(!m_hLib)
			return;

		pSetServerMessage(lpszMessage, nLevel);
	}

	void AddListStrategy(CScriptInfomation* pStrategy)
	{
		if(!m_hLib)
			return;

		pAddListStrategy(pStrategy);
	}

	void AddListFunction(CScriptInfomation* pFunction)
	{
		if(!m_hLib)
			return;

		pAddListFunction(pFunction);
	}
	
	void AddListFunctionInStrategy(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction)
	{
		if(!m_hLib)
			return;

		pAddListFunctionInStrategy(lpszName, pVtUsedUserFunction);
	}

	void AddListFunctionInFunction(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction)
	{
		if(!m_hLib)
			return;

		pAddListFunctionInFunction(lpszName, pVtUsedUserFunction);
	}

	void ChangeCompiledOption(UINT nFileType, LPCSTR szName, UINT nCompiled)
	{
		if(!m_hLib)
			return;

		pChangeCompiledOption(nFileType, szName, nCompiled);
	}

	void DeleteFunctionInStrategy(LPCSTR lpszName)
	{
		if(!m_hLib)
			return;

		pDeleteFunctionInStrategy(lpszName);
	}

	void EnsureVisibleFunctionInStrategy(LPCSTR lpszName)
	{
		if(!m_hLib)
			return;

		pEnsureVisibleFunctionInStrategy(lpszName);
	}

	void ClearAllOutputText()
	{
		if(!m_hLib)
			return;

		pClearAllOutputText();
	}

	void ClearAllFindText()
	{
		if(!m_hLib)
			return;

		pClearAllFindText();
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoadSystemTradingExtLibHelper
/////////////////////////////////////////////////////////////////////////////
typedef BOOL		(* FpInitExtLib)(FpNotifyList, LPSTR);
typedef void		(* FpExitExtLib)(FpNotifyList);
typedef CPtrList*	(* FpGetScriptList)(void);
typedef BOOL		(* FpGetScriptInfo)(LPSTR, CString&);
typedef void		(* FpReloadScriptInfo)(LPSTR);
class CLoadSystemTradingExtLibHelper : public CLoadLib
{
public:
	CLoadSystemTradingExtLibHelper(char* szModuleName = NULL) 
	{
		if(szModuleName)
			::strcpy(m_szDllName, szModuleName);
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\SystemTradingExtLib.dll", szPath);
			else
				::strcpy(m_szDllName, "SystemTradingExtLib.dll");
		}

		pInitExtLib = NULL;
		pExitExtLib = NULL;
		pGetScriptList = NULL;
		pGetScriptInfo = NULL;
		pReloadScriptInfo = NULL;
		
		LoadLib();
	}

	~CLoadSystemTradingExtLibHelper() 
	{
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoadSystemTradingExtLibHelper의 GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pInitExtLib			= (FpInitExtLib)::GetProcAddress(m_hLib, "EX_InitExtLib");
		pExitExtLib			= (FpExitExtLib)::GetProcAddress(m_hLib, "EX_ExitExtLib");
		pGetScriptList		= (FpGetScriptList)::GetProcAddress(m_hLib, "EX_GetScriptList");
		pGetScriptInfo		= (FpGetScriptInfo)::GetProcAddress(m_hLib, "EX_GetScriptInfo");
		pReloadScriptInfo	= (FpReloadScriptInfo)::GetProcAddress(m_hLib, "EX_ReloadScriptInfo");

		if(!pInitExtLib ||
			!pExitExtLib ||
			!pGetScriptList ||
			!pGetScriptInfo ||
			!pReloadScriptInfo)
		{
			TRACE0("CLoadSystemTradingExtLibHelper의 Load에러 발생");

			return FALSE;
		}
		
		return TRUE;
	}

public:
	// Function
	FpInitExtLib		pInitExtLib;
	FpExitExtLib		pExitExtLib;
	FpGetScriptList		pGetScriptList;
	FpGetScriptInfo		pGetScriptInfo;
	FpReloadScriptInfo	pReloadScriptInfo;

	// Implementation
public:
	BOOL InitExtLib(FpNotifyList pFunction, LPSTR lpszRootPath)
	{
		if(!m_hLib)
			return FALSE;

		return pInitExtLib(pFunction, lpszRootPath);
	}

	void ExitExtLib(FpNotifyList pFunction)
	{
		if(!m_hLib)
			return;

		pExitExtLib(pFunction);
	}

	CPtrList* GetScriptList()
	{
		if(!m_hLib)
			return NULL;

		return pGetScriptList();
	}
	
	BOOL GetScriptInfo(LPSTR lpszScripName, CString& strInfo)
	{
		if(!m_hLib)
			return FALSE;

		return pGetScriptInfo(lpszScripName, strInfo);
	}

	void ReloadScriptInfo(LPSTR lpszRootPath)
	{
		if(!m_hLib)
			return;

		pReloadScriptInfo(lpszRootPath);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoadSystemTradingZipHelper
/////////////////////////////////////////////////////////////////////////////
typedef BOOL		(* FpCompressFile)(LPSTR, LPSTR);
typedef BOOL		(* FpCompressFiles)(LPSTR, CStringArray&);
typedef BOOL		(* FpExtractFile)(LPCSTR, LPSTR);
typedef void		(* FpExtractFileNotify)(LPCSTR, LPSTR, FpNotifyExtract);
class CLoadSystemTradingZipHelper : public CLoadLib
{
public:
	CLoadSystemTradingZipHelper(char* szModuleName = NULL) 
	{
		if(szModuleName)
			::strcpy(m_szDllName, szModuleName);
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\SystemTradingZipper.dll", szPath);
			else
				::strcpy(m_szDllName, "SystemTradingZipper.dll");
		}

		pCompressFile = NULL;
		pCompressFiles = NULL;
		pExtractFile = NULL;
		pExtractFileNotify = NULL;
		
		LoadLib();
	}

	~CLoadSystemTradingZipHelper() 
	{
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoadSystemTradingZipHelper GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pCompressFile		= (FpCompressFile)::GetProcAddress(m_hLib, "EX_CompressFile");
		pCompressFiles		= (FpCompressFiles)::GetProcAddress(m_hLib, "EX_CompressFiles");
		pExtractFile		= (FpExtractFile)::GetProcAddress(m_hLib, "EX_ExtractFile");
		pExtractFileNotify	= (FpExtractFileNotify)::GetProcAddress(m_hLib, "EX_ExtractFileNotify");

		if(!pCompressFile ||
			!pCompressFiles ||
			!pExtractFile ||
			!pExtractFileNotify)
		{
			TRACE0("CLoadSystemTradingZipHelper Load에러 발생");

			return FALSE;
		}
		
		return TRUE;
	}

public:
	// Function
	FpCompressFile		pCompressFile;
	FpCompressFiles		pCompressFiles;
	FpExtractFile		pExtractFile;
	FpExtractFileNotify	pExtractFileNotify;

	// Implementation
public:
	BOOL CompressFile(LPSTR lpszZipFile, LPSTR lpszFile)
	{
		if(!m_hLib)
			return FALSE;

		return pCompressFile(lpszZipFile, lpszFile);
	}

	BOOL CompressFiles(LPSTR lpszZipFile, CStringArray& arFiles)
	{
		if(!m_hLib)
			return FALSE;

		return pCompressFiles(lpszZipFile, arFiles);
	}

	BOOL ExtractFile(LPCSTR lpszZipFile, LPSTR lpszFolder)
	{
		if(!m_hLib)
			return FALSE;

		return pExtractFile(lpszZipFile, lpszFolder);
	}

	void ExtractFileNotify(LPCSTR lpszZipFile, LPSTR lpszFolder, FpNotifyExtract pNotify)
	{
		if(!m_hLib)
			return;

		pExtractFileNotify(lpszZipFile, lpszFolder, pNotify);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoadSystemTradingEncoder
/////////////////////////////////////////////////////////////////////////////
typedef BOOL 	(* FpEncryptFile)(LPCSTR, LPCSTR, LPSTR, LPSTR, LPSTR);
typedef BOOL 	(* FpDecryptFile)(LPCSTR, LPCSTR, LPSTR, LPSTR, LPSTR);
typedef BOOL 	(* FpEncryptWord)(LPSTR, LPSTR);
typedef BOOL 	(* FpDecryptWord)(LPSTR, LPSTR);
class CLoadSystemTradingEncoder : public CLoadLib
{
public:
	CLoadSystemTradingEncoder(char* szModuleName = NULL) 
	{
		if(szModuleName)
			::strcpy(m_szDllName, szModuleName);
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\SystemTradingEncoder.dll", szPath);
			else
				::strcpy(m_szDllName, "SystemTradingEncoder.dll");
		}
		
		pEncryptFile	= NULL;
		pDecryptFile	= NULL;
		pEncryptWord	= NULL;
		pDecryptWord	= NULL;

		LoadLib();
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
			return FALSE;

		pEncryptFile	= (FpEncryptFile)::GetProcAddress(m_hLib, "EX_EncryptFile");
		pDecryptFile	= (FpDecryptFile)::GetProcAddress(m_hLib, "EX_DecryptFile");
		pEncryptWord	= (FpEncryptWord)::GetProcAddress(m_hLib, "EX_EncryptWord");
		pDecryptWord	= (FpDecryptWord)::GetProcAddress(m_hLib, "EX_DecryptWord");

		return TRUE;
	}

public:
	// Function
	FpEncryptFile		pEncryptFile;
	FpDecryptFile		pDecryptFile;
	FpEncryptWord		pEncryptWord;
	FpDecryptWord		pDecryptWord;

// Implementation
public:
	BOOL EncryptFile(LPCSTR szSource, LPCSTR szDestination, LPSTR lpszLine1 = NULL, LPSTR lpszLine2 = NULL, LPSTR lpszLine3 = NULL)
	{
		if(!m_hLib || !pEncryptFile)
			return FALSE;
		
		return pEncryptFile(szSource, szDestination, lpszLine1, lpszLine2, lpszLine3);
	}

	BOOL DecryptFile(LPCSTR szSource, LPCSTR szDestination, LPSTR lpszLine1 = NULL, LPSTR lpszLine2 = NULL, LPSTR lpszLine3 = NULL)
	{
		if(!m_hLib || !pDecryptFile)
			return FALSE;
		
		return pDecryptFile(szSource, szDestination, lpszLine1, lpszLine2, lpszLine3);
	}

	BOOL EncryptWord(LPSTR szSource, LPSTR szDestination)
	{
		if(!m_hLib || !pEncryptWord)
			return FALSE;
		
		return pEncryptWord(szSource, szDestination);
	}

	BOOL DecryptWord(LPSTR szSource, LPSTR szDestination)
	{
		if(!m_hLib || !pDecryptWord)
			return FALSE;
		
		return pDecryptWord(szSource, szDestination);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoadSystemTradingConfigDll
/////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI* FpShowConfigureDlg) (HWND, LPVOID, LPCTSTR, int);
typedef CString (WINAPI* FpGetPassword)(void);
typedef CString (WINAPI* FpGetUserID)(void);
typedef CString (WINAPI* FpGetExpiredDate)(void);
typedef void (WINAPI* FpSetPassword)(LPCTSTR);
typedef void (WINAPI* FpSetUserID)(LPCTSTR);
typedef void (WINAPI* FpSetExpiredDate)(LPCTSTR);
typedef BOOL (WINAPI* FpGetUsePassword)(void);
typedef BOOL (WINAPI* FpGetUseUserID)(void);
typedef BOOL (WINAPI* FpGetUseExpiredDate)(void);
typedef void (WINAPI* FpSetUsePassword)(BOOL);
typedef void (WINAPI* FpSetUseUserID)(BOOL);
typedef void (WINAPI* FpSetUseExpiredDate)(BOOL);
class CLoadSystemTradingConfigDll : public CLoadLib
{
public:
	CLoadSystemTradingConfigDll(char* szModuleName = NULL) 
	{
		if(szModuleName)
			::strcpy(m_szDllName, szModuleName);
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\STConfigureDlg.dll", szPath);
			else
				::strcpy(m_szDllName, "STConfigureDlg.dll");
		}
		
		pShowConfigureDlg = NULL;
		pGetPassword	= NULL;
		pGetUserID		= NULL;
		pGetExpiredDate	= NULL;
		pSetPassword	= NULL;
		pSetUserID		= NULL;
		pSetExpiredDate	= NULL;
		pGetUsePassword	= NULL;
		pGetUseUserID	= NULL;
		pGetUseExpiredDate	= NULL;
		pSetUsePassword	= NULL;
		pSetUseUserID	= NULL;
		pSetUseExpiredDate	= NULL;

		LoadLib();
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
			return FALSE;

		pShowConfigureDlg	= (FpShowConfigureDlg)::GetProcAddress(m_hLib, "fnShowConfigureDlg");
		pGetPassword	= (FpGetPassword)::GetProcAddress(m_hLib, "fnGetPassword");
		pGetUserID		= (FpGetUserID)::GetProcAddress(m_hLib, "fnGetUserID");
		pGetExpiredDate	= (FpGetExpiredDate)::GetProcAddress(m_hLib, "fnGetExpiredDate");
		pSetPassword	= (FpSetPassword)::GetProcAddress(m_hLib, "fnSetPassword");
		pSetUserID		= (FpSetUserID)::GetProcAddress(m_hLib, "fnSetUserID");
		pSetExpiredDate	= (FpSetExpiredDate)::GetProcAddress(m_hLib, "fnSetExpiredDate");
		pGetUsePassword	= (FpGetUsePassword)::GetProcAddress(m_hLib, "fnGetUsePassword");
		pGetUseUserID	= (FpGetUseUserID)::GetProcAddress(m_hLib, "fnGetUseUserID");
		pGetUseExpiredDate	= (FpGetUseExpiredDate)::GetProcAddress(m_hLib, "fnGetUseExpiredDate");
		pSetUsePassword	= (FpSetUsePassword)::GetProcAddress(m_hLib, "fnSetUsePassword");
		pSetUseUserID	= (FpSetUseUserID)::GetProcAddress(m_hLib, "fnSetUseUserID");
		pSetUseExpiredDate	= (FpSetUseExpiredDate)::GetProcAddress(m_hLib, "fnSetUseExpiredDate");

		return TRUE;
	}

public:
	// Function
	FpShowConfigureDlg		pShowConfigureDlg;
	FpGetPassword		pGetPassword;
	FpGetUserID			pGetUserID;
	FpGetExpiredDate	pGetExpiredDate;
	FpSetPassword		pSetPassword;
	FpSetUserID			pSetUserID;
	FpSetExpiredDate	pSetExpiredDate;
	FpGetUsePassword	pGetUsePassword;
	FpGetUseUserID		pGetUseUserID;
	FpGetUseExpiredDate	pGetUseExpiredDate;
	FpSetUsePassword	pSetUsePassword;
	FpSetUseUserID		pSetUseUserID;
	FpSetUseExpiredDate	pSetUseExpiredDate;

// Implementation
public:
	BOOL ShowConfigureDlg(HWND hWnd, LPVOID lpData, LPCTSTR lpszFile, int nType)
	{
		if(!m_hLib || !pShowConfigureDlg)
			return FALSE;

		return pShowConfigureDlg(hWnd, lpData, lpszFile, nType);
	}
	
	CString GetPassword()
	{
		if(!m_hLib || !pGetPassword)
			return _T("");
		
		return pGetPassword();
	}

	CString GetUserID()
	{
		if(!m_hLib || !pGetUserID)
			return _T("");
		
		return pGetUserID();
	}

	CString GetExpiredDate()
	{
		if(!m_hLib || !pGetExpiredDate)
			return _T("");
		
		return pGetExpiredDate();
	}

	void SetPassword(LPCTSTR lpszText)
	{
		if(!m_hLib || !pSetPassword)
			return;
		
		pSetPassword(lpszText);
	}

	void SetUserID(LPCTSTR lpszText)
	{
		if(!m_hLib || !pSetUserID)
			return;
		
		pSetUserID(lpszText);
	}

	void SetExpiredDate(LPCTSTR lpszText)
	{
		if(!m_hLib || !pSetExpiredDate)
			return;
		
		pSetExpiredDate(lpszText);
	}

	BOOL GetUsePassword()
	{
		if(!m_hLib || !pGetUsePassword)
			return FALSE;
		
		return pGetUsePassword();
	}

	BOOL GetUseUserID()
	{
		if(!m_hLib || !pGetUseUserID)
			return FALSE;
		
		return pGetUseUserID();
	}

	BOOL GetUseExpiredDate()
	{
		if(!m_hLib || !pGetUseExpiredDate)
			return FALSE;
		
		return pGetUseExpiredDate();
	}

	void SetUsePassword(BOOL bSet)
	{
		if(!m_hLib || !pSetUsePassword)
			return;
		
		pSetUsePassword(bSet);
	}

	void SetUseUserID(BOOL bSet)
	{
		if(!m_hLib || !pSetUseUserID)
			return;
		
		pSetUseUserID(bSet);
	}

	void SetUseExpiredDate(BOOL bSet)
	{
		if(!m_hLib || !pSetUseExpiredDate)
			return;
		
		pSetUseExpiredDate(bSet);
	}
};

#endif // !defined(AFX_LOADSYSTEMTRADINGPARSERHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)