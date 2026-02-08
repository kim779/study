// DataAdmin.cpp: implementation of the CDataAdmin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataAdmin.h"
#include "MainFrmWnd.h"

#include "Winuser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataAdmin::CDataAdmin()
{
	//## Field Info File Create(Load) ##
// 	LoadMoveSysFiledXML();
// 	LoadFixSysFiledXML();

	m_pMainWnd = NULL;
}

CDataAdmin::~CDataAdmin()
{
	//	m_listConditionGroup.RemoveAll();
	
	DeleteAll();
}

void CDataAdmin::SetMainWnd(CWnd* pMainWnd)
{
	m_pMainWnd = pMainWnd;

	LoadMoveSysFiledXML();
	LoadFixSysFiledXML();
}

// 2009.11.11 -hidden-
void CDataAdmin::DeleteAll()
{
	int nCondSize = 0;
	int nSize = m_arrGroups.GetSize();
	
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		for(int j = 0; j < nCondSize; ++j)
		{
			pGroup->arrConGrpInfo.RemoveAll();
			delete pGroup;
		}
	}
	
	m_arrGroups.RemoveAll();
}

// 2009.11.13 -hidden-
void CDataAdmin::MakeGroupStruct(int nIndex, int nGroup)
{
	if(nIndex < 0)
		return;
	
	int nGroups = m_arrGroups.GetSize();
	GROUP_INFO* pGroupInfo = NULL;
	
	for(int i = 0; i < nGroups; ++i)
	{
		pGroupInfo = m_arrGroups.GetAt(i);
		if(pGroupInfo->nGroup == nGroup)
			return;
	}
	
	pGroupInfo = new GROUP_INFO;
	pGroupInfo->nGroup = nGroup;
	
	if(!m_arrGroups.GetSize())
	{
		m_arrGroups.Add(pGroupInfo);
		return;
	}
	
	if(nGroup == 0)
		m_arrGroups.InsertAt(0, pGroupInfo);
	else if(1 == nIndex)
	{
		if(!m_arrGroups.GetSize())
			m_arrGroups.Add(pGroupInfo);
		else
		{
			if(0 == m_arrGroups.GetAt(0)->nGroup)
				m_arrGroups.InsertAt(nIndex, pGroupInfo);
			else
				m_arrGroups.InsertAt(nIndex - 1, pGroupInfo);
		}
	}
	else
		m_arrGroups.InsertAt(nIndex, pGroupInfo);
	/*m_arrGroups.InsertAt(nIndex - 1, pGroupInfo);*/
}

void CDataAdmin::LoadMoveSysFiledXML()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pMainWnd;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));
	
	CString strIndexFilePath;
	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + MOVEFIELD_SYS_FILE;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	m_bMoveSysLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	m_pMoveSysDOM = pDOMConditionIndex;
}

void CDataAdmin::LoadFixSysFiledXML()
{
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)m_pMainWnd;
	CString strAppPath;
	strAppPath.Format("%s", pwndMainFrm->Variant(homeCC));
	
	CString strIndexFilePath;
	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + FIXFIELD_SYS_FILE;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	m_bFixSysLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));
	m_pFixSysDOM = pDOMConditionIndex;
}


