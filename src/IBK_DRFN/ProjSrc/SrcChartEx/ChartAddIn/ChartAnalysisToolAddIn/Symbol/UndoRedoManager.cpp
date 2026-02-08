// UndoRedoManager.h: interface for the CUndoRedoManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UndoRedoManager.h"
#include "SymbolData.h"
#include "ElementSaverBuilder.h"
#include <assert.h>

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
	m_lSymbolAddress(NULL)
{
}

CUndo_Redo_Info::CUndo_Redo_Info(int nUndoRedoType, long lSymbolAddress, CString strSymbolData, CString strSymbolData_Prev)
{
	SetUndoRedoType(nUndoRedoType);
	m_lSymbolAddress = lSymbolAddress;
	m_strSymbolData = strSymbolData;
	m_strSymbolData_Prev = strSymbolData_Prev;
}

CUndo_Redo_Info::~CUndo_Redo_Info()
{
	SetUndoRedoType(URT_NONE);
	m_lSymbolAddress = NULL;
	m_strSymbolData.Empty();
	m_strSymbolData_Prev.Empty();
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

long CUndo_Redo_Info::GetSymbolAddress()
{
	return m_lSymbolAddress;
}

void CUndo_Redo_Info::SetSymbolAddress(long lNewSymbolAddress)
{
	m_lSymbolAddress = lNewSymbolAddress;
}

CString CUndo_Redo_Info::GetSymbolData()
{
	return m_strSymbolData;
}

CString CUndo_Redo_Info::GetSymbolData_Prev()
{
	return m_strSymbolData_Prev;
}

///////////////////////////////////////////////////////////////////////////////
// class CUndoRedoManager

CUndoRedoManager::CUndoRedoManager()
: m_pSymbolData(NULL)
{
	m_UndoList.RemoveAll();
	m_RedoList.RemoveAll();
}

CUndoRedoManager::~CUndoRedoManager()
{
	DeleteAll();
}

void CUndoRedoManager::SetSymbolData(CSymbolData* pSymbolData)
{
	m_pSymbolData = pSymbolData;
}

void CUndoRedoManager::RegisterElementToUnRedoManager(const int nUndoRedoType, const long lSymbolAddress, LPCTSTR lpszSymbolData_Prev, LPCTSTR lpszSymbolData)
{
	if(lSymbolAddress == 0)
		return;

	CString strSymbolData;
	if(lpszSymbolData)
		strSymbolData = lpszSymbolData;
	else
	{
		CElementSaverBuilder builder;
		strSymbolData = builder.GetSavedSymbolData_ForUndoRedo(GetOrder(), (CDrawingSymbol*)lSymbolAddress);
	}

	if(lpszSymbolData_Prev != NULL && strSymbolData.Compare(lpszSymbolData_Prev) == NULL)
		return;
	
	CUndo_Redo_Info* pUndoRedoInfo = new CUndo_Redo_Info(nUndoRedoType, lSymbolAddress, strSymbolData, lpszSymbolData_Prev);
	pUndoRedoInfo->SetBlockIndex(m_pSymbolData->GetBlockIndexInSymbol(pUndoRedoInfo->GetSymbolAddress()));
	if(!pUndoRedoInfo->GetBlockIndex().IsAllValueBiggerThanZero())
	{
		delete pUndoRedoInfo;
		pUndoRedoInfo = NULL;
		return;
	}
	m_UndoList.AddTail(pUndoRedoInfo);

	IChartOCX* pIChartOCX = m_pSymbolData->GetChartOCX();
	IAddInToolMgr* pAddInToolManager = pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_UNDO_REDO_MANAGER_ADD_WORK_TO_MANAGER,
															m_pSymbolData->m_strAddInItemName, (long)pUndoRedoInfo);
	pAddInToolManager->Release();
	pIChartOCX->Release();
}

