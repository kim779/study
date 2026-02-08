#include "stdafx.h"
#include "DispPrimLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDispPrimLib
OLECHAR *CDispPrimLib::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"IsUserDefinedMode",
	L"ReLoadOtherMap",
	L"SetMessage",
	L"MapSwitch",
	L"InsertSplit",
	L"DeleteSplit",
	L"TabToggle",
	L"DefaultSetUp",
	L"SetActiveTab",
	L"ChangeInputRangeResize",
	L"ToggleWindow",
	L"SplitResizeOption",

};


CDispPrimLib::CDispPrimLib()
{
	m_pdispatch = NULL;
}

void CDispPrimLib::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispPrimLib

short CDispPrimLib::IsUserDefinedMode(LPCTSTR strMode, short nConID)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_IsUserDefinedMode], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strMode ,nConID);
	return result;
}
void CDispPrimLib::ReLoadOtherMap(LPCTSTR szPath)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ReLoadOtherMap], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szPath);
}

void CDispPrimLib::SetMessage(LPCTSTR nId)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetMessage], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nId);
}

void CDispPrimLib::MapSwitch(LPCTSTR strLR)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_MapSwitch], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strLR);
}

void CDispPrimLib::InsertSplit(LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPublic, LPCTSTR szGroup, LPCTSTR szSameXfmResize)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
	InvokeHelper(m_pdID[IIx_InsertSplit], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szPath1, szPath2, szPublic, szGroup, szSameXfmResize);
}

void CDispPrimLib::DeleteSplit(LPCTSTR szPath)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_DeleteSplit], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szPath);
}

void CDispPrimLib::TabToggle(LPCTSTR strUpDown)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_TabToggle], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strUpDown);
}

void CDispPrimLib::DefaultSetUp(LPCTSTR szLR, LPCTSTR szUpDown, LPCTSTR szHoga, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPath3, LPCTSTR szExt1, LPCTSTR szExt2) 
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
	InvokeHelper(m_pdID[IIx_DefaultSetUp], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szLR, szUpDown, szHoga, szPath1, szPath2, szPath3, szExt1, szExt2);
}

void CDispPrimLib::SetActiveTab(LPCTSTR szTabID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetActiveTab], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szTabID);
}

void CDispPrimLib::ChangeInputRangeResize(LPCTSTR szChange)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ChangeInputRangeResize], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szChange);
}

void CDispPrimLib::ToggleWindow(LPCTSTR strID, LPCTSTR strDIR, LPCTSTR strSUBDIR)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR ;
	InvokeHelper(m_pdID[IIx_ToggleWindow], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strID, strDIR, strSUBDIR);
}

void CDispPrimLib::SplitResizeOption(LPCTSTR strID, LPCTSTR RightDIR, LPCTSTR BottomDIR)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR ;
	InvokeHelper(m_pdID[IIx_SplitResizeOption], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strID, RightDIR, BottomDIR);
}


