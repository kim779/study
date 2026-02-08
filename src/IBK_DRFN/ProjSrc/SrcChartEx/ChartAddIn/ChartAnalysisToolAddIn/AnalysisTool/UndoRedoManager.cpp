// UndoRedoManager.h: interface for the CUndoRedoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UndoRedoManager.h"
#include "AnalysisData.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// class CUndo_Redo_Info

///////////////////////////////////////////////////////////////////////////////
// class CUndo_Redo_Info

CUndo_Redo_Info::CUndo_Redo_Info():
	m_nUndoRedoType(URT_NONE), 
	m_lElementAddress(NULL)
{
}

CUndo_Redo_Info::CUndo_Redo_Info(int nUndoRedoType, long lElementAddress, CString strElementData, CString strElementData_Prev)
{
	SetUndoRedoType(nUndoRedoType);
	m_lElementAddress = lElementAddress;
	m_strElementData = strElementData;
	m_strElementData_Prev = strElementData_Prev;
}

CUndo_Redo_Info::~CUndo_Redo_Info()
{
	SetUndoRedoType(URT_NONE);
	m_lElementAddress = NULL;
	m_strElementData.Empty();
	m_strElementData_Prev.Empty();
}

int CUndo_Redo_Info::GetUndoRedoType() const
{
	return m_nUndoRedoType;
}

void CUndo_Redo_Info::SetUndoRedoType(const int& nUndoRedoType)
{
	m_nUndoRedoType = nUndoRedoType;
}

CBlockIndex& CUndo_Redo_Info::GetBlockIndex()
{
	return m_blockIndex;
}

void CUndo_Redo_Info::SetBlockIndex(const CBlockIndex& blockIndex)
{
	m_blockIndex = blockIndex;
}

BOOL CUndo_Redo_Info::IsSameBlockIndex(const CBlockIndex& blockIndex) const
{
	if( m_blockIndex.GetColumn() == blockIndex.GetColumn() && 
		m_blockIndex.GetRow() == blockIndex.GetRow())
		return TRUE;
	else
		return FALSE;
}

long CUndo_Redo_Info::GetElementAddress()
{
	return m_lElementAddress;
}

void CUndo_Redo_Info::SetElementAddress(long lNewElementAddress)
{
	m_lElementAddress = lNewElementAddress;
}

CString CUndo_Redo_Info::GetElementData()
{
	return m_strElementData;
}

CString CUndo_Redo_Info::GetElementData_Prev()
{
	return m_strElementData_Prev;
}

///////////////////////////////////////////////////////////////////////////////
// class CUndoRedoManager

CUndoRedoManager::CUndoRedoManager()
: m_pAnalysisData(NULL)
{
	m_UndoList.RemoveAll();
	m_RedoList.RemoveAll();
}

CUndoRedoManager::~CUndoRedoManager()
{
	DeleteAll();
}

void CUndoRedoManager::SetAnalysisData(CAnalysisData* pAnalysisData)
{
	m_pAnalysisData = pAnalysisData;
}

void CUndoRedoManager::RegisterElementToUnRedoManager(const int nUndoRedoType, const long lElementAddress, LPCTSTR lpszElementData_Prev, LPCTSTR lpszElementData)
{
	if(lElementAddress == 0)
		return;

	CString strElementData;
	if(lpszElementData)
		strElementData = lpszElementData;
	else
		strElementData = ((CElement*)lElementAddress)->GetSavedElementData(GetOrder());

	//>> (2008/12/17 - Seung-Lyong Park) 추세선 역순지우기 및 UndoRedo
	if(strElementData.IsEmpty())			return;
	//>> (2008/12/17 - Seung-Lyong Park) 추세선 역순지우기 및 UndoRedo

	CUndo_Redo_Info* pUndoRedoInfo = new CUndo_Redo_Info(nUndoRedoType, lElementAddress, strElementData, lpszElementData_Prev);
	pUndoRedoInfo->SetBlockIndex(m_pAnalysisData->GetBlockIndexInElement(pUndoRedoInfo->GetElementAddress()));
	if(!pUndoRedoInfo->GetBlockIndex().IsAllValueBiggerThanZero())
	{
		delete pUndoRedoInfo;
		pUndoRedoInfo = NULL;
		return;
	}
	m_UndoList.AddTail(pUndoRedoInfo);

	IChartOCX* pIChartOCX = m_pAnalysisData->GetChartOCX();
	IAddInToolMgr* pAddInToolManager = pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_UNDO_REDO_MANAGER_ADD_WORK_TO_MANAGER,
															m_pAnalysisData->m_strAddInItemName, (long)pUndoRedoInfo);
	pAddInToolManager->Release();
	pIChartOCX->Release();
}

