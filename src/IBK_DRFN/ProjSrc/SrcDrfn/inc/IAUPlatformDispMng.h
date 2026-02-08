#if !defined(AFX_HANARO_PLATFORMDISPATCH_MANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_PLATFORMDISPATCH_MANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/OAUPlatformDispMng.h"
#include "./IAUDispMng.h"

#define		UUID_IAUFormOpenDispManager			30
#define		UUID_IAUFormDataDispManager			31
#define		UUID_IAUDeskDispManager				32
#define		UUID_IAUMainDispManager				33
#define		UUID_IAUFrameDispManager			34
#define		UUID_IAUViewDispManager				35
#define		UUID_IAUPrimLibDispManager			36
#define		UUID_IAUPrimScrDispManager			37


#define		SCRIPT_OPENMANAGER		_T("Form_OpenMan")
#define		SCRIPT_DATAMAN			_T("Form_DataMan")
#define		SCRIPT_FORM				_T("Form")
#define		SCRIPT_MAIN				_T("Main")
#define		SCRIPT_FRAME			_T("frame")
#define		SCRIPT_VIEW				_T("view")
#define		SCRIPT_PRIMLIB			_T("primlib")
#define		SCRIPT_PRIMSCR			_T("primscr")

#define		FORM_OPENMANAGER		SCRIPT_OPENMANAGER
#define		FORM_DATAMAN			SCRIPT_DATAMAN

//	참고... 메인의 xgpView.cpp에서 구현한 부분.
//	m_ctrlDesk->SetParentDispatch(pDisPatch);
//	m_ctrlDesk->AddUserScriptItem("frame", m_pIChildFrm->m_pIFrame->GetIDispatch(FALSE));
//	m_ctrlDesk->AddUserScriptItem("view", m_IView.GetIDispatch(FALSE));
//
// 	m_ctrlDesk->AddUserScriptItem("primlib", m_pIChildFrm->GetPrimLibInterface());// 추가 이전 프라임 라이브러리 적용부분 주석
//	m_ctrlDesk->AddUserScriptItem("primscr", GetPrimScrInterface());


// #define		FORMITEM				_T("Form")
// #define		MAINITEM				_T("Main")

//  int nID = 30;
// 	IAUFormOpenDispManager* pMng = (IAUFormOpenDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUFormOpenDispManager* pMng = (IAUFormOpenDispManager*)AfxGetPctrInterface(nID);

//  int nID = 31;
// 	IAUFormDataDispManager* pMng = (IAUFormDataDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUFormDataDispManager* pMng = (IAUFormDataDispManager*)AfxGetPctrInterface(nID);

//  int nID = 32;
// 	IAUDeskDispManager* pMng = (IAUDeskDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUDeskDispManager* pMng = (IAUDeskDispManager*)AfxGetPctrInterface(nID);

//  int nID = 33;
// 	IAUMainDispManager* pMng = (IAUMainDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUMainDispManager* pMng = (IAUMainDispManager*)AfxGetPctrInterface(nID);

//  int nID = 34;
// 	IAUFrameDispManager* pMng = (IAUFrameDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUFrameDispManager* pMng = (IAUFrameDispManager*)AfxGetPctrInterface(nID);

//  int nID = 35;
// 	IAUViewDispManager* pMng = (IAUViewDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUViewDispManager* pMng = (IAUViewDispManager*)AfxGetPctrInterface(nID);

//  int nID = 36;
// 	IAUPrimLibDispManager* pMng = (IAUPrimLibDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUPrimLibDispManager* pMng = (IAUPrimLibDispManager*)AfxGetPctrInterface(nID);

//  int nID = 37;
// 	IAUPrimScrDispManager* pMng = (IAUPrimScrDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUPrimScrDispManager* pMng = (IAUPrimScrDispManager*)AfxGetPctrInterface(nID);


// 만든이  : 이준옥 2006.01.25(수)
// History : 2006.01.25 인터페이스
//

//순서.
//0. LPDISPATCH pDeskDisp = m_pBaseDesk->GetUnknown();
//   pDeskDisp->Release();
//1. IAUDeskManager* pMng=인터페이스구하기.
//2. LONG dwKey = pMng->Advise(pDeskDisp);
//3. pMng->GetOpenMapData(dwKey, ....);
//4. pMng->UnAdvise(dwKey);


// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
DECLARE_INTERFACE_(IAUDeskDispManager, IAUDispManager)
{
	STDMETHOD_(short ,SendTR)(LONG dwKey ,LPCTSTR szTrName) PURE;
	STDMETHOD_(short ,ReceiveTR)(LONG dwKey ,LPCTSTR szTrName, long szData, short nDataLen) PURE;
	STDMETHOD_(void ,SetSocketHandle)(LONG dwKey ,LPCTSTR szSocket, long dwHandle) PURE;
	STDMETHOD_(long ,GetSocketHandle)(LONG dwKey ,LPCTSTR szSocket) PURE;
	STDMETHOD_(long ,SetSharedInterface)(LONG dwKey ,long dwInterface) PURE;
	STDMETHOD_(void ,UnLoadScript)(LONG dwKey) PURE;
	STDMETHOD_(void ,Blank)(LONG dwKey) PURE;
	STDMETHOD_(void ,ShowTrMessage)(LONG dwKey ,LPCTSTR _szMessage) PURE;
	STDMETHOD_(short ,ShowMessageBox)(LONG dwKey ,short _nType, LPCTSTR _szMessage) PURE;
	STDMETHOD_(CString ,GetTrList)(LONG dwKey) PURE;
	STDMETHOD_(CString ,GetMapName)(LONG dwKey) PURE;
	STDMETHOD_(BOOL ,GetMapInfo)(LONG dwKey ,LPCTSTR _szKey, VARIANT FAR* _vData) PURE;
	STDMETHOD_(long ,GetCtrlHwndFromName)(LONG dwKey ,LPCTSTR _szCtrlName) PURE;
	STDMETHOD_(CString ,GetCtrlNameFromHwnd)(LONG dwKey ,long _hWnd) PURE;
	STDMETHOD_(CString ,GetCtrlTypeFromHwnd)(LONG dwKey ,long _hWnd) PURE;
	STDMETHOD_(CString ,GetCtrlTypeFromName)(LONG dwKey ,LPCTSTR _szCtrlName) PURE;
	STDMETHOD_(BOOL ,CallFunction)(LONG dwKey ,LPCTSTR _szFunction, LPCTSTR _szParam, short _nParamLen) PURE;
	STDMETHOD_(void ,SetParentDispatch)(LONG dwKey ,LPDISPATCH _pDisp) PURE;
	STDMETHOD_(LPDISPATCH ,GetDispFromName)(LONG dwKey ,LPCTSTR _szCtrlName) PURE;
	STDMETHOD_(CString ,GetRootPath)(LONG dwKey ,LPCTSTR _szSubDir) PURE;
	STDMETHOD_(BOOL ,GetFormTrData)(LONG dwKey ,LPCTSTR _szTr, BSTR* _szData, short *_nDataLen) PURE;
	STDMETHOD_(BOOL ,SetFormTrData)(LONG dwKey ,LPCTSTR _szTr, LPCTSTR _szData, short _nDataLen) PURE;
	STDMETHOD_(void ,FormUnLoad)(LONG dwKey) PURE;
	STDMETHOD_(void ,SetOpenMapData)(LONG dwKey ,short nMapType, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen, LPCTSTR aOpener) PURE;
	STDMETHOD_(BOOL ,SendDataToOpener)(LONG dwKey ,LPCTSTR szData, short nDataLen) PURE;
	STDMETHOD_(void ,CallCommand)(LONG dwKey ,LPCTSTR szCmd) PURE;

	// vData 에 vData->vt = VT_CString 속성으로 데이터를 얻어온다.
	// vData->CStringVal 값에 실제 데이터 들어감.
	// 출력값long은 데이터의 길이.
	STDMETHOD_(CString ,GetOpenMapData)(LONG dwKey) PURE;
	STDMETHOD_(void ,SetOpenMapData2)(LONG dwKey ,long dwClipFormat) PURE;
	STDMETHOD_(void ,NotifyFromMain)(LONG dwKey ,LPCTSTR szKey, LPCTSTR szData) PURE;
	STDMETHOD_(BOOL ,EnableFocus)(LONG dwKey ,LPCTSTR szCtrlName, short bEnable) PURE;
	STDMETHOD_(void ,GotoFocus)(LONG dwKey ,LPCTSTR szCtrlName) PURE;
	STDMETHOD_(void ,SetKeyData)(LONG dwKey ,LPCTSTR szKey, long dwData, short nCount) PURE;
	STDMETHOD_(long ,GetKeyData)(LONG dwKey ,LPCTSTR szKey, short FAR* nCount) PURE;
	STDMETHOD_(void ,SetGSharedNotifyFlag)(LONG dwKey ,short bNofity) PURE;
	STDMETHOD_(BOOL ,LoadXfmEx)(LONG dwKey ,long dwLoadInfo) PURE;
	STDMETHOD_(void ,ResizeFont)(LONG dwKey ,short nWidth, short nHeight, LPCTSTR szFontName) PURE;
	STDMETHOD_(BOOL ,GetMapSize)(LONG dwKey ,VARIANT FAR* vrWidth, VARIANT FAR* vrHeight) PURE;
	STDMETHOD_(void ,SetInitFont)(LONG dwKey ,short nWidth, short nHeight, LPCTSTR szFontName) PURE;
	STDMETHOD_(void ,SetTabStop)(LONG dwKey ,short bNew) PURE;
	STDMETHOD_(void ,NotifyGShareData)(LONG dwKey ,LPCTSTR szKey, LPCTSTR szValue) PURE;
	STDMETHOD_(short ,SetSendCountFlag)(LONG dwKey ,short nNew) PURE;
	STDMETHOD_(void ,DebugTrData)(LONG dwKey ,LPCTSTR szFileName) PURE;
	STDMETHOD_(short ,SendTrDirect)(LONG dwKey ,LPCTSTR szTrName, LPCTSTR szData, short nDataLen) PURE;
	STDMETHOD_(void ,SetFlag)(LONG dwKey ,LPCTSTR szKey, short szValue) PURE;
	STDMETHOD_(void ,ChangeTRInOut)(LONG dwKey ,LPCTSTR szTRName, LPCTSTR szInData, LPCTSTR szOutData, short nFlag) PURE;
	STDMETHOD_(OLE_COLOR ,GetUserColor)(LONG dwKey ,short nIndex, LPCTSTR szName, short nFlag) PURE;
	STDMETHOD_(CString ,GetCurrentTrMessage)(LONG dwKey) PURE;
	STDMETHOD_(CString ,GetCurrentMsgCode)(LONG dwKey) PURE;
	STDMETHOD_(short ,SelectPacketCommand)(LONG dwKey ,short nNew) PURE;
	STDMETHOD_(void ,SetRscMng)(LONG dwKey ,long _dwKey) PURE;
	STDMETHOD_(void ,MainKeyPress)(LONG dwKey ,LPCTSTR szKey) PURE;
	STDMETHOD_(void ,SetMonitoringHandle)(LONG dwKey ,long pMonitoringHandle) PURE;
	STDMETHOD_(long ,AddUserScriptItem)(LONG dwKey ,LPCTSTR pstrItemName, LPDISPATCH pIDispatch) PURE;
	STDMETHOD_(long ,ExcuteScript)(LONG dwKey ,LPCTSTR sVBName, LPCTSTR sEventName, LPCTSTR sParam) PURE;
	STDMETHOD_(CString ,GetMapData)(LONG dwKey) PURE;
	STDMETHOD_(long ,SetNotifyCommand)(LONG dwKey ,LPCTSTR sKey) PURE;
	STDMETHOD_(BOOL ,ResizeControl)(LONG dwKey ,LPCTSTR strCtrlName, long nLRPlusSize, long nUDPlusSize) PURE;
	STDMETHOD_(BOOL ,MoveControl)(LONG dwKey ,LPCTSTR strCtrlName, long nXDir, long nYDir) PURE;
	STDMETHOD_(BOOL ,SetCtrlResizeFlag)(LONG dwKey ,LPCTSTR strCtrlName, BOOL bMove_X, BOOL bMove_Y, BOOL bResize_X, BOOL bResize_Y) PURE;
	STDMETHOD_(short ,SendTRForCert)(LONG dwKey ,LPCTSTR szTrName) PURE;
	STDMETHOD_(BOOL ,IsDeskActive)(LONG dwKey) PURE;

};




// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
DECLARE_INTERFACE_(IAUFormOpenDispManager, IAUDispManager)
{
	STDMETHOD_(void ,ReLoadMap)(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)PURE;
	STDMETHOD_(void ,OpenMap)(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)  PURE;
	STDMETHOD_(void ,LoadMap)(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)  PURE;
	STDMETHOD_(void ,DialogMap)(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)PURE;
	STDMETHOD_(void ,PopupMap)(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) PURE;
	STDMETHOD_(void ,CloseMap)(LONG dwKey ) PURE;
	STDMETHOD_(void ,SinglePopupMap)(LONG dwKey, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) PURE;
};


// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
DECLARE_INTERFACE_(IAUFormDataDispManager, IAUDispManager)
{
	STDMETHOD_(BOOL ,SaveData)(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue) PURE;
	STDMETHOD_(BOOL ,LoadData)(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue) PURE;
	STDMETHOD_(BOOL ,SaveIniData)(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, const VARIANT FAR& _szValue) PURE;
	STDMETHOD_(BOOL ,LoadIniData)(LONG dwKey, LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, VARIANT FAR* _szValue) PURE;
	STDMETHOD_(BOOL ,SaveStringDataToCfg)(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue) PURE;
	STDMETHOD_(BOOL ,SaveIntDataToCfg)(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, short nValue) PURE;
	STDMETHOD_(BOOL ,LoadIntDataInCfg)(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, short nDefault, VARIANT FAR* nValue) PURE;
	STDMETHOD_(CString ,GetMapCfgFile)(LONG dwKey ) PURE;
	STDMETHOD_(CString ,GetUserDataPath)(LONG dwKey, LPCTSTR szKey) PURE;
	STDMETHOD_(short ,WritePrivateDataString)(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile) PURE;
	STDMETHOD_(CString ,GetPrivateDataString)(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile) PURE;
	STDMETHOD_(void ,WritePrivateDelete)(LONG dwKey, LPCTSTR szSection, LPCTSTR szFile) PURE;
	STDMETHOD_(CString ,LoadStringDataInCfg)(LONG dwKey, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault) PURE;
};

