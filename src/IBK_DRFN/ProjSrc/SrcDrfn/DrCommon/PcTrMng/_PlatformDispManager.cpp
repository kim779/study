// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"
#include "../../inc/DispDesk.h"
#include "../../inc/DispFormOpen.h"
#include "../../inc/DispFormData.h"
#include "../../inc/DispMain.h"
#include "../../inc/DispFrame.h"
#include "../../inc/DispView.h"
#include "../../inc/DispPrimLib.h"

#define VOID_CHK_ITEM(xxClass, xxKey)	{ if(xxKey==NULL) return; }
#define BOOL_CHK_ITEM(xxClass, xxKey)	{ if(xxKey==NULL) return FALSE; }
#define STRING_CHK_ITEM(xxClass, xxKey)	{ if(xxKey==NULL) return ""; }
#define SHORT_CHK_ITEM(xxClass, xxKey)	{ if(xxKey==NULL) return 0; }
#define LONG_CHK_ITEM(xxClass, xxKey)	{ if(xxKey==NULL) return 0; }
	
// -----------------------------------------------------------------------------------------
//					CDeskDispManager
// -----------------------------------------------------------------------------------------
LONG CDeskDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispDesk*	 pNew = new CDispDesk;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	return (LONG)pNew;
}

void CDeskDispManager::UnAdvise(LONG dwKey)
{
	CDispDesk*	 pItem = (CDispDesk*)dwKey;
	SafeDelete(pItem);
}

short CDeskDispManager::SendTR(LONG dwKey ,LPCTSTR szTrName) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SendTR(szTrName);
}

short CDeskDispManager::ReceiveTR(LONG dwKey ,LPCTSTR szTrName, long szData, short nDataLen) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->ReceiveTR(szTrName, szData, nDataLen);
}

void CDeskDispManager::SetSocketHandle(LONG dwKey ,LPCTSTR szSocket, long dwHandle) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetSocketHandle(szSocket, dwHandle);
}

long CDeskDispManager::GetSocketHandle(LONG dwKey ,LPCTSTR szSocket) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetSocketHandle(szSocket);
}

long CDeskDispManager::SetSharedInterface(LONG dwKey ,long dwInterface) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SetSharedInterface(dwInterface);
}

void CDeskDispManager::UnLoadScript(LONG dwKey) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->UnLoadScript();
}

void CDeskDispManager::Blank(LONG dwKey) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->Blank();
}

void CDeskDispManager::ShowTrMessage(LONG dwKey ,LPCTSTR _szMessage) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->ShowTrMessage(_szMessage);
}

short CDeskDispManager::ShowMessageBox(LONG dwKey ,short _nType, LPCTSTR _szMessage) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->ShowMessageBox(_nType, _szMessage);
}

CString CDeskDispManager::GetTrList(LONG dwKey) 
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetTrList();
}

CString CDeskDispManager::GetMapName(LONG dwKey) 
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetMapName();
}

BOOL CDeskDispManager::GetMapInfo(LONG dwKey ,LPCTSTR _szKey, VARIANT FAR* _vData) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetMapInfo(_szKey, _vData);
}

long CDeskDispManager::GetCtrlHwndFromName(LONG dwKey ,LPCTSTR _szCtrlName) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetCtrlHwndFromName(_szCtrlName);
}

CString CDeskDispManager::GetCtrlNameFromHwnd(LONG dwKey ,long _hWnd) 
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetCtrlNameFromHwnd(_hWnd);
}

CString CDeskDispManager::GetCtrlTypeFromHwnd(LONG dwKey ,long _hWnd) 
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetCtrlTypeFromHwnd(_hWnd);
}

CString CDeskDispManager::GetCtrlTypeFromName(LONG dwKey ,LPCTSTR _szCtrlName) 
{
	STRING_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetCtrlTypeFromName(_szCtrlName);
}

BOOL CDeskDispManager::CallFunction(LONG dwKey ,LPCTSTR _szFunction, LPCTSTR _szParam, short _nParamLen) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->CallFunction(_szFunction, _szParam, _nParamLen);
}

