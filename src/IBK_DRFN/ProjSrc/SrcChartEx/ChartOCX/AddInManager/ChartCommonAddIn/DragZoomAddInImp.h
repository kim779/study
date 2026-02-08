// DragZoomAddInImp.h: interface for the CDragZoomAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGZOOMADDINIMP_H__AA06E315_5F66_40D5_B3F1_98EB9931E5AE__INCLUDED_)
#define AFX_DRAGZOOMADDINIMP_H__AA06E315_5F66_40D5_B3F1_98EB9931E5AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin/I000000/_ChartAddInBase.h"				// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/2/15
// Comments		: Drag Zoom Tool.
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CDragZoomAddInImp : public CChartAddInBase
{
public:
	CDragZoomAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CDragZoomAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// save point for inflate when mouse drag & drop
protected:
	CPoint	m_ptStartDragDrop;
	CPoint	m_ptCurrDragDrop;

// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
protected:
	DWORD	m_dwMouseActionID;

// (2006/2/6 - Seung-Won, Bae) Draw Trace Rectangle
protected:
	CRect DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptCurr, int nWeight);

// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	BOOL	m_bRun;

protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) Mouse Drag Zoom¿Ã ∫Ø∞Ê
	// virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual void OnToolCommand( const int p_nToolType, const int p_nCmdOption );
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_DRAGZOOMADDINIMP_H__AA06E315_5F66_40D5_B3F1_98EB9931E5AE__INCLUDED_)