void CUndoRedoManager::UnRegisterElementToUnRedoManager(const long lUndoRedoInfoAdress)
{
	if(lUndoRedoInfoAdress == 0)
		return;	
	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	for(int nIndx = GetUndoListCount() - 1; nIndx > 0 ; --nIndx)
	{
		pUndoRedoInfo = m_UndoList.GetAt(nIndx);
		if(pUndoRedoInfo && pUndoRedoInfo == (CUndo_Redo_Info*)lUndoRedoInfoAdress)
		{
			m_UndoList.RemoveAt(pUndoRedoInfo);

			delete pUndoRedoInfo;
			pUndoRedoInfo = NULL;
			break;
		}
	}

	IChartOCX* pIChartOCX = m_pAnalysisData->GetChartOCX();
	IAddInToolMgr* pAddInToolManager = pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_UNDO_REDO_MANAGER_DELETE_REDO_WORK_LIST,//EAI_UNDO_REDO_MANAGER_DELETE_WORK_THIS,
															m_pAnalysisData->m_strAddInItemName, lUndoRedoInfoAdress);
	pAddInToolManager->Release();
	pIChartOCX->Release();
}

BOOL CUndoRedoManager::ChangeUndoInfo(long lUndoRedoInfo)
{
	if(lUndoRedoInfo == NULL)
		return FALSE;

	CUndo_Redo_Info* pUndoRedoInfo = (CUndo_Redo_Info*)lUndoRedoInfo;
	int nUndoRedoType = pUndoRedoInfo->GetUndoRedoType();
	int nIndx = 0;
	switch(nUndoRedoType)
	{
	case URT_NEW_OR_DEL :
		{
			CDequePointerList<CUndo_Redo_Info> undoredoInfoList;
			undoredoInfoList.RemoveAll();

			GetUndoRedoInfoListOfBlock(pUndoRedoInfo->GetBlockIndex(), undoredoInfoList);

			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			for( nIndx = 0; nIndx < undoredoInfoList.GetCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = undoredoInfoList.GetAt(nIndx);
				// 존재할 경우 Del
				if(pUndoRedoInfo == pUndoRedoInfo_InList)
				{
					m_pAnalysisData->GetElementTree().DeleteElementPos(pUndoRedoInfo->GetElementAddress());
					return TRUE;
				}
			}

			// 존재하지 않을 경우 New
			CString strElementData = pUndoRedoInfo->GetElementData();
			if(strElementData.IsEmpty())			return FALSE;
			CElementTreeBuilder builder( m_pAnalysisData->GetOcxHwnd(), m_pAnalysisData->GetChartMode());
			long lNewElementAddress = builder.BuildElementsFromString_ForUndoRedo(*m_pAnalysisData, pUndoRedoInfo->GetBlockIndex(), strElementData, m_pAnalysisData->GetElementTree().GetTree(), m_pAnalysisData->GetElementTree().GetOrder());

			// Element Address 동기화
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetElementAddress();
			for(nIndx = 0; nIndx < GetUndoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_UndoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}
			for(nIndx = 0; nIndx < GetRedoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_RedoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}

			return TRUE;
		}
		break;
	case URT_MOVE :
	case URT_MODIFY_ENV :
		{
			// 존재할 경우 Del
			m_pAnalysisData->GetElementTree().DeleteElementPos(pUndoRedoInfo->GetElementAddress());

			// 새로 New
			CString strElementData_Prev = pUndoRedoInfo->GetElementData_Prev();
			if(strElementData_Prev.IsEmpty())			return FALSE;
			CElementTreeBuilder builder( m_pAnalysisData->GetOcxHwnd(), m_pAnalysisData->GetChartMode());
			long lNewElementAddress = builder.BuildElementsFromString_ForUndoRedo(*m_pAnalysisData, pUndoRedoInfo->GetBlockIndex(), strElementData_Prev, m_pAnalysisData->GetElementTree().GetTree(), m_pAnalysisData->GetElementTree().GetOrder());

			// Element Address 동기화
			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetElementAddress();
			for(int nIndx = 0; nIndx < GetUndoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_UndoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}
			for(nIndx = 0; nIndx < GetRedoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_RedoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}

			return TRUE;
		}
		break;
	}

	return FALSE;
}