BOOL CDataAdmin::GetMoveFieldIDToInfo(CString strHeadID, HeadMoveInfo  &stHeadMoveInfo)
{
	BOOL bResult = TRUE;
	BOOL bHaveFieldID = FALSE;
	HeadInfoType      stHeadType;
	
	MSXML::IXMLDOMNodePtr		      pNIndex;		//# Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;	//# Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   //# Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	if( m_bMoveSysLoadedXMLFile == VARIANT_FALSE || 
		m_bFixSysLoadedXMLFile == VARIANT_FALSE ) 
	{
		bResult = FALSE;
		return bResult;
	}
	
	int nColCount = 0;
	int nFindPos = 0;
	CString tmpStr = strHeadID;
	CString ExtractStr;
	
	if (!tmpStr.IsEmpty()) 
	{
		while (nFindPos >= 0) 
		{
			nFindPos = tmpStr.Find("/");
			if (nFindPos < 0)
				break;
			ExtractStr = tmpStr.Mid(0, nFindPos);
			tmpStr = tmpStr.Mid(nFindPos + 1, tmpStr.GetLength());
			
			try {
				if (ExtractStr.Left(1).Compare("5") == 0) 
					pNIndex = m_pMoveSysDOM->selectSingleNode("INDEX");
				else if (ExtractStr.Left(1).Compare("9")== 0) 
					pNIndex = m_pFixSysDOM->selectSingleNode("INDEX");
				
				pNS_FItem  = pNIndex->selectNodes("FITEM");
				
				int nCount, nLength;
				CString strTemp;
				nLength = pNS_FItem->length;
				for (nCount = 0; nCount < nLength; nCount++)
				{
					pN_FItem = pNS_FItem->item[nCount];	
					
					MSXML::IXMLDOMNodePtr	pNAttributeTemp;
					pNA_FItem= pN_FItem->attributes;
					
					pNAttributeTemp = pNA_FItem->getNamedItem("ID");
					strTemp = CString((PCHAR)pNAttributeTemp->text);
					
					if (strTemp.Compare(ExtractStr) == 0)
					{
						pNAttributeTemp = pNA_FItem->getNamedItem("ID");
						strTemp = CString((PCHAR)pNAttributeTemp->text);
						stHeadType.lHeadID = atol(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
						stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
						strTemp = CString((PCHAR)pNAttributeTemp->text);
						stHeadType.nUnitNum = atoi(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
						stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
						stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
						strTemp = CString((PCHAR)pNAttributeTemp->text); 
						stHeadType.nDataLength = atoi(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
						strTemp = CString((PCHAR)pNAttributeTemp->text);
						stHeadType.nColWidth = atoi(strTemp);
						
						pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
						stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
						
						stHeadMoveInfo.stHeadType[nColCount] = stHeadType;
						
						bHaveFieldID = TRUE;
						break;
					}
				}
			}
			catch (_com_error e)
			{
				bResult = FALSE;
				return bResult;
			}
			
			/*
			//## Not Have Move Field => Search from Move System File ##
			if ((ExtractStr.Left(1).Compare("5") == 0) && (bHaveFieldID == FALSE))
			{
			bHaveFieldID = GetMoveSysFieldIDToInfo(ExtractStr, stHeadType);
			stHeadMoveInfo.stHeadType[nColCount] = stHeadType;
			}
			*/
			
			nColCount++;
			bHaveFieldID = FALSE;
		}
	}
	
	stHeadMoveInfo.nColCnt = nColCount;
	
	return bResult;
}


BOOL CDataAdmin::GetMoveSysFieldIDToInfo(CString ExtractStr, HeadInfoType &stHeadType)
{
	BOOL bHaveFieldID = FALSE;
	
	MSXML::IXMLDOMNodePtr		      pNIndex;		//# Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;		//# Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   //# Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;
	
	try {
		pNIndex = m_pMoveSysDOM->selectSingleNode("INDEX");
		
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		int nCount, nLength;
		CString strTemp;
		nLength = pNS_FItem->length;
		for(nCount = 0; nCount < nLength; nCount++)
		{
			pN_FItem = pNS_FItem->item[nCount];	
			
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			
			if (strTemp.Compare(ExtractStr) == 0)
			{
				pNAttributeTemp = pNA_FItem->getNamedItem("ID");
				strTemp = CString((PCHAR)pNAttributeTemp->text);
				stHeadType.lHeadID = atol(strTemp);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
				stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
				strTemp = CString((PCHAR)pNAttributeTemp->text);
				stHeadType.nUnitNum = atoi(strTemp);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
				stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
				stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
				strTemp = CString((PCHAR)pNAttributeTemp->text); 
				stHeadType.nDataLength = atoi(strTemp);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
				strTemp = CString((PCHAR)pNAttributeTemp->text);
				stHeadType.nColWidth = atoi(strTemp);
				
				pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
				stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
				
				bHaveFieldID = TRUE;
				break;
			}
		}
	}
	catch (_com_error e)
	{
		bHaveFieldID = FALSE;
	}
	
	return bHaveFieldID;
}


BOOL CDataAdmin::AddCondition(int nGroup, int nElementIndex, OneConditionInfo  stOneConditionInfo, CString strFieldID,
							  BOOL bResultViewDefVal, BOOL bLoadFlag)
{
	strFieldID.TrimRight(" ");
	strFieldID.TrimLeft(" ");
	
	ConditionGroupInfo  stConditionGroupInfo;
	
	stConditionGroupInfo.nGroup = nGroup;
	
	stConditionGroupInfo.strElement = CONDITION_INDEX[nElementIndex];
	
	stConditionGroupInfo.stOneConditionInfo = stOneConditionInfo;
	
	stConditionGroupInfo.stOneConditionInfo.stCondition.GroupNo = nGroup;
	
	if (strFieldID.IsEmpty() == FALSE)
	{
		//# 값보기 정보 
		if (GetMoveFieldIDToInfo(strFieldID, stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo) == FALSE)
			return FALSE;
		else
		{
			if (bLoadFlag == FALSE)
			{
				int nValViewCnt = stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = nValViewCnt;
				
				stConditionGroupInfo.stOneConditionInfo.bFieldState = (nValViewCnt > 0) ? TRUE : FALSE;    
				stConditionGroupInfo.stOneConditionInfo.bFieldEnable = !stConditionGroupInfo.stOneConditionInfo.bFalg;
			}
		}
	}
	else
	{
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt = 0;
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = FALSE;
		
		stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = 0;
		
		stConditionGroupInfo.stOneConditionInfo.bFieldState = FALSE;    
		stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
	}
	stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.strElement = CONDITION_INDEX[nElementIndex];
	
	//# 값보기
	stConditionGroupInfo.stOneConditionInfo.bFieldVal = bResultViewDefVal;          
	stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bResultViewDefVal;
	
	//	m_listConditionGroup.AddTail(stConditionGroupInfo);
	
	// 2009.11.10 -hidden-
	int nGroups = m_arrGroups.GetSize();
	GROUP_INFO* pGroupInfo = NULL;
	
	for(int i = 0; i < nGroups; ++i)
	{
		pGroupInfo = m_arrGroups.GetAt(i);
		if(pGroupInfo->nGroup == nGroup)
		{
			pGroupInfo->arrConGrpInfo.Add(stConditionGroupInfo);
			return TRUE;
		}
	}
	
	pGroupInfo = new GROUP_INFO;
	pGroupInfo->nGroup = nGroup;
	pGroupInfo->arrConGrpInfo.Add(stConditionGroupInfo);
	
	if(nGroup == 0)
		m_arrGroups.InsertAt(0, pGroupInfo);
	else
		m_arrGroups.Add(pGroupInfo);
	
#ifdef _DEBUG
	DebugInputDataView();
#endif
	
	return TRUE;
}


BOOL CDataAdmin::InsertCondition(int nGroup, CString strElement, int nElementIndex,	OneConditionInfo  stOneConditionInfo, 
								 CString strFieldID, BOOL	bResultViewDefVal, 	BOOL bLoadFlag)
{
	strFieldID.TrimRight(" ");
	strFieldID.TrimLeft(" ");
	
	ConditionGroupInfo  stConditionGroupInfo;
	
	stConditionGroupInfo.nGroup = nGroup;
	stConditionGroupInfo.strElement = CONDITION_INDEX[nElementIndex];
	stConditionGroupInfo.stOneConditionInfo = stOneConditionInfo;
	
	stConditionGroupInfo.stOneConditionInfo.stCondition.GroupNo = nGroup;
	
	if (strFieldID.IsEmpty() == FALSE)
	{
		//# 값보기 정보 
		if (GetMoveFieldIDToInfo(strFieldID, stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo) == FALSE)
			return FALSE;
		else
		{
			if (bLoadFlag == FALSE)
			{
				int nValViewCnt = stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = nValViewCnt;
				
				stConditionGroupInfo.stOneConditionInfo.bFieldState = (nValViewCnt > 0) ? TRUE : FALSE;    
				stConditionGroupInfo.stOneConditionInfo.bFieldEnable = !stConditionGroupInfo.stOneConditionInfo.bFalg;
			}
		}
	}
	else
	{
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt = 0;
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = FALSE;
		
		stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = 0;
		
		stConditionGroupInfo.stOneConditionInfo.bFieldState = FALSE;    
		stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
	}
	stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.strElement = CONDITION_INDEX[nElementIndex];
	
	//# 값보기
	stConditionGroupInfo.stOneConditionInfo.bFieldVal = bResultViewDefVal;          
	stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bResultViewDefVal;
	
	// 2009.11.11- hidden-	
	//	POSITION pos = GetElementToPosition(strElement);
	//	m_listConditionGroup.InsertBefore(pos, stConditionGroupInfo);
	
#ifdef _DEBUG
	DebugInputDataView();
#endif
	
	return TRUE;
}


BOOL CDataAdmin::ModifyCondition(CString strElement, OneConditionInfo  stOneConditionInfo, CString strFieldID, 
								 BOOL	bResultViewDefVal,/* = FALSE*/ BOOL	bLoadFlag /*= FALSE*/)
{
/*	POSITION pos;
pos =	GetElementToPosition(strElement);

  strFieldID.TrimRight(" ");
  strFieldID.TrimLeft(" ");
  
	ConditionGroupInfo  stConditionGroupInfo;
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	stConditionGroupInfo.stOneConditionInfo = stOneConditionInfo;
	
	  if (strFieldID.IsEmpty() == FALSE)
	  {
	  //# 값보기 정보 
	  if (GetMoveFieldIDToInfo(strFieldID, stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo) == FALSE)
	  return FALSE;
	  else
	  {
	  if (bLoadFlag == FALSE)
	  {
	  int nValViewCnt = stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
	  stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = nValViewCnt;
	  
		stConditionGroupInfo.stOneConditionInfo.bFieldState = (nValViewCnt > 0) ? TRUE : FALSE;    
		stConditionGroupInfo.stOneConditionInfo.bFieldEnable = !stConditionGroupInfo.stOneConditionInfo.bFalg;
		}
		}
		}
		else
		{
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt = 0;
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = FALSE;
		
		  stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = 0;
		  
			stConditionGroupInfo.stOneConditionInfo.bFieldState = FALSE;    
			stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
			}
			stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.strElement = strElement;
			
			  //# 값보기
			  stConditionGroupInfo.stOneConditionInfo.bFieldVal = bResultViewDefVal;          
			  stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bResultViewDefVal;
			  
				m_listConditionGroup.SetAt(pos, stConditionGroupInfo);
				
				  #ifdef _DEBUG
				  DebugInputDataView();
				  #endif
	*/
	
	// 2009.11.11 -hidden- -------------------------------------------------
	strFieldID.TrimRight(" ");
	strFieldID.TrimLeft(" ");
	
	ConditionGroupInfo  stConditionGroupInfo;
	GetElementToGroup(strElement, stConditionGroupInfo);
	stConditionGroupInfo.stOneConditionInfo = stOneConditionInfo;
	
	if (strFieldID.IsEmpty() == FALSE)
	{
		//# 값보기 정보 
		if (GetMoveFieldIDToInfo(strFieldID, stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo) == FALSE)
			return FALSE;
		else
		{
			if (bLoadFlag == FALSE)
			{
				int nValViewCnt = stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = nValViewCnt;
				
				stConditionGroupInfo.stOneConditionInfo.bFieldState = (nValViewCnt > 0) ? TRUE : FALSE;    
				stConditionGroupInfo.stOneConditionInfo.bFieldEnable = !stConditionGroupInfo.stOneConditionInfo.bFalg;
			}
		}
	}
	else
	{
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt = 0;
		stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = FALSE;
		
		stConditionGroupInfo.stOneConditionInfo.stCondition.ResultViewNum = 0;
		
		stConditionGroupInfo.stOneConditionInfo.bFieldState = FALSE;    
		stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
	}
	stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.strElement = strElement;
	
	//# 값보기
	stConditionGroupInfo.stOneConditionInfo.bFieldVal = bResultViewDefVal;          
	stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bResultViewDefVal;
	
	return TRUE;
}


int CDataAdmin::SetElementStandLine()
{
/*	int nResult = 0;
int nElementIndex = 0;

  POSITION pos;
  ConditionGroupInfo	stConditionGroupInfo;
  int nCount = m_listConditionGroup.GetCount();
  int nMaxGroup = GetMaxGroup();
  
	for (int nGroupIndex = 0; nGroupIndex <= nMaxGroup; nGroupIndex++)
	{
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	pos = m_listConditionGroup.FindIndex(nIndex);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  if (stConditionGroupInfo.nGroup == nGroupIndex)
	  {
	  stConditionGroupInfo.strElement = CONDITION_INDEX[nElementIndex];
	  m_listConditionGroup.SetAt(pos, stConditionGroupInfo);
	  
		nElementIndex++;
		nResult++;
		}
		}
		}
	*/
	
	// 2009.11.13 -hidden-	
	int nResult = 0;
	int nElementIndex = 0;
	
	ConditionGroupInfo	stConditionGroupInfo;
	int nCodeSize = 0;
	
	int nSize = m_arrGroups.GetSize();
	for (int nGroupIndex = 0; nGroupIndex < nSize; nGroupIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nGroupIndex);
		nCodeSize = pGroup->arrConGrpInfo.GetSize();
		
		for (int nIndex = 0; nIndex < nCodeSize; nIndex++)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(nIndex);
			stConditionGroupInfo.strElement = CONDITION_INDEX[nElementIndex];
			pGroup->arrConGrpInfo.SetAt(nIndex, stConditionGroupInfo);
			nElementIndex++;
			nResult++;
		}
	}
	
	return nResult;
}


