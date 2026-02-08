// AnalysisAddInImp.h: interface for the CAnalysisAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISADDINIMP_H__4BBE858E_9817_4FA2_BA75_2D8C1837836B__INCLUDED_)
#define AFX_ANALYSISADDINIMP_H__4BBE858E_9817_4FA2_BA75_2D8C1837836B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/IPropertyMap.h"
#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "../Include_Analysis/BlockBaseEnum2.h"				// for CAnalysis
#include "../AnalysisTool/AnalysisGlobalDefine.h"

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/28
// Comments
//
//	1. Tool Type with CrossLine and Numerical Inquiry DLG (NIDLG) with Z-Order
//			.......... (Look header of Drag NIDLG AddIn Tool Object Class)
//	2. AddIn Object and Co-Operations.
//			.......... (Look header of Drag NIDLG AddIn Tool Object Class)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/23
// Comments		: Analysis Tool AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CMainAnalysisTool;
class CAnalysisAddInImp : public CChartAddInBase
{
public:
	CAnalysisAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CAnalysisAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/1/24 - Seung-Won, Bae) Manage Analysis Tool Interface.
protected:
	CMainAnalysisTool* m_pAnalysisTool;
private:
	CPoint m_ptOldMouse;

// (2006/2/16 - Seung-Won, Bae) Manage Flag for LButtonUp Case
protected:
	DWORD	m_dwMouseActionID;

// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
protected:
	BOOL	m_bOnAllMouseExclusive;

// (2006/7/3 - Seung-Won, Bae) With NIDLG
protected:
	BOOL							m_bWithNIDLG;
	BOOL							m_bWithTRACINGLG;
	CToolOptionInfo::TOOLOPTION		m_eToolOption;

private:
	CString		m_strUserPath;

// (2006/3/30 - Seung-Won, Bae) for ID_CAA_CROSS_FOLLOW_CLOSE
protected:
	BOOL	m_bCrossLineFollowClose;
	HWND	m_hOcxWnd;

// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
protected:
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eTimeType;
	CString									m_strItemCode;

// (2006/6/28 - Seung-Won, Bae) Notify from CrossLine Object Tool to Over-Cross for avoiding the Multiple Cross Active.
protected:
	void	SetCurrentAnalysisTool( const CAnalysis::TOOLTYPE eToolTYPE);
	BOOL	LoadData(int nTool,ST_ANALYSIS_TOOL_INFO &stAnalysisTool);


protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	virtual void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_ANALYSISADDINIMP_H__4BBE858E_9817_4FA2_BA75_2D8C1837836B__INCLUDED_)