BOOL CUndoRedoManager::ChangeRedoInfo(long lUndoRedoInfo)
{
	if(lUndoRedoInfo == NULL)
		return FALSE;

	CUndo_Redo_Info* pUndoRedoInfo = (CUndo_Redo_Info*)lUndoRedoInfo;
	int nUndoRedoType = pUndoRedoInfo->GetUndoRedoType();
	int nIndx = 0;
	switch(nUndoRedoType)
	{
	case URT_NEW_OR_DEL :
		{
			CDequePointerList<CUndo_Redo_Info> undoredoInfoList;
			undoredoInfoList.RemoveAll();

			GetUndoRedoInfoListOfBlock(pUndoRedoInfo->GetBlockIndex(), undoredoInfoList);

			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			for( nIndx = 0; nIndx < undoredoInfoList.GetCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = undoredoInfoList.GetAt(nIndx);
				// 존재할 경우 Del
				if(pUndoRedoInfo == pUndoRedoInfo_InList)
				{
					m_pAnalysisData->GetElementTree().DeleteElementPos(pUndoRedoInfo->GetElementAddress());
					return TRUE;
				}
			}

			// 존재하지 않을 경우 New
			CString strElementData = pUndoRedoInfo->GetElementData();
			if(strElementData.IsEmpty())			return FALSE;
			CElementTreeBuilder builder( m_pAnalysisData->GetOcxHwnd(), m_pAnalysisData->GetChartMode());
			long lNewElementAddress = builder.BuildElementsFromString_ForUndoRedo(*m_pAnalysisData, pUndoRedoInfo->GetBlockIndex(), strElementData, m_pAnalysisData->GetElementTree().GetTree(), m_pAnalysisData->GetElementTree().GetOrder());

			// Element Address 동기화
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetElementAddress();
			for(nIndx = 0; nIndx < GetUndoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_UndoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}
			for(nIndx = 0; nIndx < GetRedoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_RedoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}

			return TRUE;
		}
		break;
	case URT_MOVE :
	case URT_MODIFY_ENV :
		{
			// 존재할 경우 Del
			m_pAnalysisData->GetElementTree().DeleteElementPos(pUndoRedoInfo->GetElementAddress());

			// 새로 New
			CString strElementData = pUndoRedoInfo->GetElementData();
			if(strElementData.IsEmpty())			return FALSE;
			CElementTreeBuilder builder( m_pAnalysisData->GetOcxHwnd(), m_pAnalysisData->GetChartMode());
			long lNewElementAddress = builder.BuildElementsFromString_ForUndoRedo(*m_pAnalysisData, pUndoRedoInfo->GetBlockIndex(), strElementData, m_pAnalysisData->GetElementTree().GetTree(), m_pAnalysisData->GetElementTree().GetOrder());

			// Element Address 동기화
			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetElementAddress();
			for(int nIndx = 0; nIndx < GetUndoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_UndoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}
			for(nIndx = 0; nIndx < GetRedoListCount(); ++nIndx)
			{
				pUndoRedoInfo_InList = m_RedoList.GetAt(nIndx);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetElementAddress())
					pUndoRedoInfo_InList->SetElementAddress(lNewElementAddress);
			}

			return TRUE;
		}
		break;
	}

	return FALSE;
}

void CUndoRedoManager::MoveToUndoList(CUndo_Redo_Info* pUndoRedoInfo)
{
	if(pUndoRedoInfo != m_RedoList.GetTail())
		return;

	m_RedoList.RemoveTail();
	m_UndoList.AddTail(pUndoRedoInfo);
}

void CUndoRedoManager::MoveToRedoList(CUndo_Redo_Info* pUndoRedoInfo)
{
	if(pUndoRedoInfo != m_UndoList.GetTail())
		return;

	m_UndoList.RemoveTail();
	m_RedoList.AddTail(pUndoRedoInfo);
}

