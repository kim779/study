// ResourceDllManager.h: interface for the CResourceDllManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCEDLLMANAGER_H__BC261F05_7A22_4913_8468_B0F6B4F2ACA6__INCLUDED_)
#define AFX_RESOURCEDLLMANAGER_H__BC261F05_7A22_4913_8468_B0F6B4F2ACA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>									// for CTypedPtrMap
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable

#define _DEFAULT_RESOURCE_ID	IMetaTable::KOREAN

typedef HMODULE ( ResourceModules[ IMetaTable::LANGUAGE_ID_END]);

class CResourceDllManager  
{
public:
	CResourceDllManager();
	virtual ~CResourceDllManager();

protected:
	CTypedPtrMap < CMapStringToPtr, CString, ResourceModules *>		m_mapResModules;
	static char *													m_szLanguageIDs[ IMetaTable::LANGUAGE_ID_END];
public:
	HMODULE		LoadResourceDLL( HMODULE p_hMainModule, IMetaTable::E_LANGUAGEID p_eLanguageID);
	void		UnloadResourceDLL( void);
};

#endif // !defined(AFX_RESOURCEDLLMANAGER_H__BC261F05_7A22_4913_8468_B0F6B4F2ACA6__INCLUDED_)
