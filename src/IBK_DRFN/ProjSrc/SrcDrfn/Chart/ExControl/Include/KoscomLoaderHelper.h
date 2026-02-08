#if !defined(AFX_LOADHELPERKOSCOM_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
#define AFX_LOADHELPERKOSCOM_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_

#include "BaseLoadHelper.h"

/////////////////////////////////////////////////////////////////////////////
//	CLoaderKoscomCtrl
/////////////////////////////////////////////////////////////////////////////
//typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef UINT	(* FpGetMarketKind)(LPVOID);
typedef void	(* FpSetMarketKind)(LPVOID, UINT);
typedef void	(* FpGetClientRect)(LPVOID, LPRECT);
typedef OLE_HANDLE	(* FpGetHWnd)(LPVOID);
typedef HWND	(* FpGetSafeHwnd)(LPVOID);
typedef void	(* FpGetWindowRect)(LPVOID, LPRECT);
typedef LPVOID	(* FpCreateKoscomCtrl)(CWnd*, CWnd*, UINT, const CRect&, UINT, long);
typedef LPVOID	(* FpCreateCodeInput)(CWnd*, CWnd*, const CRect&, UINT, UINT);
typedef LPVOID	(* FpCreateButton)(CWnd*, CWnd*, const CRect&);
typedef void	(* FpExitKoscomCtrl)(LPVOID);
//typedef short	(* FpGetGubunFromCode)(LPVOID, LPCTSTR);
typedef void	(* FpSetDataText)(LPVOID, LPCTSTR);
typedef void	(* FpSetDataText2)(LPVOID, LPCTSTR);
typedef CString	(WINAPI* FpGetDataText)(LPVOID);
//typedef long	(* FpGetBase)(LPVOID, LPCTSTR);
typedef void	(* FpSetEnabled)(LPVOID, BOOL);
typedef BOOL	(* FpGetEnabled)(LPVOID);
typedef void	(* FpSetFocus)(LPVOID);
typedef void	(* FpSetCurSel)(LPVOID, short);
typedef void	(* FpMoveWindow)(LPVOID, int, int, int, int, BOOL);
typedef void	(* FpMoveWindow2)(LPVOID, LPCRECT, BOOL);
typedef BOOL	(* FpSetWindowPos)(LPVOID, const CWnd*, int, int, int, int, UINT);
typedef BOOL	(* FpShowWindow)(LPVOID, int);

class CLoaderKoscomCtrl : public CExLoadLib
{
public:
	CLoaderKoscomCtrl(LPCSTR lpszRootPath = NULL) 
	{
		if(lpszRootPath)
			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, _T("ExKoscom.dll"));
		else
		{
			char szPath[MAX_PATH] = {0, };
			GetCurrentWorkDirectory(szPath, sizeof(szPath));

			if(szPath)
				::sprintf(m_szDllName, "%s\\%s", szPath, _T("ExKoscom.dll"));
			else
				::strcpy(m_szDllName, _T("ExKoscom.dll"));
		}

		//		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

//		pActionControl = NULL;
		pExitKoscomCtrl = NULL;

		pCreateKoscomCtrl = NULL;
		pCreateCodeInput = NULL;
		pCreateButton = NULL;

//		pGetGubunFromCode = NULL;

		pSetDataText = NULL;
		pSetDataText2 = NULL;
		pGetDataText = NULL;

		pSetEnabled = NULL;
		pGetEnabled = NULL;

//		pGetBase = NULL;

		pSetFocus = NULL;

		pSetCurSel = NULL;

		pGetHWnd = NULL;
		pGetSafeHwnd = NULL;

		pMoveWindow = NULL;
		pMoveWindow2 = NULL;
		pSetWindowPos = NULL;
		pShowWindow = NULL;
		pGetWindowRect = NULL;
		pGetClientRect = NULL;
		pSetMarketKind = NULL;
		pGetMarketKind = NULL;

