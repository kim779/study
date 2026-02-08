#include "stdafx.h"
#include "DispFormOpen.h"


/////////////////////////////////////////////////////////////////////////////
// CDispFormOpen
OLECHAR *CDispFormOpen::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"ReLoadMap",
	L"OpenMap",
	L"LoadMap",	
	L"DialogMap",
	L"PopupMap",
	L"CloseMap",
	L"SinglePopupMap",
};


CDispFormOpen::CDispFormOpen()
{
	m_pdispatch = NULL;
}

void CDispFormOpen::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispFormOpen properties


/////////////////////////////////////////////////////////////////////////////
// CDispFormOpen Method
void CDispFormOpen::ReLoadMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_ReLoadMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szMap, szOpenData, nDataLen);
}

void CDispFormOpen::OpenMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_OpenMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szMap, szOpenData, nDataLen);
}

void CDispFormOpen::LoadMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) 
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_LoadMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szMap, szOpenData, nDataLen);
}

void CDispFormOpen::DialogMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_DialogMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szMap, szOpenData, nDataLen);
}

void CDispFormOpen::PopupMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen){
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_PopupMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szMap, szOpenData, nDataLen);
}

void CDispFormOpen::CloseMap()
{
	InvokeHelper(m_pdID[IIx_CloseMap], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispFormOpen::SinglePopupMap(LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SinglePopupMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szMap, szOpenData, nDataLen);
}

