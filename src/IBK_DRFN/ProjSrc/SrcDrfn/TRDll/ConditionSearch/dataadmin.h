// DataAdmin.h: interface for the CDataAdmin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAADMIN_H__D5BE3C36_B850_48BD_B070_C8D561A47F8E__INCLUDED_)
#define AFX_DATAADMIN_H__D5BE3C36_B850_48BD_B070_C8D561A47F8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Use XML
#import "msxml.dll"
using namespace MSXML;

#include <afxtempl.h>   //<= CList

class CDataAdmin  
{
public:
	CDataAdmin();
	virtual ~CDataAdmin();
	
public:
	//## Condition 관련 values ##
	CList<ConditionGroupInfo, ConditionGroupInfo&> m_listConditionGroup;

	// 2009.11.10 -hidden-
	ArrGroups m_arrGroups;
	
	//## Fields ##
	VARIANT_BOOL m_bMoveSysLoadedXMLFile;
	VARIANT_BOOL m_bFixSysLoadedXMLFile;
	
	MSXML::IXMLDOMDocumentPtr m_pMoveSysDOM;   //# Field ID : 5XXX
	MSXML::IXMLDOMDocumentPtr m_pFixSysDOM;    //# Field ID : 9XXX

	CWnd* m_pMainWnd;
	
public:
	//## Condition 관련 Function ##
	BOOL AddCondition(int nGroup, int nElementIndex, OneConditionInfo  stOneConditionInfo,	
						CString strFieldID, BOOL bResultViewDefVal = FALSE, BOOL bLoadFlag = FALSE);

	BOOL InsertCondition(int nGroup,				
						CString strElement,
						int nElementIndex,			//## Condition Index
						OneConditionInfo  stOneConditionInfo,	//## One Condition Info
						CString			  strFieldID,
						BOOL	bResultViewDefVal = FALSE,
						BOOL			  bLoadFlag = FALSE);

	BOOL ModifyCondition(CString strElement,
												OneConditionInfo  stOneConditionInfo,	//## One Condition Info
												CString			  strFieldID,
												BOOL	bResultViewDefVal = FALSE,
												BOOL	bLoadFlag = FALSE);
		
	BOOL ModifyOneCondition(CString strElement, OneConditionInfo stOneConditionInfo);
	BOOL ModifyConditionUserDefine(CString strElement, User_Define stCondition,
		                              CString strConditionText);

	BOOL ModifyGroupMoveFieldCheck(int nGroupNo, BOOL bView);
	BOOL ModifyConditionMoveFieldCheck(CString strElement, BOOL bView);

	BOOL ModifyConditionNotCheck(CString strElement, int nNot);
	BOOL ModifyConditionCheck(CString strElement, BOOL bView);
	
	BOOL ModifyConditionTargetBase(int nGroup, int nTargetIndex, int nOutType = -1);
	BOOL ModifyOneConditionTargetBase(CString strElement, int nTargetIndex);

	BOOL ModifyConditionTerm(int nGroup, int nTerm, CString strCase);
	BOOL ModifyConditionTerm(int nGroup, int nSTerm, int nETerm);
	
	BOOL ModifyConditionInsertGroup(int nGroup);
	BOOL ModifyConditionDeleteGroup(int nGroup);
	
	BOOL ModifyConditionLogicOpt(CStringArray *psaLogicInfoData);
	
	// 2009.11.10 -hidden-
	BOOL ModifyConditionInsertUpGroup(int nIndex, int nGroup);
	BOOL ModifyConditionInsertDownGroup(int nIndex, int nGroup);

	void DeleteOneCondition(CString strElement);
	void DeleteGroup(int nGroup);

// 2009.11.13 -hidden-
//	void DeleteAll() { m_listConditionGroup.RemoveAll(); };
	void DeleteAll();

	void MakeGroupStruct(int nIndex, int nGroup);
	
	int GetConditionCnt();
	int GetLanguageConditionCnt();
	int GetConditionViewCnt();
	int GetGroupToConditionCnt(int nGroup, BOOL bException = TRUE);
	int GetMaxGroup();
	int GetGroupCnt();
	int GetMoveFieldTotCnt(BOOL bCommonGroup = TRUE);

	void GetAllConditionName(CStringArray* psaConditionName);

//	POSITION GetElementToPosition(CString strElement);
	BOOL GetElementToPosition(CString strElement, OneConditionInfo& stCondition);		// 2009.11.11 -hidden-
	BOOL GetElementToGroup(CString strElement, ConditionGroupInfo& stConditionGroup);	// 2009.11.11 -hidden-
	
	CString GetElementToTagID(CString strElement);	//#<= Not User
	BOOL GetElementToTagInfo(CString strElement, CString &strTag, CString &strMapFile);
	
	CString GetTagIDToElement(CString strTag);
	
	BOOL GetElementToUser_Define(CString strElement, User_Define &stUser_define);
	BOOL GetElementToOneConditionInfo(CString strElement, OneConditionInfo  &stOneConditionInfo);
	
	BOOL SetDataGroupSort();
	int SetElementStandLine();
	
	void LoadMoveUserFiledXML();
	void LoadMoveSysFiledXML();
	void LoadFixSysFiledXML();
	BOOL GetMoveFieldIDToInfo(CString strHeadID, HeadMoveInfo  &stHeadMoveInfo);
	BOOL GetMoveSysFieldIDToInfo(CString ExtractStr, HeadInfoType &stHeadType);
	
	BOOL CheckCondiGubunState(CString strTagID);
	


	BOOL GetNonAlertCondiHave(CStringArray *psaElement);
	void SetNonAlertCondiDelete();


	BOOL GetOnlyTargetCondition();


	int GetMinTargetBase();



	void DebugInputDataView();
	void DebugMoveFiledInfo();
	BOOL ModifyConditionSwitch(CString strElement, CString strElementAfter);

	// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 >> 
	void SetChangeElement(CString strElementOrigin, CString strElementChange);

	void SetMainWnd(CWnd* pMainwnd);
};

#endif // !defined(AFX_DATAADMIN_H__D5BE3C36_B850_48BD_B070_C8D561A47F8E__INCLUDED_)
