// IndicatorInfoUndoRedoManagerAddInImp.cpp: implementation of the CIndicatorInfoUndoRedoManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "IndicatorInfoUndoRedoManagerAddInImp.h"


///////////////////////////////////////////////////////////////////////////////
//
// class CIndiInfoData
//
//////////////////////////////////////////////////////////////////////////

CIndiInfoData::CIndiInfoData(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning):
	m_bTheBeginning(bTheBeginning)
{
	m_pIGraphInfo = p_pIGraphInfo->CreateCopyObject();
}


CIndiInfoData::~CIndiInfoData()
{
	if( m_pIGraphInfo)
	{
		m_pIGraphInfo->Release();
		m_pIGraphInfo = NULL;
	}
}


CIndiInfoData* CIndiInfoData::Make(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning)
{
	return new CIndiInfoData(p_pIGraphInfo, bTheBeginning);
}


void CIndiInfoData::Delete(CIndiInfoData* pIndiInfoData)
{
	delete pIndiInfoData;
}


void CIndiInfoData::SetTheBeginning(const bool bTheBeginning)
{
	m_bTheBeginning = bTheBeginning;
}

void CIndiInfoData::SetIndicatorInfo( IGraphInfo* p_pIGraphInfo)
{
	if(p_pIGraphInfo == NULL)
		return;

	if( m_pIGraphInfo) m_pIGraphInfo->Release();
	m_pIGraphInfo = p_pIGraphInfo->CreateCopyObject();
}

bool CIndiInfoData::IsTheBeginning() const
{
	return m_bTheBeginning;
}

IGraphInfo* CIndiInfoData::GetIGraphInfo()
{
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	return m_pIGraphInfo;
}


CString CIndiInfoData::GetIndicatorName() const
{
	if( !m_pIGraphInfo) return "";
	ILPCSTR szGraphName = m_pIGraphInfo->GetGraphNameString();
	return szGraphName;
}





///////////////////////////////////////////////////////////////////////////////
//
// class CIndiInfoDataList
//
//////////////////////////////////////////////////////////////////////////

CIndiInfoDataList::CIndiInfoDataList()
{
}


CIndiInfoDataList::~CIndiInfoDataList()
{
	DeleteAll();
}


bool CIndiInfoDataList::AddTail(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning)
{
	if(p_pIGraphInfo == NULL)
		return false;

	ILPCSTR szGraphName = p_pIGraphInfo->GetGraphNameString();
	if( IsIndiInfoData(szGraphName)) return false;

	m_indiInfoDataList.AddTail( CIndiInfoData::Make(p_pIGraphInfo, bTheBeginning));
	return true;
}


void CIndiInfoDataList::DeleteAll()
{
	POSITION pos = m_indiInfoDataList.GetHeadPosition();

	while(pos != NULL)
	{
		CIndiInfoData::Delete(m_indiInfoDataList.GetNext(pos));
	}
}


bool CIndiInfoDataList::ChangeAllIndicatorList(IGraphInfoManager* p_pIGraphInfoManager)
{
	if(p_pIGraphInfoManager == NULL)
		return false;

	bool bResult = false;
	IGraphInfo* pSourceIndicatorInfo = NULL;
	IGraphInfo* pTargetIndicatorInfo = NULL;
	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pSourceIndiInfo = m_indiInfoDataList.GetNext(pos);
		if(pSourceIndiInfo == NULL)
			return false;

		pSourceIndicatorInfo = pSourceIndiInfo->GetIGraphInfo();
		if( !pSourceIndicatorInfo) continue;

		ILPCSTR szGraphName = pSourceIndicatorInfo->GetGraphNameString();
		pTargetIndicatorInfo = p_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
		if( !pTargetIndicatorInfo) 
		{
			pSourceIndicatorInfo->Release();
			continue;
		}

		if( pTargetIndicatorInfo->GetCopyDataFrom( pSourceIndicatorInfo)) bResult = true;
		pTargetIndicatorInfo->Release();
		pSourceIndicatorInfo->Release();
	}

	return bResult;
}


POSITION CIndiInfoDataList::GetHeadPosition() const
{
	return m_indiInfoDataList.GetHeadPosition();
}


POSITION CIndiInfoDataList::GetHeadPosition()
{
	return m_indiInfoDataList.GetHeadPosition();
}


POSITION CIndiInfoDataList::GetTailPosition() const
{
	return m_indiInfoDataList.GetTailPosition();
}


POSITION CIndiInfoDataList::GetTailPosition()
{
	return m_indiInfoDataList.GetTailPosition();
}