void CDeskDispManager::SetParentDispatch(LONG dwKey ,LPDISPATCH _pDisp) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetParentDispatch(_pDisp);
}

LPDISPATCH CDeskDispManager::GetDispFromName(LONG dwKey ,LPCTSTR _szCtrlName) 
{
	if(dwKey==NULL) return NULL;

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetDispFromName(_szCtrlName);
}

CString CDeskDispManager::GetRootPath(LONG dwKey ,LPCTSTR _szSubDir) 
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetRootPath(_szSubDir);
}

BOOL CDeskDispManager::GetFormTrData(LONG dwKey ,LPCTSTR _szTr, BSTR* _szData, short * _nDataLen) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetFormTrData(_szTr, _szData, _nDataLen);
}

BOOL CDeskDispManager::SetFormTrData(LONG dwKey ,LPCTSTR _szTr, LPCTSTR _szData, short _nDataLen) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SetFormTrData(_szTr, _szData, _nDataLen);
}

void CDeskDispManager::FormUnLoad(LONG dwKey) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->FormUnLoad();
}

void CDeskDispManager::SetOpenMapData(LONG dwKey ,short nMapType, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen, LPCTSTR aOpener) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetOpenMapData(nMapType, szMap, szOpenData, nDataLen, aOpener);
}

BOOL CDeskDispManager::SendDataToOpener(LONG dwKey ,LPCTSTR szData, short nDataLen) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SendDataToOpener(szData, nDataLen);
}

void CDeskDispManager::CallCommand(LONG dwKey ,LPCTSTR szCmd) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->CallCommand(szCmd);
}

CString CDeskDispManager::GetOpenMapData(LONG dwKey) 
{
	STRING_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetOpenMapData();
}

void CDeskDispManager::SetOpenMapData2(LONG dwKey ,long dwClipFormat) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetOpenMapData2(dwClipFormat);
}

void CDeskDispManager::NotifyFromMain(LONG dwKey ,LPCTSTR szKey, LPCTSTR szData) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->NotifyFromMain(szKey, szData);
}

BOOL CDeskDispManager::EnableFocus(LONG dwKey ,LPCTSTR szCtrlName, short bEnable) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->EnableFocus(szCtrlName, bEnable);
}

void CDeskDispManager::GotoFocus(LONG dwKey ,LPCTSTR szCtrlName) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->GotoFocus(szCtrlName);
}

void CDeskDispManager::SetKeyData(LONG dwKey ,LPCTSTR szKey, long dwData, short nCount) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetKeyData(szKey, dwData, nCount);
}

long CDeskDispManager::GetKeyData(LONG dwKey ,LPCTSTR szKey, short FAR* nCount) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetKeyData(szKey, nCount);
}

void CDeskDispManager::SetGSharedNotifyFlag(LONG dwKey ,short bNofity) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetGSharedNotifyFlag(bNofity);
}

BOOL CDeskDispManager::LoadXfmEx(LONG dwKey ,long dwLoadInfo) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->LoadXfmEx(dwLoadInfo);
}

void CDeskDispManager::ResizeFont(LONG dwKey ,short nWidth, short nHeight, LPCTSTR szFontName) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->ResizeFont(nWidth, nHeight, szFontName);
}

BOOL CDeskDispManager::GetMapSize(LONG dwKey ,VARIANT FAR* vrWidth, VARIANT FAR* vrHeight) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetMapSize(vrWidth, vrHeight);
}

void CDeskDispManager::SetInitFont(LONG dwKey ,short nWidth, short nHeight, LPCTSTR szFontName) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetInitFont(nWidth, nHeight, szFontName);
}

void CDeskDispManager::SetTabStop(LONG dwKey ,short bNew) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetTabStop(bNew);
}

void CDeskDispManager::NotifyGShareData(LONG dwKey ,LPCTSTR szKey, LPCTSTR szValue) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->NotifyGShareData(szKey, szValue);
}

short CDeskDispManager::SetSendCountFlag(LONG dwKey ,short nNew) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SetSendCountFlag(nNew);
}

void CDeskDispManager::DebugTrData(LONG dwKey ,LPCTSTR szFileName) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->DebugTrData(szFileName);
}

