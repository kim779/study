#include "stdafx.h"
#include "DispDesk.h"

/////////////////////////////////////////////////////////////////////////////
// CDispDesk
OLECHAR *CDispDesk::m_osInterfaceName[] = {
	// Variable
	
	// Method
	L"SendTR",
	L"ReceiveTR",
	L"SetSocketHandle",
	L"GetSocketHandle",
	L"SetSharedInterface",
	L"UnLoadScript",
	L"Blank",
	L"ShowTrMessage",
	L"ShowMessageBox",
	L"GetTrList",
	L"GetMapName",
	L"GetMapInfo",
	L"GetCtrlHwndFromName",
	L"GetCtrlNameFromHwnd",
	L"GetCtrlTypeFromHwnd",
	L"GetCtrlTypeFromName",
	L"CallFunction",
	L"SetParentDispatch",
	L"GetDispFromName",
	L"GetRootPath",
	L"GetFormTrData",
	L"SetFormTrData",
	L"FormUnLoad",
	L"SetOpenMapData",
	L"SendDataToOpener",
	L"CallCommand",
	L"GetOpenMapData",
	L"SetOpenMapData2",
	L"NotifyFromMain",
	L"EnableFocus",
	L"GotoFocus",
	L"SetKeyData",
	L"GetKeyData",
	L"SetGSharedNotifyFlag",
	L"LoadXfmEx",
	L"ResizeFont",
	L"GetMapSize",
	L"SetInitFont",
	L"SetTabStop",
	L"NotifyGShareData",
	L"SetSendCountFlag",
	L"DebugTrData",
	L"SendTrDirect",
	L"SetFlag",
	L"ChangeTRInOut",
	L"GetUserColor",
	L"GetCurrentTrMessage",
	L"GetCurrentMsgCode",
	L"SelectPacketCommand",
	L"SetRscMng",
	L"MainKeyPress",
	L"SetMonitoringHandle",
	L"SetNotifyCommand",
	L"AddUserScriptItem",
	L"SetTimeOut",

	L"ExcuteScript",
	L"GetMapData",
	L"SendTRForCert",
	L"ResizeControl",
	L"MoveControl",
	L"SetCtrlResizeFlag",
	L"IsDeskActive",
};


CDispDesk::CDispDesk()
{
	m_pdID = NULL;
	m_pdispatch = NULL;
}

CDispDesk::~CDispDesk()
{
	if(m_pdID) delete m_pdID; m_pdID=NULL;
}

void CDispDesk::InitInterfaceName(IDispatch *pDisp)
{
	int i, nCount = sizeof( m_osInterfaceName) / sizeof( OLECHAR *);
	m_pdID = new DISPID[ nCount];
	for( i = 0; i < nCount; i++)
		pDisp->GetIDsOfNames( IID_NULL, &m_osInterfaceName[ i], 1, LOCALE_USER_DEFAULT, m_pdID + i);
}


/////////////////////////////////////////////////////////////////////////////
// CDispDesk

CString CDispDesk::GetOpenMapData()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetOpenMapData], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short CDispDesk::SendTR(LPCTSTR szTrName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SendTR], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szTrName);
	return result;
}

short CDispDesk::ReceiveTR(LPCTSTR szTrName, long szData, short nDataLen)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I2;
	InvokeHelper(m_pdID[IIx_ReceiveTR], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szTrName, szData, nDataLen);
	return result;
}

void CDispDesk::SetSocketHandle(LPCTSTR szSocket, long dwHandle)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(m_pdID[IIx_SetSocketHandle], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szSocket, dwHandle);
}

long CDispDesk::GetSocketHandle(LPCTSTR szSocket)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetSocketHandle], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szSocket);
	return result;
}

long CDispDesk::SetSharedInterface(long dwInterface)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_SetSharedInterface], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		dwInterface);
	return result;
}

void CDispDesk::UnLoadScript()
{
	InvokeHelper(m_pdID[IIx_UnLoadScript], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispDesk::Blank()
{
	InvokeHelper(m_pdID[IIx_Blank], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispDesk::ShowTrMessage(LPCTSTR _szMessage)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ShowTrMessage], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 _szMessage);
}

short CDispDesk::ShowMessageBox(short _nType, LPCTSTR _szMessage)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ShowMessageBox], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		_nType, _szMessage);
	return result;
}

