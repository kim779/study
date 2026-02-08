// DllSTLoader.h: interface for the CDllSTLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLSTLOADER_H__8233E5AB_CC33_471E_9FE5_2783ECA88040__INCLUDED_)
#define AFX_DLLSTLOADER_H__8233E5AB_CC33_471E_9FE5_2783ECA88040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 20060705 클래스선언 제거
//class CDBMgr;

typedef  CString (WINAPI* _DLL_GetInputs)(long lIndex);

//typedef  long (WINAPI* _DLL_LoadST)(HWND hParent, HWND hSocket,  CDBMgr* pDrMLib_CDBMgr, long lKey, LPCTSTR lpSTName, int nSTPos, LPCTSTR lpAdditionalOption);
typedef  long (WINAPI* _DLL_LoadST)(HWND hParent, HWND hSocket,  long pDrMLib_CDBMgr, long lKey, LPCTSTR lpSTName, int nSTPos, LPCTSTR lpAdditionalOption);
typedef  long (WINAPI* _DLL_ExecuteST)(long lActor, LPCTSTR lpInputData);
typedef  long (WINAPI* _DLL_UnloadST)(long lActor);
typedef  long (WINAPI* _DLL_StopST)(long lActor);
typedef  long (WINAPI* _DLL_PauseST)(long lActor);
typedef  long (WINAPI* _DLL_UpdateST)(long lActor,long lPos, BOOL bIsUpdateData);
typedef  long (WINAPI* _DLL_CommandST)(long lSTActor, LPCTSTR lpCommand, LPCTSTR lpOption);

class CDllSTLoader  
{
public:
	CDllSTLoader();
	virtual ~CDllSTLoader();
	// 20060705 함수인자 제거 : , HWND hSocket,  CDBMgr* pDrMLib_CDBMgr
	//long Load(LPCTSTR lpSDDllNamePath, HWND hParent,HWND hSocket, long pDrMLib_CDBMgr, long lKey, LPCTSTR lpSTName,int nSTPos,LPCTSTR lpAdditionalOption);
	long Load(LPCTSTR lpSDDllNamePath, HWND hParent, long lKey, LPCTSTR lpSTName,int nSTPos,LPCTSTR lpAdditionalOption,long pChartItem=NULL);

	long Execute(LPCTSTR lpInputData);
	long Stop();
	long Pause();
	long Update(long lPos, BOOL bIsUpdateData);
	long Unload();	
	long Command(LPCTSTR lpCommand, LPCTSTR lpOption);
	CString GetInput(long lIndex);
	CString GetErrorMessage() { return m_strError;}
private:
	BOOL LoadDLL(LPCTSTR lpSTFileName);
	BOOL UnloadDLL();
	_DLL_GetInputs m_fnGetInputs;
	_DLL_LoadST m_fnLoadST;
	_DLL_UnloadST m_fnUnloadST;
	_DLL_ExecuteST m_fnExecuteST;
	_DLL_StopST m_fnStopST;
	_DLL_PauseST m_fnPauseST;
	_DLL_UpdateST m_fnUpdateST;
	_DLL_CommandST m_fnCommandST;
	HINSTANCE m_hSTDll;
	CString m_strSTDll;
	long m_lSTActor;
	CString m_strError;
};

#endif // !defined(AFX_DLLSTLOADER_H__8233E5AB_CC33_471E_9FE5_2783ECA88040__INCLUDED_)