short CDeskDispManager::SendTrDirect(LONG dwKey ,LPCTSTR szTrName, LPCTSTR szData, short nDataLen) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SendTrDirect(szTrName, szData, nDataLen);
}

void CDeskDispManager::SetFlag(LONG dwKey ,LPCTSTR szKey, short szValue) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetFlag(szKey, szValue);
}

void CDeskDispManager::ChangeTRInOut(LONG dwKey ,LPCTSTR szTRName, LPCTSTR szInData, LPCTSTR szOutData, short nFlag) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->ChangeTRInOut(szTRName, szInData, szOutData, nFlag);
}

OLE_COLOR CDeskDispManager::GetUserColor(LONG dwKey ,short nIndex, LPCTSTR szName, short nFlag) 
{
	if(dwKey==NULL) return NULL;

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetUserColor(nIndex, szName, nFlag);
}

CString CDeskDispManager::GetCurrentTrMessage(LONG dwKey) 
{
	STRING_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetCurrentTrMessage();
}

CString CDeskDispManager::GetCurrentMsgCode(LONG dwKey) 
{
	STRING_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetCurrentTrMessage();
}

short CDeskDispManager::SelectPacketCommand(LONG dwKey ,short nNew) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SelectPacketCommand(nNew);
}

void CDeskDispManager::SetRscMng(LONG dwKey ,long _dwKey) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetRscMng(_dwKey);
}

void CDeskDispManager::MainKeyPress(LONG dwKey ,LPCTSTR szKey) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->MainKeyPress(szKey);
}

void CDeskDispManager::SetMonitoringHandle(LONG dwKey ,long pMonitoringHandle) 
{
	VOID_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	pItem->SetMonitoringHandle(pMonitoringHandle);
}

long CDeskDispManager::AddUserScriptItem(LONG dwKey ,LPCTSTR pstrItemName, LPDISPATCH pIDispatch) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->AddUserScriptItem(pstrItemName, pIDispatch);
}

long CDeskDispManager::ExcuteScript(LONG dwKey ,LPCTSTR sVBName, LPCTSTR sEventName, LPCTSTR sParam) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->ExcuteScript(sVBName, sEventName, sParam);
}

CString CDeskDispManager::GetMapData(LONG dwKey) 
{
	STRING_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->GetMapData();
}

long CDeskDispManager::SetNotifyCommand(LONG dwKey ,LPCTSTR sKey) 
{
	LONG_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SetNotifyCommand(sKey);
}

short CDeskDispManager::SendTRForCert(LONG dwKey ,LPCTSTR szTrName) 
{
	SHORT_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SendTRForCert(szTrName);
}

BOOL CDeskDispManager::ResizeControl(LONG dwKey ,LPCTSTR strCtrlName, long nLRPlusSize, long nUDPlusSize) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->ResizeControl(strCtrlName, nLRPlusSize, nUDPlusSize);
}

BOOL CDeskDispManager::MoveControl(LONG dwKey ,LPCTSTR strCtrlName, long nXDir, long nYDir) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->MoveControl(strCtrlName, nXDir, nYDir);
}

BOOL CDeskDispManager::SetCtrlResizeFlag(LONG dwKey ,LPCTSTR strCtrlName, BOOL bMove_X, BOOL bMove_Y, BOOL bResize_X, BOOL bResize_Y) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->SetCtrlResizeFlag(strCtrlName, bMove_X, bMove_Y, bResize_X, bResize_Y);
}

BOOL CDeskDispManager::IsDeskActive(LONG dwKey) 
{
	BOOL_CHK_ITEM(CDispDesk, dwKey);

	CDispDesk* pItem = (CDispDesk*)dwKey;
	return pItem->IsDeskActive();
}


// -----------------------------------------------------------------------------------------
//					CFormOpenDispManager
// -----------------------------------------------------------------------------------------
LONG CFormOpenDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispFormOpen*	 pNew = new CDispFormOpen;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	//m_pCurobWnd->SetGridDispatch(pDisp);
	return (LONG)pNew;
}

