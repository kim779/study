#include "stdafx.h"
#include "DispFormData.h"


/////////////////////////////////////////////////////////////////////////////
// CDispFormData
OLECHAR *CDispFormData::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"SaveData",
	L"LoadData",
	L"SaveIniData",
	L"LoadIniData",
	L"SaveStringDataToCfg",
	L"SaveIntDataToCfg",
	L"LoadIntDataInCfg",
	L"GetMapCfgFile",
	L"GetUserDataPath",
	L"WritePrivateDataString",
	L"GetPrivateDataString",
	L"WritePrivateDelete",
	L"LoadStringDataInCfg",	
};


CDispFormData::CDispFormData()
{
	m_pdispatch = NULL;
}

void CDispFormData::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispFormData properties


/////////////////////////////////////////////////////////////////////////////
// CDispFormData Method
BOOL CDispFormData::SaveData(LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_SaveData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szFileName, _szKey, _szValue);
	return result;
}

BOOL CDispFormData::LoadData(LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_LoadData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szFileName, _szKey, _szValue);
	return result;
}

BOOL CDispFormData::SaveIniData(LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, const VARIANT FAR& _szValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_SaveIniData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szFileName, _szSection, _szKey, _szValue);
	return result;
}

BOOL CDispFormData::LoadIniData(LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, VARIANT FAR* _szValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_LoadIniData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szFileName, _szSection, _szKey, _szValue);
	return result;
}

BOOL CDispFormData::SaveStringDataToCfg(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SaveStringDataToCfg], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szSection, szKey, szValue);
	return result;
}

BOOL CDispFormData::SaveIntDataToCfg(LPCTSTR szSection, LPCTSTR szKey, short nValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SaveIntDataToCfg], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szSection, szKey, nValue);
	return result;
}

BOOL CDispFormData::LoadIntDataInCfg(LPCTSTR szSection, LPCTSTR szKey, short nDefault, VARIANT FAR* nValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_LoadIntDataInCfg], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szSection, szKey, nDefault, nValue);
	return result;
}

CString CDispFormData::GetMapCfgFile()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetMapCfgFile], DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CDispFormData::GetUserDataPath(LPCTSTR szKey)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetUserDataPath], DISPATCH_METHOD, VT_I4, (void*)&result, parms, szKey);
	return result;
}

short CDispFormData::WritePrivateDataString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_WritePrivateDataString], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szSection, szKey, szData, szFile);
	return result;
}

CString CDispFormData::GetPrivateDataString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetPrivateDataString], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
					szSection, szKey, szDefault, szFile);
	return result;
}

void CDispFormData::WritePrivateDelete(LPCTSTR szSection, LPCTSTR szFile)
{
	static BYTE parms[] =
		 VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_WritePrivateDelete], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szSection, szFile);

}
CString CDispFormData::LoadStringDataInCfg(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_LoadStringDataInCfg], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
					szSection, szKey, szDefault);
	return result;
}