CString CDispDesk::GetTrList()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetTrList], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CDispDesk::GetMapName()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetMapName], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL CDispDesk::GetMapInfo(LPCTSTR _szKey, VARIANT* _vData)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(m_pdID[IIx_GetMapInfo], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szKey, _vData);
	return result;
}

long CDispDesk::GetCtrlHwndFromName(LPCTSTR _szCtrlName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetCtrlHwndFromName], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		_szCtrlName);
	return result;
}

CString CDispDesk::GetCtrlNameFromHwnd(long _hWnd)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_GetCtrlNameFromHwnd], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		_hWnd);
	return result;
}

CString CDispDesk::GetCtrlTypeFromHwnd(long _hWnd)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_GetCtrlTypeFromHwnd], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		_hWnd);
	return result;
}

CString CDispDesk::GetCtrlTypeFromName(LPCTSTR _szCtrlName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetCtrlTypeFromName], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		_szCtrlName);
	return result;
}

BOOL CDispDesk::CallFunction(LPCTSTR _szFunction, LPCTSTR _szParam, short _nParamLen)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_CallFunction], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szFunction, _szParam, _nParamLen);
	return result;
}

void CDispDesk::SetParentDispatch(LPDISPATCH _pDisp)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(m_pdID[IIx_SetParentDispatch], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 _pDisp);
}

LPDISPATCH CDispDesk::GetDispFromName(LPCTSTR _szCtrlName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetDispFromName], DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		_szCtrlName);
	return result;
}

CString CDispDesk::GetRootPath(LPCTSTR _szSubDir)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GetRootPath], DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		_szSubDir);
	return result;
}

BOOL CDispDesk::GetFormTrData(LPCTSTR _szTr, BSTR* _szData, short* _nDataLen)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_PBSTR VTS_PI2;
	InvokeHelper(m_pdID[IIx_GetFormTrData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szTr, _szData, _nDataLen);
	return result;
}

BOOL CDispDesk::SetFormTrData(LPCTSTR _szTr, LPCTSTR _szData, short _nDataLen)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SetFormTrData], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		_szTr, _szData, _nDataLen);
	return result;
}

void CDispDesk::FormUnLoad()
{
	InvokeHelper(m_pdID[IIx_FormUnLoad], DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDispDesk::SetOpenMapData(short nMapType, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen, LPCTSTR aOpener)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetOpenMapData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nMapType, szMap, szOpenData, nDataLen, aOpener);
}

BOOL CDispDesk::SendDataToOpener(LPCTSTR szData, short nDataLen)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SendDataToOpener], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szData, nDataLen);
	return result;
}

void CDispDesk::CallCommand(LPCTSTR szCmd)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_CallCommand], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szCmd);
}


void CDispDesk::SetOpenMapData2(long dwClipFormat)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_SetOpenMapData2], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwClipFormat);
}

void CDispDesk::NotifyFromMain(LPCTSTR szKey, LPCTSTR szData)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_NotifyFromMain], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, szData);
}

BOOL CDispDesk::EnableFocus(LPCTSTR szCtrlName, short bEnable)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_EnableFocus], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		szCtrlName, bEnable);
	return result;
}

void CDispDesk::GotoFocus(LPCTSTR szCtrlName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_GotoFocus], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szCtrlName);
}

void CDispDesk::SetKeyData(LPCTSTR szKey, long dwData, short nCount)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I2;
	InvokeHelper(m_pdID[IIx_SetKeyData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, dwData, nCount);
}

long CDispDesk::GetKeyData(LPCTSTR szKey, short* nCount)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_PI2;
	InvokeHelper(m_pdID[IIx_GetKeyData], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szKey, nCount);
	return result;
}

void CDispDesk::SetGSharedNotifyFlag(short bNofity)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_SetGSharedNotifyFlag], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bNofity);
}

BOOL CDispDesk::LoadXfmEx(long dwLoadInfo)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_LoadXfmEx], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		dwLoadInfo);
	return result;
}

void CDispDesk::ResizeFont(short nWidth, short nHeight, LPCTSTR szFontName)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ResizeFont], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nWidth, nHeight, szFontName);
}

