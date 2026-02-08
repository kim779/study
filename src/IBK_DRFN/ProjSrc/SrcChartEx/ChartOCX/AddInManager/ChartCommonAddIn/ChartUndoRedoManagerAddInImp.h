// ChartUndoRedoManagerAddInImp.h: interface for the CChartUndoRedoManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTUNDOREDOMANAGERADDINIMP_H__681F5A20_64B5_40C4_88B5_B7FAD73AC624__INCLUDED_)
#define AFX_CHARTUNDOREDOMANAGERADDINIMP_H__681F5A20_64B5_40C4_88B5_B7FAD73AC624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase


// Declare Interface
interface IChartOCX;


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 22
// Reason		: 
//-----------------------------------------------------------------------------
struct UNDO_REDO_MESSAGE_INFO
{
	CString strMessageName;			// AddIn에서 보내온 Message
	long	lMessagelValue;			// AddIn에서 보내온 Value
};


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 20
// Comments		: 
// Using way	: 
// See			: Chart의 Undo, Redo를 관리한다.
//-----------------------------------------------------------------------------
class CChartUndoRedoManagerAddInImp : public CChartAddInBase  
{
// Constructor & Destructor
public:
	CChartUndoRedoManagerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase );
	virtual ~CChartUndoRedoManagerAddInImp();
	
	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()


// Attributes
public:
	static CString m_strAddInItemName;		// Multiple Item in DLL


protected:
	// Undo를 실행시킬 수 있게 실행되고 있는 작업들을 등록한다.
	CList < UNDO_REDO_MESSAGE_INFO *, UNDO_REDO_MESSAGE_INFO * > m_lstUndoMessageInfo;

	// Undo가 실행되면 Redo를 실행시킬 수 있게 UndoList에 등록된 작업을 가지고 온다.
	CList < UNDO_REDO_MESSAGE_INFO *, UNDO_REDO_MESSAGE_INFO * > m_lstRedoMessageInfo;


// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);

	virtual void OnToolCommand( const int p_nToolType, const int p_nCmdOption );

	virtual void OnResetMainBlock( void );	
};

#endif // !defined(AFX_CHARTUNDOREDOMANAGERADDINIMP_H__681F5A20_64B5_40C4_88B5_B7FAD73AC624__INCLUDED_)
