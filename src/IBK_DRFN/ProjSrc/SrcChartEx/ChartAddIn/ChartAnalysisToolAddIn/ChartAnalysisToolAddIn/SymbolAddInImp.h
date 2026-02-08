// SymbolAddInImp.h: interface for the CSymbolAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLADDINIMP_H__94B39F73_3709_4A15_8E8D_D3377441B483__INCLUDED_)
#define AFX_SYMBOLADDINIMP_H__94B39F73_3709_4A15_8E8D_D3377441B483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/IPropertyMap.h"
#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/2
// Comments		: Symbol Tool AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CSymbolHelper;
class CSymbolAddInImp : public CChartAddInBase
{
public:
	CSymbolAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CSymbolAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;
	static CString		m_strToolAddInItemName;

// (2006/2/2 - Seung-Won, Bae) Manage Simbol Helper DLL
protected:
	CSymbolHelper *	m_pSymbolHelper;

private:
	BOOL m_bLButtonDBClk;
	CPoint m_ptOldMouse;
	CToolOptionInfo::TOOLOPTION		m_eToolOption;

private:
	CString m_strUserPath;
	BOOL LoadData(int nTool);
// (2006/3/7 - Seung-Won, Bae) Manage All Exclusive Flag between OnLButtonDown and OnLButtonUp.
protected:
	BOOL	m_bOnAllMouseExclusive;
	HWND	m_hOcxWnd;

// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
protected:
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eTimeType;
	CString									m_strItemCode;

	
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
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
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_SYMBOLADDINIMP_H__94B39F73_3709_4A15_8E8D_D3377441B483__INCLUDED_)
