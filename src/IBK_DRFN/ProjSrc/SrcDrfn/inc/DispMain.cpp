#include "stdafx.h"
#include "DispMain.h"

/////////////////////////////////////////////////////////////////////////////
// CDispMain
OLECHAR *CDispMain::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"GetMapInfo", 
	L"GetUserInfo", 
	L"MsgBox", 
	L"ChangeTabProperty", 
	L"SetGShareData", 
	L"GetGShareData", 
	L"SetFrameVariable", 
	L"GetFrameVariable", 
	L"RemoveGlobalData", 
	L"CloseMain", 
	L"Restart", 
	L"GetAcctList", 
	L"SetOpenMapStyle", 
	L"GetGwanList", 
	L"GetGwanCode", 
	L"ClearMessageBar", 
	L"ShowDialog", 
	L"NotifyEvent", 
	L"GetGwanCodeOneGroup", 
	L"SetAcctPasswd", 
	L"GetAcctPasswd", 
	L"SetBatchAcctInfo", 
	L"GetBatchAcctInfo", 
	L"GetMasterData", 
	L"SetOpenMapStyle2", 
	L"IHWriteSectionData", 
	L"IHGetSectionData", 
	L"IHGetSectionNames", 
	L"IHAddSection", 
	L"IHDeleteSection", 
	L"IHRenameSection", 
	L"ChangeFrameSize", 
	L"GetFrameSize", 
	L"ReportCall", 
};


CDispMain::CDispMain()
{
	m_pdispatch = NULL;
}

void CDispMain::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispMain

CString CDispMain::GetMapInfo(short nFlag)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_GetMapInfo], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nFlag);
	return result;
}

CString CDispMain::GetUserInfo(short nType)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_GetUserInfo], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nType);
	return result;
}

short CDispMain::MsgBox(LPCTSTR szMessage, LPCTSTR szCaption, short nFlags)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_MsgBox], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szMessage, szCaption, nFlags);
	return result;
}

BOOL CDispMain::ChangeTabProperty(short nUnitScr, short nTabPage, short nType, short nValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_ChangeTabProperty], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		nUnitScr, nTabPage, nType, nValue);
	return result;
}

void CDispMain::SetGShareData(LPCTSTR szKey, LPCTSTR szValue, short nFlag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SetGShareData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, szValue, nFlag);
}

CString CDispMain::GetGShareData(LPCTSTR szKey)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetGShareData], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szKey);
	return result;
}

void CDispMain::SetFrameVariable(LPCTSTR szKey, LPCTSTR szValue)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetFrameVariable], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, szValue);
}

CString CDispMain::GetFrameVariable(LPCTSTR szKey)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetFrameVariable], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szKey);
	return result;
}

BOOL CDispMain::RemoveGlobalData(LPCTSTR szKey, LPCTSTR szValue)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_RemoveGlobalData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szKey, szValue);
	return result;
}

void CDispMain::CloseMain(short nFlag)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_CloseMain], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nFlag);
}

void CDispMain::Restart()
{
	InvokeHelper(m_pdID[IIx_Restart], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString CDispMain::GetAcctList()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetAcctList], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CDispMain::SetOpenMapStyle(short nType, short nInfo1, short nInfo2, short nMessageBar)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_SetOpenMapStyle], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nType, nInfo1, nInfo2, nMessageBar);
}

CString CDispMain::GetGwanList()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetGwanList], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CDispMain::GetGwanCode(LPCTSTR szKey)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetGwanCode], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szKey);
	return result;
}

void CDispMain::ClearMessageBar()
{
	InvokeHelper(m_pdID[IIx_ClearMessageBar], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CDispMain::ShowDialog(LPCTSTR szKey, short nParam1, short nParam2)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_ShowDialog], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nParam1, nParam2);
	return result;
}

void CDispMain::NotifyEvent(LPCTSTR szKey, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_NotifyEvent], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, szData);
}

CString CDispMain::GetGwanCodeOneGroup(LPCTSTR szKey, LPCTSTR szOption)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetGwanCodeOneGroup], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szKey, szOption);
	return result;
}

void CDispMain::SetAcctPasswd(LPCTSTR szAcct, LPCTSTR szPasswd)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetAcctPasswd], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szAcct, szPasswd);
}

CString CDispMain::GetAcctPasswd(LPCTSTR szAcct)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetAcctPasswd], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szAcct);
	return result;
}

void CDispMain::SetBatchAcctInfo(LPCTSTR szAcctInfo)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetBatchAcctInfo], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szAcctInfo);
}

CString CDispMain::GetBatchAcctInfo(LPCTSTR szAcct)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetBatchAcctInfo], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szAcct);
	return result;
}

CString CDispMain::GetMasterData(short nFlag)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_GetMasterData], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nFlag);
	return result;
}

void CDispMain::SetOpenMapStyle2(short nType, short nInfo1, short nInfo2, short nMessageBar, short nPostOpen)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_SetOpenMapStyle2], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nType, nInfo1, nInfo2, nMessageBar, nPostOpen);
}

short CDispMain::IHWriteSectionData(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_IHWriteSectionData], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szGroup, szSection, szKey, szData, szFile);
	return result;
}

CString CDispMain::IHGetSectionData(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_IHGetSectionData], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szGroup, szSection, szKey, szDefault, szFile);
	return result;
}

CString CDispMain::IHGetSectionNames(LPCTSTR szGroup, LPCTSTR szFile)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_IHGetSectionNames], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szGroup, szFile);
	return result;
}

short CDispMain::IHAddSection(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_IHAddSection], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szGroup, szSection, szFile);
	return result;
}

short CDispMain::IHDeleteSection(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_IHDeleteSection], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szGroup, szSection, szFile);
	return result;
}

short CDispMain::IHRenameSection(LPCTSTR szGroup, LPCTSTR szOldSecName, LPCTSTR szNewSecName, LPCTSTR szFile)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_IHRenameSection], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szGroup, szOldSecName, szNewSecName, szFile);
	return result;
}

void CDispMain::ChangeFrameSize(short nFlag, short nParam1, short nParam2)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(m_pdID[IIx_ChangeFrameSize], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nFlag, nParam1, nParam2);
}

CString CDispMain::GetFrameSize(short nFlag)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_GetFrameSize], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nFlag);
	return result;
}

void CDispMain::ReportCall(LPCTSTR szFile, long lStreamID, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ReportCall], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szFile, lStreamID, szData);
}