BOOL CDispDesk::GetMapSize(VARIANT* vrWidth, VARIANT* vrHeight)
{
	BOOL result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(m_pdID[IIx_GetMapSize], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		vrWidth, vrHeight);
	return result;
}

void CDispDesk::SetInitFont(short nWidth, short nHeight, LPCTSTR szFontName)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetInitFont], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nWidth, nHeight, szFontName);
}

void CDispDesk::SetTabStop(short bNew)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_SetTabStop], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bNew);
}

void CDispDesk::NotifyGShareData(LPCTSTR szKey, LPCTSTR szValue)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_NotifyGShareData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, szValue);
}

short CDispDesk::SetSendCountFlag(short nNew)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_SetSendCountFlag], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nNew);
	return result;
}

void CDispDesk::DebugTrData(LPCTSTR szFileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_DebugTrData], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szFileName);
}

short CDispDesk::SendTrDirect(LPCTSTR szTrName, LPCTSTR szData, short nDataLen)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SendTrDirect], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szTrName, szData, nDataLen);
	return result;
}

void CDispDesk::SetFlag(LPCTSTR szKey, short szValue)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_SetFlag], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey, szValue);
}

void CDispDesk::ChangeTRInOut(LPCTSTR szTrName, LPCTSTR szInData, LPCTSTR szOutData, short nFlag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_ChangeTRInOut], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szTrName, szInData, szOutData, nFlag);
}

unsigned long CDispDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	unsigned long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_I2;
	InvokeHelper(m_pdID[IIx_GetUserColor], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nIndex, szName, nFlag);
	return result;
}

CString CDispDesk::GetCurrentTrMessage()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetCurrentTrMessage], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CDispDesk::GetCurrentMsgCode()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetCurrentMsgCode], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short CDispDesk::SelectPacketCommand(short nNew)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(m_pdID[IIx_SelectPacketCommand], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nNew);
	return result;
}

void CDispDesk::SetRscMng(long dwKey)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_SetRscMng], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwKey);
}

void CDispDesk::MainKeyPress(LPCTSTR szKey)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_MainKeyPress], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 szKey);
}

void CDispDesk::SetMonitoringHandle(long pMonitoringHandle)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(m_pdID[IIx_SetMonitoringHandle], DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pMonitoringHandle);
}

long CDispDesk::AddUserScriptItem(LPCTSTR pstrItemName, LPDISPATCH pIDispatch)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_DISPATCH;
	InvokeHelper(m_pdID[IIx_AddUserScriptItem], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pstrItemName, pIDispatch);
	return result;
}

long CDispDesk::ExcuteScript(LPCTSTR sVBName, LPCTSTR sEventName, LPCTSTR sParam)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(m_pdID[IIx_ExcuteScript], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		sVBName, sEventName, sParam);
	return result;
}

CString CDispDesk::GetMapData()
{
	CString result;
	InvokeHelper(m_pdID[IIx_GetMapData], DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CDispDesk::SetNotifyCommand(LPCTSTR sKey)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SetNotifyCommand], DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		sKey);
	return result;
}

short CDispDesk::SendTRForCert(LPCTSTR szTrName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(m_pdID[IIx_SendTRForCert], DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		szTrName);
	return result;
}

BOOL CDispDesk::ResizeControl(LPCTSTR strCtrlName, long nLRPlusSize, long nUDPlusSize) 
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(m_pdID[IIx_ResizeControl], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strCtrlName, nLRPlusSize, nUDPlusSize);
	return result;
}

BOOL CDispDesk::MoveControl(LPCTSTR strCtrlName, long nXDir, long nYDir) 
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(m_pdID[IIx_MoveControl], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strCtrlName, nXDir, nYDir);
	return result;
}

BOOL CDispDesk::SetCtrlResizeFlag(LPCTSTR strCtrlName, BOOL bMove_X, BOOL bMove_Y, BOOL bResize_X, BOOL bResize_Y)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
	InvokeHelper(m_pdID[IIx_MoveControl], DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strCtrlName, bMove_X, bMove_Y, bResize_X, bResize_Y);
	return result;
}

BOOL CDispDesk::IsDeskActive()
{
	BOOL result;
	InvokeHelper(m_pdID[IIx_IsDeskActive], DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}
