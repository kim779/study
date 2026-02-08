// Lib_ConditionMng.h : main header file for the LIB_CONDITIONMNG DLL
//

#if !defined(AFX_LIB_CONDITIONMNG_H__88F392CF_3042_4DE9_BD40_86C47F726DD9__INCLUDED_)
#define AFX_LIB_CONDITIONMNG_H__88F392CF_3042_4DE9_BD40_86C47F726DD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

/********************************************************************/
/*																	                                */
/*										조건 검색 기능 지원 Lib												*/
/*																	                                */
/*										2005. 12.  		  Doori		                      */
/*                    작성자 : 우동우																*/
/********************************************************************/

//# [지원 내역]
//# 참조 파일 => Lib_ConditionMng.dll


//# [History] => 세부 내역 각 함수 주석 참조

/////////////////////////////////////////////////////////////////////////////
// CEzFinderLib window
#include <afxtempl.h>
#import "msxml.dll"
using namespace MSXML;

#include "Condition_MngDef.h"
#include "resource.h"		// main symbols

//# [설명] : Lib 생성 및 초기화
extern "C" __declspec(dllexport) int _DLL_Open(CStringArray *psaMSTCode);

//# [설명] : Lib 소멸
extern "C" __declspec(dllexport) void _DLL_Close();

extern "C" __declspec(dllexport) void* _DLL_GetPortItem();

extern "C" __declspec(dllexport) int _DLL_Open2();



//# [설명] : 요청 타입별 조건 파일의 정보 추출
//# [입력] 
//#        int nType => 0:추천, 1:사용자, 2:Language
//#        BOOL bGetAlert => TRUE : Alert 사용 조건만, FALSE : 전체 조건
//#
//#        LPSTR &lpName => 조건 이름을 받을 주소
//#        LPSTR &lpPath => 조건 경로를 받을 주소
//#          [형식] lpName[100], lpPath[256]
//#              => FindData Folder/User/User Folder/Condition File or 
//#                 FindData Folder/Sys/DefineFilder/Condition File
//#
//#        CStringArray &saName => 조건 이름을 받을 주소
//#        CStringArray &saPath => 조건 경로를 받을 주소
//#
//#        CStringArray &saGroupName => 조건 그룹을 받을 주소
//#        CStringArray &saPath => 조건 경로를 받을 주소
//#						[형식] saPath => [Item format] Group View Name | File View Name | Condition File Path
//# 
//# [출력] 
//#       - 조건 이름/경로의 데이터 개수
extern "C" __declspec(dllexport) int _DLL_GetFind_ConditionPath(int nType, BOOL bGetAlert, LPSTR &lpName, LPSTR &lpPath);


//# [설명] : 요청 타입별 조건 파일의 정보 추출
//# [입력] 
//#        int nType => 0:추천, 1:사용자, 2:Language
//#        BOOL bGetAlert => TRUE : Alert 사용 조건만, FALSE : 전체 조건
//#
//#        CStringArray &saName => 조건 이름을 받을 주소
//#        CStringArray &saPath => 조건 경로를 받을 주소
//#						[형식] lpName[100], lpPath[256]
//#              => FindData Folder/User/User Folder/Condition File or 
//#                 FindData Folder/Sys/DefineFilder/Condition File
//#
//#
//# [출력] 
//#       - 요청 정보 성공 여부
extern "C" __declspec(dllexport) BOOL _DLL_GetFind_ConditionPath_Array(int nType, BOOL bGetAlert, 
                                                                       CStringArray &saName, CStringArray &saPath);

extern "C" __declspec(dllexport) BOOL _DLL_GetFind_CondiPath_Array(int nType, CStringArray &saName, CStringArray &saPath, CStringArray &saAlert);