CIndiInfoData* CIndiInfoDataList::GetPrev(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetPrev(pos);
}


CIndiInfoData* CIndiInfoDataList::GetPrev(POSITION& pos)
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetPrev(pos);
}


CIndiInfoData* CIndiInfoDataList::GetNext(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetNext(pos);
}


CIndiInfoData* CIndiInfoDataList::GetNext(POSITION& pos)
{
	if(pos == NULL)
		return NULL;

	return m_indiInfoDataList.GetNext(pos);
}


CIndiInfoData* CIndiInfoDataList::GetIndiInfoData(const CString& strIndicatorName)
{
	if(strIndicatorName.IsEmpty())
		return NULL;

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		if(pIndiInfoData == NULL)
			return NULL;
		
		if(strIndicatorName == pIndiInfoData->GetIndicatorName())
			return pIndiInfoData;
	}

	return NULL;
}


bool CIndiInfoDataList::GetIndiInfoInTheBeginning(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const
{
	indiInfoList.RemoveAll();

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		if(pIndiInfoData == NULL)
			return false;

		if(pIndiInfoData->IsTheBeginning())
			indiInfoList.AddTail(pIndiInfoData->GetIGraphInfo());
	}

	return (indiInfoList.GetCount() > 0);
}


bool CIndiInfoDataList::GetIndiInfoIntheSave(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const
{
	indiInfoList.RemoveAll();

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		if( pIndiInfoData != NULL )
			indiInfoList.AddTail(pIndiInfoData->GetIGraphInfo());
	}

	return (indiInfoList.GetCount() > 0);
}


bool CIndiInfoDataList::IsIndiInfoData( const char *p_szGraphName) const
{
	if( !p_szGraphName) return false;
	if( !*p_szGraphName) return false;

	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	while(pos != NULL)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
		if(pIndiInfoData == NULL)
			continue;

		if(pIndiInfoData->GetIndicatorName() == p_szGraphName)
			return true;
	}

	return false;
}





///////////////////////////////////////////////////////////////////////////////
//
// class CGraphEtcData
//
//////////////////////////////////////////////////////////////////////////
CGraphEtcData::CGraphEtcData():
	m_bIsVertScaleLogType(false), 
	m_bIsVertScaleInvertType(false)
{
}


CGraphEtcData::CGraphEtcData(const CString& strFunctionName, const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType):
	m_strFunctionName(strFunctionName), 
	m_bIsVertScaleLogType(bIsVertScaleLogType), 
	m_bIsVertScaleInvertType(bIsVertScaleInvertType)
{
}


void CGraphEtcData::SetFunctionName(const CString& strFunctionName)
{
	m_strFunctionName = strFunctionName;
}


void CGraphEtcData::SetVertScaleLogType(const bool bIsVertScaleLogType)
{
	m_bIsVertScaleLogType = bIsVertScaleLogType;
}


void CGraphEtcData::SetVertScaleInvertType(const bool bIsVertScaleInvertType)
{
	m_bIsVertScaleInvertType = bIsVertScaleInvertType;
}


CString CGraphEtcData::GetFunctionName() const
{
	return m_strFunctionName;
}


bool CGraphEtcData::IsVertScaleLogType() const
{
	return m_bIsVertScaleLogType;
}


bool CGraphEtcData::IsVertScaleInvertType() const
{
	return m_bIsVertScaleInvertType;
}





//////////////////////////////////////////////////////////////////////
//
//		class CIndicatorInfoBackUp
//
//////////////////////////////////////////////////////////////////////
CIndicatorInfoBackUp::CIndicatorInfoBackUp()	// : m_bAllHiding_PriceMA(false)
{

}


CIndicatorInfoBackUp::~CIndicatorInfoBackUp()
{
}


bool CIndicatorInfoBackUp::AddTail( IGraphInfo *p_pIGraphInfo, const bool bTheBeginning )
{
	// 같은 지표가 없을 경우만 추가한다.
	return m_indiInfoDataList.AddTail(p_pIGraphInfo, bTheBeginning);
}


void CIndicatorInfoBackUp::Initialize()
{
	m_indiInfoDataList.DeleteAll();
}


bool CIndicatorInfoBackUp::ChangeAllIndicatorList( IGraphInfoManager *p_pIGraphInfoManager )
{
	return m_indiInfoDataList.ChangeAllIndicatorList(p_pIGraphInfoManager);
}


