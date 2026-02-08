// Lib_SignalMng.h : main header file for the LIB_SIGNALMNG DLL
//

#if !defined(AFX_LIB_SIGNALMNG_H__7316E737_B404_4F93_811F_52ED6647497E__INCLUDED_)
#define AFX_LIB_SIGNALMNG_H__7316E737_B404_4F93_811F_52ED6647497E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/****************************************************************/
/*                                                              */
/*						 신호 메니저 (SignalMng.dll)									    */
/*																															*/
/*                   2005. 12.  		Doori												*/
/*                   작성자 : 우동우(Danny)										  */
/****************************************************************/

//# Signal Mng 관련!
#include "Signal_PacketDef.h"
#include "Signal_MngDef.h"

//# Open / Close! => 반드시 Open 후 Colse!
extern "C" __declspec(dllexport) CWnd* _DLL_SetLibOpen(CWnd* pwndMain, HINSTANCE hInstance, CWnd* pWndModuleWnd);
extern "C" __declspec(dllexport) void _DLL_SetLibClose(CWnd* pWndTarget);

//# Signal Rev Reg!
extern "C" __declspec(dllexport) int _DLL_SetSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType, CString sPBID = "", CString strKey = "*");

//# 신호 구분 테이블
//#  CString strType                Data 형태
//   - TYPE_TABLE_SYSTEM    ----->  (Key | 조건명)
//   - TYPE_TABLE_NEWS      ----->  (Key | 제공처명)
//   - TYPE_TABLE_PORT      ----->  (Key | 조건명)
//# CStringArray &saSignalTable
//   : Type에 따른 DATA! 
//# Return Values => Table Cnt!
extern "C" __declspec(dllexport) BOOL _DLL_GetSiganlTable(CString strType, CStringArray &saSignalTable);

//# 신호 설정 관련 함수!
extern "C" __declspec(dllexport) void _DLL_GetSignalData(CString strType, CMapStringToPtr &mapSignalSet);
extern "C" __declspec(dllexport) void _DLL_SetSignalData(CString strType, CMapStringToPtr *pmapSignalSet);

extern "C" __declspec(dllexport) void _DLL_GetFilterData(long &stFilterInfo);
//extern "C" __declspec(dllexport) void _DLL_GetFilterData(long &stFilterInfo, BOOL bDefultVal = FALSE);
extern "C" __declspec(dllexport) void _DLL_SetFilterData(long *pstFilterInfo);

// 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
extern "C" __declspec(dllexport) void _DLL_SetDefSysSignalData(CMapStringToPtr *pmapSignalSet);

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLib_SignalMngApp
// See Lib_SignalMng.cpp for the implementation of this class
//
class CCommApiHelper;
class CDummyWnd;

class CLib_SignalMngApp : public CWinApp
{
public:	
	CLib_SignalMngApp();
	CRITICAL_SECTION m_CriticalSection;
	
	CDummyWnd *m_pwndDummy;
	
	BOOL m_bLibOpen;
	CStringArray m_saNewsKeyTable;
	CStringArray m_saSysKeyTable;
	CStringArray m_saPortKeyTable;
	
	CMapStringToPtr	*m_pmapSignalSet_News;
	CMapStringToPtr	*m_pmapSignalSet_Sys;
	FilterInfo *m_pstFilterInfo;
	
	CString m_strAppPath, m_strUserPath;
	CString m_strUserId;
	
	CString m_strRegSys_Code;
	int m_nRegSys_Cnt;
	
	//# Debug!
	BOOL m_bTrace, m_bTrace_Filter, m_bAdvise_SYS, m_bSend_DRDS;
	
	CMapStringToOb  *m_pmapAlert_Sys;    //# Key + CWnd
	CMapStringToOb  *m_pmapAlert_Sigal;  //# Key + CWnd
	CMapStringToOb  *m_pmapNews_Signal;  //# Key + CWnd
	CMapStringToOb  *m_pmapNews_Real;    //# Key + CWnd

	CCommApiHelper*	m_pCommApiHelper;
	int				m_nConnectID;

	BOOL SetInit_AdvTable();
	void SetClear_AdvTable(CWnd* pTargetWnd);
	
	CMapStringToOb *m_pmapCondition;
	void SetClear_ConditionTable();
	