//  int nID = 33;
// 	IAUMainDispManager* pMng = (IAUMainDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUMainDispManager* pMng = (IAUMainDispManager*)AfxGetPctrInterface(nID);
//
// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
//
DECLARE_INTERFACE_(IAUMainDispManager, IAUDispManager)
{
	STDMETHOD_(CString, GetMapInfo)(LONG dwKey, short nFlag) PURE;
	STDMETHOD_(CString, GetUserInfo)(LONG dwKey, short nType) PURE;
	STDMETHOD_(short, MsgBox)(LONG dwKey, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags) PURE;
	STDMETHOD_(BOOL, ChangeTabProperty)(LONG dwKey, short nUnitScr, short nTabPage, short nType, short nValue) PURE;
	STDMETHOD_(void, SetGShareData)(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue, short nFlag) PURE;
	STDMETHOD_(CString, GetGShareData)(LONG dwKey, LPCTSTR szKey) PURE;
	STDMETHOD_(void, SetFrameVariable)(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue) PURE;
	STDMETHOD_(CString, GetFrameVariable)(LONG dwKey, LPCTSTR szKey) PURE;
	STDMETHOD_(BOOL, RemoveGlobalData)(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue) PURE;
	STDMETHOD_(void, CloseMain)(LONG dwKey, short nFlag) PURE;
	STDMETHOD_(void, Restart)(LONG dwKey) PURE;
	STDMETHOD_(CString, GetAcctList)(LONG dwKey) PURE;
	STDMETHOD_(void, SetOpenMapStyle)(LONG dwKey, short nType, short nInfo1, short nInfo2, short nMessageBar) PURE;
	STDMETHOD_(CString, GetGwanList)(LONG dwKey) PURE;
	STDMETHOD_(CString, GetGwanCode)(LONG dwKey, LPCTSTR szKey) PURE;
	STDMETHOD_(void, ClearMessageBar)(LONG dwKey) PURE;
	STDMETHOD_(short, ShowDialog)(LONG dwKey, LPCTSTR szKey, short nParam1, short nParam2) PURE;
	STDMETHOD_(void, NotifyEvent)(LONG dwKey, LPCTSTR szKey, LPCTSTR szData) PURE;
	STDMETHOD_(CString, GetGwanCodeOneGroup)(LONG dwKey, LPCTSTR szKey, LPCTSTR szOption) PURE;
	STDMETHOD_(void, SetAcctPasswd)(LONG dwKey, LPCTSTR szAcct, LPCTSTR szPasswd) PURE;
	STDMETHOD_(CString, GetAcctPasswd)(LONG dwKey, LPCTSTR szAcct) PURE;
	STDMETHOD_(void, SetBatchAcctInfo)(LONG dwKey, LPCTSTR szAcctInfo) PURE;
	STDMETHOD_(CString, GetBatchAcctInfo)(LONG dwKey, LPCTSTR szAcct) PURE;
	STDMETHOD_(CString, GetMasterData)(LONG dwKey, short nFlag) PURE;
	STDMETHOD_(void, SetOpenMapStyle2)(LONG dwKey, short nType, short nInfo1, short nInfo2, short nMessageBar, short nPostOpen) PURE;
	STDMETHOD_(short, IHWriteSectionData)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile) PURE;
	STDMETHOD_(CString, IHGetSectionData)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile) PURE;
	STDMETHOD_(CString, IHGetSectionNames)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szFile) PURE;
	STDMETHOD_(short, IHAddSection)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile) PURE;
	STDMETHOD_(short, IHDeleteSection)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile) PURE;
	STDMETHOD_(short, IHRenameSection)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szOldSecName, LPCTSTR szNewSecName, LPCTSTR szFile) PURE;
	STDMETHOD_(void, ChangeFrameSize)(LONG dwKey, short nFlag, short nParam1, short nParam2) PURE;
	STDMETHOD_(CString, GetFrameSize)(LONG dwKey, short nFlag) PURE;
	STDMETHOD_(void, ReportCall)(LONG dwKey, LPCTSTR szFile, long lStreamID, LPCTSTR szData) PURE;
};