BOOL CDataAdmin::SetDataGroupSort()
{  
	BOOL bResult = FALSE;
	/*	
	POSITION posCondition;
	ConditionGroupInfo  stConditionGroupInfo;
	CList<ConditionGroupInfo, ConditionGroupInfo&> tempConditionGroupList;
	int nCheckCnt = 0;
	
	  int nConditionCount = m_listConditionGroup.GetCount();
	  tempConditionGroupList.RemoveAll();
	  
		if (nConditionCount <= 0)
		return FALSE;
		
		  int nMaxGroup = GetMaxGroup();
		  for (int nGroupIndex = 0; nGroupIndex <= nMaxGroup; nGroupIndex++)
		  {
		  for (int nIndex = 0; nIndex < nConditionCount; nIndex++)
		  {
		  posCondition = m_listConditionGroup.FindIndex(nIndex);
		  stConditionGroupInfo = m_listConditionGroup.GetAt(posCondition);
		  
			if (stConditionGroupInfo.nGroup == nGroupIndex)
			{
			tempConditionGroupList.AddTail(stConditionGroupInfo);
			nCheckCnt++;
			}
			}
			}
			
			  if (nConditionCount == nCheckCnt)
			  {
			  bResult = TRUE;
			  
				m_listConditionGroup.RemoveAll();
				nConditionCount = tempConditionGroupList.GetCount();
				
				  for (int nConditionIndex = 0; nConditionIndex < nConditionCount; nConditionIndex++)
				  {
				  posCondition = tempConditionGroupList.FindIndex(nConditionIndex);
				  stConditionGroupInfo = tempConditionGroupList.GetAt(posCondition);
				  
					m_listConditionGroup.AddTail(stConditionGroupInfo);
					}
					}
	*/
	
	// 2009.11.13 -hidden-
	bResult = TRUE;
	
	return bResult;
}


BOOL CDataAdmin::ModifyOneCondition(CString strElement, OneConditionInfo stConditionInfo)
{
/*	BOOL bResult = FALSE;

  POSITION posElement;
  ConditionGroupInfo  stConditionGroupInfo;
  
	posElement = GetElementToPosition(strElement);
	if (posElement != NULL)
	{
	stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
	
	  stConditionGroupInfo.stOneConditionInfo = stConditionInfo;
	  m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
	  
		bResult = TRUE;
		}
	*/
	
	// 2009.11.11 -hidden-
	ConditionGroupInfo  stConditionGroupInfo;
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		int nItems = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < nItems; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			stConditionGroupInfo.strElement.TrimLeft();
			stConditionGroupInfo.strElement.TrimRight();
			if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
			{
				stConditionGroupInfo.stOneConditionInfo = stConditionInfo;
				pGroup->arrConGrpInfo.SetAt(j, stConditionGroupInfo);
				return TRUE; 
			}
		}
	}
	
	return FALSE;
}