	CString m_sPBID;
	CString m_sDelTRID;

	CWnd* m_pWndModuleMain;
	int   m_nLoadWndCnt;

private:
	void SetClear_AdvTable(int nRealType, CWnd* pTargetWnd);
	BOOL SetAdd_AdvTable(CWnd *pWnd, int nRealType, CString strKey);
	BOOL SetDelete_AdvTable(CWnd *pWnd, int nRealType, CString strKey);
	
public : 
	CString GetAppFileRoot();
	BOOL SetAdvData_Sys(BOOL bReg);
	BOOL SetAdvData_News(BOOL bReg);
	BOOL SetAdvData_Alert(BOOL bReg, CString strKey);

	BOOL GetLoad_SignalSet();
	BOOL SetSave_SignalSet();
	
	static CString GetSignalDataToKey(SetSignalData	*pstSetSignalData);
	
	static BOOL SetSave_SignalData(CString sFullPathName, CMapStringToPtr *pmapSignalSet);
	static BOOL GetLoad_SinalData(CString sFullPathName, CMapStringToPtr *pmapSignalSet);
	static void SetSerialize(CArchive& ar, int nVersion, SetSignalData *pstSetSignalData);
	void SetClear_SignalSet(CMapStringToPtr *pmapSignalSet);
	
	BOOL GetLoad_FilterData(CString sFullPathName, FilterInfo *pstFilterInfo);
	BOOL SetSave_FilterData(CString sFullPathName, FilterInfo *pstFilterInfo);
	void SetAttach_FilterInfo(FilterInfo *pstFilterInfo_Base, FilterInfo *pstFilterInfo_Target);
	void SetDefult_FilterInfo(FilterInfo *pstFilterInfo);
	
	void SetCopy_SetSignalData(SetSignalData *pstSetSignalData, SetSignalData *pstSetSignalData_Target);
	
	int GetRealData(int nSignalType, LPSTR sRealData, int nLen, CString sPBID);
	BOOL SetRealData(int nRealType, CString strKey, LPSTR sRealData, CString sPBID = _T(""));
	
	//유진 추가 메인 윈도우 핸들 체크
	BOOL IsMainExeHandle(HWND hTarget);
	
	
	//# Add : Danny(2006.10)
	int SetSiganl(BOOL bAdvise, CWnd* pWnd, CString strType, CString sPBID = "", CString strKey = "*");
	
	//# Add : Danny(2006.10)
	BOOL GetRealData_News(LPSTR &sRealData, int nLen);
	
	//# Filter!
	BOOL GetCheck_News(LPSTR &sRealData, int nLen);
	BOOL GetCheck_News(Signal_NewsInfo stNewsInfo);
	
	BOOL GetCheck_CaseCodeToPoint(CString strCaseCodeList, SIGANL_NEWS *pstNews);
	
	BOOL GetCheck_Keyword(BOOL bNewsProffer, CString strTitle, CString strBody,
		SIGANL_NEWS *pstNews, int &nCompomentType);
	
	int GetCheck_Condition(LPSTR &sRealData, int nLen, CString sPBID, BOOL bUseFilter = TRUE);
	
	BOOL GetCheck_Filter(FilterInfo *pstFilterInfo, 
		CString strTime, CString strCode, Signal_CommonInfo stCommonInfo);
	
	BOOL GetConvertPacket_News(CString strType, Signal_NewsInfo stNewsInfo, SIGANL_NEWS &stNews);
	//# Add : Danny(2006.10)
	BOOL GetConvertPacket_Condition(int nSignalType, CString strKey, Signal_ConditionInfo	stConditionInfo,
		SIGNAL_CONDITION	&stCondition);
	
	void GetPointToImgIndex(int nSignalType, int nPoint, char *psSignalImageIndex);
	
	
	void GetLoad_PortData(CString strPortGroup, CStringArray *psaInCode);
	CString GetFormat_Port(CString& strData);
	
	//# Debug!
	void SetWriteDebug(CString strPos, LPCTSTR strData);
	void SetLoad_DebugInfo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLib_SignalMngApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLib_SignalMngApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIB_SIGNALMNG_H__7316E737_B404_4F93_811F_52ED6647497E__INCLUDED_)