//  int nID = 34;
// 	IAUFrameDispManager* pMng = (IAUFrameDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUFrameDispManager* pMng = (IAUFrameDispManager*)AfxGetPctrInterface(nID);
//
// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
//
DECLARE_INTERFACE_(IAUFrameDispManager, IAUDispManager)
{
	STDMETHOD_(short ,GetDataSaveType)(LONG dwKey) PURE;

	STDMETHOD_(CString ,GetFrameInfoFile)(LONG dwKey) PURE;

	// 현재 프레임에 세팅된 폰트의 크기를 알려준다.
	STDMETHOD_(short ,GetFontSize)(LONG dwKey) PURE;

	STDMETHOD_(CString ,GetFrameScrNum)(LONG dwKey) PURE;

	STDMETHOD_(void ,ChangeFrameSize)(LONG dwKey ,short nFlag, short nParam1, short nParam2) PURE;
};


//  int nID = 35;
// 	IAUViewDispManager* pMng = (IAUViewDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUViewDispManager* pMng = (IAUViewDispManager*)AfxGetPctrInterface(nID);
//
// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
//
DECLARE_INTERFACE_(IAUViewDispManager, IAUDispManager)
{
	STDMETHOD_(short ,ShowDialog)(LONG dwKey ,LPCTSTR szKey, short nIndex) PURE;
	STDMETHOD_(short ,MsgBoxID)(LONG dwKey ,short nID, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags) PURE;
};


//  int nID = 36;
// 	IAUPrimLibDispManager* pMng = (IAUPrimLibDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUPrimLibDispManager* pMng = (IAUPrimLibDispManager*)AfxGetPctrInterface(nID);
//
// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
//
DECLARE_INTERFACE_(IAUPrimLibDispManager, IAUDispManager)
{
	STDMETHOD_(short, IsUserDefinedMode)(LONG dwKey, LPCTSTR strMode, short nConID) PURE;
	STDMETHOD_(void, ReLoadOtherMap)(LONG dwKey, LPCTSTR szPath) PURE;
	STDMETHOD_(void, SetMessage)(LONG dwKey, LPCTSTR nId) PURE;
	STDMETHOD_(void, MapSwitch)(LONG dwKey, LPCTSTR strLR) PURE;
	STDMETHOD_(void, InsertSplit)(LONG dwKey, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPublic, LPCTSTR szGroup, LPCTSTR szSameXfmResize = _T("")) PURE;
	STDMETHOD_(void, DeleteSplit)(LONG dwKey, LPCTSTR szPath) PURE;
	STDMETHOD_(void, TabToggle)(LONG dwKey, LPCTSTR strUpDown) PURE;
	STDMETHOD_(void, DefaultSetUp)(LONG dwKey, LPCTSTR szLR, LPCTSTR szUpDown, LPCTSTR szHoga, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPath3, LPCTSTR szExt1, LPCTSTR szExt2) PURE; 
	STDMETHOD_(void, SetActiveTab)(LONG dwKey, LPCTSTR szTabID) PURE;
	STDMETHOD_(void, ChangeInputRangeResize)(LONG dwKey, LPCTSTR szChange) PURE;
	STDMETHOD_(void, ToggleWindow)(LONG dwKey, LPCTSTR strID, LPCTSTR strDIR, LPCTSTR strSUBDIR) PURE;
	STDMETHOD_(void, SplitResizeOption)(LONG dwKey, LPCTSTR strID, LPCTSTR RightDIR, LPCTSTR BottomDIR) PURE;
};


//  int nID = 37;
// 	IAUPrimScrDispManager* pMng = (IAUPrimScrDispManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUPrimScrDispManager* pMng = (IAUPrimScrDispManager*)AfxGetPctrInterface(nID);
//
// 공통적으로 dwKey값은  Advise했을 때 넘겨받은 값.
//
DECLARE_INTERFACE_(IAUPrimScrDispManager, IAUDispManager)
{
};

#endif // AFX_HANARO_PLATFORMDISPATCH_MANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