BOOL CDataAdmin::ModifyConditionLogicOpt(CStringArray *psaLogicInfoData)
{
/*	BOOL bResult = FALSE;
POSITION posElement;
ConditionGroupInfo  stConditionGroupInfo;

  CString strElement = psaLogicInfoData->GetAt(0);
  CString strOpt = psaLogicInfoData->GetAt(1);
  
	int nCount = m_listConditionGroup.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	posElement = m_listConditionGroup.FindIndex(nIndex);
	stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
	
	  if (stConditionGroupInfo.strElement == strElement)
	  {
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Operator = (strOpt == "0")?'0':'1';
	  m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
	  
		bResult = TRUE;
		}
		}
	*/
	
	// 2009.11.13 -hidden-
	
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	
	CString strElement = psaLogicInfoData->GetAt(0);
	CString strOpt = psaLogicInfoData->GetAt(1);
	int nCondSize = 0;
	
	int nSize = m_arrGroups.GetSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nIndex);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			if (stConditionGroupInfo.strElement == strElement)
			{
				stConditionGroupInfo.stOneConditionInfo.stCondition.Operator = (strOpt == "0")?'0':'1';
				pGroup->arrConGrpInfo.SetAt(j, stConditionGroupInfo);
				
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

BOOL CDataAdmin::ModifyConditionTargetBase(int nGroup, int nTargetIndex, int nOutType/*= -1*/)
{
/*	BOOL bResult = FALSE;
POSITION posElement;
ConditionGroupInfo  stConditionGroupInfo;

  int nCount = m_listConditionGroup.GetCount();
  for (int nIndex = 0; nIndex < nCount; nIndex++)
  {
		posElement = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
		
		  if (nGroup == stConditionGroupInfo.nGroup)
		  {
		  BOOL bChangeState = TRUE;
		  
			int nSetTargetIndex = 0;
			if (stConditionGroupInfo.stOneConditionInfo.nTargetBaseType == 0)
			{
			nSetTargetIndex = TARGETINDEX_TYPE1[nTargetIndex];
			
			  if (nOutType == 1)		//# 일간제외 => 2004.09.21
			  bChangeState = FALSE;
			  }
			  else if (stConditionGroupInfo.stOneConditionInfo.nTargetBaseType == 1)  //# 일간 고정
			  bChangeState = FALSE;
			  else if (stConditionGroupInfo.stOneConditionInfo.nTargetBaseType == 2)  //# 분 고정
			  {
			  if ((TARGETINDEX_TYPE3[0] <= nTargetIndex) &&
			  (TARGETINDEX_TYPE3[_countof(TARGETINDEX_TYPE3) - 1] >= nTargetIndex))
			  {
			  int nIndex = 0;
			  for (nIndex = 0; nIndex < _countof(TARGETINDEX_TYPE3); nIndex++)
			  {
			  if (TARGETINDEX_TYPE3[nIndex] == nTargetIndex)
			  break;
			  }
			  nSetTargetIndex = TARGETINDEX_TYPE3[nIndex];
			  }
			  else
			  bChangeState = FALSE;
			  }
			  
				if (bChangeState == TRUE)
				{
				stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase = nSetTargetIndex;
				m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
				}
				
				  bResult = TRUE;
				  }
				  }
	*/	
	
	// 2009.11.13 -hidden-
	
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;
	
	int nSize = m_arrGroups.GetSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nIndex);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		if (nGroup == pGroup->nGroup)
		{
			for(int j = 0; j < nCondSize; ++j)
			{
				stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
				
				BOOL bChangeState = TRUE;
				
				int nSetTargetIndex = 0;
				if (stConditionGroupInfo.stOneConditionInfo.nTargetBaseType == 0)
				{
					nSetTargetIndex = TARGETINDEX_TYPE1[nTargetIndex];
					
					if (nOutType == 1)		//# 일간제외 => 2004.09.21
						bChangeState = FALSE;
				}
				else if (stConditionGroupInfo.stOneConditionInfo.nTargetBaseType == 1)  //# 일간 고정
					bChangeState = FALSE;
				else if (stConditionGroupInfo.stOneConditionInfo.nTargetBaseType == 2)  //# 분 고정
				{/* 0015707: -hidden-
					if ((TARGETINDEX_TYPE3[0] <= nTargetIndex) &&
						(TARGETINDEX_TYPE3[_countof(TARGETINDEX_TYPE3) - 1] >= nTargetIndex))
					{
						int nIndex = 0;
						for (nIndex = 0; nIndex < _countof(TARGETINDEX_TYPE3); nIndex++)
						{
							if (TARGETINDEX_TYPE3[nIndex] == nTargetIndex)
								break;
						}
						nSetTargetIndex = TARGETINDEX_TYPE3[nIndex];
					}
					else
						bChangeState = FALSE;
				*/
					//--> 0015707: [7127] "당일내발생" 내 조건에 2분, 20분, 120분 주기 추가요청
					if (nTargetIndex >=3)
						nSetTargetIndex = TARGETINDEX_TYPE3[nTargetIndex - 3];
					else
						bChangeState = FALSE;
					//<--
				}
				
				if (bChangeState == TRUE)
				{
					stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase = nSetTargetIndex;
					pGroup->arrConGrpInfo.SetAt(j, stConditionGroupInfo);
				}
				
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

BOOL CDataAdmin::ModifyConditionTerm(int nGroup, int nTerm, CString strCase)
{
/*	BOOL bResult = FALSE;
POSITION posCondition;
ConditionGroupInfo  stConditionGroupInfo;

  int nCount = m_listConditionGroup.GetCount();
  for (int nIndex = 0; nIndex < nCount; nIndex++)
  {
		posCondition = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(posCondition);
		
		  if (nGroup == stConditionGroupInfo.nGroup)
		  {
		  if (strCase == "START")
		  stConditionGroupInfo.stOneConditionInfo.stCondition.STerm = nTerm;
		  else if (strCase == "END")
		  stConditionGroupInfo.stOneConditionInfo.stCondition.ETerm = nTerm;
		  
			m_listConditionGroup.SetAt(posCondition, stConditionGroupInfo);
			bResult = TRUE;
			}
			}
	*/
	
	// 2009.11.13 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;
	
	int nSize = m_arrGroups.GetSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nIndex);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		if (nGroup == pGroup->nGroup)
		{
			for(int j = 0; j < nCondSize; ++j)
			{
				stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
				
				if (strCase == "START")
					stConditionGroupInfo.stOneConditionInfo.stCondition.STerm = nTerm;
				else if (strCase == "END")
					stConditionGroupInfo.stOneConditionInfo.stCondition.ETerm = nTerm;
				
				pGroup->arrConGrpInfo.SetAt(j, stConditionGroupInfo);
				bResult = TRUE;
			}
		}
	}
	return bResult;
}


BOOL CDataAdmin::ModifyConditionTerm(int nGroup, int nSTerm, int nETerm)
{
/*	BOOL bResult = FALSE;
POSITION posCondition;
ConditionGroupInfo  stConditionGroupInfo;

  int nCount = m_listConditionGroup.GetCount();
  for (int nIndex = 0; nIndex < nCount; nIndex++)
  {
		posCondition = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(posCondition);
		
		  if (nGroup == stConditionGroupInfo.nGroup)
		  {
		  stConditionGroupInfo.stOneConditionInfo.stCondition.STerm = nSTerm;
		  stConditionGroupInfo.stOneConditionInfo.stCondition.ETerm = nETerm;
		  
			m_listConditionGroup.SetAt(posCondition, stConditionGroupInfo);
			bResult = TRUE;
			}
			}
	*/
	
	// 2009.11.13 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;
	
	int nSize = m_arrGroups.GetSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nIndex);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		if(pGroup->nGroup == nGroup)
		{
			for(int j = 0; j < nCondSize; ++j)
			{
				stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
				stConditionGroupInfo.stOneConditionInfo.stCondition.STerm = nSTerm;
				stConditionGroupInfo.stOneConditionInfo.stCondition.ETerm = nETerm;
				
				pGroup->arrConGrpInfo.SetAt(j, stConditionGroupInfo);
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

BOOL CDataAdmin::ModifyOneConditionTargetBase(CString strElement, int nTargetIndex)
{
/*	BOOL bResult = FALSE;
POSITION posElement;
ConditionGroupInfo  stConditionGroupInfo;

  int nCount = m_listConditionGroup.GetCount();
  
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	posElement = m_listConditionGroup.FindIndex(nIndex);
	stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
	
	  if (strElement == stConditionGroupInfo.strElement)
	  {
	  stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase = nTargetIndex;
	  m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
	  
		bResult = TRUE;
		}
		}
	*/
	
	// 2009.11.13 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;
	
	int nSize = m_arrGroups.GetSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nIndex);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			if (strElement == stConditionGroupInfo.strElement)
			{
				stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase = nTargetIndex;
				pGroup->arrConGrpInfo.SetAt(j, stConditionGroupInfo);
				
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}


BOOL CDataAdmin::ModifyConditionCheck(CString strElement, BOOL bView)
{
/*	BOOL bResult = FALSE;

  POSITION posElement;
  ConditionGroupInfo  stConditionGroupInfo;
  
	posElement = GetElementToPosition(strElement);
	if (posElement != NULL)
	{
	stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
	stConditionGroupInfo.stOneConditionInfo.bFalg = bView;
	
	  if (bView == TRUE)
	  {
	  if (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE)
	  {
	  if (stConditionGroupInfo.stOneConditionInfo.stCondition.Reverse == '0')
	  stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
	  }
	  }
	  else
	  {
	  if (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE)
	  {
	  stConditionGroupInfo.stOneConditionInfo.bFieldEnable = TRUE;
	  stConditionGroupInfo.stOneConditionInfo.bFieldVal = FALSE;
	  }
	  }
	  m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
	  
		bResult = TRUE;
		}
		
		  #ifdef _DEBUG
		  DebugMoveFiledInfo();
		  #endif
	*/
	
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
    
	if (GetElementToGroup(strElement, stConditionGroupInfo))
	{
		stConditionGroupInfo.stOneConditionInfo.bFalg = bView;
		
		if (bView == TRUE)
		{
			if (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE)
			{
				if (stConditionGroupInfo.stOneConditionInfo.stCondition.Reverse == '0')
					stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
			}
		}
		else
		{
			if (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE)
			{
				stConditionGroupInfo.stOneConditionInfo.bFieldEnable = TRUE;
				stConditionGroupInfo.stOneConditionInfo.bFieldVal = FALSE;
			}
		}
		bResult = ModifyOneCondition(strElement, stConditionGroupInfo.stOneConditionInfo);
	}
	
	return bResult;
}

BOOL CDataAdmin::ModifyGroupMoveFieldCheck(int nGroupNo, BOOL bView)
{
/*	BOOL bResult = FALSE;
int nResult = 0;

  ConditionGroupInfo  stConditionGroupInfo;
  POSITION pos;
  int nCount = m_listConditionGroup.GetCount();
  
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  if ((nGroupNo == stConditionGroupInfo.nGroup) &&
	  (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE) &&
	  (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE))
	  {
	  stConditionGroupInfo.stOneConditionInfo.bFieldVal = bView;
	  stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bView;
	  
		m_listConditionGroup.SetAt(pos, stConditionGroupInfo);
		bResult = TRUE;
		}
		}
		
		  #ifdef _DEBUG
		  DebugMoveFiledInfo();
		  #endif
	*/
	
	BOOL bResult = FALSE;
	CString strElement;
	
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		
		if(pGroup->nGroup == nGroupNo)
		{
			nCondSize = pGroup->arrConGrpInfo.GetSize();
			
			for(int j= 0; j < nCondSize; ++j)
			{
				stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
				
				if((stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE) && (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE))
				{
					stConditionGroupInfo.stOneConditionInfo.bFieldVal = bView;
					stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bView;
					
					stConditionGroupInfo.strElement.TrimLeft();
					stConditionGroupInfo.strElement.TrimRight();
					
					bResult = ModifyOneCondition(stConditionGroupInfo.strElement, stConditionGroupInfo.stOneConditionInfo);
				}
			}
		}
	}
	
	return bResult;
}

BOOL CDataAdmin::ModifyConditionMoveFieldCheck(CString strElement, BOOL bView)
{
/*	BOOL bResult = FALSE;
POSITION posElement;
ConditionGroupInfo  stConditionGroupInfo;

  posElement = GetElementToPosition(strElement);
  if (posElement != NULL)
  {
		stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
		
		  //## 값보기 필드가 존재하는 지표만 수정 ##
		  if (stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE)
		  {
		  stConditionGroupInfo.stOneConditionInfo.bFieldVal = bView;
		  stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.bView = bView;
		  
			m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
			bResult = TRUE;
			}
			}
			
			  #ifdef _DEBUG
			  DebugMoveFiledInfo();
			  #endif
	*/
	
	// 2009.11.11 - hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
    
	OneConditionInfo stCodition;
	if (GetElementToOneConditionInfo(strElement, stCodition))
	{
		if (stCodition.bFieldState == TRUE)
		{
			stCodition.bFieldVal = bView;
			stCodition.stHeadMoveInfo.bView = bView;
			
			bResult = ModifyOneCondition(strElement, stCodition);
		}
	}
	
	return bResult;
}

void CDataAdmin::DebugMoveFiledInfo()
{
	CString strTemp;
	
	POSITION pos;
	ConditionGroupInfo  stConditionGroupInfo;
	
	for (int nIndex = 0; nIndex < m_listConditionGroup.GetCount(); nIndex++)
	{
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		strTemp.Format("\n# Condition Flag : %d, FieldVal : %d, FieldState : %d, FieldEnableVal : %d",
			stConditionGroupInfo.stOneConditionInfo.bFalg,
			stConditionGroupInfo.stOneConditionInfo.bFieldVal,
			stConditionGroupInfo.stOneConditionInfo.bFieldState,
			stConditionGroupInfo.stOneConditionInfo.bFieldEnable);
		
		TRACE(strTemp);
	}
}


BOOL CDataAdmin::ModifyConditionNotCheck(CString strElement, int nNot)
{
/*	BOOL bResult = FALSE;

  POSITION posElement;
  ConditionGroupInfo  stConditionGroupInfo;
  
	posElement = GetElementToPosition(strElement);
	if (posElement != NULL)
	{
	stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
	
	  if (nNot == 0)
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Reverse = '0';
	  else
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Reverse = '1';
	  m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
	  
		bResult = TRUE;
		}
	*/
	
	// 2009.11.11 -hidden-
	BOOL bResult = FALSE;
	OneConditionInfo stCodition;
	
	if (GetElementToOneConditionInfo(strElement, stCodition))
	{
		if (nNot == 0)
			stCodition.stCondition.Reverse = '0';
		else
			stCodition.stCondition.Reverse = '1';
		
		bResult = ModifyOneCondition(strElement, stCodition);
	}
	
	return bResult;
}


BOOL CDataAdmin::ModifyConditionUserDefine(CString strElement, User_Define stCondition, CString strText)
{
/*	BOOL bResult = FALSE;

  POSITION posElement;
  ConditionGroupInfo  stConditionGroupInfo;
  
	posElement = GetElementToPosition(strElement);
	if (posElement != NULL)
	{
	stConditionGroupInfo = m_listConditionGroup.GetAt(posElement);
	
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Type2 = stCondition.Type2;
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Trend = stCondition.Trend;
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Rank = stCondition.Rank;
	  stConditionGroupInfo.stOneConditionInfo.stCondition.Reverse = stCondition.Reverse;
	  
		//# Not 조건 => Grid 값보기 : FALSE, 상태 : disEable
		if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
		{
		if ((stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE) &&
		(stCondition.Reverse == '0'))
		stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
		else
		{
		stConditionGroupInfo.stOneConditionInfo.bFieldEnable = TRUE;
		stConditionGroupInfo.stOneConditionInfo.bFieldVal = FALSE;
		}
		}
		else
		{
		if ((stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE) &&
		(stCondition.Reverse == '1'))
		stConditionGroupInfo.stOneConditionInfo.bFieldVal = FALSE;
		
		  stConditionGroupInfo.stOneConditionInfo.bFieldEnable = TRUE;
		  }
		  
			//# 단일/연속 지표의 UpDate 구분
			//		if (CheckCondiGubunState(stConditionGroupInfo.stOneConditionInfo.strConditionTag) == TRUE)
			stConditionGroupInfo.stOneConditionInfo.stCondition.IdxContiGubun = stCondition.IdxContiGubun;
			
			  if (stConditionGroupInfo.nGroup == 0)
			  {
			  stConditionGroupInfo.stOneConditionInfo.stCondition.STerm = stCondition.STerm;
			  stConditionGroupInfo.stOneConditionInfo.stCondition.ETerm = stCondition.ETerm;
			  }
			  
				stConditionGroupInfo.stOneConditionInfo.stCondition.SignalCnt1 = stCondition.SignalCnt1;
				stConditionGroupInfo.stOneConditionInfo.stCondition.SignalCnt2 = stCondition.SignalCnt2;
				
				  memcpy(&stConditionGroupInfo.stOneConditionInfo.stCondition.Code, &stCondition.Code, sizeof(stCondition.Code));
				  
					int nIndex;
					for (nIndex = 0; nIndex < MAX_PARAM; nIndex++)
					stConditionGroupInfo.stOneConditionInfo.stCondition.Param[nIndex] = stCondition.Param[nIndex];
					
					  for (nIndex = 0; nIndex < MAX_CONDI; nIndex++)
					  stConditionGroupInfo.stOneConditionInfo.stCondition.Condition[nIndex] = stCondition.Condition[nIndex];
					  
						stConditionGroupInfo.stOneConditionInfo.strText = strText;
						
						  m_listConditionGroup.SetAt(posElement, stConditionGroupInfo);
						  
							bResult = TRUE;
							}
	*/
	
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	
	if (GetElementToGroup(strElement, stConditionGroupInfo))
	{
		stConditionGroupInfo.stOneConditionInfo.stCondition.Type = stCondition.Type;
		stConditionGroupInfo.stOneConditionInfo.stCondition.Type2 = stCondition.Type2;
		stConditionGroupInfo.stOneConditionInfo.stCondition.Trend = stCondition.Trend;
		stConditionGroupInfo.stOneConditionInfo.stCondition.Rank = stCondition.Rank;
		stConditionGroupInfo.stOneConditionInfo.stCondition.Reverse = stCondition.Reverse;
		
		if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
		{
			if ((stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE) && (stCondition.Reverse == '0'))
				stConditionGroupInfo.stOneConditionInfo.bFieldEnable = FALSE;
			else
			{
				stConditionGroupInfo.stOneConditionInfo.bFieldEnable = TRUE;
				stConditionGroupInfo.stOneConditionInfo.bFieldVal = FALSE;
			}
		}
		else
		{
			if ((stConditionGroupInfo.stOneConditionInfo.bFieldState == TRUE) && (stCondition.Reverse == '1'))
				stConditionGroupInfo.stOneConditionInfo.bFieldVal = FALSE;
			
			stConditionGroupInfo.stOneConditionInfo.bFieldEnable = TRUE;
		}
		
		stConditionGroupInfo.stOneConditionInfo.stCondition.IdxContiGubun = stCondition.IdxContiGubun;
		
		if (stConditionGroupInfo.nGroup == 0)
		{
			stConditionGroupInfo.stOneConditionInfo.stCondition.STerm = stCondition.STerm;
			stConditionGroupInfo.stOneConditionInfo.stCondition.ETerm = stCondition.ETerm;
		}
		
		stConditionGroupInfo.stOneConditionInfo.stCondition.SignalCnt1 = stCondition.SignalCnt1;
		stConditionGroupInfo.stOneConditionInfo.stCondition.SignalCnt2 = stCondition.SignalCnt2;
		
		// 0621 재무
		if(stConditionGroupInfo.stOneConditionInfo.stCondition.Type == 4001 ||stConditionGroupInfo.stOneConditionInfo.stCondition.Type == 4002 ||
			stConditionGroupInfo.stOneConditionInfo.stCondition.Type == 4003 ||stConditionGroupInfo.stOneConditionInfo.stCondition.Type == 4004)
			memcpy(&stConditionGroupInfo.stOneConditionInfo.stCondition.IndexType, &stCondition.IndexType, sizeof(stCondition.IndexType));
		
		/*memcpy(&stConditionGroupInfo.stOneConditionInfo.stCondition.IndexType, &stCondition.IndexType, sizeof(stCondition.IndexType));*/
		
		memcpy(&stConditionGroupInfo.stOneConditionInfo.stCondition.Code, &stCondition.Code, sizeof(stCondition.Code));
		
		int nIndex;
		for (nIndex = 0; nIndex < MAX_PARAM; nIndex++)
			stConditionGroupInfo.stOneConditionInfo.stCondition.Param[nIndex] = stCondition.Param[nIndex];
		
		for (nIndex = 0; nIndex < MAX_CONDI; nIndex++)
			stConditionGroupInfo.stOneConditionInfo.stCondition.Condition[nIndex] = stCondition.Condition[nIndex];
		
		stConditionGroupInfo.stOneConditionInfo.strText = strText;
		bResult = ModifyOneCondition(strElement, stConditionGroupInfo.stOneConditionInfo);
	}
	
	return bResult;
}

BOOL CDataAdmin::CheckCondiGubunState(CString strTagID)
{
	BOOL bResult = FALSE;
	
	if (strTagID.Left(1) == "Y")
	{
		if ((strTagID.Compare("Y1_6") == 0) ||
			(strTagID.Compare("Y1_23") == 0) ||
			(strTagID.Compare("Y1_29") == 0) ||
			(strTagID.Compare("Y2_3") == 0))
			bResult = FALSE;
		else
			bResult = TRUE;
	}
	
	return bResult;
}

BOOL CDataAdmin::ModifyConditionInsertGroup(int nGroup)
{
	BOOL bResult = FALSE;
	
	POSITION posCondition;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCount = m_listConditionGroup.GetCount();
	
	if (nCount == 0)
		bResult = TRUE;
	else
	{
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			posCondition = m_listConditionGroup.FindIndex(nIndex);
			stConditionGroupInfo = m_listConditionGroup.GetAt(posCondition);
			
			if (nGroup <= stConditionGroupInfo.nGroup)
			{
				stConditionGroupInfo.nGroup++;
				stConditionGroupInfo.stOneConditionInfo.stCondition.GroupNo++;
				m_listConditionGroup.SetAt(posCondition, stConditionGroupInfo);
				
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

// 2009.11.10 -hidden-
BOOL CDataAdmin::ModifyConditionInsertUpGroup(int nIndex, int nGroup)
{
	GROUP_INFO* pGroupInfo = new GROUP_INFO;
	pGroupInfo->nGroup = nGroup;
	
	if(m_arrGroups.GetSize())
	{
		GROUP_INFO* pFirst = m_arrGroups.GetAt(0);
		if(pFirst->nGroup == 0)
			m_arrGroups.InsertAt(nIndex, pGroupInfo);
		else
			m_arrGroups.InsertAt(nIndex-1, pGroupInfo);
	}
	else
		m_arrGroups.Add(pGroupInfo);
	
	return TRUE;
}

// 2009.11.10 -hidden-
BOOL CDataAdmin::ModifyConditionInsertDownGroup(int nIndex, int nGroup)
{
	GROUP_INFO* pGroupInfo = new GROUP_INFO;
	pGroupInfo->nGroup = nGroup;
	
	int nSize = m_arrGroups.GetSize();
	if(nSize >= nIndex)
	{
		GROUP_INFO* pFirst = m_arrGroups.GetAt(0);
		if(pFirst->nGroup == 0)
			m_arrGroups.InsertAt(nIndex, pGroupInfo);
		else
			m_arrGroups.InsertAt(nIndex-1, pGroupInfo);
	}
	else
		m_arrGroups.Add(pGroupInfo);
	
	return TRUE;
}

BOOL CDataAdmin::ModifyConditionDeleteGroup(int nGroup)
{
/*	BOOL bResult = FALSE;

  POSITION posCondition;
  ConditionGroupInfo  stConditionGroupInfo;
  int nCount = m_listConditionGroup.GetCount();
  
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	posCondition = m_listConditionGroup.FindIndex(nIndex);
	stConditionGroupInfo = m_listConditionGroup.GetAt(posCondition);
	
	  if (nGroup == stConditionGroupInfo.nGroup)
	  {
	  m_listConditionGroup.RemoveAt(posCondition);
	  bResult = TRUE;
	  break;
	  }
	  }
	  ////////////////////////////////////////////////////////////////
	  for(int nIndex = nCount-1; nIndex > -1; --nIndex)
	  {
	  posCondition = m_listConditionGroup.FindIndex(nIndex);
	  stConditionGroupInfo = m_listConditionGroup.GetAt(posCondition);
	  
		if (nGroup == stConditionGroupInfo.nGroup)
		{
		m_listConditionGroup.RemoveAt(posCondition);
		bResult = TRUE;
		}
		}
		
		  return bResult;
	*/
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroupInfo = m_arrGroups.GetAt(i);
		if(pGroupInfo->nGroup == nGroup)
		{
			pGroupInfo->arrConGrpInfo.RemoveAll();
			delete pGroupInfo;
			
			m_arrGroups.RemoveAt(i);
			break;
		}
	}
	
	return TRUE;
}


int CDataAdmin::GetGroupCnt()
{
/*	POSITION pos;
int nCount = m_listConditionGroup.GetCount();
ConditionGroupInfo stConditionGroupInfo;

  CUIntArray		iaGroup;
  int nIndex, nGroup;
  for (nIndex = 0; nIndex < nCount; nIndex++)
  {
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		  BOOL bState = FALSE;
		  for (nGroup = 0; nGroup < iaGroup.GetSize(); nGroup++)
		  {
		  if ((stConditionGroupInfo.nGroup - iaGroup.GetAt(nGroup)) == 0)
		  {
		  bState = TRUE;
		  break;
		  }
		  }
		  
			if (bState == FALSE)
			iaGroup.Add(stConditionGroupInfo.nGroup);
			}
			
			  return iaGroup.GetSize();
	*/
	
	
	// 2009.11.16 -hidden-
	
	int nGroupSize = 0;
	int nSize = m_arrGroups.GetSize();
	
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		if(pGroup->arrConGrpInfo.GetSize())
			++nGroupSize;
	}
	
	return nGroupSize;
}


int CDataAdmin::GetMaxGroup()
{
	int nResult = -1;
	
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	ConditionGroupInfo stConditionGroupInfo;
	
	for (int i = 0; i < nCount; i++)
	{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		if (nResult < stConditionGroupInfo.nGroup)
			nResult = stConditionGroupInfo.nGroup;
	}
	
	return nResult;
}


int CDataAdmin::GetMoveFieldTotCnt(BOOL bCommonGroup/* = TRUE*/)
{
/*	int nResult = 0;

  POSITION pos;
  int nCount = m_listConditionGroup.GetCount();
  ConditionGroupInfo stConditionGroupInfo;
  
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  if (bCommonGroup == TRUE)
	  {
	  if (stConditionGroupInfo.nGroup == 0)
	  {
	  if (stConditionGroupInfo.stOneConditionInfo.bFieldVal == TRUE)
	  nResult += stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
	  }
	  }
	  else if (bCommonGroup == FALSE)
	  {
	  if (stConditionGroupInfo.nGroup > 0)
	  {
	  if (stConditionGroupInfo.stOneConditionInfo.bFieldVal == TRUE)
	  nResult += stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
	  }
	  }
	  }
	*/
	
	// 2009.11.13 -hidden-
	
	int nResult = 0;
	int nCondSize = 0;
	ConditionGroupInfo stConditionGroupInfo;
	
	int nCount = m_arrGroups.GetSize();
	for (int i = 0; i < nCount; ++i)
	{	
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			if (bCommonGroup == TRUE)
			{
				if (stConditionGroupInfo.nGroup == 0)
				{
					if (stConditionGroupInfo.stOneConditionInfo.bFieldVal == TRUE)
						nResult += stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				}
			}
			else if (bCommonGroup == FALSE)
			{
				if (stConditionGroupInfo.nGroup > 0)
				{
					if (stConditionGroupInfo.stOneConditionInfo.bFieldVal == TRUE)
						nResult += stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt;
				}
			}
		}
	}
	
	return nResult;
}

void CDataAdmin::DeleteOneCondition(CString strElement)
{
/*	POSITION posElement;
posElement = GetElementToPosition(strElement);

  if (posElement != NULL)
		m_listConditionGroup.RemoveAt(posElement);
		else
		TRACE("\n[조건검색] DataAdmin => Grid Item Delete Error!");
	*/
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	ConditionGroupInfo stConditionGroupInfo;	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		//		int nItems = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < pGroup->arrConGrpInfo.GetSize(); ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			stConditionGroupInfo.strElement.TrimLeft();
			stConditionGroupInfo.strElement.TrimRight();
			
			if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
				pGroup->arrConGrpInfo.RemoveAt(j);
		}
	}
}