void CUndoRedoManager::UnRegisterElementToUnRedoManager(const long lUndoRedoInfoAdress)
{
	if(lUndoRedoInfoAdress == 0)
		return;	

	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	POSITION pos = m_UndoList.GetHeadPosition();
	while(pos)
	{
		pUndoRedoInfo = m_UndoList.GetAt(pos);
		if(pUndoRedoInfo && pUndoRedoInfo == (CUndo_Redo_Info*)lUndoRedoInfoAdress)
		{
			m_UndoList.RemoveAt(pos);

			delete pUndoRedoInfo;
			pUndoRedoInfo = NULL;
			break;
		}
		m_UndoList.GetNext(pos);
	}
	
	IChartOCX* pIChartOCX = m_pSymbolData->GetChartOCX();
	IAddInToolMgr* pAddInToolManager = pIChartOCX->GetIAddInToolMgr();
	pAddInToolManager->OnAddInToolCommand(EAI_UNDO_REDO_MANAGER_DELETE_REDO_WORK_LIST,//EAI_UNDO_REDO_MANAGER_DELETE_WORK_THIS,
															m_pSymbolData->m_strAddInItemName, lUndoRedoInfoAdress);
	pAddInToolManager->Release();
	pIChartOCX->Release();
}

BOOL CUndoRedoManager::ChangeUndoInfo(long lUndoRedoInfo)
{
	if(lUndoRedoInfo == NULL)
		return FALSE;

	CUndo_Redo_Info* pUndoRedoInfo = (CUndo_Redo_Info*)lUndoRedoInfo;
	int nUndoRedoType = pUndoRedoInfo->GetUndoRedoType();
	switch(nUndoRedoType)
	{
	case URT_NEW_OR_DEL :
		{
			CList<CUndo_Redo_Info*, CUndo_Redo_Info*> undoredoInfoList;
			undoredoInfoList.RemoveAll();

			GetUndoRedoInfoListOfBlock(pUndoRedoInfo->GetBlockIndex(), undoredoInfoList);

			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			POSITION pos = undoredoInfoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = undoredoInfoList.GetNext(pos);
				// 존재할 경우 Del
				if(pUndoRedoInfo == pUndoRedoInfo_InList)
				{
					m_pSymbolData->GetElementTree()->DeleteCurrentDrawingSymbol((CDrawingSymbol*)pUndoRedoInfo_InList->GetSymbolAddress());
					return TRUE;
				}
			}

			// 존재하지 않을 경우 New
			CString strSymbolData = pUndoRedoInfo->GetSymbolData();
			if(strSymbolData.IsEmpty())			return FALSE;
			CElementSaverBuilder builder;
			long lNewSymbolAddress = builder.BuildSymbolsFromString_ForUndoRedo(*m_pSymbolData, pUndoRedoInfo->GetBlockIndex(), strSymbolData, m_pSymbolData->GetElementTree()->GetTree(), m_pSymbolData->GetElementTree()->GetOrder());

			// Symbol Address 동기화
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetSymbolAddress();
			pos = m_UndoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_UndoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
			}
			pos = m_RedoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_RedoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
			}

			return TRUE;
		}
		break;
	case URT_MOVE :
	case URT_MODIFY_ENV :
		{
			// 존재할 경우 Del
			m_pSymbolData->GetElementTree()->DeleteCurrentDrawingSymbol((CDrawingSymbol*)pUndoRedoInfo->GetSymbolAddress());

			// 새로 New
			CString strSymbolData_Prev = pUndoRedoInfo->GetSymbolData_Prev();
			if(strSymbolData_Prev.IsEmpty())			return FALSE;
			CElementSaverBuilder builder;
			long lNewSymbolAddress = builder.BuildSymbolsFromString_ForUndoRedo(*m_pSymbolData, pUndoRedoInfo->GetBlockIndex(), strSymbolData_Prev, m_pSymbolData->GetElementTree()->GetTree(), m_pSymbolData->GetElementTree()->GetOrder());

			// Symbol Address 동기화
			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetSymbolAddress();
			POSITION pos = m_UndoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_UndoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
			}
			pos = m_RedoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_RedoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
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
	switch(nUndoRedoType)
	{
	case URT_NEW_OR_DEL :
		{
			CList<CUndo_Redo_Info*, CUndo_Redo_Info*> undoredoInfoList;
			undoredoInfoList.RemoveAll();

			GetUndoRedoInfoListOfBlock(pUndoRedoInfo->GetBlockIndex(), undoredoInfoList);

			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			POSITION pos = undoredoInfoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = undoredoInfoList.GetNext(pos);
				// 존재할 경우 Del
				if(pUndoRedoInfo == pUndoRedoInfo_InList)
				{
					m_pSymbolData->GetElementTree()->DeleteCurrentDrawingSymbol((CDrawingSymbol*)pUndoRedoInfo_InList->GetSymbolAddress());
					return TRUE;
				}
			}

			// 존재하지 않을 경우 New
			CString strSymbolData = pUndoRedoInfo->GetSymbolData();
			if(strSymbolData.IsEmpty())			return FALSE;
			CElementSaverBuilder builder;
			long lNewSymbolAddress = builder.BuildSymbolsFromString_ForUndoRedo(*m_pSymbolData, pUndoRedoInfo->GetBlockIndex(), strSymbolData, m_pSymbolData->GetElementTree()->GetTree(), m_pSymbolData->GetElementTree()->GetOrder());

			// Symbol Address 동기화
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetSymbolAddress();
			pos = m_UndoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_UndoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
			}
			pos = m_RedoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_RedoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
			}

			return TRUE;
		}
		break;
	case URT_MOVE :
	case URT_MODIFY_ENV :
		{
			// 존재할 경우 Del
			m_pSymbolData->GetElementTree()->DeleteCurrentDrawingSymbol((CDrawingSymbol*)pUndoRedoInfo->GetSymbolAddress());

			// 새로 New
			CString strSymbolData = pUndoRedoInfo->GetSymbolData();
			if(strSymbolData.IsEmpty())			return FALSE;
			CElementSaverBuilder builder;
			long lNewSymbolAddress = builder.BuildSymbolsFromString_ForUndoRedo(*m_pSymbolData, pUndoRedoInfo->GetBlockIndex(), strSymbolData, m_pSymbolData->GetElementTree()->GetTree(), m_pSymbolData->GetElementTree()->GetOrder());

			// Symbol Address 동기화
			CUndo_Redo_Info* pUndoRedoInfo_InList = NULL;
			long lUndoRedoInfo_Origin = pUndoRedoInfo->GetSymbolAddress();
			POSITION pos = m_UndoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_UndoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
			}
			pos = m_RedoList.GetHeadPosition();
			while(pos)
			{
				pUndoRedoInfo_InList = m_RedoList.GetNext(pos);
				if(lUndoRedoInfo_Origin == pUndoRedoInfo_InList->GetSymbolAddress())
					pUndoRedoInfo_InList->SetSymbolAddress(lNewSymbolAddress);
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
	POSITION pos = m_UndoList.GetHeadPosition();
	while(pos)
	{
		pUndoRedoInfo = m_UndoList.GetNext(pos);
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
	POSITION pos = m_RedoList.GetHeadPosition();
	while(pos)
	{
		pUndoRedoInfo = m_RedoList.GetNext(pos);
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
		ChangeRedoInfo(lRedoID);
		MoveToUndoList((CUndo_Redo_Info*)lRedoID);
		return TRUE;
	}

	return FALSE;
}

void CUndoRedoManager::GetUndoRedoInfoListOfBlock(const CBlockIndex& blockIndex, CList<CUndo_Redo_Info*, CUndo_Redo_Info*>& undoredoInfoList)
{
	undoredoInfoList.RemoveAll();
	if(!blockIndex.IsAllValueBiggerThanZero())
		return;

	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	POSITION pos = m_UndoList.GetHeadPosition();
	while(pos)
	{
		pUndoRedoInfo = m_UndoList.GetNext(pos);
		assert(pUndoRedoInfo != NULL);
		if(pUndoRedoInfo->IsSameBlockIndex(blockIndex))
			undoredoInfoList.AddTail(pUndoRedoInfo);
	}
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
	return m_pSymbolData->GetElementTree()->GetOrder();
}

void CUndoRedoManager::DeleteAll()
{
	int nUndoCount = GetUndoListCount();
	int nRedoCount = GetRedoListCount();

	CUndo_Redo_Info* pUndoRedoInfo = NULL;
	POSITION pos = m_UndoList.GetHeadPosition();
	while(pos)
	{
		pUndoRedoInfo = m_UndoList.GetNext(pos);
		assert(pUndoRedoInfo != NULL);
		delete pUndoRedoInfo;
		pUndoRedoInfo = NULL;
	}
	pos = m_RedoList.GetHeadPosition();
	while(pos)
	{
		pUndoRedoInfo = m_RedoList.GetNext(pos);
		assert(pUndoRedoInfo != NULL);
		delete pUndoRedoInfo;
		pUndoRedoInfo = NULL;
	}

	m_UndoList.RemoveAll();
	m_RedoList.RemoveAll();
}