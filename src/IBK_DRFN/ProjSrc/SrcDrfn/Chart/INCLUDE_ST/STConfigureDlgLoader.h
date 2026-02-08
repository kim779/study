// STConfigureDlgLoader.h: interface for the CSTConfigureDlgLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STCONFIGUREDLGLOADER_H__CEC6B2D8_DD41_4DEC_B94D_897805D1C408__INCLUDED_)
#define AFX_STCONFIGUREDLGLOADER_H__CEC6B2D8_DD41_4DEC_B94D_897805D1C408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/ISTControllerD.h"
typedef  BOOL (WINAPI* _DLL_ShowConfigureDlg)(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType);
typedef  BOOL (WINAPI* _DLL_ShowconfigureDlgOnWizard)(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, CRect* pRect);
typedef  BOOL (WINAPI* _DLL_WriteConfigure)(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpFileName, int nType, int nMode);
typedef  void (WINAPI* _DLL_SetPassword)(LPCTSTR lpPassword);
typedef  void (WINAPI* _DLL_SetUserID)(LPCTSTR lpUserID);
typedef  void (WINAPI* _DLL_SetExpiredDate)(LPCTSTR lpExpiredDate);
typedef  CString (WINAPI* _DLL_GetPassword)();
typedef  CString (WINAPI* _DLL_GetUserID)();
typedef  CString (WINAPI* _DLL_GetExpiredDate)();
typedef  void (WINAPI* _DLL_SetUsePassword)(BOOL bUse);
typedef  void (WINAPI* _DLL_SetUseUserID)(BOOL bUse);
typedef  void (WINAPI* _DLL_SetUseExpiredDate)(BOOL bUse);
typedef  BOOL (WINAPI* _DLL_GetUsePassword)();
typedef  BOOL (WINAPI* _DLL_GetUseUserID)();
typedef  BOOL (WINAPI* _DLL_GetUseExpiredDate)();


class CSTConfigureDlgLoader  
{
public:
	CSTConfigureDlgLoader();
	virtual ~CSTConfigureDlgLoader();
	BOOL ShowConfigureDlg(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType);
	BOOL ShowConfigureDlgOnWizard(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, CRect* pRect);
	BOOL WriteConfigure(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpFileName, int nType, int nMode);

	void SetPassword(LPCTSTR lpPassword);
	void SetUserID(LPCTSTR lpUserID);
	void SetExpiredDate(LPCTSTR lpExpiredDate);
	CString GetPassword();
	CString GetUserID();
	CString GetExpiredDate();
	void SetUsePassword(BOOL bUse);
	void SetUseUserID(BOOL bUse);
	void SetUseExpiredDate(BOOL bUse);
	BOOL GetUsePassword();
	BOOL GetUseUserID();
	BOOL GetUseExpiredDate();
private:	
	long LoadSTConfigureDlgLoader();
	BOOL UnloadSTConfigureDlgLoader();
	_DLL_ShowConfigureDlg m_fnShowConfigureDlg;
	_DLL_ShowconfigureDlgOnWizard m_fnShowConfigureDlgOnWizard;
	_DLL_WriteConfigure m_fnWriteConfigure;
	_DLL_SetPassword m_fnSetPassword;
	_DLL_SetUserID m_fnSetUserID;
	_DLL_SetExpiredDate m_fnSetExpiredDate;
	_DLL_GetPassword m_fnGetPassword;
	_DLL_GetUserID m_fnGetUserID;
	_DLL_GetExpiredDate m_fnGetExpiredDate;	
	_DLL_SetUsePassword m_fnSetUsePassword;
	_DLL_SetUseUserID m_fnSetUseUserID;
	_DLL_SetUseExpiredDate m_fnSetUseExpiredDate;
	_DLL_GetUsePassword m_fnGetUsePassword;
	_DLL_GetUseUserID m_fnGetUseUserID;
	_DLL_GetUseExpiredDate m_fnGetUseExpiredDate;

	HINSTANCE m_hSTConfigureDlgLoader;
	CString m_strSTConfigureDlgLoader;

};

#endif // !defined(AFX_STCONFIGUREDLGLOADER_H__CEC6B2D8_DD41_4DEC_B94D_897805D1C408__INCLUDED_)