void CFormOpenDispManager::UnAdvise(LONG dwKey)
{
	CDispFormOpen*	 pItem = (CDispFormOpen*)dwKey;
	SafeDelete(pItem);
}

void CFormOpenDispManager::ReLoadMap(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->ReLoadMap(szMap, szOpenData, nDataLen);
}

void CFormOpenDispManager::OpenMap(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)  
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->OpenMap(szMap, szOpenData, nDataLen);
}

void CFormOpenDispManager::LoadMap(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)  
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->LoadMap(szMap, szOpenData, nDataLen);
}

void CFormOpenDispManager::DialogMap(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->DialogMap(szMap, szOpenData, nDataLen);
}

void CFormOpenDispManager::PopupMap(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->PopupMap(szMap, szOpenData, nDataLen);
}

void CFormOpenDispManager::CloseMap(LONG dwKey) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->CloseMap();
}

void CFormOpenDispManager::SinglePopupMap(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) 
{
	VOID_CHK_ITEM(CDispFormOpen, dwKey);

	CDispFormOpen* pItem = (CDispFormOpen*)dwKey;
	pItem->SinglePopupMap(szMap, szOpenData, nDataLen);
}


// -----------------------------------------------------------------------------------------
//					CFormOpenDispManager
// -----------------------------------------------------------------------------------------
LONG CFormDataDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispFormData*	 pNew = new CDispFormData;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	return (LONG)pNew;
}

void CFormDataDispManager::UnAdvise(LONG dwKey)
{
	CDispFormData*	 pItem = (CDispFormData*)dwKey;
	SafeDelete(pItem);
}

BOOL CFormDataDispManager::SaveData(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->SaveData(_szFileName, _szKey, _szValue);
}

BOOL CFormDataDispManager::LoadData(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->LoadData(_szFileName, _szKey, _szValue);
}

BOOL CFormDataDispManager::SaveIniData(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, const VARIANT FAR& _szValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->SaveIniData(_szFileName, _szSection, _szKey, _szValue);
}

BOOL CFormDataDispManager::LoadIniData(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, VARIANT FAR* _szValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->LoadIniData(_szFileName, _szSection, _szKey, _szValue);
}

BOOL CFormDataDispManager::SaveStringDataToCfg(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->SaveStringDataToCfg(szSection, szKey , szValue);
}

BOOL CFormDataDispManager::SaveIntDataToCfg(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, short nValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->SaveIntDataToCfg(szSection, szKey, nValue);
}

BOOL CFormDataDispManager::LoadIntDataInCfg(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, short nDefault, VARIANT FAR* nValue)
{
	BOOL_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->LoadIntDataInCfg(szSection, szKey, nDefault, nValue);
}

CString CFormDataDispManager::GetMapCfgFile(LONG dwKey)
{
	STRING_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->GetMapCfgFile();
}

CString CFormDataDispManager::GetUserDataPath(LONG dwKey, LPCTSTR szKey)
{
	STRING_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->GetUserDataPath(szKey);
}

short CFormDataDispManager::WritePrivateDataString(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile)
{
	SHORT_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->WritePrivateDataString(szSection, szKey, szData, szFile);
}

CString CFormDataDispManager::GetPrivateDataString(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile)
{
	STRING_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->GetPrivateDataString(szSection, szKey, szDefault, szFile);
}

void CFormDataDispManager::WritePrivateDelete(LONG dwKey, LPCTSTR szSection, LPCTSTR szFile)
{
	VOID_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	pItem->WritePrivateDelete(szSection, szFile);
}

CString CFormDataDispManager::LoadStringDataInCfg(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault)
{
	STRING_CHK_ITEM(CDispFormData, dwKey);

	CDispFormData* pItem = (CDispFormData*)dwKey;
	return pItem->LoadStringDataInCfg(szSection, szKey, szDefault);
}



// -----------------------------------------------------------------------------------------
//					CMainDispManager
// -----------------------------------------------------------------------------------------
LONG CMainDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispMain*	 pNew = new CDispMain;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	return (LONG)pNew;
}

void CMainDispManager::UnAdvise(LONG dwKey)
{
	CDispMain*	 pItem = (CDispMain*)dwKey;
	SafeDelete(pItem);
}