//# [설명] : 요청 타입별 조건 파일의 정보 추출
//# [입력] 
//#        int nType => 0:추천, 1:사용자, 2:Language
//#        BOOL bGetAlert => TRUE : Alert 사용 조건만, FALSE : 전체 조건
//#
//#        CStringArray &saGroupName => 조건 그룹을 받을 주소
//#        CStringArray &saPath => 조건 경로를 받을 주소
//#						[형식] saPath => [Item format] Group View Name | File View Name | Condition File Path
//# 
//# [출력] 
//#       - 요청 정보 성공 여부
extern "C" __declspec(dllexport) BOOL _DLL_GetFind_GroupConditionPath(int nType, BOOL bGetAlert,
                                                                      CStringArray &saGroupName, CStringArray &saPath);


//# [설명] : 요청 List File별 조건 파일의 정보 추출
//# [입력] 
//#        CString strFilePath => List File Path
//#        BOOL bGetAlert => TRUE : Alert 사용 조건만, FALSE : 전체 조건
//#
//#        CStringArray &saGroupName => 조건 그룹을 받을 주소
//#        CStringArray &saPath => 조건 경로를 받을 주소
//#						[형식] saPath => [Item format] Group View Name | File View Name | Condition File Path
//#
//#						<Modify> 2006.11
//#						[형식] saPath => [Item format] Group View Name | File View Name | Condition File Path | Alert State
//# 
//# [출력] 
//#       - 요청 정보 성공 여부
extern "C" __declspec(dllexport) BOOL _DLL_GetOther_ConditionPath(CString strFilePath, CString strWorkFolder, BOOL bGetAlert, 
                                                                  CStringArray &saGroupName, CStringArray &saPath);

extern "C" __declspec(dllexport) BOOL _DLL_GetOther_CondiPath(CString strFilePath, CString strWorkFolder, 
															  CStringArray &saGroupName, CStringArray &saPath, CStringArray &saAlert);


//#        CString nType => 0 : 추천, 1 : 사용자, 2 : 빠른조건검색
//#        BOOL bIndustry => TRUE : 업종, FALSE : 종목
//#        BOOL bGetAlert => TRUE : Alert 사용 조건만, FALSE : 전체 조건
//#        int nGroupLockType => 1 : Lock [O] Group, 0 : Lock [X] Group, -1 : ALL Group
//#
//#        CStringArray &saGroupName => 조건 그룹을 받을 주소
//#        CStringArray &saCondiData => 조건 데이터를 받을 주소
//#						[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
extern "C" __declspec(dllexport) BOOL _DLL_GetConditionData(int nType, BOOL bIndustry, int nGroupLockType, 
                                                            CStringArray &saGroupName, CStringArray &saCondiData);

//# [설명] : 요청 조건 파일의 TR Packet 정보 추출
//# [입력] 
//#        CString strPath => 조건파일의  파일 경로
//#        int nTrIndex => Tr 구분자
//#        BOOL bGetIndustry => TRUE : 업종 Packet, FALSE : 종목 Packet
//#        BOOL bDefultResultField => TRUE : 기본 결과 필드, FALSE : 값보기 필드
//#        BOOL bOrdering_Apply => TRUE : Ordering O, FALSE : Ordering X
//# 
//#        LPSTR &lpPacket => Packet Struct
//#        int &nOrderCnt => 순차 지표의 개수 
//#
//# [출력]
//#     Packet 의 크기
extern "C" __declspec(dllexport) int _DLL_GetFind_Packet(CString strPath, int nTrIndex, BOOL bGetIndustry, BOOL bDefultResultField, 
														 BOOL bOrdering_Apply, LPSTR &lpPacket, int &nOrderCnt);

extern "C" __declspec(dllexport) int _DLL_GetFind_GroupPacket(CStringArray *psaPath, LPSTR &lpPacket);


//# [설명] : TR Packet's Byte Ordering!
//# [입력] 
//#        User_Finder의 구조체 포인터
//# [History]
//#					- 2005.08.18 추가
extern "C" __declspec(dllexport) void _DLL_GetFind_Packet_ByteOrdering(User_Finder *pstUserFinder);