bool CIndicatorInfoBackUp::GetIndiInfoInTheBeginning( CList< IGraphInfo*, IGraphInfo*>& indiInfoList ) const
{
	return m_indiInfoDataList.GetIndiInfoInTheBeginning(indiInfoList);
}


bool CIndicatorInfoBackUp::GetIndiInfoIntheSave( CList< IGraphInfo*, IGraphInfo*>& indiInfoList ) const
{
	return m_indiInfoDataList.GetIndiInfoIntheSave(indiInfoList);
}


bool CIndicatorInfoBackUp::IsIndicatorInfo( IGraphInfo *p_pIGraphInfo )
{
	POSITION pos = m_indiInfoDataList.GetHeadPosition();
	
	while(pos)
	{
		CIndiInfoData* pIndiInfoData = m_indiInfoDataList.GetNext(pos);
	
		ILPCSTR szGraphName = p_pIGraphInfo->GetGraphNameString();
		if( pIndiInfoData->GetIndicatorName() == szGraphName) return true;
	}

	return false;
}


CGraphEtcData& CIndicatorInfoBackUp::GetGraphEtcData()
{
	return m_graphEtcData;
}


CIndiInfoDataList& CIndicatorInfoBackUp::GetIndiInfoDataList()
{
	return m_indiInfoDataList;
}


// ----------------------------------------------------------------------------
/*void CIndicatorInfoBackUp::SetAllHiding_PriceMA(const bool bAllHiding_PriceMA)
{
	m_bAllHiding_PriceMA = bAllHiding_PriceMA;
}

// ----------------------------------------------------------------------------
bool CIndicatorInfoBackUp::IsAllHiding_PriceMA() const
{
	return m_bAllHiding_PriceMA;
}*/



//////////////////////////////////////////////////////////////////////////
//
// class CIndicatorInfoUndoRedoManagerAddInImp
//
//////////////////////////////////////////////////////////////////////////


CIndicatorInfoUndoRedoManagerAddInImp::CIndicatorInfoUndoRedoManagerAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase) 
{
	m_bIsOnlyViewData = false;
}


CIndicatorInfoUndoRedoManagerAddInImp::~CIndicatorInfoUndoRedoManagerAddInImp()
{
	if( m_lstIndicatorInfoBackUpUndo.GetCount() > 0 )
	{
		POSITION pos = m_lstIndicatorInfoBackUpUndo.GetHeadPosition();
		CIndicatorInfoBackUp *pIndicatorBackUp;

		while(pos)
		{
			pIndicatorBackUp = NULL;
			pIndicatorBackUp = m_lstIndicatorInfoBackUpUndo.GetAt(pos);
			if( pIndicatorBackUp == NULL ) break;
				
			delete pIndicatorBackUp;
			
			m_lstIndicatorInfoBackUpUndo.GetNext(pos);
		}
	}

	if( m_lstIndicatorInfoBackUpRedo.GetCount() > 0 )
	{
		POSITION pos = m_lstIndicatorInfoBackUpRedo.GetHeadPosition();
		CIndicatorInfoBackUp *pIndicatorBackUp;

		while(pos)
		{
			pIndicatorBackUp = NULL;
			pIndicatorBackUp = m_lstIndicatorInfoBackUpRedo.GetAt(pos);
			if( pIndicatorBackUp == NULL ) break;
				
			delete pIndicatorBackUp;
			
			m_lstIndicatorInfoBackUpRedo.GetNext(pos);
		}
	}
}