CString CMainDispManager::GetMapInfo(LONG dwKey, short nType)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetMapInfo(nType);
}

CString CMainDispManager::GetUserInfo(LONG dwKey, short nType)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetUserInfo(nType);
}

short CMainDispManager::MsgBox(LONG dwKey, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags)
{
	SHORT_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->MsgBox(szMessage, szCaption, nFlags);
}

BOOL CMainDispManager::ChangeTabProperty(LONG dwKey, short nUnitScr, short nTabPage, short nType, short nValue)
{
	BOOL_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->ChangeTabProperty(nUnitScr, nTabPage, nType, nValue);
}

void CMainDispManager::SetGShareData(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue, short nFlag)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->SetGShareData(szKey, szValue, nFlag);
}

CString CMainDispManager::GetGShareData(LONG dwKey, LPCTSTR szKey)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetGShareData(szKey);
}

void CMainDispManager::SetFrameVariable(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->SetFrameVariable(szKey, szValue);
}

CString CMainDispManager::GetFrameVariable(LONG dwKey, LPCTSTR szKey)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetFrameVariable(szKey);
}

BOOL CMainDispManager::RemoveGlobalData(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue)
{
	BOOL_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->RemoveGlobalData(szKey, szValue);
}

void CMainDispManager::CloseMain(LONG dwKey, short nFlag)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->CloseMain(nFlag);
}

void CMainDispManager::Restart(LONG dwKey)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->Restart();
}

CString CMainDispManager::GetAcctList(LONG dwKey)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetAcctList();
}

void CMainDispManager::SetOpenMapStyle(LONG dwKey, short nType, short nInfo1, short nInfo2, short nMessageBar)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->SetOpenMapStyle(nType, nInfo1, nInfo2, nMessageBar);
}

CString CMainDispManager::GetGwanList(LONG dwKey)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetGwanList();
}

CString CMainDispManager::GetGwanCode(LONG dwKey, LPCTSTR szKey)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetGwanCode(szKey);
}

void CMainDispManager::ClearMessageBar(LONG dwKey)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->ClearMessageBar();
}

short CMainDispManager::ShowDialog(LONG dwKey, LPCTSTR szKey, short nParam1, short nParam2)
{
	SHORT_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->ShowDialog(szKey, nParam1, nParam2);
}

void CMainDispManager::NotifyEvent(LONG dwKey, LPCTSTR szKey, LPCTSTR szData)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->NotifyEvent(szKey, szData);
}

CString CMainDispManager::GetGwanCodeOneGroup(LONG dwKey, LPCTSTR szKey, LPCTSTR szOption)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetGwanCodeOneGroup(szKey, szOption);
}

void CMainDispManager::SetAcctPasswd(LONG dwKey, LPCTSTR szAcct, LPCTSTR szPasswd)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->SetAcctPasswd(szAcct, szPasswd);
}

CString CMainDispManager::GetAcctPasswd(LONG dwKey, LPCTSTR szAcct)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetAcctPasswd(szAcct);
}

void CMainDispManager::SetBatchAcctInfo(LONG dwKey, LPCTSTR szAcctInfo)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->SetBatchAcctInfo(szAcctInfo);
}

CString CMainDispManager::GetBatchAcctInfo(LONG dwKey, LPCTSTR szAcct)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetBatchAcctInfo(szAcct);
}

CString CMainDispManager::GetMasterData(LONG dwKey, short nFlag)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetMasterData(nFlag);
}

void CMainDispManager::SetOpenMapStyle2(LONG dwKey, short nType, short nInfo1, short nInfo2, short nMessageBar, short nPostOpen)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->SetOpenMapStyle2(nType, nInfo1, nInfo2, nMessageBar, nPostOpen);
}

short CMainDispManager::IHWriteSectionData(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile)
{
	SHORT_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->IHWriteSectionData(szGroup, szSection, szKey, szData, szFile);
}

CString CMainDispManager::IHGetSectionData(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->IHGetSectionData(szGroup, szSection, szKey, szDefault, szFile);
}

