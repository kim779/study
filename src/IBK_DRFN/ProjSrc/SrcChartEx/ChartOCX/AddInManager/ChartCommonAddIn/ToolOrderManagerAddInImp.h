// ToolOrderManagerAddInImp.h: interface for the CToolOrderManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLORDERMANAGERADDINIMP_H__597FA0AE_7621_480B_BE0C_AACC5D7ED003__INCLUDED_)
#define AFX_TOOLORDERMANAGERADDINIMP_H__597FA0AE_7621_480B_BE0C_AACC5D7ED003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase


// Declare Interface
interface IChartOCX;


struct TOOL_MESSAGE_INFO
{
	CString strToolName;
	long	lToolValue;
};


class CToolOrderManagerAddInImp : public CChartAddInBase
{
public:
	CToolOrderManagerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase );
	virtual ~CToolOrderManagerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()


// Attribute
public:
	static CString m_strAddInItemName;		// Multiple Item in DLL

	
protected:
	CList < TOOL_MESSAGE_INFO *, TOOL_MESSAGE_INFO * > m_lstToolMessageInfo;
	

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual void OnResetMainBlock( void );	
	virtual void OnToolCommand( const int p_nToolType, const int p_nCmdOption );
};

#endif // !defined(AFX_TOOLORDERMANAGERADDINIMP_H__597FA0AE_7621_480B_BE0C_AACC5D7ED003__INCLUDED_)