void CDataAdmin::DeleteGroup(int nGroup)
{
/*	BOOL bState = FALSE;

  ConditionGroupInfo  stConditionGroupInfo;
  POSITION pos;
  
	int nCount = m_listConditionGroup.GetCount();
	if (nCount == 0)
	return;
	
	  while (bState == FALSE)
	  {
	  nCount = m_listConditionGroup.GetCount();
	  
		bState = FALSE;
		for (int i = 0; i < nCount; i++)
		{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		  if (stConditionGroupInfo.nGroup == nGroup)
		  {
		  m_listConditionGroup.RemoveAt(pos);
		  bState = FALSE;
		  break;
		  }
		  else
		  bState = TRUE;
		  }
		  }
	*/
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		if(pGroup->nGroup == nGroup)
		{
			pGroup->arrConGrpInfo.RemoveAll();
			delete pGroup;
			
			m_arrGroups.RemoveAt(i);
			break;
		}
	}
}


int CDataAdmin::GetGroupToConditionCnt(int nGroup, BOOL bException/* = TRUE*/)
{
/*	int nResult = 0;

  ConditionGroupInfo  stConditionGroupInfo;
  POSITION pos;
  int nCount = m_listConditionGroup.GetCount();
  for (int i = 0; i < nCount; i++)
  {
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		  if (stConditionGroupInfo.nGroup == nGroup)
		  {
		  if (bException == TRUE)
		  {
		  if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
		  nResult++;
		  }
		  else
		  nResult++;
		  }
		  }
	*/
	
	int nResult = 0;
	
	ConditionGroupInfo  stConditionGroupInfo;
	int nCount = m_arrGroups.GetSize();
	
	for (int i = 0; i < nCount; i++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		if (pGroup->nGroup == nGroup)
		{
			int nCondSize = pGroup->arrConGrpInfo.GetSize();
			for(int j = 0; j < nCondSize; ++j)
			{
				stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
				if (bException == TRUE)
				{
					if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
						nResult++;
				}
				else
					nResult++;
			}
		}
	}
	
	return nResult;
}

