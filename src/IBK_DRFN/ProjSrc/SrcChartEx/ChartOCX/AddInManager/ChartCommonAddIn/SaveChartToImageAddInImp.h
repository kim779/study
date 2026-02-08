// SaveChartToImageAddInImp.h: interface for the CSaveChartToImageAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVECHARTTOIMAGEADDINIMP_H__588BCBEF_F0FF_4681_92BA_A9EA53E64268__INCLUDED_)
#define AFX_SAVECHARTTOIMAGEADDINIMP_H__588BCBEF_F0FF_4681_92BA_A9EA53E64268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/15
// Comments		: Saving Chart Image AddIn Tool
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

class CSaveChartToImageAddInImp : public CChartAddInBase
{
public:
	CSaveChartToImageAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CSaveChartToImageAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

public:
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID);
};

#endif // !defined(AFX_SAVECHARTTOIMAGEADDINIMP_H__588BCBEF_F0FF_4681_92BA_A9EA53E64268__INCLUDED_)