CString CMainDispManager::IHGetSectionNames(LONG dwKey, LPCTSTR szGroup, LPCTSTR szFile)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->IHGetSectionNames(szGroup, szFile);
}

short CMainDispManager::IHAddSection(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile)
{
	SHORT_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->IHAddSection(szGroup, szSection, szFile);
}

short CMainDispManager::IHDeleteSection(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile)
{
	SHORT_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->IHDeleteSection(szGroup, szSection, szFile);
}

short CMainDispManager::IHRenameSection(LONG dwKey, LPCTSTR szGroup, LPCTSTR szOldSecName, LPCTSTR szNewSecName, LPCTSTR szFile)
{
	SHORT_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->IHRenameSection(szGroup, szOldSecName, szNewSecName, szFile);
}

void CMainDispManager::ChangeFrameSize(LONG dwKey, short nFlag, short nParam1, short nParam2)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->ChangeFrameSize(nFlag, nParam1, nParam2);
}

CString CMainDispManager::GetFrameSize(LONG dwKey, short nFlag)
{
	STRING_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	return pItem->GetFrameSize(nFlag);
}

void CMainDispManager::ReportCall(LONG dwKey, LPCTSTR szFile, long lStreamID, LPCTSTR szData)
{
	VOID_CHK_ITEM(CDispMain, dwKey);

	CDispMain* pItem = (CDispMain*)dwKey;
	pItem->ReportCall(szFile, lStreamID, szData);
}
	
// -----------------------------------------------------------------------------------------
//					CFrameDispManager
// -----------------------------------------------------------------------------------------
LONG CFrameDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispFrame*	 pNew = new CDispFrame;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	return (LONG)pNew;
}

void CFrameDispManager::UnAdvise(LONG dwKey)
{
	CDispFrame*	 pItem = (CDispFrame*)dwKey;
	SafeDelete(pItem);
}

short CFrameDispManager::GetDataSaveType(LONG dwKey )
{
	SHORT_CHK_ITEM(CDispFrame, dwKey);

	CDispFrame* pItem = (CDispFrame*)dwKey;
	return pItem->GetDataSaveType();
}

CString CFrameDispManager::GetFrameInfoFile(LONG dwKey )
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispFrame* pItem = (CDispFrame*)dwKey;
	return pItem->GetFrameInfoFile();
}

short CFrameDispManager::GetFontSize(LONG dwKey) 
{
	SHORT_CHK_ITEM(CDispFrame, dwKey);

	CDispFrame* pItem = (CDispFrame*)dwKey;
	return pItem->GetFontSize();
}

CString CFrameDispManager::GetFrameScrNum(LONG dwKey )
{
	STRING_CHK_ITEM(CDispFrame, dwKey);

	CDispFrame* pItem = (CDispFrame*)dwKey;
	return pItem->GetFrameScrNum();
}

void CFrameDispManager::ChangeFrameSize(LONG dwKey ,short nFlag, short nParam1, short nParam2)
{
	VOID_CHK_ITEM(CDispFrame, dwKey);

	CDispFrame* pItem = (CDispFrame*)dwKey;
	pItem->ChangeFrameSize(nFlag ,nParam1 ,nParam2);
}

// -----------------------------------------------------------------------------------------
//					CViewDispManager
// -----------------------------------------------------------------------------------------
LONG CViewDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispView*	 pNew = new CDispView;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	return (LONG)pNew;
}

void CViewDispManager::UnAdvise(LONG dwKey)
{
	CDispView*	 pItem = (CDispView*)dwKey;
	SafeDelete(pItem);
}

short CViewDispManager::ShowDialog(LONG dwKey ,LPCTSTR szKey, short nIndex)
{
	SHORT_CHK_ITEM(CDispView, dwKey);

	CDispView* pItem = (CDispView*)dwKey;
	return pItem->ShowDialog(szKey ,nIndex);
}

short CViewDispManager::MsgBoxID(LONG dwKey ,short nID, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags)
{
	SHORT_CHK_ITEM(CDispView, dwKey);

	CDispView* pItem = (CDispView*)dwKey;
	return pItem->MsgBoxID(nID ,szMessage ,szCaption ,nFlags);
}