int CDataAdmin::GetConditionCnt()
{
	//	int nViewCnt = m_listConditionGroup.GetCount();
	
	// 2009.11.11 -hidden-
	int nViewCnt = 0;
	int nSize = m_arrGroups.GetSize();
	
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nViewCnt += pGroup->arrConGrpInfo.GetSize();
	}
	
	return nViewCnt;
}

int CDataAdmin::GetLanguageConditionCnt()
{
/*	int nResult = 0;

  ConditionGroupInfo  stConditionGroupInfo;
  POSITION pos;
  for (int nIndex = 0; nIndex < m_listConditionGroup.GetCount(); nIndex++)
  {
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
        
		  if (stConditionGroupInfo.stOneConditionInfo.strTag == (CString)TAG_TYPE_LANGUAGE)
		  nResult++;
		  }
	*/
	
	// 2009.11.11 -hidden-
	int nResult = 0;
	int nCondSize = 0;
	
	ConditionGroupInfo  stConditionGroupInfo;
	int nSize = m_arrGroups.GetSize();
	
	for (int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			if (stConditionGroupInfo.stOneConditionInfo.strTag == (CString)TAG_TYPE_LANGUAGE)
				nResult++;
		}
	}
	
	return nResult;
}

int CDataAdmin::GetConditionViewCnt()
{
	int nViewCnt = 0;
	
	/*	ConditionGroupInfo  stConditionGroupInfo;
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	
	  for (int i = 0; i < nCount; i++)
	  {
	  pos = m_listConditionGroup.FindIndex(i);
	  stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	  
		if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
		nViewCnt++;
		}
	*/
	
	
	// 2009.11.11 -hidden-
	int nCondSize = 0;
	ConditionGroupInfo  stConditionGroupInfo;
	int nSize = m_arrGroups.GetSize();
	
	for (int i = 0; i < nSize; i++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			if (stConditionGroupInfo.stOneConditionInfo.bFalg == TRUE)
				nViewCnt++;
		}
	}
	return nViewCnt;
}


