// UndoRedoManager.h: interface for the CUndoRedoManager class.
//
//////////////////////////////////////////////////////////////////////
//{AB4B982C-F00A-4285-B0EF-8CB4B109E440}
#if !defined(AFX_UNDOREDOMANAGER_H__AB4B982C_F00A_4285_B0EF_8CB4B109E440__INCLUDED_)
#define AFX_UNDOREDOMANAGER_H__AB4B982C_F00A_4285_B0EF_8CB4B109E440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../\Include_Chart\BlockBaseData.h"
#include "DrawingSymbol.h"
#include "ElementSaverBuilder.h"
//#include "AnalysisData.h"

class CSymbolData;
// ----------------------------------------------------------------------------
// Undo / Redo Info
enum UNDO_REDO_TYPE
{
	URT_NONE		= 0,
	URT_NEW_OR_DEL	= 1,
	URT_MOVE		= 2,
	URT_MODIFY_ENV	= 3
};

class CUndo_Redo_Info
{
// Construction / Destruction
public:
	CUndo_Redo_Info();
	CUndo_Redo_Info(int nUndoRedoType, long lSymbolAddress, CString strSymbolData, CString strSymbolData_Prev);
	~CUndo_Redo_Info();

public:
	int GetUndoRedoType() const;
	void SetUndoRedoType(const int& nUndoRedoType);

	CBlockIndex& GetBlockIndex();
	void SetBlockIndex(const CBlockIndex& blockIndex);
	BOOL IsSameBlockIndex(const CBlockIndex& blockIndex) const;

	long GetSymbolAddress();
	void SetSymbolAddress(long lNewSymbolAddress);

	CString GetSymbolData();
	CString GetSymbolData_Prev();

private:
	int m_nUndoRedoType;
	CBlockIndex m_blockIndex;
	long m_lSymbolAddress;

	CString m_strSymbolData;
	CString m_strSymbolData_Prev;
};

// ----------------------------------------------------------------------------
// UndoRedoManager
class CUndoRedoManager
{
// Construction / Destruction
public:
	CUndoRedoManager();
	~CUndoRedoManager();

public:
	void SetSymbolData(CSymbolData* pSymbolData);

	void RegisterElementToUnRedoManager(const int nUndoRedoType, const long lSymbolAddress, LPCTSTR lpszSymbolData_Prev = NULL, LPCTSTR lpszSymbolData = NULL);
	void UnRegisterElementToUnRedoManager(const long lUndoRedoInfoAdress);

	BOOL ChangeUndoInfo(long lUndoRedoInfo);
	BOOL ChangeRedoInfo(long lUndoRedoInfo);

	void MoveToUndoList(CUndo_Redo_Info* pUndoRedoInfo);
	void MoveToRedoList(CUndo_Redo_Info* pUndoRedoInfo);

	BOOL CheckUndoList(long lUndoRedoInfo);
	BOOL CheckRedoList(long lUndoRedoInfo);

	BOOL UndoElement(long lUndoID);
	BOOL RedoElement(long lRedoID);

	void GetUndoRedoInfoListOfBlock(const CBlockIndex& blockIndex, CList<CUndo_Redo_Info*, CUndo_Redo_Info*>& undoredoInfoList);
	int GetUndoListCount();
	int GetRedoListCount();
	const COrder& GetOrder() const;

	void DeleteAll();

public:
	CSymbolData* m_pSymbolData;	

private:
	CList<CUndo_Redo_Info*, CUndo_Redo_Info*> m_UndoList;
	CList<CUndo_Redo_Info*, CUndo_Redo_Info*> m_RedoList;
};

#endif // !defined(AFX_UNDOREDOMANAGER_H__AB4B982C_F00A_4285_B0EF_8CB4B109E440__INCLUDED_)

