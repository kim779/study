#include "stdafx.h"
#include "PcTrMng.h"

void CRegistryManager::SetStrValue(HKEY hHkey,LPCSTR strKey, LPCSTR strSubKey, LPCSTR strKeyValue)
{
	HKEY hKey;
	DWORD type; 

	::RegCreateKeyEx(hHkey,
				 (LPCTSTR)strKey,0,
				 NULL,
				 REG_OPTION_NON_VOLATILE ,
				 KEY_CREATE_SUB_KEY|KEY_ALL_ACCESS ,
				 NULL,
				 &hKey,
				 &type
				 );

	::RegSetValueEx(hKey,
				(LPCTSTR)strSubKey,
				0,
				REG_SZ,
				(const BYTE *)strKeyValue,
				(DWORD) strlen(strKeyValue)
				);

	::RegCloseKey(hKey);
}

LPCSTR CRegistryManager::GetStrValue(HKEY hHkey,LPCSTR strkey, LPCSTR strsubkey, LPCSTR strDefault)
{
	LONG err;
	HKEY openKey;
	BYTE Data[128];
	DWORD type;
	DWORD size=sizeof(Data);
	
	err=::RegOpenKey(hHkey,strkey,&openKey);
	
	if(ERROR ==err)
		err=::RegQueryValueEx(openKey,strsubkey,0,&type,Data,&size);
	
	if(err==ERROR_SUCCESS)
	{
		::RegCloseKey(openKey);
		CString str(Data);
		return str;
	}
	::RegCloseKey(openKey);
	
	return strDefault;
}

