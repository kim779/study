#if !defined(AFX_DISP_DESK__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
#define AFX_DISP_DESK__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CDispDesk wrapper class

class CDispDesk : public COleDispatchDriver
{
public:
	CDispDesk();
	~CDispDesk();

//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable
//		IIx_Rows=0,

		// Method
		IIx_SendTR,
		IIx_ReceiveTR,
		IIx_SetSocketHandle,
		IIx_GetSocketHandle,
		IIx_SetSharedInterface,
		IIx_UnLoadScript,
		IIx_Blank,
		IIx_ShowTrMessage,
		IIx_ShowMessageBox,
		IIx_GetTrList,
		IIx_GetMapName,
		IIx_GetMapInfo,
		IIx_GetCtrlHwndFromName,
		IIx_GetCtrlNameFromHwnd,
		IIx_GetCtrlTypeFromHwnd,
		IIx_GetCtrlTypeFromName,
		IIx_CallFunction,
		IIx_SetParentDispatch,
		IIx_GetDispFromName,
		IIx_GetRootPath,
		IIx_GetFormTrData,
		IIx_SetFormTrData,
		IIx_FormUnLoad,
		IIx_SetOpenMapData,
		IIx_SendDataToOpener,
		IIx_CallCommand,
		IIx_GetOpenMapData,
		IIx_SetOpenMapData2,
		IIx_NotifyFromMain,
		IIx_EnableFocus,
		IIx_GotoFocus,
		IIx_SetKeyData,
		IIx_GetKeyData,
		IIx_SetGSharedNotifyFlag,
		IIx_LoadXfmEx,
		IIx_ResizeFont,
		IIx_GetMapSize,
		IIx_SetInitFont,
		IIx_SetTabStop,
		IIx_NotifyGShareData,
		IIx_SetSendCountFlag,
		IIx_DebugTrData,
		IIx_SendTrDirect,
		IIx_SetFlag,
		IIx_ChangeTRInOut,
		IIx_GetUserColor,
		IIx_GetCurrentTrMessage,
		IIx_GetCurrentMsgCode,
		IIx_SelectPacketCommand,
		IIx_SetRscMng,
		IIx_MainKeyPress,
		IIx_SetMonitoringHandle,
		IIx_SetNotifyCommand,
		IIx_AddUserScriptItem,
		IIx_SetTimeOut,

		IIx_ExcuteScript,
		IIx_GetMapData,
		IIx_SendTRForCert,
		IIx_ResizeControl,
		IIx_MoveControl,
		IIx_SetCtrlResizeFlag,
		IIx_IsDeskActive,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);
//}} Dispid자동처리
	
public:
// Attributes

	
// Operations
	short SendTR(LPCTSTR szTrName);
	short ReceiveTR(LPCTSTR szTrName, long szData, short nDataLen);
	void SetSocketHandle(LPCTSTR szSocket, long dwHandle);
	long GetSocketHandle(LPCTSTR szSocket);
	long SetSharedInterface(long dwInterface);
	void UnLoadScript();
	void Blank();
	void ShowTrMessage(LPCTSTR _szMessage);
	short ShowMessageBox(short _nType, LPCTSTR _szMessage);
	CString GetTrList();
	CString GetMapName();
	BOOL GetMapInfo(LPCTSTR _szKey, VARIANT* _vData);
	long GetCtrlHwndFromName(LPCTSTR _szCtrlName);
	CString GetCtrlNameFromHwnd(long _hWnd);
	CString GetCtrlTypeFromHwnd(long _hWnd);
	CString GetCtrlTypeFromName(LPCTSTR _szCtrlName);
	BOOL CallFunction(LPCTSTR _szFunction, LPCTSTR _szParam, short _nParamLen);
	void SetParentDispatch(LPDISPATCH _pDisp);
	LPDISPATCH GetDispFromName(LPCTSTR _szCtrlName);
	CString GetRootPath(LPCTSTR _szSubDir);
	BOOL GetFormTrData(LPCTSTR _szTr, BSTR* _szData, short* _nDataLen);
	BOOL SetFormTrData(LPCTSTR _szTr, LPCTSTR _szData, short _nDataLen);
	void FormUnLoad();
	void SetOpenMapData(short nMapType, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen, LPCTSTR aOpener);
	BOOL SendDataToOpener(LPCTSTR szData, short nDataLen);
	void CallCommand(LPCTSTR szCmd);
	CString GetOpenMapData();
	void SetOpenMapData2(long dwClipFormat);
	void NotifyFromMain(LPCTSTR szKey, LPCTSTR szData);
	BOOL EnableFocus(LPCTSTR szCtrlName, short bEnable);
	void GotoFocus(LPCTSTR szCtrlName);
	void SetKeyData(LPCTSTR szKey, long dwData, short nCount);
	long GetKeyData(LPCTSTR szKey, short* nCount);
	void SetGSharedNotifyFlag(short bNofity);
	BOOL LoadXfmEx(long dwLoadInfo);
	void ResizeFont(short nWidth, short nHeight, LPCTSTR szFontName);
	BOOL GetMapSize(VARIANT* vrWidth, VARIANT* vrHeight);
	void SetInitFont(short nWidth, short nHeight, LPCTSTR szFontName);
	void SetTabStop(short bNew);
	void NotifyGShareData(LPCTSTR szKey, LPCTSTR szValue);
	short SetSendCountFlag(short nNew);
	void DebugTrData(LPCTSTR szFileName);
	short SendTrDirect(LPCTSTR szTrName, LPCTSTR szData, short nDataLen);
	void SetFlag(LPCTSTR szKey, short szValue);
	void ChangeTRInOut(LPCTSTR szTrName, LPCTSTR szInData, LPCTSTR szOutData, short nFlag);
	unsigned long GetUserColor(short nIndex, LPCTSTR szName, short nFlag);
	CString GetCurrentTrMessage();
	CString GetCurrentMsgCode();
	short SelectPacketCommand(short nNew);
	void SetRscMng(long dwKey);
	void MainKeyPress(LPCTSTR szKey);
	void SetMonitoringHandle(long pMonitoringHandle);
	long AddUserScriptItem(LPCTSTR pstrItemName, LPDISPATCH pIDispatch);
	long ExcuteScript(LPCTSTR sVBName, LPCTSTR sEventName, LPCTSTR sParam);
	CString GetMapData();
	long SetNotifyCommand(LPCTSTR sKey);
	short SendTRForCert(LPCTSTR szTrName);
	BOOL ResizeControl(LPCTSTR strCtrlName, long nLRPlusSize, long nUDPlusSize) ;
	BOOL MoveControl(LPCTSTR strCtrlName, long nXDir, long nYDir) ;
	BOOL SetCtrlResizeFlag(LPCTSTR strCtrlName, BOOL bMove_X, BOOL bMove_Y, BOOL bResize_X, BOOL bResize_Y);
	BOOL IsDeskActive();

};


#endif // !defined(AFX_DISP_DESK__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)