//# [설명] : 결과 내역의 편집
//# [입력]
//#       LPVOID	aTRData => 통신에서 받은 데이터 
//#       long dwTRDateLen => 통신에서 받은 데이터의 길이
//#				CStringArray &saColData => COL 데이터명을 받을 주소
//#       CStringArray &saResultData => COL의 실데이터를 받을 주소
//#       [추가] 2005.08.12
//#         BOOL bDefultResultField => TRUE : 고정 필드 타입, FALSE : 값보기 필드 타입
//#          : ReaultData 형식
//#            - TRUE : 대비부호/장구분/종목코드/종목명/현재가/전일대비/등락율/거래량 
//#                     => 고정 필드 정보 얻어 오는 함수 : _DLL_GetFind_FixFieldInfo
//#            - FALSE : 대비부호/종목코드/종목명/값보기필드값(n개)
//# 
//# [출력]
//#				데이터 추출 성공 여부.
extern "C" __declspec(dllexport) BOOL _DLL_GetFind_Result(LPVOID	aTRData, long dwTRDateLen, 
                                                          CStringArray &saColData, CStringArray &saResultData, 
                                                          BOOL bDefultResultField = FALSE);


//# [설명] : 고정 필드 정보 추출
extern "C" __declspec(dllexport) void _DLL_GetFind_FixFieldInfo(CStringArray &saColName, CUIntArray &iaColUnit);


//# Filter!
extern "C" __declspec(dllexport) BOOL _DLL_GetFilter(CString strFullPath, long* plFilterInfo);
extern "C" __declspec(dllexport) void _DLL_GetFilter_Base(long* plFilterInfo);
extern "C" __declspec(dllexport) int _DLL_GetFilterToPacket(CString strFullPath, long *plPacket);
extern "C" __declspec(dllexport) BOOL _DLL_SetFilter(CString strFullPath, long* plFilterInfo);
// 2009.10.27 -hidden-
extern "C" __declspec(dllexport) void _DLL_GetFromatText(CString strFullPath, CString& strText);

//# Logic Calculate!
extern "C" __declspec(dllexport) BOOL _DLL_SetFree_PtrArray(CPtrArray *paData);
extern "C" __declspec(dllexport) BOOL _DLL_GetLogicCalculate(char *psData, CPtrArray *paBitSet, char **psResult);

extern "C" __declspec(dllexport) void _DLL_SetUserFolderPath(CString strUserFolder);

/////////////////////////////////////////////////////////////////////////////
// CLib_ConditionMngApp
// See Lib_ConditionMng.cpp for the implementation of this class
//

class CLogicCalculate;
class CLib_ConditionMngApp : public CWinApp
{
public:
	CLib_ConditionMngApp();
	
	CStringArray m_saPortItem;
	CStringArray m_saMSTCode;
	CLogicCalculate   *m_pLogicCalculate;
	
	CString m_strLoadFilePath;
	int m_nOrderCnt;
	User_Finder	m_stUser_Finder;
	
	CList<ConditionGroupInfo, ConditionGroupInfo&> m_listConditionGroup;

	CString		m_strUserFolder;

private:
	CStringArray	m_saPortGroup;
	BOOL					m_bPortOpt;
	BOOL					m_bPortChk;
	
	CList<OneElementInfo, OneElementInfo&>      m_listOneElement;
	CList<GroupElementInfo, GroupElementInfo&>  m_listGroupElement;
	
	CMapStringToPtr	*m_pmapFieldInfo;
	void GetLoad_FieldInfo();
	BOOL LoadFiledXML(CString strFullFilePath);
	void SetClear_FieldInfo();
	void GetMoveFieldIDToInfo(CString strHeadID, HeadMoveInfo  &stHeadMoveInfo);
	
	//## System function START ##
	BOOL m_bLoadFile_User, m_bLoadFile_Sys;
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> m_listUserInfoFolder;
	CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> m_listSysInfoFolder;
	
