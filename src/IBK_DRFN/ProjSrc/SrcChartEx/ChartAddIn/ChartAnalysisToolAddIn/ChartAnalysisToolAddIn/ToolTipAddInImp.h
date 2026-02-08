// ToolTipAddInImp.h: interface for the CToolTipAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIPADDINIMP_H__F2CFB103_FE5A_40AD_972C_7B547DC386BE__INCLUDED_)
#define AFX_TOOLTIPADDINIMP_H__F2CFB103_FE5A_40AD_972C_7B547DC386BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tfxdatatip.h"										// for TFXDataTip
#include "NumericalInquiryDataQueryer.h"					// for CNumericalInquiryDataQueryer
#define _CLICK_DIFFERENTIAL		2

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/20
// Comments		: Chart ToolTip AddIn
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// (2006/7/3 - Seung-Won, Bae) Caution! Do not use AFX_MANAGE_STATE() with ToolTip Window
//		Because, It is static Object. So it is managed by Main Module (LoadLibrary Caller, OCX)
//////////////////////////////////////////////////////////////////////

class CToolTipAddInImp : public CChartAddInBase
{
public:
	CToolTipAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CToolTipAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2006/2/15 - Seung-Won, Bae) Backup for checking Mouse Move.
protected:
	CPoint	m_ptOldMousePos;
	BOOL	m_bTracingToolTip;
	BOOL	m_bMouseToolTip;
	BOOL	m_bMouseToolTipAll;		//[A00000441]110407 풍선도움말 보기시 전체 조회

// (2006/6/24 - Seung-Won, Bae) Manage the static Tooltip Object
protected:
	static TFXDataTip m_toolTip;							// 툴팁

// (2006/6/24 - Seung-Won, Bae) Manage the flag of Tooltip Using
protected:
	BOOL	m_bToolTipCreate;

// (2006/6/24 - Seung-Won, Bae) Manage the Numerical Inquiry Data Queryer
protected:
	CNumericalInquiryDataQueryer	m_niDataQueryer;

// (2007/2/9 - Seung-Won, Bae) Delay in TracingToolTip.
protected:
	DWORD	m_dwDrawStartTime;
//	HWND	m_hOcxWnd;

	BOOL DrawTooltipText(CString szRQ, CPoint point);
	// XScale Text 툴팁표시 - ojtaso (20080429)
	BOOL DrawTooltipHorzScaleText(CString szRQ, CPoint point, int nTextSize, int nHeight);


// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces for Inquiry Tooltip
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
};

#endif // !defined(AFX_TOOLTIPADDINIMP_H__F2CFB103_FE5A_40AD_972C_7B547DC386BE__INCLUDED_)