CString CDataAdmin::GetElementToTagID(CString strElement)
{
/*	CString strResult = "";
ConditionGroupInfo  stConditionGroupInfo;

  strElement.TrimLeft();
  strElement.TrimRight();
  
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  stConditionGroupInfo.strElement.TrimLeft();
	  stConditionGroupInfo.strElement.TrimRight();
	  
		if (stConditionGroupInfo.strElement == strElement)
		{
		strResult = stConditionGroupInfo.stOneConditionInfo.strTag;
		break;
		}
		}
	*/
	
	// 2009.11.11 -hidden-
	CString strResult = "";
	ConditionGroupInfo  stConditionGroupInfo;
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	if(GetElementToGroup(strElement, stConditionGroupInfo))
	{
		stConditionGroupInfo.strElement.TrimLeft();
		stConditionGroupInfo.strElement.TrimRight();
		
		if (stConditionGroupInfo.strElement == strElement)
			strResult = stConditionGroupInfo.stOneConditionInfo.strTag;
	}
	
	return strResult;
}


BOOL CDataAdmin::GetElementToTagInfo(CString strElement,  CString &strTag, CString &strMapFile)
{
/*	BOOL bResult = FALSE;
ConditionGroupInfo  stConditionGroupInfo;

  strElement.TrimLeft();
  strElement.TrimRight();
  
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  stConditionGroupInfo.strElement.TrimLeft();
	  stConditionGroupInfo.strElement.TrimRight();
	  
		if (stConditionGroupInfo.strElement == strElement)
		{
		strTag = stConditionGroupInfo.stOneConditionInfo.strTag;
		strMapFile = stConditionGroupInfo.stOneConditionInfo.strMapFile;
		bResult = TRUE;
		break;
		}
		}
	*/
	
	// 2009.11.11 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	if(GetElementToGroup(strElement, stConditionGroupInfo))
	{
		stConditionGroupInfo.strElement.TrimLeft();
		stConditionGroupInfo.strElement.TrimRight();
		
		if (stConditionGroupInfo.strElement == strElement)
		{
			strTag = stConditionGroupInfo.stOneConditionInfo.strTag;
			strMapFile = stConditionGroupInfo.stOneConditionInfo.strMapFile;
			bResult = TRUE;
		}
	}
	return bResult;
}


CString CDataAdmin::GetTagIDToElement(CString strTag)
{
/*	CString strResult = "";
ConditionGroupInfo  stConditionGroupInfo;

  strTag.TrimLeft();
  strTag.TrimRight();
  
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  stConditionGroupInfo.stOneConditionInfo.strTag.TrimLeft();
	  stConditionGroupInfo.stOneConditionInfo.strTag.TrimRight();
	  if (stConditionGroupInfo.stOneConditionInfo.strTag == strTag)
	  {
	  strResult = stConditionGroupInfo.strElement;
	  break;
	  }
	  }
	*/
	
	// 2009.11.11 -hidden-
	CString strResult = "";
	ConditionGroupInfo  stConditionGroupInfo;
	
	strTag.TrimLeft();
	strTag.TrimRight();
	
	int nCondSize = 0;
	int nSize = m_arrGroups.GetSize();
	
	for (int i = 0; i < nSize; i++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			stConditionGroupInfo.stOneConditionInfo.strTag.TrimLeft();
			stConditionGroupInfo.stOneConditionInfo.strTag.TrimRight();
			
			if (stConditionGroupInfo.stOneConditionInfo.strTag == strTag)
			{
				strResult = stConditionGroupInfo.strElement;
				return strResult;
			}
		}
	}
	
	return strResult;
}


BOOL CDataAdmin::GetElementToUser_Define(CString strElement, User_Define &stUser_define)
{
/*	BOOL bResult = FALSE;
ConditionGroupInfo  stConditionGroupInfo;

  strElement.TrimLeft();
  strElement.TrimRight();
  
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  stConditionGroupInfo.strElement.TrimLeft();
	  stConditionGroupInfo.strElement.TrimRight();
	  if (stConditionGroupInfo.strElement == strElement)
	  {
	  stUser_define = stConditionGroupInfo.stOneConditionInfo.stCondition;
	  bResult = TRUE; 
	  break;
	  }
	  }
	*/
	
	// 2009.11.11 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	if(GetElementToGroup(strElement, stConditionGroupInfo))
	{		
		stConditionGroupInfo.strElement.TrimLeft();
		stConditionGroupInfo.strElement.TrimRight();
		if (stConditionGroupInfo.strElement == strElement)
		{
			stUser_define = stConditionGroupInfo.stOneConditionInfo.stCondition;
			bResult = TRUE; 
		}
	}
	
	return bResult;
}


BOOL CDataAdmin::GetElementToOneConditionInfo(CString strElement, OneConditionInfo  &stOneConditionInfo)
{
	BOOL bResult = FALSE;
	
	/*	ConditionGroupInfo  stConditionGroupInfo;
	
	  strElement.TrimLeft();
	  strElement.TrimRight();
	  
		POSITION pos;
		int nCount = m_listConditionGroup.GetCount();
		for (int i = 0; i < nCount; i++)
		{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		  stConditionGroupInfo.strElement.TrimLeft();
		  stConditionGroupInfo.strElement.TrimRight();
		  if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
		  {
		  stOneConditionInfo = stConditionGroupInfo.stOneConditionInfo;
		  bResult = TRUE; 
		  break;
		  }
		  }
	*/
	
	// 2009.11.11 -hidden-
	strElement.TrimLeft();
	strElement.TrimRight();
	
	bResult = GetElementToPosition(strElement, stOneConditionInfo);
	
	return bResult;
}


BOOL CDataAdmin::GetElementToPosition(CString strElement, OneConditionInfo& stCondition)
{
/*	POSITION posResult = NULL;
ConditionGroupInfo  stConditionGroupInfo;

  strElement.TrimLeft();
  strElement.TrimRight();
  
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int i = 0; i < nCount; i++)
	{
	pos = m_listConditionGroup.FindIndex(i);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  stConditionGroupInfo.strElement.TrimLeft();
	  stConditionGroupInfo.strElement.TrimRight();
	  if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
	  {
	  posResult = pos;
	  break;
	  }
	  }
	  
		return posResult;
	*/
	
	// 2009.11.11 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		int nItems = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < nItems; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			stConditionGroupInfo.strElement.TrimLeft();
			stConditionGroupInfo.strElement.TrimRight();
			if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
			{
				stCondition = stConditionGroupInfo.stOneConditionInfo;
				bResult = TRUE; 
			}
		}
	}
	
	return bResult;
}

// 2009.11.11 -hidden-
BOOL CDataAdmin::GetElementToGroup(CString strElement, ConditionGroupInfo& stConditionGroup)
{
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	
	strElement.TrimLeft();
	strElement.TrimRight();
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		int nItems = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < nItems; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			stConditionGroupInfo.strElement.TrimLeft();
			stConditionGroupInfo.strElement.TrimRight();
			if (stConditionGroupInfo.strElement.Compare(strElement) == 0)
			{
				stConditionGroup = stConditionGroupInfo;
				bResult = TRUE; 
			}
		}
	}
	
	return bResult;
}

