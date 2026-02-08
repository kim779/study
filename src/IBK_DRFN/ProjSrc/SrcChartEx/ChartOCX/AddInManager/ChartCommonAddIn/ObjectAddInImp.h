// ObjectAddInImp.h: interface for the CObjectAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTOBJECTADDINIMP_H__7A29C236_20FE_424C_8CB5_3AAF03F0077C__INCLUDED_)
#define AFX_CHARTOBJECTADDINIMP_H__7A29C236_20FE_424C_8CB5_3AAF03F0077C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include_Chart/DLL_Load/MainBlockTool.h"		// for CMainBlockTool
#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/17
// Comments		: Block Tool AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CObjectAddInImp : public CChartAddInBase
{
public:
	CObjectAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CObjectAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

protected:
	//	MainBlock Tool (into Chart Object)
	HMODULE				m_hBlockTool;			// (2007/6/25 - Seung-Won, Bae) Support Dynamic Loading
	CMainBlockTool *	m_pBlockTool;

// 20081007 JS.Kim	사용자시간대 선택
protected:
	CPtrList			m_lstTimeDiffInfo;
	bool				m_bReadInfo;
	int					m_nCountTimeDiff;
	BOOL				MakeConfigMenu(CMenu *pMenu);
	void				LoadTimeDiffConf();

// (2009/9/2 - Seung-Won, Bae) Check Market Index with search.
protected:
	int					GetSubMenuIndex( CMenu *p_pMainMenu, const char *p_szMenuString);
	CMenu *				GetSubMenu( CMenu *p_pMainMenu, const char *p_szMenuString);

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void);
// (2005.07.08, 배승원) AddIn Special Interface의 제공을 위하여 기본 Interface (IUnknown용)를 제공한다.
//		*. 위의 GetAddInCustomInterface()의 기능 제공을 위한 것으로, 
//			GetAddInCustomInterface() 삭제시 함께 삭제한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// (2006/1/17 - Seung-Won, Bae) Drawing Interface
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
	virtual void	OnResetMainBlock( void);
	// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
	virtual void	OnChartItemChanged( void);
	// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
	virtual void	OnGraphDataChanged( void);
	// (2006/9/25 - Seung-Won, Bae) On Block Removed.
	virtual void	OnBlockRemoved( const int p_nRow, const int p_nColumn);
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName);
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	virtual void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam);
public:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_CHARTOBJECTADDINIMP_H__7A29C236_20FE_424C_8CB5_3AAF03F0077C__INCLUDED_)