// -----------------------------------------------------------------------------------------
//					CPrimLibDispManager
// -----------------------------------------------------------------------------------------
LONG CPrimLibDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;

	CDispPrimLib* pNew = new CDispPrimLib;
	pNew->m_pdispatch=pDisp;
	pNew->AttachDispatch(pDisp);
	pNew->InitInterfaceName(pDisp);

	return (LONG)pNew;
	return NULL;
}

void CPrimLibDispManager::UnAdvise(LONG dwKey)
{
	CDispPrimLib*	 pItem = (CDispPrimLib*)dwKey;
	SafeDelete(pItem);
}

short CPrimLibDispManager::IsUserDefinedMode(LONG dwKey, LPCTSTR strMode, short nConID)
{
	SHORT_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	return pItem->IsUserDefinedMode(strMode, nConID);
}

void CPrimLibDispManager::ReLoadOtherMap(LONG dwKey, LPCTSTR szPath)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->ReLoadOtherMap(szPath);
}

void CPrimLibDispManager::SetMessage(LONG dwKey, LPCTSTR nId)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->SetMessage(nId);
}

void CPrimLibDispManager::MapSwitch(LONG dwKey, LPCTSTR strLR)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->MapSwitch(strLR);
}

void CPrimLibDispManager::InsertSplit(LONG dwKey, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPublic, LPCTSTR szGroup, LPCTSTR szSameXfmResize)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->InsertSplit(szPath1, szPath2, szPublic, szGroup, szSameXfmResize);
}

void CPrimLibDispManager::DeleteSplit(LONG dwKey, LPCTSTR szPath)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->DeleteSplit(szPath);
}

void CPrimLibDispManager::TabToggle(LONG dwKey, LPCTSTR strUpDown)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->TabToggle(strUpDown);
}

void CPrimLibDispManager::DefaultSetUp(LONG dwKey, LPCTSTR szLR, LPCTSTR szUpDown, LPCTSTR szHoga, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPath3, LPCTSTR szExt1, LPCTSTR szExt2) 
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->DefaultSetUp(szLR, szUpDown, szHoga, szPath1, szPath2, szPath3, szExt1, szExt2);
}

void CPrimLibDispManager::SetActiveTab(LONG dwKey, LPCTSTR szTabID)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->SetActiveTab(szTabID);
}

void CPrimLibDispManager::ChangeInputRangeResize(LONG dwKey, LPCTSTR szChange)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->ChangeInputRangeResize(szChange);
}

void CPrimLibDispManager::ToggleWindow(LONG dwKey, LPCTSTR strID, LPCTSTR strDIR, LPCTSTR strSUBDIR)
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->ToggleWindow(strID, strDIR, strSUBDIR);
}

void CPrimLibDispManager::SplitResizeOption(LONG dwKey, LPCTSTR strID, LPCTSTR RightDIR, LPCTSTR BottomDIR) 
{
	VOID_CHK_ITEM(CDispPrimLib, dwKey);

	CDispPrimLib* pItem = (CDispPrimLib*)dwKey;
	pItem->SplitResizeOption(strID, RightDIR, BottomDIR);
}



// -----------------------------------------------------------------------------------------
//					CPrimScrDispManager
// -----------------------------------------------------------------------------------------
LONG CPrimScrDispManager::Advise(LPUNKNOWN pUnKnown)
{
	if(!pUnKnown) return NULL;

	IDispatch* pDisp;
	HRESULT hr = pUnKnown->QueryInterface(IID_IDispatch, (void**)&pDisp);
	if(FAILED(hr))	return NULL;
//
//	CDispDesk*	 pNew = new CDispDesk;
//	pNew->m_pdispatch=pDisp;
//	pNew->AttachDispatch(pDisp);
//	pNew->InitInterfaceName(pDisp);
//
//	return (LONG)pNew;
	return NULL;
}

void CPrimScrDispManager::UnAdvise(LONG dwKey)
{
//	CDispDesk*	 pItem = (CDispDesk*)dwKey;
//	SafeDelete(pItem);
}