		LoadLib();
	}

	virtual ~CLoaderKoscomCtrl() 
	{
		if(m_hLib)
			pExitKoscomCtrl(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderKoscomCtrl GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

//		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pCreateKoscomCtrl	= (FpCreateKoscomCtrl)::GetProcAddress(m_hLib, "Ex_CreateKoscomCtrl");
		pCreateCodeInput	= (FpCreateCodeInput)::GetProcAddress(m_hLib, "Ex_CreateCodeInput");
		pCreateButton	= (FpCreateButton)::GetProcAddress(m_hLib, "Ex_CreateButton");
		pExitKoscomCtrl	= (FpExitKoscomCtrl)::GetProcAddress(m_hLib, "Ex_ExitKoscomCtrl");

//		pGetGubunFromCode	= (FpGetGubunFromCode)::GetProcAddress(m_hLib, "Ex_GetGubunFromCode");

		pSetDataText	= (FpSetDataText)::GetProcAddress(m_hLib, "Ex_SetDataText");
		pSetDataText2	= (FpSetDataText2)::GetProcAddress(m_hLib, "Ex_SetDataText2");
		pGetDataText	= (FpGetDataText)::GetProcAddress(m_hLib, "Ex_GetDataText");

		pSetEnabled	= (FpSetEnabled)::GetProcAddress(m_hLib, "Ex_SetEnabled");
		pGetEnabled	= (FpGetEnabled)::GetProcAddress(m_hLib, "Ex_GetEnabled");

//		pGetBase	= (FpGetBase)::GetProcAddress(m_hLib, "Ex_GetBase");

		pSetFocus	= (FpSetFocus)::GetProcAddress(m_hLib, "Ex_SetFocus");

		pSetCurSel	= (FpSetCurSel)::GetProcAddress(m_hLib, "Ex_SetCurSel");

		pGetHWnd	= (FpGetHWnd)::GetProcAddress(m_hLib, "Ex_GetHWnd");
		pGetSafeHwnd	= (FpGetSafeHwnd)::GetProcAddress(m_hLib, "Ex_GetSafeHwnd");

		pMoveWindow	= (FpMoveWindow)::GetProcAddress(m_hLib, "Ex_MoveWindow");
		pMoveWindow2	= (FpMoveWindow2)::GetProcAddress(m_hLib, "Ex_MoveWindow2");
		pSetWindowPos	= (FpSetWindowPos)::GetProcAddress(m_hLib, "Ex_SetWindowPos");
		pShowWindow	= (FpShowWindow)::GetProcAddress(m_hLib, "Ex_ShowWindow");
		pGetWindowRect	= (FpGetWindowRect)::GetProcAddress(m_hLib, "Ex_GetWindowRect");
		pGetClientRect	= (FpGetClientRect)::GetProcAddress(m_hLib, "Ex_GetClientRect");
		pSetMarketKind	= (FpSetMarketKind)::GetProcAddress(m_hLib, "Ex_SetMarketKind");
		pGetMarketKind	= (FpGetMarketKind)::GetProcAddress(m_hLib, "Ex_GetMarketKind");
		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

//	FpActionControl		pActionControl;

	FpCreateKoscomCtrl		pCreateKoscomCtrl;
	FpCreateCodeInput		pCreateCodeInput;
	FpCreateButton			pCreateButton;
	FpExitKoscomCtrl		pExitKoscomCtrl;

//	FpGetGubunFromCode		pGetGubunFromCode;

	FpSetDataText		pSetDataText;
	FpSetDataText2		pSetDataText2;
	FpGetDataText		pGetDataText;

	FpSetEnabled		pSetEnabled;
	FpGetEnabled		pGetEnabled;

//	FpGetBase		pGetBase;

	FpSetFocus		pSetFocus;
	FpSetCurSel		pSetCurSel;

	FpGetHWnd		pGetHWnd;
	FpGetSafeHwnd		pGetSafeHwnd;

	FpMoveWindow		pMoveWindow;
	FpMoveWindow2		pMoveWindow2;
	FpSetWindowPos		pSetWindowPos;
	FpShowWindow		pShowWindow;
	FpGetWindowRect		pGetWindowRect;
	FpGetClientRect		pGetClientRect;
	FpSetMarketKind		pSetMarketKind;
	FpGetMarketKind		pGetMarketKind;

public:
	// Function
	//long ActionControl(LPVOID lpAction)
	//{
	//	if(!m_hLib)
	//		return 0L;

	//	return pActionControl(m_lpPointer, lpAction);
	//}

	void ExitKoscomCtrl()
	{
		if(!m_hLib)
			return;

		pExitKoscomCtrl(m_lpPointer);
	}

	BOOL CreateKoscomCtrl(CWnd *pParentWnd, CWnd *pAUPAgent, UINT nCtrlType, const CRect& rect, UINT nCtrlID, long pMasterMgr)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateKoscomCtrl(pParentWnd, pAUPAgent, nCtrlType, rect, nCtrlID, pMasterMgr);
		return (BOOL)m_lpPointer;
	}

	BOOL CreateCodeInput(CWnd *pParentWnd, CWnd *pAUPAgent, const CRect& rect, UINT nCtrlID, UINT nProType)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateCodeInput(pParentWnd, pAUPAgent, rect, nCtrlID, nProType);
		return (BOOL)m_lpPointer;
	}

	BOOL CreateButton(CWnd *pParentWnd, CWnd *pAUPAgent, const CRect& rect)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateButton(pParentWnd, pAUPAgent, rect);
		return (BOOL)m_lpPointer;
	}

	//short GetGubunFromCode(LPCTSTR szCode)
	//{
	//	if(!m_hLib)
	//		return -1;

	//	return pGetGubunFromCode(m_lpPointer, szCode);
	//}

	void SetDataText(LPCTSTR szText)
	{
		if(!m_hLib)
			return;

		pSetDataText(m_lpPointer, szText);
	}

	void SetDataText2(LPCTSTR szCode)
	{
		if(!m_hLib)
			return;

		pSetDataText2(m_lpPointer, szCode);
	}

	CString GetDataText()
	{
		if(!m_hLib)
			return _T("");

		return pGetDataText(m_lpPointer);
	}

	void SetEnabled(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pSetEnabled(m_lpPointer, bEnable);
	}

	BOOL GetEnabled()
	{
		if(!m_hLib)
			return FALSE;

		return pGetEnabled(m_lpPointer);
	}

	//long GetBase(LPCTSTR lpszKey)
	//{
	//	if(!m_hLib)
	//		return 0L;

	//	return pGetBase(m_lpPointer, lpszKey);
	//}

	void SetFocus()
	{
		if(!m_hLib)
			return;

		pSetFocus(m_lpPointer);
	}

	void SetCurSel(short nIndex)
	{
		if(!m_hLib)
			return;

		pSetCurSel(m_lpPointer, nIndex);
	}

	OLE_HANDLE GetHWnd()
	{
		if(!m_hLib)
			return 0;

		return pGetHWnd(m_lpPointer);
	}

	HWND GetSafeHwnd()
	{
		if(!m_hLib)
			return 0;

		return pGetSafeHwnd(m_lpPointer);
	}

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow(m_lpPointer, x, y, nWidth, nHeight, bRepaint);
	}

	void MoveWindow(LPCRECT lpRect, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow2(m_lpPointer, lpRect, bRepaint);
	}

	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{
		if(!m_hLib)
			return FALSE;

		return pSetWindowPos(m_lpPointer, pWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		if(!m_hLib)
			return FALSE;

		return pShowWindow(m_lpPointer, nCmdShow);
	}

	void GetWindowRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetWindowRect(m_lpPointer, lpRect);
	}

	void GetClientRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetClientRect(m_lpPointer, lpRect);
	}

	void SetMarketKind(UINT nMarketKind)
	{
		pSetMarketKind(m_lpPointer, nMarketKind);
	}
	UINT GetMarketKind()
	{
		return pGetMarketKind(m_lpPointer);
	}
};


#endif // !defined(AFX_LOADHELPERKOSCOM_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)