	// CLinkDataManager	m_LinkDataManager;
	
public:
	BOOL GetFind_ConditionPath(int nType, BOOL bGetAlert, CStringArray &saName, CStringArray &saPath, CStringArray &saAlert);
	BOOL GetFind_GroupConditionPath(int nType, BOOL bGetAlert, CStringArray &saGroupName, CStringArray &saPath);
	BOOL GetOther_ConditionPath(CString strFilePath, CString strWorkFolder, BOOL bGetAlert, 
		CStringArray &saGroupName, CStringArray &saPath, CStringArray &saAlert);
	BOOL GetConditionData(int nType, BOOL bIndustry, BOOL bJustAlert, int nJustLockGroup,
							 CStringArray &saGroupName, CStringArray &saCondiData);
	BOOL GetFind_LoadListFile(int nType, BOOL bIndustry, CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistInfoFolder);
	BOOL GetFind_Packet(CString strPath, int nTrIndex, BOOL bDefultResultField);
	void SetByteOrderRevDouble(LPSTR pDouble);
	void SetByteOrdering_Out(int nFieldCnt, OutListPacket *pstOutListPacket);
	
	CString GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	CString GetFilePath(CString strLoadType, BOOL bIndustry = FALSE);
	CString GetFolderPath(CString strLoadType, BOOL bIndustry = FALSE);
	
	//# Filter
	BOOL GetLoad_FilterData(CString strFullPath, FilterInfo *pstFilterInfo);
	BOOL SetSave_FilterData(CString strFullPath, FilterInfo *pstFilterInfo);
	void SetDefult_FilterInfo(FilterInfo *pstFilterInfo);
	void SetConvert_FilterToPacket(FilterInfo *pstFilterInfo, User_Finder	&stUser_Finder);
	CString GetAppFileRoot();
	void SetUserFolder(CString strUserFolder);
	
private:
	//	CString GetAppFileRoot();
	
	void SetInitVal();
	BOOL SetMakePacket(BOOL bDefultResultField, User_Finder &stUser_Finder, int &nOrderCnt);
	BOOL SetTargetBit(CString strLogic, char *psResultData);    //# Add : 2006.10
	
	int GetMaxGroup();
	int GetGroupToConditionCnt(int nGroup, BOOL bAll = TRUE);
	CString GetElementInfo(int nGroup, int nConditionIndex);
	BOOL GetElementToOneConditionInfo(CString strElement, OneConditionInfo  &stOneConditionInfo);
	
	void SetMakePacket_Field(int nMoveFieldCnt, User_Finder	&stUser_Finder);
	
	BOOL GetLoad_ConditionData(CString strFullFilePath);
	
	void GetString_LogicOne(CString &strLogic);
	void GetString_LogicGroup(CString &strGroupLogic);
	
	void SetUser_Define_BaseFilter(User_Define &stUser_Define);
	
	void SetSendMainMsg(CString strMsg);
	
	//## System function START ##
	void GetLoad_ConditionInfo();
	void SetDeleteFolderList(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	
	BOOL SetLoadTreeXml(CString strType);
	BOOL SetSaveTreeXml(CString strType, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	
	void SetJobFolder();
	void SyncTreeListFile(CString strFilePath);
	void GetPathToList(CString strRootFolderPath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	void GetTempCondition(CString strRootFolderPath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	BOOL GetXMLToList(CString strFilePath, CString strType, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	void ProcessDeleteFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList);
	void ProcessInsertFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
		CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList);
	void ProcessDeleteFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList);
	void ProcessInsertFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList);
	
	BOOL GetFindFile(CString strFilePath, CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList);
	BOOL GetFindFile(CString strFolderPath, CString strFilePath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	BOOL GetFindFolder(CString strFolderPath, CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList);
	BOOL GetFileSetState(CString strType, CString strFolderPath, CString strFilePath, CString &strAlertReg, BOOL &bUnitCan);				
	POSITION GetPosFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, CString strFolderPath);
	POSITION GetPosFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList, CString strFolderPath, CString strFilePath);

public:
	
// 	CInterestApiHelper m_intrMngr;
// 	CWrpMainApi		m_MainAPI;
	
	void GetGwansimItem(CString strKey, CStringArray& arrCode, CStringArray& arrName);
	CString GetGwanKeyFromName(LPCSTR szGwanGroupName);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLib_ConditionMngApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(CLib_ConditionMngApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIB_CONDITIONMNG_H__88F392CF_3042_4DE9_BD40_86C47F726DD9__INCLUDED_)
