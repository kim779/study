#include "stdafx.h"
#include "CodeOcx.h"

/////////////////////////////////////////////////////////////////////////////
// CDispCodeOcx

OLECHAR *CDispCodeOcx::m_osInterfaceName[] = {
	// Variable
//	L"Rows",
	
	// Method
	L"GetBase",
	L"SetInfo",
	L"ShowCodeTable",
	L"ShowModeCodeTable",
	L"SetDataString",
	L"InitData",
	L"GetCodeInfo",
	L"SetPosition",
	L"GetMaxHangSaGa",
	L"ShowCodeTableWithPosition",
	L"RemoveAllListData",
	L"InsertHistory",
	L"GetHistoryList",
	L"GetGlobalDataTypeList",
	L"GetCodePointer",
	L"GetBaseCodeTable",
	L"GetRscMng",
};


void CDispCodeOcx::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}

CDispCodeOcx::CDispCodeOcx()
{
	m_pdispatch = NULL;
}

CDispCodeOcx::~CDispCodeOcx()
{
	delete m_pdID;
	m_pdID = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CDispCodeOcx properties

/////////////////////////////////////////////////////////////////////////////
// CDispCodeOcx operations

long CDispCodeOcx::GetBaseCodeTable()
{
	long result;
	InvokeHelper(m_pdID[IIx_GetBaseCodeTable], DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	// m_pdID[IIx_GetBaseCodeTable]
	return result;
}

void CDispCodeOcx::SetInfo(long _dwInfo)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_SetInfo], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 _dwInfo);
}

long CDispCodeOcx::ShowCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_ShowCodeTable], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		_szCodeKey, &_hWnd);
	return result;
}

long CDispCodeOcx::ShowModeCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(m_pdID[IIx_ShowModeCodeTable], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		_szCodeKey, &_hWnd);
	return result;
}

void CDispCodeOcx::SetDataString(short nKey, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetDataString], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nKey, szData);
}

BOOL CDispCodeOcx::InitData(LPCTSTR szRootPath)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_InitData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szRootPath);
	return result;
}

CString CDispCodeOcx::GetCodeInfo(LPCTSTR _szCodeKey, short _nFlag, LPCTSTR _szKey)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetCodeInfo], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		_szCodeKey, _nFlag, _szKey);
	return result;
}

void CDispCodeOcx::SetPosition(LPCTSTR szKey, short nX, short nY)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_SetPosition], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, nX, nY);
}

CString CDispCodeOcx::GetMaxHangSaGa()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetMaxHangSaGa], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CDispCodeOcx::ShowCodeTableWithPosition(LPCTSTR _szCodeKey, const VARIANT& _hWnd, short nX, short nY)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_ShowCodeTableWithPosition], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		_szCodeKey, &_hWnd, nX, nY);
	return result;
}

BOOL CDispCodeOcx::RemoveAllListData()
{
	BOOL result;
	InvokeHelper(m_pdID[IIx_RemoveAllListData], DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL CDispCodeOcx::InsertHistory(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_I4 VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_InsertHistory], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		nMaxCount, nType, Code, CodeName);
	return result;
}

CString CDispCodeOcx::GetHistoryList(short nMaxCount, long nType)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(m_pdID[IIx_GetHistoryList], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nMaxCount, nType);
	return result;
}

long CDispCodeOcx::GetGlobalDataTypeList()
{
	long result;
	InvokeHelper(m_pdID[IIx_GetGlobalDataTypeList], DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CDispCodeOcx::GetCodePointer(LPCTSTR szKey)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetCodePointer], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szKey);
	return result;
}

void CDispCodeOcx::GetRscMng(long lKey)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_GetRscMng], DISPATCH_METHOD, VT_EMPTY, (void*)&result, parms,
		lKey);
}
