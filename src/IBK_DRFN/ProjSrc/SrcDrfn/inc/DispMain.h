#if !defined(AFX_DISP_MAIN__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
#define AFX_DISP_MAIN__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CDispMain wrapper class

class CDispMain : public COleDispatchDriver
{
public:
	CDispMain();

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
		IIx_GetMapInfo=0,
		IIx_GetUserInfo,
		IIx_MsgBox,
		IIx_ChangeTabProperty,
		IIx_SetGShareData,
		IIx_GetGShareData,
		IIx_SetFrameVariable,
		IIx_GetFrameVariable,
		IIx_RemoveGlobalData,
		IIx_CloseMain,
		IIx_Restart,
		IIx_GetAcctList,
		IIx_SetOpenMapStyle,
		IIx_GetGwanList,
		IIx_GetGwanCode,
		IIx_ClearMessageBar,
		IIx_ShowDialog,
		IIx_NotifyEvent,
		IIx_GetGwanCodeOneGroup,
		IIx_SetAcctPasswd,
		IIx_GetAcctPasswd,
		IIx_SetBatchAcctInfo,
		IIx_GetBatchAcctInfo,
		IIx_GetMasterData,
		IIx_SetOpenMapStyle2,
		IIx_IHWriteSectionData,
		IIx_IHGetSectionData,
		IIx_IHGetSectionNames,
		IIx_IHAddSection,
		IIx_IHDeleteSection,
		IIx_IHRenameSection,
		IIx_SetChartHotkey,
		IIx_ChangeFrameSize,
		IIx_GetFrameSize,
		IIx_ReportCall
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);
//}} Dispid자동처리
	
public:
// Attributes

	
// Operations
	CString GetMapInfo(short nFlag);
	CString GetUserInfo(short nType);
	short MsgBox(LPCTSTR szMessage, LPCTSTR szCaption, short nFlags);
	BOOL ChangeTabProperty(short nUnitScr, short nTabPage, short nType, short nValue);
	void SetGShareData(LPCTSTR szKey, LPCTSTR szValue, short nFlag);
	CString GetGShareData(LPCTSTR szKey);
	void SetFrameVariable(LPCTSTR szKey, LPCTSTR szValue);
	CString GetFrameVariable(LPCTSTR szKey);
	BOOL RemoveGlobalData(LPCTSTR szKey, LPCTSTR szValue);
	void CloseMain(short nFlag);
	void Restart();
	CString GetAcctList();
	void SetOpenMapStyle(short nType, short nInfo1, short nInfo2, short nMessageBar);
	CString GetGwanList();
	CString GetGwanCode(LPCTSTR szKey);
	void ClearMessageBar();
	short ShowDialog(LPCTSTR szKey, short nParam1, short nParam2);
	void NotifyEvent(LPCTSTR szKey, LPCTSTR szData);
	CString GetGwanCodeOneGroup(LPCTSTR szKey, LPCTSTR szOption);
	void SetAcctPasswd(LPCTSTR szAcct, LPCTSTR szPasswd);
	CString GetAcctPasswd(LPCTSTR szAcct);
	void SetBatchAcctInfo(LPCTSTR szAcctInfo);
	CString GetBatchAcctInfo(LPCTSTR szAcct);
	CString GetMasterData(short nFlag);
	void SetOpenMapStyle2(short nType, short nInfo1, short nInfo2, short nMessageBar, short nPostOpen);
	short IHWriteSectionData(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile);
	CString IHGetSectionData(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile);
	CString IHGetSectionNames(LPCTSTR szGroup, LPCTSTR szFile);
	short IHAddSection(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile);
	short IHDeleteSection(LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile);
	short IHRenameSection(LPCTSTR szGroup, LPCTSTR szOldSecName, LPCTSTR szNewSecName, LPCTSTR szFile);
	void ChangeFrameSize(short nFlag, short nParam1, short nParam2);
	CString GetFrameSize(short nFlag);
	void ReportCall(LPCTSTR szFile, long lStreamID, LPCTSTR szData);
	
};

#endif // !defined(AFX_DISP_MAIN__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