BOOL CUndoRedoManager::CheckUndoList(long lUndoRedoInfo)
{
	if(lUndoRedoInfo == NULL)
		return FALSE;

	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	for(int nIndx = 0; nIndx < GetUndoListCount(); ++nIndx)
	{
		CUndo_Redo_Info* pUndoRedoInfo = m_UndoList.GetAt(nIndx);
		assert(pUndoRedoInfo != NULL);
		if(pUndoRedoInfo == (CUndo_Redo_Info*)lUndoRedoInfo)
			return TRUE;
	}

	return FALSE;
}

BOOL CUndoRedoManager::CheckRedoList(long lUndoRedoInfo)
{
	if(lUndoRedoInfo == NULL)
		return FALSE;

	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	for(int nIndx = 0; nIndx < GetRedoListCount(); ++nIndx)
	{
		CUndo_Redo_Info* pUndoRedoInfo = m_RedoList.GetAt(nIndx);
		assert(pUndoRedoInfo != NULL);
		if(pUndoRedoInfo == (CUndo_Redo_Info*)lUndoRedoInfo)
			return TRUE;
	}

	return FALSE;
}

BOOL CUndoRedoManager::UndoElement(long lUndoID)
{
	if(lUndoID <= 0 || GetUndoListCount() <= 0)
		return FALSE;

	// Undo 작업
	if(CheckUndoList(lUndoID))
	{
		ChangeUndoInfo(lUndoID);
		MoveToRedoList((CUndo_Redo_Info*)lUndoID);
		return TRUE;
	}

	return FALSE;
}

BOOL CUndoRedoManager::RedoElement(long lRedoID)
{
	if(lRedoID <= 0 || GetRedoListCount() <= 0)
		return FALSE;

	// Redo 작업
	if(CheckRedoList(lRedoID))
	{
		MoveToUndoList((CUndo_Redo_Info*)lRedoID);
		ChangeRedoInfo(lRedoID);
		return TRUE;
	}

	return FALSE;
}

void CUndoRedoManager::GetUndoRedoInfoListOfBlock(const CBlockIndex& blockIndex, CDequePointerList<CUndo_Redo_Info>& undoredoInfoList)
{
	undoredoInfoList.RemoveAll();
	if(!blockIndex.IsAllValueBiggerThanZero())
		return;

	//>> (2008/12/17 - Seung-Lyong Park) 추세선 역순지우기 및 UndoRedo
	CDequePointerList<CElement> elementList;
	m_pAnalysisData->GetElementTree().GetTree().GetElementList(blockIndex, elementList);

	for(int nIndx = 0; nIndx < GetUndoListCount(); ++nIndx)
	{
		CUndo_Redo_Info* pUndoRedoInfo = m_UndoList.GetAt(nIndx);
		assert(pUndoRedoInfo != NULL);
		if(pUndoRedoInfo->IsSameBlockIndex(blockIndex))
		{
			int nElementCount = elementList.GetCount();
			for(int nEleIndx = 0; nEleIndx < nElementCount; ++nEleIndx)
			{
				if(pUndoRedoInfo->GetElementAddress() == (long)elementList.GetAt(nEleIndx))
					undoredoInfoList.AddTail(pUndoRedoInfo);
			}
		}
	}
	//<< (2008/12/17 - Seung-Lyong Park) 추세선 역순지우기 및 UndoRedo
}

int CUndoRedoManager::GetUndoListCount()
{
	return m_UndoList.GetCount();
}

int CUndoRedoManager::GetRedoListCount()
{
	return m_RedoList.GetCount();
}

const COrder& CUndoRedoManager::GetOrder() const
{
	return m_pAnalysisData->GetElementTree().GetOrder();
}

void CUndoRedoManager::DeleteAll()
{
	int nUndoCount = GetUndoListCount();
	int nRedoCount = GetRedoListCount();
	int nIndx = 0;
	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	for( nIndx = 0; nIndx < nUndoCount; ++nIndx)
	{
		pUndoRedoInfo = m_UndoList.GetAt(nIndx);
		assert(pUndoRedoInfo != NULL);
		delete pUndoRedoInfo;
		pUndoRedoInfo = NULL;
	}
	for(nIndx = 0; nIndx < nRedoCount; ++nIndx)
	{
		pUndoRedoInfo = m_RedoList.GetAt(nIndx);
		assert(pUndoRedoInfo != NULL);
		delete pUndoRedoInfo;
		pUndoRedoInfo = NULL;
	}

	m_UndoList.RemoveAll();
	m_RedoList.RemoveAll();
}