// Multiple Item in DLL
CString CIndicatorInfoUndoRedoManagerAddInImp::m_strAddInItemName = "INDICATORINFO_UNDO_REDO_MANAGER";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CIndicatorInfoUndoRedoManagerAddInImp )
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char )
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnResetMainBlock )
END_ADDIN_EVENT_MAP()


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 13
// Return BOOL	: 
// Param  const EAICommandType p_eCommandID : 
// Param  const long p_lParam : 
// Param  const char *p_szParam1 : 
// Param  const char *p_szParam2 : 
// Param  const char *p_szParam3 : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CIndicatorInfoUndoRedoManagerAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	if( m_pIAddInToolMgr )
	{
		if( EAI_INDICATORINFO_UNDO_REDO_MANAGER_BACKUP_INDICATORINFO == p_eCommandID )
		{
			if( !m_pIChartManager )
				return FALSE;

			CIndicatorInfoBackUp *pIndicatorInfoBackUp = NULL;
			pIndicatorInfoBackUp = BackUpIndicatorInfo( (IStringList *)p_szParam1, (int)p_lParam );

			if( pIndicatorInfoBackUp )
			{
				m_pIAddInToolMgr->OnAddInToolCommand( EAI_UNDO_REDO_MANAGER_ADD_WORK_TO_MANAGER,	\
													  m_strAddInItemName,							\
													  (long)pIndicatorInfoBackUp );
			}
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 03
// Return BOOL	: 
// Param  	const EAICommandType p_eCommandID : 
// Param  const char *p_szParam : 
// Param  const long p_lParam : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CIndicatorInfoUndoRedoManagerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( strcmp( p_szParam, m_strAddInItemName ) )
		return FALSE;

	if( m_pIAddInToolMgr )
	{
		switch( p_eCommandID )
		{
			case EAI_UNDO_REDO_MANAGER_DELETE_REDO_WORK_LIST :
			{
				// 1. Redo List에 등록되어 있는 Node들을 삭제한다.
				if( m_lstIndicatorInfoBackUpRedo.GetCount() > 0 )
				{
					POSITION pos = m_lstIndicatorInfoBackUpRedo.GetHeadPosition();
					CIndicatorInfoBackUp *pIndicatorBackUp;

					while(pos)
					{
						pIndicatorBackUp = NULL;
						pIndicatorBackUp = m_lstIndicatorInfoBackUpRedo.GetAt(pos);
						if( pIndicatorBackUp == NULL ) break;
							
						delete pIndicatorBackUp;
						
						m_lstIndicatorInfoBackUpRedo.GetNext(pos);
					}
				}

				break;
			}
			

			case EAI_UNDO_REDO_MANAGER_UNDO_THIS_WORK :
			{
				if( m_lstIndicatorInfoBackUpUndo.GetCount() > 0 )
				{
					CIndicatorInfoBackUp *pIndicatorBackUp = NULL;

					pIndicatorBackUp = m_lstIndicatorInfoBackUpUndo.GetTail();

					if( pIndicatorBackUp == NULL )
						return TRUE;

					pIndicatorBackUp->ChangeAllIndicatorList( m_pIGraphInfoManager );

					m_lstIndicatorInfoBackUpRedo.AddTail( pIndicatorBackUp );

					m_lstIndicatorInfoBackUpUndo.RemoveTail();
				}
			
				break;
			}


			case EAI_UNDO_REDO_MANAGER_REDO_THIS_WORK :
			{
				if( m_lstIndicatorInfoBackUpRedo.GetCount() > 0 )
				{
					CIndicatorInfoBackUp *pIndicatorBackUp = NULL;

					pIndicatorBackUp = m_lstIndicatorInfoBackUpRedo.GetTail();

					if( pIndicatorBackUp == NULL )
						return TRUE;

					pIndicatorBackUp->ChangeAllIndicatorList( m_pIGraphInfoManager );

					m_lstIndicatorInfoBackUpUndo.AddTail( pIndicatorBackUp );

					m_lstIndicatorInfoBackUpRedo.RemoveTail();
				}

				break;
			}
		}
	}
	
	return FALSE;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 01
// Return CIndicatorInfoBackUp*	: 
// Param  IStringList *szIndicatorNameList : 
// Param  int nIndicatorCount : 
// Comments		: 
//-----------------------------------------------------------------------------
CIndicatorInfoBackUp* CIndicatorInfoUndoRedoManagerAddInImp::BackUpIndicatorInfo( IStringList *szIndicatorNameList, int nIndicatorCount)
{
	// IndicatorInfo의 내용을 GraphInfo를 포함해서 생성시킨다.
	m_pIChartManager->ChangeAllGraphInfoFromGraphData();

	// IndicatorInfo의 BackUp을 만든다.
	CIndicatorInfoBackUp *pIndicatorInfoBackUp = NULL;
	pIndicatorInfoBackUp = new CIndicatorInfoBackUp;

	IStringList *pINameList = m_pIChartOCX->CreateObjectIStringList();

	POSITION pos = szIndicatorNameList->GetHeadPosition();
	while(pos)
	{
		ILPCSTR szIndicatorName = NULL;
		szIndicatorNameList->GetNext( pos, szIndicatorName);
		
		IGraphInfo *pOrgIndicatorInfo = m_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szIndicatorName);	
		if(pOrgIndicatorInfo == NULL)
			continue;
 
		pIndicatorInfoBackUp->AddTail( pOrgIndicatorInfo, true );
		pOrgIndicatorInfo->Release();
	}

	m_lstIndicatorInfoBackUpUndo.AddTail( pIndicatorInfoBackUp );
	
	pINameList->Release();

	return pIndicatorInfoBackUp;
}


void CIndicatorInfoUndoRedoManagerAddInImp::OnResetMainBlock()
{
	if( m_lstIndicatorInfoBackUpUndo.GetCount() > 0 )
	{
		POSITION pos = m_lstIndicatorInfoBackUpUndo.GetHeadPosition();
		CIndicatorInfoBackUp *pIndicatorBackUp;

		while(pos)
		{
			pIndicatorBackUp = NULL;
			pIndicatorBackUp = m_lstIndicatorInfoBackUpUndo.GetAt(pos);
			if( pIndicatorBackUp == NULL ) break;
				
			delete pIndicatorBackUp;
			
			m_lstIndicatorInfoBackUpUndo.GetNext(pos);
		}
	}

	if( m_lstIndicatorInfoBackUpRedo.GetCount() > 0 )
	{
		POSITION pos = m_lstIndicatorInfoBackUpRedo.GetHeadPosition();
		CIndicatorInfoBackUp *pIndicatorBackUp;

		while(pos)
		{
			pIndicatorBackUp = NULL;
			pIndicatorBackUp = m_lstIndicatorInfoBackUpRedo.GetAt(pos);
			if( pIndicatorBackUp == NULL ) break;
				
			delete pIndicatorBackUp;
			
			m_lstIndicatorInfoBackUpRedo.GetNext(pos);
		}
	}
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 07
// Return bool	: 
// Param  const bool bIsOnlyViewData : 
// Param  IStringList * pIndicatorNameList : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CIndicatorInfoUndoRedoManagerAddInImp::GetGraphNameList(const bool bIsOnlyViewData, IStringList * pIndicatorNameList)
{
	if(m_pIGraphInfoManager == NULL)
		return false;

	pIndicatorNameList->RemoveAll();
	
	// 화면에 존해하는 지표명
	bool bResult = GetGraphNameList(m_pIndicatorNameList, pIndicatorNameList);
	if(bIsOnlyViewData)
		return bResult;

	// userList 에 존재하는 지표명
	IStringList *pIUserIndiNameList = m_pIGraphInfoManager->GetIndicatorNameStringList( IGraphInfoManager::IEILT_USER, IGraphInfoManager::IEIIT_GENERAL);
	if( pIUserIndiNameList)
	{
		if( GetGraphNameList(pIUserIndiNameList, pIndicatorNameList))
			bResult = true;
	}

	pIUserIndiNameList->RemoveAll();
	pIUserIndiNameList->Release();
	pIUserIndiNameList = m_pIGraphInfoManager->GetIndicatorNameStringList( IGraphInfoManager::IEILT_USER, IGraphInfoManager::IEIIT_SPECIAL);
	if( pIUserIndiNameList )
	{
		if(GetGraphNameList(pIUserIndiNameList, pIndicatorNameList))
			bResult = true;
	}
	pIUserIndiNameList->Release();

	return bResult;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 07
// Return bool	: 
// Param  IStringList * pISourceIndiNameList : 
// Param  IStringList * targetIndiNameList : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CIndicatorInfoUndoRedoManagerAddInImp::GetGraphNameList( IStringList * pISourceIndiNameList, IStringList * targetIndiNameList)
{
	if( !pISourceIndiNameList) return false;
	if( pISourceIndiNameList->GetCount() <= 0) return false;

	ILPCSTR szGraphName = NULL;
	POSITION pos = pISourceIndiNameList->GetHeadPosition();
	while( pos != NULL )
	{
		pISourceIndiNameList->GetNext( pos, szGraphName);
		if(szGraphName.IsEmpty())
			continue;
		
		if( !IsExistenceIndicatorName( targetIndiNameList, szGraphName ) )
			targetIndiNameList->AddTail(szGraphName);
	}
	
	return (targetIndiNameList->GetCount() > 0);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 07
// Return bool	: 
// Param  IStringList * pIndicatorNameList : 
// Param  const char *p_szGraphName  const : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CIndicatorInfoUndoRedoManagerAddInImp::IsExistenceIndicatorName(IStringList * pIndicatorNameList, const char *p_szGraphName)
{
	if( !p_szGraphName)  return false;
	if( !*p_szGraphName) return false;

	ILPCSTR szGraphName = NULL;

	POSITION pos = pIndicatorNameList->GetHeadPosition();
	while( pos != NULL )
	{
		pIndicatorNameList->GetNext(pos, szGraphName);
		if( !strcmp( szGraphName, p_szGraphName) )
			return true;
	}

	return false;
}