BOOL CDataAdmin::GetNonAlertCondiHave(CStringArray *psaElement)
{
/*	BOOL bResult = FALSE;

  POSITION posResult = NULL;
  ConditionGroupInfo  stConditionGroupInfo;
  
	POSITION pos;
	int nCount = m_listConditionGroup.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	pos = m_listConditionGroup.FindIndex(nIndex);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  if (stConditionGroupInfo.stOneConditionInfo.nNonAlert == 1)
	  {
	  psaElement->Add(stConditionGroupInfo.strElement);
	  bResult = TRUE;
	  }
	  }
	*/
	
	// 2009.11.11 -hidden-
	BOOL bResult = FALSE;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;	
	int nSize = m_arrGroups.GetSize();
	
	for (int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			if (stConditionGroupInfo.stOneConditionInfo.nNonAlert == 1)
			{
				psaElement->Add(stConditionGroupInfo.strElement);
				bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

void CDataAdmin::SetNonAlertCondiDelete()
{
/*	POSITION posResult = NULL;
ConditionGroupInfo  stConditionGroupInfo;

  BOOL bState = FALSE;
  while (bState == FALSE)
  {
		bState = TRUE;
		POSITION pos;
		int nCount = m_listConditionGroup.GetCount();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		if (stConditionGroupInfo.stOneConditionInfo.nNonAlert == 1)
		{
		m_listConditionGroup.RemoveAt(pos);
		bState = FALSE;
		break;
		}
		}
		}
	*/
	
	// 2009.11.11 -hidden-
	ConditionGroupInfo  stConditionGroupInfo;
	
	int nCondSize = 0;
	int nSize = m_arrGroups.GetSize();
	
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		for(int j = nCondSize-1; j > -1; --j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			if (stConditionGroupInfo.stOneConditionInfo.nNonAlert == 1)
				pGroup->arrConGrpInfo.RemoveAt(j);
		}
	}
}

BOOL CDataAdmin::GetOnlyTargetCondition()
{
/*	BOOL bResult = TRUE;

  CString strTag;
  
	POSITION pos;
	ConditionGroupInfo  stConditionGroupInfo;
	int nCount = m_listConditionGroup.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
	pos = m_listConditionGroup.FindIndex(nIndex);
	stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
	
	  strTag = stConditionGroupInfo.stOneConditionInfo.strTag;
	  strTag = strTag.Left(1);
	  strTag.MakeUpper();
	  
		if (strTag != "A")
		{
		bResult = FALSE;
		break;
		}
		}
	*/
	
	// 2009.11.11 -hidden-
	BOOL bResult = TRUE;
	CString strTag;
	
	ConditionGroupInfo  stConditionGroupInfo;
	int nCondSize = 0;
	int nSize = m_arrGroups.GetSize();
	
	for (int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGoup = m_arrGroups.GetAt(i);
		nCondSize = pGoup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGoup->arrConGrpInfo.GetAt(j);
			strTag = stConditionGroupInfo.stOneConditionInfo.strTag;
			strTag = strTag.Left(1);
			strTag.MakeUpper();
			
			if (strTag != "A")
				return  FALSE;
		}
	}
	
	return bResult;
}


int CDataAdmin::GetMinTargetBase()
{
/*	int nResult = 100;

  //# 3:1분, 4:3분, 5:5분, 6:10분, 7:15분, 8:30분, 9:60분, 0:일간, 1:주간, 2:월간 
  
	POSITION posResult = NULL;
	ConditionGroupInfo  stConditionGroupInfo;
	
	  POSITION pos;
	  int nCount = m_listConditionGroup.GetCount();
	  
		int nIndex;
		for (int i = 0; i < nCount; i++)
		{
		pos = m_listConditionGroup.FindIndex(i);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		  if ((stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase >= 0) &&
		  (stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase <= 2))
		  nIndex = stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase + 10;
		  else
		  nIndex = stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase;
		  
			if (nIndex < nResult)
			nResult = nIndex;
			}
			
			  if (nResult >= 10)
			  nResult -= 10;
	*/
	
	// 2009.11.11 -hidden-
	int nResult = 100;
	
	//# 3:1분, 4:3분, 5:5분, 6:10분, 7:15분, 8:30분, 9:60분, 0:일간, 1:주간, 2:월간 
	//# 20:2분, 21:20분, 22:120분
	
	ConditionGroupInfo  stConditionGroupInfo;
	
	int nCondSize = 0;
	int nSize = m_arrGroups.GetSize();
	
	int nIndex;
	for (int i = 0; i < nSize; i++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		nCondSize = pGroup->arrConGrpInfo.GetSize();
		
		for(int j = 0; j < nCondSize; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			
			if ((stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase >= 0) &&
				(stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase <= 2))
				nIndex = stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase + 10;
			else
				nIndex = stConditionGroupInfo.stOneConditionInfo.stCondition.TargetBase;
			
			if (nIndex < nResult)
				nResult = nIndex;
		}
	}

	//--> 0015707: [7127] "당일내발생" 내 조건에 2분, 20분, 120분 주기 추가요청
	if (nResult >= 20)
		nResult -= 20;
	//<--

	if (nResult >= 10)
		nResult -= 10;
	return nResult;
}


void CDataAdmin::DebugInputDataView()
{
	// 2009.11.11 -hidden-
	/*	ConditionGroupInfo stConditionGroupInfo;
	
	  int nCount = m_listConditionGroup.GetCount();
	  
		TRACE("\n\n######## Insert Total Data View ##########");
		POSITION pos;
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
		pos = m_listConditionGroup.FindIndex(nIndex);
		stConditionGroupInfo = m_listConditionGroup.GetAt(pos);
		
		  TRACE("\n [Condition Group: %d, Group No: %d", stConditionGroupInfo.nGroup, stConditionGroupInfo.stOneConditionInfo.stCondition.GroupNo);
		  TRACE("\n [Condition Element: %s", stConditionGroupInfo.strElement);
		  TRACE("\n [Condition Result Field Cnt : %d", stConditionGroupInfo.stOneConditionInfo.stHeadMoveInfo.nColCnt);
		  }
	*/
}

// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 >>
BOOL CDataAdmin::ModifyConditionSwitch(CString strElement, CString strElementAfter)
{
	BOOL bResult  = FALSE;
	BOOL bResult2 = FALSE;
	ConditionGroupInfo stConditionGroupInfo;
	ConditionGroupInfo stConditionGroupInfoAfter;
    
	if (GetElementToGroup(strElement, stConditionGroupInfo) && GetElementToGroup(strElementAfter, stConditionGroupInfoAfter)) 
	{		
		bResult = ModifyOneCondition(strElement, stConditionGroupInfoAfter.stOneConditionInfo);
		bResult2 = ModifyOneCondition(strElementAfter, stConditionGroupInfo.stOneConditionInfo);
		
		SetChangeElement(strElement, strElementAfter);
		
	}
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(i);
		int nItems = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < nItems; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			stConditionGroupInfo.strElement.TrimLeft();
			stConditionGroupInfo.strElement.TrimRight();
		}
	}	
	
	
	return bResult&bResult2;
}

void CDataAdmin::SetChangeElement(CString strElementOrigin, CString strElementChange)
{
	int nOriginIndex = -1;
	int nChangeIndex = -1;
	int nGroupIndex = -1;
	int nGroupIndexChange = -1;
	ConditionGroupInfo stConditionGroupInfo;
	GROUP_INFO* pGroup;
	
	int nSize = m_arrGroups.GetSize();
	for(int i = 0; i < nSize; ++i)
	{
		pGroup = m_arrGroups.GetAt(i);
		int nItems = pGroup->arrConGrpInfo.GetSize();
		for(int j = 0; j < nItems; ++j)
		{
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(j);
			stConditionGroupInfo.strElement.TrimLeft();
			stConditionGroupInfo.strElement.TrimRight();
			
			if(stConditionGroupInfo.strElement == strElementOrigin)
			{
				nOriginIndex = j;
				nGroupIndex = i;
			}
			else if(stConditionGroupInfo.strElement == strElementChange)
			{
				nChangeIndex = j;
				nGroupIndexChange = i;
			}
			
		}
	}
	
	if(nOriginIndex != -1 && nChangeIndex != -1)
	{
		if(nGroupIndex == -1 && nGroupIndex != nGroupIndexChange)
			return;
		
		pGroup = m_arrGroups.GetAt(nGroupIndex);
		
		stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(nOriginIndex);
		stConditionGroupInfo.strElement = strElementChange;
		pGroup->arrConGrpInfo.SetAt(nOriginIndex, stConditionGroupInfo);
		
		stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(nChangeIndex);
		stConditionGroupInfo.strElement = strElementOrigin;
		pGroup->arrConGrpInfo.SetAt(nChangeIndex, stConditionGroupInfo);
	}
}
// 20110701 박찬모 조건의 순서를 바꾸면 해당 조건의 심볼도 수정하도록 개선 <<

void CDataAdmin::GetAllConditionName(CStringArray* psaConditionName)
{
	int nTotGroupCnt = GetGroupCnt();
	
	for(int nIndex = 0; nIndex < nTotGroupCnt; nIndex++)
	{
		GROUP_INFO* pGroup = m_arrGroups.GetAt(nIndex);
		int nConditionCnt = GetGroupToConditionCnt(nIndex);
		
		for(int nGroupIndex = 0; nGroupIndex < nConditionCnt; nGroupIndex++)
		{
			ConditionGroupInfo  stConditionGroupInfo;
			stConditionGroupInfo = pGroup->arrConGrpInfo.GetAt(nGroupIndex);
			psaConditionName->Add(stConditionGroupInfo.stOneConditionInfo.strName);
		}
	}
}